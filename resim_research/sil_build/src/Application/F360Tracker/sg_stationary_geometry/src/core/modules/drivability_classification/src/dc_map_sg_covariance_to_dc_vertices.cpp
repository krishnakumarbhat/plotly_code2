/*===================================================================================*\
* FILE: dc_map_sg_covariance_to_dc_vertices.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains implementation of map_sg_covariance_to_dc_vertices function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_map_sg_covariance_to_dc_vertices.h"

#include <cmath>

#include "geometry/geo_distance.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {

      void map_sg_covariance_to_dc_vertices(FusedContourStorage &fused_contours,
                                            const ContourStorage &sg_contours,
                                            const float min_segment_length)
      {
         for (auto &fused_contour : fused_contours)
         {
            const auto sg_contour_it = find_matching_contour(fused_contour.get_id(), sg_contours);

            if (sg_contour_it != nullptr)
            {
               auto sg_vertex_it      = sg_contour_it->vertices.begin();
               auto sg_vertex_next_it = std::next(sg_vertex_it);
               for (auto &vertex : fused_contour.vertices)
               {
                  const float tolerance = 10e-5F;
                  if (are_points_close(vertex.position, sg_vertex_it->position, tolerance))
                  {
                     vertex.pos_cov       = sg_vertex_it->pos_cov;
                     vertex.pos_cross_cov = sg_vertex_it->pos_cross_cov;
                  }
                  else if ((sg_vertex_next_it != sg_contour_it->vertices.end()))
                  {
                     if (are_points_close(vertex.position, sg_vertex_next_it->position, tolerance))
                     {
                        sg_vertex_it = sg_vertex_next_it;
                        sg_vertex_next_it++;
                        vertex.pos_cov       = sg_vertex_it->pos_cov;
                        vertex.pos_cross_cov = sg_vertex_it->pos_cross_cov;
                     }
                     else
                     {
                        interpolate_vertex_covariance(vertex, *sg_vertex_it, *sg_vertex_next_it, min_segment_length);
                     }
                  }
                  else
                  {
                     // MISRA
                  }
               }
            }
         }
      }

      bool are_points_close(const geometry::Point2D_T point1, const geometry::Point2D_T point2, const float tolerance)
      {
         bool f_are_close  = false;
         const auto vector = point2 - point1;
         if ((std::fabs(vector.x) <= tolerance) && (std::fabs(vector.y) <= tolerance))
         {
            f_are_close = true;
         }
         return f_are_close;
      }

      ContourStorage::ContourList::iterator find_matching_contour(const uint32_t contour_id, const ContourStorage &sg_contours)
      {
         bool f_contour_found                                = false;
         ContourStorage::ContourList::iterator sg_contour_it = sg_contours.begin();
         const auto sg_contours_end                          = sg_contours.end();
         for (; sg_contour_it != sg_contours_end; ++sg_contour_it)
         {
            if (sg_contour_it->unique_id() == contour_id)
            {
               f_contour_found = true;
               break;
            }
         }
         if (!f_contour_found)
         {
            sg_contour_it = nullptr;
         }
         return sg_contour_it;
      }

      void interpolate_vertex_covariance(Fused_Vertex_T &vertex,
                                         const Vertex_T &sg_vertex,
                                         const Vertex_T &next_sg_vertex,
                                         const float min_segment_length)
      {
         assert(min_segment_length > 0.0F);
         const float segment_length = geometry::euclidean_distance(sg_vertex.position, next_sg_vertex.position);
         if (segment_length < min_segment_length)
         {
            vertex.pos_cov       = sg_vertex.pos_cov;
            vertex.pos_cross_cov = sg_vertex.pos_cross_cov;
         }
         else
         {
            const float distance_to_first_sg_vertex = geometry::euclidean_distance(sg_vertex.position, vertex.position);
            const float second_scale_factor         = distance_to_first_sg_vertex / segment_length;
            const float first_scale_factor          = 1.0F - second_scale_factor;

            const auto interpolate_values = [](const float value1, const float factor1, const float value2, const float factor2)
            { return value1 * factor1 + value2 * factor2; };

            vertex.pos_cov.x =
               interpolate_values(sg_vertex.pos_cov.x, first_scale_factor, next_sg_vertex.pos_cov.x, second_scale_factor);
            vertex.pos_cov.xy =
               interpolate_values(sg_vertex.pos_cov.xy, first_scale_factor, next_sg_vertex.pos_cov.xy, second_scale_factor);
            vertex.pos_cov.y =
               interpolate_values(sg_vertex.pos_cov.y, first_scale_factor, next_sg_vertex.pos_cov.y, second_scale_factor);

            vertex.pos_cross_cov.x1x2 = interpolate_values(sg_vertex.pos_cross_cov.x1x2, first_scale_factor,
                                                           next_sg_vertex.pos_cross_cov.x1x2, second_scale_factor);
            vertex.pos_cross_cov.x1y2 = interpolate_values(sg_vertex.pos_cross_cov.x1y2, first_scale_factor,
                                                           next_sg_vertex.pos_cross_cov.x1y2, second_scale_factor);
            vertex.pos_cross_cov.y1x2 = interpolate_values(sg_vertex.pos_cross_cov.y1x2, first_scale_factor,
                                                           next_sg_vertex.pos_cross_cov.y1x2, second_scale_factor);
            vertex.pos_cross_cov.y1y2 = interpolate_values(sg_vertex.pos_cross_cov.y1y2, first_scale_factor,
                                                           next_sg_vertex.pos_cross_cov.y1y2, second_scale_factor);
         }
      }
   }
}
