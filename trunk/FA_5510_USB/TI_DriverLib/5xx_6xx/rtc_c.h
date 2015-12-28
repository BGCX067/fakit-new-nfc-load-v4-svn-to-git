/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//THIS FILE IS INCLUDED FOR BACKWARD COMPATIBILTY. PLEASE DO NOT USE THIS FILE
//AND INCLUDED APIS FOR NEW APPLICATIONS.
//*****************************************************************************
#ifndef __MSP430WARE_RTC_C_H__
#define __MSP430WARE_RTC_C_H__

//*****************************************************************************
//
//The following are the defines to include the required modules for this
//peripheral in msp430xgeneric.h file
//
//*****************************************************************************
#define __MSP430_HAS_RTC_C__

//*****************************************************************************
//
//The following is a struct that can be passed to RTCC_CalendarInit() in the
//CalendarTime parameter, as well as returned by RTCC_getCalendarTime()
//
//*****************************************************************************
typedef struct {
    unsigned char Seconds;
    unsigned char Minutes;
    unsigned char Hours;
    unsigned char DayOfWeek;
    unsigned char DayOfMonth;
    unsigned char Month;
    unsigned int Year;
} Calendar;

//*****************************************************************************
//
//The following are values that can be passed to RTCC_setCalibrationData()
//in the offsetDirection parameter.
//
//*****************************************************************************
#define RTCC_CALIBRATIONFREQ_OFF   (RTCCALF_0)
#define RTCC_CALIBRATIONFREQ_512HZ (RTCCALF_1)
#define RTCC_CALIBRATIONFREQ_256HZ (RTCCALF_2)
#define RTCC_CALIBRATIONFREQ_1HZ   (RTCCALF_3)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_setCalibrationData()
//in the offsetDirection parameter.
//
//*****************************************************************************
#define RTCC_CALIBRATION_DOWN1PPM  ( !(RTCOCALS) )
#define RTCC_CALIBRATION_UP1PPM    (RTCOCALS)

//*****************************************************************************
//
//The following are values that can be passed to
//RTCC_setTemperatureCompensation() in the offsetDirection parameter.
//
//*****************************************************************************
#define RTCC_COMPENSATION_DOWN1PPM  ( !(RTCTCMPS) )
#define RTCC_COMPENSATION_UP1PPM    (RTCTCMPS)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_setClockRegistersFormat()
//in the formatSelect parameter.
//
//*****************************************************************************
#define RTCC_FORMAT_BINARY  ( !(RTCBCD) )
#define RTCC_FORMAT_BCD     (RTCBCD)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_counterInit()
//in the clockSelect parameter.
//
//*****************************************************************************
#define RTCC_CLOCKSELECT_ACLK  (RTCSSEL_0)
#define RTCC_CLOCKSELECT_SMCLK (RTCSSEL_1)
#define RTCC_CLOCKSELECT_RT1PS (RTCSSEL_2)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_counterInit()
//in the counterSizeSelect parameter.
//
//*****************************************************************************
#define RTCC_COUNTERSIZE_8BIT  (RTCTEV_0)
#define RTCC_COUNTERSIZE_16BIT (RTCTEV_1)
#define RTCC_COUNTERSIZE_24BIT (RTCTEV_2)
#define RTCC_COUNTERSIZE_32BIT (RTCTEV_3)

//*****************************************************************************
//
//The following is a value that can be passed to RTCC_setCalendarAlarm() in the
//minutesAlarm, hoursAlarm, dayOfWeekAlarm, and dayOfMonthAlarm parameters.
//
//*****************************************************************************
#define RTCC_ALARMCONDITION_OFF  (0x80)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_setCalendarEvent()
//in the eventSelect parameter.
//
//*****************************************************************************
#define RTCC_CALENDAREVENT_MINUTECHANGE  (RTCTEV_0)
#define RTCC_CALENDAREVENT_HOURCHANGE    (RTCTEV_1)
#define RTCC_CALENDAREVENT_NOON          (RTCTEV_2)
#define RTCC_CALENDAREVENT_MIDNIGHT      (RTCTEV_3)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_counterPrescaleInit(),
//RTCC_definePreScaleEvent(), RTCC_readPrescaleCounterValue(), and
//RTCC_setPrescaleCounterValue() in the prescaleSelect parameter.
//
//*****************************************************************************
#define RTCC_PRESCALE_0  (0x0)
#define RTCC_PRESCALE_1  (0x2)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_counterPrescaleInit()
//in the prescaleClockSelect parameter.
//
//*****************************************************************************
#define RTCC_PSCLOCKSELECT_ACLK  (RT1SSEL_0)
#define RTCC_PSCLOCKSELECT_SMCLK (RT1SSEL_1)
#define RTCC_PSCLOCKSELECT_RT0PS (RT1SSEL_2)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_counterPrescaleInit()
//in the prescaleDivider parameter.
//
//*****************************************************************************
#define RTCC_PSDIVIDER_2   (RT0PSDIV_0)
#define RTCC_PSDIVIDER_4   (RT0PSDIV_1)
#define RTCC_PSDIVIDER_8   (RT0PSDIV_2)
#define RTCC_PSDIVIDER_16  (RT0PSDIV_3)
#define RTCC_PSDIVIDER_32  (RT0PSDIV_4)
#define RTCC_PSDIVIDER_64  (RT0PSDIV_5)
#define RTCC_PSDIVIDER_128 (RT0PSDIV_6)
#define RTCC_PSDIVIDER_256 (RT0PSDIV_7)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_definePrescaleEvent()
//in the prescaleEventDivider parameter.
//
//*****************************************************************************
#define RTCC_PSEVENTDIVIDER_2   (RT0IP_0)
#define RTCC_PSEVENTDIVIDER_4   (RT0IP_1)
#define RTCC_PSEVENTDIVIDER_8   (RT0IP_2)
#define RTCC_PSEVENTDIVIDER_16  (RT0IP_3)
#define RTCC_PSEVENTDIVIDER_32  (RT0IP_4)
#define RTCC_PSEVENTDIVIDER_64  (RT0IP_5)
#define RTCC_PSEVENTDIVIDER_128 (RT0IP_6)
#define RTCC_PSEVENTDIVIDER_256 (RT0IP_7)

//*****************************************************************************
//
//The following are values that can be passed to RTCC_getInterruptStatus(),
//RTCC_clearInterrupt(), RTCC_enableInterrupt(),  RTCC_disableInterrupt()
//in the interruptFlagMask parameter.
//
//*****************************************************************************
#define RTCC_OSCILLATOR_FAULT_INTERRUPT  RTCOFIE
#define RTCC_TIME_EVENT_INTERRUPT        RTCTEVIE
#define RTCC_CLOCK_ALARM_INTERRUPT       RTCAIE
#define RTCC_CLOCK_READ_READY_INTERRUPT  RTCRDYIE
#define RTCC_PRESCALE_TIMER0_INTERRUPT   0x02
#define RTCC_PRESCALE_TIMER1_INTERRUPT   0x01


//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern void RTCC_startClock (unsigned int baseAddress);

extern void RTCC_holdClock (unsigned int baseAddress);

extern void RTCC_setCalibrationFrequency (unsigned int baseAddress,
    unsigned int frequencySelect);

extern void RTCC_setCalibrationData (unsigned int baseAddress,
    unsigned char offsetDirection,
    unsigned char offsetValue);
    
extern unsigned short RTCC_setTemperatureCompensation(unsigned int baseAddress,
	unsigned char offsetDirection,
	unsigned char offsetValue);

extern void RTCC_calendarInit (unsigned int baseAddress,
    Calendar CalendarTime,
    unsigned int formatSelect);

extern Calendar RTCC_getCalendarTime (unsigned int baseAddress);

extern void RTCC_setCalendarAlarm (unsigned int baseAddress,
    unsigned char minutesAlarm,
    unsigned char hoursAlarm,
    unsigned char dayOfWeekAlarm,
    unsigned char dayOfMonthAlarm);

extern void RTCC_setCalendarEvent (unsigned int baseAddress,
    unsigned int eventSelect);

extern void RTCC_definePrescaleEvent (unsigned int baseAddress,
    unsigned char prescaleSelect,
    unsigned char prescaleEventDivider);

extern unsigned char RTCC_getPrescaleValue (unsigned int baseAddress,
    unsigned char prescaleSelect);

extern void RTCC_setPrescaleValue (unsigned int baseAddress,
    unsigned char prescaleSelect,
    unsigned char prescaleCounterValue);

extern void RTCC_enableInterrupt (unsigned int baseAddress,
    unsigned char interruptMask);

extern void RTCC_disableInterrupt (unsigned int baseAddress,
    unsigned char interruptMask);

extern unsigned char RTCC_getInterruptStatus (unsigned int baseAddress,
    unsigned char interruptFlagMask);

extern void RTCC_clearInterrupt (unsigned int baseAddress,
    unsigned char interruptFlagMask);

extern unsigned int RTCC_convertBCDToBinary (unsigned int baseAddressu,
    unsigned int valueToConvert);

extern unsigned int RTCC_convertBinaryToBCD (unsigned int baseAddress,
    unsigned int valueToConvert);

#endif
