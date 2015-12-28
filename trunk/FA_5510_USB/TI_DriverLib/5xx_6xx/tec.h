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
//AND INCLUDED APIS FOR NEW APPLICATIONS.
//*****************************************************************************
#ifndef __MSP430WARE_TEC_H__
#define __MSP430WARE_TEC_H__

//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
//*****************************************************************************
#define __MSP430_HAS_TEV0__
//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_configureExternalClearInput() API as the signalType parameter.
//
//*****************************************************************************
#define TEC_EXTERNAL_CLEAR_SIGNALTYPE_EDGE_SENSITIVE            0x00
#define TEC_EXTERNAL_CLEAR_SIGNALTYPE_LEVEL_SENSITIVE           TECEXCLRLVS

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_configureExternalClearInput() API as the signalHold parameter.
//
//*****************************************************************************
#define TEC_EXTERNAL_CLEAR_SIGNAL_NOT_HELD       0x00
#define TEC_EXTERNAL_CLEAR_SIGNAL_HELD           TECEXCLRHLD

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_configureExternalClearInput() API as the polarityBit parameter.
//
//*****************************************************************************
#define TEC_EXTERNAL_CLEAR_POLARITY_FALLING_EDGE_OR_LOW_LEVEL       0x00
#define TEC_EXTERNAL_CLEAR_POLARITY_RISING_EDGE_OR_HIGH_LEVEL       TECEXCLRPOL

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_configureExternalFaultInput() API as the signalHold parameter.
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_SIGNAL_NOT_HELD       0x00
#define TEC_EXTERNAL_FAULT_SIGNAL_HELD           TECXFLTHLD0

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_configureExternalFaultInput() API as the polarityBit parameter.
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_POLARITY_FALLING_EDGE_OR_LOW_LEVEL       0x00
#define TEC_EXTERNAL_FAULT_POLARITY_RISING_EDGE_OR_HIGH_LEVEL       TECXFLTPOL0

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_configureExternalFaultInput() API as the signalType parameter.
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_SIGNALTYPE_EDGE_SENSITIVE       0x00
#define TEC_EXTERNAL_FAULT_SIGNALTYPE_LEVEL_SENSITIVE      TECXFLTLVS0

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_configureExternalFaultInput() as the selectedExternalFault parameter
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_0			0
#define TEC_EXTERNAL_FAULT_1			1
#define TEC_EXTERNAL_FAULT_2			2
#define TEC_EXTERNAL_FAULT_3			3
#define TEC_EXTERNAL_FAULT_4			4
#define TEC_EXTERNAL_FAULT_5			5
#define TEC_EXTERNAL_FAULT_6			6

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_disableInterrupt(), TEC_enableInterrupt(), TEC_getInterruptStatus(),
// TEC_clearInterruptFlag(),  API as the mask parameter.
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_INTERRUPT      TECXFLTIFG
#define TEC_EXTERNAL_CLEAR_INTERRUPT      TECEXCLRIFG
#define TEC_AUXILIARY_CLEAR_INTERRUPT	  TECAXCLRIFG

//*****************************************************************************
//
//The following are values that can be passed to the
//TEC_disableExternalFaultInput(), TEC_enableExternalFaultInput(), 
//TEC_clearExternalFaultStatus(), TEC_getExternalFaultStatus()
//as the mask parameter
//
//*****************************************************************************
#define TEC_CE0	TECXFLT0STA
#define TEC_CE1	TECXFLT1STA
#define TEC_CE2	TECXFLT2STA
#define TEC_CE3	TECXFLT3STA
#define TEC_CE4	TECXFLT4STA
#define TEC_CE5	TECXFLT5STA
#define TEC_CE6	TECXFLT6STA

//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern void TEC_configureExternalClearInput (	unsigned int baseAddress,
									  	unsigned char signalType,
									  	unsigned char signalHold,
									  	unsigned char polarityBit
									  );									  
extern void TEC_configureExternalFaultInput (	unsigned int baseAddress,
										unsigned char selectedExternalFault,
									  	unsigned int signalType,
									  	unsigned char signalHold,
									  	unsigned char polarityBit
									  );
extern void TEC_enableExternalFaultInput (unsigned int baseAddress,
										unsigned char channelEventBlock
										);
extern void TEC_disableExternalFaultInput (unsigned int baseAddress,
										unsigned char channelEventBlock
										);
extern void TEC_enableExternalClearInput (unsigned int baseAddress );
extern void TEC_disableExternalClearInput (unsigned int baseAddress );
extern void TEC_enableAuxiliaryClearSignal (unsigned int baseAddress );
extern void TEC_disableAuxiliaryClearSignal (unsigned int baseAddress );
extern void TEC_clearInterruptFlag (unsigned int baseAddress,
    unsigned char mask
    );
extern unsigned char TEC_getInterruptStatus (unsigned int baseAddress,
    unsigned char mask
    );
extern void TEC_enableInterrupt (unsigned int baseAddress,
    unsigned char mask
    );
extern void TEC_disableInterrupt (unsigned int baseAddress,
    unsigned char mask
    );
extern unsigned char TEC_getExternalFaultStatus (unsigned int baseAddress,
    unsigned char mask
    );
extern void TEC_clearExternalFaultStatus (unsigned int baseAddress,
    unsigned char mask
    );
extern unsigned char TEC_getExternalClearStatus (unsigned int baseAddress);
extern void TEC_clearExternalClearStatus (unsigned int baseAddress);
//*****************************************************************************
//
//Close the Doxygen group.
//! @}
//
//*****************************************************************************

#endif
