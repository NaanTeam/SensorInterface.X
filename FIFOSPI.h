/**
 * @Author: Connor Martin
 * @Description: A FIFO interrupt driven spi communication method.
 * @Pins:
 *  Chipkit     PIC32MX3XX/4XX  Registers    Descritpion
 *  10          52              RD4          SS2 write
 *  11          6               RG8          SDO2 write
 *  12          5               RG7          SDI2 read
 *  13          4               RG6          SCK2 write
 */

#ifndef FIFOSPI_H
#define	FIFOSPI_H

#include <plib.h>

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * Sets up SPI2 in master modeusing a FIFO buffer at PER_FREQ/4 frequency,
     * TX and RX IRQ, Idle Clock polarity and data sampled at end of
     * output time.
     */
    void FIFOSPI2_Setup();
    /**
     * Adds characters to the SPI buffer to be send.
     *
     * @param data: An array of characters to queue for the SPI to send.
     * @param length: The length of the data array.
     *
     * @return A '1' if the characters where succesfully added. -1 if the send
     * buffer is full.
     */
    int FIFOSPI2_SendQueue(unsigned char data[], int length);

    /**
     * Transfers a byte from the received SPI buffer to a pointer.
     *
     * @param readByte: A pointer to a char variable.
     *
     * @return A '1' if the character was succesfully retrieved. 
     * A '0' if the buffer is empty.
     * A '-1' if the receive buffer is overflowing.
     * A '-2' unknown error.
    */
    int FIFOSPI2_ReadQueue(unsigned char *bytesBuffer);

    /**
     * Determines the occupied length of the receive buffer.
     * @return An integer representing the index where the buffer is currently
     * full to.
     */
    int FIFOSPI2_RecieveBufferIndex();


#ifdef	__cplusplus
}
#endif

#endif	/* FIFOSPI_H */

