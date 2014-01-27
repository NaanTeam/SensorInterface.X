/*
 * File:   HMC5883L.c
 * Author: Connor
 *
 * Created on November 10, 2013, 11:27 PM
 */

#include "HMC5883L.h"


double HMC5883L_XMagneticVector = 0;
double HMC5883L_ZMagneticVector = 0;
double HMC5883L_YMagneticVector = 0;


void HMC5883L_startMeasurements()
{
    int i = 0;
    uint8 buffer[10];

    //Config Register A
    buffer[0] = 0x74; //111 0100  //30hz refresh rate

    //Config Register B
    buffer[1] = 0xA0;

    //Mode Register
    buffer[2] = 0x00; //cont meaurement mode

    FIFOI2C2_addQueue_writeDeviceRegisters(0, 0x00, buffer, 3);


//    //Give it time to start up
//    while (i < 1000000)
//    {
//        i++;
//    }

}


void HMC5883L_queueReadXZY()
{
    FIFOI2C2_addQueue_readDeviceRegisters(0, 0x03, 6);
}


void HMC5883L_interpretXZY()
{
    uint8 x_msb = 0, x_lsb = 0;
    uint8 z_msb = 0, z_lsb = 0;
    uint8 y_msb = 0, y_lsb = 0;
    short x_tmp = 0, z_tmp = 0, y_tmp= 0;
    double gauss_per_LSB = 0;

    x_msb = FIFOI2C2_readQueue(0).rx_byte;
    x_lsb = FIFOI2C2_readQueue(0).rx_byte;

    z_msb = FIFOI2C2_readQueue(0).rx_byte;
    z_lsb = FIFOI2C2_readQueue(0).rx_byte;

    y_msb = FIFOI2C2_readQueue(0).rx_byte;
    y_lsb = FIFOI2C2_readQueue(0).rx_byte;

    x_tmp = (x_msb << 8) | x_lsb;
    z_tmp = (z_msb << 8) | z_lsb;
    y_tmp = (y_msb << 8) | y_lsb;

    //Based on devie 'Mode Register.' Refer to HMC5883L data sheet.
    gauss_per_LSB = 0.92e-3;

    HMC5883L_XMagneticVector = gauss_per_LSB * (double)x_tmp;
    HMC5883L_ZMagneticVector = gauss_per_LSB * (double)z_tmp;
    HMC5883L_YMagneticVector = gauss_per_LSB * (double)y_tmp;

}

