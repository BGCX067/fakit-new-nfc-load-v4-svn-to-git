





/********************************************************************************
* Golbal Variable Control Bits	Define											*
********************************************************************************/
/////////////////////////////////////////////////////////////////////////////////
/* Module Status Control Bits */
/* For G_Module_Status ; unsigned int */
//Low byte
#define Current_Dir_CHG          	(0x0001)    //Charging
#define Current_Dir_Static          (0x0002)    //relaxation mode or Quiescent current, usually equal to discharging
#define Current_Dir_DSG        		(0x0004)    //Discharging,
#define SetAsOnlyComPort			(0x0008)    //Translate the kit to ComPort only,
#define NoUse3        				(0x0010)    //No Use,
#define Dynamic_UVP                 (0x0020)    //No Use,
#define Module_UT                   (0x0040)    // Under Temperature Alarm
//#define Polling_At_Last_Exec_And_Ready  (0x0080)    //
//Hight byte
#define Module_D_OC                 (0x0100)    //=D_OC, DisCharge over current flag
#define Module_C_OC                 (0x0200)    //=C_OC, Charge over current flag
#define Module_BAT_OV               (0x0400)    // Over Voltage Alarm, Whole Battery voltage
#define Module_BAT_UV               (0x0800)    // Under Voltage Alarm, Whole Battery voltage
//#define Module_PIC_OV               (0x1000)    // Cell Voltage Over Voltage Alarm
//#define Module_PIC_UV               (0x2000)    // Cell Voltage Under Voltage Alarm
#define Module_CHG_OT               (0x4000)    // CHG Over Temperature Alarm
#define Module_DSG_OT               (0x8000)    // DSG Over Temperature Alarm

/////////////////////////////////////////////////////////////////////////////////
/* G_Module_Function_Status Control Bits */
/* For G_Module_Function_Status ; unsigned int */
//Low byte
#define ADC_Start_Conversion            (0x0001)    //
#define ADC_Done_Conversion             (0x0002)    //
#define ADC_First_Conversion            (0x0004)    //Start CHG Over current counting for releasing OC. if finish, will set COC_COUNTING_FINISH flag
//#define COC_COUNTING_FINISH             (0x0008)    //COC Over current counting Finish.
#define DirMeasuredProcessingViaADC_Ch  (0x0010)    //direct set porcess for measured, without for setting Peripheral
#define Charger_ID_Level_1_Check        (0x0020)    //
#define Charger_ID_Level_2_Check        (0x0040)    //
//#define Charger_ID_Level_3_Check        (0x0080)    //
//Hight byte
#define Set_Charger_24V_Measured_Processing     (0x0100)    //
#define Set_Charger_36V_Measured_Processing     (0x0200)    //
#define Set_Charger_48V_Measured_Processing     (0x0400)    //
#define Process_Set_Channel                     (0x0800)    //
#define Process_Charger_Measured_Done           (0x1000)    //
#define Set_Pack_DSG_Vol_Measured_Processing    (0x2000)    //
#define Set_Pack_CHG_Vol_Measured_Processing    (0x4000)    //
#define Set_Channels_ADC_Measured_Processing    (0x8000)    //


/////////////////////////////////////////////////////////////////////////////////
/* G_1st_Module_Function_Status Control Bits */
/* For G_1st_Module_Function_Status ; unsigned int */
//Low byte
#define Set_Chg_Current_Measured_Processing     (0x0001)    //
#define Set_Dsg_Current_Measured_Processing     (0x0002)    //
//#define ADC_First_Conversion            (0x0004)    //Start CHG Over current counting for releasing OC. if finish, will set COC_COUNTING_FINISH flag
//#define COC_COUNTING_FINISH             (0x0008)    //COC Over current counting Finish.
//#define DirMeasuredProcessingViaADC_Ch  (0x0010)    //direct set porcess for measured, without for setting Peripheral
//#define Charger_ID_Level_1_Check        (0x0020)    //
//#define Charger_ID_Level_2_Check        (0x0040)    //
//#define Charger_ID_Level_3_Check        (0x0080)    //
//Hight byte
//#define Set_Charger_24V_Measured_Processing     (0x0100)    //
//#define Set_Charger_36V_Measured_Processing     (0x0200)    //
//#define Set_Charger_48V_Measured_Processing     (0x0400)    //
//#define Process_Set_Channel                     (0x0800)    //
//#define Process_Charger_Measured_Done           (0x1000)    //
//#define Set_Pack_DSG_Vol_Measured_Processing    (0x2000)    //
//#define Set_Pack_CHG_Vol_Measured_Processing    (0x4000)    //
//#define Set_Channels_ADC_Measured_Processing    (0x8000)    //
