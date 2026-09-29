#include "sg_remove_vertices.h"

#include "sg_remove_vertices_helpers.h"

namespace sg
{
   void remove_vertices(ContourStorage &contours, const Contour_Postprocessing_Calibrations_T::Remove_Vertices_T &calibrations)
   {
      uint16_t num_empty_contour_slots = SG_MAX_NUM_CONTOURS - static_cast<uint16_t>(contours.size());

      std::array<Contour_T, SG_MAX_NUM_CONTOURS> all_new_contours;

      // initialize mask of contours after split (to be removed) and array of new contours
      std::bitset<SG_MAX_NUM_CONTOURS> contours_to_remove_mask{};
      uint16_t num_new_contours = 0U;
      uint8_t contour_idx       = 0U;

      for (auto &contour : contours)
      {
         std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> vertices_to_remove_mask;
         mark_out_of_range_vertices(contour.vertices, calibrations.remove_vertices_rear_limit,
                                    calibrations.remove_vertices_front_limit, calibrations.remove_vertices_left_limit,
                                    calibrations.remove_vertices_right_limit, vertices_to_remove_mask);
         mark_uncertain_vertices(contour.vertices, calibrations.accepted_uncertainty, vertices_to_remove_mask);
         mark_unreliable_vertices(contour.vertices, calibrations.max_num_cycles_no_update, vertices_to_remove_mask);

         if (vertices_to_remove_mask.any())
         {
            split_or_shorten_contour(contour_idx, vertices_to_remove_mask, all_new_contours, contour, contours_to_remove_mask,
                                     num_empty_contour_slots, num_new_contours);
         }

         contour_idx++;
      }

      if (contours_to_remove_mask.any())
      {
         const std::size_t num_valid_contours = contours.size();
         auto contours_iter                   = contours.begin();

         for (std::size_t i = 0U; i < num_valid_contours; i++)
         {
            if (contours_to_remove_mask[i])
            {
               (void) contours.erase(contours_iter);
            }
            ++contours_iter;
         }
      }

      if (num_new_contours > 0U)
      {
         for (auto &contour : all_new_contours)
         {
            if (contour.vertices.size() > 1U)
            {
               (void) contours.push_back(std::move(contour));
               // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
            }
         }
      }

      // decrease the reliability of vertices of contours
      for (auto &contour : contours)
      {
         for (auto &vertex : contour.vertices)
         {
            vertex.reliability -= 1.0F;
         }
      }
   }
}
