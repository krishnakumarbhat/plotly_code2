#include "sg_split_or_shorten_contour.h"

namespace sg
{
void split_or_shorten_contour(Contour_T &contour, std::array<Contour_T, SG_MAX_NUM_CONTOURS> &all_new_contours,
                              std::bitset<SG_MAX_NUM_CONTOURS> &contours_to_remove_mask, uint16_t &num_empty_contour_slots,
                              uint16_t &num_new_contours,
                              const std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_to_remove_mask,
                              const uint8_t contour_idx)
{
   const size_t original_contour_len = contour.vertices.size();
   uint16_t split_contour_len        = 0U;

   for (size_t i = 0U; i < original_contour_len && num_empty_contour_slots > 0U; ++i)
   {
      if (!vertices_to_remove_mask[i])
      {
         split_contour_len++;
      }
      else
      {
         // end of the new contour reached - check if length is valid and split
         if (split_contour_len > 1U)
         {
            Contour_T new_contour = {};
            fill_empty_contour(new_contour, contour, split_contour_len);

            if (i < original_contour_len - 1U)
            {
               move_new_contour_to_list(new_contour, all_new_contours, num_empty_contour_slots, num_new_contours);
            }
            else
            {
               // original contour length reached - last iteration
               move_vertices_to_new_contour(contour, new_contour);
            }
         }
         else
         {
            remove_obsolete_vertices(contour, split_contour_len);
         }
      }
   }
   // leftover after split - to be killed.
   if (static_cast<uint16_t>(contour.size()) < 2U)
   {
      contours_to_remove_mask[contour_idx] = true;
   }
}

void fill_empty_contour(Contour_T &empty_contour, Contour_T &contour, uint16_t &subcontour_len)
{
   auto vertex_it = contour.vertices.begin();
   for (uint16_t i = 0U; i < subcontour_len; i++)
   {
      (void)empty_contour.vertices.move_back(contour.vertices, vertex_it);
      ++vertex_it;
   }
   empty_contour.cluster_id  = contour.cluster_id;
   empty_contour.drivability = contour.drivability;
   empty_contour.priority    = contour.priority;

   // to-be-removed vertice is now at the beginning of the list - pop vertex
   contour.vertices.pop_front();

   // need to zero subcontour len now
   subcontour_len = 0U;
}

void move_vertices_to_new_contour(Contour_T &contour, Contour_T &new_contour)
{
   contour.vertices.clear();
   for (auto vertex_it = new_contour.vertices.begin(); vertex_it != new_contour.vertices.end(); vertex_it++)
   {
      (void)contour.vertices.move_back(new_contour.vertices, vertex_it);
   }
}

void remove_obsolete_vertices(Contour_T &contour, uint16_t &splitted_contour_len)
{
   for (uint16_t j = 0U; j <= splitted_contour_len; j++)
   {
      contour.vertices.pop_front();
   }
   splitted_contour_len = 0U;
}

void move_new_contour_to_list(Contour_T &new_contour, std::array<Contour_T, SG_MAX_NUM_CONTOURS> &all_new_contours,
                              uint16_t &num_empty_contour_slots, uint16_t &num_new_contours)
{
   all_new_contours[num_new_contours] = std::move(new_contour);
   num_new_contours++;
   num_empty_contour_slots--;
}
} // namespace sg
