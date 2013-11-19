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
#include "FIFOSPI2.h"
#include "Hardware.h"


// <editor-fold defaultstate="collapsed" desc="Config Bits">
/*CP: Code-Protect bit
 * Prevents boot and program Flash memory from being read or modified by an external programming device.
 *      1 = Protection is disabled
 *      0 = Protection is enabled*/
#pragma config CP       = OFF
/*BWP: Boot Flash Write-Protect bit
 * Prevents boot Flash memory from being modified during code execution.
 *      1 = Boot Flash is writable
 *      0 = Boot Flash is not writable*/
#pragma config BWP      = OFF
/*PWP<7:0>: Program Flash Write-Protect bits
 * Prevents selected program Flash memory pages from being modified during code execution. The PWP bits
 *  represent the 1?s complement of the number of write-protected program Flash memory pages.
 *      11111111 = Disabled
 *      11111110 = 0xBD00_0FFF
 *      11111101 = 0xBD00_1FFF
 *      ...*/
#pragma config PWP      = OFF
/*ICESEL: In-Circuit Emulator/Debugger Communication Channel Select bit
 *      1 = PGEC2/PGED2 pair is used
 *      0 = PGEC1/PGED1 pair is used*/
#pragma config ICESEL   = ICS_PGx2
/*DEBUG<1:0>: Background Debugger Enable bits (forced to ?11? if code-protect is enabled)
 *      11 = Debugger is disabled
 *      10 = Debugger is enabled
 *      01 = Reserved (same as ?11? setting)
 *      00 = Reserved (same as ?11? setting)*/
#pragma config DEBUG    = OFF
/*FWDTEN: Watchdog Timer Enable bit
 *      1 = The WDT is enabled and cannot be disabled by software
 *      0 = The WDT is not enabled; it can be enabled in software*/
#pragma config FWDTEN   = OFF           // Watchdog Timer
/*WDTPS<4:0>: Watchdog Timer Postscale Select bits
 *      10100 = 1:1048576
 *      10011 = 1:524288
 *      10010 = 1:262144
 *      ...
 *      00001 = 1:2
 *      00000 = 1:1*/
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
/*FCKSM<1:0>: Clock Switching and Monitor Selection Configuration bits
 *      1x = Clock switching is disabled, Fail-Safe Clock Monitor is disabled
 *      01 = Clock switching is enabled, Fail-Safe Clock Monitor is disabled
 *      00 = Clock switching is enabled, Fail-Safe Clock Monitor is enabled*/
#pragma config FCKSM    = CSDCMD
/*OSCIOFNC: CLKO Enable Configuration bit
 *      1 = CLKO output is disabled
 *      0 = CLKO output signal is active on the OSCO pin; the Primary Oscillator must be disabled or configured
 *          for External Clock mode (EC) for the CLKO to be active (POSCMOD<1:0> = 11 or 00)*/
#pragma config OSCIOFNC = OFF
/*POSCMOD<1:0>: Primary Oscillator Configuration bits
 *      11 = Primary Oscillator disabled
 *      10 = HS Oscillator mode selected
 *      01 = XT Oscillator mode selected
 *      00 = External Clock mode selected*/
#pragma config POSCMOD  = HS
/*IESO: Internal External Switchover bit
 *      1 = Internal External Switchover mode is enabled (Two-Speed Start-up is enabled)
 *      0 = Internal External Switchover mode is disabled (Two-Speed Start-up is disabled)*/
#pragma config IESO     = OFF
/*FSOSCEN: Secondary Oscillator Enable bit
 *      1 = Enable the Secondary Oscillator
 *      0 = Disable the Secondary Oscillator*/
#pragma config FSOSCEN  = OFF
/*FNOSC<2:0>: Oscillator Selection bits
 *      111 = Fast RC Oscillator with divide-by-N (FRCDIV)
 *      110 = FRCDIV16 Fast RC Oscillator with fixed divide-by-16 postscaler
 *      101 = Low-Power RC Oscillator (LPRC)
 *      100 = Secondary Oscillator (SOSC)
 *      011 = Primary Oscillator (POSC) with PLL module (XT+PLL, HS+PLL, EC+PLL)
 *      010 = Primary Oscillator (XT, HS, EC)(1)
 *      001 = Fast RC Oscillator with divide-by-N with PLL module (FRCDIV+PLL)
 *      000 = Fast RC Oscillator (FRC)*/
#pragma config FNOSC    = PRIPLL
/*FPLLMUL<2:0>: PLL Multiplier bits
 *      111 = 24x multiplier
 *      110 = 21x multiplier
 *      101 = 20x multiplier
 *      100 = 19x multiplier
 *      011 = 18x multiplier
 *      010 = 17x multiplier
 *      001 = 16x multiplier
 *      000 = 15x multiplier*/
#pragma config FPLLMUL  = MUL_20
/*FPLLIDIV<2:0>: PLL Input Divider bits
 *      111 = 12x divider
 *      110 = 10x divider
 *      101 = 6x divider
 *      100 = 5x divider
 *      011 = 4x divider
 *      010 = 3x divider
 *      001 = 2x divider
 *      000 = 1x divider*/
#pragma config FPLLIDIV = DIV_2
/*FPLLODIV<2:0>: PLL Output Divider bits
 *      111 = PLL output divided by 256
 *      110 = PLL output divided by 64
 *      101 = PLL output divided by 32
 *      100 = PLL output divided by 16
 *      011 = PLL output divided by 8
 *      010 = PLL output divided by 4
 *      001 = PLL output divided by 2
 *      000 = PLL output divided by 1*/
#pragma config FPLLODIV = DIV_1
/*FPBDIV<1:0>: Peripheral Bus Clock Divisor Default Value bits
 *      11 = PBCLK is SYSCLK divided by 8
 *      10 = PBCLK is SYSCLK divided by 4
 *      01 = PBCLK is SYSCLK divided by 2
 *      00 = PBCLK is SYSCLK divided by 1*/
#pragma config FPBDIV   = DIV_8
// </editor-fold>

#define UART_MODULE_ID UART1 // PIM is connected to WF32 through UART1 module

    
void UART_setup(void){
    int b = 0;
    UARTConfigure(UART_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    //UARTSetFifoMode(UART_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    b = UARTSetDataRate(UART_MODULE_ID, GetPeripheralClock(), 9600);
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
//    char buffer[32];
//    char b = 'g';
//    int a3=5, b3=3;
//    b = 'k';
//    double fg = 9.6654346;
//    //sprintf (buffer, "%d plus %d is %d", a3, b3, a3+b3);
//    sprintf (buffer, "%f", fg);
    UART_setup();

    SensorLoop_SetupAll();

    SerialComm_start();

//    while(1)
//    {
//
//        DelayTime(1000);
//        char buff[50];
//
//        sprintf(buff, "-------------------------------------------------------\n\r");
//        putsUART1(buff);
//
//        sprintf(buff, " X Accel: %f \n\r Y_Accel: %f \n\r Z_Accel: %f \n\r Temp: %f \n\r\n\r",
//                ADXL362_XAcceleration, ADXL362_YAcceleration, ADXL362_ZAcceleration, ADXL362_Temperature);
//        putsUART1(buff);
//
//
//        sprintf(buff, " X Gyro: %f \n\r Y Gyro: %f \n\r Z Gyro:: %f \n\r Temp: %f \n\r\n\r",
//                L3G4200D_XAngularRate, L3G4200D_YAngularRate, L3G4200D_ZAngularRate, L3G4200D_Temperature);
//        putsUART1(buff);
//
//
//        sprintf(buff, " X Comp: %f \n\r Y Comp: %f \n\r Z Comp: %f \n\r\n\r",
//                HMC5883L_XMagneticVector, HMC5883L_ZMagneticVector, HMC5883L_YMagneticVector);
//        putsUART1(buff);
//
//    }

    return (EXIT_SUCCESS);
}

