/**
 * @file: FIFOI2C.h
 * @author: Connor Martin
 * @date: Sept 10, 2013
 * @brief: A FIFO interrupt driven I2C communication method with this
 *  device setup as the master.
 * @precondition:
 * @device:
 *      -PIC32MX695F512L
 *          -Uses I2C2, the second I2C module.
 * @remarks: 
 */

#ifndef FIFOI2C_H
#define	FIFOI2C_H

    #include <plib.h>
    #include "Hardware.h"

    // <editor-fold defaultstate="collapsed" desc="Configuration #defines">
    /**Controls the baud rate of the I2C device.*/
    #define FIFOI2C_BAUD_RATE 100000    

    /**Controls the maximum number of devices the I2C module supports.
     * Note: Modification of FIFOI2C_initialize() is required to support
     *  more than 5 devices.*/
    #define FIFOI2C_DEVICES_COUNT 1

    /**Controls I2C device 0's 7-bit address*/
    #define FIFOI2C_DEVICE0_ADDRESS 0x1E  //7 bit address
    /**Controls I2C device 1's 7-bit address*/
    #define FIFOI2C_DEVICE1_ADDRESS 0x00
    /**Controls I2C device 2's 7-bit address*/
    #define FIFOI2C_DEVICE2_ADDRESS 0x00
    /**Controls I2C device 3's 7-bit address*/
    #define FIFOI2C_DEVICE3_ADDRESS 0x00
    /**Controls I2C device 4's 7-bit address*/
    #define FIFOI2C_DEVICE4_ADDRESS 0x00

    /**Controls the maximum size of the transmit buffer.*/
    #define FIFOI2C_TRANSMIT_BUFFER_SIZE 50
    /**Controls the maximum size of the receive buffer.*/
    #define FIFOI2C_RECEIVE_BUFFER_SIZE 50 //Not having this large enough causes wierd bugs
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Public Variables and Typedefs">
    /**Commands the IRQ what action to take next.*/
    typedef enum
    {
        /**Signifies an unknown error*/
        FIFOI2C_DEVICE_COMMAND_CMDERROR,
        /**Signifies an EOT to the program*/
        FIFOI2C_DEVICE_COMMAND_CMDEND,
        /**Start Transmission Routine*/
        FIFOI2C_DEVICE_COMMAND_START,
        /**Transmit corresponding byte*/
        FIFOI2C_DEVICE_COMMAND_TX_BYTE,
        /**Read the received byte*/
        FIFOI2C_DEVICE_COMMAND_RX_BYTE,
        /**Restart device routine*/
        FIFOI2C_DEVICE_COMMAND_RESTART,
        /**ACK routine*/
        FIFOI2C_DEVICE_COMMAND_ACK,
        /**Not ACK routine*/
        FIFOI2C_DEVICE_COMMAND_NACK,
        /**Stop routine*/
        FIFOI2C_DEVICE_COMMAND_STOP
    }FIFOI2C_Device_Commands;
    /**Represents an action for the I2C Master IRQ to take with a corresponding byte.*/
    typedef struct fifoi2c_tx_byte
    {
        /**The corresponding byte*/
        uint8 tx_byte;
        /**Action for I2C IRQ to take.*/
        FIFOI2C_Device_Commands device_command;
    }FIFOI2C_TX_Byte;
    /**Represents a received byte from I2C.*/
    typedef struct fifoi2c_rx_byte
    {
        /**The read byte*/
        uint8 rx_byte;
        /**Corresponding action that the IRQ was on.*/
        FIFOI2C_Device_Commands device_command;
    }FIFOI2C_RX_Byte;
    /**Represents a I2C device*/
    typedef struct fifoi2c_device
    {
        /**The 7-bit address*/
        uint8 address;

        /**The index of the byte to be transmitted next*/
        int transmit_buffer_current;
        /**The length of the transmission buffer.*/
        int transmit_buffer_length;
        /**Buffer of bytes to be transmitted.*/
        FIFOI2C_TX_Byte transmit_buffer[FIFOI2C_TRANSMIT_BUFFER_SIZE];

        /**The index of the next byte to be read from the reception buffer*/
        int receive_buffer_current;
        /**The length of the reception buffer*/
        int receive_buffer_length;
        /**The reception buffer of bytes from the I2C IRQ.*/
        FIFOI2C_RX_Byte receive_buffer[FIFOI2C_TRANSMIT_BUFFER_SIZE];
    }FIFOI2C_Device;
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Public Function Declarations">
    /**
     * Initializes I2C module 2 for interrupt driven communication.
     */
    void FIFOI2C_initialize();
    /**
     * Adds device commands, with their corresponding byte, to the FIFO I2C
     * buffer to be sent.
     * @param device: The device number (listed in the header file) for the
     * commands to be sent to.
     * @param byte_buffer: Bytes to be sent.
     * @param state_buffer: Commands to be sent. This should end with
     * a FIFOI2C_DEVICE_COMMAND_CMDERROR for the buffer to be processed right.
     * @param buffer_length: The length of the byte and command buffer.
     * @return: Returns 1 if successful, -1 if their is an error.
     */
    uint8 FIFOI2C_addQueue(uint16 device, uint8 byte_buffer[], FIFOI2C_Device_Commands state_buffer[], uint32 buffer_length);
    /**
     * Reads a single RX_byte from receive buffer.
     * @param device: The device this byte was sent from.
     * @return: An RX_byte from the device receive buffer.
     */
    FIFOI2C_RX_Byte FIFOI2C_readQueue(uint16 device);
    /**
     * Queues a 'read register' command sequence for I2C in the TX buffer.
     * @param device: The device number (listed in the header file) for the
     * commands to be sent to.
     * @param start_register: The first register to read from. Successive reads
     * will be of the next register.
     * @param number_to_read: The number of register to read, starting at start_register.
     * @return: Returns 0 if the command sequence was succesively added to
     * the TX buffer, -1 otherwise.
     */
    uint8 FIFOI2C_addQueue_readDeviceRegisters(uint16 device, uint8 start_register, int number_to_read);
    /**
     * 
     * @param device: The device number (listed in the header file) for the
     * commands to be sent to.
     * @param start_address: The first register to write to. Additional bytes
     * will be written to the next register in memory.
     * @param byte_buffer: The register values you wish to transmit.
     * @param buffer_length: The length of the byte buffer.
     * @return: Returns 0 if the command sequence was succesively added to
     * the TX buffer, -1 otherwise.
     */
    uint8 FIFOI2C_addQueue_writeDeviceRegisters(uint16 device, uint8 start_address, uint8 byte_buffer[], uint32 buffer_length);
    // </editor-fold>

#endif	/* FIFOI2C_H */

