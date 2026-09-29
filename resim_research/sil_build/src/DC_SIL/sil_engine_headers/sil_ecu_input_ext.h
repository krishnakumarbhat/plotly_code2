#ifndef ECU_SIL_INPUT_EXTENDED_H
#define ECU_SIL_INPUT_EXTENDED_H
/*===========================================================================*
 * FILE: sil_ecu_input_ext.h
 *===========================================================================
 * Copyright 2019 Aptiv, Inc., All Rights Reserved.
 * APTIV Confidential
 *---------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the header files for AUDI Data Structures for SRR3 Software In Loop(SIL) Simulation.
 *
 * ABBREVIATIONS:
 *   TODO: List of abbreviations used, or reference(s) to external document(s)
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *     TODO: Update list of design document(s)
 *
 *   Requirements Document(s):
 *     TODO: Update list of requirements document(s)
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     SW REF 264.15D "Aptiv C Coding Standards" [12-Mar-2006]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
 *===========================================================================*/
#define SIL_ECU_DEBUG_INPUT_STRUCTURE_VERSION (14)

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <stdint.h>
#include "dvl_message.h"
#include "some_ip_input.h"

/*===========================================================================*
 * Typedef's or Structure Declarations
 *===========================================================================*/
#define MAX_PCAN_MESSAGE_ECUSYMBIF                   500
#define MAX_NUMBER_OF_DETECTIONS_PER_RADAR_ECUSYMBIF (1500)
#define TRACKER_NUMBER_OF_OBJECTS                    (64)
#define EGO_DATA_ARRAY_SIZE                          10
#define MAX_SENSOR_COUNT                             20
#define MAX_FILE_COUNT                               10

#ifdef _WIN32
#define MAX_PATH_ECUSYMBIF 260 /* max. length of full pathname */
#elif __GNUC__
#define MAX_PATH_ECUSYMBIF 4096 /* max. length of full pathname */
#endif

#pragma pack(push, save_pack)
#pragma pack(push, 2)

/* SRR3 SIL Input Symbol Data Structure - Newly Added Interface */
typedef enum DC_Customer_TAG {
   enCUST_INVALID_CUSTOMER = 0x0,
   enCUST_BMW_SP25_L2      = 0xA1,
   enCUST_BMW_SP25_L3      = 0xA2,
   enCUST_STLA_SCALE1      = 0x55,
   enCUST_STLA_SCALE3      = 0x57,
   enCUST_STLA_SCALE4      = 0x56
} DC_Customer_T;
typedef struct SimulationCycleStartTimeInfo {
   int64_t abs_timeStamp_ns;
   int64_t rel_timeStamp_ns;
} SimulationCycleStartTimeInfo_T;

typedef struct Timestamp {
   uint32_t seconds;
   uint32_t fractional_seconds;
} Timestamp_T;

typedef struct EgomotionData {
   Timestamp_T timeStamp;
   float32_T vcs_long_acc;
   uint32_t valueQEgoAccelerationLongitudinalCog;
   float32_T vcs_lat_acc;
   uint32_t valueQEgoAccelerationLateralCog;
   float32_T yawrate;
   uint32_t valueQYawRateVehicleBody;
   float32_T steering_angle;
   float32_T abs_speed;
   uint32_t valueQEgoSpeedCog;
   uint32_t drivingDirectionConfirmed;
} EgomotionData_T;

typedef struct Detections_Input_Tag {
   float32_T elevation;
   float32_T azimuth;    /* [rad] */
   float32_T range_rate; /* [m/s] */
   float32_T range;      /* [m]*/
   float32_T amplitude;  /* [dB] */
   float32_T snr;
   float32_T std_elevation;
   float32_T std_azimuth;
   float32_T std_range_rate;
   float32_T std_range;
   float32_T std_rcs;
   float32_T multi_target_probability;
   float32_T existence_probability;
   unsigned8_T azimuth_confidence;
   unsigned8_T elevation_confidence;
   unsigned8_T valid;
   unsigned8_T host_veh_clutter : 1;
   unsigned8_T super_res_target : 1;
   unsigned8_T nd_target : 1;
   unsigned8_T bistatic : 1;
   unsigned8_T K_Unused8_1 : 4;
   unsigned8_T K_Unused8_2;
   unsigned16_T K_Unused16_1;
} Detections_Input_T;

/* Additional Data needed along with Detections */
typedef struct Detection_Header_Tag {
   float32_T AutoAlignAzimuth;
   float32_T AutoAlignElevation;
   float32_T timestamp;
   Timestamp_T headerTimestamp;
   unsigned32_T Count;
   unsigned32_T ScanIndex;
   unsigned8_T AutoAlignAzimuthQF;
   unsigned8_T AutoAlignElevationQF;
   unsigned8_T timestamp_consistency;
   unsigned8_T LookID;
   unsigned8_T LookType;
   unsigned8_T K_Unused8_1;
   unsigned16_T K_Unused16_1;
} Detection_Header_T;
/* Detections for SIL Data Injection */
typedef struct Tracker_Input_Reports_Logging {
   Detection_Header_T dets_info;
   Detections_Input_T dets_input[MAX_NUMBER_OF_DETECTIONS_PER_RADAR_ECUSYMBIF];
} DetectionObj_Data_Input_T;
typedef struct APT_ECU_Vehicle_Data_Input_Tag {
   float32_T abs_speed;
   float32_T yawrate;
   float32_T steering_angle;
   float32_T rear_axle_steering_angle;
   float32_T rear_axle_position;
   float32_T lane_width_external;
   float32_T lane_center_offset_external;
   float32_T host_vehicle_length;
   float32_T host_vehicle_width;
   float32_T host_vehicle_height;
   float32_T curve_radius;
   float32_T vcs_long_acc;
   float32_T vcs_lat_acc;
   float32_T bb_center_to_rear_x;
   float32_T bb_center_to_rear_y;
   float32_T bb_center_to_rear_z;
   unsigned32_T vehicle_data_buff_timestamp;
   unsigned8_T prndl;
   unsigned8_T turn_signal;
   unsigned8_T f_reverse_gear;
   unsigned8_T f_trailer_present;
   unsigned8_T f_traffic_side;
   EgomotionData_T egomotionData[EGO_DATA_ARRAY_SIZE];
   unsigned8_T K_Unused8_1;
   unsigned16_T K_Unused16_1;
} APT_ECU_Vehicle_Data_Input_T;

typedef enum SIL_INJECTION_MODE_TAG {
   UDP        = 0, /* SIL Input is UDP Frames - SIL / Decoder using API- SIL_SRR3_UDP_Frame_process*/
   DETECTIONS = 1, /* - SIL Input is Decoded data from Flexray Frames - SIL using API- SIL_SRR3_DATA_Frame_process */
   TRACKS     = 2, /* SIL Input is object and vehicle data from virtual environment - SIL using API- SIL_SRR3_DATA_Frame_process */
   TRACKS_UDP = 3
} DPH_SIL_INJECTION_MODE_T;

/* Source to use for the above mounting parameters */
typedef enum MOUNTING_POSITION_SOURCE_TAG {
   MOUNT_UDP = 0,
   MOUNT_XML = 1
} MOUNTING_POSITION_SOURCE_T;

/* Mounting Parameters from XML File is chosen */
typedef struct MOUNTING_VALUES_TAG {
   float32_T azimuth_polarity;
   float32_T boresight_angle;
   float32_T vcs_lat_position;
   float32_T vcs_lon_position;
   float32_T vcs_z_position;
} MOUNTING_VALUES_T;

typedef struct PCAN_DBC_Version_Tag {
   unsigned16_T PCAN_Major_Version;
   unsigned16_T PCAN_Minor_Version;
} PCAN_DBC_Version_T;

typedef struct PCAN_RX_Tag {
   dvlMessageUnion dvl_payload[MAX_PCAN_MESSAGE_ECUSYMBIF];
   signed32_T msg_count;
   unsigned16_T PCAN_Counter;
} PCAN_RX_T;

typedef struct SIL_ECU_Data_Input_Hdr_Tag {
   unsigned32_T ecu_sym_Size;     // size of the SIL_Data_Input structure
   unsigned32_T ecu_sym_Counter;  // counter to be incremented
   unsigned16_T ecu_sym_Checksum; // checksum to be calculated over entire structure leaving header part
   unsigned16_T ecu_sym_version;  // symbol interface version number
} SIL_ECU_Data_Input_Hdr_T;

typedef enum {
   DEBUG_TRACK_STATUS_INVALID = (0), /**< 0*/
   DEBUG_TRACK_STATUS_NEW     = (1), /**< 1*/
   DEBUG_TRACK_STATUS_MATURE  = (2), /**< 2*/
   DEBUG_TRACK_STATUS_COASTED = (3)  /**< 3*/

} DEBUG_TRAC_STATUS_T;

typedef enum {
   DEBUG_OBJECT_CLASS_UNKNOWN    = (0), /**< 0*/
   DEBUG_OBJECT_CLASS_PEDESTRIAN = (1), /**< 1*/
   DEBUG_OBJECT_CLASS_2WHEEL     = (2), /**< 2*/
   DEBUG_OBJECT_CLASS_CAR        = (3), /**< 3*/
   DEBUG_OBJECT_CLASS_TRUCK      = (4)  /**< 4*/

} DEBUG_OBJECT_CLASS_T;

/*Delphi Track Structure for SIL Data Injection of Artificial Created Tracks For FF testing in SIL  */
typedef struct TrackObject_Tag {
   uint8 id;                          // unique id number
   DEBUG_TRAC_STATUS_T status;        // values are INVALID, NEW, MATURE, and COASTED
   unsigned char age;                 // Start from 1 ; number of scans this object has existed
   unsigned int stage_age;            // age
   float vcs_long_posn;               //[m] longitudinal position of object in vcs coordinates
   float vcs_long_vel;                //[m/s] longitudinal velocity of object in vcs coordinates
   float vcs_long_accel;              // [m/s^2] longitudinal acceleration of object in vcs coordinates
   float vcs_lat_posn;                //[m] lateral position of object in vcs coordinates
   float vcs_lat_vel;                 //[m/s] lateral velocity of object in vcs coordinates
   float vcs_lat_accel;               //[m/s^2] lateral acceleration of object in vcs coordinates
   float vcs_long_vel_rel;            //[m/s] relative longitudinal velocity of object in vcs coordinates
   float vcs_lat_vel_rel;             //[m/s] relative lateral velocity of object in vcs coordinates
   float speed;                       //[m/s^2] speed of the object in vcs coordinates
   float tangential_accel;            //[m/s^2] tangential acceleration of the object in vcs coordinates
   float heading;                     //[rad] heading of the object in vcs coordinates
   float heading_rate;                // Yaw rate of the target in the VCS
   float length;                      //[m] length of the target vehicle
   float width;                       //[m] width of the target vehicle
   DEBUG_OBJECT_CLASS_T object_class; // classification of the object based on target size

#ifdef _WIN32
} TrackObject_T;
#endif
#ifdef __GNUC__
}
TrackObject_T;
#endif

typedef struct SIL_ECU_Data_Input_TAG {
   /* header added to protect the symbol data*/
   SIL_ECU_Data_Input_Hdr_T sil_ecu_data_input_hdr;

   /* SIL DATA Injection Mode - Detections , Tracks and EGO vehicle data
   Used for Feature function Testing using Artificially Created Tracks in VTD/ Carmaker
   Used in Series Production SIL where there is NO UDP and Detections and vehicle data are availiable from Flexray / Debug Frames */
   DPH_SIL_INJECTION_MODE_T SIL_Mode;

   /* Sensor Position - REAR_LEFT = 0x01, REAR_RIGHT = 0x02, FRONT_RIGHT = 0x03, FRONT_LEFT = 0x04, */
   uint8 Radar_Position;

   /* Vehicle Data Input */
   APT_ECU_Vehicle_Data_Input_T vehicle_inputs;

   /* Detections REAR_LEFT Sensor */
   DetectionObj_Data_Input_T sym_detection_rl_radar;

   /* Detections REAR_RIGHT Sensor */
   DetectionObj_Data_Input_T sym_detection_rr_radar;

   /* Detections FRONT_LEFT Sensor */
   DetectionObj_Data_Input_T sym_detection_fl_radar;

   /* Detections FRONT_RIGHT Sensor */
   DetectionObj_Data_Input_T sym_detection_fr_radar;

   /* Detections FRONT_CENTER Sensor */
   DetectionObj_Data_Input_T sym_detection_fc_radar;

   /* Detections BP_LEFT Sensor */
   DetectionObj_Data_Input_T sym_detection_bpil_bl_radar;

   /* Detections BP_RIGHT Sensor */
   DetectionObj_Data_Input_T sym_detection_bpil_br_radar;

   /* Detections REAR_CENTER Sensor */
   DetectionObj_Data_Input_T sym_detection_rc_radar;

   /* Detections POS1 Sensor */
   DetectionObj_Data_Input_T sym_detection_pos1_radar;

   /* Detections POS2 Sensor */
   DetectionObj_Data_Input_T sym_detection_pos2_radar;

   /* Detections POS3 Sensor */
   DetectionObj_Data_Input_T sym_detection_pos3_radar;

   /* Mounting Source from XML Cofiguration */
   MOUNTING_POSITION_SOURCE_T Mounting_Source;

   /* Mounting Values from XML Cofiguration for REAR LEFT Radar */
   MOUNTING_VALUES_T Mounting_Values_RL;

   /* Mounting Values from XML Cofiguration for REAR RIGHT Radar */
   MOUNTING_VALUES_T Mounting_Values_RR;

   /* Mounting Values from XML Cofiguration for FRONT LEFT Radar */
   MOUNTING_VALUES_T Mounting_Values_FL;

   /* Mounting Values from XML Cofiguration for FRONT RIGHT Radar */
   MOUNTING_VALUES_T Mounting_Values_FR;

   /* Mounting Values from XML Cofiguration for FRONT CENTER Radar */
   MOUNTING_VALUES_T Mounting_Values_FC;

   /* Mounting Values from XML Cofiguration for BP RIGHT Radar */
   MOUNTING_VALUES_T Mounting_Values_BR;

   /* Mounting Values from XML Cofiguration for BP LEFT Radar */
   MOUNTING_VALUES_T Mounting_Values_BL;

   /* Mounting Values from XML Cofiguration for REAR CENTER Radar */
   MOUNTING_VALUES_T Mounting_Values_RC;

   /* Mounting Values from XML Cofiguration for POS1 Radar */
   MOUNTING_VALUES_T Mounting_Values_POS1;

   /* Mounting Values from XML Cofiguration for POS2 Radar */
   MOUNTING_VALUES_T Mounting_Values_POS2;

   /* Mounting Values from XML Cofiguration for POS3 Radar */
   MOUNTING_VALUES_T Mounting_Values_POS3;

   /* Tracker in REAR_LEFT - REAR_LEFT combined REAR_RIGHT FOV */
   TrackObject_T object_rl[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in REAR_RIGHT - REAR_LEFT combined REAR_RIGHT FOV */
   TrackObject_T object_rr[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in FL FRONT_LEFT FOV */
   TrackObject_T object_fl[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in FR FRONT_RIGHT FOV */
   TrackObject_T object_fr[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in FC FRONT_CENTRE FOV */
   TrackObject_T object_fc[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in BP FRONT_LEFT FOV */
   TrackObject_T object_bl[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in BP FRONT_RIGHT FOV */
   TrackObject_T object_br[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in IPNEXT, Introduced for BMWSP25 and STLA for MRR objects */
   TrackObject_T f360_object_mrr[TRACKER_NUMBER_OF_OBJECTS];

   /* Tracker in IPNEXT, Introduced for BMWSP25 and STLA for SRR objects */
   TrackObject_T f360_object_srr[TRACKER_NUMBER_OF_OBJECTS];

   /* PCAN DBC version for PCAN BB input */
   PCAN_DBC_Version_T dbc_version_in;

   /* PCAN_ Data for Rear left sensor*/
   PCAN_RX_T RL_PCAN_Data;

   /* PCAN_ Data for Rear right sensor*/
   PCAN_RX_T RR_PCAN_Data;

   /* PCAN_ Data for front right sensor*/
   PCAN_RX_T FR_PCAN_Data;

   /* PCAN_ Data for front left sensor*/
   PCAN_RX_T FL_PCAN_Data;

   /* PCAN_ Data for front center sensor*/
   PCAN_RX_T FC_PCAN_Data;

   /* PCAN_ Data for bpillar right sensor*/
   PCAN_RX_T BR_PCAN_Data;

   /* PCAN_ Data for bpillar left sensor*/
   PCAN_RX_T BL_PCAN_Data;

   /* PCAN_ Data for rear center sensor*/
   PCAN_RX_T RC_PCAN_Data;

   /* PCAN_ Data for POS1 sensor*/
   PCAN_RX_T POS1_PCAN_Data;

   /* PCAN_ Data for POS2 sensor*/
   PCAN_RX_T POS2_PCAN_Data;

   /* PCAN_ Data for POS3 sensor*/
   PCAN_RX_T POS3_PCAN_Data;

   // Flag to indicate whether or not to read & update the file paths in emb lib.
   char f_readFilePath; // to be set to 1 for reading file name when there is a change in the file names & set to 0 to indicate not to read.

   // input file path from SRR_DEBUG
   char fPath_Debug[MAX_PATH_ECUSYMBIF];

   // input file path from BN_CALIFR
   char fPath_BN_CALIFR[MAX_PATH_ECUSYMBIF];

   // input file path from BN_YUKETH
   char fPath_BN_IUKETH[MAX_PATH_ECUSYMBIF];

   // input file path from SRR_Ref
   char fPath_SRR_Ref[MAX_PATH_ECUSYMBIF];

   // input of Next file path from SRR_DEBUG
   char fPath_Debug_Next[MAX_PATH_ECUSYMBIF];

   // input of Next file path from BN_CALIFR
   char fPath_BN_CALIFR_Next[MAX_PATH_ECUSYMBIF];

   // input of Next file path from BN_IUKETH
   char fPath_BN_IUKETH_Next[MAX_PATH_ECUSYMBIF];

   // input of Next file path from SRR_Ref
   char fPath_SRR_Ref_Next[MAX_PATH_ECUSYMBIF];

   char *inputFilePaths[MAX_FILE_COUNT];
   char *inputFilePathsNext[MAX_FILE_COUNT];

   // first chunk of ECU UDP PLP TS of that cycle
   unsigned long long ECU_PLP_timeStamp;

   // SomeIP RX Signal Interface dSPACE
   DC_Some_IP_RX_Signal_Symbol_t *p_some_IP_RX_Signal;

   // total sensor count is 20
   SimulationCycleStartTimeInfo_T sim_cycle_start_time_info[MAX_SENSOR_COUNT];

} DC_INPUT_DATA_T;
#pragma pack(pop, save_pack)

#endif
/*===========================================================================*\
* File Revision History (top to bottom: first revision to last revision)
*===========================================================================
*
* Date        Name       (Description on following lines: SCR #, etc.)
* ----------- --------   ---------------------------------------------------
* 06-FEB-2020 SHALINI              V4 SRR5_SIL_ECU_DEBUG_INPUT_STRUCTURE_VERSION
* 05-DEC-2019 AJIT	               V3  SomeIP RX Signal Interface dSPACE

* 14-JAN-2020 KIRAN					V4. Re-structured the symbol by avoiding the pointers inside. CYW-551
* 01-MAY-2020 GHAZAL				V5. added scan id counter in the header strcucture
*\*====================================================================================*/
