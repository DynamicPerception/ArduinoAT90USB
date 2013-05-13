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

#ifndef usb_api_h
#define usb_api_h

//#if defined(USBCON)

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
//#include <stdbool.h>
#include "LUFA/Descriptors.h"
#include "LUFA/USB.h"
//#include "LUFA/Common.h"
#include "LUFA/usb_cdc_hid.h"
#include "LUFA/Board.h"
#include <Stream.h>
//#include "Platform.h"


//================================================================================
//================================================================================
//      USB

class USBDevice_
{
  public:
    USBDevice_();
    bool configured();

    void attach();
    void detach();  // Serial port goes down too...
    void poll();
};
extern USBDevice_ USBDevice;


//================================================================================
//================================================================================
//      Serial over CDC (Serial1 is the physical port)

class USBSerial : public Stream
{
  private:
    int readRXEndpoint(void);
  public:
   // USBSerial();
    void begin(long);
    void end();
    virtual int available(void);
    virtual void accept(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    operator bool();
    using Print::write; // pull in write(str) and write(buf, size) from Print
};
extern USBSerial Serial;


//================================================================================
//================================================================================
//      Mouse

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 4
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)

class Mouse_
{
  private:
    uint8_t _buttons;
    void buttons(uint8_t b);
  public:
    Mouse_(void);
    void begin(void);
    void end(void);
    void click(uint8_t b = MOUSE_LEFT);
    void move(signed char x, signed char y, signed char wheel = 0); 
    void press(uint8_t b = MOUSE_LEFT);             // press LEFT by default
    void release(uint8_t b = MOUSE_LEFT);   // release LEFT by default
    bool isPressed(uint8_t b = MOUSE_LEFT); // check LEFT by default
};
extern Mouse_ Mouse;

//================================================================================
//================================================================================
//      Keyboard

#define KEY_LEFT_CTRL           0x80
#define KEY_LEFT_SHIFT          0x81
#define KEY_LEFT_ALT            0x82
#define KEY_LEFT_GUI            0x83
#define KEY_RIGHT_CTRL          0x84
#define KEY_RIGHT_SHIFT         0x85
#define KEY_RIGHT_ALT           0x86
#define KEY_RIGHT_GUI           0x87

#define KEY_UP_ARROW            0xDA
#define KEY_DOWN_ARROW          0xD9
#define KEY_LEFT_ARROW          0xD8
#define KEY_RIGHT_ARROW         0xD7
#define KEY_BACKSPACE           0xB2
#define KEY_TAB                         0xB3
#define KEY_RETURN                      0xB0
#define KEY_ESC                         0xB1
#define KEY_INSERT                      0xD1
#define KEY_DELETE                      0xD4
#define KEY_PAGE_UP                     0xD3
#define KEY_PAGE_DOWN           0xD6
#define KEY_HOME                        0xD2
#define KEY_END                         0xD5
#define KEY_CAPS_LOCK                   0xC1
#define KEY_F1                          0xC2
#define KEY_F2                          0xC3
#define KEY_F3                          0xC4
#define KEY_F4                          0xC5
#define KEY_F5                          0xC6
#define KEY_F6                          0xC7
#define KEY_F7                          0xC8
#define KEY_F8                          0xC9
#define KEY_F9                          0xCA
#define KEY_F10                         0xCB
#define KEY_F11                         0xCC
#define KEY_F12                         0xCD

class Keyboard_ : public Print
{
  private:
    void sendReport(USB_KeyboardReport_Data_t* keys);
  public:
    Keyboard_(void);
    void begin(void);
    void end(void);
    virtual size_t write(uint8_t k);
    virtual size_t press(uint8_t k);
    virtual size_t release(uint8_t k);
    virtual void releaseAll(void);
};
extern Keyboard_ Keyboard;



//#endif
#endif
