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
 * @Devices: PIC32MX320F128H
 */

#include "FIFOSPI2.h"



//Flags
unsigned char FIFOSPI2_isRunnning = 0; //Represents if the SPI2 is currently running
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

    //TODO: This needs to based on what device we are using and the sub-devices of the #defines.
    //Sets up the SPI2 pins for the PIC32M320H
    TRISGbits.TRISG8 = 0; //SDO2 write
    TRISGbits.TRISG7 = 1; //SDI2 read
    TRISGbits.TRISG6 = 0; //SCK2 write
    TRISDbits.TRISD4 = 0; //SS2 D1 write //Especially here
    TRISDbits.TRISD3 = 0; //SS2 D2 write

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
    //TODO: ERROR: CAUSED FROM EVER ENDING TX INTERUPT FLAGS
    
    //Check to see if their are bytes queued up to be sent
    if (SendBuffer_Index > 0)
    {
        //Enables the IRQ to start sending.
        INTEnable(INT_SPI2TX, INT_ENABLED); //SPI2 transmit buffer empty.
    }
        

    INTEnable(INT_SPI2RX, INT_ENABLED);//SPI2 Receive Done.
    
    //Configure SPI2 and turn it on
    SPI2CONbits.CKP = 1;    //Clock polarity: Idle state for clock is a
                            //high level; Active state is a low level
    SPI2CONbits.SMP = 1;    // Input data sampled at end of data output time
    SPI2CONbits.MSTEN = 1;  //Master Mode Enable bit
    SPI2CONbits.ON = 1;     //SPI ON
}

int FIFOSPI2_SendQueue(unsigned char data[], int length, int deviceSSLine)
{
    int i = 0;
    //If the send buffer isn't full then add another char
    if ((SendBuffer_Index + length) < FIFOSPI2_BUFFERSIZE)
    {

//        //Adds a token to signify end signal group.
//        SendBufferFlags[SendBuffer_Index + length - 1] = 1;

        //Add each byte to the buffer
        while (i < length)
        {
            //Populate SendBuuferFlags with which device we a re sending to.
            SendBufferFlags[i] = deviceSSLine;
            SendBuffer[SendBuffer_Index++] = data[i];
            i++;
        }

        //If SPI2 IRQ is not running then start it
        if (FIFOSPI2_isRunnning == 0)
        {
            FIFOSPI2_isRunnning = 1;
            
            //If the device is on and ready to send
            if (SPI2CONbits.ON == 1)
            {
                //Start sending
                INTSetFlag(INT_SPI2TX);
                INTEnable(INT_SPI2TX, INT_ENABLED);
            }
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

//PIC32MX320F128L
//#define RECEIVE_FLAG        INT_SPI2RX
//#define TX_FLAG             INT_SPI2TX

//PIC32MX320F128L
#define RECEIVE_FLAG        INT_U3RXIF
#define TX_FLAG             INT_U3TXIF

//This is the SPI2 Interupt Handler
void __ISR(_SPI_2_VECTOR, IPL4AUTO)__SPI2Interrupt(void)
{

//    INTSetFlag(INT_SPI2TX);
//    INTClearFlag(INT_SPI2TX);
//    INTSetFlag(INT_U3TX);
//    INTClearFlag(INT_U3TX);
//    SPI2BUF = 0x45;
//    int tst = IFS1;
//    int tst2 = SPI2STAT;
    
    //Receive interupt
    if (INTGetFlag(INT_SPI2RX))
    {
        //If it's RX's turn
        if (isTransmitsTurn == 0) 
        {
            //Set the flag so it's TX's turn next
            isTransmitsTurn = 1;

            //Read byte from buffer
            ReceiveBuffer[ReceiveBuffer_Index++] = SPI2BUF;

            //Clear Interrupt flag
            INTClearFlag(INT_SPI2RX);


            //If the current device we are sending to doesn't match the next byte's device
            if (SendBufferFlags[SendBuffer_SentIndex] != SendBufferFlags[SendBuffer_SentIndex -1])
            {
                //TODO: decide if it's better just to deselect all devices.
                //Deselect the current device
                switch (SendBufferFlags[ReceiveBuffer_Index - 1])
                {
                        case 0:
                            FIFOSPI2_DeviceSSLine1 = 1;
                            FIFOSPI2_DeviceSSLine2 = 1;
                            break;
                        case 1:
                             FIFOSPI2_DeviceSSLine1 = 1; //Hi to deselect
                             break;
                        case 2:
                             FIFOSPI2_DeviceSSLine2 = 1; //Hi to deselect
                             break;
                }
            }

            //If their are bytes to send
            if (SendBuffer_Index > SendBuffer_SentIndex)
            {
                //Set the TX Interupt flag so that it can send them
                INTSetFlag(INT_SPI2TX);
                INTEnable(INT_SPI2TX, INT_ENABLED);
            }
            else
            {
                //Clear and disable the TX interupt
                INTEnable(INT_SPI2TX, INT_DISABLED);
                INTClearFlag(INT_SPI2TX);
                
                
                //update the flag that it's not running anymore.
                FIFOSPI2_isRunnning = 0;
                //Clear the Send Buffer indecies
                SendBuffer_SentIndex = 0;
                SendBuffer_Index = 0;
            }

        }
        
    }

    //Transmit interrupt and it is enabled.
    if (INTGetFlag(INT_SPI2TX) && INTGetEnable(INT_SPI2TX))
    {
        //Clear Interrupt flag
        //INTClearFlag(INT_SPI2TX);
        INTEnable(INT_SPI2TX, INT_DISABLED);

        //If it's TX's turn
        if (isTransmitsTurn == 1)
        {
            //Set the flag so it's RX's turn next
            isTransmitsTurn = 0;

//            //Select the current device
//            FIFOSPI2_DeviceSSLine1 = 0;
            //Select the current device
            switch (SendBufferFlags[SendBuffer_SentIndex])
            {
                    case 1:
                         FIFOSPI2_DeviceSSLine1 = 0; //Low to select
                         break;
                    case 2:
                         FIFOSPI2_DeviceSSLine2 = 0; //Low to select
                         break;
            }

            //Send the next byte
            SPI2BUF = SendBuffer[SendBuffer_SentIndex++];
        }
    }
}