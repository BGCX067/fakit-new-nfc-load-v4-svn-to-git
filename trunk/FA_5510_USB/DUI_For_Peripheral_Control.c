/**
  ******************************************************************************
  * @file    Driver_User_Interface for Peripheral control.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    2-November-2012
  * @brief   for user to calling and control Devices
  ******************************************************************************
  * @attention
  *
  * DESCRIPTION....
  *
  * <h2><center>&copy; COPYRIGHT 2012 Dynapack</center></h2>
  */

//==============================================================================
// Includes
//==============================================================================
//#include <intrinsics.h>
//#include <string.h>
//
//#include "USB_config/descriptors.h"
//
//#include "USB_API/USB_Common/device.h"
//#include "USB_API/USB_Common/types.h"               //Basic Type declarations
//#include "USB_API/USB_Common/usb.h"                 //USB-specific functions
//
//#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"
//#include "F5xx_F6xx_Core_Lib/HAL_PMM.h"
//
//#include "USB_API/USB_CDC_API/UsbCdc.h"
////#include "usbConstructs.h"
//
//#include "gpio.h"

#include "MCU_Devices/MCU_Devices.h"
#include "Global_Vars_Define.h"
#include "Vars_Bit_Define.h"

#include "Utilities/Utilities.h"
#include "DUI_For_Peripheral_Control.h"
#include "SystemConfigDefineForFlash.h"

#include "DUI_For_USB_CDC.h"
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
//#define Auto_Check_Charger_result_Size  12

// Vcharger turn on , at least 2.65s than can be turn off
//#define Charger_ID_Turn_On_Delay_Cycle_Times    30   //3000ms, Cycle Times * Timer interval time
//#define Faster_Charger_ID_Turn_On_Delay_Cycle_Times    3   //300ms, Cycle Times * Timer interval time
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
//t_uint16 g_Auto_Check_Charger_result[Auto_Check_Charger_result_Size];
t_uint8 Charger_ID_Turn_On_Delay_Cycle_counter;
//==============================================================================
// Private functions
//==============================================================================

//==============================================================================
// Public functions
//==============================================================================
/////////////////////////////////////////////////////////////////////
// Power_Management_Module Config
/////////////////////////////////////////////////////////////////////
void _DUI_Init_Power_Management_Module(){
    _Device_Set_Power_Management_Module();
}

/////////////////////////////////////////////////////////////////////
// Clock Config
/////////////////////////////////////////////////////////////////////
void _DUI_Init_Clock_Module(){
    _Device_Init_Clock_Module();
}

/////////////////////////////////////////////////////////////////////
// Polling Timer config
/////////////////////////////////////////////////////////////////////
void (*polling_fun_array[Max_TimerA_INTERRUPT_Function_Calling])(void);
static void empty_polling_fun(void){;}
void _DUI_Init_Polling_Timer(){
    t_uint8 i;
    for(i = 0 ; i < Max_Polling_Function_Num; i++){
        polling_fun_array[i] = empty_polling_fun;
    }
    _Device_Init_Timer_A();
}
void _DUI_Start_Polling_Timer(){
    _Device_Enable_Timer_A();
}
void _DUI_Stop_Polling_Timer(){
    _Device_Disable_Timer_A();
}

void _DUI_Set_Function_To_Polling(void (*polling_fun)()){
    t_uint8 i;
    for(i = 0 ; i < Max_Polling_Function_Num; i++){
        if(polling_fun_array[i] == empty_polling_fun){
            polling_fun_array[i] = polling_fun;
            _Device_Set_TimerA_Interrupt_Timer_Calling_Function(i, polling_fun);
            return;
        }
    }
}
void _DUI_Remove_Function_Out_Of_Polling(void (*polling_fun)()){
    t_uint8 i;
    for(i = 0 ; i < Max_Polling_Function_Num; i++){
        if(polling_fun_array[i] == polling_fun){
            polling_fun_array[i] = empty_polling_fun;
            _Device_Remove_TimerA_Interrupt_Timer_Calling_Function(i);
            return;
        }
    }
}

/////////////////////////////////////////////////////////////////////
// Communication packet Form detection Timer config
/////////////////////////////////////////////////////////////////////

void _DUI_Init_Comm_Packet_Form_Detection_Timer(){
    _Device_Init_Timer_B();
}

void _DUI_Set_Calling_Function_While_Form_Detection(t_uint8 fun_index, void (*calling_fun)(), t_uint16 ms_Dealy ){
    _Device_Set_TimerB_Interrupt_Timer_Calling_Function_With_Delay_And_Exec(fun_index, calling_fun, ms_Dealy);
}
void _DUI_Remove_Detection_Calling_Function(t_uint8 fun_index){
    _Device_Remove_TimerB_Interrupt_Timer_Calling_Function(fun_index);
}

/////////////////////////////////////////////////////////////////////
// I/O Ports config
/////////////////////////////////////////////////////////////////////
void _DUI_Init_IO_Ports(){
    _Device_Init_Ports_To_Output_Low();
    //Port2_Pin5_6_Init();
}

/////////////////////////////////////////////////////////////////////
// Mux switch Rs485, Uart config
/////////////////////////////////////////////////////////////////////

void _DUI_Commun_MUX_Init(){
    _Device_Commun_MUX_Init();
    _DUI_Switch_To_Communication_Port(UART_Channel);
}

void _DUI_Switch_To_Communication_Port(Commun_Peripheral_ch channel){

    switch(channel){
        case RS485_Channel:
            _Device_Set_Commun_Mux_Channel(Comm_RS485_Mux_Ch);
            break;
        case UART_Channel:
        default:
            _Device_Set_Commun_Mux_Channel(Comm_UART_Mux_Ch);
            break;
    }
}

/////////////////////////////////////////////////////////////////////
// Charger Function Control
/////////////////////////////////////////////////////////////////////
void _DUI_Charger_Function_Init(){
    _Device_Charger_Func_Init();
    //_Device_Set_Chg_ID_Level(CHG_ID_Level_All_Lo);
    _Device_Chger_Mux_Channel_Enable(Device_On);
    //_Device_Set_Chger_Mux_Channel(Chger_Mux_Ch4);
    //_Device_Set_Chger_Input_Voltage_Channel(Chger_InputVol_Ch_All_Lo);

    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
}
void _DUI_SwitchChargerInputAndIDStep(Switch_Channnel ch, Chger_ID_Steps idStep){
    //reset ID level control
    _Device_Set_Chg_ID_Level(CHG_ID_Level_All_Lo);
    switch(idStep){
        case Chger_ID_1st_Step:
            _Device_Set_Chg_ID_Level(CHG_ID_Level1_Hi);
            _Device_Set_Chg_ID_Level(CHG_ID_Level2_Hi);
            break;
        case Chger_ID_2nd_Step:
            _Device_Set_Chg_ID_Level(CHG_ID_Level1_Hi);
            //_Device_Set_Chg_ID_Level(CHG_ID_Level2_Hi);
            break;
        case Chger_ID_OFF:
        default:
            _Device_Set_Chg_ID_Level(CHG_ID_Level_All_Lo);
            break;
    }
    //reset Chger_Input_Voltage_Channel
    _Device_Set_Chger_Input_Voltage_Channel(Chger_InputVol_Ch_All_Lo);
    switch(ch){
        case Switch_To_24V_Ch:
            _Device_Set_Chger_Mux_Channel(Chger_Mux_Ch1);
            _Device_Set_Chger_Input_Voltage_Channel(Chger_24Vol_Ch_Hi);
            break;
        case Switch_To_36V_Ch:
            _Device_Set_Chger_Mux_Channel(Chger_Mux_Ch2);
            _Device_Set_Chger_Input_Voltage_Channel(Chger_36Vol_Ch_Hi);
            break;
        case Switch_To_48V_Ch:
            _Device_Set_Chger_Mux_Channel(Chger_Mux_Ch3);
            _Device_Set_Chger_Input_Voltage_Channel(Chger_48Vol_Ch_Hi);
            break;
        case Switch_To_Empty_Ch:
        default:
            _Device_Set_Chger_Input_Voltage_Channel(Chger_InputVol_Ch_All_Lo);
            //__delay_cycles(100);    //12us at 8MHz
            _Device_Set_Chger_Mux_Channel(Chger_Mux_Ch1);
            break;
    }
}
Chger_Status _DUI_Get_Charger_ID_Status(){
    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
    if(_Device_Get_Chg_ID_Sense_Status() == USE_CHG_ID){
        return Chger_With_ID_Step;
    }else{
        return Chger_Without_ID_Step;
    }
}


/////////////////////////////////////////////////////////////////////
// System Function control
/////////////////////////////////////////////////////////////////////
void _DUI_System_Function_Ctrl_Init(){
    _Device_System_Func_Ctrl_Init();
    _DUI_SetPackDSGInputPortForMeasurement(Turn_On);
    _DUI_SetPackCHGInputPortForMeasurement(Turn_On);
    //Init LOADING_GATE (set Hi level)(which is for Loading through PACK DSG )
    // turn on == Low
    // turn off == High
    _DUI_SetKitLoading(Turn_Off);
}
void _DUI_SetPackDSGInputPortForMeasurement(Device_Switch status){
    if(status == Turn_On){
        _Device_Set_VPD_GATE(Device_On);
    }else{
        _Device_Set_VPD_GATE(Device_Off);
    }
}
void _DUI_SetPackCHGInputPortForMeasurement(Device_Switch status){
    if(status == Turn_On){
        _Device_Set_VPC_GATE(Device_On);
    }else{
        _Device_Set_VPC_GATE(Device_Off);
    }
}
void _DUI_SetKitLoading(Device_Switch status){
    if(status == Turn_On){
        _Device_Set_LOADING_GATE(Device_On);
    }else{
        _Device_Set_LOADING_GATE(Device_Off);
    }
}
void _DUI_SetChargingViaDSGPort(Device_Switch status){
    if(status == Turn_On){
        _Device_Set_CHGING_VIA_PACK_D_GATE(Device_On);
    }else{
        _Device_Set_CHGING_VIA_PACK_D_GATE(Device_Off);
    }
}
/////////////////////////////////////////////////////////////////////
// ADC Function Control
/////////////////////////////////////////////////////////////////////
void _ADC_Done_conversion(){
    G_Module_Function_Status |= ADC_Done_Conversion;
}


void _DUI_ADC_Function_Init_for_SequenceSampling(){
    _Device_Measured_Sequence_ADC_Init();
    _DUI_Set_Function_For_ADC_Conversion_Done_Calling(_ADC_Done_conversion);
    G_Module_Function_Status &= ~ADC_Start_Conversion;
    G_Module_Function_Status &= ~ADC_Done_Conversion;
}

void _DUI_Start_ADC_Conversion_for_SequenceSampling(){
    G_Module_Function_Status |= ADC_Start_Conversion;
    G_Module_Function_Status &= ~ADC_Done_Conversion;
    _Device_Measured_Sequence_ADC_Conversion_Start();
}


void _DUI_ADC_Function_Init_for_RepeatedSingleCh(){
    _Device_Measured_RepeatedSingle_ADC_Init();
    _DUI_Set_Function_For_ADC_Conversion_Done_Calling(_ADC_Done_conversion);
    G_Module_Function_Status &= ~ADC_Start_Conversion;
    G_Module_Function_Status &= ~ADC_Done_Conversion;
}
void _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(MeasuredADCChannels channel){
    _Device_Set_Measured_RepeatedSingle_ADC_Chasnnel((MeasuredSingleADCChannels)channel);
}

void _DUI_Start_ADC_Conversion_for_RepeatedSingleCh(){
    G_Module_Function_Status |= ADC_Start_Conversion;
    G_Module_Function_Status &= ~ADC_Done_Conversion;
    _Device_Measured_RepeatedSingle_ADC_Conversion_Start();
}


void _DUI_Set_Function_For_ADC_Conversion_Done_Calling(void (*calling_fun)()){
    _Device_Set_Interrupt_For_ADC_Conversion_Done_Calling_Function(calling_fun);
}
void _DUI_Remove_ADC_Conversion_Done_Function(){
    _Device_Remove_ADC_Conversion_Done_Timer_Calling_Function();
}


t_uint16 _DUI_Get_Raw_ADC_SingleChannle_Result(){
    return _Device_Get_RepeatedSingle_ADC_Result();
}

t_uint16 _DUI_Get_Calibrated_ADC_SingleChannle_Result(MeasuredFunctions mf){
    t_int16 temp;
    t_int8 temp_c;
    temp = _DUI_Get_Raw_ADC_SingleChannle_Result();
    switch(mf){
        case Measured_24V_Chger:
            temp_c = FA_24V_CAL_OFFSET_ADC;
            temp = (temp -  (t_int16)temp_c);
            break;
        case Measured_36V_Chger:
            temp_c = FA_36V_CAL_OFFSET_ADC;
            temp = (temp -  (t_int16)temp_c);
            break;
        case Measured_48V_Chger:
            temp_c = FA_48V_CAL_OFFSET_ADC;
            temp = (temp -  (t_int16)temp_c);
            break;
        case Measured_Pack_DSG_Vol:
            temp_c = FA_Pack_DSG_CAL_OFFSET_ADC;
            temp = (temp -  (t_int16)temp_c);
            break;
        case Measured_Pack_CHG_Vol:
            temp_c = FA_Pack_CHG_CAL_OFFSET_ADC;
            temp = (temp -  (t_int16)temp_c);
            break;
        case Measured_Charging_Current:
            temp_c = FA_CHG_Current_CAL_OFFSET_ADC;
            temp = (temp -  (t_int16)temp_c);
            break;
        case Measured_Discharging_Current:
            temp_c = FA_DSG_Current_CAL_OFFSET_ADC;
            temp = (temp -  (t_int16)temp_c);
            break;
        default:
            temp = 0;
            break;
    }
    if(temp < 0 ){
        temp = 0;
    }
    return temp;
}
t_uint16 _DUI_Get_RealMeasuredDate_By_ADC(MeasuredFunctions mf, t_uint16 Adc){
    float temp_float;

    switch(mf){
        case Measured_24V_Chger:
            //24V
            temp_float = FA_24V_mV_To_ADC_Factor;
            temp_float = (Adc/temp_float) + 0.5f; //round off 四捨五入
            break;
        case Measured_36V_Chger:
            //36V
            temp_float = FA_36V_mV_To_ADC_Factor;
            temp_float = (Adc/temp_float) + 0.5f; //round off 四捨五入
            break;
        case Measured_48V_Chger:
            //48V
            temp_float = FA_48V_mV_To_ADC_Factor;
            temp_float = (Adc/temp_float) + 0.5f; //round off 四捨五入
            break;
        case Measured_Pack_DSG_Vol:
            //Pack_DSG
            temp_float = FA_Pack_DSG_mV_To_ADC_Factor;
            temp_float = (Adc/temp_float) + 0.5f; //round off 四捨五入
            break;
        case Measured_Pack_CHG_Vol:
            //Pack_CHG
            temp_float = FA_Pack_CHG_mV_To_ADC_Factor;
            temp_float = (Adc/temp_float) + 0.5f; //round off 四捨五入
            break;
        case Measured_Charging_Current:
            temp_float = FA_CHG_Current_mA_To_ADC_Factor;
            temp_float = (Adc/temp_float) + 0.5f; //round off 四捨五入
            break;
        case Measured_Discharging_Current:
            temp_float = FA_DSG_Current_mA_To_ADC_Factor;
            temp_float = (Adc/temp_float) + 0.5f; //round off 四捨五入
            break;
        default:
            temp_float = 0.0f;
            break;
    }
    return (unsigned int)temp_float;
}
//t_uint16 _DUI_Get_Voltage_By_ADC(t_uint8 ADCchannel, t_uint16 Vadc){
//
//}

//#if 0
//
//t_uint16 _DUI_Channle_ADC_Result(t_uint8 ADCchannel){
//    t_int16 temp;
//    t_int8 temp_c;
//    switch(ADCchannel){
//        case ADC_24V_Channel:
//            temp_c = FA_24V_CAL_OFFSET_ADC;
//            temp = (t_int16)(_Device_Get_ADC_Result(ADC_Channel2));
//            temp = (temp -  (t_int16)temp_c);
//            break;
//        case ADC_36V_Channel:
//            temp_c = FA_36V_CAL_OFFSET_ADC;
//            temp = (t_int16)(_Device_Get_ADC_Result(ADC_Channel1));
//            temp = (temp -  (t_int16)temp_c);
//            break;
//        case ADC_48V_Channel:
//            temp_c = FA_48V_CAL_OFFSET_ADC;
//            temp = (t_int16)(_Device_Get_ADC_Result(ADC_Channel0));
//            temp = (temp -  (t_int16)temp_c);
//            break;
//        case ADC_BatteryV_Channel:
//            temp_c = FA_Battery_CAL_OFFSET_ADC;
//            temp = (t_int16)(_Device_Get_ADC_Result(ADC_Channel4));
//            temp = (temp -  (t_int16)temp_c);
//            break;
//        default:
//            temp = 0;
//            break;
//    }
//    if(temp < 0 ){
//        temp = 0;
//    }
//    return (t_uint16)temp;
//}
//t_uint16 _DUI_Get_Voltage_By_ADC(t_uint8 ADCchannel, t_uint16 Vadc){
//    t_uint16 g_temp_uint16;
//    float g_temp_float;
//    g_temp_uint16 = Vadc;
//    switch(ADCchannel){
//        case ADC_24V_Channel:
//            //24V
//            g_temp_float = FA_24V_mV_To_ADC_Factor;
//            g_temp_float = g_temp_uint16/g_temp_float;
//            g_temp_uint16 = (unsigned int)g_temp_float;
//            return g_temp_uint16;
//            break;
//        case ADC_36V_Channel:
//            //36V
//            g_temp_float = FA_36V_mV_To_ADC_Factor;
//            g_temp_float = g_temp_uint16/g_temp_float;
//            g_temp_uint16 = (unsigned int)g_temp_float;
//            return g_temp_uint16;
//            break;
//        case ADC_48V_Channel:
//            //48V
//            g_temp_float = FA_48V_mV_To_ADC_Factor;
//            g_temp_float = g_temp_uint16/g_temp_float;
//            g_temp_uint16 = (unsigned int)g_temp_float;
//            return g_temp_uint16;
//            break;
//        default:
//            return 0;
//            break;
//
//    }
//}
//
//void start_Charger_Checking(){
//    //step 5
//    if(G_Module_Function_Status & Charger_Check_48V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Normal_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[10] = G_Charger_48V_Channel_ADC;
//            g_Auto_Check_Charger_result[11] = _DUI_Get_Voltage_By_ADC(ADC_48V_Channel, G_Charger_48V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_OFF_Channel);
//            G_Module_Function_Status &= ~Charger_Check_48V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_Done;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 4
//    if(G_Module_Function_Status & Charger_Check_36V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Normal_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[8] = G_Charger_36V_Channel_ADC;
//            g_Auto_Check_Charger_result[9] = _DUI_Get_Voltage_By_ADC(ADC_36V_Channel, G_Charger_36V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_48V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_36V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_48V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 3
//    if(G_Module_Function_Status & Charger_Check_24V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Normal_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[6] = G_Charger_24V_Channel_ADC;
//            g_Auto_Check_Charger_result[7] = _DUI_Get_Voltage_By_ADC(ADC_24V_Channel, G_Charger_24V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_36V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_24V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_36V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 2
//    if(G_Module_Function_Status & Charger_Check_OFF_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Normal_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[0] = G_Charger_24V_Channel_ADC;
//            g_Auto_Check_Charger_result[1] = _DUI_Get_Voltage_By_ADC(ADC_24V_Channel, G_Charger_24V_Channel_ADC);
//            g_Auto_Check_Charger_result[2] = G_Charger_36V_Channel_ADC;
//            g_Auto_Check_Charger_result[3] = _DUI_Get_Voltage_By_ADC(ADC_36V_Channel, G_Charger_36V_Channel_ADC);
//            g_Auto_Check_Charger_result[4] = G_Charger_48V_Channel_ADC;
//            g_Auto_Check_Charger_result[5] = _DUI_Get_Voltage_By_ADC(ADC_48V_Channel, G_Charger_48V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_24V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_OFF_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_24V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 1 (Start_Charger_Check)
//    if(G_Module_Function_Status & Charger_Check_Actived){
//        //G_Module_Function_Status |= Charger_Check_Actived;
//        _DUI_Set_Charger_Channel(CHG_ON_Channel);
//        _DUI_Set_Charger_ID(CHG_OFF_Channel);
//        G_Module_Function_Status &= ~Charger_Check_Actived;
//        G_Module_Function_Status |= Charger_Check_OFF_ID_Ch;
//        Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//    }
//
//    //step 6
//    //last Done
//    if(G_Module_Function_Status & Charger_Check_Done){
//        //
//        _DUI_Set_Charger_ID(CHG_OFF_Channel);
//        _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(0xA6,(t_uint8 *)g_Auto_Check_Charger_result, Auto_Check_Charger_result_Size*2);
//        G_Module_Function_Status &= ~Charger_Check_Done;
//        G_Module_Function_Status &= ~Start_Charger_Check;
//        Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//    }
//}
//
//void start_Fast_Charger_Checking(){
//    //step 5
//    if(G_Module_Function_Status & Charger_Check_48V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Faster_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[10] = G_Charger_48V_Channel_ADC;
//            g_Auto_Check_Charger_result[11] = _DUI_Get_Voltage_By_ADC(ADC_48V_Channel, G_Charger_48V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_OFF_Channel);
//            G_Module_Function_Status &= ~Charger_Check_48V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_Done;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 4
//    if(G_Module_Function_Status & Charger_Check_36V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Faster_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[8] = G_Charger_36V_Channel_ADC;
//            g_Auto_Check_Charger_result[9] = _DUI_Get_Voltage_By_ADC(ADC_36V_Channel, G_Charger_36V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_48V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_36V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_48V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 3
//    if(G_Module_Function_Status & Charger_Check_24V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Faster_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[6] = G_Charger_24V_Channel_ADC;
//            g_Auto_Check_Charger_result[7] = _DUI_Get_Voltage_By_ADC(ADC_24V_Channel, G_Charger_24V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_36V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_24V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_36V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 2
//    if(G_Module_Function_Status & Charger_Check_OFF_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Faster_Auto_Charger_Check_Delay_Cycle){
//            g_Auto_Check_Charger_result[0] = G_Charger_24V_Channel_ADC;
//            g_Auto_Check_Charger_result[1] = _DUI_Get_Voltage_By_ADC(ADC_24V_Channel, G_Charger_24V_Channel_ADC);
//            g_Auto_Check_Charger_result[2] = G_Charger_36V_Channel_ADC;
//            g_Auto_Check_Charger_result[3] = _DUI_Get_Voltage_By_ADC(ADC_36V_Channel, G_Charger_36V_Channel_ADC);
//            g_Auto_Check_Charger_result[4] = G_Charger_48V_Channel_ADC;
//            g_Auto_Check_Charger_result[5] = _DUI_Get_Voltage_By_ADC(ADC_48V_Channel, G_Charger_48V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_24V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_OFF_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_24V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 1 (Start_Fast_Charger_Check)
//    if(G_Module_Function_Status & Charger_Check_Actived){
//        //G_Module_Function_Status |= Charger_Check_Actived;
//        _DUI_Set_Charger_Channel(CHG_ON_Channel);
//        _DUI_Set_Charger_ID(CHG_OFF_Channel);
//        G_Module_Function_Status &= ~Charger_Check_Actived;
//        G_Module_Function_Status |= Charger_Check_OFF_ID_Ch;
//        Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//    }
//
//    //step 6
//    //last Done
//    if(G_Module_Function_Status & Charger_Check_Done){
//        //
//        _DUI_Set_Charger_ID(CHG_OFF_Channel);
//        _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(0xA7,(t_uint8 *)g_Auto_Check_Charger_result, Auto_Check_Charger_result_Size*2);
//        G_Module_Function_Status &= ~Charger_Check_Done;
//        G_Module_Function_Status &= ~Start_Fast_Charger_Check;
//        Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//    }
//}
//void _DUI_Auto_check_Charger_for_Pollong(){
//    //serial Steps for auto check charger
//    // ID->Hi to Vcharger->Hi ==> about 150ms
//    // ID->Lo to Vcharger->Lo ==> about 150ms
//    // Vcharger turn on , at least 2.65s than can be turn off
//    if(G_Module_Function_Status & Start_Charger_Check){
//        start_Charger_Checking();
//    }else if(G_Module_Function_Status & Start_Fast_Charger_Check){
//        start_Fast_Charger_Checking();
//    }
//}
//#endif
//void start_Charger_Checking_org_ok(){
//    //step 5
//    if(G_Module_Function_Status & Charger_Check_48V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Charger_ID_Turn_On_Delay_Cycle_Times){
//            g_Auto_Check_Charger_result[10] = G_Charger_48V_Channel_ADC;
//            g_Auto_Check_Charger_result[11] = _DUI_Get_Voltage_By_ADC(ADC_48V_Channel, G_Charger_48V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_OFF_Channel);
//            G_Module_Function_Status &= ~Charger_Check_48V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_Done;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 4
//    if(G_Module_Function_Status & Charger_Check_36V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Charger_ID_Turn_On_Delay_Cycle_Times){
//            g_Auto_Check_Charger_result[8] = G_Charger_36V_Channel_ADC;
//            g_Auto_Check_Charger_result[9] = _DUI_Get_Voltage_By_ADC(ADC_36V_Channel, G_Charger_36V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_48V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_36V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_48V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 3
//    if(G_Module_Function_Status & Charger_Check_24V_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Charger_ID_Turn_On_Delay_Cycle_Times){
//            g_Auto_Check_Charger_result[6] = G_Charger_24V_Channel_ADC;
//            g_Auto_Check_Charger_result[7] = _DUI_Get_Voltage_By_ADC(ADC_24V_Channel, G_Charger_24V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_36V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_24V_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_36V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 2
//    if(G_Module_Function_Status & Charger_Check_OFF_ID_Ch){
//        //after 100 ms
//        if(Charger_ID_Turn_On_Delay_Cycle_counter >= Charger_ID_Turn_On_Delay_Cycle_Times){
//            g_Auto_Check_Charger_result[0] = G_Charger_24V_Channel_ADC;
//            g_Auto_Check_Charger_result[1] = _DUI_Get_Voltage_By_ADC(ADC_24V_Channel, G_Charger_24V_Channel_ADC);
//            g_Auto_Check_Charger_result[2] = G_Charger_36V_Channel_ADC;
//            g_Auto_Check_Charger_result[3] = _DUI_Get_Voltage_By_ADC(ADC_36V_Channel, G_Charger_36V_Channel_ADC);
//            g_Auto_Check_Charger_result[4] = G_Charger_48V_Channel_ADC;
//            g_Auto_Check_Charger_result[5] = _DUI_Get_Voltage_By_ADC(ADC_48V_Channel, G_Charger_48V_Channel_ADC);
//            _DUI_Set_Charger_ID(CHG_24V_Channel);
//            G_Module_Function_Status &= ~Charger_Check_OFF_ID_Ch;
//            G_Module_Function_Status |= Charger_Check_24V_ID_Ch;
//            Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//        }else{
//            Charger_ID_Turn_On_Delay_Cycle_counter++;
//        }
//    }
//
//    //step 1
//    if(G_Module_Function_Status & Start_Charger_Check){
//        G_Module_Function_Status |= Charger_Check_Actived;
//        _DUI_Set_Charger_Channel(CHG_ON_Channel);
//        _DUI_Set_Charger_ID(CHG_OFF_Channel);
//        G_Module_Function_Status &= ~Start_Charger_Check;
//        G_Module_Function_Status |= Charger_Check_OFF_ID_Ch;
//        Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//    }
//
//    //step 6
//    //last Done
//    if(G_Module_Function_Status & Charger_Check_Done){
//        //
//        _DUI_Set_Charger_ID(CHG_OFF_Channel);
//        _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(0xA6,(t_uint8 *)g_Auto_Check_Charger_result, Auto_Check_Charger_result_Size*2);
//        G_Module_Function_Status &= ~Charger_Check_Done;
//        G_Module_Function_Status &= ~Charger_Check_Actived;
//        Charger_ID_Turn_On_Delay_Cycle_counter = 0;
//    }
//}


