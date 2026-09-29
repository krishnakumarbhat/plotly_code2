/*===========================================================================*\
* FILE: f360_identify_relevant_low_azimuth_confidence_dets.cpp
*============================================================================
* Copyright (C) 2019-2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains funcions which idetenify relevant low azimuth confidence detections
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#include "f360_math.h"
#include "f360_try_to_dealiase_range_rate.h"
#include "f360_identify_relevant_low_azimuth_confidence_dets.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Low_Az_Conf_Countermeassure_For_SRR7plus()
   *===========================================================================
   * RETURN VALUE:
   * Bool
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T& rspp_det
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
   * This function sets the f_low_az_conf_det flag for moving low azimuth confidence detections.
   * The flag can be set for SRR7plus low az conf detections that are within ROI around host.
   * or in general for SRR7plus low az conf detections that are not directly in front or behind host.
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
   static bool Low_Az_Conf_Countermeassure_For_SRR7plus(const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Detection_Props_T& det_prop)
   {
      bool f_low_az_conf_det = false;

      if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_prop.motion_status)
      {
         //If det is from SRR7plus, is moving, is ok to use, and not in front or behind host (based on azimuth), then set the flag
         constexpr float32_t max_valid_azimuth_in_front_of_host = 0.3491F; // 20.0 degrees;
         constexpr float32_t min_valid_azimuth_behind_of_host = 2.7925F; // 160.0 degrees;

         // Check if vcs_az is greater than 20 degrees and less than 90 degrees
         const bool f_det_not_in_front_of_host = ((std::abs(rspp_det.processed.vcs_az) > max_valid_azimuth_in_front_of_host) && (std::abs(rspp_det.processed.vcs_az) < 1.5708F));
         // Check if vcs_az is less than 160 degrees and greater than 90 degrees
         const bool f_det_not_behind_host = ((std::abs(rspp_det.processed.vcs_az) < min_valid_azimuth_behind_of_host) && (std::abs(rspp_det.processed.vcs_az) > 1.5708F));

         if ((f_det_not_behind_host) || (f_det_not_in_front_of_host))
         {
            f_low_az_conf_det = true;
         }
         else
         {
            // If the detection is in ROI near host and has low az conf, is from SRR7plus, is moving, then set the flag
            // Set up ROI near host
            constexpr float32_t max_ROI_long_threshold = 30.0F;
            constexpr float32_t min_ROI_long_threhold = -35.0F;
            constexpr float32_t abs_ROI_lat_threshold = 7.0F;

            if ((det_prop.vcs_position.x < max_ROI_long_threshold)
               && (det_prop.vcs_position.x > min_ROI_long_threhold)
               && (std::abs(det_prop.vcs_position.y) < abs_ROI_lat_threshold))
            {
               f_low_az_conf_det = true;
            }
         }
      }
      return f_low_az_conf_det;
   }

   /*===========================================================================*\
   * FUNCTION: Handle_Low_Az_Conf_Detections_Near_Host()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T sensor
   * const rspp_variant_A::RSPP_Detection_T& rspp_det
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
   * This function sets the f_low_az_conf_det flag and f_ok_to_use flag for low azimuth confidence and low elevation confidence detections
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
   void Handle_Low_Az_Conf_Detections(
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      F360_Detection_Props_T& det_prop)
   {
      bool f_low_az_conf_det = false;
      const bool f_low_az_conf_rspp = (rspp_det.raw.confid_azimuth == rspp_variant_A::RSPP_CONF_AZIMUTH_LOW);
      const bool f_low_el_conf_rspp = (rspp_det.raw.confid_elevation == 3);

      if ((f_low_az_conf_rspp || f_low_el_conf_rspp) && (det_prop.f_ok_to_use))
      {
         const bool f_srr7_plus_sensor = ((F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR == sensor.constant.sensor_type) ||
            (F360_SENSOR_TYPE_SRR7_PLUS_RADAR == sensor.constant.sensor_type) ||
            (F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR == sensor.constant.sensor_type));

         const bool f_flr7_sensor = ((F360_SENSOR_TYPE_FLR7_PLT_RADAR == sensor.constant.sensor_type) ||
            (F360_SENSOR_TYPE_FLR7_RADAR == sensor.constant.sensor_type) ||
            (F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR == sensor.constant.sensor_type));

         if ((f_srr7_plus_sensor) && (f_low_az_conf_rspp))
         {
            f_low_az_conf_det = Low_Az_Conf_Countermeassure_For_SRR7plus(rspp_det, det_prop);
         }
         else if (f_flr7_sensor)
         {
            // All low azimuth confidence and low elevation confidence FLR detections are classified as f_low_az_conf_det
            f_low_az_conf_det = true;
         }
         else
         {
            f_low_az_conf_det = false; // For other sensors, this should be false by default
         }
      }
      det_prop.f_low_az_conf_det = f_low_az_conf_det;
   }
}
