/*===================================================================================*\
* FILE:  f360_object_motion_classification_helpers.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

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
static bool Is_Obj_OTG_Height_Above_Threshold_Historically(const float32_t obj_otg_height,
                                                           const float32_t k_max_otg_height_for_small_obj_prior);

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
bool Object_Motion_Status_Should_Be_Updated(const F360_Object_Track_T &object,
                                            const float32_t k_coasted_track_max_time_from_update)
{
   return (
      (object.status == F360_OBJECT_STATUS_NEW) || (object.status == F360_OBJECT_STATUS_UPDATED) ||
      ((object.status == F360_OBJECT_STATUS_COASTED) && (object.time_since_track_updated < k_coasted_track_max_time_from_update)));
}

/*===========================================================================*\
* FUNCTION: Is_Obj_OTG_Height_Above_Threshold_Historically
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* F360_Object_Track_T& object - reference to the analysed object track
*
* ABSTRACT:
* Latching guard for the small-object movable prior height gate.
* Once an object's otg_height exceeds the threshold, sets
* f_ever_seen_tall_for_small_obj_prior and never clears it,
* permanently disabling the prior for that track.
*
\*===========================================================================*/
static bool Is_Obj_OTG_Height_Above_Threshold_Historically(const float32_t obj_otg_height,
                                                           const float32_t k_max_otg_height_for_small_obj_prior)
{
   bool f_obj_height_over_threshold = false;
   if (obj_otg_height > k_max_otg_height_for_small_obj_prior)
   {
      f_obj_height_over_threshold = true;
   }
   return f_obj_height_over_threshold;
}

/*===========================================================================*\
* FUNCTION: Calculate_Object_Moving_Probability
*===========================================================================
* RETURN VALUE:
* float32_t - LPF-smoothed linear score in [0, 1], used as movable_prob
*
* PARAMETERS:
* F360_Object_Track_T& object             - reference to the analysed object track
* const F360_Detection_Props_T& det_props - detection properties for rr_comp lookup
* const F360_Host_T& host                 - host vehicle data (yaw rate)
*
* ABSTRACT:
* Computes an early movable prior for objects in front of ego.
* The score is a weighted sum of four sub-scores:
*   1. Mean absolute compensated range-rate across associated detections (w=0.45)
*   2. Per-scan lateral (y) displacement magnitude                       (w=0.20)
*   3. Lateral velocity magnitude                                         (w=0.10)
*   4. Directional consistency (consecutive same-direction lateral scans) (w=0.25)
* The raw score is smoothed with a first-order LPF each cycle and stored
* in object.small_obj_prior_linear_score. The prior is suppressed for tall
* objects (OTG height > 2.5 m) and during host steering (|yaw_rate| > 0.45 rad/s).
*
\*===========================================================================*/
float32_t Calculate_Object_Moving_Probability(const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
                                              const F360_Host_T &host, F360_Object_Track_T &object)
{
   if (!object.f_otg_height_has_been_big)
   {
      const float32_t k_max_otg_height_for_small_obj_prior = 2.5F;
      object.f_otg_height_has_been_big =
         Is_Obj_OTG_Height_Above_Threshold_Historically(object.otg_height, k_max_otg_height_for_small_obj_prior);
   }

   const float32_t k_max_yaw_rate_for_small_obj_prior = 0.45F;
   const bool f_in_front_of_ego                       = (object.vcs_position.x > 0.0F);
   const bool f_has_previous_scan                     = (0U < object.num_updates_since_init);
   const bool f_host_yaw_rate_high                    = (std::abs(host.yaw_rate_rad) > k_max_yaw_rate_for_small_obj_prior);
   const bool f_height_ok                             = (!object.f_otg_height_has_been_big);

   float32_t moving_probability = 0.0F;
   float32_t linear_score_raw   = 0.0F;

   // Objects that belong to an LSC are excluded from the small-object movable prior,
   // because they are stationary infrastructure (e.g. guardrails).
   // Exception: the longitudinally topmost object in the cluster (no lsc_next_in_cluster)
   // is treated like a non-LSC object, since it is the leading edge and may be a real target.
   const bool f_is_lsc_leading_edge = (object.lsc_next_in_cluster == NULL);
   const bool f_is_lsc_bottom_edge  = (object.lsc_prev_in_cluster == NULL);
   const bool f_not_blocked_by_lsc  = f_is_lsc_leading_edge || f_is_lsc_bottom_edge;

   // Do not apply the small-object movable prior for tall objects.

   if (f_height_ok && (!f_host_yaw_rate_high) && f_in_front_of_ego && f_has_previous_scan && f_not_blocked_by_lsc)
   {
      // Calculating the subscores
      // 1. Calculate mean absolute range-rate-compensated value across all associated detections to the object
      float32_t mean_abs_rr_comp;
      if (0U < object.ndets)
      {
         float32_t abs_rr_comp_sum = 0.0F;
         for (uint32_t det_idx_in_obj = 0U; det_idx_in_obj < object.ndets; det_idx_in_obj++)
         {
            const uint32_t det_idx      = object.detids[det_idx_in_obj] - 1U;
            const float32_t abs_rr_comp = std::abs(det_props[det_idx].range_rate_compensated);
            abs_rr_comp_sum += abs_rr_comp;
         }
         mean_abs_rr_comp = abs_rr_comp_sum / static_cast<float32_t>(object.ndets);
      }
      else
      {
         mean_abs_rr_comp = 0.0F;
      }
      const float32_t k_rr_comp_abs_score_scale = 2.0F; // [m/s] abs(rr_comp) that maps to score=1.0
      const float32_t rr_comp_score             = F360_Saturate(mean_abs_rr_comp * k_rr_comp_abs_score_scale, 0.0F, 1.0F);

      // 2. Calculate lateral movement (y movement in VCS) since previous scan, with host-motion compensation
      // Note: object.prev_vcs_center_pos is already transformed from previous VCS to current VCS
      // in Time_Update_Tracks() via Convert_Object_Prev_Vcs_To_Current_Vcs().
      // Do not apply additional host-motion compensation here.
      const float32_t y_prev_compensated = object.prev_vcs_center_pos.y;
      // Y movement score and consistency should consider any lateral movement (either direction),
      // not only movement towards the host path (y=0).
      const float32_t y_curr                    = object.bbox.Get_Center().y;
      const float32_t signed_y_delta            = y_curr - y_prev_compensated;
      const float32_t y_movement                = std::abs(signed_y_delta);
      const float32_t k_y_movement_full_score_m = 0.03F; // Any lateral movement above 3cm will give score 1 for
      const float32_t y_movement_score          = F360_Saturate(y_movement / k_y_movement_full_score_m, 0.0F, 1.0F);

      // Determine if the object is moving towards the host path based on y movement and direction.
      // Use the threshold constant as a minimum movement deadband.
      const float32_t k_min_y_towards_path_delta       = 0.005F;
      const bool f_lateral_motion_large                = (k_min_y_towards_path_delta < y_movement);
      bool f_lateral_motion_large_with_const_direction = f_lateral_motion_large;

      // If the object crosses the host path (y changes sign), keep the movement flag true
      // to avoid resetting the consecutive counter due to a small near-zero delta.
      const bool f_y_sign_switched = ((y_prev_compensated * y_curr) < 0.0F);
      if ((!f_lateral_motion_large) && f_y_sign_switched)
      {
         // Take into account the case that the object crosses the centerline between two scans, which can result in a small y
         // delta but should still be considered as significant lateral movement.
         f_lateral_motion_large_with_const_direction = true;
      }

      // Determine lateral movement direction this scan.
      // +1: moving in positive y direction (left), -1: moving in negative y direction (right), 0: within deadband (no clear
      // direction).
      int8_t current_y_dir;
      if (signed_y_delta > k_min_y_towards_path_delta)
      {
         current_y_dir = static_cast<int8_t>(1); // Moving in positive y direction
      }
      else if (signed_y_delta < -k_min_y_towards_path_delta)
      {
         current_y_dir = static_cast<int8_t>(-1); // Moving in negative y direction
      }
      else
      {
         current_y_dir = static_cast<int8_t>(0); // Within deadband, direction undetermined
      }

      // Reset the consecutive counter when the lateral movement direction reverses.
      const bool f_direction_reversed = f_lateral_motion_large && (current_y_dir != 0) && (object.prior_prev_y_dir != 0) &&
                                        (current_y_dir != object.prior_prev_y_dir);
      if (f_direction_reversed)
      {
         f_lateral_motion_large_with_const_direction = false;
      }

      // Update stored direction when movement is unambiguous.
      if (current_y_dir != 0)
      {
         object.prior_prev_y_dir = current_y_dir;
      }

      // 3. Calculate consistency score based on consecutive scans of movement towards the same lateral direction.
      if (f_lateral_motion_large_with_const_direction)
      {
         // With the current implementation and tuning, 8 instead of 255 would be enough as well
         const uint8_t k_max_counter_value = 255U;
         if (object.cntConsecutiveLatMovement < k_max_counter_value)
         {
            object.cntConsecutiveLatMovement += 1U;
         }
      }
      else
      {
         object.cntConsecutiveLatMovement = 0U;
      }

      float32_t towards_path_consistency_score = 0.0F;
      if (object.cntConsecutiveLatMovement > 0U)
      {
         const float32_t k_consistency_scan_scale           = 0.2F;
         const uint8_t k_min_consecutive_towards_path_scans = 3U;
         towards_path_consistency_score = F360_Saturate((static_cast<float32_t>(object.cntConsecutiveLatMovement) -
                                                         static_cast<float32_t>(k_min_consecutive_towards_path_scans)) *
                                                           k_consistency_scan_scale,
                                                        0.0F, 1.0F);
      }

      // 4. Calculate lateral velocity score based on current lateral velocity in VCS
      // Normalize lateral velocity to pedestrian-scale: 0.3 m/s -> score = 1.0.
      // Without normalization, a 0.3 m/s pedestrian would only score 0.3.
      const float32_t k_lat_vel_full_score_mps = 3.33F;
      const float32_t abs_lat_vel              = std::abs(object.vcs_velocity.lateral);
      const float32_t lat_vel_score            = F360_Saturate(abs_lat_vel * k_lat_vel_full_score_mps, 0.0F, 1.0F);

      // Weights
      const float32_t k_w_rr_comp_leading = 0.45F;
      const float32_t k_w_y_mvt_leading   = 0.20F;
      const float32_t k_w_lat_vel_leading = 0.10F;
      const float32_t k_w_cons_leading    = 0.25F;

      linear_score_raw = (k_w_rr_comp_leading * rr_comp_score) + (k_w_y_mvt_leading * y_movement_score) +
                         (k_w_lat_vel_leading * lat_vel_score) + (k_w_cons_leading * towards_path_consistency_score);
   }
   else
   {
      object.cntConsecutiveLatMovement = 0U;
      object.prior_prev_y_dir          = static_cast<int8_t>(0);
   }

   // Low-pass filter the linear score every scan.
   // When the prior is not applied, filter towards 0.0F to smoothly decay the state.
   const float32_t k_linear_score_lpf_constant = 0.45F;
   moving_probability = F360_Low_Pass_Filter_First_Order(linear_score_raw, object.movable_prob, k_linear_score_lpf_constant);
   // Leave enough gap between the saturation point and threshold, to be sure that we wont have unwanted effects due to floating
   // point estimation error
   moving_probability = F360_Saturate(moving_probability, 0.0F, MOVABLE_PROB_THRESHOLD - 0.01F);

   return moving_probability;
}

/*===========================================================================*\
* FUNCTION: Update_Object_Properties
*===========================================================================
* RETURN VALUE:
* None.
*
* PARAMETERS:
* F360_Object_Track_T& object - reference to object
* const F360_Globals_T& globals - reference to global variables
* const bool f_calculate_movable_prob - flag controlling whether movable_prob is recalculated for non-moving objects
* const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS] - detection properties for movable prior calculation
* const F360_Host_T& host - host vehicle data for movable prior calculation
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
*
* PRECONDITIONS:
* None.
*
* POSTCONDITIONS:
* None.
*
\*===========================================================================*/
void Update_Object_Properties(const F360_Globals_T &globals, const bool f_calculate_movable_prob,
                              const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS], const F360_Host_T &host,
                              F360_Object_Track_T &object)
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
   else
   {
      if (f_calculate_movable_prob)
      {
         object.movable_prob = Calculate_Object_Moving_Probability(det_props, host, object);
      }
   }

   if ((object.vcs_velocity.longitudinal < globals.oncoming_speed_thresh) && (object.f_moving) &&
       (F360_INVALID_UNSIGNED_ID == object.on_sep_id))
   {
      object.f_oncoming = true;
   }
   else
   {
      object.f_oncoming = false;
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
int32_t Get_Min_Num_Consec_Moving(const F360_Object_Track_T &object, const float32_t host_yaw_rate_rad,
                                  const F360_Calibrations_T &calib)
{
   int32_t min_num_consec_moving;
   if (object.movable_prob > MOVABLE_PROB_THRESHOLD)
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
} // namespace f360_variant_A
