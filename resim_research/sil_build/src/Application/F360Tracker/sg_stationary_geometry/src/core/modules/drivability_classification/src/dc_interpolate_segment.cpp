/*===================================================================================*\
* FILE: dc_interpolate_segment.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains implementation of interpolate segment method.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_interpolate_segment.h"

#include "dc_get_subvertices_positions.h"
#include "dc_is_vertex_in_critical_region.h"

namespace sg
{
   namespace dc
   {
      void interpolate_segment(DC_Contour_T &dc_contour,
                               std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &leftovers_mask,
                               const geometry::Point2D_T &start_point,
                               const geometry::Point2D_T &end_point,
                               const float subsegment_length,
                               const CriticalRegion &critical_region)
      {
         ArrayWrapper<geometry::Point2D_T> points_array;

         get_subvertices_positions(points_array, start_point, end_point, subsegment_length, true, true);
         const auto num_interpolated_subvertex_positions = points_array.get_num_elements();

         std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> f_in_region;
         for (auto i = 0U; i < num_interpolated_subvertex_positions; i++)
         {
            (void) f_in_region.set(static_cast<size_t>(i), common::is_vertex_in_critical_region(critical_region, points_array[i]));
         }

         if (num_interpolated_subvertex_positions > 0U)
         {
            uint16_t num_leftover = 0U;
            for (uint16_t i = 0U; i < (num_interpolated_subvertex_positions - 1U); i++)
            {
               const bool f_is_first_vertex = (i == 0U);
               const auto curr_point        = points_array[i];
               const auto next_point        = points_array[i + 1U];
               (void) curr_point; // MISRA
               (void) next_point; // MISRA

               // preventing overflow
               if ((f_in_region[i] || f_is_first_vertex) && (dc_contour.get_num_of_vertices() >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR))
               {
                  break;
               }

               if (f_in_region[i] && f_in_region[i + 1U])
               {
                  Subsegment_T subsegment{{Subsegment_Vertex_T{curr_point, f_in_region[i], f_is_first_vertex}},
                                          Subsegment_Vertex_T{next_point, f_in_region[i + 1U], false}};
                  (void) dc_contour.subsegments.push_back(subsegment);
                  (void) leftovers_mask.set(static_cast<size_t>(num_leftover));
                  num_leftover++;
               }
               else if ((f_in_region[i] && (!f_in_region[i + 1U])) || (f_is_first_vertex && (!f_in_region[i])))
               {
                  auto end_vertex = Subsegment_Vertex_T{};
                  if (f_in_region[i + 1U])
                  {
                     end_vertex.position   = next_point;
                     end_vertex.f_critical = true;
                  }
                  Subsegment_T subsegment{{Subsegment_Vertex_T{curr_point, f_in_region[i], f_is_first_vertex}}, end_vertex};
                  (void) dc_contour.subsegments.push_back(subsegment);
                  num_leftover++;
               }
               else if ((!f_in_region[i]) && f_in_region[i + 1U])
               {
                  dc_contour.subsegments.back().end_vertex.position   = next_point;
                  dc_contour.subsegments.back().end_vertex.f_critical = true;
               }
               else
               {
                  // MISRA
               }
            }
            (void) num_leftover; // MISRA

            // assign end_vertex of the last subsegment
            if (!f_in_region[num_interpolated_subvertex_positions - 1U])
            {
               dc_contour.subsegments.back().end_vertex.position   = end_point;
               dc_contour.subsegments.back().end_vertex.f_critical = false;
            }
            dc_contour.subsegments.rbegin()->end_vertex.f_primary = true;
         }
      }
   }
}