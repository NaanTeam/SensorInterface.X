/* 
 * File:   main.c
 * Author: Connor
 *
 * Created on October 15, 2013, 11:44 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <plib.h>
#include <peripheral/ports.h>

#include "SensorTimerLoop.h"
#include "ADXL362.h"
#include "L3G4200D.h"
#include "FIFOSPI.h"



// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 10 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ (80000000L)

#define UART_MODULE_ID UART1 // PIM is connected to WF32 through UART1 module
#define	GetPeripheralClock()		(SYS_FREQ/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()		(SYS_FREQ)

void UART_setup(void){
    UARTConfigure(UART_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_MODULE_ID, GetPeripheralClock(), 9600);
    UARTEnable(UART_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}


void DelayTime(int ms)
{
    int i = 0;
    while (i < (ms*1000))
    {
        i++;
    }
}





int main(int argc, char** argv)
{
    UART_setup();
    SensorLoop_SetupAll();

    
    while(1)
    {

//        char j, k;
//            //Read the X Acceleration
//        char ReadReg[10];
//        ReadReg[0] = 0x0B;
//        ReadReg[1] = 0x08;
//        ReadReg[2] = 0x00; //X
//        FIFOSPI2_SendQueue(ReadReg, 3);
        
        DelayTime(1000);
        char buff[50];

        //itoa(buff, ADXL362_XAcceleration,10);
        //char *statement = "Hello World!\n\r";

        //High resolution: 1 mg/LSB
        double xa = ADXL362_XAcceleration * 0.001;
        double xy = ADXL362_YAcceleration * 0.001;
        double xz = ADXL362_ZAcceleration * 0.001;

        sprintf(buff, " X Accel: %d \n\r Y_Accel: %d \n\r Z_Accel: %d \n\r Temp: %d \n\r\n\r", ADXL362_XAcceleration,
                ADXL362_YAcceleration, ADXL362_ZAcceleration, ADXL362_Temperature); //Increases used flash by 5%
        sprintf(buff, " X Accel: %f \n\r Y_Accel: %f \n\r Z_Accel: %f \n\r Temp: %d \n\r\n\r", xa,
                xy, xz, ADXL362_Temperature); //Increases used flash by 5%
        putsUART1(buff);
        
//        double xa = ADXL362_XAcceleration * 0.001;
//        double xy = ADXL362_YAcceleration * 0.001;
//        double xz = ADXL362_ZAcceleration * 0.001;

        sprintf(buff, " X Gyro: %d \n\r Y Gyro: %d \n\r Z Gyro:: %d \n\r Temp: %d \n\r\n\r",
                L3G4200D_XAxis, L3G4200D_YAxis, L3G4200D_ZAxis, L3G4200D_Temperature); //Increases used flash by 5%
        putsUART1(buff);

    }
    return (EXIT_SUCCESS);
}

