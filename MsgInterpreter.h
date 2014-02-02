/**
 * @file: MsgInterpreter
 * @brief: Interprets messages
 *
 * @author: Connor Martin
 * @date: Jan 31, 2014
 *
 * @preconditions:
 * @device:
 *      -PIC32MX695F512L
 *
 * @remarks:
 *
 */

#ifndef MSGINTERPRETER_H
#define	MSGINTERPRETER_H

#ifdef	__cplusplus
extern "C" {
#endif

    
    #include <plib.h>

    #include "Hardware.h"
    #include "ADXL362.h"
    #include "L3G4200D.h"
    #include "HMC5883L.h"
    #include "FIFOUART1.h"

    #define SERIALCOMM_START_TOKEN      0x01
    #define SERIALCOMM_READ_REGISTER    0x02
    #define SERIALCOMM_WRITE_REGISTER   0x03

    #define SERIALCOMM_MOTOR1_ON        0x10
    #define SERIALCOMM_END_TOKEN        0xFF


    #define SERIALCOMM_REGISTER_XAcceleration       0x10
    #define SERIALCOMM_REGISTER_YAcceleration       0x11
    #define SERIALCOMM_REGISTER_ZAcceleration       0x12
    #define SERIALCOMM_REGISTER_AcceloTemperature   0x13

    #define SERIALCOMM_REGISTER_XAngularRate        0x20
    #define SERIALCOMM_REGISTER_YAngularRate        0x21
    #define SERIALCOMM_REGISTER_ZAngularRate        0x22
    #define SERIALCOMM_REGISTER_GyroTemperature     0x23

    #define SERIALCOMM_REGISTER_XMagneticVector     0x30
    #define SERIALCOMM_REGISTER_YMagneticVector     0x31
    #define SERIALCOMM_REGISTER_ZMagneticVector     0x32

    
    int MsgInterpreter_interpret(uint8 message[], uint16 length);



#ifdef	__cplusplus
}
#endif

#endif	/* MSGINTERPRETER_H */

