
#include "SerialComm.h"

#define UART_MODULE_ID UART1 // PIM is connected to WF32 through UART1 module


void SerialComm_start()
{
    uint8 byte1 = 0;
    uint8 byte2 = 0;
    uint8 byte3 = 0;

    FIFOUART_initialize();

    while(1)
    {
        while(!UARTReceivedDataIsAvailable(UART_MODULE_ID))
                ;//Waiting for data on the line.
        

        byte1 = UARTGetDataByte(UART_MODULE_ID);

        if (byte1 == SERIALCOMM_START_TOKEN)
        {
            while(!UARTReceivedDataIsAvailable(UART_MODULE_ID))
                ;//Waiting for next data piece
            byte2 = UARTGetDataByte(UART_MODULE_ID);
            switch (byte2)
            {
                case SERIALCOMM_READ_REGISTER:
                    SerialComm_readRegistersRoutine();
                    break;
                case SERIALCOMM_WRITE_REGISTER:
                    SerialComm_writeRegistersRoutine();
                    break;
                default:
                    break;
            }
        }
        else
        {
            //ignore
        }
    }
}


void SerialComm_readRegistersRoutine()
{
    uint8 register_address = 0;
    char buffer[32];
    int len = 0;

    while (1)
    {
        while(!UARTReceivedDataIsAvailable(UART_MODULE_ID));//Waiting for next data piece
        register_address = UARTGetDataByte(UART_MODULE_ID);

        switch(register_address)
        {
            case SERIALCOMM_END_TOKEN:
                return;

            //Accelerometer
            case SERIALCOMM_REGISTER_XAcceleration:
                len = sprintf (buffer, "%f\n", ADXL362_XAcceleration);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_YAcceleration:
                len = sprintf (buffer, "%f\n", ADXL362_YAcceleration);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_ZAcceleration:
                len = sprintf (buffer, "%f\n", ADXL362_ZAcceleration);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_AcceloTemperature:
                len = sprintf (buffer, "%f\n", ADXL362_Temperature);
                FIFOUART_sendBuffer(buffer, len);
                break;

            //Gyroscope
            case SERIALCOMM_REGISTER_XAngularRate:
                len = sprintf (buffer, "%f\n", L3G4200D_XAngularRate);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_YAngularRate:
                len = sprintf (buffer, "%f\n", L3G4200D_YAngularRate);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_ZAngularRate:
                len = sprintf (buffer, "%f\n", L3G4200D_ZAngularRate);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_GyroTemperature:
                len = sprintf (buffer, "%f\n", L3G4200D_Temperature);
                FIFOUART_sendBuffer(buffer, len);
                break;

            //3-axis Compass
            case SERIALCOMM_REGISTER_XMagneticVector:
                len = sprintf (buffer, "%f", HMC5883L_XMagneticVector);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_YMagneticVector:
                len = sprintf (buffer, "%f", HMC5883L_YMagneticVector);
                FIFOUART_sendBuffer(buffer, len);
                break;
            case SERIALCOMM_REGISTER_ZMagneticVector:
                len = sprintf (buffer, "%f", HMC5883L_ZMagneticVector);
                FIFOUART_sendBuffer(buffer, len);
                break;
        }
    }


}


void SerialComm_writeRegistersRoutine()
{
    //Not implemented yet
}
