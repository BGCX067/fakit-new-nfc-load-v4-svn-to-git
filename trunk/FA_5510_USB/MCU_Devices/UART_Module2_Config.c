/**
  ******************************************************************************
  * @file    UART_Modul 2 Config.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   UART_Modul 2  setting
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
#include "ucs.h"
#include "usci_a_uart.h"
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


//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//==============================================================================
// Private variables
//==============================================================================
static unsigned int SendingWhileTimeOutCount;

//#define Receiving_Max_Length    10
//__IO unsigned int UART_Receiving_Data_Index;
//unsigned char UART_Receiving_Data[Receiving_Max_Length ];

//==============================================================================
// Private function prototypes
//==============================================================================
static void (*Interrupt_UART_ReceiveData_ptr_fuc)(__IO t_uint8 receivedByte);
static void Empty_UART_fun(__IO t_uint8 receivedByte){}


//==============================================================================
// Private functions
//==============================================================================
//static void clear_Buffer(){
//    t_uint16 i;
//    for(i = 0; i < Receiving_Max_Length; i++){
//        UART_Receiving_Data[i] = 0;
//    }
//    UART_Receiving_Data_Index = 0;
//}
//static void set_Value_To_Receive_Buffer( __IO t_uint8 value){
//    t_uint8 temp;
//    if(UART_Receiving_Data_Index >= Receiving_Max_Length){
//        UART_Receiving_Data_Index = 0;
//    }
//    temp = value;
//    UART_Receiving_Data[UART_Receiving_Data_Index] = temp;
//    UART_Receiving_Data_Index++;
//}
//static void Empty_UART_fun(__IO t_uint8 receivedByte){
//    set_Value_To_Receive_Buffer(receivedByte);
//
//}



t_uint8 _Device_Uart_Module_2_Enable(t_uint32 baud_rate){

    //setUSCI_X TXD
    //GPIO_setAsPeripheralModuleFunctionInputPin( USART_Module_2_TX_PORT, USART_Module_2_TX_PIN );
    //setUSCI_X RXD
    //GPIO_setAsPeripheralModuleFunctionInputPin( USART_Module_2_RX_PORT, USART_Module_2_RX_PIN );

    P3DIR &= ~(BIT3+BIT4); //set as input
    P3REN |= BIT3+BIT4;
    P3OUT |= BIT3+BIT4; //Input with pullup resistor
    P3SEL |= BIT3+BIT4;                        // P3.3,4 = USCI_A0 TXD/RXD
    // below setting could not work, i don't know why
    //GPIO_setAsInputPinWithPullUpresistor(USART_Module_2_RX_PORT, USART_Module_2_RX_PIN);
    //GPIO_setAsInputPinWithPullUpresistor(USART_Module_2_TX_PORT, USART_Module_2_TX_PIN);

    //set charger id sense pin as input pins.
    //GPIO_setAsInputPin(USART_Module_2_RX_PORT, USART_Module_2_RX_PIN);
    //GPIO_setAsInputPinWithPullDownresistor(USART_Module_2_RX_PORT, USART_Module_2_RX_PIN);



    if ( STATUS_FAIL == USCI_A_UART_init(UART_Module_2_USCI_A_BASEADDRESS,
             USCI_A_UART_CLOCKSOURCE_SMCLK,
             UCS_getSMCLK(UCS_BASE),
             baud_rate,
             USCI_A_UART_NO_PARITY,
             USCI_A_UART_LSB_FIRST,
             USCI_A_UART_ONE_STOP_BIT,
             USCI_A_UART_MODE,
             USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION )){
        return Func_Failure;
    }

    //Enable UART module for operation
    USCI_A_UART_enable(UART_Module_2_USCI_A_BASEADDRESS);

    //Enable Receive Interrupt
	USCI_A_UART_clearInterruptFlag(UART_Module_2_USCI_A_BASEADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(UART_Module_2_USCI_A_BASEADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);




//  P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
//  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
//  UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
//  UCA0BR0 = 0x03;                           // 32kHz/9600=3.41 (see User's Guide)
//  UCA0BR1 = 0x00;                           //
//  UCA0MCTL = UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
//  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt


    //Enter LPM3, interrupts enabled
    //__bis_SR_register(LPM3_bits + GIE);
    __no_operation();
    Interrupt_UART_ReceiveData_ptr_fuc = Empty_UART_fun;
    SendingWhileTimeOutCount = 0;

    return Func_Success;
}

void _Device_Uart_Module_2_Disable(void){
    //Disable UART module for operation
    USCI_A_UART_disable(UART_Module_2_USCI_A_BASEADDRESS);

    //Disable Receive Interrupt
	USCI_A_UART_clearInterruptFlag(UART_Module_2_USCI_A_BASEADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_disableInterrupt(UART_Module_2_USCI_A_BASEADDRESS, USCI_A_UART_RECEIVE_INTERRUPT);
    __no_operation();
    Interrupt_UART_ReceiveData_ptr_fuc = Empty_UART_fun;

}

void _Device_Uart_Module_2_Set_Calling_Function_By_Uart_Receive_Interrupt(void (*calling_fun)(__IO t_uint8 receivedByte)){
    Interrupt_UART_ReceiveData_ptr_fuc = calling_fun;
}

t_uint8 _Device_Uart_Module_2_Send_Bytes(unsigned char *sendByte, unsigned int length){
	unsigned int i;

//    if(Usart_Peripheral_Enable_Flag == DeviceOff){
//        return Func_Fail;
//    }

	for(i = 0; i < length; i++){
		SendingWhileTimeOutCount = 0;
        //confirm TX buffer is ready first, USCI_A1 TX buffer ready?
        while (!USCI_A_UART_getInterruptStatus(UART_Module_2_USCI_A_BASEADDRESS, USCI_A_UART_TRANSMIT_INTERRUPT_FLAG)){
            __no_operation();
			if(SendingWhileTimeOutCount >= UART_Module_2_USCI_A_BASEADDRESS){
				break;
			}
			SendingWhileTimeOutCount++;
        }

        //Transmit data
        USCI_A_UART_transmitData(UART_Module_2_USCI_A_BASEADDRESS, (*(sendByte + i)));
    }
    __no_operation();

    return Func_Success;
}


//******************************************************************************
//
//This is the USCI_A0 interrupt vector service routine.
//
//******************************************************************************
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR (void)
{
    switch (__even_in_range(UCA0IV,4)){

        case 0:break;                             // Vector 0 - no interrupt
        case 2:                                   // Vector 2 - RXIFG
            //Receive data
            Interrupt_UART_ReceiveData_ptr_fuc(USCI_A_UART_receiveData(UART_Module_2_USCI_A_BASEADDRESS));
        break;
        case 4:break;                             // Vector 4 - TXIFG
        default: break;
    }
}


