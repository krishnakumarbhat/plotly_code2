/*===================================================================================*\
* FILE: dc_update_subsegments_helpers.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains update_subsegments helper functions implementation.
*
*  Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_update_subsegments_helpers.h"

#include "dc_is_vertex_in_critical_region.h"

namespace sg
{
   namespace dc
   {
      void add_not_projected_subsegments_to_leftovers(OldLeftovers &leftovers,
                                                      const DCContourStorage::ContourList::iterator dc_contour_it,
                                                      const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &f_projected_old)
      {
         if ((dc_contour_it != nullptr) && (dc_contour_it->get_num_of_vertices() >= 2U))
         {
            uint8_t subsegment_idx{0U};
            auto subsegments_it           = dc_contour_it->subsegments.begin();
            const auto subsegments_it_end = dc_contour_it->subsegments.end();
            for (; (subsegments_it != subsegments_it_end) && (subsegment_idx < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
                   && (leftovers.num_old_leftovers < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR);
                 subsegments_it++)
            {
               if ((!f_projected_old[subsegment_idx]) && (subsegments_it->is_critical())
                   && (subsegments_it->subsegment_id != INVALID_SEGMENT_ID))
               {
                  leftovers.contour_leftovers[leftovers.num_old_leftovers]    = dc_contour_it;
                  leftovers.subsegment_leftovers[leftovers.num_old_leftovers] = subsegments_it;
                  ++(leftovers.num_old_leftovers);
               }
               ++subsegment_idx;
            }
            (void) subsegment_idx; // MISRA
         }
      }

      void assign_vertex_to_contour(DC_Contour_T &dc_contour, const Vertex_T &segment_beg, const Vertex_T &segment_end, const bool f_critical)
      {
         Subsegment_T new_subsegment;
         new_subsegment.subsegment_id           = INVALID_SEGMENT_ID;
         new_subsegment.segment_id              = segment_beg.segment_id;
         new_subsegment.sg_age                  = segment_beg.age;
         new_subsegment.sg_cycles_since_coasted = segment_beg.num_cycles_no_update;

         new_subsegment.begin_vertex.position.x = segment_beg.position.x;
         new_subsegment.begin_vertex.position.y = segment_beg.position.y;
         new_subsegment.begin_vertex.f_critical = false;
         new_subsegment.begin_vertex.f_primary  = true;

         new_subsegment.end_vertex.f_critical = f_critical;
         new_subsegment.end_vertex.position   = segment_end.position;
         new_subsegment.end_vertex.f_primary  = true;

         dc_contour.subsegments.push_back(new_subsegment);
      }

      void assign_last_vertex_to_contour(DC_Contour_T &dc_contour, const Contour_T &current_contour, const bool f_critical)
      {
         // checking if contours are not empty
         if ((!dc_contour.subsegments.empty()) && (!current_contour.vertices.empty()))
         {
            dc_contour.subsegments.rbegin()->end_vertex.position   = current_contour.vertices.rbegin()->position;
            dc_contour.subsegments.rbegin()->end_vertex.f_critical = f_critical;
            dc_contour.subsegments.rbegin()->end_vertex.f_primary  = true;
         }
      }

      std::pair<DCContourStorage::ContourList::iterator, DC_Contour_T::SubsegmentList::iterator>
      find_corresponding_segment(const DCContourStorage &dc_contour_list, const uint32_t segment_id)
      {
         std::pair<DCContourStorage::ContourList::iterator, DC_Contour_T::SubsegmentList::iterator> output_iters =
            std::make_pair(nullptr, nullptr);
         for (auto iter = dc_contour_list.begin(); iter != dc_contour_list.end(); ++iter)
         {
            // what with f_critical from matlab code?
            const auto subsegment_iter = std::find_if(iter->subsegments.begin(), iter->subsegments.end(),
                                                      [segment_id](const Subsegment_T &item)
                                                      { return item.segment_id == segment_id; });

            if (subsegment_iter != iter->subsegments.end())
            {
               output_iters.first  = iter;
               output_iters.second = subsegment_iter;
               break;
            }
         }

         return output_iters;
      }

      void populate_not_assigned_leftovers(DCContourStorage &dc_contour_list,
                                           const NewLeftovers &new_contour_leftovers,
                                           const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &new_leftovers_assigned_mask)
      {
         for (uint8_t leftover_idx{0U};
              (leftover_idx < new_contour_leftovers.num_new_leftovers) && (leftover_idx < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR);
              ++leftover_idx)
         {
            if (!new_leftovers_assigned_mask[leftover_idx])
            {
               new_contour_leftovers.subsegment_leftovers[leftover_idx]->subsegment_id =
                  dc_contour_list.get_available_subsegment_id();
            }
         }
      }

      void reassign_DC_contours(DCContourStorage &dc_contours_list,
                                ArrayWrapper<DC_Contour_T, SG_MAX_NUM_CONTOURS> &dc_contours_array_temp)
      {
         dc_contours_list.reset_contour_list();

         for (auto &dc_contour : dc_contours_array_temp)
         {
            const auto contour_id = dc_contour.get_id();
            if (contour_id != INVALID_CONTOUR_ID)
            {
               (void) dc_contours_list.push_back(std::move(dc_contour), contour_id);
            }
         }
      }

      void update_new_leftovers(NewLeftovers &leftovers,
                                const DC_Contour_T::SubsegmentList::iterator segment_start,
                                const std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &leftovers_mask,
                                const uint16_t curr_num_vertices,
                                const uint16_t num_subvertices_to_add_safe,
                                const bool f_next_critical)
      {
         // find new leftovers iterators
         std::array<DC_Contour_T::SubsegmentList::iterator, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> new_leftover_iterators{};
         std::fill_n(new_leftover_iterators.begin(), SG_MAX_NUM_SUBVERTICES_PER_CONTOUR, nullptr);
         uint16_t num_new_leftovers_iterators{0U};
         auto subsegment_iterator = segment_start;
         for (uint16_t leftover_idx{0U}; leftover_idx < num_subvertices_to_add_safe; ++leftover_idx)
         {
            if (((leftover_idx + curr_num_vertices) == (SG_MAX_NUM_SUBVERTICES_PER_CONTOUR - 1U)) && (!f_next_critical))
            {
               break;
            }
            if (leftovers_mask[leftover_idx])
            {
               new_leftover_iterators[num_new_leftovers_iterators] = subsegment_iterator;
               ++num_new_leftovers_iterators;
            }
            if (subsegment_iterator != nullptr)
            {
               ++subsegment_iterator;
            }
         }

         (void) new_leftover_iterators; // MISRA

         // append new leftovers to new_leftovers struct
         const uint16_t num_new_leftovers_after_addition = leftovers.num_new_leftovers + num_new_leftovers_iterators;
         const uint16_t num_new_leftovers_safe = std::min(SG_MAX_NUM_SUBVERTICES_PER_CONTOUR, num_new_leftovers_after_addition);
         const uint16_t num_new_leftovers_added_safe = num_new_leftovers_safe - leftovers.num_new_leftovers;
         for (auto new_leftovers_offset = 0U; new_leftovers_offset < num_new_leftovers_added_safe; ++new_leftovers_offset)
         {
            leftovers.subsegment_leftovers[leftovers.num_new_leftovers + new_leftovers_offset] =
               new_leftover_iterators[new_leftovers_offset];
         }
         leftovers.num_new_leftovers = num_new_leftovers_safe;
      }
   }
}