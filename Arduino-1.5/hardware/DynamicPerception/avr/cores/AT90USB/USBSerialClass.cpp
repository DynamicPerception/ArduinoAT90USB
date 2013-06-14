#include "USBSerial.h"




USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface = {
    // .Config =
    {
         // .ControlInterfaceNumber   =  
        0,
        // .DataINEndpoint           = 
        {
           // .Address          = 
            CDC_TX_EPADDR,
           // .Size             = 
            CDC_TXRX_EPSIZE,
           // .Banks            = 
            1,
        },
        // .DataOUTEndpoint =
        {
            // .Address          = 
            CDC_RX_EPADDR,
            // .Size             = 
            CDC_TXRX_EPSIZE,
            // .Banks            = 
            1,
        },
        // .NotificationEndpoint =
        {
            // .Address          = 
            CDC_NOTIFICATION_EPADDR,
            // .Size             = 
            CDC_NOTIFICATION_EPSIZE,
            // .Banks            = 
            1,
        },
    },
};




USBSerial_::USBSerial_() {
    m_didPeek = 0;
    m_peek    = 0;
    
}

void USBSerial_::begin(long p_baud) {
    
    /* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
    
	/* Disable clock division */
	clock_prescale_set(clock_div_1);
    
        // baud rate is ignored
#if defined(USB_CAN_BE_BOTH)
    USB_Init(USB_MODE_UID, USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL);
#else
    USB_Init(USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL);
#endif
    

    
    GlobalInterruptEnable();
    
    _doTasks();

}

void USBSerial_::end() {
	USB_Disable();
}

int USBSerial_::available() {
   
    int bytes = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);
    
    return bytes;
    
}

void USBSerial_::accept() {
    return;
}

int USBSerial_::peek() {
    if( available() > 0 ) {
        m_didPeek = 1;
        m_peek = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
        return m_peek;
    }
    else {
        return 0;
    }
    
}


int USBSerial_::read() {
    
    _doTasks();
    
    if( m_didPeek ) {
        m_didPeek = 0;
        return m_peek;
    }
    else {
        
        if (USB_DeviceState != DEVICE_STATE_Configured) {
            return -1;
        }

        if( available() > 0 ) {
            int val = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
        }
    }
    
}

size_t USBSerial_::write(uint8_t p_char) {
    
    _doTasks();

    if (USB_DeviceState != DEVICE_STATE_Configured) {
        return 0;
    }
    
    int ret = CDC_Device_SendByte(&VirtualSerial_CDC_Interface, p_char);
    
    if( ret == ENDPOINT_READYWAIT_NoError )
        return 1;
    else 
        return 0;
    
}


void USBSerial_::_doTasks() {
    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    USB_USBTask();
}


void USBSerial_::flush() {
    CDC_Device_Flush(&VirtualSerial_CDC_Interface);
}


USBSerial_::operator bool() {
	_doTasks();
    
    if (USB_DeviceState != DEVICE_STATE_Configured)
        return false;
                
    return true;
}


USBSerial_ USBSerial = USBSerial_();

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void) {
	
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void) {
	
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;
    
	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

    
 }

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void) {
       	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}


