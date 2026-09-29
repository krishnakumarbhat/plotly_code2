/*===========================================================================*\
 * FILE: f360_pvtrailer_length_estimation.cpp
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include <algorithm>
#include "f360_pvtrailer_length_estimation.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   static const float32_t k_row_interval = 0.15F;

   /*===========================================================================*\
   * FUNCTION: PVTrailer_Estimate_Length()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function used to estimate trailer length
   \*===========================================================================*/
   void PVTrailer_Estimate_Length(
      const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Length_Data_T& pvtrailer_length)
   {
      const uint32_t k_window_timer_threshold = 1800U;
      const float32_t k_host_length_compensator_from_rear_axle = 1.1F;
      const float32_t host_length = k_host_length_compensator_from_rear_axle * vehicle_data.dist_rear_axle_to_vcs_m;
      if ((vehicle_data.speed > 0.2F) && (pvtrailer_length.window_timer < k_window_timer_threshold))
      {
         pvtrailer_length.window_timer++;
         pvtrailer_length.window_timer = std::min(pvtrailer_length.window_timer, k_window_timer_threshold);

         for (uint32_t det_idx = 0U; det_idx < raw_detect_list.number_of_valid_detections; det_idx++)
         {
            const rspp_variant_A::RSPP_Detection_T current_detection = raw_detect_list.detections[det_idx];
            const F360_Detection_Props_T& current_detection_prop = all_detections[det_idx];
            const int32_t current_sensor_id = current_detection.raw.sensor_id;
            const F360_Radar_Sensor_T& current_sensor = sensors[current_sensor_id - 1];

            const float32_t k_max_trailer_length = 12.0F;
            const float32_t k_max_trailer_width = 3.0F;
            const float32_t k_range_rate_gate = 0.3F;

            // check detection validation
            const bool f_trailer_detection =
               (((F360_MOUNTING_LOCATION_RIGHT_REAR == current_sensor.constant.mounting_location) ||
                  (F360_MOUNTING_LOCATION_LEFT_REAR == current_sensor.constant.mounting_location) ||
                  (F360_MOUNTING_LOCATION_CENTER_REAR == current_sensor.constant.mounting_location)) &&
                  (std::abs(current_detection.raw.range_rate) < k_range_rate_gate) &&
                  (current_detection_prop.vcs_position.x <= -host_length) &&
                  (current_detection_prop.vcs_position.x >= -(host_length + k_max_trailer_length)) &&
                  (std::abs(current_detection_prop.vcs_position.y) <= k_max_trailer_width * 0.5F) &&
                  (!current_detection_prop.f_double_bounce) &&
                  (F360_DETECTION_WHEELSPIN_TYPE_INVALID == current_detection_prop.wheel_spin_type) &&
                  (!current_detection_prop.f_water_spray));

            if (f_trailer_detection)
            {
               // accumulate detections
               int32_t row_index = static_cast<int32_t>(F360_Floorf(-(current_detection_prop.vcs_position.x + host_length) / k_row_interval + 0.5F));
               row_index = Clamp(row_index, 0, static_cast<int32_t>(DETECTION_ROWS) - 1);
               pvtrailer_length.detection_row[row_index] += 1;
            }
         }
      }

      if (!pvtrailer_length.f_estimation_done)
      {
         if (pvtrailer_length.window_timer == k_window_timer_threshold)
         {
            TL_Peak first_peak_group[PEAK_GROUP_SIZE];
            TL_Peak second_peak_group[PEAK_GROUP_SIZE];
            TL_Peak third_peak_group[PEAK_GROUP_SIZE];
            int32_t first_peak_cnt = 0;
            int32_t second_peak_cnt = 0;
            int32_t third_peak_cnt = 0;
            bool first_peak_extension;

            Adjust_Sample(pvtrailer_length);

            Locate_Peaks(pvtrailer_length, first_peak_group, first_peak_cnt, first_peak_extension, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt);

            Estimate_Trailer_Length_Peaks(pvtrailer_length, first_peak_group, first_peak_cnt, first_peak_extension, second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt);

            Estimate_Trailer_Length_SVM(pvtrailer_length);

            Post_Processing(pvtrailer_length, first_peak_group);

            pvtrailer_length.f_estimation_done = true;
         }
         else
         {
            const float32_t default_length = 10.0F;
            pvtrailer_length.trailer_length = default_length;
            pvtrailer_length.axle_trailer_length = default_length * 0.7F;
            pvtrailer_length.trailer_HV_gap = 0.0F;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Adjust_Sample()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function used to adjust detection row struct.
   \*===========================================================================*/
   void Adjust_Sample(F360_PVTrailer_Length_Data_T& pvtrailer_length)
   {
      int32_t max_val = 0;
      max_val = F360_Max_Element(pvtrailer_length.detection_row, DETECTION_ROWS - 1U);
      const float32_t temp = 0.05F * static_cast<float32_t>(max_val);
      const int32_t threshold = static_cast<int32_t>(temp);

      for (uint8_t i = 1U; i < DETECTION_ROWS; i++)
      {
         if ((pvtrailer_length.detection_row[i - 1U] == pvtrailer_length.detection_row[i]) && (pvtrailer_length.detection_row[i - 1U] >= threshold))
         {
            pvtrailer_length.detection_row[i - 1U] -= 1;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Locate_Peaks()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Peak finding function used to locate first, second and third peaks.
   \*===========================================================================*/
   void Locate_Peaks(
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      int32_t& first_peak_cnt,
      bool& first_peak_extension,
      TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      int32_t& second_peak_cnt,
      TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      int32_t& third_peak_cnt)
   {
      int32_t temp_detection_row[DETECTION_ROWS];
      int32_t max_val = 0;
      for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
      {
         temp_detection_row[i] = pvtrailer_length.detection_row[i];
         max_val = std::max(max_val, temp_detection_row[i]);
      }

      // first peak groups
      float32_t threshold_forward = 0.8F;
      float32_t threshold_backward = 0.6F;
      uint32_t peak_gap_max = 3U;
      uint32_t peak_r_gap_max = 10U;

      Find_Peak_Group_Info(pvtrailer_length, temp_detection_row, threshold_forward, threshold_backward, peak_gap_max, peak_r_gap_max, max_val, first_peak_group, first_peak_cnt);

      // to avoid first peak group split
      const int32_t tmp_val = pvtrailer_length.detection_row[first_peak_group[0].peak_pos + first_peak_group[0].peak_right_radius];
      if ((first_peak_cnt == 1) && (static_cast<float32_t>(tmp_val) >= 0.08F * static_cast<float32_t>(first_peak_group[first_peak_cnt - 1].peak_val)) && (first_peak_group[first_peak_cnt - 1].peak_right_radius <= 3) && (first_peak_group[first_peak_cnt - 1].peak_pos <= 15))
      {
         first_peak_group[0].peak_right_radius += 4;
         first_peak_extension = true;
      }

      // second peak group
      if (first_peak_cnt >= 1)
      {
         int32_t first_peak_group_end_pos = first_peak_group[first_peak_cnt - 1].peak_pos + first_peak_group[first_peak_cnt - 1].peak_right_radius;
         for (int8_t i = 0; i < first_peak_group_end_pos; i++)
         {
            temp_detection_row[i] = 0;
         }

         max_val = 0;
         for (uint8_t i = 0U; i < 40U; i++)
         {
            max_val = std::max(max_val, temp_detection_row[i]);
         }

         threshold_forward = 0.8F;
         threshold_backward = 0.5F;
         peak_gap_max = 8U;
         peak_r_gap_max = 3U;

         Find_Peak_Group_Info(pvtrailer_length, temp_detection_row, threshold_forward, threshold_backward, peak_gap_max, peak_r_gap_max, max_val, second_peak_group, second_peak_cnt);

         // for some samples, the second peak group is part of first peak group. Here to merge the first one and the second one. Then,
         // calculate the new second peak group.
         if (((first_peak_cnt == 1) && (second_peak_cnt == 1)) && (second_peak_group[0].peak_pos - first_peak_group[0].peak_pos <= 6) && (second_peak_group[0].peak_pos <= 12) && (first_peak_group[0].peak_pos + first_peak_group[0].peak_right_radius == second_peak_group[0].peak_pos - second_peak_group[0].peak_left_radius))
         {
            first_peak_group[0].peak_right_radius += second_peak_group[0].peak_left_radius + second_peak_group[0].peak_right_radius + 1;

            second_peak_cnt = 0;
            first_peak_group_end_pos = first_peak_group[first_peak_cnt - 1].peak_pos + first_peak_group[first_peak_cnt - 1].peak_right_radius;
            for (int8_t i = 0; i < first_peak_group_end_pos; i++)
            {
               temp_detection_row[i] = 0;
            }
            max_val = 0;
            for (uint8_t i = 0U; i < 40U; i++)
            {
               max_val = std::max(max_val, temp_detection_row[i]);
            }
            Find_Peak_Group_Info(pvtrailer_length, temp_detection_row, threshold_forward, threshold_backward, peak_gap_max, peak_r_gap_max, max_val, second_peak_group, second_peak_cnt);
         }
      }

      // third peak
      if (second_peak_cnt >= 1)
      {
         const int32_t second_peak_group_end_pos = second_peak_group[second_peak_cnt - 1].peak_pos + second_peak_group[second_peak_cnt - 1].peak_right_radius;
         for (int8_t i = 0; i < second_peak_group_end_pos; i++)
         {
            temp_detection_row[i] = 0;
         }

         max_val = 0;
         for (uint8_t i = 0U; i < DETECTION_ROWS; i++)
         {
            max_val = std::max(max_val, temp_detection_row[i]);
         }

         threshold_forward = 0.8F;
         threshold_backward = 0.5F;
         peak_gap_max = 8U;
         peak_r_gap_max = 10U;

         Find_Peak_Group_Info(pvtrailer_length, temp_detection_row, threshold_forward, threshold_backward, peak_gap_max, peak_r_gap_max, max_val, third_peak_group, third_peak_cnt);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Find_Peak_Group_Info()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function confirms that found peaks are valid and finds peak radius.
   \*===========================================================================*/
   void Find_Peak_Group_Info(
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const int32_t(&temp_detection_row)[DETECTION_ROWS],
      const float32_t threshold_forward,
      const float32_t threshold_backward,
      const uint32_t peak_gap_max,
      const uint32_t peak_r_gap_max,
      const int32_t max_val,
      TL_Peak(&peak_list_point)[PEAK_GROUP_SIZE],
      int32_t& peak_cnt)
   {
      const float32_t temp = threshold_forward * static_cast<float32_t>(max_val);
      int32_t threshold = static_cast<int32_t>(temp);
      int32_t cur_second_peak_index = -1;

      for (uint32_t k = 0U; k < DETECTION_ROWS; k++)
      {
         if ((cur_second_peak_index == -1) && (temp_detection_row[k] >= threshold) && Is_Peak(temp_detection_row, k))
         {
            uint32_t r_left = 0U;
            uint32_t r_right = 0U;
            Cal_Radius(pvtrailer_length.detection_row, k, r_left, r_right);

            TL_Peak temp_peak;
            temp_peak.peak_pos = static_cast<int32_t>(k);
            temp_peak.peak_val = temp_detection_row[k];
            temp_peak.peak_left_radius = static_cast<int32_t>(r_left);
            temp_peak.peak_right_radius = static_cast<int32_t>(r_right);
            peak_cnt = std::min(peak_cnt, PEAK_GROUP_SIZE - 1);
            peak_list_point[peak_cnt] = temp_peak;
            peak_cnt += 1;

            cur_second_peak_index = static_cast<int32_t>(k);
            const float32_t thres_float = threshold_backward * static_cast<float32_t>(temp_detection_row[k]);
            threshold = static_cast<int32_t>(thres_float);
         }
         else if ((cur_second_peak_index != -1) && (temp_detection_row[k] >= threshold) &&
            (k - static_cast<uint32_t>(cur_second_peak_index) <= peak_gap_max) && Is_Peak(temp_detection_row, k))
         {
            uint32_t r_left_cur = 0U;
            uint32_t r_right_cur = 0U;
            Cal_Radius(pvtrailer_length.detection_row, static_cast<uint32_t>(cur_second_peak_index), r_left_cur, r_right_cur);

            uint32_t r_left = 0U;
            uint32_t r_right = 0U;
            Cal_Radius(pvtrailer_length.detection_row, k, r_left, r_right);

            if ((k - r_left) - (static_cast<uint32_t>(cur_second_peak_index) + r_right_cur) >= peak_r_gap_max)
            {
               continue;
            }

            TL_Peak temp_peak;
            temp_peak.peak_pos = static_cast<int32_t>(k);
            temp_peak.peak_val = temp_detection_row[k];
            temp_peak.peak_left_radius = static_cast<int32_t>(r_left);
            temp_peak.peak_right_radius = static_cast<int32_t>(r_right);
            peak_cnt = std::min(peak_cnt, PEAK_GROUP_SIZE - 1);
            peak_list_point[peak_cnt] = temp_peak;
            peak_cnt += 1;

            const float32_t thres_float = threshold_backward * static_cast<float32_t>(temp_detection_row[k]);
            threshold = static_cast<int32_t>(thres_float);
            cur_second_peak_index = static_cast<int32_t>(k);
         }
         else
         {
            // do nothing
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Peak()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function determines if found peak is valid.
   \*===========================================================================*/
   bool Is_Peak(const int32_t(&sample)[DETECTION_ROWS], const uint32_t index)
   {
      bool peak_sts = false;
      if ((0U < index) && (index < DETECTION_ROWS - 1U))
      {
         peak_sts = (sample[index] > sample[index - 1U]) && (sample[index] > sample[index + 1U]);
      }

      return peak_sts;
   }

   /*===========================================================================*\
   * FUNCTION: Cal_Radius()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function determines peak radius.
   \*===========================================================================*/
   void Cal_Radius(const int32_t(&sample)[DETECTION_ROWS], const uint32_t index, uint32_t& r_left, uint32_t& r_right)
   {
      const float32_t drop_ratio = 0.02F;

      uint32_t index_left = index;
      while (index_left >= 2U)
      {
         const float32_t temp = drop_ratio * static_cast<float32_t>(sample[index]) + 0.5F;
         const int32_t threshold = static_cast<int32_t>(temp);
         if (((sample[index_left - 1U] < sample[index_left]) && (sample[index_left] - sample[index_left - 1U] >= threshold)) || (sample[index_left - 1U] - sample[index_left - 2U] >= threshold))
         {
            index_left -= 1U;
         }
         else if ((sample[index_left - 1U] < sample[index_left]) && (sample[index_left] - sample[index_left - 1U] < threshold) && (sample[index_left - 1U] - sample[index_left - 2U] < threshold) && (static_cast<float32_t>(sample[index_left - 1U]) / static_cast<float32_t>(sample[index_left]) > 0.5F))
         {
            index_left -= 1U;
         }
         else
         {
            break;
         }
      }

      uint32_t index_right = index;
      while (index_right < DETECTION_ROWS - 2U)
      {
         const float32_t temp = drop_ratio * static_cast<float32_t>(sample[index]) + 0.5F;
         const int32_t threshold = static_cast<int32_t>(temp);
         if (((sample[index_right + 1U] < sample[index_right]) && (sample[index_right] - sample[index_right + 1U] >= threshold)) || (sample[index_right + 1U] - sample[index_right + 2U] >= threshold))
         {
            index_right += 1U;
         }
         else if ((sample[index_right + 1U] < sample[index_right]) && (sample[index_right] - sample[index_right + 1U] < threshold) && (sample[index_right + 1U] - sample[index_right + 2U] < threshold) && (static_cast<float32_t>(sample[index_right + 1U]) / static_cast<float32_t>(sample[index_right]) > 0.5F))
         {
            index_right += 1U;
         }
         else
         {
            break;
         }
      }

      r_left = std::max(index - index_left, 1U);
      r_right = std::max(index_right - index, 1U);
   }

   /*===========================================================================*\
   * FUNCTION: Estimate_Trailer_Length_Peaks()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function trailer length based on previously determined peaks.
   \*===========================================================================*/
   void Estimate_Trailer_Length_Peaks(
      F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& first_peak_cnt,
      const bool& first_peak_extension,
      const TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& second_peak_cnt,
      const TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& third_peak_cnt)
   {
      // init i_array_max, j_array_max and k_array_max
      int32_t i_array_max = 0;
      int32_t j_array_max = 0;
      int32_t k_array_max = 0;

      TL_Area_T front_area;

      for (int8_t p = 0; p < PEAK_GROUP_SIZE; p++)
      {
         if (p < first_peak_cnt)
         {
            i_array_max = std::max(i_array_max, first_peak_group[p].peak_val);
         }
         if (p < second_peak_cnt)
         {
            j_array_max = std::max(j_array_max, second_peak_group[p].peak_val);
         }
         if (p < third_peak_cnt)
         {
            k_array_max = std::max(k_array_max, third_peak_group[p].peak_val);
         }
      }

      // default trailer length output
      float32_t last_peak = 15.0F;
      pvtrailer_length.trailer_length_peaks = static_cast<float32_t>(last_peak) * k_row_interval;

      // situation 1: only first peak group is valid
      const int32_t peak_pos_shift = 20;
      front_area.starting_pos = Peak_Right_Edge(first_peak_group, first_peak_cnt - 1);
      front_area.ending_pos = std::min(front_area.starting_pos + peak_pos_shift, static_cast<int32_t>(DETECTION_ROWS - 1U));
      front_area.ref_val = first_peak_group[first_peak_cnt - 1].peak_val;
      front_area.mean_val = 0.0F;
      front_area.max_val = 0.0F;
      Get_Area_Info(pvtrailer_length, front_area);

      bool flag1 = (second_peak_cnt == 0);
      bool flag2 = (first_peak_extension == false) && (first_peak_group[first_peak_cnt - 1].peak_pos >= 17) && (front_area.starting_pos >= 20) && (front_area.max_val <= 0.35F) && (front_area.mean_val <= 0.2F);
      const bool flag3 = (front_area.starting_pos >= 35) && (front_area.max_val <= 0.6F) && (front_area.mean_val <= 0.2F);
      const int32_t temp_max_val = F360_Max_Element_Bounded(pvtrailer_length.detection_row, static_cast<uint32_t>(front_area.starting_pos), DETECTION_ROWS);
      const bool flag4 = (static_cast<float32_t>(temp_max_val) <= 0.1F * static_cast<float32_t>(i_array_max)) && (front_area.starting_pos <= 20); // short box trailer pattern.

      if (flag1 || flag2 || flag3 || flag4)
      {
         if (flag4)
         {
            last_peak = 25.0F; // it may be a very small box trailer that radar can't reach the end of trailer, so we set a default value which is a little longer than first peak group.
         }
         else
         {
            // limit the max last peak to 50 in case there are some wired reflection.
            last_peak = static_cast<float32_t>(std::min(Peak_Right_Edge(first_peak_group, first_peak_cnt - 1) + 1, 50));
         }

         pvtrailer_length.trailer_length_peaks = (last_peak + 1.0F) * k_row_interval;
      }
      else
      {

         // situation 2: at least second peak group is valid
         // front reflection info, these info will be used in the following steps in this script.
         front_area.starting_pos = Peak_Right_Edge(first_peak_group, first_peak_cnt - 1);
         front_area.ending_pos = Peak_Left_Edge(second_peak_group, 0);
         front_area.ref_val = second_peak_group[0].peak_val;
         front_area.mean_val = 0.0F;
         front_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, front_area);

         // middle reflection info, these info will be used in the following steps in this script.
         TL_Area_T middle_area;
         bool f_noise = true; // third_peak_group, assume the third peak group is noise by default.

         if (third_peak_cnt >= 1)
         {
            // long trailer
            const int32_t dist_first_second = Peak_Left_Edge(second_peak_group, 0) - Peak_Right_Edge(first_peak_group, first_peak_cnt - 1);
            const int32_t dist_second_third = Peak_Left_Edge(third_peak_group, 0) - Peak_Right_Edge(second_peak_group, second_peak_cnt - 1);
            flag1 = ((third_peak_group[0].peak_pos > 30) && (second_peak_group[0].peak_pos < 30) && ((dist_first_second >= 10) || (dist_second_third >= 10)));

            // short trailer
            flag2 = (second_peak_cnt <= 2) && (third_peak_cnt <= 2) && (third_peak_group[third_peak_cnt - 1].peak_pos <= 30);

            // check if third peak group is valid or not
            // if third peak group is valid, there must be some strong reflection between second peak group and third peak group.
            if (flag1 || flag2)
            {
               bool f_front_block = false;
               middle_area.starting_pos = Peak_Right_Edge(second_peak_group, second_peak_cnt - 1);
               middle_area.ending_pos = Peak_Left_Edge(third_peak_group, 0);
               middle_area.ref_val = std::max(second_peak_group[second_peak_cnt - 1].peak_val, third_peak_group[0].peak_val);
               middle_area.mean_val = 0.0F;
               middle_area.max_val = 0.0F;
               Get_Area_Info(pvtrailer_length, middle_area);

               // for some short open trailers, the first peak group and second peak group are very close to each other.
               f_front_block = ((static_cast<float32_t>(j_array_max) >= 0.7F * static_cast<float32_t>(i_array_max)) && ((front_area.mean_val >= 0.2F) || (front_area.max_val >= 0.3F) || (middle_area.mean_val * 20.0F <= front_area.mean_val)) && (middle_area.mean_val <= 0.1F) && (third_peak_group[third_peak_cnt - 1].peak_pos <= 30)) || ((middle_area.ending_pos - middle_area.starting_pos <= 2) && (static_cast<float32_t>(j_array_max) >= 0.7F * static_cast<float32_t>(i_array_max)) && (third_peak_group[third_peak_cnt].peak_pos <= 30));

               if ((!f_front_block) && ((middle_area.ending_pos - middle_area.starting_pos <= 4) || (third_peak_group[0].peak_pos - second_peak_group[second_peak_cnt - 1].peak_pos < 8) || ((middle_area.ending_pos - middle_area.starting_pos >= 10) && (middle_area.mean_val >= 0.15F) && (middle_area.max_val >= 0.4F))) && ((static_cast<float32_t>(k_array_max) >= 0.32F * static_cast<float32_t>(j_array_max)) || ((front_area.mean_val <= 0.1F) && (front_area.max_val <= 0.2F) && flag1 && (static_cast<float32_t>(k_array_max) >= 0.25F * static_cast<float32_t>(j_array_max)))))
               {
                  f_noise = false;
               }
               else if ((!f_front_block) && (second_peak_cnt <= 2) && (third_peak_cnt <= 2) && (third_peak_group[third_peak_cnt - 1].peak_pos <= 30) && (static_cast<float32_t>(k_array_max) >= 0.4F * static_cast<float32_t>(j_array_max)) && (middle_area.mean_val >= 0.1F) && (middle_area.max_val >= 0.1F))
               {
                  f_noise = false;
               }
               else
               {
                  // do nothing
               }
            }
         }

         // initial estimation
         if ((third_peak_cnt == 0) || ((third_peak_cnt >= 1) && f_noise))
         {
            if (Peak_Right_Edge(first_peak_group, first_peak_cnt - 1) >= 30)
            {
               last_peak = static_cast<float32_t>(second_peak_group[second_peak_cnt - 1].peak_pos);
            }
            else
            {
               const int32_t temp = second_peak_group[second_peak_cnt - 1].peak_pos + std::min(second_peak_group[second_peak_cnt - 1].peak_right_radius, 3);
               last_peak = static_cast<float32_t>(temp);
            }
         }
         else
         {
            const int32_t temp = third_peak_group[third_peak_cnt - 1].peak_pos + std::min(third_peak_group[third_peak_cnt - 1].peak_right_radius, 3);
            last_peak = static_cast<float32_t>(temp);
         }

         // shrink estimation
         bool f_shrink = false;
         Shrink_Trailer_Length(f_noise, i_array_max, j_array_max, last_peak, f_shrink, pvtrailer_length, first_peak_group, first_peak_cnt,
            second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area, middle_area);

         // extend estimation
         if (!f_shrink)
         {
            Extend_Trailer_Length(f_noise, i_array_max, j_array_max, k_array_max, last_peak, pvtrailer_length, first_peak_group, first_peak_cnt,
               second_peak_group, second_peak_cnt, third_peak_group, third_peak_cnt, front_area);
         }

         // final estimation
         pvtrailer_length.trailer_length_peaks = (last_peak + 1.0F) * k_row_interval;
         pvtrailer_length.trailer_length_peaks = fminf(pvtrailer_length.trailer_length_peaks, 11.0F);
         pvtrailer_length.trailer_length_peaks = fmaxf(pvtrailer_length.trailer_length_peaks, 4.0F);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Peak_Left_Edge()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function finds left edge of the peak.
   \*===========================================================================*/
   int32_t Peak_Left_Edge(const TL_Peak(&peak_group)[PEAK_GROUP_SIZE], const int32_t pos)
   {
      return peak_group[pos].peak_pos - peak_group[pos].peak_left_radius;
   }

   /*===========================================================================*\
   * FUNCTION: Peak_Right_Edge()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function finds left edge of the peak.
   \*===========================================================================*/
   int32_t Peak_Right_Edge(const TL_Peak(&peak_group)[PEAK_GROUP_SIZE], const int32_t pos)
   {
      return peak_group[pos].peak_pos + peak_group[pos].peak_right_radius;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Area_Info()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function finds left edge of the peak.
   \*===========================================================================*/
   void Get_Area_Info(const F360_PVTrailer_Length_Data_T& pvtrailer_length, TL_Area_T& area_info)
   {
      for (int32_t i = area_info.starting_pos; i <= area_info.ending_pos; i++)
      {
         area_info.mean_val += static_cast<float32_t>(pvtrailer_length.detection_row[i]);
         area_info.max_val = fmaxf(area_info.max_val, static_cast<float32_t>(pvtrailer_length.detection_row[i]));
      }

      const int32_t temp = (area_info.ending_pos - area_info.starting_pos + 1) * area_info.ref_val;
      area_info.mean_val = area_info.mean_val / static_cast<float32_t>(temp);
      area_info.max_val = area_info.max_val / static_cast<float32_t>(area_info.ref_val);
   }

   /*===========================================================================*\
   * FUNCTION: Shrink_Trailer_Length()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function shrinks estimated trailer length.
   \*===========================================================================*/
   void Shrink_Trailer_Length(
      const bool f_noise,
      const int32_t i_array_max,
      const int32_t j_array_max,
      float32_t& last_peak,
      bool& f_shrink,
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& first_peak_cnt,
      const TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& second_peak_cnt,
      const TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& third_peak_cnt,
      const TL_Area_T& front_area,
      const TL_Area_T& middle_area)
   {
      // some valid peaks are not located by second peak group (short open trailer)
      int32_t temp_max_val = 0;
      for (int32_t i = Peak_Right_Edge(second_peak_group, second_peak_cnt - 1); i < static_cast<int32_t>(DETECTION_ROWS); i++)
      {
         temp_max_val = std::max(temp_max_val, pvtrailer_length.detection_row[i]);
      }

      // front reflection is a little strong means some peaks are there which are not located.
      if ((f_noise) && (second_peak_cnt == 1) && (second_peak_group[second_peak_cnt - 1].peak_pos >= 30) && (second_peak_group[second_peak_cnt - 1].peak_pos <= 40) && (static_cast<float32_t>(temp_max_val) <= 0.25F * static_cast<float32_t>(i_array_max)) && (((front_area.mean_val >= 0.07F) && (front_area.mean_val <= 0.3F)) || ((front_area.mean_val < 0.07F) && (front_area.max_val >= 0.4F))))
      {
         temp_max_val = 0;
         int32_t temp_cnt = 0;
         int32_t temp_cnt_pos[DETECTION_ROWS] = {};
         const int32_t i_start = Peak_Right_Edge(first_peak_group, first_peak_cnt - 1);
         const int32_t i_end = Peak_Left_Edge(second_peak_group, second_peak_cnt - 1);
         for (int32_t i = i_start; i <= i_end; i++)
         {
            if (pvtrailer_length.detection_row[i] > temp_max_val)
            {
               temp_max_val = pvtrailer_length.detection_row[i];
            }
            else
            {
               // do nothing
            }
         }

         for (int32_t i = i_start; i <= i_end; i++)
         {
            if (static_cast<float32_t>(pvtrailer_length.detection_row[i]) >= 0.8F * static_cast<float32_t>(temp_max_val))
            {
               temp_cnt += 1;
               temp_cnt_pos[temp_cnt] = i;
            }
         }

         TL_Area_T rear_area;
         rear_area.starting_pos = Peak_Right_Edge(second_peak_group, second_peak_cnt - 1);
         rear_area.ending_pos = static_cast<int32_t>(DETECTION_ROWS - 1U);
         rear_area.ref_val = second_peak_group[second_peak_cnt - 1].peak_val;
         rear_area.mean_val = 0.0F;
         rear_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, rear_area);

         // if no strong reflection behind second_peak_cnt_ - 1, it means second_peak_cnt_ - 1 is multipath reflection.
         if ((rear_area.mean_val < front_area.mean_val) && (rear_area.mean_val <= 0.1F) && (((temp_cnt == 1) || (temp_cnt == 2)) && ((temp_cnt_pos[1] - temp_cnt_pos[0] == 1) || (temp_cnt == 2)) && (temp_cnt_pos[1] - temp_cnt_pos[0] <= 6) && (((second_peak_group[0].peak_pos - i_start + temp_cnt_pos[1]) >= 10)) && ((front_area.max_val >= 0.5F) || (front_area.max_val < 0.5F) || (static_cast<float32_t>(j_array_max) <= 0.25F * static_cast<float32_t>(i_array_max)))))
         {
            const int32_t temp = first_peak_group[first_peak_cnt - 1].peak_pos + second_peak_group[0].peak_pos;
            last_peak = 0.6F * static_cast<float32_t>(temp);
            f_shrink = true;
         }
         else
         {
            // do nothing
         }
      }

      // some loaded boat trailer has wired reflection
      if ((!f_shrink) && (f_noise) && (second_peak_cnt >= 2) && (last_peak >= 40.0F) && (first_peak_group[first_peak_cnt - 1].peak_pos >= 17))
      {
         int32_t tmp_last_index = -1;
         for (int32_t i = 0; i < second_peak_cnt - 1; i++)
         {
            const float32_t temp = 0.8F * static_cast<float32_t>(second_peak_group[i].peak_val);
            if (second_peak_group[i + 1].peak_val <= static_cast<int32_t>(temp))
            {
               tmp_last_index = i;
               break;
            }
            else
            {
               tmp_last_index = i;
            }
         }

         last_peak = static_cast<float32_t>(second_peak_group[tmp_last_index].peak_pos);
         f_shrink = true;
      }

      // some utility trailer
      if ((!f_shrink) && (!f_noise) && (first_peak_cnt == 1) && (second_peak_cnt == 1) && (third_peak_cnt >= 1) && (third_peak_group[0].peak_pos > 30) && (third_peak_group[third_peak_cnt - 1].peak_pos <= 47) && (static_cast<float32_t>(j_array_max) >= 0.2F * static_cast<float32_t>(i_array_max)) && (middle_area.ending_pos - middle_area.starting_pos >= 10) && (middle_area.mean_val >= 0.15F) && (middle_area.max_val >= 0.4F))
      {
         TL_Area_T rear_area;

         rear_area.starting_pos = Peak_Right_Edge(third_peak_group, third_peak_cnt - 1) + 4;
         rear_area.starting_pos = std::min(rear_area.starting_pos, 50);

         rear_area.ending_pos = static_cast<int32_t>(DETECTION_ROWS - 1U);
         rear_area.ref_val = std::max(second_peak_group[second_peak_cnt - 1].peak_val, third_peak_group[0].peak_val);
         rear_area.mean_val = 0.0F;
         rear_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, rear_area);

         if ((rear_area.mean_val <= 0.05F) && (rear_area.max_val <= 0.1F))
         {
            last_peak = static_cast<float32_t>(Peak_Right_Edge(second_peak_group, second_peak_cnt - 1));
            f_shrink = true;
         }
         else
         {
            // do nothing
         }
      }

      // BMZ-18705-short-flatbed
      if ((!f_shrink) && f_noise && (third_peak_cnt >= 1) && (third_peak_group[0].peak_pos > 35) && (second_peak_cnt >= 2) && (second_peak_group[second_peak_cnt - 1].peak_pos >= 30) && ((static_cast<float32_t>(j_array_max) >= 0.2F * static_cast<float32_t>(i_array_max)) && (static_cast<float32_t>(j_array_max) <= 0.4F * static_cast<float32_t>(i_array_max))))
      {
         TL_Area_T mid_area;

         mid_area.starting_pos = Peak_Right_Edge(first_peak_group, first_peak_cnt - 1);
         mid_area.ending_pos = Peak_Right_Edge(second_peak_group, second_peak_cnt - 1);
         mid_area.ref_val = i_array_max;
         mid_area.mean_val = 0.0F;
         mid_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, mid_area);

         TL_Area_T rear_area;

         rear_area.starting_pos = mid_area.ending_pos + 1;
         rear_area.ending_pos = static_cast<int32_t>(DETECTION_ROWS - 1U);
         rear_area.ref_val = i_array_max;
         rear_area.mean_val = 0.0F;
         rear_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, rear_area);

         if ((mid_area.mean_val >= 0.05F) && (mid_area.max_val >= 0.2F) && (rear_area.mean_val <= 0.02F) && (rear_area.max_val <= 0.1F))
         {
            last_peak = 0.5F * static_cast<float32_t>(second_peak_group[0].peak_pos) + 0.5F * static_cast<float32_t>(second_peak_group[second_peak_cnt - 1].peak_pos);
            f_shrink = true;
         }
         else
         {
            // do nothing
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Extend_Trailer_Length()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function extends estimated trailer length.
   \*===========================================================================*/
   void Extend_Trailer_Length(
      const bool f_noise,
      const int32_t i_array_max,
      const int32_t j_array_max,
      const int32_t k_array_max,
      float32_t& last_peak,
      const F360_PVTrailer_Length_Data_T& pvtrailer_length,
      const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& first_peak_cnt,
      const TL_Peak(&second_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& second_peak_cnt,
      const TL_Peak(&third_peak_group)[PEAK_GROUP_SIZE],
      const int32_t& third_peak_cnt,
      const TL_Area_T& front_area)
   {
      bool f_extend = false;
      // extend some estimation
      // some peaks are not located for 6m open trailer
      if (f_noise && (third_peak_cnt == 1) && (third_peak_group[0].peak_pos <= 30) && (static_cast<float32_t>(k_array_max) >= 0.25F * static_cast<float32_t>(j_array_max)))
      {
         TL_Area_T rear_area;
         rear_area.starting_pos = Peak_Right_Edge(third_peak_group, third_peak_cnt - 1);
         rear_area.ending_pos = static_cast<int32_t>(DETECTION_ROWS - 1U);
         rear_area.ref_val = third_peak_group[third_peak_cnt - 1].peak_val;
         rear_area.mean_val = 0.0F;
         rear_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, rear_area);

         if ((rear_area.mean_val <= 0.1F) && (rear_area.max_val <= 0.5F))
         {
            last_peak = static_cast<float32_t>(third_peak_group[0].peak_pos);
            f_extend = true;
         }
      }

      // some long box trailer, it is difficult to reach the end of trailer
      if ((!f_extend) && f_noise && (second_peak_cnt == 1) && (first_peak_cnt == 1) && (first_peak_group[0].peak_pos <= 10) && (second_peak_group[0].peak_pos >= 20) && (static_cast<float32_t>(j_array_max) <= 0.4F * static_cast<float32_t>(i_array_max)) && (front_area.mean_val <= 0.05F))
      {
         TL_Area_T rear_area;
         rear_area.starting_pos = Peak_Right_Edge(second_peak_group, second_peak_cnt - 1);
         rear_area.ending_pos = static_cast<int32_t>(DETECTION_ROWS - 1U);
         rear_area.ref_val = second_peak_group[second_peak_cnt - 1].peak_val;
         rear_area.mean_val = 0.0F;
         rear_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, rear_area);

         if (rear_area.mean_val <= 0.05F)
         {
            const int32_t temp = second_peak_group[second_peak_cnt - 1].peak_pos - first_peak_group[second_peak_cnt - 1].peak_pos;
            last_peak = last_peak + 0.7F * static_cast<float32_t>(temp);
            f_extend = true;
         }
      }

      // some long open trailer
      // if the reflection is strong after 8m, it may be a long trailer.
      if ((!f_extend) && f_noise && (third_peak_cnt >= 1) && (first_peak_group[0].peak_pos <= 15))
      {
         TL_Area_T part1_area;
         part1_area.starting_pos = Peak_Right_Edge(first_peak_group, first_peak_cnt - 1);
         part1_area.ending_pos = static_cast<int32_t>(DETECTION_ROWS - 1U);
         part1_area.ref_val = i_array_max;
         part1_area.mean_val = 0.0F;
         part1_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, part1_area);

         TL_Area_T part2_area;
         part2_area.starting_pos = 40;
         part2_area.ending_pos = static_cast<int32_t>(DETECTION_ROWS - 1U);
         part2_area.ref_val = i_array_max;
         part2_area.mean_val = 0.0F;
         part2_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, part2_area);

         TL_Area_T part3_area;
         part3_area.starting_pos = 15;
         part3_area.ending_pos = 25;
         part3_area.ref_val = i_array_max;
         part3_area.mean_val = 0.0F;
         part3_area.max_val = 0.0F;
         Get_Area_Info(pvtrailer_length, part3_area);

         if (((((part1_area.max_val <= 0.2F) && (part1_area.mean_val >= 0.03F)) || ((part1_area.max_val <= 0.35F) && (part1_area.mean_val >= 0.05F))) && (part2_area.max_val >= 0.03F) && (part2_area.mean_val >= 0.01F) && (part1_area.max_val <= 2.0F * part2_area.max_val)) || ((part1_area.max_val <= 0.25F) && (part1_area.mean_val >= 0.025F) && (part2_area.max_val >= 0.1F) && (part2_area.mean_val >= 0.02F)) || ((part1_area.max_val <= 0.8F) && (part1_area.mean_val >= 0.05F) && (part1_area.mean_val <= 0.1F) && (part2_area.max_val >= 0.5F) && (part2_area.mean_val >= 0.07F) && (part3_area.mean_val >= 0.05F) && (third_peak_group[0].peak_pos >= 40)))
         {
            last_peak = 1.5F * static_cast<float32_t>(third_peak_group[third_peak_cnt - 1].peak_pos) - 0.5F * static_cast<float32_t>(second_peak_group[second_peak_cnt - 1].peak_pos);
         }
         else if ((part1_area.max_val >= 0.7F) && (part1_area.mean_val >= 0.2F) && (third_peak_group[third_peak_cnt - 1].peak_pos <= 45))
         {
            last_peak = 1.5F * static_cast<float32_t>(third_peak_group[third_peak_cnt - 1].peak_pos) - 0.5F * static_cast<float32_t>(second_peak_group[second_peak_cnt - 1].peak_pos);
         }
         else if ((part1_area.max_val >= 0.7F) && (part1_area.mean_val >= 0.2F) && (third_peak_group[third_peak_cnt - 1].peak_pos > 45))
         {
            last_peak = 0.5F * (static_cast<float32_t>(third_peak_group[third_peak_cnt - 1].peak_pos) + static_cast<float32_t>(second_peak_group[second_peak_cnt - 1].peak_pos));
         }
         else
         {
            // do nothing
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Extend_Trailer_Length()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function estimates trailer length.
   \*===========================================================================*/
   void Estimate_Trailer_Length_SVM(F360_PVTrailer_Length_Data_T& pvtrailer_length)
   {
      float32_t norm_sample[DETECTION_ROWS];

      Norm_Detection_Row(pvtrailer_length.detection_row, norm_sample);
      SVM_Classification(norm_sample, pvtrailer_length);
   }

   /*===========================================================================*\
   * FUNCTION: Norm_Detection_Row()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *
   \*===========================================================================*/
   void Norm_Detection_Row(const int32_t(&detection_row)[DETECTION_ROWS], float32_t(&norm_sample)[DETECTION_ROWS])
   {
      for (uint32_t i = 0U; i < DETECTION_ROWS; i++)
      {
         norm_sample[i] = 0.0F;
      }

      int32_t detection_row_norm_int = 0;
      for (uint32_t i = 0U; i < DETECTION_ROWS; i++)
      {
         const int32_t temp = detection_row[i] * detection_row[i];
         detection_row_norm_int += temp;
      }
      float32_t detection_row_norm_float = F360_Sqrtf(static_cast<float32_t>(detection_row_norm_int));
      detection_row_norm_float = std::max(detection_row_norm_float, 1.0F);

      const float32_t detection_row_norm_inv = 1.0F / detection_row_norm_float;
      for (uint32_t i = 1U; i < DETECTION_ROWS; i++)
      {
         norm_sample[i - 1U] = static_cast<float32_t>(detection_row[i]) * detection_row_norm_inv;
      }
   }

   /*===========================================================================*\
   * FUNCTION: SVM_Classification()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *
   \*===========================================================================*/
   void SVM_Classification(const float32_t(&norm_sample)[DETECTION_ROWS], F360_PVTrailer_Length_Data_T& pvtrailer_length)
   {
      const uint32_t CLASS_NUMBER = 20U;
      const int32_t SVM_CONF_LEVEL = 10;

      const float32_t length_array[CLASS_NUMBER] = { 4.0F, 4.5F, 5.5F, 5.5F, 5.5F, 6.0F, 8.0F, 7.5F, 12.0F, 5.0F,
                                                    9.0F, 4.0F, 4.5F, 4.5F, 6.0F, 5.0F, 4.5F, 8.0F, 12.0F, 6.0F };

      const float32_t scale[CLASS_NUMBER] = { 0.4129F, 0.52202F, 0.51121F, 0.43387F, 0.37632F, 0.68247F, 0.49218F,
                                             0.91949F, 0.44904F, 0.77948F, 0.97985F, 0.67035F, 0.31087F, 0.40033F,
                                             0.52493F, 0.18135F, 0.30022F, 0.41702F, 0.48509F, 0.52922F };

      const float32_t bias[CLASS_NUMBER] = { -8.5753F, -5.5003F, -2.0519F, -4.3189F, -4.1905F, -1.9026F, -3.3041F,
                                            -0.8776F, -4.7328F, -2.9775F, -1.4362F, -6.2018F, -7.2663F, -19.8264F,
                                            -6.6528F, -7.5845F, -9.9056F, -6.5739F, -3.1329F, -1.5586F };

      const float32_t beta[CLASS_NUMBER][DETECTION_ROWS] = {
          {0.0F, -0.19363F, -0.90468F, -1.2506F, 0.64207F, 2.6677F, -0.2681F, 3.5717F, 2.1859F, 0.83982F,
           -0.37425F, 0.57946F, -0.16263F, 0.67564F, 2.7994F, 1.6362F, -3.8887F, -3.1448F, -2.1749F, 0.44779F,
           -0.25192F, -0.72891F, -0.82365F, -1.402F, -0.9352F, -0.64821F, -1.4041F, -1.9658F, -0.89305F, -0.66524F,
           -1.1011F, 0.41419F, 1.1646F, -0.19265F, -0.528F, -1.2372F, -1.7478F, -1.7392F, -1.0133F, -0.6634F,
           -0.52063F, -0.7277F, -0.33164F, -0.2638F, -0.74888F, -1.28F, -1.0686F, -0.6395F, -0.50941F, -0.59143F,
           -0.40756F, -0.27926F, -0.39251F, -0.92785F, -0.88629F, -0.37033F, -0.4099F, -0.93001F, -1.9745F, -0.82809F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
           0.0F, 0.0F},

          {0.0F, 0.0F, 0.0F, 0.080389F, 0.36007F, 0.69605F, 0.84954F, 1.6251F, 1.5374F,
           -0.81616F, 0.15461F, -0.049047F, -0.63579F, -0.074719F, -0.90473F, 0.15699F, 3.3483F, 0.80492F,
           -0.18669F, -0.56234F, -0.26757F, 1.8796F, 1.3893F, 0.6885F, 1.0027F, 0.32858F, 0.085081F,
           -0.1585F, -0.16173F, -0.28256F, -0.51706F, -0.81414F, -0.37128F, -0.18423F, -0.17864F, -0.19874F,
           -0.29849F, -0.24971F, -0.2005F, 0.023784F, 0.0038863F, -0.065001F, -0.057747F, -0.067563F, -0.014453F,
           -0.021558F, -0.015083F, -0.015837F, -0.018698F, -0.0033372F, 0.00072393F, -0.0042907F, -0.0014302F, -0.017609F,
           -0.095692F, -0.069673F, -0.021646F, -0.0074419F, -0.0050582F, 0.0030301F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.032531F, 0.0F, 0.0F, -0.0017909F, -0.28182F, -0.097416F, 1.3456F, -0.23747F, 1.8728F, 0.74914F,
           -1.2429F, -0.84162F, -2.4188F, -1.226F, -1.4637F, -1.004F, -0.36556F, 0.4128F, 0.4844F, -1.0688F,
           -0.92193F, -0.97626F, -0.7318F, -0.90106F, -4.0921F, -2.6279F, -1.7333F, 0.82817F, -1.338F, -1.2858F,
           -1.6254F, -0.94236F, -0.8788F, -0.9201F, -0.66601F, -0.39268F, -1.0471F, -0.953F, -0.43434F, -0.37413F,
           -0.70014F, -1.8828F, -2.8239F, -1.1629F, -0.31255F, 0.29758F, -0.18218F, -0.14961F, -0.090086F, -0.11498F,
           -0.033225F, -0.0084975F, -0.029534F, -0.074187F, -0.017322F, 0.0060161F, -0.0029946F, 0.019975F, -0.0084709F, 0.024885F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.0014222F, 0.0F, 0.0F, 0.0F, -0.00066075F, -0.4193F, -0.40847F, -0.44474F, 0.80146F, 1.3794F,
           1.971F, -0.022723F, -0.53471F, -0.53344F, -0.60059F, 0.51834F, -0.046793F, -0.98561F, -0.31544F, 0.10766F,
           1.4474F, 0.99952F, -0.88027F, -0.92149F, -0.7048F, -0.61635F, 0.20963F, -0.29733F, -1.0116F, -0.41795F,
           0.40302F, 0.75374F, 0.49205F, -0.028719F, -0.12967F, -0.026686F, 0.022819F, 0.080089F, 0.24746F, 0.37875F,
           0.46079F, 0.29816F, 0.17067F, 0.083105F, 0.1218F, 0.17267F, -0.090701F, 0.016113F, 0.0010572F, -0.12879F,
           0.17085F, 0.052111F, -0.007709F, -0.047771F, -0.14346F, -0.057329F, -0.010494F, -0.012455F, 0.039173F, 0.0023148F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.013654F, 0.0F, 0.0F, 0.0F, -0.023321F, -0.35355F, -0.72704F, -0.26053F, 0.13107F, 1.1961F,
           -0.64769F, -1.2261F, -1.0665F, 0.16329F, 0.26408F, -1.2192F, 1.2035F, 1.448F, 0.7862F, -0.44691F,
           -0.50633F, 0.23854F, -0.59639F, -0.51566F, 0.15457F, 0.25843F, 1.8524F, 1.0733F, 1.1514F, -0.34543F,
           -0.99191F, -0.71386F, 0.2026F, 0.32775F, -0.55115F, -0.16876F, 1.7343F, 1.4048F, 0.52473F, 0.50474F,
           0.57126F, 1.5234F, 3.4288F, 1.9694F, 0.7467F, -0.20856F, -0.037684F, -0.058815F, 0.3676F, 0.49995F,
           -0.042591F, -0.15193F, -0.15499F, -0.1456F, -0.10841F, -0.11966F, -0.13362F, -0.23713F, -0.47904F, -0.16158F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.28763F, 0.58545F, -0.33085F, -0.13725F, -0.20369F, 1.8233F, 1.4716F, -1.8033F, -0.19899F, 0.15629F,
           0.71964F, 0.44163F, 1.103F, -0.29458F, -0.66093F, -0.62911F, 0.13625F, 0.0085433F, -0.085397F, -0.4657F,
           -0.57178F, -0.40514F, -0.27015F, -0.27523F, -0.29884F, -0.29443F, -0.19082F, -0.13492F, -0.05233F, -0.40615F,
           0.20179F, -0.23232F, -0.64204F, -0.45277F, -0.37757F, -0.33015F, -0.17689F, -0.16786F, -0.15556F, -0.11117F,
           -0.17917F, -0.56381F, -0.11866F, -0.087702F, -0.28308F, -0.1983F, -0.1986F, -0.14955F, -0.087522F, -0.065644F,
           -0.064715F, -0.093326F, -0.24554F, -0.53554F, -0.28265F, -0.094879F, -0.036989F, -0.20079F, -0.32209F, -0.11204F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.22291F, 0.0F, 0.038622F, 0.28446F, -0.11115F, -0.19459F, -0.25828F, -0.4493F, -0.32412F, 0.48747F,
           -0.40918F, -0.23454F, -1.6889F, -0.55105F, -0.25421F, 0.87695F, -1.083F, -0.91106F, 1.2902F, -0.0067575F,
           1.2655F, -0.31824F, 1.0926F, 1.2734F, -0.79965F, -1.8112F, -0.32449F, 2.8498F, 0.77537F, 0.96574F,
           2.2567F, 1.0472F, -0.72472F, 0.60078F, 1.8516F, -1.99F, 0.74182F, -0.1945F, 0.26145F, -1.4528F,
           -2.4339F, -1.4529F, 0.19984F, 1.6862F, 0.94194F, 1.4351F, 2.4556F, 4.8523F, -0.40362F, -0.52355F,
           -0.65124F, -0.82628F, 2.2138F, -3.2288F, 0.0036695F, -0.89946F, -0.92558F, -2.1385F, -5.5444F, -2.6F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.11263F, -0.22505F, -0.60037F, 0.6703F, 0.39625F, 0.52819F, -1.5869F, -1.0572F, -0.29619F, -0.27462F,
           -0.41039F, -0.29288F, -0.47318F, -0.48451F, -0.40871F, -0.32908F, 0.082552F, -0.1141F, -0.11661F, -0.24799F,
           -0.032227F, 0.080336F, 0.19269F, -0.38614F, 0.094868F, 0.17484F, -0.078634F, 0.0013103F, 0.5447F, 1.6331F,
           0.27546F, -0.027553F, -0.12625F, -0.48855F, 0.13368F, 0.13207F, -0.10305F, 0.049481F, 0.22626F, 0.12498F,
           0.90974F, 1.4791F, -0.030734F, -0.11674F, -0.2016F, -0.29972F, 0.37205F, -0.025945F, -0.3193F, -0.43578F,
           -0.15439F, 0.40909F, -0.29255F, 0.11746F, 0.010598F, 0.11057F, 0.10748F, 0.21052F, 0.1408F, -0.0097012F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.003417F, -0.32488F, -0.076807F, 0.9609F, 0.21011F, -0.025325F, 1.5238F, 1.501F, 0.53816F, -0.7675F,
           -0.42515F, -0.61946F, 0.19516F, 1.3709F, 1.9908F, -2.8085F, -1.2948F, -1.39F, -0.067532F, -0.72348F,
           -0.66029F, 0.24486F, 0.9975F, 1.1153F, 0.74647F, 0.44817F, 0.76477F, 1.3931F, 0.27577F, -0.24329F,
           -0.10451F, -0.30541F, -1.6593F, -1.2754F, -0.58206F, -0.14739F, 1.0981F, 0.19491F, 0.60138F, 0.08101F,
           0.28565F, 1.337F, 0.034577F, -0.10977F, -2.8992F, -0.69831F, 0.31129F, 0.12167F, 0.37962F, 0.63652F,
           0.47095F, 0.20675F, -0.29571F, -0.33107F, 0.79588F, 0.44953F, 0.59276F, 0.55643F, 1.8304F, 0.38872F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.0F, -0.028501F, -0.255F, 0.39611F, 0.96213F, -1.0478F, 2.595F, 0.24568F, 0.29181F, 0.087943F,
           0.10868F, -0.059975F, -0.0811F, -0.04124F, -0.66753F, 2.7883F, 0.80861F, -1.1597F, -0.82998F, -0.95542F,
           -1.044F, -1.4939F, -0.94541F, -0.80091F, -0.31112F, -0.16439F, -0.2624F, -0.15948F, -0.044159F, -0.10602F,
           -0.17652F, -0.14114F, -0.4833F, -0.083929F, -0.10772F, -0.14597F, -0.21956F, -0.26328F, -0.11977F, -0.055639F,
           -0.038716F, -0.066818F, -0.046794F, -0.060887F, -0.27581F, -0.2303F, -0.081113F, -0.064446F, -0.056894F, -0.056769F,
           -0.034081F, -0.034613F, -0.078055F, -0.15439F, -0.087573F, -0.035649F, -0.025844F, -0.043408F, -0.13411F, -0.048243F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.0F, 0.0F, 0.0F, 0.0F, 0.02178F, -0.17044F, -0.11609F, -0.17583F, 0.10214F, 0.30789F,
           0.53064F, 0.72889F, 0.92773F, 0.18354F, -0.21779F, -0.76828F, -0.43181F, -0.15861F, -0.25533F, -0.33942F,
           -0.47542F, -0.61446F, -0.38564F, -0.6995F, -0.29497F, -0.50443F, -0.81119F, -0.53364F, 0.34772F, 0.0976F,
           -0.32041F, -0.22234F, 0.34913F, 1.399F, 0.52215F, -0.12155F, -0.003301F, -0.013849F, 0.021699F, -0.0021051F,
           -0.056979F, 0.042966F, 0.098179F, 0.092332F, 0.57247F, 1.0684F, 0.41669F, 0.34101F, 0.94507F, 1.7681F,
           1.0114F, 0.071457F, 0.019531F, 0.32213F, 0.57993F, 0.2484F, 0.10029F, 0.12179F, 0.065657F, 0.077029F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.005379F, 0.0F, -0.031253F, -0.012269F, 0.72419F, -0.35366F, -1.3553F, 2.5344F, 3.511F, -0.22603F,
           1.5932F, 1.5615F, 3.4562F, 1.46F, 1.2426F, -0.76763F, -0.98165F, -1.7634F, 0.4248F, 0.89851F,
           0.60607F, -0.036673F, 0.9939F, 0.21673F, 4.3252F, 2.8021F, 1.5905F, -0.15996F, 0.36715F, 1.1409F,
           1.8946F, 0.47425F, -0.22077F, -0.95391F, -0.2389F, -0.15564F, -0.036538F, 0.083347F, 0.23164F, 0.061773F,
           -0.036676F, 0.090793F, 0.38219F, -0.024961F, -0.04919F, -0.11123F, -0.021605F, -0.2089F, -0.26932F, -0.053009F,
           -0.095145F, -0.37427F, -0.65825F, -0.22287F, -0.16219F, -0.15541F, -0.055559F, 0.012861F, 0.092872F, 0.026257F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.089748F, 0.0F, 0.0F, 0.0F, -0.20388F, -2.9457F, 1.5506F, 0.10586F, -4.5323F, -2.013F,
           -0.089768F, 3.2097F, -2.4957F, 3.9195F, -0.41914F, 1.7252F, -1.0472F, -0.30641F, 2.2999F, 1.8588F,
           0.11189F, 0.70415F, 7.1065F, 3.5648F, -0.70464F, -4.5739F, -6.2795F, -1.9629F, -1.0186F, -0.82861F,
           -3.5481F, -3.2675F, -2.5948F, -2.2569F, -0.89835F, -2.7768F, -1.6296F, -1.3683F, -1.1904F, -1.0465F,
           -0.50482F, -0.85475F, -0.64579F, 0.55816F, 1.1046F, 0.33877F, 0.26469F, -0.070649F, 0.028983F, 0.19188F,
           -0.011108F, 2.6743F, 0.69667F, 0.85287F, 0.60018F, -0.51894F, -0.60124F, -0.23965F, 1.2681F, 1.0647F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.0F, 0.0F, 0.054998F, 0.54998F, -0.82497F, -4.0301F, 1.158F, 2.6941F, 6.3939F, 2.4669F,
           3.5376F, 2.2866F, 1.5001F, -2.8903F, 7.8054F, 3.6948F, 1.3843F, 1.404F, 1.7211F, -3.0907F,
           5.1603F, 0.54593F, -0.87278F, -4.2174F, -3.4019F, -0.28495F, -1.5366F, -1.2767F, -0.62577F, -0.90518F,
           -0.37539F, -0.92098F, -0.21856F, -0.27062F, -0.2258F, 0.12977F, -0.14637F, -0.16243F, -0.031442F, 0.52882F,
           0.026941F, 0.039484F, -0.37055F, -0.69346F, -0.16235F, 0.12529F, -0.22514F, -0.34103F, 0.20361F, -0.26465F,
           -0.072096F, -1.4331F, -0.04815F, 0.064742F, 0.15916F, 0.45659F, 0.38788F, 0.45031F, -0.04382F, -0.41013F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.026631F, 0.0F, 0.0F, -0.071016F, 0.013329F, 0.65982F, 0.53917F, 1.6878F, -0.25932F, -0.44546F,
           -0.45529F, -0.068777F, 0.046772F, 0.17372F, 0.84883F, 2.553F, -0.64075F, 0.16239F, -0.25352F, -0.27129F,
           0.032036F, 0.14009F, 1.3794F, 1.675F, 0.18675F, 0.10945F, 0.90336F, 0.49111F, 0.84919F, 0.94179F,
           0.94022F, 1.4565F, 1.0436F, -0.17379F, 0.11522F, 0.65402F, 1.6497F, 1.4931F, 0.91828F, 0.13213F,
           0.34308F, 0.4569F, 0.020321F, -0.14775F, 0.078053F, 0.015315F, -0.14025F, -0.36871F, -0.26086F, -0.012766F,
           -0.075706F, -0.27045F, -0.43524F, -0.11123F, 0.48145F, 0.10187F, -0.10175F, -0.064801F, -0.066474F, -0.043391F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.0F, -0.099179F, -0.034414F, 0.025814F, 0.58318F, 1.146F, -0.52881F, 1.4549F, -1.7918F, -0.30596F,
           0.40886F, 0.025859F, -0.067258F, -0.20136F, -0.94608F, -2.0608F, 0.95675F, 1.8106F, 1.0707F, 0.16385F,
           -0.036346F, 0.14714F, -0.030099F, 0.46383F, 0.1922F, 0.16311F, 0.21602F, 0.2078F, 0.22563F, 0.58489F,
           0.57876F, 0.26645F, 0.0010066F, 0.0074828F, 0.25495F, 0.25284F, -0.25344F, -0.21821F, -0.11212F, -0.15752F,
           -0.23413F, -0.18478F, 0.0015428F, -0.04862F, -0.48941F, -0.7448F, -0.24881F, -0.064847F, -0.063162F, -0.042002F,
           -0.020692F, -0.040825F, -0.18044F, -0.26798F, -0.034337F, -0.015088F, -0.032332F, -0.12121F, -0.39459F, -0.14336F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.0F, 0.0F, 0.0F, 0.0028593F, 1.2105F, 2.809F, 0.15532F, -1.041F, -3.295F, 0.52866F,
           -2.3922F, -2.6847F, 1.1449F, 0.61894F, -0.17574F, 1.8163F, 1.3173F, 1.8362F, -0.84384F, 1.747F,
           1.4583F, 2.2872F, -9.3537F, -4.6831F, -4.0783F, 0.22261F, 8.2912F, 1.5346F, 0.9394F, 1.2147F,
           0.68716F, 0.5459F, 0.65725F, -0.067757F, 0.37016F, 0.48001F, 0.23192F, -0.064705F, -0.12343F, 0.18236F,
           0.4165F, 0.31737F, 0.48489F, 0.39508F, -0.07122F, -0.026399F, -0.040603F, 0.12376F, 0.15961F, 0.21301F,
           0.54308F, -0.61227F, -0.2395F, 0.2565F, -0.3148F, 0.0042358F, -0.18639F, 0.2603F, 0.21796F, 0.44119F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.14762F, -0.28802F, -0.35894F, -0.87908F, 1.9726F, 1.2522F, 1.174F, 0.14586F, 1.1244F, 0.13509F,
           0.39595F, -0.52762F, 0.63508F, 0.42177F, -1.05F, -1.1172F, 1.0895F, 1.0395F, -0.48111F, 0.76736F,
           -0.42756F, 0.50803F, -0.28942F, 1.6618F, 1.3791F, 1.308F, 0.50509F, -2.5414F, -1.9549F, -2.7701F,
           -1.1705F, 0.67461F, 1.8553F, 1.1422F, 0.20208F, 2.8786F, -0.29463F, 0.14324F, -0.37817F, 0.80788F,
           1.2734F, -0.50293F, -0.55016F, -1.3971F, -1.0207F, -1.7779F, -2.3892F, -3.2055F, 0.83451F, 0.1114F,
           0.81852F, 0.96007F, -0.96026F, 3.4362F, -0.14618F, 0.66749F, 0.92918F, 1.9302F, 2.894F, 1.5713F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {-0.11991F, 0.72742F, 1.3905F, 0.85391F, 0.58477F, 0.00023436F, 0.02502F, 1.0473F, -1.3091F, -0.046751F,
           -0.59994F, 0.018543F, -1.2532F, -0.93712F, -1.8309F, 1.1735F, -0.91034F, -1.2764F, -0.75789F, 1.5305F,
           1.213F, -0.19191F, -0.66417F, -2.0064F, -0.068872F, -0.12347F, 0.18991F, -0.059674F, -0.1368F, -0.32736F,
           -0.49554F, -0.23179F, 2.8559F, 0.19334F, 0.40621F, -0.0083221F, 0.30472F, 1.5327F, 1.048F, 0.5106F,
           0.014083F, -1.8031F, 0.63331F, 0.99686F, 4.4054F, 2.7307F, 1.8588F, 0.52535F, -0.19237F, -1.3883F,
           -0.21745F, 0.020813F, 0.29807F, 1.2287F, -0.49451F, 0.056012F, 0.25169F, 0.7191F, -1.5659F, 0.42136F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},

          {0.0059887F, 0.0F, 0.0F, -0.00077307F, -0.015613F, -0.021435F, -0.033999F, -0.8246F, -1.2954F, -1.1849F,
           0.77029F, 1.3282F, -0.35519F, 0.38579F, 0.45403F, -0.16511F, 0.34155F, 0.46714F, -0.19512F, -0.45172F,
           -1.9461F, -0.6338F, 1.1836F, 1.0046F, -0.23787F, 0.4504F, 0.4605F, 0.99946F, 1.0476F, -0.075737F,
           -0.55684F, -1.054F, -0.49328F, -0.90821F, -0.1995F, 0.027495F, 0.027843F, -0.1534F, -0.2638F, -0.3874F,
           -0.55807F, -0.32766F, -0.25579F, -0.19138F, -0.42737F, -0.64586F, -0.088121F, -0.25397F, -0.3751F, -0.70805F,
           -0.35865F, -0.18727F, -0.19918F, -0.33673F, -0.20954F, -0.14647F, -0.071301F, -0.086733F, -0.074889F, -0.023515F,
           0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F} };

      const float32_t svm_conf[SVM_CONF_LEVEL] = { 1.0F, 0.9F, 0.8F, 0.7F, 0.6F, 0.5F, 0.4F, 0.3F, 0.2F, 0.1F };

      const float32_t svm_score[CLASS_NUMBER][SVM_CONF_LEVEL] =
      {
          {2.1234F, 1.9569F, 1.8222F, 1.7063F, 1.616F, 1.5075F, 1.3948F, 1.2601F, 1.0723F, 0.0052619F},
          {2.2228F, 2.0824F, 1.9796F, 1.8959F, 1.8152F, 1.7419F, 1.6444F, 1.5469F, 1.3859F, 0.48846F},
          {1.6016F, 1.424F, 1.3734F, 1.3288F, 1.2943F, 1.2521F, 1.1839F, 1.1178F, 1.0544F, 0.25574F},
          {1.6976F, 1.6335F, 1.586F, 1.5007F, 1.4291F, 1.3867F, 1.3484F, 1.2928F, 1.2197F, 0.83449F},
          {2.7147F, 2.5012F, 2.3805F, 2.2671F, 2.1653F, 2.0415F, 1.8928F, 1.6621F, 1.3628F, -0.74941F},
          {1.5905F, 1.5456F, 1.5161F, 1.4929F, 1.4617F, 1.4248F, 1.378F, 1.3176F, 1.2243F, -0.30855F},
          {2.5133F, 2.2874F, 2.1217F, 1.9777F, 1.8488F, 1.7414F, 1.6246F, 1.4249F, 1.1288F, -0.41917F},
          {1.377F, 1.3319F, 1.2995F, 1.2622F, 1.2298F, 1.2075F, 1.1833F, 1.1571F, 1.1048F, 0.37106F},
          {2.4301F, 2.2282F, 2.0686F, 1.944F, 1.8183F, 1.7199F, 1.5987F, 1.4372F, 1.2595F, -1.1129F},
          {1.8046F, 1.7557F, 1.7028F, 1.6454F, 1.5944F, 1.5414F, 1.4728F, 1.411F, 1.2811F, 0.67409F},
          {1.6155F, 1.5699F, 1.5313F, 1.5009F, 1.4717F, 1.4352F, 1.4013F, 1.3651F, 1.2794F, 0.78913F},
          {3.3864F, 3.1939F, 2.939F, 2.6859F, 2.2244F, 1.9086F, 1.6892F, 1.5321F, 1.339F, 0.5965F},
          {4.5126F, 3.056F, 2.2018F, 1.6238F, 1.1891F, 0.92402F, 0.50939F, -0.047981F, -0.511F, -3.6364F},
          {3.4107F, 3.0143F, 2.5737F, 2.2238F, 1.9803F, 1.7117F, 1.3333F, 0.98458F, 0.50382F, -2.0337F},
          {1.5653F, 1.5027F, 1.4042F, 1.3314F, 1.2671F, 1.2152F, 1.1728F, 1.1356F, 1.0803F, 0.51724F},
          {2.0162F, 1.8666F, 1.7753F, 1.7098F, 1.6488F, 1.5838F, 1.5142F, 1.4068F, 1.2213F, 0.90366F},
          {2.2961F, 2.0253F, 1.8236F, 1.6222F, 1.421F, 1.2233F, 0.97647F, 0.62214F, 0.13455F, -4.1747F},
          {2.3257F, 2.1736F, 2.0473F, 1.8813F, 1.7579F, 1.628F, 1.4419F, 1.2512F, 1.0237F, -1.3491F},
          {2.8838F, 2.5228F, 2.2204F, 2.0031F, 1.8183F, 1.6967F, 1.5767F, 1.4443F, 1.2442F, 0.35254F},
          {1.8483F, 1.7182F, 1.6395F, 1.5463F, 1.4622F, 1.3906F, 1.3157F, 1.2182F, 1.1408F, 0.6765F} };

      float32_t scores[CLASS_NUMBER];

      for (uint32_t i = 0U; i < CLASS_NUMBER; i++)
      {
         scores[i] = 0.0F;
      }

      for (uint32_t i = 0U; i < CLASS_NUMBER; i++)
      {
         for (uint32_t k = 0U; k < DETECTION_ROWS; k++)
         {
            scores[i] += (norm_sample[k] / scale[i]) * beta[i][k];
         }

         scores[i] += bias[i];
      }

      // class
      const int32_t predict_class = static_cast<int32_t>(F360_Max_Index(scores, CLASS_NUMBER));
      const float32_t max_score = scores[predict_class];
      pvtrailer_length.trailer_length_SVM = length_array[predict_class];

      // confidence
      for (int8_t i = 0; i < SVM_CONF_LEVEL; i++)
      {
         if (max_score >= svm_score[predict_class][i])
         {
            pvtrailer_length.trailer_length_SVM_conf = svm_conf[i];
            break;
         }
         else
         {
            pvtrailer_length.trailer_length_SVM_conf = 0.0F;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: SVM_Classification()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function used to finalize estimation and post process the output from SVM
   * classification and SVM confidence functions.
   \*===========================================================================*/
   void Post_Processing(F360_PVTrailer_Length_Data_T& pvtrailer_length, const TL_Peak(&first_peak_group)[PEAK_GROUP_SIZE])
   {
      if (pvtrailer_length.trailer_length_peaks >= 1.0F)
      {
         const float32_t SVM_conf_threshold = 0.05F;
         if (pvtrailer_length.trailer_length_SVM_conf >= SVM_conf_threshold)
         {
            pvtrailer_length.trailer_length = pvtrailer_length.trailer_length_SVM;
         }
         else
         {
            pvtrailer_length.trailer_length = pvtrailer_length.trailer_length_peaks;
         }

         pvtrailer_length.axle_trailer_length = pvtrailer_length.trailer_length * 0.7F;
         pvtrailer_length.trailer_HV_gap = fminf(static_cast<float32_t>(first_peak_group[0].peak_pos) * k_row_interval, 2.0F);
      }
      else
      {
         pvtrailer_length.trailer_length = 0.0F;
         pvtrailer_length.axle_trailer_length = 0.0F;
         pvtrailer_length.trailer_HV_gap = 0.0F;
      }
   }
}
