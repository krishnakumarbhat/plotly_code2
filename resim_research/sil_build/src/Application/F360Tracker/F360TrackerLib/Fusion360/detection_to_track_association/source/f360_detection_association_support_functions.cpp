/*===========================================================================*\
* FILE: f360_detection_association_support_functions.cpp
*============================================================================
* Copyright (C) 2020-2022 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of supporting functions for detection association.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#include <cmath>
#include <algorithm>
#include "f360_detection_association_support_functions.h"
#include "f360_norm_heading_angle.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_math_func.h"
#include "f360_associate_detection_to_object.h"
#include "f360_iterator.h"
#include "f360_calc_dist_to_edge.h"
#include "f360_bounding_box.h"
#include "f360_try_to_dealiase_range_rate.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"

namespace f360_variant_A
{
   static void Collect_Detection_Candidates_For_Association(
      const uint32_t number_of_valid_detections,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const int32_t obj_idx,
      uint32_t (&det_idx_want_to_assoc)[MAX_DETS_FOR_SINGLE_SENSOR],
      uint16_t & num_dets_want_to_assoc);

   static void Deflag_ambigous_countermeasures(
      F360_Detection_Props_T& det_prop);

   /*===========================================================================*\
   * FUNCTION: Calc_Det_Score()
   *===========================================================================
   * RETURN VALUE:
   * float32_t score
   *
   * PARAMETERS:
   *  const F360_Object_Track_T
   *  const F360_Calibrations_T
   *  const float32_t range_rat
   *  const Point & vcs_pos,
   *  const bool f_water_spray) 
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
   * Calculates a score on how well a detection matches predicted position and
   * range rate to an object. The lower the score the better the association is.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calc_Det_Score(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib,
      const float32_t range_rate_diff,
      const Point & vcs_pos,
      const bool f_water_spray)
   {
      float32_t final_score;

      if(object_track.movable_prob > 0.5F)
      {
         if (object_track.bbox.Contains(vcs_pos))
         {
            final_score = Calculate_Final_Det_Score_Inside_Solid_Bbox(vcs_pos, object_track, calib, range_rate_diff);
         }
         else if (Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(vcs_pos, object_track, f_water_spray, calib))
         {
            // Detection is inside extended bounding box but outside solid bounding box, calculate score accordingly
            final_score = Calculate_Final_Det_Score_Inside_Extended_Bbox(vcs_pos, object_track, calib, range_rate_diff);
         }
         else
         {
            // Highest value for detections outside the extended bounding box
            final_score = calib.k_score_outside_ext_bbox;
         }
      }
      else
      {
         if (object_track.bbox.Circle_Contains(vcs_pos))
         {
            final_score = Calculate_Final_Det_Score_Inside_Solid_Circle(vcs_pos, object_track, calib, range_rate_diff);
         }
         else
         {
            final_score = Calculate_Final_Det_Score_Inside_Extended_Circle(vcs_pos, object_track, calib, range_rate_diff);           
         }
         // Increase score (worsen) for newly created objects
         const float32_t time_weight = F360_Linear_Equation_With_Saturation(object_track.time_since_initialization, 0.0F, 1.0F, 2.0F, 1.0F);
         final_score *= time_weight;
      }


      return final_score;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Range_Rate_Threshold()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track,
   * const rspp_variant_A::RSPP_Detection_T & det_raw,
   * const F360_Detection_Props_T & det_prop,
   * const F360_Calibrations_T & calibrations
   * const float32_t host_vcs_speed,
   * float & range_rate_th_lower,
   * float & range_rate_th_upper
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
   * Calculate range rate thresholds for association on range rate.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Calc_Range_Rate_Threshold(
      const F360_Object_Track_T & object_track,
      const rspp_variant_A::RSPP_Detection_T & det_raw,
      const F360_Detection_Props_T & det_prop,
      const F360_Radar_Sensor_T& sens,
      const F360_Calibrations_T & calibrations,
      const float32_t host_vcs_speed,
      float32_t & range_rate_th_lower,
      float32_t & range_rate_th_upper)
   {
      float32_t range_rate_thr = calibrations.k_range_rate_score_threshold;
      const bool f_elevation_unreliable = (fabsf(det_raw.raw.elevation) > F360_DEG2RAD(6.0F)) &&
         (sens.constant.sensor_type == F360_SENSOR_TYPE_MRR360_RADAR) &&
         (host_vcs_speed > 2.0F);
      constexpr float32_t assoc_dets_thr = 0.5F;
      constexpr float32_t time_thr = 1.0F;

      const bool f_short_range_high_elevation = (det_prop.f_low_az_conf_det) && (det_raw.raw.elevation > std::abs(F360_DEG2RAD(20.0F))) && (det_raw.raw.range < 4.0F);

      if (det_prop.f_FOV_edge || f_elevation_unreliable || f_short_range_high_elevation)
      {
         // Detection is on edge of FOV or has high elevation with low azimuth confidence at small distance. Decrease range rate threshold
         range_rate_thr *= calibrations.k_rr_thr_factor_fov_edge;
      }
      else if ((object_track.status == F360_OBJECT_STATUS_COASTED) &&
         (object_track.time_since_stage_start >= calibrations.k_hyst_time_for_coasted_objects) &&
         (F360_Get_Hypotenuse_Squared(object_track.vcs_position.x, object_track.vcs_position.y) > calibrations.k_vcs_distance_sqr_thr) &&
         (object_track.speed > calibrations.k_speed_threshold))
      {
         // Object is coasted and far away. Increase range rate threshold
         range_rate_thr *=  calibrations.k_rr_thr_factor_far_away_coasted;
      }
      else if (Is_Object_Valid_For_Tightened_Gates(object_track))
      {
         const float32_t tightened_range_rate_threshold = Calculate_Tightened_Range_Rate_Threshold(object_track, calibrations.k_range_rate_score_threshold);
         range_rate_thr = tightened_range_rate_threshold;
      }
      else if ((object_track.movable_prob > 0.5F) &&
               (object_track.assoc_dets_pct_filtered < assoc_dets_thr) &&
               (object_track.time_since_started_move > time_thr))
      {
         constexpr float32_t rr_thr_factor_chaotic_env = 0.75F;
         range_rate_thr *= rr_thr_factor_chaotic_env;
      }
      else if ((rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS == det_prop.motion_status) && (object_track.speed < 5.0F)
              && (object_track.speed > 2.0F) && (object_track.behind_sep_id != F360_INVALID_UNSIGNED_ID))
      {
          // Object is slow moving and might associate to noisy detections coming from a stationary environment
          range_rate_thr *= 0.75F;
      }
      else
      {
         // Do nothing
      }

      range_rate_th_lower = -range_rate_thr;
      range_rate_th_upper = range_rate_thr;

      /* Increase association gates for fast moving CCA objects depending on how much
         they are yawing (0 yaw rate => no increase). Gates are extended non-symetrically
         in the direction of the possible prediction error (given the CTCA prediction 
         model where yaw rate is accounted for) */
      const bool f_fast_moving_cca = (F360_TRACKER_TRKFLTR_CCA == object_track.trk_fltr_type) && 
         (calibrations.fast_moving_thresh < std::abs(object_track.speed));
      if (f_fast_moving_cca)
      {  
         // Prediced range rate error is given by "yaw rate" cross "distance from reference point to detection"
         const float32_t possible_det_xvel_pred_error = -object_track.heading_rate * (det_prop.vcs_position.y - object_track.vcs_position.y);
         const float32_t possible_det_yvel_pred_error = object_track.heading_rate * (det_prop.vcs_position.x - object_track.vcs_position.x);
         const float32_t possible_pred_rr_error = possible_det_xvel_pred_error * det_raw.processed.cos_vcs_az + possible_det_yvel_pred_error * det_raw.processed.sin_vcs_az;

         /* Extend gates in either lower or upper direction if predicted error indicates they might be too restricitve.
            Only one direction can be extended at a time.The sign of predicted error indicates if it is the lower or 
            upper gate thet should be extended and the magnitude of predicted range rate error indicateshow much it 
            should be extended. Keep normal gate size if no extension is needed. Saturate at a maximum/minimum gate value. */
         const float32_t k_max_abs_rr_th = 4.0F;
         range_rate_th_lower = std::min(range_rate_th_lower, range_rate_th_lower - possible_pred_rr_error);
         range_rate_th_lower = std::max(range_rate_th_lower, -k_max_abs_rr_th);
         range_rate_th_upper = std::max(range_rate_th_upper, range_rate_th_upper - possible_pred_rr_error);
         range_rate_th_upper = std::min(range_rate_th_upper, k_max_abs_rr_th);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Allowed_To_Associate()
   *===========================================================================
   * RETURN VALUE:
   * bool f_allowed_to_associate
   *
   * PARAMETERS:
   * const F360_Detection_Props_T & det_p,
   * const rspp_variant_A::RSPP_Detection_T &det_raw,
   * const F360_Object_Track_T & obj,
   * const F360_Calibrations_T & calib,
   * const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
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
   * Check various conditions and determine if detection is allowed to associate
   * to an object.
   *
   * Conditions for association
   *   - Detections have to be ok to use
   *   - Detection can't be on a SEP if object is a moving CTCA object.
   *   - If object seems to be a mirror then ambiguous detections are not allowed to associate
   *   - Detection and object must be on the same side of a SEP if the object is a moving CTCA object (unless the
   *     the object is crossing guardrail such that front and rear are on different sides for which association to both sides of the SEP is allowed)
   *   - Stretchy track countermeasure is not needed
   *   - If the object is young (obj.time_since_initialization < 2.0 s)
   *   ---Then such objects are not allowed to associate with detections that have det_p.f_low_az_conf_det = true and high elevation angle
   *   ---det_p.f_low_az_conf_det is set in Handle_Low_Az_Conf_Detections()
   *   ---det_p.f_low_az_conf_det is available only for FLR7, SRR7+ sensor types
   *   - If the detection was marked as an object based angle jump (so also not f_ok_to_use) it can still be associated if:
   *   ---object is mature (obj.time_since_initialization > 4.0 s)
   *   ---object has zero mirror probability
   *   - If the detection was marked as a nd_target it can still be associated if:
   *   ---object is mature (obj.time_since_initialization > 2.0 s)
   *   ---object has zero mirror probability
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Is_Det_Allowed_To_Associate(
      const F360_Detection_Props_T & det_p,
      const rspp_variant_A::RSPP_Detection_T &det_raw,
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS])
   {
      constexpr float32_t elevation_threshold = F360_DEG2RAD(11.0F);
      const bool f_suspected_mirror_obj = obj.mirror_prob > calib.k_mirror_prob_threshold;
      const bool f_moving_ctca_or_cca_trk_detection_on_guardrail = ((det_p.on_sep_id != F360_INVALID_UNSIGNED_ID) && obj.f_moving && ((F360_TRACKER_TRKFLTR_CTCA == obj.trk_fltr_type) || (std::abs(obj.speed) > calib.fast_moving_thresh)));
      const bool f_suspected_mirror_obj_and_detection_ambiguous_motion = (f_suspected_mirror_obj && (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS == det_p.motion_status));
      const bool f_SEP_condition_fulfilled = Is_Association_Wrt_SEP_Allowed(det_p, obj, sep);
      const bool f_low_az_conf_for_close_obj = Is_Low_Az_Conf_At_Boundaries_Association_Allowed(det_p, obj, calib.rp_max_abs_pointing_disagreement);
      const bool f_exceptions_for_stable_obj = ((det_p.f_object_based_angle_jump) && (obj.time_since_initialization > 4.0F) && (obj.mirror_prob < F360_EPSILON));

      // If the object is young (obj.time_since_initialization < 2.0 s)
      // Then such objects are not allowed to associate with detections that have low azimuth confidence and high elevation angle
      // det_p.f_low_az_conf_det is set in Handle_Low_Az_Conf_Detections()
      // det_p.f_low_az_conf_det is available only for FLR7, SRR7+ sensor types
      const bool f_low_az_conf_high_elevation_det = ((det_p.f_low_az_conf_det) && (std::abs(det_raw.raw.elevation) > elevation_threshold) && (obj.time_since_initialization < 2.0F));

      bool f_is_bistatic_allowed_to_associate = true;
      if (det_raw.raw.f_bistatic)
      {
         if ((obj.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA) || (std::abs(obj.speed) > calib.fast_moving_thresh))
         {
            f_is_bistatic_allowed_to_associate = true;
         }
         else
         {
            f_is_bistatic_allowed_to_associate = false;
         }
      }

      bool f_is_nd_target_allowed_to_associate = true;
      if (det_p.f_nd_target)
      {
         if ((obj.time_since_initialization < 2.0F) || (obj.mirror_prob > F360_EPSILON))
         {
            f_is_nd_target_allowed_to_associate = false;
         }
      }

      const bool f_allowed_to_associate = ((det_p.f_ok_to_use || det_p.f_double_bounce || f_exceptions_for_stable_obj) && // allow double bounce detections to associate for now to find mirror objects
         (f_is_bistatic_allowed_to_associate) &&
         (!f_moving_ctca_or_cca_trk_detection_on_guardrail) &&
         (!f_suspected_mirror_obj_and_detection_ambiguous_motion) &&
         (f_SEP_condition_fulfilled) &&
         (!f_low_az_conf_high_elevation_det) &&
         (f_low_az_conf_for_close_obj) &&
         (f_is_nd_target_allowed_to_associate));

      return f_allowed_to_associate;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Low_Az_Conf_At_Boundaries_Association_Allowed()
   *===========================================================================
   * RETURN VALUE:
   * bool f_association_allowed
   *
   * PARAMETERS:
   * const F360_Detection_Props_T & det_p,
   * const F360_Object_Track_T & obj,
   * const float32_t max_heading_threshold
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determines if association between an object and detection with low_az_conf is allowed at the boundaries of the assosiation gate.
   * If the object is close to the host and the detection that has low_az_conf at the boundries of the association gates, not associate it.
   * More information about area itself can be found in ticket DFD-2841 in presentation attached.
   *
   \*===========================================================================*/
   bool Is_Low_Az_Conf_At_Boundaries_Association_Allowed(
       const F360_Detection_Props_T& det_p,
       const F360_Object_Track_T& obj,
       const float32_t max_heading_threshold)
   {
       bool f_association_allowed = true;
       constexpr float32_t detection_lateral_position_threshold = 2.5F;
       constexpr float32_t object_lateral_position_threshold = 2.5F;
       constexpr float32_t object_longitudal_position_threshold = 3.0F;

       const bool low_az_candidate = ((det_p.f_low_az_conf_det) &&
           (std::abs(det_p.vcs_position.y) < detection_lateral_position_threshold) &&
           (std::abs(obj.vcs_position.y) < object_lateral_position_threshold) &&
           (std::abs(obj.vcs_position.x) < object_longitudal_position_threshold) &&
           (std::abs(obj.vcs_heading.Value()) < max_heading_threshold));
       
       if (low_az_candidate)
       {
           Point det_tcs;
           Point excluded_zone_left_rear_point;
           Point excluded_zone_right_front_point;
           Convert_VCS_Posn_To_TCS_Posn(
               det_p.vcs_position.x,
               det_p.vcs_position.y,
               obj.bbox.Get_Center().x,
               obj.bbox.Get_Center().y,
               obj.bbox.Get_Orientation(),
               det_tcs.x,
               det_tcs.y);

           if (obj.vcs_position.y > 0.0F)
           {
               excluded_zone_left_rear_point = { 0.0F, -(obj.bbox.Get_Width() * 0.5F) - obj.lat_buffer_zone_wid1};
               excluded_zone_right_front_point = { obj.bbox.Get_Length() * 0.5F + obj.long_buffer_zone_len2, -(obj.bbox.Get_Width() * 0.5F) };
           }
           else
           {
               excluded_zone_left_rear_point = { 0.0F, obj.bbox.Get_Width() * 0.5F };
               excluded_zone_right_front_point = { obj.bbox.Get_Length() * 0.5F + obj.long_buffer_zone_len2, obj.bbox.Get_Width() * 0.5F + obj.lat_buffer_zone_wid2 };
           }
           const bool f_detection_in_excluded_zone = ((det_tcs.x > excluded_zone_left_rear_point.x) &&
               (det_tcs.x < excluded_zone_right_front_point.x) &&
               (det_tcs.y > excluded_zone_left_rear_point.y) &&
               (det_tcs.y < excluded_zone_right_front_point.y));

           if (f_detection_in_excluded_zone)
           {
               f_association_allowed = false;
           }
       }
       return f_association_allowed;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Association_Wrt_SEP_Allowed()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_association_allowed
   *
   * PARAMETERS:
   * const F360_Detection_Props_T & det_p,
   * const F360_Object_Track_T & obj,
   * const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determines if association between an object and detection is allowed w.r.t. SEPs.
   * - If the object is ambiguously behind the SEP, then association is allowed even if detection and object are on opposite side of the SEP.
   * - If detection and object are on the same side of the SEP, association is always allowed.
   * - Association across SEPs is always allowed for non-moving tracks.
   *
   \*===========================================================================*/
   bool Is_Association_Wrt_SEP_Allowed(
      const F360_Detection_Props_T & det_p,
      const F360_Object_Track_T & obj,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS])
   {
      bool f_association_allowed;
      if (obj.f_moving) // Moving object => restrict association over guardrails
      {
         // Get object rear and front VCS positions
         Point obj_rear_vcs_pos;
         Convert_TCS_Posn_To_VCS_Posn(
            -obj.bbox.Get_Length() * 0.5F,
            0.0F,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            obj_rear_vcs_pos.x,
            obj_rear_vcs_pos.y);

         Point obj_front_vcs_pos;
         Convert_TCS_Posn_To_VCS_Posn(
            obj.bbox.Get_Length() * 0.5F,
            0.0F,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            obj_front_vcs_pos.x,
            obj_front_vcs_pos.y);

         f_association_allowed = true; // Default is true. Loop over all seps and see if we need to change to false
         for (uint8_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
         {
            if (F360_STATIC_ENV_POLY_STATUS_INVALID != sep[sep_idx].status)
            {
               const float32_t sep_x_limit_extention = 6.0F; // [m]
               const bool f_obj_next_to_sep = (obj.bbox.Get_Center().x > (sep[sep_idx].lower_limit - sep_x_limit_extention)) && (obj.bbox.Get_Center().x < (sep[sep_idx].upper_limit + sep_x_limit_extention));
               const bool f_det_next_to_sep = (det_p.vcs_position.x > (sep[sep_idx].lower_limit)) && (det_p.vcs_position.x < (sep[sep_idx].upper_limit));

               if (f_obj_next_to_sep && f_det_next_to_sep)
               {
                  const float32_t y_sep_det = det_p.vcs_position.y - sep[sep_idx].SEP_Lateral_Pos_At(det_p.vcs_position.x);
                  const float32_t y_sep_obj_center = obj.bbox.Get_Center().y - sep[sep_idx].SEP_Lateral_Pos_At(obj.bbox.Get_Center().x);
                  const bool f_opposite_sides = F360_Sign(y_sep_det) != F360_Sign(y_sep_obj_center);

                  const float32_t y_sep_obj_front = obj_front_vcs_pos.y - sep[sep_idx].SEP_Lateral_Pos_At(obj_front_vcs_pos.x);
                  const float32_t y_sep_obj_rear = obj_rear_vcs_pos.y - sep[sep_idx].SEP_Lateral_Pos_At(obj_rear_vcs_pos.x);
                  const bool f_obj_ambigous_side = F360_Sign(y_sep_obj_front) != F360_Sign(y_sep_obj_rear);

                  if (f_opposite_sides && (!f_obj_ambigous_side))
                  {
                     f_association_allowed = false;
                     break;
                  }
               }
            }
         }
      }
      else
      {
         // Stationary object => always allow association across guardrail
         f_association_allowed = true;
      }
      return f_association_allowed;
   }

   /*===========================================================================*\
   * FUNCTION: Compare_Against_Stationary_Hypothesis()
   *===========================================================================
   * RETURN VALUE:
   * bool f_moving_hypothesis_significantly_better_than_stat
   *
   * PARAMETERS:
   *  const F360_Calibrations_T& calibrations,
   *  const F360_Radar_Sensor_T& sensor,
   *  const rspp_variant_A::RSPP_Detection_T& detection,
   *  const float32_t range_rate_threshold,
   *  const float32_t predicted_range_rate,
   *  const float32_t dealiased_rngrate,
   *  F360_Detection_Props_T& det_p
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * This function calculates the difference between predicted and de-aliased range rate
   * for an assumed stationary object. This difference is compared to that of the real object
   * and if the stationary object fits better, and the difference is significant, association
   * to the real object is blocked.
   * --------------------------------------------------------------------------
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Compare_Against_Stationary_Hypothesis(
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& detection,
      const float32_t range_rate_threshold_lower,
      const float32_t range_rate_threshold_upper,
      const float32_t predicted_range_rate,
      const float32_t dealiased_rngrate,
      F360_Detection_Props_T& det_p
   )
   {
      // Calculate predicted range rate for an assumed stationary object
      const float32_t relative_velocity_x = -sensor.variable.vcs_velocity.longitudinal;
      const float32_t relative_velocity_y = -sensor.variable.vcs_velocity.lateral;
      const float32_t stat_predicted_range_rate = relative_velocity_x * detection.processed.cos_vcs_az + relative_velocity_y * detection.processed.sin_vcs_az;

      float32_t stat_dealiased_rngrate = 0.0F;
      float32_t stat_rr_interval = 0.0F;
      const bool f_stat_inside_gate = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(
         detection.raw.range_rate,
         stat_predicted_range_rate,
         range_rate_threshold_lower,
         range_rate_threshold_upper,
         sensor.constant.v_wrapping[sensor.variable.look_id],
         sensor.constant.min_aliaised_range_rate[sensor.variable.look_id],
         stat_dealiased_rngrate,
         stat_rr_interval);

      bool f_moving_hypothesis_significantly_better_than_stat = true;
      if (f_stat_inside_gate) // Stationary hypothesis is inside rough range rate gate, compare with moving object's range rate diff
      {
         const float32_t moving_obj_rr_hypothesis_error = std::abs(predicted_range_rate - dealiased_rngrate);
         const float32_t stat_rr_hypothesis_error = std::abs(stat_predicted_range_rate - stat_dealiased_rngrate);

         const bool f_moving_hypothesis_better = (stat_rr_hypothesis_error > moving_obj_rr_hypothesis_error);
         const bool f_significant_hypothesis_difference = (std::abs(stat_rr_hypothesis_error - moving_obj_rr_hypothesis_error) > calibrations.k_min_rr_diff_from_stationary_hypothesis);

         f_moving_hypothesis_significantly_better_than_stat = (f_moving_hypothesis_better && (f_significant_hypothesis_difference || det_p.f_inside_mov_gate));
         if ((f_moving_hypothesis_better) && (!f_significant_hypothesis_difference) && (det_p.f_inside_mov_gate))
         {
             det_p.f_rr_ambiguity = true;
         }
         else
         {
             // Do nothing, to avoid MISRA warning
         }
      }
      else
      {
          // Do nothing, to avoid MISRA warning
      }
      // To avoid MISRA warning
      (void)(stat_rr_interval);
      return f_moving_hypothesis_significantly_better_than_stat;
   }

   /*===========================================================================*\
   * FUNCTION: Collect_Detection_Candidates_For_Association()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint32_t number_of_valid_detections
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * const int32_t obj_idx
   * uint32_t (&det_idx_want_to_assoc)[MAX_DETS_IN_OBJ_TRK]
   * uint16_t & num_dets_want_to_assoc
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function goes through all the detections and collects the indices of all detections that want
   * to associate to the specified object and counts how many detections that want to associate to the object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Collect_Detection_Candidates_For_Association(
      const uint32_t number_of_valid_detections,
      const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const int32_t obj_idx,
      uint32_t (&det_idx_want_to_assoc)[MAX_DETS_FOR_SINGLE_SENSOR],
      uint16_t & num_dets_want_to_assoc)
   {
      for (uint32_t det_i = 0U; det_i < number_of_valid_detections; det_i++)
      {
         if (detection_props[det_i].object_track_id == (obj_idx + 1))
         {
            // Detection wants to associate to object, store in temporary array
            if (num_dets_want_to_assoc < MAX_DETS_FOR_SINGLE_SENSOR)
            {
               det_idx_want_to_assoc[num_dets_want_to_assoc] = det_i;
               num_dets_want_to_assoc++;
            }
            else
            {
               break;
            }

         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Association_Hypothesis()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint32_t number_of_valid_detections
   * const float32_t det_rdot_comp_array[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Tracker_Info_T & tracker_info
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function loops over all valid objects and associates detections to objects.
   * For every detection that associates to an object, the detection's compensated range rate property
   * is updated and the flag f_dealiased is set to true.
   * If more than maximum allowed detections in an object wants to associate to an object,
   * a cost is calculated for each detection and the detections with the lowest costs are associated.
   * The leftover detections are set as not ok to use.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Assign_Association_Hypothesis(
      const uint32_t number_of_valid_detections,
      const float32_t(&det_rdot_comp_array)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      for (int32_t obj_i = 0; obj_i < tracker_info.num_active_objs; obj_i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[obj_i] - 1;

         uint16_t num_dets_want_to_assoc = 0U;
         uint32_t det_idx_want_to_assoc[MAX_DETS_FOR_SINGLE_SENSOR];

         Collect_Detection_Candidates_For_Association(number_of_valid_detections, detection_props, obj_idx, det_idx_want_to_assoc, num_dets_want_to_assoc);

         if (num_dets_want_to_assoc > tracker_info.variant.num_dets_in_track)
         {
            // Calculate a detection cost for all detections and choose the best ones
            float32_t temp_cost_array[MAX_DETS_FOR_SINGLE_SENSOR];

            for (uint16_t det_i = 0U; det_i < num_dets_want_to_assoc; det_i++)
            {
               const uint32_t det_idx = det_idx_want_to_assoc[det_i];
               temp_cost_array[det_i] = Calculate_Detection_Association_Cost(detection_props[det_idx], object_tracks[obj_idx]);
            }

            uint32_t perm[MAX_DETS_FOR_SINGLE_SENSOR];
            (void)F360_Sort(static_cast<uint32_t>(num_dets_want_to_assoc), true, temp_cost_array, perm);

            // Associate the MAX_DETS_IN_OBJ_TRK detections with lowest cost
            for (uint32_t i = 0U; i < tracker_info.variant.num_dets_in_track; i++)
            {
               const uint32_t det_idx = det_idx_want_to_assoc[perm[i]];

               // Return value is redundant, all detections will associate correctly as Associate_Detection_To_Object()
               // is called MAX_DETS_IN_OBJ_TRK times and object is valid.
               (void)Associate_Detection_To_Object(tracker_info, object_tracks[obj_idx], detection_props[det_idx], (det_idx + 1U));
               detection_props[det_idx].range_rate_compensated = det_rdot_comp_array[det_idx];
               detection_props[det_idx].f_dealiased = true;

               // If associated detection had flag f_ok_to_use = false and was thought to be angle jump, set it back to true
               Deflag_ambigous_countermeasures(detection_props[det_idx]);
            }

            // Discard the leftover detections
            for (uint32_t i = tracker_info.variant.num_dets_in_track; i < num_dets_want_to_assoc; i++)
            {
               const uint32_t det_idx = det_idx_want_to_assoc[perm[i]];
               detection_props[det_idx].f_ok_to_use = false;
               detection_props[det_idx].object_track_id = 0;
            }
         }
         else
         {
            // All detections that want to associate fit in object
            for (uint32_t i = 0U; i < num_dets_want_to_assoc; i++)
            {
               const uint32_t det_idx = det_idx_want_to_assoc[i];

               // Return value is redundant, all detections will associate correctly as Associate_Detection_To_Object()
               // is called less than MAX_DETS_IN_OBJ_TRK times and object is valid.
               (void)Associate_Detection_To_Object(tracker_info, object_tracks[obj_idx], detection_props[det_idx], (det_idx + 1U));
               detection_props[det_idx].range_rate_compensated = det_rdot_comp_array[det_idx];
               detection_props[det_idx].f_dealiased = true;

               // If associated detection had flag f_ok_to_use = false and was thought to be angle jump, set it back to true
               Deflag_ambigous_countermeasures(detection_props[det_idx]);
            }
         }
      }
   }

/*===========================================================================*\
* FUNCTION: Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox()
*===========================================================================
* RETURN VALUE:
* float32_t position_score
*
* PARAMETERS:
* const Point& det_position_vcs
* const F360_Object_Track_T& object_track,
* const F360_Calibrations_T& calib
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Calculates the score of detection based on the position of the detection
* in the bounding box. In the bbox shown belowm it is divided into zones numbered
* 1,2,3,4 and the front of the bbox, is the bbox edge that lies in zone 1.
* For Zones numbered 1 and 3, the score is calculated considering the distance
* of the detection to  the shortest edge and in zones numbered 2 and 4 the
* score is calculated considering the distance of the detection to the longest edge.

  FRONT
+--------+
|\  1  / |
| \   /  |
|  \ /   |
| 4 X  2 |
|  / \   |
| /   \  |
|/   3 \ |
+--------+

*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
\*===========================================================================*/

   float32_t Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track,
      const F360_Calibrations_T& calib)
   {
      float32_t position_score = calib.k_base_score_bbox_center;

      /* Transform detection in to the objects reference frame TCS */
      Point detection_in_tcs = det_position_vcs;
      detection_in_tcs.Transform_To_Relative_Coordinate_System(object_track.bbox.Get_Center(), object_track.bbox.Get_Orientation());
      const BoundingBox bbox_in_tcs = BoundingBox(Point(0.0F,0.0F), object_track.bbox.Get_Length(), object_track.bbox.Get_Width(), Angle(0.0F));

      const float32_t distance_from_det_to_bbox_center_along_tcs_x = std::fabs(detection_in_tcs.x);
      const float32_t distance_from_det_to_bbox_center_along_tcs_y = std::fabs(detection_in_tcs.y);
      const float32_t slope_from_det_to_bbox_center = bbox_in_tcs.Get_Slope_from_bbox_center_to_point(detection_in_tcs);
      const float32_t slope_of_diagonal_of_bbox = bbox_in_tcs.Get_Slope_Of_Diagonal();
      const float32_t abs_slope_from_det_to_bbox_center = std::fabs(slope_from_det_to_bbox_center);
      const float32_t abs_slope_of_diagonal_of_bbox = std::fabs(slope_of_diagonal_of_bbox);

      /*Check the slope of the detction to the center and compare it the slope of the bbox diagonal,
      which indicates the zone the detection belongs to and then normalize its distance with the length or width of the bbox*/
      if (abs_slope_from_det_to_bbox_center >= abs_slope_of_diagonal_of_bbox)
      {
         position_score = calib.k_base_score_bbox_center * (1.0F - (distance_from_det_to_bbox_center_along_tcs_y / (0.5F*bbox_in_tcs.Get_Width())));
      }
      else
      {
         position_score = calib.k_base_score_bbox_center *(1.0F - (distance_from_det_to_bbox_center_along_tcs_x / (0.5F*bbox_in_tcs.Get_Length())));
      }

      return position_score;
   }
   /*===========================================================================*\
   * FUNCTION: Calculate_Final_Det_Score_Inside_Solid_Bbox()
   *===========================================================================
   * RETURN VALUE:
   * float32_t final_score
   *
   * PARAMETERS:
   * const Point & det_position_vcs
   * const F360_Object_Track_T & object_track
   * const F360_Calibrations_T & calib
   * const float32_t range_rate_diff
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculate final detection score for detections positioned inside object bounding box
   * based on the position and range rate of the object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calculate_Final_Det_Score_Inside_Solid_Bbox(
      const Point & det_position_vcs,
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib,
      const float32_t range_rate_diff)
   {
      const float32_t score_based_on_det_position = Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox(det_position_vcs, object_track, calib);
      float32_t weights[3];
      weights[0] = calib.k_para_diff_weight_inside_box;
      weights[1] = calib.k_orth_diff_weight_inside_box;
      weights[2] = calib.k_rdot_diff_weight_inside_box;

      const float32_t final_score = (weights[0] + weights[1]) * score_based_on_det_position + weights[2] * (std::fabs(range_rate_diff / calib.k_range_rate_score_threshold));
      return final_score;
   }


/*===========================================================================*\
* FUNCTION: Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox()
*===========================================================================
* RETURN VALUE:
* float32_t position_score
*
* PARAMETERS:
* const Point& det_position_vcs
* const F360_Object_Track_T& object_track,
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Calculates the score of detection based on the position of the detection
* outside the bounding box but inside the extended bounding box. For the bbox and
* extended bbox shown below, various zones are numbered between 1 and 12. The front
* of the bbox is the edge in zone 2. The front of the extended bbox is the edge which
* is in the zone 9,6,10. For detections in zone5 and zone 7
* the distance from the longest edge of the bbox is considered for scoring.
* For detections in zone 6 and 8 the distance from the shortest edge of the bbox
* is considered for scoring. For detections in zone 9, 10, 11, 12
* where it is not obvious which edge to use to calculate the score
* we draw a diagonal from the vertex of the bbox to the closest corresponding
* vertex of the extended bbox and the detections below the diagonal
* are scored based on the distance from the longest edge
* and the detections above the diagonal are scored based on the distance
* to the shortest edge.
*

       FRONT
%%%%%%%%%%%%%%%%%%%%%%
%     |        |      %
%   9 |   6    | 10   %
%     |        |      %
%-----+--------+------%
%     |\      /|      %
%     | \  2 / |      %
%     |  \  /  |      %
%   5 | 1 \/  3| 7    %
%     |   /\   |      %
%     |  /  \  |      %
%     | /  4 \ |      %
%     |/      \|      %
%-----+--------+------%
%     |        |      %
% 12  |   8    |   11 %
%     |        |      %
%%%%%%%%%%%%%%%%%%%%%%


*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
\*===========================================================================*/

   float32_t Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track)
   {
      float32_t position_score = 1.0F;
      float32_t length_of_obj_buffer_zone_along_tcs_x = 1.0F;
      float32_t width_of_obj_buffer_zone_along_tcs_y = 1.0F;

      /* Transform detection in to the objects reference frame TCS */
      Point detection_in_tcs = det_position_vcs;
      detection_in_tcs.Transform_To_Relative_Coordinate_System(object_track.bbox.Get_Center(), object_track.bbox.Get_Orientation());

      /*Create a BoundingBox instance in tcs frame*/
      const BoundingBox bbox_in_tcs = BoundingBox(Point(0.0F,0.0F), object_track.bbox.Get_Length(), object_track.bbox.Get_Width(), Angle(0.0F));

      if (detection_in_tcs.x < 0.0F)
      {
         length_of_obj_buffer_zone_along_tcs_x = object_track.long_buffer_zone_len1;
      }
      else
      {
         length_of_obj_buffer_zone_along_tcs_x = object_track.long_buffer_zone_len2;
      }
      if ( detection_in_tcs.y < 0.0F)
      {
         width_of_obj_buffer_zone_along_tcs_y = object_track.lat_buffer_zone_wid1;
      }
      else
      {
         width_of_obj_buffer_zone_along_tcs_y = object_track.lat_buffer_zone_wid2;
      }

      const float32_t bbox_half_width = 0.5F * bbox_in_tcs.Get_Width();
      const float32_t bbox_half_length = 0.5F * bbox_in_tcs.Get_Length();

      const bool check_less_than_half_width = std::fabs( detection_in_tcs.y) <= bbox_half_width;
      const bool check_greater_than_half_length = std::fabs(detection_in_tcs.x) >= bbox_half_length;
      const bool check_greater_than_half_width = std::fabs( detection_in_tcs.y) > bbox_half_width;
      const bool check_less_than_half_length = std::fabs(detection_in_tcs.x) < bbox_half_length;

      const float32_t dist_to_detection_from_bbox_edge_along_tcs_x = std::fabs(detection_in_tcs.x) - bbox_half_length;
      const float32_t dist_to_detection_from_bbox_edge_along_tcs_y = std::fabs( detection_in_tcs.y) - bbox_half_width;

      /*If the detection is behind or in front of the bbox, score based on the distance to the distance to the edge along tcs x*/
      if (check_less_than_half_width && check_greater_than_half_length)
      {
         position_score = dist_to_detection_from_bbox_edge_along_tcs_x / length_of_obj_buffer_zone_along_tcs_x;
      }
      /*If the detection is to the left or right of the bbox, score based on the distance to the edge along tcs y*/
      else if (check_greater_than_half_width && check_less_than_half_length)
      {
         position_score = dist_to_detection_from_bbox_edge_along_tcs_y / width_of_obj_buffer_zone_along_tcs_y;
      }
      /* Refer to the figure in the Abstract of the function, if the detection lies in zone 9,10,11,12
      then we consider scoring based on the diagonals of the vertex of the bbox and the closest corresponding vertex of the extended bbox */
      else
      {
         const Point closest_corner = bbox_in_tcs.Get_Closest_Corner_To_Point(detection_in_tcs);
         const float32_t slope_of_detection_to_closest_bbox_corner = Point::get_slope_between_points(closest_corner, detection_in_tcs);
         const float32_t slope_of_diagonal_of_corner_rectangle = width_of_obj_buffer_zone_along_tcs_y / length_of_obj_buffer_zone_along_tcs_x;

         if (std::fabs(slope_of_detection_to_closest_bbox_corner) >= std::fabs(slope_of_diagonal_of_corner_rectangle))
         {
            position_score = dist_to_detection_from_bbox_edge_along_tcs_y / width_of_obj_buffer_zone_along_tcs_y;
         }
         else
         {
            position_score = dist_to_detection_from_bbox_edge_along_tcs_x / length_of_obj_buffer_zone_along_tcs_x;
         }
      }

      return position_score;
   }
   /*===========================================================================*\
   * FUNCTION: Calculate_Final_Det_Score_Inside_Extended_Bbox()
   *===========================================================================
   * RETURN VALUE:
   * float32_t final_score
   *
   * PARAMETERS:
   * const Point & det_position_vcs
   * const F360_Object_Track_T & object_track
   * const F360_Calibrations_T & calib
   * const float32_t range_rate_diff
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculate final detection score for detections positioned inside object extended
   * bounding box, using detection position and range rate.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calculate_Final_Det_Score_Inside_Extended_Bbox(
      const Point & det_position_vcs,
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib,
      const float32_t range_rate_diff)
   {

      float32_t weights[3];
      weights[0] = calib.k_para_diff_weight_inside_ext_box;
      weights[1] = calib.k_orth_diff_weight_inside_ext_box;
      weights[2] = calib.k_rdot_diff_weight_inside_ext_box;

      const float position_score = Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(det_position_vcs, object_track);
      const float final_score = (weights[0] + weights[1]) * position_score + weights[2] * (std::fabs(range_rate_diff) / calib.k_range_rate_score_threshold);

      return final_score;
   }


   /*===========================================================================*\
   * FUNCTION: Calculate_Final_Det_Score_Inside_Solid_Circle()
   *===========================================================================
   * RETURN VALUE:
   * float32_t score
   *
   * PARAMETERS:
   * const Point & det_position_vcs
   * const F360_Object_Track_T & object_track
   * const F360_Calibrations_T & calib
   * const float32_t range_rate_diff
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculate detection score for detections positioned inside the outline of a
   * non-moveable object, i.e inside the solid object circle.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calculate_Final_Det_Score_Inside_Solid_Circle(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track,
      const F360_Calibrations_T& calib,
      const float32_t range_rate_diff)
   {
      const float32_t dist = object_track.bbox.Distance_To_Center(det_position_vcs);
      const float32_t normalized_dist = dist / (calib.k_nonmoveable_target_diameter * 0.5F);
      const float32_t normalized_rr_diff = range_rate_diff / calib.k_range_rate_score_threshold;

      const float32_t position_score = calib.k_base_score_bbox_center*(1.0F - std::fabs(normalized_dist));

      const float32_t score = (calib.k_dist_weight_inside_solid_circle * position_score +
         calib.k_rdot_diff_weight_inside_solid_circel *std::fabs(normalized_rr_diff));

      return score;
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Final_Det_Score_Inside_Extended_Circle()
   *===========================================================================
   * RETURN VALUE:
   * float32_t score
   *
   * PARAMETERS:
   * const Point & det_position_vcs
   * const F360_Object_Track_T & object_track
   * const F360_Calibrations_T & calib
   * const float32_t range_rate_diff
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculate detection score for detections positioned inside the extened
   * circle that constitutes the position association gate of a non-moveable object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calculate_Final_Det_Score_Inside_Extended_Circle(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track,
      const F360_Calibrations_T& calib,
      const float32_t range_rate_diff)
   {
      const float32_t dist = object_track.bbox.Distance_To_Center(det_position_vcs);
      const float32_t bbox_radius = object_track.bbox.Get_Length() * 0.5F;
      const float32_t normalized_dist = (dist-bbox_radius) / (object_track.long_buffer_zone_len1);
      const float32_t normalized_rr_diff = range_rate_diff / calib.k_range_rate_score_threshold;

      const float32_t score =  calib.k_dist_weight_inside_ext_circle * std::fabs(normalized_dist) +
         calib.k_rdot_diff_weight_inside_ext_circel * std::fabs(normalized_rr_diff);

      return score;
   }


   /*===========================================================================*\
   * FUNCTION: Calculate_Detection_Association_Cost()
   *===========================================================================
   * RETURN VALUE:
   * float32_t det_cost - The detection cost
   *
   * PARAMETERS:
   * const F360_Detection_Props_T & det_prop - Detection property
   * const F360_Object_Track_T & obj_trk     - Object track
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function calculates a cost for a detection in order to be able to prioritize
   * what detections to associate to a track. The function calculates the distance from
   * the detection to each edge and uses the distance to the closest edge as cost.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calculate_Detection_Association_Cost(
      const F360_Detection_Props_T & det_prop,
      const F360_Object_Track_T & obj_trk)
   {
      float32_t det_cost = 0.0F;

      if (obj_trk.movable_prob > 0.5F)
      {
         Point det_tcs;

         Convert_VCS_Posn_To_TCS_Posn(
            det_prop.vcs_position.x,
            det_prop.vcs_position.y,
            obj_trk.bbox.Get_Center().x,
            obj_trk.bbox.Get_Center().y,
            obj_trk.bbox.Get_Orientation(),
            det_tcs.x,
            det_tcs.y);

         const float32_t half_length = 0.5F * obj_trk.bbox.Get_Length();
         const float32_t half_width = 0.5F * obj_trk.bbox.Get_Width();

         const Point tcs_front_left = { half_length, -half_width };
         const Point tcs_front_right = { half_length, half_width };
         const Point tcs_rear_left = { -half_length, -half_width };
         const Point tcs_rear_right = { -half_length, half_width };

         const float32_t dist_front = Calculate_Distance_To_Edge(tcs_front_left, tcs_front_right, det_tcs);
         const float32_t dist_left = Calculate_Distance_To_Edge(tcs_front_left, tcs_rear_left, det_tcs);
         const float32_t dist_rear = Calculate_Distance_To_Edge(tcs_rear_left, tcs_rear_right, det_tcs);
         const float32_t dist_right = Calculate_Distance_To_Edge(tcs_front_right, tcs_rear_right, det_tcs);

         // Find shortest distance to one of the edges and use that as cost for detection
         const float32_t shortest_dist_para = std::min(dist_front, dist_rear);
         const float32_t shortest_dist_orth = std::min(dist_left, dist_right);
         det_cost = std::min(shortest_dist_para, shortest_dist_orth);
      }
      else
      {
         det_cost = obj_trk.bbox.Distance_To_Center(det_prop.vcs_position);
      }

      return det_cost;
   }

   /*===========================================================================*\
   * FUNCTION: Deflag_ambigous_countermeasures()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T & det_prop
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function deflags detections that were previously thought to be
   * angle jumps, but are not.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Deflag_ambigous_countermeasures(
      F360_Detection_Props_T& det_prop
   )
   {
      if (det_prop.f_object_based_angle_jump)
      {
         det_prop.f_ok_to_use = true;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Deassociate_Detection()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T& detection,
   * const F360_Radar_Sensor_T& sensor,
   * F360_Detection_Props_T& det_prop
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
   * Deassociate a detection from its object track and make it available for
   * association to other object tracks. Reset all signals that could have been
   * changed due to association.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * Object association signals updated.
   *
   \*===========================================================================*/
   void Deassociate_Detection(
      const rspp_variant_A::RSPP_Detection_T& detection,
      const F360_Radar_Sensor_T& sensor,
      F360_Detection_Props_T& det_prop
   )
   {
      det_prop.object_track_id = 0;
      det_prop.f_dealiased = false;
      det_prop.range_dealiased = detection.raw.range;
      det_prop.range_rate_dealiased = detection.raw.range_rate;
      det_prop.vcs_position.x = detection.processed.vcs_position_x;
      det_prop.vcs_position.y = detection.processed.vcs_position_y;

      // Recompute the compensated range rate based on the raw detection values and sensor mounting velocity
      const float32_t rdot_pred = -sensor.variable.vcs_velocity.longitudinal * detection.processed.cos_vcs_az -
         sensor.variable.vcs_velocity.lateral * detection.processed.sin_vcs_az;
      det_prop.range_rate_compensated = detection.raw.range_rate - rdot_pred;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Valid_For_Tightened_Gates()
   *===========================================================================
   * RETURN VALUE:
   * bool - true if object meets criteria for tightened association gates, false otherwise
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_track - Object track to evaluate
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
   * Determines if an object qualifies for specialized tightened association gates
   * based on multiple criteria. This function identifies slow-moving objects (2-7 m/s)
   * that are in front of the host vehicle with stable motion characteristics.
   * 
   * PRECONDITIONS:
   * Object track structure must be properly initialized.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   bool Is_Object_Valid_For_Tightened_Gates(
      const F360_Object_Track_T& object_track
   )
   {
      const float32_t min_time_since_started_move = 0.1F; // 2 * 0.05s -> 2 cycles
      bool is_valid = false;
      if ((object_track.movable_prob > 0.5F) &&
         (object_track.speed < 7.0F) &&
         (object_track.speed > 2.0F) &&
         (std::abs(object_track.vcs_position.y) < 15.0F) &&
         (std::abs(object_track.vcs_heading.Value_Deg()) < 60.0F) &&
         (std::abs(object_track.bbox.Get_Orientation().Value_Deg()) < 60.0F) &&
         (object_track.vcs_position.x > 0.0F) &&
         (object_track.time_since_started_move > min_time_since_started_move) &&
         (std::abs(object_track.tang_accel) < 1.0F) &&
         (object_track.filtered_hist_assoc_det_rr_err_mean < 1.0F))
      {
         is_valid = true;
      }
      return is_valid;
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Tightened_Range_Rate_Threshold()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - Calculated range rate threshold in m/s
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_track        - Object track containing speed and heading rate
   * const float32_t max_range_rate_threshold       - Maximum allowed range rate threshold (saturates output)
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
   * Calculates a tightened range rate threshold for slow-moving objects that meet
   * specialized association criteria. The threshold is computed dynamically based on
   * the object's speed and heading rate.
   *
   * PRECONDITIONS:
   * - Object track must have valid speed and heading_rate values
   *
   * POSTCONDITIONS:
   * Returns a threshold value between 0.7 m/s and max_range_rate_threshold.
   \*===========================================================================*/
   float32_t Calculate_Tightened_Range_Rate_Threshold(
      const F360_Object_Track_T& object_track,
      const float32_t max_range_rate_threshold
   )
   {
      const float32_t min_rr_thold = 0.7F;
      // Constants for speed component calculation
      const float32_t min_speed = 5.0F; // [m/s]
      const float32_t max_speed = 7.0F; // [m/s]

      const float32_t speed_component = F360_Linear_Equation_With_Saturation(object_track.speed, min_speed, max_speed, 0.0F, max_range_rate_threshold);

      // Constants for heading rate component calculation
      const float32_t min_heading_rate = 0.0F; // [rad/s]
      const float32_t max_heading_rate = 0.5F; // [rad/s]
      const float32_t min_rr_heading_rate_component = 0.0F; // [m/s]
      const float32_t max_rr_heading_rate_component = 0.8F; // [m/s]
      const float32_t heading_rate_component = F360_Linear_Equation_With_Saturation(std::abs(object_track.heading_rate), min_heading_rate, max_heading_rate, min_rr_heading_rate_component, max_rr_heading_rate_component);
      
      const float32_t combined_thr = min_rr_thold + speed_component + heading_rate_component;
      /* Examples:
         - speed 5 m/s, heading_rate 0.0 rad/s -> threshold = 0.7 + 0.0 + 0.0 = 0.7 m/s
         - speed 7 m/s, heading_rate 0.0 rad/s -> threshold = 0.7 + 2.0 + 0.0 = 2.7 m/s
         - speed 5 m/s, heading_rate 0.5 rad/s -> threshold = 0.7 + 0.0 + 0.8 = 1.5 m/s
      */
      const float32_t range_rate_thr = std::min(combined_thr, max_range_rate_threshold);
      return range_rate_thr;
   }
}
