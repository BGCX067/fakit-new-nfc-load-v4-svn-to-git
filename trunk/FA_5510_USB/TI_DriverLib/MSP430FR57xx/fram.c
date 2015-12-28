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
//
//fram.c - Driver for the FRAM Module.
//
//*****************************************************************************
#include "inc/hw_regaccess.h"
#include "fram.h"
#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430xgeneric.h"
#else
#include "deprecated/CCS/msp430xgeneric.h"
#endif
#include "assert.h"

//*****************************************************************************
//
//! Write data into the fram memory in byte format.
//!
//! \param baseAddress is the base address of the FRAM module.
//! \param dataPtr is the pointer to the data to be written
//! \param framPtr is the pointer into which to write the data
//! \param numberOfBytes is the number of bytes to be written
//!
//! \returns NONE
//
//*****************************************************************************


void FRAM_write8(uint32_t baseAddress,
    uint8_t *dataPtr,
    uint8_t *framPtr,
    uint16_t numberOfBytes
    )
{

    while (numberOfBytes > 0)
    {
        //test busy
        while (HWREG8(baseAddress + OFS_GCCTL0) & BUSY) ;

        //Write to Fram
        *framPtr++ = *dataPtr++;
        numberOfBytes--;
    }

}

//*****************************************************************************
//
//! Write data into the fram memory in word format.
//!
//! \param baseAddress is the base address of the Fram module.
//! \param dataPtr is the pointer to the data to be written
//! \param framPtr is the pointer into which to write the data
//! \param numberOfBytes is the number of bytes to be written
//!
//! \returns NONE
//
//*****************************************************************************

void FRAM_write16(uint32_t baseAddress,
		uint16_t *dataPtr,uint16_t *framPtr,
		uint16_t numberOfWords)
{

    while (numberOfWords > 0)
    {
        //test busy
        while (HWREG8(baseAddress + OFS_GCCTL0) & BUSY) ;

        //Write to Fram
        *framPtr++ = *dataPtr++;
        numberOfWords--;
    }

}

//*****************************************************************************
//
//! Write data into the fram memory in long format, pass by reference
//!
//! \param baseAddress is the base address of the Fram module.
//! \param dataPtr is the pointer to the data to be written
//! \param framPtr is the pointer into which to write the data
//! \param numberOfBytes is the number of bytes to be written
//!
//! \returns NONE
//
//*****************************************************************************

void FRAM_write32(uint32_t baseAddress,
		uint32_t *dataPtr,uint32_t *framPtr,
		uint16_t count)
{

    while (count > 0)
    {
        //test busy
        while (HWREG8(baseAddress + OFS_GCCTL0) & BUSY) ;

        //Write to Fram
        *framPtr++ = *dataPtr++;
        count--;
    }

}

//*****************************************************************************
//
//! Write data into the fram memory in long format, pass by value
//!
//! \param baseAddress is the base address of the Flash module.
//! \param value is the value to written to FRAM memory
//! \param framPtr is the pointer into which to write the data
//! \param numberOfBytes is the number of bytes to be written
//!
//! \returns NONE
//
//*****************************************************************************
void FRAM_memoryFill32 (uint32_t baseAddress,
    uint32_t value,
    uint32_t *framPtr,
    uint16_t count
    )
{
    while (count> 0)
    {
        //test busy
        while (HWREG8(baseAddress + OFS_GCCTL0) & BUSY) ;

        //Write to Fram
        *framPtr++ = value;
        count--;
    }
}


//*****************************************************************************
//
//! Enables selected FRAM interrupt sources.
//!
//!
//! \param baseAddress is the base address of the FRAM module.
//! \param interruptMask is the bit mask of the memory buffer interrupt
//!       sources to be disabled.
//!        Mask Value is the logical OR of any of the following
//!		   \b FRAM_PUC_ON_UNCORRECTABLE_BIT - Enable PUC reset if FRAM
//!				uncorrectable bit error detected.
//!        \b FRAM_UNCORRECTABLE_BIT_INTERRUPT - Interrupts when an
//!				uncorrectable bit error is detected.
//!        \b FRAM_CORRECTABLE_BIT_INTERRUPT - Interrupts when
//!				a correctable bit error is detected.
//!        \b FRAM_ACCESS_VIOLATION_INTERRUPT - Interrupts when an
//!				access violation occurs.
//!		   \b FRAM_ACCESS_TIME_ERROR_INTERRUPT - Interrupts when an
//!				access time error occurs.
//!
//!			If header file contains legacy definitions:
//!		   \b FRAM_PUC_ON_DOUBLE_BIT_ERROR - Enable PUC reset if FRAM
//!				detects double bit error is detected.
//!		   \b FRAM_DOUBLE_BIT_ERROR_INTERRUPT - Interrupts when a double
//!				bit error is detected.
//!		   \b FRAM_SINGLE_BIT_ERROR_INTERRUPT - Interrupts when a single
//!				bit error is detected.
//!
//! Disables the indicated FRAM interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor.
//!
//!	//! <b>Does not clear interrupt flags.</b>
//! Modified registers are \b FRCTL0, \b GCCTL0 and \b GCCTL1.
//!
//! \return NONE
//
//*****************************************************************************

void FRAM_enableInterrupt (uint32_t baseAddress,
    uint8_t interruptMask)
{

	uint16_t waitSelection;

	waitSelection=(HWREG16(baseAddress + OFS_FRCTL0) & 0x00FF);
	// Clear lock in FRAM control registers
	HWREG16(baseAddress + OFS_FRCTL0) = FWPW + waitSelection;

	// Enable user selected interrupt sources
    HWREG16(baseAddress + OFS_GCCTL0) |= interruptMask;

}


//*****************************************************************************
//
//! Returns the status of the selected FRAM interrupt flags.
//!
//! \param baseAddress is the base address of the FRAM module.
//! \param interruptFlagMask is a bit mask of the interrupt flags status
//!       to be returned.
//!        Mask Value is the logical OR of any of the following
//!        \b FRAM_ACCESS_TIME_ERROR_FLAG.- Interrupt flag is set if a wrong
//!			  	setting for NPRECHG and NACCESS is set and FRAM access time
//!				is not hold.
//!        \b FRAM_UNCORRECTABLE_BIT_FLAG.- Interrupt flag is set if
//!				an uncorrectable bit error has been detected in the FRAM memory
//!				error detection logic.
//!        \b FRAM_CORRECTABLE_BIT_FLAG.- Interrupt flag is set if a correctable
//!				bit error has been detected and corrected in the FRAM memory
//!				error detection logic.
//!        \b FRAM_ACCESS_VIOLATION_FLAG.- Interrupt flag is set if an
//!				access violation is triggered.
//!
//!			If header file contains legacy definitions:
//!		   \b FRAM_SINGLE_BIT_ERROR_FLAG - Interrupt flag is set if a correctable bit error
//!				has been detected and corrected in the FRAM memory error
//!				detection logic.
//!        \b FRAM_DOUBLE_BIT_ERROR_FLAG.- Interrupt flag is set if an uncorrectable bit error
//!				has been detected in the FRAM memory error detection logic.
//!
//!
//! \return The current interrupt flag status for the corresponding mask.
//
//*****************************************************************************


uint8_t FRAM_getInterruptStatus(uint32_t baseAddress,
		uint16_t interruptFlagMask)
{
	return ( HWREG16(baseAddress + OFS_GCCTL1) & interruptFlagMask );
}

//*****************************************************************************
//
//! Disables selected FRAM interrupt sources.
//!
//! \param baseAddress is the base address of the FRAM module.
//! \param interruptMask is the bit mask of the memory buffer interrupt
//!       sources to be disabled.
//!        Mask Value is the logical OR of any of the following
//!		   \b FRAM_PUC_ON_UNCORRECTABLE_BIT - Enable PUC reset if FRAM
//!				uncorrectable bit error detected.
//!        \b FRAM_UNCORRECTABLE_BIT_INTERRUPT - Interrupts when an
//!				uncorrectable bit error is detected.
//!        \b FRAM_CORRECTABLE_BIT_INTERRUPT - Interrupts when
//!				a correctable bit error is detected.
//!        \b FRAM_ACCESS_VIOLATION_INTERRUPT - Interrupts when an
//!				access violation occurs.
//!		   \b FRAM_ACCESS_TIME_ERROR_INTERRUPT - Interrupts when an
//!				access time error occurs.
//!
//!			If header file contains legacy definitions:
//!		   \b FRAM_PUC_ON_DOUBLE_BIT_ERROR - Enable PUC reset if FRAM
//!				detects double bit error is detected.
//!		   \b FRAM_DOUBLE_BIT_ERROR_INTERRUPT - Interrupts when a double
//!				bit error is detected.
//!		   \b FRAM_SINGLE_BIT_ERROR_INTERRUPT - Interrupts when a single
//!				bit error is detected.
//!
//! Disables the indicated FRAM interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor.
//! Modified registers are \b FRCTL0 and \b GCCTL0.
//!
//! \return NONE
//
//*****************************************************************************

void FRAM_disableInterrupt(uint32_t baseAddress,
		uint16_t interruptMask)
{
	uint16_t waitSelection;

	waitSelection=(HWREG16(baseAddress + OFS_FRCTL0) & 0x00FF);
	//Clear lock in FRAM control registers
	HWREG16(baseAddress + OFS_FRCTL0) = FWPW + waitSelection;

    HWREG16(baseAddress + OFS_GCCTL0) &= ~(interruptMask);
}

//*****************************************************************************
//
//! Returns the busy status of the FRAM core.
//!
//! \param baseAddress is the base address of the FRAM module.
//!
//! Returns a busy status if the FRAM is currently busy programming.
//!
//! \return FRAM_BUSY or FRAM_NOTBUSY dependent FRAM is currently busy
//!	programming.
//
//*****************************************************************************

unsigned short FRAM_isBusy(uint32_t baseAddress)
{
	if (HWREG16(baseAddress + OFS_GCCTL0) & BUSY){
		return ( FRAM_BUSY) ;
	} else   {
		return ( FRAM_NOTBUSY) ;
	}
}

//*****************************************************************************
//
//! Configures the wait state control of the FRAM module
//!
//! \param baseAddress is the base address of the FRAM module.
//! \param manualWaitState chooses if the wait state control is manual or
//!      automatic
//!      Valid values are:
//!      \b FRAM_AUTO_MODE
//!      \b FRAM_MANUAL_MODE
//! \param accessTime defines the number of CPU cycles required for access time
//!      defined in the datasheet
//!      Valid values are:
//!      \b FRAM_ACCESS_TIME_CYCLES_0
//!      \b FRAM_ACCESS_TIME_CYCLES_1
//!      \b FRAM_ACCESS_TIME_CYCLES_2
//!      \b FRAM_ACCESS_TIME_CYCLES_3
//!      \b FRAM_ACCESS_TIME_CYCLES_4
//!      \b FRAM_ACCESS_TIME_CYCLES_5
//!      \b FRAM_ACCESS_TIME_CYCLES_6
//!      \b FRAM_ACCESS_TIME_CYCLES_7
//! \param prechargeTime defines the number of CPU cycles required for precharge
//!      time
//!      Valid values are:
//!      \b FRAM_PRECHARGE_TIME_CYCLES_0
//!      \b FRAM_PRECHARGE_TIME_CYCLES_1
//!      \b FRAM_PRECHARGE_TIME_CYCLES_2
//!      \b FRAM_PRECHARGE_TIME_CYCLES_3
//!      \b FRAM_PRECHARGE_TIME_CYCLES_4
//!      \b FRAM_PRECHARGE_TIME_CYCLES_5
//!      \b FRAM_PRECHARGE_TIME_CYCLES_6
//!      \b FRAM_PRECHARGE_TIME_CYCLES_7
//!
//! Configures the wait state control of the FRAM module. If using the
//! FRAM_AUTO_MODE the values for accessTime and prechargeTime do not matter.
//!
//! \return none
//
//*****************************************************************************

void FRAM_configureWaitStateControl(uint32_t baseAddress,
		uint8_t manualWaitState,
		uint8_t accessTime,
		uint8_t prechargeTime )
{

	// Clear lock in FRAM control registers
	HWREG16(baseAddress + OFS_FRCTL0) = FWPW;

	HWREG8(baseAddress + OFS_FRCTL0_L) |= ( accessTime | manualWaitState | prechargeTime);
}

//*****************************************************************************
//
//Close the Doxygen group.
//! @}
//
//*****************************************************************************








