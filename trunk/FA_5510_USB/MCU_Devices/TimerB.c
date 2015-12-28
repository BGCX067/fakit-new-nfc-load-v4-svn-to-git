/**
  ******************************************************************************
  * @file    Timer B.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   Timer B setting
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
#include "timer_b.h"
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
#if defined (_Config_TIMER_B_PERIOD_1MS_)
    #define TIMERB_CLOCKSOURCE          TIMER_B_CLOCKSOURCE_SMCLK       //2MHz
    #define TIMERB_CLOCKSOURCE_DIVIDER  TIMER_B_CLOCKSOURCE_DIVIDER_1   //  = SMCLK / 8
    #define TIMER_B_COUNT               2001    //25000 + 1
#elif defined (_Config_TIMER_B_PERIOD_10MS_)
    #define TIMERB_CLOCKSOURCE          TIMER_B_CLOCKSOURCE_SMCLK
    #define TIMERB_CLOCKSOURCE_DIVIDER  TIMER_B_CLOCKSOURCE_DIVIDER_1
    #define TIMER_B_COUNT               20001   //20000 + 1
#else
    #error "please define Timer B Period"
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
void (*Interrupt_TimerB_ptr_fuc[Max_TimerB_INTERRUPT_Function_Calling])(void);
void empty_timerB_fun(void){}

__IO t_uint16 Setting_Interrupt_Calling_TimingDelay_TimerB[Max_TimerB_INTERRUPT_Function_Calling];
__IO t_uint16 Interrupt_Calling_TimingDelay_counter_TimerB[Max_TimerB_INTERRUPT_Function_Calling];
__IO t_uint16 Interrupt_Calling_Function_Bit_Flag;

__IO uint32_t TimingDelay;
//==============================================================================
// Private functions
//==============================================================================
/**
  * @brief  Configure TIM B peripheral
  * @param  None
  * @retval None
  */
void _Device_Init_Timer_B (void)
{
    //Stop WDT
    //WDT_A_hold(WDT_A_BASE);

	t_uint16 i;


    for(i = 0; i < Max_TimerB_INTERRUPT_Function_Calling; i++){
        Interrupt_TimerB_ptr_fuc[i] = empty_timerB_fun;
        Setting_Interrupt_Calling_TimingDelay_TimerB[i] = 0;
        Interrupt_Calling_TimingDelay_counter_TimerB[i] = 0;
    }


//    //Set P1.0 to output direction
//    GPIO_setAsOutputPin(
//        GPIO_PORT_P1,
//        GPIO_PIN0
//        );

    //Start timer in up mode
	TIMER_B_clearTimerInterruptFlag(TIMER_B0_BASE);
    TIMER_B_configureUpMode(   TIMER_B0_BASE,
        TIMERB_CLOCKSOURCE,
        TIMERB_CLOCKSOURCE_DIVIDER,
        TIMER_B_COUNT,      //TIMER_PERIOD,
        TIMER_B_TBIE_INTERRUPT_DISABLE,
        TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE,
        TIMER_B_DO_CLEAR
        );


    __no_operation();
}

void _Device_Enable_Timer_B(){
    TIMER_B_startCounter(
		TIMER_B0_BASE,
		TIMER_B_UP_MODE
		);
}
void _Device_Disable_Timer_B(){
    TIMER_B_stop(TIMER_B0_BASE );
}

void _Device_Set_TimerB_Interrupt_Timer_Calling_Function_With_Delay_And_Exec(t_uint8 fun_index, void (*calling_fun)(), __IO t_uint16 ms_Dealy ){
    t_uint8 calling_count;
    t_uint8 i;

    if(fun_index >= Max_TimerB_INTERRUPT_Function_Calling){
        return;
    }
    if(ms_Dealy <= 1){
        ms_Dealy = 1;
    }
    if(Interrupt_TimerB_ptr_fuc[fun_index] == calling_fun){
        //ReStart Count
        Interrupt_Calling_TimingDelay_counter_TimerB[fun_index] = 0;
        Setting_Interrupt_Calling_TimingDelay_TimerB[fun_index] = ms_Dealy - 1;
        return;
    }

    Interrupt_TimerB_ptr_fuc[fun_index] = calling_fun;
    Setting_Interrupt_Calling_TimingDelay_TimerB[fun_index] = ms_Dealy - 1;
    Interrupt_Calling_TimingDelay_counter_TimerB[fun_index] = 0;
    ///////////////////////////////////////////////////////////////////////////
    calling_count=0;
    for( i = 0; i < Max_TimerB_INTERRUPT_Function_Calling; i++){
        if(i == fun_index){
            continue;
        }
        if(Interrupt_TimerB_ptr_fuc[i] != empty_timerB_fun){
            calling_count++;
        }
    }
    //without current setting function
    if(calling_count == 0){
        _Device_Enable_Timer_B();
    }
}


void _Device_Remove_TimerB_Interrupt_Timer_Calling_Function(uint8_t fun_index){
    t_uint8 calling_count;
    t_uint8 i;
    if(fun_index >= Max_TimerB_INTERRUPT_Function_Calling){
        return;
    }
    Interrupt_TimerB_ptr_fuc[fun_index] = empty_timerB_fun;
    Setting_Interrupt_Calling_TimingDelay_TimerB[fun_index] = 0;
    Interrupt_Calling_TimingDelay_counter_TimerB[fun_index] = 0;
    ///////////////////////////////////////////////////////////////////////////
    calling_count=0;
    for( i = 0; i < Max_TimerB_INTERRUPT_Function_Calling; i++){
        if(Interrupt_TimerB_ptr_fuc[i] != empty_timerB_fun){
            calling_count++;
        }
    }
    if(calling_count == 0){
        _Device_Disable_Timer_B();
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//void _Device_Set_TimerA_Interrupt_Timer_Calling_Function(t_uint8 fun_index, void (*calling_fun)()){
//    if(fun_index >= Max_TimerB_INTERRUPT_Function_Calling){
//        return;
//    }
//    Interrupt_TimerB_ptr_fuc[fun_index] = calling_fun;
//}
//void _Device_Remove_TimerB_Interrupt_Timer_Calling_Function(t_uint8 fun_index){
//    if(fun_index >= Max_TimerB_INTERRUPT_Function_Calling){
//        return;
//    }
//    Interrupt_TimerB_ptr_fuc[fun_index] = empty_timerB_fun;
//}
//******************************************************************************
//
//This is the Timer B0 interrupt vector service routine.
//
//******************************************************************************
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR (void)
{
//    //Toggle P1.0
//    GPIO_toggleOutputOnPin(
//        GPIO_PORT_P1,
//        GPIO_PIN0
//        );
//    for( t_uint8 i = 0; i < Max_TimerB_INTERRUPT_Function_Calling; i++){
//        (*Interrupt_TimerB_ptr_fuc[i])();
//    }

	t_uint16 i;
    t_uint8 empty_Count;
    t_uint8 setting_Count;



    empty_Count = 0;
    for( i = 0; i < Max_TimerB_INTERRUPT_Function_Calling; i++){
        if(Interrupt_TimerB_ptr_fuc[i] == empty_timerB_fun){
            empty_Count++;
        }else{
            setting_Count = Setting_Interrupt_Calling_TimingDelay_TimerB[i];
            if(Interrupt_Calling_TimingDelay_counter_TimerB[i] >= setting_Count){
                (*Interrupt_TimerB_ptr_fuc[i])();
                Interrupt_TimerB_ptr_fuc[i] = empty_timerB_fun;
                Interrupt_Calling_TimingDelay_counter_TimerB[i] = 0;
            }else{
                Interrupt_Calling_TimingDelay_counter_TimerB[i]++;
            }
        }
    }

    if(empty_Count >= Max_TimerB_INTERRUPT_Function_Calling){
        _Device_Disable_Timer_B();
    }

}

