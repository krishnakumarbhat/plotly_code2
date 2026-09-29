/*=============================================================================================*\
* FILE: sg_downselect_input_detections_helpers.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains functions used locally for downselection of input detections.
*
*
*  Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_DOWNSELECT_INPUT_DETECTIONS_HELPERS_H
#define SG_DOWNSELECT_INPUT_DETECTIONS_HELPERS_H

#include "sg_calibrations.h"
#include "sg_input.h"

namespace sg
{
   /**
    * @brief          Checks if detection azimuth confidence is below given threshold.
    *
    * @param[in]      azimuth_confidence
    * @param[in]      maximum_valid_azimuth_confidence_value
    * @return         bool
    **/
   inline bool is_azimuth_confidence_sufficient(const int8_t azimuth_cofidence, const int8_t maximum_valid_azimuth_confidence_value)
   {
      return (azimuth_cofidence <= maximum_valid_azimuth_confidence_value);
   }

   /**
    * @brief         Checks if a detection with given range_rate_compensated and motion_status
    *                should be treated as stationary.
    *
    * @param[in]     range_rate_compensated
    * @param[in]     motion_status
    * @param[in]     max_range_rate_compensated
    * @return        result - bool
    **/
   inline bool is_detection_stationary(const float range_rate_compensated,
                                       const rspp::RSPP_Detection_Motion_Status_T motion_status,
                                       const float max_range_rate_compensated)
   {
      bool result = false;
      if ((std::fabs(range_rate_compensated) < max_range_rate_compensated)
          && (motion_status != rspp::RSPP_DETECTION_MOTION_STATUS_MOVING))
      {
         result = true;
      }
      return result;
   }

   /**
    * @brief         Checks if probability of detection is at least equal
                     to accepted probability level.
    *
    * @param[in]     probability_of_detection
    * @param[in]     accepted_probability_level
    * @return        bool
    **/
   inline bool is_probability_accepted(const float probability_of_detection, const float accepted_probability_level)
   {
      return (probability_of_detection >= accepted_probability_level);
   }

   /**
    * @brief         Checks if a detection lays within given region of interest.
    *
    * @param[in]     x - longitudinal coordinate
    * @param[in]     y - lateral coordinate
    * @param[in]     z - vertical coordinate
    * @param[in]     region_of_interest
    * @return        result - bool
    **/
   inline bool is_within_region_of_interest(const float x, const float y, const float z, const View_Range_T &region_of_interest)
   {
      return (((region_of_interest.longitudinal.min <= x) && (x <= region_of_interest.longitudinal.max))
              && ((region_of_interest.lateral.min <= y) && (y <= region_of_interest.lateral.max))
              && (((region_of_interest.vertical.overground.min <= z) && (z <= region_of_interest.vertical.overground.max))
                  || ((region_of_interest.vertical.underground.min <= z) && (z <= region_of_interest.vertical.underground.max))));
   }

   /**
    * @brief        Checks if detection is close to host and host speed is below threshold
    *
    * @param[in]    host_speed
    * @param[in]    det_position_x
    * @param[in]    max_host_speed_for_poor_azim_confid_det
    * @param[in]    max_distance_for_poor_azim_confid_det
    *
    * @return       result - bool
    **/
   inline bool is_poor_azimuth_confidence_allowed(const float host_speed,
                                                  const float det_position_x,
                                                  const float max_host_speed_for_poor_azim_confid_det,
                                                  const float max_distance_for_poor_azim_confid_det)
   {
      return ((host_speed < max_host_speed_for_poor_azim_confid_det) && (det_position_x < max_distance_for_poor_azim_confid_det));
   }
}

#endif
