/* Arduino LUFA USBSerial Library
 
 C.A. Church / Dynamic Perception LLC
 
 Based on the excellent work by Dean Camera for his LUFA project, and also
 on the excellent usb_api.cpp work by Peter Barrett and Justin Mattair.
 
 Shared under the MIT License
 
 */


/** @file USBSerial.h
 
 @brief USBSerial Header File
 */


#ifndef _USBSerial_h
#define _USBSerial_h

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <inttypes.h>

#include <Arduino.h>
#include <Stream.h>
#include <Print.h>

#if defined(USE_USB_SERIAL)


#include <LUFA/Drivers/USB/USB.h>
#include "USBSerialDescriptors.h"


extern USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface;    


volatile static uint8_t m_lineState = 0;

class USBSerial_ : public Stream {

public:
    USBSerial_();
    void begin(long p_baud);
    void end();
    virtual int available();
    virtual void accept();
    virtual int peek();
    virtual int read();
    virtual void flush();
    virtual size_t write(uint8_t p_char);
    operator bool();
    using Print::write; // pull in write(str) and write(buf, size) from Print
    
private:
    int m_peek;
    uint8_t m_didPeek;
    
    void _doTasks();
};

extern USBSerial_ USBSerial;


// C functions from LUFA

    
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t * p_dev);
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t * p_dev); 	

#endif

#endif
