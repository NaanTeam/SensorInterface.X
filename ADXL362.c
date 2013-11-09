/**
 * @Author: Connor Martin
 * @Description: A set of function for interfacing with the ADXL362 Accelerometer.
 * @Requirements: FIFOSPI.c
 * @Devices: PIC32MX320F128H
 */

#include "ADXL362.h"

short ADXL362_XAcceleration = 0;
short ADXL362_YAcceleration = 0;
short ADXL362_ZAcceleration = 0;
short ADXL362_Temperature = 0;

void ADXL362_StartMeasurements()
{
    //Reset the device
    
    char func_rslt, read_rslt1;
    char reset[3];
    reset[0] = ADXL362_REG_WRITE;
    reset[1] = ADXL362_SOFT_RESET;
    reset[2] = ADXL362_SOFT_RESET_KEY; //Represents the letter r
    FIFOSPI2_SendQueue(reset, 3, 1);
    //Wait for the device to reset
    int i = 0;
    while (i < (1000))
    {
        i++;
    }
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);


    //Turns on Measurement Mode
    char measurementMode[3];
    measurementMode[0] = ADXL362_REG_WRITE;
    measurementMode[1] = ADXL362_POWER_CTL;
    measurementMode[2] = 0x00 |
            ADXL362_MEASURE_3D; //Turn on Measurement mode
    FIFOSPI2_SendQueue(measurementMode, 3, 1);


    //Immediately Follow it with a read X register
    char readReg[3];
    readReg[0] = ADXL362_REG_READ; //Read
    readReg[1] = ADXL362_XDATA8; //read X-reg
    readReg[2] = 0x00; //fluff for the read
    FIFOSPI2_SendQueue(measurementMode, 3, 1);


    //Wait for all data to be sent
    while (FIFOSPI2_RecieveBufferIndex() < 6) {}
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1);
    //k = FIFOSPI2_ReadQueue(&j);
}

void ADXL362_QueueReadXYZT()
{
    //Burst read
    char read[10];
    read[0] = ADXL362_REG_READ; //Read
    read[1] = ADXL362_XDATAL; //read X-reg
    read[2] = 0x00; //ADXL362_XDATAL
    read[3] = 0x00; //ADXL362_XDATAH
    read[4] = 0x00; //ADXL362_YDATAL
    read[5] = 0x00; //ADXL362_YDATAH
    read[6] = 0x00; //ADXL362_ZDATAL
    read[7] = 0x00; //ADXL362_ZDATAH
    read[8] = 0x00; //ADXL362_TEMPL
    read[9] = 0x00; //ADXL362_TEMPH
    FIFOSPI2_SendQueue(read, 10, 1);
}
void ADXL362_InterpretXYZT()
{
    char func_rslt, read_rslt1, read_rslt2;


    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //fluff

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //ADXL362_XDATAL
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //ADXL362_XDATAH
    ADXL362_XAcceleration = (read_rslt2 << 8) + read_rslt1;

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //ADXL362_YDATAL
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //ADXL362_YDATAH
    ADXL362_YAcceleration = (read_rslt2 << 8) + read_rslt1;

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //ADXL362_ZDATAL
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //ADXL362_ZDATAH
    ADXL362_ZAcceleration = (read_rslt2 << 8) + read_rslt1;

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //ADXL362_TEMPL
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //ADXL362_TEMPH
    ADXL362_Temperature = (read_rslt2 << 8) + read_rslt1;

    //TODO: twos complement acceleration or temperature data.

}

void ADXL362_QueueReadMSBX()
{
    char read[3];
    read[0] = ADXL362_REG_READ; //Read
    read[1] = ADXL362_XDATA8; //read X-reg
    read[2] = 0x00; //fluff for read (8 MSB of X accel reg)
    FIFOSPI2_SendQueue(read, 3, 1);
}
void ADXL362_InterpretMSBX()
{
    char func_rslt, read_rslt1, read_rslt2;

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //fluff

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //ADXL362_XDATA8 (8 MSB)
    ADXL362_XAcceleration = read_rslt1 ;

}

void ADXL362_QueueReadMSBY()
{
    char read[3];
    read[0] = ADXL362_REG_READ; //Read
    read[1] = ADXL362_YDATA8; //read Y-reg
    read[2] = 0x00; //fluff
    FIFOSPI2_SendQueue(read, 3, 1);
}
void ADXL362_InterpretMSBY()
{
    char func_rslt, read_rslt1, read_rslt2;

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //fluff

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //ADXL362_YDATA8 (Y 8-MSB)
    ADXL362_YAcceleration = read_rslt1 ;

}

void ADXL362_QueueReadMSBZ()
{
    char read[3];
    read[0] = ADXL362_REG_READ; //Read
    read[1] = ADXL362_ZDATA8; //read Z-reg
    read[2] = 0x00; //fluff
    FIFOSPI2_SendQueue(read, 3, 1);
}
void ADXL362_InterpretMSBZ()
{
    char func_rslt, read_rslt1, read_rslt2;
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //fluff

    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //ADXL362_ZDATA8 (Z 8-MSB)
    ADXL362_ZAcceleration = read_rslt1 ;

}

