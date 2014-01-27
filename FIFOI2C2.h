/**
 * @file: FIFOI2C2.h
 * @brief: A 'First In First Out' interrupt driven I2C communication method
 * with this device setup as the master.
 *
 * @author: Connor Martin
 * @date: Nov 26, 2013
 *
 * @preconditions:
 * @device:
 *      -PIC32MX695F512L
 *          -Uses I2C2, the second I2C module.
 *
 * @remarks:
 */

#ifndef FIFOI2C2_H
#define	FIFOI2C2_H

#include <plib.h>
#include "Hardware.h"

//******************************************************************************
//Configuration #defines
//******************************************************************************

/**@brief Controls the baud rate of the I2C device.*/
#define FIFOI2C2_BAUD_RATE 100000

/**@brief Controls the maximum number of devices the I2C module supports.
 * 
 * @remarks Some modifications to FIFOI2C2_initialize() are required to support
 * more than 5 devices.*/
#define FIFOI2C2_DEVICES_COUNT 1

/**@brief Controls I2C device 0's 7-bit address.*/
#define FIFOI2C2_DEVICE0_ADDRESS 0x1E  //7 bit address
/**Controls I2C device 1's 7-bit address*/
#define FIFOI2C2_DEVICE1_ADDRESS 0x00
/**Controls I2C device 2's 7-bit address*/
#define FIFOI2C2_DEVICE2_ADDRESS 0x00
/**Controls I2C device 3's 7-bit address*/
#define FIFOI2C2_DEVICE3_ADDRESS 0x00
/**Controls I2C device 4's 7-bit address*/
#define FIFOI2C2_DEVICE4_ADDRESS 0x00

/**@brief Controls the maximum size of the transmit buffer.
 * 
 * @remarks Insufficient size may lead to lost data or overflows if transmission
 * speed isn't fast enough.*/
#define FIFOI2C2_TRANSMIT_BUFFER_SIZE 50

/**@brief Controls the maximum size of the receive buffer.
 * 
 * @remarks Insufficient size may lead to lost data or overflows if transmission
 * speed isn't fast enough.*/
#define FIFOI2C2_RECEIVE_BUFFER_SIZE 50  //TODO: Fix so that it can't overflow



//******************************************************************************
// Public Variables and Typedefs
//******************************************************************************

/**@brief Represents an action or step for the I2C Master IRQ to take.
 *
 * @remarks Some require specific orders to be followed..*/
typedef enum
{
    /**@brief Signifies an unknown error*/
    FIFOI2C2_DEVICE_COMMAND_CMDERROR,
    /**@brief Signifies an end of transmission to the IRQ*/
    FIFOI2C2_DEVICE_COMMAND_CMDEND,
    /**@brief Start Transmission Routine*/
    FIFOI2C2_DEVICE_COMMAND_START,
    /**@brief Transmit corresponding byte*/
    FIFOI2C2_DEVICE_COMMAND_TX_BYTE,
    /**@brief Read the received byte*/
    FIFOI2C2_DEVICE_COMMAND_RX_BYTE,
    /**@brief Restart device routine*/
    FIFOI2C2_DEVICE_COMMAND_RESTART,
    /**@brief ACK routine*/
    FIFOI2C2_DEVICE_COMMAND_ACK,
    /**@brief Not ACK routine*/
    FIFOI2C2_DEVICE_COMMAND_NACK,
    /**@brief Stop routine*/
    FIFOI2C2_DEVICE_COMMAND_STOP
}FIFOI2C2_Device_Commands;

/**@brief Represents a transmit byte from I2C.*/
typedef struct FIFOI2C2_tx_byte
{
    /**The corresponding byte*/
    uint8 tx_byte;
    /**Action for I2C IRQ to take.*/
    FIFOI2C2_Device_Commands device_command;
}FIFOI2C2_TX_Byte;

/**@brief Represents a received byte from I2C.*/
typedef struct FIFOI2C2_rx_byte
{
    /**The read byte*/
    uint8 rx_byte;
    /**Corresponding action that the IRQ was on.*/
    FIFOI2C2_Device_Commands device_command;
}FIFOI2C2_RX_Byte;

/**@brief Represents a I2C device*/
typedef struct FIFOI2C2_device
{
    /**The 7-bit address*/
    uint8 address;

    /**The index of the byte to be transmitted next*/
    int transmit_buffer_current;
    /**The length of the transmission buffer.*/
    int transmit_buffer_length;
    /**Buffer of bytes to be transmitted.*/
    FIFOI2C2_TX_Byte transmit_buffer[FIFOI2C2_TRANSMIT_BUFFER_SIZE];

    /**The index of the next byte to be read from the reception buffer*/
    int receive_buffer_current;
    /**The length of the reception buffer*/
    int receive_buffer_length;
    /**The reception buffer of bytes from the I2C IRQ.*/
    FIFOI2C2_RX_Byte receive_buffer[FIFOI2C2_TRANSMIT_BUFFER_SIZE];
}FIFOI2C2_Device;



//******************************************************************************
// Public Function Declarations
//******************************************************************************

/**
 * @brief Initializes I2C module 2 for interrupt driven communication.
 *
 * @return void
 */
void FIFOI2C2_initialize();

/**
 * @brief Adds device commands, with their corresponding byte, to the FIFO I2C
 * buffer to be sent over I2C.
 *
 * @param device: The device number (listed in the header file) for the
 * commands to be sent to.
 * @param byte_buffer: Bytes to be sent.
 * @param state_buffer: Commands to be sent. This should end with
 * a FIFOI2C2_DEVICE_COMMAND_CMDERROR for the buffer to be processed right.
 * @param buffer_length: The length of the byte and command buffer.
 *
 * @return: Returns 1 if successful, -1 if their is an error.
 */
uint8 FIFOI2C2_addQueue(uint16 device, uint8 byte_buffer[], FIFOI2C2_Device_Commands state_buffer[], uint32 buffer_length);

/**
 * @brief Reads a single RX_byte from receive buffer.
 *
 * @param device: The device this byte was sent from.
 *
 * @return: An RX_byte from the device receive buffer.
 */
FIFOI2C2_RX_Byte FIFOI2C2_readQueue(uint16 device);

/**
 * @brief Queues a 'read register' command sequence for I2C in the TX buffer.
 *
 * @param device: The device number (listed in the header file) for the
 * commands to be sent to.
 * @param start_register: The first register to read from. Successive reads
 * will be of the next register.
 * @param number_to_read: The number of register to read, starting at start_register.
 *
 * @return: Returns 0 if the command sequence was succesively added to
 * the TX buffer, -1 otherwise.
 */
uint8 FIFOI2C2_addQueue_readDeviceRegisters(uint16 device, uint8 start_register, int number_to_read);

/**
 * @brief Queues a 'write register' command sequence for I2C in the TX buffer.
 *
 * @param device: The device number (listed in the header file) for the
 * commands to be sent to.
 * @param start_address: The first register to write to. Additional bytes
 * will be written to the next register in memory.
 * @param byte_buffer: The register values you wish to transmit.
 * @param buffer_length: The length of the byte buffer.
 *
 * @return: Returns 0 if the command sequence was succesively added to
 * the TX buffer, -1 otherwise.
 */
uint8 FIFOI2C2_addQueue_writeDeviceRegisters(uint16 device, uint8 start_address, uint8 byte_buffer[], uint32 buffer_length);

#endif	/* FIFOI2C2_H */

