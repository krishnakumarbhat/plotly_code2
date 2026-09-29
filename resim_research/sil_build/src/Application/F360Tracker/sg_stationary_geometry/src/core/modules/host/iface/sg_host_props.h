/*===================================================================================*\
* FILE: sg_host_props.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains the host property structure declaration and
*   the function signiture(s) that work on the structure.
*
* Applicable Standards (in order of precedence: highest first):
*   ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards" [May 26, 2019]
*   ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef SG_HOST_PROPS_H
#define SG_HOST_PROPS_H

#include "geometry/geo_point.h"
#include "rspp_host.h"

namespace sg
{
   struct HostProps
   {
      geometry::Point2D_T position{};

      struct
      {
         float x;
         float y;
      } delta_position; // Delta for host position between previous and current tracker iteration [m]

      float vel_cov_scm[2][2];          // Covariance of host front center velocity vector (in WCS) [(m/s)^2]
      float vel_cov[2][2];              // Covariance of velocity
      float position_inc_cov_scm[2][2]; // Covariance of position incrementation vector (in WCS) [m^2]
      float position_inc_cov[2][2];     // Covariance of position incrementation.
      float std_speed_scm;              // Standard deviation of host speed at center of rear axle [m/s]
      float std_yaw_rate_scm;           // Standard deviation of host yaw rate [rad/s]
      float heading_angle;
      float cos_heading;
      float sin_heading;
      float delta_pointing;     // Delta for host pointing angle between previous and current tracker iteration
      float cos_delta_pointing; // Cosine of delta for host pointing angle between previous and current tracker iteration
      float sin_delta_pointing; // Sine of delta for host pointing angle between previous and current tracker iteration
   };

   /**
    * @brief      Reset all host data
    *
    * @return     void
    **/
   void reset_host_properties(HostProps &host_properties);

   void calculate_host_properties(const float elapsed_time, const RSPP_Host_T &host, HostProps &host_properties);

   static_assert(112 == sizeof(HostProps), "sizeof(HostProps) not as expected. Remember to align padding if needed");
}

#endif
