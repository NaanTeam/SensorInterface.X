FIFOSPI.X
=========

A PIC32 interrupt driven SPI communication with an ADXL362 and L3G4200D which
then sends it to a uart port (connected to a usb to serial converter). For use
on the chipkit WF32 board.


--Pin Mapping--
A5: I2C2: SCL
A4: I2C2: SDA

13: SPI2: SCLK
12: SPI2: SDIO (in)
11: SPI2: SDIO (out)
10: SPI2: CS (device 1)
9:  SPI2: CS (device 2)
