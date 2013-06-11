/* Keyboard Host Arduino LUFA Library
 
 C.A. Church / Dynamic Perception LLC
 
 Based on the excellent work by Dean Camera for his LUFA project.
 
 Shared under the MIT License
 
 */


#include "KeyboardHost.h"


/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */

extern "C" {

    USB_ClassInfo_HID_Host_t Keyboard_HID_Interface = {
        /* .Config = */ {
            /* .DataINPipe             = */ {
                /* .Address        = */ (PIPE_DIR_IN  | 1),
                /* .Banks          = */ 1,
            },
            /* .DataOUTPipe             = */ {
                /* .Address        = */ (PIPE_DIR_OUT | 2),
                /* .Banks          = */ 1,
            },
            /* .HIDInterfaceProtocol   = */ HID_CSCP_KeyboardBootProtocol,
        },
    };
    
}


const unsigned char KeyboardHost::m_usKeyMap[][2] = {
    /* HID_KEYBOARD_SC_ERROR_ROLLOVER */
    { 0, 0 }, 
    /* HID_KEYBOARD_SC_POST_FAIL */
    { 0, 0 },         
    /* HID_KEYBOARD_SC_ERROR_UNDEFINED */
    { 0, 0 },
    /* HID_KEYBOARD_SC_A */
    { 'a', 'A' },
    { 'b', 'B' },
    { 'c', 'C' },
    { 'd', 'D' },
    { 'e', 'E' },
    { 'f', 'F' },
    { 'g', 'G' },
    { 'h', 'H' },
    { 'i', 'I' },
    { 'j', 'J' },
    { 'k', 'K' },
    { 'l', 'L' },
    { 'm', 'M' },
    { 'n', 'N' },
    { 'o', 'O' },
    { 'p', 'P' },
    { 'q', 'Q' },
    { 'r', 'R' },
    { 's', 'S' },
    { 't', 'T' },
    { 'u', 'U' },
    { 'v', 'V' },
    { 'w', 'W' },
    { 'x', 'X' },
    { 'y', 'Y' },
    { 'z', 'Z' },
    { '1', '!' },
    { '2', '@' },
    { '3', '#' },
    { '4', '$' },
    { '5', '%' },
    { '6', '^' },
    { '7', '&' },
    { '8', '&' },
    { '9', '(' },
    { '0', ')' },
    /** HID_KEYBOARD_SC_ENTER */
    { '\n', '\n' },
    /** HID_KEYBOARD_SC_ESCAPE */
    { 33, 33 },
    /** HID_KEYBOARD_SC_BACKSPACE */
    { 10, 10 },
    /** HID_KEYBOARD_SC_TAB */
    { 11, 11 },
    /** HID_KEYBOARD_SC_SPACE */
    { ' ', ' ' },
    { '-', '_' },
    { '=', '+' },
    { '[', '{' },
    { ']', '}' },
    { '\\', '|' },
    /* HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE */
    { '`', '~' },
    { ';', ':' },
    { '\'', '"' },
    /* HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE */
    { '`', '~' },
    { ',', '<' },
    { '.', '>' },
    { '/', '?' },
    /* HID_KEYBOARD_SC_CAPS_LOCK */
    { 0, 0 },
    /* HID_KEYBOARD_SC_F1 */
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    /* HID_KEYBOARD_SC_F12 */
    { 0, 0 },
    /* HID_KEYBOARD_SC_PRINT_SCREEN */
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    /* HID_KEYBOARD_SC_DELETE */
    { 177, 177 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    /* HID_KEYBOARD_SC_KEYPAD_SLASH */
    { '/', '/' },
    { '*', '*' },
    { '-', '-' },
    { '+', '+' },
    { '\n', '\n' },
    { '1', '1' },
    { '2', '2' },
    { '3', '3' },
    { '4', '4' },
    { '5', '5' },
    { '6', '6' },
    { '7', '7' },
    { '8', '8' },
    { '9', '9' },
    { '0', '0' },
    { '.', '.' },
    { '\\', '|' },
    /* HID_KEYBOARD_SC_APPLICATION */
    { 0, 0 },
    /* HID_KEYBOARD_SC_POWER */
    { 0, 0 },
    /* HID_KEYBOARD_SC_KEYPAD_EQUAL_SIGN */
    { '=', '=' }
};


/** Constructor with Default Keymap
 
    Create an instance of the class with a default keymap (MAP_US_English)
 */

KeyboardHost::KeyboardHost() : ArduinoLUFA() {
    mapping(MAP_US_English);
}

/** Constructor
 
    Create an instance of the class with a specified keymap
 
    @params c_map
    A Valid KeyMapType
 */

KeyboardHost::KeyboardHost( KeyMapType c_map ) : ArduinoLUFA() {
    mapping( c_map );
}

/** Set Keyboard Mapping
 
 Specifies which supported keyboard mapping to use.
 
 @param p_map
 A valid KeyMapType
 */

void KeyboardHost::mapping( KeyMapType p_map ) {
    m_map = p_map;
}


/** Get The Current Keys Being Pressed
 
 Returns a list of all pressed keys, and the status of the keyboard at the request time based on
 the underlying USB class driver.  
 
 The return value is a KeysPressed structure, which will represent the (up to) 6 keys pressed by
 the user, along with any modifier.  See KeysPressed for more info on modifiers.  
 
  @return 
 A KeysPressed structure defining the keys pressed (or not) along with the status of the request
 */

KeysPressed KeyboardHost::getKeys() {
    
        // perform required USB tasks, from LUFA examples

    HID_Host_USBTask(&Keyboard_HID_Interface);
    USB_USBTask();
    
    KeysPressed ReturnDesc = { STAT_CONNECTED, m_map, 0, { { 0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} } };
    
        // check for errors, not connected
    if ( status() == STAT_ERROR ) {
        ReturnDesc.status = STAT_ERROR;
        return ReturnDesc;
    }

    if (USB_HostState != HOST_STATE_Configured) {
        ReturnDesc.status = STAT_NOTCONNECTED;
        return ReturnDesc;
    }
    
    
    if (HID_Host_IsReportReceived(&Keyboard_HID_Interface))	{
		USB_KeyboardReport_Data_t KeyboardReport;
		HID_Host_ReceiveReport(&Keyboard_HID_Interface, &KeyboardReport);
        
        uint8_t Modifier = KeyboardReport.Modifier;
                
        for(uint8_t i = 0; i < 6; i++) {
              _mapKey(KeyboardReport.KeyCode[i], Modifier, ReturnDesc.keys[i]);

        }
            
	}

        // check for errors from keymapping
    if( error() != ERROR_NONE ) 
        ReturnDesc.status = STAT_ERROR;
    
    return ReturnDesc;
    
}

void KeyboardHost::_mapKey(uint8_t p_key, uint8_t p_mod, KeyDesc& p_desc) {
    
    p_desc.raw   = p_key;
    
    if( m_map == MAP_None ) {
        p_desc.value = p_desc.raw;
    }
    else if( m_map == MAP_US_English ) {
            
            // don't try to map any values out of range
        if( p_key > sizeof(m_usKeyMap) / sizeof(char[2]) || p_key == 0 )
            return;
        
        uint8_t idx = 0;
        
        if( p_mod & ( HID_KEYBOARD_MODIFIER_LEFTSHIFT | HID_KEYBOARD_MODIFIER_RIGHTSHIFT ) ) 
            idx = 1;
        
        p_desc.value = pgm_read_byte(&( m_usKeyMap[p_key - 1][idx] ));
        
        
    }
    else {
        error(ERROR_NOMAP);
        status(STAT_ERROR);
    }
}

    // create an object, we'll need it below
KeyboardHost KeyHost = KeyboardHost();


/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached(void) {
	KeyHost.status(STAT_CONNECTED);
    KeyHost.error(ERROR_NONE);
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached(void) {
	KeyHost.status(STAT_NOTCONNECTED);
    KeyHost.error(ERROR_NONE);
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete(void) {

	uint16_t ConfigDescriptorSize;
	uint8_t  ConfigDescriptorData[512];
    
	if (USB_Host_GetDeviceConfigDescriptor(1, &ConfigDescriptorSize, ConfigDescriptorData, sizeof(ConfigDescriptorData)) != HOST_GETCONFIG_Successful) {
        KeyHost.status(STAT_ERROR);
        KeyHost.error(ERROR_CFGDESC);
		return;
	}
    
	if (HID_Host_ConfigurePipes(&Keyboard_HID_Interface, ConfigDescriptorSize, ConfigDescriptorData) != HID_ENUMERROR_NoError) {
        KeyHost.status(STAT_ERROR);
        KeyHost.error(ERROR_DEV);
		return;
	}
    
	if (USB_Host_SetDeviceConfiguration(1) != HOST_SENDCONTROL_Successful) {
        KeyHost.status(STAT_ERROR);
        KeyHost.error(ERROR_SETCFG);
		return;
	}
    
	if (HID_Host_SetBootProtocol(&Keyboard_HID_Interface) != 0) {
        KeyHost.status(STAT_ERROR);
        KeyHost.error(ERROR_SETPROTO);
		USB_Host_SetDeviceConfiguration(0);
		return;
	}

    // KeyHost.status(STAT_CONNECTED);
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError(const uint8_t p_err) {
	USB_Disable();

    KeyHost.status(STAT_ERROR);
    KeyHost.error(ERROR_HARD);

}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t p_err, const uint8_t p_suberr) {
    KeyHost.status(STAT_ERROR);
    KeyHost.error(ERROR_HARD);
}

/** Callback for the HID Report Parser. This function is called each time the HID report parser is about to store
 *  an IN, OUT or FEATURE item into the HIDReportInfo structure. To save on RAM, we are able to filter out items
 *  we aren't interested in (preventing us from being able to extract them later on, but saving on the RAM they would
 *  have occupied).
 *
 *  \param[in] CurrentItem  Pointer to the item the HID report parser is currently working with
 *
 *  \return Boolean true if the item should be stored into the HID report structure, false if it should be discarded
 */
bool CALLBACK_HIDParser_FilterHIDReportItem(HID_ReportItem_t* const p_item) {
	/* Check the attributes of the current item - see if we are interested in it or not;
	 * only store KEYBOARD usage page items into the Processed HID Report structure to
	 * save RAM and ignore the rest
	 */
	return (p_item->Attributes.Usage.Page == USAGE_PAGE_KEYBOARD);
}


