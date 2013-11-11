/**
 * @Author: Connor Martin
 * @Description: A set of function for interfacing with the L3G4200D Gyroscope
 * @Requirements: FIFOSPI.c
 * @Devices: PIC32MX320F128H
 */

#ifndef L3G4200D_H
#define	L3G4200D_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "FIFOSPI2.h"
    #include "Hardware.h"

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
    
    /**Global variable that contains gyroscope's X-axis data.*/
    extern double L3G4200D_XAxis;
    /**Global variable that contains gyroscope's Y-axis data.*/
    extern double L3G4200D_YAxis;
    /**Global variable that contains gyroscope's Z-axis data.*/
    extern double L3G4200D_ZAxis;
    /**Global variable that contains gyroscope's temperature data.*/
    extern double L3G4200D_Temperature;

    /**
     * Sets up and turns on the L3G4200D for measurement reading.
     * @return -1 if the device is not commuicating properly.
     */
    int L3G4200D_StartMeasurements();

    /**
     * Queues a write-to-register command for the L3G4200D in the FIFOSPI.c buffer.
     * @param reg: The register to write to.
     * @param value: The value to write to the register.
     */
    void L3G4200D_QueueWriteRegister(unsigned char reg, unsigned char value);
    /**
     * Queues a read-register command for the L3G4200D in the FIFOSPI buffer.
     * @param reg: The register to write to.
     */
    void L3G4200D_QueueReadRegister(unsigned char reg);

    /**
     * Queues a write-register command for the L3G4200D in the FIFOSPI buffer
     * then waits for the transmission to complete.
     * @param reg: The register to write to.
     * @param value: The value to write to the register.
     */
    void L3G4200D_WriteRegister_Blocking(unsigned char reg, unsigned char value);
    /**
     * Queues a read-register command for the L3G4200D in the FIFOSPI buffer
     * then waits for the transmission to complete.
     * @param reg: The register to write to.
     * @param value: The value to write to the register.
     * @return The read unsigned character
     */
    unsigned char L3G4200D_ReadRegister_Blocking(unsigned char reg);

    /**
     * Queues a read X, Y, Z and temperature data register command for the 
     * L3G4200D in the FIFOSPI buffer.
     */
    void L3G4200D_QueueReadXYZT();
    /**
     * Interpret's the read X, Y, Z and temperature data register command for
     * the L3G4200D from the FIFOSPI buffer.
     */
    void L3G4200D_InterpretXYZT();

#ifdef	__cplusplus
}
#endif

#endif	/* L3G4200D_H */

