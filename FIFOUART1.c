
#include "FIFOUART1.h"

//******************************************************************************
// Local Variables and Typedefs
//******************************************************************************
uint16 FIFOUART1_RxBuffer_Index = 0;
uint16 FIFOUART1_RxBuffer_ReadIndex = 0;
uint8 FIFOUART1_RxBuffer[FIFOUART1_BUFFERSIZE]; //Holds all the receive chars from UART

uint16 FIFOUART1_TxBuffer_Index = 0;
uint16 FIFOUART1_TxBuffer_TxIndex = 0;
uint8 FIFOUART1_TxBuffer[FIFOUART1_BUFFERSIZE]; //Holds all the chars to TX on UART1


//******************************************************************************
//Local Function Declarations
//******************************************************************************
inline int FIFOUART1_pushRxQueue(uint8 rxBytes[], int length);
inline int FIFOUART1_popTxQueue(uint8* txByte);

//******************************************************************************
//Local Function Definitions
//******************************************************************************
inline int FIFOUART1_pushRxQueue(uint8 rxBytes[], int length)
{
    int i = 0;

    if ((FIFOUART1_RxBuffer_Index + length) >= FIFOUART1_BUFFERSIZE)
    {
        //Overflow condition
        return -1;
    }
    else
    {
        //Add each byte to the tx buffer
        while (i < length)
        {
            FIFOUART1_RxBuffer[FIFOUART1_RxBuffer_Index] = rxBytes[i];
            FIFOUART1_RxBuffer_Index++;
            i++;

        }

        return 1;
    }
}
inline int FIFOUART1_popTxQueue(uint8* txByte)
{
    if (FIFOUART1_TxBuffer_Index <= 0)
    {//The receive buffer is empty.

        *txByte = 0;
        return -2;
    }
    else if (FIFOUART1_TxBuffer_Index >= FIFOUART1_BUFFERSIZE)
    {//The receive buffer is overflowing.

        *txByte = 0;
        return -1;
    }

    else
    {//The receiver buffer has entries.

        //pop byte
        *txByte = FIFOUART1_TxBuffer[FIFOUART1_TxBuffer_TxIndex++];

        //If all bytes from the buffer have been read.
        if (FIFOUART1_TxBuffer_TxIndex >= FIFOUART1_TxBuffer_Index)
        {
            //Reset the indexs
            FIFOUART1_TxBuffer_TxIndex = 0;
            FIFOUART1_TxBuffer_Index = 0;

            return 1; //success, empty buffer
        }
        else
        {
            return 2; //success, non-empty buffer
        }

    }

    //unknown error (possibly a wild program counter got us here)
    return -3;
}

//******************************************************************************
//Public Function Definitions
//******************************************************************************
void FIFOUART1_initialize()
{
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);

    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | //Sets the data transfer size to 8-bits per frame. 
            UART_PARITY_NONE | //Disables parity bit generation. 
            UART_STOP_BITS_1); //1 stop bit per frame (default). 

    UARTSetDataRate(UART1, GetPeripheralClock(), FIFOUART1_BAUD_RATE);



    //Interrupt Stuff
    INTSetVectorPriority(INT_UART_1_VECTOR, INT_PRIORITY_LEVEL_5);
    INTSetVectorSubPriority(INT_UART_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    INTClearFlag(INT_U1RX);
    INTClearFlag(INT_U1TX);

    //configure what triggers UART1 itnerrupts
    UARTSetFifoMode(UART1,
        UART_INTERRUPT_ON_TX_BUFFER_EMPTY | //TX interrupt will occur when the TX buffer is empty. 
        UART_INTERRUPT_ON_RX_NOT_EMPTY); //RX interrupt will occur whenever the RX buffer has any data. 

    //Enable UART1 Rx Interrupt
    INTEnable(INT_U1RX, INT_ENABLED);
    //Enable UART1 Tx Interrupt
    //INTEnable(INT_U1TX, INT_ENABLED);

    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

}

int FIFOUART1_pushTxQueue(uint8 txBytes[], int length)
{
    int i = 0;

    if ((FIFOUART1_TxBuffer_Index + length) >= FIFOUART1_BUFFERSIZE)
    {
        //Overflow condition
        return -1;
    }
    else
    {
        //Add each byte to the tx buffer
        while (i < length)
        {
            FIFOUART1_TxBuffer[FIFOUART1_TxBuffer_Index] = txBytes[i];
            FIFOUART1_TxBuffer_Index++;
            i++;
        }

        //enable TX interrupt
        INTEnable(INT_U1TX, INT_ENABLED);


        return 1;
    }


}

int FIFOUART1_popRxQueue(uint8* rxByte)
{
    if (FIFOUART1_RxBuffer_Index <= 0)
    {//The receive buffer is empty.

        *rxByte = 0;
        return -2;
    }
    else if (FIFOUART1_RxBuffer_Index >= FIFOUART1_BUFFERSIZE)
    {//The receive buffer is overflowing.

        *rxByte = 0;
        return -1;
    }

    else
    {//The receiver buffer has entries.

        //pop byte
        *rxByte = FIFOUART1_RxBuffer[FIFOUART1_RxBuffer_ReadIndex++];

        //If all bytes from the buffer have been read.
        if (FIFOUART1_RxBuffer_ReadIndex >= FIFOUART1_RxBuffer_Index)
        {
            //Reset the indexs
            FIFOUART1_RxBuffer_ReadIndex = 0;
            FIFOUART1_RxBuffer_Index = 0;

            return 1; //success, empty buffer
        }
        else
        {
            return 2; //success, non-empty buffer
        }
    }

    //unknown error (possibly a wild program counter got us here)
    return -3;
}


//******************************************************************************
//Interrupt Request Routines
//******************************************************************************
void __ISR(_UART_1_VECTOR, IPL5AUTO) __UART1Interrupt(void)
{
    uint8 rxByte = 0;
    uint8 txByte = 0;
    int rslt = 0;

    if (INTGetFlag(INT_U1RX))
    {
        //Add received byte
        rxByte = UARTGetDataByte(UART1);
        FIFOUART1_pushRxQueue(&rxByte, 1);

        INTClearFlag(INT_U1RX);
    }
    if (INTGetFlag(INT_U1TX))
    {
        rslt = FIFOUART1_popTxQueue(&txByte);

        switch (rslt)
        {
            case 2: //Success, non-empty buffer
                UARTSendDataByte(UART1, txByte);
                break;

            case 1: //Success, empty buffer
                UARTSendDataByte(UART1, txByte);
                INTEnable(INT_U1TX, INT_DISABLED);
                break;

            case -1: //Queue is Overflowing
                //Reset the indexs
                FIFOUART1_TxBuffer_TxIndex = 0;
                FIFOUART1_TxBuffer_Index = 0;
            case -2: //Queue is Empty
            default: //unknown result
                INTEnable(INT_U1TX, INT_DISABLED);
                break;
        }

        INTClearFlag(INT_U1TX);
    }
}