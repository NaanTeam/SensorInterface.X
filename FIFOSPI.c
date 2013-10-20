/**
 * @Author: Connor Martin
 * @Description: A FIFO interrupt driven spi communication method.
 *      SDIx: Serial Data Input
 *      SDOx: Serial Data Output
 *      SCKx: Shift Clock Input or Output
 *      SSx: Active-Low Slave Select or Frame Synchronization I/O Pulse
 * @Pins:
 *  Chipkit     PIC32MX3XX/4XX  Registers    Descritpion
 *  10          52              RD4          SS2 write
 *  11          6               RG8          SDO2 write
 *  12          5               RG7          SDI2 read
 *  13          4               RG6          SCK2 write
 */

#include "FIFOSPI.h"


#define FIFOSPI2_BUFFERSIZE 100

//Flags
int isRunnning = 0; //Represents if the SPI2 is currently running
int isTransmitsTurn = 1; //used for toggling between transmit and receive 

//Send buffers and indexes
int SendBuffer_Index = 0; //Index of how full the send buffer is.
int SendBuffer_SentIndex = 0; //Index of what has been sent
char SendBuffer[FIFOSPI2_BUFFERSIZE]; //Contains the chars to send
char SendBufferFlags[FIFOSPI2_BUFFERSIZE]; //Flags for driving SS high.

int ReceiveBuffer_Index = 0; //Index of how full the receive buffer is
int ReceiveBuffer_ReadIndex = 0; //Index of what has been read out of the buffer
char ReceiveBuffer[FIFOSPI2_BUFFERSIZE]; //Holds all the receive chars from SPI2


void FIFOSPI2_Setup()
{
    //populate the buffers with zeros.
    int i = 0;
    while (i < FIFOSPI2_BUFFERSIZE) 
    {
        SendBuffer[i] = 0;
        SendBufferFlags[i] = 0;
        ReceiveBuffer[i] = 0;
        i++;
    }

    //Sets up the SPI2 pins for the PIC32M320H
    TRISGbits.TRISG8 = 0; //SDO2 write
    TRISGbits.TRISG7 = 1; //SDI2 read
    TRISGbits.TRISG6 = 0; //SCK2 write
    TRISDbits.TRISD4 = 0; //SS2 write

    // disable all interrupts
    INTEnable(INT_SPI2E, INT_DISABLED);
    INTEnable(INT_SPI2TX, INT_DISABLED);
    INTEnable(INT_SPI2RX, INT_DISABLED);
    
    //Stops and clears SPI2
    SPI2CON = 0;        // Stops and resets the SPI2.
    int rData;
    rData=SPI2BUF;      // clears the receive buffer    
    SPI2STATbits.SPIROV = 0;   // clear the Overflow
    
    //Set the prescale (Note: ADXL362 recommends 1 MHz to 5 MHz)
    SPI2BRG = 1;        // use FPB/4 clock frequency = 2.5Mhz
    //      F_ SCK = F_PB / (2 * (SPIxBRG + 1))
    //          F_SCK: SPI clock freq
    //          F_PB: peripheral clock freq
    //          SPIxBRG: SPI device baud rate prescalar
   
    
    //Sets the interupt vector priorty
    INTSetVectorPriority(INT_SPI_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_SPI_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    //Clear any existing interupt events
    INTClearFlag(INT_SPI2TX); 
    INTClearFlag(INT_SPI2RX);

    //Enable SPI2 Interupts
    INTEnable(INT_SPI2TX, INT_ENABLED); //SPI2 transmit buffer empty.
    INTEnable(INT_SPI2RX, INT_ENABLED);//SPI2 Receive Done.
    
    //Configure SPI2 and turn it on
    SPI2CONbits.CKP = 1;    //Clock polarity: Idle state for clock is a
                            //high level; Active state is a low level
    SPI2CONbits.SMP = 1;    // Input data sampled at end of data output time
    SPI2CONbits.MSTEN = 1;  //Master Mode Enable bit
    SPI2CONbits.ON = 1;     //SPI ON
}

int FIFOSPI2_SendQueue(unsigned char data[], int length)
{
    int i = 0;
    //If the send buffer isn't full then add another char
    if ((SendBuffer_Index + length) < FIFOSPI2_BUFFERSIZE)
    {
        //Adds a token to signify end signal group.
        SendBufferFlags[SendBuffer_Index + length - 1] = 1; 

        //Add each byte to the buffer
        while (i < length)
        {
            SendBuffer[SendBuffer_Index++] = data[i];
            i++;
        }

        //If SPI2 IRQ is not running then start it
        if (isRunnning == 0) 
        {
            isRunnning = 1;
            INTSetFlag(INT_SPI2TX);
        }

        return 1;
    }
    //Overflow error
    else
    {
        return -1;
    }
}

int FIFOSPI2_ReadQueue(unsigned char *bytesBuffer)
{
    //The receive buffer is empty.
    if (ReceiveBuffer_Index == 0)
    {
        *bytesBuffer = 0;
        return 0;
    }
    //The receiver buffer has entries.
    else if (ReceiveBuffer_Index < FIFOSPI2_BUFFERSIZE)
    {
        char rtn = ReceiveBuffer[ReceiveBuffer_ReadIndex++];
        //If all bytes from the buffer have been read.
        if (ReceiveBuffer_ReadIndex >= ReceiveBuffer_Index)
        {
            //Reset the indexs
            ReceiveBuffer_ReadIndex = 0;
            ReceiveBuffer_Index = 0;
        }

        *bytesBuffer = rtn;
        return 1;
    }
    //The receive buffer is overflowing.
    else if (ReceiveBuffer_Index >= FIFOSPI2_BUFFERSIZE)
    {
        *bytesBuffer = 0;
        return -1;
    }

    return -3;
}

int FIFOSPI2_RecieveBufferIndex()
{
    return ReceiveBuffer_Index;
}

//This is the SPI2 Interupt Handler
void __ISR(_SPI_2_VECTOR, IPL4AUTO)__SPI2Interrupt(void)
{
    //Receive interupt
    if (INTGetFlag(INT_SPI2RX)) 
    {
        //Clear Interrupt flag
        INTClearFlag(INT_SPI2RX);

        //If it's RX's turn
        if (isTransmitsTurn == 0) 
        {
            //Set the flag so it's TX's turn next
            isTransmitsTurn = 1;

            //Read byte from buffer
            ReceiveBuffer[ReceiveBuffer_Index++] = SPI2BUF;

            //Check if this byte is the end of the group.
            if (SendBufferFlags[ReceiveBuffer_Index - 1] == 1)
            {
                //Deselect the current device
                PORTDbits.RD4 = 1; //Hi to deselect
                //Reset the buffer's flag
                SendBufferFlags[ReceiveBuffer_Index - 1] = 0; //reset token
            }

            //If their are bytes to send
            if (SendBuffer_Index > SendBuffer_SentIndex)
            {
                //Set the TX Interupt flag so that it can send them
                INTSetFlag(INT_SPI2TX);
            }
            else
            {
                //Clear the TX interupt
                INTClearFlag(INT_SPI2TX);
                //update the flag that it's not running anymore.
                isRunnning = 0;
                //Clear the Send Buffer indecies
                SendBuffer_SentIndex = 0;
                SendBuffer_Index = 0;
            }

        }
        
    }

    //Transmit interrupt
    if (INTGetFlag(INT_SPI2TX))
    {
        //Clear Interrupt flag
        INTClearFlag(INT_SPI2TX);

        //If it's TX's turn
        if (isTransmitsTurn == 1)
        {
            //Set the flag so it's RX's turn next
            isTransmitsTurn = 0;

            //Select the current device
            PORTDbits.RD4 = 0;
            //Send the next byte
            SPI2BUF = SendBuffer[SendBuffer_SentIndex++];
        }
    }
}