
/* Core Arduino LUFA Library
 
 C.A. Church / Dynamic Perception LLC
 
 Based on the excellent work by Dean Camera for his LUFA project.
 
 Shared under the MIT License
 
 */


/** @file ArduinoLUFA.h
 
 @brief Core ArduinoLUFA Header File
 */
 

/** 
 @mainpage
 
 <h1>ArduinoLUFA</h1>
 
 The ArduinoLUFA Libraries provide high-level, encapsulated access to key LUFA components.
 
 These libraries allow you to access their specified functions directly and easily in Arduino
 skecthes without having to rely on external C code, or to be forced to deal with the underlying
 functionality of the LUFA libraries.
 
 These libraries require the use of the AT90USB hardware definition for Arduino, and some libraries
 require boards which support host mode.
 
 The LUFA libraries were created by Dean Camera and are instrumental in effective USB host and device
 implementation on AVR platforms.  Consider supporting Dean for his great work - or at least just saying
 thank you!
 
 <h2>Included Libraries</h2>
 
 <h3>KeyboardHost</h3>
 
 A keyboard host class that allows you to read keys from an attached USB keyboard easily. 
 
 */
 
#ifndef _ARDUINOLUFA_H_
#define _ARDUINOLUFA_H_

#include <LUFA/Drivers/USB/USB.h>

static HID_ReportInfo_t HIDReportInfo;



/** HID Report Descriptor Usage Page value for a desktop keyboard. */
static const uint8_t USAGE_PAGE_KEYBOARD = 0x07;

/** Common LUFA Codes */
enum LUFAErr { 
    /** No Error Set */
    ERROR_NONE = 0,
    /** Error Retrieving Configuration Descriptor */
    ERROR_CFGDESC = 1,
    /** Error, Not the Expected Device Type */
    ERROR_DEV,
    /** Error Setting Device Configuration */
    ERROR_SETCFG,
    /** Error Setting Report Protocol, or Not the Expected Device Type */
    ERROR_SETPROTO,
    /** Hardware Error Occurred */
    ERROR_HARD,
    /** Unknown Keyboard Mapping Specified for KeyboardHost */
    ERROR_NOMAP,
    /** Error Transmitting Byte */
    ERROR_XMIT,
    /** Error Receiving Byte */
    ERROR_RECV
};

/** Common Status Codes */
enum LUFAStat {
    /** Device is Connected */
    STAT_CONNECTED = -1,
    /** Device is Not Connected */
    STAT_NOTCONNECTED = -2,
    /** An Error Occurred */
    STAT_ERROR = -3
};



/** ArduinoLUFA Class
 
 The ArduinoLUFA Class is the base class for all ArduinoLUFA libraries.  It provides
 core capabilities shared by all libraries, such as status reporting, error reporting,
 USB device initialization and more.
 
 Users should not use this class directly, and should instead use a derived class. However,
 it is perfectly safe to use the static init method in user-code, especially to indicate
 that initialization only needs to be done once, and not once per class.
 
 @author C. A. Church
 
 @copyright
 Copyright &copy; 2013 C. A. Church / Dynamic Perception LLC
 Includes and is based on code Copyright &copy; 2012 Dean Camera
 
 */

class ArduinoLUFA {
  
public:
    ArduinoLUFA();
    
    void error(LUFAErr p_err);
    LUFAErr error();
    
    void status(LUFAStat p_stat);
    LUFAStat status();
    
    static void init();
    
private:
    
    LUFAStat m_stat;
    LUFAErr m_err;
  

};

#endif // _ARDUINOLUFA_H_
