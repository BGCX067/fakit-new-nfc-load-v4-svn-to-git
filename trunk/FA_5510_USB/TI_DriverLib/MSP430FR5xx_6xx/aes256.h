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
#ifndef __MSP430WARE_AES256_H__
#define __MSP430WARE_AES256_H__

//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
//*****************************************************************************
#define __MSP430_HAS_AES256__

#include "inc/hw_regaccess.h"

//*****************************************************************************
//
//The following value can be passed to AES256_clearInterruptFlag(),
//AES256_enableInterrupt(), AES256_disableInterrupt()
//
//*****************************************************************************
#define AES256_READY_INTERRUPT AESRDYIE

//*****************************************************************************
//
//The following value can be returned by AES256_getErrorFlagStatus
//
//*****************************************************************************
#define AES256_ERROR_OCCURRED	AESERRFG
#define AES256_NO_ERROR			0x00

//*****************************************************************************
//
//The following value can be passed to AES256_enableInterrupt()
//
//*****************************************************************************
#define		AES_getEncryptedData	AES_getDataOut
#define		AES_getDecryptedData	AES_getDataOut

//*****************************************************************************
//
//The following value is returned by the AES256_isBusy() API
//
//*****************************************************************************
#define AES256_BUSY		AESBUSY
#define AES256_NOT_BUSY	0x00

//*****************************************************************************
//
//The following value can be passed to AES256 functions as keyLength
//
//*****************************************************************************
#define AES256_KEYLENGTH_128BIT	128
#define AES256_KEYLENGTH_192BIT	192
#define AES256_KEYLENGTH_256BIT	256

//*****************************************************************************
//
//The following value can be passed to AES_enableInterrupt()
//
//*****************************************************************************
#define AES_INTERRUPT_ENABLE AESRDYI

//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern uint8_t AES256_setCipherKey (uint32_t baseAddress,
	const uint8_t * CipherKey, 
 uint16_t keyLength);
extern uint8_t AES256_encryptData (uint32_t baseAddress,
	const uint8_t * Data, 
	uint8_t * encryptedData);
extern uint8_t AES256_decryptDataUsingEncryptionKey (uint32_t baseAddress,
	const uint8_t * Data, 
	uint8_t * decryptedData);
extern uint8_t AES256_decryptData (uint32_t baseAddress,
	const uint8_t * Data, 
	uint8_t * decryptedData);
extern uint8_t AES256_generateFirstRoundKey (uint32_t baseAddress,
	const uint8_t * CipherKey);
extern void AES256_clearInterruptFlag (uint32_t baseAddress );
extern uint32_t AES256_getInterruptFlagStatus (uint32_t baseAddress);
extern void AES256_enableInterrupt (uint32_t baseAddress);
extern void AES256_disableInterrupt (uint32_t baseAddress);
extern void AES256_reset (uint32_t baseAddress);
extern uint8_t AES256_startEncryptData (uint32_t baseAddress,
	const uint8_t * Data, 
	uint8_t * encryptedData);
extern uint8_t AES256_startDecryptDataUsingEncryptionKey (
	uint32_t baseAddress, 
	const uint8_t * Data);
extern uint8_t AES256_startDecryptData (uint32_t baseAddress,
	const uint8_t * Data);
extern uint8_t AES256_startGenerateFirstRoundKey (uint32_t baseAddress,
	const uint8_t * CipherKey);
extern uint8_t  AES256_getDataOut(uint32_t baseAddress,
							uint8_t *OutputData
							);
extern uint8_t AES256_isBusy (uint32_t baseAddress);
extern void AES256_clearErrorFlag (uint32_t baseAddress );
extern uint32_t AES256_getErrorFlagStatus (uint32_t baseAddress);

//*****************************************************************************
//
//Deprecated
//The following value can be passed to AES256 functions as keyLength
//
//*****************************************************************************

#define Key_128BIT	128
#define Key_192BIT	192
#define Key_256BIT	256


#endif

