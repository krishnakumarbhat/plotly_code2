/*===========================================================================*\
* FILE: f360_mark_high_elevation_detection.cpp
*============================================================================
* Copyright (C) 2023 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_constants.h"
#include "f360_mark_high_elevation_detection.h"
#include "f360_math_func.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Should_Detection_Be_Processed()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const rspp_variant_A::RSPP_Detection_T& detection
   *   const F360_Detection_Props_T& detection_prop
   *   const F360_Sensor_Type_T sensor_type
   *   const float32_t host_vcs_speed
   *   const float32_t k_mrr3_max_range
   *   const float32_t k_min_host_speed_for_check_det_az_conf_and_elevation
   *   const int8_t k_mrr3_conf_thresh
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
   * Function determines if detection should be processed.based on detection motion status, azimuth confidence and detection range.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Should_Detection_Be_Processed(
      const rspp_variant_A::RSPP_Detection_T& detection,
      const F360_Detection_Props_T& detection_prop,
      const F360_Sensor_Type_T sensor_type,
      const float32_t host_vcs_speed,
      const float32_t k_mrr3_max_range,
      const float32_t k_min_host_speed_for_check_det_az_conf_and_elevation,
      const int8_t k_mrr3_conf_thresh)
   {
      const bool f_ok_sensor_type =  (F360_SENSOR_TYPE_MRR3_RADAR == sensor_type);
      const bool f_host_speed_ok = (k_min_host_speed_for_check_det_az_conf_and_elevation < host_vcs_speed);
      const bool f_det_is_moving = (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == detection_prop.motion_status);
      const bool f_azimuth_confidence_above_threshold = (k_mrr3_conf_thresh <= detection.raw.confid_azimuth);
      const bool f_correct_detection_range = (k_mrr3_max_range >= detection.raw.range);

      return  (f_ok_sensor_type && f_host_speed_ok && f_azimuth_confidence_above_threshold && f_correct_detection_range && f_det_is_moving);
   }

   /*===========================================================================*\
   * FUNCTION: Process_High_Elevation_Detection_Mrr3()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const int8_t &k_mrr3_conf_thresh
   *   const float32_t &k_mrr3_max_abs_elev_angle
   *   const rspp_variant_A::RSPP_Detection_T &detection
   *   F360_Detection_Props_T& detection_prop
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
   * Function processes high elevation MRR3 detection. If detection elevation is above threshold, it marks detection as not ok to use.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Process_High_Elevation_Detection_Mrr3(
      const int8_t& k_mrr3_conf_thresh,
      const float32_t& k_mrr3_max_abs_elev_angle,
      const rspp_variant_A::RSPP_Detection_T& detection,
      F360_Detection_Props_T& detection_prop)
   {
      const bool elevation_too_big = (detection.raw.confid_azimuth > k_mrr3_conf_thresh) ||
         (std::abs(detection.raw.elevation) > k_mrr3_max_abs_elev_angle);

      if (elevation_too_big)
      {
         detection_prop.f_ok_to_use = false;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_High_Elevation_Detection()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const F360_Radar_Sensor_T& sensor
   *   const F360_Calibrations_T& f360_calib
   *   const float32_t host_vcs_speed
   *   const rspp_variant_A::RSPP_Detection_T& detection
   *   F360_Detection_Props_T& detection_prop
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
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_High_Elevation_Detection(
      const F360_Radar_Sensor_T& sensor,
      const F360_Calibrations_T& f360_calib,
      const float32_t host_vcs_speed,
      const rspp_variant_A::RSPP_Detection_T& detection,
      F360_Detection_Props_T& detection_prop)
   {
      const bool f_process_detection = Should_Detection_Be_Processed(
         detection, detection_prop, sensor.constant.sensor_type, host_vcs_speed, f360_calib.k_mrr3_max_range,
         f360_calib.k_min_host_speed_for_check_det_az_conf_and_elevation, static_cast<int8_t>(f360_calib.k_mrr3_conf_thresh));

      if (f_process_detection)
      {
         Process_High_Elevation_Detection_Mrr3(static_cast<int8_t>(f360_calib.k_mrr3_conf_thresh),
            f360_calib.k_mrr3_max_abs_elev_angle, detection, detection_prop);
      }
   }
}
