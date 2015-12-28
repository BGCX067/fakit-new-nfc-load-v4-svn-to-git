

#include "TypeDefine.h"
////////////////////////////////////////////////////////////////////////////////

typedef enum{
    Device_Off,
    Device_On
}Device_Status;

/*
 * ======== Power_Management_Module Config ========
 */
void _Device_Set_Power_Management_Module(void);

/*
 * ======== Clock Config ========
 */
#define REQUIRE_FREQ_MCLK       8000000 //Hz
#define REQUIRE_FREQ_SMCLK      2000000 //Hz
#define REQUIRE_FREQ_ACLK       32768 //Hz

//void Init_Clock_By_TI_For_USB (void);
void _Device_Clock_Source_Set_Out_To_Pin(void);
void _Device_Init_Clock_Module(void);
t_uint32 _Device_Get_Clock_Source_SMCLK();
t_uint32 _Device_Get_Clock_Source_MCLK();
t_uint32 _Device_Get_Clock_Source_ACLK();

/*
 * ======== Init_Ports ========
 */
void _Device_Init_Ports_To_Output_Low (void);
/*
 * ======== Init_Ports ========
 */
void Port2_Pin5_6_Init();

/*
 * ======== USB Config ========
 */
typedef enum {
    USB_Status_USB_DISCONNECTED,
    USB_Status_USB_CONNECTED_NO_ENUM,
    USB_Status_ENUM_ACTIVE,
    USB_Status_ENUM_SUSPENDED,
    USB_Status_ENUM_IN_PROGRESS,
    USB_Status_FAILED_ENUM,
    USB_Status_ERROR,
    USB_Status_NOENUM_SUSPENDED
}   t_STATUS_USB_Status_List;

void Init_USB_Config (void);
void Polling_For_USB_Connection_Status(void);


/*
 * ======== Timer A Config ========
 */
#define _Config_TIMER_A_PERIOD_100MS_
//#define _Config_TIMER_A_PERIOD_10MS_

#if defined (_Config_TIMER_A_PERIOD_100MS_)
    #define Timer_A_Polling_Base_MS     100
#elif defined (_Config_TIMER_A_PERIOD_10MS_)
    #define Timer_A_Polling_Base_MS     10
#else
    #error "please define Timer A Period"
#endif

#define Max_TimerA_INTERRUPT_Function_Calling   5
void _Device_Init_Timer_A (void);
void _Device_Enable_Timer_A(void);
void _Device_Disable_Timer_A(void);
void _Device_Set_TimerA_Interrupt_Timer_Calling_Function(t_uint8 fun_index, void (*calling_fun)());
void _Device_Remove_TimerA_Interrupt_Timer_Calling_Function(t_uint8 fun_index);

/*
 * ======== Timer B Config ========
 */
#define _Config_TIMER_B_PERIOD_1MS_
//#define _Config_TIMER_B_PERIOD_10MS_

#if defined (_Config_TIMER_B_PERIOD_1MS_)
    #define Timer_B_Polling_Base_MS     1
#elif defined (_Config_TIMER_B_PERIOD_10MS_)
    #define Timer_B_Polling_Base_MS     10
#else
    #error "please define Timer B Period"
#endif

#define Max_TimerB_INTERRUPT_Function_Calling   5
void _Device_Init_Timer_B (void);
void _Device_Enable_Timer_B(void);
void _Device_Disable_Timer_B(void);
void _Device_Set_TimerB_Interrupt_Timer_Calling_Function_With_Delay_And_Exec(t_uint8 fun_index, void (*calling_fun)(), __IO t_uint16 ms_Dealy );
void _Device_Remove_TimerB_Interrupt_Timer_Calling_Function(t_uint8 fun_index);

/*
 * ======== Commun Mux_Control ========
 */
typedef enum{
    Comm_RS485_Mux_Ch,
    Comm_UART_Mux_Ch
}Communication_Mux_Channels;

void  _Device_Commun_MUX_Init(void);
void _Device_Set_Commun_Mux_Channel(Communication_Mux_Channels channel);

/*
 * ======== UART Module 1 Config ========
 */
#define UART_Module_1_USCI_A_BASEADDRESS        USCI_A1_BASE
//#define Module_1_BAUD_RATE                      9600
#define Uart_Module_1_SendingTimeOutCycle       2000


#define USART_Module_1_TX_PORT                  GPIO_PORT_P4
#define USART_Module_1_TX_PIN                   GPIO_PIN4
#define USART_Module_1_RX_PORT                  GPIO_PORT_P4
#define USART_Module_1_RX_PIN                   GPIO_PIN5

t_uint8 _Device_Uart_Module_1_Enable(t_uint32 baud_rate);
void _Device_Uart_Module_1_Disable(void);
void _Device_Uart_Module_1_Set_Calling_Function_By_Uart_Receive_Interrupt(void (*calling_fun)(__IO t_uint8 receivedByte));
t_uint8 _Device_Uart_Module_1_Send_Bytes(unsigned char *sendByte, unsigned int length);

/*
 * ======== UART Module 2 Config ========
 */
#define UART_Module_2_USCI_A_BASEADDRESS        USCI_A0_BASE
//#define Module_2_BAUD_RATE                      9600
#define Uart_Module_2_SendingTimeOutCycle       2000


#define USART_Module_2_TX_PORT                  GPIO_PORT_P3
#define USART_Module_2_TX_PIN                   GPIO_PIN3
#define USART_Module_2_RX_PORT                  GPIO_PORT_P3
#define USART_Module_2_RX_PIN                   GPIO_PIN4

t_uint8 _Device_Uart_Module_2_Enable(t_uint32 baud_rate);
void _Device_Uart_Module_2_Disable(void);
void _Device_Uart_Module_2_Set_Calling_Function_By_Uart_Receive_Interrupt(void (*calling_fun)(__IO t_uint8 receivedByte));
t_uint8 _Device_Uart_Module_2_Send_Bytes(unsigned char *sendByte, unsigned int length);

/*
 * ======== I2C UCB0 Master Config ========
 */
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


void _Device_Init_I2C_IO_Port_As_Input(void);
unsigned char _Device_get_SDA_Pin_Status(void);
unsigned char _Device_get_SCL_Pin_Status(void);

////#define NUMBER_OF_MODULES       5         // 6KWh = ESS 1.2KWh x 5 Units
//#define MOD01_ADDRESS           0x01
//#define MOD02_ADDRESS           0x02
//#define MOD03_ADDRESS           0x03
//#define MOD04_ADDRESS           0x04
//#define MOD05_ADDRESS           0x05
//
//
///************************************************************\
//| IIC_Master.c                                              |
//\************************************************************/
//#define RX_DATA_SIZE            4//16        // Receiving data in bytes
//#define TX_DATA_SIZE            13//21
//#define WAIT_COUNT              100
//
//enum I2C_Condition
//{
//  I2C_Good,
//  I2C_WriteFail,
//  I2C_ReadFail,
//  //hsinmo
//  I2cAlFail,
//  NoRespondence
//};
//
//void Init_IIC_Master_UCB0(char);
//char MasterIIC_Write_UCB0(unsigned char *, unsigned char );
//char MasterIIC_Read_UCB0(unsigned char *, unsigned char );
//
//
///************************************************************\
//| IIC_Slave.c                                               |
//\************************************************************/
//#define MAX_TRANSMIT_COUNT          128//120
//#define MAX_RECEIVE_COUNT           8
//#define OWNADDRESS                  0x48
//void Init_IIC_Slave_UCB1(char);
//void checkRXAndGetTX();
//void ResetRXBuffer();
//void ResetTXBuffer();

/*
 * ======== USB Config ========
 */
void _Device_Init_USB_Config (void);
void _Device_Set_USB_Receive_From_PC_Calling_Function(void (*calling_fun)(t_uint8* receivedBytesBuffer, t_uint16 receivingSize));
t_uint8 _Device_USB_Send_Bytes_To_PC(unsigned char *sendByte, unsigned int length);
t_uint8 _Device_Polling_For_USB_Connection_Status();

/*
 * ======== Charger Function_Control ========
 */
//enum CHG_IO_Channel{
//    CHG_OFF_Channel,
//    CHG_ON_Channel,
//    CHG_24V_Channel,
//    CHG_36V_Channel,
//    CHG_48V_Channel
//};
//enum CHG_ID_Level{
//    CHG_ID_Level1,
//    CHG_ID_Level2,
//    CHG_ID_Level3
//};

typedef enum {
    NO_CHG_ID,
    USE_CHG_ID
} CHG_ID_Sense_Status;
typedef enum {
    CHG_ID_Level_All_Lo,
    CHG_ID_Level_All_Hi,
    CHG_ID_Level1_Hi,
    CHG_ID_Level2_Hi
} CHG_ID_Levels;

typedef enum{
    Chger_Mux_Ch1,
    Chger_Mux_Ch2,
    Chger_Mux_Ch3,
    Chger_Mux_Ch4
}Chger_Mux_Channels;

typedef enum {
    Chger_InputVol_Ch_All_Lo,
    Chger_InputVol_Ch_All_Hi,
    Chger_24Vol_Ch_Hi,
    Chger_36Vol_Ch_Hi,
    Chger_48Vol_Ch_Hi
} Chger_InputVol_Ch;

void _Device_Charger_Func_Init(void);
CHG_ID_Sense_Status _Device_Get_Chg_ID_Sense_Status();
void _Device_Set_Chg_ID_Level(CHG_ID_Levels level);
void _Device_Chger_Mux_Channel_Enable(Device_Status ds);
void _Device_Set_Chger_Mux_Channel(Chger_Mux_Channels channel);
void _Device_Set_Chger_Input_Voltage_Channel(Chger_InputVol_Ch channel);
//void _Device_CHG_IO_Init(void);
//void _Device_Set_CHG_ID_Channel(t_uint8 channel);
//void _Device_Set_CHG_Voltage_Channel(t_uint8 channel);
//void  _Device_CHG_ADC_Init(void);
//void  _Device_CHG_ADC_Conversion_Start(void);
//void _Device_Set_Interrupt_For_ADC_Conversion_Done_Calling_Function(void (*calling_fun)());
//void _Device_Remove_ADC_Conversion_Done_Timer_Calling_Function(void);
//t_uint16 _Device_Get_ADC_Result(t_uint8 ADCchannel);
/*
 * ======== ADC Function_Control ========
 */

//enum ADC_Channel{
//    ADC_Channel0,   //no used
//    ADC_Channel1,   //no used
//    ADC_Channel2,   //no used
//    ADC_Channel3,   //no used
//    ADC_Channel4,   //no used
//    ADC_Channel5,   //adc pack c
//    ADC_Channel6,   //adc pack d
//    ADC_Channel7    //chger vol
//};
typedef enum{
    ADC_Channel0 = 0,   //ADC10_A_INPUT_A0,
    ADC_Channel1,
    ADC_Channel2,
    ADC_Channel3,
    ADC_Channel4,
    ADC_Channel5,
    ADC_Channel6,
    ADC_Channel7
}MeasuredSingleADCChannels;

void _Device_Measured_Sequence_ADC_Init(void);
void _Device_Measured_Sequence_ADC_Conversion_Start(void);
t_uint16 _Device_Get_Sequence_ADC_Result(t_uint8 ADCchannel);

void  _Device_Measured_RepeatedSingle_ADC_Init();
void  _Device_Set_Measured_RepeatedSingle_ADC_Chasnnel(MeasuredSingleADCChannels adc_channel);
void _Device_Measured_RepeatedSingle_ADC_Conversion_Start(void);
t_uint16 _Device_Get_RepeatedSingle_ADC_Result();

void _Device_Set_Interrupt_For_ADC_Conversion_Done_Calling_Function(void (*calling_fun)());
void _Device_Remove_ADC_Conversion_Done_Timer_Calling_Function(void);


/************************************************************\
| InformationFlashAccess.c                                   |
\************************************************************/
void WriteInitialDataToFlash(unsigned int Offset_Address, unsigned char *value, unsigned char dataLength );
void WriteDataToFlash(unsigned int Offset_Address, unsigned char *value, unsigned char dataLength );
void ReadInitialDataFromFlash(unsigned int Offset_Address, unsigned char *value, unsigned char dataLength );

/*
 * ======== System Function control setting ========
 */
void _Device_System_Func_Ctrl_Init(void);
void _Device_Set_VPD_GATE(Device_Status status);
void _Device_Set_VPC_GATE(Device_Status status);
void _Device_Set_LOADING_GATE(Device_Status status);
void _Device_Set_CHGING_VIA_PACK_D_GATE(Device_Status status);


