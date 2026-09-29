/*===================================================================================*\
 FILE: f360_move_dets_from_killed_to_kept_object.cpp
====================================================================================
* Copyright(C) 2020 Aptiv Advanced Safety and User Experience.All rights reserved.
* Confidential – Restricted Aptiv information.Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   Implementation of a class responsible for changing detections associations,
*   between two objects
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
* None.
*
\*===================================================================================*/

#include "f360_move_dets_from_killed_to_kept_object.h"
#include "f360_associate_detection_to_object.h"
#include "f360_find_detection_inliers.h"
#include "f360_calc_predicted_range_rate.h"

namespace f360_variant_A
{
  /*===========================================================================*\
  * FUNCTION: Move_Dets_From_Killed_To_Kept_Object()
  *===========================================================================
  * RETURN VALUE:
  * None
  *
  * PARAMETERS:
  * const F360_Host_T& host,
  * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
  * const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list
  * const F360_Calibrations_T & calib
  * const F360_Object_Track_T  & object_track_to_kill
  * F360_Object_Track_T & object_track_to_keep
  * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
  *
  * EXTERNAL REFERENCES:
  * None.
  *
  * DEVIATIONS FROM STANDARDS:
  * None.
  *
  * --------------------------------------------------------------------------
  * ABSTRACT: Function Change detections association, dets are move from 
  * object_tracks_to_kill to object_tracks_to_keep by changing object_track_id 
  * in moved detection.
  * Also all object_tracks_to_keep parameters connected to stored information 
  * about associated detections are changed. 
  * --------------------------------------------------------------------------
  *
  *
  * PRECONDITIONS:
  * All the Pointers should Point to valid structures.
  *
  * POSTCONDITIONS:
  * None
  \*===========================================================================*/
   void Move_Dets_From_Killed_To_Kept_Object(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calib,
      const F360_Object_Track_T  & object_track_to_kill,
      F360_Object_Track_T & object_track_to_keep,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      uint32_t detids[MAX_DETS_IN_OBJ_TRK] = {};
      uint32_t leftover_idx = 0U;

      //Try to add only downselected detections first
      for (uint32_t det_slot_idx = 0U; det_slot_idx < object_track_to_kill.ndets; det_slot_idx++)
      {
         const uint32_t det_id = object_track_to_kill.detids[det_slot_idx];
         F360_Detection_Props_T& det_p = detection_props[det_id - 1U];

         if (det_p.f_rr_inlier)
         {
            const bool f_detection_assoaciated_to_object = Associate_Detection_To_Object(tracker_info, object_track_to_keep, det_p, det_id);
            if (f_detection_assoaciated_to_object)
            {
               assert((raw_detection_list.detections[det_id - 1U].raw.sensor_id) > 0);
               const rspp_variant_A::RSPP_Detection_T &det = raw_detection_list.detections[det_id - 1U];
               const uint32_t sensor_idx = static_cast<uint32_t>(raw_detection_list.detections[det_id-1U].raw.sensor_id) - 1U;
               det_p.range_rate_predicted = Calc_Predicted_Range_Rate(det_p, det, object_track_to_keep, sensors[sensor_idx]);
               (object_track_to_keep.num_rr_inlier_dets)++;
            }
            else
            {
               break;
            }
         }
         else
         {
            detids[leftover_idx] = det_id;
            leftover_idx++;
         }
      }

      //Then if it's still place in table add  Other detections
      for (uint32_t det_slot_idx = 0U; det_slot_idx < leftover_idx; det_slot_idx++)
      {
         const uint32_t det_id = detids[det_slot_idx];
         F360_Detection_Props_T& det_p = (detection_props[det_id - 1U]);
         const bool f_detection_assoaciated_to_object = Associate_Detection_To_Object(tracker_info, object_track_to_keep, det_p, det_id);

         if (!f_detection_assoaciated_to_object)
         {
            break;
         }
         else
         {
            const rspp_variant_A::RSPP_Detection_T &det = raw_detection_list.detections[det_id - 1U];
            const uint32_t sensor_idx = static_cast<uint32_t>(raw_detection_list.detections[det_id - 1U].raw.sensor_id) - 1U;
            det_p.range_rate_predicted = Calc_Predicted_Range_Rate(det_p, det, object_track_to_keep, sensors[sensor_idx]);
         }

      }
      // Run detection downselection on merged object to make sure detections are consistent
      Clear_Detection_Inliers_To_Object(object_track_to_keep, detection_props);
      Find_Detection_Inliers_For_Single_Object(host, calib, object_track_to_keep, detection_props);
   }
}
