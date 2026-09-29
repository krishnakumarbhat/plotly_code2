/*===================================================================================*\
* FILE:  f360_update_exist_prob.cpp
*====================================================================================
*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of function Update_Existence_Probability().
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#include "f360_update_exist_prob.h"
#include "f360_visibility_info.h"
#include "f360_get_wall_time.h"
#include "f360_update_exist_prob_helpers.h"
#include "f360_timing_info.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Existence_Probability()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info - reference to tracker_info struct
   * const F360_Calibrations_T & calib - reference to calibrations struct
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - reference to array of object tracks
   * F360_TRKR_TIMING_INFO_T & timing_info - reference to timing_info struct
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
   * This function performs measurement update of existence probability.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Existence_Probability(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();
      const float32_t cos_host_sideslip = F360_Cosf(host.vcs_sideslip);
      const float32_t sin_host_sideslip = F360_Sinf(host.vcs_sideslip);

      const bool f_allow_idm_variant = Allow_IDM_variant(tracker_info.variant.type);

      for (int32_t active_obj_idx = 0; active_obj_idx < tracker_info.num_active_objs; active_obj_idx++)
      {
         const int32_t trk_idx = tracker_info.active_obj_ids[active_obj_idx] - 1;

         F360_Object_Track_T& obj = object_tracks[trk_idx];

         obj.p_track_state = Calculate_P_Track_State(obj, calib);

         if (F360_OBJECT_STATUS_UPDATED == obj.status)
         {            
            const float32_t p_measurement = obj.raw_confidence_level;
            Update_Track_EP_With_New_Information(obj.p_track_state * p_measurement, calib.k_ep_clutter_prob_with_meas, calib.k_ep_min_allowed_exist_prob, obj.exist_prob);
         }
         else if (F360_OBJECT_STATUS_COASTED == obj.status)
         {            
            const float32_t p_measurement = calib.k_ep_p_measurement_with_no_new_meas;
            Update_Track_EP_With_No_Information(obj.p_track_state * p_measurement, calib.k_ep_clutter_prob_with_no_meas, calib.k_ep_min_allowed_exist_prob, obj.exist_prob);
         }
         else
         {
            // another track status => do nothing
         }

         Decrease_EP_If_Low_RCS(host, cos_host_sideslip, sin_host_sideslip, calib, obj);

         // Decrease EP for objects with high IDM detections fraction (if in current variant IDM dets are allowed to associate)
         if (f_allow_idm_variant)
         {
             Decrease_EP_Based_On_IDM_Detections(detections, obj);
         } 
      }

      timing_info.update_existence_probability = get_wall_time() - start_time;
   }
}
