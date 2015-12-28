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
#ifndef __MSP430WARE_COMP_E_H__
#define __MSP430WARE_COMP_E_H__

#define __MSP430_HAS_COMP_E__

#include "inc/hw_regaccess.h"

//*****************************************************************************
//
//The following are values that can be passed to Comp_init(),
//Comp_disableInputBuffer(), and Comp_enableInputBuffer() in the
//positiveTerminalInput, negativeTerminalInput, and inputPort parameters.
//
//*****************************************************************************
#define COMP_E_INPUT0  (CEIPSEL_0)
#define COMP_E_INPUT1  (CEIPSEL_1)
#define COMP_E_INPUT2  (CEIPSEL_2)
#define COMP_E_INPUT3  (CEIPSEL_3)
#define COMP_E_INPUT4  (CEIPSEL_4)
#define COMP_E_INPUT5  (CEIPSEL_5)
#define COMP_E_INPUT6  (CEIPSEL_6)
#define COMP_E_INPUT7  (CEIPSEL_7)
#define COMP_E_INPUT8  (CEIPSEL_8)
#define COMP_E_INPUT9  (CEIPSEL_9)
#define COMP_E_INPUT10 (CEIPSEL_10)
#define COMP_E_INPUT11 (CEIPSEL_11)
#define COMP_E_INPUT12 (CEIPSEL_12)
#define COMP_E_INPUT13 (CEIPSEL_13)
#define COMP_E_INPUT14 (CEIPSEL_14)
#define COMP_E_INPUT15 (CEIPSEL_15)
#define COMP_E_VREF    (0x10)

//*****************************************************************************
//
//The following are values that can be passed to Comp_init() in the
//outputFilterEnableAndDelayLevel parameter.
//
//*****************************************************************************
#define COMP_E_FILTEROUTPUT_OFF     0x00
#define COMP_E_FILTEROUTPUT_DLYLVL1 (CEF + CEFDLY_0)
#define COMP_E_FILTEROUTPUT_DLYLVL2 (CEF + CEFDLY_1)
#define COMP_E_FILTEROUTPUT_DLYLVL3 (CEF + CEFDLY_2)
#define COMP_E_FILTEROUTPUT_DLYLVL4 (CEF + CEFDLY_3)

//*****************************************************************************
//
//The following are values that can be passed to Comp_init() in the
//invertedOutputPolarity parameter.
//
//*****************************************************************************
#define COMP_E_NORMALOUTPUTPOLARITY   ( !(CEOUTPOL) )
#define COMP_E_INVERTEDOUTPUTPOLARITY (CEOUTPOL)

//*****************************************************************************
//
//The following are values that can be passed to Comp_setReferenceVoltage() in
//the supplyVoltageBase parameter.
//
//*****************************************************************************
#define COMP_E_REFERENCE_AMPLIFIER_DISABLED (CEREFL_0)
#define COMP_E_VREFBASE1_2V (CEREFL_1)
#define COMP_E_VREFBASE2_0V (CEREFL_2)
#define COMP_E_VREFBASE2_5V (CEREFL_3)
//deprecated
#define COMP_E_VREFBASE_VCC (CEREFL_0)

//*****************************************************************************
//
//The following are values that can be passed to Comp_setReferenceVoltage() in
//the refrenceAccuracy parameter.
//
//*****************************************************************************
#define COMP_E_ACCURACY_STATIC	(!CEREFACC)
#define COMP_E_ACCURACY_CLOCKED	(CEREFACC)

//*****************************************************************************
//
//The following are values that can be passed to COMP_E_setPowerMode() in
//the powerMode parameter.
//
//*****************************************************************************
#define COMP_E_HIGH_SPEED_MODE 		(CEPWRMD_0)
#define COMP_E_NORMAL_MODE 			(CEPWRMD_1)
#define COMP_E_ULTRA_LOW_POWER_MODE	(CEPWRMD_2)


//*****************************************************************************
//
//The following are values that can be passed to Comp_setEdgeDirection() in
//the edgeDirection parameter.
//
//*****************************************************************************
#define COMP_E_FALLINGEDGE ( !(CEIES) )
#define COMP_E_RISINGEDGE  (CEIES)

//*****************************************************************************
//
//The following are values that returned by COMP_E_outputValue().
//
//*****************************************************************************
#define COMP_E_LOW  (0x0)
#define COMP_E_HIGH (0x1)

//*****************************************************************************
//
//The following are values that can be passed to COMP_E_enableInterrupt().
// COMP_E_disableInterrupt()
//
//*****************************************************************************
#define COMP_E_OUTPUT_INTERRUPT (CEIE)
#define COMP_E_INVERTED_POLARITY_INTERRUPT (CEIIE)
#define COMP_E_READY_INTERRUPT (CERDYIE)

//*****************************************************************************
//
//The following are values that can be passed to COMP_E_clearInterrupt().
// and returned from COMP_E_getInterruptStatus()
//
//*****************************************************************************
#define COMP_E_OUTPUT_INTERRUPT_FLAG (CEIFG)
#define COMP_E_INTERRUPT_FLAG_INVERTED_POLARITY (CEIIFG)
#define COMP_E_INTERRUPT_FLAG_READY (CERDYIFG)
//deprecated
#define COMP_E_INTERRUPT_FLAG (CEIFG)

//*****************************************************************************
//API
//*****************************************************************************

extern unsigned short COMP_E_init(uint32_t baseAddress,
		uint8_t positiveTerminalInput,
		uint8_t negativeTerminalInput,
		uint8_t outputFilterEnableAndDelayLevel,
		unsigned short invertedOutputPolarity);

extern void COMP_E_setReferenceVoltage (uint32_t baseAddress,
    uint16_t supplyVoltageReferenceBase,
    uint16_t lowerLimitSupplyVoltageFractionOf32,
    uint16_t upperLimitSupplyVoltageFractionOf32);

extern void COMP_E_setReferenceAccuracy (uint32_t baseAddress,
	uint16_t referenceAccuracy);
	
extern void COMP_E_setPowerMode (uint32_t baseAddress,
		uint16_t powerMode);
	
extern void COMP_E_enableInterrupt(uint32_t baseAddress, uint16_t mask);

extern void COMP_E_disableInterrupt(uint32_t baseAddress, uint16_t mask);

extern void COMP_E_clearInterrupt(uint32_t baseAddress, uint16_t mask);

extern uint8_t COMP_E_getInterruptStatus(uint32_t baseAddress,
		uint16_t mask);

extern void COMP_E_interruptSetEdgeDirection(uint32_t baseAddress,
		unsigned short edgeDirection);

extern void COMP_E_interruptToggleEdgeDirection(uint32_t baseAddress);

extern void COMP_E_enable(uint32_t baseAddress);

extern void COMP_E_disable(uint32_t baseAddress);

extern void COMP_E_shortInputs (uint32_t baseAddress);

extern void COMP_E_unshortInputs (uint32_t baseAddress);

extern void COMP_E_disableInputBuffer(uint32_t baseAddress,
		uint8_t inputPort);

extern void COMP_E_enableInputBuffer(uint32_t baseAddress,
		uint8_t inputPort);

extern void COMP_E_IOSwap(uint32_t baseAddress);

extern unsigned short COMP_E_outputValue(uint32_t baseAddress);

#endif

