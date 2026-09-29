/*===================================================================================*\
* FILE: f360_track_validity.h
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This is the main function for the vehicle processing module.
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/

#ifndef TRACK_VALIDITY_H
#define TRACK_VALIDITY_H

#include "f360_reuse.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_static_env_poly_types.h"
#include "f360_occlusion_types.h"
#include "f360_detection_props.h"
#include "f360_trailer_detector_flt_fus_output.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Track_Validity(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibrations,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Trailer_Estimator_Output_T& trailer_data,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info);
}

#endif
