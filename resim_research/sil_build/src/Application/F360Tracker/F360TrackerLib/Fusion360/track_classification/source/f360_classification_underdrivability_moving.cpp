/*===================================================================================*\
 * FILE:  f360_classification_underdrivability_moving.cpp
 *====================================================================================
 * Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 * This file contains the implementation of Determine_Underdrivability_For_Movable()
 * 
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 **/

#include "f360_classification_underdrivability_moving.h"
#include "f360_get_wall_time.h"
#include <numeric>

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Assign_Underdrivability_Status_To_Moving_Object()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib,
   * const F360_Host_T &host,
   * F360_Object_Track_T & object,
   * F360_TRKR_TIMING_INFO_T& timing_info
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
   * Main function assigning underdrivability status to moving objects.
   * Historical height mean and number of scan indexes above the height threshold is considered.
   *
   * PRECONDITIONS:
   * None
   * 
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/

   void Assign_Underdrivability_Status_To_Moving_Object(
      const F360_Calibrations_T & calib,
      const F360_Host_T &host,
      F360_Object_Track_T & object,
      F360_TRKR_TIMING_INFO_T& timing_info)

   {
      const float32_t start_time = get_wall_time();
      const bool f_object_in_front = object.vcs_position.x > -5.0F;  // add some buffer for object coasting from front area
      float32_t underdrivable_height_thres;
      if (f_object_in_front)
      {
         const float32_t max_measureable_elevation_angle_FoV_deg = 15.0F;
         const float32_t lower_bound_height_threshold = 4.0F;
         const float32_t distance_object_to_host = F360_Get_Hypotenuse(object.vcs_position.x, object.vcs_position.y);      
         const float32_t max_possible_height_of_object = distance_object_to_host * F360_DEG2RAD(max_measureable_elevation_angle_FoV_deg);  // max possible height at higher border of FoV, with small angle approximation with x = sin(x)
         underdrivable_height_thres = F360_Saturate(max_possible_height_of_object, lower_bound_height_threshold, calib.ud_mov_height_threshold);  // the underdrivable object should be higher than this max height.
      }
      else
      {
         underdrivable_height_thres = calib.ud_mov_height_threshold;
      }

      // Currently the logic for underdrivable and overdrivable logic is the same and are using otg_height signal which is unsigned value.
      // Check if the object is overdrivable (below the host) and change the height threshold accordingly.
      // ud_mov_historic_ndets is saturated in f360_calc_obj_height.cpp, if the threshold is changed make sure to check intended use case in f360_classification_underdrivability_moving.cpp
      if ((object.ud_overdrivable_det_pct > 0.7F) &&
         (object.ud_mov_historic_ndets > 20.0F) &&
         (std::abs(object.vcs_position.x) < 40.0F) &&
         (std::abs(object.vcs_position.y) < 50.0F))
      {
         underdrivable_height_thres = std::min(2.0F, underdrivable_height_thres);
      }

      if (object.otg_height > underdrivable_height_thres)
      {
         const bool f_object_just_start_moving = (-0.01F < object.time_since_started_move) && (object.time_since_started_move < 0.1F);   // object just start moving from stationary state
         const bool f_object_alive_long_time = object.time_since_initialization > 0.55F;   // object has been initialized for a while, such that the otg height is reliable
         const bool f_host_speed_large_enough = host.speed > 18.0F;  // Host speed larger than 65 kph (18.0 m/s)? then stationary object can be moving due to high elevation.
         const bool f_object_on_ego_lane = (std::fabs(object.vcs_position.y) < 5.0F);
         
         if (f_object_in_front &&
             f_object_just_start_moving &&
             f_object_alive_long_time &&
             f_host_speed_large_enough &&
             f_object_on_ego_lane)
         {
            // If there is a stationary-to-moving transition for the object on ego lane -- "time_since_stared_move" small -- , 
            // and the object is high enough, reset the counter to threshold for a "shortcut" to underdrivable
            object.ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U;
         }
         else
         {
            object.ud_mov_cnt_underdrivable++;
         }
      }
      else
      {
         object.ud_mov_cnt_underdrivable = 0U;
      }

      if ((F360_OBJECT_STATUS_COASTED == object.status) ||
         ((calib.ud_mov_posx_min_limit <= object.vcs_position.x) && (object.vcs_position.x <= calib.ud_mov_posx_max_limit)))
      {
         if (object.ud_mov_cnt_underdrivable > calib.ud_mov_cnt_consecutive_scans)
         {
            object.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
            object.probability_underdrivable_ocg = calib.ud_mov_prob_can_pass_under;

            object.drivable_status_sg = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
            object.drivable_confidence_sg = 100U;
         }
         else
         {
            object.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
            object.probability_underdrivable_ocg = calib.ud_mov_prob_can_not_pass_under;

            object.drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
            object.drivable_confidence_sg = 100U;
         }
      }
      else
      {
         object.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
         object.probability_underdrivable_ocg = calib.ud_mov_prob_not_to_consider;

         object.drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
         object.drivable_confidence_sg = 100U;
      }

      timing_info.determine_underdrivability_for_movable += get_wall_time() - start_time;
    }
}
