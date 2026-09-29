/*===========================================================================*\
* FILE: f360_discard_unreliable_detections_in_clutter.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains functions definitions: Discard_Unreliable_Detections_In_Clutter().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_discard_unreliable_detections_in_clutter.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Discard_Unreliable_Detections_In_Clutter()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T &calib
   * const F360_Host_T &host
   * const F360_Tracker_Info_T &tracker_info
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS])
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
   * When host speed is above given threshold.
   * Function is looking for objects which may cause specific multipath, called
   * stationary bounce.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Discard_Unreliable_Detections_In_Clutter(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Radar_Sensor_T& sensor,
      F360_Detection_Props_T& det_prop)
   {
      if(tracker_info.f_low_power_clutter)
      {
         const int8_t sensor_type = sensor.constant.sensor_type;
         const bool f_gen7_sensor = ((sensor_type == F360_SENSOR_TYPE_SRR7_PLUS_RADAR) ||
            (sensor_type == F360_SENSOR_TYPE_FLR7_RADAR) ||
            (sensor_type == F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR) ||
            (sensor_type == F360_SENSOR_TYPE_FLR7_PLT_RADAR) ||
            (sensor_type == F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR) ||
            (sensor_type == F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR));

         const float32_t max_vcs_longpos = f_gen7_sensor ? 100.0F : 75.0F;
         const float32_t min_vcs_longpos = f_gen7_sensor ? -100.0F : 0.0F;
         constexpr float32_t max_vcs_latpos = 30.0F;
         constexpr float32_t detection_elevation_threshold = F360_DEG2RAD(10.0F);
         constexpr int8_t detection_az_confid_threshold = 3;
         constexpr float32_t min_detection_rcs = -10.0F;

         float32_t max_detection_rcs = 5.0F;
         float32_t detection_snr_threshold_high = 13.0F;
         float32_t detection_snr_threshold_low = 10.0F;

         // Adjust thresholds for very close detections
         if ((rspp_det.raw.range < 20.0F) && (f_gen7_sensor))
         {
            max_detection_rcs = 0.0F;
            detection_snr_threshold_high = 10.0F;
            detection_snr_threshold_low = 8.0F;
         }

         const bool f_det_in_roi = ((det_prop.vcs_position.x < max_vcs_longpos) &&
            (det_prop.vcs_position.x > min_vcs_longpos) &&
            (fabsf(det_prop.vcs_position.y) < max_vcs_latpos));

         const bool f_det_props_match_hypothesis = (rspp_det.raw.rcs < min_detection_rcs) ||
            (rspp_det.raw.snr < detection_snr_threshold_high) ||
            (rspp_det.raw.confid_azimuth >= detection_az_confid_threshold) ||
            (fabsf(rspp_det.raw.elevation) > detection_elevation_threshold);

         if (f_det_in_roi &&
            (rspp_det.raw.rcs < max_detection_rcs) &&
            f_det_props_match_hypothesis)
         {
            det_prop.f_unreliable_in_clutter = true; // don't use in clusters for initialization
            constexpr float32_t min_detection_range = 8.0F;
            if (((rspp_det.raw.confid_azimuth >= detection_az_confid_threshold) || (rspp_det.raw.snr < detection_snr_threshold_low)) &&
               (rspp_det.raw.range > min_detection_range))
            {
               det_prop.f_ok_to_use = false; // don't use az conf 3 or low snr anywhere
            }
         }
      }
   }
}
