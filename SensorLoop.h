/* 
 * File:   Timer1_Loop.h
 * Author: Connor
 *
 * Created on October 16, 2013, 2:01 PM
 */

#ifndef SensorLoop_H
#define	SensorLoop_H

#ifdef	__cplusplus
extern "C" {
#endif
    /**
     * Sets up the IRQ overflow for Timer1 to run at div 8 and a counter of 0xFFFF.
     */
    void SensorLoop_SetupAll();

    /**
     * Sets up ADXL362 to be prepared for reading. Requires FIFOSPi setup.
     */
    void SensorLoop_SetupADXL362();


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER1_LOOP_H */

