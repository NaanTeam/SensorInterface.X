/* 
 * File:   SerialComm.h
 * Author: Connor
 *
 * Created on November 13, 2013, 7:08 PM
 */

#ifndef SERIALCOMM_H
#define	SERIALCOMM_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <plib.h>

    #include "Hardware.h"
    #include "FIFOUART.h"
    #include "ADXL362.h"
    #include "L3G4200D.h"
    #include "HMC5883L.h"

    #define SERIALCOMM_START_TOKEN 0x01
    #define SERIALCOMM_READ_REGISTER 0x02
    #define SERIALCOMM_WRITE_REGISTER 0x03
    #define SERIALCOMM_END_TOKEN 0xFF


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



    void SerialComm_start();
    void SerialComm_readRegistersRoutine();
    void SerialComm_writeRegistersRoutine();
    

#ifdef	__cplusplus
}
#endif

#endif	/* SERIALCOMM_H */

