/*===========================================================================*\
 * FILE: rr_cal_log.h
 *===========================================================================
 * Copyright 2015 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 *---------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the header files for Common Functions.
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
 *     SW REF 264.15D "Delphi C Coding Standards" [12-Mar-2006]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*===========================================================================*/
#ifndef RR_CAL_LOG_H
#define RR_CAL_LOG_H

/*===========================================================================*\
 * Other Header Files
\*===========================================================================*/
#include "fixmac.h"

/*===========================================================================*\
 * #defines
\*===========================================================================*/
/**No. of Data logging structures*/
#define LOGGING_DATA_BUFF_INDEX (2)

/* Flag to allow Customer Frame Header in UDP
 * 1 - Allow Cust Header in payload
 * 0 - Disable Cust Header in payload
 */

/*===========================================================================*\
 * enums
\*===========================================================================*/
/* Enum identifying the cal_source value for each cal and the max no. of cals to be logged.
 * To be updated everytime a cal is added for logging. Add new cals following the current ones */
typedef enum Cal_Source_Tag {
   USC                       = 0,
   SMC                       = 1,
   RSDS_FEATURE_FUNCTION_CAL = 2,
   RSDS_TRACKER_CAL          = 3,
   DSD_CAL                   = 4,
   TRACKER_CAL_DF_V          = 5,
   CTA_CAL_DF_V              = 6,
   LCDA_CAL_DF_V             = 7,
   FENCES_CAL_DF_V           = 8,
   RECW_CAL_DF_V             = 9,
   ASW_CAL_DF_V              = 10,
   OH_CAL_DF_V               = 11,
   ABA_CAL_DF_V              = 12,
   USC_CAL_DF_V              = 13,
   DYN_ALIGN_CAL_DF_V        = 14,
   STAT_ALIGN_CAL_DF_V       = 15,
   VISOB_CAL_DF_V            = 16,
   FREE_SPACE_CAL_DF_V       = 17,
   IPC_FAILURE_COUNTS        = 18,
   CORE2_TIMING_INFO         = 19,
   TRACKER_ERR               = 20,
   OH_AUDI_ERR               = 21,
   FENCES_ERR                = 22,
   CTA_ERR                   = 23,
   SWA_ERR                   = 24,
   ABA_ERR                   = 25,
   C2_MEM_PROFILING          = 26,
   ARM_DIG                   = 27,
   HUD_CAL_DF_V              = 28,
   ELK_CAL_DF_V              = 29,
   RCCW_Cal_DF_V             = 30,
   FOW_Cal_DF_V              = 31,
   HMC_ABSD_Cal_DF_V         = 32,
   RR_BCARPLUS_Cal_DF_V      = 33,
   RR_ACSF_Cal_DF_V          = 34,
   UDP_LOGGING_VERSION       = 35,
   Dataset_Vehicle_Config    = 36,
   Dataset_VehicleConfig_BSW = 37,
   BSW_Parameter             = 38,
   RCTA_Parameter            = 39,
   PATH_TRACKING_CAL_F_V     = 40,
   MAX_CAL_SOURCE            = 41

} Cal_Source_T;

/* Enum identifying the core to be logged. To be updated everytime a new core is added */
typedef enum Radar_Logging_Data_Source_Tag {
   Z7A_LOGGING_DATA           = 0,
   Z7B_LOGGING_DATA           = 1,
   Z7B_LOGGING_DATA_DUMMY     = 2,
   Z4_LOGGING_DATA            = 3,
   CALIBRATION_DATA           = 4,
   DEBUG_DATA                 = 5,
   CDC_DATA                   = 6,
   SRR3_Z7B_CUST_LOGGING_DATA = 7,
   Z7B_CUST_TRACKER_DATA      = 8,
   Z4_CUST_LOGGING_DATA       = 9,
   DSPACE_CUSTOMER_DATA       = 10,
   RADAR_ECU_CORE_0           = 11,
   RADAR_ECU_CORE_1           = 12,
   RADAR_ECU_CORE_3           = 13,
   RADAR_ECU_OG               = 14,
   RADAR_ECU_CALIB            = 15,
   MAX_LOGGING_SOURCE         = 20 /*NOTE : also change the #define MAX_RADAR_LOGGING_SOURCE when this enum gets changed*/
} Radar_Logging_Data_Source_T;
/* Enum identifying the core to be logged. To be updated everytime a new core is added */
typedef enum e_SIL_Latch_Status_Tag {
   LATCH_SIL_UNINITIALIZED          = -1,
   LATCH_BLOCK_END                  = 0,
   LATCH_IN_PROGRESS                = 1,
   LATCH_UNSUPPORTED_STREAM_VERSION = 2,
   LATCH_UNSUPPORTED_STREAM         = 3,
   LATCH_INVALID_SENSOR_ID          = 4,
   LATCH_INVALID_STREAM_NUMBER      = 5,
   LATCH_INVALID_STREAM_SIZE        = 6
} e_SIL_Latch_Status_T;
typedef enum ECU_Logging_Data_Source_Tag {
   ECU_UNKNOWN_STREAM     = -1,
   ECU_CORE_0_TRK_IAL     = 0, // ECU UDP stream number - 90
   ECU_CORE_1_TRK_OAL     = 1, // ECU UDP stream number - 91
   ECU_CORE_3_FF_OAL      = 2, // ECU UDP stream number - 92
   ECU_OG_INTERNAL        = 3, // ECU UDP stream number - 93
   ECU_CALIB_DATA         = 4, // ECU UDP stream number - 94
   MAX_ECU_LOGGING_SOURCE = 20
} ECU_Logging_Data_Source_T;

typedef enum New_Udp_source_Tag {
   CDC_8_IQ                             = 15,
   Z7A_Z7B_IPC_SRR5_116_CORE            = 20,
   Z7A_Z7B_IPC_SRR5_210_CORE            = 21,
   Z7A_Z7B_IPC_MRR360_116RBIN_64D_CORE  = 23,
   Z7A_Z7B_IPC_MRR360_116RBIN_200D_CORE = 25,
   Z7A_Z7B_IPC_MRR360_116RBIN_150D_CORE = 24,
   Z7A_SRR5P_116RB_64D_CORE             = 27,
   Z7A_SRR5P_116RB_150D_CORE            = 28,
   Z7A_SRR5P_116RB_200D_CORE            = 29,
   Z7B_SIDE_64D_SAT_CORE                = 40,
   Z7B_SIDE_200D_SAT_CORE               = 41,
   Z7B_FRNT_128D_SAT_CORE               = 42,
   Z7B_64SD_64FD_64TRK_STAND_CORE       = 43,
   Z7B_64SD_64TRK_STAND_CORE            = 44,
   Z7B_BPIL_150D_64T_CORE               = 45,
   Z4_Z7B_CORE                          = 50,
   Z4_Z7B_64_FD_CORE                    = 51,
   Z7B_CUST                             = 70,
   Z4_CUST                              = 80,
   ECU_CORE0                            = 90,
   ECU_CORE1                            = 91,
   ECU_CORE3                            = 92,
   ECU_OG                               = 93,
   ECU_CAL_STREAM                       = 94
} New_Udp_source_T;
/*===========================================================================*\
 * Exported Type Declarations
\*===========================================================================*/
/*Structure holding information about the cores to facilitate logging */
typedef struct data_buffer_Tag {
   unsigned8_T *data_Buff_Ptr;
   unsigned32_T Total_size;
   unsigned8_T Struct_version;
   Radar_Logging_Data_Source_T Log_Data_Source;
} logging_data_buffer_T;

#ifdef ENABLE_CUSTHDR

#if !defined(UDP_CUST_FRAME_HEADER_TAG)
#define UDP_CUST_FRAME_HEADER_TAG
/* Frame Header specific to customer - Will be part of the Payload based on Parse_CustHdr setting */
typedef struct UDP_CUST_FRAME_HEADER_Tag {
   u32p0_T packetnumber; /* UDP packet number � increments with each single UDP packet */
   u32p0_T utc_time;     /* Latest UTC time available on Flexray at the middle of the radar dwell */
   u32p0_T timestamp;    /* Time signal based upon an internal clock that is synchronized to the Flexray Signal SC_Masterzeit */
   u8p0_T sensorid;      /* Radar Position */
   u8p0_T sensorstatus;  /* 0: Sensor working, 1: Sensor blind, 2: Error */
   u8p0_T detectioncnt;  /* Number of detections in current scan */
   u8p0_T spare[5];      /* Reserved for future use */
} UDP_CUST_FRAME_HEADER_T;

#endif // UDP_CUST_FRAME_HEADER_TAG

#endif

/*===========================================================================*\
 * Exported Object Declarations
\*===========================================================================*/
extern logging_data_buffer_T logging_data_buffer[LOGGING_DATA_BUFF_INDEX];

/*===========================================================================*\
 * Exported Function Prototypes
\*===========================================================================*/
extern void FlexRay_Protocol_Interrupt_Handler(void);
extern void Update_Logging_Checksum_And_Cal_Data(void);
extern boolean_T Init_Logging_Data_Buffer(void);
extern void Update_Core2_Logging(void);

/*===========================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *===========================================================================
 *
 * Ver  Date       Name   (Description on following lines: SCR #, etc.)
 * ---  --------   -----   ---------------------------------------------
 * 1    03/11/15   RP      kok_css2#25461 : Send SMC and USC calibrations over Flexray
 * 2    2/26/15   	GK		    Moving Flexray driver to Core0 SCR: kok_css2#25700
 * 2    03/26/15   Priya   kok_css2#25698 : Added Radar_Logging_Data_Source_T, logging_data_buffer_T
 * 3    04/09/15   Priya   kok_css2#25864 : Moved Determine_Cal_Error to file UDP_Logging_Data
 * 4    04/14/15   Priya   kok_css2#25957 : Added Debug core to logging
 * 5    05/13/15   Priya   kok_css2#26354 : Added Audi Specific Header and macro to control its inclusion
 * 6    05/22/15   Priya   kok_css2#26354 : Added CDC to enum Radar_Logging_Data_Source_T
\*===========================================================================*/
#endif /* RR_CAL_LOG_H */
