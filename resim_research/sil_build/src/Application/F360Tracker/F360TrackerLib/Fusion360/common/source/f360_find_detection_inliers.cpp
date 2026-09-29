/*===================================================================================*\
* FILE: f360_find_detection_inliers.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Find_Detection_Inliers().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

/******************************
 * Includes
 *******************************/

#include "f360_find_detection_inliers.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_detection_wheelspin_type.h"
#include "f360_calculate_curvi_position.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include <numeric>

namespace f360_variant_A
{
   /*===========================================================================*\
     * FUNCTION: Find_Detection_Inliers()
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     * const F360_Host_T& host,
     * const F360_Tracker_Info_T& tracker_info,
     * const F360_Calibrations_T& calibrations,
     * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
     * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
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
     * Wrapper function to call detection downselect for each object individually.
     *
     * PRECONDITIONS:
     *
     * POSTCONDITIONS:
     * None
     \*===========================================================================*/
   void Find_Detection_Inliers(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibrations,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (int32_t i_obj = 0; i_obj < tracker_info.num_active_objs; i_obj++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i_obj] - 1;
         F360_Object_Track_T& object = object_tracks[obj_idx];

         Find_Detection_Inliers_For_Single_Object(host, calibrations, object, det_props);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Find_Detection_Inliers_For_Single_Object()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Calibrations_T& calibrations,
   * F360_Object_Track_T& object,
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Function to downselect detections associated to an existing track. This is done by
   * comparing the detection's compensated range rate with its predicted range rate (track
   * velocity projected onto the detection radial) and selecting detections where
   * the difference is under a certain threshold, based on current and historic detection information.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Find_Detection_Inliers_For_Single_Object(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibrations,
      F360_Object_Track_T& object,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      object.num_rr_inlier_dets = 0U;

      uint32_t valid_det_indices[MAX_DETS_IN_OBJ_TRK];
      uint32_t n_valid_dets = 0U;
      Find_Valid_Detections(object, det_props, valid_det_indices, n_valid_dets);

      float32_t abs_rdot_diffs[MAX_DETS_IN_OBJ_TRK];
      Calculate_Abs_Range_Rate_Diffs(det_props, valid_det_indices, n_valid_dets, abs_rdot_diffs);

      Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

      Select_Detection_Inliers(host, calibrations, valid_det_indices, n_valid_dets, abs_rdot_diffs, object, det_props);
   }

   /*===========================================================================*\
   * FUNCTION: Find_Valid_Detections()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track - Object track to find valid detections for
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS] - Detection properties
   * uint32_t (&valid_det_indices)[MAX_DETS_IN_OBJ_TRK] - Array to fill with indicies for valid detections
   * uint32_t & n_valid_dets - Number of valid detections that were found
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
   * Function to find valid detections as candidates to be downselected to track.
   * Valid detections are detections that are not flagged as wheel spin, close target or
   * azimuth error detections and are within a distance, from the reference point,
   * defined in the function based on
   * the length of the object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Find_Valid_Detections(
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t(&valid_det_indices)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& n_valid_dets)
   {
       float32_t long_obj_len_thr = 7.0F;

       const bool f_only_consider_dets_close_to_ref_pnt = ((object_track.bbox.Get_Length() > long_obj_len_thr)
           && ((F360_REFERENCE_POINT_REAR == object_track.reference_point)
           || (F360_REFERENCE_POINT_REAR_RIGHT == object_track.reference_point)
           || (F360_REFERENCE_POINT_REAR_LEFT == object_track.reference_point)
           ));

       if ((object_track.ndets > 0U) && f_only_consider_dets_close_to_ref_pnt)
       {
           // Declare usefull variables that are needed later
 
           const float32_t tcs_mid_x_object = object_track.bbox.Get_Length() * 0.5F; // Represents the middle point of the object in TCS at x axis.
           uint32_t number_top_half_dets = 0U; // Number of detections that are located in the top half, in x axis, of the object

           // Find the total number of detections that are located in top half of the object
           for (uint32_t i = 0U; i < object_track.ndets; i++)
           {
               const uint32_t det_idx = object_track.detids[i] - 1U;
               const F360_Detection_Props_T& det_prop = det_props[det_idx];
               float32_t dets_tcs_x;
               float32_t dets_tcs_y = 0.0F; // Not used in the code, its necessary for Convert_VCS_Posn_To_TCS_Posn though. Equal to 0 to not throw MISRA warning

               // Convert the detections to TCS from VCS      
               Convert_VCS_Posn_To_TCS_Posn(det_prop.vcs_position.x, det_prop.vcs_position.y, object_track.vcs_position.x, object_track.vcs_position.y,
                   object_track.bbox.Get_Orientation(), dets_tcs_x, dets_tcs_y);

               if (dets_tcs_x > tcs_mid_x_object)
               {
                   number_top_half_dets++;
               }
           }
           
           /* This application, regarding the top half of the object, applies only if we take into account
           objects that are in front of the host, based on their reference point, which happen using
           the boolean f_only_consider_dets_close_to_ref_pnt. If we include other references points
           the below application which was developed in ICV-41 will become a bug.
           */
           if ((static_cast<float32_t>(number_top_half_dets) / static_cast<float32_t>(object_track.ndets) > 0.5F))
           {
               const float32_t difference_mul_by_factor = (object_track.bbox.Get_Length() - long_obj_len_thr) * 0.3F; // No need of absolute value since we will use long_obj_len_thr only if its > 7
               long_obj_len_thr += difference_mul_by_factor;
           }
           else
           {
               // Leave long_obj_len_thr unaffected and dont increase it
           }
       }

       for (uint32_t i = 0U; i < object_track.ndets; i++)
       {
           const uint32_t det_idx = object_track.detids[i] - 1U;
           const F360_Detection_Props_T& det_prop = det_props[det_idx];
           bool f_det_within_max_dist;

           if (f_only_consider_dets_close_to_ref_pnt)
           {
               const float32_t sq_dist_thr = long_obj_len_thr * long_obj_len_thr;
               const float32_t dx = det_prop.vcs_position.x - object_track.vcs_position.x;
               const float32_t dy = det_prop.vcs_position.y - object_track.vcs_position.y;
               const float32_t sq_dist_det_to_ref_pnt = (dx * dx + dy * dy);
               f_det_within_max_dist = sq_dist_det_to_ref_pnt < sq_dist_thr;
           }
           else
           {
               f_det_within_max_dist = true;
           }

           if ((!det_prop.f_close_target)
               && (!det_prop.f_azimuth_rdot_outlier)
               && (F360_DETECTION_WHEELSPIN_TYPE_INVALID == det_prop.wheel_spin_type)
               && f_det_within_max_dist)
           {
               valid_det_indices[n_valid_dets] = det_idx;
               n_valid_dets++;
           }
       }
   }

   /*===========================================================================*\
   * FUNCTION: Select_Detection_Inliers()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Calibrations_T& calibrations,
   * const uint32_t(&valid_det_indices)[MAX_DETS_IN_OBJ_TRK],
   * const uint32_t n_valid_dets,
   * float32_t (&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK],
   * F360_Object_Track_T& object_track,
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
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
   * Selects and marks detections as inliers based on the detections' range rate errors by
   * checking if they're below a threshold that is calculated using current and historic range rate
   * errors.
   *
   * PRECONDITIONS:
   * Historic filtered mean and variance has been updated. Special case:
   * - If the object is newly initialized and has a single valid detection, variance is 0.
   *   So the detection will not be selected if it's range rate error is above k_min_range_rate_error_threshold.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Select_Detection_Inliers(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibrations,
      const uint32_t(&valid_det_indices)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t n_valid_dets,
      float32_t (&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK],
      F360_Object_Track_T& object_track,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      if (n_valid_dets > 0U)
      {
         // Calculate standard deviation of range rate errors with negative values protection. 
         float32_t filtered_hist_assoc_det_rr_err_std = 0.0F;
         if (object_track.filtered_hist_assoc_det_rr_err_var > 0.0F)
         {
            filtered_hist_assoc_det_rr_err_std = F360_Sqrtf(object_track.filtered_hist_assoc_det_rr_err_var);
         }
         else
         {
            filtered_hist_assoc_det_rr_err_std = 0.0F;
         }

         float32_t range_rate_error_threshold = std::max(object_track.filtered_hist_assoc_det_rr_err_mean + filtered_hist_assoc_det_rr_err_std, calibrations.k_min_range_rate_error_threshold);

         // Make easier for detections to be inlier for objects that are stationary and downselected
         if ((object_track.movable_prob < 0.5F) && (object_track.reduced_id > 0))
         {
             // Create standard deviation multiplier for calculating range rate error threshold
             constexpr float32_t std_dev_multiplier = 1.17F;

             const bool f_is_obj_in_roi = host.speed > 5.0F ? Is_Obj_In_Curvi_Roi(host, object_track) : Is_Obj_In_Rect_Roi(object_track);
             
             range_rate_error_threshold = f_is_obj_in_roi ? std::max(object_track.filtered_hist_assoc_det_rr_err_mean + std_dev_multiplier * filtered_hist_assoc_det_rr_err_std, calibrations.k_min_range_rate_error_threshold) : range_rate_error_threshold;
         }

         for (uint32_t i = 0U; i < n_valid_dets; i++)
         {
            if (abs_rdot_diffs[i] < range_rate_error_threshold)
            {
               const uint32_t det_idx = valid_det_indices[i];
               det_props[det_idx].f_rr_inlier = true;
               object_track.num_rr_inlier_dets++;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Obj_In_Curvi_Roi
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * F360_Object_Track_T& object_track,
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
   * Function checks if object is in the declared curvatured region of interest
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Obj_In_Curvi_Roi(
       const F360_Host_T& host,
       const F360_Object_Track_T& object_track)
   {
       constexpr float32_t max_vcs_long_pos_threshold = 40.0F;
       constexpr float32_t min_vcs_long_pos_threshold = -20.0F;
       constexpr float32_t vcs_lat_pos_threshold = 3.0F;

       const float32_t obj_lat_pos = Calculate_Curvi_Lat_Pos(host, object_track.vcs_position.x, object_track.vcs_position.y);

       const bool f_in_longzone = (object_track.vcs_position.x < max_vcs_long_pos_threshold) && (object_track.vcs_position.x > min_vcs_long_pos_threshold);
       const bool f_in_latzone = std::abs(obj_lat_pos) < vcs_lat_pos_threshold;
       return f_in_latzone && f_in_longzone;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Obj_In_Rect_Roi
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * F360_Object_Track_T& object_track,
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
   * Function checks if object is in the declared curvatured region of interest
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Obj_In_Rect_Roi(
       const F360_Object_Track_T& object_track)
   {
       constexpr float32_t max_vcs_long_pos_threshold = 20.0F;
       constexpr float32_t min_vcs_long_pos_threshold = -10.0F;
       constexpr float32_t vcs_lat_pos_threshold = 5.0F;

       const bool f_in_longzone = (object_track.vcs_position.x < max_vcs_long_pos_threshold) && (object_track.vcs_position.x > min_vcs_long_pos_threshold);
       const bool f_in_latzone = std::abs(object_track.vcs_position.y) < vcs_lat_pos_threshold;
       return f_in_latzone && f_in_longzone;
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Abs_Range_Rate_Diffs()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS] - Detection properties
   * uint32_t (&valid_det_indices)[MAX_DETS_IN_OBJ_TRK] - Valid detection indicies
   * uint32_t & n_valid_dets - Number of valid detections
   * float32_t (&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK] - Difference between predicted and dealiased range rate for valid detections
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
   * Function to calculate difference between predicted and dealiased range rate
   * for all valid detections.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Calculate_Abs_Range_Rate_Diffs(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&valid_det_indices)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t n_valid_dets,
      float32_t(&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK])
   {
      for (uint32_t i = 0U; i < n_valid_dets; i++)
      {
         const uint32_t det_idx = valid_det_indices[i];

         const float32_t predicted_rdot = det_props[det_idx].range_rate_predicted;
         const float32_t det_rdot = det_props[det_idx].range_rate_dealiased; // Detection should always be dealiased at this stage since it is associated to an object
         abs_rdot_diffs[i] = std::abs(predicted_rdot - det_rdot);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Update_Historic_Range_Rate_Error_Statistics()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibrations
   * const float32_t(&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK]
   * const uint32_t n_valid_dets
   * F360_Object_Track_T& object_track
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
   * Calculates the mean and variance of range rate errors for the current scan using the historically
   * calculated mean and variance. The forgetting factor determines how much the errors from the current
   * scan impacts the result compared to the historical. For more details on the calculations, see attached
   * pdf in JIRA ticket DEX-3200.
   *
   * PRECONDITIONS:
   * abs_rdot_diffs are the absolute range rate errors.
   *
   * POSTCONDITIONS:
   * For small historical number of associated detections, mean and variance can not be computed. This leads to a few special cases that are handeled. 
   * See SDD document for a summary of these special cases.
   * 
   \*===========================================================================*/
   void Update_Historic_Range_Rate_Error_Statistics(
      const F360_Calibrations_T& calibrations,
      const float32_t(&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t n_valid_dets,
      F360_Object_Track_T& object_track)
   {
      const float32_t forgetting_factor = calibrations.k_rr_error_statistics_forgetting_factor;
      const float32_t historic_mean = object_track.filtered_hist_assoc_det_rr_err_mean;
      const float32_t historic_variance = object_track.filtered_hist_assoc_det_rr_err_var;
      const float32_t historic_n_dets = object_track.filtered_hist_assoc_n_dets;

      // Calculate help variables that are used to update the historic mean and variance of range rate errors
      float32_t rr_err_sum = 0.0F;
      float32_t rr_err_squared_sum = 0.0F;
      for (uint32_t i = 0U; i < n_valid_dets; i++)
      {
         rr_err_sum += abs_rdot_diffs[i];
         rr_err_squared_sum += abs_rdot_diffs[i] * abs_rdot_diffs[i];
      }

      // Update the number of historic detections (s1) with detection information from current scan. If there are no valid current detections, let it decrease with the forgetting factor.
      const float32_t s1 = forgetting_factor * historic_n_dets + static_cast<float32_t>(n_valid_dets);
      const float32_t s2 = forgetting_factor * historic_n_dets * historic_mean + rr_err_sum;
      // Limit the effect of historical information.
      const float32_t max_filtered_historic_dets = ((object_track.movable_prob > 0.5F) ? calibrations.k_max_number_of_historic_dets_obj_movable : calibrations.k_max_number_of_historic_dets_obj_non_movable);
      object_track.filtered_hist_assoc_n_dets = std::min(s1, max_filtered_historic_dets);

      const float32_t sample_mean = (s1 < F360_EPSILON) ? 0.0F : (s2 / s1);
      object_track.filtered_hist_assoc_det_rr_err_mean = sample_mean;

      // If s1 (similar to the number of detections) is less than 2, the variance from previous iteration is kept.
      if (s1 > 2.0F)
      {
         const float32_t hist_n_forget = forgetting_factor * historic_n_dets;
         const float32_t squared_sample_mean = sample_mean * sample_mean;
         const float32_t historic_squared_mean = historic_mean * historic_mean;
         // Make sure number of historic detections from previous iteration is bigger than 1. Otherwise variance won't be valid, and will be set to 0.
         const float32_t hist_squared_sample = (hist_n_forget > 1.0F) ? historic_variance * (hist_n_forget - 1.0F) + historic_squared_mean * hist_n_forget : 0.0F;
         const float32_t sample_variance = (hist_squared_sample + rr_err_squared_sum - squared_sample_mean * s1) / (s1 - 1.0F);
         object_track.filtered_hist_assoc_det_rr_err_var = F360_Saturate(sample_variance, 0.0F, calibrations.k_max_historic_rr_error_variance);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Clear_Detection_Inliers_To_Object()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T &object_track
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Reset f_rr_inlier flags in all detections associated to the object
   * and number of reduced detections counter in the object
   *
   * PRECONDITIONS:
   * All input references should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Clear_Detection_Inliers_To_Object(
      F360_Object_Track_T& object_track,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      object_track.num_rr_inlier_dets = 0U;
      for (uint32_t det_slot_idx = 0U; det_slot_idx < object_track.ndets; det_slot_idx++)
      {
         const uint32_t det_id = object_track.detids[det_slot_idx];
         F360_Detection_Props_T& det_p = (det_props[det_id - 1U]);
         det_p.f_rr_inlier = false;
      }
   }
}

