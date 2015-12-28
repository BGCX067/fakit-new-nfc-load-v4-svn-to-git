

#include "FA_HW_FunctionBitDefine.h"
////////////////////////////////////////////////////////////

#define _Erase_Infor_Flash_While_Download_Prg_

////////////////////////////////////////////////////////////
//  FA System Manufacture Date setting
////////////////////////////////////////////////////////////
#define _FA_System_Initial_Date_Year_            2014  //2bytes
#define _FA_System_Initial_Date_Month_           2     //1byte
#define _FA_System_Initial_Date_Day_             6     //1byte
#define _FA_System_Initial_Date_Hour_24hSystem_  0     //1byte
#define _FA_System_Initial_Date_Minute_          0     //1byte

////////////////////////////////////////////////////////////
//  FA System String Information
////////////////////////////////////////////////////////////
//Manufacture_Name max size in bytes = 11 + 1
#define _FA_Manufacture_Name_  "FA Tool"
#define _FA_Manufacture_Name_Length_  7

//Device_Name max size in bytes = 7 + 1
#define _FA_Device_Name_  "FA03"
#define _FA_Device_Name_Length_  4

//Manufacture_Company max size in bytes = 14 + 1
#define _FA_Manufacture_Company_  "Dynapack"
#define _FA_Manufacture_Company_Length_ 8

//Manufacture_Country_Data max size in bytes = 14 + 1
#define _FA_Manufacture_Country_Data_  "TAIWAN"
#define _FA_Manufacture_Country_Data_Length_ 6

////////////////////////////////////////////////////////////
//  FA System Information
////////////////////////////////////////////////////////////
#define _FA_VERSION_                3  //; 1 bytes//for main function addition and EEPROM Format changing
#define _FA_MINOR_VERSION_          0  //; 1 bytes//only for minor functions changing
#define _FA_EEPROM_VERSION_         3  //; 1 bytes //only for EEPROM values changing
#define _FA_RESERVED_VERSION_       1  //; 1 bytes //0x00: Protuction, 0x01: Samples
#define _FA_Serial_Num_             1   // 2bytes
#define _FA_Serial_Num_Extend_      100     //2bytes
#define _FA_Manufacture_Date_   	(_FA_System_Initial_Date_Year_ - 1980) * 512 + _FA_System_Initial_Date_Month_ * 32 + _FA_System_Initial_Date_Day_   //2bytes

#define  _FA_24V_mV_To_ADC_Factor_	    0.0256f   // ; 4bytes
#define  _FA_36V_mV_To_ADC_Factor_	    0.017808696f   // ; 4bytes
#define  _FA_48V_mV_To_ADC_Factor_	    0.013212903f   // ; 4bytes

#define  _FA_24V_CAL_OFFSET_ADC_        0   // ; 1bytes
#define  _FA_36V_CAL_OFFSET_ADC_	    0   // ; 1bytes
#define  _FA_48V_CAL_OFFSET_ADC_	    0   // ; 1bytes
#define  _FA_Pack_DSG_CAL_OFFSET_ADC_   0    // ; 1bytes
#define  _FA_Pack_CHG_CAL_OFFSET_ADC_   0    // ; 1bytes
#define  _Reserved_1_Byte_   0               // ; 1bytes//(align to even)


#define  _FA_Pack_DSG_mV_To_ADC_Factor_	    0.017808696f   // ; 4bytes
////////////////////////////////////////////////////////////
#define  _FA_HW_Version_	                3   // ; 1bytes//for HW main Version
#define  _FA_HW_MINOR_Version_              0   // ; 1bytes//for Minor Version
////////////////////////////////////////////////////////////

#define  _FA_HW_FUNCTION1_BIT_               Functions_1_Status   // ; 2bytes//for HW Function1 Status Bits
#define  _FA_HW_FUNCTION2_BIT_               Functions_2_Status   // ; 2bytes//for HW Function2 Status Bits
#define  _FA_HW_FUNCTION3_BIT_               Functions_3_Status   // ; 2bytes//for HW Function3 Status Bits
////////////////////////////////////////////////////////////
#define  _FA_Pack_CHG_mV_To_ADC_Factor_	    0.017808696f   // ; 4bytes

// Vcharger turn on , at least 2.65s than can be turn off
#define  _Normal_Auto_Charger_Check_Delay_Cycle_    30  // ; 1bytes; 3s ==> Delay_Cycle * timer interval time(100ms)
#define  _Faster_Auto_Charger_Check_Delay_Cycle_    20   // ; 1bytes; 400ms ==> Delay_Cycle * timer interval time(100ms)

////////////////////////////////////////////////////////////
#define  _FA_CHG_Current_CAL_OFFSET_ADC_              0   // ; 1bytes;
#define  _FA_DSG_Current_CAL_OFFSET_ADC_              0   // ; 1bytes;
#define  _FA_CHG_Current_mA_To_ADC_Factor_	    2.048f   // ; 4bytes
#define  _FA_DSG_Current_mA_To_ADC_Factor_	    2.048f   // ; 4bytes


