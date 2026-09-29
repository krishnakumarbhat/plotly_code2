/*===================================================================================*\
* FILE: f360_track_validity.cpp
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


/******************************
* Includes
*******************************/

#include "f360_track_validity.h"
#include "f360_update_exist_prob.h"
#include "f360_update_object_confidence_levels.h"
#include "f360_get_wall_time.h"
#include "f360_determine_reflected_obj.h"
#include "f360_overall_confidence.h"
#include "f360_detect_multipath.h"
#include "f360_identify_fast_approach_ghost_behind_host.h"
#include "f360_update_aeb_confidence.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Track_Validity
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Tracker_Info_T& tracker_info,
   * const F360_Calibrations_T& calibrations,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
   * const F360_Occlusion_Data_T& occlusion_data,
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_TRKR_TIMING_INFO_T& timing_info)
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
   * Main function of Track Validity module
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
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
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();
      Multipath_Detector mp_detector(static_env_polys, object_tracks, tracker_info, calibrations);

      //  Accuracy / Uncertainty  estimation func
      for (int32_t iobj = 0; iobj < tracker_info.num_active_objs; iobj++)
      {
         const int32_t trk_idx = tracker_info.active_obj_ids[iobj] - 1;

         F360_Object_Track_T& object = object_tracks[trk_idx];

         const bool f_marked_as_multipath = Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[trk_idx], mp_detector);

         if (F360_OBJECT_STATUS_INVALID != object.status)
         {
            Update_Object_Confidence_Levels(tracker_info, calibrations, object);

            // A countermeasure to capture a likely fast approach ghost from behind due to multi-path.
            Identify_Fast_Approach_Ghost_Behind_Host(host, raw_detect_list, occlusion_data, sensors, det_props, object);

         }
         Determine_Reflected_Obj(tracker_info, trk_idx, host, static_env_polys, calibrations, sensors, trailer_data, f_marked_as_multipath, object_tracks);
      }

      Update_Existence_Probability(tracker_info, calibrations, host, raw_detect_list.detections, object_tracks, timing_info);

      Overall_Confidence(object_tracks, raw_detect_list, tracker_info, calibrations);

      Update_AEB_Confidence(host, tracker_info, object_tracks);

      timing_info.track_validity = get_wall_time() - start_time;
   }
}
