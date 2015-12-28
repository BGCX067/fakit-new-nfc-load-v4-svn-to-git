/**
  ******************************************************************************
  * @file    Timer A.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   Timer A setting
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
#include "../USB_API/USB_Common/device.h"
#include "../USB_API/USB_Common/types.h"               //Basic Type declarations

#include "inc/hw_memmap.h"
#include "timer_a.h"
#include "wdt_a.h"
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
///////////////////////////////////////
// ACLK = 32768 Hz
// SMCLK = 2 MHz
// ////////////////////////////////////
#if defined (_Config_TIMER_A_PERIOD_100MS_)
    #define TIMERA_CLOCKSOURCE          TIMER_A_CLOCKSOURCE_SMCLK
    #define TIMERA_CLOCKSOURCE_DIVIDER  TIMER_A_CLOCKSOURCE_DIVIDER_4   //  = SMCLK / 8
    #define TIMER_A_COUNT               25001    //25000 + 1
#elif defined (_Config_TIMER_A_PERIOD_10MS_)
    #define TIMERA_CLOCKSOURCE          TIMER_A_CLOCKSOURCE_SMCLK
    #define TIMERA_CLOCKSOURCE_DIVIDER  TIMER_A_CLOCKSOURCE_DIVIDER_1
    #define TIMER_A_COUNT               20001   //20000 + 1
#else
    #error "please define Timer A Period"
#endif


//#define TIMER_A_PERIOD_100MS
//#define COMPARE_VALUE 50000
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
//UINT16 Setting_Interrupt_Calling_TimingDelay_TimerA;
//__IO UINT16 Interrupt_Calling_TimingDelay_counter_TimerA;
void (*Interrupt_TimerA_ptr_fuc[Max_TimerA_INTERRUPT_Function_Calling])(void);
void empty_timerA_fun(void){}


//==============================================================================
// Private functions
//==============================================================================
/**
  * @brief  Configure TIM A peripheral
  * @param  None
  * @retval None
  */
void _Device_Init_Timer_A (void)
{
    //Stop WDT
    //WDT_A_hold(WDT_A_BASE);

	int i;


    for(i = 0; i < Max_TimerA_INTERRUPT_Function_Calling; i++){
        Interrupt_TimerA_ptr_fuc[i] = empty_timerA_fun;
    }


//    //Set P1.0 to output direction
//    GPIO_setAsOutputPin(
//        GPIO_PORT_P1,
//        GPIO_PIN0
//        );


    TIMER_A_clearTimerInterruptFlag(TIMER_A1_BASE);
    //Initiaze compare mode
	TIMER_A_clearCaptureCompareInterruptFlag(TIMER_A1_BASE,
		TIMER_A_CAPTURECOMPARE_REGISTER_0
		);

    //Start timer in up mode
    TIMER_A_configureUpMode(
        TIMER_A1_BASE,
        TIMERA_CLOCKSOURCE,
        TIMERA_CLOCKSOURCE_DIVIDER,
        TIMER_A_COUNT,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR
        );


//    //Start timer in continuous mode sourced by SMCLK
//    TIMER_A_configureContinuousMode( TIMER_A1_BASE,
//        TIMER_A_CLOCKSOURCE_SMCLK,
//        //TIMER_A_CLOCKSOURCE_ACLK
//        TIMER_A_CLOCKSOURCE_DIVIDER_1,
//        TIMER_A_TAIE_INTERRUPT_DISABLE,
//        TIMER_A_DO_CLEAR
//        );

//    //Initiaze compare mode
//	TIMER_A_clearCaptureCompareInterruptFlag(TIMER_A1_BASE,
//		TIMER_A_CAPTURECOMPARE_REGISTER_0
//		);
//    TIMER_A_initCompare(TIMER_A1_BASE,
//        TIMER_A_CAPTURECOMPARE_REGISTER_0,
//        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
//        TIMER_A_OUTPUTMODE_OUTBITVALUE,
//        COMPARE_VALUE
//        );
//
//    TIMER_A_startCounter( TIMER_A1_BASE,
//    		TIMER_A_CONTINUOUS_MODE
//                );

    //Enter LPM0, enable interrupts
    //__bis_SR_register(LPM0_bits + GIE);

    //For debugger
    __no_operation();
}

void _Device_Enable_Timer_A(){
    TIMER_A_startCounter( TIMER_A1_BASE, TIMER_A_UP_MODE );
}
void _Device_Disable_Timer_A(){
    TIMER_A_stop(TIMER_A1_BASE );
}
void _Device_Set_TimerA_Interrupt_Timer_Calling_Function(t_uint8 fun_index, void (*calling_fun)()){
    if(fun_index >= Max_TimerA_INTERRUPT_Function_Calling){
        return;
    }
    Interrupt_TimerA_ptr_fuc[fun_index] = calling_fun;
}
void _Device_Remove_TimerA_Interrupt_Timer_Calling_Function(t_uint8 fun_index){
    if(fun_index >= Max_TimerA_INTERRUPT_Function_Calling){
        return;
    }
    Interrupt_TimerA_ptr_fuc[fun_index] = empty_timerA_fun;
}
//******************************************************************************
//
//This is the TIMER1_A3 interrupt vector service routine.
//
//******************************************************************************
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{
//    //Toggle P1.0
//    GPIO_toggleOutputOnPin(
//        GPIO_PORT_P1,
//        GPIO_PIN0
//        );
    for( t_uint8 i = 0; i < Max_TimerA_INTERRUPT_Function_Calling; i++){
        (*Interrupt_TimerA_ptr_fuc[i])();
    }
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM0-3
//    //Add Offset to CCR0
//    TIMER_A_setCompareValue(TIMER_A1_BASE,
//        TIMER_A_CAPTURECOMPARE_REGISTER_0,
//        COMPARE_VALUE
//        );
}

