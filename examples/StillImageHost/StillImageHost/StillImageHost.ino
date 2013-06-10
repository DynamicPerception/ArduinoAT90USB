/*

             LUFA Library

     Copyright (C) Dean Camera, 2012.



  dean [at] fourwalledcubicle [dot] com

           www.lufa-lib.org

*/



/*

  Copyright 2012  Dean Camera (dean [at] fourwalledcubicle [dot] com)



  Permission to use, copy, modify, distribute, and sell this

  software and its documentation for any purpose is hereby granted

  without fee, provided that the above copyright notice appear in

  all copies and that both that the copyright notice and this

  permission notice and warranty disclaimer appear in supporting

  documentation, and that the name of the author not be used in

  advertising or publicity pertaining to distribution of the

  software without specific, written prior permission.



  The author disclaim all warranties with regard to this

  software, including all implied warranties of merchantability

  and fitness.  In no event shall the author be liable for any

  special, indirect or consequential damages or any damages

  whatsoever resulting from loss of use, data or profits, whether

  in an action of contract, negligence or other tortious action,

  arising out of or in connection with the use or performance of

  this software.

*/



/** \file

 *

 *  Main source file for the StillImageHost demo. This file contains the main tasks of

 *  the demo and is responsible for the initial application hardware configuration.

 */


  #include <LUFA/Drivers/USB/USB.h>
  #include <LUFA/Drivers/USB/Class/Host/StillImageClassHost.h>



/* NOTE: Arduino's compiler does not support designated initializers easily */

/** LUFA Still Image Class driver interface configuration and state information. This structure is
 *  passed to all Still Image Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */

USB_ClassInfo_SI_Host_t DigitalCamera_SI_Interface = {
		{
		  {
		    (PIPE_DIR_IN  | 1),
		    1,
		  },
		  {
		    (PIPE_DIR_OUT | 2),
		    1,
		  },
		 {
		    (PIPE_DIR_IN  | 3),
		    1,
		 },
	       },
};


void setup() {
  SetupHardware();
}

void loop() {
  StillImageHost_Task();
  SI_Host_USBTask(&DigitalCamera_SI_Interface);
  USB_USBTask();
}


int main(void) {
  sei();
  setup();
  while(1)
   loop(); 
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */

void SetupHardware(void) {

	/* Disable watchdog if enabled by bootloader/fuses */

	//MCUSR &= ~(1 << WDRF);
	//wdt_disable();

	/* Disable clock division */

	//clock_prescale_set(clock_div_1);

	/* Hardware Initialization */


	USB_Init(USB_MODE_UID, USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL);


}



/** Task to manage an enumerated USB Still Image device once connected, to manage a
 *  new PIMA session in order to send commands to the attached device.
 */

void StillImageHost_Task(void) {

	if (USB_HostState != HOST_STATE_Configured)
	  return;

	if (SI_Host_OpenSession(&DigitalCamera_SI_Interface) != PIPE_RWSTREAM_NoError) {
                  // error occurred
		USB_Host_SetDeviceConfiguration(0);
		return;
	}

	SI_Host_SendCommand(&DigitalCamera_SI_Interface, 0x1013, 0, NULL);

	if (SI_Host_ReceiveResponse(&DigitalCamera_SI_Interface)) {
		USB_Host_SetDeviceConfiguration(0);
		return;
	}

	if (SI_Host_CloseSession(&DigitalCamera_SI_Interface) != PIPE_RWSTREAM_NoError)	{
                  // error occurred
		USB_Host_SetDeviceConfiguration(0);
		return;
	}


	USB_Host_SetDeviceConfiguration(0);

}



/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */

void EVENT_USB_Host_DeviceAttached(void) {
}



/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */

void EVENT_USB_Host_DeviceUnattached(void) {
}



/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */

void EVENT_USB_Host_DeviceEnumerationComplete(void) {

	uint16_t ConfigDescriptorSize;
	uint8_t  ConfigDescriptorData[512];

	if (USB_Host_GetDeviceConfigDescriptor(1, &ConfigDescriptorSize, ConfigDescriptorData, sizeof(ConfigDescriptorData)) != HOST_GETCONFIG_Successful) {
		return;
	}



	if (SI_Host_ConfigurePipes(&DigitalCamera_SI_Interface, ConfigDescriptorSize, ConfigDescriptorData) != SI_ENUMERROR_NoError)	{
		return;
	}



	if (USB_Host_SetDeviceConfiguration(1) != HOST_SENDCONTROL_Successful) {
		return;
	}

}



/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */

void EVENT_USB_Host_HostError(const uint8_t ErrorCode) {

	USB_Disable();

/*	printf_P(PSTR(ESC_FG_RED "Host Mode Error\r\n"

	                         " -- Error Code %d\r\n" ESC_FG_WHITE), ErrorCode);

*/

	for(;;);

}



/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */

void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t ErrorCode, const uint8_t SubErrorCode) {

/*	printf_P(PSTR(ESC_FG_RED "Dev Enum Error\r\n"

	                         " -- Error Code %d\r\n"

	                         " -- Sub Error Code %d\r\n"

	                         " -- In State %d\r\n" ESC_FG_WHITE), ErrorCode, SubErrorCode, USB_HostState);
*/

}


uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,

                                    const uint8_t wIndex,

                                    const void** const DescriptorAddress) {
  
  return 0;
}



