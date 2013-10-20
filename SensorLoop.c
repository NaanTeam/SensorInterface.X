/**
 * This file sets up timer 1 to be used to update the sensor data.
 *
 */

#include <plib.h>

#include "FIFOSPI.h"
#include "SensorLoop.h"
#include "ADXL362.h"






void SensorLoop_SetupAll()
{
    //Enable toggle LED directional register
    TRISFbits.TRISF0 = 0;

    //Setup SPI2
    FIFOSPI2_Setup();

    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    ADXL362_StartMeasurements();
    INTDisableInterrupts();

    //Setup Timer1
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);
    // Enable multi-vector interrupts

    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_64, 0x1FFF);

    

    //Turn on Interrupts
    INTEnableInterrupts();
}


//TODO Make a "Start Reading" Function

//IRQ for timer1. Used for polling sensors.
void __ISR(_TIMER_1_VECTOR, IPL3AUTO) Timer1Handler(void)
{
    //char j, k;
    ADXL362_QueueReadXYZT();
    ADXL362_InterpretXYZT(); //Interpret the previous one.
    
//    char read[3];
//    read[0] = 0x0B; //Read
//    read[1] = 0x08; //read X-reg
//    read[2] = 0x00; //X1
//    FIFOSPI2_SendQueue(read, 3);
//    int a;
//    a = FIFOSPI2_RecieveBufferIndex();
//    k = FIFOSPI2_ReadQueue(&j);
//    k = FIFOSPI2_ReadQueue(&j);
//    k = FIFOSPI2_ReadQueue(&j);

    //PORTFbits.RF0 = PORTFbits.RF0 ^ 1; //toggle LED
    INTClearFlag(INT_T1);// Be sure to clear the Timer1 interrupt status
}
