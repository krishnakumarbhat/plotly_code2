/*===================================================================================*\
* FILE: f360_concrete_wall_detector.cpp
*====================================================================================
*Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
\*===================================================================================*/
#include "f360_concrete_wall_detector.h"
#include "f360_get_wall_time.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Run_CWD()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Estimate the distance to concrete walls on right and left side of host.
   \*===========================================================================*/

   void Run_CWD(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      CWD_Data_T& cwd_data,
      Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      bool current_msmt_valid[MAX_NUMBER_OF_SENSORS];
      float32_t current_msmt[MAX_NUMBER_OF_SENSORS];
      float32_t sensor_estimate[MAX_NUMBER_OF_SENSORS];
      float32_t sensor_estimate_conf[MAX_NUMBER_OF_SENSORS];
      (void)memset(&current_msmt_valid[0], 0, sizeof(current_msmt_valid));
      (void)memset(&current_msmt[0], 0, sizeof(current_msmt));
      (void)memset(&sensor_estimate[0], 0, sizeof(sensor_estimate));
      (void)memset(&sensor_estimate_conf[0], 0, sizeof(sensor_estimate_conf));

      float32_t min_relevant_det_longpos = 100.0F;
      float32_t max_relevant_det_longpos = -100.0F;

      /* Find long zone limits */
      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         const float32_t k_min_abs_az = 0.4F; // 23 deg
         if (sensors[i].variable.is_valid &&
            (sensors[i].variable.number_of_valid_detections > 0U) &&
            (fabsf(sensors[i].constant.mounting_position.vcs_boresight_azimuth_angle) > k_min_abs_az))
         {
            min_relevant_det_longpos = fminf(min_relevant_det_longpos, sensors[i].constant.mounting_position.vcs_position.longitudinal);
            max_relevant_det_longpos = fmaxf(max_relevant_det_longpos, sensors[i].constant.mounting_position.vcs_position.longitudinal);

            cwd_data.mount_loc[i] = sensors[i].constant.mounting_location; //resim will reorder sensor position, so need to record this for logging
         }
      }

      const float32_t k_longpos_buffer = 0.26F;
      min_relevant_det_longpos -= k_longpos_buffer;
      max_relevant_det_longpos += k_longpos_buffer;

      /* Look for detections */
      int32_t det_idx = Get_First_Relevant_Long_Sorted_Det_Idx(min_relevant_det_longpos, raw_detect_list);
      bool f_continue = true;
      for (uint32_t i = 0U; i < raw_detect_list.number_of_valid_detections; i++)
      {
         f_continue = f_continue && (det_idx > F360_INVALID_ID);
         if (!f_continue)
         {
            break;
         }
         const rspp_variant_A::RSPP_Detection_T& det = raw_detect_list.detections[det_idx];
         const F360_Detection_Props_T& det_prop = det_props[det_idx];

         if (det_prop.vcs_position.x > max_relevant_det_longpos)
         {
            f_continue = false;
         }
         else if (det_prop.vcs_position.x > min_relevant_det_longpos)
         {
            const int32_t sensor_idx = det.raw.sensor_id - 1;

            bool f_det_not_assoc_or_assoc_to_stat_obj;
            if (det_prop.object_track_id > 0)
            {
               f_det_not_assoc_or_assoc_to_stat_obj = (object_tracks[det_prop.object_track_id - 1].movable_prob < 0.5F);
            }
            else
            {
               f_det_not_assoc_or_assoc_to_stat_obj = true;
            }

            const float32_t longpos_diff = fabsf(det_prop.vcs_position.x - sensors[sensor_idx].constant.mounting_position.vcs_position.longitudinal);

            const bool f_valid = ((f_det_not_assoc_or_assoc_to_stat_obj) &&
               (longpos_diff < k_longpos_buffer) &&
               (det_prop.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS) &&
               (det.raw.rcs <= 7.0F) &&
               (det.raw.f_bistatic || det_prop.f_ok_to_use || (det_prop.on_sep_id > 0U)) &&
               (!det_prop.f_double_bounce) &&
               (!det_prop.f_water_spray) &&
               (!det_prop.f_stationary_bounce) &&
               (!det_prop.f_object_based_angle_jump) &&
               (!det_prop.f_azimuth_rdot_outlier));

            if (f_valid)
            {
               const float32_t abs_latpos = fabsf(det_prop.vcs_position.y);
               if (current_msmt_valid[sensor_idx])
               {
                  current_msmt[sensor_idx] = fminf(current_msmt[sensor_idx], abs_latpos);
               }
               else
               {
                  current_msmt_valid[sensor_idx] = true;
                  current_msmt[sensor_idx] = abs_latpos;
               }
            }
         }
         else
         {
            // Advance det_idx to get in range
         }
         det_idx = det.processed.next_sorted_idx;
      }

      /* Update buffer and create estimates */
      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         /* Update buffer */
         const int8_t idx = cwd_data.buffer_index[i];
         cwd_data.circular_buffer[i][idx] = current_msmt[i];
         cwd_data.buffer_index[i] = (idx + 1) % cwd_buffer_size;

         if (sensors[i].variable.is_valid &&
            (sensors[i].variable.number_of_valid_detections > 0U))
         {
            /* Create estimates */
            float32_t sum = 0.0F;
            float32_t num_valid_msmt = 0.0F;
            float32_t min_val = 100.0F;
            float32_t max_val = 0.0F;
            float32_t sum_age_conf = 0.0F;

            const float32_t age_weights[cwd_buffer_size] = { 1.0F, 0.8F, 0.6F, 0.4F, 0.2F };
            for (int8_t j = 0; j < cwd_buffer_size; j++)
            {
               const float32_t val = cwd_data.circular_buffer[i][j];
               if (val > 0.0F)
               {
                  sum += val;
                  num_valid_msmt += 1.0F;
                  min_val = fminf(val, min_val);
                  max_val = fmaxf(val, max_val);

                  const int8_t age_idx = (idx - j + cwd_buffer_size) % cwd_buffer_size; // the age index of current detection, assuming continuous updates
                  sum_age_conf += age_weights[age_idx];
               }
            }

            const float32_t lateral_spread = max_val - min_val;
            const float32_t k_max_lateral_spread = 1.0F;
            if ((num_valid_msmt > (static_cast<float32_t>(cwd_buffer_size) - 0.5F)) && (lateral_spread < k_max_lateral_spread))
            {
               const float32_t mean_latpos = sum / num_valid_msmt;
               const bool f_use_det = current_msmt_valid[i] && ((current_msmt[i] - mean_latpos) < 0.2F);
               sensor_estimate[i] = f_use_det ? current_msmt[i] : mean_latpos;

               const float32_t k_max_age_conf = 3.0F;
               const float32_t k_spread_weight = 3.0F;
               const float32_t age_conf = sum_age_conf / k_max_age_conf;
               const float32_t spread_conf = 1.0F / fmaxf(1.0F, lateral_spread * k_spread_weight);

               sensor_estimate_conf[i] = age_conf * spread_conf;
            }
            else
            {
               sensor_estimate[i] = 0.0F;
               sensor_estimate_conf[i] = 0.0F;
            }
         }
         else
         {
            sensor_estimate[i] = 0.0F;
            sensor_estimate_conf[i] = 0.0F;
         }
      }

      /* Fusion of sensor estimates */
      const float32_t side_sign[2] = { -1.0F, 1.0F };
      const int32_t output_map[2] = { 6, 7 }; // left 6, right 7
      for (int32_t i = 0; i < 2; i++)
      {
         float32_t sum_weighted_estimate = 0.0F;
         float32_t sum_weight = 0.0F;
         float32_t num_estimates = 0.0F;
         uint8_t min_estimate_idx = MAX_NUMBER_OF_SENSORS;

         float32_t max_estimate = -100.0F;
         float32_t min_estimate = 100.0F;
         float32_t max_sensor_longpos = -100.0F;
         float32_t min_sensor_longpos = 100.0F;

         for (uint8_t j = 0U; j < MAX_NUMBER_OF_SENSORS; j++)
         {
            if ((sensor_estimate[j] > 0.0F) &&
               ((sensors[j].constant.mounting_position.vcs_boresight_azimuth_angle * side_sign[i]) > 0.0F))
            {
               const float32_t estimate = sensor_estimate[j];
               const float32_t conf = sensor_estimate_conf[j];

               sum_weighted_estimate += estimate * conf;
               sum_weight += conf;
               num_estimates += 1.0F;

               max_estimate = fmaxf(max_estimate, estimate);
               if (estimate < min_estimate)
               {
                  min_estimate = estimate;
                  min_estimate_idx = j;
               }

               const float32_t longpos = sensors[j].constant.mounting_position.vcs_position.longitudinal;
               max_sensor_longpos = fmaxf(max_sensor_longpos, longpos);
               min_sensor_longpos = fminf(min_sensor_longpos, longpos);
            }
         }

         const float32_t k_min_host_speed = 1.5F;
         float32_t fused_estimate;
         float32_t fused_confidence;
         float32_t fused_longpos_ext_fwd;
         float32_t fused_longpos_ext_rear;
         if ((host.speed > k_min_host_speed) &&
            (sum_weight > 0.0F) &&
            (num_estimates > 0.0F) &&
            (min_estimate_idx < MAX_NUMBER_OF_SENSORS))
         {
            const float32_t k_max_estimate_diff = 0.6F;
            const float32_t k_min_single_sensor_conf = 0.60001F;
            if ((max_estimate - min_estimate) < k_max_estimate_diff)
            {
               fused_estimate = sum_weighted_estimate / sum_weight;
               fused_confidence = sum_weight / num_estimates;
               fused_longpos_ext_fwd = max_sensor_longpos;
               fused_longpos_ext_rear = min_sensor_longpos;
            }
            else if (sensor_estimate_conf[min_estimate_idx] > k_min_single_sensor_conf)
            {
               fused_estimate = sensor_estimate[min_estimate_idx];
               fused_confidence = sensor_estimate_conf[min_estimate_idx];
               fused_longpos_ext_fwd = sensors[min_estimate_idx].constant.mounting_position.vcs_position.longitudinal;
               fused_longpos_ext_rear = fused_longpos_ext_fwd;
            }
            else
            {
               fused_estimate = 0.0F;
               fused_confidence = 0.0F;
               fused_longpos_ext_fwd = 0.0F;
               fused_longpos_ext_rear = 0.0F;
            }
         }
         else
         {
            fused_estimate = 0.0F;
            fused_confidence = 0.0F;
            fused_longpos_ext_fwd = 0.0F;
            fused_longpos_ext_rear = 0.0F;
         }

         float32_t extended_dist;
         if (tracker_info.f_highway_suspected)
         {
            const float32_t min_ext = 40.0F;
            const float32_t max_ext = 70.0F;
            const float32_t min_speed = 17.0F;
            const float32_t max_speed = 25.0F;
            const float32_t coeff = (max_ext - min_ext) / (max_speed - min_speed);

            extended_dist = coeff * (host.speed - min_speed) + min_ext;
            extended_dist = fminf(fmaxf(extended_dist, min_ext), max_ext);
         }
         else
         {
            const float32_t min_ext = 2.0F;
            const float32_t max_ext = 10.0F;
            const float32_t min_speed = 2.77F;
            const float32_t max_speed = 13.88F;
            const float32_t coeff = (max_ext - min_ext) / (max_speed - min_speed);

            extended_dist = coeff * (host.speed - min_speed) + min_ext;
            extended_dist = fminf(fmaxf(extended_dist, min_ext), max_ext);
         }

         if (fused_estimate > 0.0F)
         {
            const float32_t stationkeeping_check_longpos_forward_thres = 0.0F; // Forward limit of longitudinal zone to check for true stationkeeping objects misclassified as CW
            const float32_t stationkeeping_check_longpos_rear_thres = -5.0F; // Rearward limit of longitudinal zone to check for true stationkeeping objects misclassified as CW

            const float32_t stationkeeping_check_latpos_moving_dets = 0.5F; // Lateral zone to check for moving dets to detect true stationkeeping objects misclassified as CW
            const float32_t stationkeeping_check_latpos_reduced_objs = 0.25F; // Lateral zone to check for downselected objects to detect true stationkeeping objects misclassified as CW

            /*loop over all the dets and if there are any dets with motion status moving within the 0.5m laterally of fused wall estimate and within 0 to - 5m longitudinally from vcs origin, then block CW creation*/
            for (uint32_t p = 0U; p < raw_detect_list.number_of_valid_detections; p++)
            {
               const F360_Detection_Props_T& det_prop = det_props[p];
               const float32_t lat_diff = fabsf(det_prop.vcs_position.y - (fused_estimate * side_sign[i]));
               if ((det_prop.vcs_position.x < stationkeeping_check_longpos_forward_thres) && 
                  (det_prop.vcs_position.x > stationkeeping_check_longpos_rear_thres) &&
                  (lat_diff < stationkeeping_check_latpos_moving_dets) &&
                  (det_prop.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING))
               {
                  fused_estimate = 0.0F;
                  fused_confidence = 0.0F;
                  fused_longpos_ext_fwd = 0.0F;
                  fused_longpos_ext_rear = 0.0F;
                  break;
               }
            }
            /* loop over all the object tracks and if there are any reduced tracks with vcs position within the 0.25m laterally of fused wall estimate and within 0 to - 5m longitudinally from vcs origin, then block CW creation */
            for (int32_t q = 0; q < tracker_info.num_active_objs; q++)
            {
               const F360_Object_Track_T& obj_trk = object_tracks[q];
               if (obj_trk.reduced_status > F360_OBJECT_STATUS_INVALID)
               {
                  const float32_t lat_diff = fabsf(obj_trk.vcs_position.y - (fused_estimate * side_sign[i]));
                  if ((obj_trk.vcs_position.x < stationkeeping_check_longpos_forward_thres) &&
                     (obj_trk.vcs_position.x > stationkeeping_check_longpos_rear_thres) &&
                     (lat_diff < stationkeeping_check_latpos_reduced_objs) &&
                     (obj_trk.movable_prob > 0.5))
                  {
                     fused_estimate = 0.0F;
                     fused_confidence = 0.0F;
                     fused_longpos_ext_fwd = 0.0F;
                     fused_longpos_ext_rear = 0.0F;
                     break;
                  }
               }
            }
         }

         /* Map to output */
         const int32_t out_idx = output_map[i];
         static_env_polys[out_idx].age = (fused_estimate > 0.0F) ? 1U : 0U;
         static_env_polys[out_idx].confidence = fused_confidence;
         static_env_polys[out_idx].upper_limit = fused_longpos_ext_fwd + extended_dist;
         static_env_polys[out_idx].lower_limit = fused_longpos_ext_rear - extended_dist;
         static_env_polys[out_idx].p0 = fused_estimate * side_sign[i];
         static_env_polys[out_idx].p1 = 0.0F;
         static_env_polys[out_idx].p2 = 0.5F * host.curvature_rear;
         static_env_polys[out_idx].status = (fused_estimate > 0.0F) ? F360_STATIC_ENV_POLY_STATUS_UPDATED : F360_STATIC_ENV_POLY_STATUS_INVALID;
         static_env_polys[out_idx].poly_type = F360_STATIC_ENV_POLY_TYPE_CWD;
      }

      timing_info.concrete_wall_detector = get_wall_time() - start_time;
   }
}
