/**
  ******************************************************************************
  * @file    System Function control.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    6-Jan-2014
  * @brief   System Function control setting
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
#include "inc/hw_memmap.h"

#include "gpio.h"
//#include "adc10_a.h"
#include "ref.h"
//#include "msp_dma.h"
//#include "ucs.h"
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
#define VPD_GATE_PORT               GPIO_PORT_P1
#define VPD_GATE_PIN                GPIO_PIN1

#define VPC_GATE_PORT               GPIO_PORT_P1
#define VPC_GATE_PIN                GPIO_PIN2

#define LOADING_GATE_PORT           GPIO_PORT_P5
#define LOADING_GATE_PIN            GPIO_PIN0

#define CHGING_VIA_PACK_D_GATE_PORT     GPIO_PORT_P1
#define CHGING_VIA_PACK_D_GATE_PIN      GPIO_PIN0

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

void _Device_System_Func_Ctrl_Init(void){

    //Init VPD_GATE (which is for mesurement of Voltage of PACK DSG)
    GPIO_setAsOutputPin(VPD_GATE_PORT, VPD_GATE_PIN);
    GPIO_setOutputLowOnPin(VPD_GATE_PORT, VPD_GATE_PIN);

    //Init VPC_GATE (which is for mesurement of Voltage of PACK CHG)
    GPIO_setAsOutputPin(VPC_GATE_PORT, VPC_GATE_PIN);
    GPIO_setOutputLowOnPin(VPC_GATE_PORT, VPC_GATE_PIN);

    //Init LOADING_GATE (set Hi level)(which is for Loading through PACK DSG )
    GPIO_setAsOutputPin(LOADING_GATE_PORT, LOADING_GATE_PIN);
    GPIO_setOutputLowOnPin(LOADING_GATE_PORT, LOADING_GATE_PIN);

    //Init CHGING_VIA_PACK_D_GATE (Charging through PACK DSG by switching Relay)
    GPIO_setAsOutputPin(CHGING_VIA_PACK_D_GATE_PORT, CHGING_VIA_PACK_D_GATE_PIN);
    GPIO_setOutputLowOnPin(CHGING_VIA_PACK_D_GATE_PORT, CHGING_VIA_PACK_D_GATE_PIN);
}

void _Device_Set_VPD_GATE(Device_Status status){

    if(status == Device_On){
        GPIO_setOutputHighOnPin(VPD_GATE_PORT, VPD_GATE_PIN);
    }else{
        GPIO_setOutputLowOnPin(VPD_GATE_PORT, VPD_GATE_PIN);
    }
}
void _Device_Set_VPC_GATE(Device_Status status){

    if(status == Device_On){
        GPIO_setOutputHighOnPin(VPC_GATE_PORT, VPC_GATE_PIN);
    }else{
        GPIO_setOutputLowOnPin(VPC_GATE_PORT, VPC_GATE_PIN);
    }
}
void _Device_Set_LOADING_GATE(Device_Status status){

    if(status == Device_On){
        GPIO_setOutputHighOnPin(LOADING_GATE_PORT, LOADING_GATE_PIN);
    }else{
        GPIO_setOutputLowOnPin(LOADING_GATE_PORT, LOADING_GATE_PIN);
    }
}
void _Device_Set_CHGING_VIA_PACK_D_GATE(Device_Status status){

    if(status == Device_On){
        GPIO_setOutputHighOnPin(CHGING_VIA_PACK_D_GATE_PORT, CHGING_VIA_PACK_D_GATE_PIN);
    }else{
        GPIO_setOutputLowOnPin(CHGING_VIA_PACK_D_GATE_PORT, CHGING_VIA_PACK_D_GATE_PIN);
    }
}

