/*===========================================================================*\
 * FILE: f360_pvtrailer_width_estimation.cpp
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include <algorithm>
#include "f360_pvtrailer_width_estimation.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   const float32_t k_col_interval = 0.1F;
   const uint32_t k_window_timer_threshold = 1800U;

   /*===========================================================================*\
   * FUNCTION: PVTrailer_Estimate_Width()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Main function of trailer width estimation
   \*===========================================================================*/
   void PVTrailer_Estimate_Width(const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Width_Data_T& pvtrailer_width)
   {
      if ((vehicle_data.speed > 0.2F) && (pvtrailer_width.window_timer < k_window_timer_threshold))
      {
         Process_Input(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_width);
      }
      
      if(!pvtrailer_width.f_estimation_done)
      {
         if (pvtrailer_width.window_timer == k_window_timer_threshold)
         {
            Estimate(pvtrailer_width);
         }
         else
         {
            const float32_t k_default_width = 2.5F;
            pvtrailer_width.trailer_width = k_default_width;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Process_Input()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function to process input
   \*===========================================================================*/
   void Process_Input(const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Width_Data_T& pvtrailer_width)
   {
      const float32_t k_host_length_compensator_from_rear_axle = 1.1F;
      const float32_t host_length = k_host_length_compensator_from_rear_axle * vehicle_data.dist_rear_axle_to_vcs_m;
      const float32_t k_max_trailer_length = 12.0F;

      pvtrailer_width.window_timer++;
      pvtrailer_width.window_timer = std::min(pvtrailer_width.window_timer, k_window_timer_threshold);

      for (uint32_t det_idx = 0U; det_idx < raw_detect_list.number_of_valid_detections; det_idx++)
      {
         const rspp_variant_A::RSPP_Detection_T current_detection = raw_detect_list.detections[det_idx];
         const F360_Detection_Props_T& current_detection_prop = all_detections[det_idx];
         const int32_t current_sensor_id = current_detection.raw.sensor_id;
         const F360_Radar_Sensor_T& current_sensor = sensors[current_sensor_id - 1];

         const float32_t half_width = (static_cast<float32_t>(DETECTION_COLS) * k_col_interval) * 0.5F;
         const float32_t k_range_rate_gate = 0.3F;

         // check detection validation
         const bool f_trailer_detection =
            (((F360_MOUNTING_LOCATION_RIGHT_REAR == current_sensor.constant.mounting_location) ||
               (F360_MOUNTING_LOCATION_LEFT_REAR == current_sensor.constant.mounting_location) ||
               (F360_MOUNTING_LOCATION_CENTER_REAR == current_sensor.constant.mounting_location)) &&
               (std::abs(current_detection.raw.range_rate) < k_range_rate_gate) &&
               (current_detection_prop.vcs_position.x <= -host_length) &&
               (current_detection_prop.vcs_position.x >= -(host_length + k_max_trailer_length)) &&
               (std::abs(current_detection_prop.vcs_position.y) <= half_width) &&
               (!current_detection_prop.f_double_bounce) &&
               (F360_DETECTION_WHEELSPIN_TYPE_INVALID == current_detection_prop.wheel_spin_type) &&
               (!current_detection_prop.f_water_spray));

         if (f_trailer_detection)
         {
            // Index for the x position bin
            uint8_t interval_index = 0U;
            const float32_t  x_pos_to_max_trailer_length = (current_detection_prop.vcs_position.x - host_length) / (-k_max_trailer_length); // will never be 0 as k_max_trailer_length > 0 always
            interval_index = Clamp(static_cast<uint8_t>(std::floor(x_pos_to_max_trailer_length * static_cast<float32_t>(X_INTERVALS_NUMBER))), static_cast<uint8_t>(0U), static_cast<uint8_t>(X_INTERVALS_NUMBER - 1U));

            // Index for the y position bin
            uint8_t col_index = static_cast<uint8_t>(std::floor((half_width + current_detection_prop.vcs_position.y) / k_col_interval));
            col_index = Clamp(col_index, static_cast<uint8_t>(0U), static_cast<uint8_t>(DETECTION_COLS - 1U));

            // accumulate detections
            pvtrailer_width.dets_cnt_per_x_interval[interval_index][col_index] += 1U;
            pvtrailer_width.n_dets_per_area[interval_index] += 1U;
            pvtrailer_width.total_number_of_dets += 1U;
            pvtrailer_width.dets_in_left_side[interval_index] = (pvtrailer_width.dets_in_left_side[interval_index] || (current_detection_prop.vcs_position.y <= 0.0F));
            pvtrailer_width.dets_in_right_side[interval_index] = (pvtrailer_width.dets_in_right_side[interval_index] || (current_detection_prop.vcs_position.y > 0.0F));
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Estimate()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function to estimate trailer width
   \*===========================================================================*/
   void Estimate(F360_PVTrailer_Width_Data_T& pvtrailer_width)
   {
      bool area_to_consider[X_INTERVALS_NUMBER];

      for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
      {
         area_to_consider[i] = false;
         if ((pvtrailer_width.n_dets_per_area[i] > 0U) && (pvtrailer_width.total_number_of_dets > 0U))
         {
            if (((static_cast<float32_t>(pvtrailer_width.n_dets_per_area[i]) / static_cast<float32_t>(pvtrailer_width.total_number_of_dets)) > 0.1F) &&
               (pvtrailer_width.dets_in_left_side[i]) && (pvtrailer_width.dets_in_right_side[i]))
            {
               area_to_consider[i] = true;
            }
         }
      }

      float32_t trailer_widths[X_INTERVALS_NUMBER];
      for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
      {
         if (area_to_consider[i])
         {
            uint32_t left_max_index_in_column;
            uint32_t right_max_index_in_column;

            uint32_t left_index = 0U;
            left_max_index_in_column = pvtrailer_width.dets_cnt_per_x_interval[i][0U];
            uint32_t right_index = DETECTION_COLS - 1U;
            right_max_index_in_column = pvtrailer_width.dets_cnt_per_x_interval[i][DETECTION_COLS - 1U];

            // Consider the half number of dets of the maximum bin as threshold for selecting the furthest bin on each side
            const uint32_t col_index_half = DETECTION_COLS / 2U;
            for (uint32_t j = 0U; j < col_index_half; j++)
            {
               if (pvtrailer_width.dets_cnt_per_x_interval[i][j] > left_max_index_in_column)
               {
                  left_max_index_in_column = pvtrailer_width.dets_cnt_per_x_interval[i][j];
                  left_index = j;
               }

               if (pvtrailer_width.dets_cnt_per_x_interval[i][j + col_index_half] > right_max_index_in_column)
               {
                  right_max_index_in_column = pvtrailer_width.dets_cnt_per_x_interval[i][j + col_index_half];
                  right_index = j + col_index_half;
               }
            }

            int32_t left_index_update = static_cast<int32_t>(left_index);
            int32_t right_index_update = static_cast<int32_t>(right_index);

            const float32_t left_max_update = static_cast<float32_t>(left_max_index_in_column) * 0.5F;
            const float32_t right_max_update = static_cast<float32_t>(right_max_index_in_column) * 0.5F;

            bool f_not_leftmost_bin = true;
            bool f_not_rightmost_bin = true;


            while (f_not_leftmost_bin || f_not_rightmost_bin)
            {
               if ((left_index_update - 1 >= 0) && (static_cast<float32_t>(pvtrailer_width.dets_cnt_per_x_interval[i][left_index_update - 1]) >= left_max_update))
               {
                  left_index_update--;
               }
               else
               {
                  f_not_leftmost_bin = false;
               }
               if ((right_index_update + 1 < static_cast<int32_t>(DETECTION_COLS)) && (static_cast<float32_t>(pvtrailer_width.dets_cnt_per_x_interval[i][right_index_update + 1]) >= right_max_update))
               {
                  right_index_update++;
               }
               else
               {
                  f_not_rightmost_bin = false;
               }
            }
            const int32_t peaks_difference = right_index_update - left_index_update + 1;
            trailer_widths[i] = static_cast<float32_t>(peaks_difference) * k_col_interval;
         }
         else
         {
            trailer_widths[i] = 0.0F;
         }
      }

      // Find the maximum value among the X_INTERVALS_NUMBER areas
      pvtrailer_width.trailer_width = F360_Max_Element(trailer_widths, 12U);

      const float32_t k_min_trailer_width = 1.2F; // [m] minimum trailer width
      pvtrailer_width.trailer_width = fmaxf(pvtrailer_width.trailer_width, k_min_trailer_width);
      pvtrailer_width.f_estimation_done = true;
   }
}
