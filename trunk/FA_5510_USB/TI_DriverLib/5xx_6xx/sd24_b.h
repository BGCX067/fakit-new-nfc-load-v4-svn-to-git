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
#ifndef __MSP430WARE_SD24B_H__
#define __MSP430WARE_SD24B_H__

//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
//*****************************************************************************
#define  __MSP430_HAS_SD24_B__
#define  __MSP430_HAS_SD24_B8__

//*****************************************************************************
//
//The following are values that can be passed to SD24B_init() in the
//clockSourceSelect parameter.
//
//*****************************************************************************
#define SD24B_CLOCKSOURCE_MCLK						(SD24SSEL__MCLK)
#define SD24B_CLOCKSOURCE_SMCLK						(SD24SSEL__SMCLK)
#define SD24B_CLOCKSOURCE_ACLK						(SD24SSEL__ACLK)
#define SD24B_CLOCKSOURCE_SD24CLK					(SD24SSEL__SD24CLK)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_init() in the
//clockPreDivider parameter.
//
//*****************************************************************************
#define SD24B_PRECLOCKDIVIDER_1						(SD24PDIV_0)
#define SD24B_PRECLOCKDIVIDER_2						(SD24PDIV_1)
#define SD24B_PRECLOCKDIVIDER_4						(SD24PDIV_2)
#define SD24B_PRECLOCKDIVIDER_8						(SD24PDIV_3)
#define SD24B_PRECLOCKDIVIDER_16					(SD24PDIV_4)
#define SD24B_PRECLOCKDIVIDER_32					(SD24PDIV_5)
#define SD24B_PRECLOCKDIVIDER_64					(SD24PDIV_6)
#define SD24B_PRECLOCKDIVIDER_128					(SD24PDIV_7)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_init() in the
//clockDivider parameter.
//
//*****************************************************************************
#define SD24B_CLOCKDIVIDER_1	(0x00)
#define SD24B_CLOCKDIVIDER_2	(SD24DIV0)
#define SD24B_CLOCKDIVIDER_3	(SD24DIV1)
#define SD24B_CLOCKDIVIDER_4	(SD24DIV1 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_5	(SD24DIV2)
#define SD24B_CLOCKDIVIDER_6	(SD24DIV2 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_7	(SD24DIV2 | SD24DIV1)
#define SD24B_CLOCKDIVIDER_8	(SD24DIV2 | SD24DIV1 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_9	(SD24DIV3)
#define SD24B_CLOCKDIVIDER_10	(SD24DIV3 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_11	(SD24DIV3 | SD24DIV1)
#define SD24B_CLOCKDIVIDER_12	(SD24DIV3 | SD24DIV1 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_13	(SD24DIV3 | SD24DIV2)
#define SD24B_CLOCKDIVIDER_14	(SD24DIV3 | SD24DIV2 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_15	(SD24DIV3 | SD24DIV2 | SD24DIV1)
#define SD24B_CLOCKDIVIDER_16	(SD24DIV3 | SD24DIV2 | SD24DIV1 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_17	(SD24DIV4)
#define SD24B_CLOCKDIVIDER_18	(SD24DIV4 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_19	(SD24DIV4 | SD24DIV1)
#define SD24B_CLOCKDIVIDER_20	(SD24DIV4 | SD24DIV1 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_21	(SD24DIV4 | SD24DIV2)
#define SD24B_CLOCKDIVIDER_22	(SD24DIV4 | SD24DIV2 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_23	(SD24DIV4 | SD24DIV2 | SD24DIV1)
#define SD24B_CLOCKDIVIDER_24	(SD24DIV4 | SD24DIV2 | SD24DIV1 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_25	(SD24DIV4 | SD24DIV3)
#define SD24B_CLOCKDIVIDER_26	(SD24DIV4 | SD24DIV3 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_27	(SD24DIV4 | SD24DIV3 | SD24DIV1)
#define SD24B_CLOCKDIVIDER_28	(SD24DIV4 | SD24DIV3 | SD24DIV1 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_29	(SD24DIV4 | SD24DIV3 | SD24DIV2)
#define SD24B_CLOCKDIVIDER_30	(SD24DIV4 | SD24DIV3 | SD24DIV2 | SD24DIV0)
#define SD24B_CLOCKDIVIDER_31	(SD24DIV4 | SD24DIV3 | SD24DIV2 | SD24DIV1)
#define SD24B_CLOCKDIVIDER_32	(SD24DIV4 | SD24DIV3 | \
								SD24DIV2 | SD24DIV1 | SD24DIV0)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_init() in the
//refrenceSelect parameter.
//
//*****************************************************************************
#define SD24B_REF_EXTERNAL							(0x00)
#define SD24B_REF_INTERNAL							(SD24REFS)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_configureConverter() and
//SD24B_configureConverterAdvanced() in the alignment parameter.
//
//*****************************************************************************
#define SD24B_ALIGN_RIGHT							(0x00)
#define SD24B_ALIGN_LEFT							(SD24ALGN)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_configureConverter() and
//SD24B_configureConverterAdvanced() in the startSelect parameter.
//
//*****************************************************************************
#define SD24B_CONVERSION_SELECT_SD24SC				(SD24SCS__SD24SC)
#define SD24B_CONVERSION_SELECT_EXT1				(SD24SCS__EXT1)
#define SD24B_CONVERSION_SELECT_EXT2				(SD24SCS__EXT2)
#define SD24B_CONVERSION_SELECT_EXT3				(SD24SCS__EXT3)
#define SD24B_CONVERSION_SELECT_GROUP0				(SD24SCS__GROUP0)
#define SD24B_CONVERSION_SELECT_GROUP1				(SD24SCS__GROUP1)
#define SD24B_CONVERSION_SELECT_GROUP2				(SD24SCS__GROUP2)
#define SD24B_CONVERSION_SELECT_GROUP3				(SD24SCS__GROUP3)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_configureConverter() and
//SD24B_configureConverterAdvanced() in the conversionMode parameter.
//
//*****************************************************************************
#define SD24B_CONTINUOUS_MODE						(0x00)
#define SD24B_SINGLE_MODE							(SD24SNGL_H)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_setConverterDataFormat()
//and SD24B_configureConverterAdvanced() in the dataFormat parameter.
//
//*****************************************************************************
#define SD24_DATA_FORMAT_BINARY						(SD24DF_0)
#define SD24_DATA_FORMAT_2COMPLEMENT				(SD24DF_1)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_configureDMATrigger()
//in the interruptFlag parameter.
//
//*****************************************************************************
#define SD24_DMA_TRIGGER_IFG0						(SD24DMA_0)
#define SD24_DMA_TRIGGER_IFG1						(SD24DMA_1)
#define SD24_DMA_TRIGGER_IFG2						(SD24DMA_2)
#define SD24_DMA_TRIGGER_IFG3						(SD24DMA_3)
#define SD24_DMA_TRIGGER_IFG4						(SD24DMA_4)
#define SD24_DMA_TRIGGER_IFG5						(SD24DMA_5)
#define SD24_DMA_TRIGGER_IFG6						(SD24DMA_6)
#define SD24_DMA_TRIGGER_IFG7						(SD24DMA_7)
#define SD24_DMA_TRIGGER_TRGIFG						(SD24DMA_8)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_setInterruptDelay()
//and SD24B_configureConverterAdvanced() in the sampleDelay parameter.
//
//*****************************************************************************
#define SD24B_FIRST_SAMPLE_INTERRUPT				(SD24INTDLY_3)
#define SD24B_SECOND_SAMPLE_INTERRUPT				(SD24INTDLY_2)
#define SD24B_THIRD_SAMPLE_INTERRUPT				(SD24INTDLY_1)
#define SD24B_FOURTH_SAMPLE_INTERRUPT				(SD24INTDLY_0)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_setOversampling()
//and SD24B_configureConverterAdvanced() in the oversampleRatio parameter.
//
//*****************************************************************************
#define SD24B_OVERSAMPLE_32							(OSR__32)
#define SD24B_OVERSAMPLE_64							(OSR__64)
#define SD24B_OVERSAMPLE_128						(OSR__128)
#define SD24B_OVERSAMPLE_256						(OSR__256)
#define SD24B_OVERSAMPLE_512						(OSR__512)
#define SD24B_OVERSAMPLE_1024						(OSR__1024)

//*****************************************************************************
//
//The following are values that can be passed to SD24B_setGain() and
//SD24B_configureConverterAdvanced() in the gain parameter.
//
//*****************************************************************************
#define SD24B_GAIN_1								(SD24GAIN_1)
#define SD24B_GAIN_2								(SD24GAIN_2)
#define SD24B_GAIN_4								(SD24GAIN_4)
#define SD24B_GAIN_8								(SD24GAIN_8)
#define SD24B_GAIN_16								(SD24GAIN_16)
#define SD24B_GAIN_32								(SD24GAIN_32)
#define SD24B_GAIN_64								(SD24GAIN_64)
#define SD24B_GAIN_128								(SD24GAIN_128)

//*****************************************************************************
//
//The following are values that can be passed to 
//SD24B_startGroupConversion() in the group parameter.
//
//*****************************************************************************
#define SD24B_GROUP0	0
#define SD24B_GROUP1	1
#define SD24B_GROUP2	2
#define SD24B_GROUP3	3

//*****************************************************************************
//
//The following are values that can be passed to 
//SD24B_startConverterConversion(), SD24B_getHighWordResults(),
//SD24B_getResults(), SD24B_setGain(),  SD24B_setOversampling()
//SD24B_setInterruptDelay(), SD24B_stopConverterConversion(),
//SD24B_startConverterConversion(), SD24B_enableInterrupt(), 
//SD24B_disableInterrupt(), SD24B_clearInterrupt(),
//SD24B_getInterruptStatus()  in the converter parameter.
//
//*****************************************************************************
#define SD24B_CONVERTER_0	0
#define SD24B_CONVERTER_1	1
#define SD24B_CONVERTER_2	2
#define SD24B_CONVERTER_3	3
#define SD24B_CONVERTER_4	4
#define SD24B_CONVERTER_5	5
#define SD24B_CONVERTER_6	6
#define SD24B_CONVERTER_7	7

//*****************************************************************************
//
//The following are values that can be passed to 
//SD24B_startGroupConversion() in the group parameter.
//
//*****************************************************************************
#define SD24_CONVERTER_INTERRUPT	SD24IE0
#define SD24_CONVERTER_OVERFLOW_INTERRUPT	SD24OVIE0

//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern void SD24B_init(unsigned int baseAddress,
		unsigned char clockSourceSelect,
		unsigned char clockPreDivider,
		unsigned char clockDivider,
		unsigned char refrenceSelect );

extern void SD24B_configureConverter(unsigned int baseAddress,
		unsigned char converter,
		unsigned char alignment,
		unsigned char startSelect,
		unsigned char conversionMode );

extern void SD24B_configureConverterAdvanced(unsigned int baseAddress,
		unsigned char converter,
		unsigned char alignment,
		unsigned char startSelect,
		unsigned char conversionMode,
		unsigned char dataFormat,
		unsigned char sampleDelay,
		unsigned int oversampleRatio,
		unsigned char gain );

extern void SD24B_setConverterDataFormat(unsigned int baseAddress,
		unsigned char converter,
		unsigned char dataFormat);

extern void SD24B_startGroupConversion(unsigned int baseAddress,
		unsigned char group);

extern void SD24B_stopGroupConversion(unsigned int baseAddress,
		unsigned char group);

extern void SD24B_startConverterConversion(unsigned int baseAddress,
		unsigned char converter);

extern void SD24B_stopConverterConversion(unsigned int baseAddress,
		unsigned char converter);

extern void SD24B_configureDMATrigger(unsigned int baseAddress,
		unsigned char interruptFlag);

extern void SD24B_setInterruptDelay(unsigned int baseAddress,
		unsigned char converter,
		unsigned char sampleDelay);

extern void SD24B_setOversampling(unsigned int baseAddress,
		unsigned char converter,
		unsigned int oversampleRatio);

extern void SD24B_setGain(unsigned int baseAddress,
		unsigned char converter,
		unsigned char gain);

extern unsigned long SD24B_getResults(unsigned int baseAddress,
		unsigned char converter);

extern unsigned int SD24B_getHighWordResults(unsigned int baseAddress,
		unsigned char converter);

extern void SD24B_enableInterrupt (unsigned int baseAddress,
	unsigned char converter,
    unsigned int mask);

extern void SD24B_disableInterrupt (unsigned int baseAddress,
	unsigned char converter,
    unsigned int mask);

extern void SD24B_clearInterrupt (unsigned int baseAddress,
	unsigned char converter,
    unsigned int mask);

extern unsigned int SD24B_getInterruptStatus (unsigned int baseAddress,
	unsigned char converter,
    unsigned int mask);



#endif

