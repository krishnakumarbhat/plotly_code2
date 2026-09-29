/*===================================================================================*\
* FILE:  f360_detect_multipath.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of functions declared in f360_detect_multipath.h
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_constants.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_timing_info.h"
#include "f360_multipath_detector.h"
#include "f360_visibility_info.h"
#include "f360_detect_multipath.h"
#include "f360_range_rates.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Check_If_Object_Is_MP_For_Any_Sensor()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_marked_as_multipath - True if object is multipath
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Tracker_Info_T & tracker_info
   * F360_Object_Track_T &object
   * Multipath_Detector &mp_detector
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
   * Check for all sensors if object that is approaching host and is in field of view
   * of a sensor is multipath, returns flag indicating if object is multipath.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_If_Object_Is_MP_For_Any_Sensor(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T& object,
      Multipath_Detector &mp_detector)
   {
      const Point object_position = object.vcs_position;
      const float32_t forgetting_factor_alpha = 0.98F;

      // forgetting factor alpha is introduced to consider only the history of last 7 seconds for tracker convergence
      object.time_since_initialization_with_forgetting_factor = object.time_since_initialization_with_forgetting_factor * forgetting_factor_alpha + tracker_info.elapsed_time_s;
      object.number_of_events_of_multipath_with_forgetting_factor = object.number_of_events_of_multipath_with_forgetting_factor * forgetting_factor_alpha;

      for (uint32_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         if (sensors[sensor_idx].variable.is_valid && Is_VCS_Point_Within_Current_FOV_Of_Sensor(object_position, sensors[sensor_idx], sensors[sensor_idx].variable.look_id))
         {
            const F360_Sensor_Mounting_Position_T & sensor_position = sensors[sensor_idx].constant.mounting_position;
            const Point sensor_position_2d = { sensor_position.vcs_position.longitudinal, sensor_position.vcs_position.lateral };
            const float32_t object_range_rate = Calculate_Projected_Range_Rate(sensor_position_2d, object_position, object.vcs_velocity);

            // Negative object_range_rate implies that object gets closer to the host
            if ((object_range_rate < 0.0F) && mp_detector.Is_Multipath(sensor_position_2d, object, object_range_rate, tracker_info))
            {
                object.number_of_events_of_multipath_with_forgetting_factor = object.number_of_events_of_multipath_with_forgetting_factor + 1.0F;
            }
         }
      }
      // poisson_estimated_lambda is a bayesian inference estimate with a non informative gamma prior, given by the formula (1+number_of_events_of_multipath_with_forgetting_factor)/(1+time_since_initialization_with_forgetting_factor)
      // division by zero is not possible because object.time_since_initialization_with_forgetting_factor is always positive > 0
      const float32_t poisson_estimated_lambda = (1.0F + object.number_of_events_of_multipath_with_forgetting_factor) / (1.0F + object.time_since_initialization_with_forgetting_factor);
      bool f_marked_as_multipath = false;

      // This threshold stands for 3.4 events per unit time of multipath events reported by the MP algorithm
      const float32_t lambda_threshold = 3.4F;

      if (poisson_estimated_lambda > lambda_threshold)
      {
          f_marked_as_multipath = true;
      }
      return f_marked_as_multipath;
   }


   /*===========================================================================*\
   * FUNCTION: Detect_And_Mark_Multipath_Objects()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_marked_as_multipath - True if object is multipath
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info
   * const F360_Calibrations_T & calibrations
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS]
   * F360_Object_Track_T& object
   * Multipath_Detector & mp_detector
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
   * Checks if any of active objects is multipath candidate, then checks if candidate
   * is confirmed as multipath by any sensor. If so - marks object as multipath and
   * sets default mirror probability for that object.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Detect_And_Mark_Multipath_Object(const F360_Host_T & host,
      const F360_Tracker_Info_T & tracker_info,
      const F360_Calibrations_T & calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T& object,
      Multipath_Detector & mp_detector)
   {
      bool f_marked_as_multipath = false;
      if (std::abs(host.speed) < calibrations.k_mp_max_allowed_host_speed_to_use_MP)
      {
         const bool f_is_multipath_candidate = object.f_moving &&
             ( (std::abs(object.speed) > calibrations.fast_moving_thresh));

         if (f_is_multipath_candidate)
         {
            f_marked_as_multipath = Check_If_Object_Is_MP_For_Any_Sensor(sensors, tracker_info, object, mp_detector);
            if (f_marked_as_multipath)
            {
                object.mirror_prob = calibrations.k_mp_default_mirror_probability;
            }
         }
      }
      return f_marked_as_multipath;
   }
}
