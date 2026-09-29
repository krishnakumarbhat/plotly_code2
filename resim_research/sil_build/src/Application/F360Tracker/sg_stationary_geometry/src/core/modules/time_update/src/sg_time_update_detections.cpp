/*=============================================================================================*\
* FILE: sg_time_update_detections.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for time update detections function
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_time_update_detections.h"

#include "sg_time_update_utils.h"

namespace sg
{
   void time_update_detections(const float elapsed_time,
                               const float host_speed,
                               const HostProps &host_properties,
                               const Time_Update_Calibrations_T::Process_Noise_T &process_noise,
                               DetectionStorage &detections)
   {
      // Host sin_heading is taken with negative value because it is in VCS coordiante system. SG internally uses ISO.
      const float host_distance_traveled = host_speed * elapsed_time;
      const Host_Displacement_T host_displacement(host_distance_traveled, host_properties.cos_delta_pointing,
                                                  -host_properties.sin_delta_pointing);

      const Covariance_2D process_noise_covariance = calc_process_noise_covariance(host_displacement, process_noise);

      const auto detections_dummy_tail = detections.end();
      for (auto detection_it = detections.begin(); detection_it != detections_dummy_tail; ++detection_it)
      {
         auto &detection = *detection_it;
         update_position(-host_properties.sin_delta_pointing, host_properties.cos_delta_pointing, host_displacement,
                         detection.position);
         update_position_covariance(-host_properties.sin_delta_pointing, host_properties.cos_delta_pointing,
                                    process_noise_covariance, detection.position_cov);
         if (detection.segment_id[0U] != INVALID_SEGMENT_ID)
         {
            detection.vertex_age++;
         }
         detection.age++;
      }
      (void) process_noise_covariance; // MISRA
   }
}
