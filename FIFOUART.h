/* 
 * File:   FIFOUART.h
 * Author: Connor
 *
 * Created on November 13, 2013, 7:12 PM
 */

#ifndef FIFOUART_H
#define	FIFOUART_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "Hardware.h"

    #define FIFOUART_MODULE_ID UART1
    #define FIFOUART_BAUD_RATE 9600


    void FIFOUART_initialize();

    void FIFOUART_sendBuffer(const char *buffer, int size);


#ifdef	__cplusplus
}
#endif

#endif	/* FIFOUART_H */

