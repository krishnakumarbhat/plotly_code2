/**
 * @file emb_tracker_stream_interface.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Embedded tracker interface to algorithm streams with compile time validation.
 *
 * @section ABBR ABBREVIATIONS:
 *   - @todo List any abbreviations, precede each with a dash ('-').
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - @todo Update list of design document(s).
 *
 *   - Requirements Document(s):
 *     - @todo Update list of requirements document(s)
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - @todo Update list of applicable standards
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - @todo List of deviations from standards in this file, or "None".
 */
/*==========================================================================*/
#ifndef EMB_TRACKER_STREAM_INTERFACE_H
#define EMB_TRACKER_STREAM_INTERFACE_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* For getting array lengths used below */
#include "emb_tracker_project_parameters.h"

/* ========================================================================== */
/*                       Exported Type Declarations                           */
/* ========================================================================== */
/* Structure for tracker status information */
typedef struct Emb_Tracker_Status_Tag
{
   uint32_t scan_index;
} __attribute__((aligned(32))) Emb_Tracker_Status_T;

/* ========================================================================== */
/*                         Stream Type Definitions                            */
/* ========================================================================== */
/* Sync Info Stream */
#include "SyncInfoLog.h"
typedef Sync_Info_Log_T Sync_Info_Stream_T;
#define SYNC_INFO_STREAM_NUMBER  (120U) /* 1U for TPC */
#define SYNC_INFO_STREAM_VERSION (3U)

/* Vehicle Info Stream */
#include "VehicleInfoLog.h"
typedef Vehicle_Info_Log_T Vehicle_Info_Stream_T;
#define VEHICLE_INFO_STREAM_NUMBER  (121U) /* 4U for TPC */
#define VEHICLE_INFO_STREAM_VERSION (10U)

/* Tracker Info Stream */
#include "TrackerInfoLog.h"
typedef Tracker_Info_Log_T Tracker_Info_Stream_T;
#define TRACKER_INFO_STREAM_NUMBER  (122U) /* 7U for TPC */
#define TRACKER_INFO_STREAM_VERSION (10U)

/* Timing Info Stream */
#include "TimingInfoLog.h"
typedef Timing_Info_Log_T Timing_Info_Stream_T;
#define TIMING_INFO_STREAM_NUMBER  (123U) /* 9U for TPC */
#define TIMING_INFO_STREAM_VERSION (22U)

/* Host Calibs Stream */
#include "HostCalibsLog.h"
typedef Host_Calibs_Log_T Host_Calibs_Stream_T;
#define HOST_CALIBS_STREAM_NUMBER  (124U) /* 12U for TPC */
#define HOST_CALIBS_STREAM_VERSION (3U)

/* Host Props Stream */
#include "F360HostPropsLog.h"
typedef F360_Host_Props_Log_T Host_Props_Stream_T;
#define HOST_PROPS_STREAM_NUMBER  (125U) /* 14U for TPC */
#define HOST_PROPS_STREAM_VERSION (2U)

/* Static Env Poly Stream */
#include "StaticEnvPolysLog.h"
typedef F360_Static_Env_Poly_Log_T Static_Env_Poly_Stream_T;
#define STATIC_ENV_POLY_STREAM_NUMBER  (126U) /* 19U for TPC */
#define STATIC_ENV_POLY_STREAM_VERSION (3U)

/* Functional Safety Faults Stream */
#include "f360_functional_safety_faults_log.h"
typedef Functional_Safety_Faults_Log_T Functional_Safety_Faults_Stream_T;
#define FUNCTIONAL_SAFETY_FAULTS_STREAM_NUMBER  (127U) /* 22U for TPC */
#define FUNCTIONAL_SAFETY_FAULTS_STREAM_VERSION (13U)

/* Object List Stream */
#include "f360_log_data.h"
#include "f360_object_log.h"
typedef struct Object_List_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Object_Log_T object[NUMBER_OF_OBJECTS_EMB];
} Object_List_Stream_T;
typedef F360_Object_Log_T Object_List_StreamDef_T; /* For proper streamdef.txt generation */
#define OBJECT_LIST_STREAM_NUMBER  (128U)          /* 70U for TPC */
#define OBJECT_LIST_STREAM_VERSION (12U)

/* Processed Detection Stream */
#include "f360_detection_log.h"
typedef struct Processed_Detection_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Detection_Log_T detection[NUMBER_OF_DETECTIONS_EMB];
} Processed_Detection_Stream_T;
typedef F360_Detection_Log_T Processed_Detection_StreamDef_T; /* For proper streamdef.txt generation */
#define PROCESSED_DETECTION_STREAM_NUMBER  (129U)             /* 71U for TPC */
#define PROCESSED_DETECTION_STREAM_VERSION (4U)

/* Sensor Calib Stream */
#include "f360_sensor_calib_log.h"
typedef struct Sensor_Calib_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Sensor_Calib_Log_T sensor[NUMBER_OF_SENSORS_EMB];
} Sensor_Calib_Stream_T;
typedef F360_Sensor_Calib_Log_T Sensor_Calib_StreamDef_T; /* For proper streamdef.txt generation */
#define SENSOR_CALIB_STREAM_NUMBER  (130U)                /* 72U for TPC */
#define SENSOR_CALIB_STREAM_VERSION (5U)

/* Trailer Detector Stream */
#include "TrailerDetectorLog.h"
typedef Trailer_Detector_Log_T Trailer_Detector_Stream_T;
#define TRAILER_DETECTOR_STREAM_NUMBER  (131U) /* 86U for TPC */
#define TRAILER_DETECTOR_STREAM_VERSION (2U)

/* Internal Trailer Detector Stream */
#include "TrailerDetectorInternalLog.h"
typedef F360_Internal_Trailer_Detector_T Internal_Trailer_Detector_Stream_T;
#define INTERNAL_TRAILER_DETECTOR_STREAM_NUMBER  (132U) /* 87U for TPC */
#define INTERNAL_TRAILER_DETECTOR_STREAM_VERSION (4U)

/* Cv Trailer Stream */
#include "CVTrailerOutputLog.h"
typedef CV_Trailer_Log_T Cv_Trailer_Stream_T;
#define CV_TRAILER_STREAM_NUMBER  (133U) /* 88U for TPC */
#define CV_TRAILER_STREAM_VERSION (4U)

/* Internal Object Stream */
#include "f360_internal_object.h"
typedef struct Internal_Object_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Internal_Object_T object[NUMBER_OF_OBJECTS_EMB];
} Internal_Object_Stream_T;
typedef F360_Internal_Object_T Internal_Object_StreamDef_T; /* For proper streamdef.txt generation */
#define INTERNAL_OBJECT_STREAM_NUMBER  (134U)               /* 150U for TPC */
#define INTERNAL_OBJECT_STREAM_VERSION (37U)

/* Internal Cluster Stream */
#include "f360_internal_cluster.h"
typedef struct Internal_Cluster_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Internal_Cluster_T cluster[NUMBER_OF_CLUSTERS_EMB];
} Internal_Cluster_Stream_T;
typedef F360_Internal_Cluster_T Internal_Cluster_StreamDef_T; /* For proper streamdef.txt generation */
#define INTERNAL_CLUSTER_STREAM_NUMBER  (135U)                /* 151U for TPC */
#define INTERNAL_CLUSTER_STREAM_VERSION (5U)

/* Internal Detection Hist Stream */
#include "f360_internal_detection_history.h"
typedef struct Internal_Detection_Hist_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Internal_Detection_Hist_T det_hist[NUMBER_OF_HIST_DETS_EMB];
} Internal_Detection_Hist_Stream_T;
typedef F360_Internal_Detection_Hist_T Internal_Detection_Hist_StreamDef_T; /* For proper streamdef.txt generation */
#define INTERNAL_DETECTION_HIST_STREAM_NUMBER  (136U)                       /* 152U for TPC */
#define INTERNAL_DETECTION_HIST_STREAM_VERSION (4U)

/* Internal Reflection buffer Stream */
#include "f360_internal_reflection_buffer.h"
typedef struct Internal_Reflection_Buffer_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Internal_Reflection_Buffer_T reflection_buffer[NUMBER_OF_SENSORS_EMB];
} Internal_Reflection_Buffer_Stream_T;
typedef F360_Internal_Reflection_Buffer_T Internal_Reflection_Buffer_StreamDef_T; /* For proper streamdef.txt generation */
#define INTERNAL_REFLECTION_BUFFER_STREAM_NUMBER  (137U)                          /* 154U for TPC */
#define INTERNAL_REFLECTION_BUFFER_STREAM_VERSION (1U)

/* Internal CWD Stream */
#include "f360_internal_cwd.h"
typedef struct Internal_CWD_Stream_Tag
{
   F360_Log_Header_T f360header;
   F360_Internal_CWD_T cwd[NUMBER_OF_SENSORS_EMB];
} Internal_CWD_Stream_T;
typedef F360_Internal_CWD_T Internal_CWD_StreamDef_T; /* For proper streamdef.txt generation */
#define INTERNAL_CWD_STREAM_NUMBER  (138U)            /* 155U for TPC */
#define INTERNAL_CWD_STREAM_VERSION (3U)

/* OCG Output Stream */
#if !defined(DISABLE_OCG)
   #include "ocg_output_log_type.h"
typedef OCG_Output_Log_T OCG_Output_Stream_T;
   #define OCG_OUTPUT_STREAM_NUMBER  (139U) /* 89U for TPC */
   #define OCG_OUTPUT_STREAM_VERSION (1U)
#endif

/* ========================================================================== */
/*                      Compile Time Validation                               */
/* ========================================================================== */
/* C++ static asserts to verify matching structure sizes, alignment, stream numbers, and stream versions */
#ifdef __cplusplus
   #include "f360_log_types.h"
using namespace f360_variant_A;

/* Sync_Info_Stream_T checks */
static_assert(sizeof(Sync_Info_Stream_T) == sizeof(Sync_Info_Log_T), "Sync_Info_Stream_T size mismatch vs Sync_Info_Log_T!");
static_assert(alignof(Sync_Info_Stream_T) == alignof(Sync_Info_Log_T),
              "Sync_Info_Stream_T alignment mismatch vs Sync_Info_Log_T!");
// static_assert(SYNC_INFO_STREAM_NUMBER == SYNC_INFO_LOG_STREAM_NUM, "Sync info stream number mismatch!");
static_assert(SYNC_INFO_STREAM_VERSION == SYNC_INFO_LOG_STREAM_VERSION, "Sync info stream version mismatch!");

/* Vehicle_Info_Stream_T checks */
static_assert(sizeof(Vehicle_Info_Stream_T) == sizeof(Vehicle_Info_Log_T),
              "Vehicle_Info_Stream_T size mismatch vs Vehicle_Info_Log_T!");
static_assert(alignof(Vehicle_Info_Stream_T) == alignof(Vehicle_Info_Log_T),
              "Vehicle_Info_Stream_T alignment mismatch vs Vehicle_Info_Log_T!");
// static_assert(VEHICLE_INFO_STREAM_NUMBER == VEHICLE_INFO_LOG_STREAM_NUM, "Vehicle info stream number mismatch!");
static_assert(VEHICLE_INFO_STREAM_VERSION == VEHICLE_INFO_LOG_STREAM_VERSION, "Vehicle info stream version mismatch!");

/* Tracker_Info_Stream_T checks */
static_assert(sizeof(Tracker_Info_Stream_T) == sizeof(Tracker_Info_Log_T),
              "Tracker_Info_Stream_T size mismatch vs Tracker_Info_Log_T!");
static_assert(alignof(Tracker_Info_Stream_T) == alignof(Tracker_Info_Log_T),
              "Tracker_Info_Stream_T alignment mismatch vs Tracker_Info_Log_T!");
// static_assert(TRACKER_INFO_STREAM_NUMBER == TRACKER_INFO_LOG_STREAM_NUM, "Tracker info stream number mismatch!");
static_assert(TRACKER_INFO_STREAM_VERSION == TRACKER_INFO_LOG_STREAM_VERSION, "Tracker info stream version mismatch!");

/* Timing_Info_Stream_T checks */
static_assert(sizeof(Timing_Info_Stream_T) == sizeof(Timing_Info_Log_T),
              "Timing_Info_Stream_T size mismatch vs Timing_Info_Log_T!");
static_assert(alignof(Timing_Info_Stream_T) == alignof(Timing_Info_Log_T),
              "Timing_Info_Stream_T alignment mismatch vs Timing_Info_Log_T!");
// static_assert(TIMING_INFO_STREAM_NUMBER == TIMING_INFO_LOG_STREAM_NUM, "Timing info stream number mismatch!");
static_assert(TIMING_INFO_STREAM_VERSION == TIMING_INFO_LOG_STREAM_VERSION, "Timing info stream version mismatch!");

/* Host_Calibs_Stream_T checks */
static_assert(sizeof(Host_Calibs_Stream_T) == sizeof(Host_Calibs_Log_T),
              "Host_Calibs_Stream_T size mismatch vs Host_Calibs_Log_T!");
static_assert(alignof(Host_Calibs_Stream_T) == alignof(Host_Calibs_Log_T),
              "Host_Calibs_Stream_T alignment mismatch vs Host_Calibs_Log_T!");
// static_assert(HOST_CALIBS_STREAM_NUMBER == HOST_CALIBS_LOG_STREAM_NUM, "Host calibs stream number mismatch!");
static_assert(HOST_CALIBS_STREAM_VERSION == HOST_CALIBS_LOG_STREAM_VERSION, "Host calibs stream version mismatch!");

/* Host_Props_Stream_T checks */
static_assert(sizeof(Host_Props_Stream_T) == sizeof(F360_Host_Props_Log_T),
              "Host_Props_Stream_T size mismatch vs F360_Host_Props_Log_T!");
static_assert(alignof(Host_Props_Stream_T) == alignof(F360_Host_Props_Log_T),
              "Host_Props_Stream_T alignment mismatch vs F360_Host_Props_Log_T!");
// static_assert(HOST_PROPS_STREAM_NUMBER == F360_HOST_PROPS_LOG_STREAM_NUM, "Host props stream number mismatch!");
static_assert(HOST_PROPS_STREAM_VERSION == F360_HOST_PROPS_LOG_STREAM_VERSION, "Host props stream version mismatch!");

/* Static_Env_Poly_Stream_T checks */
static_assert(sizeof(Static_Env_Poly_Stream_T) == sizeof(F360_Static_Env_Poly_Log_T),
              "Static_Env_Poly_Stream_T size mismatch vs F360_Static_Env_Poly_Log_T!");
static_assert(alignof(Static_Env_Poly_Stream_T) == alignof(F360_Static_Env_Poly_Log_T),
              "Static_Env_Poly_Stream_T alignment mismatch vs F360_Static_Env_Poly_Log_T!");
// static_assert(STATIC_ENV_POLY_STREAM_NUMBER == STATIC_ENV_POLYS_LOG_STREAM_NUM, "Static env poly stream number mismatch!");
static_assert(STATIC_ENV_POLY_STREAM_VERSION == STATIC_ENV_POLYS_LOG_STREAM_VERSION, "Static env poly stream version mismatch!");

/* Functional_Safety_Faults_Stream_T checks */
static_assert(sizeof(Functional_Safety_Faults_Stream_T) == sizeof(Functional_Safety_Faults_Log_T),
              "Functional_Safety_Faults_Stream_T size mismatch vs Functional_Safety_Faults_Log_T!");
static_assert(alignof(Functional_Safety_Faults_Stream_T) == alignof(Functional_Safety_Faults_Log_T),
              "Functional_Safety_Faults_Stream_T alignment mismatch vs Functional_Safety_Faults_Log_T!");
// static_assert(FUNCTIONAL_SAFETY_FAULTS_STREAM_NUMBER == F360_FUNCTIONAL_SAFETY_FAULTS_LOG_STREAM_NUM,
//               "Functional safety faults stream number mismatch!");
static_assert(FUNCTIONAL_SAFETY_FAULTS_STREAM_VERSION == F360_FUNCTIONAL_SAFETY_FAULTS_LOG_STREAM_VERSION,
              "Functional safety faults stream version mismatch!");

/* Object_List_Stream_T checks */
static_assert(sizeof(Object_List_Stream_T) == sizeof(F360_Object_Log_Output_T),
              "Object_List_Stream_T size mismatch vs F360_Object_Log_Output_T!");
static_assert(alignof(Object_List_Stream_T) == alignof(F360_Object_Log_Output_T),
              "Object_List_Stream_T alignment mismatch vs F360_Object_Log_Output_T!");
// static_assert(OBJECT_LIST_STREAM_NUMBER == F360_Objects_Log_Stream_Num, "Object list stream number mismatch!");
static_assert(OBJECT_LIST_STREAM_VERSION == F360_Objects_Log_Stream_Ver, "Object list stream version mismatch!");

/* Processed_Detection_Stream_T checks */
static_assert(sizeof(Processed_Detection_Stream_T) == sizeof(F360_Detection_Log_Output_T),
              "Processed_Detection_Stream_T size mismatch vs F360_Detection_Log_Output_T!");
static_assert(alignof(Processed_Detection_Stream_T) == alignof(F360_Detection_Log_Output_T),
              "Processed_Detection_Stream_T alignment mismatch vs F360_Detection_Log_Output_T!");
// static_assert(PROCESSED_DETECTION_STREAM_NUMBER == F360_Detection_Log_Stream_Num, "Processed detection stream number
// mismatch!");
static_assert(PROCESSED_DETECTION_STREAM_VERSION == F360_Detection_Log_Stream_Ver, "Processed detection stream version mismatch!");

/* Sensor_Calib_Stream_T checks */
static_assert(sizeof(Sensor_Calib_Stream_T) == sizeof(F360_Sensor_Calib_Log_Output_T),
              "Sensor_Calib_Stream_T size mismatch vs F360_Sensor_Calib_Log_Output_T!");
static_assert(alignof(Sensor_Calib_Stream_T) == alignof(F360_Sensor_Calib_Log_Output_T),
              "Sensor_Calib_Stream_T alignment mismatch vs F360_Sensor_Calib_Log_Output_T!");
// static_assert(SENSOR_CALIB_STREAM_NUMBER == F360_Sensor_Calib_Log_Stream_Num, "Sensor calib stream number mismatch!");
static_assert(SENSOR_CALIB_STREAM_VERSION == F360_Sensor_Calib_Log_Stream_Ver, "Sensor calib stream version mismatch!");

/* Trailer_Detector_Stream_T checks */
static_assert(sizeof(Trailer_Detector_Stream_T) == sizeof(Trailer_Detector_Log_T),
              "Trailer_Detector_Stream_T size mismatch vs Trailer_Detector_Log_T!");
static_assert(alignof(Trailer_Detector_Stream_T) == alignof(Trailer_Detector_Log_T),
              "Trailer_Detector_Stream_T alignment mismatch vs Trailer_Detector_Log_T!");
// static_assert(TRAILER_DETECTOR_STREAM_NUMBER == TRAILER_DETECTOR_LOG_STREAM_NUM,
//               "Trailer detector stream number mismatch!");
static_assert(TRAILER_DETECTOR_STREAM_VERSION == TRAILER_DETECTOR_LOG_STREAM_VERSION, "Trailer detector stream version mismatch!");

/* Internal_Trailer_Detector_Stream_T checks */
static_assert(sizeof(Internal_Trailer_Detector_Stream_T) == sizeof(F360_Internal_Trailer_Detector_T),
              "Internal_Trailer_Detector_Stream_T size mismatch vs F360_Internal_Trailer_Detector_T!");
static_assert(alignof(Internal_Trailer_Detector_Stream_T) == alignof(F360_Internal_Trailer_Detector_T),
              "Internal_Trailer_Detector_Stream_T alignment mismatch vs F360_Internal_Trailer_Detector_T!");
// static_assert(INTERNAL_TRAILER_DETECTOR_STREAM_NUMBER == TRAILER_DETECTOR_INTERNAL_LOG_STREAM_NUM,
//               "Internal trailer detector stream number mismatch!");
static_assert(INTERNAL_TRAILER_DETECTOR_STREAM_VERSION == TRAILER_DETECTOR_INTERNAL_LOG_STREAM_VERSION,
              "Internal trailer detector stream version mismatch!");

/* Cv_Trailer_Stream_T checks */
static_assert(sizeof(Cv_Trailer_Stream_T) == sizeof(CV_Trailer_Log_T), "Cv_Trailer_Stream_T size mismatch vs CV_Trailer_Log_T!");
static_assert(alignof(Cv_Trailer_Stream_T) == alignof(CV_Trailer_Log_T),
              "Cv_Trailer_Stream_T alignment mismatch vs CV_Trailer_Log_T!");
// static_assert(CV_TRAILER_STREAM_NUMBER == CV_TRAILER_OUTPUT_LOG_STREAM_NUM, "CV trailer stream number mismatch!");
static_assert(CV_TRAILER_STREAM_VERSION == CV_TRAILER_OUTPUT_STREAM_VERSION, "CV trailer stream version mismatch!");

/* Internal_Object_Stream_T checks */
static_assert(sizeof(Internal_Object_Stream_T) == sizeof(F360_Internal_Object_Log_Output_T),
              "Internal_Object_Stream_T size mismatch vs F360_Internal_Object_Log_Output_T!");
static_assert(alignof(Internal_Object_Stream_T) == alignof(F360_Internal_Object_Log_Output_T),
              "Internal_Object_Stream_T alignment mismatch vs F360_Internal_Object_Log_Output_T!");
// static_assert(INTERNAL_OBJECT_STREAM_NUMBER == F360_Internal_Object_Stream_Num,
//               "Internal object stream number mismatch!");
static_assert(INTERNAL_OBJECT_STREAM_VERSION == F360_Internal_Object_Stream_Ver, "Internal object stream version mismatch!");

/* Internal_Cluster_Stream_T checks */
static_assert(sizeof(Internal_Cluster_Stream_T) == sizeof(F360_Internal_Cluster_Log_Output_T),
              "Internal_Cluster_Stream_T size mismatch vs F360_Internal_Cluster_Log_Output_T!");
static_assert(alignof(Internal_Cluster_Stream_T) == alignof(F360_Internal_Cluster_Log_Output_T),
              "Internal_Cluster_Stream_T alignment mismatch vs F360_Internal_Cluster_Log_Output_T!");
// static_assert(INTERNAL_CLUSTER_STREAM_NUMBER == F360_Internal_Cluster_Stream_Num,
//               "Internal cluster stream number mismatch!");
static_assert(INTERNAL_CLUSTER_STREAM_VERSION == F360_Internal_Cluster_Stream_Ver, "Internal cluster stream version mismatch!");

/* Internal_Detection_Hist_Stream_T checks */
static_assert(sizeof(Internal_Detection_Hist_Stream_T) == sizeof(F360_Internal_Detection_History_Log_Output_T),
              "Internal_Detection_Hist_Stream_T size mismatch vs F360_Internal_Detection_History_Log_Output_T!");
static_assert(alignof(Internal_Detection_Hist_Stream_T) == alignof(F360_Internal_Detection_History_Log_Output_T),
              "Internal_Detection_Hist_Stream_T alignment mismatch vs F360_Internal_Detection_History_Log_Output_T!");
// static_assert(INTERNAL_DETECTION_HIST_STREAM_NUMBER == F360_Internal_Detection_History_Stream_Num,
//               "Internal detection hist stream number mismatch!");
static_assert(INTERNAL_DETECTION_HIST_STREAM_VERSION == F360_Internal_Detection_History_Stream_Ver,
              "Internal detection hist stream version mismatch!");

/* Internal_Reflection_Buffer_Stream_T checks */
static_assert(sizeof(Internal_Reflection_Buffer_Stream_T) == sizeof(F360_Internal_Reflection_Buffer_Log_Output_T),
              "Internal_Reflection_Buffer_Stream_T size mismatch vs F360_Internal_Reflection_Buffer_Log_Output_T!");
static_assert(alignof(Internal_Reflection_Buffer_Stream_T) == alignof(F360_Internal_Reflection_Buffer_Log_Output_T),
              "Internal_Reflection_Buffer_Stream_T alignment mismatch vs F360_Internal_Reflection_Buffer_Log_Output_T!");
// static_assert(INTERNAL_REFLECTION_BUFFER_STREAM_NUMBER == F360_Internal_Reflection_Buffer_Stream_Num,
//               "Internal reflection buffer stream number mismatch!");
static_assert(INTERNAL_REFLECTION_BUFFER_STREAM_VERSION == F360_Internal_Reflection_Buffer_Stream_Ver,
              "Internal reflection buffer stream version mismatch!");

/* Internal_CWD_Stream_T checks */
static_assert(sizeof(Internal_CWD_Stream_T) == sizeof(F360_Internal_CWD_Log_Output_T),
              "Internal_CWD_Stream_T size mismatch vs F360_Internal_CWD_Log_Output_T!");
static_assert(alignof(Internal_CWD_Stream_T) == alignof(F360_Internal_CWD_Log_Output_T),
              "Internal_CWD_Stream_T alignment mismatch vs F360_Internal_CWD_Log_Output_T!");
// static_assert(INTERNAL_CWD_STREAM_NUMBER == F360_Internal_CWD_Stream_Num,
//               "Internal CWD stream number mismatch!");
static_assert(INTERNAL_CWD_STREAM_VERSION == F360_Internal_CWD_Stream_Ver, "Internal CWD stream version mismatch!");

   /* OCG_Output_Stream_T checks */
   #if !defined(DISABLE_OCG)
static_assert(sizeof(OCG_Output_Stream_T) == sizeof(OCG_Output_Log_T), "OCG_Output_Stream_T size mismatch vs OCG_Output_Log_T!");
static_assert(alignof(OCG_Output_Stream_T) == alignof(OCG_Output_Log_T),
              "OCG_Output_Stream_T alignment mismatch vs OCG_Output_Log_T!");
// static_assert(OCG_OUTPUT_STREAM_NUMBER == OCG_OUTPUT_LOG_STREAM_NUM, "OCG output stream number mismatch!");
static_assert(OCG_OUTPUT_STREAM_VERSION == OCG_OUTPUT_LOG_STREAM_VERSION, "OCG output stream version mismatch!");
   #endif
#endif

#endif /* EMB_TRACKER_STREAM_INTERFACE_H */
