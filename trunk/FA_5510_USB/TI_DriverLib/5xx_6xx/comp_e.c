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
//THIS FILE IS INCLUDED FOR BACKWARD COMPATIBILTY. PLEASE DO NOT USE THIS FILE
//AND INCLUDED APIS FOR NEW APPLICATIONS.
//*****************************************************************************
#include "comp_e.h"
#include "inc/hw_types.h"
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
//!        \b COMPE_INPUT0 [Default]
//!        \b COMPE_INPUT1
//!        \b COMPE_INPUT2
//!        \b COMPE_INPUT3
//!        \b COMPE_INPUT4
//!        \b COMPE_INPUT5
//!        \b COMPE_INPUT6
//!        \b COMPE_INPUT7
//!        \b COMPE_INPUT8
//!        \b COMPE_INPUT9
//!        \b COMPE_INPUT10
//!        \b COMPE_INPUT11
//!        \b COMPE_INPUT12
//!        \b COMPE_INPUT13
//!        \b COMPE_INPUT14
//!        \b COMPE_INPUT15
//!        \b COMPE_VREF
//!        Modified bits are \b CEIPSEL and \b CEIPEN of \b CECTL0 register,
//!        \b CERSEL of \b CECTL2 register, and CEPDx of \b CECTL3 register.
//! \param negativeTerminalInput selects the input to the negative terminal.
//!        Valid values are
//!        \b COMPE_INPUT0 [Default]
//!        \b COMPE_INPUT1
//!        \b COMPE_INPUT2
//!        \b COMPE_INPUT3
//!        \b COMPE_INPUT4
//!        \b COMPE_INPUT5
//!        \b COMPE_INPUT6
//!        \b COMPE_INPUT7
//!        \b COMPE_INPUT8
//!        \b COMPE_INPUT9
//!        \b COMPE_INPUT10
//!        \b COMPE_INPUT11
//!        \b COMPE_INPUT12
//!        \b COMPE_INPUT13
//!        \b COMPE_INPUT14
//!        \b COMPE_INPUT15
//!        \b COMPE_VREF
//!        Modified bits are \b CEIMSEL and \b CEIMEN of \b CECTL0 register,
//!        \b CERSEL of \b CECTL2 register, and CEPDx of \b CECTL3 register.
//! \param outputFilterEnableAndDelayLevel controls the output filter delay
//!       state, which is either off or enabled with a specified delay level.
//!        Valid values are
//!        \b COMPE_FILTEROUTPUT_OFF [Default]
//!        \b COMPE_FILTEROUTPUT_DLYLVL1
//!        \b COMPE_FILTEROUTPUT_DLYLVL2
//!        \b COMPE_FILTEROUTPUT_DLYLVL3
//!        \b COMPE_FILTEROUTPUT_DLYLVL4
//!        This parameter is device specific and delay levels should be found in
//!        the device's datasheet.
//!        Modified bits are \b CEF and \b CEFDLY of \b CECTL1 register.
//! \param invertedOutput controls if the output will be inverted or not
//!        Valid values are
//!        \b COMPE_NORMALOUTPUTPOLARITY - indicates the output should be normal.
//!             [Default]
//!        \b COMPE_INVERTEDOUTPUTPOLARITY -  the output should be inverted.
//!        Modified bits are \b CEOUTPOL of \b CECTL1 register.
//!
//! Upon successful initialization of the Comparator module, this function will
//! have reset all necessary register bits and set the given options in the
//! registers. To actually use the comparator module, the COMPE_enable()
//! function must be explicitly called before use.
//! If a Reference Voltage is set to a terminal, the Voltage should be set
//! using the setReferenceVoltage() function.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process.
//
//*****************************************************************************
unsigned short COMPE_init (unsigned int baseAddress,
    unsigned char positiveTerminalInput,
    unsigned char negativeTerminalInput,
    unsigned char outputFilterEnableAndDelayLevel,
    unsigned short invertedOutputPolarity)
{
    assert(positiveTerminalInput <= COMPE_VREF);
    assert(negativeTerminalInput <= COMPE_VREF);
    assert(positiveTerminalInput != negativeTerminalInput);
    assert(outputFilterEnableAndDelayLevel <= COMPE_FILTEROUTPUT_DLYLVL4);

    unsigned char retVal = STATUS_SUCCESS;

    //Reset COMPE Control 1 & Interrupt Registers for initialization (OFS_CECTL3
    //is not reset because it controls the input buffers of the analog signals
    //and may cause parasitic effects if an analog signal is still attached and
    //the buffer is re-enabled
    HWREG(baseAddress + OFS_CECTL0) &= 0x0000;
    HWREG(baseAddress + OFS_CEINT)  &= 0x0000;

    //Set the Positive Terminal
    if (COMPE_VREF != positiveTerminalInput){
        //Enable Positive Terminal Input Mux and Set it to the appropriate input
        HWREG(baseAddress + OFS_CECTL0) |= CEIPEN + positiveTerminalInput;

        //Disable the input buffer
        HWREG(baseAddress + OFS_CECTL3) |= (1 << positiveTerminalInput);
    } else {
        //Reset and Set COMPE Control 2 Register
        HWREG(baseAddress + OFS_CECTL2) = ~(CERSEL); //Set Vref to go to (+)terminal
    }

    //Set the Negative Terminal
    if (COMPE_VREF != negativeTerminalInput){
        //Enable Negative Terminal Input Mux and Set it to the appropriate input
        HWREG(baseAddress + OFS_CECTL0) |= CEIMEN + (negativeTerminalInput << 8);

        //Disable the input buffer
        HWREG(baseAddress + OFS_CECTL3) |= (1 << negativeTerminalInput);
    } else {
        //Reset and Set COMPE Control 2 Register
        HWREG(baseAddress + OFS_CECTL2) = CERSEL; //Set Vref to go to (-) terminal
    }

    //Reset and Set COMPE Control 1 Register
    HWREG(baseAddress + OFS_CECTL1) =
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
//!        \b COMPE_REFERENCE_AMPLIFIER_DISABLED
//!        \b COMPE_VREFBASE1_5V
//!        \b COMPE_VREFBASE2_0V
//!        \b COMPE_VREFBASE2_5V
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
void COMPE_setReferenceVoltage (unsigned int baseAddress,
    unsigned int supplyVoltageReferenceBase,
    unsigned int lowerLimitSupplyVoltageFractionOf32,
    unsigned int upperLimitSupplyVoltageFractionOf32)
{
    assert(supplyVoltageReferenceBase <= COMPE_VREFBASE2_5V);
    assert(upperLimitSupplyVoltageFractionOf32 <= 32);
    assert(lowerLimitSupplyVoltageFractionOf32 <= 32);
    assert(upperLimitSupplyVoltageFractionOf32
        >= lowerLimitSupplyVoltageFractionOf32);

    HWREG(baseAddress + OFS_CECTL1) &= ~(CEMRVS); //Set to VREF0

    //Reset COMPE Control 2 Bits (Except for CERSEL which is set in Comp_Init() )
    HWREG(baseAddress + OFS_CECTL2) &= CERSEL;

    //Set Voltage Source (Vcc | Vref, resistor ladder or not)
    if (COMPE_VREFBASE_VCC == supplyVoltageReferenceBase){
        HWREG(baseAddress + OFS_CECTL2) |= CERS_1; //Vcc with resistor ladder
    } else if (lowerLimitSupplyVoltageFractionOf32 == 32){
        //If the lower limit is 32, then the upper limit has to be 32 due to the
        //assertion that upper must be >= to the lower limit. If the numerator is
        //equal to 32, then the equation would be 32/32 == 1, therefore no resistor
        //ladder is needed
        HWREG(baseAddress + OFS_CECTL2) |= CERS_3; //Vref, no resistor ladder
    } else {
        HWREG(baseAddress + OFS_CECTL2) |= CERS_2; //Vref with resistor ladder
    }

    //Set COMPE Control 2 Register
    HWREG(baseAddress + OFS_CECTL2) |=
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
//!      \b COMPE_ACCURACY_STATIC
//!      \b COMPE_ACCURACY_CLOCKED
//!      Modified bits are \b CEREFACC of \b CECTL2 register.
//!
//! The reference accuracy is set to the desired setting. Clocked is better for low power
//! operations but has a lower accuracy.
//!
//! \return NONE
//
//*****************************************************************************
void COMPE_setReferenceAccuracy (unsigned int baseAddress,
	unsigned int referenceAccuracy)
{
	assert(
	  (referenceAccuracy == COMPE_ACCURACY_STATIC) ||
	  (referenceAccuracy == COMPE_ACCURACY_CLOCKED)
	);
	
    HWREG(baseAddress + OFS_CECTL2) &= ~(CEREFACC);
    HWREG(baseAddress + OFS_CECTL2) |= referenceAccuracy;
}

//*****************************************************************************
//
//! Sets the power mode 
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param powerMode decides the power mode 
//!        Valid values are
//!        \b COMPE_HIGH_SPEED_MODE
//!        \b COMPE_NORMAL_MODE
//!        \b COMPE_ULTRA_LOW_POWER_MODE
//!        Modified bits are \b CEPWRMD of \b CECTL1 register.
//!
//! \return NONE
//
//*****************************************************************************
void COMPE_setPowerMode (unsigned int baseAddress,
	unsigned int powerMode)
{
	HWREG(baseAddress + OFS_CECTL1) &= ~(COMPE_NORMAL_MODE | COMPE_ULTRA_LOW_POWER_MODE);
	
	HWREG(baseAddress + OFS_CECTL1) |= powerMode;
}

//*****************************************************************************
//
//! Enables selected Comparator interrupt sources.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is the bit mask of the interrupt sources to be enabled.
//!        Mask value is the logical OR of any of the following
//!        \b COMPE_OUTPUT_INTERRUPT - Output interrupt
//!        \b COMPE_INVERTED_POLARITY_INTERRUPT - Output interrupt inverted polarity
//!        \b COMPE_READY_INTERRUPT - Ready interrupt
//!
//! Enables the indicated Comparator interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor. The default trigger for the non-inverted
//! interrupt is a rising edge of the output, this can be changed with the
//! interruptSetEdgeDirection() function.
//!
//! \return NONE
//
//*****************************************************************************
void COMPE_enableInterrupt (unsigned int baseAddress,
    unsigned int interruptMask)
{
    HWREG(baseAddress + OFS_CEINT) &= ~(interruptMask);
    //Set the edge direction as default direction
    HWREG(baseAddress + OFS_CECTL1) &= ~(CEIES);
    //Set the Interrupt enable bit
    HWREG(baseAddress + OFS_CEINT) |= interruptMask;
}

//*****************************************************************************
//
//! Disables selected Comparator interrupt sources.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is the bit mask of the interrupt sources to be disabled.
//!        Mask value is the logical OR of any of the following
//!        \b COMPE_OUTPUT_INTERRUPT - Output interrupt
//!        \b COMPE_INVERTED_POLARITY_INTERRUPT - Output interrupt inverted polarity
//!        \b COMPE_READY_INTERRUPT - Ready interrupt 
//!
//! Disables the indicated Comparator interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor.
//!
//! \return NONE
//
//*****************************************************************************
void COMPE_disableInterrupt (unsigned int baseAddress,
    unsigned int interruptMask)
{
    HWREG(baseAddress + OFS_CEINT) &= ~(interruptMask);
}

//*****************************************************************************
//
//! Clears Comparator interrupt flags.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is a bit mask of the interrupt sources to be cleared.
//!        Mask value is the logical OR of any of the following
//!        \b COMPE_INTERRUPT_FLAG - Output interrupt flag
//!        \b COMPE_INTERRUPT_FLAG_INVERTED_POLARITY - Output interrupt flag inverted polarity
//!        \b COMPE_INTERRUPT_FLAG_READY - Ready interrupt flag
//!
//! The Comparator interrupt source is cleared, so that it no longer asserts.
//! The highest interrupt flag is automatically cleared when an interrupt vector
//! generator is used.
//!
//! \return NONE
//
//*****************************************************************************
void COMPE_clearInterrupt (unsigned int baseAddress,
    unsigned int interruptFlagMask)
{
    HWREG(baseAddress + OFS_CEINT) &= ~(interruptFlagMask);
}

//*****************************************************************************
//
//! Gets the current Comparator interrupt status.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following
//!        \b COMPE_INTERRUPT_FLAG - Output interrupt flag
//!        \b COMPE_INTERRUPT_FLAG_INVERTED_POLARITY - Output interrupt flag inverted polarity
//!        \b COMPE_INTERRUPT_FLAG_READY - Ready interrupt flag
//!
//! This returns the interrupt status for the Comparator module based on which
//! flag is passed.
//!
//! \return The current interrupt flag status for the corresponding mask.
//
//*****************************************************************************
unsigned char COMPE_getInterruptStatus (unsigned int baseAddress,
    unsigned int interruptFlagMask)
{
    return ( HWREG(baseAddress + OFS_CEINT) & interruptFlagMask );
}

//*****************************************************************************
//
//! Explicitly sets the edge direction that would trigger an interrupt.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param edgeDirection determines which direction the edge would have to go to
//!       generate an interrupt based on the non-inverted interrupt flag.
//!        Valid values are
//!        \b COMPE_FALLINGEDGE - sets the bit to generate an interrupt when the
//!             output of the comparator falls from HIGH to LOW if the normal
//!             interrupt bit is set(and LOW to HIGH if the inverted interrupt
//!             enable bit is set). [Default]
//!        \b COMPE_RISINGEDGE - sets the bit to generate an interrupt when the
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
void COMPE_interruptSetEdgeDirection (unsigned int baseAddress,
    unsigned short edgeDirection)
{
    assert(edgeDirection <= COMPE_RISINGEDGE);

    //Set the edge direction that will trigger an interrupt
    if (COMPE_RISINGEDGE == edgeDirection){
        HWREG(baseAddress + OFS_CECTL1) |= CEIES;
    } else if (COMPE_FALLINGEDGE == edgeDirection){
        HWREG(baseAddress + OFS_CECTL1) &= ~(CEIES);
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
void COMPE_interruptToggleEdgeDirection (unsigned int baseAddress)
{
    HWREG(baseAddress + OFS_CECTL1) ^= CEIES;
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
void COMPE_enable (unsigned int baseAddress)
{
    HWREG(baseAddress + OFS_CECTL1) |= CEON;
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
void COMPE_disable (unsigned int baseAddress)
{
    HWREG(baseAddress + OFS_CECTL1) &= ~(CEON);
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
void COMPE_shortInputs (unsigned int baseAddress)
{
    HWREG(baseAddress + OFS_CECTL1) |= CESHORT;
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
void COMPE_unshortInputs (unsigned int baseAddress)
{
    HWREG(baseAddress + OFS_CECTL1) &= ~(CESHORT);
}

//*****************************************************************************
//
//! Disables the input buffer of the selected input port to effectively allow
//! for analog signals.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param inputPort is the port in which the input buffer will be disabled.
//!        Valid values are
//!        \b COMPE_INPUT0 [Default]
//!        \b COMPE_INPUT1
//!        \b COMPE_INPUT2
//!        \b COMPE_INPUT3
//!        \b COMPE_INPUT4
//!        \b COMPE_INPUT5
//!        \b COMPE_INPUT6
//!        \b COMPE_INPUT7
//!        \b COMPE_INPUT8
//!        \b COMPE_INPUT9
//!        \b COMPE_INPUT10
//!        \b COMPE_INPUT11
//!        \b COMPE_INPUT12
//!        \b COMPE_INPUT13
//!        \b COMPE_INPUT14
//!        \b COMPE_INPUT15
//!        \b COMPE_VREF
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
void COMPE_disableInputBuffer (unsigned int baseAddress,
    unsigned char inputPort)
{
    HWREG(baseAddress + OFS_CECTL3) |= (1 << inputPort);
}

//*****************************************************************************
//
//! Enables the input buffer of the selected input port to allow for digital
//! signals.
//!
//! \param baseAddress is the base address of the Comparator module.
//! \param inputPort is the port in which the input buffer will be enabled.
//!        Valid values are
//!        \b COMPE_INPUT0 [Default]
//!        \b COMPE_INPUT1
//!        \b COMPE_INPUT2
//!        \b COMPE_INPUT3
//!        \b COMPE_INPUT4
//!        \b COMPE_INPUT5
//!        \b COMPE_INPUT6
//!        \b COMPE_INPUT7
//!        \b COMPE_INPUT8
//!        \b COMPE_INPUT9
//!        \b COMPE_INPUT10
//!        \b COMPE_INPUT11
//!        \b COMPE_INPUT12
//!        \b COMPE_INPUT13
//!        \b COMPE_INPUT14
//!        \b COMPE_INPUT15
//!        \b COMPE_VREF
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
void COMPE_enableInputBuffer (unsigned int baseAddress, unsigned char inputPort)
{
    HWREG(baseAddress + OFS_CECTL3) &= ~(1 << inputPort);
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
void COMPE_IOSwap (unsigned int baseAddress)
{
    HWREG(baseAddress + OFS_CECTL1) ^= CEEX; //Toggle CEEX bit
}

//*****************************************************************************
//
//! Returns the output value of the Comparator module.
//!
//! \param baseAddress is the base address of the Comparator module.
//!
//! Returns the output value of the Comparator module.
//!
//! \return COMPE_HIGH or COMPE_LOW as the output value of the Comparator module.
//
//*****************************************************************************
unsigned short COMPE_outputValue (unsigned int baseAddress)
{
    if ( HWREG(baseAddress + OFS_CECTL1) & CEOUT){
        return ( COMPE_HIGH) ;
    } else {
        return ( COMPE_LOW) ;
    }
}

//*****************************************************************************
//
//Close the Doxygen group.
//! @}
//
//*****************************************************************************
