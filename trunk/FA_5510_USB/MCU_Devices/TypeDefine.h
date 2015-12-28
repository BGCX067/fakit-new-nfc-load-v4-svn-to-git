

#pragma once
/*----------------------------------------------------------------------------+
 | Type Definition & Macro                                                     |
 +----------------------------------------------------------------------------*/
//typedef signed char     INT8;
//typedef unsigned char   UINT8;
//typedef signed int      INT16;
//typedef unsigned int    UINT16;
//typedef signed long     INT32;
//typedef unsigned long   UINT32;
//
//typedef unsigned char*  PUINT8;
//typedef unsigned int*   PUINT16;
//typedef unsigned long*  PUINT32;
//typedef signed char*    PINT8;
//typedef signed int*     PINT16;
//typedef signed long*    PINT32;

typedef signed char     t_int8;
typedef unsigned char   t_uint8;
typedef signed int      t_int16;
typedef unsigned int    t_uint16;
typedef signed long     t_int32;
typedef unsigned long   t_uint32;

typedef unsigned char*  t_puint8;
typedef unsigned int*   t_puint16;
typedef unsigned long*  t_puint32;
typedef signed char*    t_pint8t;
typedef signed int*     t_pint16;
typedef signed long*    t_pint32;
#define __IO            volatile



#define Func_Success   0
#define Func_Failure   1

#define IO_INPUT_HIGH 0x01
#define IO_INPUT_LOW  0x00
