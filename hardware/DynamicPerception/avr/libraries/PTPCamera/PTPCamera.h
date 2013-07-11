/* Camera PTP Host Arduino LUFA Library
 
 C.A. Church / Dynamic Perception LLC
 
 Based on the excellent work by Dean Camera for his LUFA project, and
 on the excellent work done by Elijah Parker for Timelapse+.
 
 Included in this work is code derived from GPLv3-licensed code written by 
 Elijah Parker, and as such, this entire library is licensed under GPLv3.  
 Please make note of this, as most other ArduinoLUFA libraries are MIT-Licensed.
  
 New Code License:
 
 Copyright 2013 C.A. Church / Dynamic Perception LLC
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */


/** @file PTPCamera.h
 
 @brief Core PTPCamera.h Header File
 */

#ifndef _PTPCAMERA_H_
#define _PTPCAMERA_H_

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "../ArduinoLUFA/ArduinoLUFA.h"
#include "PTPCodes.h"

#if ! defined(USB_CAN_BE_BOTH) || ! defined(USB_CAN_BE_HOST)
    #error The current board does not support host mode
#endif

extern char PTP_Buffer[PTP_BUFFER_SIZE];
extern uint16_t PTP_Bytes_Received, PTP_Bytes_Remaining, PTP_Bytes_Total, PTP_Response_Code, PTP_OpsCount;
extern char PTP_CameraModel[23];
extern uint16_t* PTP_Ops; // note that this memory space is reused -- only available immediately after init
extern uint32_t PTP_CameraVendor;

enum PTPStat {
    PTP_ERROR          = -1,
    PTP_OK             = 0,
    PTP_DATA_REMAINING = 1,
    PTP_UNSUPPORTED
};

enum PTPVendor {
  VENDOR_CANON,
  VENDOR_NIKON,
  VENDOR_PTP
};

class PTPCamera : public ArduinoLUFA {
    
public:
    
        // ctor
    PTPCamera();

    void disable();

    uint16_t ptpError();
    void ptpError(uint16_t p_err);
    
    bool connect();
    bool check();
    
    bool capture();
    bool videoStart();
    bool videoStop();
    bool liveView(bool p_on = true);
    bool moveFocus(int16_t p_steps);
    
    bool shutter(uint8_t p_idx);
    bool aperture(uint8_t p_idx);
    bool iso(uint8_t p_idx);
    
    uint8_t shutter();
    uint8_t aperture();
    uint8_t iso();
    
    
    
private:
    
    void _openSession();
    void _doTasks();
    void _checkOps();
    bool _processEventCanon();
    bool _processEventNikon();
    
    bool _setEosParameter(uint16_t p_param, uint32_t p_value);
    bool _setPtpParameter(uint16_t p_param, uint32_t p_value);
    bool _setPtpParameter(uint16_t p_param, uint16_t p_value);
    bool _setPtpParameter(uint16_t p_param, uint8_t  p_value);
    bool _getPtpParameterList(uint16_t p_param, uint8_t* p_count, uint16_t* p_list, uint16_t* p_current);
    bool _getPtpParameterList(uint16_t p_param, uint8_t* p_count, uint32_t* p_list, uint32_t* p_current);
    bool _getPtpParameter(uint16_t p_param, uint16_t *value);
    bool _updatePtpParameters();
    bool _getPropertyInfo(uint16_t p_prop_code, uint8_t p_expected_size, uint16_t* p_count, uint8_t* p_current, uint8_t* p_list);
    
    uint8_t _isoEv(uint32_t p_id, bool p_ptp = false);
    uint8_t _apertureEv(uint32_t p_id, bool p_ptp = false);
    uint8_t _shutterEv(uint32_t p_id, bool p_ptp = false);


    struct camProps_t {
        bool capture;
        bool bulb;
        bool iso;
        bool shutter;
        bool aperture;
        bool focus;
        bool video;
        bool ready;
        
        // default constructor
        
        camProps_t() {
            capture  = false;
            bulb     = false;
            iso      = false;
            shutter  = false;
            aperture = false;
            focus    = false;
            video    = false;
            ready    = false;
        }
    };

    
    struct propertyDescription_t {
        char name[8];
        uint32_t eos;
        uint32_t nikon;
        uint8_t ev; // 1/3 stop increments
    };

    struct capabilities_t {
        uint8_t isoCount;
        uint8_t isoAvail[32];
        uint8_t shutterCount;
        uint8_t shutterAvail[64];
        uint8_t apertureCount;
        uint8_t apertureAvail[32];
    };
    
    uint16_t m_ptpErr;
    uint16_t m_offset;
    
    uint32_t m_curShutter;
    uint32_t m_curAperture;
    uint32_t m_curISO;
    
    bool m_ready;
    bool m_busy;
    bool m_lvOn;
    bool m_recStart;
    bool m_ptpUpdate;
    
    PTPVendor      m_vendor;
    camProps_t     m_props;
    capabilities_t m_capabilities;
    
    static const propertyDescription_t m_isoList[];
    static const propertyDescription_t m_apertureList[];
    static const propertyDescription_t m_shutterList[];
    
    
};

    // pre-initialized object
extern PTPCamera Camera;

    // global functions

void EVENT_USB_Host_HostError(const uint8_t p_err);
void EVENT_USB_Host_DeviceAttached(void);
void EVENT_USB_Host_DeviceUnattached(void);
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t p_err, const uint8_t p_suberr);
void EVENT_USB_Host_DeviceEnumerationComplete(void);

PTPStat PTP_Transaction(uint16_t p_op, uint8_t p_recv, uint8_t p_parmCount, uint32_t *p_parms, uint8_t p_dataCount, uint8_t *p_data);
PTPStat PTP_FetchData(uint16_t p_offset);
uint8_t SI_Host_ReceiveResponseCode(USB_ClassInfo_SI_Host_t* const p_interface, PIMA_Container_t *p_PIMABlock);
PTPStat PTP_OpenSession(void);
PTPStat PTP_CloseSession(void);
PTPStat PTP_GetDeviceInfo(void);

void UnicodeToASCII(char *p_string, char *p_buf);

#endif // _PTPCAMERA_H_
