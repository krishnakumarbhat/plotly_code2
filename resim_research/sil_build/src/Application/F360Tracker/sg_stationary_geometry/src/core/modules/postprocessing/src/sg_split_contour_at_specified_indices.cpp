#include "sg_split_contour_at_specified_indices.h"

#include <algorithm>
#include <cassert>

namespace sg
{
   uint16_t split_contour_at_specified_indices(const Contour_T &contour,
                                               std::array<Contour_T, SG_MAX_NUM_CONTOURS> &new_contours,
                                               std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &split_indices,
                                               const uint16_t num_split_indices,
                                               const uint16_t max_num_new_contours)
   {
      uint16_t num_new_contours = 0U;

      // we split contour if we have space for new contours
      if (max_num_new_contours > 0U)
      {
         // if there are no split indices - return the input contour
         if (num_split_indices == 0U)
         {
            new_contours[num_new_contours] = contour;
            num_new_contours++;
            ;
         }
         // we split contour only if there are at least 2 vertices left
         else if ((num_split_indices + 1U) < contour.size())
         {
            // define array of non_split_indices and auxiliary variables
            std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> non_split_indices{};
            const uint16_t num_non_split_indices =
               get_non_split_indices(split_indices, non_split_indices, num_split_indices, contour.size());
            uint16_t non_split_idx_start = 0U;
            auto current_vertex_it       = contour.vertices.begin();
            uint16_t contour_idx_end     = 0U;

            // we create new contours if there are still split indices and we didn't reach the limit for new contours
            while ((non_split_idx_start < num_non_split_indices) && (num_new_contours < max_num_new_contours))
            {
               const uint16_t non_split_first_nonconsecutive_idx =
                  get_first_nonconsecutive_idx(non_split_indices, num_non_split_indices, non_split_idx_start);
               const uint16_t contour_idx_begin = non_split_indices[non_split_idx_start];
               const uint16_t vertex_advance    = static_cast<uint16_t>(contour_idx_begin - contour_idx_end);
               contour_idx_end                  = non_split_indices[non_split_first_nonconsecutive_idx - 1U];

               // create a contour if it has more than one vertex
               if (contour_idx_begin < contour_idx_end)
               {
                  std::advance(current_vertex_it, vertex_advance);
                  Contour_T new_contour{};
                  for (auto idx = contour_idx_begin; idx <= contour_idx_end; idx++)
                  {
                     (void) new_contour.vertices.push_back(*current_vertex_it);
                     // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822

                     if (idx < contour_idx_end)
                     {
                        ++current_vertex_it;
                     }
                  }
                  new_contour.cluster_id         = contour.cluster_id;
                  new_contour.drivability        = contour.drivability;
                  new_contour.priority           = contour.priority;
                  new_contours[num_new_contours] = std::move(new_contour);
                  num_new_contours++;
               }
               // move non_split_idx_start
               non_split_idx_start = non_split_first_nonconsecutive_idx;
            }
         }
         else
         {
            // otherwise we do nothing and return 0 as the number of new contours
         }
      }
      return num_new_contours;
   }

   uint16_t get_non_split_indices(std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &split_indices,
                                  std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &non_split_indices,
                                  const uint16_t num_split_indices,
                                  const uint16_t num_vertices)
   {
      // check if the num_split_indices and num_vertices are correct
      assert(num_split_indices < SG_MAX_NUM_VERTICES_PER_CONTOUR);
      assert(num_vertices <= SG_MAX_NUM_VERTICES_PER_CONTOUR);

      // if array split_indices is not sorted - we need to order its elements
      if (!std::is_sorted(&split_indices[0U], &split_indices[num_split_indices]))
      {
         std::sort(&split_indices[0U], &split_indices[num_split_indices]);
      }
      uint16_t num_non_split_indices = 0U;
      uint16_t split_indices_idx     = 0U;
      for (uint16_t idx = 0U; idx < num_vertices; idx++)
      {
         if (idx != split_indices[split_indices_idx])
         {
            non_split_indices[num_non_split_indices] = idx;
            num_non_split_indices++;
         }
         else
         {
            split_indices_idx++;
         }
      }
      return num_non_split_indices;
   }

   uint16_t get_first_nonconsecutive_idx(const std::array<uint16_t, SG_MAX_NUM_VERTICES_PER_CONTOUR> &numbers,
                                         const uint16_t num_elements,
                                         const uint16_t start_idx)
   {
      uint16_t idx;
      if (num_elements > SG_MAX_NUM_VERTICES_PER_CONTOUR)
      {
         idx = SG_MAX_NUM_VERTICES_PER_CONTOUR;
      }
      else
      {
         idx = num_elements;
         if (start_idx < num_elements)
         {
            for (uint16_t i = start_idx + 1U; i < num_elements; i++)
            {
               if (numbers[i] != numbers[i - 1U] + 1U)
               {
                  idx = i;
                  break;
               }
            }
         }
      }
      return idx;
   }
}