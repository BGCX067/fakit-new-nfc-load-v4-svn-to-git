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
//tec.c - Driver for the TEC Module.
//THIS FILE IS INCLUDED FOR BACKWARD COMPATIBILTY. PLEASE DO NOT USE THIS FILE
//AND INCLUDED APIS FOR NEW APPLICATIONS.
//*****************************************************************************
#include "inc/hw_types.h"
#include "assert.h"
#include "tec.h"
#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430xgeneric.h"
#else
#include "deprecated/CCS/msp430xgeneric.h"
#endif

//*****************************************************************************
//
//! Configures the Timer Event Control External Clear Input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param signalType is the selected signal type
//! Valid values are
//!     \b TEC_EXTERNAL_CLEAR_SIGNALTYPE_EDGE_SENSITIVE  [Default]
//!     \b TEC_EXTERNAL_CLEAR_SIGNALTYPE_LEVEL_SENSITIVE 
//! \param signalHold is the selected signal hold
//! Valid values are
//!     \b TEC_EXTERNAL_CLEAR_SIGNAL_NOT_HELD [Default]
//!     \b TEC_EXTERNAL_CLEAR_SIGNAL_HELD
//! \param polarityBit is the selected signal type
//! Valid values are
//!     \b TEC_EXTERNAL_CLEAR_POLARITY_FALLING_EDGE_OR_LOW_LEVEL [Default]
//!     \b TEC_EXTERNAL_CLEAR_POLARITY_RISING_EDGE_OR_HIGH_LEVEL 
//!
//! Modified register is \b TECxCTL2 register
//!
//! \return None
//
//*****************************************************************************
void TEC_configureExternalClearInput (	unsigned int baseAddress,
									  	unsigned char signalType,
									  	unsigned char signalHold,
									  	unsigned char polarityBit
									  )
{
	assert( signalType  == TEC_EXTERNAL_CLEAR_SIGNALTYPE_EDGE_SENSITIVE ||
			signalType  == TEC_EXTERNAL_CLEAR_SIGNALTYPE_LEVEL_SENSITIVE 
			);
	assert( signalHold  == TEC_EXTERNAL_CLEAR_SIGNAL_NOT_HELD ||
			signalHold  == TEC_EXTERNAL_CLEAR_SIGNAL_HELD 
			);
	assert( polarityBit  == TEC_EXTERNAL_CLEAR_POLARITY_FALLING_EDGE_OR_LOW_LEVEL ||
			polarityBit  == TEC_EXTERNAL_CLEAR_POLARITY_RISING_EDGE_OR_HIGH_LEVEL 
			);
			
	HWREGB(baseAddress + OFS_TEC0XCTL2_L)  &= ~(TEC_EXTERNAL_CLEAR_SIGNALTYPE_LEVEL_SENSITIVE +
												TEC_EXTERNAL_CLEAR_SIGNAL_HELD +
												TEC_EXTERNAL_CLEAR_POLARITY_RISING_EDGE_OR_HIGH_LEVEL
												);
												
    HWREGB(baseAddress + OFS_TEC0XCTL2_L)  |= (signalType +
    											signalHold +
    											polarityBit
    											);
}									  
				
//*****************************************************************************
//
//! Configures the Timer Event Control External Fault Input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param selectedExternalFault is the selected external fault
//! Valid values are 
//!     \b TEC_EXTERNAL_FAULT_0
//!     \b TEC_EXTERNAL_FAULT_1
//!     \b TEC_EXTERNAL_FAULT_2
//!     \b TEC_EXTERNAL_FAULT_3
//!     \b TEC_EXTERNAL_FAULT_4
//!     \b TEC_EXTERNAL_FAULT_5
//!     \b TEC_EXTERNAL_FAULT_6
//! \param signalType is the selected signal type
//! Valid values are
//!     \b TEC_EXTERNAL_FAULT_SIGNALTYPE_EDGE_SENSITIVE  [Default]
//!     \b TEC_EXTERNAL_FAULT_SIGNALTYPE_LEVEL_SENSITIVE 
//! \param signalHold is the selected signal hold
//! Valid values are
//!     \b TEC_EXTERNAL_FAULT_SIGNAL_NOT_HELD [Default]
//!     \b TEC_EXTERNAL_FAULT_SIGNAL_HELD
//! \param polarityBit is the selected signal type
//! Valid values are
//!     \b TEC_EXTERNAL_FAULT_POLARITY_FALLING_EDGE_OR_LOW_LEVEL [Default]
//!     \b TEC_EXTERNAL_FAULT_POLARITY_RISING_EDGE_OR_HIGH_LEVEL 
//!
//! Modified register is \b TECxCTL2 register
//!
//! \return None
//
//*****************************************************************************				
void TEC_configureExternalFaultInput (	unsigned int baseAddress,
										unsigned char selectedExternalFault,
									  	unsigned int signalType,
									  	unsigned char signalHold,
									  	unsigned char polarityBit
									  )
{
	
    assert( selectedExternalFault  == TEC_EXTERNAL_FAULT_0 ||
			selectedExternalFault  == TEC_EXTERNAL_FAULT_1 ||
			selectedExternalFault  == TEC_EXTERNAL_FAULT_2 ||
			selectedExternalFault  == TEC_EXTERNAL_FAULT_3 ||
			selectedExternalFault  == TEC_EXTERNAL_FAULT_4 ||
			selectedExternalFault  == TEC_EXTERNAL_FAULT_5 ||
			selectedExternalFault  == TEC_EXTERNAL_FAULT_6
			);
			
    assert( signalType  == TEC_EXTERNAL_FAULT_SIGNALTYPE_EDGE_SENSITIVE ||
			signalType  == TEC_EXTERNAL_FAULT_SIGNALTYPE_LEVEL_SENSITIVE 
			);
    
    assert( signalHold  == TEC_EXTERNAL_FAULT_SIGNAL_NOT_HELD ||
			signalHold  == TEC_EXTERNAL_FAULT_SIGNAL_HELD 
			);
	assert( polarityBit  == TEC_EXTERNAL_FAULT_POLARITY_FALLING_EDGE_OR_LOW_LEVEL ||
			polarityBit  == TEC_EXTERNAL_FAULT_POLARITY_RISING_EDGE_OR_HIGH_LEVEL 
			);			
			
	HWREGB(baseAddress + OFS_TEC0XCTL2_L)  &= ~((TEC_EXTERNAL_FAULT_SIGNALTYPE_LEVEL_SENSITIVE << selectedExternalFault)+
												(TEC_EXTERNAL_FAULT_POLARITY_RISING_EDGE_OR_HIGH_LEVEL << selectedExternalFault)+
												(TEC_EXTERNAL_FAULT_SIGNAL_HELD << selectedExternalFault )
												);
	
    HWREGB(baseAddress + OFS_TEC0XCTL2_L)  |= ((signalType << selectedExternalFault)+
											   (polarityBit << selectedExternalFault) +			
											   (signalHold << selectedExternalFault )
											   );			
}				
//*****************************************************************************
//
//! Enable the Timer Event Control External fault input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param channelEventBlock selects the channel event block
//! Valid values are 
//!     \b TEC_CE0
//!     \b TEC_CE1
//!     \b TEC_CE2
//!     \b TEC_CE3  (available on TEC5 TEC7)
//!     \b TEC_CE4  (available on TEC5 TEC7)
//!     \b TEC_CE5  (only available on TEC7)
//!     \b TEC_CE6  (only available on TEC7)
//!
//!
//! Modified register is \b TECxCTL0 register
//!
//! \return None
//
//*****************************************************************************
void TEC_enableExternalFaultInput (unsigned int baseAddress,
										unsigned char channelEventBlock
										)
{

	assert( channelEventBlock  == TEC_CE0 ||
			channelEventBlock  == TEC_CE1 || 
			channelEventBlock  == TEC_CE2 ||
			channelEventBlock  == TEC_CE3 || 
			channelEventBlock  == TEC_CE4 ||
			channelEventBlock  == TEC_CE5 || 
			channelEventBlock  == TEC_CE6
			);
			
	HWREGB(baseAddress + OFS_TEC0XCTL0_H)  |= (1 << channelEventBlock );
}	
//*****************************************************************************
//
//! Disable the Timer Event Control External fault input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param channelEventBlock selects the channel event block
//! Valid values are 
//!     \b TEC_CE0
//!     \b TEC_CE1
//!     \b TEC_CE2
//!     \b TEC_CE3  (available on TEC5 TEC7)
//!     \b TEC_CE4  (available on TEC5 TEC7)
//!     \b TEC_CE5  (only available on TEC7)
//!     \b TEC_CE6  (only available on TEC7)
//!
//!
//! Modified register is \b TECxCTL0 register
//!
//! \return None
//
//*****************************************************************************
void TEC_disableExternalFaultInput (unsigned int baseAddress,
										unsigned char channelEventBlock
										)
{

	assert( channelEventBlock  == TEC_CE0 ||
			channelEventBlock  == TEC_CE1 || 
			channelEventBlock  == TEC_CE2 ||
			channelEventBlock  == TEC_CE3 || 
			channelEventBlock  == TEC_CE4 ||
			channelEventBlock  == TEC_CE5 || 
			channelEventBlock  == TEC_CE6
			);
			
	HWREGB(baseAddress + OFS_TEC0XCTL0_H)  &= ~(1 << channelEventBlock );
}			
//*****************************************************************************
//
//! Enable the Timer Event Control External Clear Input
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified register is \b TECxCTL2 register
//!
//! \return None
//
//*****************************************************************************
void TEC_enableExternalClearInput (unsigned int baseAddress )
{
   HWREGB(baseAddress + OFS_TEC0XCTL2_L)  |= TECEXCLREN;
}
//*****************************************************************************
//
//! Disable the Timer Event Control External Clear Input
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified register is \b TECxCTL2 register
//!
//! \return None
//
//*****************************************************************************
void TEC_disableExternalClearInput (unsigned int baseAddress )
{
   HWREGB(baseAddress + OFS_TEC0XCTL2_L)  &= ~TECEXCLREN;
}
		  
//*****************************************************************************
//
//! Enable the Timer Event Control Auxiliary Clear Signal
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified register is \b TECxCTL2 register
//!
//! \return None
//
//*****************************************************************************
void TEC_enableAuxiliaryClearSignal (unsigned int baseAddress )
{
   HWREGB(baseAddress + OFS_TEC0XCTL2_L)  |= TECAXCLREN;
}

//*****************************************************************************
//
//! Disable the Timer Event Control Auxiliary Clear Signal
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified register is \b TECxCTL2 register
//!
//! \return None
//
//*****************************************************************************
void TEC_disableAuxiliaryClearSignal (unsigned int baseAddress )
{
   HWREGB(baseAddress + OFS_TEC0XCTL2_L)  &= ~TECAXCLREN;
}		
//*****************************************************************************
//
//! Clears the Timer Event Control Interrupt flag
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked interrupt flag to be cleared.
//! used. Valid values are
//!     \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag.
//!     \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!     \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! Modified register is \b TECxINT register
//!
//! \return None
//
//*****************************************************************************
void TEC_clearInterruptFlag (unsigned int baseAddress,
    unsigned char mask
    )
{
	assert( 0x00 == ( mask & ~(TEC_EXTERNAL_FAULT_INTERRUPT +
						   TEC_EXTERNAL_CLEAR_INTERRUPT +
						   TEC_AUXILIARY_CLEAR_INTERRUPT 
							   ))
							   );
								   
    HWREGB(baseAddress + OFS_TEC0XINT_L)  &= ~mask;
}


//*****************************************************************************
//
//! Gets the current Timer Event Control interrupt status.
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked interrupt flag status to be returned.
//!
//! This returns the interrupt status for the module based on which
//! flag is passed. mask parameter can be a mask of any of the following
//! selection.
//!
//!     \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag.
//!     \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!     \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! Modified register is None.
//!
//! \returns the masked status of the interrupt flag
//
//*****************************************************************************
unsigned char TEC_getInterruptStatus (unsigned int baseAddress,
    unsigned char mask
    )
{
	assert( 0x00 == ( mask & ~(TEC_EXTERNAL_FAULT_INTERRUPT +
		                           TEC_EXTERNAL_CLEAR_INTERRUPT +
		                           TEC_AUXILIARY_CLEAR_INTERRUPT 
		                               ))
		                               );
    //Return the interrupt status of the request masked bit.
	return (HWREGB(baseAddress + OFS_TEC0XINT_L) & mask);
}
//*****************************************************************************
//
//! Enables individual Timer Event Control interrupt sources.
//!
//! \param baseAddress is the base address of the TEC module.
//! \param interruptFlags is the bit mask of the interrupt sources to be enabled.
//!
//! Enables the indicated Timer Event Control interrupt sources.  
//!	Only the sources that are enabled can be reflected to the 
//!	processor interrupt; disabled sources have no effect on the processor.
//!
//! The mask parameter is the logical OR of any of the following:
//!
//!     \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag.
//!     \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!     \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! Modified registers are \b TECxINT
//!
//! \return None.
//
//*****************************************************************************
void TEC_enableInterrupt (unsigned int baseAddress,
    unsigned char mask
    )
{
	assert( 0x00 == ( mask & ~(TEC_EXTERNAL_FAULT_INTERRUPT +
						   TEC_EXTERNAL_CLEAR_INTERRUPT +
						   TEC_AUXILIARY_CLEAR_INTERRUPT 
							   ))
							   );

	HWREGB(baseAddress + OFS_TEC0XINT_L)  &= ~mask;
    
    //Enable the interrupt masked bit
    HWREGB(baseAddress + OFS_TEC0XINT_H) |= mask;
}

//*****************************************************************************
//
//! Disables individual Timer Event Control interrupt sources.
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the bit mask of the interrupt sources to be
//! disabled.
//!
//! Disables the indicated Timer Event Control interrupt sources.  
//!	Only the sources that are enabled can be reflected to the processor 
//!	interrupt; disabled sources have no effect on the processor.
//!
//! The mask parameter is the logical OR of any of the following:
//!
//!     \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag.
//!     \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!     \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! Modified register is \b TECxINT
//!
//! \return None.
//
//*****************************************************************************
void TEC_disableInterrupt (unsigned int baseAddress,
    unsigned char mask
    )
{
	assert( 0x00 == ( mask & ~(TEC_EXTERNAL_FAULT_INTERRUPT +
						   TEC_EXTERNAL_CLEAR_INTERRUPT +
						   TEC_AUXILIARY_CLEAR_INTERRUPT 
							   ))
							   );

    //Disable the interrupt masked bit
	HWREGB(baseAddress + OFS_TEC0XINT_H) &= ~(mask);
}


//*****************************************************************************
//
//! Gets the current Timer Event Control External Fault Status
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked interrupt flag status to be returned.
//!
//! This returns the Timer Event Control fault status for the module.
//!	The mask parameter can be a mask of any of the following
//! selection.
//!
//!     \b TEC_CE0 - External fault signal in CE0
//!     \b TEC_CE1 - External fault signal in CE1
//!     \b TEC_CE2 - External fault signal in CE2
//!     \b TEC_CE3 - External fault signal in CE3
//!     \b TEC_CE4 - External fault signal in CE4
//!     \b TEC_CE5 - External fault signal in CE5
//!     \b TEC_CE6 - External fault signal in CE6
//!
//! Modified register is None.
//!
//! \returns the masked status of the status flag
//
//*****************************************************************************
unsigned char TEC_getExternalFaultStatus (unsigned int baseAddress,
    unsigned char mask
    )
{
	assert( 0x00 == ( mask & ~(TEC_CE0 +
		                       TEC_CE1 +
		                       TEC_CE2 +  
							   TEC_CE3 +
		                       TEC_CE4 +
							   TEC_CE5 +
		                       TEC_CE6 
	                           ))
		                        );
    //Return the interrupt status of the request masked bit.
	return (HWREGB(baseAddress + OFS_TEC0STA_L) & mask);
}  

//*****************************************************************************
//
//! Clears the Timer Event Control External Fault Status
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked status flag be cleared.
//! used. Valid values are
//!     \b TEC_CE0 - External fault signal in CE0
//!     \b TEC_CE1 - External fault signal in CE1
//!     \b TEC_CE2 - External fault signal in CE2
//!     \b TEC_CE3 - External fault signal in CE3
//!     \b TEC_CE4 - External fault signal in CE4
//!     \b TEC_CE5 - External fault signal in CE5
//!     \b TEC_CE6 - External fault signal in CE6
//!
//! Modified register is \b TECxINT register
//!
//! \return None
//
//*****************************************************************************
void TEC_clearExternalFaultStatus (unsigned int baseAddress,
    unsigned char mask
    )
{
	assert( 0x00 == ( mask & ~(TEC_CE0 +
		                       TEC_CE1 +
		                       TEC_CE2 +  
							   TEC_CE3 +
		                       TEC_CE4 +
							   TEC_CE5 +
		                       TEC_CE6 
	                           ))
		                        );
								   
    HWREGB(baseAddress + OFS_TEC0STA_L)  &= ~mask;
}

//*****************************************************************************
//
//! Gets the current Timer Event Control External Clear Status
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified register is None.
//!
//! \returns the masked status of the status flag
//
//*****************************************************************************
unsigned char TEC_getExternalClearStatus (unsigned int baseAddress)
    
{
    //Return the interrupt status of the request masked bit.
	return (HWREGB(baseAddress + OFS_TEC0STA_L) & TECXCLRSTA);
}  

//*****************************************************************************
//
//! Clears the Timer Event Control External Clear Status 
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified register is \b TECxINT register
//!
//! \return None
//
//*****************************************************************************
void TEC_clearExternalClearStatus (unsigned int baseAddress)
{
   HWREGB(baseAddress + OFS_TEC0STA_L)  &= ~TECXCLRSTA;
}

//*****************************************************************************
//
//Close the Doxygen group.
//! @}
//
//*****************************************************************************					  	
										
