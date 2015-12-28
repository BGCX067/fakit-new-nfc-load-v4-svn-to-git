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
//adc10_b.c - Driver for the ADC10B Module.
//
//*****************************************************************************
#include "inc/hw_regaccess.h"
#include "adc10_b.h"
#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430xgeneric.h"
#else
#include "deprecated/CCS/msp430xgeneric.h"
#endif
#include "assert.h"

//*****************************************************************************
//
//! Initializes the ADC10B Module.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param sampleHoldSignalSourceSelect is the signal that will trigger a
//!       sample-and-hold for an input signal to be converted.
//!        Valid values are
//!        \b ADC10_B_SAMPLEHOLDSOURCE_SC [Default]	
//!        \b ADC10_B_SAMPLEHOLDSOURCE_1
//!        \b ADC10_B_SAMPLEHOLDSOURCE_2
//!        \b ADC10_B_SAMPLEHOLDSOURCE_3
//!        This parameter is device specific and sources should be found in the
//!        device's datasheet.
//!        Modified bits are \b ADC10SHSx of \b ADC10CTL1 register.
//! \param clockSourceSelect selects the clock that will be used by the ADC10B
//!       core and the sampling timer if a sampling pulse mode is enabled.
//!        Valid values are
//!        \b ADC10_B_CLOCKSOURCE_ADC10OSC - MODOSC 5 MHz oscillator from the UCS
//!            [Default]
//!        \b ADC10_B_CLOCKSOURCE_ACLK     - The Auxilary Clock
//!        \b ADC10_B_CLOCKSOURCE_MCLK     - The Master Clock
//!        \b ADC10_B_CLOCKSOURCE_SMCLK    - The Sub-Master Clock
//!        Modified bits are \b ADC10SSELx of \b ADC10CTL1 register.
//! \param clockSourceDivider selects the amount that the clock will be divided.
//!        Valid values are
//!        \b ADC10_B_CLOCKDIVIDER_1 [Default]
//!        \b ADC10_B_CLOCKDIVIDER_2
//!        \b ADC10_B_CLOCKDIVIDER_3
//!        \b ADC10_B_CLOCKDIVIDER_4
//!        \b ADC10_B_CLOCKDIVIDER_5
//!        \b ADC10_B_CLOCKDIVIDER_6
//!        \b ADC10_B_CLOCKDIVIDER_7
//!        \b ADC10_B_CLOCKDIVIDER_8
//!        \b ADC10_B_CLOCKDIVIDER_12
//!        \b ADC10_B_CLOCKDIVIDER_16
//!        \b ADC10_B_CLOCKDIVIDER_20
//!        \b ADC10_B_CLOCKDIVIDER_24
//!        \b ADC10_B_CLOCKDIVIDER_28
//!        \b ADC10_B_CLOCKDIVIDER_32
//!        \b ADC10_B_CLOCKDIVIDER_64
//!        \b ADC10_B_CLOCKDIVIDER_128
//!        \b ADC10_B_CLOCKDIVIDER_192
//!        \b ADC10_B_CLOCKDIVIDER_256
//!        \b ADC10_B_CLOCKDIVIDER_320
//!        \b ADC10_B_CLOCKDIVIDER_384
//!        \b ADC10_B_CLOCKDIVIDER_448
//!        \b ADC10_B_CLOCKDIVIDER_512
//!        Modified bits are \b ADC10DIVx of \b ADC10CTL1 register and
//!         \b ADC10PDIVx of \b ADC10CTL2 register.
//!
//! This function initializes the ADC module to allow for analog-to-digital
//! conversions. Specifically this function sets up the sample-and-hold signal
//! and clock sources for the ADC core to use for conversions. Upon successful
//! completion of the initialization all of the ADC control registers will be
//! reset, excluding the memory controls and reference module bits, the given
//! parameters will be set, and the ADC core will be turned on (Note, that the
//! ADC core only draws power during conversions and remains off when not
//! converting).Note that sample/hold signal sources are device dependent. Note
//! that if re-initializing the ADC after starting a conversion with the
//! startConversion() function, the disableConversion() must be called BEFORE
//! this function can be called.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process.
//
//*****************************************************************************
unsigned short ADC10_B_init (uint32_t baseAddress,
    uint16_t sampleHoldSignalSourceSelect,
    uint8_t clockSourceSelect,
    uint16_t clockSourceDivider)
{
    assert(sampleHoldSignalSourceSelect <= ADC10_B_SAMPLEHOLDSOURCE_3);
    assert(clockSourceSelect <= ADC10_B_CLOCKSOURCE_SMCLK);
    assert(clockSourceDivider <= ADC10_B_CLOCKDIVIDER_512);

    uint8_t retVal = STATUS_SUCCESS;

    //Turn OFF ADC10B Module & Clear Interrupt Registers
    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~(ADC10ON + ADC10ENC + ADC10SC);
    HWREG16(baseAddress + OFS_ADC10IE)   &= 0x0000; //Reset ALL interrupt enables
    HWREG16(baseAddress + OFS_ADC10IFG)  &= 0x0000; //Reset ALL interrupt flags

    //Set ADC10B Control 1
    HWREG16(baseAddress + OFS_ADC10CTL1) =
        sampleHoldSignalSourceSelect //Setup the Sample-and-Hold Source
        + (clockSourceDivider & ADC10DIV_7) //Set Clock Divider
        + clockSourceSelect; //Setup Clock Source

    //Set ADC10B Control 2
    HWREG16(baseAddress + OFS_ADC10CTL2) =
        (clockSourceDivider & ADC10PDIV_3) //Set Clock Pre-Divider
        + ADC10RES; //Default resolution to 10-bits

    return ( retVal) ;
}

//*****************************************************************************
//
//! Enables the ADC10B block.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! This will enable operation of the ADC10B block.
//! Modified bits are \b ADC10ON of \b ADC10CTL0 register.
//!
//! \return None.
//
//*****************************************************************************
void ADC10_B_enable (uint32_t baseAddress)
{
    //Reset the ADC10ON bit to enable the ADC10B Module
    HWREG16(baseAddress + OFS_ADC10CTL0) |= ADC10ON;
}

//*****************************************************************************
//
//! Disables the ADC10B block.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! This will disable operation of the ADC10B block.
//! Modified bits are \b ADC10ON of \b ADC10CTL0 register.
//!
//! \return None.
//
//*****************************************************************************
void ADC10_B_disable (uint32_t baseAddress)
{
    //Set the ADC10ON bit to disable the ADC10B Module
    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~ADC10ON;
}

//*****************************************************************************
//
//! Sets up and enables the Sampling Timer Pulse Mode.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param clockCycleHoldCount sets the amount of clock cycles to
//!       sample-and-hold for the memory buffer.
//!        Valid values are
//!        \b ADC10_B_CYCLEHOLD_4_CYCLES [Default]
//!        \b ADC10_B_CYCLEHOLD_8_CYCLES
//!        \b ADC10_B_CYCLEHOLD_16_CYCLES
//!        \b ADC10_B_CYCLEHOLD_32_CYCLES
//!        \b ADC10_B_CYCLEHOLD_64_CYCLES
//!        \b ADC10_B_CYCLEHOLD_96_CYCLES
//!        \b ADC10_B_CYCLEHOLD_128_CYCLES
//!        \b ADC10_B_CYCLEHOLD_192_CYCLES
//!        \b ADC10_B_CYCLEHOLD_256_CYCLES
//!        \b ADC10_B_CYCLEHOLD_384_CYCLES
//!        \b ADC10_B_CYCLEHOLD_512_CYCLES
//!        \b ADC10_B_CYCLEHOLD_768_CYCLES
//!        \b ADC10_B_CYCLEHOLD_1024_CYCLES
//!        Modified bits are \b ADC10SHTx of \b ADC10CTL0 register.
//! \param multipleSamplesEnabled allows multiple conversions to start
//!       without a trigger signal from the sample/hold signal
//!        Valid values are
//!        \b ADC10_B_MULTIPLESAMPLESDISABLE - a timer trigger will be needed to
//!                  start every ADC conversion.
//!        \b ADC10_B_MULTIPLESAMPLESENABLE  - during a sequenced and/or repeated
//!                  conversion mode, after the first conversion, no sample/hold
//!                  signal is necessary to start subsequent samples.
//!        Modified bits are \b ADC10MSC of \b ADC10CTL0 register.
//!
//! This function sets up the sampling timer pulse mode which allows the
//! sample/hold signal to trigger a sampling timer to sample-and-hold an input
//! signal for a specified number of clock cycles without having to hold the
//! sample/hold signal for the entire period of sampling. Note that if a
//! conversion has been started with the startConversion() function, then a call
//! to disableConversions() is required before this function may be called.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_setupSamplingTimer (uint32_t baseAddress,
    uint16_t clockCycleHoldCount,
    unsigned short multipleSamplesEnabled)
{
    //Make sure the ENC bit is cleared before setting up sampling pulse mode
    assert( !(HWREG16(baseAddress + OFS_ADC10CTL0) & ADC10ENC) );

    assert(clockCycleHoldCount <= ADC10_B_CYCLEHOLD_1024_CYCLES);

    HWREG16(baseAddress + OFS_ADC10CTL1) |= ADC10SHP;

    //Reset and Set CB Control 0 Bits
    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~(ADC10SHT_15 + ADC10MSC);
    HWREG16(baseAddress + OFS_ADC10CTL0) |= clockCycleHoldCount
                                          + multipleSamplesEnabled;
}

//*****************************************************************************
//
//! Disables Sampling Timer Pulse Mode.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! Disables the Sampling Timer Pulse Mode. Note that if a conversion has been
//! started with the startConversion() function, then a call to
//! disableConversions() is required before this function may be called.
//!
//! Modified bits are \b ADC10SHP of \b ADC10CTL1 register.
//! \return None
//
//*****************************************************************************
void ADC10_B_disableSamplingTimer (uint32_t baseAddress)
{
    //Make sure the ENC bit is cleared before disabling sampling pulse mode
    assert( ~(HWREG16(baseAddress + OFS_ADC10CTL0) & ADC10ENC) );

    HWREG16(baseAddress + OFS_ADC10CTL1) &= ~(ADC10SHP);
}

//*****************************************************************************
//
//! Configures the controls of the selected memory buffer.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param inputSourceSelect is the input that will store the converted data
//!       into the specified memory buffer.
//!        Valid values are
//!        \b ADC10_B_INPUT_A0 [Default]
//!        \b ADC10_B_INPUT_A1
//!        \b ADC10_B_INPUT_A2
//!        \b ADC10_B_INPUT_A3
//!        \b ADC10_B_INPUT_A4
//!        \b ADC10_B_INPUT_A5
//!        \b ADC10_B_INPUT_A6
//!        \b ADC10_B_INPUT_A7
//!        \b ADC10_B_INPUT_VEREF+
//!        \b ADC10_B_INPUT_VEREF-
//!        \b ADC10_B_INPUT_TEMPSENSOR
//!        \b ADC10_B_INPUT_BATTERYMONITOR
//!        \b ADC10_B_INPUT_A12
//!        \b ADC10_B_INPUT_A13
//!        \b ADC10_B_INPUT_A14
//!        \b ADC10_B_INPUT_A15
//!        Modified bits are \b ADC10INCHx of \b ADC10MCTL0 register.
//! \param positiveRefVoltageSourceSelect is the reference voltage source to set
//!       as the upper limit for the conversion that is to be stored in the
//!       specified memory buffer.
//!        Valid values are
//!        \b ADC10_B_VREFPOS_AVCC [Default]
//!        \b ADC10_B_VREFPOS_EXT
//!        \b ADC10_B_VREFPOS_INT
//!        Modified bits are \b ADC10SREF of \b ADC10MCTL0 register.
//! \param negativeRefVoltageSourceSelect is the reference voltage source to set
//!       as the lower limit for the conversion that is to be stored in the
//!       specified memory buffer.
//!        Valid values are
//!        \b ADC10_B_VREFPOS_AVSS [Default]
//!        \b ADC10_B_VREFPOS_EXT
//!        Modified bits are \b ADC10SREF of \b ADC10CTL0 register.
//!
//! Maps an input signal conversion into the memory buffer, as
//! well as the positive and negative reference voltages for each conversion
//! being stored into the memory buffer. If the internal reference is used for
//! the positive reference voltage, the internal REF module has to control
//! the voltage level. Note that if a conversion has been started with the
//! startConversion() function, then a call to disableConversions() is required
//! before this function may be called.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_memoryConfigure (uint32_t baseAddress,
    uint8_t inputSourceSelect,
    uint8_t positiveRefVoltageSourceSelect,
    uint8_t negativeRefVoltageSourceSelect)
{
    //Make sure the ENC bit is cleared before configuring a Memory Buffer Control
    assert( !(HWREG16(baseAddress + OFS_ADC10CTL0) & ADC10ENC) );

    assert(inputSourceSelect <= ADC10_B_INPUT_A15);
    assert(positiveRefVoltageSourceSelect <= ADC10_B_VREFPOS_INT);
    assert(negativeRefVoltageSourceSelect <= ADC10_B_VREFNEG_EXT);

    //Reset and Set the Memory Buffer Control Bits
    HWREG8(baseAddress + OFS_ADC10MCTL0) = inputSourceSelect
                                           + positiveRefVoltageSourceSelect +
                                           negativeRefVoltageSourceSelect;
}

//*****************************************************************************
//
//! Enables selected ADC10B interrupt sources.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param interruptMask is the bit mask of the memory buffer interrupt
//!       sources to be enabled.
//!        Mask Value is the logical OR of any of the following
//!        \b ADC10OVIE - Interrupts when a new conversion is about to overwrite
//!                 the previous one
//!        \b ADC10TOVIE - Interrupts when a new conversion is starting befor
//!                 the previous one has finished
//!        \b ADC10HIIE - Interrups when the input signal has gone above the
//!                 high threshold of the window comparator
//!        \b ADC10LOIE - Interrupts when the input signal has gone below the
//!                 low threshold of the low window comparator
//!        \b ADC10INIE - Interrupts when the input signal is in between the
//!                 high and low thresholds of the window comparator
//!        \b ADC10IE0 - Interrupt for new conversion data in the memory buffer
//!
//! Enables the indicated ADC10B interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor. <b>Does not clear interrupt flags.</b>
//! Modified registers are  \b ADC10IFG and \b ADC10IE.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_enableInterrupt (uint32_t baseAddress,
    uint8_t interruptMask)
{
    HWREG16(baseAddress + OFS_ADC10IE) |= interruptMask;
}

//*****************************************************************************
//
//! Disables selected ADC10B interrupt sources.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param interruptMask is the bit mask of the memory buffer interrupt
//!       sources to be disabled.
//!        Mask Value is the logical OR of any of the following
//!        \b ADC10OVIE - Interrupts when a new conversion is about to overwrite
//!                 the previous one
//!        \b ADC10TOVIE - Interrupts when a new conversion is starting befor
//!                 the previous one has finished
//!        \b ADC10HIIE - Interrups when the input signal has gone above the
//!                 high threshold of the window comparator
//!        \b ADC10LOIE - Interrupts when the input signal has gone below the
//!                 low threshold of the low window comparator
//!        \b ADC10INIE - Interrupts when the input signal is in between the
//!                 high and low thresholds of the window comparator
//!        \b ADC10IE0 - Interrupt for new conversion data in the memory buffer
//!
//! Disables the indicated ADC10B interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor.
//! Modified registers are ADC10IE.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_disableInterrupt (uint32_t baseAddress,
    uint8_t interruptMask)
{
    HWREG16(baseAddress + OFS_ADC10IE) &= ~(interruptMask);
}

//*****************************************************************************
//
//! Clears ADC10B selected interrupt flags.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param interruptFlagMask is a bit mask of the interrupt flags to be cleared.
//!        Mask Value is the logical OR of any of the following
//!        \b ADC10OVIFG - Interrupt flag for when a new conversion is about to
//!                  overwrite the previous one
//!        \b ADC10TOVIFG - Interrupt flag for when a new conversion is starting
//!                 before the previous one has finished
//!        \b ADC10HIIFG - Interrup flag for when the input signal has gone
//!                 above the high threshold of the window comparator
//!        \b ADC10LOIFG - Interrupt flag for when the input signal has gone
//!                 below the low threshold of the window comparator
//!        \b ADC10INIFG - Interrupt flag for when the input signal is in
//!                 between the high and low thresholds of the window comparator
//!        \b ADC10IFG0 - Interrupt flag for new conversion data in the memory
//!                 buffer
//!
//! The selected ADC10B interrupt flags are cleared, so that it no longer asserts.
//! The memory buffer interrupt flags are only cleared when the memory buffer is
//! accessed.
//! Modified registers are  \b ADC10IFG
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_clearInterrupt (uint32_t baseAddress,
    uint8_t interruptFlagMask)
{
    HWREG16(baseAddress + OFS_ADC10IFG) &= ~(interruptFlagMask);
}

//*****************************************************************************
//
//! Returns the status of the selected memory interrupt flags.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param interruptFlagMask is a bit mask of the interrupt flags status
//!       to be returned.
//!        Mask Value is the logical OR of any of the following
//!        \b ADC10OVIFG - Interrupt flag for when a new conversion is about to
//!                  overwrite the previous one
//!        \b ADC10TOVIFG - Interrupt flag for when a new conversion is starting
//!                 before the previous one has finished
//!        \b ADC10HIIFG - Interrup flag for when the input signal has gone
//!                 above the high threshold of the window comparator
//!        \b ADC10LOIFG - Interrupt flag for when the input signal has gone
//!                 below the low threshold of the window comparator
//!        \b ADC10INIFG - Interrupt flag for when the input signal is in
//!                 between the high and low thresholds of the window comparator
//!        \b ADC10IFG0 - Interrupt flag for new conversion data in the memory
//!                 buffer
//!
//! Returns the status of the selected interrupt flags.
//!
//! \return The current interrupt flag status for the corresponding mask.
//
//*****************************************************************************
uint8_t ADC10_B_getInterruptStatus (uint32_t baseAddress,
    uint8_t interruptFlagMask)
{
    return ( HWREG16(baseAddress + OFS_ADC10IFG) & interruptFlagMask );
}

//*****************************************************************************
//
//! Enables/Starts an Analog-to-Digital Conversion.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param conversionSequenceModeSelect determines the ADC operating mode.
//!        Valid values are
//!        \b ADC10_B_SINGLECHANNEL - one-time conversion of a single channel into
//!           a single memory buffer [Default]
//!        \b ADC10_B_SEQOFCHANNELS - one time conversion of multiple channels
//!           into the specified starting memory buffer and each subsequent
//!           memory buffer up until the conversion is stored in a memory buffer
//!           dedicated as the end-of-sequence by the memory's control register
//!        \b ADC10_B_REPEATED_SINGLECHANNEL - repeated conversions of one channel
//!           into a single memory buffer
//!        \b ADC10_B_REPEATED_SEQOFCHANNELS - repeated conversions of multiple
//!           channels into the specified starting memory buffer and each
//!           subsequent memory buffer up until the conversion is stored in a
//!           memory buffer dedicated as the end-of-sequence by the memory's
//!           control register
//!        Modified bits are \b ADC10CONSEQx of \b ADC10CTL1 register.
//!
//! This function  enables/starts the conversion process of the ADC.
//! If the sample/hold signal source chosen during initialization was
//! ADC10OSC, then the conversion is started immediately, otherwise the chosen
//! sample/hold signal source starts the conversion by a rising edge of the
//! signal. Keep in mind when selecting conversion modes, that for sequenced
//! and/or repeated modes, to keep the sample/hold-and-convert process
//! continuing without a trigger from the sample/hold signal source, the
//! multiple samples must be enabled using the ADC10_B_setupSamplingTimer()
//! function. Also note that when a sequence conversion mode is selected, the
//! first input channel is the one mapped to the memory buffer, the next input
//! channel selected for conversion is one less than the input channel just
//! converted (i.e. A1 comes after A2), until A0 is reached, and if in repeating
//! mode, then the next input channel will again be the one mapped to the memory
//! buffer. Note that after this function is called, the ADC10_B_stopConversions()
//! has to be called to re-initialize the ADC, reconfigure a memory buffer
//! control, enable/disable the sampling timer, or to change the internal
//! reference voltage.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_startConversion (uint32_t baseAddress,
    uint8_t conversionSequenceModeSelect)
{
    assert(conversionSequenceModeSelect <= ADC10_B_REPEATED_SEQOFCHANNELS);

    //Reset the ENC bit to set the conversion mode sequence
    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~(ADC10ENC);

    HWREG16(baseAddress + OFS_ADC10CTL1) |= conversionSequenceModeSelect;
    HWREG16(baseAddress + OFS_ADC10CTL0) |= ADC10ENC + ADC10SC;
}

//*****************************************************************************
//
//! Disables the ADC from converting any more signals.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param preempt specifies if the current conversion should be preemptly
//!       stopped before the end of the conversion
//!        Valid values are
//!        \b ADC10_B_COMPLETECONVERSION - Allows the ADC10B to end the current
//!                  conversion before disabling conversions.
//!        \b ADC10_B_PREEMPTCONVERSION - Stops the ADC10B immediatly, with
//!                  unpredicatble results of the current conversion. Cannot be
//!                  used with repeated conversion.
//!
//! Disables the ADC from converting any more signals.
//! If there is a conversion in progress, this function can stop it immediatly
//! if the preempt parameter is set as ADC10_B_PREEMPTCONVERSION, by changing the
//! conversion mode to single-channel, single-conversion and disabling
//! conversions. If the conversion mode is set as single-channel,
//! single-conversion and this function is called without preemption, then the
//! ADC core conversion status is polled until the conversion is complete before
//! disabling conversions to prevent unpredictable data. If the
//! ADC10_B_startConversion() has been called, then this function has to be called
//! to re-initialize the ADC, reconfigure a memory buffer control,
//! enable/disable the sampling pulse mode, or change the internal reference
//! voltage.
//! Modified registers are  \b ADC10CTL1 and \b ADC10CTL0
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_disableConversions (uint32_t baseAddress, unsigned short preempt)
{
    if (ADC10_B_PREEMPTCONVERSION == preempt){
        HWREG16(baseAddress + OFS_ADC10CTL1) &= ~(ADC10CONSEQ_3);
        //Reset conversion sequence mode to single-channel, single-conversion
    } else if ( ~(HWREG16(baseAddress + OFS_ADC10CTL1) & ADC10CONSEQ_3) ){
        //To prevent preemoption of a single-channel, single-conversion we must
        //wait for the ADC core to finish the conversion.
        while (HWREG16(baseAddress + OFS_ADC10CTL1) & ADC10BUSY) ;
    }

    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~(ADC10ENC);
}

//*****************************************************************************
//
//! Returns the raw contents of the specified memory buffer.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! Returns the raw contents of the specified memory buffer. The format of the
//! content depends on the read-back format of the data: if the data is in
//! signed 2's complement format then the contents in the memory buffer will be
//! left-justified with the least-siginificant bits as 0's, whereas if the data
//! is in unsigned format then the contents in the memory buffer will be
//! right-justified with the most-significant bits as 0's.
//!
//! \return A Signed Integer of the contents of the specified memory buffer.
//
//*****************************************************************************
int ADC10_B_getResults (uint32_t baseAddress)
{
    return ( HWREG16(baseAddress + OFS_ADC10MEM0) );
}

//*****************************************************************************
//
//! Use to change the resolution of the converted data.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param resolutionSelect determines the resolution of the converted data.
//!        Valid values are
//!        \b ADC10_B_RESOLUTION_8BIT
//!        \b ADC10_B_RESOLUTION_10BIT [Default]
//!        Modified bits are \b ADC10RES of \b ADC10CTL2 register.
//!
//! This function can be used to change the resolution of the converted data
//! from the default of 12-bits.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_setResolution (uint32_t baseAddress,
    uint8_t resolutionSelect)
{
    assert(resolutionSelect <= ADC10_B_RESOLUTION_10BIT);

    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10RES);
    HWREG16(baseAddress + OFS_ADC10CTL2) |= resolutionSelect;
}

//*****************************************************************************
//
//! Use to invert or un-invert the sample/hold signal
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param invertedSignal set if the sample/hold signal should be inverted
//!        Valid values are
//!        \b ADC10_B_NONINVERTEDSIGNAL - a sample-and-hold of an input signal for
//!                  conversion will be started on a rising edge of the
//!                  sample/hold signal. [Default]
//!        \b ADC10_B_INVERTEDSIGNAL  - a sample-and-hold of an input signal for
//!                  conversion will be started on a falling edge of the
//!                  sample/hold signal.
//!        Modified bits are \b ADC10ISSH of \b ADC10CTL1 register.
//!
//! This function can be used to invert or un-invert the sample/hold signal.
//! Note that if a conversion has been started with the startConversion()
//! function, then a call to disableConversions() is required before this
//! function may be called.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_setSampleHoldSignalInversion (uint32_t baseAddress,
    uint16_t invertedSignal)
{
    //Make sure the ENC bit is cleared before using this function
    assert( !(HWREG16(baseAddress + OFS_ADC10CTL0) & ADC10ENC) );

    HWREG16(baseAddress + OFS_ADC10CTL1) &= ~(ADC10ISSH);
    HWREG16(baseAddress + OFS_ADC10CTL1) |= invertedSignal;
}

//*****************************************************************************
//
//! Use to set the read-back format of the converted data
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param readBackFormat is the specified format to store the conversions in
//!       the memory buffer.
//!        Valid values are
//!        \b ADC10_B_UNSIGNED_BINARY [Default]
//!        \b ADC10_B_SIGNED_2SCOMPLEMENT
//!        Modified bits are \b ADC10DF of \b ADC10CTL2 register.
//!
//! Sets the format of the converted data: how it will be stored into the
//! memory buffer, and how it should be read back. The format can be set as
//! right-justified (default), which indicates that the number will be unsigned,
//! or left-justified, which indicates that the number will be signed in
//! 2's complement format. This change affects all memory buffers for subsequent
//! conversions.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_setDataReadBackFormat (uint32_t baseAddress,
    unsigned short readBackFormat)
{
    assert(readBackFormat <= ADC10_B_SIGNED_2SCOMPLEMENT);

    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10DF);
    HWREG16(baseAddress + OFS_ADC10CTL2) |= readBackFormat;
}

//*****************************************************************************
//
//! Enables the reference buffer's burst ability.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! Enables the reference buffer's burst ability, allowing the reference buffer
//! to turn off while the ADC is not converting, and automatically turning on
//! when the ADC needs the generated reference voltage for a conversion.
//!
//! Modified bits are \b ADC10REFBURST of \b ADC10CTL2 register.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_enableReferenceBurst (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_ADC10CTL2) |= ADC10REFBURST;
}

//*****************************************************************************
//
//! Disables the reference buffer's burst ability.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! Disables the reference buffer's burst ability, forcing the reference buffer
//! to remain on continuously.
//!
//! Modified bits are \b ADC10REFBURST of \b ADC10CTL2 register.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_disableReferenceBurst (uint32_t baseAddress)
{
    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10REFBURST);
}

//*****************************************************************************
//
//! Use to set the reference buffer's sampling rate.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param samplingRateSelect is the specified maximum sampling rate.
//!        Valid values are
//!        \b ADC10_B_MAXSAMPLINGRATE_200KSPS [Default]
//!        \b ADC10_B_MAXSAMPLINGRATE_50KSPS
//!        Modified bits are \b ADC10SR of \b ADC10CTL2 register.
//!
//! Sets the reference buffer's sampling rate to the selected sampling rate. The
//! default sampling rate is maximum of 200-ksps, and can be reduced to a
//! maximum of 50-ksps to conserve power.
//!
//! Modified regsiters are \b ADC10CTL2.
//!
//! \return None
//
//*****************************************************************************
void ADC10_B_setReferenceBufferSamplingRate (uint32_t baseAddress,
    unsigned short samplingRateSelect)
{
    assert(samplingRateSelect <= ADC10_B_MAXSAMPLINGRATE_50KSPS);

    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10SR);
    HWREG16(baseAddress + OFS_ADC10CTL2) |= samplingRateSelect;
}

//*****************************************************************************
//
//! Sets the high and low threshold for the window comparator feature.
//!
//! \param baseAddress is the base address of the ADC10B module.
//! \param highThreshold is the upper bound  that could trip an interrupt for
//!       the window comparator.
//! \param lowThreshold is the lower bound that could trip on interrupt for the
//!       window comparator.
//!
//! Sets the high and low threshold for the window comparator feature. Use the
//! ADC10HIIE, ADC10INIE, ADC10LOIE interrupts to utilize this feature.
//!
//! Modified regsiters are \b ADC10HI and \b ADC10LO.
//!
//! \return NONE
//
//*****************************************************************************
void ADC10_B_setWindowComp (uint32_t baseAddress,
    uint16_t highThreshold,
    uint16_t lowThreshold)
{
    HWREG16(baseAddress + OFS_ADC10HI) = highThreshold;
    HWREG16(baseAddress + OFS_ADC10LO) = lowThreshold;
}

//*****************************************************************************
//
//! Returns the address of the memory buffer for the DMA module.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! Returns the address of the memory buffer. This can be used in conjunction
//! with the DMA to store the converted data directly to memory.
//!
//! \return NONE
//
//*****************************************************************************
uint32_t ADC10_B_getMemoryAddressForDMA (uint32_t baseAddress)
{
    return ( baseAddress + OFS_ADC10MEM0 );
}

//*****************************************************************************
//
//! Returns the busy status of the ADC10B core.
//!
//! \param baseAddress is the base address of the ADC10B module.
//!
//! Returns the status of the ADC core if there is a conversion currently taking
//! place.
//!
//! \return ADC10_B_BUSY or ADC10_B_NOTBUSY dependent if there is a conversion
//!        currently taking place.
//
//*****************************************************************************
unsigned short ADC10_B_isBusy (uint32_t baseAddress)
{
    if (HWREG16(baseAddress + OFS_ADC10CTL1) & ADC10BUSY){
        return ( ADC10_B_BUSY) ;
    } else   {
        return ( ADC10_B_NOTBUSY) ;
    }
}

//*****************************************************************************
//
//Close the Doxygen group.
//! @}
//
//*****************************************************************************

