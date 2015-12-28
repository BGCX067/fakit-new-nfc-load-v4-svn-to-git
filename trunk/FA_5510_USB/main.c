
//#define _Debug_Disable_USB_Function_

/**
  ******************************************************************************
  * @file    main.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   main
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
#include <intrinsics.h>
#include <string.h>

#include "Global_Vars_Define.h"
#include "Vars_Bit_Define.h"
#include "SystemConfigDefineForFlash.h"

#include "USB_config/descriptors.h"

#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"               //Basic Type declarations
#include "USB_API/USB_Common/usb.h"                 //USB-specific functions

#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"
#include "F5xx_F6xx_Core_Lib/HAL_PMM.h"

#include "USB_API/USB_CDC_API/UsbCdc.h"
//#include "usbConstructs.h"

#include "gpio.h"

#include "MCU_Devices/TypeDefine.h"
#include "DUI_For_USB_CDC.h"
#include "DUI_For_UART.h"
#include "DUI_For_Peripheral_Control.h"
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

//#define Auto_Check_Charger_result_Size  24
//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//==============================================================================
// Private variables
//==============================================================================
unsigned int G_Var_Array[Global_VarArray_Int_Size];

//t_uint16 g_ADC_Result_data16[3];

//t_uint16 g_Auto_Check_Charger_result[Auto_Check_Charger_result_Size];
extern t_uint8 gCdcTempUint8;
//==============================================================================
// Private function prototypes
//==============================================================================
void Processing_Charger_Check(MeasuredFunctions Measured_Voltage_Chger, Switch_Channnel Switch_To_Voltage_Ch);
void Clear_Temp_Array_Buffer(unsigned int* ptr_StartArray, unsigned char num);
void DelayCycles(unsigned int cycles){unsigned int i; for(i=0;i<cycles;i++) __delay_cycles(5000);/*634us at 8MHz*/}

//==============================================================================
// Private functions
//==============================================================================
void LED1_Test(){
    //Toggle P1.0
    //GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN6 );
    //GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    //GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
}
void LED2_Test(){
    ////Toggle P8.1
    //GPIO_toggleOutputOnPin( GPIO_PORT_P8, GPIO_PIN1 );
    GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN5 );
}
void LED3_Test(){
    //Toggle P8.2
    //GPIO_toggleOutputOnPin( GPIO_PORT_P8, GPIO_PIN2 );
    GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN4 );
}
void PAD1_LED_Test(){
    //Toggle P1.1
    GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN1 );
}
void PAD2_LED_Test(){
    //Toggle P1.2
    GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN2 );
}
void PAD3_LED_Test(){
    //Toggle P1.3
    GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN3 );
}
void PAD4_LED_Test(){
    //Toggle P1.4
    GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN4 );
}
void PAD5_LED_Test(){
    //Toggle P1.5
    GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN5 );
}
void ADC_Test(){
    if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
        //GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN6 );
        GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5 );
        _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Chger_Vol_ch);
        _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
        //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
    }else if(G_Module_Function_Status & ADC_Done_Conversion){
        //GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN4 );
        GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );

        G_Module_Function_Status &= ~ADC_Done_Conversion;
        G_Module_Function_Status &= ~ADC_Start_Conversion;

        //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN6 );
        //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN4 );
    }



}



/*----------------------------------------------------------------------------+
 | Main Routine                                                                |
 +----------------------------------------------------------------------------*/
//            __delay_cycles(100);    //12us at 8MHz
//            __delay_cycles(1000);   //130us at 8MHz
//            __delay_cycles(2000);   //260us at 8MHz
//            __delay_cycles(3000);   //390us at 8MHz
//            __delay_cycles(5000);   //634us at 8MHz
//            __delay_cycles(10000);   //1260us at 8MHz


void main (void)
{
    WDTCTL = WDTPW + WDTHOLD;   //Stop watchdog timer

    _DUI_Init_IO_Ports();       //Init ports (do first ports because clocks do change ports)

    //////////////////////////////////////////////////////////
    // status init
    G_Module_Status = 0;
    G_Module_Function_Status = 0;
    G_1st_Module_Function_Status = 0;

//    _Device_Init_I2C_IO_Port_As_Input();
//    if((_Device_get_SDA_Pin_Status() == IO_INPUT_LOW) && (_Device_get_SCL_Pin_Status() == IO_INPUT_HIGH)){
//        G_Module_Status |= SetAsOnlyComPort;
//    }else{
//        G_Module_Status &= ~SetAsOnlyComPort;
//    }
    //////////////////////////////////////////////////////////
    // direct call from device
    _DUI_Init_Power_Management_Module();
    //_Device_Clock_Source_Set_Out_To_Pin();
    _DUI_Init_Clock_Module();

    //while(1);

#if !defined(_Debug_Disable_USB_Function_)
    _DUI_Init_USB_AS_CDC_Communication();
#endif

    _DUI_Charger_Function_Init();
    _DUI_Commun_MUX_Init();
    _DUI_System_Function_Ctrl_Init();
    //_DUI_ADC_Function_Init();
    _DUI_ADC_Function_Init_for_RepeatedSingleCh();

    _DUI_Init_Polling_Timer();


#if !defined(_Debug_Disable_USB_Function_)
    _DUI_Set_Function_To_Polling(_DUI_USB_CDC_Polling_Status_Function);
#endif

    //_DUI_Set_Function_To_Polling(_DUI_Auto_check_Charger_for_Pollong);
    //_DUI_Set_Function_To_Polling(LED1_Test);
    //_DUI_Set_Function_To_Polling(ADC_Test);

    //_DUI_Set_Function_To_Polling(LED2_Test);
    //_DUI_Set_Function_To_Polling(LED3_Test);
    _DUI_Start_Polling_Timer();


    _DUI_Init_Comm_Packet_Form_Detection_Timer();
    _DUI_Communication_Enable(Uart_RS485_Module);
    _DUI_Communication_Enable(One_Wire_Module);


    //////////////////////////////////
    // Test
//    _DUI_Switch_To_Communication_Port(RS485_Channel);
//    _DUI_Switch_To_Communication_Port(UART_Channel);
//    _DUI_Switch_To_Communication_Port(RS485_Channel);
//    _DUI_Switch_To_Communication_Port(UART_Channel);
//
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_1st_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_2nd_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_3rd_Step);
//
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_24V_Ch, Chger_ID_1st_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_24V_Ch, Chger_ID_2nd_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_24V_Ch, Chger_ID_3rd_Step);
//
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_36V_Ch, Chger_ID_1st_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_36V_Ch, Chger_ID_2nd_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_36V_Ch, Chger_ID_3rd_Step);
//
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_48V_Ch, Chger_ID_1st_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_48V_Ch, Chger_ID_2nd_Step);
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_48V_Ch, Chger_ID_3rd_Step);
//
//    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
//
//    _DUI_SetPackDSGInputPortForMeasurement(Turn_On);
//    _DUI_SetPackDSGInputPortForMeasurement(Turn_Off);
//
//    _DUI_SetPackCHGInputPortForMeasurement(Turn_On);
//    _DUI_SetPackCHGInputPortForMeasurement(Turn_Off);
//
//    _DUI_SetKitLoading(Turn_On);
//    _DUI_SetKitLoading(Turn_Off);
//
//    _DUI_SetChargingViaDSGPort(Turn_On);
//    _DUI_SetChargingViaDSGPort(Turn_Off);


    //////////////////////////////////

    //_DUI_Set_Charger_Channel(CHG_ON_Channel);

    __enable_interrupt();                           //Enable interrupts globally

    //G_Module_Function_Status |= Set_Charger_24V_Measured_Processing;


    //t_uint8 count = 0;

    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN6 );
    GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN6 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN4 );



    while(1){
            _NOP();

//            ////////////////////////////////////
//            GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN4 );
//            DelayCycles(10);
//            GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN4 );
//            DelayCycles(10);
//            ////////////////////////////////////


            //GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN5 );
            //ADC_Test();
            //Delay
            //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN6 );
            //GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN6 );
//            __delay_cycles(100);    //12us at 8MHz
//            __delay_cycles(1000);   //130us at 8MHz
//            __delay_cycles(2000);   //260us at 8MHz
//            __delay_cycles(3000);   //390us at 8MHz
//            __delay_cycles(5000);   //634us at 8MHz
//            __delay_cycles(10000);   //1260us at 8MHz

            //__delay_cycles(1000000);

            //LED2_Test();
            //_DUI_Set_Charger_Channel(CHG_ON_Channel);
            //G_Module_Function_Status |= Set_Charger_24V_Measured_Processing;


            /////////////////////////////////////////////////////////////////////
            // First Measured Processing For delete empty result
            if(!(G_Module_Function_Status & ADC_First_Conversion)){
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    G_Module_Function_Status |= ADC_First_Conversion;
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing For 24V Charger
            }else if((G_Module_Function_Status & Set_Charger_24V_Measured_Processing)&&((G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)==0)){
                if(G_Module_Function_Status & Process_Charger_Measured_Done){
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_24V_Voltage_Auto,(t_uint8 *)(&G_Temp_ADC_L0), 12);
                    //Process_Charger_Check_Done
                    G_Module_Function_Status &= ~Process_Charger_Measured_Done;
                    G_Module_Function_Status &= ~Set_Charger_24V_Measured_Processing;
                    //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
                }else{
                    // process start
                    //GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5 );
                    Processing_Charger_Check(Measured_24V_Chger, Switch_To_24V_Ch);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing For 36V Charger
            }else if((G_Module_Function_Status & Set_Charger_36V_Measured_Processing)&&((G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)==0)){
                if(G_Module_Function_Status & Process_Charger_Measured_Done){
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_36V_Voltage_Auto,(t_uint8 *)(&G_Temp_ADC_L0), 12);
                    //Process_Charger_Check_Done
                    G_Module_Function_Status &= ~Process_Charger_Measured_Done;
                    G_Module_Function_Status &= ~Set_Charger_36V_Measured_Processing;
                }else{
                    // process start
                    Processing_Charger_Check(Measured_36V_Chger, Switch_To_36V_Ch);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing For 48V Charger
            }else if((G_Module_Function_Status & Set_Charger_48V_Measured_Processing)&&((G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)==0)){
                if(G_Module_Function_Status & Process_Charger_Measured_Done){
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_48V_Voltage_Auto,(t_uint8 *)(&G_Temp_ADC_L0), 12);
                    //Process_Charger_Check_Done
                    G_Module_Function_Status &= ~Process_Charger_Measured_Done;
                    G_Module_Function_Status &= ~Set_Charger_48V_Measured_Processing;
                }else{
                    // process start
                    Processing_Charger_Check(Measured_48V_Chger, Switch_To_48V_Ch);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing For Pack Voltage Via DSG Port
            }else if((G_Module_Function_Status & Set_Pack_DSG_Vol_Measured_Processing)&&((G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)==0)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    _DUI_SetPackDSGInputPortForMeasurement(Turn_On);
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Pack_Dsg_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Pack_DSG_Vol);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Pack_DSG_Vol, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~Set_Pack_DSG_Vol_Measured_Processing;
                    _DUI_SetPackDSGInputPortForMeasurement(Turn_Off);
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_PACK_DSG_Voltage_Auto,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing For Pack Voltage Via CHG Port
            }else if((G_Module_Function_Status & Set_Pack_CHG_Vol_Measured_Processing)&&((G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)==0)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    _DUI_SetPackCHGInputPortForMeasurement(Turn_On);
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Pack_Chg_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Pack_CHG_Vol);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Pack_CHG_Vol, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~Set_Pack_CHG_Vol_Measured_Processing;
                    _DUI_SetPackCHGInputPortForMeasurement(Turn_Off);
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_PACK_CHG_Voltage_Auto,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing For Directly ADC channel
            }else if(G_Module_Function_Status & Set_Channels_ADC_Measured_Processing){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh((MeasuredADCChannels)gCdcTempUint8);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Raw_ADC_SingleChannle_Result();
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~Set_Channels_ADC_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Channel_Raw_ADC,(t_uint8 *)(&G_Temp_ADC_L0), 2);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing without setting devices For Pack Voltage Via DSG Port
            }else if((G_Module_Function_Status & Set_Pack_DSG_Vol_Measured_Processing)&&(G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Pack_Dsg_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Pack_DSG_Vol);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Pack_DSG_Vol, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status &= ~Set_Pack_DSG_Vol_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_PackDSG_Voltage,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing without setting devices For Pack Voltage Via CHG Port
            }else if((G_Module_Function_Status & Set_Pack_CHG_Vol_Measured_Processing)&&(G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Pack_Chg_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Pack_CHG_Vol);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Pack_CHG_Vol, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status &= ~Set_Pack_CHG_Vol_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_PackCHG_Voltage,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing without setting devices For Charger_24V_Measured_Processing
            }else if((G_Module_Function_Status & Set_Charger_24V_Measured_Processing)&&(G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Chger_Vol_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_24V_Chger);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_24V_Chger, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status &= ~Set_Charger_24V_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_Chger_24Voltage,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing without setting devices For Charger_36V_Measured_Processing
            }else if((G_Module_Function_Status & Set_Charger_36V_Measured_Processing)&&(G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Chger_Vol_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_36V_Chger);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_36V_Chger, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status &= ~Set_Charger_36V_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_Chger_36Voltage,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing without setting devices For Charger_48V_Measured_Processing
            }else if((G_Module_Function_Status & Set_Charger_48V_Measured_Processing)&&(G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Chger_Vol_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_48V_Chger);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_48V_Chger, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status &= ~Set_Charger_48V_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_Chger_48Voltage,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing without setting devices For Charging Current_Measured_Processing
            }else if((G_1st_Module_Function_Status & Set_Chg_Current_Measured_Processing)&&(G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_CHG_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Charging_Current);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Charging_Current, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_1st_Module_Function_Status &= ~Set_Chg_Current_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_CHG_Current,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }
            /////////////////////////////////////////////////////////////////////
            // start Measured Processing without setting devices For Discharging Current_Measured_Processing
            }else if((G_1st_Module_Function_Status & Set_Dsg_Current_Measured_Processing)&&(G_Module_Function_Status & DirMeasuredProcessingViaADC_Ch)){
                if(!(G_Module_Function_Status & Process_Set_Channel)){
                    //exec once
                    G_Module_Function_Status |= Process_Set_Channel;
                    Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
                    _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_DSG_ch);
                }
                if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
                    _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
                }else if(G_Module_Function_Status & ADC_Done_Conversion){
                    //calculate and save results
                    G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Discharging_Current);
                    G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Discharging_Current, G_Temp_ADC_L0);
                    //clear ADC Flags
                    G_Module_Function_Status &= ~ADC_Done_Conversion;
                    G_Module_Function_Status &= ~ADC_Start_Conversion;
                    //Process_Vol_Measured_Done
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_1st_Module_Function_Status &= ~Set_Dsg_Current_Measured_Processing;
                    //send data out via usb
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_DSG_Current,(t_uint8 *)(&G_Temp_ADC_L0), 4);
                }



            }else{


                //reset flag
#if !defined(_Debug_Disable_USB_Function_)
            __bis_SR_register(LPM0_bits);
            _DUI_USB_Main_Polling_Function_For_Parsing_Receiving_Packet();
#endif
                //G_Module_Function_Status &= ~(Charger_ID_Level_1_Check + Charger_ID_Level_2_Check + Charger_ID_Level_3_Check);
                //G_Module_Function_Status &= ~(Charger_24V_Check + Charger_36V_Check + Charger_48V_Check);
                //G_Module_Function_Status &= ~Charger_Check_Actived;
            }




#if 0
    //float t_float;
    if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
        //GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN6 );
        GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5 );
        _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(Measured_ADC_Channel1);
        _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
        //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
    }else if(G_Module_Function_Status & ADC_Done_Conversion){
        //GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN4 );
//        G_Charger_48V_Channel_ADC = _DUI_Channle_ADC_Result(ADC_48V_Channel);
//        G_Charger_36V_Channel_ADC = _DUI_Channle_ADC_Result(ADC_36V_Channel);
//        G_Charger_24V_Channel_ADC = _DUI_Channle_ADC_Result(ADC_24V_Channel);
//        G_BatteryV_Channel_ADC = _DUI_Channle_ADC_Result(ADC_BatteryV_Channel);


//        t_float = FA_24V_mV_To_ADC_Factor;
//        t_float = G_Charger_24V_Channel_ADC/t_float;
//        g_Voltage_Result_data16[2] = (unsigned int)t_float;
//
//        t_float = FA_36V_mV_To_ADC_Factor;
//        t_float = G_Charger_36V_Channel_ADC/t_float;
//        g_Voltage_Result_data16[1] = (unsigned int)t_float;
//
//        t_float = FA_48V_mV_To_ADC_Factor;
//        t_float = G_Charger_48V_Channel_ADC/t_float;
//        g_Voltage_Result_data16[0] = (unsigned int)t_float;


        G_Module_Function_Status &= ~ADC_Done_Conversion;
        G_Module_Function_Status &= ~ADC_Start_Conversion;

        GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
        //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN6 );
        //GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN4 );
    }
#endif

    }//while(1){


}                               //main()
void Clear_Temp_Array_Buffer(unsigned int* ptr_StartArray, unsigned char num){
    unsigned char index;

    for(index = 0; index < num; index++){
        *(ptr_StartArray + index) = 0;
    }
}

void Processing_Charger_Check(MeasuredFunctions Measured_Voltage_Chger, Switch_Channnel Switch_To_Voltage_Ch){

    if(!(G_Module_Function_Status & Process_Set_Channel)){
        //exec once
        G_Module_Function_Status |= Process_Set_Channel;
        Clear_Temp_Array_Buffer(&G_Temp_ADC_L0, 8);
        _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(ADC_Chger_Vol_ch);
        _DUI_SwitchChargerInputAndIDStep(Switch_To_Voltage_Ch, Chger_ID_OFF);
    }
    //process charger ID Off
    if((G_Module_Function_Status & (Charger_ID_Level_1_Check + Charger_ID_Level_2_Check)) == 0){
        if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
            _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
        }else if(G_Module_Function_Status & ADC_Done_Conversion){
            //switch to next step, Level 1
            _DUI_SwitchChargerInputAndIDStep(Switch_To_Voltage_Ch, Chger_ID_1st_Step);
            DelayCycles(G_Temp_1_DelayCycles);
            G_Module_Function_Status |= Charger_ID_Level_1_Check;
            //calculate and save results
            G_Temp_ADC_L0 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Voltage_Chger);
            G_Temp_RealValue_From_ADC_L0 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Voltage_Chger, G_Temp_ADC_L0);
//            G_Temp_ADC_L0 = 0x1000 + Measured_Voltage_Chger;
//            G_Temp_RealValue_From_ADC_L0 = 0x1100 + Measured_Voltage_Chger;
            //clear ADC Flags
            G_Module_Function_Status &= ~ADC_Done_Conversion;
            G_Module_Function_Status &= ~ADC_Start_Conversion;
        }
    }
    //process charger ID Level 1
    if(G_Module_Function_Status & Charger_ID_Level_1_Check){
        if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
            _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
        }else if(G_Module_Function_Status & ADC_Done_Conversion){
            //switch to next step, Level 2
            _DUI_SwitchChargerInputAndIDStep(Switch_To_Voltage_Ch, Chger_ID_2nd_Step);
            DelayCycles(G_Temp_1_DelayCycles);
            G_Module_Function_Status &= ~(Charger_ID_Level_1_Check + Charger_ID_Level_2_Check);
            G_Module_Function_Status |= Charger_ID_Level_2_Check;
            //calculate and save results
            G_Temp_ADC_L1 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Voltage_Chger);
            G_Temp_RealValue_From_ADC_L1 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Voltage_Chger, G_Temp_ADC_L1);
//            G_Temp_ADC_L1 = 0x2000 + Measured_Voltage_Chger;
//            G_Temp_RealValue_From_ADC_L1 = 0x2200 + Measured_Voltage_Chger;
            //clear ADC Flags
            G_Module_Function_Status &= ~ADC_Done_Conversion;
            G_Module_Function_Status &= ~ADC_Start_Conversion;
        }
    }
//    //process charger ID Level 2
//    if(G_Module_Function_Status & Charger_ID_Level_2_Check){
//        if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
//            _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
//        }else if(G_Module_Function_Status & ADC_Done_Conversion){
//            //switch to next step, Level 3
//            _DUI_SwitchChargerInputAndIDStep(Switch_To_Voltage_Ch, Chger_ID_3rd_Step);
//            G_Module_Function_Status &= ~(Charger_ID_Level_1_Check + Charger_ID_Level_2_Check + Charger_ID_Level_3_Check);
//            G_Module_Function_Status |= Charger_ID_Level_3_Check;
//            //calculate and save results
//            G_Temp_ADC_L2 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Voltage_Chger);
//            G_Temp_RealValue_From_ADC_L2 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Voltage_Chger, G_Temp_ADC_L2);
////            G_Temp_ADC_L2 = 0x3000 + Measured_Voltage_Chger;
////            G_Temp_RealValue_From_ADC_L2 = 0x3300 + Measured_Voltage_Chger;
//            //clear ADC Flags
//            G_Module_Function_Status &= ~ADC_Done_Conversion;
//            G_Module_Function_Status &= ~ADC_Start_Conversion;
//        }
//    }
    //process charger ID Level 2
    if(G_Module_Function_Status & Charger_ID_Level_2_Check){
        if(((G_Module_Function_Status & ADC_Start_Conversion)==0) && ((G_Module_Function_Status & ADC_Done_Conversion)==0)){
            _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();
        }else if(G_Module_Function_Status & ADC_Done_Conversion){
            //Final step, set id off, clear flag
            _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
            G_Module_Function_Status &= ~(Charger_ID_Level_1_Check + Charger_ID_Level_2_Check);
            G_Module_Function_Status &= ~Process_Set_Channel;
            G_Module_Function_Status |= Process_Charger_Measured_Done;
            //calculate and save results
            G_Temp_ADC_L2 = _DUI_Get_Calibrated_ADC_SingleChannle_Result(Measured_Voltage_Chger);
            G_Temp_RealValue_From_ADC_L2 = _DUI_Get_RealMeasuredDate_By_ADC(Measured_Voltage_Chger, G_Temp_ADC_L2);
//            G_Temp_ADC_L3 = 0x4000 + Measured_Voltage_Chger;
//            G_Temp_RealValue_From_ADC_L3 = 0x4400 + Measured_Voltage_Chger;
            //clear ADC Flags
            G_Module_Function_Status &= ~ADC_Done_Conversion;
            G_Module_Function_Status &= ~ADC_Start_Conversion;
        }
    }

}//void Processing_Charger_Check(MeasuredFunctions Measured_Voltage_Chger, Switch_Channnel Switch_To_Voltage_Ch){




/*
 * ======== Init_Clock ========
 */
//VOID Init_Clock (VOID)
//{
//    //Initialization of clock module
//    if (USB_PLL_XT == 2){
//		#if defined (__MSP430F552x) || defined (__MSP430F550x)
//			P5SEL |= 0x0C;                                      //enable XT2 pins for F5529
//		#elif defined (__MSP430F563x_F663x)
//			P7SEL |= 0x0C;
//		#endif
//
//        //use REFO for FLL and ACLK
//        UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
//        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);
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
//}
//


/*
 * ======== UNMI_ISR ========
 */
#pragma vector = UNMI_VECTOR
__interrupt VOID UNMI_ISR (VOID)
{
    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG ))
    {
        case SYSUNIV_NONE:
            __no_operation();
            break;
        case SYSUNIV_NMIIFG:
            __no_operation();
            break;
        case SYSUNIV_OFIFG:
            UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG); //Clear OSC flaut Flags fault flags
            SFRIFG1 &= ~OFIFG;                          //Clear OFIFG fault flag
            break;
        case SYSUNIV_ACCVIFG:
            __no_operation();
            break;
        case SYSUNIV_BUSIFG:
            SYSBERRIV = 0;                                      //clear bus error flag
            USB_disable();                                      //Disable
    }
}

