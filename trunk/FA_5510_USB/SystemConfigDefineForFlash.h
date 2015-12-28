
#include "MCU_Devices/InformationFlash_Memory_Define.h"

/*****************************************************************
  the Flash Real Data are created in SystemConfigInitialData.s43
******************************************************************/
//
/////////////////////////////////////////////////////////////
//// define Flash Segment Address (one segment = 128 bytes)
/////////////////////////////////////////////////////////////
//#define Flash_segment_Size 128  // bytes
//#define Flash_segment_A   0x1980  //can not be written
//#define Flash_segment_B   0x1900
//#define Flash_segment_C   0x1880
//#define Flash_segment_D   0x1800
//
//#define Config_Segment   Flash_segment_C  //importment define
//

//////////////////////////////////////////////////////////////////////////////////////
//  Define flash Data address offset (range: 0~112 bytes)
//  the offset order below is followed by SystemConfigInitialData.s43 data order
//////////////////////////////////////////////////////////////////////////////////////
#define FA_VERSION_offset               0   // 1 bytes//for main function addition and EEPROM Format changing
#define FA_MINOR_VERSION_offset         1   // 1 bytes//only for minor functions changing
#define FA_EEPROM_VERSION_offset        2   // 1 bytes //only for EEPROM values changing
#define FA_RESERVED_VERSION_offset      3   //1 bytes // 0x00: Protuction, 0x01: Samples

#define  FA_Serial_Num_offset          4   //;FA serial NUM ; 2bytes
#define  FA_Serial_Num_Extend_offset   6   //;FA serial NUM Extend_; 2bytes
#define  FA_Manufacture_Date_offset    8   //;FA Manufacture Date ; 2bytes

#define  FA_24V_mV_To_ADC_Factor_offset     10  // ; 4bytes
#define  FA_36V_mV_To_ADC_Factor_offset	    14   // ; 4bytes
#define  FA_48V_mV_To_ADC_Factor_offset	    18   // ; 4bytes

#define  FA_24V_CAL_OFFSET_ADC_offset       22   // ; 1bytes
#define  FA_36V_CAL_OFFSET_ADC_offset       23   // ; 1bytes
#define  FA_48V_CAL_OFFSET_ADC_offset       24   // ; 1bytes
#define  FA_Pack_DSG_CAL_OFFSET_ADC_offset  25   // ; 1bytes for address
#define  FA_Pack_CHG_CAL_OFFSET_ADC_offset      26   // ; 1bytes for address
#define  Reserved_1_Byte_offset                 27   // ; 1bytes for address //(align to even)

#define  FA_Pack_DSG_mV_To_ADC_Factor_offset    28   // ; 4bytes;

#define  FA_HW_Version_offset               32   // ; 1bytes
#define  FA_HW_MINOR_Version_offset         33   // ; 1bytes
#define  FA_HW_FUNCTION1_BIT_offset         34   // ; 2bytes
#define  FA_HW_FUNCTION2_BIT_offset         36   // ; 2bytes
#define  FA_HW_FUNCTION3_BIT_offset         38   // ; 2bytes

#define  FA_Pack_CHG_mV_To_ADC_Factor_offset    40   // ; 4bytes;

#define  Normal_Auto_Charger_Check_Delay_Cycle_offset    44  // ; 1bytes; 3s ==> Delay_Cycle * timer interval time(100ms)
#define  Faster_Auto_Charger_Check_Delay_Cycle_offset    45   // ; 1bytes; 400ms ==> Delay_Cycle * timer interval time(100ms)

#define  FA_CHG_Current_CAL_OFFSET_ADC_offset       46   // ; 1bytes for address //(align to even)
#define  FA_DSG_Current_CAL_OFFSET_ADC_offset       47   // ; 1bytes for address //(align to even)
#define  FA_CHG_Current_mA_To_ADC_Factor_offset     48   // ; 4bytes;
#define  FA_DSG_Current_mA_To_ADC_Factor_offset     52   // ; 4bytes;


////////////////////////////////////////////////////////////
//  FA System Information
////////////////////////////////////////////////////////////
#define FA_VERSION              (*((unsigned char *)(Config_Segment + FA_VERSION_offset)))
#define FA_MINOR_VERSION        (*((unsigned char *)(Config_Segment + FA_MINOR_VERSION_offset)))
#define FA_EEPROM_VERSION       (*((unsigned char *)(Config_Segment + FA_EEPROM_VERSION_offset)))
#define FA_RESERVED_VERSION     (*((unsigned char *)(Config_Segment + FA_RESERVED_VERSION_offset)))

#define  FA_Serial_Num          (*((unsigned int *)(Config_Segment + FA_Serial_Num_offset)))
#define  FA_Serial_Num_Extend   (*((unsigned int *)(Config_Segment + FA_Serial_Num_Extend_offset)))
#define  FA_Manufacture_Date    (*((unsigned int *)(Config_Segment + FA_Manufacture_Date_offset)))

#define  FA_24V_mV_To_ADC_Factor        (*((float *)(Config_Segment + FA_24V_mV_To_ADC_Factor_offset)))
#define  FA_36V_mV_To_ADC_Factor        (*((float *)(Config_Segment + FA_36V_mV_To_ADC_Factor_offset)))
#define  FA_48V_mV_To_ADC_Factor        (*((float *)(Config_Segment + FA_48V_mV_To_ADC_Factor_offset)))

#define  FA_24V_CAL_OFFSET_ADC          (*((signed char *)(Config_Segment + FA_24V_CAL_OFFSET_ADC_offset)))
#define  FA_36V_CAL_OFFSET_ADC          (*((signed char *)(Config_Segment + FA_36V_CAL_OFFSET_ADC_offset)))
#define  FA_48V_CAL_OFFSET_ADC          (*((signed char *)(Config_Segment + FA_48V_CAL_OFFSET_ADC_offset)))
#define  FA_Pack_DSG_CAL_OFFSET_ADC     (*((signed char *)(Config_Segment + FA_Pack_DSG_CAL_OFFSET_ADC_offset)))
#define  FA_Pack_CHG_CAL_OFFSET_ADC     (*((signed char *)(Config_Segment + FA_Pack_CHG_CAL_OFFSET_ADC_offset)))
#define  Reserved_1_Byte                (*((unsigned char *)(Config_Segment + Reserved_1_Byte_offset)))

#define  FA_Pack_DSG_mV_To_ADC_Factor           (*((float *)(Config_Segment + FA_Pack_DSG_mV_To_ADC_Factor_offset)))

#define  FA_HW_Version                   (*((unsigned char *)(Config_Segment + FA_HW_Version_offset)))
#define  FA_HW_MINOR_Version             (*((unsigned char *)(Config_Segment + FA_HW_MINOR_Version_offset)))

#define  FA_HW_FUNCTION1_BIT              (*((unsigned int *)(Config_Segment + FA_HW_FUNCTION1_BIT_offset)))
#define  FA_HW_FUNCTION2_BIT              (*((unsigned int *)(Config_Segment + FA_HW_FUNCTION2_BIT_offset)))
#define  FA_HW_FUNCTION3_BIT              (*((unsigned int *)(Config_Segment + FA_HW_FUNCTION3_BIT_offset)))

#define  FA_Pack_CHG_mV_To_ADC_Factor       (*((float *)(Config_Segment + FA_Pack_CHG_mV_To_ADC_Factor_offset)))

#define  Normal_Auto_Charger_Check_Delay_Cycle  (*((unsigned char *)(Config_Segment + Normal_Auto_Charger_Check_Delay_Cycle_offset)))
#define  Faster_Auto_Charger_Check_Delay_Cycle  (*((unsigned char *)(Config_Segment + Faster_Auto_Charger_Check_Delay_Cycle_offset)))

#define  FA_CHG_Current_CAL_OFFSET_ADC          (*((signed char *)(Config_Segment + FA_CHG_Current_CAL_OFFSET_ADC_offset)))
#define  FA_DSG_Current_CAL_OFFSET_ADC          (*((signed char *)(Config_Segment + FA_DSG_Current_CAL_OFFSET_ADC_offset)))

#define  FA_CHG_Current_mA_To_ADC_Factor    (*((float *)(Config_Segment + FA_CHG_Current_mA_To_ADC_Factor_offset)))
#define  FA_DSG_Current_mA_To_ADC_Factor    (*((float *)(Config_Segment + FA_DSG_Current_mA_To_ADC_Factor_offset)))


