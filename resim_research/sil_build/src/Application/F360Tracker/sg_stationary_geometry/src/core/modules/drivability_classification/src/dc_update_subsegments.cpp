/*===================================================================================*\
* FILE: dc_update_subsegments.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains update_subsegments function implementation.
*
*  Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_update_subsegments.h"

#include "dc_amend_segment.h"
#include "dc_assign_closest_subsegments.h"
#include "dc_common.h"
#include "dc_get_subvertices_positions.h"
#include "dc_interpolate_segment.h"
#include "dc_is_vertex_in_critical_region.h"
#include "dc_project_points_on_line.h"
#include "dc_update_subsegments_helpers.h"
#include "geometry/geo_distance.h"

namespace sg
{
   namespace dc
   {
      void DC_Update_Subsegments::update_subsegments(DCContourStorage &dc_contour_list,
                                                     const ContourStorage &contour_list,
                                                     const CriticalRegion &critical_region,
                                                     const Drivability_Classification_Calibrations_T &cfg)
      {
         ArrayWrapper<DC_Contour_T, SG_MAX_NUM_CONTOURS> new_dc_contour_array;

         auto dc_contour_list_it = dc_contour_list.begin();
         (void) dc_contour_list_it; // MISRA
         for (const auto &current_contour : contour_list)
         {
            (void) current_contour; // MISRA
            if (new_dc_contour_array.is_full())
            {
               break;
            }

            std::size_t vertex_index{0U};
            std::bitset<SG_MAX_NUM_VERTICES> f_critical{false};
            for (auto &vertex : current_contour.vertices)
            {
               f_critical[vertex_index] = common::is_vertex_in_critical_region(critical_region, vertex.position);
               vertex_index++;
            }

            if (f_critical.any())
            {
               DC_Contour_T new_dc_contour{};
               new_dc_contour_array.add_element(std::move(new_dc_contour));
               const auto new_dc_contour_array_it = new_dc_contour_array.rbegin();
               auto &current_dc_contour           = *new_dc_contour_array_it;
               current_dc_contour.set_id(current_contour.unique_id());
               const auto partner_dc_contour_it =
                  get_partner_dc_contour(dc_contour_list, dc_contour_list_it, current_contour.unique_id());

               update_contour_subsegments(current_dc_contour, dc_contour_list, f_critical, partner_dc_contour_it, current_contour,
                                          critical_region, cfg);
            }
            // Increment dc_contour_list_it to be used in get_partner_dc_contour function
            // If dc contours are in the same order as sg contours then it saves us looping over all contours
            if ((dc_contour_list_it != dc_contour_list.end()) && (dc_contour_list_it->get_id() == current_contour.unique_id()))
            {
               dc_contour_list_it++;
            }
         }

         reassign_DC_contours(dc_contour_list, new_dc_contour_array);
      }

      void DC_Update_Subsegments::update_contour_subsegments(DC_Contour_T &current_dc_contour,
                                                             DCContourStorage &dc_contour_list,
                                                             const std::bitset<SG_MAX_NUM_VERTICES> &f_critical,
                                                             const DCContourStorage::ContourList::iterator current_old_dc_contour_iter,
                                                             const Contour_T &current_contour,
                                                             const CriticalRegion &critical_region,
                                                             const Drivability_Classification_Calibrations_T &cfg)
      {
         NewLeftovers contour_new_leftovers;
         OldLeftovers contour_old_leftovers;
         std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> f_projected_old;

         uint16_t i_primary_vertex = 0U;
         (void) i_primary_vertex; // MISRA

         const auto last_contour_vertex_it = --current_contour.vertices.end();
         (void) last_contour_vertex_it; // MISRA
         for (auto vertex_iter = current_contour.vertices.begin(); vertex_iter != last_contour_vertex_it; vertex_iter++)
         {
            const uint16_t curr_num_vertices = current_dc_contour.get_num_of_vertices();
            if (curr_num_vertices >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
            {
               break;
            }

            auto &current_vertex  = *vertex_iter;
            auto next_vertex_iter = vertex_iter;
            next_vertex_iter++;
            auto &next_vertex = *next_vertex_iter;
            (void) next_vertex; // MISRA

            const bool f_current_vertex_critical = f_critical[i_primary_vertex];
            const bool f_next_vertex_critical    = f_critical[i_primary_vertex + 1U];

            if (current_dc_contour.get_num_of_vertices() < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
            {
               const uint16_t current_contour_vertices_count = static_cast<uint16_t>(current_contour.vertices.size());
               if ((f_current_vertex_critical || f_next_vertex_critical) && (i_primary_vertex != current_contour_vertices_count - 1U))
               {
                  const auto partners = find_corresponding_segment(dc_contour_list, current_vertex.segment_id);

                  const auto partner_contour = partners.first;
                  const auto partner_segment = partners.second;
                  (void) partner_segment; // MISRA

                  std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> leftovers_mask;
                  // if partner segment exists, project its subvertices to a current segment
                  if ((partner_contour != nullptr) && (partner_segment != nullptr))
                  {
                     UpdatedSegment projected_segment;
                     project_points_on_line(projected_segment, contour_old_leftovers, f_projected_old, vertex_iter,
                                            f_current_vertex_critical, f_next_vertex_critical, partner_segment, partner_contour,
                                            critical_region, (*partner_contour).subsegments, cfg);

                     amend_segment(current_dc_contour, leftovers_mask, projected_segment, next_vertex.position, critical_region,
                                   f_next_vertex_critical, cfg.subsegment_length);
                  }
                  else
                  {
                     interpolate_segment(current_dc_contour, leftovers_mask, current_vertex.position, next_vertex.position,
                                         cfg.subsegment_length, critical_region);
                  }
                  const uint16_t num_vertices_after_addition = current_dc_contour.get_num_of_vertices();
                  const uint16_t num_vertices_after_addition_safe =
                     std::min(num_vertices_after_addition, static_cast<uint16_t>(SG_MAX_NUM_SUBVERTICES_PER_CONTOUR - 1U));
                  const uint16_t num_subvertices_to_add_safe = num_vertices_after_addition_safe - curr_num_vertices;

                  DC_Contour_T::SubsegmentList::iterator segment_start = current_dc_contour.subsegments.begin();
                  if (curr_num_vertices > 0U)
                  {
                     std::advance(segment_start, static_cast<ptrdiff_t>(curr_num_vertices - 1U));
                  }
                  if (segment_start == current_dc_contour.subsegments.end())
                  {
                     break;
                  }
                  update_new_leftovers(contour_new_leftovers, segment_start, leftovers_mask, curr_num_vertices,
                                       num_subvertices_to_add_safe, f_next_vertex_critical);

                  // if we are in the middle of the contour we need to update primacy of the last segment's end_vertex
                  if (segment_start != current_dc_contour.subsegments.begin())
                  {
                     auto prev_segment = segment_start;
                     --prev_segment;
                     prev_segment->end_vertex.f_primary = true;
                  }
                  segment_start->begin_vertex.f_primary  = true;
                  segment_start->segment_id              = current_vertex.segment_id;
                  segment_start->sg_age                  = current_vertex.age;
                  segment_start->sg_cycles_since_coasted = current_vertex.num_cycles_no_update;
                  for (uint16_t num_subvertices{1U}; num_subvertices < num_subvertices_to_add_safe; ++num_subvertices)
                  {
                     ++segment_start;
                     if (segment_start != current_dc_contour.subsegments.end())
                     {
                        segment_start->begin_vertex.f_primary  = false;
                        segment_start->segment_id              = current_vertex.segment_id;
                        segment_start->sg_age                  = current_vertex.age;
                        segment_start->sg_cycles_since_coasted = current_vertex.num_cycles_no_update;
                     }
                  }
               }
               else if ((f_next_vertex_critical && (i_primary_vertex == current_contour_vertices_count - 1U))
                        || f_current_vertex_critical || (vertex_iter == last_contour_vertex_it))
               {
                  assign_last_vertex_to_contour(current_dc_contour, current_contour, f_critical[current_contour_vertices_count - 1U]);
               }
               else if (!f_current_vertex_critical)
               {
                  assign_vertex_to_contour(current_dc_contour, current_vertex, next_vertex, f_next_vertex_critical);
               }
               else
               {
                  // MISRA
               }
            }

            i_primary_vertex++;
            (void) i_primary_vertex; // MISRA
         }
         const uint16_t curr_num_vertices = current_dc_contour.get_num_of_vertices();
         if (curr_num_vertices < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
         {
            const uint16_t current_contour_vertices_count = static_cast<uint16_t>(current_contour.vertices.size());
            assign_last_vertex_to_contour(current_dc_contour, current_contour, f_critical[current_contour_vertices_count - 1U]);
         }

         if ((contour_new_leftovers.num_new_leftovers > 0U) && (current_old_dc_contour_iter != dc_contour_list.end()))
         {
            add_not_projected_subsegments_to_leftovers(contour_old_leftovers, current_old_dc_contour_iter, f_projected_old);
         }

         std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> new_leftovers_assigned_mask{false};
         assign_closest_subsegments(new_leftovers_assigned_mask, contour_new_leftovers, contour_old_leftovers);

         populate_not_assigned_leftovers(dc_contour_list, contour_new_leftovers, new_leftovers_assigned_mask);
      }

      DCContourStorage::ContourList::iterator DC_Update_Subsegments::get_partner_dc_contour(
         const DCContourStorage &dc_contours, const DCContourStorage::ContourList::iterator dc_contour_list_it, const uint32_t id)
      {
         DCContourStorage::ContourList::iterator partner_dc_contour_it{nullptr};
         if (!dc_contours.empty())
         {
            // If dc_contour_list_it already points to a partner dc contour there is no need to iterate over all contours
            if ((dc_contour_list_it != dc_contours.end()) && (dc_contour_list_it->get_id() == id))
            {
               partner_dc_contour_it = dc_contour_list_it;
            }
            else
            {
               partner_dc_contour_it = dc_contours.begin();
               for (; partner_dc_contour_it != dc_contours.end(); ++partner_dc_contour_it)
               {
                  if (partner_dc_contour_it->get_id() == id)
                  {
                     break;
                  }
               }
               if (partner_dc_contour_it == dc_contours.end())
               {
                  partner_dc_contour_it = nullptr;
               }
            }
         }
         return partner_dc_contour_it;
      }
   }
}