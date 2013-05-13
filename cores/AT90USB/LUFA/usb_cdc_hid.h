/*
  Copyright (C) Dean Camera, 2009-2013 (www.fourwalledcubicle.com)
  Copyright (C) 2013  Justin Mattair (www.mattairtech.com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef _USB_CDC_HID_H_
#define _USB_CDC_HID_H_

/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

/** ************** Includes ************** **/
#include <avr/io.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "Board.h"
#include "Descriptors.h"
#include "USB.h"


#define USB_LED_DISABLED		0x01000000UL		// put in upper byte
#define USB_LED_ENABLED			0

#define USB_AUTOFLUSH_DISABLED	        0x02000000UL		// put in upper byte
#define USB_AUTOFLUSH_ENABLED		0

#define USB_WAITFORCONNECT_DISABLED     0x04000000UL            // put in upper byte
#define USB_WAITFORCONNECT_ENABLED      0

#define USB_AUTORESET_DISABLED          0x08000000UL            // put in upper byte
#define USB_AUTORESET_ENABLED           0

#define USB_TASKS_INTERVAL_MS 	        50
#define USB_TICKSTILLRESET_MS           100
#define USB_TICKSTILLRESET_TICKS        (USB_TICKSTILLRESET_MS / USB_TASKS_INTERVAL_MS) + 1

typedef void (*AppPtr_t)(void) __attribute__ ((noreturn));


/** ************** Function Prototypes ************** **/
void set_blink_LED(void);
void Set_USB_LED(bool LEDState);
void Reboot(void);

/* USB Functions */
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_Suspend(void);
void EVENT_USB_Device_Wake(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize);
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize);
bool HID_SendReport (uint8_t  ReportID);

#if defined(INCLUDE_FROM_USB_CDC_HID_C)
	
#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif
