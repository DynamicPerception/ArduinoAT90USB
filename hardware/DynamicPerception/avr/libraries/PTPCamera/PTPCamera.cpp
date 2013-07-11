
#include "PTPCamera.h"


USB_ClassInfo_SI_Host_t DigitalCamera_SI_Interface =
{
    // .Config =
    {
        // .DataINPipe             =
        {
            // .Address        = 
            (PIPE_DIR_IN  | 1),
            // .Banks          = 
            1,
        },
        // .DataOUTPipe            =
        {
            // .Address        = 
            (PIPE_DIR_OUT | 2),
            // .Banks          = 
            1,
        },
        // .EventsPipe             =
        {
            // .Address        = 
            (PIPE_DIR_IN  | 3),
            // .Banks          = 
            1,
        },
    },
};



char PTP_Buffer[PTP_BUFFER_SIZE];
uint16_t PTP_Bytes_Received, PTP_Bytes_Remaining, PTP_Bytes_Total, PTP_OpsCount;

char PTP_CameraModel[23];
uint32_t PTP_CameraVendor; 
uint16_t* PTP_Ops;


const PTPCamera::propertyDescription_t PTPCamera::m_apertureList[] PROGMEM = {
    {"  Error", 0xFF, 0xFF, 255 },
    {"  f/1.2", 0x0d, 120, 2 },
    {"  f/1.4", 0x10, 140, 3 },
    {"  f/1.6", 0x13, 160, 4 },
    {"  f/1.8", 0x15, 180, 5 },
    {"  f/2.0", 0x18, 200, 6 },
    {"  f/2.2", 0x1b, 220, 7 },
    {"  f/2.5", 0x1d, 250, 8 },
    {"  f/2.8", 0x20, 280, 9 },
    {"  f/3.2", 0x23, 320, 10 },
    {"  f/3.5", 0x25, 350, 11 },
    {"  f/4.0", 0x28, 400, 12 },
    {"  f/4.5", 0x2b, 450, 13 },
    {"  f/5.0", 0x2d, 500, 14 },
    {"  f/5.6", 0x30, 560, 15 },
    {"  f/6.3", 0x33, 630, 16 },
    {"  f/7.1", 0x35, 710, 17 },
    {"  f/8.0", 0x38, 800, 18 },
    {"  f/9.0", 0x3b, 900, 19 },
    {"   f/10", 0x3d, 1000, 20 },
    {"   f/11", 0x40, 1100, 21 },
    {"   f/13", 0x43, 1300, 22 },
    {"   f/14", 0x45, 1400, 23 },
    {"   f/16", 0x48, 1600, 24 },
    {"   f/18", 0x4b, 1800, 25 },
    {"   f/20", 0x4d, 2000, 26 },
    {"   f/22", 0x50, 2200, 27 },
    {"   f/25", 0x53, 2500, 28 },
    {"   f/29", 0x55, 2900, 29 },
    {"   f/32", 0x58, 3200, 30 },
    {"   f/36", 0x5b, 3600, 31 }
};

const PTPCamera::propertyDescription_t PTPCamera::m_shutterList[] PROGMEM = {
    {"  Error", 0xFF, 0x000000FF, 255 },
    {" 1/8000", 0xa0, 0x00000000, 82 },
    {" 1/6400", 0x9d, 0x00000000, 81 },
    {" 1/5000", 0x9b, 0x00000000, 80 },
    {" 1/4000", 0x98, 2,      79 },
    {" 1/3200", 0x95, 3,      78 },
    {" 1/2500", 0x93, 4,      77 },
    {" 1/2000", 0x90, 5,      76 },
    {" 1/1600", 0x8d, 6,      75 },
    {" 1/1250", 0x8b, 8,      74 },
    {" 1/1000", 0x88, 10,     73 },
    {"  1/800", 0x85, 12,     72 },
    {"  1/640", 0x83, 15,     71 },
    {"  1/500", 0x80, 20,     70 },
    {"  1/400", 0x7d, 25,     69 },
    {"  1/320", 0x7b, 31,     68 },
    {"  1/250", 0x78, 40,     67 },
    {"  1/200", 0x75, 50,     66 },
    {"  1/160", 0x73, 62,     65 },
    {"  1/125", 0x70, 80,     64 },
    {"  1/100", 0x6d, 100,    63 },
    {"   1/80", 0x6b, 125,    62 },
    {"   1/60", 0x68, 166,    61 },
    {"   1/50", 0x65, 200,    60 },
    {"   1/40", 0x63, 250,    59 },
    {"   1/30", 0x60, 333,    58 },
    {"   1/25", 0x5d, 400,    57 },
    {"   1/20", 0x5b, 500,    56 },
    {"   1/15", 0x58, 666,    55 },
    {"   1/13", 0x55, 769,    54 },
    {"   1/10", 0x53, 1000,   53 },
    {"    1/8", 0x50, 1250,   52 },
    {"    1/6", 0x4d, 1666,   51 },
    {"    1/5", 0x4b, 2000,   50 },
    {"    1/4", 0x48, 2500,   49 },
    {"    0.3", 0x45, 3333,   48 },
    {"    0.4", 0x43, 4000,   47 },
    {"    0.5", 0x40, 5000,   46 },
    {"    0.6", 0x3d, 6250,   45 },
    {"    0.8", 0x3b, 7692,   44 },
    {"     1s", 0x38, 10000,  43 },
    {"   1.3s", 0x35, 13000,  42 },
    {"   1.6s", 0x33, 16000,  41 },
    {"     2s", 0x30, 20000,  40 },
    {"   2.5s", 0x2d, 25000,  39 },
    {"   3.2s", 0x2b, 30000,  38 },
    {"     4s", 0x28, 40000,  37 },
    {"     5s", 0x25, 50000,  36 },
    {"     6s", 0x23, 60000,  35 },
    {"     8s", 0x20, 80000,  34 },
    {"    10s", 0x1d, 100000, 33 },
    {"    13s", 0x1b, 130000, 32 },
    {"    15s", 0x18, 150000, 31 },
    {"    20s", 0x15, 200000, 30 },
    {"    25s", 0x13, 250000, 29 },
    {"    30s", 0x10, 300000, 28 },
    {"   Bulb", 0x0c, 0xffffffff, 253 }
};

const PTPCamera::propertyDescription_t PTPCamera::m_isoList[] PROGMEM = {
    {"  Error", 0xFF, 0x000000FF, 255 },
    {"   Auto", 0x00, 0x00000000, 254 },
    {"     50", 0x40, 50, 46 },
    {"    100", 0x48, 100, 43 },
    {"    125", 0x4b, 125, 42 },
    {"    160", 0x4d, 160, 41 },
    {"    200", 0x50, 200, 40 },
    {"    250", 0x53, 250, 39 },
    {"    320", 0x55, 320, 38 },
    {"    400", 0x58, 400, 37 },
    {"    500", 0x5b, 500, 36 },
    {"    640", 0x5d, 640, 35 },
    {"    800", 0x60, 800, 34 },
    {"   1000", 0x63, 1000, 33 },
    {"   1250", 0x65, 1250, 32 },
    {"   1600", 0x68, 1600, 31 },
    {"   2000", 0x6B, 2000, 30 },
    {"   2500", 0x6D, 2500, 29 },
    {"   3200", 0x70, 3200, 28 },
    {"   4000", 0x73, 4000, 27 },
    {"   5000", 0x75, 5000, 26 },
    {"   6400", 0x78, 6400, 25 },
    {"   8000", 0x7B, 8000, 24 },
    {"  10000", 0x7D, 10000, 23 },
    {"  12800", 0x80, 12800, 22 },
    {"  16000", 0x83, 16000, 21 },
    {"  20000", 0x85, 20000, 20 },
    {"  25600", 0x88, 25600, 19 },
    {"  32000", 0x8B, 32000, 18 },
    {"  40000", 0x8D, 40000, 17 },
    {"  51200", 0x90, 51200, 16 },
    {"  64000", 0x93, 64000, 15 },
    {"  81000", 0x95, 81000, 14 },
    {" 102400", 0x98, 102400, 13 },
    {" 129000", 0x9B, 129000, 12 },
    {" 162000", 0x9D, 162000, 11 },
    {" 204800", 0xA0, 204800, 10 }
}; 



/** Constructor */

PTPCamera::PTPCamera() {
    
    m_ptpErr    = 0;
    m_ready     = false;
    m_vendor    = VENDOR_PTP;
    m_busy      = false;
    m_lvOn      = false;
    m_recStart  = false;
    m_ptpUpdate = false;
    
}

/** Initiate Connection to Camera
 
 You must call this method after init(), and before running
 any command.
 
 If the camera USB interface is not yet fully enumerated, this method
 will do nothing. 
 
 @return
 true if the camera was successfully connected, false otherwise. If false is returned,
 check status() and error() to see why.
 
*/

bool PTPCamera::connect() {
    
    if( status() != STAT_CONNECTED )
        return false;
    
    
    if( PTP_CameraVendor == PTP_VENDOR_CANON ) {
        m_vendor = VENDOR_CANON;
            // store settings offset
        m_offset = (uint16_t)(((uint8_t*)&m_isoList[0].eos) - (uint8_t *)&m_isoList[0].name[0]);
    }
    
    else if( PTP_CameraVendor == PTP_VENDOR_NIKON ) {
        m_vendor = VENDOR_NIKON;
        m_offset = (uint16_t)(((uint8_t*)&m_isoList[0].nikon) - (uint8_t *)&m_isoList[0].name[0]);
    }
    
    else {
            // neither canon nor nikon
        switch(PTP_CameraVendor) {
            case PTP_VENDOR_EASTMAN_KODAK:
            case PTP_VENDOR_AGILENT:
            case PTP_VENDOR_POLAROID:
            case PTP_VENDOR_AGFA_GEVAERT:
            case PTP_VENDOR_MICROSOFT:
            case PTP_VENDOR_EQUINOX:
            case PTP_VENDOR_VIEWQUEST:
            case PTP_VENDOR_STMICROELECTRONICS:
                m_vendor = VENDOR_PTP;
                break;
            default:
                status(STAT_ERROR);
                error(ERROR_SUPPORTED);
                return false;
        }
        
        m_offset = (uint16_t)(((uint8_t*)&m_isoList[0].nikon) - (uint8_t *)&m_isoList[0].name[0]);
        
    }

    
    _checkOps();
    
    
    if( m_vendor == VENDOR_CANON ) {
        uint32_t data[1];
        
        data[0] = 0x00000001;
        
            // setup camera
        
        if( PTP_Transaction(EOS_OC_PC_CONNECT, 0, 1, data, 0, NULL) != PTP_OK ) {
            status(STAT_ERROR);
            error(ERROR_SUPPORTED);
            return false;
        }

        data[0] = 0x00000001;

        if( PTP_Transaction(EOS_OC_EXTENDED_EVENT_INFO_SET, 0, 1, data, 0, NULL) != PTP_OK ) {
            status(STAT_ERROR);
            error(ERROR_SUPPORTED);
            return false;
        }
        
    }
    
    
    check();
        
    m_ready = true;
    
    return true;
    
}

/** Check Camera Status, Perform Required Actions
 
 This method perfoms needed USB tasks, you must call this method
 regularly.
 */

bool PTPCamera::check() {
    
    _doTasks();
    
    if(! m_ready == 0) 
        return false;

    if(m_vendor == VENDOR_NIKON)
        return _processEventNikon();
    else if(m_vendor == VENDOR_CANON )
        return _processEventCanon();
    else
        return false;
 
}


/** Get Current ISO as Index
 
 Returns the current ISO as an index value (index values are
 used in commands to the camera).
 
 @return
 0 if no no ISOs supported, or the index value currently set
 
 */

uint8_t PTPCamera::iso() {
	if(m_capabilities.isoCount > 0) 
        return _isoEv(m_curISO); 
    else 
        return 0;
}

/** Get Current Aperture as Index
 
 Returns the current Aperture as an index value (index values are
 used in commands to the camera).
 
 @return
 0 if no no Apertures supported, or the index value currently set
 
 */


uint8_t PTPCamera::aperture() {
	if(m_capabilities.apertureCount > 0) 
        return _apertureEv(m_curAperture); 
    else 
        return 0;
}

/** Get Current Shutter as Index
 
 Returns the current Shutter as an index value (index values are
 used in commands to the camera).
 
 @return
 0 if no no Shutter supported, or the index value currently set
 
 */

uint8_t PTPCamera::shutter() {
	if(m_capabilities.shutterCount > 0) 
        return _shutterEv(m_curShutter); 
    else 
        return 0;
}

/** Set ISO Value by Index
 
 Sets the current ISO value, by index.
 
 @param p_idx
 The current index
 
 @return
 true if the command succeeds, false otherwise
 */

bool PTPCamera::iso(uint8_t p_idx) {
	if(p_idx == 0xff) 
        return false;
    
	if(m_vendor == VENDOR_CANON){ 
		return _setEosParameter(EOS_DPC_ISO, _isoEv(p_idx, true));
	}
	else {
		return _setPtpParameter(NIKON_DPC_ISO, (uint16_t) _isoEv(p_idx, true));
	}
}

/** Set Shutter Value by Index
 
 Sets the current Shutter value, by index.
 
 @param p_idx
 The current index
 
 @return
 true if the command succeeds, false otherwise
 */

bool PTPCamera::shutter(uint8_t p_idx) {
    
	if(p_idx == 0xff) 
        return false;

	// manualMode();
    
	if(m_vendor == VENDOR_CANON) {
		return _setEosParameter(EOS_DPC_SHUTTER, _shutterEv(p_idx, true));
	}
	else {
		return _setPtpParameter(NIKON_DPC_SHUTTER, (uint32_t) _shutterEv(p_idx, true));
	}
}

/** Set Aperture Value by Index
 
 Sets the current Aperture value, by index.
 
 @param p_idx
 The current index
 
 @return
 true if the command succeeds, false otherwise
 */


bool PTPCamera::aperture(uint8_t p_idx) {
    
	if(p_idx == 0xff) 
        return 1;
    
	if(m_vendor == VENDOR_CANON) {
		return _setEosParameter(EOS_DPC_APERTURE, _apertureEv(p_idx, true));
	}
	else {
		return _setPtpParameter(NIKON_DPC_APERTURE, (uint16_t) _apertureEv(p_idx, true));
	}
}

/** Trigger a Capture
 
 Triggers a capture.  If the camera is busy, or an error occurs
 false will be returned, otherwise true will be returned.
 
 @return
 true if capture successfully triggered, false otherwise
 */

bool PTPCamera::capture() {
    
    if( ! m_ready || m_busy )
        return false;
    
    //    if(!static_ready) return 0;
    //	if(modePTP == 0x04 || preBulbShutter) manualMode();
    //	busy = true;
    
	if(m_vendor == VENDOR_CANON) {
		if( PTP_Transaction(EOS_OC_CAPTURE, 0, 0, NULL, 0, NULL) == PTP_OK )
            return true;
	}
  	else {
        uint32_t data[2] = { 0x00000000, 0x00000000 };
		if( PTP_Transaction(PTP_OC_CAPTURE, 0, 2, data, 0, NULL) == PTP_OK )
            return true;
	}
    
    return false;
    
}

/** Live View Control
 
 Enables or disables live view, for Canon cameras only.
 
 @param p_on
 true to enable live view, false to disable
 
 @return
 true if successful, false otherwise
 */

bool PTPCamera::liveView(bool p_on) {
    
    if( ! m_ready )
        return false;
    
        // only canon supported here
    
    if( m_vendor != VENDOR_CANON )
        return false;
    
    if( !  _setEosParameter(EOS_DPC_LiveView, (p_on) ? 2 : 0) )
       return false;
    
    
    m_lvOn = p_on;
    
}

bool PTPCamera::moveFocus(int16_t p_steps) {
    
    uint8_t ret;
    
    if( m_vendor == VENDOR_CANON ) {
        if( ! m_lvOn )
            return false;
        
        uint32_t data[1] = { (uint32_t) p_steps };
        ret = PTP_Transaction(EOS_OC_MoveFocus, 0, 1, data, 0, NULL);
    }
    else if( m_vendor == VENDOR_NIKON ) {
        
        uint32_t data[2];
        data[0] = (uint32_t) p_steps > 0 ? 1 : 0;
        
        p_steps = p_steps < 0 ? p_steps * -1 : p_steps;
        
        data[1] = (uint32_t) p_steps;
        
        ret = PTP_Transaction(NIKON_OC_MoveFocus, 0, 2, data, 0, NULL);
    }
    else {
            // unsupported camera
        return false;
    }
    
    if( ret == PTP_ERROR )
        return false;
    
    return true;
    
}


bool PTPCamera::videoStart() {
     
    if( ! m_ready || m_busy) 
        return false;
    
	if( ! m_props.video ) 
        return false;
    
        // only canon supported here
    
	if(m_vendor == VENDOR_CANON && m_lvOn) {
		m_recStart = true;
		m_busy     = true;
        
		return _setEosParameter(EOS_DPC_Video, 0x04);
                    
        return true;
	}
    
	return false;

}

bool PTPCamera::videoStop() {
    
        // only canon supported here
    
    if(m_vendor == VENDOR_CANON && m_recStart) {
		m_busy     = false;
		m_recStart = false;
        
		return _setEosParameter(EOS_DPC_Video, 0x00);
        
        return true;
	}
    
    return false;

}

/** Set Last PTP OP Code That Errored
 
 Set the last OP code that errored, or zero
 if no error to have occurred.
 
 @param p_err
 The op code that errored
 */

void PTPCamera::ptpError(uint16_t p_err) {
    m_ptpErr = p_err;
}


/** Get Last PTP Op Code That Errored
 
 Returns the last PTP Op Code that produced an error, if
 no Op Code produced an error, returns 0.
 
 @return
 The last op code that errored, or 0 if no op code errored
 */


uint16_t PTPCamera::ptpError() {
    return m_ptpErr;
}

/** Disable the PTP Interface */

void PTPCamera::disable() {
    PTP_CloseSession();
    USB_USBTask();
    USB_Detach();
    USB_Disable();
    return;
}



void PTPCamera::_checkOps() {
    
    uint8_t bulb_support = 0;
    
    
    // loop through operations and see what activities the
    // camera supports
    
    for(uint16_t i = 0; i < PTP_OpsCount; i++) {
        
        if(m_vendor == VENDOR_CANON) {
            switch(PTP_Ops[i]) {
                case EOS_OC_MoveFocus:
                    m_props.focus = true;
                    break;
                case EOS_OC_CAPTURE:
                    m_props.capture = true;
                    break;
                case EOS_OC_VIDEO_START:
                    m_props.video = true;
                    break;
                    //				case EOS_OC_SETUILOCK:
                    //				case EOS_OC_RESETUILOCK:
                    //				case EOS_OC_BULBSTART:
                    //				case EOS_OC_BULBEND:
                case EOS_OC_REMOTE_RELEASE_ON:
                case EOS_OC_REMOTE_RELEASE_OFF:
                    bulb_support++;
                    if(bulb_support == 2) 
                        m_props.bulb = true;
                    break;
                    // case EOS_OC_PC_CONNECT:
                    //    pc_connect = 1;
                    //    break;
            }
            
        }
        else if(m_vendor == VENDOR_NIKON) {
            switch(PTP_Ops[i]) {
                case PTP_OC_CAPTURE:
                    m_props.capture = true;
                    break;
                case NIKON_OC_BULBSTART:
                case NIKON_OC_BULBEND:
                    m_props.bulb = true;
                    break;
                case PTP_OC_PROPERTY_SET:
                    m_props.iso = true;
                    m_props.aperture = true;
                    m_props.shutter = true;
                    break;
                case NIKON_OC_CAMERA_READY:
                    m_props.ready = true;
                    break;
            }
            
        }
        else {
            // neither canon nor nikon - only capture supported
            if( PTP_Ops[i] == PTP_OC_CAPTURE )
                m_props.capture = true;
        }
    }
    
}



bool PTPCamera::_processEventNikon() {
    
    uint8_t ret;
    uint32_t event_size;
    uint32_t event_type;
    uint32_t event_item;
    uint32_t event_value;
    uint32_t i = 0;

    if(m_props.ready) {
        ret = PTP_Transaction(NIKON_OC_CAMERA_READY, 0, 0, NULL, 0, NULL);
        if(PTP_Response_Code == 0x2019) {
            // wdt_reset();
            m_busy = true;
            return true;
        }
        m_busy = false;
    }
    
    ret = PTP_Transaction(NIKON_OC_EVENT_GET, 1, 0, NULL, 0, NULL);
    
    uint16_t nevents, tevent;
    memcpy(&nevents, &PTP_Buffer[i], sizeof(uint16_t));
    
    i += sizeof(uint16_t);
    
    while(i < PTP_Bytes_Received) {
        
        memcpy(&tevent, &PTP_Buffer[i], sizeof(uint16_t));
        i += sizeof(uint16_t);
        memcpy(&event_value, &PTP_Buffer[i], sizeof(uint32_t));
        i += sizeof(uint32_t);
        
        switch(tevent) {
            case PTP_EC_OBJECT_CREATED:
                // currentObject = event_value; // Save the object ID for later retrieving the thumbnail
                break;
            case PTP_EC_PROPERTY_CHANGED:
                m_ptpUpdate = true;
                break;
        }
        
        if(i >= PTP_BUFFER_SIZE) 
            break;
    }
    
    if(m_ptpUpdate) 
        _updatePtpParameters();
    

    
}


bool PTPCamera::_processEventCanon() {
    
    uint8_t ret;
    uint32_t event_size;
    uint32_t event_type;
    uint32_t event_item;
    uint32_t event_value;
    uint32_t i = 0;

    ret = PTP_Transaction(EOS_OC_EVENT_GET, 1, 0, NULL, 0, NULL);
 
    if(ret == PTP_ERROR) 
        return false;	
    else if(PTP_Bytes_Received == 0)
         m_busy = false;
    

    while(ret == PTP_DATA_REMAINING) {
            // fetch next data packet
        ret = PTP_FetchData(0);
        
        if(ret == PTP_ERROR)
            return false;	
        
        while(i < PTP_Bytes_Received) {
            
            if(i + sizeof(uint32_t) >= PTP_BUFFER_SIZE) {
                
                if(ret == PTP_DATA_REMAINING) {
                        // pre-fetch next packet
                    ret = PTP_FetchData(PTP_BUFFER_SIZE - i);
                    
                    if(ret == PTP_ERROR) 
                        return false;
                    
                    i = 0;
                    continue;
                }
                else {
                        // pre-fecth incomplete
                    return false;
                }
            }
            
            memcpy(&event_size, &PTP_Buffer[i], sizeof(uint32_t));
            
            if(event_size == 0) {
                // zero-length events are a problem
                return false;
            }
            
            if((event_size + i) > PTP_BUFFER_SIZE) {
                
                if(ret == PTP_DATA_REMAINING) {
                    
                    if(event_size > PTP_BUFFER_SIZE) {
                        // too big
                        
                        while(event_size > PTP_BUFFER_SIZE && ret == PTP_DATA_REMAINING) {
                            ret = PTP_FetchData(0);
                            event_size -= PTP_BUFFER_SIZE;
                        }
                        
                        i = event_size;
                        continue;
                    }
                    else  {
                        
                        ret = PTP_FetchData(PTP_BUFFER_SIZE - i);
                        
                        if(ret == PTP_ERROR) 
                            return false;
                        
                        i = 0;
                        continue;
                        
                    }
                } // end if(ret ==
                else {
                    return false;
                }
            } // end if(( event_size...
            
            memcpy(&event_type, &PTP_Buffer[i + sizeof(uint32_t) * 1], sizeof(uint32_t));
            memcpy(&event_item, &PTP_Buffer[i + sizeof(uint32_t) * 2], sizeof(uint32_t));
            memcpy(&event_value, &PTP_Buffer[i + sizeof(uint32_t) * 3], sizeof(uint32_t));
            
            if(event_type == EOS_EC_PROPERTY_CHANGE) {
                switch(event_item)  {
                    case EOS_DPC_ISO:
                            // iso
                        m_curISO = event_value;
                        break;
                    case EOS_DPC_SHUTTER:
                            // shutter
                        m_curShutter = event_value;
                        break;
                    case EOS_DPC_APERTURE:
                            // aperture
                        m_curAperture = event_value;
                        break;
                    case EOS_DPC_MODE:
                            // mode
                        // modePTP = event_value;
                        break;
                }
            }
            else if(event_type == EOS_EC_PROPERTY_VALUES) {
                
                uint32_t x;
                
                switch(event_item) {
                    case EOS_DPC_ISO:
                        for(x = 0; x < event_size / sizeof(uint32_t) - 5; x++) {
                            m_capabilities.isoAvail[x] = PTPCamera::_isoEv(PTP_Buffer[i+(x+5)*sizeof(uint32_t)]);
                        }
                        m_capabilities.isoCount = x;
                        m_props.iso = x > 0;
                        
                        break;
                    case EOS_DPC_SHUTTER:
                        for(x = 0; x < event_size / sizeof(uint32_t) - 5; x++) {
                            m_capabilities.shutterAvail[x] = PTPCamera::_shutterEv(PTP_Buffer[i+(x+5)*sizeof(uint32_t)]);
                        }
                        m_capabilities.shutterCount = x;
                        m_props.shutter = x > 0;
                        
                        break;
                    case EOS_DPC_APERTURE:
                        for(x = 0; x < event_size / sizeof(uint32_t) - 5; x++) {
                            m_capabilities.apertureAvail[x] = PTPCamera::_apertureEv(PTP_Buffer[i+(x+5)*sizeof(uint32_t)]);
                        }
                        m_capabilities.apertureCount = x;
                        m_props.aperture = x > 0;
                        
                        break;
                }
            }
            else if(event_type == EOS_EC_OBJECT_CREATED) {
                m_busy = false;
                
                // memcpy(&currentObject, &PTP_Buffer[i + sizeof(uint32_t) * 2], sizeof(uint32_t)); // Save the object ID for later retrieving the thumbnail
                                
                //getThumb(currentObject);
            }
 /*           else if(event_type == EOS_EC_WillShutdownSoon) {
                //				debug(STR("Keeping camera on\r\n"));
                //				PTP_Transaction(EOS_OC_KeepDeviceOn, 0, 0, NULL);
            }
            else if(event_type > 0)  {
                // non-supported event
            } */
            
            i += event_size;
        } // end while( i < PTP_Bytes...
    } // end while( ret == 
    
    return true;
}


bool PTPCamera::_setEosParameter(uint16_t p_param, uint32_t p_value) {
	// shutter_off_quick(); // Can't set parameters while half-pressed
    uint32_t data[3];
    
	data[0] = 0x0000000C;
	data[1] = (uint32_t) p_param;
	data[2] = (uint32_t) p_value;
	uint8_t ret = PTP_Transaction(EOS_OC_PROPERTY_SET, 0, 0, NULL, sizeof(uint32_t) * 3, (uint8_t*)data);
    
    if( ret == PTP_ERROR )
        return false;
    
    return true;
}

bool PTPCamera::_setPtpParameter(uint16_t p_param, uint32_t p_value) {
	m_ptpUpdate = 1;
    uint32_t data[3];
	data[0] = (uint32_t)p_param;
	// shutter_off_quick(); // Can't set parameters while half-pressed
	uint8_t ret = PTP_Transaction(PTP_OC_PROPERTY_SET, 1, 1, data, sizeof(p_value), (uint8_t*) &p_value);
    
    if( ret == PTP_ERROR )
        return false;
    
    return true;

}

bool PTPCamera::_setPtpParameter(uint16_t p_param, uint16_t p_value) {
	m_ptpUpdate = 1;
    uint32_t data[3];
	data[0] = (uint32_t)p_param;
	// shutter_off_quick(); // Can't set parameters while half-pressed
	uint8_t ret = PTP_Transaction(PTP_OC_PROPERTY_SET, 1, 1, data, sizeof(p_value), (uint8_t*) &p_value);
    
    if( ret == PTP_ERROR )
        return false;
    
    return true;

}

bool PTPCamera::_setPtpParameter(uint16_t p_param, uint8_t p_value) {
	m_ptpUpdate = 1;
    uint32_t data[3];
	data[0] = (uint32_t)p_param;
	// shutter_off_quick(); // Can't set parameters while half-pressed
	uint8_t ret = PTP_Transaction(PTP_OC_PROPERTY_SET, 1, 1, data, sizeof(p_value), (uint8_t*) &p_value);
    
    if( ret == PTP_ERROR )
        return false;
    
    return true;

}

bool PTPCamera::_getPtpParameterList(uint16_t p_param, uint8_t* p_count, uint16_t* p_list, uint16_t* p_current) {
	uint8_t cnt;
    uint32_t data[3];
	data[0] = (uint32_t)p_param;
	uint8_t ret = PTP_Transaction(PTP_OC_PROPERTY_LIST, 1, 1, data, 0, NULL);
    
    if( ret == PTP_ERROR )
        return false;
    

	if(PTP_Bytes_Received > 10)	{
        
		cnt = (uint8_t)PTP_Buffer[10];
        
		memcpy(p_current, &PTP_Buffer[7], sizeof(uint16_t));
        
		for(uint8_t i = 0; i < cnt; i++) {
			memcpy(&p_list[i], &PTP_Buffer[12 + i * sizeof(uint16_t)], sizeof(uint16_t));
		}
        
		*p_count = cnt;
	}
    
	return true;
}


bool PTPCamera::_getPtpParameterList(uint16_t p_param, uint8_t* p_count, uint32_t* p_list, uint32_t* p_current) {
	uint8_t cnt;
    
    uint32_t data[3];
    
	data[0] = (uint32_t)p_param;
    
	uint8_t ret = PTP_Transaction(PTP_OC_PROPERTY_LIST, 1, 1, data, 0, NULL);
    
    if( ret == PTP_ERROR )
        return false;
        
	if(PTP_Bytes_Received > 10)	{
        
		cnt = (uint8_t)PTP_Buffer[14];
		memcpy(p_current, &PTP_Buffer[9], sizeof(uint32_t));
        
		for(uint8_t i = 0; i < cnt; i++) {
			memcpy(&p_list[i], &PTP_Buffer[16 + i * sizeof(uint32_t)], sizeof(uint32_t));
		}
        
		*p_count = cnt;
	}
    
	return true;
}

bool PTPCamera::_getPtpParameter(uint16_t p_param, uint16_t* p_value) {
    uint32_t data[3];
    
	data[0] = (uint32_t) p_param;
    
	uint8_t ret = PTP_Transaction(PTP_OC_PROPERTY_GET, 1, 1, data, 0, NULL);
    
    if( ret == PTP_ERROR )
        return false;
    
	if( PTP_Bytes_Received == sizeof(uint16_t) ) 
        *p_value = (uint16_t)PTP_Buffer;
    
	return true;
}

bool PTPCamera::_updatePtpParameters() {
    
	m_ptpUpdate = 0;
    
    uint32_t data[3];
    
	if(m_vendor == VENDOR_NIKON)	{
		/*
         getPropertyInfo(NIKON_DPC_ISO, sizeof(uint16_t), (uint16_t*)&isoAvailCount, (uint8_t*)&m_curISO, (uint8_t*)&isoAvail);
         if(isoAvailCount > 0) supports.iso = true; else supports.iso = false;
         getPropertyInfo(NIKON_DPC_APERTURE, sizeof(uint16_t), (uint16_t*)&apertureAvailCount, (uint8_t*)&m_curAperture, (uint8_t*)&apertureAvail);
         if(apertureAvailCount > 0) supports.aperture = true; else supports.aperture = false;
         getPropertyInfo(NIKON_DPC_SHUTTER, sizeof(uint32_t), (uint16_t*)&shutterAvailCount, (uint8_t*)&m_curShutter, (uint8_t*)&shutterAvail);
         if(shutterAvailCount > 0) supports.shutter = true; else supports.shutter = false;
         */
		
		data[0] = (uint32_t)NIKON_DPC_ISO;
		PTP_Transaction(PTP_OC_PROPERTY_LIST, 1, 1, data, 0, NULL);
        
		if(PTP_Bytes_Received > 10 && PTP_Buffer[2] == 4) {
            
			m_capabilities.isoCount = (uint8_t)PTP_Buffer[10];
            
			if(m_capabilities.isoCount > 0) 
                m_props.iso = true; 
            else 
                m_props.iso = false;
            
			uint16_t tmp16;
			memcpy(&tmp16, &PTP_Buffer[7], sizeof(uint16_t));
            
			m_curISO = tmp16;
            
			for(uint8_t i = 0; i < m_capabilities.isoCount; i++) {
                
				if(i >= 32) 
                    break;
                
				memcpy(&tmp16, &PTP_Buffer[12 + i * sizeof(uint16_t)], sizeof(uint16_t));
				m_capabilities.isoAvail[i] = _isoEv((uint32_t)tmp16);
			}
		}
        
		data[0] = (uint32_t)NIKON_DPC_APERTURE;
		PTP_Transaction(PTP_OC_PROPERTY_LIST, 1, 1, data, 0, NULL);
        
		if(PTP_Bytes_Received > 10 && PTP_Buffer[2] == 4) {
            
			m_capabilities.apertureCount = (uint8_t)PTP_Buffer[10];
            
			if(m_capabilities.apertureCount > 0) 
                m_props.aperture = true; 
            else 
                m_props.aperture = false;
            
			uint16_t tmp16;
			memcpy(&tmp16, &PTP_Buffer[7], sizeof(uint16_t));
            
			m_curAperture = tmp16;
            
			for(uint8_t i = 0; i < m_capabilities.apertureCount; i++) {
                
				if(i >= 32) 
                    break;
                
				memcpy(&tmp16, &PTP_Buffer[12 + i * sizeof(uint16_t)], sizeof(uint16_t));
                
				m_capabilities.apertureAvail[i] = _apertureEv((uint32_t)tmp16);
			}
		}
        
		data[0] = (uint32_t)NIKON_DPC_SHUTTER;
		PTP_Transaction(PTP_OC_PROPERTY_LIST, 1, 1, data, 0, NULL);
        
		if(PTP_Bytes_Received > 14 && PTP_Buffer[2] == 6) { 
            
			m_capabilities.shutterCount = (uint8_t)PTP_Buffer[14];
            
			if(m_capabilities.shutterCount > 0) 
                m_props.shutter = true; 
            else 
                m_props.shutter = false;
            
			memcpy(&m_curShutter, &PTP_Buffer[9], sizeof(uint32_t));
            
			for(uint8_t i = 0; i < m_capabilities.shutterCount; i++) {
                
				if(i >= 64) 
                    break;
                
				uint32_t tmp32;
				memcpy(&tmp32, &PTP_Buffer[16 + i * sizeof(uint32_t)], sizeof(uint32_t));
                
				m_capabilities.shutterAvail[i] = _shutterEv(tmp32);
			}
		}
		
	}
    
	return true;
}

// This method still needs work... //
bool PTPCamera::_getPropertyInfo(uint16_t p_prop_code, uint8_t p_expected_size, uint16_t* p_count, uint8_t* p_current, uint8_t* p_list) {
    
	//First check that the prop_code is in the list of supported properties (not implemented)
    
    uint32_t data[3];
    
	// Send a command to the device to describe this property.
	data[0] = (uint32_t)p_prop_code;
	PTP_Transaction(PTP_OC_PROPERTY_LIST, 1, 1, data, 0, NULL);
    
	// data[0]
	// data[1] -- Property code back
	uint16_t prop;
	memcpy(&prop, &PTP_Buffer[0], sizeof(uint16_t));
    
    if(prop != p_prop_code) 
        return false;
        
	// data[2]
	// data[3] -- data type code
	// Setting the type code for the property also turns its
	// support flag on.
    uint16_t type = 0;
	memcpy(&type, &PTP_Buffer[2], sizeof(uint16_t));
        
	// data[4] -- GetSet flag
    //uint8_t getset = PTP_Buffer[4];
    
    uint16_t index = 5;
	// Starting at data[5]...
	//   -- Factory Default value
	//   -- Current value
    switch (type) {
        case 0:  // UNDEFINED
        case 1:  // INT8
        case 2:  // UINT8
            if(p_expected_size != sizeof(uint8_t)) 
                  return false;
            
            memcpy(p_current, &PTP_Buffer[index], sizeof(uint8_t));
            index += sizeof(uint8_t) * 2;
            break;
        case 3:  // INT16
        case 4:  // UINT16
            if(p_expected_size != sizeof(uint16_t)) 
                  return false;
            
            memcpy(p_current, &PTP_Buffer[index], sizeof(uint16_t));
            index += sizeof(uint16_t) * 2;
            break;
        case 5:  // INT32
        case 6:  // UINT32
            if(p_expected_size != sizeof(uint32_t)) 
                  return false;
            
            memcpy(p_current, &PTP_Buffer[index], sizeof(uint32_t));
            index += sizeof(uint32_t) * 2;
            break;
        case 7:  // INT64
        case 8:  // UINT64
        case 9:  // INT128;
        case 10: // UINT128;
            return false;
            break;
        case 0xffff: // String
            return false;
            break;
        default:
            return false;
            break;
    }
    
	// The form flag...
    uint8_t form = PTP_Buffer[index];
    index++;
    
    
    if (form == 1) { // RANGE
        // The range description includes 3 values: the minimum
        // value, the maximum value and the step.
	    switch (type) {
            case 1:  // INT8
            case 2:  // UINT8
                { 
                    uint8_t* range = (uint8_t*)p_list;
                    range[0] = PTP_Buffer[index];
                    index++;
                    range[1] = PTP_Buffer[index];
                    index++;
                    range[2] = PTP_Buffer[index];
                    index++;
                }
                break;
            default: 
                break;
            
	    }
        
    } 
    else if (form == 2) { // ENUM
        // An enumeration is a complete list of the possible
        // value that the property can take.
		uint16_t enum_count;
		memcpy(&enum_count, &PTP_Buffer[index], sizeof(uint16_t));
		index += sizeof(uint16_t);
		*p_count = enum_count;
        
        
	    switch (type) {
            case 1: // INT8
            case 2: // UINT8
            {
                for (uint16_t idx = 0 ; idx < enum_count ; idx++) {
                    memcpy(&p_list[idx * sizeof(uint8_t)], &PTP_Buffer[index], sizeof(uint8_t));
                    index += sizeof(uint8_t);
                }
                break;
            }
            case 3: // INT16
            case 4: // UINT16
            {
                for (uint16_t idx = 0 ; idx < enum_count ; idx++) {
                    memcpy(&p_list[idx * sizeof(uint16_t)], &PTP_Buffer[index], sizeof(uint16_t));
                    index += sizeof(uint16_t);
                }
                break;
            }
            case 5: // INT32
            case 6: // UINT32
            {
                for (uint16_t idx = 0 ; idx < enum_count ; idx++)   {
                    memcpy(&p_list[idx * sizeof(uint32_t)], &PTP_Buffer[index], sizeof(uint32_t));
                    index += sizeof(uint32_t);
                }
                break;
            }
            case 0xffff: // String
                break;
            default:
                break;
	    }
    } 
    
    
    return true;
}



void PTPCamera::_openSession() {
    
    static uint8_t conf = 0;
    
    if(USB_HostState == HOST_STATE_Configured) {
        if(conf != USB_HostState) {
            conf = USB_HostState;
            PTP_OpenSession();
            PTP_GetDeviceInfo();
        }
    }
    else {
        conf = USB_HostState;
    }
    
}

void PTPCamera::_doTasks() {
    _openSession();
    SI_Host_USBTask(&DigitalCamera_SI_Interface);
    USB_USBTask();
}


// Get EV from camera value (p_ptp = false) or get camera value from EV (p_ptp = true)
uint8_t PTPCamera::_isoEv(uint32_t p_id, bool p_ptp) {
	for(uint8_t i = 0; i < sizeof(m_isoList) / sizeof(m_isoList[0]); i++)	{
        if( ! p_ptp ) {
            if(pgm_read_u32((uint8_t*)&m_isoList[i] + m_offset) == p_id) 
                return pgm_read_byte(&m_isoList[i].ev);
        }
        else {
            if(pgm_read_byte(&m_isoList[i].ev) == p_id)
                return pgm_read_u32((uint8_t*)&m_isoList[i] + m_offset);
        }
	}
	return 0;
}

// Get EV from camera value (p_ptp = false) or get camera value from EV (p_ptp = true)
uint8_t PTPCamera::_apertureEv(uint32_t p_id, bool p_ptp) {
	for(uint8_t i = 0; i < sizeof(m_apertureList) / sizeof(m_apertureList[0]); i++)	{
        if( ! p_ptp ) {
            if(pgm_read_u32((uint8_t*)&m_apertureList[i] + m_offset) == p_id) 
                return pgm_read_byte(&m_apertureList[i].ev);
        }
        else {
            if(pgm_read_byte(&m_apertureList[i].ev) == p_id)
                return pgm_read_u32((uint8_t*)&m_apertureList[i] + m_offset);
        }

	}
	return 0;
}

// Get EV from camera value (p_ptp = false) or get camera value from EV (p_ptp = true)
uint8_t PTPCamera::_shutterEv(uint32_t p_id, bool p_ptp) {
	for(uint8_t i = 0; i < sizeof(m_shutterList) / sizeof(m_shutterList[0]); i++)	{
        if( ! p_ptp ) {
            if(pgm_read_u32((uint8_t*)&m_shutterList[i] + m_offset) == p_id) 
                return pgm_read_byte(&m_shutterList[i].ev);
        }
        else {
            if(pgm_read_byte(&m_shutterList[i].ev) == p_id)
                return pgm_read_u32((uint8_t*)&m_shutterList[i] + m_offset);
        }
	}
	return 0;
}



// create an object, we'll need it below
PTPCamera Camera;




//=================
// Global Functions
//=================


/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached(void) {
	Camera.status(STAT_CONNECTED);
    Camera.error(ERROR_NONE);
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached(void) {
	Camera.status(STAT_NOTCONNECTED);
    Camera.error(ERROR_NONE);
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete(void) {
    
	uint16_t ConfigDescriptorSize;
    
    if ( USB_Host_GetDeviceConfigDescriptor(1, &ConfigDescriptorSize, PTP_Buffer, sizeof(PTP_Buffer)) != HOST_GETCONFIG_Successful ) {
        Camera.status(STAT_ERROR);
        Camera.error(ERROR_CFGDESC);
		return;
    }
    
    if ( SI_Host_ConfigurePipes(&DigitalCamera_SI_Interface, ConfigDescriptorSize, PTP_Buffer) != SI_ENUMERROR_NoError ) {
        Camera.status(STAT_ERROR);
        Camera.error(ERROR_DEV);
		return;
	}
    
    if (USB_Host_SetDeviceConfiguration(1) != HOST_SENDCONTROL_Successful) {
        Camera.status(STAT_ERROR);
        Camera.error(ERROR_SETCFG);
		return;
	}
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError(const uint8_t p_err) {
	USB_Disable();
    
    Camera.status(STAT_ERROR);
    Camera.error(ERROR_HARD);
    
}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t p_err, const uint8_t p_suberr) {
    Camera.status(STAT_ERROR);
    Camera.error(ERROR_HARD);
}






PTPStat PTP_Transaction(uint16_t p_op, uint8_t p_recv, uint8_t p_parmCount, uint32_t *p_parms, uint8_t p_dataCount, uint8_t *p_data) {

    
    uint8_t error_code;
    
    PIMA_Container_t PIMA_Block  = {
        // .DataLength = 
        CPU_TO_LE32(PIMA_DATA_SIZE(p_dataCount)),
        // .Type = 
        CPU_TO_LE16(PIMA_CONTAINER_DataBlock),
        // .Code = 
        CPU_TO_LE16(p_op)
    };

    
    if( Camera.ptpError() ) 
        return PTP_ERROR;
    
    if(PTP_Bytes_Remaining > 0) 
        return PTP_FetchData(0);
    
    if(p_parmCount > 0 && p_parms)
        SI_Host_SendCommand(&DigitalCamera_SI_Interface, CPU_TO_LE16(p_op), p_parmCount, p_parms);
    else
        SI_Host_SendCommand(&DigitalCamera_SI_Interface, CPU_TO_LE16(p_op), 0, NULL);
    
    if(p_dataCount > 0 && p_data) {
            // send data, if necessary
        
        DigitalCamera_SI_Interface.State.TransactionID--;
        
       // PIMA_Block        
        memcpy(&PIMA_Block.Params, p_data, p_dataCount);
        SI_Host_SendBlockHeader(&DigitalCamera_SI_Interface, &PIMA_Block);
    }
    else if(p_recv) {
            // receive data
        PTP_Bytes_Received = 0;
        SI_Host_ReceiveBlockHeader(&DigitalCamera_SI_Interface, &PIMA_Block);
        PTP_Bytes_Received = (PIMA_Block.DataLength - PIMA_COMMAND_SIZE(0));

        PTP_Bytes_Total = PTP_Bytes_Received;
        if(PTP_Bytes_Received > PTP_BUFFER_SIZE) {
            PTP_Bytes_Remaining = PTP_Bytes_Received - PTP_BUFFER_SIZE;
            PTP_Bytes_Received = PTP_BUFFER_SIZE;
            SI_Host_ReadData(&DigitalCamera_SI_Interface, PTP_Buffer, PTP_Bytes_Received);

            return PTP_DATA_REMAINING;
        }
        else {
            PTP_Bytes_Remaining = 0;
            if(PTP_Bytes_Received > 0) SI_Host_ReadData(&DigitalCamera_SI_Interface, PTP_Buffer, PTP_Bytes_Received);
        }
    }
    
    PTP_Response_Code = 0;
    error_code = SI_Host_ReceiveResponseCode(&DigitalCamera_SI_Interface, &PIMA_Block);
    PTP_Response_Code = PIMA_Block.Code;
    
    if(PTP_Response_Code == 0x2019) 
        error_code = 0; // Ignore BUSY error

    if(PTP_Response_Code) {
        Camera.ptpError(p_op);
        //USB_Host_SetDeviceConfiguration(0);
        return PTP_ERROR;
    }
    
    return PTP_OK;
}

PTPStat PTP_FetchData(uint16_t p_offset) {
    
    if(PTP_Bytes_Remaining > 0) {
        
        if(PTP_Bytes_Remaining > PTP_BUFFER_SIZE) 
            PTP_Bytes_Received = PTP_BUFFER_SIZE; 
        else 
            PTP_Bytes_Received = PTP_Bytes_Remaining;
        
        if(p_offset > 0) {
            if(PTP_Bytes_Received + p_offset > PTP_BUFFER_SIZE) 
                PTP_Bytes_Received = PTP_BUFFER_SIZE - p_offset;
            memmove(PTP_Buffer, PTP_Buffer + (PTP_BUFFER_SIZE - p_offset), p_offset);
        }
        
        PTP_Bytes_Remaining -= PTP_Bytes_Received;
        SI_Host_ReadData(&DigitalCamera_SI_Interface, (PTP_Buffer + p_offset), PTP_Bytes_Received);
        
        PTP_Bytes_Received += p_offset;
        
        if(PTP_Bytes_Remaining == 0) {
            if (SI_Host_ReceiveResponse(&DigitalCamera_SI_Interface)) {
                // puts_P(PSTR("PTP_FetchData Error.\r\n"));
                USB_Host_SetDeviceConfiguration(0);
                return PTP_ERROR;
            }
            return PTP_OK;
        }
        return PTP_DATA_REMAINING;
    }
    else {
        return PTP_ERROR;
    }
}

uint8_t SI_Host_ReceiveResponseCode(USB_ClassInfo_SI_Host_t* const p_interface, PIMA_Container_t *p_PIMABlock) {

    uint8_t ErrorCode;
    
    if ((USB_HostState != HOST_STATE_Configured) || !(p_interface->State.IsActive))
        return PIPE_RWSTREAM_DeviceDisconnected;
    
    if ((ErrorCode = SI_Host_ReceiveBlockHeader(p_interface, p_PIMABlock)) != PIPE_RWSTREAM_NoError)
        return ErrorCode;
    
    if ((p_PIMABlock->Type != CPU_TO_LE16(PIMA_CONTAINER_ResponseBlock)) || (p_PIMABlock->Code != CPU_TO_LE16(0x2001)))
        return SI_ERROR_LOGICAL_CMD_FAILED;
    
    return PIPE_RWSTREAM_NoError;
}

PTPStat PTP_OpenSession() {
    if (SI_Host_OpenSession(&DigitalCamera_SI_Interface) != PIPE_RWSTREAM_NoError) {
        // puts_P(PSTR("Could not open PIMA session.\r\n"));

        USB_Host_SetDeviceConfiguration(0);
        return PTP_ERROR;
    }
    
    return PTP_OK;
}

PTPStat PTP_CloseSession() {
    if (SI_Host_CloseSession(&DigitalCamera_SI_Interface) != PIPE_RWSTREAM_NoError) {

        // puts_P(PSTR("Could not close PIMA session.\r\n"));
        USB_Host_SetDeviceConfiguration(0);
        return PTP_ERROR;
    }
    
    USB_Host_SetDeviceConfiguration(0);
    return PTP_OK;
}

PTPStat PTP_GetDeviceInfo() {
    
    if(PTP_Transaction(PIMA_OPERATION_GETDEVICEINFO, 1, 0, NULL, 0, NULL)) 
        return PTP_ERROR;
    
    char *DeviceInfoPos = PTP_Buffer;
    
    /* Skip over the data before the unicode device information strings */
    DeviceInfoPos += 8;                                          // Skip to VendorExtensionDesc String
    
        // extract vendor id
    char VendorId[*DeviceInfoPos];
    UnicodeToASCII(DeviceInfoPos, VendorId);
    
    PTP_CameraVendor = strtoul(VendorId, (char**)NULL, 16);
    
    DeviceInfoPos += (1 + UNICODE_STRING_LENGTH(*DeviceInfoPos)); // Skip over VendorExtensionDesc String
    DeviceInfoPos += 2;                                          // Skip over FunctionalMode
    
        // extract supported operations
    PTP_OpsCount = (uint16_t) *(uint32_t*)DeviceInfoPos;
    PTP_Ops      = (uint16_t*)(DeviceInfoPos + 4);
    
    DeviceInfoPos += (4 + (*(uint32_t*)DeviceInfoPos << 1));      // Skip over Supported Operations Array
    DeviceInfoPos += (4 + (*(uint32_t*)DeviceInfoPos << 1));      // Skip over Supported Events Array
    DeviceInfoPos += (4 + (*(uint32_t*)DeviceInfoPos << 1));      // Skip over Supported Device Properties Array
    DeviceInfoPos += (4 + (*(uint32_t*)DeviceInfoPos << 1));      // Skip over Capture Formats Array
    DeviceInfoPos += (4 + (*(uint32_t*)DeviceInfoPos << 1));      // Skip over Image Formats Array
    DeviceInfoPos += 1 + UNICODE_STRING_LENGTH(*DeviceInfoPos);   // Skip over Manufacturer String
    
    /* Extract and convert the Model Unicode string to ASCII and print it through the USART */
    char Model[*DeviceInfoPos];
    UnicodeToASCII(DeviceInfoPos, Model);
    strncpy(PTP_CameraModel, Model, 22);
 
    DeviceInfoPos += 1 + UNICODE_STRING_LENGTH(*DeviceInfoPos);   // Skip over Model String

    return PTP_OK;
}


/** Function to convert a given Unicode encoded string to ASCII. This function will only work correctly on Unicode
 *  strings which contain ASCII printable characters only.
 *
 *  \param[in] UnicodeString  Pointer to a Unicode encoded input string
 *  \param[out] Buffer        Pointer to a buffer where the converted ASCII string should be stored
 */
void UnicodeToASCII(char *p_string, char *p_buf) {
    /* Get the number of characters in the string, skip to the start of the string data */
    uint8_t CharactersRemaining = *(p_string);
    p_string++;
    
    /* Loop through the entire unicode string */
    while (--CharactersRemaining) {
        /* Load in the next unicode character (only the lower byte, as only Unicode coded ASCII is supported) */
        *(p_buf++) = *p_string;
        
        /* Jump to the next unicode character */
        p_string += 2;
    }
    
    /* Null terminate the string */
    *p_buf = 0;
}



