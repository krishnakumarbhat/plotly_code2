/*===================================================================================*\
* FILE: dc_is_vertex_in_critical_region.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of function checking if vertex is in critical region.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_IS_VERTEX_IN_CRITICAL_REGION_H
#define DC_IS_VERTEX_IN_CRITICAL_REGION_H

#include "dc_critical_region.h"
#include "geometry/geo_point.h"
#include "sg_constants.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      namespace common
      {
         /**
          * @brief       Determining if a point lies on the interior of the polygon.
          * @brief       Horizontal ray is casted to the right of the point.
          * @brief       If the number of intersections this lines and the polygon border is even, point is outside, if odd -
          *inside
          *
          * @param[in]   CriticalRegion &critical_region - critical region that needs to be checked if the point is inside
          * @param[in]   geometry::Point2D_T vertex - point to be checked if it is inside the critical region
          * @param[in]   f_critical - true - skip determining and returns vertex is in critical region., false -  determining if a
          *point lies on the interior of the polygon
          *
          * @return      bool - vertex is in critical region or not
          **/
         bool is_vertex_in_critical_region(const CriticalRegion &critical_region, const geometry::Point2D_T &vertex, bool f_critical);

         /**
          * @brief       Determining if a point lies on the interior of the polygon.
          * @brief       Horizontal ray is casted to the right of the point.
          * @brief       If the number of intersections this lines and the polygon border is even, point is outside, if odd -
          *inside
          *
          * @param[in]   CriticalRegion &critical_region - critical region that needs to be checked if the point is inside
          * @param[in]   geometry::Point2D_T vertex - point to be checked if it is inside the critical region
          *
          * @return      bool - vertex is in critical region or not
          **/
         bool is_vertex_in_critical_region(const CriticalRegion &critical_region, const geometry::Point2D_T &vertex);
      }
   }
}
#endif