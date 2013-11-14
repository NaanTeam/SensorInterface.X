
#include "FIFOUART.h"
#include <plib.h>

//Filler functions until I actually make this.



void FIFOUART_initialize()
{
    UARTConfigure(FIFOUART_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(FIFOUART_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(FIFOUART_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(FIFOUART_MODULE_ID, GetPeripheralClock(), FIFOUART_BAUD_RATE);
    UARTEnable(FIFOUART_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}




// *****************************************************************************
// void UARTTxBuffer(char *buffer, UINT32 size)
// *****************************************************************************
void FIFOUART_sendBuffer(const char *buffer, int size)
{
    while(size)
    {
        while(!UARTTransmitterIsReady(FIFOUART_MODULE_ID))
            ;

        UARTSendDataByte(FIFOUART_MODULE_ID, *buffer);

        buffer++;
        size--;
    }

    while(!UARTTransmissionHasCompleted(FIFOUART_MODULE_ID))
        ;
}

// *****************************************************************************
// UINT32 GetDataBuffer(char *buffer, UINT32 max_size)
// *****************************************************************************
UINT32 GetDataBuffer(char *buffer, UINT32 max_size)
{
    UINT32 num_char;

    num_char = 0;

    while(num_char < max_size)
    {
        UINT8 character;

        while(!UARTReceivedDataIsAvailable(FIFOUART_MODULE_ID))
            ;

        character = UARTGetDataByte(FIFOUART_MODULE_ID);

        if(character == '\r')
            break;

        *buffer = character;

        buffer++;
        num_char++;


    }

    return num_char;
}