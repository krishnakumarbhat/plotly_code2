/*===================================================================================*\
* FILE: f360_update_aeb_confidence.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Update_AEB_Confidence() function
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_update_aeb_confidence.h"
#include "f360_update_aeb_confidence_helpers.h"
#include "f360_occlusion.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_AEB_Confidence
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Tracker_Info_T& tracker_info,
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
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
   * Updates AEB confidence level for each active object track based on
   * object state, existence probability, motion status, drivability,
   * heading, confidence level, speed, occlusion status, and ISO relative
   * velocity. Confidence can increase by one level per cycle or drop
   * instantly to the computed instantaneous level. (for more information
   * about specific conditions, see DFD-3469)
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_AEB_Confidence(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      //  Accuracy / Uncertainty  estimation func
      for (int32_t iobj = 0; iobj < tracker_info.num_active_objs; iobj++)
      {
         const int32_t trk_idx = tracker_info.active_obj_ids[iobj] - 1;
         F360_Object_Track_T& object = object_tracks[trk_idx];

         const float32_t iso_relative_x_vel = Compute_Iso_Relative_X_Vel(object, host);
         const float32_t iso_relative_x_vel_threshold = 19.4F;
         const float32_t object_speed_threshold = 3.0F;

         F360_AEB_Confidence_T aeb_confidence_instant{};

         if ((object.time_since_initialization > 2.0F) &&
            (object.exist_prob > 0.99F) &&
            object.f_moving &&
            (object.status == F360_OBJECT_STATUS_UPDATED) &&
            ((object.underdrivable_status_ocg == ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER) ||
             (object.drivable_status_sg == sg::SG_Drivability_Class_T::NONDRIVABLE)) &&
            (std::abs(object.vcs_heading.Value()) < F360_DEG2RAD(15.0F)) &&
            (object.confidenceLevel > 0.97F) &&
            (object.speed > object_speed_threshold) &&
            (object.occlusion_status == F360_Occlusion_Status_T::OCCLUSION_STATUS_VISIBLE) &&
            (iso_relative_x_vel < iso_relative_x_vel_threshold))
            {
               aeb_confidence_instant = AEB_CONF_HIGH;
            }
         else if (
            (object.time_since_initialization > 1.5F) &&
            (object.exist_prob > 0.95F) &&
            object.f_moving &&
            (object.status == F360_OBJECT_STATUS_UPDATED) &&
            ((object.underdrivable_status_ocg == ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER) ||
             (object.drivable_status_sg == sg::SG_Drivability_Class_T::NONDRIVABLE)) &&
            (std::abs(object.vcs_heading.Value()) < F360_DEG2RAD(15.0F)) &&
            (object.confidenceLevel > 0.97F) &&
            (object.speed > object_speed_threshold) &&
            (object.occlusion_status == F360_Occlusion_Status_T::OCCLUSION_STATUS_VISIBLE) &&
            (iso_relative_x_vel < iso_relative_x_vel_threshold)
            )
         {
            aeb_confidence_instant = AEB_CONF_MEDIUM_HIGH;
         }
         else if (
            (object.time_since_initialization > 1.0F) &&
            (object.exist_prob > 0.92F) &&
            object.f_moving &&
            (object.time_since_track_updated < 0.11F) &&
            ((object.underdrivable_status_ocg == ocg::OCG_Underdrivable_Status_T::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER) ||
             (object.drivable_status_sg == sg::SG_Drivability_Class_T::NONDRIVABLE)) &&
            (std::abs(object.vcs_heading.Value()) < F360_DEG2RAD(25.0F)) &&
            (object.confidenceLevel > 0.92F) &&
            (object.speed > object_speed_threshold) &&
            (object.occlusion_status == F360_Occlusion_Status_T::OCCLUSION_STATUS_VISIBLE) &&
            (iso_relative_x_vel < iso_relative_x_vel_threshold)
            )
         {
            aeb_confidence_instant = AEB_CONF_MEDIUM;
         }
         else
         {
            aeb_confidence_instant = AEB_CONF_LOW;
         }

         if (aeb_confidence_instant > object.aeb_confidence)
         {
            object.aeb_confidence = static_cast<F360_AEB_Confidence_T>(static_cast<uint8_t>(object.aeb_confidence) + 1U);
         }
         else if(aeb_confidence_instant < object.aeb_confidence)
         {
            object.aeb_confidence = aeb_confidence_instant;
         }
         else
         {
            //keep previous aeb_confidence
         }
      }
   }
}
