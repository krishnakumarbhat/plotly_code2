#include "sg_simplify_contours_helpers.h"

#include "geometry/geo_angle.h"

namespace sg
{
   void remove_acute_endpoints(Contour_T &contour, const float simplify_min_turning_angle)
   {
      if (contour.size() > 3U)
      {
         const geometry::Point2D_T second_segment_end =
            std::next(contour.vertices.begin(), 2)->position - std::next(contour.vertices.begin())->position;
         const geometry::Point2D_T first_segment_begin =
            contour.vertices.begin()->position - std::next(contour.vertices.begin())->position;
         const geometry::Angle2D_T front_segment_angle(second_segment_end, first_segment_begin);

         const geometry::Point2D_T second_last_segment_end =
            std::prev(contour.vertices.end(), 3)->position - std::prev(contour.vertices.end(), 2)->position;
         const geometry::Point2D_T last_segment_begin =
            std::prev(contour.vertices.end())->position - std::prev(contour.vertices.end(), 2)->position;
         const geometry::Angle2D_T end_segment_angle(second_last_segment_end, last_segment_begin);

         const bool f_first_vtx_to_erase = (front_segment_angle.deg() < simplify_min_turning_angle);
         const bool f_last_vtx_to_erese  = (end_segment_angle.deg() < simplify_min_turning_angle);

         // In case where contour has 4 vertices and 2 of them are acute entpoints,
         // we do not removing any vertex. This is how it was done in matlab.
         if (f_first_vtx_to_erase && f_last_vtx_to_erese && (contour.size() > 4U))
         {
            (void) contour.vertices.erase(contour.vertices.begin());
            auto vtx_it            = contour.vertices.erase(--contour.vertices.end());
            (--vtx_it)->segment_id = 0U;
         }
         else if (f_first_vtx_to_erase && (!f_last_vtx_to_erese))
         {
            (void) contour.vertices.erase(contour.vertices.begin());
         }
         else if ((!f_first_vtx_to_erase) && f_last_vtx_to_erese)
         {
            auto vtx_it            = contour.vertices.erase(--contour.vertices.end());
            (--vtx_it)->segment_id = 0U;
         }
         else
         {
            // MISRA
         }
      }
   }
}
