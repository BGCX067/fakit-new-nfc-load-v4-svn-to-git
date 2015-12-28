
/**
  ******************************************************************************
  * @file    Driver_User_Interface_For_USB_CDC.h
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    2-November-2012
  * @brief   Deiver_User_Interface Header
  ******************************************************************************
  * @attention
  *
  * DESCRIPTION....
  *
  * <h2><center>&copy; COPYRIGHT 2012 Dynapack</center></h2>
  */
//
//==============================================================================
// Includes
//==============================================================================

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
///////////////////////////////////////////////////////////////////
// usb cmd list
// new H/W setting cmd
#define Cmd_Set_DSG_Load_Gate           (0x70)
#define Cmd_Set_CHG_Shiftet_Gate        (0x71)
#define Cmd_Set_ADC_VPC_Gate            (0x72)
#define Cmd_Set_ADC_VPD_Gate            (0x73)


// H/W setting cmd
#define Cmd_Communi_Multiplex_Reset         (0x80)
#define Cmd_Communi_Multiplex_Set_Channel   (0x81)
#define Cmd_UART_Set_Baud_Rate          (0x82)
#define Cmd_UART_Set_Default_Baud_Rate  (0x83)
#define Cmd_UART_RS485_Enable           (0x84)
#define Cmd_UART_RS485_Disable          (0x85)
#define Cmd_One_Wire_Commu_Enable       (0x86)
#define Cmd_One_Wire_Commu_Disable      (0x87)
#define Cmd_I2C_Reset                   (0x88)  //no used on Ver 2.0
#define Cmd_I2C_Set_Address             (0x89)  //no used on Ver 2.0



// Data Status cmd
#define Cmd_I2C_Transmit_Data           (0x90)
#define Cmd_I2C_Receive_Data            (0x91)
#define Cmd_UART_RS485_Transmit_Data    (0x92)
#define Cmd_UART_RS485_Receive_Data     (0x93)
#define Cmd_One_Wire_Transmit_Data      (0x94)
#define Cmd_One_Wire_Receive_Data       (0x95)


//Charger Cmd
#define Cmd_Charger_24V_Channel_Set_ID      (0x8A)  //switch Mux, set input gate and set ID Levels
#define Cmd_Charger_36V_Channel_Set_ID      (0x8B)  //switch Mux, set input gate and set ID Levels
#define Cmd_Charger_48V_Channel_Set_ID      (0x8C)  //switch Mux, set input gate and set ID Levels

#define Cmd_Get_Charger_24V_Voltage_Auto    (0x8D)  //auto switching three levels
#define Cmd_Get_Charger_36V_Voltage_Auto    (0x8E)  //auto switching three levels
#define Cmd_Get_Charger_48V_Voltage_Auto    (0x8F)  //auto switching three levels

#define Cmd_Charger_24V_Channel_Set_Vin     (0xA0)  //switch Mux, set input gate but set ID Levels Off
#define Cmd_Charger_36V_Channel_Set_Vin     (0xA1)  //switch Mux, set input gate but set ID Levels Off
#define Cmd_Charger_48V_Channel_Set_Vin     (0xA2)  //switch Mux, set input gate but set ID Levels Off

#define Cmd_Charger_All_Channel_ID_Set_OFF  (0xA3)  //switch Mux empty, set input gate off and set ID Levels Off
#define Cmd_Charger_All_Channel_Set_Vin     (0xA4)  //orginal function for version1, no Used on Ver 2.0
#define Cmd_Get_All_Charger_Channel_Voltage (0xA5)  //no Used on Ver 2.0
#define Cmd_Auto_Charger_Checking           (0xA6)  //no Used on Ver 2.0
#define Cmd_Fast_Auto_Charger_Checking      (0xA7)  //no Used on Ver 2.0

#define Cmd_Get_PACK_DSG_Voltage_Auto       (0xA8)  //auto turn on gate
#define Cmd_Get_PACK_CHG_Voltage_Auto       (0xA9)  //auto turn on gate

#define Cmd_Get_Channel_Raw_ADC             (0xAA)
#define Cmd_Get_Direct_PackDSG_Voltage      (0xAB)
#define Cmd_Get_Direct_PackCHG_Voltage      (0xAC)
#define Cmd_Get_Direct_Chger_24Voltage      (0xAD)
#define Cmd_Get_Direct_Chger_36Voltage      (0xAE)
#define Cmd_Get_Direct_Chger_48Voltage      (0xAF)

#define Cmd_Get_Direct_DSG_Current          (0xB0)
#define Cmd_Get_Direct_CHG_Current          (0xB1)

#define Cmd_Get_Charger_Is_ID_Level         (0xB2)


// Calibration Status cmd
#define Cmd_Cal_Set_Charger_24V_Channel_Offset  (0xD0)
#define Cmd_Cal_Set_Charger_36V_Channel_Offset  (0xD1)
#define Cmd_Cal_Set_Charger_48V_Channel_Offset  (0xD2)
#define Cmd_Get_All_Calibration_Data            (0xD3)
#define Cmd_Get_All_Flash_Data                  (0xD4)
#define Cmd_Set_Cal_Data_To_Flash               (0xD5)

#define Cmd_Cal_Set_PACK_DSG_Vol_CAL_ADC_offset     (0xD6)
#define Cmd_Cal_Set_PACK_CHG_Vol_CAL_ADC_offset     (0xD7)

#define Cmd_Cal_Set_DSG_Current_CAL_ADC_offset      (0xD8)
#define Cmd_Cal_Set_CHG_Current_CAL_ADC_offset      (0xD9)


// Test/Debug Status cmd
#define Cmd_Error_Cmd                   (0xE0)
#define Cmd_For_Connect_Detection       (0xE1)
#define Cmd_Test_Data_Send_Back         (0xE2)
#define Cmd_Test_Get_All_Raw_ADC_Data   (0xE3)

// cmd
#define Cmd_FW_HW_Version               (0xE5)  //
#define Cmd_SetDetectCharger_DelayCycle (0xE6)  //




//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//==============================================================================
// Global variables define
//==============================================================================
///* Driver G_Device_Interface_Status Control Bits */
///* For G_Device_Interface_Status ; unsigned int */
////Low byte
//#define CHG_MOSFET_STATUS               (0x0001)    //CHG MOSFET Status, set = turn on
//#define DSG_MOSFET_STATUS               (0x0002)    //DSG MOSFET Status, set = turn on
//#define ADC_CONVERSION                  (0x0004)    //
//#define ADP_SOC_STATUS                  (0x0008)    //Adapter Soc Pin status
//#define ONE_WIRE_ENABLE_STATUS          (0x0010)    //One Wire control pin enable for one-wire communication
//#define UART_ISOLATE_ENABLE_STATUS      (0x0020)    //UART Isolation Enable for communication
//#define NTC_ENABLE_STATUS               (0x0040)    //NTC Detection Enable control pin
//#define POLLING_TIMER_ENABLE_STATUS     (0x0080)    //
//////Hight byte
////#define                      (0x0100)    //
////#define                   (0x0200)    //
////#define                   (0x0400)    //
////#define               (0x0800)    //
////#define         (0x1000)    ///
////#define         (0x2000)    //
////#define            (0x4000)    //
////#define            (0x8000)    //
///////////////////////////////////////////////////////////////////////////////////
//
///* Driver G_Extend_Device_Interface_Status Control Bits */
///* For G_Extend_Device_Interface_Status ; unsigned int */
////Low byte
//#define UART_STATUS                     (0x0001)    //
//#define UART_TX_IT_STATUS               (0x0002)    //
//#define UART_RX_IT_STATUS               (0x0004)    //
//#define UART_RX_FRAME_DONE              (0x0008)    //
//#define UART_RX_PrecedingCode_Find      (0x0010)    //
//#define UART_RX_EndingCode_Find         (0x0020)    //
//#define UART_RX_FRAME_ADDRESS_FAIL      (0x0040)    //
//#define UART_RX_FRAME_PACKET_FAIL       (0x0080)    //
//////Hight byte
////#define    (0x0100)    //
////#define                   (0x0200)    //
////#define                   (0x0400)    //
////#define               (0x0800)    //
////#define         (0x1000)    ///
////#define         (0x2000)    //
////#define            (0x4000)    //
////#define            (0x8000)    //
///////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// Private variables
//==============================================================================
//==============================================================================
// Private function prototypes
//==============================================================================
//////////////////////////////////////////////////
// For USB CDC Setup  : (section start)
void _DUI_Init_USB_AS_CDC_Communication();
void _DUI_USB_CDC_Polling_Status_Function();
void _DUI_CDC_Receive_Calling_Function(t_uint8* receivedBytesBuffer, t_uint16 receivingSize);
void _DUI_CDC_Transmitting_Data(t_uint8* sendBuffer, t_uint16 length);
void _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(t_uint8 respons_cmd, t_uint8* sendBuffer, t_uint16 length);
void _DUI_USB_Main_Polling_Function_For_Parsing_Receiving_Packet();

// For USB CDC Setup  : (section stop)
//////////////////////////////////////////////////


////////////////////////////////////////////////////
//// Utility Functions  : (section start)
//// Utility Functions  : (section stop)
////////////////////////////////////////////////////

//==============================================================================
// Private functions
//==============================================================================