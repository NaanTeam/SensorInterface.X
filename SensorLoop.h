/**
 * @Author: Connor Martin
 * @Description: A interrupt driven timer loop that queues and interprets
 * data from the different sensors.
 * @Requirements: FIFOSPI.c, ADXL362.c, L3G4200D.c
 * @Devices: PIC32MX320F128H
 */

#ifndef SensorLoop_H
#define	SensorLoop_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include <plib.h>

    #include "FIFOSPI2.h"
    #include "FIFOI2C2.h"

    #include "ADXL362.h"
    #include "L3G4200D.h"
    #include "HMC5883L.h"

    /**
     * Sets up the IRQ overflow for Timer1 to run at div 8 and a counter of 0xFFFF.
     */
    void SensorLoop_SetupAll();



#ifdef	__cplusplus
}
#endif

#endif	/* TIMER1_LOOP_H */

