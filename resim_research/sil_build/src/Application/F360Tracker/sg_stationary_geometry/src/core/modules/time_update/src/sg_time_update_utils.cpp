/*=============================================================================================*\
* FILE: sg_time_update_detections_utils.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions for time update detections utility functions
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_time_update_utils.h"

#include "geometry/geo_rotate.h"

namespace sg
{
   void update_position(const float host_sin_pointing,
                        const float host_cos_pointing,
                        const Host_Displacement_T &host_displacement,
                        geometry::Point2D_T &position)
   {
      const Matrix<float, 2U, 2U> rotation_matrix = {
         {{host_cos_pointing, host_sin_pointing}, {-host_sin_pointing, host_cos_pointing}}};

      geometry::rotate(position, rotation_matrix);

      position.x -= host_displacement.longitudinal();
      position.y -= host_displacement.lateral();
   }

   void update_position_covariance(const float host_sin_pointing,
                                   const float host_cos_pointing,
                                   const Covariance_2D &process_noise_covariance,
                                   Covariance_2D &position_covariance)
   {
      const float new_position_covariance_xx =
         process_noise_covariance.x
         + ((host_cos_pointing) * (host_cos_pointing * position_covariance.x + host_sin_pointing * position_covariance.xy))
         + ((host_sin_pointing) * (host_cos_pointing * position_covariance.xy + host_sin_pointing * position_covariance.y));

      const float new_position_covariance_yy =
         process_noise_covariance.y
         + ((-host_sin_pointing) * (-host_sin_pointing * position_covariance.x + host_cos_pointing * position_covariance.xy))
         + ((host_cos_pointing) * (-host_sin_pointing * position_covariance.xy + host_cos_pointing * position_covariance.y));

      const float new_position_covariance_xy =
         process_noise_covariance.xy
         + ((-host_sin_pointing) * (host_cos_pointing * position_covariance.x + host_sin_pointing * position_covariance.xy))
         + ((host_cos_pointing) * (host_cos_pointing * position_covariance.xy + host_sin_pointing * position_covariance.y));

      const float new_position_covariance_yx =
         process_noise_covariance.xy
         + ((host_cos_pointing) * (-host_sin_pointing * position_covariance.x + host_cos_pointing * position_covariance.xy))
         + ((host_sin_pointing) * (-host_sin_pointing * position_covariance.xy + host_cos_pointing * position_covariance.y));

      const float new_position_covariance_xy_yx = (new_position_covariance_xy + new_position_covariance_yx) / 2.0F;

      position_covariance.x  = new_position_covariance_xx;
      position_covariance.y  = new_position_covariance_yy;
      position_covariance.xy = new_position_covariance_xy_yx;
   }

}
