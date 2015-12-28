/**
  ******************************************************************************
  * @file    Driver_User_Interface_for_USB_CDC.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    17-April-2013
  * @brief   User_Interface_for_USB_CDC
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
#include "Global_Vars_Define.h"
#include "Vars_Bit_Define.h"
#include "SystemConfigDefineForFlash.h"

#include "MCU_Devices/MCU_Devices.h"
#include "Utilities/Utilities.h"
#include "DUI_For_USB_CDC.h"
#include "DUI_For_Peripheral_Control.h"
#include "DUI_For_UART.h"

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

#define CDC_Receiving_Max_Data_Length       (0x1f)  //not whole structure Length, only Data buffer Length
#define CDC_Transmitting_Max_Data_Length    550 //(0xff)  //not whole structure Length, only Data buffer Length

//#define LeadingCode                 (0xCA)
//#define SlaveAddressCode            (0xA6)
#define LeadingCode                 (0x3A)
#define SlaveAddressCode            (0xA6)
#define EndingCode1                 (0x0D)
#define EndingCode2                 (0x0A)

#define Respond_Error_Check_Code    (0xF3)
#define Respond_Accept_Check_Code   (0xF0)

#define Comm_Receive_Buffer_Size        (CDC_Receiving_Max_Data_Length * 2)
#define Comm_Transmitting_Buffer_Size   (CDC_Transmitting_Max_Data_Length + 15)

/* Driver g_Usb_Cdc_Status_FLAG Control Bits */
/* For g_Usb_Cdc_Status_FLAG ; unsigned int */
//Low byte
#define CDC_RX_Packet_Found                     (0x0001)    //
#define CDC_RX_Packet_Check_True                 (0x0002)    //
//#define CDC_RX_Packet_Check_True            (0x0004)    //for CRC or checkSum Flag
//#define UART_RX_FRAME_DONE              (0x0008)    //
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
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//=======USB Receiving From PC Data Structure===============================
typedef struct{
    //static char cStart = 0X3A;	    //起始字元
    unsigned char SlAdd;		        //Slave Address
    unsigned char Command;		        //命令
    unsigned char DataLenExpected_Low;	    //數據長度,
    unsigned char DataLenExpected_High;	    //數據長度,
    unsigned char DataBuf[CDC_Receiving_Max_Data_Length];//數據內容
    unsigned char LRCDataLow;           //checkSum16 Low byte, included slave address, command, length and data.
    unsigned char LRCDataHigh;	        //checkSum16 High byte, included slave address, command, length and data.
    //static char cEND1= 0X0D;	        //結束字元 1
    //static char cEND1= 0X0A;	        //結束字元 2
}USB_Receiving_Protocol_Packet;

//========USB Transmitting To PC  Data Structure===========================
typedef struct{
//    static char cStart = 0X3A;        //起始字元
    unsigned char SlAdd;                //Slave Address
    unsigned char ResCommand;		    //應回應的命令
    unsigned char DataLenExpected_Low;	    //數據長度,
    unsigned char DataLenExpected_High;	    //數據長度,
    unsigned char DataBuf[CDC_Transmitting_Max_Data_Length];//數據內容
    unsigned char LRCDataLow;	        //checkSum16 Low byte, included slave address, command, length and data.
    unsigned char LRCDataHigh;	        //checkSum16 High byte, included slave address, command, length and data.
//    static char cEND1= 0X0D;	        //結束字元 1
//    static char cEND1= 0X0A;	        //結束字元 2
}USB_Transmitting_Protocol_Packet;
//

//==============================================================================
// Public variables
//==============================================================================
t_uint16 g_Usb_Cdc_Status_FLAG;
//==============================================================================
// Private variables
//==============================================================================
t_uint8 Comm_Receive_Buffer[Comm_Receive_Buffer_Size];
t_uint16 Comm_Receive_Buffer_Index;
t_uint8 Comm_Transmitting_Buffer[Comm_Transmitting_Buffer_Size];
t_uint8 Comm_Temp_Transmitting_Data_Buffer[CDC_Transmitting_Max_Data_Length];
USB_Receiving_Protocol_Packet receiving_Data_Packet;
//==============================================================================
// Private function prototypes
//==============================================================================
//extern t_uint8 tdata[];

//==============================================================================
// Private functions
//==============================================================================
static void clear_Comm_Receive_Buffer(){
    t_uint16 i;
    for(i = 0; i < Comm_Receive_Buffer_Size; i++){
        Comm_Receive_Buffer[i] = 0;
    }
    Comm_Receive_Buffer_Index = 0;
}
static void shift_Comm_Receive_Buffer_To_First_Position(t_uint16 start_shift_Index){
    t_uint16 i;
    t_uint16 idx;
    if(Comm_Receive_Buffer_Index > Comm_Receive_Buffer_Size){
        Comm_Receive_Buffer_Index = 0;
    }
    if(start_shift_Index >= Comm_Receive_Buffer_Index){
        clear_Comm_Receive_Buffer();
        return;
    }
    idx = 0;
    for(i = start_shift_Index; i < Comm_Receive_Buffer_Index; i++){
        Comm_Receive_Buffer[idx] = Comm_Receive_Buffer[start_shift_Index];
        idx++;
    }
    Comm_Receive_Buffer_Index = idx;
}
static void set_Value_To_Receive_Buffer(t_uint8 value){
    if(Comm_Receive_Buffer_Index >= Comm_Receive_Buffer_Size){
        Comm_Receive_Buffer_Index = 0;
    }
    Comm_Receive_Buffer[Comm_Receive_Buffer_Index] = value;
    Comm_Receive_Buffer_Index++;
}
////////////////////////////////////////////////////////////////////////////////
static t_uint8 Calculate_Receive_Data_To_Packet_For_CheckSum16(){

    t_uint16 chkSum;
    t_uint8 chkSum_hi, chkSum_lo;
    t_uint16 DataLenExpected;

    DataLenExpected = receiving_Data_Packet.DataLenExpected_High;
    DataLenExpected = (DataLenExpected << 8) + receiving_Data_Packet.DataLenExpected_Low;
    chkSum = usCheckSum16(&(receiving_Data_Packet.SlAdd), DataLenExpected + 4);
    chkSum_hi = chkSum >> 8;
    chkSum_lo = chkSum & 0x00ff;
    if((chkSum_hi == receiving_Data_Packet.LRCDataHigh) && (chkSum_lo == receiving_Data_Packet.LRCDataLow)){
        return Func_Success;
    }else{
        return Func_Failure;
    }
}
static void Parsing_Receive_Data_To_Packet(){
    t_uint16 i,j;
    t_uint16 startFormIndex;
    t_uint16 dataLength;
    t_uint16 endCode1_idx, endCode2_idx;
    //t_uint16 CheckSumLow_idx, CheckSumHigh_idx;
    if(g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Found){
        return;
    }

    for(i = 0; i < (Comm_Receive_Buffer_Index - 1); i++){
        //finding leading codes
        if((Comm_Receive_Buffer[i] == LeadingCode) && (Comm_Receive_Buffer[i+1] == SlaveAddressCode)){
            startFormIndex = i;
            dataLength = Comm_Receive_Buffer[startFormIndex + 4];    //offset 4 to get receiving data length_High
            dataLength = (dataLength << 8) + Comm_Receive_Buffer[startFormIndex + 3];    //offset 3 to get receiving data length_Low
            endCode1_idx = startFormIndex + 4 + dataLength + 2 + 1;  //add offset 2 is two of Cuecksum bytes
            endCode2_idx = startFormIndex + 4 + dataLength + 2 + 2;  //add offset 2 is two of Cuecksum bytes
            //CheckSumLow_idx = startFormIndex + 3 + dataLength + 1;  //Cuecksum Low byte idx
            //CheckSumHigh_idx = startFormIndex + 3 + dataLength + 2;  //Cuecksum High byte idx
            //finding Ending Codes
            if((Comm_Receive_Buffer[endCode1_idx] == EndingCode1) && (Comm_Receive_Buffer[endCode2_idx] == EndingCode2)){
                //Save data to structure
                receiving_Data_Packet.SlAdd = SlaveAddressCode;
                receiving_Data_Packet.Command = Comm_Receive_Buffer[startFormIndex + 2];
                receiving_Data_Packet.DataLenExpected_Low = dataLength;
                receiving_Data_Packet.DataLenExpected_High = dataLength >> 8;
                for(j = 0; j < dataLength; j++){
                    receiving_Data_Packet.DataBuf[j] = Comm_Receive_Buffer[startFormIndex + 4 + 1 + j];
                }//for
                receiving_Data_Packet.LRCDataLow = Comm_Receive_Buffer[endCode1_idx - 2];
                receiving_Data_Packet.LRCDataHigh = Comm_Receive_Buffer[endCode1_idx - 1];
                g_Usb_Cdc_Status_FLAG |= CDC_RX_Packet_Found;
                shift_Comm_Receive_Buffer_To_First_Position(endCode2_idx + 1);
                //check RX Packet with checkSum16
                if(Calculate_Receive_Data_To_Packet_For_CheckSum16() == Func_Success){
                    g_Usb_Cdc_Status_FLAG |= CDC_RX_Packet_Check_True;
                }else{
                    g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Found;
                    g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Check_True;
                }
                return;
            }//if
        }// if
    }//for(i = 0; i < (Comm_Receive_Buffer_Index - 1); i++){
}

static void CDC_Receive_Calling_Function(t_uint8* receivedBytesBuffer, t_uint16 receivingSize){
    t_uint16 i;
    for(i = 0; i < receivingSize; i++){
        set_Value_To_Receive_Buffer(receivedBytesBuffer[i]);
    }
    Parsing_Receive_Data_To_Packet();
//    if((g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Found) && (g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Check_True)){
//        //_DUI_CDC_Transmitting_Data(&(receiving_Data_Packet.SlAdd), receiving_Data_Packet.DataLenExpected + 3);
//        _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(receiving_Data_Packet.Command, &(receiving_Data_Packet.SlAdd), receiving_Data_Packet.DataLenExpected + 3);
//        g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Found;
//        g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Check_True;
//    }
}
//==============================================================================
// Public functions
//==============================================================================
void _DUI_Init_USB_AS_CDC_Communication(){
    clear_Comm_Receive_Buffer();
    _Device_Init_USB_Config();
    g_Usb_Cdc_Status_FLAG = 0;
    _Device_Set_USB_Receive_From_PC_Calling_Function(CDC_Receive_Calling_Function);

}

void _DUI_USB_CDC_Polling_Status_Function(){
    t_uint8 status;

    status = _Device_Polling_For_USB_Connection_Status();

    switch(status){

        case USB_Status_USB_DISCONNECTED:
            break;
        case USB_Status_USB_CONNECTED_NO_ENUM:
            break;
        case USB_Status_ENUM_ACTIVE:
            break;
        case USB_Status_ENUM_SUSPENDED:
            break;
        case USB_Status_ENUM_IN_PROGRESS:
            break;
        case USB_Status_FAILED_ENUM:
             break;
        case USB_Status_ERROR:
            break;
        case USB_Status_NOENUM_SUSPENDED:
            break;
    }
}



void _DUI_CDC_Transmitting_Data(t_uint8* sendBuffer, t_uint16 length){
    if (_Device_USB_Send_Bytes_To_PC(sendBuffer,length) == Func_Failure){  	//Echo is back to the host
        //Something went wrong -- exit

        //send again
        _Device_USB_Send_Bytes_To_PC(sendBuffer,length);
        //return Func_Failure;
    }
    //return Func_Success;
}
void _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(t_uint8 respons_cmd, t_uint8* sendBuffer, t_uint16 length){
    t_uint16 index, i;
    t_uint16 chkSum;

    if(length >= CDC_Transmitting_Max_Data_Length){
        length = CDC_Transmitting_Max_Data_Length;
    }
    index = 0;
    Comm_Transmitting_Buffer[index++] = LeadingCode;
    Comm_Transmitting_Buffer[index++] = SlaveAddressCode;
    Comm_Transmitting_Buffer[index++] = respons_cmd;
    Comm_Transmitting_Buffer[index++] = length; //low
    Comm_Transmitting_Buffer[index++] = length >> 8; //high
    for(i = 0; i < length; i++){
        Comm_Transmitting_Buffer[index++] = *(sendBuffer++);
    }

    chkSum = usCheckSum16(&(Comm_Transmitting_Buffer[1]), length + 4);
    Comm_Transmitting_Buffer[index++] = chkSum & 0x00ff;    //checkSum Low Bytes
    Comm_Transmitting_Buffer[index++] = chkSum >> 8;        //checkSum High Bytes
    Comm_Transmitting_Buffer[index++] = EndingCode1;
    Comm_Transmitting_Buffer[index++] = EndingCode2;

    _DUI_CDC_Transmitting_Data(Comm_Transmitting_Buffer, index);
}



t_uint32 gCdcTempUint32;
t_uint16 gCdcTempUint16;
t_uint8 gCdcTempUint8;
float gCdcTempFloat;
t_uint8* gCdcTempUint8_ptr;                         // Initialize Flash pointer

void _DUI_USB_Main_Polling_Function_For_Parsing_Receiving_Packet(){
//    if( ((g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Found) == 0 ) ||
//        ((g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Check_True) == 0)){
//        return;
//    }
//    if((g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Found) && (g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Check_True)){
//        //_DUI_CDC_Transmitting_Data(&(receiving_Data_Packet.SlAdd), receiving_Data_Packet.DataLenExpected + 3);
//        _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(receiving_Data_Packet.Command, &(receiving_Data_Packet.SlAdd), receiving_Data_Packet.DataLenExpected + 3);
//        g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Found;
//        g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Check_True;
//    }
    if((g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Found) && (g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Check_True)){
        switch(receiving_Data_Packet.Command){

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Set_DSG_Load_Gate           (0x70)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:turn off, 1:turn on
            //=====================================================================
            // Transmitting DataLenExpected = N/A
            // Transmitting DataBuf[0] = N/A(Lo-byte);      Transmitting DataBuf[1] = N/A(Hi-byte)
            case Cmd_Set_DSG_Load_Gate:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_DSG_Load_Gate,&(gCdcTempUint8), 1);
                    break;
                }
                _DUI_SetKitLoading(receiving_Data_Packet.DataBuf[0]?Turn_On:Turn_Off);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_DSG_Load_Gate,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Set_CHG_Shiftet_Gate           (0x71)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:turn off, 1:turn on
            //=====================================================================
            // Transmitting DataLenExpected = N/A
            // Transmitting DataBuf[0] = N/A(Lo-byte);      Transmitting DataBuf[1] = N/A(Hi-byte)
            case Cmd_Set_CHG_Shiftet_Gate:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_CHG_Shiftet_Gate,&(gCdcTempUint8), 1);
                    break;
                }
                _DUI_SetChargingViaDSGPort(receiving_Data_Packet.DataBuf[0]?Turn_On:Turn_Off);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_CHG_Shiftet_Gate,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Set_ADC_VPC_Gate           (0x72)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:turn off, 1:turn on
            //=====================================================================
            // Transmitting DataLenExpected = N/A
            // Transmitting DataBuf[0] = N/A(Lo-byte);      Transmitting DataBuf[1] = N/A(Hi-byte)
            case Cmd_Set_ADC_VPC_Gate:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_ADC_VPC_Gate,&(gCdcTempUint8), 1);
                    break;
                }
                _DUI_SetPackCHGInputPortForMeasurement(receiving_Data_Packet.DataBuf[0]?Turn_On:Turn_Off);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_ADC_VPC_Gate,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Set_ADC_VPD_Gate           (0x73)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:turn off, 1:turn on
            //=====================================================================
            // Transmitting DataLenExpected = N/A
            // Transmitting DataBuf[0] = N/A(Lo-byte);      Transmitting DataBuf[1] = N/A(Hi-byte)
            case Cmd_Set_ADC_VPD_Gate:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_ADC_VPD_Gate,&(gCdcTempUint8), 1);
                    break;
                }
                _DUI_SetPackDSGInputPortForMeasurement(receiving_Data_Packet.DataBuf[0]?Turn_On:Turn_Off);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_ADC_VPD_Gate,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Communi_Multiplex_Reset         (0x80)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            case Cmd_Communi_Multiplex_Reset:
                _DUI_Commun_MUX_Init();
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Communi_Multiplex_Reset,&(gCdcTempUint8), 1);
                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Communi_Multiplex_Set_Channel   (0x81)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:RS485_Channel, 1:UART_Channel
            case Cmd_Communi_Multiplex_Set_Channel:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Communi_Multiplex_Set_Channel,&(gCdcTempUint8), 1);
                    break;
                }
                _DUI_Switch_To_Communication_Port(receiving_Data_Packet.DataBuf[0]?UART_Channel:RS485_Channel);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Communi_Multiplex_Set_Channel,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_UART_Set_Baud_Rate          (0x82)
            // receiving_Data_Packet.DataLenExpected = 4
            // receiving_Data_Packet.DataBuf[0] = Baud_Rate, Lo-Word Lo-Byte
            // receiving_Data_Packet.DataBuf[1] = Baud_Rate, Lo-Word Hi-Byte
            // receiving_Data_Packet.DataBuf[2] = Baud_Rate, Hi-Word Lo-Byte
            // receiving_Data_Packet.DataBuf[3] = Baud_Rate, Hi-Word Hi-Byte
            case Cmd_UART_Set_Baud_Rate:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low < 4)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_Set_Baud_Rate,&(gCdcTempUint8), 1);
                    break;
                }
                gCdcTempUint32 = receiving_Data_Packet.DataBuf[3];
                gCdcTempUint32 = (gCdcTempUint32 << 8) | receiving_Data_Packet.DataBuf[2];
                gCdcTempUint32 = (gCdcTempUint32 << 8) | receiving_Data_Packet.DataBuf[1];
                gCdcTempUint32 = (gCdcTempUint32 << 8) | receiving_Data_Packet.DataBuf[0];
                _DUI_Set_Communication_BAUD_RATE(gCdcTempUint32);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_Set_Baud_Rate,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            //  Cmd_UART_Set_Default_Baud_Rate  (0x83)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            case Cmd_UART_Set_Default_Baud_Rate:
                _DUI_Set_Communication_BAUD_RATE(0);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_Set_Default_Baud_Rate,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_UART_RS485_Enable           (0x84)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            case Cmd_UART_RS485_Enable:
                _DUI_Communication_Enable(Uart_RS485_Module);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_RS485_Enable,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_UART_RS485_Disable          (0x85)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            case Cmd_UART_RS485_Disable:
                _DUI_Communication_Disable(Uart_RS485_Module);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_RS485_Disable,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_One_Wire_Commu_Enable       (0x86)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            case Cmd_One_Wire_Commu_Enable:
                _DUI_Communication_Enable(One_Wire_Module);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_One_Wire_Commu_Enable,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_One_Wire_Commu_Disable      (0x87)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            case Cmd_One_Wire_Commu_Disable:
                _DUI_Communication_Disable(One_Wire_Module);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_One_Wire_Commu_Disable,&(gCdcTempUint8), 1);
                break;
//            ///////////////////////////////////////////////////////////////////////
//            // Cmd_I2C_Reset                   (0x88)
//            // receiving_Data_Packet.DataLenExpected = 0
//            // receiving_Data_Packet.DataBuf[0] = N/A
//            case Cmd_I2C_Reset:
//                gCdcTempUint8 = Respond_Error_Check_Code;
//                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_I2C_Reset,&(gCdcTempUint8), 1);
//                break;
//            ///////////////////////////////////////////////////////////////////////
//            // Cmd_I2C_Set_Address             (0x89)
//            // receiving_Data_Packet.DataLenExpected = 1
//            // receiving_Data_Packet.DataBuf[0] = Slave Address 7bits
//            case Cmd_I2C_Set_Address:
//                gCdcTempUint8 = Respond_Accept_Check_Code;
//                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_I2C_Set_Address,&(gCdcTempUint8), 1);
//                break;


            ///////////////////////////////////////////////////////////////////////
            // Cmd_Charger_24V_Channel_Set_ID     (0x8A)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:Chger_ID_OFF, 1:Chger_ID_1st_Step, 2:Chger_ID_2nd_Step
            //=====================================================================
            // Transmitting DataLenExpected = NA
            // Transmitting DataBuf[0] = NA(Lo-byte);      Transmitting DataBuf[1] = NA(Hi-byte)
            case Cmd_Charger_24V_Channel_Set_ID:
                if(((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1))
                   || (receiving_Data_Packet.DataBuf[0] >= 3)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_24V_Channel_Set_ID,&(gCdcTempUint8), 1);
                    break;
                }
                if(receiving_Data_Packet.DataBuf[0] == 0){
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
                }else{
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_24V_Ch, (Chger_ID_Steps)receiving_Data_Packet.DataBuf[0]);
                }
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_24V_Channel_Set_ID,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Charger_36V_Channel_Set_ID     (0x8B)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:Chger_ID_OFF, 1:Chger_ID_1st_Step, 2:Chger_ID_2nd_Step
            //=====================================================================
            // Transmitting DataLenExpected = NA
            // Transmitting DataBuf[0] = NA(Lo-byte);      Transmitting DataBuf[1] = NA(Hi-byte)
            case Cmd_Charger_36V_Channel_Set_ID:
                if(((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1))
                   || (receiving_Data_Packet.DataBuf[0] >= 3)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_36V_Channel_Set_ID,&(gCdcTempUint8), 1);
                    break;
                }
                if(receiving_Data_Packet.DataBuf[0] == 0){
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
                }else{
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_36V_Ch, (Chger_ID_Steps)receiving_Data_Packet.DataBuf[0]);
                }
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_36V_Channel_Set_ID,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Charger_48V_Channel_Set_ID     (0x8C)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:Chger_ID_OFF, 1:Chger_ID_1st_Step, 2:Chger_ID_2nd_Step
            //=====================================================================
            // Transmitting DataLenExpected = NA
            // Transmitting DataBuf[0] = NA(Lo-byte);      Transmitting DataBuf[1] = NA(Hi-byte)
            case Cmd_Charger_48V_Channel_Set_ID:
                if(((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1))
                   || (receiving_Data_Packet.DataBuf[0] >= 3)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_48V_Channel_Set_ID,&(gCdcTempUint8), 1);
                    break;
                }
                if(receiving_Data_Packet.DataBuf[0] == 0){
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
                }else{
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_48V_Ch, (Chger_ID_Steps)receiving_Data_Packet.DataBuf[0]);
                }
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_48V_Channel_Set_ID,&(gCdcTempUint8), 1);
                break;

            ///////////////////////////////////////////////////////////////////////
            // Get_Charger_24V_Voltage_Auto     (0x8D)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 12
            // Transmitting DataBuf[0] = ID off, ADC(Lo-byte);      Transmitting DataBuf[1] = ID off, ADC(Hi-byte)
            // Transmitting DataBuf[2] = ID off, Real-mV(Lo-byte);  Transmitting DataBuf[3] = ID off, Real-mV(Hi-byte)
            // Transmitting DataBuf[4] = ID L1, ADC(Lo-byte);       Transmitting DataBuf[5] = ID L1, ADC(Hi-byte)
            // Transmitting DataBuf[6] = ID L1, Real-mV(Lo-byte);   Transmitting DataBuf[7] = ID L1, Real-mV(Hi-byte)
            // Transmitting DataBuf[8] = ID L2, ADC(Lo-byte);       Transmitting DataBuf[9] = ID L2, ADC(Hi-byte)
            // Transmitting DataBuf[10] = ID L2, Real-mV(Lo-byte);  Transmitting DataBuf[11] = ID L2, Real-mV(Hi-byte)
            case Cmd_Get_Charger_24V_Voltage_Auto:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                     G_Module_Function_Status &= ~(Charger_ID_Level_1_Check + Charger_ID_Level_2_Check);
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Charger_24V_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_24V_Voltage_Auto,&(gCdcTempUint8), 1);
                }
///
//                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_24V_Voltage, Comm_Temp_Transmitting_Data_Buffer, 4);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Charger_36V_Voltage_Auto     (0x8E)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 12
            // Transmitting DataBuf[0] = ID off, ADC(Lo-byte);      Transmitting DataBuf[1] = ID off, ADC(Hi-byte)
            // Transmitting DataBuf[2] = ID off, Real-mV(Lo-byte);  Transmitting DataBuf[3] = ID off, Real-mV(Hi-byte)
            // Transmitting DataBuf[4] = ID L1, ADC(Lo-byte);       Transmitting DataBuf[5] = ID L1, ADC(Hi-byte)
            // Transmitting DataBuf[6] = ID L1, Real-mV(Lo-byte);   Transmitting DataBuf[7] = ID L1, Real-mV(Hi-byte)
            // Transmitting DataBuf[8] = ID L2, ADC(Lo-byte);       Transmitting DataBuf[9] = ID L2, ADC(Hi-byte)
            // Transmitting DataBuf[10] = ID L2, Real-mV(Lo-byte);  Transmitting DataBuf[11] = ID L2, Real-mV(Hi-byte)
            case Cmd_Get_Charger_36V_Voltage_Auto:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                     G_Module_Function_Status &= ~(Charger_ID_Level_1_Check + Charger_ID_Level_2_Check);
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Charger_36V_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_36V_Voltage_Auto,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Charger_48V_Voltage_Auto     (0x8F)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 12
            // Transmitting DataBuf[0] = ID off, ADC(Lo-byte);      Transmitting DataBuf[1] = ID off, ADC(Hi-byte)
            // Transmitting DataBuf[2] = ID off, Real-mV(Lo-byte);  Transmitting DataBuf[3] = ID off, Real-mV(Hi-byte)
            // Transmitting DataBuf[4] = ID L1, ADC(Lo-byte);       Transmitting DataBuf[5] = ID L1, ADC(Hi-byte)
            // Transmitting DataBuf[6] = ID L1, Real-mV(Lo-byte);   Transmitting DataBuf[7] = ID L1, Real-mV(Hi-byte)
            // Transmitting DataBuf[8] = ID L2, ADC(Lo-byte);       Transmitting DataBuf[9] = ID L2, ADC(Hi-byte)
            // Transmitting DataBuf[10] = ID L2, Real-mV(Lo-byte);  Transmitting DataBuf[11] = ID L2, Real-mV(Hi-byte)
            case Cmd_Get_Charger_48V_Voltage_Auto:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                     G_Module_Function_Status &= ~(Charger_ID_Level_1_Check + Charger_ID_Level_2_Check);
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Charger_48V_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_48V_Voltage_Auto,&(gCdcTempUint8), 1);
                }
                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Charger_24V_Channel_Set_Vin     (0xA0)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:turn off Channel, 1:Turn on Channel
            case Cmd_Charger_24V_Channel_Set_Vin:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_24V_Channel_Set_Vin,&(gCdcTempUint8), 1);
                    break;
                }
                if(receiving_Data_Packet.DataBuf[0] == 0){
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
                }else{
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_24V_Ch, Chger_ID_OFF);
                }
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_24V_Channel_Set_Vin,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Charger_36V_Channel_Set_Vin     (0xA1)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:turn off Channel, 1:Turn on Channel
            case Cmd_Charger_36V_Channel_Set_Vin:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_36V_Channel_Set_Vin,&(gCdcTempUint8), 1);
                    break;
                }
                if(receiving_Data_Packet.DataBuf[0] == 0){
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
                }else{
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_36V_Ch, Chger_ID_OFF);
                }
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_36V_Channel_Set_Vin,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Charger_48V_Channel_Set_Vin     (0xA2)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:turn off Channel, 1:Turn on Channel
            case Cmd_Charger_48V_Channel_Set_Vin:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_48V_Channel_Set_Vin,&(gCdcTempUint8), 1);
                    break;
                }
                if(receiving_Data_Packet.DataBuf[0] == 0){
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
                }else{
                    _DUI_SwitchChargerInputAndIDStep(Switch_To_48V_Ch, Chger_ID_OFF);
                }
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_48V_Channel_Set_Vin,&(gCdcTempUint8), 1);
                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Charger_All_Channel_ID_Set_OFF      (0xA3)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            case Cmd_Charger_All_Channel_ID_Set_OFF:
                _DUI_SwitchChargerInputAndIDStep(Switch_To_Empty_Ch, Chger_ID_OFF);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_All_Channel_ID_Set_OFF,&(gCdcTempUint8), 1);
                break;
//            ///////////////////////////////////////////////////////////////////////
//            // Cmd_Charger_All_Channel_Set_Vin     (0xA4)
//            // receiving_Data_Packet.DataLenExpected = 1
//            // receiving_Data_Packet.DataBuf[0] = 0:turn off Channel, 1:Turn on Channel
//            case Cmd_Charger_All_Channel_Set_Vin:
//                if(receiving_Data_Packet.DataBuf[0] == 0){
//                    _DUI_Set_Charger_Channel(CHG_OFF_Channel);
//                }else{
//                    _DUI_Set_Charger_Channel(CHG_ON_Channel);
//                }
//                gCdcTempUint8 = Respond_Accept_Check_Code;
//                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Charger_All_Channel_Set_Vin,&(gCdcTempUint8), 1);
//                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_All_Charger_Channel_Voltage (0xA5)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 12
            // Transmitting DataBuf[0] = 24V ADC(Lo-byte);      Transmitting DataBuf[1] = 24V ADC(Hi-byte)
            // Transmitting DataBuf[2] = 24V real-mV(Lo-byte);  Transmitting DataBuf[3] = 24V real-mV(Hi-byte)
            // Transmitting DataBuf[4] = 36V ADC(Lo-byte);      Transmitting DataBuf[5] = 36V ADC(Hi-byte)
            // Transmitting DataBuf[6] = 36V real-mV(Lo-byte);  Transmitting DataBuf[7] = 36V real-mV(Hi-byte)
            // Transmitting DataBuf[8] = 48V ADC(Lo-byte);      Transmitting DataBuf[9] = 48V ADC(Hi-byte)
            // Transmitting DataBuf[10] = 48V real-mV(Lo-byte); Transmitting DataBuf[11] = 48V real-mV(Hi-byte)
//            case Cmd_Get_All_Charger_Channel_Voltage:
//                //24V
//                gCdcTempUint16 = G_Charger_24V_Channel_ADC;
//                Comm_Temp_Transmitting_Data_Buffer[0] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[1] = gCdcTempUint16 >> 8;
//                gCdcTempFloat = FA_24V_mV_To_ADC_Factor;
//                gCdcTempFloat = gCdcTempUint16/gCdcTempFloat;
//                gCdcTempUint16 = (unsigned int)gCdcTempFloat;
//                Comm_Temp_Transmitting_Data_Buffer[2] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[3] = gCdcTempUint16 >> 8;
//
//                //36V
//                gCdcTempUint16 = G_Charger_36V_Channel_ADC;
//                Comm_Temp_Transmitting_Data_Buffer[4] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[5] = gCdcTempUint16 >> 8;
//                gCdcTempFloat = FA_36V_mV_To_ADC_Factor;
//                gCdcTempFloat = gCdcTempUint16/gCdcTempFloat;
//                gCdcTempUint16 = (unsigned int)gCdcTempFloat;
//                Comm_Temp_Transmitting_Data_Buffer[6] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[7] = gCdcTempUint16 >> 8;
//
//                //48V
//                gCdcTempUint16 = G_Charger_48V_Channel_ADC;
//                Comm_Temp_Transmitting_Data_Buffer[8] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[9] = gCdcTempUint16 >> 8;
//                gCdcTempFloat = FA_48V_mV_To_ADC_Factor;
//                gCdcTempFloat = gCdcTempUint16/gCdcTempFloat;
//                gCdcTempUint16 = (unsigned int)gCdcTempFloat;
//                Comm_Temp_Transmitting_Data_Buffer[10] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[11] = gCdcTempUint16 >> 8;
//
//                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_All_Charger_Channel_Voltage,Comm_Temp_Transmitting_Data_Buffer, 12);
//                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Auto_Charger_Checking           (0xA6)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 24
            // channel ID set Off
            // Transmitting DataBuf[0] = 24V ADC(Lo-byte);      Transmitting DataBuf[1] = 24V ADC(Hi-byte)
            // Transmitting DataBuf[2] = 24V real-mV(Lo-byte);  Transmitting DataBuf[3] = 24V real-mV(Hi-byte)
            // Transmitting DataBuf[4] = 36V ADC(Lo-byte);      Transmitting DataBuf[5] = 36V ADC(Hi-byte)
            // Transmitting DataBuf[6] = 36V real-mV(Lo-byte);  Transmitting DataBuf[7] = 36V real-mV(Hi-byte)
            // Transmitting DataBuf[8] = 48V ADC(Lo-byte);      Transmitting DataBuf[9] = 48V ADC(Hi-byte)
            // Transmitting DataBuf[10] = 48V real-mV(Lo-byte); Transmitting DataBuf[11] = 48V real-mV(Hi-byte)
            // -----------------------------------------
            // channel ID set on
            // Transmitting DataBuf[0] = 24V ADC(Lo-byte);      Transmitting DataBuf[1] = 24V ADC(Hi-byte)
            // Transmitting DataBuf[2] = 24V real-mV(Lo-byte);  Transmitting DataBuf[3] = 24V real-mV(Hi-byte)
            // Transmitting DataBuf[4] = 36V ADC(Lo-byte);      Transmitting DataBuf[5] = 36V ADC(Hi-byte)
            // Transmitting DataBuf[6] = 36V real-mV(Lo-byte);  Transmitting DataBuf[7] = 36V real-mV(Hi-byte)
            // Transmitting DataBuf[8] = 48V ADC(Lo-byte);      Transmitting DataBuf[9] = 48V ADC(Hi-byte)
            // Transmitting DataBuf[10] = 48V real-mV(Lo-byte); Transmitting DataBuf[11] = 48V real-mV(Hi-byte)
//            case Cmd_Auto_Charger_Checking:
//                if(((G_Module_Function_Status & Start_Fast_Charger_Check)==0)&&
//                   ((G_Module_Function_Status & Start_Charger_Check)==0)){
//                    G_Module_Function_Status |= Start_Charger_Check;
//                    G_Module_Function_Status |= Charger_Check_Actived;
//                }
//                //_DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_All_Charger_Channel_Voltage,Comm_Temp_Transmitting_Data_Buffer, 12);
//                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Fast_Auto_Charger_Checking      (0xA7)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 24
            // channel ID set Off
            // Transmitting DataBuf[0] = 24V ADC(Lo-byte);      Transmitting DataBuf[1] = 24V ADC(Hi-byte)
            // Transmitting DataBuf[2] = 24V real-mV(Lo-byte);  Transmitting DataBuf[3] = 24V real-mV(Hi-byte)
            // Transmitting DataBuf[4] = 36V ADC(Lo-byte);      Transmitting DataBuf[5] = 36V ADC(Hi-byte)
            // Transmitting DataBuf[6] = 36V real-mV(Lo-byte);  Transmitting DataBuf[7] = 36V real-mV(Hi-byte)
            // Transmitting DataBuf[8] = 48V ADC(Lo-byte);      Transmitting DataBuf[9] = 48V ADC(Hi-byte)
            // Transmitting DataBuf[10] = 48V real-mV(Lo-byte); Transmitting DataBuf[11] = 48V real-mV(Hi-byte)
            // -----------------------------------------
            // channel ID set on
            // Transmitting DataBuf[0] = 24V ADC(Lo-byte);      Transmitting DataBuf[1] = 24V ADC(Hi-byte)
            // Transmitting DataBuf[2] = 24V real-mV(Lo-byte);  Transmitting DataBuf[3] = 24V real-mV(Hi-byte)
            // Transmitting DataBuf[4] = 36V ADC(Lo-byte);      Transmitting DataBuf[5] = 36V ADC(Hi-byte)
            // Transmitting DataBuf[6] = 36V real-mV(Lo-byte);  Transmitting DataBuf[7] = 36V real-mV(Hi-byte)
            // Transmitting DataBuf[8] = 48V ADC(Lo-byte);      Transmitting DataBuf[9] = 48V ADC(Hi-byte)
            // Transmitting DataBuf[10] = 48V real-mV(Lo-byte); Transmitting DataBuf[11] = 48V real-mV(Hi-byte)
//            case Cmd_Fast_Auto_Charger_Checking:
//                if(((G_Module_Function_Status & Start_Fast_Charger_Check)==0)&&
//                   ((G_Module_Function_Status & Start_Charger_Check)==0)){
//                    G_Module_Function_Status |= Start_Fast_Charger_Check;
//                    G_Module_Function_Status |= Charger_Check_Actived;
//                }
//                //_DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_All_Charger_Channel_Voltage,Comm_Temp_Transmitting_Data_Buffer, 12);
//                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_PACK_DSG_Voltage_Auto (0xA8)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = BatteryV ADC(Lo-byte);      Transmitting DataBuf[1] = BatteryV ADC(Hi-byte)
            // Transmitting DataBuf[2] = BatteryV real-mV(Lo-byte);  Transmitting DataBuf[3] = BatteryV real-mV(Hi-byte)
            case Cmd_Get_PACK_DSG_Voltage_Auto:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Pack_DSG_Vol_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_PACK_DSG_Voltage_Auto,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_PACK_CHG_Voltage_Auto (0xA9)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = BatteryV ADC(Lo-byte);      Transmitting DataBuf[1] = BatteryV ADC(Hi-byte)
            // Transmitting DataBuf[2] = BatteryV real-mV(Lo-byte);  Transmitting DataBuf[3] = BatteryV real-mV(Hi-byte)
            case Cmd_Get_PACK_CHG_Voltage_Auto:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status &= ~DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Pack_CHG_Vol_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_PACK_CHG_Voltage_Auto,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Channel_Raw_ADC (0xAA)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = 0:ADC Channel 0, 1:ADC Channel 1, ~ to ~ 7:ADC Channel 7
            //=====================================================================
            // Transmitting DataLenExpected = 2
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            case Cmd_Get_Channel_Raw_ADC:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1) && (receiving_Data_Packet.DataBuf[0] > 7)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Channel_Raw_ADC,&(gCdcTempUint8), 1);
                    break;
                }
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    gCdcTempUint8 = receiving_Data_Packet.DataBuf[0];
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Channels_ADC_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Channel_Raw_ADC,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Direct_PackDSG_Voltage (0xAB)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            // Transmitting DataBuf[2] = Real Values(Lo-byte);      Transmitting DataBuf[3] = Real Values(Hi-byte)
            case Cmd_Get_Direct_PackDSG_Voltage:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Pack_DSG_Vol_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_PackDSG_Voltage,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Direct_PackCHG_Voltage (0xAC)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            // Transmitting DataBuf[2] = Real Values(Lo-byte);      Transmitting DataBuf[3] = Real Values(Hi-byte)
            case Cmd_Get_Direct_PackCHG_Voltage:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Pack_CHG_Vol_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_PackCHG_Voltage,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Direct_Chger_24Voltage (0xAD)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            // Transmitting DataBuf[2] = Real Values(Lo-byte);      Transmitting DataBuf[3] = Real Values(Hi-byte)
            case Cmd_Get_Direct_Chger_24Voltage:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Charger_24V_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_Chger_24Voltage,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Direct_Chger_36Voltage (0xAE)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            // Transmitting DataBuf[2] = Real Values(Lo-byte);      Transmitting DataBuf[3] = Real Values(Hi-byte)
            case Cmd_Get_Direct_Chger_36Voltage:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Charger_36V_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_Chger_36Voltage,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Direct_Chger_48Voltage (0xAF)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            // Transmitting DataBuf[2] = Real Values(Lo-byte);      Transmitting DataBuf[3] = Real Values(Hi-byte)
            case Cmd_Get_Direct_Chger_48Voltage:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_Module_Function_Status |= Set_Charger_48V_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_Chger_48Voltage,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Direct_DSG_Current (0xB0)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            // Transmitting DataBuf[2] = Real Values(Lo-byte);      Transmitting DataBuf[3] = Real Values(Hi-byte)
            case Cmd_Get_Direct_DSG_Current:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_1st_Module_Function_Status |= Set_Dsg_Current_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_DSG_Current,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Direct_CHG_Current (0xB1)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 4
            // Transmitting DataBuf[0] = channel ADC(Lo-byte);      Transmitting DataBuf[1] = channel ADC(Hi-byte)
            // Transmitting DataBuf[2] = Real Values(Lo-byte);      Transmitting DataBuf[3] = Real Values(Hi-byte)
            case Cmd_Get_Direct_CHG_Current:
                if(((G_Module_Function_Status & (Set_Charger_24V_Measured_Processing + Set_Charger_36V_Measured_Processing + Set_Charger_48V_Measured_Processing)) == 0)&&
                    ((G_Module_Function_Status & (Set_Pack_DSG_Vol_Measured_Processing + Set_Pack_CHG_Vol_Measured_Processing + Set_Channels_ADC_Measured_Processing))== 0)){
                    G_Module_Function_Status &= ~Process_Set_Channel;
                    G_Module_Function_Status |= DirMeasuredProcessingViaADC_Ch;
                    G_1st_Module_Function_Status |= Set_Chg_Current_Measured_Processing;
                }else{
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Direct_CHG_Current,&(gCdcTempUint8), 1);
                }
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_Charger_Is_ID_Level (0xB2)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = NA
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = (1: use ID switch Voltage adapter)(0: No ID switch adapter);
            case Cmd_Get_Charger_Is_ID_Level:
                gCdcTempUint8 = _DUI_Get_Charger_ID_Status();
                if(gCdcTempUint8 == Chger_With_ID_Step){
                    //send data out via usb
                    gCdcTempUint8 = 1;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_Is_ID_Level,&(gCdcTempUint8), 1);
                }else{
                     //send data out via usb
                    gCdcTempUint8 = 0;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_Charger_Is_ID_Level,&(gCdcTempUint8), 1);
               }
                break;
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

            ///////////////////////////////////////////////////////////////////////
            // Cmd_I2C_Transmit_Data
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            case Cmd_I2C_Transmit_Data:
                gCdcTempUint8 = Respond_Error_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_I2C_Transmit_Data,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_I2C_Receive_Data
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            case Cmd_I2C_Receive_Data:
                gCdcTempUint8 = Respond_Error_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_I2C_Receive_Data,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_UART_RS485_Transmit_Data
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            case Cmd_UART_RS485_Transmit_Data:
                gCdcTempUint16 = receiving_Data_Packet.DataLenExpected_High;
                gCdcTempUint16 = (gCdcTempUint16 << 8) + receiving_Data_Packet.DataLenExpected_Low;
                _DUI_Communication_Send_Bytes(Uart_RS485_Module, &(receiving_Data_Packet.DataBuf[0]), gCdcTempUint16);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_RS485_Transmit_Data,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_UART_RS485_Receive_Data
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            case Cmd_UART_RS485_Receive_Data:
                // not here to do
                gCdcTempUint8 = Respond_Error_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_RS485_Receive_Data,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_One_Wire_Transmit_Data
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            case Cmd_One_Wire_Transmit_Data:
                // no function to do
                gCdcTempUint8 = Respond_Error_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_One_Wire_Transmit_Data,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_One_Wire_Receive_Data
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            case Cmd_One_Wire_Receive_Data:
                // not here to do
                gCdcTempUint8 = Respond_Error_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_One_Wire_Receive_Data,&(gCdcTempUint8), 1);
                break;
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    // Calibration Status cmd
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Cal_Set_Charger_24V_Channel_Offset  (0xD0)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = set Cal_Offset_Data
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Cal_Set_Charger_24V_Channel_Offset:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_Charger_24V_Channel_Offset,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(FA_24V_CAL_OFFSET_ADC_offset, &(receiving_Data_Packet.DataBuf[0]), 1);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_Charger_24V_Channel_Offset,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Cal_Set_Charger_36V_Channel_Offset  (0xD1)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = set Cal_Offset_Data
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Cal_Set_Charger_36V_Channel_Offset:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_Charger_36V_Channel_Offset,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(FA_36V_CAL_OFFSET_ADC_offset, &(receiving_Data_Packet.DataBuf[0]), 1);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_Charger_36V_Channel_Offset,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Cal_Set_Charger_48V_Channel_Offset  (0xD2)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = set Cal_Offset_Data
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Cal_Set_Charger_48V_Channel_Offset:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_Charger_48V_Channel_Offset,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(FA_48V_CAL_OFFSET_ADC_offset, &(receiving_Data_Packet.DataBuf[0]), 1);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_Charger_48V_Channel_Offset,&(gCdcTempUint8), 1);
                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_All_Calibration_Data           (0xD3)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 7
            // Transmitting DataBuf[0] = FA_24V_CAL_OFFSET_ADC;
            // Transmitting DataBuf[1] = FA_36V_CAL_OFFSET_ADC;
            // Transmitting DataBuf[2] = FA_48V_CAL_OFFSET_ADC;
            // Transmitting DataBuf[3] = FA_Pack_DSG_CAL_OFFSET_ADC;
            // Transmitting DataBuf[4] = FA_Pack_CHG_CAL_OFFSET_ADC;
            // Transmitting DataBuf[5] = FA_DSG_Current_CAL_OFFSET_ADC;
            // Transmitting DataBuf[6] = FA_CHG_Current_CAL_OFFSET_ADC;
            case Cmd_Get_All_Calibration_Data:
                gCdcTempUint8_ptr = (t_uint8 *)(Flash_segment_C + FA_24V_CAL_OFFSET_ADC_offset);
                for(gCdcTempUint16 = 0; gCdcTempUint16 < 5; gCdcTempUint16++){
                    Comm_Temp_Transmitting_Data_Buffer[gCdcTempUint16] = (*gCdcTempUint8_ptr++);
                }
                Comm_Temp_Transmitting_Data_Buffer[5] = FA_DSG_Current_CAL_OFFSET_ADC;
                Comm_Temp_Transmitting_Data_Buffer[6] = FA_CHG_Current_CAL_OFFSET_ADC;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_All_Calibration_Data, Comm_Temp_Transmitting_Data_Buffer, 7);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Get_All_Flash_Data           (0xD4)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 128
            // Transmitting DataBuf[0]~ Transmitting DataBuf[127] : flash data
            case Cmd_Get_All_Flash_Data:
                gCdcTempUint8_ptr = (t_uint8 *)(Flash_segment_C);
                for(gCdcTempUint16 = 0; gCdcTempUint16 < Flash_segment_Size; gCdcTempUint16++){
                    Comm_Temp_Transmitting_Data_Buffer[gCdcTempUint16] = (*gCdcTempUint8_ptr++);
                }
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Get_All_Flash_Data,Comm_Temp_Transmitting_Data_Buffer, Flash_segment_Size);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Set_Cal_Data_To_Flash               (0xD5)
            // receiving_Data_Packet.DataLenExpected = 2+n
            // receiving_Data_Packet.DataBuf[0] = Cal_Write_Data_Offset
            // receiving_Data_Packet.DataBuf[1] = Cal_Write_Data_Length(n)
            // receiving_Data_Packet.DataBuf[2 ~ 1+n] = Cal_Write_Data (Lo-byte first)
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Set_Cal_Data_To_Flash:
                gCdcTempUint16 = receiving_Data_Packet.DataLenExpected_High;
                gCdcTempUint16 = (gCdcTempUint16 << 8) + receiving_Data_Packet.DataLenExpected_Low;
                if((gCdcTempUint16 < 3)||((receiving_Data_Packet.DataBuf[1] + 2) != gCdcTempUint16)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_Cal_Data_To_Flash,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(receiving_Data_Packet.DataBuf[0], &(receiving_Data_Packet.DataBuf[2]), receiving_Data_Packet.DataBuf[1]);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Set_Cal_Data_To_Flash,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Cal_Set_PACK_DSG_Vol_CAL_ADC_offset  (0xD6)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = set Cal_Offset_Data
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Cal_Set_PACK_DSG_Vol_CAL_ADC_offset:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_PACK_DSG_Vol_CAL_ADC_offset,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(FA_Pack_DSG_CAL_OFFSET_ADC_offset, &(receiving_Data_Packet.DataBuf[0]), 1);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_PACK_DSG_Vol_CAL_ADC_offset,&(gCdcTempUint8), 1);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Cal_Set_PACK_CHG_Vol_CAL_ADC_offset  (0xD7)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = set Cal_Offset_Data
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Cal_Set_PACK_CHG_Vol_CAL_ADC_offset:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_PACK_CHG_Vol_CAL_ADC_offset,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(FA_Pack_CHG_CAL_OFFSET_ADC_offset, &(receiving_Data_Packet.DataBuf[0]), 1);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_PACK_CHG_Vol_CAL_ADC_offset,&(gCdcTempUint8), 1);
                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Cal_Set_DSG_Current_CAL_ADC_offset  (0xD8)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = set Cal_Offset_Data
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Cal_Set_DSG_Current_CAL_ADC_offset:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_DSG_Current_CAL_ADC_offset,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(FA_DSG_Current_CAL_OFFSET_ADC_offset, &(receiving_Data_Packet.DataBuf[0]), 1);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_DSG_Current_CAL_ADC_offset,&(gCdcTempUint8), 1);
                break;

            ///////////////////////////////////////////////////////////////////////
            // Cmd_Cal_Set_CHG_Current_CAL_ADC_offset  (0xD9)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = set Cal_Offset_Data
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_Cal_Set_CHG_Current_CAL_ADC_offset:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 1)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_CHG_Current_CAL_ADC_offset,&(gCdcTempUint8), 1);
                    break;
                }
                WriteDataToFlash(FA_CHG_Current_CAL_OFFSET_ADC_offset, &(receiving_Data_Packet.DataBuf[0]), 1);
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_CHG_Current_CAL_ADC_offset,&(gCdcTempUint8), 1);
                break;

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////
            // Cmd_For_Connect_Detection   (0xE1)
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            //case Cmd_For_Connect_Detection:
                //gCdcTempUint8 = Respond_Accept_Check_Code;
                //_DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_For_Connect_Detection,&(gCdcTempUint8), 1);
                //break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Test_Data_Send_Back   (0xE2)
            // receiving_Data_Packet.DataLenExpected
            // receiving_Data_Packet.DataBuf[n]
            case Cmd_Test_Data_Send_Back:
                gCdcTempUint16 = receiving_Data_Packet.DataLenExpected_High;
                gCdcTempUint16 = (gCdcTempUint16 << 8) + receiving_Data_Packet.DataLenExpected_Low;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Test_Data_Send_Back,&(receiving_Data_Packet.DataBuf[0]), gCdcTempUint16);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_Test_Get_All_Raw_ADC_Data           (0xE3)
            // receiving_Data_Packet.DataLenExpected = 1
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 12
            // Transmitting DataBuf[0] = ADC_Channel0(Lo-byte);  Transmitting DataBuf[1] = ADC_Channel0(Hi-byte)
            // Transmitting DataBuf[2] = ADC_Channel1(Lo-byte);  Transmitting DataBuf[3] = ADC_Channel1(Hi-byte)
            // Transmitting DataBuf[4] = ADC_Channel2(Lo-byte);  Transmitting DataBuf[5] = ADC_Channel2(Hi-byte)
            // Transmitting DataBuf[6] = ADC_Channel3(Lo-byte);  Transmitting DataBuf[7] = ADC_Channel3(Hi-byte)
            // Transmitting DataBuf[8] = ADC_Channel4(Lo-byte);  Transmitting DataBuf[9] = ADC_Channel4(Hi-byte)
            // Transmitting DataBuf[10] = ADC_Channel5(Lo-byte);  Transmitting DataBuf[11] = ADC_Channel5(Hi-byte)
//            case Cmd_Test_Get_All_Raw_ADC_Data:
//                gCdcTempUint16 = _Device_Get_ADC_Result(ADC_Channel0);
//                Comm_Temp_Transmitting_Data_Buffer[0] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[1] = gCdcTempUint16 >> 8;
//                gCdcTempUint16 = _Device_Get_ADC_Result(ADC_Channel1);
//                Comm_Temp_Transmitting_Data_Buffer[2] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[3] = gCdcTempUint16 >> 8;
//                gCdcTempUint16 = _Device_Get_ADC_Result(ADC_Channel2);
//                Comm_Temp_Transmitting_Data_Buffer[4] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[5] = gCdcTempUint16 >> 8;
//                gCdcTempUint16 = _Device_Get_ADC_Result(ADC_Channel3);
//                Comm_Temp_Transmitting_Data_Buffer[6] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[7] = gCdcTempUint16 >> 8;
//                gCdcTempUint16 = _Device_Get_ADC_Result(ADC_Channel4);
//                Comm_Temp_Transmitting_Data_Buffer[8] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[9] = gCdcTempUint16 >> 8;
//                gCdcTempUint16 = _Device_Get_ADC_Result(ADC_Channel5);
//                Comm_Temp_Transmitting_Data_Buffer[10] = gCdcTempUint16;
//                Comm_Temp_Transmitting_Data_Buffer[11] = gCdcTempUint16 >> 8;
//
//                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Test_Get_All_Raw_ADC_Data,Comm_Temp_Transmitting_Data_Buffer, 12);
//                break;


            ///////////////////////////////////////////////////////////////////////
            // Cmd_HW_FW_Version           (0xE5)
            // receiving_Data_Packet.DataLenExpected = 0
            // receiving_Data_Packet.DataBuf[0] = N/A
            //=====================================================================
            // Transmitting DataLenExpected = 5
            // Transmitting DataBuf[0] = FA_VERSION;
            // Transmitting DataBuf[1] = FA_MINOR_VERSION;
            // Transmitting DataBuf[2] = FA_EEPROM_VERSION;
            // Transmitting DataBuf[3] = FA_RESERVED_VERSION;
            // Transmitting DataBuf[4] = FA_HW_Version;
            // Transmitting DataBuf[5] = FA_HW_MINOR_Version;
            case Cmd_FW_HW_Version:
                Comm_Temp_Transmitting_Data_Buffer[0] = FA_VERSION;
                Comm_Temp_Transmitting_Data_Buffer[1] = FA_MINOR_VERSION;
                Comm_Temp_Transmitting_Data_Buffer[2] = FA_EEPROM_VERSION;
                Comm_Temp_Transmitting_Data_Buffer[3] = FA_RESERVED_VERSION;
                Comm_Temp_Transmitting_Data_Buffer[4] = FA_HW_Version;
                Comm_Temp_Transmitting_Data_Buffer[5] = FA_HW_MINOR_Version;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_FW_HW_Version,Comm_Temp_Transmitting_Data_Buffer, 6);
                break;
            ///////////////////////////////////////////////////////////////////////
            // Cmd_SetDetectCharger_DelayCycle           (0xE6)
            // receiving_Data_Packet.DataLenExpected = 2
            // receiving_Data_Packet.DataBuf[0] = Delay Cycles (Lo bytes)
            // receiving_Data_Packet.DataBuf[1] = Delay Cycles (Hi bytes)
            //=====================================================================
            // Transmitting DataLenExpected = 1
            // Transmitting DataBuf[0] = Respond_Accept_Check_Code or Respond_Error_Check_Code
            case Cmd_SetDetectCharger_DelayCycle:
                if((receiving_Data_Packet.DataLenExpected_High == 0) && (receiving_Data_Packet.DataLenExpected_Low != 2)){
                    gCdcTempUint8 = Respond_Error_Check_Code;
                    _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Cal_Set_PACK_CHG_Vol_CAL_ADC_offset,&(gCdcTempUint8), 1);
                    break;
                }
                G_Temp_1_DelayCycles = receiving_Data_Packet.DataBuf[1];
                G_Temp_1_DelayCycles = (gCdcTempUint16 << 8) + receiving_Data_Packet.DataBuf[0];
                gCdcTempUint8 = Respond_Accept_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_SetDetectCharger_DelayCycle,&(gCdcTempUint8), 1);
                break;

            default:
                gCdcTempUint8 = Respond_Error_Check_Code;
                _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_Error_Cmd,&(gCdcTempUint8), 1);
                break;
        }


        g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Found;
        g_Usb_Cdc_Status_FLAG &= ~CDC_RX_Packet_Check_True;
    }//if((g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Found) && (g_Usb_Cdc_Status_FLAG & CDC_RX_Packet_Check_True)){
    ///////////////////////////////////////////////////////////////////////////////////
    //Check_UART_RS485_Receive_Data Ready, and send out
    if(_DUI_Is_Comm_Module_Receiving_Data_Ready(Uart_RS485_Module) == UART_RECEIVING_DATA_READY){
        _DUI_Get_Receiving_Data_To_Array(Uart_RS485_Module, Comm_Temp_Transmitting_Data_Buffer, &gCdcTempUint16, CDC_Transmitting_Max_Data_Length);
        _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_UART_RS485_Receive_Data, Comm_Temp_Transmitting_Data_Buffer, gCdcTempUint16);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    //Check_One_Wire_Receive_Data Ready, and send out
    if(_DUI_Is_Comm_Module_Receiving_Data_Ready(One_Wire_Module) == UART_RECEIVING_DATA_READY){
        _DUI_Get_Receiving_Data_To_Array(One_Wire_Module, Comm_Temp_Transmitting_Data_Buffer, &gCdcTempUint16, CDC_Transmitting_Max_Data_Length);
        _DUI_CDC_Transmitting_Data_With_USB_Protocol_Packet(Cmd_One_Wire_Receive_Data, Comm_Temp_Transmitting_Data_Buffer, gCdcTempUint16);
    }

}

//void _Device_Set_USB_Receive_From_PC_Calling_Function(void (*calling_fun)(t_uint8* receivedBytesBuffer, t_uint16 receivingSize)){
//
//}
//
///*----------------------------------------------------------------------------+
// | Main Routine                                                                |
// +----------------------------------------------------------------------------*/
//void main (void)
//{
//
//    WDTCTL = WDTPW + WDTHOLD;                                   //Stop watchdog timer
//
//    Init_Ports();                                               //Init ports (do first ports because clocks do change ports)
//
//    _Device_Set_Power_Management_Module();
//
//    //_Device_Clock_Source_Set_Out_To_Pin();
//    _Device_Init_Clock_Module();
//    //Init_Clock_By_TI_For_USB();
//    Init_USB_Config();
//    _Device_Init_Timer_A();
//    _Device_Enable_Timer_A();
//    //_Device_Set_TimerA_Interrupt_Timer_Calling_Function(0, LED1_Test);
//    _Device_Set_TimerA_Interrupt_Timer_Calling_Function(0, PAD2_LED_Test);
//    _Device_Set_TimerA_Interrupt_Timer_Calling_Function(1, Polling_For_USB_Connection_Status);
//    _Device_Set_TimerA_Interrupt_Timer_Calling_Function(2, PAD1_LED_Test);
//    _Device_Init_I2C_Master_Module();
//    _Device_Uart_Module_Enable();
//    _Device_Set_Uart_Interrupt_Receive_Calling_Function(Uart_receive_Test);
//
//    __enable_interrupt();                           //Enable interrupts globally
//    while(1){
//        //Polling_For_USB_Connection_Status();
//                _NOP();
//                _NOP();
//                _NOP();
//                _NOP();
//
//            //Delay
//            //GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN4 );
//            //GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN4 );
//            __delay_cycles(100);    //12us at 8MHz
//            __delay_cycles(1000);   //130us at 8MHz
//            __delay_cycles(2000);   //260us at 8MHz
//            __delay_cycles(3000);   //390us at 8MHz
//            __delay_cycles(5000);   //634us at 8MHz
//            __delay_cycles(10000);   //1260us at 8MHz
//
//
//            __delay_cycles(1000000);
//
//            sst = _Device_I2C_Master_Write_Bytes(TransmitCMD,1);
//            sst = _Device_I2C_Master_Read_Bytes(TransmitData,2);
//            __delay_cycles(1000000);
//            sst = _Device_I2C_Master_Write_Bytes(TransmitCMD,1);
//            //_Device_I2C_Master_Write_Bytes(&(TransmitCMD[1]),1);
//            sst = _Device_I2C_Master_Read_Bytes(TransmitData,2);
//        //_Device_I2C_Master_Write();
//        //_Device_I2C_Master_Read();
//            //LED2_Test();
//    }

//    SetVCore(3);
//    Init_Clock();                                               //Init clocks
//
//    USB_init();                 //Init USB
//
//    //Enable various USB event handling routines
//
//    USB_setEnabledEvents(
//        kUSB_VbusOnEvent + kUSB_VbusOffEvent + kUSB_receiveCompletedEvent
//        + kUSB_dataReceivedEvent + kUSB_UsbSuspendEvent + kUSB_UsbResumeEvent +
//        kUSB_UsbResetEvent);
//
//    //See if we're already attached physically to USB, and if so, connect to it
//    //Normally applications don't invoke the event handlers, but this is an exception.
//    if (USB_connectionInfo() & kUSB_vbusPresent){
//        if (USB_enable() == kUSB_succeed){
//            USB_reset();
//            USB_connect();
//        }
//    }
//
//    __enable_interrupt();                           //Enable interrupts globally
//    while (1)
//    {
//        BYTE ReceiveError = 0, SendError = 0;
//        WORD count;
//        //Check the USB state and loop accordingly
//        switch (USB_connectionState())
//        {
//            case ST_USB_DISCONNECTED:
//                __bis_SR_register(LPM3_bits + GIE);                             //Enter LPM3 until USB is connected
//                __no_operation();
//                break;
//
//            case ST_USB_CONNECTED_NO_ENUM:
//                break;
//
//            case ST_ENUM_ACTIVE:
//                __bis_SR_register(LPM0_bits + GIE);                             //Enter LPM0 until awakened by an event handler
//
//                                                                                //Exit LPM because of a data-receive event, and
//                                                                                //fetch the received data
//                if (bCDCDataReceived_event){
//
//                    bCDCDataReceived_event = FALSE;                             //Clear flag early -- just in case execution breaks
//                                                                                //below because of an error
//                    count = cdcReceiveDataInBuffer((BYTE*)dataBuffer,
//                        BUFFER_SIZE,
//                        CDC0_INTFNUM);                                          //Count has the number of bytes received into
//                                                                                //dataBuffer
//                    if (cdcSendDataInBackground((BYTE*)dataBuffer,count,CDC0_INTFNUM,1)){  	//Echo is back to the host
//                        SendError = 0x01;                                       			//Something went wrong -- exit
//                        break;
//                    }
//                }
//                break;
//
//            case ST_ENUM_SUSPENDED:
//                __bis_SR_register(LPM3_bits + GIE);             //Enter LPM3 until a resume or VBUS-off event
//                break;
//
//            case ST_ENUM_IN_PROGRESS:
//                break;
//
//            case ST_NOENUM_SUSPENDED:
//                __bis_SR_register(LPM3_bits + GIE);
//                break;
//
//            case ST_ERROR:
//                _NOP();
//                break;
//
//            default:;
//        }
//        if (ReceiveError || SendError){
//            //TO DO: User can place code here to handle error
//        }
//    }  //while(1)
//}                               //main()
///*
// * ======== Init_Clock ========
// */
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
///*
// * ======== Init_Ports ========
// */
//VOID Init_Ports (VOID)
//{
//    //Initialization of ports (all unused pins as outputs with low-level
//    P1OUT = 0x00;
//    P1DIR = 0xFF;
//	P2OUT = 0x00;
//    P2DIR = 0xFF;
//    P3OUT = 0x00;
//    P3DIR = 0xFF;
//    P4OUT = 0x00;
//    P4DIR = 0xFF;
//    P5OUT = 0x00;
//    P5DIR = 0xFF;
//    P6OUT = 0x00;
//    P6DIR = 0xFF;
//	P7OUT = 0x00;
//    P7DIR = 0xFF;
//    P8OUT = 0x00;
//    P8DIR = 0xFF;
//    #if defined (__MSP430F563x_F663x)
//		P9OUT = 0x00;
//		P9DIR = 0xFF;
//    #endif
//}
//
///*
// * ======== UNMI_ISR ========
// */
//#pragma vector = UNMI_VECTOR
//__interrupt VOID UNMI_ISR (VOID)
//{
//    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG ))
//    {
//        case SYSUNIV_NONE:
//            __no_operation();
//            break;
//        case SYSUNIV_NMIIFG:
//            __no_operation();
//            break;
//        case SYSUNIV_OFIFG:
//            UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG); //Clear OSC flaut Flags fault flags
//            SFRIFG1 &= ~OFIFG;                          //Clear OFIFG fault flag
//            break;
//        case SYSUNIV_ACCVIFG:
//            __no_operation();
//            break;
//        case SYSUNIV_BUSIFG:
//            SYSBERRIV = 0;                                      //clear bus error flag
//            USB_disable();                                      //Disable
//    }
//}
//


