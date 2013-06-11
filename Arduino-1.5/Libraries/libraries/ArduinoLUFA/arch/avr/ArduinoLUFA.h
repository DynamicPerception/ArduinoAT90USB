
/* Core Arduino LUFA Library
 
 C.A. Church / Dynamic Perception LLC
 
 Based on the excellent work by Dean Camera for his LUFA project.
 
 Shared under the MIT License
 
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
    ERROR_NOMAP
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
