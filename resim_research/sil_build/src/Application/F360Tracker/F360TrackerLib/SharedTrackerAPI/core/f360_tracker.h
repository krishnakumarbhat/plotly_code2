#ifndef F360_TRACKER_VARIANT_A_H
#define F360_TRACKER_VARIANT_A_H
/*===================================================================================*\
* FILE: f360_tracker.h
*====================================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Tracker class declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#include "../Types/f360_reuse.h"
#include "f360_constants.h"
#include "f360_variant.h"

#include "f360_ocg_interface.h"
#include "f360_sg_interface.h"
#include "rspp_detection_list.h"

#include "../Types/f360_core_info.h"
#include "../Types/f360_host.h"
#include "../Types/f360_host_raw.h"
#include "../Types/f360_host_calib.h"
#include "../Types/f360_radar_sensor.h"

#include "../Logging/f360_log_types.h"
#include "../Logging/TrackerInfoLog.h"
#include "../Logging/HostCalibsLog.h"
#include "../Logging/SyncInfoLog.h"
#include "../Logging/F360HostPropsLog.h"
#include "../Logging/StaticEnvPolysLog.h"
#include "../Logging/TrackerInternal.h"
#include "../Logging/VehicleInfoLog.h"
#include "../Logging/TimingInfoLog.h"
#include "../Logging/f360_log_types.h"
#include "../Logging/TrailerDetectorLog.h"
#include "../Logging/CVTrailerOutputLog.h"
#include "../Logging/TrailerDetectorInternalLog.h"
#include "../Logging/f360_functional_safety_faults_log.h"

namespace f360_variant_A
{
class F360_Tracker {

 public:
   F360_Tracker();
   virtual ~F360_Tracker();

   virtual void Initialize() const;
   virtual void Initialize(const F360_Variant_T &variant) const;
   virtual void Initialize_CV_Trailer(const CV_Trailer_Log_T &cv_log) const;
   virtual void Reset() const;
   virtual void Execute(
       const F360_Core_Info_T &core_info,
       const F360_Host_T &host,
       const ocg::OCG_Outputs_T *const p_occupancy_grid,
       const sg::SG_Output_T *const p_sg_output,
       const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
       const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]) const;

   virtual bool Initialize_Tracker_State_From_Log(
       const F360_Host_T &r_host,
       const F360_Object_Log_Output_T &object_log,
       const Tracker_Info_Log_T &r_tracker_info,
       const F360_Host_Props_Log_T &r_host_props,
       const F360_Static_Env_Poly_Log_T &r_static_env_polys,
       const Trailer_Detector_Log_T &r_trailer_detector,
       const Tracker_Internal_T &r_tracker_internals,
       const F360_Internal_Trailer_Detector_T &r_trailer_detector_internal) const;

   static void Get_Version(uint8_t *const major, uint8_t *const minor, uint8_t *const patch, uint64_t *const build_version);

   void Log_Tracker_Info(Tracker_Info_Log_T *const log) const;
   void Log_F360_Host_Props(F360_Host_Props_Log_T &log) const;
   void Log_Timing_Info(Timing_Info_Log_T *const log) const;
   void Log_Static_Env_Polys(F360_Static_Env_Poly_Log_T *const log) const;
   void Log_CV_Trailer_Output(CV_Trailer_Log_T *const log) const;
   void Log_Trailer_Detector(Trailer_Detector_Log_T *const log) const;
   void Log_Host_Calibs(Host_Calibs_Log_T &log, const F360_Host_Calib_T &host_calib) const;
   void Log_Vehicle_Info(
       Vehicle_Info_Log_T &log,
       const F360_Host_T &host,
       const F360_Host_Raw_T &host_raw,
       const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]) const;
   void Log_Sync_Info(
       Sync_Info_Log_T &log,
       const F360_Host_T &host,
       const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]) const;

   virtual void Log_Objects(F360_Object_Log_Output_T &log) const;
   static void Log_Detections(F360_Detection_Log_Output_T *const log, const rspp_variant_A::RSPP_Detection_List_T &det_list);
   static void Log_Sensor_Calibs(F360_Sensor_Calib_Log_Output_T &log, const F360_Radar_Sensor_Tag (&sensor)[MAX_NUMBER_OF_SENSORS]);

   static void Log_Internal_Clusters(F360_Internal_Cluster_Log_Output_T &log);
   static void Log_Internal_Objects(F360_Internal_Object_Log_Output_T &log);
   static void Log_Internal_Detection_History(F360_Internal_Detection_History_Log_Output_T &log);
   static void Log_Internal_CWD(F360_Internal_CWD_Log_Output_T &log);
   static void Log_Internal_Reflection_Buffer(F360_Internal_Reflection_Buffer_Log_Output_T &log);
   static void Log_Internal_Trailer_Detector(F360_Internal_Trailer_Detector_T &log);

   static void Fill_ROT_Object_Output(
       const rspp_variant_A::RSPP_Detection_List_T &det_list,
       const F360_Host_T &host_info,
       const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
       const Functional_Safety_Faults_Log_T &functional_safety_faults_log,
       ROT_Object_List_Info_T &rot_object_list_info);

#if 1
   void Write_Debug_Info(
       const F360_Host_Calib_T *const host_calib,
       const F360_Host_Raw_T *const host_raw,
       const F360_Host_T *const host,
       const rspp_variant_A::RSPP_Detection_List_T *const raw_detect_list,
       const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
       const ocg::OCG_Outputs_T *occupancy_grid,
       const sg::SG_Output_T *sg_output);

   bool Open_Debug_Info(const char *filename);
   void Close_Debug_Info(void);
#endif
 private:
   static void Initialize_Globals();
   static void Initialize_Calibrations();
   static void Initialize_Det_Prop();
   static void Initialize_Det_Hist();
   static void Initialize_Host_Props();
   static void Initialize_Sensor_Props();
   static void Initialize_Timing_Info();
   static void Initialize_Clusters();
   static void Initialize_Obj_Trck();
   static void Initialize_Trkr_Info();
   static void Initialize_Static_Env_Polys();
   static void Initialize_Variant();
   static void Initialize_Variant(const F360_Variant_T &input_variant);
   static void Initialize_Trailer_Detector();
};
} // namespace f360_variant_A
#endif
