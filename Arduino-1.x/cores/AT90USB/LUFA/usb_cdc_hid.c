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

#define INCLUDE_FROM_USB_CDC_HID_C

#include "usb_cdc_hid.h"


CDC_LineEncoding_t LineEncoding = { .BaudRateBPS = 57600,
                                    .CharFormat  = CDC_LINEENCODING_OneStopBit,
                                    .ParityType  = CDC_PARITY_None,
                                    .DataBits    = 8                            };
uint8_t LineState = 0;
volatile uint8_t stateLED;
volatile bool USBLEDDisabled;
volatile bool USBAutoflushDisabled;
volatile bool USBAutoResetDisabled;
volatile uint8_t USBTasksCountMS;
volatile uint8_t ticksTillReset = 0;
bool firstConnected;
USB_MouseReport_Data_t MouseReport;
USB_KeyboardReport_Data_t KeyboardReport;
volatile bool HID_SendReport_Active = false;


/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Device_HID_Interface =
{
  .Config =
  {
    .InterfaceNumber              = 2,
    .ReportINEndpoint             =
    {
      .Address              = KEYBOARDMOUSE_EPADDR,
      .Size                 = KEYBOARDMOUSE_EPSIZE,
      .Banks                = 1,
    },
    .PrevReportINBuffer           = NULL,
    .PrevReportINBufferSize       = MAX(sizeof(USB_KeyboardReport_Data_t), sizeof(USB_MouseReport_Data_t)),
  },
};



// This must fire about once per millisecond using timer 0
ISR(TIMER0_COMPB_vect)
{
	if (++USBTasksCountMS > USB_TASKS_INTERVAL_MS) {
		USBTasksCountMS = 0;
		
		if (!USBAutoflushDisabled) {
			uint8_t prevEndpoint = Endpoint_GetCurrentEndpoint();
                        Endpoint_SelectEndpoint(CDC_TX_EPADDR);
			Endpoint_ClearIN();
			Endpoint_SelectEndpoint(prevEndpoint);
		}
	
		if (!USBLEDDisabled) {
			if (stateLED == 1) {
				stateLED = 0;	// end inter-blink delay, ready for new blink
			} else if (stateLED == 2) {
				stateLED = 1;	// end blink, begin inter-blink delay
				LED_On();
			}
		}

                if (!USBAutoResetDisabled && ticksTillReset) {
                        ticksTillReset--;
                        if (ticksTillReset == 0) {
                                Reboot();
                        }
                }
	}
}


void set_blink_LED(void)
{
	if (!USBLEDDisabled) {
		if (stateLED == 0) {
			USBTasksCountMS = 0;
			stateLED = 2;	// start blink
			LED_Off();
		}
	}
}

void Set_USB_LED(bool LEDState)
{
	if (!USBLEDDisabled) {
		if (LEDState) {
			LED_On();
		} else {
			LED_Off();
		}
	}
}

void Reboot(void)
{
        /* Disconnect from the host - USB interface will be reset later along with the AVR */
        USB_Detach();

        cli();
        
        wdt_enable(WDTO_60MS);
        
        *(uint16_t *)0x0280 = 0x7777;

        while(1);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	Set_USB_LED(true);
	firstConnected = true;

      // enable timer 0 COMPB interrupt for USB
      TIMSK0 |= (1 << OCIE0B);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	// disable timer 0 COMPB interrupt for USB
	//TIMSK0 &= ~(1 << OCIE0B);
	
	Set_USB_LED(false);
}

void EVENT_USB_Device_Suspend (void)
{
	Set_USB_LED(false);
}

void EVENT_USB_Device_WakeUp (void)
{
	Set_USB_LED(true);
}



/** Event handler for the library USB Configuration Changed event. */
// Remember to configure endpoints in numerical order when using ORDERED_EP_CONFIG
void EVENT_USB_Device_ConfigurationChanged(void)
{
	/* Indicate USB connected and ready */
	Set_USB_LED(true);
        
        /* Setup HID KeyboardMouse Endpoint */
        if (!HID_Device_ConfigureEndpoints(&Device_HID_Interface))
        {
                Set_USB_LED(false);
        }
	
	/* Setup CDC Notification, Rx and Tx Endpoints */
        if (!(Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT,
              CDC_NOTIFICATION_EPSIZE, 1)))
	{
		Set_USB_LED(false);
	}
	
        if (!(Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 2)))
	{
		Set_USB_LED(false);
	}

        if (!(Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 2)))
	{
		Set_USB_LED(false);
	}
		
	/* Reset line encoding baud rate so that the host knows to send new values */
	LineEncoding.BaudRateBPS = 0;
        
        USB_Device_EnableSOFEvents();
}

/** Event handler for the USB_UnhandledControlRequest event. This is used to catch standard and class specific
 *  control requests that are not handled internally by the USB library (including the CDC control commands,
 *  which are all issued via the control endpoint), so that they can be handled appropriately for the application.
 */
void EVENT_USB_Device_ControlRequest(void)
{
        if (USB_ControlRequest.wIndex == 0)
        {
		/* Process CDC specific control requests */
		switch (USB_ControlRequest.bRequest)
		{
			case CDC_REQ_GetLineEncoding:
				if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
				{
					Endpoint_ClearSETUP();

					/* Write the line coding data to the control endpoint */
					Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
					Endpoint_ClearOUT();
				}
				
				break;
			case CDC_REQ_SetLineEncoding:
				if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
				{
					Endpoint_ClearSETUP();

					/* Read the line coding data in from the host into the global struct */
					Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
					Endpoint_ClearIN();
				}

				break;
			case CDC_REQ_SetControlLineState:
				if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
				{
					Endpoint_ClearSETUP();
					Endpoint_ClearStatusStage();
                                        
                                        //_usbLineInfo.lineState = setup.wValueL;
                                        LineState = (uint8_t)(USB_ControlRequest.wValue & (CDC_CONTROL_LINE_OUT_DTR));
                                        
				        /* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
					lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
				        CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
                                        */
					// auto-reset into the bootloader is triggered when the port, already 
                                        // open at 1200 bps, is closed.  this is the signal to start the watchdog
                                        // with a relatively long period so it can finish housekeeping tasks
                                        // like servicing endpoints before the sketch ends
                                        if (!USBAutoResetDisabled && LineEncoding.BaudRateBPS == 1200) {
                                                // We check DTR state to determine if host port is open (bit 0 of lineState).
                                                if (LineState == 0) {
                                                        if (ticksTillReset == 0) {
                                                                ticksTillReset = USB_TICKSTILLRESET_TICKS;
                                                        }
                                                } else {
                                                        // Most OSs do some intermediate steps when configuring ports and DTR can
                                                        // twiggle more than once before stabilizing.
                                                        // To avoid spurious resets we set the watchdog to 250ms and eventually
                                                        // cancel if DTR goes back high.

                                                        if (ticksTillReset != 0) {
                                                                ticksTillReset = 0;     // reset will not occur
                                                        }
                                                }
					}
				}
				break;
		}
        } else {
                HID_Device_ProcessControlRequest(&Device_HID_Interface);
        }
}


/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
        HID_Device_MillisecondElapsed(&Device_HID_Interface);
        
        if (Device_HID_Interface.State.IdleCount && !(Device_HID_Interface.State.IdleMSRemaining)) {
                if (HID_SendReport_Active == false) {
                        HID_SendReport(HID_REPORTID_KeyboardReport);
                        HID_SendReport(HID_REPORTID_MouseReport);
                } else {
                        Device_HID_Interface.State.IdleMSRemaining = 0;
                }
        }
}


/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean true to force the sending of the report, false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
        if (*ReportID == HID_REPORTID_MouseReport)
        {
                USB_MouseReport_Data_t* mouseReport = (USB_MouseReport_Data_t*)ReportData;

                mouseReport->Y = MouseReport.Y;
                mouseReport->X = MouseReport.X;
                mouseReport->Button = MouseReport.Button;

                //*ReportID   = HID_REPORTID_MouseReport;
                *ReportSize = sizeof(USB_MouseReport_Data_t);
                return true;
        }
        else
        {
                USB_KeyboardReport_Data_t* keyboardReport = (USB_KeyboardReport_Data_t*)ReportData;

                keyboardReport->Modifier = KeyboardReport.Modifier;

                keyboardReport->KeyCode[0] = KeyboardReport.KeyCode[0];
                keyboardReport->KeyCode[1] = KeyboardReport.KeyCode[1];
                keyboardReport->KeyCode[2] = KeyboardReport.KeyCode[2];
                keyboardReport->KeyCode[3] = KeyboardReport.KeyCode[3];
                keyboardReport->KeyCode[4] = KeyboardReport.KeyCode[4];
                keyboardReport->KeyCode[5] = KeyboardReport.KeyCode[5];

                *ReportID   = HID_REPORTID_KeyboardReport;
                *ReportSize = sizeof(USB_KeyboardReport_Data_t);
                return false;
        }
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
/*        uint8_t  LEDMask   = LEDS_NO_LEDS;
        uint8_t* LEDReport = (uint8_t*)ReportData;

        if (*LEDReport & HID_KEYBOARD_LED_NUMLOCK)
          LEDMask |= LEDS_LED1;

        if (*LEDReport & HID_KEYBOARD_LED_CAPSLOCK)
          LEDMask |= LEDS_LED3;

        if (*LEDReport & HID_KEYBOARD_LED_SCROLLLOCK)
          LEDMask |= LEDS_LED4;

        LEDs_SetAllLEDs(LEDMask);*/
}

bool HID_SendReport (uint8_t  ReportID)
{
        HID_SendReport_Active = true;
  
        uint8_t  ReportINData[Device_HID_Interface.Config.PrevReportINBufferSize];
        uint16_t ReportINSize = 0;
        uint8_t success = false;

        memset(ReportINData, 0, sizeof(ReportINData));

        CALLBACK_HID_Device_CreateHIDReport(&Device_HID_Interface, &ReportID, HID_REPORT_ITEM_In,
                                            ReportINData, &ReportINSize);
        
        uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
	  GlobalInterruptDisable();

                uint8_t prevEndpoint = Endpoint_GetCurrentEndpoint();
                Endpoint_SelectEndpoint(KEYBOARDMOUSE_EPADDR);
        
                if (!Endpoint_IsReadWriteAllowed()) {
                        Endpoint_ClearIN();
                        if (!Endpoint_WaitUntilReady()) {
                                 success = true;
                        }
                } else {
                        success = true;
                }

                if (success) {
                		if (ReportID)
                        	Endpoint_Write_8(ReportID);
                
                		if (Endpoint_Write_Stream_LE(ReportINData, ReportINSize, NULL)) {
					success = false;
				}

                		Endpoint_ClearIN();
                }

		    Endpoint_SelectEndpoint(prevEndpoint);
                Device_HID_Interface.State.IdleMSRemaining = Device_HID_Interface.State.IdleCount;

        SetGlobalInterruptMask(CurrentGlobalInt);
        
        HID_SendReport_Active = false;
        return (success);
}


