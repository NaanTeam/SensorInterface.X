//Accelerometer

#include "ADXL362.h"

short ADXL362_XAcceleration = 0;
short ADXL362_YAcceleration = 0;
short ADXL362_ZAcceleration = 0;
short ADXL362_Temperature = 0;

void ADXL362_StartMeasurements()
{
    //Reset the device
    char k, j;
    char reset[3];
    reset[0] = 0x0A;
    reset[1] = 0x1F;
    reset[2] = 0x52;

    FIFOSPI2_SendQueue(reset, 3, 1);
    //Wait for all data to be sent
    int i = 0;
    while (i < (1*1000))
    {
        i++;
    }

    //while (FIFOSPI2_RecieveBufferIndex() < 3) {}
    k = FIFOSPI2_ReadQueue(&j);
    k = FIFOSPI2_ReadQueue(&j);
    k = FIFOSPI2_ReadQueue(&j);


    //Turns on Measurement Mode
    char measurementMode[3];
    measurementMode[0] = 0x0A;
    measurementMode[1] = 0x2D;
    measurementMode[2] = 0x02; //Turn on Measurement mode
    FIFOSPI2_SendQueue(measurementMode, 3, 1);
    char readReg[4];
//    measurementMode[3] = 0x0B;
//    measurementMode[4] = 0x08;
//    measurementMode[5] = 0x00; //Follow it with a read X register
    readReg[0] = 0x0B; //Read
    readReg[1] = 0x08; //read X-reg
    readReg[2] = 0x00; //X1
    //readReg[3] = 0x00; //X2
    FIFOSPI2_SendQueue(measurementMode, 3, 1);


    //Wait for all data to be sent
    while (FIFOSPI2_RecieveBufferIndex() < 6) {}
    k = FIFOSPI2_ReadQueue(&j);
    k = FIFOSPI2_ReadQueue(&j);
    k = FIFOSPI2_ReadQueue(&j);
    k = FIFOSPI2_ReadQueue(&j);
    k = FIFOSPI2_ReadQueue(&j);
    k = FIFOSPI2_ReadQueue(&j);
    //k = FIFOSPI2_ReadQueue(&j);
}

void ADXL362_QueueReadXYZT()
{
    char k, j;
    char read[11];
    read[0] = 0x0B; //Read
    read[1] = 0x0E; //read X-reg
    read[2] = 0x00; //X1
    read[3] = 0x00; //X2
    read[4] = 0x00; //Y1
    read[5] = 0x00; //Y2
    read[6] = 0x00; //Z1
    read[7] = 0x00; //Z2
    read[8] = 0x00; //Temperature 1
    read[9] = 0x00; //Temperature 2
    //read[10] = 0x00; //Extra read needed for some reason...
    FIFOSPI2_SendQueue(read, 10, 1);
}
void ADXL362_InterpretXYZT()
{
    char j1, j2, k;


    //while (FIFOSPI2_RecieveBufferIndex() < 11) {}

    k = FIFOSPI2_ReadQueue(&j1); //fluff
    k = FIFOSPI2_ReadQueue(&j2); //fluff

    k = FIFOSPI2_ReadQueue(&j1); //X low
    k = FIFOSPI2_ReadQueue(&j2); //X high
    ADXL362_XAcceleration = ((j2 & 0xFF) << 8) + j1;

    k = FIFOSPI2_ReadQueue(&j1); //Y low
    k = FIFOSPI2_ReadQueue(&j2); //Y hi
    ADXL362_YAcceleration = (j2 << 8) + j1;

    k = FIFOSPI2_ReadQueue(&j1); //Z low
    k = FIFOSPI2_ReadQueue(&j2); //Z hi
    ADXL362_ZAcceleration = (j2 << 8) + j1;

    k = FIFOSPI2_ReadQueue(&j1); //Temp lo
    k = FIFOSPI2_ReadQueue(&j2); //Temp hi
    ADXL362_Temperature = (j2 << 8) + j1;

    //k = FIFOSPI2_ReadQueue(&j1); //Fluff
    //TODO: twos complement acceleration or temperature data.

}

void ADXL362_QueueReadMSBX()
{
    char k, j;
    char read[3];
    read[0] = 0x0B; //Read
    read[1] = 0x08; //read X-reg
    read[2] = 0x00; //8 MSB X
    FIFOSPI2_SendQueue(read, 3, 1);
}
void ADXL362_InterpretMSBX()

{
    char j1, j2, k;


    //while (FIFOSPI2_RecieveBufferIndex() < 11) {}

    k = FIFOSPI2_ReadQueue(&j1); //fluff
    k = FIFOSPI2_ReadQueue(&j2); //fluff

    k = FIFOSPI2_ReadQueue(&j1); //X 8 MSB
    ADXL362_XAcceleration = j1 ;

}
void ADXL362_QueueReadMSBY()
{
    char k, j;
    char read[3];
    read[0] = 0x0B; //Read
    read[1] = 0x09; //read X-reg
    read[2] = 0x00; //8 MSB X
    FIFOSPI2_SendQueue(read, 3, 1);
}
void ADXL362_InterpretMSBY()
{
    char j1, j2, k;


    //while (FIFOSPI2_RecieveBufferIndex() < 11) {}

    k = FIFOSPI2_ReadQueue(&j1); //fluff
    k = FIFOSPI2_ReadQueue(&j2); //fluff

    k = FIFOSPI2_ReadQueue(&j1); //X 8 MSB
    ADXL362_YAcceleration = j1 ;

}

void ADXL362_QueueReadMSBZ()
{
    char k, j;
    char read[3];
    read[0] = 0x0B; //Read
    read[1] = 0x10; //read X-reg
    read[2] = 0x00; //8 MSB X
    FIFOSPI2_SendQueue(read, 3, 1);
}
void ADXL362_InterpretMSBZ()
{
    char j1, j2, k;


    //while (FIFOSPI2_RecieveBufferIndex() < 11) {}

    k = FIFOSPI2_ReadQueue(&j1); //fluff
    k = FIFOSPI2_ReadQueue(&j2); //fluff

    k = FIFOSPI2_ReadQueue(&j1); //X 8 MSB
    ADXL362_ZAcceleration = j1 ;

}

