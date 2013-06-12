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
 *  USB Device Descriptors, for library use when in USB device mode. Descriptors are special
 *  computer-readable structures which the host requests upon device enumeration, to determine
 *  the device's capabilities and functions.
 */

#include "Descriptors.h"


/** HID class report descriptor. This is a special descriptor constructed with values from the
 *  USBIF HID class specification to describe the reports and capabilities of the HID device. This
 *  descriptor is parsed by the host and its contents used to determine what data (and in what encoding)
 *  the device will send, and what it may be sent back from the host. Refer to the HID specification for
 *  more details on HID report descriptors.
 *
 *  This descriptor describes the multiple possible reports of the HID interface's report structure.
 */
const USB_Descriptor_HIDReport_Datatype_t PROGMEM KeyboardMouseReport[] =
{
        /* Mouse Report */
        HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
        HID_RI_USAGE(8, 0x02), /* Mouse */
        HID_RI_COLLECTION(8, 0x01), /* Application */
                HID_RI_REPORT_ID(8, HID_REPORTID_MouseReport),
            HID_RI_USAGE(8, 0x01), /* Pointer */
            HID_RI_COLLECTION(8, 0x00), /* Physical */
                HID_RI_USAGE_PAGE(8, 0x09), /* Button */
                HID_RI_USAGE_MINIMUM(8, 0x01),
                HID_RI_USAGE_MAXIMUM(8, 0x03),
                HID_RI_LOGICAL_MINIMUM(8, 0x00),
                HID_RI_LOGICAL_MAXIMUM(8, 0x01),
                HID_RI_REPORT_COUNT(8, 0x03),
                HID_RI_REPORT_SIZE(8, 0x01),
                HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
                HID_RI_REPORT_COUNT(8, 0x01),
                HID_RI_REPORT_SIZE(8, 0x05),
                HID_RI_INPUT(8, HID_IOF_CONSTANT),
                HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
                HID_RI_USAGE(8, 0x30), /* Usage X */
                HID_RI_USAGE(8, 0x31), /* Usage Y */
                HID_RI_LOGICAL_MINIMUM(8, -1),
                HID_RI_LOGICAL_MAXIMUM(8, 1),
                HID_RI_PHYSICAL_MINIMUM(8, -1),
                HID_RI_PHYSICAL_MAXIMUM(8, 1),
                HID_RI_REPORT_COUNT(8, 0x02),
                HID_RI_REPORT_SIZE(8, 0x08),
                HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),
            HID_RI_END_COLLECTION(0),
        HID_RI_END_COLLECTION(0),

        /* Keyboard Report */
        HID_RI_USAGE_PAGE(8, 0x01), /* Generic Desktop */
        HID_RI_USAGE(8, 0x06), /* Keyboard */
        HID_RI_COLLECTION(8, 0x01), /* Application */
                HID_RI_REPORT_ID(8, HID_REPORTID_KeyboardReport),
            HID_RI_USAGE_PAGE(8, 0x07), /* Key Codes */
            HID_RI_USAGE_MINIMUM(8, 0xE0), /* Keyboard Left Control */
            HID_RI_USAGE_MAXIMUM(8, 0xE7), /* Keyboard Right GUI */
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x01),
            HID_RI_REPORT_COUNT(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_CONSTANT),
            HID_RI_USAGE_PAGE(8, 0x08), /* LEDs */
            HID_RI_USAGE_MINIMUM(8, 0x01), /* Num Lock */
            HID_RI_USAGE_MAXIMUM(8, 0x05), /* Kana */
            HID_RI_REPORT_COUNT(8, 0x05),
            HID_RI_REPORT_SIZE(8, 0x01),
            HID_RI_OUTPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE | HID_IOF_NON_VOLATILE),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x03),
            HID_RI_OUTPUT(8, HID_IOF_CONSTANT),
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(8, 0x65),
            HID_RI_USAGE_PAGE(8, 0x07), /* Keyboard */
            HID_RI_USAGE_MINIMUM(8, 0x00), /* Reserved (no event indicated) */
            HID_RI_USAGE_MAXIMUM(8, 0x65), /* Keyboard Application */
            HID_RI_REPORT_COUNT(8, 0x06),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
        HID_RI_END_COLLECTION(0),
};


/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(01.10),
        .Class                  = USB_CSCP_IADDeviceClass,
        .SubClass               = USB_CSCP_IADDeviceSubclass,
        .Protocol               = USB_CSCP_IADDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x16D0,
	.ProductID              = 0x0557,
	.ReleaseNumber          = VERSION_BCD(00.01),

	.ManufacturerStrIndex   = 0x01,
	.ProductStrIndex        = 0x02,
	.SerialNumStrIndex      = 0x03,
//        .SerialNumStrIndex      = USE_INTERNAL_SERIAL,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
        .Config =
                {
                        .Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

                        .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
                        .TotalInterfaces        = 3,

                        .ConfigurationNumber    = 1,
                        .ConfigurationStrIndex  = NO_DESCRIPTOR,

                        .ConfigAttributes       = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

                        .MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
                },

        .CDC_IAD =
                {
                        .Header                 = {.Size = sizeof(USB_Descriptor_Interface_Association_t), .Type = DTYPE_InterfaceAssociation},

                        .FirstInterfaceIndex    = 0,
                        .TotalInterfaces        = 2,

                        .Class                  = CDC_CSCP_CDCClass,
                        .SubClass               = CDC_CSCP_ACMSubclass,
                        .Protocol               = CDC_CSCP_ATCommandProtocol,

                        .IADStrIndex            = NO_DESCRIPTOR
                },

	.CDC_CCI_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 0,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 1,

			.Class                  = CDC_CSCP_CDCClass,
			.SubClass               = CDC_CSCP_ACMSubclass,
			.Protocol               = CDC_CSCP_ATCommandProtocol,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.CDC_Functional_Header =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalHeader_t), .Type = DTYPE_CSInterface},
			.Subtype                = CDC_DSUBTYPE_CSInterface_Header,

			.CDCSpecification       = VERSION_BCD(01.10),
		},

	.CDC_Functional_ACM =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalACM_t), .Type = DTYPE_CSInterface},
			.Subtype                = CDC_DSUBTYPE_CSInterface_ACM,

			.Capabilities           = 0x06,
		},

	.CDC_Functional_Union =
		{
			.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalUnion_t), .Type = DTYPE_CSInterface},
			.Subtype                = CDC_DSUBTYPE_CSInterface_Union,

			.MasterInterfaceNumber  = 0,
			.SlaveInterfaceNumber   = 1,
		},

	.CDC_NotificationEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CDC_NOTIFICATION_EPADDR,
			.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CDC_NOTIFICATION_EPSIZE,
			.PollingIntervalMS      = 0xFF
		},

	.CDC_DCI_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 1,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 2,

			.Class                  = CDC_CSCP_CDCDataClass,
			.SubClass               = CDC_CSCP_NoDataSubclass,
			.Protocol               = CDC_CSCP_NoDataProtocol,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

	.CDC_DataOutEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CDC_RX_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CDC_TXRX_EPSIZE,
			.PollingIntervalMS      = 0x05
		},

	.CDC_DataInEndpoint =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = CDC_TX_EPADDR,
			.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
			.EndpointSize           = CDC_TXRX_EPSIZE,
			.PollingIntervalMS      = 0x05
		},

        .HID_Interface =
                {
                        .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

                        .InterfaceNumber        = 2,
                        .AlternateSetting       = 0,

                        .TotalEndpoints         = 1,

                        .Class                  = HID_CSCP_HIDClass,
                        .SubClass               = HID_CSCP_NonBootSubclass,
                        .Protocol               = HID_CSCP_NonBootProtocol,

                        .InterfaceStrIndex      = NO_DESCRIPTOR
                },

        .HID_KeyboardMouseHID =
                {
                        .Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

                        .HIDSpec                = VERSION_BCD(01.11),
                        .CountryCode            = 0x00,
                        .TotalReportDescriptors = 1,
                        .HIDReportType          = HID_DTYPE_Report,
                        .HIDReportLength        = sizeof(KeyboardMouseReport)
                },

        .HID_ReportINEndpoint =
                {
                        .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

                        .EndpointAddress        = KEYBOARDMOUSE_EPADDR,
                        .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
                        .EndpointSize           = KEYBOARDMOUSE_EPSIZE,
                        .PollingIntervalMS      = 0x05
                }
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
const USB_Descriptor_String_t PROGMEM LanguageString =
{
	.Header                 = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},
		
	.UnicodeString          = {LANGUAGE_ID_ENG}
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM ManufacturerString =
{
	.Header                 = {.Size = USB_STRING_LEN(22), .Type = DTYPE_String},
		
	.UnicodeString          = L"Dynamic Perception LLC"
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM ProductString =
{
	.Header                 = {.Size = USB_STRING_LEN(29), .Type = DTYPE_String},
		
	.UnicodeString          = L"Dynamic Perception CDC Serial"
};

const USB_Descriptor_String_t PROGMEM SerialString =
{
	.Header                 = {.Size = USB_STRING_LEN(6), .Type = DTYPE_String},
#if defined(__AVR_ATmega32U4__)
	.UnicodeString          = L"004572"
#elif defined(__AVR_ATmega32U2__)
	.UnicodeString          = L"004571"
#elif defined(__AVR_AT90USB646__)
        .UnicodeString          = L"004574"
#elif defined(__AVR_AT90USB1286__)
        .UnicodeString          = L"004575"
#else
	.UnicodeString          = L"004573"
#endif
};

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

        const void* Address = NULL;
        uint16_t    Size    = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
		case DTYPE_Device: 
			Address = &DeviceDescriptor;
			Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration: 
			Address = &ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
		case DTYPE_String: 
			switch (DescriptorNumber)
			{
				case 0x00: 
					Address = &LanguageString;
					Size    = pgm_read_byte(&LanguageString.Header.Size);
					break;
				case 0x01: 
					Address = &ManufacturerString;
					Size    = pgm_read_byte(&ManufacturerString.Header.Size);
					break;
				case 0x02: 
					Address = &ProductString;
					Size    = pgm_read_byte(&ProductString.Header.Size);
					break;
				case 0x03:
					Address = &SerialString;
					Size    = pgm_read_byte(&SerialString.Header.Size);
					break;
			}
			
			break;
                case HID_DTYPE_HID:
                        Address = &ConfigurationDescriptor.HID_KeyboardMouseHID;
                        Size    = sizeof(USB_HID_Descriptor_HID_t);
                        break;
                case HID_DTYPE_Report:
                        Address = &KeyboardMouseReport;
                        Size    = sizeof(KeyboardMouseReport);
                        break;
	}
	
	*DescriptorAddress = Address;
	return Size;
}
