/*===================================================================================*\
* FILE:  f360_object_motion_classification_helpers.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential � Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of supporting functions used in Object_Motion_Classification();
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_object_motion_classification_helpers.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Object_Motion_Status_Should_Be_Updated
   *===========================================================================
   * RETURN VALUE:
   * bool f_should_be_updated - flag indicating whether track motion status should be updated.
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - reference to object
   * const F360_Calibrations_T& calib - reference to tracker calibrations
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
   * Function verifies whether object motion status should be updated.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Object_Motion_Status_Should_Be_Updated(
      const F360_Object_Track_T& object,
      const float32_t k_coasted_track_max_time_from_update
   )
   {
      return ((object.status == F360_OBJECT_STATUS_NEW_UPDATED) ||
         (object.status == F360_OBJECT_STATUS_UPDATED) ||
         ((object.status == F360_OBJECT_STATUS_COASTED) && (object.time_since_track_updated < k_coasted_track_max_time_from_update)));
   }

   /*===========================================================================*\
   * FUNCTION: Update_Object_Properties
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T& object - reference to object
   * const F360_Globals_T globals - reference to global variables
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
   * Function updates following object properties:
   * - movable_prob
   * - f_oncoming
   * - f_vehicular_trk
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Update_Object_Properties(
      F360_Object_Track_T& object,
      const F360_Globals_T& globals)
   {
      if (object.f_moving)
      {
         object.movable_prob = 1.0F;

         // Reset the cca_cross_moving_buffer properties
         // This is not needed for movable objects
         object.cca_cross_moving_buffer_index = 0U;
         for (uint8_t idx = 0U; idx < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; idx++)
         {
            object.cca_cross_moving_buffer[idx] = 0;
         }
      }

      if ((object.vcs_velocity.longitudinal < globals.oncoming_speed_thresh) &&
         (object.f_moving) &&
         (F360_INVALID_UNSIGNED_ID == object.on_sep_id))
      {
         object.f_oncoming = true;
      }
      else
      {
         object.f_oncoming = false;
      }

      if ((!object.f_vehicular_trk) &&
         (object.f_moving) &&
         (globals.obj_vehicular_spd_thresh < object.speed))
      {
         object.f_vehicular_trk = true;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Get_Min_Num_Consec_Moving
   *===========================================================================
   * RETURN VALUE:
   * int32_t min_num_consec_moving - minimal number of consecutive scans to treat
   *                                      object as moving
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object - reference to analysed object
   * const float32_t host_yaw_rate_rad - host yaw rate in radians
   * const F360_Calibrations_T& calib - reference to tracker calibrations
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
   * Function returns minimum number of consecutive scans in which track was marked as moving.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   int32_t Get_Min_Num_Consec_Moving(
      const F360_Object_Track_T& object,
      const float32_t host_yaw_rate_rad,
      const F360_Calibrations_T& calib)
   {
      int32_t min_num_consec_moving;
      if (object.movable_prob > 0.5F)
      {
         min_num_consec_moving = calib.k_object_motion_min_consec_moving_cnt_movable_th;
      }
      else
      {
         if (calib.k_object_motion_min_host_yaw_rate_th < std::abs(host_yaw_rate_rad))
         {
            min_num_consec_moving = calib.k_object_motion_min_consec_moving_cnt_high_yaw_th;
         }
         else
         {
            min_num_consec_moving = calib.k_object_motion_min_consec_moving_cnt_th;
         }
      }

      return min_num_consec_moving;
   }
}
