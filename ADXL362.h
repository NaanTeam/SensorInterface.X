/* 
 * File:   ADXL362.h
 * Author: Connor
 *
 * Created on October 18, 2013, 4:51 PM
 */

#ifndef ADXL362_H
#define	ADXL362_H

#ifdef	__cplusplus
extern "C" {
#endif

    void ADXL362_StartMeasurements();
    void ADXL362_QueueReadXYZT();
    void ADXL362_InterpretXYZT();

    extern short ADXL362_XAcceleration;
    extern short ADXL362_YAcceleration;
    extern short ADXL362_ZAcceleration;
    extern short ADXL362_Temperature;

#ifdef	__cplusplus
}
#endif

#endif	/* ADXL362_H */

