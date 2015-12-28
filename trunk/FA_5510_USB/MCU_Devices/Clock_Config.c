
/**
  ******************************************************************************
  * @file    Power_Management_Module_Config.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   Power_Management_Module_  setting
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
#include "ucs.h"
#include "sfr.h"

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
#define UCS_XT1_CRYSTAL_FREQUENCY   0
#define UCS_XT2_CRYSTAL_FREQUENCY   4000000 //4MHz

#define UCS_XT2_TIMEOUT     5000

#define HF_XT2_Crystal_Port         GPIO_PORT_P5
#define HF_XT2_Crystal_In_Pin       GPIO_PIN2
#define HF_XT2_Crystal_Out_Pin      GPIO_PIN3

//Target frequency for MCLK in kHz
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ   8000

// MCLK/FLLRef Ratio
#define UCS_MCLK_FLLREF_RATIO   2   // 8MHz / 4MHz(XT2) = 2

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


///*
// * ======== Init_Clock ========
// */
//VOID Init_Clock_By_TI_For_USB (VOID)
//{
//
//    SetVCore(3);
//
//    //Initialization of clock module
//    if (USB_PLL_XT == 2){
//		#if defined (__MSP430F552x) || defined (__MSP430F550x)
//			P5SEL |= 0x0C;                                      //enable XT2 pins for F5529
//		#elif defined (__MSP430F563x_F663x)
//			P7SEL |= 0x0C;
//		#endif
//
//        //use REFO for FLL and ACLK
//        UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);  // set FLL reference clock source as REFOCLK
//        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);  // set ACLK as REFOCLK
//
//        //MCLK will be driven by the FLL (not by XT2), referenced to the REFO
//        Init_FLL_Settle(USB_MCLK_FREQ / 1000, USB_MCLK_FREQ / 32768);   //Start the FLL, at the freq indicated by the config
//                                                                        //constant USB_MCLK_FREQ
//        XT2_Start(XT2DRIVE_0);                                          //Start the "USB crystal"
//    }
//	else {
//		#if defined (__MSP430F552x) || defined (__MSP430F550x)
//			P5SEL |= 0x10;                                      //enable XT1 pins
//		#endif
//        //Use the REFO oscillator to source the FLL and ACLK
//        UCSCTL3 = SELREF__REFOCLK;
//        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);
//
//        //MCLK will be driven by the FLL (not by XT2), referenced to the REFO
//        Init_FLL_Settle(USB_MCLK_FREQ / 1000, USB_MCLK_FREQ / 32768);   //set FLL (DCOCLK)
//
//        XT1_Start(XT1DRIVE_0);                                          //Start the "USB crystal"
//    }
//
//    UCSCTL5 = (UCSCTL5 & ~(DIVS0 + DIVS1 + DIVS2)) | (DIVS0);   //SMCLK/2 = 8mhz / 2 = 4 mhz
//}


    t_uint32 clockValue = 0;
void _Device_Init_Clock_Module(void)
{
    t_uint8 status = 0;


//    //ACLK set out to pins
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//        GPIO_PORT_P1,
//        GPIO_PIN0
//        );
//    //SMCLk set out to pins
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//        GPIO_PORT_P2,
//        GPIO_PIN2
//        );
//    //MCLK set out to pins
//    GPIO_setAsPeripheralModuleFunctionOutputPin(
//        GPIO_PORT_P7,
//        GPIO_PIN7
//        );

    //Initializes the XT1 and XT2 crystal frequencies being used
    UCS_setExternalClockSource(  UCS_BASE,
        UCS_XT1_CRYSTAL_FREQUENCY,
        UCS_XT2_CRYSTAL_FREQUENCY
        );

    //Set DCO FLL reference = XT2CLK
    UCS_clockSignalInit(
        UCS_BASE,
        UCS_FLLREF,
        UCS_XT2CLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );
    //Set ACLK = REFO
    UCS_clockSignalInit(
        UCS_BASE,
        UCS_ACLK,
        UCS_REFOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );


    //Startup HF XT2 crystal Port select XT2
    GPIO_setAsPeripheralModuleFunctionInputPin(
        HF_XT2_Crystal_Port,
        HF_XT2_Crystal_In_Pin + HF_XT2_Crystal_Out_Pin
        );

    //Initailize XT2. Returns STATUS_SUCCESS if initializes successfully
    status = UCS_XT2StartWithTimeout(
        UCS_BASE,
        UCS_XT2DRIVE_4MHZ_8MHZ,
        UCS_XT2_TIMEOUT
        );
    if (status != (STATUS_SUCCESS)){
        status = 0;
    }

    //Set Ratio and Desired MCLK Frequency  and initialize DCO ==>DCOCLKDIV
    UCS_initFLLSettle(
        UCS_BASE,
        UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ,
        UCS_MCLK_FLLREF_RATIO
        );

    //Set SMCLK = DCOCLKDIV
    UCS_clockSignalInit(
        UCS_BASE,
        UCS_SMCLK,
        UCS_DCOCLKDIV_SELECT,
        UCS_CLOCK_DIVIDER_4
        );
//    //Set SMCLK = UCS_XT2CLK_SELECT
//    UCS_clockSignalInit(
//        UCS_BASE,
//        UCS_SMCLK,
//        UCS_XT2CLK_SELECT,
//        UCS_CLOCK_DIVIDER_4
//        );
    //Set MCLK = DCOCLKDIV
    UCS_clockSignalInit(
        UCS_BASE,
        UCS_MCLK,
        UCS_DCOCLKDIV_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

    //Delay
    __delay_cycles(1000000);


    // If it still can't clear the oscillator fault flags after the timeout,
    // trap and wait here.
    status = UCS_clearAllOscFlagsWithTimeout(UCS_BASE,
                                             1000
                                             );



    // Enable global oscillator fault flag
	SFR_clearInterrupt(SFR_BASE,
		SFR_OSCILLATOR_FAULT_INTERRUPT
		);
    SFR_enableInterrupt(SFR_BASE,
		SFR_OSCILLATOR_FAULT_INTERRUPT
		);

    // Enable global interrupt
    //__bis_SR_register(GIE);

//    //Verify if the Clock settings are as expected
    clockValue = UCS_getSMCLK(UCS_BASE);
    clockValue = UCS_getMCLK(UCS_BASE);
    clockValue = UCS_getACLK(UCS_BASE);

    status = status;
    //clockValue = clockValue;
}

t_uint32 _Device_Get_Clock_Source_SMCLK(){
    return UCS_getSMCLK(UCS_BASE);
}
t_uint32 _Device_Get_Clock_Source_MCLK(){
    return UCS_getMCLK(UCS_BASE);
}
t_uint32 _Device_Get_Clock_Source_ACLK(){
    return UCS_getACLK(UCS_BASE);
}




void _Device_Clock_Source_Set_Out_To_Pin(){

//  PMAPPWD = 0x02D52;                 // Enable Write-access to modify port mapping registers
//  P4MAP7 = PM_MCLK;
//  PMAPPWD = 0;                       // Disable Write-Access to modify port mapping registers


    //ACLK set out to pins
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0
        );
    //SMCLk set out to pins
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN2
        );
#if defined (__MSP430F552x)
    //MCLK set out to pins
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P7,
        GPIO_PIN7
        );
#elif defined (__MSP430F550x)
    //MCLK set out to pins
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P4,
        GPIO_PIN7
        );
#endif

//
//  P1DIR |= BIT1;                            // P1.1 output
//
//  P1DIR |= BIT0;                            // ACLK set out to pins
//  P1SEL |= BIT0;
//  P2DIR |= BIT2;                            // SMCLK set out to pins
//  P2SEL |= BIT2;
//#if defined (__MSP430F552x)
//  P7SEL |= BIT7;
//  P7DIR |= BIT7;                            // MCLK set out to pins
//#elif defined (__MSP430F550x)
//  P4SEL |= BIT7;
//  P4DIR |= BIT7;                            // MCLK set out to pins
//#endif
//
}


//#pragma vector=UNMI_VECTOR
//__interrupt void NMI_ISR(void)
//{
//    t_uint8 status;
//  do {
//    // If it still can't clear the oscillator fault flags after the timeout,
//    // trap and wait here.
//    status = UCS_clearAllOscFlagsWithTimeout(UCS_BASE,
//                                             1000
//                                             );
//  } while(status != 0);
//
//  status = status;
//}
//
