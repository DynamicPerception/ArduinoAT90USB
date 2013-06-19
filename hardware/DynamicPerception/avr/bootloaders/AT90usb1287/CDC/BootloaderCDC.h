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
 *  Header file for BootloaderCDC.c.
 */

#ifndef _CDC_H_
#define _CDC_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/boot.h>
		#include <avr/eeprom.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <stdbool.h>

		#include "Descriptors.h"
		#include "BootloaderAPI.h"
		#include "Config/AppConfig.h"

	/* Macros: */
		/** Version major of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MAJOR     0x01

		/** Version minor of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MINOR     0x00

		/** Hardware version major of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MAJOR   0x01

		/** Hardware version minor of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MINOR   0x00

		/** Eight character bootloader firmware identifier reported to the host when requested */
		#define SOFTWARE_IDENTIFIER          "LUFACDC"
		
	/* Type Defines: */
		/** Type define for a non-returning pointer to the start of the loaded application in flash memory. */
		typedef void (*AppPtr_t)(void) ATTR_NO_RETURN;

	/* Function Prototypes: */
		static void CDC_Task(void);
		
		void BL_Pulse(void);

		void EVENT_USB_Device_ConfigurationChanged(void);

		#if defined(INCLUDE_FROM_BOOTLOADERCDC_C) || defined(__DOXYGEN__)
			#if !defined(NO_BLOCK_SUPPORT)
			static void    ReadWriteMemoryBlock(const uint8_t Command);
			#endif
			static uint8_t FetchNextCommandByte(void);
			static void    WriteNextResponseByte(const uint8_t Response);
		#endif
		
	/* Defintions for each board*/
	#if defined(BOARD_MX3)
		#define BUTTON_DIR_PORT		DDRF
		#define BUTTON_PORT			PORTF
		#define BUTTON_PIN			PF0
		#define BUTTON_INPUT		PINF
		#define LED_DIR_PORT		DDRC
		#define LED_PORT			PORTC
		#define LED_PIN				PC6
	#elif defined(BOARD_MOCO_DEV)
		#define BUTTON_DIR_PORT		DDRB
		#define BUTTON_PORT			PORTB
		#define BUTTON_PIN			PB7
		#define BUTTON_INPUT		PINB
		#define LED_DIR_PORT		DDRA
		#define LED_PORT			PORTA
		#define LED_PIN				PA0
	#elif defined(BOARD_USBKEY)
		#define BUTTON_DIR_PORT		DDRB
		#define BUTTON_PORT			PORTB
		#define BUTTON_PIN			PB5
		#define BUTTON_INPUT		PINB
		#define LED_DIR_PORT		DDRC
		#define LED_PORT			PORTC
		#define LED_PIN				PC6
	#else
		#define BUTTON_DIR_PORT		DDRE
		#define BUTTON_PORT			PORTE
		#define BUTTON_PIN			PE2
		#define BUTTON_INPUT		PINE
		#define LED_DIR_PORT		DDRA
		#define LED_PORT			PORTA
		#define LED_PIN				PA0
	#endif
#endif

