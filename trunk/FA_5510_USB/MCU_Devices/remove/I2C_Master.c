
/**
  ******************************************************************************
  * @file    I2C_Master.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   I2C_Master setting
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
#include "ucs.h"
//#include "wdt_a.h"
#include "gpio.h"
#include "usci_b_i2c.h"

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
//#define SLAVE_ADDRESS_7Bit          0x0b //without last R/W bit
//
//#define I2C_MASTER_SDA_PORT             GPIO_PORT_P3
//#define I2C_MASTER_SDA_PIN              GPIO_PIN0
//#define I2C_MASTER_SCL_PORT             GPIO_PORT_P3
//#define I2C_MASTER_SCL_PIN              GPIO_PIN1
//
//#define I2C_USCI_B_BASEADDRESS          USCI_B0_BASE
//
////#define I2C_DATA_CLOCK_RATE             USCI_B_I2C_SET_DATA_RATE_400KBPS
//#define I2C_DATA_CLOCK_RATE             USCI_B_I2C_SET_DATA_RATE_100KBPS
//
////#define I2C_WhileLoopTimeOut            2000    //transmit time out, for 64 bytes transmitting at 100kbps
//#define I2C_Transmit_TimeOut            2000    //transmit time out, for 64 bytes transmitting at 100kbps (8ms)
//
//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//==============================================================================
// Private variables
//==============================================================================

///////////////////////////////////////////////////////////////////////////////
//I2C_Trace and functions
//////////////////////////////////////////////////////////////////////////
#define I2C_Trace_buffer          100
enum I2C_Trace
{
  FunW,
  FunR,
  FunStart,
  FunStop,
  INT_NACK,
  INT_Start,
  INT_Stop,
  INT_TX,
  INT_RX,
  INT_TX_count,
  INT_TX_count1,
  INT_TX_end,
  INT_RX_count,
  INT_RX_count1,
  INT_RX_end,
  BusBusyLoop,
  BusBusyFail,
  pos1,
  pos2,
  pos3,
  pos4,
  pos5,
  pos6,
  pos7,
  pos8,
  pos9,
  pos10
};
unsigned char Trace_status[I2C_Trace_buffer];
unsigned int Trace_index;
void set_trace(char status)
{
  if(Trace_index >=I2C_Trace_buffer){
    Trace_index = I2C_Trace_buffer - 1;
  }
  Trace_status[Trace_index] = status;
  Trace_index++;
}
void clear_trace()
{
  int i;
  for(i=0; i < I2C_Trace_buffer; i++)
  {
    Trace_status[i] = 0;
  }
  Trace_index = 0;
}
///////////////////////////////////////////////////////////////////////////

//
uint8_t *transmitData;
uint8_t transmitLength;
uint8_t transmitCounter;
uint8_t *receiveData;
uint8_t receiveLength;
uint8_t receiveCounter;
t_uint16 whileLoopTimeOutCount;
//==============================================================================
// Private function prototypes
//==============================================================================


//==============================================================================
// Private functions
//==============================================================================
void _Device_Init_I2C_IO_Port_As_Input(void)
{
    //Enable Pin internal resistance as pull-Up resistance
    GPIO_setAsInputPinWithPullUpresistor(
        I2C_MASTER_SDA_PORT,
        I2C_MASTER_SDA_PIN
        );
    //Enable Pin internal resistance as pull-Up resistance
    GPIO_setAsInputPinWithPullUpresistor(
        I2C_MASTER_SCL_PORT,
        I2C_MASTER_SCL_PIN
        );
}
unsigned char _Device_get_SDA_Pin_Status(void)
{
    if(GPIO_getInputPinValue(I2C_MASTER_SDA_PORT, I2C_MASTER_SDA_PIN) == GPIO_INPUT_PIN_HIGH){
        return IO_INPUT_HIGH;
    }else{
        return IO_INPUT_LOW;
    }
}
unsigned char _Device_get_SCL_Pin_Status(void)
{
    if(GPIO_getInputPinValue(I2C_MASTER_SCL_PORT, I2C_MASTER_SCL_PIN) == GPIO_INPUT_PIN_HIGH){
        return IO_INPUT_HIGH;
    }else{
        return IO_INPUT_LOW;
    }
}
#if 0
/**
  * @brief  Configure USCB0 I2C peripheral
  * @param  None
  * @retval None
  */
void _Device_Init_I2C_Master_Module(void)
{

    //Assign I2C SDA pins
    GPIO_setAsPeripheralModuleFunctionInputPin( I2C_MASTER_SDA_PORT, I2C_MASTER_SDA_PIN );
    //Assign I2C SCL pins
    GPIO_setAsPeripheralModuleFunctionInputPin( I2C_MASTER_SCL_PORT, I2C_MASTER_SCL_PIN );

    //Initialize Master
    USCI_B_I2C_masterInit(I2C_USCI_B_BASEADDRESS,
        USCI_B_I2C_CLOCKSOURCE_SMCLK,
        UCS_getSMCLK(UCS_BASE),
        I2C_DATA_CLOCK_RATE
        );

    //Specify slave address
    USCI_B_I2C_setSlaveAddress(I2C_USCI_B_BASEADDRESS, SLAVE_ADDRESS_7Bit );

    //Set Transmit mode
    USCI_B_I2C_setMode(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_MODE );

    //Enable I2C Module to start operations
    USCI_B_I2C_enable(I2C_USCI_B_BASEADDRESS);

    //For debugger
    __no_operation();

    whileLoopTimeOutCount = 0;
    transmitLength = 0;
    transmitCounter = 0;
    receiveLength = 0;
    receiveCounter = 0;
}

void _Device_Set_I2C_Slave_Address(t_uint8 slaveAddress){
    //Specify slave address
    USCI_B_I2C_setSlaveAddress(I2C_USCI_B_BASEADDRESS, slaveAddress );
    //Set Transmit mode
    USCI_B_I2C_setMode(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_MODE );
    //Enable I2C Module to start operations
    USCI_B_I2C_enable(I2C_USCI_B_BASEADDRESS);
    //For debugger
    __no_operation();
}


t_uint8 _Device_I2C_Master_Write_Bytes(t_uint8 *writeData, t_uint8 numWrite){

    t_uint8 status;

    clear_trace();
    set_trace(FunW);

    //Set Transmit mode
    USCI_B_I2C_setMode(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_MODE );

    //Enable transmit Interrupt
    USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_INTERRUPT );
    USCI_B_I2C_enableInterrupt(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_INTERRUPT );
    //Delay between each transaction
    __delay_cycles(1000);   //130us at 8MHz

    transmitData = writeData;
    transmitLength = numWrite;
    //Load TX byte counter
    transmitCounter = 1;

    //Initiate start and send first character with time out
    status = USCI_B_I2C_masterMultiByteSendStartWithTimeout(I2C_USCI_B_BASEADDRESS, transmitData[0], I2C_Transmit_TimeOut);
    set_trace(FunStart);

    if(status == STATUS_FAIL){
        //Initiate stop only
        USCI_B_I2C_masterMultiByteSendStop(I2C_USCI_B_BASEADDRESS);
        //Clear master interrupt status
        USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_INTERRUPT);
        return Func_Failure;
    }
    //Enter LPM0 with interrupts enabled
    //__bis_SR_register(LPM0_bits + GIE);
    __no_operation();

    //Delay until transmission completes
    whileLoopTimeOutCount = 0;
    while (USCI_B_I2C_isBusBusy(I2C_USCI_B_BASEADDRESS)){
        __no_operation();
        set_trace(BusBusyLoop);
        if(whileLoopTimeOutCount >= I2C_Transmit_TimeOut){
            set_trace(BusBusyFail);
            //Initiate stop only
            USCI_B_I2C_masterMultiByteSendStop(I2C_USCI_B_BASEADDRESS);
            //Clear master interrupt status
            USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_INTERRUPT);
            return Func_Failure;
            //break;
        }
        whileLoopTimeOutCount++;
    }

    set_trace(FunStop);
    return Func_Success;
}

t_uint8 _Device_I2C_Master_Read_Bytes(t_uint8 *readData, t_uint8 numRead){

    set_trace(FunR);
    //Set Master in receive mode
    USCI_B_I2C_setMode(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_RECEIVE_MODE );

    //Enable I2C Module to start operations
    //USCI_B_I2C_enable(I2C_USCI_B_BASEADDRESS);

    //Enable master Receive interrupt
	USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_RECEIVE_INTERRUPT );
    USCI_B_I2C_enableInterrupt(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_RECEIVE_INTERRUPT );


    USCI_B_I2C_masterMultiByteReceiveStart (I2C_USCI_B_BASEADDRESS);
    set_trace(FunStart);


    receiveData = readData;
    receiveLength = numRead;
    receiveCounter = 0;
    whileLoopTimeOutCount = 0;

    //Enter LPM0 with interrupts enabled
    //__bis_SR_register(LPM0_bits + GIE);
    __no_operation();

    //Delay until receive completes
    whileLoopTimeOutCount = 0;
    while (USCI_B_I2C_isBusBusy(I2C_USCI_B_BASEADDRESS)){
        set_trace(BusBusyLoop);
        __no_operation();
        if(whileLoopTimeOutCount >= I2C_Transmit_TimeOut){
            set_trace(BusBusyFail);
            //Initiate stop only
            USCI_B_I2C_masterMultiByteReceiveStop(I2C_USCI_B_BASEADDRESS);
            //Clear master interrupt status
            USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_RECEIVE_INTERRUPT );
            return Func_Failure;
            //break;
        }
        whileLoopTimeOutCount++;
    }

    set_trace(FunStop);
    return Func_Success;
}



//void _Device_I2C_Master_Read(){
//
//
//    //Set Master in receive mode
//    USCI_B_I2C_setMode(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_RECEIVE_MODE );
//
//    //Enable I2C Module to start operations
//    USCI_B_I2C_enable(I2C_USCI_B_BASEADDRESS);
//
//    //Enable master Receive interrupt
//	USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS,
//        USCI_B_I2C_RECEIVE_INTERRUPT
//        );
//    USCI_B_I2C_enableInterrupt(I2C_USCI_B_BASEADDRESS,
//        USCI_B_I2C_RECEIVE_INTERRUPT
//        );
//
//    //while (1)
//    {
//        //Initiate command to receive a single character from Slave
//        USCI_B_I2C_masterSingleReceiveStart(I2C_USCI_B_BASEADDRESS);
//
//        //Enter low power mode 0 with interrupts enabled.
//        //Wait until character is received.
//        //__bis_SR_register(LPM0_bits + GIE);
//        __no_operation();
//    }
//}
//******************************************************************************
//
//This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
uint8_t receivedData;
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR (void)
{
    switch (__even_in_range(UCB0IV,12)){

        case USCI_NONE:                 // Vector  0: No interrupts
            break;
        case USCI_I2C_UCALIFG:          // Vector  2: ALIFG
            break;
        case USCI_I2C_UCNACKIFG:        // Vector  4: NACKIFG
            set_trace(INT_NACK);
            break;
        case USCI_I2C_UCSTTIFG:         // Vector  6: STTIFG
            set_trace(INT_Start);
            break;
        case USCI_I2C_UCSTPIFG:         // Vector  8: STPIFG
            set_trace(INT_Stop);
            break;
        case USCI_I2C_UCRXIFG:          // Vector 10: RXIFG

            set_trace(INT_RX);
                //Grab data from data register
                receiveData[receiveCounter] = USCI_B_I2C_masterMultiByteReceiveNext( I2C_USCI_B_BASEADDRESS);
                //Increment RX byte counter
                receiveCounter++;
            if(receiveLength == 1){
                set_trace(INT_RX_count);
                USCI_B_I2C_masterMultiByteReceiveStop(I2C_USCI_B_BASEADDRESS);
                //Clear master interrupt status
                USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_RECEIVE_INTERRUPT );
            }else if(receiveLength == 0){
                set_trace(INT_RX_count1);
                USCI_B_I2C_masterMultiByteReceiveStop(I2C_USCI_B_BASEADDRESS);
                //Clear master interrupt status
                USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_RECEIVE_INTERRUPT );
                return;
            }
            receiveLength --;

//            if(receiveCounter < receiveLength){
//                //Grab data from data register
//                receiveData[receiveCounter] = USCI_B_I2C_masterMultiByteReceiveNext( I2C_USCI_B_BASEADDRESS);
//                //Increment RX byte counter
//                receiveCounter++;
//            }else{
//                USCI_B_I2C_masterMultiByteReceiveStop(I2C_USCI_B_BASEADDRESS);
//            }
            //Exit low power mode 0 and disable GIE on interrupt exit
            //__bic_SR_register_on_exit(LPM0_bits + GIE);
            break;
        case USCI_I2C_UCTXIFG:          // Vector 12: TXIFG
            set_trace(INT_TX);
            //Check TX byte counter
            if (transmitCounter < transmitLength){
                set_trace(INT_TX_count);
                //Initiate send of character from Master to Slave
                USCI_B_I2C_masterMultiByteSendNext(I2C_USCI_B_BASEADDRESS, transmitData[transmitCounter] );
                //Increment TX byte counter
                transmitCounter++;
            } else   {
                set_trace(INT_TX_count1);
                //Initiate stop only
                USCI_B_I2C_masterMultiByteSendStop(I2C_USCI_B_BASEADDRESS);
                //Clear master interrupt status
                USCI_B_I2C_clearInterruptFlag(I2C_USCI_B_BASEADDRESS, USCI_B_I2C_TRANSMIT_INTERRUPT);

                //Exit LPM0 on interrupt return
                //__bic_SR_register_on_exit(LPM0_bits);
            }

        //Send single byte data.
        //USCI_B_I2C_masterSendSingleByte(I2C_USCI_B_BASEADDRESS, transmitData );

            break;
        default:
            break;
    }
}

#endif