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

#ifndef FIFOSPI2_H
#define	FIFOSPI2_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <plib.h>
#include "Hardware.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************

/**@brief Controls the slave select pin for the first device on SPI-2 line.*/
#define FIFOSPI2_DeviceSSLine1_PortReg PORTDbits.RD4
/**@brief Match this to the corresponding FIFOSPI2_DeviceSSLine1_PortReg.*/
#define FIFOSPI2_DeviceSSLine1_TriState TRISDbits.TRISD4

/**@brief Controls the slave select pin for the second device on SPI-2 line.*/
#define FIFOSPI2_DeviceSSLine2_PortReg PORTDbits.RD3
/**@brief Match this to the corresponding FIFOSPI2_DeviceSSLine2_PortReg.*/
#define FIFOSPI2_DeviceSSLine2_TriState TRISDbits.TRISD3

/**@brief Controls the maximum transmit and receive buffer sizes.*/
#define FIFOSPI2_BUFFERSIZE 100



//******************************************************************************
// Public Variables and Typedefs
//******************************************************************************

/**@brief Indicates whether spi-2's IRQ is currently running. 1 it is. 0 it's not.*/
extern uint8 FIFOSPI2_isRunnning;



//******************************************************************************
// Public Function Declarations
//******************************************************************************

/**
 * @brief Sets up SPI2 in master mode using a FIFO buffer.
 *
 * Operates at PER_FREQ/4 frequency, TX and RX IRQ, Idle Clock polarity and
 * data sampled at end of output time.
 *
 * @return void
 */
void FIFOSPI2__initialize();

/**
 * @brief Adds uint8's to the SPI TX buffer.
 *
 * @param data: An array of uint8 to queue in the TX buffer.
 * @param length: The length of the data array.
 * @param deviceSSLine: The device to write to. Modify #defines for different
 * pins.
 *
 * @return A '1' if the characters where succesfully added. '-1' if the TX
 * buffer is full.
 */
int FIFOSPI2_addQueue(uint8 data[], int length, int deviceSSLine);

/**
 * @brief Transfers a byte from the SPI RX buffer to a uint8.
 *
 * @param readByte: A pointer to a uint8 variable.
 *
 * @return A '1' if the character was succesfully retrieved.
 * A '0' if the buffer is empty.
 * A '-1' if the receive buffer is overflowing.
 * A '-3' if ti's an unknown error.
*/
int FIFOSPI2_readQueue(uint8 *bytesBuffer);

/**
 * @brief Determines the occupied length of the receive buffer.
 *
 * @return An integer representing the index where the buffer is currently
 * full to.
 */
int FIFOSPI2_rxBufferIndex();


#ifdef	__cplusplus
}
#endif

#endif	/* FIFOSPI2_H */

