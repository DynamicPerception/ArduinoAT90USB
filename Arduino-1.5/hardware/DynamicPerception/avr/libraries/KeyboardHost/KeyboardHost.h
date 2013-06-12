
/* Keyboard Host Arduino LUFA Library
 
 C.A. Church / Dynamic Perception LLC
 
 Based on the excellent work by Dean Camera for his LUFA project.
 
 His original copyright notice follows.
 
 New Code License:
 
 Copyright 2013 C.A. Church / Dynamic Perception LLC
 
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



// Original Copyright Notice from LUFA

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


/** @file KeyboardHost.h
 
 @brief Core KeyboardHost.h Header File
 */

#ifndef _KEYBOARDHOST_H_
#define _KEYBOARDHOST_H_

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "../../ArduinoLUFA/arch/avr/ArduinoLUFA.h"


#if ! defined(USB_CAN_BE_BOTH) || ! defined(USB_CAN_BE_HOST)
    #error The current board does not support host mode
#endif

extern "C" {
    extern USB_ClassInfo_HID_Host_t Keyboard_HID_Interface;
}

    /** Supported Key Mappings Enum 
     
     Lists the supported key map type codes
     */
enum KeyMapType {
        /** No Mapping Used, Values will be the same as Raw */
    MAP_None,
        /** Standard, US-English Keyboard Mapping */
    MAP_US_English
};

    /** Structure Defining Key Received
     
     Provides information about the key received from the keyboard.*/

struct KeyDesc {
    
        /** The raw, numeric keycode received from they keyboard device.  
         
         Will be 0 if no key was received */
    unsigned char raw;
    
        /** The ASCII value of the key, according to the keymap with any shift modifiers applied.
         
         If no ASCII value is available for the key, a numeric 0 will be provided. */
    unsigned char value;
    
    
};


 /** All Keys Pressed Structure 
  
    This structure is returned by getKeys() - it represents up to 6 keys pressed at the time of
    polling.  
  
    Also included is status information about the request, and which map was used for
    conversion during the request.
  
  */

struct KeysPressed {
    /** The status of the keyboard during the request.
     
     Can be any valid LUFAStat, ensure that you check for STATUS_ERROR, etc.*/
    
    LUFAStat status;
    
    /** The keymap type used to provide the ASCII value for the keys*/
    KeyMapType map;

    /** The modifiers flag as received from the underlying LUFA driver.  
     
     Measure against one or more of the following flags:
     <ul>
     <li>HID_KEYBOARD_MODIFIER_LEFTCTRL</li>
     <li>HID_KEYBOARD_MODIFIER_LEFTSHIFT</li>
     <li>HID_KEYBOARD_MODIFIER_LEFTALT</li>
     <li>HID_KEYBOARD_MODIFIER_LEFTGUI</li>
     <li>HID_KEYBOARD_MODIFIER_RIGHTCTRL</li>
     <li>HID_KEYBOARD_MODIFIER_RIGHTSHIFT</li>
     <li>HID_KEYBOARD_MODIFIER_RIGHTALT</li>
     <li>HID_KEYBOARD_MODIFIER_RIGHTGUI</li>
     </ul>
     
     e.g.:
     
     @code
     if( keys.modifier & ( HID_KEYBOARD_MODIFIER_LEFTSHIFT | HID_KEYBOARD_MODIFIER_RIGHTSHIFT ) ) {
     // shift was held during keypress
     }
     @endcode
     */         
    uint8_t modifier;
    
     /** An array of KeyDesc structures representing up to six keys pressed
      
      Once you reach a KeyDesc structure where the raw value is numeric 0, you have reached
      the end of all keys pressed, e.g.:
      
      @code
      for(byte i = 0; i < 6; i++) {
        KeyDesc thisKey = keys.keys[i];
      
        if( thisKey.raw == 0 ) {
            // no more keys pressed!
            break;
        }
        else {
            // do something with the key
            doSomething( thisKey );
        }
      }
      @endcode
      */
    KeyDesc keys[6];
    
};

/** KeyboardHost Class 
 
 This class provides high-level access to an attached keyboard device when used with a 
 board that support OTG or USB Host mode.  Based on the excellent keyboard host and LUFA 
 code provided by Dean Camera.  
 
 This class is designed to be used within the Arduino IDE, using the AT90USB Arduino hardware
 extension, and can only be used with supported board types, including, but not limited to:
 
 <ul>
    <li>AT90USB647</li>
    <li>AT90USB1287</li>
 
 This library automatically handles mapping key codes to their ASCII representation, including
 applying any SHIFT-key modifiers.  Additionally, you may connect or disconnect the keyboard at
 any time during operation with this library.
 
 The user can press multiple keys at once, and if the user does, you will be able to read up to
 six of them at once.
 
 As you can only have one instance of this class, and it is necessary for that instance to exist
 at the time this class is compiled, the KeyHost object will automatically be imported into your
 sketch when including this library.  See the example below.
 
 
 Example of Using this Library in an Arduino Sketch:
 
 @code
 #include "ArduinoLUFA.h"
 #include "KeyboardHost.h"
 
 void setup() {
    ArduinoLUFA::init();
 }
 
 void loop() {
 
    KeysPressed keys = KeyHost.getKeys();
 
    if( keys.status == STATUS_ERROR ) {
        // an error occurred trying to get the key
        // retrieve the error and do soemthing with
        // it.
        
        LUFAErr errorCode = KeyHost.error();
    }
    else {
        
            // loop through all keys returned
 
        for(byte i = 0; i < 6; i++) {
            KeyDesc thisKey = keys.keys[i];
 
            if( thisKey.value != 0 ) {
 
                // key was recognized and an ascii value is 
                // available -- do something with it
            }
        }
 
    }
    
 }
 @endcode
 
 @author
 C. A. Church
 
 @copyright 
 Copyright &copy; 2013 Dynamic Perception LLC, with large contributions from work Copyright &copy; 2012 Dean Camera
 */

class KeyboardHost : public ArduinoLUFA {
    
public:
    
        // ctors
    KeyboardHost();
    KeyboardHost(KeyMapType c_map);
    
    KeysPressed getKeys();
    void mapping(KeyMapType p_map);
    
private:
    
    
    
    void _mapKey(uint8_t p_key, uint8_t p_mod, KeyDesc& p_desc);
    
    KeyMapType m_map;
    
    /* US Keyboard Mapping
     
        Provide a mapping of the ASCII character represented by a key, and the 
        SHIFT-modified version of that key. 
     
        For the US Keyboard Layout
     
        Note that values which do not have an ASCII representation, the numeric value
        0 is mapped.  For all values over HID_KEYBOARD_SC_KEYPAD_EQUALSIGN we won't
        even map, and will instead automatically return 0.
     */
    
    PROGMEM static const unsigned char m_usKeyMap[][2];
    
};

extern KeyboardHost KeyHost;

/* C Function Prototypes From LUFA */

    void EVENT_USB_Host_HostError(const uint8_t p_err);
    void EVENT_USB_Host_DeviceAttached(void);
    void EVENT_USB_Host_DeviceUnattached(void);
    void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t p_err, const uint8_t p_suberr);
    void EVENT_USB_Host_DeviceEnumerationComplete(void);
    bool CALLBACK_HIDParser_FilterHIDReportItem(HID_ReportItem_t* const p_item); 

#endif // _KEYBOARDHOST_H_

