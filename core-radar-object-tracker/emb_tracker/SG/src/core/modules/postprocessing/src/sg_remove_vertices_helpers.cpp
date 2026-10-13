#include "sg_remove_vertices_helpers.h"

#include <limits>

namespace sg
{
void mark_out_of_range_vertices(const Contour_T::VertexList &vertices, const float rear_limit, const float front_limit,
                                const float left_limit, const float right_limit,
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

void mark_uncertain_vertices(const Contour_T::VertexList &vertices, const float accepted_uncertainty,
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

void mark_unreliable_vertices(const Contour_T::VertexList &vertices, const uint16_t max_num_cycles_no_update,
                              std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask)
{
   auto vertices_it        = vertices.begin();
   const auto vertices_end = vertices.end();

   if (vertices_it != vertices_end)
   {
      std::size_t idx = 0U;
      for (; vertices_it != vertices_end; vertices_it++)
      {
         if (((*vertices_it).reliability <= std::numeric_limits<float>::epsilon()) ||
             (max_num_cycles_no_update < (*vertices_it).num_cycles_no_update))
         {
            vertices_to_remove_mask[idx] = true;
         }
         idx++;
      }
   }
}
} // namespace sg
