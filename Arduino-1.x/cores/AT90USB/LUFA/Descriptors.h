/*
             LUFA Library
     Copyright (C) Dean Camera, 2013.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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

/*
  Modified 22 March 2013 by Justin Mattair
     for MattairTech MT-DB-Ux boards (www.mattairtech.com)
*/

/** \file
 *
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Includes: */
		#include <avr/pgmspace.h>

                #include "USB.h"
		#define __INCLUDE_FROM_HID_DRIVER
		#include "HIDClassCommon.h"

	/* Macros: */
                /** Endpoint address of the KeyboardMouse HID reporting IN endpoint. */
                #define KEYBOARDMOUSE_EPADDR           (ENDPOINT_DIR_IN  | 1)

                /** Endpoint address of the CDC device-to-host notification IN endpoint. */
                #define CDC_NOTIFICATION_EPADDR        (ENDPOINT_DIR_IN  | 2)

                /** Endpoint address of the CDC device-to-host data IN endpoint. */
                #define CDC_TX_EPADDR                  (ENDPOINT_DIR_IN  | 3)

                /** Endpoint address of the CDC host-to-device data OUT endpoint. */
                #define CDC_RX_EPADDR                  (ENDPOINT_DIR_OUT | 4)

                /** Size in bytes of the CDC device-to-host notification IN endpoint. */
                #define CDC_NOTIFICATION_EPSIZE        8

                /** Size in bytes of the CDC data IN and OUT endpoints. */
                /* The CDC TX and RX endpoints are setup in usb_cdc_hid.c to be dual-banked */
                #define CDC_TXRX_EPSIZE                32

                /** Size in bytes of the KeyboardMouse HID reporting IN endpoint. */
                #define KEYBOARDMOUSE_EPSIZE           24

	/* Type Defines: */
                /** Type define for the device configuration descriptor structure. This must be defined in the
                 *  application code, as the configuration descriptor contains several sub-descriptors which
                 *  vary between devices, and which describe the device's usage to the host.
                 */
                typedef struct
                {
                        USB_Descriptor_Configuration_Header_t    Config;

                        // CDC Control Interface
                        USB_Descriptor_Interface_Association_t   CDC_IAD;
                        USB_Descriptor_Interface_t               CDC_CCI_Interface;
                        USB_CDC_Descriptor_FunctionalHeader_t    CDC_Functional_Header;
                        USB_CDC_Descriptor_FunctionalACM_t       CDC_Functional_ACM;
                        USB_CDC_Descriptor_FunctionalUnion_t     CDC_Functional_Union;
                        USB_Descriptor_Endpoint_t                CDC_NotificationEndpoint;

                        // CDC Data Interface
                        USB_Descriptor_Interface_t               CDC_DCI_Interface;
                        USB_Descriptor_Endpoint_t                CDC_DataOutEndpoint;
                        USB_Descriptor_Endpoint_t                CDC_DataInEndpoint;

                        // KeyboardMouse HID Interface
                        USB_Descriptor_Interface_t               HID_Interface;
                        USB_HID_Descriptor_HID_t                 HID_KeyboardMouseHID;
                        USB_Descriptor_Endpoint_t                HID_ReportINEndpoint;
                } USB_Descriptor_Configuration_t;

                
        /* Enums: */
                /** Enum for the HID report IDs used in the device. */
              //  enum
              //  {
            #define            HID_REPORTID_MouseReport     0x01 /**< Report ID for the Mouse report within the device. */
            #define            HID_REPORTID_KeyboardReport  0x02 /**< Report ID for the Keyboard report within the device. */
              //  } HID_Report_IDs;
                
                
        /* Function Prototypes: */
                uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                                    const uint8_t wIndex,
                                                    const void** const DescriptorAddress)
                                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

