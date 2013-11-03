//Gyroscope

#include "L3G4200D.h"

short L3G4200D_XAxis = 0;
short L3G4200D_YAxis = 0;
short L3G4200D_ZAxis = 0;
short L3G4200D_Temperature = 0;

int L3G4200D_StartMeasurements()
{
    //Check to see if we are communicating correctly.
    if (L3G4200D_ReadRegister_Blocking(L3G4200D_Reg_WHOAMI) != 0xD3)
    {
        return -1;
    }


    unsigned char CtrlReg1 = 0;
    //Basicaly turn the device on.
    CtrlReg1 |= 
            (0 << L3G4200D_RegBit_DR) | //100 hz ODR
            (0 << L3G4200D_RegBit_BW) | //12.5 Cutoff
            (1 << L3G4200D_RegBit_PD) | //Turn's off Power Down
            (1 << L3G4200D_RegBit_Zen) | //Enables Z axis
            (1 << L3G4200D_RegBit_Yen) | //Enables Y axis
            (1 << L3G4200D_RegBit_Xen); //Enables X axis
    L3G4200D_WriteRegister_Blocking(L3G4200D_Reg_CTRLREG1, CtrlReg1);


    //High Pass filter Settings
    unsigned char CtrlReg2 = 0;
    CtrlReg2 |= 0;
    L3G4200D_WriteRegister_Blocking(L3G4200D_Reg_CTRLREG2, CtrlReg2);


    //Interupt and FIFO Stuff
    unsigned char CtrlReg3 = 0;
    CtrlReg3 |= 0;
    L3G4200D_WriteRegister_Blocking(L3G4200D_Reg_CTRLREG3, CtrlReg3);


    //Data update, endian mode, scale selection, self test, SPI wire mode
    unsigned char CtrlReg4 = 0;
    CtrlReg4 |= (0 << L3G4200D_RegBit_FS); //Full scale selection of 250 dps.
    L3G4200D_WriteRegister_Blocking(L3G4200D_Reg_CTRLREG4, CtrlReg4);

    
    //reboot, fifo enable, interupt stuff
    unsigned char CtrlReg5 = 0;
    CtrlReg5 |= 0;
    L3G4200D_WriteRegister_Blocking(L3G4200D_Reg_CTRLREG5, CtrlReg5);

}

void L3G4200D_QueueWriteRegister(unsigned char reg, unsigned char value)
{
    //Control Register one
    char buff[3];
    buff[0] = reg; //Register ADDR.. Defualt return: 11010011 .. 0xD3
    buff[0] &= 0x7F; //Sets the write flag
    buff[1] = value;
    FIFOSPI2_SendQueue(buff, 2, 2);
}
void L3G4200D_QueueReadRegister(unsigned char reg)
{
    char buff[3];
    buff[0] = reg;
    //Sets the read flag
    buff[0] |= (1 << 7);
    buff[1] = 0x00; //Fluff
    FIFOSPI2_SendQueue(buff, 2, 2);
}

//TODO: Tecnically not blocking. index could be >2 to start with...
void L3G4200D_WriteRegister_Blocking(unsigned char reg, unsigned char value)
{
    //Control Register one
    char buff[3];
    buff[0] = reg; //Register ADDR.. Defualt return: 11010011 .. 0xD3
    buff[0] &= 0x7F; //Sets the write flag
    buff[1] = value;
    FIFOSPI2_SendQueue(buff, 2, 2);


    char func_rslt, read_rslt;
    while (FIFOSPI2_RecieveBufferIndex() < 2 || FIFOSPI_isRunnning != 0) {}
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt);
}


//TODO: Change things into unsigned char
unsigned char L3G4200D_ReadRegister_Blocking(unsigned char reg)
{
    
    char buff[3];
    buff[0] = reg; 
    //Sets the read flag
    buff[0] |= (1 << 7);
    buff[1] = 0x00; //Fluff
    FIFOSPI2_SendQueue(buff, 2, 2);

    
    //Force wait for the end of the transmission
    char func_rslt, read_rslt;
    while (FIFOSPI2_RecieveBufferIndex() < 2 || FIFOSPI_isRunnning != 0) {}
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt);
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt);

    return read_rslt;
}


void L3G4200D_ReadXYZT()
{
    
    L3G4200D_QueueReadRegister(L3G4200D_Reg_OUTXL);
    L3G4200D_QueueReadRegister(L3G4200D_Reg_OUTXH);
    L3G4200D_QueueReadRegister(L3G4200D_Reg_OUTYL);
    L3G4200D_QueueReadRegister(L3G4200D_Reg_OUTYH);
    L3G4200D_QueueReadRegister(L3G4200D_Reg_OUTZH);
    L3G4200D_QueueReadRegister(L3G4200D_Reg_OUTZL);
    L3G4200D_QueueReadRegister(L3G4200D_Reg_OUTTEMP);
    
}

void L3G4200D_InterpretXYZT()
{
    
    char read_rslt1, read_rslt2, func_rslt;


    //while (FIFOSPI2_RecieveBufferIndex() < 11) {}

    //X axis
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //X low
    L3G4200D_XAxis = read_rslt2;
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //X hi
    L3G4200D_XAxis |= ((read_rslt2 & 0xFF) << 8);

    //Y axis
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //Y low
    L3G4200D_YAxis = read_rslt2;
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //Y hi
    L3G4200D_YAxis |= ((read_rslt2 & 0xFF) << 8);

    //Z axis
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //Z low
    L3G4200D_ZAxis = read_rslt2;
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //Z hi
    L3G4200D_ZAxis |= ((read_rslt2 & 0xFF) << 8);

    //Temperature
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt1); //fluff
    func_rslt = FIFOSPI2_ReadQueue(&read_rslt2); //Z low
    L3G4200D_Temperature = read_rslt2;

}

