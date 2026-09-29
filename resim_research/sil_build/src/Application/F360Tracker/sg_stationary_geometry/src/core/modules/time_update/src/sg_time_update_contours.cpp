/*=============================================================================================*\
* FILE: sg_time_update_contours.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for time update contours function
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/


#include "sg_time_update_contours.h"

#include "sg_time_update_utils.h"

namespace sg
{
   void time_update_contours(const float elapsed_time,
                             const float host_speed,
                             const HostProps &host_properties,
                             const Time_Update_Calibrations_T::Process_Noise_T &process_noise,
                             const ContourStorage &contours)
   {
      // Host sin_heading is taken with negative value because it is in VCS coordiante system. SG internally uses ISO.
      const float host_distance_traveled = host_speed * elapsed_time;
      const Host_Displacement_T host_displacement(host_distance_traveled, host_properties.cos_delta_pointing,
                                                  -host_properties.sin_delta_pointing);

      const Covariance_2D process_noise_covariance = calc_process_noise_covariance(host_displacement, process_noise);

      for (auto &contour : contours)
      {
         for (auto &vertex : contour.vertices)
         {
            update_position(-host_properties.sin_delta_pointing, host_properties.cos_delta_pointing, host_displacement,
                            vertex.position);
            update_position_covariance(-host_properties.sin_delta_pointing, host_properties.cos_delta_pointing,
                                       process_noise_covariance, vertex.pos_cov);
            vertex.age += 1U;
            vertex.num_cycles_no_update += 1U;
         }
      }
      (void) process_noise_covariance; // MISRA
   }
}
