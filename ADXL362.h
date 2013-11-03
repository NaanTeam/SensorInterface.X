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

    #include "FIFOSPI.h"

    extern short ADXL362_XAcceleration;
    extern short ADXL362_YAcceleration;
    extern short ADXL362_ZAcceleration;
    extern short ADXL362_Temperature;

    void ADXL362_StartMeasurements();

    void ADXL362_QueueReadXYZT();
    void ADXL362_InterpretXYZT();

    void ADXL362_QueueReadMSBX();
    void ADXL362_InterpretMSBX();
    void ADXL362_QueueReadMSBY();
    void ADXL362_InterpretMSBY();
    void ADXL362_QueueReadMSBZ();
    void ADXL362_InterpretMSBZ();




#ifdef	__cplusplus
}
#endif

#endif	/* ADXL362_H */

