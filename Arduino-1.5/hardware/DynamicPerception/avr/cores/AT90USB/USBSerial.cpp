#include "USBSerial.h"



/** Contains the current baud rate and other settings of the virtual serial port. While this demo does not use
 *  the physical USART and thus does not use these settings, they must still be retained and returned to the host
 *  upon request or the host will assume the device is non-functional.
 *
 *  These values are set by the host via a class-specific request, however they are not required to be used accurately.
 *  It is possible to completely ignore these value or use other settings as the host is completely unaware of the physical
 *  serial link characteristics and instead sends and receives data in endpoint streams.
 */
static CDC_LineEncoding_t LineEncoding = { 
    // .BaudRateBPS = 
    0,
    // .CharFormat  = 
    CDC_LINEENCODING_OneStopBit,
    // .ParityType  = 
    CDC_PARITY_None,
    // .DataBits  =  
    8
};



USBSerial_::USBSerial_() {
    m_didPeek = 0;
    m_peek    = 0;
}

void USBSerial_::begin(long p_baud) {
        // baud rate is ignored
#if defined(USB_CAN_BE_BOTH)
    USB_Init(USB_MODE_UID, USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL | USB_DEVICE_OPT_FULLSPEED);
#else
    USB_Init(USB_OPT_REG_DISABLED | USB_OPT_AUTO_PLL | USB_DEVICE_OPT_FULLSPEED);
#endif
    
    // sei();
    GlobalInterruptEnable();
    
}

void USBSerial_::end() {
	USB_Disable();
}

int USBSerial_::available() {
   
    uint8_t endPoint = Endpoint_GetCurrentEndpoint();
    
    Endpoint_SelectEndpoint(CDC_RX_EPADDR);
    
    int bytes = Endpoint_BytesInEndpoint();
    
    Endpoint_SelectEndpoint(endPoint);
    
    return bytes;
    
}

void USBSerial_::accept() {
    return;
}

int USBSerial_::peek() {
    if( available() > 0 ) {
        m_peek    = read();
        m_didPeek = 1;
        return m_peek;
    }
    else {
        return 0;
    }
    
}


int USBSerial_::read() {
    
    int val = 0;
    
    if( m_didPeek ) {
        m_didPeek = 0;
        return m_peek;
    }
    else {
        
        if (USB_DeviceState != DEVICE_STATE_Configured) {
            return -1;
        }

        /* Select the Serial Rx Endpoint */
        Endpoint_SelectEndpoint(CDC_RX_EPADDR);
        
        if (Endpoint_BytesInEndpoint() > 0) {
            
            val = Endpoint_Read_8();
            
            if(Endpoint_BytesInEndpoint() == 0) 
                Endpoint_ClearOUT();
            
        }
            
    }
    
    _doTasks();
    
    return val;
    
}

size_t USBSerial_::write(uint8_t p_char) {
    
  
    if (USB_DeviceState != DEVICE_STATE_Configured) {
        return 0;
    }
    
    /* Select the Serial Tx Endpoint */
    Endpoint_SelectEndpoint(CDC_TX_EPADDR);
    
    /* Write the String to the Endpoint */
    Endpoint_Write_8(p_char);
    
    flush();
    
    _doTasks();

    return 1;
}


void USBSerial_::_doTasks() {
  //  CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    USB_USBTask();
}


void USBSerial_::flush() {
    
    Endpoint_SelectEndpoint(CDC_TX_EPADDR);
    
    /* Remember if the packet to send completely fills the endpoint */
    bool IsFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);
    
    /* Finalize the stream transfer to send the last packet */
    Endpoint_ClearIN();
    
    /* If the last packet filled the endpoint, send an empty packet to release the buffer on
     * the receiver (otherwise all data will be cached until a non-full packet is received) */
    
    if (IsFull) {
        /* Wait until the endpoint is ready for another packet */
        Endpoint_WaitUntilReady();
        
        /* Send an empty packet to ensure that the host does not buffer data sent to it */
        Endpoint_ClearIN();
    }    
}


USBSerial_::operator bool() {
	USB_USBTask();
    
    if (USB_DeviceState != DEVICE_STATE_Configured)
        return false;
                
    return true;
}


USBSerial_ USBSerial;

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    	
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup CDC Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK,  CDC_TXRX_EPSIZE, 1);
    
	/* Reset line encoding baud rate so that the host knows to send new values */
	LineEncoding.BaudRateBPS = 0;

    
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    
     
    /* Process CDC specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
		case CDC_REQ_GetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
                
				/* Write the line coding data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearOUT();
			}
            
			break;
		case CDC_REQ_SetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
                
				/* Read the line coding data in from the host into the global struct */
				Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearIN();
			}
            
			break;
		case CDC_REQ_SetControlLineState:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
                
				/* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
                 lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
                 CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
                 */
			}
            
			break;
	}
}


