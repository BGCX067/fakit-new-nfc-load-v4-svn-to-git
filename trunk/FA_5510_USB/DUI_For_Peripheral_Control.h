/**
  ******************************************************************************
  * @file    Driver_User_Interface for Peripheral control.h
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
//#include "MCU_Devices/MCU_Devices.h"

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
//==============================================================================
// Public functions
//==============================================================================
/////////////////////////////////////////////////////////////////////
// Power_Management_Module Config
/////////////////////////////////////////////////////////////////////
void _DUI_Init_Power_Management_Module();

/////////////////////////////////////////////////////////////////////
// Clock Config
/////////////////////////////////////////////////////////////////////
void _DUI_Init_Clock_Module();

/////////////////////////////////////////////////////////////////////
// Polling Timer config
/////////////////////////////////////////////////////////////////////
#define Max_Polling_Function_Num    Max_TimerA_INTERRUPT_Function_Calling

void _DUI_Init_Polling_Timer();
void _DUI_Start_Polling_Timer();
void _DUI_Stop_Polling_Timer();
void _DUI_Set_Function_To_Polling(void (*polling_fun)());
void _DUI_Remove_Function_Out_Of_Polling(void (*polling_fun)());

/////////////////////////////////////////////////////////////////////
// Communication packet Form detection Timer config
/////////////////////////////////////////////////////////////////////
#define Max_Calling_Function_Num_While_Detection    Max_TimerB_INTERRUPT_Function_Calling

void _DUI_Init_Comm_Packet_Form_Detection_Timer();
void _DUI_Set_Calling_Function_While_Form_Detection(t_uint8 fun_index, void (*calling_fun)(), t_uint16 ms_Dealy );
void _DUI_Remove_Detection_Calling_Function(t_uint8 fun_index);
/////////////////////////////////////////////////////////////////////
// I/O Ports config
/////////////////////////////////////////////////////////////////////
void _DUI_Init_IO_Ports();
/////////////////////////////////////////////////////////////////////
// Mux switch Rs485, Uart config
/////////////////////////////////////////////////////////////////////
typedef enum {
    RS485_Channel,  //for channel 1
    UART_Channel   //for channel 2
}Commun_Peripheral_ch;
void _DUI_Commun_MUX_Init();
void _DUI_Switch_To_Communication_Port(Commun_Peripheral_ch channel);
/////////////////////////////////////////////////////////////////////
// Charger Function Control
/////////////////////////////////////////////////////////////////////
typedef enum{
    Switch_To_Empty_Ch,
    Switch_To_24V_Ch,
    Switch_To_36V_Ch,
    Switch_To_48V_Ch
}Switch_Channnel;
typedef enum{
    Chger_ID_OFF,
    Chger_ID_1st_Step,
    Chger_ID_2nd_Step
}Chger_ID_Steps;
typedef enum{
    Chger_Without_ID_Step,
    Chger_With_ID_Step
}Chger_Status;
void _DUI_Charger_Function_Init();
//void _DUI_Set_Charger_ID(t_uint8 channel);
//void _DUI_Set_Charger_Channel(t_uint8 channel);
void _DUI_SwitchChargerInputAndIDStep(Switch_Channnel ch, Chger_ID_Steps idStep);
Chger_Status _DUI_Get_Charger_ID_Status();
/////////////////////////////////////////////////////////////////////
// System Function control
/////////////////////////////////////////////////////////////////////
typedef enum{
    Turn_Off,
    Turn_On
}Device_Switch;
void _DUI_System_Function_Ctrl_Init();
void _DUI_SetPackDSGInputPortForMeasurement(Device_Switch status);
void _DUI_SetPackCHGInputPortForMeasurement(Device_Switch status);
void _DUI_SetKitLoading(Device_Switch status);
void _DUI_SetChargingViaDSGPort(Device_Switch status);

/////////////////////////////////////////////////////////////////////
// ADC Function Control
/////////////////////////////////////////////////////////////////////
typedef enum{
    ADC_CHG_ch,    //Measured_ADC_Channel0,
    ADC_DSG_ch,    //Measured_ADC_Channel1,
    ADC_Pack_Dsg_ch,    //Measured_ADC_Channel2,
    ADC_Pack_Chg_ch,    //Measured_ADC_Channel3,
    ADC_Chger_Vol_ch,    //Measured_ADC_Channel4,
    Empty1,    //Measured_ADC_Channel5,
    Empty2,    //Measured_ADC_Channel6,
    Empty3    //Measured_ADC_Channel7
}MeasuredADCChannels;
typedef enum{
    Measured_Empty,
    Measured_24V_Chger,
    Measured_36V_Chger,
    Measured_48V_Chger,
    Measured_Pack_DSG_Vol,
    Measured_Pack_CHG_Vol,
    Measured_Charging_Current,
    Measured_Discharging_Current
}MeasuredFunctions;

void _DUI_ADC_Function_Init_for_SequenceSampling();
void _DUI_Start_ADC_Conversion_for_SequenceSampling();

void _DUI_ADC_Function_Init_for_RepeatedSingleCh();
void _DUI_Set_ADC_Conversion_Channel_for_RepeatedSingleCh(MeasuredADCChannels channel);
void _DUI_Start_ADC_Conversion_for_RepeatedSingleCh();


void _DUI_Set_Function_For_ADC_Conversion_Done_Calling(void (*calling_fun)());
void _DUI_Remove_ADC_Conversion_Done_Function();

//t_uint16 _DUI_Channle_ADC_Result(t_uint8 ADCchannel);
//t_uint16 _DUI_Get_Voltage_By_ADC(t_uint8 ADCchannel, t_uint16 Vadc);

t_uint16 _DUI_Get_Raw_ADC_SingleChannle_Result();
t_uint16 _DUI_Get_Calibrated_ADC_SingleChannle_Result(MeasuredFunctions mf);
t_uint16 _DUI_Get_RealMeasuredDate_By_ADC(MeasuredFunctions mf, t_uint16 Adc);

//void _DUI_Auto_check_Charger_for_Pollong();
