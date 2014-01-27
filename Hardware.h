/* 
 * File:   Hardware.h
 * Author: Connor
 *
 * Created on November 9, 2013, 9:39 PM
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #if defined (__32MX695F512L__) || (__32MX320F128H__)

        #define SYS_FREQ (80000000L)
        #define GetSystemClock()           (SYS_FREQ)
        #define GetPeripheralClock()       (SYS_FREQ/(1 << OSCCONbits.PBDIV))
        #define GetInstructionClock()      (SYS_FREQ)

    #endif
    
    
    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned int uint32;

   

#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_H */

