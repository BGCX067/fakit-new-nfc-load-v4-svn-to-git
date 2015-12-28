
/**
  ******************************************************************************
  * @file    Driver_User_Interface for UART.c
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
#include "Utilities/Utilities.h"
#include "DUI_For_UART.h"
#include "DUI_For_Peripheral_Control.h"
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
#define LeadingCode                 (0x3A)
#define SlaveAddressCode            (0x16)
#define EndCode1                    (0x0D)
#define EndCode2                    (0x0A)

#define Respond_Error_Check_Code    (0xF3)
#define Respond_Accept_Check_Code   (0xF0)

#define MIN_Receive_Packet_Num      (9)
#define MIN_Receive_Packet_Index    (MIN_Receive_Packet_Num - 1)

#define ONE_WIRE_PrecedingCheckCode         (0x80f8)
#define ONE_WIRE_Data_PrecedingCode         (0x80A0)
#define ONE_WIRE_EEPROM_Seg_PrecedingCode   (0x80D0)
#define ONE_WIRE_EndCheckCode               (0x70f7)

//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
////////////////////////////////////////////////////////////////////////
// Uart  : (section start)
//
//Baud rate : 9600
//=======Receiving Data Structure===============================
//typedef struct{
//    static char cStart = 0X3A;	//起始字元
//    unsigned char 0X16;		    //Slave Address
//    unsigned char Command;		//命令
//    unsigned char LenExpected;	//數據長度
//    unsigned char DataBuf[DATA_BUF_NUM];//數據內容
//    unsigned char LRCDataLow;	    //checkSum with slave Low byte, included slave address, command, length and data.
//    unsigned char LRCDataHigh;	//checkSum with slave High byte, included slave address, command, length and data.
//    static char cEND1= 0X0D;	//結束字元 1
//    static char cEND1= 0X0A;	//結束字元 2
//} LEV Protocol Packet;
//
//========Transmiting Data Structure===========================
//typedef struct{
//    static char cStart = 0X3A;	//起始字元
//    unsigned char 0X16;		//Slave Address
//    unsigned char Command;		//應回應的命令
//    unsigned char LenExpected;	//數據長度
//    unsigned char DataBuf[DATA_BUF_NUM];//數據內容
//    unsigned char LRCDataLow;	    //checkSum with slave Low byte, included slave address, command, length and data.
//    unsigned char LRCDataHigh;	//checkSum with slave High byte, included slave address, command, length and data.
//    static char cEND1= 0X0D;	//結束字元 1
//    static char cEND1= 0X0A;	//結束字元 2
//} LEV Protocol Packet;
//
////=========Transmiting Accept Respond Structure ( No used )===================================================================
//typedef struct{
//    static char cStart = 0X3A;	//起始字元
//    unsigned char 0X16;		//Slave Address
//    unsigned char Respond_Accept_Check_Code = 0x80; //Accept 碼
//    unsigned char LenExpected = 1;	//數據長度
//    unsigned char Command;		//應回應的命令
//    unsigned char LRCDataLow;	    //checkSum with slave Low byte, included slave address, command, length and data.
//    unsigned char LRCDataHigh;	//checkSum with slave High byte, included slave address, command, length and data.
//    static char cEND1= 0X0D;	//結束字元 1
//    static char cEND1= 0X0A;	//結束字元 2
//} LEV Protocol Packet;
//
//=========Transmiting Error-Respond Structure ( No used )===================================================================
//typedef struct{
//    static char cStart = 0X3A;	//起始字元
//    unsigned char 0X16;		//Slave Address
//    unsigned char Respond_Error_Check_Code = 0x83; //錯誤碼
//    unsigned char LenExpected = 1;	//數據長度
//    unsigned char Command;		//應回應的命令
//    unsigned char LRCDataLow;	    //checkSum with slave Low byte, included slave address, command, length and data.
//    unsigned char LRCDataHigh;	//checkSum with slave High byte, included slave address, command, length and data.
//    static char cEND1= 0X0D;	//結束字元 1
//    static char cEND1= 0X0A;	//結束字元 2
//} LEV Protocol Packet;
//
//========One Wire Transmiting Data Structure=======================================================
//typedef struct{
//    static uint cStart1 = 0x80f8;	//起始字元 // send High byte first, then send Low byte second.
//    static uint cStart2 = 0x80A0;	//function字元 // send High byte first, then send Low byte second.
//                                    // 0x80A0 : data 資料,
//                                    // 0x80D0 ~ 0x80DF : EEPROM Seg 0 ~ 15 (a segment = 64 bytes),
//    unsigned int LenExpected;//數據長度 ; send High byte first, then send Low byte second.
//    unsigned char DataBuf[LenExpected];//數據內容
//    unsigned char LRCDataHigh;	//CRC or checkSum High byte, calculating only for DataBuf
//    unsigned char LRCDataLow;	//CRC or checkSum Low byte, calculating only for DataBuf
//    static uint cEND1= 0x70f7;	//結束字組 1 // send High byte first, then send Low byte second.
//    static uint cEND1= 0x70f7;	//結束字組 2 // send High byte first, then send Low byte second.
//} ModbusProtocolPacket;//RTU mode
//========One Wire Transmiting EEPROM Data Structure=======================================================
//typedef struct{
//    static uint cStart1 = 0x80f8;	//起始字元 // send High byte first, then send Low byte second.
//    static uint cStart2 = 0x80D0;	//function字元 // send High byte first, then send Low byte second.
//    unsigned char DataBuf[];//數據內容
//    unsigned char LRCDataHigh;	//CRC or checkSum High byte, calculating only for DataBuf
//    unsigned char LRCDataLow;	//CRC or checkSum Low byte, calculating only for DataBuf
//    static uint cEND1= 0x70f7;	//結束字組 1 // send High byte first, then send Low byte second.
//    static uint cEND1= 0x70f7;	//結束字組 2 // send High byte first, then send Low byte second.
//} ModbusProtocolPacket;//RTU mode
//

//==============================================================================
// Private variables
//==============================================================================


unsigned int g_UART_Module_Status_Flag;

t_uint32 UART_Module_Setting_BAUD_RATE = 0;


__IO unsigned int UART_Module_1_Receiving_Data_Index;
unsigned char UART_Module_1_Receiving_Data[UART_Receiving_Max_Data_Length ];
unsigned char UART_Module_1_Transmitting_Data[UART_Transmitting_Max_Data_Length ];
//unsigned int UART_Module_1_Receiving_TempData_Index;
//unsigned char UART_Module_1_Receiving_TempData[UART_Receiving_Max_Data_Length ];

__IO unsigned int UART_Module_2_Receiving_Data_Index;
unsigned char UART_Module_2_Receiving_Data[UART_Receiving_Max_Data_Length ];
unsigned char UART_Module_2_Transmitting_Data[UART_Transmitting_Max_Data_Length ];
//unsigned int UART_Module_2_Receiving_TempData_Index;
//unsigned char UART_Module_2_Receiving_TempData[UART_Receiving_Max_Data_Length ];



//==============================================================================
// Private function prototypes
//==============================================================================

//==============================================================================
// Private functions
//==============================================================================
static void clear_Comm_Receive_Buffer(t_uint8 uart_module){
    t_uint16 i;
    switch(uart_module){
        case Uart_RS485_Module:
            for(i = 0; i < UART_Receiving_Max_Data_Length; i++){
                UART_Module_1_Receiving_Data[i] = 0;
            }
            UART_Module_1_Receiving_Data_Index = 0;
            break;
        case One_Wire_Module:
            for(i = 0; i < UART_Receiving_Max_Data_Length; i++){
                UART_Module_2_Receiving_Data[i] = 0;
            }
            UART_Module_2_Receiving_Data_Index = 0;
            break;
        default:
            break;
    }
}


static void set_Value_To_Receive_Buffer(t_uint8 uart_module, __IO t_uint8 value){
    t_uint8 temp;
    switch(uart_module){
        case Uart_RS485_Module:
            if(UART_Module_1_Receiving_Data_Index >= UART_Receiving_Max_Data_Length){
                UART_Module_1_Receiving_Data_Index = 0;
            }
            temp = value;
            UART_Module_1_Receiving_Data[UART_Module_1_Receiving_Data_Index] = temp;
            UART_Module_1_Receiving_Data_Index++;
            break;
        case One_Wire_Module:
            if(UART_Module_2_Receiving_Data_Index >= UART_Receiving_Max_Data_Length){
                UART_Module_2_Receiving_Data_Index = 0;
            }
            temp = value;
            UART_Module_2_Receiving_Data[UART_Module_2_Receiving_Data_Index] = temp;
            UART_Module_2_Receiving_Data_Index++;
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// calling by Uart Interrupt RX
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//_Module_1 calling by Timer for define frame
////////////////////////////////////////////////////////////////////////////////
static void Communication_Module_1_Receive_Frame_Detection_By_Timer(){
//    if(UART_Module_1_Receiving_Data_Index < 8){ //include : leading, addr, length, 1-data-byte, 2bytes checksum, 2bytes end code
//        clear_Comm_Receive_Buffer(Uart_RS485_Module);
//    }else{
//        g_UART_Module_Status_Flag |= Detect_UART_M1_End_Frame;
//    }
    g_UART_Module_Status_Flag |= Detect_UART_M1_End_Frame;
}
static void Communication_Module_1_Calling_By_Receive_Interrupt_With_Timer(__IO unsigned char receivedByte){
//    t_uint8 temp = 0;
//    if(g_UART_Module_Status_Flag & Detect_UART_M1_End_Frame){
//        temp = receivedByte;
//        return;
//    }
    //g_UART_Module_Status_Flag &= ~Detect_UART_M1_End_Frame;
    set_Value_To_Receive_Buffer(Uart_RS485_Module, receivedByte);
    _Device_Set_TimerB_Interrupt_Timer_Calling_Function_With_Delay_And_Exec(Uart_RS485_Module_Receiving_Frame_Fun_Index, Communication_Module_1_Receive_Frame_Detection_By_Timer, UratRXFrameEndGapTime);
}
static t_uint8 Check_Communication_Module_1_Receive_Data(){
    if((g_UART_Module_Status_Flag & Detect_UART_M1_End_Frame) == 0){
        return UART_RECEIVING_DATA_NOT_READY;
    }
    return UART_RECEIVING_DATA_READY;
}

////////////////////////////////////////////////////////////////////////////////
//_Module_2 calling by Timer for define frame
////////////////////////////////////////////////////////////////////////////////
static void Communication_Module_2_Receive_Frame_Detection_By_Timer(){
//    if(UART_Module_2_Receiving_Data_Index < 8){ //include : leading, addr, length, 1-data-byte, 2bytes checksum, 2bytes end code
//        clear_Comm_Receive_Buffer(One_Wire_Module);
//    }else{
//        g_UART_Module_Status_Flag |= Detect_UART_M2_End_Frame;
//    }
    g_UART_Module_Status_Flag |= Detect_UART_M2_End_Frame;
}
static void Communication_Module_2_Calling_By_Receive_Interrupt_With_Timer(__IO unsigned char receivedByte){
//    t_uint8 temp = 0;
//    if(g_UART_Module_Status_Flag & Detect_UART_M2_End_Frame){
//        temp = receivedByte;
//        return;
//    }
    //g_UART_Module_Status_Flag &= ~Detect_UART_M2_End_Frame;
    set_Value_To_Receive_Buffer(One_Wire_Module, receivedByte);
    _Device_Set_TimerB_Interrupt_Timer_Calling_Function_With_Delay_And_Exec(One_Wire_Module_Receiving_Frame_Fun_Index, Communication_Module_2_Receive_Frame_Detection_By_Timer, UratRXFrameEndGapTime);
}
static t_uint8 Check_Communication_Module_2_Receive_Data(){
    if((g_UART_Module_Status_Flag & Detect_UART_M2_End_Frame) == 0){
        return UART_RECEIVING_DATA_NOT_READY;
    }
    return UART_RECEIVING_DATA_READY;
}



////////////////////////////////////////////////////////////////////////////////
// calling by Timer4 for define frame
////////////////////////////////////////////////////////////////////////////////
void _DUI_Set_Communication_BAUD_RATE(t_uint32 baud_rate){
    UART_Module_Setting_BAUD_RATE = baud_rate;
}

void _DUI_Communication_Enable(t_uint8 uart_module){
    if(UART_Module_Setting_BAUD_RATE == 0){
        UART_Module_Setting_BAUD_RATE = Default_BAUD_RATE;
    }
    switch(uart_module){
        case Uart_RS485_Module:
            _Device_Uart_Module_1_Enable(UART_Module_Setting_BAUD_RATE);
            _Device_Uart_Module_1_Set_Calling_Function_By_Uart_Receive_Interrupt(Communication_Module_1_Calling_By_Receive_Interrupt_With_Timer);
            break;
        case One_Wire_Module:
            _Device_Uart_Module_2_Enable(UART_Module_Setting_BAUD_RATE);
            _Device_Uart_Module_2_Set_Calling_Function_By_Uart_Receive_Interrupt(Communication_Module_2_Calling_By_Receive_Interrupt_With_Timer);
            break;
        default:
            break;
    }
}

void _DUI_Communication_Disable(t_uint8 uart_module){
    switch(uart_module){
        case Uart_RS485_Module:
            _Device_Uart_Module_1_Disable();
            break;
        case One_Wire_Module:
            _Device_Uart_Module_2_Disable();
            break;
        default:
            break;
    }
}

void _DUI_Communication_Send_Bytes(t_uint8 uart_module, unsigned char *sendData, unsigned int length){
	//unsigned int commLength;
    int i;
    if(length > UART_Transmitting_Max_Data_Length){
        return;
    }
    switch(uart_module){
        case Uart_RS485_Module:
            for(i = 0; i < length; i++){
                UART_Module_1_Transmitting_Data[i] = sendData[i];
            }
            _Device_Uart_Module_1_Send_Bytes( UART_Module_1_Transmitting_Data, length); // send high byte first, send low byte second
            break;
        case One_Wire_Module:
            for(i = 0; i < length; i++){
                UART_Module_2_Transmitting_Data[i] = sendData[i];
            }
            _Device_Uart_Module_2_Send_Bytes( UART_Module_2_Transmitting_Data, length); // send high byte first, send low byte second
            break;
        default:
            break;
    }
}

t_uint8 _DUI_Is_Comm_Module_Receiving_Data_Ready(t_uint8 uart_module){
    t_uint8 status;
    status = UART_RECEIVING_DATA_NOT_READY;
    switch(uart_module){
        case Uart_RS485_Module:
            if(Check_Communication_Module_1_Receive_Data() == UART_RECEIVING_DATA_READY){
                status = UART_RECEIVING_DATA_READY;
            }
            break;
        case One_Wire_Module:
            if(Check_Communication_Module_2_Receive_Data() == UART_RECEIVING_DATA_READY){
                status = UART_RECEIVING_DATA_READY;
            }
            break;
        default:
            break;
    }
    return status;
}

void _DUI_Get_Receiving_Data_To_Array(t_uint8 uart_module, t_uint8 *out_Array_ptr, t_uint16 *out_Array_length, t_uint16 max_Length){
    t_uint16 i;
    t_uint16 length;

    switch(uart_module){
        case Uart_RS485_Module:
            if(UART_Module_1_Receiving_Data_Index > max_Length){
                length = max_Length;
            }else{
                 length = UART_Module_1_Receiving_Data_Index;
           }
            for(i = 0; i < length; i++){
                out_Array_ptr[i] = UART_Module_1_Receiving_Data[i];
            }
            *out_Array_length = length;
            g_UART_Module_Status_Flag &= ~Detect_UART_M1_End_Frame;
            clear_Comm_Receive_Buffer(Uart_RS485_Module);
            break;
        case One_Wire_Module:
            if(UART_Module_2_Receiving_Data_Index > max_Length){
                length = max_Length;
            }else{
                 length = UART_Module_2_Receiving_Data_Index;
           }
            for(i = 0; i < length; i++){
                out_Array_ptr[i] = UART_Module_2_Receiving_Data[i];
                UART_Module_2_Receiving_Data[i] = 0;
            }
            *out_Array_length = length;
            UART_Module_2_Receiving_Data_Index = 0;
            g_UART_Module_Status_Flag &= ~Detect_UART_M2_End_Frame;
            clear_Comm_Receive_Buffer(One_Wire_Module);
            break;
        default:
            break;
    }
}

