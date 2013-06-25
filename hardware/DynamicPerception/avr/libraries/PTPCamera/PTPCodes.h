/* PTP Codes for Nikon and Canon Cameras
 
 Original work copyright Elijah Parker / Timelapse+
 
 Modifications by C. A. Church / Dynamic Perception LLC
 */

#ifndef _PTPCODES_H_
#define _PTPCODES_H_

/** @file PTPCodes.h
 
 @brief PTPCodes.h Header File - Core PTP Definitions
 */

const uint8_t             PTP_RETURN_OK = 0;
const uint8_t          PTP_RETURN_ERROR = 1;
const uint8_t PTP_RETURN_DATA_REMAINING = 2;
const uint8_t            PTP_FIRST_TIME = 255;
const uint16_t          PTP_BUFFER_SIZE = 1280;


/* Vendor IDs */
const uint32_t      PTP_VENDOR_EASTMAN_KODAK = 0x00000001;
const uint32_t        PTP_VENDOR_SEIKO_EPSON = 0x00000002;
const uint32_t            PTP_VENDOR_AGILENT = 0x00000003;
const uint32_t           PTP_VENDOR_POLAROID = 0x00000004;
const uint32_t       PTP_VENDOR_AGFA_GEVAERT = 0x00000005;
const uint32_t          PTP_VENDOR_MICROSOFT = 0x00000006;
const uint32_t            PTP_VENDOR_EQUINOX = 0x00000007;
const uint32_t          PTP_VENDOR_VIEWQUEST = 0x00000008;
const uint32_t PTP_VENDOR_STMICROELECTRONICS = 0x00000009;
const uint32_t              PTP_VENDOR_NIKON = 0x0000000A;
const uint32_t              PTP_VENDOR_CANON = 0x0000000B;

const uint8_t     PROTOCOL_EOS = 1;
const uint8_t   PROTOCOL_NIKON = 2;
const uint8_t PROTOCOL_GENERIC = 3;

const uint16_t PIMA_OPERATION_GETDEVICEINFO = 0x1001;
const uint16_t   PIMA_OPERATION_OPENSESSION = 0x1002;
const uint16_t  PIMA_OPERATION_CLOSESESSION = 0x1003;

const uint16_t              EOS_OC_PC_CONNECT = 0x9114;
const uint16_t                 EOS_OC_CAPTURE = 0x910F;
const uint16_t            EOS_OC_PROPERTY_SET = 0x9110;
const uint16_t            EOS_OC_PROPERTY_GET = 0x9127;
const uint16_t EOS_OC_EXTENDED_EVENT_INFO_SET = 0x9115;
const uint16_t               EOS_OC_EVENT_GET = 0x9116;

const uint16_t           EOS_OC_SETUILOCK = 0x911B;
const uint16_t         EOS_OC_RESETUILOCK = 0x911C;
const uint16_t           EOS_OC_BULBSTART = 0x9125;
const uint16_t             EOS_OC_BULBEND = 0x9126;
const uint16_t   EOS_OC_REMOTE_RELEASE_ON = 0x9128;
const uint16_t  EOS_OC_REMOTE_RELEASE_OFF = 0x9129;
const uint16_t        EOS_OC_KeepDeviceOn = 0x9003;
const uint16_t EOS_OC_DepthOfFieldPreview = 0x9156;
const uint16_t           EOS_OC_MoveFocus = 0x9155;

const uint16_t    EOS_OC_LV_START = 0x9133;
const uint16_t     EOS_OC_LV_STOP = 0x9134;
const uint16_t EOS_OC_VIDEO_START = 0x9153;
const uint16_t  EOS_OC_VIDEO_STOP = 0x9154;

const uint16_t EOS_DPC_APERTURE = 0xD101;
const uint16_t  EOS_DPC_SHUTTER = 0xD102;
const uint16_t      EOS_DPC_ISO = 0xD103;
const uint16_t     EOS_DPC_MODE = 0xD105;
const uint16_t EOS_DPC_LiveView = 0xD1B0;
const uint16_t    EOS_DPC_Video = 0xD1B8;
//4=video start
//0=video stop

const uint16_t  EOS_EC_PROPERTY_CHANGE = 0xC189;
const uint16_t  EOS_EC_PROPERTY_VALUES = 0xC18A;
const uint16_t   EOS_EC_OBJECT_CREATED = 0xC181;
const uint16_t EOS_EC_WillShutdownSoon = 0xC18D;


const uint16_t      NIKON_OC_CAPTURE = 0x90C0;
const uint16_t    NIKON_OC_EVENT_GET = 0x90C7;
const uint16_t NIKON_OC_CAMERA_READY = 0x90C8;

const uint16_t       PTP_OC_CAPTURE = 0x100E;
const uint16_t  PTP_OC_PROPERTY_SET = 0x1016;
const uint16_t  PTP_OC_PROPERTY_GET = 0x1015;
const uint16_t PTP_OC_PROPERTY_LIST = 0x1014;
const uint16_t     PTP_OC_GET_THUMB = 0x100A;

const uint16_t   PTP_EC_OBJECT_CREATED = 0x4002;
const uint16_t PTP_EC_PROPERTY_CHANGED = 0x4006;

const uint16_t NIKON_DPC_APERTURE = 0x5007;
const uint16_t  NIKON_DPC_SHUTTER = 0x500D;
const uint16_t      NIKON_DPC_ISO = 0x500F;

const uint16_t NIKON_OC_MoveFocus = 0x9204;

const uint16_t NIKON_OC_BULBSTART = 0x9207;
const uint16_t   NIKON_OC_BULBEND = 0x920C;

const uint16_t          PTP_OFC_Text = 0x3004;
const uint16_t PTP_OC_SendObjectInfo = 0x100C;
const uint16_t     PTP_OC_SendObject = 0x100D;


#endif // _PTPCODES_H_
