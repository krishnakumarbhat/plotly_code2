/*===================================================================================*\
* FILE: dc_time_update_subsegments.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains time update subsegments handcode.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_time_update_subsegments.h"

#include "dc_contour_storage.h"
#include "geometry/geo_rotate.h"

namespace sg
{
   namespace dc
   {
      void TimeUpdateSubsegments::timeUpdateSubsegments(DCContourStorage &contour_list,
                                                        const float elapsed_time,
                                                        const float32_t speed,
                                                        const float cos_heading,
                                                        const float sin_heading)
      {
         const auto distance                         = speed * elapsed_time;
         const auto vehicle_movement_lon             = distance * cos_heading;
         const auto vehicle_movement_lat             = distance * -sin_heading;
         const Matrix<float, 2U, 2U> rotation_matrix = {{{cos_heading, -sin_heading}, {sin_heading, cos_heading}}};

         for (auto &contour : contour_list)
         {
            for (auto &subsegment : contour.subsegments)
            {
               geometry::rotate(subsegment.begin_vertex.position, rotation_matrix);
               geometry::rotate(subsegment.end_vertex.position, rotation_matrix);

               subsegment.begin_vertex.position.y -= vehicle_movement_lat;
               subsegment.begin_vertex.position.x -= vehicle_movement_lon;
               subsegment.end_vertex.position.y -= vehicle_movement_lat;
               subsegment.end_vertex.position.x -= vehicle_movement_lon;
            }
         }
         (void) vehicle_movement_lon; // MISRA
         (void) vehicle_movement_lat; // MISRA
         (void) rotation_matrix;      // MISRA
      }
   }
}
