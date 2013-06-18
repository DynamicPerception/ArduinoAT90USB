
/* Core Arduino LUFA Library
 
 C.A. Church / Dynamic Perception LLC
 
 Based on the excellent work by Dean Camera for his LUFA project.
 
 Shared under the MIT License
 
 */


#include "ArduinoLUFA.h"

/** Constructor
 
 Construct a new instance
 */

ArduinoLUFA::ArduinoLUFA() {
    
    m_err  = ERROR_NONE;
    m_stat = STAT_NOTCONNECTED;
    
}

/** Set Current Error
 
 @param p_err
 A valid LUFAErr value
 */

void ArduinoLUFA::error(LUFAErr p_err) {
    m_err = p_err;
}

/** Return Current Error Code
 
 Returns a LUFAErr error code, ERROR_NONE
 if no error is registered.
 
 @return
 A valid LUFAErr value
 */

LUFAErr ArduinoLUFA::error() {
    return m_err;
}

/** Set Current Status
 
 @param p_stat
 A valid LUFAStat value
 */
void ArduinoLUFA::status(LUFAStat p_stat) {
    m_stat = p_stat;
}

 /** Return Current Status
  
  @return 
  A valid LUFAStat value
  */
LUFAStat ArduinoLUFA::status() {
    return m_stat;
}


/** Initialize USB Interface
 
 Initializes the USB interface, must be done before attempting
 to utilize the USB interface in a host or device mode.
 
 Our initialization options are simplified from what LUFA fully
 supports.  If the device can be both a HOST or DEVICE, the mode
 is set to USB_MODE_UID, and otherwise the options are always
 set to USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL
 
 This method is static, and can be called without an associated object.
 
 This method only needs to be called once, during the setup of your sketch.
 
 */

void ArduinoLUFA::init() {
    
#if defined(USB_CAN_BE_BOTH)
    USB_Init(USB_MODE_UID, USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL);
#else
    USB_Init(void, USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL);
#endif
}


#if defined(USB_CAN_BE_BOTH) || defined(USB_CAN_BE_DEVICE)
 /** Default callback for devices, to be overloaded later as needed */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex, const void** const DescriptorAddress) {                                    
    return 0;
}
#endif
