/**
 * @file: FIFOI2C2.c
 * @brief: A 'First In First Out' interrupt driven I2C communication method
 * with this device setup as the master. The second I2C device is in use.
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
#include "FIFOI2C2.h"


//******************************************************************************
//Local Variable and Typedef Declarations
//******************************************************************************
static FIFOI2C2_Device FIFOI2C2_Devices_List[FIFOI2C2_DEVICES_COUNT];
static uint8 FIFOI2C2_isRunning = 0;
static uint16 FIFOI2C2_currentDevice = 0;
static uint8 FIFOI2C2_nextInterruptReceive = 0;



//******************************************************************************
//Public Function Definitions
//******************************************************************************
void FIFOI2C2_initialize()
{
    int i = 0;
    int device = 0;
    
    //Interrupt Stuff
    INTSetVectorPriority(INT_I2C_2_VECTOR, INT_PRIORITY_LEVEL_5);
    INTSetVectorSubPriority(INT_I2C_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    INTClearFlag(INT_I2C2B);
    INTClearFlag(INT_I2C2M);

    //Enable I2C Bus Collision Event interrupt
    INTEnable(INT_I2C2B, INT_ENABLED);
    //Enable I2C2 Master Event interrupt
    INTEnable(INT_I2C2M, INT_ENABLED);


    //Initialize each device's local variables
    #if FIFOI2C2_DEVICES_COUNT >= 1
        device = 0;
        FIFOI2C2_Devices_List[device].address = FIFOI2C2_DEVICE0_ADDRESS;
        FIFOI2C2_Devices_List[device].transmit_buffer_current = 0;
        FIFOI2C2_Devices_List[device].transmit_buffer_length = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_current = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_length = 0;

        i = 0;
        while(i < FIFOI2C2_TRANSMIT_BUFFER_SIZE)
        {
            FIFOI2C2_TX_Byte txb;
            txb.tx_byte = 0;
            txb.device_command = 0;
            FIFOI2C2_Devices_List[device].transmit_buffer[i++] = txb;
        }
        i = 0;
        while(i < FIFOI2C2_RECEIVE_BUFFER_SIZE)
        {
            FIFOI2C2_RX_Byte rxb;
            rxb.rx_byte = 0;
            rxb.device_command = 0;
            FIFOI2C2_Devices_List[device].receive_buffer[i++] = rxb;
        }
    #endif
    #if FIFOI2C2_DEVICES_COUNT >= 2
        device = 1;
        FIFOI2C2_Devices_List[device].address = FIFOI2C2_DEVICE1_ADDRESS;
        FIFOI2C2_Devices_List[device].transmit_buffer_current = 0;
        FIFOI2C2_Devices_List[device].transmit_buffer_length = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_current = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_length = 0;

        i = 0;
        while(i < FIFOI2C2_TRANSMIT_BUFFER_SIZE)
        {
            FIFOI2C2_TX_Byte txb;
            txb.tx_byte = 0;
            txb.device_command = 0;
            FIFOI2C2_Devices_List[device].transmit_buffer[i++] = txb;
        }
        i = 0;
        while(i < FIFOI2C2_RECEIVE_BUFFER_SIZE)
        {
            FIFOI2C2_RX_Byte rxb;
            rxb.rx_byte = 0;
            rxb.device_command = 0;
            FIFOI2C2_Devices_List[device].receive_buffer[i++] = rxb;
        }
    #endif
    #if FIFOI2C2_DEVICES_COUNT >= 3
        device = 2;
        FIFOI2C2_Devices_List[device].address = FIFOI2C2_DEVICE2_ADDRESS;
        FIFOI2C2_Devices_List[device].transmit_buffer_current = 0;
        FIFOI2C2_Devices_List[device].transmit_buffer_length = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_current = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_length = 0;

        i = 0;
        while(i < FIFOI2C2_TRANSMIT_BUFFER_SIZE)
        {
            FIFOI2C2_TX_Byte txb;
            txb.tx_byte = 0;
            txb.device_command = 0;
            FIFOI2C2_Devices_List[device].transmit_buffer[i++] = txb;
        }
        i = 0;
        while(i < FIFOI2C2_RECEIVE_BUFFER_SIZE)
        {
            FIFOI2C2_RX_Byte rxb;
            rxb.rx_byte = 0;
            rxb.device_command = 0;
            FIFOI2C2_Devices_List[device].receive_buffer[i++] = rxb;
        }
    #endif
    #if FIFOI2C2_DEVICES_COUNT >= 4
        device = 3;
        FIFOI2C2_Devices_List[device].address = FIFOI2C2_DEVICE3_ADDRESS;
        FIFOI2C2_Devices_List[device].transmit_buffer_current = 0;
        FIFOI2C2_Devices_List[device].transmit_buffer_length = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_current = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_length = 0;

        i = 0;
        while(i < FIFOI2C2_TRANSMIT_BUFFER_SIZE)
        {
            FIFOI2C2_TX_Byte txb;
            txb.tx_byte = 0;
            txb.device_command = 0;
            FIFOI2C2_Devices_List[device].transmit_buffer[i++] = txb;
        }
        i = 0;
        while(i < FIFOI2C2_RECEIVE_BUFFER_SIZE)
        {
            FIFOI2C2_RX_Byte rxb;
            rxb.rx_byte = 0;
            rxb.device_command = 0;
            FIFOI2C2_Devices_List[device].receive_buffer[i++] = rxb;
        }
    #endif
    #if FIFOI2C2_DEVICES_COUNT >= 5
        device = 4;
        FIFOI2C2_Devices_List[device].address = FIFOI2C2_DEVICE4_ADDRESS;
        FIFOI2C2_Devices_List[device].transmit_buffer_current = 0;
        FIFOI2C2_Devices_List[device].transmit_buffer_length = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_current = 0;
        FIFOI2C2_Devices_List[device].receive_buffer_length = 0;

        i = 0;
        while(i < FIFOI2C2_TRANSMIT_BUFFER_SIZE)
        {
            FIFOI2C2_TX_Byte txb;
            txb.tx_byte = 0;
            txb.device_command = 0;
            FIFOI2C2_Devices_List[device].transmit_buffer[i++] = txb;
        }
        i = 0;
        while(i < FIFOI2C2_RECEIVE_BUFFER_SIZE)
        {
            FIFOI2C2_RX_Byte rxb;
            rxb.rx_byte = 0;
            rxb.device_command = 0;
            FIFOI2C2_Devices_List[device].receive_buffer[i++] = rxb;
        }
    #endif


    //configure the clock for the I2C2
    I2C2BRG = (1.0/(2.0*FIFOI2C2_BAUD_RATE) - 104e-9) * GetPeripheralClock() - 1.5; //-1.5 takes into account rounding for -2.

    //Turns on the I2C2 Module
    I2C2CONbits.ON = 1;
}

uint8 FIFOI2C2_addQueue(uint16 device, uint8 byte_buffer[], FIFOI2C2_Device_Commands state_buffer[], uint32 buffer_length)
{
    int i = 0, ind = 0;
    FIFOI2C2_TX_Byte txb;

    //Check for potential overflow of the TX buffer.
    if ((FIFOI2C2_Devices_List[device].transmit_buffer_length + buffer_length) >= (FIFOI2C2_TRANSMIT_BUFFER_SIZE - 1))
    {
        return -1;
    }
    else
    {
        //Adds the byte and state buffers to the Device's transmit buffer
        while (i < buffer_length)
        {
            //Selects this i-th byte
            txb.tx_byte = byte_buffer[i];
            //Selects the i-th command
            txb.device_command = state_buffer[i];
            //Sets the index to the current device's transmit buffer length
            ind = FIFOI2C2_Devices_List[device].transmit_buffer_length;
            //transfer the byte to the current devices buffer
            FIFOI2C2_Devices_List[device].transmit_buffer[ind] = txb;
            FIFOI2C2_Devices_List[device].transmit_buffer_length++;
            i++;
        }

        //If FIFOI2C2 isn't running, then start it
        if (FIFOI2C2_isRunning == 0)
        {
            FIFOI2C2_currentDevice = device;
            FIFOI2C2_isRunning = 1;
            //Trigger to I2C2 Master IRQ is interrupts are enabled.
            INTSetFlag(INT_I2C2M);
        }
    }

    return 1;
}

FIFOI2C2_RX_Byte FIFOI2C2_readQueue(uint16 device)
{
    int ind = 0;
    FIFOI2C2_RX_Byte rxb;

    //Checks for read-overflow error
    if (FIFOI2C2_Devices_List[device].receive_buffer_length >= (FIFOI2C2_RECEIVE_BUFFER_SIZE - 1))
    {
        //Sets the received byte to indicate an error
        rxb.device_command = FIFOI2C2_DEVICE_COMMAND_CMDERROR;
        rxb.rx_byte = 0;
    }
    else
    {
        //Sets the index to the current length of the receive buffer
        ind = FIFOI2C2_Devices_List[device].receive_buffer_current;
        //pulls the received byte to a local variable
        rxb.device_command = FIFOI2C2_Devices_List[device].receive_buffer[ind].device_command;
        rxb.rx_byte = FIFOI2C2_Devices_List[device].receive_buffer[ind].rx_byte;
        //increments how many bytes have been read by (for the device)
        FIFOI2C2_Devices_List[device].receive_buffer_current++;


        //If all the bytes have been read from the receive buffer reset the indexes
        if (FIFOI2C2_Devices_List[device].receive_buffer_current >= FIFOI2C2_Devices_List[device].receive_buffer_length)
        {
            //Sensitive Code. Disable temporarily master interrupt (if it's enabled.)
            if (INTGetEnable(INT_I2C2M) != 0)
            {
                INTEnable(INT_I2C2M, INT_DISABLED);
                FIFOI2C2_Devices_List[device].receive_buffer_length = 0;
                FIFOI2C2_Devices_List[device].receive_buffer_current = 0;
                INTEnable(INT_I2C2M, INT_ENABLED);
            }
            else
            {
                FIFOI2C2_Devices_List[device].receive_buffer_length = 0;
                FIFOI2C2_Devices_List[device].receive_buffer_current = 0;
            }

        }

        //Return the received byte
        return rxb;
    }
}

uint8 FIFOI2C2_addQueue_readDeviceRegisters(uint16 device, uint8 start_register, int number_to_read)
{
    int i = 0;
    int ind = 0;
    int j = 0; 
    uint8 byte_buffer[FIFOI2C2_TRANSMIT_BUFFER_SIZE];
    FIFOI2C2_Device_Commands state_buffer[FIFOI2C2_TRANSMIT_BUFFER_SIZE];

    //If their are bytes actually requeste to read
    if (number_to_read > 0)
    {
        //Start the I2C device
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_START;
        byte_buffer[ind++] = 0x00; //filler

        //Transmit byte: Device Addr + write
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_TX_BYTE;
        byte_buffer[ind++] = (FIFOI2C2_Devices_List[device].address << 1) |
                0x00; //LSB is 0 to inicate a write

        //Transmit byte: The start register address
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_TX_BYTE;
        byte_buffer[ind++] = start_register;

        ////Restart the I2C process
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_RESTART;
        byte_buffer[ind++] = 0x00; //filler


        //Transmit byte: Device Address + Read
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_TX_BYTE;
        byte_buffer[ind++] = (FIFOI2C2_Devices_List[device].address << 1) |
                0x01; //LSB is 1 to inicate a read

        //If their are more than 1 'read' requests to send then this routine runs.
        //Basically it continues to add read requests.
        while (i < (number_to_read - 1))
        {
            //Run the receive byte routine because the slave device (should of)
            //sent a byte back to after the request
            state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_RX_BYTE;
            byte_buffer[ind++] = 0x00; //filler

            //Master send ACK because it received data
            state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_ACK;
            byte_buffer[ind++] = 0x00; //filler

            i++;
        }

        //Sensor sends data (receive byte routine)
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_RX_BYTE;
        byte_buffer[ind++] = 0x00; //filler

        //Master send NACK to end burst read routine
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_NACK;
        byte_buffer[ind++] = 0x00; //filler

        //Master sends STOP to end the I2C routine
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_STOP;
        byte_buffer[ind++] = 0x00; //filler

        //Required for the I2C IRQ to know when to stop.
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_CMDEND;
        byte_buffer[ind++] = 0x00; //filler


        //Adds these commands to the device's TX buffer and starts the IRQ
        FIFOI2C2_addQueue(device, byte_buffer, state_buffer, ind);

        return 0;
    }
    else
    {
        //If their aren't any read requests. Return -1.
        return -1;
    }
}

uint8 FIFOI2C2_addQueue_writeDeviceRegisters(uint16 device, uint8 start_register, uint8 byte_buffer[], uint32 buffer_length)
{
    int i = 0, ind = 0;
    uint8 send_byte_buffer[FIFOI2C2_TRANSMIT_BUFFER_SIZE];
    FIFOI2C2_Device_Commands state_buffer[FIFOI2C2_TRANSMIT_BUFFER_SIZE];

    if ((FIFOI2C2_Devices_List[device].transmit_buffer_length + buffer_length) <= (FIFOI2C2_TRANSMIT_BUFFER_SIZE - 1))
    {
        //Start the I2C device
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_START;
        send_byte_buffer[ind++] = 0x00; //filler

        //Transmit byte: Device Addr + write
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_TX_BYTE;
        send_byte_buffer[ind++] = (FIFOI2C2_Devices_List[device].address << 1) |
                0x00; //LSB is 0 to inicate a write

        //Transmit byte: The start register address
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_TX_BYTE;
        send_byte_buffer[ind++] = start_register;

        //If their are more than 1 'write' requests to send then this routine runs.
        //Basically it continues to add write requests.
        while (i < (buffer_length - 1))
        {
            //Transmit byte: Send value to write to register
            state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_TX_BYTE;
            send_byte_buffer[ind++] = byte_buffer[i];

            i++;
        }

        //Transmit byte: Send value to write to register
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_TX_BYTE;
        send_byte_buffer[ind++] = byte_buffer[i];


        //Master sends STOP
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_STOP;
        send_byte_buffer[ind++] = 0x00; //filler


        //Required for I2C IRQ to know when to stop
        state_buffer[ind] = FIFOI2C2_DEVICE_COMMAND_CMDEND;
        send_byte_buffer[ind++] = 0x00; //filler


        //Adds these commands to the device's TX buffer and starts the IRQ
        FIFOI2C2_addQueue(device, send_byte_buffer, state_buffer, ind);

        return 0;
    }
    else
    {
        //If their aren't any write requests. Return -1.
        return -1;
    }
}



//******************************************************************************
//Interrupt Request Routines
//******************************************************************************
void __ISR(_I2C_2_VECTOR, IPL5AUTO) __I2C2Interrupt(void)
{

    int i = 0;
    int ind = 0, ind2 = 0;
    FIFOI2C2_RX_Byte rxb;

    if (INTGetFlag(INT_I2C2B)) //Bus Collision
    {
        INTClearFlag(INT_I2C2B);

        //TODO: Set an error, or reset the transmission.
    }
    if (INTGetFlag(INT_I2C2M)) //Master Intterupt
    {
        INTClearFlag(INT_I2C2M);

        //If we have received a byte, read it.
        if (FIFOI2C2_nextInterruptReceive == 1)
        {
            //Setup indexes (easier to read)
            ind = FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer_current;
            ind2 = FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].receive_buffer_length;

            //Read the received byte and the current command we are on.
            rxb.device_command = FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer[ind].device_command;
            //read the transmitted byte
            rxb.rx_byte = I2C2RCV;

            //Write rxb to the receive buffer
            FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].receive_buffer[ind2] = rxb;

            //increment indexes
            FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].receive_buffer_length++;
            FIFOI2C2_nextInterruptReceive = 0;
        }

        //Switch statement for the current_device's transmit byte's device_state
        ind = FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer_current;
        switch(FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer[ind].device_command)
        {
            case FIFOI2C2_DEVICE_COMMAND_CMDERROR:
                ind = 0; //filler
                break;
            case FIFOI2C2_DEVICE_COMMAND_START:
                I2C2CONbits.SEN = 1;    //start condition sequence
                break;
            case FIFOI2C2_DEVICE_COMMAND_TX_BYTE:
                I2C2TRN = FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer[ind].tx_byte; //transmit byte
                break;
            case FIFOI2C2_DEVICE_COMMAND_RESTART:
                I2C2CONbits.RSEN = 1;   //repeated start condition sequence
                break;
            case FIFOI2C2_DEVICE_COMMAND_RX_BYTE:
                I2C2CONbits.RCEN = 1;   //Receive byte sequence
                //Flag for next intr to be for receiving said byte
                FIFOI2C2_nextInterruptReceive = 1;
                break;
            case FIFOI2C2_DEVICE_COMMAND_ACK:
                I2C2CONbits.ACKDT = 0;  //ACK
                I2C2CONbits.ACKEN = 1;  //Send ACK sequence
                break;
            case FIFOI2C2_DEVICE_COMMAND_NACK:
                I2C2CONbits.ACKDT = 1;  //NACK
                I2C2CONbits.ACKEN = 1;  //Send NACK sequence
                break;
            case FIFOI2C2_DEVICE_COMMAND_STOP:
                I2C2CONbits.PEN = 1;  //Stop routine
                break;
            case FIFOI2C2_DEVICE_COMMAND_CMDEND:
                //Loop through all devices and check if they have something (or more somethings) to send.

                //Check Current Device first
                if ((FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer_current + 1) >=
                        FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer_length)
                {
                    //Turn off I2C IRq routine
                    FIFOI2C2_isRunning = 0;

                    //Reset transmit indexes
                    FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer_current = 0;
                    FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer_length = 0;

                    //Check other devices now
                    while (i < FIFOI2C2_DEVICES_COUNT)
                    {
                        //If not this device (since we already checked it)
                        if (i != FIFOI2C2_currentDevice)
                        {
                            if(FIFOI2C2_Devices_List[i].transmit_buffer_length >
                                    FIFOI2C2_Devices_List[i].transmit_buffer_current) //If they have something to send
                            {
                                //Set device as the current device
                                FIFOI2C2_currentDevice = i;
                                //Turn back on FIFOI2C2
                                FIFOI2C2_isRunning = 1;
                                //Kick-Start Operation
                                INTSetFlag(INT_I2C2M);
                            }
                        }
                        i++;
                    }

                    //Done so quit routine.
                    return;
                }
                else
                {
                    //Keep it running
                    INTSetFlag(INT_I2C2M);
                }

                break;

            default:
                break;
        }

        //Increment next cmd to tx
        FIFOI2C2_Devices_List[FIFOI2C2_currentDevice].transmit_buffer_current++;
    }

}

