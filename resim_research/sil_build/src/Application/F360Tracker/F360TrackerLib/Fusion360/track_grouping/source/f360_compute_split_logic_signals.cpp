/*===========================================================================*\
* FILE: f360_compute_split_logic_signals.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Compute_Split_Logic_Signals()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_compute_split_logic_signals.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_math_func.h"
#include "f360_math.h"
#include <algorithm>

namespace f360_variant_A
{
   static void Convert_Dets_To_TCS(
      const F360_Object_Track_T& obj,
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      float32_t(&orth_pos)[MAX_DETS_IN_OBJ_TRK]);

   /*===========================================================================*\
   * FUNCTION: Compute_Split_Logic_Signals()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Calibrations_T& calibs
   * const F360_Tracker_Info_T& tracker_info,
   * const float32_t dist_to_rear_axle
   * F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
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
   * Main function call to compute signals for split logic. Computes 
   * information that is common for all signals. 
   *
   * Each signal consists of both a raw/instantaneous signal and a low-pass 
   * filtered signal. If there are not enough detections to compute the signal, 
   * the raw signal will be set to -1 whereas the low-passed signal will retain 
   * its value.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_Split_Logic_Signals(
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      const float32_t dist_to_rear_axle,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]) 
   {

      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;

         F360_Object_Track_T& current_object = objects[obj_idx];

         if ((F360_TRACKER_TRKFLTR_CTCA == current_object.trk_fltr_type) || 
            (F360_TRACKER_TRKFLTR_CCA == current_object.trk_fltr_type))
         {
            const F360_Object_Orth_Split_Signals_Status_Type_T split_signals_status = Derive_Object_Orth_Split_Signal_Status(calibs, current_object, dist_to_rear_axle);
            if (F360_RESET_SPLIT_SIGNALS == split_signals_status)
            {
               current_object.orth_delta_filtered = 0.0F;
               current_object.orth_gap_filtered = 0.0F;
               current_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 0.0F;
               current_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 0.0F;
               current_object.filtered_rr_err_max_gap = 0.0F;
            }
            else if (F360_UPDATE_SPLIT_SIGNALS == split_signals_status)
            {
               float32_t orth_pos[MAX_DETS_IN_OBJ_TRK] = {};
               Convert_Dets_To_TCS(current_object, det_p, orth_pos);

               // Sort detections orthogonal position in TCS
               uint32_t orth_perm[MAX_DETS_IN_OBJ_TRK] = {};
               (void)F360_Sort(current_object.ndets, true, orth_pos, orth_perm);

               // If object is undersegmented and is moving cross-radially, the expected range-rate from both objects are 0, thus the signal should not be use to determine split in such case
               if (Is_Moving_In_Radial_Direction(current_object.vcs_position, current_object.vcs_velocity))
               {
                  Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(det_p, sensors, detections, current_object);
                  Compute_And_Filter_Objects_Detections_Range_Rate_Diff(det_p, calibs, orth_pos, orth_perm, current_object);
               }
               else
               {
                  // Apply forgetting factor to rr error based split signals
                  Apply_Forgetting_Factor_To_RR_Error_Split_Signals(true, current_object);
               }
               
               Compute_And_Filter_Objects_Detections_Max_Delta(calibs.k_orth_split_orth_delta_filter_const, orth_pos, current_object);

               Compute_And_Filter_Objects_Detections_Max_Gap(calibs, orth_pos, current_object);
            }
            else
            {
               // orth split signals are frozen
               // rr error based split signals are updated with forgetting factor
               Apply_Forgetting_Factor_To_RR_Error_Split_Signals(true, current_object);
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Compute_And_Filter_Objects_Detections_Max_Delta()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t filter_constant
   * const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK]
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
   * Computes the maximum orthogonal delta of associated detections, 
   * i.e. the furthest distance between all detections associated to the object.
   *
   * PRECONDITIONS:
   * The positions in orth_sorted_pos are sorted in ascending order
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_And_Filter_Objects_Detections_Max_Delta(
      const float32_t filter_constant,
      const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
      F360_Object_Track_T& object)
   {
      const float32_t max_orth_delta = orth_sorted_pos[object.ndets - 1U] - orth_sorted_pos[0];

      object.orth_delta_filtered = F360_Low_Pass_Filter_First_Order(max_orth_delta, object.orth_delta_filtered, filter_constant);
   }

   /*===========================================================================*\
   * FUNCTION: Is_Moving_In_Radial_Direction()
   * ===========================================================================
   * RETURN VALUE:
   * bool.
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object
   * 
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
   * Determines whether object is moving in radial direction by checking how big is 
   * the radial velocity component compared to cross-adial one.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Moving_In_Radial_Direction(
       const Point vcs_position,
       const F360_VCS_Velocity_T vcs_velocity
   )
   {
       const float32_t obj_vcs_azimuth = F360_Atan2f(vcs_position.y, vcs_position.x);
       // Object Radial Velocity
       const float32_t obj_radial_vel = (vcs_velocity.longitudinal * F360_Cosf(obj_vcs_azimuth)) + (vcs_velocity.lateral * F360_Sinf(obj_vcs_azimuth));
       // Object Tangential Velocity
       const float32_t obj_tangential_vel = (-vcs_velocity.longitudinal * F360_Sinf(obj_vcs_azimuth)) + (vcs_velocity.lateral * F360_Cosf(obj_vcs_azimuth));

       const float32_t obj_radial_vel_sq = obj_radial_vel * obj_radial_vel;
       const float32_t obj_tangential_vel_sq = obj_tangential_vel * obj_tangential_vel;
       constexpr float32_t radial_factor = 0.25F;
       const bool f_moving_radial = obj_tangential_vel_sq < radial_factor * obj_radial_vel_sq;

       return f_moving_radial;
   }

   /*===========================================================================*\
      * FUNCTION: Compute_And_Filter_Objects_Detections_Range_Rate_Diff()
      * ===========================================================================
      * RETURN VALUE:
      * None.
      *
      * PARAMETERS:
      * const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      * const F360_Calibrations_T& calibs,
      * const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
      * uint32_t orth_perm[MAX_DETS_IN_OBJ_TRK]
      * F360_Object_Track_T& object)
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
      * Computes the difference between means of range rates of associated detections
      * on left and right side of an object
      *
      * PRECONDITIONS:
      * orth_sorted_pos must be sorted before call to this function.
      * orth_perm must provide permutation according to orth_sorted_pos 
      *
      * POSTCONDITIONS:
      * None
      *
      \*===========================================================================*/
   void Compute_And_Filter_Objects_Detections_Range_Rate_Diff(
       const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
       const F360_Calibrations_T& calibs,
       const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
       const uint32_t(&orth_perm)[MAX_DETS_IN_OBJ_TRK],
       F360_Object_Track_T& object)
   {

       float32_t range_rate_tcs_left = 0.0F;
       float32_t range_rate_tcs_right = 0.0F;
       uint32_t left_counter = 0U;
       uint32_t right_counter = 0U;

       for (uint32_t j = 0U; j < object.ndets; j++)
       {
           const uint32_t det_idx = object.detids[orth_perm[j]] - 1U;
           const float32_t range_rate = det_p[det_idx].range_rate_compensated;
           if ((F360_DETECTION_WHEELSPIN_TYPE_INVALID == det_p[det_idx].wheel_spin_type))
           {
               if (orth_sorted_pos[j] < 0.0F)
               {
                   left_counter++;
                   range_rate_tcs_left += range_rate;
               }
               else
               {
                   range_rate_tcs_right += range_rate;
                   right_counter++;
               }
           }
       }
       float32_t range_rate_diff = 0.0F;
       float32_t det_coeff = 0.0F;
       // if there are detections on each side, compute the mean and filter coefficient
       if ((left_counter > 0U) && (right_counter > 0U))
       {
           range_rate_tcs_left = range_rate_tcs_left / static_cast<float32_t>(left_counter);
           range_rate_tcs_right = range_rate_tcs_right / static_cast<float32_t>(right_counter);
           range_rate_diff = Clamp(range_rate_tcs_left - range_rate_tcs_right, -calibs.k_range_rate_diff_saturation_const, calibs.k_range_rate_diff_saturation_const);
           det_coeff = 2.0F * std::min(static_cast<float32_t>(left_counter), static_cast<float32_t>(right_counter)) / (static_cast<float32_t>(left_counter) + static_cast<float32_t>(right_counter)); // Note that det_coeff <= 1.0
       }
       constexpr float32_t k_range_rate_diff_filter_const = 0.4F;
       // May be positive or negative, depends whether range-rate is higher on the right or left side
       object.orth_range_rate_diff_filtered = F360_Low_Pass_Filter_First_Order(range_rate_diff, object.orth_range_rate_diff_filtered, det_coeff * k_range_rate_diff_filter_const);
   }


   /*===========================================================================*\
      * FUNCTION: Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins()
      * ===========================================================================
      * RETURN VALUE:
      * None.
      *
      * PARAMETERS:
      * const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      * const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
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
      * Computes filtered means of TCS y positions of detections divided into
      * higher and lower range rate error bins.
      * Only non wheelspin detections are taken into account.
      * Threshold for binning is determined based on max gap between range rate errors.
      * Predicted range rate for error computation is done without using the object heading rate.
      *
      * PRECONDITIONS:
      * None
      *
      * POSTCONDITIONS:
      * None
      *
      \*===========================================================================*/

   void Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object)
   {
      uint32_t valid_det_cnt = 0U;
      uint32_t valid_det_idxs[MAX_DETS_IN_OBJ_TRK]{};
      float32_t object_valid_det_rr_error[MAX_DETS_IN_OBJ_TRK]{};

      for (uint32_t i = 0U; i < object.ndets; i++)
      {
         const uint32_t det_idx = object.detids[i] - 1U;
         // reject outliers
         if (F360_DETECTION_WHEELSPIN_TYPE_INVALID == det_p[det_idx].wheel_spin_type)
         {
            valid_det_idxs[valid_det_cnt] = det_idx;

            //since the object is suspected to be undersegmented, we compute the predicted range rate without using the object heading rate (as in the CCA model)
            const float32_t relative_velocity_x = object.vcs_velocity.longitudinal - sensors[detections[det_idx].raw.sensor_id - 1].variable.vcs_velocity.longitudinal;
            const float32_t relative_velocity_y = object.vcs_velocity.lateral - sensors[detections[det_idx].raw.sensor_id - 1].variable.vcs_velocity.lateral;
            const float32_t predicted_range_rate = relative_velocity_x * detections[det_idx].processed.cos_vcs_az + relative_velocity_y * detections[det_idx].processed.sin_vcs_az;

            object_valid_det_rr_error[valid_det_cnt] = det_p[det_idx].range_rate_dealiased - predicted_range_rate;
            valid_det_cnt++;
         }
      }

      if (valid_det_cnt > 1U)
      {
         // sort dealiased detections in object
         uint32_t rr_perm[MAX_DETS_IN_OBJ_TRK] = {};
         (void)F360_Sort(valid_det_cnt, true, object_valid_det_rr_error, rr_perm);
         float32_t gap_rr_err_thresh = 0.0F;
         // compute max compensated range rate gap and separating threshold for binning
         Compute_Filtered_RR_Err_Max_Gap_And_Threshold(object_valid_det_rr_error, valid_det_cnt, object, gap_rr_err_thresh);

         const float32_t min_gap_threshold_for_updating_mean_y_pos = 0.6F;
         if (object.filtered_rr_err_max_gap > min_gap_threshold_for_updating_mean_y_pos)
         {

            int32_t cnt_lower_bin = 0;
            int32_t cnt_higher_bin = 0;

            float32_t sum_lower_rr_err_bin_tcs_y_pos = 0.0F;
            float32_t sum_higher_rr_err_bin_tcs_y_pos = 0.0F;

            float32_t tcs_x = 0.0F; // tcs_x is not used, but needed for conversion function
            float32_t tcs_y = 0.0F;

            // divide detections into two bins based on range rate error (below and above the gap) 
            // and compute sum of y tcs positions of the detections in those bins
            for (uint32_t i = 0U; i < valid_det_cnt; i++)
            {
               const uint32_t det_idx = valid_det_idxs[i];
               Convert_VCS_Posn_To_TCS_Posn(
                  det_p[det_idx].vcs_position.x,
                  det_p[det_idx].vcs_position.y,
                  object.bbox.Get_Center().x,
                  object.bbox.Get_Center().y,
                  object.bbox.Get_Orientation(),
                  tcs_x,
                  tcs_y);

               if (object_valid_det_rr_error[rr_perm[i]] <= gap_rr_err_thresh)
               {
                  sum_lower_rr_err_bin_tcs_y_pos += tcs_y;
                  cnt_lower_bin++;
               }
               else
               {
                  sum_higher_rr_err_bin_tcs_y_pos += tcs_y;
                  cnt_higher_bin++;
               }
            }

            // compute filtered mean of y tcs positions of detections for each bin
            const float32_t hist_avg_filtered_pos_y = (object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin + object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin) * 0.5F;

            float32_t lower_rr_err_bin_y_pos_mean = 0.0F;
            float32_t higher_rr_err_bin_y_pos_mean = 0.0F;

            lower_rr_err_bin_y_pos_mean = cnt_lower_bin > 0 ? (sum_lower_rr_err_bin_tcs_y_pos / static_cast<float32_t>(cnt_lower_bin)) : hist_avg_filtered_pos_y;
            higher_rr_err_bin_y_pos_mean = cnt_higher_bin > 0 ? (sum_higher_rr_err_bin_tcs_y_pos / static_cast<float32_t>(cnt_higher_bin)) : hist_avg_filtered_pos_y;

            const float32_t lp_filter_coefficient = 0.3F;
            object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = F360_Low_Pass_Filter_First_Order(lower_rr_err_bin_y_pos_mean, object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, lp_filter_coefficient);
            object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = F360_Low_Pass_Filter_First_Order(higher_rr_err_bin_y_pos_mean, object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, lp_filter_coefficient);
         }
         else
         {
            // if gap is too small, apply forgetting factor to mean y positions of bins, but not to rr max gap
            Apply_Forgetting_Factor_To_RR_Error_Split_Signals(false, object);
         }
      }
      else
      {
         // if there are less than 2 valid detections apply forgetting factor to all rr error based split signals
         Apply_Forgetting_Factor_To_RR_Error_Split_Signals(true, object);
      }
   }
   /*===========================================================================*\
   * FUNCTION: Derive_Object_Orth_Split_Signal_Status()
   * ===========================================================================
   * RETURN VALUE:
   * F360_Object_Orth_Split_Signals_Status_Type_T orth_delta_status
   *
   * PARAMETERS:
   *  const F360_Calibrations_T& calibs,
   *  const F360_Object_Track_T& object,
   *  const float32_t dist_to_rear_axle
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
   * Derives whether we should reset, freeze or update an objects
   * orthogonal split related signals
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   F360_Object_Orth_Split_Signals_Status_Type_T Derive_Object_Orth_Split_Signal_Status(
      const F360_Calibrations_T& calibs,
      const F360_Object_Track_T& object,
      const float32_t dist_to_rear_axle)
   {
      const float32_t half_host_length = 0.6F * dist_to_rear_axle;
      const float32_t obj_long_pos_host_center = object.vcs_position.x + half_host_length;

      const float32_t dist_sq = F360_Get_Hypotenuse_Squared(obj_long_pos_host_center, object.vcs_position.y);

      F360_Object_Orth_Split_Signals_Status_Type_T orth_delta_status;
      if (dist_sq > calibs.k_orth_split_max_distance_sq)
      {
         orth_delta_status = F360_RESET_SPLIT_SIGNALS;
      }
      else if ((object.ndets < 2U) || (dist_sq < calibs.k_orth_split_min_distance_sq))
      {
         orth_delta_status = F360_DONT_INNOVATE_SPLIT_SIGNALS;
      }
      else
      {
         orth_delta_status = F360_UPDATE_SPLIT_SIGNALS;
      }

      return orth_delta_status;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_And_Filter_Objects_Detections_Max_Gap()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibs,
   * const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
   * F360_Object_Track_T& object)
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
   * Computes the maximum orthogonal gap between all associated
   * detections of an object, i.e. the furthest distance between two
   * detections of an object
   *
   * PRECONDITIONS:
   * orth_sorted_pos must be sorted sorted before call to this function.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_And_Filter_Objects_Detections_Max_Gap(
      const F360_Calibrations_T& calibs,
      const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
      F360_Object_Track_T& object)
   {
      // Compute all gaps in TCS
      float32_t orth_gaps[MAX_DETS_IN_OBJ_TRK - 1U] = {};
      for (uint32_t j = 0U; j < (object.ndets - 1U); j++)
      {
         orth_gaps[j] = orth_sorted_pos[j + 1U] - orth_sorted_pos[j];
      }
      const uint32_t saturated_ndets = std::min(object.ndets, calibs.k_orth_split_orth_gap_filter_max_dets);
      const float32_t filter_constant = calibs.k_orth_split_orth_gap_filter_prop_const * static_cast<float32_t>(saturated_ndets);
      const float32_t max_orth_gap = F360_Max_Element(orth_gaps, object.ndets - 1U);
      object.orth_gap_filtered = F360_Low_Pass_Filter_First_Order(max_orth_gap, object.orth_gap_filtered, filter_constant);
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Filtered_RR_Max_Gap_And_Threshold()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t(&sorted_rr_err)[MAX_DETS_IN_OBJ_TRK],
   * const uint32_t& valid_det_cnt,
   * F360_Object_Track_T& object,
   * float32_t& gap_rr_thresh)
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
   * Computes the filtered maximum compensated range rate gap between all associated
   * detections of an object, i.e. the biggest range rate difference between two
   * detections of an object.
   *
   * PRECONDITIONS:
   * sorted_rr_err must be sorted sorted before call to this function.
   * valid_det_cnt must be > 1
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_Filtered_RR_Err_Max_Gap_And_Threshold(
      const float32_t(&sorted_rr_err)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t& valid_det_cnt,
      F360_Object_Track_T& object,
      float32_t& gap_rr_thresh)
   {
      uint32_t max_gap_element_index = 0U;
      // Compute all gaps in TCS
      float32_t rr_gaps[MAX_DETS_IN_OBJ_TRK - 1U] = {};
      for (uint32_t i = 0U; i < (valid_det_cnt - 1U); i++)
      {
         rr_gaps[i] = sorted_rr_err[i + 1U] - sorted_rr_err[i];
         if (rr_gaps[i] > rr_gaps[max_gap_element_index])
         {
            max_gap_element_index = i;
         }
      }
      gap_rr_thresh = (sorted_rr_err[max_gap_element_index] + sorted_rr_err[max_gap_element_index + 1U]) * 0.5F;

      const float32_t filter_constant = 0.25F;
      const float32_t max_rr_gap = rr_gaps[max_gap_element_index];
      object.filtered_rr_err_max_gap = F360_Low_Pass_Filter_First_Order(max_rr_gap, object.filtered_rr_err_max_gap, filter_constant);
   }

   /*===========================================================================*\
   * FUNCTION: Convert_Dets_To_TCS()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const F360_Object_Track_T& obj,
   *  const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
   *  float32_t(&orth_pos)[MAX_DETS_IN_OBJ_TRK]
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
   * Transforms an objects associated detections in to TCS system and
   * populates the orthogonal position in orth_pos array
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Convert_Dets_To_TCS(
      const F360_Object_Track_T& obj,
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      float32_t(&orth_pos)[MAX_DETS_IN_OBJ_TRK])
   {
      for (uint32_t j = 0U; j < obj.ndets; j++)
      {
         const uint32_t det_idx = obj.detids[j] - 1U;

         Point det_pos_tcs = {};
         Convert_VCS_Posn_To_TCS_Posn(
            det_p[det_idx].vcs_position.x,
            det_p[det_idx].vcs_position.y,
            obj.bbox.Get_Center().x,
            obj.bbox.Get_Center().y,
            obj.bbox.Get_Orientation(),
            det_pos_tcs.x,
            det_pos_tcs.y);

         orth_pos[j] = det_pos_tcs.y;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Apply_Forgetting_Factor_To_RR_Error_Split_Signals()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const bool f_update_rr_gap - If true, apply forgetting factor to filtered_rr_err_max_gap
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
   * Applies forgetting factor to rr error based split-related filtered signals when the
   * signals cannot be reliably updated (e.g., insufficient detections, object
   * moving cross-radially, or gap too small).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Apply_Forgetting_Factor_To_RR_Error_Split_Signals(const bool f_update_rr_gap, F360_Object_Track_T& object)
   {
      constexpr float32_t forgetting_factor = 0.50F;
      object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin *= forgetting_factor;
      object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin *= forgetting_factor;
      if (f_update_rr_gap)
      {
         object.filtered_rr_err_max_gap *= forgetting_factor;
      }
   }
}

