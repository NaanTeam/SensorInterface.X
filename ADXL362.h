/**
 * @Author: Connor Martin
 * @Description: A set of function for interfacing with the ADXL362 Accelerometer.
 * @Requirements: FIFOSPI.c
 * @Devices: 
 */

#ifndef ADXL362_H
#define	ADXL362_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "FIFOSPI2.h"
    #include "Hardware.h"

    /* --- command --- */

    #define ADXL362_REG_WRITE   0x0a
    #define ADXL362_REG_READ    0x0b
    #define ADXL362_FIFO_READ   0x0d

    /* ------- Register names ------- */
    /* -- part ID constants -- */
    #define ADXL362_DEVID_AD           0x00
    #define ADXL362_DEVID_MST          0x01
    #define ADXL362_PARTID             0x02
    #define ADXL362_REVID              0x03
    #define ADXL362_XID                0x04

    /* -- RO data -- */
    #define ADXL362_XDATA8             0x08
    #define ADXL362_YDATA8             0x09
    #define ADXL362_ZDATA8             0x0A
    #define ADXL362_STATUS             0x0B
    #define ADXL362_FIFO_ENTRIES_L     0x0C
    #define ADXL362_FIFO_ENTRIES_H     0x0D
    #define ADXL362_XDATAL             0x0e
    #define ADXL362_XDATAH             0x0f
    #define ADXL362_YDATAL             0x10
    #define ADXL362_YDATAH             0x11
    #define ADXL362_ZDATAL             0x12
    #define ADXL362_ZDATAH             0x13
    #define ADXL362_TEMPL              0x14
    #define ADXL362_TEMPH              0x15
    #define ADXL362_X_ADCL             0x16
    #define ADXL362_X_ADCH             0x17

    /* -- Control and Config -- */
    #define ADXL362_SOFT_RESET         0x1f
    #define ADXL362_THRESH_ACTL        0x20
    #define ADXL362_THRESH_ACTH        0x21
    #define ADXL362_TIME_ACT           0x22
    #define ADXL362_THRESH_INACTL      0x23
    #define ADXL362_THRESH_INACTH      0x24
    #define ADXL362_TIME_INACTL        0x25
    #define ADXL362_TIME_INACTH        0x26
    #define ADXL362_ACT_INACT_CTL      0x27
    #define ADXL362_FIFO_CONTROL       0x28
    #define ADXL362_FIFO_SAMPLES       0x29
    #define ADXL362_INTMAP1            0x2a
    #define ADXL362_INTMAP2            0x2b
    #define ADXL362_FILTER_CTL         0x2c
    #define ADXL362_POWER_CTL          0x2d
    #define ADXL362_SELF_TEST          0x2e

    /*----------------------------------------------------------------------
      Bit field definitions and register values
      ----------------------------------------------------------------------*/
    //#define ADXL362_
    /* register values for DEVID                                            */
    /* The device ID should always read this value, The customer does not
      need to use this value but it can be samity checked to check that the
      device can communicate                                                */

    #define ADXL362_ID              0xe2

    /* Key Value in soft reset                                              */
    #define ADXL362_SOFT_RESET_KEY      0x52


    /* Registers THRESH_ACTL through TIME_INACTH just take values
       in lsbs or samples.
       There are no specific bit fields in these registers                  */

    /* Bit values in ACT_INACT_CTL  (6 bits allocated)                      */
    #define ADXL362_ACT_ENABLE      0x01
    #define ADXL362_ACT_DISABLE     0x00
    #define ADXL362_ACT_AC          0x02
    #define ADXL362_ACT_DC          0x00
    #define ADXL362_INACT_ENABLE    0x04
    #define ADXL362_INACT_DISABLE   0x00
    #define ADXL362_INACT_AC        0x08
    #define ADXL362_INACT_DC        0x00
    #define ADXL362_ACT_INACT_LINK  0x10
    #define ADXL362_ACT_INACT_LOOP  0x20

    /* Bit values in FIFO_CTL  (4 bits allocated)                            */
    #define ADXL362_FIFO_MODE_OFF     0x00
    #define ADXL362_FIFO_MODE_FIFO    0x01
    #define ADXL362_FIFO_MODE_STREAM  0x02
    #define ADXL362_FIFO_MODE_TRIGGER 0x03
    #define ADXL362_FIFO_TEMP         0x04
    #define ADXL362_FIFO_SAMPLES_AH   0x08



    /* bit values in INTMAP1 and INTMAP2 (and status)                        */
    #define ADXL362_INT_DATA_READY     0x01
    #define ADXL362_INT_FIFO_READY     0x02
    #define ADXL362_INT_FIFO_WATERMARK 0x04
    #define ADXL362_INT_FIFO_OVERRUN   0x08
    #define ADXL362_INT_ACT            0x10
    #define ADXL362_INT_INACT          0x20
    #define ADXL362_INT_AWAKE          0x40
    #define ADXL362_INT_LOW            0x80

    /* Bit values in FILTER_CTL  (8 bits allocated)                            */
    #define ADXL362_RATE_400        0x05
    #define ADXL362_RATE_200        0x04
    #define ADXL362_RATE_100        0x03  /* default */
    #define ADXL362_RATE_50         0x02
    #define ADXL362_RATE_25         0x01
    #define ADXL362_RATE_12_5       0x00

    #define ADXL362_EXT_TRIGGER     0x08

    #define ADXL362_AXIS_X          0x00
    #define ADXL362_AXIS_Y          0x10
    #define ADXL362_AXIS_Z          0x20

    #define ADXL362_RANGE_2G        0x00
    #define ADXL362_RANGE_4G        0x40
    #define ADXL362_RANGE_8G        0x80


    /* Bit values in POWER_CTL  (8 bits allocated)                            */
    #define ADXL362_STANDBY           0x00
    #define ADXL362_MEASURE_1D        0x01
    #define ADXL362_MEASURE_3D        0x02
    #define ADXL362_AUTO_SLEEP        0x04
    #define ADXL362_SLEEP             0x08

    #define ADXL362_LOW_POWER         0x00
    #define ADXL362_LOW_NOISE1        0x10
    #define ADXL362_LOW_NOISE2        0x20
    #define ADXL362_LOW_NOISE3        0x30

    #define ADXL362_EXT_CLOCK         0x40
    #define ADXL362_EXT_ADC           0x80

    /* Bit values in SELF_TEST  (1 bit allocated)                            */
    #define ADXL362_SELFTEST_ON       0x01
    #define ADXL362_SELFTEST_OFF      0x00



    extern double ADXL362_XAcceleration;
    extern double ADXL362_YAcceleration;
    extern double ADXL362_ZAcceleration;
    extern double ADXL362_Temperature;

    
    void ADXL362_startMeasurements();

    void ADXL362_queueReadXYZT();
    void ADXL362_interpretXYZT();

    void ADXL362_queueReadMSBX();
    void ADXL362_interpretMSBX();
    void ADXL362_queueReadMSBY();
    void ADXL362_interpretMSBY();
    void ADXL362_queueReadMSBZ();
    void ADXL362_InterpretMSBZ();




#ifdef	__cplusplus
}
#endif

#endif	/* ADXL362_H */

