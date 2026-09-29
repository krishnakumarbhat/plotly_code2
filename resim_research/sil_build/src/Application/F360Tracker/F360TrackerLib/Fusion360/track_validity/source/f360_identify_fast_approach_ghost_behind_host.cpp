/*===================================================================================*\
* FILE:  f360_identify_fast_approach_ghost_behind_host.cpp
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of functions declared in f360_identify_fast_approach_ghost_behind_host.cpp
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_identify_fast_approach_ghost_behind_host.h"
#include "f360_occlusion.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Identify_Fast_Approach_Ghost_Behind_Host
   * ===========================================================================
   * RETURN VALUE:
   * void
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Occlusion_Data_T& occlusion_data,
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Object_Track_T& object
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
   * Flag a suspicious multipath ghost behind the host that is fast approaching
   * and all associated inlier detections are occluded per sensor.
   * 
   * The function quickly resets the flag with 1 scan retention once relevant
   * associated detections are found visible.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Identify_Fast_Approach_Ghost_Behind_Host(
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object)
   {
      F360_Occlusion_Status_T occl_status = OCCLUSION_STATUS_UNDEFINED;

      if ((object.vcs_position.x < 0.0F) 
         && (host.speed < object.speed) 
         && (object.speed > 1.0F)
         && (std::abs(object.vcs_heading.Value_Deg()) < 60.0F) 
         && (object.num_rr_inlier_dets > 0U))
      {
         for (uint32_t idx=0U; idx<object.ndets; idx++)
         {
            const uint32_t det_idx = object.detids[idx] - 1U;
            const int32_t sensor_idx = raw_detect_list.detections[det_idx].raw.sensor_id - 1;
            
            // if any "outlier" detections are associated, the range rate unmatch will likely make them wheel spin ones
            // then occlusion check is skipped for them
            if(!det_props[det_idx].f_rr_inlier)
            {
               continue;
            }

            occl_status = Get_Point_Occlusion_Status(sensors, occlusion_data, det_props[det_idx].vcs_position.x, det_props[det_idx].vcs_position.y, object.id, sensor_idx + 1);

            if (occl_status == OCCLUSION_STATUS_VISIBLE)
            {
               break;
            }
         }
      }
      
      if (occl_status == OCCLUSION_STATUS_OCCLUDED)
      {
         object.f_hide_occluded_track_behind_host = true;
         object.cnt_consecutive_visible_from_rear = 0;
      }
      else if(object.f_hide_occluded_track_behind_host && (object.num_rr_inlier_dets > 0U))
      {
         object.cnt_consecutive_visible_from_rear += 1;
      }
      else
      {
         // do nothing
      }

      if (object.cnt_consecutive_visible_from_rear > 1)
      {
         object.f_hide_occluded_track_behind_host = false;
         object.cnt_consecutive_visible_from_rear = 0;
      }
   }
}
