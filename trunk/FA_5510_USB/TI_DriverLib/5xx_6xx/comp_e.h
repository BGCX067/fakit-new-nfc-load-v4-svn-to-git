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
#ifndef __MSP430WARE_COMPE_H__
#define __MSP430WARE_COMPE_H__

#define __MSP430_HAS_COMP_E__

//*****************************************************************************
//
//The following are values that can be passed to Comp_init(),
//Comp_disableInputBuffer(), and Comp_enableInputBuffer() in the
//positiveTerminalInput, negativeTerminalInput, and inputPort parameters.
//
//*****************************************************************************
#define COMPE_INPUT0  (CEIPSEL_0)
#define COMPE_INPUT1  (CEIPSEL_1)
#define COMPE_INPUT2  (CEIPSEL_2)
#define COMPE_INPUT3  (CEIPSEL_3)
#define COMPE_INPUT4  (CEIPSEL_4)
#define COMPE_INPUT5  (CEIPSEL_5)
#define COMPE_INPUT6  (CEIPSEL_6)
#define COMPE_INPUT7  (CEIPSEL_7)
#define COMPE_INPUT8  (CEIPSEL_8)
#define COMPE_INPUT9  (CEIPSEL_9)
#define COMPE_INPUT10 (CEIPSEL_10)
#define COMPE_INPUT11 (CEIPSEL_11)
#define COMPE_INPUT12 (CEIPSEL_12)
#define COMPE_INPUT13 (CEIPSEL_13)
#define COMPE_INPUT14 (CEIPSEL_14)
#define COMPE_INPUT15 (CEIPSEL_15)
#define COMPE_VREF    (0x10)

//*****************************************************************************
//
//The following are values that can be passed to Comp_init() in the
//outputFilterEnableAndDelayLevel parameter.
//
//*****************************************************************************
#define COMPE_FILTEROUTPUT_OFF     0x00
#define COMPE_FILTEROUTPUT_DLYLVL1 (CEF + CEFDLY_0)
#define COMPE_FILTEROUTPUT_DLYLVL2 (CEF + CEFDLY_1)
#define COMPE_FILTEROUTPUT_DLYLVL3 (CEF + CEFDLY_2)
#define COMPE_FILTEROUTPUT_DLYLVL4 (CEF + CEFDLY_3)

//*****************************************************************************
//
//The following are values that can be passed to Comp_init() in the
//invertedOutputPolarity parameter.
//
//*****************************************************************************
#define COMPE_NORMALOUTPUTPOLARITY   ( !(CEOUTPOL) )
#define COMPE_INVERTEDOUTPUTPOLARITY (CEOUTPOL)

//*****************************************************************************
//
//The following are values that can be passed to Comp_setReferenceVoltage() in
//the supplyVoltageBase parameter.
//
//*****************************************************************************
#define COMPE_REFERENCE_AMPLIFIER_DISABLED (CEREFL_0)
#define COMPE_VREFBASE1_5V (CEREFL_1)
#define COMPE_VREFBASE2_0V (CEREFL_2)
#define COMPE_VREFBASE2_5V (CEREFL_3)
//deprecated
#define COMPE_VREFBASE_VCC (CEREFL_0)

//*****************************************************************************
//
//The following are values that can be passed to Comp_setReferenceVoltage() in
//the refrenceAccuracy parameter.
//
//*****************************************************************************
#define COMPE_ACCURACY_STATIC	(!CEREFACC)
#define COMPE_ACCURACY_CLOCKED	(CEREFACC)

//*****************************************************************************
//
//The following are values that can be passed to COMPE_setPowerMode() in
//the powerMode parameter.
//
//*****************************************************************************
#define COMPE_HIGH_SPEED_MODE 		(CEPWRMD_0)
#define COMPE_NORMAL_MODE 			(CEPWRMD_1)
#define COMPE_ULTRA_LOW_POWER_MODE	(CEPWRMD_2)


//*****************************************************************************
//
//The following are values that can be passed to Comp_setEdgeDirection() in
//the edgeDirection parameter.
//
//*****************************************************************************
#define COMPE_FALLINGEDGE ( !(CEIES) )
#define COMPE_RISINGEDGE  (CEIES)

//*****************************************************************************
//
//The following are values that returned by COMPE_outputValue().
//
//*****************************************************************************
#define COMPE_LOW  (0x0)
#define COMPE_HIGH (0x1)

//*****************************************************************************
//
//The following are values that can be passed to COMPE_enableInterrupt().
// COMPE_disableInterrupt()
//
//*****************************************************************************
#define COMPE_OUTPUT_INTERRUPT (CEIE)
#define COMPE_INVERTED_POLARITY_INTERRUPT (CEIIE)
#define COMPE_READY_INTERRUPT (CERDYIE)

//*****************************************************************************
//
//The following are values that can be passed to COMPE_clearInterrupt().
// and returned from COMPE_getInterruptStatus()
//
//*****************************************************************************
#define COMPE_INTERRUPT_FLAG (CEIFG)
#define COMPE_INTERRUPT_FLAG_INVERTED_POLARITY (CEIIFG)
#define COMPE_INTERRUPT_FLAG_READY (CERDYIFG)

//*****************************************************************************
//API
//*****************************************************************************

extern unsigned short COMPE_init(unsigned int baseAddress,
		unsigned char positiveTerminalInput,
		unsigned char negativeTerminalInput,
		unsigned char outputFilterEnableAndDelayLevel,
		unsigned short invertedOutputPolarity);

extern void COMPE_setReferenceVoltage (unsigned int baseAddress,
    unsigned int supplyVoltageReferenceBase,
    unsigned int lowerLimitSupplyVoltageFractionOf32,
    unsigned int upperLimitSupplyVoltageFractionOf32);

extern void COMPE_setReferenceAccuracy (unsigned int baseAddress,
	unsigned int referenceAccuracy);
	
extern void COMPE_setPowerMode (unsigned int baseAddress,
		unsigned int powerMode);
	
extern void COMPE_enableInterrupt(unsigned int baseAddress, unsigned int mask);

extern void COMPE_disableInterrupt(unsigned int baseAddress, unsigned int mask);

extern void COMPE_clearInterrupt(unsigned int baseAddress, unsigned int mask);

extern unsigned char COMPE_getInterruptStatus(unsigned int baseAddress,
		unsigned int mask);

extern void COMPE_interruptSetEdgeDirection(unsigned int baseAddress,
		unsigned short edgeDirection);

extern void COMPE_interruptToggleEdgeDirection(unsigned int baseAddress);

extern void COMPE_enable(unsigned int baseAddress);

extern void COMPE_disable(unsigned int baseAddress);

extern void COMPE_shortInputs (unsigned int baseAddress);

extern void COMPE_unshortInputs (unsigned int baseAddress);

extern void COMPE_disableInputBuffer(unsigned int baseAddress,
		unsigned char inputPort);

extern void COMPE_enableInputBuffer(unsigned int baseAddress,
		unsigned char inputPort);

extern void COMPE_IOSwap(unsigned int baseAddress);

extern unsigned short COMPE_outputValue(unsigned int baseAddress);

#endif

