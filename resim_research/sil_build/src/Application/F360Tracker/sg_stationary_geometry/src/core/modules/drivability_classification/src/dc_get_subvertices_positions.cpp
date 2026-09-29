
/*===================================================================================*\
* FILE: dc_get_subvertices_positions.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements method that performs [primary_start_vertex, primary_end_vertex] segment interpolation in a straight line
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_get_subvertices_positions.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "dc_array_wrapper.h"
#include "geometry/geo_length.h"

namespace sg
{
   namespace dc
   {
      void get_subvertices_positions(ArrayWrapper<geometry::Point2D_T> &subvertices_positions_array,
                                     const geometry::Point2D_T &first_vertex_position,
                                     const geometry::Point2D_T &last_vertex_position,
                                     const float subsegment_length,
                                     const bool include_first_vertex,
                                     const bool include_last_vertex)
      {
         if (subsegment_length > std::numeric_limits<float>::epsilon())
         {
            const auto segment_vector = last_vertex_position - first_vertex_position;
            const auto segment_length = geometry::length(segment_vector);
            if (segment_length > std::numeric_limits<float>::epsilon())
            {
               const auto direction_vector   = segment_vector / segment_length;
               const auto subsegment_vector  = subsegment_length * direction_vector;
               const auto length_ratio       = segment_length / subsegment_length;
               auto num_subvertex_candidates = static_cast<uint32_t>(floorf(length_ratio));

               if (std::fabs(static_cast<float32_t>(num_subvertex_candidates) - length_ratio) < std::numeric_limits<float>::epsilon())
               {
                  if (num_subvertex_candidates > 0U)
                  {
                     --num_subvertex_candidates;
                  }
               }

               if (include_first_vertex)
               {
                  subvertices_positions_array.add_element(first_vertex_position);
               }

               auto subvertex = first_vertex_position;
               for (auto i = 0U; i < num_subvertex_candidates; ++i)
               {
                  subvertex = subvertex + subsegment_vector;
                  subvertices_positions_array.add_element(subvertex);
               }
               (void) subvertex; // MISRA

               if (include_last_vertex)
               {
                  subvertices_positions_array.add_element(last_vertex_position);
               }
               (void) subsegment_vector; // MISRA
            }
         }
      }
   }
}
