
/**
  ******************************************************************************
  * @file    InformationFlashAccess.c
  * @author  Dynapack ADT, Hsinmo
  * @version V1.0.0
  * @date    3-August-2013
  * @brief   InformationFlashAccess setting
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

#include "flash.h"

#include "InformationFlash_Memory_Define.h"
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

//==============================================================================
// Private function prototypes
//==============================================================================


//==============================================================================
// Private functions
//==============================================================================



unsigned char InitialFlashContent[Flash_segment_Size];

void WriteInitialDataToFlash(unsigned int Offset_Address, unsigned char *value, unsigned char dataLength ){

  WriteDataToFlash(Offset_Address, value, dataLength );

}

void WriteDataToFlash(unsigned int Offset_Address, unsigned char *value, unsigned char dataLength ){
  unsigned int i;

  unsigned char *Initial_Sgement_ptr;
  Initial_Sgement_ptr = (unsigned  char *)Config_Segment; // Initialize Flash pointer;

  //read data from information falsh segment to array
  for(i = 0; i < Flash_segment_Size; i++){
    InitialFlashContent[i] = *(Initial_Sgement_ptr + i) ;
  }
  //__delay_cycles(50);

  //write setting data to array
  for(i = 0; i < dataLength; i++){
    if(Offset_Address + i >= Flash_segment_Size){
      break;
    }
    InitialFlashContent[Offset_Address + i] = *value++;
  }

  //Write array values to flash
  __disable_interrupt();                    // 5xx Workaround: Disable global
                                            // interrupt while erasing. Re-Enable
                                            // GIE if needed
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Initial_Sgement_ptr = 0;                 // Dummy write to erase Flash seg
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  for (i = 0; i < Flash_segment_Size; i++)
  {
    *Initial_Sgement_ptr++ = InitialFlashContent[i];                 // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit

  while(FCTL3 & BUSY);

  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

  _EINT();

}

void ReadInitialDataFromFlash(unsigned int Offset_Address, unsigned char *value, unsigned char dataLength ){

  unsigned int i;
  unsigned char *Initial_Sgement_ptr;
  Initial_Sgement_ptr = (unsigned  char *)Config_Segment; // Initialize Flash pointer;


  //read data from information falsh segment to array
  for(i = 0; i < dataLength; i++){
    if(Offset_Address + i >= Flash_segment_Size){
      break;
    }
    *value++ = *(Initial_Sgement_ptr + Offset_Address + i) ;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//void FlashWriteSeg(unsigned int seg_addr, char value){
void FlashWriteSeg(unsigned int Flash_Start_Address, char value){

  unsigned int i;
  char * Flash_ptr;                         // Initialize Flash pointer
  Flash_ptr = (char *) Flash_Start_Address;
  __disable_interrupt();                    // 5xx Workaround: Disable global
                                            // interrupt while erasing. Re-Enable
                                            // GIE if needed
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash seg
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  for (i = 0; i < 128; i++)
  {
    *Flash_ptr++ = value++;                 // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit

  while(FCTL3 & BUSY);


  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

  _EINT();

}


void FlashWriteData(unsigned int Flash_Start_Address, char *value, unsigned char dataLength){

  unsigned int i;
  char * Flash_ptr;                         // Initialize Flash pointer
  Flash_ptr = (char *)Flash_Start_Address;
  __disable_interrupt();                    // 5xx Workaround: Disable global
                                            // interrupt while erasing. Re-Enable
                                            // GIE if needed
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash seg
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  for (i = 0; i < dataLength; i++)
  {
    *Flash_ptr++ = *value++;                 // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit

  while(FCTL3 & BUSY);


  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

  _EINT();

}




