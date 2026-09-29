/*=============================================================================================*\
* FILE: sg_time_update_detections_utils.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for time update detections utility functions
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_TIME_UPDATE_DETECTIONS_UTILS_H
#define SG_TIME_UPDATE_DETECTIONS_UTILS_H

#include "geometry/geo_point.h"
#include "sg_basic.h"
#include "sg_calibrations.h"
#include "sg_host_displacement.h"

namespace sg
{
   /**
    * @brief            Update single detection position based on estimated host motion
    *
    * @param[in]        host_sin_pointing
    * @param[in]        host_cos_pointing
    * @param[in]        host_displacement
    * @param[in, out]   position
    *
    * @return           N/A
    **/
   void update_position(const float host_sin_pointing,
                        const float host_cos_pointing,
                        const Host_Displacement_T &host_displacement,
                        geometry::Point2D_T &position);

   /**
    * @brief            Update single detection covariance based on estimated process noise from host motion
    *
    * @param[in]        host_sin_pointing
    * @param[in]        host_cos_pointing
    * @param[in]        process_noise_covariance
    * @param[in, out]   position_covariance
    *
    * @return           N/A
    **/
   void update_position_covariance(const float host_sin_pointing,
                                   const float host_cos_pointing,
                                   const Covariance_2D &process_noise_covariance,
                                   Covariance_2D &position_covariance);

   /**
    * @brief            Calculate process noise covariance
    *
    * @param[in]        host_displacement
    * @param[in]        process_noise
    *
    * @return           covariance matrix
    **/
   inline Covariance_2D calc_process_noise_covariance(const Host_Displacement_T &host_displacement,
                                                      const Time_Update_Calibrations_T::Process_Noise_T &process_noise)
   {
      const float process_noise_lon = host_displacement.longitudinal() * process_noise.host_speed_weight_lon;
      const float process_noise_lat = host_displacement.lateral() * process_noise.host_speed_weight_lat;
      const float process_noise_std =
         process_noise.process_noise_base_std + (host_displacement.distance() * process_noise.host_speed_weight);

      Covariance_2D process_noise_covariance{};
      process_noise_covariance.x = (process_noise_lon * process_noise_lon) + (process_noise_std * process_noise_std);
      process_noise_covariance.y = (process_noise_lat * process_noise_lat) + (process_noise_std * process_noise_std);

      return process_noise_covariance;
   }
}
#endif
