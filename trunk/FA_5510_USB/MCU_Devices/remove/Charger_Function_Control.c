#if 0
/**
  ******************************************************************************
  * @file    Mux_control.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-April-2013
  * @brief   Mux_control setting
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
#include "adc10_a.h"
#include "ref.h"
#include "msp_dma.h"
//#include "ucs.h"
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

#define CHARGER_IO_PORT             GPIO_PORT_P1
#define CHARGER_24A_PIN             GPIO_PIN6
#define CHARGER_36A_PIN             GPIO_PIN5
#define CHARGER_48A_PIN             GPIO_PIN4
#define CHARGER_24B_PIN             GPIO_PIN3
#define CHARGER_36B_PIN             GPIO_PIN2
#define CHARGER_48B_PIN             GPIO_PIN1

#define CHARGER_ADC_PORT             GPIO_PORT_P6
#define CHARGER_ADC_24V_PIN             GPIO_PIN2
#define CHARGER_ADC_36V_PIN             GPIO_PIN1
#define CHARGER_ADC_48V_PIN             GPIO_PIN0

#define NoUsed1_Vol_ADC_PIN             GPIO_PIN3
#define Battery_Vol_ADC_PIN             GPIO_PIN4
#define NoUsed2_Vol_ADC_PIN             GPIO_PIN5

////////////////////////////////////////////////////////////////////////////////
//ADC_resolution  1024    // 10 bit ADC
//ADC_Ref         2500    // mV
//ADC_Step        2.44140625f  ==>(float)ADC_Ref /  ADC_resolution ==> (mV)
////////////////////////////////////////////////////////////////////////////////
// Charger Voltage circuit and setting
////////////////////////////////////////////////////////////////////////////////
//=====================================================================================================================
//     48V
//    Charger |------Resistor1----+---Resistor2-----|GND
//    Vltage  |       30KR        |   1KR
//                                |
//                                |
//                       voltage output
//                       to MCU ADC in
////////////////////////////////////////////////////////////////
//  ADC_Step        2.44140625f  ==>(float)ADC_Ref /  ADC_resolution ==> (mV)
//  V48chg_mV_To_ADC_Factor    (float)1/(ADC_Step/(Resistor2/(Resistor1+Resistor2))==>小數點6位
//  V48chg_mV_To_ADC_Factor_      0.0132129f
//  V48chg_ADC_OFFSET             (signed char)(-5)   //實際值-理論值
//  理論值 = 實際值 - Offset
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//=====================================================================================================================
//     36V
//    Charger |------Resistor1----+---Resistor2-----|GND
//    Vltage  |       22KR        |   1KR
//                                |
//                                |
//                       voltage output
//                       to MCU ADC in
////////////////////////////////////////////////////////////////
//  ADC_Step        2.44140625f  ==>(float)ADC_Ref /  ADC_resolution ==> (mV)
//  V36chg_mV_To_ADC_Factor    (float)1/(ADC_Step/(Resistor2/(Resistor1+Resistor2))==>小數點6位
//  V36chg_mV_To_ADC_Factor_      0.017808f
//  V36chg_ADC_OFFSET             (signed char)(-5)   //實際值-理論值
//  理論值 = 實際值 - Offset
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//=====================================================================================================================
//     24V
//    Charger |------Resistor1----+---Resistor2-----|GND
//    Vltage  |       15KR        |   1KR
//                                |
//                                |
//                       voltage output
//                       to MCU ADC in
////////////////////////////////////////////////////////////////
//  ADC_Step        2.44140625f  ==>(float)ADC_Ref /  ADC_resolution ==> (mV)
//  V24chg_mV_To_ADC_Factor    (float)1/(ADC_Step/(Resistor2/(Resistor1+Resistor2))==>小數點6位
//  V24chg_mV_To_ADC_Factor_      0.0256f
//  V24chg_ADC_OFFSET             (signed char)(-5)   //實際值-理論值
//  理論值 = 實際值 - Offset
////////////////////////////////////////////////////////////////
//==============================================================================
// Private macro
//==============================================================================
//==============================================================================
// Private Enum
//==============================================================================

//==============================================================================
// Private variables
//==============================================================================
//ADC conversion result array
//#define ADC_conversion_Times        5   //do not modify
#define ADC_conversion_Channels     6
//uint16_t ADC_Result[ADC_conversion_Times][ADC_conversion_Channels];
uint16_t ADC_Result[ADC_conversion_Channels];

//==============================================================================
// Private function prototypes
//==============================================================================
void (*Interrupt_ADC_Conversion_Done_ptr_fuc)(void);
void empty_ADC_Conversion_Done_fun(void){}

//==============================================================================
// Private functions
//==============================================================================

void _Device_CHG_IO_Init(void){

    //set as  output  pins
    GPIO_setAsOutputPin(GPIO_PORT_P1,CHARGER_24A_PIN);
    GPIO_setAsOutputPin(GPIO_PORT_P1,CHARGER_36A_PIN);
    GPIO_setAsOutputPin(GPIO_PORT_P1,CHARGER_48A_PIN);
    GPIO_setAsOutputPin(GPIO_PORT_P1,CHARGER_24B_PIN);
    GPIO_setAsOutputPin(GPIO_PORT_P1,CHARGER_36B_PIN);
    GPIO_setAsOutputPin(GPIO_PORT_P1,CHARGER_48B_PIN);

    GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24A_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36A_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48A_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24B_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36B_PIN);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48B_PIN);

}

void _Device_Set_CHG_ID_Channel(t_uint8 channel){

    switch(channel){
        case CHG_24V_Channel:
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_24A_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36A_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48A_PIN);
            break;
        case CHG_36V_Channel:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24A_PIN);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_36A_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48A_PIN);
            break;
        case CHG_48V_Channel:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24A_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36A_PIN);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_48A_PIN);
            break;

        case CHG_OFF_Channel:
        default:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24A_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36A_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48A_PIN);
            break;
    }
}

void _Device_Set_CHG_Voltage_Channel(t_uint8 channel){

    switch(channel){
        case CHG_24V_Channel:
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_24B_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36B_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48B_PIN);
            break;
        case CHG_36V_Channel:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24B_PIN);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_36B_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48B_PIN);
            break;
        case CHG_48V_Channel:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24B_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36B_PIN);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_48B_PIN);
            break;
        case CHG_ON_Channel:
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_24B_PIN);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_36B_PIN);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1,CHARGER_48B_PIN);
            break;

        case CHG_OFF_Channel:
        default:
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_24B_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_36B_PIN);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1,CHARGER_48B_PIN);
            break;
    }
}



void  _Device_CHG_ADC_Init(void){

    //ADC_TEST();
    Interrupt_ADC_Conversion_Done_ptr_fuc = empty_ADC_Conversion_Done_fun;

    GPIO_setAsInputPin( CHARGER_ADC_PORT, CHARGER_ADC_48V_PIN );
    GPIO_setAsInputPin( CHARGER_ADC_PORT, CHARGER_ADC_36V_PIN );
    GPIO_setAsInputPin( CHARGER_ADC_PORT, CHARGER_ADC_24V_PIN );

    GPIO_setAsPeripheralModuleFunctionInputPin( CHARGER_ADC_PORT, CHARGER_ADC_48V_PIN );
    GPIO_setAsPeripheralModuleFunctionInputPin( CHARGER_ADC_PORT, CHARGER_ADC_36V_PIN );
    GPIO_setAsPeripheralModuleFunctionInputPin( CHARGER_ADC_PORT, CHARGER_ADC_24V_PIN );

    GPIO_setAsInputPin( CHARGER_ADC_PORT, NoUsed1_Vol_ADC_PIN );
    GPIO_setAsInputPin( CHARGER_ADC_PORT, Battery_Vol_ADC_PIN );
    GPIO_setAsInputPin( CHARGER_ADC_PORT, NoUsed2_Vol_ADC_PIN );

    GPIO_setAsPeripheralModuleFunctionInputPin( CHARGER_ADC_PORT, NoUsed1_Vol_ADC_PIN );
    GPIO_setAsPeripheralModuleFunctionInputPin( CHARGER_ADC_PORT, Battery_Vol_ADC_PIN );
    GPIO_setAsPeripheralModuleFunctionInputPin( CHARGER_ADC_PORT, NoUsed2_Vol_ADC_PIN );
    //Initialize the ADC10_A Module
    /*
     * Base Address for the ADC10_A Module
     * Use internal ADC10_A bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC10_A_init(ADC10_A_BASE,
        ADC10_A_SAMPLEHOLDSOURCE_SC,
        ADC10_A_CLOCKSOURCE_ADC10OSC,
        ADC10_A_CLOCKDIVIDER_1);

    ADC10_A_enable(ADC10_A_BASE);

    /*
     * Base Address for the ADC10_A Module
     * Sample/hold for 16 clock cycles
     * Enable Multiple Sampling
     */
    ADC10_A_setupSamplingTimer(ADC10_A_BASE,
        ADC10_A_CYCLEHOLD_16_CYCLES,
        ADC10_A_MULTIPLESAMPLESENABLE);

    //Configure Memory Buffer
    /*
     * Base Address for the ADC10_A Module
     * Use input A2
     * Use positive reference of Internally generated Vref
     * Use negative reference of AVss
     */
    ADC10_A_memoryConfigure(ADC10_A_BASE,
        ADC10_A_INPUT_A5,
        ADC10_A_VREFPOS_INT,
        ADC10_A_VREFNEG_AVSS);

    //Configure internal reference
    //If ref generator busy, WAIT
    while ( REF_BUSY == REF_isRefGenBusy(REF_BASE) ) ;


    //Select internal ref = 2.5V
    REF_setReferenceVoltage(REF_BASE,
        REF_VREF2_5V);
    //Internal Reference ON
    REF_enableReferenceVoltage(REF_BASE);

    //Delay (~75us) for Ref to settle
    __delay_cycles(75);


    //Initialize and Setup DMA Channel 1
    /*
     * Base Address of the DMA Module
     * Configure DMA channel 1
     * Configure channel for repeated single transfer
     * DMA interrupt flag will be set after every 2 transfers
     * Use DMA Trigger Source 24 (ADC10IFG)
     * Tranfer Word-to-Word
     * Trigger upon Rising Edge of Trigger Source
     */
    DMA_init(DMA_BASE,
        DMA_CHANNEL_1,
        DMA_TRANSFER_REPEATED_SINGLE,
        ADC_conversion_Channels,
        DMA_TRIGGERSOURCE_24,
        DMA_SIZE_SRCWORD_DSTWORD,
        DMA_TRIGGER_RISINGEDGE);


    /*
     * Base Address of the DMA Module
     * Configure DMA channel 1
     * Use ADC10_A Memory Buffer as source
     * Increment destination address after every transfer
     */
    DMA_setSrcAddress(DMA_BASE,
        DMA_CHANNEL_1,
        ADC10_A_getMemoryAddressForDMA(ADC10_A_BASE),
        DMA_DIRECTION_UNCHANGED);


            /*
             * Base Address of the DMA Module
             * Configure DMA channel 1
             * Use ADC_Result[i*2] as destination
             * Increment destination address after every transfer
             */
            DMA_setDstAddress(DMA_BASE,
                DMA_CHANNEL_1,
                //(uint32_t)&ADC_Result[i][0],
                (uint32_t)&ADC_Result[0],
                DMA_DIRECTION_INCREMENT);

    //Enable DMA channel 1 interrupt
	DMA_clearInterrupt(DMA_BASE,
        DMA_CHANNEL_1);
    DMA_enableInterrupt(DMA_BASE,
        DMA_CHANNEL_1);

    //Enable transfers on DMA channel 1
    DMA_enableTransfers(DMA_BASE,
        DMA_CHANNEL_1);

//    //Initialize and Setup DMA Channel 0
//    /*
//     * Base Address of the DMA Module
//     * Configure DMA channel 0
//     * Configure channel for repeated single transfer
//     * DMA interrupt flag will be set after every 2 transfers
//     * Use DMA Trigger Source 24 (ADC10IFG)
//     * Tranfer Word-to-Word
//     * Trigger upon Rising Edge of Trigger Source
//     */
//    DMA_init(DMA_BASE,
//        DMA_CHANNEL_0,
//        DMA_TRANSFER_REPEATED_SINGLE,
//        ADC_conversion_Channels,
//        DMA_TRIGGERSOURCE_24,
//        DMA_SIZE_SRCWORD_DSTWORD,
//        DMA_TRIGGER_RISINGEDGE);
//
//
//    /*
//     * Base Address of the DMA Module
//     * Configure DMA channel 0
//     * Use ADC10_A Memory Buffer as source
//     * Increment destination address after every transfer
//     */
//    DMA_setSrcAddress(DMA_BASE,
//        DMA_CHANNEL_0,
//        ADC10_A_getMemoryAddressForDMA(ADC10_A_BASE),
//        DMA_DIRECTION_UNCHANGED);
//
//    //Enable DMA channel 0 interrupt
//	DMA_clearInterrupt(DMA_BASE,
//        DMA_CHANNEL_0);
//    DMA_enableInterrupt(DMA_BASE,
//        DMA_CHANNEL_0);
//
//    //Enable transfers on DMA channel 0
//    DMA_enableTransfers(DMA_BASE,
//        DMA_CHANNEL_0);

}

void _Device_Set_Interrupt_For_ADC_Conversion_Done_Calling_Function(void (*calling_fun)()){
    Interrupt_ADC_Conversion_Done_ptr_fuc = calling_fun;
}
void _Device_Remove_ADC_Conversion_Done_Timer_Calling_Function(void){
    Interrupt_ADC_Conversion_Done_ptr_fuc = empty_ADC_Conversion_Done_fun;
}

void _Device_CHG_ADC_Conversion_Start(void){
    volatile uint16_t i;

    //for (i = 0; i < ADC_conversion_Times; i++)
        {

//            /*
//             * Base Address of the DMA Module
//             * Configure DMA channel 1
//             * Use ADC_Result[i*2] as destination
//             * Increment destination address after every transfer
//             */
//            DMA_setDstAddress(DMA_BASE,
//                DMA_CHANNEL_1,
//                //(uint32_t)&ADC_Result[i][0],
//                (uint32_t)&ADC_Result[0],
//                DMA_DIRECTION_INCREMENT);

            //Wait if ADC10_A core is active
            while (ADC10_A_isBusy(ADC10_A_BASE)) ;

            //Enable and Start the conversion
            //in Repeated Sequence of Channels Conversion Mode
            ADC10_A_startConversion(ADC10_A_BASE,
                ADC10_A_REPEATED_SEQOFCHANNELS);
            //__bis_SR_register(LPM0_bits);
        }
}

//t_uint16 _Device_Get_ADC_Result(t_uint8 ADCchannel){
//    t_uint16 i = 0;
//    t_uint16 sum = 0;
//    switch(ADCchannel){
//        case ADC_Channel0:
//            for(i = 1; i < ADC_conversion_Times; i++){
//               sum += ADC_Result[i][0];
//            }
//            break;
//        case ADC_Channel1:
//            for(i = 1; i < ADC_conversion_Times; i++){
//               sum += ADC_Result[i][2];
//            }
//            break;
//        case ADC_Channel2:
//            for(i = 1; i < ADC_conversion_Times; i++){
//               sum += ADC_Result[i][1];
//            }
//            break;
//
//        default:
//            return 0;
//            break;
//    }
//
//    return (sum>>2);
//}
t_uint16 _Device_Get_ADC_Result(t_uint8 ADCchannel){
    switch(ADCchannel){
        case ADC_Channel0:
            return ADC_Result[0];
            break;
        case ADC_Channel1:
            return ADC_Result[5];
            break;
        case ADC_Channel2:
            return ADC_Result[4];
            break;
        case ADC_Channel3:
            return ADC_Result[3];
            break;
        case ADC_Channel4:
            return ADC_Result[2];
            break;
        case ADC_Channel5:
            return ADC_Result[1];
            break;

        default:
            return 0;
            break;
    }
}



#pragma vector=DMA_VECTOR
__interrupt void DMA1_ISR (void)
{
    switch (__even_in_range(DMAIV,16)){
        case  0: break; //No interrupt
        case  2:        //DMA0IFG
//            //Sequence of Channels Conversion Complete
//            //Disable Conversion without pre-empting any conversions taking place.
//            ADC10_A_disableConversions(ADC10_A_BASE,
//            ADC10_A_PREEMPTCONVERSION);
//            (*Interrupt_ADC_Conversion_Done_ptr_fuc)();
//            //exit LPM
//            //__bic_SR_register_on_exit(CPUOFF);
//            //__bic_SR_register_on_exit(LPM3_bits);   // Exit LPM0-3

            break;
        case  4: //DMA1IFG
            //Sequence of Channels Conversion Complete
            //Disable Conversion without pre-empting any conversions taking place.
            ADC10_A_disableConversions(ADC10_A_BASE,
            ADC10_A_PREEMPTCONVERSION);
            (*Interrupt_ADC_Conversion_Done_ptr_fuc)();
            //exit LPM
            //__bic_SR_register_on_exit(CPUOFF);
            //__bic_SR_register_on_exit(LPM3_bits);   // Exit LPM0-3
            break;
        case  6: break; //DMA2IFG
        default: break;
    }
}
#endif