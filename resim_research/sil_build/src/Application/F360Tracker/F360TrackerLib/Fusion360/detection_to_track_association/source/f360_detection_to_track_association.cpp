/*===================================================================================*\
* FILE:  f360_detection_to_track_association.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the different function calls that make up to detection to track
* association module.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_detection_to_track_association.h"
#include "f360_get_wall_time.h"
#include "f360_detection_association_countermeasures.h"
#include "f360_detection_association_evaluation.h"
#include "f360_find_detection_inliers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Detection_To_Track_Association()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Tracker_Info_T & tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list
   * const F360_Calibrations_T & calibrations
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS]
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_TRKR_TIMING_INFO_T & timing_info
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function collects the two main parts of detection to track association.
   * Firstly, an evaluation of possible detection associations is done. Secondly, a set
   * of countermeasures are called where some are detection-based and some track-based. 
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Detection_To_Track_Association(
      const F360_Host_T & host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T & tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();

      Detection_Association_Evaluation(host, sensors, tracker_info, raw_detection_list, calibrations, static_env_polys, detection_props, object_tracks);

      Detection_Association_Countermeasures(tracker_info, raw_detection_list, calibrations, host, sensors, object_tracks, detection_props);

      Find_Detection_Inliers(host, tracker_info, calibrations, object_tracks, detection_props);
      
      timing_info.detection_to_track_association = get_wall_time() - start_time;
   }
}
