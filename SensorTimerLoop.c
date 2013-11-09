/**
 * @Author: Connor Martin
 * @Description: A interrupt driven timer loop that queues and interprets
 * data from the different sensors.
 * @Requirements: FIFOSPI.c, ADXL362.c, L3G4200D.c
 * @Devices: PIC32MX320F128H
 */

#include "SensorTimerLoop.h"


void SensorLoop_SetupAll()
{
    //Enable toggle LED directional register
    TRISFbits.TRISF0 = 0;
    // Enable multi-vector interrupts
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    //Setup Accelerometer
    FIFOSPI2_Setup();
    INTEnableInterrupts();
    ADXL362_StartMeasurements();
    

    //Setup Gyroscope
    L3G4200D_StartMeasurements();
    INTDisableInterrupts();


    //Setup Timer1
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);
    //Turn on clock
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_64, 0x4FFF);
    //Turn on Interrupts
    INTEnableInterrupts();
}


//TODO Make a "Start Reading" Function

//IRQ for timer1. Used for polling sensors.
void __ISR(_TIMER_1_VECTOR, IPL3AUTO) Timer1Handler(void)
{
    L3G4200D_InterpretXYZT();   
    ADXL362_InterpretXYZT(); //Interpret the previous one.

    L3G4200D_QueueReadXYZT();
    ADXL362_QueueReadXYZT();
    

////    ADXL362_QueueReadMSBX();
////    ADXL362_InterpretMSBX();
//////    ADXL362_QueueReadMSBY();
//////    ADXL362_InterpretMSBY();
//////    ADXL362_QueueReadMSBZ();
//////    ADXL362_InterpretMSBZ();
////
////    //PORTFbits.RF0 = PORTFbits.RF0 ^ 1; //toggle LED
    INTClearFlag(INT_T1);// Be sure to clear the Timer1 interrupt status
}
