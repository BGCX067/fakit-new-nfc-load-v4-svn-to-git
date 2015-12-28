/**
  ******************************************************************************
  * @file    USB_CDC_Config.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   USB_CDC_Config
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

#include "../USB_config/descriptors.h"

#include "../USB_API/USB_Common/device.h"
#include "../USB_API/USB_Common/types.h"               //Basic Type declarations
#include "../USB_API/USB_Common/usb.h"                 //USB-specific functions

//#include "../F5xx_F6xx_Core_Lib/HAL_UCS.h"
//#include "../F5xx_F6xx_Core_Lib/HAL_PMM.h"

#include "../USB_API/USB_CDC_API/UsbCdc.h"
#include "usbConstructs.h"
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
#define usb_RECEIVE_MAX_BUFFER_SIZE     64

//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//==============================================================================
// Private variables
//==============================================================================
unsigned char usb_ReceiveDataBuffer[usb_RECEIVE_MAX_BUFFER_SIZE] = "";
//WORD count;

//Global flags set by events
volatile BYTE bCDCDataReceived_event = FALSE;   //Indicates data has been received without an open rcv operation

//#define MAX_STR_LENGTH 64
//char wholeString[MAX_STR_LENGTH] = "";          //The entire input string from the last 'return'

//==============================================================================
// Private function prototypes
//==============================================================================
void (*USB_CDC_ReceiveData_ptr_fuc)(t_uint8* receivedBytesBuffer, t_uint16 receivingSize);
void Empty_USB_CDC_ReceiveData_fun(t_uint8* receivedBytesBuffer, t_uint16 receivingSize){}


//==============================================================================
// Private functions
//==============================================================================
/*
 * ======== Init_USB For CDC ========
 */
void _Device_Init_USB_Config (void)
{
    //Initialization of USB module
    USB_init();                 //Init USB

    //Enable various USB event handling routines
    USB_setEnabledEvents(
        kUSB_VbusOnEvent + kUSB_VbusOffEvent + kUSB_receiveCompletedEvent
        + kUSB_dataReceivedEvent + kUSB_UsbSuspendEvent + kUSB_UsbResumeEvent +
        kUSB_UsbResetEvent);

    //See if we're already attached physically to USB, and if so, connect to it
    //Normally applications don't invoke the event handlers, but this is an exception.
    if (USB_connectionInfo() & kUSB_vbusPresent){
        USB_handleVbusOnEvent();
    }

    USB_CDC_ReceiveData_ptr_fuc = Empty_USB_CDC_ReceiveData_fun;
}

void _Device_Set_USB_Receive_From_PC_Calling_Function(void (*calling_fun)(t_uint8* receivedBytesBuffer, t_uint16 receivingSize)){
    USB_CDC_ReceiveData_ptr_fuc = calling_fun;
}

t_uint8 _Device_USB_Send_Bytes_To_PC(unsigned char *sendByte, unsigned int length){
    if (cdcSendDataInBackground(sendByte,length,CDC0_INTFNUM,1)){  	//Echo is back to the host
        return Func_Failure;                                    	//Something went wrong -- exit
    }
    return Func_Success;
}

t_uint8 _Device_Polling_For_USB_Connection_Status(){

        BYTE ReceiveError = 0, SendError = 0;
        WORD count;

        t_uint8 status;

        status = USB_Status_USB_DISCONNECTED;

        //Check the USB state and loop accordingly
        switch (USB_connectionState())
        {
            case ST_USB_DISCONNECTED:
                //__bis_SR_register(LPM3_bits + GIE);                             //Enter LPM3 until USB is connected
                __no_operation();
                status = USB_Status_USB_DISCONNECTED;
                break;

            case ST_USB_CONNECTED_NO_ENUM:
                status = USB_Status_USB_CONNECTED_NO_ENUM;
                break;

            case ST_ENUM_ACTIVE:
                //__bis_SR_register(LPM0_bits + GIE);                             //Enter LPM0 until awakened by an event handler

                                                                                //Exit LPM because of a data-receive event, and
                                                                                //fetch the received data
                if (bCDCDataReceived_event){

                    bCDCDataReceived_event = FALSE;                             //Clear flag early -- just in case execution breaks
                                                                                //below because of an error

                    //Count has the number of bytes received into
                    count = cdcReceiveDataInBuffer((BYTE*)usb_ReceiveDataBuffer, usb_RECEIVE_MAX_BUFFER_SIZE, CDC0_INTFNUM);
                    USB_CDC_ReceiveData_ptr_fuc(usb_ReceiveDataBuffer, count);

                                                                                //dataBuffer
//                    if (cdcSendDataInBackground((BYTE*)dataBuffer,count,CDC0_INTFNUM,1)){  	//Echo is back to the host
//                        SendError = 0x01;                                       			//Something went wrong -- exit
//                        break;
//                    }
                }
                status = USB_Status_ENUM_ACTIVE;
                break;

            case ST_ENUM_SUSPENDED:
                //__bis_SR_register(LPM3_bits + GIE);             //Enter LPM3 until a resume or VBUS-off event
                status = USB_Status_ENUM_SUSPENDED;
                break;

            case ST_ENUM_IN_PROGRESS:
                status = USB_Status_ENUM_IN_PROGRESS;
                break;

            case ST_NOENUM_SUSPENDED:
                //__bis_SR_register(LPM3_bits + GIE);
                status = USB_Status_NOENUM_SUSPENDED;
                break;

            case ST_ERROR:
                _NOP();
                status = USB_Status_ERROR;
                break;

            default:
                //status = USB_Status_ERROR;
                break;
        }
        if (ReceiveError || SendError){
            //TO DO: User can place code here to handle error
        }
        return status;

}//void Polling_For_USB_Connection_Status(){

