/**
 * @file: FIFOSPI2.h
 * @brief: A 'First In First Out' interrupt driven SPI communication method
 * with this device setup as the master.
 *
 * @author: Connor Martin
 * @date: Nov 26, 2013
 *
 * @preconditions:
 * @device:
 *      -PIC32MX695F512L
 *
 * @remarks: Currently only supports 2 SPI devices. Some modifications need in
 * the setup and IRQ needed to support more. (possibly other places as well)
 *
 */

#include "FIFOSPI2.h"


//******************************************************************************
// Local Variables and Typedefs
//******************************************************************************

//Flags
uint8 FIFOSPI2_isRunnning = 0; //Represents if the SPI2 is currently running
int isTransmitsTurn = 1; //used for toggling between transmit and receive 

//Send buffers and indexes
int TxBuffer_Index = 0; //Index of how full the send buffer is.
int TxBuffer_TxIndex = 0; //Index of what has been sent
uint8 TxBuffer[FIFOSPI2_BUFFERSIZE]; //Contains the unit8's to send
uint8 TxBufferFlags[FIFOSPI2_BUFFERSIZE]; //Flags for when to drive SS.

int RxBuffer_Index = 0; //Index of how full the receive buffer is
int RxBuffer_ReadIndex = 0; //Index of what has been read out of the buffer
uint8 RxBuffer[FIFOSPI2_BUFFERSIZE]; //Holds all the receive chars from SPI2


//******************************************************************************
//Public Function Definitions
//******************************************************************************
void FIFOSPI2_initialize()
{
    //populate the buffers with zeros.
    int i = 0;
    while (i < FIFOSPI2_BUFFERSIZE) 
    {
        TxBuffer[i] = 0;
        TxBufferFlags[i] = 0;
        RxBuffer[i] = 0;
        i++;
    }

    //Sets up the pins for SPI for the second device pins
    TRISGbits.TRISG8 = 0; //SDO2 write
    TRISGbits.TRISG7 = 1; //SDI2 read
    TRISGbits.TRISG6 = 0; //SCK2 write


    FIFOSPI2_DeviceSSLine1_TriState = 0; //SS2 D1 write //Especially here
    FIFOSPI2_DeviceSSLine2_TriState = 0; //SS2 D2 write

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


    //Check to see if their are bytes queued up to be sent
    if (TxBuffer_Index > 0)
    {
        //Enables the SPI2 IRQ to start sending.
        INTEnable(INT_SPI2TX, INT_ENABLED); //SPI2 transmit buffer empty.
    }
        
    //Enable SPI2 Interupts
    INTEnable(INT_SPI2RX, INT_ENABLED);//SPI2 Receive Done.
    
    //Configure SPI2 and turn it on
    SPI2CONbits.CKP = 1;    //Clock polarity: Idle state for clock is a
                            //high level; Active state is a low level
    SPI2CONbits.SMP = 1;    // Input data sampled at end of data output time
    SPI2CONbits.MSTEN = 1;  //Master Mode Enable bit
    SPI2CONbits.ON = 1;     //SPI ON
}

int FIFOSPI2_addQueue(uint8 data[], int length, int deviceSSLine)
{
    int i = 0;
    //If the send buffer isn't full then add another char
    if ((TxBuffer_Index + length) < FIFOSPI2_BUFFERSIZE)
    {

//        //Adds a token to signify end signal group.
//        SendBufferFlags[SendBuffer_Index + length - 1] = 1;

        //Add each byte to the buffer
        while (i < length)
        {
            //Populate SendBuuferFlags with which device we a re sending to.
            TxBufferFlags[i] = deviceSSLine;
            TxBuffer[TxBuffer_Index++] = data[i];
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

int FIFOSPI2_readQueue(uint8 *bytesBuffer)
{
    //The receive buffer is empty.
    if (RxBuffer_Index == 0)
    {
        *bytesBuffer = 0;
        return 0;
    }
    //The receiver buffer has entries.
    else if (RxBuffer_Index < FIFOSPI2_BUFFERSIZE)
    {
        char rtn = RxBuffer[RxBuffer_ReadIndex++];
        //If all bytes from the buffer have been read.
        if (RxBuffer_ReadIndex >= RxBuffer_Index)
        {
            //Reset the indexs
            RxBuffer_ReadIndex = 0;
            RxBuffer_Index = 0;
        }

        *bytesBuffer = rtn;
        return 1;
    }
    //The receive buffer is overflowing.
    else if (RxBuffer_Index >= FIFOSPI2_BUFFERSIZE)
    {
        *bytesBuffer = 0;
        return -1;
    }

    return -3;
}

int FIFOSPI2_rxBufferIndex()
{
    return RxBuffer_Index;
}

//******************************************************************************
//Interrupt Request Routines
//******************************************************************************
void __ISR(_SPI_2_VECTOR, IPL4AUTO)__SPI2Interrupt(void)
{
    
    //Receive interupt
    if (INTGetFlag(INT_SPI2RX))
    {
        //If it's RX's turn
        if (isTransmitsTurn == 0) 
        {
            //Set the flag so it's TX's turn next
            isTransmitsTurn = 1;

            //Read byte from buffer
            RxBuffer[RxBuffer_Index++] = SPI2BUF;

            //Clear Interrupt flag
            INTClearFlag(INT_SPI2RX);


            //If the current device we are sending to doesn't match the next byte's device
            if (TxBufferFlags[TxBuffer_TxIndex] != TxBufferFlags[TxBuffer_TxIndex -1])
            {
                //TODO: decide if it's better just to deselect all devices.
                //Deselect the current device
                switch (TxBufferFlags[RxBuffer_Index - 1])
                {
                        case 0:
                            FIFOSPI2_DeviceSSLine1_PortReg = 1;
                            FIFOSPI2_DeviceSSLine2_PortReg = 1;
                            break;
                        case 1:
                             FIFOSPI2_DeviceSSLine1_PortReg = 1; //Hi to deselect
                             break;
                        case 2:
                             FIFOSPI2_DeviceSSLine2_PortReg = 1; //Hi to deselect
                             break;
                }
            }

            //If their are bytes to send
            if (TxBuffer_Index > TxBuffer_TxIndex)
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
                TxBuffer_TxIndex = 0;
                TxBuffer_Index = 0;
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
            switch (TxBufferFlags[TxBuffer_TxIndex])
            {
                    case 1:
                         FIFOSPI2_DeviceSSLine1_PortReg = 0; //Low to select
                         break;
                    case 2:
                         FIFOSPI2_DeviceSSLine2_PortReg = 0; //Low to select
                         break;
            }

            //Send the next byte
            SPI2BUF = TxBuffer[TxBuffer_TxIndex++];
        }
    }
}
