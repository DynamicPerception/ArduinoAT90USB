
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
 
 Derived classes may override this method.
 
 This method only needs to be called once, during the setup of your sketch.
 
 */

void ArduinoLUFA::init() {
    
    cli();
    
#if defined(USB_CAN_BE_BOTH)
    USB_Init(USB_MODE_UID, USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL);
#else
    USB_Init(void, USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL);
#endif
    
    sei();
}


#if defined(USB_CAN_BE_BOTH) || defined(USB_CAN_BE_DEVICE)
 /** Default callback for devices, to be overloaded later as needed */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex, const void** const DescriptorAddress) {                                    
    return 0;
}
#endif


/** Read a 32-bit Value From PGMSpace */

uint32_t pgm_read_u32(const void* p_addr) {
	uint32_t val;
	uint8_t i;
	uint8_t *p, *c;
	p = (uint8_t*)&val;
	c = (uint8_t*)p_addr;
	for(i = 0; i < sizeof(uint32_t); i++)
	{
		p[i] = pgm_read_byte(c);
		c++;
	}
	return val;
}

