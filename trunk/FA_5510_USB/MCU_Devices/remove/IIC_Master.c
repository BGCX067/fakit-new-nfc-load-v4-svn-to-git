#if 0
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
#define SLAVE_ADDRESS_7Bit          0x0b //without last R/W bit

#define I2C_MASTER_SDA_PORT             GPIO_PORT_P3
#define I2C_MASTER_SDA_PIN              GPIO_PIN0
#define I2C_MASTER_SCL_PORT             GPIO_PORT_P3
#define I2C_MASTER_SCL_PIN              GPIO_PIN1

#define I2C_USCI_B_BASEADDRESS          USCI_B0_BASE

//#define I2C_DATA_CLOCK_RATE             USCI_B_I2C_SET_DATA_RATE_400KBPS
#define I2C_DATA_CLOCK_RATE             USCI_B_I2C_SET_DATA_RATE_100KBPS

//#define I2C_WhileLoopTimeOut            2000    //transmit time out, for 64 bytes transmitting at 100kbps
#define I2C_Transmit_TimeOut            2000    //transmit time out, for 64 bytes transmitting at 100kbps (8ms)

//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================
//==============================================================================
// Private variables
//==============================================================================
//#include <msp430f5510.h>
//#include "IIC_Header.h"

unsigned char *PRxData_UCB0;                     // Pointer to RX data
unsigned char RXByteCtr_UCB0;
volatile unsigned char RxBuffer_UCB0[128];       // Allocate 128 byte of RAM

//Write
unsigned char *PTxData_UCB0;                     // Pointer to TX data
unsigned char TXByteCtr_UCB0;


//hsinmo
char I2C_Status_UCB0;
char CurrentSlaveAddress;

void Init_IIC_Master_UCB0(char SlaveAddress)
{
  P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 24;//12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = SlaveAddress;//0x48;          // Slave Address is 048h
  CurrentSlaveAddress = SlaveAddress;
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0IE |= UCRXIE;                         // Enable RX interrupt

  UCB0IE |= UCTXIE;                         // Enable TX interrupt

  //hsinmo
  UCB0IE |= UCNACKIE;                       //NACK Condition interrupt enable
  UCB0IE |= UCALIE;                         //Arbitration Lost interrupt enable

}
//void Init_IIC_Master0()
//{
//  P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0
//  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
//  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
//  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
//  UCB0BR0 = 24;//12;                             // fSCL = SMCLK/12 = ~100kHz
//  UCB0BR1 = 0;
//  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
//  UCB0IE |= UCRXIE;                         // Enable RX interrupt
//
//  UCB0IE |= UCTXIE;                         // Enable TX interrupt
//
//  //hsinmo
//  UCB0IE |= UCNACKIE;                       //NACK Condition interrupt enable
//  UCB0IE |= UCALIE;                         //Arbitration Lost interrupt enable
//
//}

char MasterIIC_Write_UCB0(unsigned char *TxData, unsigned char txByteCount)
{
  int wait = 0;
  int retrycount = WAIT_COUNT * 2;

  //hsinmo
  P3SEL &= ~0x03;
  P3DIR |= 0x03;
  P3OUT &= ~0x03;     // output low level
  P3OUT |= BIT1;    //output scl high
  __delay_cycles(100);   //delay 0.5ms
  P3OUT |= BIT0;    //output sda high for I2C stop
  __delay_cycles(100);   //delay 0.5ms
  P3OUT &= ~BIT1;    //output scl low
  P3OUT &= ~BIT0;    //output sda low
  Init_IIC_Master_UCB0(CurrentSlaveAddress);


  PTxData_UCB0 = (unsigned char *)TxData;      // TX array start address
                                          // Place breakpoint here to see each
                                          // transmit operation.
  TXByteCtr_UCB0 = txByteCount;//sizeof TxData;              // Load TX byte counter

  UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

  //__bis_SR_register(LPM0_bits + GIE);     // Enter LPM0, enable interrupts
  __no_operation();                       // Remain in LPM0 until all data
                                          // is TX'd
  //while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

  //hsinmo first check
  while (UCB0CTL1 & UCTXSTP)//;             // Ensure stop condition got sent
  //hsinmo
  {
    wait++;
    __delay_cycles(523);   //delay 0.5ms
    if (wait > retrycount)
    {
      UCB0CTL1 |= UCTXSTP;  //2012/2/3
      TXByteCtr_UCB0 = 0;
      break;
    }
  }
  //hsinmo second check
  while (UCB0CTL1 & UCTXSTP)//;             // Ensure stop condition got sent
  //hsinmo
  {
    wait++;
    __delay_cycles(523);   //delay 0.5ms
    if (wait > retrycount)
    {
      Init_IIC_Master_UCB0(CurrentSlaveAddress);
      break;
    }
  }


  //while (UCB1CTL1 & UCTXSTT)             // Ensure start condition got sent
  while (UCB0CTL1 & UCTXSTT)  //2012/2/3
  {
    wait++;
    __delay_cycles(523);   //delay 0.5ms
    if (wait > WAIT_COUNT)
    {
      //if (UCB1CTL1 & UCTXSTT)
      if (UCB0CTL1 & UCTXSTT) //2012/2/3
      {
        //UCB1CTL1 &= ~UCTXSTT;             //clear start ifg
        UCB0CTL1 &= ~UCTXSTT; //2012/2/3
      }
      //UCB1CTL1 |= UCTXSTP;                // Generate I2C stop condition
      UCB0CTL1 |= UCTXSTP;  //2012/2/3
      TXByteCtr_UCB0 = 0;
      return I2C_WriteFail;
    }
  }

  wait = 0;
  do
  {
    wait++;
    __delay_cycles(523);                     // Delay required between transaction
    if(wait > WAIT_COUNT)
    {
        //UCB1CTL1 |= UCTXSTP;                    // Generate I2C stop condition
        UCB0CTL1 |= UCTXSTP;  //2012/2/3
        return I2C_WriteFail;
        //break;
    }
  }while(TXByteCtr_UCB0);

  return I2C_Good;
}

char MasterIIC_Read_UCB0(unsigned char *RxData, unsigned char rxByteCount)
{
  int wait = 0;
  unsigned char Status = I2C_Good;
  //__delay_cycles(50);                     // Delay required between transaction
  unsigned int i;

  int retrycount = WAIT_COUNT * 2;


  RXByteCtr_UCB0 = rxByteCount;

  for(i=0; i<RXByteCtr_UCB0; i++)
  {
    RxBuffer_UCB0[i] = 0;
  }

  PRxData_UCB0 = (unsigned char *)RxBuffer_UCB0;    // Start of RX buffer
  //RXByteCtr_UCB0 = 5;                          // Load RX byte counter


  //while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
  //hsinmo first check
  while (UCB0CTL1 & UCTXSTP)//;             // Ensure stop condition got sent
  //hsinmo
  {
    wait++;
    __delay_cycles(523);   //delay 0.5ms
    if (wait > retrycount)
    {
      UCB0CTL1 |= UCTXSTP;  //2012/2/3
      TXByteCtr_UCB0 = 0;
      break;
    }
  }
  //hsinmo second check
  while (UCB0CTL1 & UCTXSTP)//;             // Ensure stop condition got sent
  //hsinmo
  {
    wait++;
    __delay_cycles(523);   //delay 0.5ms
    if (wait > retrycount)
    {
      Init_IIC_Master_UCB0(CurrentSlaveAddress);
      break;
    }
  }



  UCB0CTL1 &= ~UCTR;
  UCB0CTL1 |= UCTXSTT;                    // I2C start condition

  //__bis_SR_register(LPM0_bits + GIE);     // Enter LPM0, enable interrupts
                                          // Remain in LPM0 until all data
                                          // is RX'd
  __no_operation();                       // Set breakpoint >>here<< and
                                        // read out the RxBuffer_UCB0 buffer

  //__delay_cycles(2090);         // delay 2ms; 2000.10e-3 x 1.048.10e6

  while(RXByteCtr_UCB0)
  {
    wait++;
    __delay_cycles(523);   //delay 0.5ms
    if (wait > WAIT_COUNT)
    {
      //UCB1CTL1 |= UCTXSTP;                // Generate I2C stop condition
      UCB0CTL1 |= UCTXSTP;  //2012/2/3
      Status = I2C_ReadFail;
      break;
    }
    __delay_cycles(523);                   // Delay required between transaction
  }

  for (i=0; i<rxByteCount; i++)
  {
    RxData[i] =  RxBuffer_UCB0[i];
  }

  return Status;
}


//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr_UCB0 with the byte count.
//-------------------------------------------------------------------------------
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(__even_in_range(UCB0IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: //break;                           // Vector  2: ALIFG
    I2C_Status_UCB0 = I2cAlFail;
    break;
  case  4: //break;                           // Vector  4: NACKIFG
    UCB0IFG &= ~UCNACKIFG;                  // Clear USCI_B1 NACK int flag
    if(UCB0CTL1 & UCTR)
    {
      //TX
      UCB0CTL1 |= UCTXSTP;                    // Generate I2C stop condition
    }else
    {
      //RX
      if(RXByteCtr_UCB0 != 1){
        UCB0CTL1 |= UCTXSTP;                    // Generate I2C stop condition
      }
    }
    RXByteCtr_UCB0 = 0;
    TXByteCtr_UCB0 = 0;
    //I2C_Status_UCB0 = NoRespondence;

    break;
  case  6: break;                           // Vector  6: STTIFG
  case  8: break;                           // Vector  8: STPIFG
  case 10:                                  // Vector 10: RXIFG
    RXByteCtr_UCB0--;                            // Decrement RX byte counter
    if (RXByteCtr_UCB0)
    {
      *PRxData_UCB0++ = UCB0RXBUF;               // Move RX data to address PRxData_UCB0
      if (RXByteCtr_UCB0 == 1)                   // Only one byte left?
        UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
    }
    else
    {
      *PRxData_UCB0 = UCB0RXBUF;                 // Move final RX data to PRxData_UCB0
      //__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
    }
//    else
//    {
//      *PRxData_UCB0 = UCB0RXBUF;                 // Move final RX data to PRxData_UCB0
//      UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
//      //__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
//    }
    break;
  case 12: //break;                           // Vector 12: TXIFG
    //Write
    if (TXByteCtr_UCB0)                          // Check TX byte counter
    {
      UCB0TXBUF = *PTxData_UCB0++;               // Load TX buffer
      TXByteCtr_UCB0--;                          // Decrement TX byte counter
    }
    else
    {
      UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
      UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
      //__bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
    }
    //
    break;
  default: break;
  }
}

#endif