#include "sg_presimplify_contours.h"

#include <algorithm>
#include <numeric>

#include "geometry/geo_length.h"

namespace sg
{
   void presimplify_contours(ContourStorage &contours, const Contour_Postprocessing_Calibrations_T::Simplify_Contours_T &calibrations)
   {
      for (auto &contour : contours)
      {
         if (contour.size() >= 3U)
         {
            mean_of_deviations_presimplify(contour, calibrations.presimplify_host_length);
         }
      }
   }

   void mean_of_deviations_presimplify(Contour_T &contour, const float host_length)
   {
      std::array<float, SG_MAX_NUM_VERTICES_PER_CONTOUR> deviations_y{};
      float deviations_sum      = 0.0F;
      const auto last_vertex_it = --contour.vertices.end();
      auto deviations_it        = deviations_y.begin();

      for (auto vtx_it = contour.vertices.begin(); vtx_it != last_vertex_it; ++vtx_it)
      {
         const float dev_y = std::abs(vtx_it->position.y - std::next(vtx_it)->position.y);
         *deviations_it++  = dev_y;
         deviations_sum += dev_y;
      }
      const float mean_dy = deviations_sum / (static_cast<float>(contour.size()) - 1.0F);

      deviations_it = deviations_y.begin();
      for (auto current_vtx_it = ++contour.vertices.begin(); current_vtx_it != std::prev(contour.vertices.end()); ++current_vtx_it)
      {
         const geometry::Point2D_T diff = std::next(current_vtx_it)->position - std::prev(current_vtx_it)->position;
         const float gap_length         = geometry::length(diff);
         if ((gap_length < host_length) && (*deviations_it > mean_dy) && (*std::next(deviations_it) > mean_dy))
         {
            const float diff_vtxs_y_coef = ((std::next(current_vtx_it)->position.y - current_vtx_it->position.y)
                                            * (current_vtx_it->position.y - std::prev(current_vtx_it)->position.y));
            if (diff_vtxs_y_coef < 0.0F)
            {
               (void) contour.vertices.erase(current_vtx_it);
            }
         }
         ++deviations_it;
      }
      (void) mean_dy;       // MISRA
      (void) deviations_y;  // MISRA
      (void) deviations_it; // MISRA
   }
}
