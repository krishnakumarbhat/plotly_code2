/*===========================================================================*\
* FILE: f360_check_for_low_power_clutter.cpp
*============================================================================
* Copyright (C) 2019-2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains funcions which may flag detections as rain clutter.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#include "f360_math.h"
#include "f360_check_for_low_power_clutter.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
    * FUNCTION: Check_For_Low_Power_Clutter
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Host_T& host
    * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
    * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list
    * F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS]
    * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
    * F360_Tracker_Info_T& tracker_info
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
    * Function aims to reduce number of moving ghost objects in rainy conditions.
    * SRR6p detections in the longitudinal and lateral position gate have their 
    * properties matched against the hypothesis. If the check passes the tracker
    * info sets a low_power_clutter flag indicating rainy conditions.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   void Check_For_Low_Power_Clutter(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T& tracker_info)
   {
      constexpr float32_t max_vcs_longpos_threshold = 50.0F;
      bool f_done = (host.speed < 0.0F);
      int32_t det_idx = 0;
      uint32_t num_matching_dets[MAX_NUMBER_OF_SENSORS] = { 0U };
      uint32_t num_matching_inner_dets[MAX_NUMBER_OF_SENSORS] = { 0U };

      low_power_clutter_thresholds thresholds{};

      if (!f_done)
      {
         det_idx = Get_First_Relevant_Long_Sorted_Det_Idx(-max_vcs_longpos_threshold, raw_detect_list);
         f_done = (det_idx < 0);
      }

      if (!f_done)
      {
         const uint32_t number_of_valid_detections = raw_detect_list.number_of_valid_detections;
         for (uint32_t i = 0U; i < number_of_valid_detections; i++)
         {
            const int32_t sensor_id = raw_detect_list.detections[det_idx].raw.sensor_id;
            const int8_t sensor_type = sensors[sensor_id - 1].constant.sensor_type;
            Set_Sensor_Dependent_Thresholds(sensor_type, thresholds);

            const bool f_det_props_match_hypothesis = Detection_Props_Match_Hypothesis(raw_detect_list.detections[det_idx], detection_props[det_idx], thresholds);

            const bool f_gen7_sensor = thresholds.f_flr7_sensor || thresholds.f_srr7plus_sensor;
            
            constexpr float32_t min_vcs_longpos_threshold = 8.0F;
            const bool f_det_in_roi = Is_Det_In_ROI(min_vcs_longpos_threshold, max_vcs_longpos_threshold, detection_props[det_idx], f_gen7_sensor);

            if (f_det_in_roi && f_det_props_match_hypothesis)
            {
               constexpr float32_t max_vcs_latpos_inner_threshold = 1.0F;
               num_matching_dets[sensor_id - 1] = num_matching_dets[sensor_id - 1] + 1U;
               if (std::abs(detection_props[det_idx].vcs_position.y) < max_vcs_latpos_inner_threshold)
               {
                  num_matching_inner_dets[sensor_id - 1] = num_matching_inner_dets[sensor_id - 1] + 1U;
               }
            }
            det_idx = raw_detect_list.detections[det_idx].processed.next_sorted_idx;
            if ((det_idx < 0) || (detection_props[det_idx].vcs_position.x > max_vcs_longpos_threshold))
            {
               break;
            }
         }
      }
      Check_Radar_Rain_Flag(sensors, sensor_props, tracker_info);
      tracker_info.low_power_clutter_severity = Update_Max_Severity_Value(num_matching_dets, num_matching_inner_dets, sensors, sensor_props);
      const float32_t max_severity = (thresholds.f_flr7_sensor || thresholds.f_srr7plus_sensor) ? std::min(tracker_info.low_power_clutter_severity, tracker_info.rain_level_filtered) : tracker_info.low_power_clutter_severity;
      constexpr float32_t power_clutter_upper_boundary = 0.65F;
      constexpr float32_t power_clutter_lower_boundary = 0.3F;
      tracker_info.f_low_power_clutter = F360_Hysteresis(
         max_severity,
         tracker_info.f_low_power_clutter,
         power_clutter_upper_boundary,
         power_clutter_lower_boundary);
   }

   /*===========================================================================*\
    * FUNCTION: Is_Det_In_ROI
    *===========================================================================
    * RETURN VALUE:
    * bool
    *
    * PARAMETERS:
    * const float32_t min_vcs_longpos_threshold
    * const float32_t max_vcs_longpos_threshold
    * const F360_Detection_Props_T& detection_prop
    * const bool f_check_abs_longpos
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
    * Function checks if detection is in the zone of interest and should be considered
    * in further analysis.
    *
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   bool Is_Det_In_ROI(
      const float32_t min_vcs_longpos_threshold,
      const float32_t max_vcs_longpos_threshold,
      const F360_Detection_Props_T& detection_prop,
      const bool f_check_abs_longpos)
   {
      constexpr float32_t max_vcs_latpos_threshold = 3.0F;
      constexpr float32_t latpos_thresh_factor = 6.0F / 50.0F; //defines a cone 6m lateral at 50m longitudinal
      const float variable_latpos_thresh = latpos_thresh_factor * detection_prop.vcs_position.x;

      const bool f_in_longzone = f_check_abs_longpos ? (std::abs(detection_prop.vcs_position.x) < max_vcs_longpos_threshold) && (std::abs(detection_prop.vcs_position.x) > min_vcs_longpos_threshold) :
         (detection_prop.vcs_position.x < max_vcs_longpos_threshold) && (detection_prop.vcs_position.x > min_vcs_longpos_threshold);
      const bool f_in_latzone = (std::abs(detection_prop.vcs_position.y) < max_vcs_latpos_threshold) || (std::abs(detection_prop.vcs_position.y) < variable_latpos_thresh);
      return f_in_latzone && f_in_longzone;
   }

   /*===========================================================================*\
    * FUNCTION: Detection_Props_Match_Hypothesis
    *===========================================================================
    * RETURN VALUE:
    * bool
    *
    * PARAMETERS:
    * const rspp_variant_A::RSPP_Detection_T& detection,
    * const F360_Detection_Props_T& detection_prop,
    * const low_power_clutter_thresholds& thresholds
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
    * Function checks if detection properties matches rain clutter hypothesis.
    *
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   bool Detection_Props_Match_Hypothesis(
      const rspp_variant_A::RSPP_Detection_T& detection,
      const F360_Detection_Props_T& detection_prop,
      const low_power_clutter_thresholds& thresholds)
   {

      bool f_det_props_match_hypothesis = false;
      if(thresholds.f_gen6_sensor)
      {
         constexpr int8_t detection_az_confid_higher = 2;

         const bool f_detection_rcs_az_confid_matches_hypothesis = (detection.raw.rcs < thresholds.max_rcs) && (detection.raw.confid_azimuth >= detection_az_confid_higher);
         const bool f_detection_az_confid_matches_hypothesis = (detection.raw.confid_azimuth >= thresholds.detection_az_confid);

         f_det_props_match_hypothesis = (f_detection_az_confid_matches_hypothesis || f_detection_rcs_az_confid_matches_hypothesis);
      }
      else if(thresholds.f_flr7_sensor)
      {
         const bool f_detection_rcs_snr_matches_hypothesis = ((detection.raw.rcs < thresholds.max_rcs) && (detection.raw.snr < thresholds.max_snr));
         const bool f_detection_moving = detection_prop.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

         f_det_props_match_hypothesis = f_detection_rcs_snr_matches_hypothesis && f_detection_moving;
      }
      else if (thresholds.f_srr7plus_sensor)
      {
         const bool f_detection_rcs_snr_matches_hypothesis = ((detection.raw.rcs < thresholds.max_rcs) && (detection.raw.snr < thresholds.max_snr));
         const bool f_detection_moving = detection_prop.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         const bool f_detection_az_confid_matches_hypothesis = ((detection.raw.confid_azimuth >= thresholds.detection_az_confid) && (detection.raw.snr < thresholds.max_snr));

         f_det_props_match_hypothesis = (f_detection_rcs_snr_matches_hypothesis || f_detection_az_confid_matches_hypothesis) && f_detection_moving;
      }
      else
      {
         // do nothing
      }
      return f_det_props_match_hypothesis;
   }

   /*===========================================================================*\
    * FUNCTION: Update_Max_Severity_Value
    *===========================================================================
    * RETURN VALUE:
    * float
    *
    * PARAMETERS:
    * const uint32_t(&num_matching_dets)[MAX_NUMBER_OF_SENSORS],
    * const uint32_t(&num_matching_inner_dets)[MAX_NUMBER_OF_SENSORS],
    * F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS]
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
    * Function updates the maximum severity value.
    *
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   float Update_Max_Severity_Value(
      const uint32_t(&num_matching_dets)[MAX_NUMBER_OF_SENSORS],
      const uint32_t(&num_matching_inner_dets)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS])
   {
      float32_t ref = 0.0F;
      float32_t max_severity = 0.0F;

      for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         if (sensors[i].variable.is_valid)
         {
            uint32_t k_severity_level_low = 4U;
            uint32_t k_severity_level_mid = 6U;
            uint32_t k_severity_level_high = 8U;

            uint32_t k_inner_severity_level_low = 1U;
            uint32_t k_inner_severity_level_high = 2U;

            const int8_t sensor_type = sensors[i].constant.sensor_type;
            if ((sensor_type == F360_SENSOR_TYPE_FLR7_RADAR) ||
               (sensor_type == F360_SENSOR_TYPE_FLR7_PLT_RADAR) ||
                (sensor_type == F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR))
            {
               k_severity_level_low += 2U;
               k_severity_level_mid += 3U;
               k_severity_level_high += 4U;
               k_inner_severity_level_low += 1U;
               k_inner_severity_level_high += 2U;
            }
            if ((num_matching_dets[i] >= k_severity_level_high) && (num_matching_inner_dets[i] >= k_inner_severity_level_high))
            {
               ref = 1.0F;
            }
            else if ((num_matching_dets[i] >= k_severity_level_mid) && (num_matching_inner_dets[i] >= k_inner_severity_level_low))
            {
               ref = 0.8F;
            }
            else if ((num_matching_dets[i] >= k_severity_level_low) && (num_matching_inner_dets[i] >= k_inner_severity_level_low))
            {
               ref = 0.6F;
            }
            else
            {
               ref = 0.0F;
            }
            const float32_t alpha = ref < sensor_props[i].low_power_clutter_severity ? 0.995F : 0.95F;
            sensor_props[i].low_power_clutter_severity = sensor_props[i].low_power_clutter_severity * alpha + ref * (1.0F - alpha);
            max_severity = std::max(max_severity, sensor_props[i].low_power_clutter_severity);
         }
      }
      return max_severity;
   }

   /*===========================================================================*\
    * FUNCTION: Check_Radar_Rain_Flag
    *===========================================================================
    * RETURN VALUE:
    * bool
    *
    * PARAMETERS:
    * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
    * F360_Tracker_Info_T& tracker_info
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
    * Function calculates average vlue of radar rain flag from all sensors and
    * filters it through a low-pass filter.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   void Check_Radar_Rain_Flag(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info
   )
   {
      float32_t rain_level_sum = 0.0F;
      uint32_t n_supported_sensors = 0U;
      constexpr float32_t alpha = 0.95F;
      for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         if (sensors[i].variable.is_valid)
         {
            sensor_props[i].rain_level_filtered = alpha * sensor_props[i].rain_level_filtered + (1.0F - alpha) * static_cast<float32_t>(sensors[i].variable.overall_rain_level);
            rain_level_sum += sensor_props[i].rain_level_filtered;
            n_supported_sensors++;
         }
      }

      float32_t rain_level_avg = 0.0F;
      if (n_supported_sensors > 0U)
      {
         constexpr float32_t max_rain_level_inv = 0.5F; // max rain level is 2
         rain_level_avg = (rain_level_sum * max_rain_level_inv) / static_cast<float32_t>(n_supported_sensors);
      }

      tracker_info.rain_level_filtered = rain_level_avg;
   }

   /*===========================================================================*\
    * FUNCTION: Set_Sensor_Dependent_Thresholds
    *===========================================================================
    * RETURN VALUE:
    * None.
    *
    * PARAMETERS:
    * const int8_t sensor_type,
    * low_power_clutter_thresholds& thresholds
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
    * Sets threshold used in low power clutter calculation based on the type of the sensor.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
   \*===========================================================================*/
   void Set_Sensor_Dependent_Thresholds(
      const int8_t sensor_type,
      low_power_clutter_thresholds& thresholds
   )
   {
      thresholds.f_flr7_sensor = false;
      thresholds.f_srr7plus_sensor = false;
      thresholds.f_gen6_sensor = false;
      thresholds.detection_az_confid = 3;

      if ((sensor_type == F360_SENSOR_TYPE_FLR7_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_FLR7_PLT_RADAR) ||
          (sensor_type == F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR))
      {
         thresholds.f_flr7_sensor = true;
         thresholds.max_rcs = -10.0F;
         thresholds.max_snr = 10.0F;
      }

      else if((sensor_type == F360_SENSOR_TYPE_SRR7_PLUS_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR) ||
          (sensor_type == F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR))
      {
         thresholds.f_srr7plus_sensor = true;
         thresholds.max_rcs = -10.0F;
         thresholds.max_snr = 10.0F;
      }

      else if((sensor_type == F360_SENSOR_TYPE_SRR6_PLUS_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR) ||
         (sensor_type == F360_SENSOR_TYPE_FLR4_PLT_RADAR))
      {

         thresholds.f_gen6_sensor = true;
         thresholds.max_rcs = -20.0F;
         thresholds.max_snr = 13.0F;
      }
      else
      {
         // do nothing
      }
   }
   
}
