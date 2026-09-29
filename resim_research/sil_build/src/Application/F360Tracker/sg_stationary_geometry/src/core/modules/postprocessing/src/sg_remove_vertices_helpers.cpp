#include "sg_remove_vertices_helpers.h"

#include <limits>

#include "sg_split_contour_at_specified_indices.h"


namespace sg
{
   void mark_out_of_range_vertices(const Contour_T::VertexList &vertices,
                                   const float rear_limit,
                                   const float front_limit,
                                   const float left_limit,
                                   const float right_limit,
                                   std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask)
   {
      auto vertices_it        = vertices.begin();
      const auto vertices_end = vertices.end();

      if (vertices_it != vertices_end)
      {
         std::size_t idx = 0U;
         for (; vertices_it != vertices_end; ++vertices_it)
         {
            if (vertex_position_out_of_range(*vertices_it, rear_limit, front_limit, left_limit, right_limit))
            {
               vertices_to_remove_mask[idx] = true;
            }
            idx++;
         }
      }
   }

   void mark_uncertain_vertices(const Contour_T::VertexList &vertices,
                                const float accepted_uncertainty,
                                std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask)
   {
      auto vertices_it        = vertices.begin();
      const auto vertices_end = vertices.end();

      if (vertices_it != vertices_end)
      {
         std::size_t idx = 0U;
         for (; vertices_it != vertices_end; vertices_it++)
         {
            if ((accepted_uncertainty < (*vertices_it).pos_cov.x) && (accepted_uncertainty < (*vertices_it).pos_cov.y))
            {
               vertices_to_remove_mask[idx] = true;
            }
            idx++;
         }
      }
   }

   void mark_unreliable_vertices(const Contour_T::VertexList &vertices,
                                 const uint16_t max_num_cycles_no_update,
                                 std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask)
   {
      auto vertices_it        = vertices.begin();
      const auto vertices_end = vertices.end();

      if (vertices_it != vertices_end)
      {
         std::size_t idx = 0U;
         for (; vertices_it != vertices_end; vertices_it++)
         {
            if (((*vertices_it).reliability <= std::numeric_limits<float>::epsilon())
                || (max_num_cycles_no_update < (*vertices_it).num_cycles_no_update))
            {
               vertices_to_remove_mask[idx] = true;
            }
            idx++;
         }
      }
   }

   bool is_split_scenario(const std::size_t num_of_vertices, const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask)
   {
      bool f_split_scenario     = false;
      bool f_zero_to_one_change = false;
      bool f_previous_value     = vertices_to_remove_mask[0];

      for (std::size_t i = 1U; i < num_of_vertices; i++)
      {
         if ((f_previous_value == false) && vertices_to_remove_mask[i]) // value changed from 0 to 1
         {
            f_zero_to_one_change = true;
         }
         // one to zero change
         else if (f_previous_value && (vertices_to_remove_mask[i] == false) && f_zero_to_one_change)
         {
            f_split_scenario = true;
         }
         else
         {
            // no changes
         }
         f_previous_value = vertices_to_remove_mask[i];
      }

      return f_split_scenario;
   }

   void split_or_shorten_contour(const uint8_t contour_idx,
                                 const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask,
                                 std::array<Contour_T, SG_MAX_NUM_CONTOURS> &all_new_contours,
                                 Contour_T &contour,
                                 std::bitset<SG_MAX_NUM_CONTOURS> &contours_to_remove_mask,
                                 uint16_t &num_empty_contour_slots,
                                 uint16_t &num_new_contours)
   {
      // determine if contour should be splitted or shortened
      const bool f_split_scenario = is_split_scenario(contour.vertices.size(), vertices_to_remove_mask);

      if (f_split_scenario)
      {
         // perform split, flag current contour to be removedand expand array of new contours
         std::array<Contour_T, SG_MAX_NUM_CONTOURS> new_contours;
         std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> vertex_remove_indices{};
         const uint16_t num_split_indices     = get_indices_of_mask(vertices_to_remove_mask, vertex_remove_indices);
         const uint16_t num_split_contours    = split_contour_at_specified_indices(contour, new_contours, vertex_remove_indices,
                                                                                   num_split_indices, num_empty_contour_slots);
         contours_to_remove_mask[contour_idx] = true;

         if (num_split_contours > 0U)
         {
            for (std::size_t i = 0U; i < num_split_contours; i++)
            {
               all_new_contours[num_new_contours + i] = std::move(new_contours[i]);
            }

            num_new_contours        = num_new_contours + num_split_contours;
            num_empty_contour_slots = num_empty_contour_slots - num_split_contours;
         }
      }
      // shorten contour
      else
      {
         // remove vertices from contour
         auto const num_vertices = contour.vertices.size();
         if (num_vertices > 0U)
         {
            auto vertices_iter = contour.vertices.begin();

            for (std::size_t i = 0U; i < num_vertices; i++)
            {
               if (vertices_to_remove_mask[i])
               {
                  (void) contour.vertices.erase(vertices_iter);
               }
               ++vertices_iter;
            }
         }

         if (contour.vertices.size() < 2U)
         {
            contours_to_remove_mask[contour_idx] = true;
         }
      }
   }

   uint16_t get_indices_of_mask(const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &mask,
                                std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &indices)
   {
      uint16_t num_of_indices  = 0U;
      const uint16_t mask_size = static_cast<uint16_t>(mask.size());

      for (uint16_t bool_idx = 0U; bool_idx < mask_size; bool_idx++)
      {
         if (mask[bool_idx])
         {
            indices[num_of_indices] = bool_idx;
            ++num_of_indices;
         }
      }
      return num_of_indices;
   }
}
