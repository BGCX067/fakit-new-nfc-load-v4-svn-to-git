
/**
  ******************************************************************************
  * @file    Port2_IO.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   Port2_IO  setting
  ******************************************************************************
  * @attention
  *
  * DESCRIPTION....
  *
  * <h2><center>&copy; COPYRIGHT 2013 Dynapack</center></h2>
  ******************************************************************************
  */

//==============================================================================
// Includes
//==============================================================================

//#include "../USB_config/descriptors.h"
//
#include "../USB_API/USB_Common/device.h"
//#include "../USB_API/USB_Common/types.h"               //Basic Type declarations
////#include "../USB_API/USB_Common/usb.h"                 //USB-specific functions
////
//#include "../F5xx_F6xx_Core_Lib/HAL_UCS.h"
//#include "../F5xx_F6xx_Core_Lib/HAL_PMM.h"
////
////#include "../USB_API/USB_CDC_API/UsbCdc.h"



#include "inc/hw_memmap.h"
#include "gpio.h"


#include "MCU_Devices.h"
//==============================================================================
// Global/Extern variables
//==============================================================================
//==============================================================================
// Extern functions
//==============================================================================
//==============================================================================
// Private typedef
//==============================================================================
//==============================================================================
// Private define
//==============================================================================


//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//==============================================================================
// Private variables
//==============================================================================

//==============================================================================
// Private function prototypes
//==============================================================================


//==============================================================================
// Private functions
//==============================================================================

void Port2_Pin5_6_Init(){

    //set as  output  pins
    GPIO_setAsOutputPin( GPIO_PORT_P2,GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );

    GPIO_setAsOutputPin( GPIO_PORT_P2,GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN6 );

//    //Enable Pin internal resistance as pull-Up resistance
//    GPIO_setAsInputPinWithPullUpresistor(
//        GPIO_PORT_P2,
//        GPIO_PIN4
//        );
//
//	//Pin IFG cleared
//    GPIO_clearInterruptFlag(
//        GPIO_PORT_P2,
//        GPIO_PIN4
//        );
//
//    //Pin interrupt enabled
//    GPIO_enableInterrupt(
//        GPIO_PORT_P2,
//        GPIO_PIN4
//        );
//
//    //Pin Hi/Lo edge
//    GPIO_interruptEdgeSelect(
//        GPIO_PORT_P2,
//        GPIO_PIN4,
//        GPIO_HIGH_TO_LOW_TRANSITION
//        );
}

//******************************************************************************
//
//This is the PORT2_VECTOR interrupt vector service routine
//
//******************************************************************************
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void)
{
    __delay_cycles(10000);   //1234us at 8MHz
    if (GPIO_INPUT_PIN_LOW == GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN4)){
         __delay_cycles(10);
    }else{
         __delay_cycles(10);
    }

    //Pin IFG cleared
    GPIO_clearInterruptFlag(
        GPIO_PORT_P2,
        GPIO_PIN4
        );
}



