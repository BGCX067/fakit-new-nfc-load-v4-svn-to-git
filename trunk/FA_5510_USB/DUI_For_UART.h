
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


//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
enum Uart_Module{
    Uart_RS485_Module,  //UART_Module_1
    One_Wire_Module     //UART_Module_2
};

enum Receiving_Fun_Index_For_Detect_Timer{
    Uart_RS485_Module_Receiving_Frame_Fun_Index,     // 0
    One_Wire_Module_Receiving_Frame_Fun_Index        // 1
};
//==============================================================================
// Global variables define
//==============================================================================
#define UART_RECEIVING_DATA_NOT_READY       0
#define UART_RECEIVING_DATA_READY           1

#define UratRXFrameEndGapTime	                50  // unit: 1ms
#define UART_Receiving_Max_Data_Length          550 //(0xef)  //whole structure Length
#define UART_Transmitting_Max_Data_Length       (0x1f)  //whole structure Length

#define Default_BAUD_RATE                       9600


/* Driver g_UART_Module_Status_Flag Control Bits */
/* For g_UART_Module_Status_Flag ; unsigned int */
//Low byte
#define Detect_UART_M1_End_Frame            (0x0001)    //
#define Detect_UART_M2_End_Frame            (0x0002)    //
#define Detect_UART_M1_End_Code             (0x0004)    //
#define Detect_UART_M2_End_Code             (0x0008)    //
//#define UART_RX_PrecedingCode_Find      (0x0010)    //
//#define UART_RX_EndingCode_Find         (0x0020)    //
//#define UART_RX_FRAME_ADDRESS_FAIL      (0x0040)    //
//#define UART_RX_FRAME_PACKET_FAIL       (0x0080)    //
////Hight byte
//#define    (0x0100)    //
//#define                   (0x0200)    //
//#define                   (0x0400)    //
//#define               (0x0800)    //
//#define         (0x1000)    ///
//#define         (0x2000)    //
//#define            (0x4000)    //
//#define            (0x8000)    //
//==============================================================================
// Private variables
//==============================================================================
//==============================================================================
// Private function prototypes
//==============================================================================
//////////////////////////////////////////////////
// For DUI UART Setup  : (section start)
void _DUI_Set_Communication_BAUD_RATE(t_uint32 baud_rate);
void _DUI_Communication_Enable(t_uint8 uart_module);
void _DUI_Communication_Disable(t_uint8 uart_module);
void _DUI_Communication_Send_Bytes(t_uint8 uart_module, unsigned char *sendData, unsigned int length);
t_uint8 _DUI_Is_Comm_Module_Receiving_Data_Ready(t_uint8 uart_module);
void _DUI_Get_Receiving_Data_To_Array(t_uint8 uart_module, t_uint8 *out_Array_ptr, t_uint16 *out_Array_length, t_uint16 max_Length);


// For DUI UART Setup  : (section stop)
//////////////////////////////////////////////////


////////////////////////////////////////////////////
//// Utility Functions  : (section start)
//// Utility Functions  : (section stop)
////////////////////////////////////////////////////

//==============================================================================
// Private functions
//==============================================================================