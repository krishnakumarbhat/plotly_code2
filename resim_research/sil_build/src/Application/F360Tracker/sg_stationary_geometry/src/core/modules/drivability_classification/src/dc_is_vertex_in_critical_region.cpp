/*===================================================================================*\
* FILE: dc_is_vertex_in_critical_region.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition of function checking if vertex is in critical region.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "dc_is_vertex_in_critical_region.h"

#include <cmath>
#include <limits>

namespace sg
{
   namespace dc
   {
      namespace common
      {
         bool is_vertex_in_critical_region(const CriticalRegion &critical_region, const geometry::Point2D_T &vertex, bool f_critical)
         {
            if (!f_critical)
            {
               f_critical = is_vertex_in_critical_region(critical_region, vertex);
            }
            return f_critical;
         }

         bool is_vertex_in_critical_region(const CriticalRegion &critical_region, const geometry::Point2D_T &vertex)
         {
            auto inside_cnt = 0U;
            auto prev_idx   = critical_region.get_size() - 1U;
            const auto &x   = vertex.x;
            for (auto idx = 0U; idx < critical_region.get_size(); ++idx)
            {
               const auto prev_vertex = critical_region[prev_idx];
               const auto curr_vertex = critical_region[idx];
               if (((x >= prev_vertex.x) && (x < curr_vertex.x)) || ((x >= curr_vertex.x) && (x < prev_vertex.x)))
               {
                  if (std::fabs(prev_vertex.x - curr_vertex.x) > std::numeric_limits<float>::epsilon())
                  {
                     const auto &reference_point_y =
                        (curr_vertex.y - prev_vertex.y) * (x - prev_vertex.x) / (curr_vertex.x - prev_vertex.x) + prev_vertex.y;
                     if (vertex.y <= reference_point_y)
                     {
                        inside_cnt++;
                     }
                  }
               }
               prev_idx = idx;
            }
            return (inside_cnt % 2U) != 0U;
         }
      }
   }
}
