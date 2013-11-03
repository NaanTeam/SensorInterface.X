/* 
 * File:   L3G4200D.h
 * Author: Connor
 *
 * Created on November 2, 2013, 6:59 PM
 */

#ifndef L3G4200D_H
#define	L3G4200D_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "FIFOSPI.h"

    /**Device identification registe.r*/
    #define L3G4200D_Reg_WHOAMI  0x0F
    /**Control register one.*/
    #define L3G4200D_Reg_CTRLREG1  0x20
        /**Output Data Rate selection.*/
        #define L3G4200D_RegBit_DR 6 //2 bits
        /**Bandwidth selection.*/
        #define L3G4200D_RegBit_BW 4 //2 bits
        /**Power down mode enable.*/
        #define L3G4200D_RegBit_PD 3
        /**Z axis enable.*/
        #define L3G4200D_RegBit_Zen 2
        /**Y axis enable.*/
        #define L3G4200D_RegBit_Yen 1
        /**x axis enable.*/
        #define L3G4200D_RegBit_Xen 0
    /**Control register two.*/
    #define L3G4200D_Reg_CTRLREG2  0x21
    /**Control register three.*/
    #define L3G4200D_Reg_CTRLREG3  0x22
    /**Control register four.*/
    #define L3G4200D_Reg_CTRLREG4  0x23
        /** Full Scale Selection. 2 bits.*/
        #define L3G4200D_RegBit_FS 4
        /** Self Test. 2 bits.*/
        #define L3G4200D_RegBit_ST 1
    /**Control register five.*/
    #define L3G4200D_Reg_CTRLREG5  0x24
    /**Reference value register for Interrupt generation.*/
    #define L3G4200D_Reg_REF  0x25
    /**Temperature data register.*/
    #define L3G4200D_Reg_OUTTEMP  0x26
    #define L3G4200D_Reg_STATUSREG  0x27
    /** Lower X-axis angular rate data. The value is expressed as two?s complement.*/
    #define L3G4200D_Reg_OUTXL  0x28
    /** Higher X-axis angular rate data. The value is expressed as two?s complement.*/
    #define L3G4200D_Reg_OUTXH  0x29
    /** Lower Y-axis angular rate data. The value is expressed as two?s complement.*/
    #define L3G4200D_Reg_OUTYL  0x2A
    /** Higher Y-axis angular rate data. The value is expressed as two?s complement.*/
    #define L3G4200D_Reg_OUTYH  0x2B
    /** Lower Z-axis angular rate data. The value is expressed as two?s complement.*/
    #define L3G4200D_Reg_OUTZL  0x2C
    /** Higher Z-axis angular rate data. The value is expressed as two?s complement.*/
    #define L3G4200D_Reg_OUTZH  0x2D
    /**FIFO buffer control register.*/
    #define L3G4200D_Reg_FIFOCTRLREG  0x2E
    //... Additional interrupt registers that I didn't include.
    

    extern short L3G4200D_XAxis;
    extern short L3G4200D_YAxis;
    extern short L3G4200D_ZAxis;
    extern short L3G4200D_Temperature;

    int L3G4200D_StartMeasurements();

    void L3G4200D_QueueWriteRegister(unsigned char reg, unsigned char value);
    void L3G4200D_QueueReadRegister(unsigned char reg);

    void L3G4200D_WriteRegister_Blocking(unsigned char reg, unsigned char value);
    unsigned char L3G4200D_ReadRegister_Blocking(unsigned char reg);

    void L3G4200D_ReadXYZT();
    void L3G4200D_InterpretXYZT();

#ifdef	__cplusplus
}
#endif

#endif	/* L3G4200D_H */

