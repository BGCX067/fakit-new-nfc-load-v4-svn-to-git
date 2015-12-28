/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//THIS FILE IS INCLUDED FOR BACKWARD COMPATIBILTY. PLEASE DO NOT USE THIS FILE
//AND INCLUDED APIS FOR NEW APPLICATIONS. PLEASE REFER usci_a_uart.h
//FOR NEW PROJECTS
//*****************************************************************************

#ifndef __MSP430WARE_USCI_UART_H__
#define __MSP430WARE_USCI_UART_H__

//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
//*****************************************************************************
#define __MSP430_HAS_USCI_Ax__

#include "inc/hw_regaccess.h"

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_init() API
//as the parity parameter.
//
//*****************************************************************************
#define USCI_UART_NO_PARITY   0x00
#define USCI_UART_ODD_PARITY  0x01
#define USCI_UART_EVEN_PARITY 0x02

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_init() API
//as the selectClockSource parameter.
//
//*****************************************************************************
#define USCI_UART_CLOCKSOURCE_ACLK    UCSSEL__ACLK
#define USCI_UART_CLOCKSOURCE_SMCLK   UCSSEL__SMCLK

//*****************************************************************************

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_init() API
//as the numberofStopBits parameter.
//
//*****************************************************************************
#define USCI_UART_ONE_STOP_BIT    0x00
#define USCI_UART_TWO_STOP_BITS   UCSPB


//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_init() API
//as the msborLsbFirst parameter.
//
//*****************************************************************************
#define USCI_UART_MSB_FIRST    UCMSB
#define USCI_UART_LSB_FIRST    0x00

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_getInterruptStatus(),
//as the mask parameter.
//
//*****************************************************************************
#define USCI_UART_RECEIVE_INTERRUPT_FLAG            UCRXIFG
#define USCI_UART_TRANSMIT_INTERRUPT_FLAG           UCTXIFG

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_enableInterrupt(),
//USCI_UART_disableInterrupt() API as the mask parameter.
//
//*****************************************************************************
#define USCI_UART_RECEIVE_INTERRUPT                  UCRXIE
#define USCI_UART_TRANSMIT_INTERRUPT                 UCTXIE
#define USCI_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT    UCRXEIE
#define USCI_UART_BREAKCHAR_INTERRUPT                UCBRKIE

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_queryStatusFlags()
//API as the mask parameter.
//
//*****************************************************************************
#define USCI_UART_LISTEN_ENABLE      UCLISTEN
#define USCI_UART_FRAMING_ERROR      UCFE
#define USCI_UART_OVERRUN_ERROR      UCOE
#define USCI_UART_PARITY_ERROR       UCPE
#define USCI_UART_BREAK_DETECT		 UCBRK
#define USCI_UART_RECEIVE_ERROR      UCRXERR
#define USCI_UART_ADDRESS_RECEIVED   UCADDR
#define USCI_UART_IDLELINE           UCIDLE
#define USCI_UART_BUSY               UCBUSY

//Deprecated
#define UARTBREAK_DETECT        	UCBRK

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_init()
//API as the mode parameter.
//
//*****************************************************************************
#define USCI_UART_MODE                              UCMODE_0
#define USCI_UART_IDLE_LINE_MULTI_PROCESSOR_MODE    UCMODE_1
#define USCI_UART_ADDRESS_BIT_MULTI_PROCESSOR_MODE  UCMODE_2
#define USCI_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE UCMODE_3

//*****************************************************************************
//
//The following are values that can be passed to the USCI_UART_init()
//API as the overSampling parameter.
//
//*****************************************************************************
#define USCI_UART_OVERSAMPLING_BAUDRATE_GENERATION     0x01
#define USCI_UART_LOW_FREQUENCY_BAUDRATE_GENERATION    0x00


//*****************************************************************************
//
//The following are values are the sync characters possible
//
//*****************************************************************************
#define DEFAULT_SYNC 0x00
#define USCI_UART_AUTOMATICBAUDRATE_SYNC 0x55

//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern
unsigned short USCI_UART_init ( uint32_t baseAddress,
    uint8_t selectClockSource,
    uint32_t clockSourceFrequency,
    uint32_t desiredUartBaudRate,
    uint8_t parity,
    uint8_t msborLsbFirst,
    uint8_t numberofStopBits,
    uint8_t uartMode,
    unsigned short overSampling
    );
extern
unsigned short USCI_UART_initAdvance ( uint32_t baseAddress,
    uint8_t selectClockSource,
    uint16_t clockPrescalar,
    uint8_t firstModReg,
    uint8_t secondModReg,
    uint8_t parity,
    uint8_t msborLsbFirst,
    uint8_t numberofStopBits,
    uint8_t uartMode,
    unsigned short overSampling
    );
extern
void USCI_UART_transmitData ( uint32_t baseAddress,
    uint8_t transmitData
    );
extern
uint8_t USCI_UART_receiveData (uint32_t baseAddress);
extern
void USCI_UART_enableInterrupt (uint32_t baseAddress,
    uint8_t mask
    );
extern
void USCI_UART_disableInterrupt (uint32_t baseAddress,
    uint8_t mask
    );
extern
uint8_t USCI_UART_getInterruptStatus (uint32_t baseAddress,
    uint8_t mask
    );
extern
void USCI_UART_clearInterruptFlag (uint32_t baseAddress,
    uint8_t mask
    );
extern
void USCI_UART_enable (uint32_t baseAddress);
extern
void USCI_UART_disable (uint32_t baseAddress);
extern
uint8_t USCI_UART_queryStatusFlags (uint32_t baseAddress,
    uint8_t mask);
extern
void USCI_UART_setDormant (uint32_t baseAddress);
extern
void USCI_UART_resetDormant (uint32_t baseAddress);
extern
void USCI_UART_transmitAddress (uint32_t baseAddress,
    uint8_t transmitAddress);
extern
void USCI_UART_transmitBreak (uint32_t baseAddress);
extern
uint32_t USCI_UART_getReceiveBufferAddressForDMA (uint32_t baseAddress);
extern
uint32_t USCI_UART_getTransmitBufferAddressForDMA (uint32_t baseAddress);
#endif
