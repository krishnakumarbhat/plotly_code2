/******************************************************************************
 * Copyright 2024 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include <cstring>
#include <cmath>
#include "f360_cvt_generate_measurement.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================
    * FUNCTION: Calc_Range_Rate_Gate
    *===========================================================================
    * DESCRIPTION:
    * Calculate the range-rate gate for detections that could come from trailer.
    *=========================================================================*/
   float32_t Calc_Range_Rate_Gate(
      const float32_t host_yaw_rate,
      const bool f_reversing)
   {
      const float32_t abs_yaw_rate = fabsf(host_yaw_rate);
      constexpr float32_t max_abs_yaw_rate_for_interpolation = F360_DEG2RAD(15.0F);
      float32_t range_rate_gate;
      if (f_reversing)
      {
         const float32_t range_rate_tresh_lower_bound_rev = 0.3F;
         const float32_t range_rate_tresh_higher_bound_rev = 0.75F;
         range_rate_gate = F360_Linear_Equation_With_Saturation(abs_yaw_rate, 0.0F, max_abs_yaw_rate_for_interpolation, range_rate_tresh_lower_bound_rev, range_rate_tresh_higher_bound_rev);
      }
      else
      {
         const float32_t range_rate_tresh_lower_bound = 0.6F;
         const float32_t range_rate_tresh_higher_bound = 1.5F;
         range_rate_gate = F360_Linear_Equation_With_Saturation(abs_yaw_rate, 0.0F, max_abs_yaw_rate_for_interpolation, range_rate_tresh_lower_bound, range_rate_tresh_higher_bound);
      }

      return range_rate_gate;
   }

   /*===========================================================================
    * FUNCTION: Find_Valid_Detections
    *===========================================================================
    * DESCRIPTION:
    * Find detections that are valid for line detection.
    *=========================================================================*/
   void Find_Valid_Detections(
      const F360_CVT_Input_Data_T& cvt_input,
      const F360_CVT_State_T& cvt_state,
      const float32_t range_rate_gate,
      int32_t(&valid_det_idx)[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER],
      int32_t& counter_valid_det)
   {
      const float32_t radar_vcs_longpos = cvt_state.radar_vcs_longpos;
      const float32_t radar_vcs_latpos = cvt_state.radar_vcs_latpos;
      const float32_t host_length = cvt_state.host_length;
      const float32_t host_width = cvt_state.host_width;

      counter_valid_det = 0;
      if (cvt_input.host_speed > 2.0F)
      {
         for (int32_t i = 0; i < cvt_input.n_detections; i++)
         {
            const bool f_range_rate_gate_pass = fabsf(cvt_input.detections[i].range_rate) <= range_rate_gate;
            if (f_range_rate_gate_pass)
            {
               const float32_t lat_pos_det_vcs = cvt_input.detections[i].vcs_latpos;
               const float32_t lon_pos_det_vcs = cvt_input.detections[i].vcs_longpos;
               const float32_t lat_pos_det_in_radar = lat_pos_det_vcs - radar_vcs_latpos;
               const float32_t lon_pos_det_in_radar = lon_pos_det_vcs - radar_vcs_longpos;

               // flag for this detection to be in the predefined circle area
               const float32_t radius_sq = lon_pos_det_in_radar * lon_pos_det_in_radar + lat_pos_det_in_radar * lat_pos_det_in_radar;
               const float32_t threshold = 30.0F * 30.0F;
               const bool f_in_region = (radius_sq < threshold) && (lon_pos_det_in_radar < 0.0F);

               const bool in_host = (fabsf(lat_pos_det_vcs) <= (host_width * 0.5F)) && (lon_pos_det_vcs < 0.0F) && (lon_pos_det_vcs > -host_length);

               if (f_in_region && (!in_host))
               {
                  valid_det_idx[counter_valid_det] = i; // save the idx of valid detection
                  counter_valid_det++;

                  if (counter_valid_det == MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER)
                  {
                     break;
                  }
               }
            }
         }
      }
   }

   /*===========================================================================
    * FUNCTION: Find_Approximate_Lines
    *===========================================================================
    * DESCRIPTION:
    * Find up to two approximate lines in the detection data.
    *=========================================================================*/
   void Find_Approximate_Lines(
      const F360_CVT_Detection_Info_T(&detections)[CVT_MAX_NUMBER_OF_DETECTIONS],
      const float32_t radar_vcs_latpos,
      const int32_t n_valid_dets,
      const int32_t(&valid_det_idxs)[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER],
      Trailer_Measurement_Info_T& primary_measurement,
      Trailer_Measurement_Info_T& secondary_measurement)
   {
      const int32_t num_orth_bins = 50;
      const float32_t abs_max_orth = 25.0F;
      const float32_t bin_size = (2.0F * abs_max_orth) / static_cast<float32_t>(num_orth_bins);
      const int32_t num_rotations = 24;
      constexpr float32_t angle_step = F360_DEG2RAD(5.0F);

      const bool f_right_side_sensor = radar_vcs_latpos > 0.0F;
      const float32_t angle_sign = f_right_side_sensor ? 1.0F : -1.0F;
      const float32_t k_longpos_offset = 15.0F;

      float32_t angle_scores[num_rotations];
      float32_t line_slope[num_rotations];
      float32_t line_intersect[num_rotations];

      for (int32_t i = 0; i < num_rotations; i++)
      {
         // Rotate detections from hypothesis angle to align with vcs long axis. For a trailer with
         const int32_t step_n = i - 2;
         const float32_t cos_angle = F360_Cosf(angle_sign * angle_step * static_cast<float32_t>(step_n));
         const float32_t sin_angle = F360_Sinf(angle_sign * angle_step * static_cast<float32_t>(step_n));
         float32_t orth_grid[num_orth_bins] = {};

         for (int32_t j = 0; j < n_valid_dets; j++)
         {
            const int32_t det_idx = valid_det_idxs[j];
            const float32_t det_longpos = detections[det_idx].vcs_longpos;
            const float32_t det_latpos = detections[det_idx].vcs_latpos;
            const float32_t det_orth = (det_longpos + k_longpos_offset) * sin_angle + det_latpos * cos_angle;
            const float32_t bin_val = (det_orth + abs_max_orth) / bin_size;
            const int32_t bin_idx = static_cast<int32_t>(bin_val);

            if ((bin_idx >= 0) && (bin_idx < num_orth_bins))
            {
               orth_grid[bin_idx] += 1.0F;
            }
         }

         float32_t angle_candidate_score = 0.0F;
         float32_t angle_candidate_slope = 0.0F;
         float32_t angle_candidate_intersect = 0.0F;

         for (int32_t j = 1; j < num_orth_bins - 1; j++)
         {
            const float32_t sum_score = orth_grid[j] + 0.15F * orth_grid[j - 1] + 0.15F * orth_grid[j + 1];
            if (sum_score > angle_candidate_score)
            {
               angle_candidate_score = sum_score;
               if (fabsf(sin_angle) > 0.001F)
               {
                  angle_candidate_slope = cos_angle / -sin_angle;
                  const float32_t orth_pos = ((static_cast<float32_t>(j) + 0.5F) * bin_size - abs_max_orth); // recover the orthogonal position from the bins
                  angle_candidate_intersect = orth_pos / sin_angle - k_longpos_offset;
               }
               else
               {
                  // Handle vertical line
                  angle_candidate_slope = 10000.0F;
                  angle_candidate_intersect = 0.0F;
               }
            }
         }

         angle_scores[i] = angle_candidate_score;
         line_slope[i] = angle_candidate_slope;
         line_intersect[i] = angle_candidate_intersect;
      }

      const float32_t k_validity_threshold = 6.0F;
      bool idx1_valid = false;
      bool idx2_valid = false;
      int32_t candidate_idx1 = 0;
      int32_t candidate_idx2 = 0;
      for (int32_t i = 2; i < num_rotations - 2; i++)
      {
         // look for local max
         if ((angle_scores[i] > k_validity_threshold) &&
            (angle_scores[i] > angle_scores[candidate_idx1]) &&
            (angle_scores[i] >= angle_scores[i - 1]) && (angle_scores[i] >= angle_scores[i + 1]) &&
            (angle_scores[i] > angle_scores[i - 2]) && (angle_scores[i] > angle_scores[i + 2]))
         {
            candidate_idx1 = i;
            idx1_valid = true;
         }
      }

      for (int32_t i = 2; i < num_rotations - 2; i++)
      {
         // look for 2nd local max
         const int32_t candidate_dist = candidate_idx1 - i;
         if ((angle_scores[i] > k_validity_threshold) &&
            ((candidate_dist > 2) || (candidate_dist < -2)) &&
            (angle_scores[i] > angle_scores[candidate_idx2]) &&
            (angle_scores[i] >= angle_scores[i - 1]) && (angle_scores[i] >= angle_scores[i + 1]) &&
            (angle_scores[i] > angle_scores[i - 2]) && (angle_scores[i] > angle_scores[i + 2]))
         {
            candidate_idx2 = i;
            idx2_valid = true;
         }
      }

      if (idx1_valid && idx2_valid)
      {
         if (fabsf(line_slope[candidate_idx1]) < fabsf(line_slope[candidate_idx2]))
         {
            const int32_t temp = candidate_idx1;
            candidate_idx1 = candidate_idx2;
            candidate_idx2 = temp;
         }

         primary_measurement.f_line_valid = true;
         primary_measurement.line_slope = line_slope[candidate_idx2];
         primary_measurement.line_intersect = line_intersect[candidate_idx2];
         secondary_measurement.f_line_valid = true;
         secondary_measurement.line_slope = line_slope[candidate_idx1];
         secondary_measurement.line_intersect = line_intersect[candidate_idx1];
      }
      else if (idx1_valid)
      {
         primary_measurement.f_line_valid = true;
         primary_measurement.line_slope = line_slope[candidate_idx1];
         primary_measurement.line_intersect = line_intersect[candidate_idx1];
         secondary_measurement.f_line_valid = false;
         secondary_measurement.line_slope = 0.0F;
         secondary_measurement.line_intersect = 0.0F;
      }
      else
      {
         primary_measurement.f_line_valid = false;
         secondary_measurement.f_line_valid = false;
      }
   }

   /*===========================================================================
    * FUNCTION: Generate_Measurement
    *===========================================================================
    * DESCRIPTION:
    * Generate the trailer measurements through line identification
    *=========================================================================*/
   void Generate_Measurement(
      const F360_CVT_Input_Data_T& cvt_input,
      F360_CVT_State_T& cvt_state)
   {
      (void)memset(cvt_state.valid_det_idx, 0, sizeof(cvt_state.valid_det_idx));
      cvt_state.n_valid_dets = 0;
      cvt_state.primary_measurement.f_msmt_valid = false;
      cvt_state.secondary_measurement.f_msmt_valid = false;

      const float32_t k_cvt_small_float = 1.0E-15F;  // Small float for comparing almost zero
      const bool f_reversing = cvt_input.host_speed < -k_cvt_small_float;
      const float32_t range_rate_gate = Calc_Range_Rate_Gate(cvt_input.host_yawrate, f_reversing);

      Find_Valid_Detections(cvt_input, cvt_state, range_rate_gate, cvt_state.valid_det_idx, cvt_state.n_valid_dets);

      Find_Approximate_Lines(cvt_input.detections, cvt_state.radar_vcs_latpos, cvt_state.n_valid_dets, cvt_state.valid_det_idx, cvt_state.primary_measurement, cvt_state.secondary_measurement);

      const float32_t k_trailer_orth_offset_min = 0.35F * 2.55F; // the minimum offset to the measured line of the trailer edge, to obtain central line of the trailer. Used if trailer angle is large.

      if (cvt_state.primary_measurement.f_line_valid)
      {
         const float32_t slope_sign = (cvt_state.primary_measurement.line_slope > 0.0F) ? 1.0F : -1.0F;
         cvt_state.primary_measurement.trailer_angle_vcs = F360_Atan2f(slope_sign, fabsf(cvt_state.primary_measurement.line_slope));
         if (fabsf(cvt_state.primary_measurement.trailer_angle_vcs) > 0.001F)
         {
            const float32_t k_trailer_orth_offset = F360_Linear_Equation_With_Saturation(fabsf(cvt_state.primary_measurement.trailer_angle_vcs), F360_DEG2RAD(0.0F), F360_DEG2RAD(10.0F), 0.9F * 2.55F, k_trailer_orth_offset_min);
            const float32_t intersect_offset = fabsf((k_trailer_orth_offset) / F360_Sinf(cvt_state.primary_measurement.trailer_angle_vcs));
            cvt_state.primary_measurement.trailer_intersect_vcs_long = cvt_state.primary_measurement.line_intersect - intersect_offset;
            cvt_state.primary_measurement.f_msmt_valid = true;
         }
      }

      if (cvt_state.secondary_measurement.f_line_valid)
      {
         const float32_t slope_sign = (cvt_state.secondary_measurement.line_slope > 0.0F) ? 1.0F : -1.0F;
         cvt_state.secondary_measurement.trailer_angle_vcs = F360_Atan2f(slope_sign, fabsf(cvt_state.secondary_measurement.line_slope));
         if (fabsf(cvt_state.secondary_measurement.trailer_angle_vcs) > 0.001F)
         {
            const float32_t k_trailer_orth_offset = F360_Linear_Equation_With_Saturation(fabsf(cvt_state.secondary_measurement.trailer_angle_vcs), F360_DEG2RAD(0.0F), F360_DEG2RAD(10.0F), 0.9F * 2.55F, k_trailer_orth_offset_min);
            const float32_t intersect_offset = fabsf((k_trailer_orth_offset) / F360_Sinf(cvt_state.secondary_measurement.trailer_angle_vcs));
            cvt_state.secondary_measurement.trailer_intersect_vcs_long = cvt_state.secondary_measurement.line_intersect - intersect_offset;
            cvt_state.secondary_measurement.f_msmt_valid = true;
         }
      }
   }
}
