

////////////////////////////////////////////////////////////
//  FA System Hardware Functions setting Status
////////////////////////////////////////////////////////////
/*---------------------------------------------------------
// Functions 1 setting Status
-----------------------------------------------------------*/
//Low byte
#define Commun_UART             (0x0001)    //
#define Commun_RS485            (0x0002)    //
#define Commun_MUX              (0x0004)    //
#define CHGER_24V_IN            (0x0008)    //
#define CHGER_36V_IN            (0x0010)    //
#define CHGER_48V_IN            (0x0020)    //
#define CHGER_ID_Check          (0x0040)    //
#define CHGER_Vol_MUX           (0x0080)    //
//Hight byte
#define Commun_One_Wire         (0x0100)    //
#define Detect_Pack_DSG_Vol     (0x0200)    //
#define Detect_Pack_CHG_Vol     (0x0400)    //
#define LOAD_150MA              (0x0800)    //
#define CHGING_VIA_PACK_DSG     (0x1000)    //
#define NFC_READER              (0x2000)    //
#define NFC_TAG                 (0x4000)    //
//#define            (0x8000)    //

//=============================================================
#define Functions_1_Status      Commun_UART+Commun_RS485+Commun_MUX+\
            CHGER_24V_IN+CHGER_36V_IN+CHGER_48V_IN+CHGER_ID_Check+CHGER_Vol_MUX+\
            Commun_One_Wire+Detect_Pack_DSG_Vol+Detect_Pack_CHG_Vol+LOAD_150MA+\
            CHGING_VIA_PACK_DSG+NFC_READER
//=============================================================

/*---------------------------------------------------------
// Functions 2 setting Status
-----------------------------------------------------------*/
//Low byte
//#define Detect_UART_M1_End_Frame            (0x0001)    //
//#define Detect_UART_M2_End_Frame            (0x0002)    //
//#define Detect_UART_M1_End_Code             (0x0004)    //
//#define Detect_UART_M2_End_Code             (0x0008)    //
//#define UART_RX_PrecedingCode_Find      (0x0010)    //
//#define UART_RX_EndingCode_Find         (0x0020)    //
//#define UART_RX_FRAME_ADDRESS_FAIL      (0x0040)    //
//#define UART_RX_FRAME_PACKET_FAIL       (0x0080)    //
//Hight byte
//#define    (0x0100)    //
//#define                   (0x0200)    //
//#define                   (0x0400)    //
//#define               (0x0800)    //
//#define         (0x1000)    ///
//#define         (0x2000)    //
//#define            (0x4000)    //
//#define            (0x8000)    //

//=============================================================
#define Functions_2_Status          (0)
//=============================================================

/*---------------------------------------------------------
// Functions 3 setting Status
-----------------------------------------------------------*/
//Low byte
//#define Detect_UART_M1_End_Frame            (0x0001)    //
//#define Detect_UART_M2_End_Frame            (0x0002)    //
//#define Detect_UART_M1_End_Code             (0x0004)    //
//#define Detect_UART_M2_End_Code             (0x0008)    //
//#define UART_RX_PrecedingCode_Find      (0x0010)    //
//#define UART_RX_EndingCode_Find         (0x0020)    //
//#define UART_RX_FRAME_ADDRESS_FAIL      (0x0040)    //
//#define UART_RX_FRAME_PACKET_FAIL       (0x0080)    //
//Hight byte
//#define    (0x0100)    //
//#define                   (0x0200)    //
//#define                   (0x0400)    //
//#define               (0x0800)    //
//#define         (0x1000)    ///
//#define         (0x2000)    //
//#define            (0x4000)    //
//#define            (0x8000)    //

//=============================================================
#define Functions_3_Status          (0)
//=============================================================

