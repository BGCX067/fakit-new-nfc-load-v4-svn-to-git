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
//compe.c - Driver for the COMPARATOR_E Module.
//
//*****************************************************************************
#include "inc/hw_regaccess.h"
#include "comp_e.h"
#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430xgeneric.h"
#else
#include "deprecated/CCS/msp430xgeneric.h"
#endif
#include "assert.h"

//*****************************************************************************
//
//! Initializes the Comparator Module.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param positiveTerminalInput selects the input to the positive terminal.
//!        Valid values are
//!        \b COMP_E_INPUT0 [Default]
//!        \b COMP_E_INPUT1
//!        \b COMP_E_INPUT2
//!        \b COMP_E_INPUT3
//!        \b COMP_E_INPUT4
//!        \b COMP_E_INPUT5
//!        \b COMP_E_INPUT6
//!        \b COMP_E_INPUT7
//!        \b COMP_E_INPUT8
//!        \b COMP_E_INPUT9
//!        \b COMP_E_INPUT10
//!        \b COMP_E_INPUT11
//!        \b COMP_E_INPUT12
//!        \b COMP_E_INPUT13
//!        \b COMP_E_INPUT14
//!        \b COMP_E_INPUT15
//!        \b COMP_E_VREF
//!        Modified bits are \b CEIPSEL and \b CEIPEN of \b CECTL0 register,
//!        \b CERSEL of \b CECTL2 register, and CEPDx of \b CECTL3 register.
//! \param negativeTerminalInput selects the input to the negative terminal.
//!        Valid values are
//!        \b COMP_E_INPUT0 [Default]
//!        \b COMP_E_INPUT1
//!        \b COMP_E_INPUT2
//!        \b COMP_E_INPUT3
//!        \b COMP_E_INPUT4
//!        \b COMP_E_INPUT5
//!        \b COMP_E_INPUT6
//!        \b COMP_E_INPUT7
//!        \b COMP_E_INPUT8
//!        \b COMP_E_INPUT9
//!        \b COMP_E_INPUT10
//!        \b COMP_E_INPUT11
//!        \b COMP_E_INPUT12
//!        \b COMP_E_INPUT13
//!        \b COMP_E_INPUT14
//!        \b COMP_E_INPUT15
//!        \b COMP_E_VREF
//!        Modified bits are \b CEIMSEL and \b CEIMEN of \b CECTL0 register,
//!        \b CERSEL of \b CECTL2 register, and CEPDx of \b CECTL3 register.
//! \param outputFilterEnableAndDelayLevel controls the output filter delay
//!       state, which is either off or enabled with a specified delay level.
//!        Valid values are
//!        \b COMP_E_FILTEROUTPUT_OFF [Default]
//!        \b COMP_E_FILTEROUTPUT_DLYLVL1
//!        \b COMP_E_FILTEROUTPUT_DLYLVL2
//!        \b COMP_E_FILTEROUTPUT_DLYLVL3
//!        \b COMP_E_FILTEROUTPUT_DLYLVL4
//!        This parameter is device specific and delay levels should be found in
//!        the device's datasheet.
//!        Modified bits are \b CEF and \b CEFDLY of \b CECTL1 register.
//! \param invertedOutput controls if the output will be inverted or not
//!        Valid values are
//!        \b COMP_E_NORMALOUTPUTPOLARITY - indicates the output should be normal.
//!             [Default]
//!        \b COMP_E_INVERTEDOUTPUTPOLARITY -  the output should be inverted.
//!        Modified bits are \b CEOUTPOL of \b CECTL1 register.
//!
//! Upon successful initialization of the Comparator module, this function will
//! have reset all necessary register bits and set the given options in the
//! registers. To actually use the comparator module, the COMP_E_enable()
//! function must be explicitly called before use.
//! If a Reference Voltage is set to a terminal, the Voltage should be set
//! using the setReferenceVoltage() function.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process.
//
//*****************************************************************************
unsigned short COMP_E_init (uint32_t baseAddress,
    uint8_t positiveTerminalInput,
    uint8_t negativeTerminalInput,
    uint8_t outputFilterEnableAndDelayLevel,
    unsigned short invertedOutputPolarity)
{
    assert(positiveTerminalInput <= COMP_E_VREF);
    assert(negativeTerminalInput <= COMP_E_VREF);
    assert(positiveTerminalInput != negativeTerminalInput);
    assert(outputFilterEnableAndDelayLevel <= COMP_E_FILTEROUTPUT_DLYLVL4);

    uint8_t retVal = STATUS_SUCCESS;

    //Reset COMPE Control 1 & Interrupt Registers for initialization (OFS_CECTL3
    //is not reset because it controls the input buffers of the analog signals
    //and may cause parasitic effects if an analog signal is still attached and
    //the buffer is re-enabled
    HWREG16(baseAddress + OFS_CECTL0) &= 0x0000;
    HWREG16(baseAddress + OFS_CEINT)  &= 0x0000;

    //Set the Positive Terminal
    if (COMP_E_VREF != positiveTerminalInput){
        //Enable Positive Terminal Input Mux and Set it to the appropriate input
        HWREG16(baseAddress + OFS_CECTL0) |= CEIPEN + positiveTerminalInput;

        //Disable the input buffer
        HWREG16(baseAddress + OFS_CECTL3) |= (1 << positiveTerminalInput);
    } else {
        //Reset and Set COMPE Control 2 Register
        HWREG16(baseAddress + OFS_CECTL2) = ~(CERSEL); //Set Vref to go to (+)terminal
    }

    //Set the Negative Terminal
    if (COMP_E_VREF != negativeTerminalInput){
        //Enable Negative Terminal Input Mux and Set it to the appropriate input
        HWREG16(baseAddress + OFS_CECTL0) |= CEIMEN + (negativeTerminalInput << 8);

        //Disable the input buffer
        HWREG16(baseAddress + OFS_CECTL3) |= (1 << negativeTerminalInput);
    } else {
        //Reset and Set COMPE Control 2 Register
        HWREG16(baseAddress + OFS_CECTL2) = CERSEL; //Set Vref to go to (-) terminal
    }

    //Reset and Set COMPE Control 1 Register
    HWREG16(baseAddress + OFS_CECTL1) =
        + outputFilterEnableAndDelayLevel //Set the filter enable bit and delay
        + invertedOutputPolarity; //Set the polarity of the output

    return ( retVal) ;
}
//*****************************************************************************
//
//! Generates a Reference Voltage to the terminal selected during initialization.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param supplyVoltageReferenceBase decides the source and max amount of Voltage that
//!       can be used as a reference.
//!        Valid values are
//!        \b COMP_E_REFERENCE_AMPLIFIER_DISABLED
//!        \b COMP_E_VREFBASE1_2V
//!        \b COMP_E_VREFBASE2_0V
//!        \b COMP_E_VREFBASE2_5V
//!        Modified bits are \b CEREFL of \b CECTL2 register.
//! \param upperLimitSupplyVoltageFractionOf32 is the numerator of the equation
//!       to generate the reference voltage for the upper limit reference voltage.
//!        Modified bits are \b CEREF1 of \b CECTL2 register.
//! \param lowerLimitSupplyVoltageFractionOf32 is the numerator of the equation
//!       to generate the reference voltage for the lower limit reference voltage.
//!        Modified bits are \b CEREF0 of \b CECTL2 register.
//!
//! Use this function to generate a voltage to serve as a reference to the
//! terminal selected at initialization. The voltage is determined by the
//! equation: Vbase * (Numerator / 32). If the upper and lower limit voltage
//! numerators are equal, then a static reference is defined, whereas they are
//! different then a hysteresis effect is generated.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_setReferenceVoltage (uint32_t baseAddress,
    uint16_t supplyVoltageReferenceBase,
    uint16_t lowerLimitSupplyVoltageFractionOf32,
    uint16_t upperLimitSupplyVoltageFractionOf32)
{
    assert(supplyVoltageReferenceBase <= COMP_E_VREFBASE2_5V);
    assert(upperLimitSupplyVoltageFractionOf32 <= 32);
    assert(lowerLimitSupplyVoltageFractionOf32 <= 32);
    assert(upperLimitSupplyVoltageFractionOf32
        >= lowerLimitSupplyVoltageFractionOf32);

    HWREG16(baseAddress + OFS_CECTL1) &= ~(CEMRVS); //Set to VREF0

    //Reset COMPE Control 2 Bits (Except for CERSEL which is set in Comp_Init() )
    HWREG16(baseAddress + OFS_CECTL2) &= CERSEL;

    //Set Voltage Source (Vcc | Vref, resistor ladder or not)
    if (COMP_E_VREFBASE_VCC == supplyVoltageReferenceBase){
        HWREG16(baseAddress + OFS_CECTL2) |= CERS_1; //Vcc with resistor ladder
    } else if (lowerLimitSupplyVoltageFractionOf32 == 32){
        //If the lower limit is 32, then the upper limit has to be 32 due to the
        //assertion that upper must be >= to the lower limit. If the numerator is
        //equal to 32, then the equation would be 32/32 == 1, therefore no resistor
        //ladder is needed
        HWREG16(baseAddress + OFS_CECTL2) |= CERS_3; //Vref, no resistor ladder
    } else {
        HWREG16(baseAddress + OFS_CECTL2) |= CERS_2; //Vref with resistor ladder
    }

    //Set COMPE Control 2 Register
    HWREG16(baseAddress + OFS_CECTL2) |=
        supplyVoltageReferenceBase //Set Supply Voltage Base
        + ((upperLimitSupplyVoltageFractionOf32 - 1) << 8) //Set Supply Voltage Num.
        + (lowerLimitSupplyVoltageFractionOf32 - 1);
}

//*****************************************************************************
//
//! Sets the reference accuracy 
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param referenceAccuracy is the refrence accuracy setting of the comparator. Clocked
//!      is for low power/low accuracy.
//!      Valid values are
//!      \b COMP_E_ACCURACY_STATIC
//!      \b COMP_E_ACCURACY_CLOCKED
//!      Modified bits are \b CEREFACC of \b CECTL2 register.
//!
//! The reference accuracy is set to the desired setting. Clocked is better for low power
//! operations but has a lower accuracy.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_setReferenceAccuracy (uint32_t baseAddress,
	uint16_t referenceAccuracy)
{
	assert(
	  (referenceAccuracy == COMP_E_ACCURACY_STATIC) ||
	  (referenceAccuracy == COMP_E_ACCURACY_CLOCKED)
	);
	
    HWREG16(baseAddress + OFS_CECTL2) &= ~(CEREFACC);
    HWREG16(baseAddress + OFS_CECTL2) |= referenceAccuracy;
}

//*****************************************************************************
//
//! Sets the power mode 
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param powerMode decides the power mode 
//!        Valid values are
//!        \b COMP_E_HIGH_SPEED_MODE
//!        \b COMP_E_NORMAL_MODE
//!        \b COMP_E_ULTRA_LOW_POWER_MODE
//!        Modified bits are \b CEPWRMD of \b CECTL1 register.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_setPowerMode (uint32_t baseAddress,
	uint16_t powerMode)
{
	HWREG16(baseAddress + OFS_CECTL1) &= ~(COMP_E_NORMAL_MODE | COMP_E_ULTRA_LOW_POWER_MODE);
	
	HWREG16(baseAddress + OFS_CECTL1) |= powerMode;
}

//*****************************************************************************
//
//! Enables selected Comparator interrupt sources.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is the bit mask of the interrupt sources to be enabled.
//!        Mask value is the logical OR of any of the following
//!        \b COMP_E_OUTPUT_INTERRUPT - Output interrupt
//!        \b COMP_E_INVERTED_POLARITY_INTERRUPT - Output interrupt inverted polarity
//!        \b COMP_E_READY_INTERRUPT - Ready interrupt
//!
//! Enables the indicated Comparator interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor. <b>Does not clear interrupt flags.</b>
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_enableInterrupt (uint32_t baseAddress,
    uint16_t interruptMask)
{
    //Set the Interrupt enable bit
    HWREG16(baseAddress + OFS_CEINT) |= interruptMask;
}

//*****************************************************************************
//
//! Disables selected Comparator interrupt sources.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is the bit mask of the interrupt sources to be disabled.
//!        Mask value is the logical OR of any of the following
//!        \b COMP_E_OUTPUT_INTERRUPT - Output interrupt
//!        \b COMP_E_INVERTED_POLARITY_INTERRUPT - Output interrupt inverted polarity
//!        \b COMP_E_READY_INTERRUPT - Ready interrupt 
//!
//! Disables the indicated Comparator interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_disableInterrupt (uint32_t baseAddress,
    uint16_t interruptMask)
{
    HWREG16(baseAddress + OFS_CEINT) &= ~(interruptMask);
}

//*****************************************************************************
//
//! Clears Comparator interrupt flags.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is a bit mask of the interrupt sources to be cleared.
//!        Mask value is the logical OR of any of the following
//!        \b COMP_E_OUTPUT_INTERRUPT_FLAG - Output interrupt flag
//!        \b COMP_E_INTERRUPT_FLAG_INVERTED_POLARITY - Output interrupt flag inverted polarity
//!        \b COMP_E_INTERRUPT_FLAG_READY - Ready interrupt flag
//!
//! The Comparator interrupt source is cleared, so that it no longer asserts.
//! The highest interrupt flag is automatically cleared when an interrupt vector
//! generator is used.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_clearInterrupt (uint32_t baseAddress,
    uint16_t interruptFlagMask)
{
    HWREG16(baseAddress + OFS_CEINT) &= ~(interruptFlagMask);
}

//*****************************************************************************
//
//! Gets the current Comparator interrupt status.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following
//!        \b COMP_E_OUTPUT_INTERRUPT_FLAG - Output interrupt flag
//!        \b COMP_E_INTERRUPT_FLAG_INVERTED_POLARITY - Output interrupt flag inverted polarity
//!        \b COMP_E_INTERRUPT_FLAG_READY - Ready interrupt flag
//!
//! This returns the interrupt status for the Comparator module based on which
//! flag is passed.
//!
//! \return The current interrupt flag status for the corresponding mask.
//
//*****************************************************************************
uint8_t COMP_E_getInterruptStatus (uint32_t baseAddress,
    uint16_t interruptFlagMask)
{
    return ( HWREG16(baseAddress + OFS_CEINT) & interruptFlagMask );
}

//*****************************************************************************
//
//! Explicitly sets the edge direction that would trigger an interrupt.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param edgeDirection determines which direction the edge would have to go to
//!       generate an interrupt based on the non-inverted interrupt flag.
//!        Valid values are
//!        \b COMP_E_FALLINGEDGE - sets the bit to generate an interrupt when the
//!             output of the comparator falls from HIGH to LOW if the normal
//!             interrupt bit is set(and LOW to HIGH if the inverted interrupt
//!             enable bit is set). [Default]
//!        \b COMP_E_RISINGEDGE - sets the bit to generate an interrupt when the
//!             output of the comparator rises from LOW to HIGH if the normal
//!             interrupt bit is set(and HIGH to LOW if the inverted interrupt
//!             enable bit is set).
//!        Modified bits are \b CEIES of \b CECTL1 register.
//!
//! This function will set which direction the output will have to go, whether
//! rising or falling, to generate an interrupt based on a non-inverted
//! interrupt.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_interruptSetEdgeDirection (uint32_t baseAddress,
    unsigned short edgeDirection)
{
    assert(edgeDirection <= COMP_E_RISINGEDGE);

    //Set the edge direction that will trigger an interrupt
    if (COMP_E_RISINGEDGE == edgeDirection){
        HWREG16(baseAddress + OFS_CECTL1) |= CEIES;
    } else if (COMP_E_FALLINGEDGE == edgeDirection){
        HWREG16(baseAddress + OFS_CECTL1) &= ~(CEIES);
    }
}

//*****************************************************************************
//
//! Toggles the edge direction that would trigger an interrupt.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! This function will toggle which direction the output will have to go,
//! whether rising or falling, to generate an interrupt based on a non-inverted
//! interrupt. If the direction was rising, it is now falling, if it was
//! falling, it is now rising.
//!
//! Modified bits are \b CEIES of \b CECTL1 register.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_interruptToggleEdgeDirection (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_CECTL1) ^= CEIES;
}

//*****************************************************************************
//
//! Turns on the Comparator module.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! This function sets the bit that enables the operation of the
//! Comparator module.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_enable (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_CECTL1) |= CEON;
}

//*****************************************************************************
//
//! Turns off the Comparator module.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! This function clears the CEON bit disabling the operation of the Comparator
//! module, saving from excess power consumption.
//!
//! Modified bits are \b CEON of \b CECTL1 register.
//! \return NONE
//
//*****************************************************************************
void COMP_E_disable (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_CECTL1) &= ~(CEON);
}

//*****************************************************************************
//
//! Shorts the two input pins chosen during initialization.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! This function sets the bit that shorts the devices attached to the input
//! pins chosen from the initialization of the comparator.
//!
//! Modified bits are \b CESHORT of \b CECTL1 register.
//! \return NONE
//
//*****************************************************************************
void COMP_E_shortInputs (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_CECTL1) |= CESHORT;
}

//*****************************************************************************
//
//! Disables the short of the two input pins chosen during initialization.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! This function clears the bit that shorts the devices attached to the input
//! pins chosen from the initialization of the comparator.
//!
//! Modified bits are \b CESHORT of \b CECTL1 register.
//! \return NONE
//
//*****************************************************************************
void COMP_E_unshortInputs (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_CECTL1) &= ~(CESHORT);
}

//*****************************************************************************
//
//! Disables the input buffer of the selected input port to effectively allow
//! for analog signals.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param inputPort is the port in which the input buffer will be disabled.
//!        Valid values are
//!        \b COMP_E_INPUT0 [Default]
//!        \b COMP_E_INPUT1
//!        \b COMP_E_INPUT2
//!        \b COMP_E_INPUT3
//!        \b COMP_E_INPUT4
//!        \b COMP_E_INPUT5
//!        \b COMP_E_INPUT6
//!        \b COMP_E_INPUT7
//!        \b COMP_E_INPUT8
//!        \b COMP_E_INPUT9
//!        \b COMP_E_INPUT10
//!        \b COMP_E_INPUT11
//!        \b COMP_E_INPUT12
//!        \b COMP_E_INPUT13
//!        \b COMP_E_INPUT14
//!        \b COMP_E_INPUT15
//!        \b COMP_E_VREF
//!        Modified bits are \b CEPDx of \b CECTL3 register.
//!
//! This function sets the bit to disable the buffer for the specified input
//! port to allow for analog signals from any of the comparator input pins. This
//! bit is automatically set when the input is initialized to be used with the
//! comparator module. This function should be used whenever an analog input is
//! connected to one of these pins to prevent parasitic voltage from causing
//! unexpected results.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_disableInputBuffer (uint32_t baseAddress,
    uint8_t inputPort)
{
    HWREG16(baseAddress + OFS_CECTL3) |= (1 << inputPort);
}

//*****************************************************************************
//
//! Enables the input buffer of the selected input port to allow for digital
//! signals.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param inputPort is the port in which the input buffer will be enabled.
//!        Valid values are
//!        \b COMP_E_INPUT0 [Default]
//!        \b COMP_E_INPUT1
//!        \b COMP_E_INPUT2
//!        \b COMP_E_INPUT3
//!        \b COMP_E_INPUT4
//!        \b COMP_E_INPUT5
//!        \b COMP_E_INPUT6
//!        \b COMP_E_INPUT7
//!        \b COMP_E_INPUT8
//!        \b COMP_E_INPUT9
//!        \b COMP_E_INPUT10
//!        \b COMP_E_INPUT11
//!        \b COMP_E_INPUT12
//!        \b COMP_E_INPUT13
//!        \b COMP_E_INPUT14
//!        \b COMP_E_INPUT15
//!        \b COMP_E_VREF
//!        Modified bits are \b CEPDx of \b CECTL3 register.
//!
//! This function clears the bit to enable the buffer for the specified input
//! port to allow for digital signals from any of the comparator input pins.
//! This should not be reset if there is an analog signal connected to the
//! specified input pin to prevent from unexpected results.
//!
//! \return NONE
//
//*****************************************************************************
void COMP_E_enableInputBuffer (uint32_t baseAddress, uint8_t inputPort)
{
    HWREG16(baseAddress + OFS_CECTL3) &= ~(1 << inputPort);
}

//*****************************************************************************
//
//! Toggles the bit that swaps which terminals the inputs go to, while also
//! inverting the output of the comparator.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! This function toggles the bit that controls which input goes to which
//! terminal. After initialization, this bit is set to 0, after toggling it once
//! the inputs are routed to the opposite terminal and the output is inverted.
//!
//! Modified bits are \b CEEX of \b CECTL1 register.
//! \return NONE
//
//*****************************************************************************
void COMP_E_IOSwap (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_CECTL1) ^= CEEX; //Toggle CEEX bit
}

//*****************************************************************************
//
//! Returns the output value of the Comparator module.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! Returns the output value of the Comparator module.
//!
//! \return COMP_E_HIGH or COMP_E_LOW as the output value of the Comparator module.
//
//*****************************************************************************
unsigned short COMP_E_outputValue (uint32_t baseAddress)
{
    if ( HWREG16(baseAddress + OFS_CECTL1) & CEOUT){
        return ( COMP_E_HIGH) ;
    } else {
        return ( COMP_E_LOW) ;
    }
}

//*****************************************************************************
//
//Close the Doxygen group.
//! @}
//
//*****************************************************************************
