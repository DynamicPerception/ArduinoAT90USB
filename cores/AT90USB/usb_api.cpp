/* Copyright (c) 2011, Peter Barrett  
**  
** Permission to use, copy, modify, and/or distribute this software for  
** any purpose with or without fee is hereby granted, provided that the  
** above copyright notice and this permission notice appear in all copies.  
** 
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
** SOFTWARE.  
*/

/*
  Modified 26 March 2013 by Justin Mattair
     for MattairTech MT-DB-U6 boards (www.mattairtech.com)
*/

/*
  Copyright (C) 2011-2013  Justin Mattair (www.mattairtech.com)

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


#include "usb_api.h"

extern volatile bool USBLEDDisabled;
extern volatile bool USBAutoflushDisabled;
extern volatile bool USBAutoResetDisabled;
extern uint8_t LineState;
extern USB_MouseReport_Data_t MouseReport;
extern USB_KeyboardReport_Data_t KeyboardReport;

uint8_t peekByte = 0;
uint8_t peekByteCnt = 0;


// Constructors ////////////////////////////////////////////////////////////////

//USBSerial::USBSerial()
//{
	
//}


// Public Methods //////////////////////////////////////////////////////////////

void USBSerial::begin(long USBOptions)
{
	if (USBOptions & USB_LED_DISABLED) {
		USBLEDDisabled = true;
	} else {
		USBLEDDisabled = false;
		LED_Init();
	}
	
	if (USBOptions & USB_AUTOFLUSH_DISABLED) {
		USBAutoflushDisabled = true;
	} else {
		USBAutoflushDisabled = false;
	}
        
        if (USBOptions & USB_AUTORESET_DISABLED) {
                USBAutoResetDisabled = true;
        } else {
                USBAutoResetDisabled = false;
        }
        
	USB_Init();
	sei();
        
        if (!(USBOptions & USB_WAITFORCONNECT_DISABLED)) {
                while (!Serial);
        }
}

void USBSerial::end()
{
	USB_Disable();
}

// Not necessary with this implementation
void USBSerial::accept(void)
{
        return;
}

int USBSerial::available(void)
{
        Endpoint_SelectEndpoint(CDC_RX_EPADDR);
	
	if (Endpoint_IsOUTReceived()) {
		if (Endpoint_BytesInEndpoint()) {
			return (unsigned int)Endpoint_BytesInEndpoint() + peekByteCnt;
		} else {
			Endpoint_ClearOUT();
			if (Endpoint_IsOUTReceived()) {
				return (unsigned int)Endpoint_BytesInEndpoint() + peekByteCnt;
			}
		}
	}
	
	return 0 + peekByteCnt;
}

int USBSerial::peek(void)
{
        if (peekByteCnt > 0) {
                return peekByte;
        } else {
                uint8_t byte = readRXEndpoint();
                if (byte >= 0) {
                        peekByteCnt = 1;
                        peekByte = byte;
                }
                return byte;
        }
}

int USBSerial::readRXEndpoint(void)
{
        Endpoint_SelectEndpoint(CDC_RX_EPADDR);
        
        if (Endpoint_IsOUTReceived()) {
                if (Endpoint_BytesInEndpoint()) {
                        return (unsigned char)Endpoint_Read_8();
                } else {
                        Endpoint_ClearOUT();
                        if (Endpoint_IsOUTReceived()) {
                                return (unsigned char)Endpoint_Read_8();
                        }
                }
        }
        
        return -1;
}

int USBSerial::read(void)
{
        if (peekByteCnt > 0) {
                peekByteCnt = 0;
                set_blink_LED();
                return peekByte;
        } else {
                uint8_t byte = readRXEndpoint();
                if (byte >= 0) {
                        set_blink_LED();
                }
                return byte;
        }
}

void USBSerial::flush()
{
	uint8_t prevEndpoint = Endpoint_GetCurrentEndpoint();
	
	Endpoint_SelectEndpoint(CDC_TX_EPADDR);
	bool IsFull = !Endpoint_IsReadWriteAllowed();
	Endpoint_ClearIN();
	
	/* Send an empty packet to ensure that the host does not buffer data sent to it */
	if (IsFull) {
		while (!Endpoint_IsINReady() && USB_DeviceState == DEVICE_STATE_Configured) {
			USB_USBTask();
		}
		Endpoint_ClearIN();
	}
	//Endpoint_WaitUntilReady();
	while (!Endpoint_IsINReady() && USB_DeviceState == DEVICE_STATE_Configured) {
		USB_USBTask();
	}
	
	Endpoint_SelectEndpoint(prevEndpoint);
}

size_t USBSerial::write(uint8_t c)
{
        /* only try to send bytes if the high-level CDC connection itself 
         is open (not just the pipe) - the OS should set lineState when the port
         is opened and clear lineState when the port is closed.
         bytes sent before the user opens the connection or after
         the connection is closed are lost - just like with a UART. */
        
        // TODO - ZE - check behavior on different OSes and test what happens if an
        // open connection isn't broken cleanly (cable is yanked out, host dies
        // or locks up, or host virtual serial port hangs)
        if (LineState > 0) {
                Endpoint_SelectEndpoint(CDC_TX_EPADDR);
                
                if (!Endpoint_IsReadWriteAllowed()) {
                        Endpoint_ClearIN();
                        while (!Endpoint_IsINReady() && USB_DeviceState == DEVICE_STATE_Configured) {
                                USB_USBTask();
                        }
                }
                
                set_blink_LED();
                Endpoint_Write_8(c);
                
                return 1;
        } else {
                setWriteError();
                return 0;
        }
}


// This operator is a convenient way for a sketch to check whether the
// port has actually been configured and opened by the host (as opposed
// to just being connected to the host).  It can be used, for example, in 
// setup() before printing to ensure that an application on the host is
// actually ready to receive and display the data.
// We add a short delay before returning to fix a bug observed by Federico
// where the port is configured (LineState != 0) but not quite opened.
USBSerial::operator bool() {
	USB_USBTask();
        bool result = false;
        if (LineState > 0)
                result = true;
        _delay_ms(10);
        return result;
}


// Preinstantiate Objects //////////////////////////////////////////////////////

 USBSerial Serial;
 
 
 //=======================================================================
//=======================================================================

USBDevice_ USBDevice;

USBDevice_::USBDevice_()
{
}

void USBDevice_::attach()
{
}

void USBDevice_::detach()
{
}

bool USBDevice_::configured()
{
        if (USB_DeviceState == DEVICE_STATE_Configured) {
                return true;
        } else {
                return false;
        }
}

void USBDevice_::poll()
{
}


Mouse_ Mouse;

//================================================================================
//================================================================================
//      Mouse

Mouse_::Mouse_(void) : _buttons(0)
{
}

void Mouse_::begin(void) 
{
}

void Mouse_::end(void) 
{
}

void Mouse_::click(uint8_t b)
{
        _buttons = b;
        move(0,0,0);
        _buttons = 0;
        move(0,0,0);
}

void Mouse_::move(signed char x, signed char y, signed char wheel)
{
        MouseReport.Button = _buttons;
        MouseReport.X = x;
        MouseReport.Y = y;
        //m[3] = wheel;
        HID_SendReport(HID_REPORTID_MouseReport);
}

void Mouse_::buttons(uint8_t b)
{
        if (b != _buttons)
        {
                _buttons = b;
                move(0,0,0);
        }
}

void Mouse_::press(uint8_t b) 
{
        buttons(_buttons | b);
}

void Mouse_::release(uint8_t b)
{
        buttons(_buttons & ~b);
}

bool Mouse_::isPressed(uint8_t b)
{
        if ((b & _buttons) > 0) 
                return true;
        return false;
}


Keyboard_ Keyboard;


//================================================================================
//================================================================================
//      Keyboard

Keyboard_::Keyboard_(void) 
{
}

void Keyboard_::begin(void) 
{
}

void Keyboard_::end(void) 
{
}

void Keyboard_::sendReport(USB_KeyboardReport_Data_t* keys)
{
        HID_SendReport(HID_REPORTID_KeyboardReport);
}

extern
const uint8_t _asciimap[128] PROGMEM;

#define SHIFT 0x80
const uint8_t _asciimap[128] =
{
        0x00,             // NUL
        0x00,             // SOH
        0x00,             // STX
        0x00,             // ETX
        0x00,             // EOT
        0x00,             // ENQ
        0x00,             // ACK  
        0x00,             // BEL
        0x2a,                   // BS   Backspace
        0x2b,                   // TAB  Tab
        0x28,                   // LF   Enter
        0x00,             // VT 
        0x00,             // FF 
        0x00,             // CR 
        0x00,             // SO 
        0x00,             // SI 
        0x00,             // DEL
        0x00,             // DC1
        0x00,             // DC2
        0x00,             // DC3
        0x00,             // DC4
        0x00,             // NAK
        0x00,             // SYN
        0x00,             // ETB
        0x00,             // CAN
        0x00,             // EM 
        0x00,             // SUB
        0x00,             // ESC
        0x00,             // FS 
        0x00,             // GS 
        0x00,             // RS 
        0x00,             // US 

        0x2c,              //  ' '
        0x1e|SHIFT,        // !
        0x34|SHIFT,        // "
        0x20|SHIFT,    // #
        0x21|SHIFT,    // $
        0x22|SHIFT,    // %
        0x24|SHIFT,    // &
        0x34,          // '
        0x26|SHIFT,    // (
        0x27|SHIFT,    // )
        0x25|SHIFT,    // *
        0x2e|SHIFT,    // +
        0x36,          // ,
        0x2d,          // -
        0x37,          // .
        0x38,          // /
        0x27,          // 0
        0x1e,          // 1
        0x1f,          // 2
        0x20,          // 3
        0x21,          // 4
        0x22,          // 5
        0x23,          // 6
        0x24,          // 7
        0x25,          // 8
        0x26,          // 9
        0x33|SHIFT,      // :
        0x33,          // ;
        0x36|SHIFT,      // <
        0x2e,          // =
        0x37|SHIFT,      // >
        0x38|SHIFT,      // ?
        0x1f|SHIFT,      // @
        0x04|SHIFT,      // A
        0x05|SHIFT,      // B
        0x06|SHIFT,      // C
        0x07|SHIFT,      // D
        0x08|SHIFT,      // E
        0x09|SHIFT,      // F
        0x0a|SHIFT,      // G
        0x0b|SHIFT,      // H
        0x0c|SHIFT,      // I
        0x0d|SHIFT,      // J
        0x0e|SHIFT,      // K
        0x0f|SHIFT,      // L
        0x10|SHIFT,      // M
        0x11|SHIFT,      // N
        0x12|SHIFT,      // O
        0x13|SHIFT,      // P
        0x14|SHIFT,      // Q
        0x15|SHIFT,      // R
        0x16|SHIFT,      // S
        0x17|SHIFT,      // T
        0x18|SHIFT,      // U
        0x19|SHIFT,      // V
        0x1a|SHIFT,      // W
        0x1b|SHIFT,      // X
        0x1c|SHIFT,      // Y
        0x1d|SHIFT,      // Z
        0x2f,          // [
        0x31,          // bslash
        0x30,          // ]
        0x23|SHIFT,    // ^
        0x2d|SHIFT,    // _
        0x35,          // `
        0x04,          // a
        0x05,          // b
        0x06,          // c
        0x07,          // d
        0x08,          // e
        0x09,          // f
        0x0a,          // g
        0x0b,          // h
        0x0c,          // i
        0x0d,          // j
        0x0e,          // k
        0x0f,          // l
        0x10,          // m
        0x11,          // n
        0x12,          // o
        0x13,          // p
        0x14,          // q
        0x15,          // r
        0x16,          // s
        0x17,          // t
        0x18,          // u
        0x19,          // v
        0x1a,          // w
        0x1b,          // x
        0x1c,          // y
        0x1d,          // z
        0x2f|SHIFT,    // 
        0x31|SHIFT,    // |
        0x30|SHIFT,    // }
        0x35|SHIFT,    // ~
        0                               // DEL
};

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way 
// USB HID works, the host acts like the key remains pressed until we 
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t Keyboard_::press(uint8_t k) 
{
        uint8_t i;
        if (k >= 136) {                 // it's a non-printing key (not a modifier)
                k = k - 136;
        } else if (k >= 128) {  // it's a modifier key
                KeyboardReport.Modifier |= (1<<(k-128));
                k = 0;
        } else {                                // it's a printing key
                k = pgm_read_byte(_asciimap + k);
                if (!k) {
                        setWriteError();
                        return 0;
                }
                if (k & 0x80) {                                         // it's a capital letter or other character reached with shift
                        KeyboardReport.Modifier |= 0x02;   // the left shift modifier
                        k &= 0x7F;
                }
        }
        
        // Add k to the key report only if it's not already present
        // and if there is an empty slot.
        if (KeyboardReport.KeyCode[0] != k && KeyboardReport.KeyCode[1] != k && 
            KeyboardReport.KeyCode[2] != k && KeyboardReport.KeyCode[3] != k &&
            KeyboardReport.KeyCode[4] != k && KeyboardReport.KeyCode[5] != k) {
                
                for (i=0; i<6; i++) {
                        if (KeyboardReport.KeyCode[i] == 0x00) {
                                KeyboardReport.KeyCode[i] = k;
                                break;
                        }
                }
                if (i == 6) {
                        setWriteError();
                        return 0;
                }       
        }
        sendReport(&KeyboardReport);
        return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t Keyboard_::release(uint8_t k) 
{
        uint8_t i;
        if (k >= 136) {                 // it's a non-printing key (not a modifier)
                k = k - 136;
        } else if (k >= 128) {  // it's a modifier key
                KeyboardReport.Modifier &= ~(1<<(k-128));
                k = 0;
        } else {                                // it's a printing key
                k = pgm_read_byte(_asciimap + k);
                if (!k) {
                        return 0;
                }
                if (k & 0x80) {                                                 // it's a capital letter or other character reached with shift
                        KeyboardReport.Modifier &= ~(0x02);        // the left shift modifier
                        k &= 0x7F;
                }
        }
        
        // Test the key report to see if k is present.  Clear it if it exists.
        // Check all positions in case the key is present more than once (which it shouldn't be)
        for (i=0; i<6; i++) {
                if (0 != k && KeyboardReport.KeyCode[i] == k) {
                        KeyboardReport.KeyCode[i] = 0x00;
                }
        }

        sendReport(&KeyboardReport);
        return 1;
}

void Keyboard_::releaseAll(void)
{
        KeyboardReport.KeyCode[0] = 0;
        KeyboardReport.KeyCode[1] = 0; 
        KeyboardReport.KeyCode[2] = 0;
        KeyboardReport.KeyCode[3] = 0; 
        KeyboardReport.KeyCode[4] = 0;
        KeyboardReport.KeyCode[5] = 0; 
        KeyboardReport.Modifier = 0;
        sendReport(&KeyboardReport);
}

size_t Keyboard_::write(uint8_t c)
{       
        uint8_t p = press(c);           // Keydown
        release(c);         // Keyup
        return (p);                                     // just return the result of press() since release() almost always returns 1
}


