#include "sg_simplify_contours.h"

#include <algorithm>

#include "geometry/geo_angle.h"
#include "geometry/geo_length.h"
#include "sg_presimplify_contours.h"
#include "sg_simplify_contours_helpers.h"

namespace sg
{
   void simplify_contours(ContourStorage &contours, const Contour_Postprocessing_Calibrations_T::Simplify_Contours_T &calibrations)
   {
      presimplify_contours(contours, calibrations);

      for (auto &contour : contours)
      {
         remove_acute_endpoints(contour, calibrations.simplify_min_turning_angle);
         const uint16_t num_vertices = contour.size();

         if (num_vertices > 2U)
         {
            switch (calibrations.simplify_type)
            {
               case 1:
               {
                  float cumulative_angle = 0.0F;
                  // Loop through vertices in contour starts from second one
                  const auto last_but_one_vertex_it = std::prev(contour.vertices.end());
                  for (auto vtx_it = std::next(contour.vertices.begin()); vtx_it != last_but_one_vertex_it; ++vtx_it)
                  {
                     const float next_segment_length = geometry::length({{std::next(vtx_it)->position}, {vtx_it->position}});
                     const float prev_segment_length = geometry::length({{vtx_it->position}, {std::prev(vtx_it)->position}});
                     const float simplified_segment_length =
                        geometry::length({{std::prev(vtx_it)->position}, {std::next(vtx_it)->position}});
                     // Check if at least one of adjacent segments is short enaught and simplified segment would not be too long
                     if ((std::min(next_segment_length, prev_segment_length) < calibrations.simplify_length_range[0U])
                         && (simplified_segment_length < calibrations.simplify_length_range[1U]))
                     {
                        const geometry::Point2D_T prev_segment_end = vtx_it->position - std::prev(vtx_it)->position;
                        const geometry::Point2D_T next_segment_end = std::next(vtx_it)->position - vtx_it->position;
                        const geometry::Angle2D_T angle(prev_segment_end, next_segment_end);
                        cumulative_angle += angle.signed_deg();
                        // Check the angle
                        if (std::abs(cumulative_angle) < calibrations.simplify_angle / std::max(1.0F, simplified_segment_length))
                        {
                           std::prev(vtx_it)->pos_cross_cov = Pos_2D_Cross_Cov();

                           (void) contour.vertices.erase(vtx_it);
                        }
                        else
                        {
                           cumulative_angle = 0.0F;
                        }
                     }
                     (void) simplified_segment_length; // MISRA
                  }
                  (void) cumulative_angle; // MISRA
               }
               break;
               case 2:
               {
                  // Reference angle vector at the begining represents host moving direction
                  geometry::Point2D_T reference_angle_vector = {1.0F, 0.0F};
                  // Loop through vertices in contour starts from second one
                  const auto last_but_one_vertex_it = std::prev(contour.vertices.end());
                  for (auto vtx_it = std::next(contour.vertices.begin()); vtx_it != last_but_one_vertex_it; ++vtx_it)
                  {
                     const float prev_segment_length = geometry::length({{vtx_it->position}, {std::prev(vtx_it)->position}});
                     const float next_segment_length = geometry::length({{std::next(vtx_it)->position}, {vtx_it->position}});
                     const float simplified_segment_length =
                        geometry::length({{std::prev(vtx_it)->position}, {std::next(vtx_it)->position}});
                     // Check if at least one of adjacent segments is short enaught and simplified segment would not be too long
                     if ((std::min(next_segment_length, prev_segment_length) < calibrations.simplify_length_range[0U])
                         && (simplified_segment_length < calibrations.simplify_length_range[1U]))
                     {
                        const geometry::Angle2D_T angle(reference_angle_vector,
                                                        {(std::next(vtx_it)->position) - (std::prev(vtx_it)->position)});
                        // Check the angle
                        if (angle.deg() < calibrations.simplify_angle / std::max(1.0F, simplified_segment_length))
                        {
                           (void) contour.vertices.erase(vtx_it);
                        }
                        else
                        {
                           reference_angle_vector = std::next(vtx_it)->position - vtx_it->position;
                        }
                     }
                     else
                     {
                        reference_angle_vector = std::next(vtx_it)->position - vtx_it->position;
                     }
                     (void) simplified_segment_length; // MISRA
                  }
                  (void) reference_angle_vector; // MISRA
               }
               break;
               default:
                  break;
            }
         }
      }
   }
}
