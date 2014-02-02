
#include "MsgInterpreter.h"

//******************************************************************************
// Local Variables and Typedefs
//******************************************************************************
uint16 MsgInterpreter_Index = 0;
uint16 MsgInterpreter_Length = 0;

//******************************************************************************
//Local Function Declarations
//******************************************************************************
inline int MsgInterpreter_interpret_readRegisters(uint8 message[]);
inline int MsgInterpreter_interpret_writeRegisters(uint8 message[]);


//******************************************************************************
//Local Function Definitions
//******************************************************************************

inline int MsgInterpreter_interpret_readRegisters(uint8 message[])
{
    uint8* temp;
    uint8 buffer[sizeof(float)*256];
    int buffer_len = 0;

    while(MsgInterpreter_Index < MsgInterpreter_Length)
    {
        switch(message[MsgInterpreter_Index++])
        {
            default:
                return;
            case SERIALCOMM_END_TOKEN:
                return;

            //Accelerometer
            case SERIALCOMM_REGISTER_XAcceleration:
//                buffer_len = sprintf (buffer, "%f", ADXL362_XAcceleration);
//                FIFOUART1_pushTxQueue(buffer, buffer_len);
                temp = (uint8*)(&ADXL362_XAcceleration);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YAcceleration:
                temp = (uint8*)(&ADXL362_YAcceleration);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZAcceleration:
                temp = (uint8*)(&ADXL362_ZAcceleration);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_AcceloTemperature:
                temp = (uint8*)(&ADXL362_Temperature);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

            //Gyroscope
            case SERIALCOMM_REGISTER_XAngularRate:
                temp = (uint8*)(&L3G4200D_XAngularRate);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YAngularRate:
                temp = (uint8*)(&L3G4200D_YAngularRate);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZAngularRate:
                temp = (uint8*)(&L3G4200D_ZAngularRate);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_GyroTemperature:
                temp = (uint8*)(&L3G4200D_Temperature);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;

            //3-axis Compass
            case SERIALCOMM_REGISTER_XMagneticVector:
                temp = (uint8*)(&HMC5883L_XMagneticVector);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_YMagneticVector:
                temp = (uint8*)(&HMC5883L_YMagneticVector);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
            case SERIALCOMM_REGISTER_ZMagneticVector:
                temp = (uint8*)(&HMC5883L_ZMagneticVector);
                buffer[buffer_len++] = temp[0];
                buffer[buffer_len++] = temp[1];
                buffer[buffer_len++] = temp[2];
                buffer[buffer_len++] = temp[3];
                break;
        }
    }
    //buffer[buffer_len++] = '\n';
    FIFOUART1_pushTxQueue(buffer, buffer_len);
    return 0;
}

inline int MsgInterpreter_interpret_writeRegisters(uint8 message[])
{
    return 0;
}


//******************************************************************************
//Public Function Definitions
//******************************************************************************

int MsgInterpreter_interpret(uint8 message[], uint16 length)
{
    MsgInterpreter_Index = 0;
    MsgInterpreter_Length = length;
    
    switch (message[MsgInterpreter_Index++])
    {
        case SERIALCOMM_READ_REGISTER:
            MsgInterpreter_interpret_readRegisters(message);
            break;
        case SERIALCOMM_WRITE_REGISTER:
            MsgInterpreter_interpret_writeRegisters(message);
            break;
        default:
            break;
    }

    return 0;
}
