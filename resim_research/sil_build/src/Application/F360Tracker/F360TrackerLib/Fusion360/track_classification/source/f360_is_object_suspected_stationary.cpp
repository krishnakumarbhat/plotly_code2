/*===================================================================================*\
* FILE:  f360_check_if_object_is_suspected_stationary.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of Is_Object_Suspected_Stationary() function.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_is_object_suspected_stationary.h"
#include "f360_math_func.h"
#include "f360_bounding_box.h"
#include "f360_determine_cross_moving_obj.h"
#include "f360_is_object_suspected_stationary_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Object_Suspected_Stationary
   *===========================================================================
   * RETURN VALUE:
   * bool f_stationary_suspected - flag indicating whether object is suspected of being stationary.
   *
   * PARAMETERS:
   *  F360_Object_Track_T& object,
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
   *  const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   *  const F360_Host_T& host,
   *  const F360_Calibrations_T& calib,
   *  const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   *  const F360_Occlusion_Data_T& occlusion_data
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
   * Function checks if object is suspected of being stationary.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Object_Suspected_Stationary(
      F360_Object_Track_T& object,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS])
   {
      bool f_stationary_suspected;
      const float32_t k_max_vel_for_considering_stat_hypothesis = 15.0F; // [m/s]

      if (std::abs(object.speed) > k_max_vel_for_considering_stat_hypothesis)  // Never mark the object as stationary if its speed is larger than the threshold
      {
         f_stationary_suspected = false;
      }
      else
      {
         const float32_t moving_speed_threshold = Calc_Moving_Speed_Threshold(raw_detect_list, sensors, calib, object, occlusion_data);

         if (std::abs(object.speed) <= moving_speed_threshold)
         {
            f_stationary_suspected = true;
         }

         else if ((object.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA) || (object.trk_fltr_type == F360_TRACKER_TRKFLTR_CCA))
         {
            const bool f_moving_by_dets =!Is_Object_Stationary_By_Num_Dets(object, det_props, calib.k_object_motion_min_moving_dets_percentage_th);

            /* CTCA has ONE state representing object speed(object.speed) so Is_Object_Stationary_By_Vel_Sigma() should be used
            *  CCA has TWO states representing object speed (object.vcs_velocity.longitudinal and object.vce_velocity.lateral) so Is_Object_Stationary_By_Vel_NEES() should be used 
            */
            const bool f_moving_by_vel = (object.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA) ?  !Is_Object_Stationary_By_Vel_Sigma(object, calib) : !Is_Object_Stationary_By_Vel_NEES(object, calib);

            const bool f_parallel_moving = Is_Object_Parallel_Moving(object, host, calib);

            const bool f_probably_cross_moving = ((object.movable_prob > 0.5F) && Is_Object_Cross_Moving(object, calib));

            const bool f_cross_radial_moving = ((object.movable_prob > 0.5F) && Is_Object_Cross_Radial_Moving(object));

            const bool f_moving_suspected =
               f_moving_by_dets ||
               f_moving_by_vel ||
               f_parallel_moving ||
               f_probably_cross_moving ||
               f_cross_radial_moving;

            f_stationary_suspected = !f_moving_suspected;
         }
         else
         {
            f_stationary_suspected = true;
         }
      }

      return f_stationary_suspected;
   }
}
