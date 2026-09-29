/*===================================================================================*\
* FILE: dc_amend_segment.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition amend_segment function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_amend_segment.h"

#include "dc_get_subvertices_positions.h"
#include "dc_is_vertex_in_critical_region.h"
#include "geometry/geo_distance.h"

namespace sg
{
   namespace dc
   {
      static void add_vertex_and_assign_past_data(DC_Contour_T &dc_contour,
                                                  const DC_Contour_T::SubsegmentList::iterator &assigned_old_subsegment_it,
                                                  const geometry::Point2D_T &v,
                                                  const bool f_beg,
                                                  const bool f_end,
                                                  const bool f_critical);

      static void add_vertex(DC_Contour_T &dc_contour,
                             const geometry::Point2D_T &vertex,
                             const bool is_segment_begin,
                             const bool is_segment_end,
                             const bool f_critical);

      void amend_segment(DC_Contour_T &dc_contour,
                         std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &f_leftover,
                         const UpdatedSegment &projected_segment,
                         const geometry::Point2D_T &end_state,
                         const CriticalRegion &critical_region,
                         const bool f_end_vertex_in_region,
                         const float subsegment_length)
      {
         if (dc_contour.get_num_of_vertices() < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
         {
            auto num_leftovers = 0U;
            add_vertex_and_assign_past_data(dc_contour, projected_segment.assigned_old_subsegment[0U], projected_segment.state[0U],
                                            true, false, projected_segment.f_critical[0U]);
            f_leftover[num_leftovers++] = projected_segment.f_leftover[0U];

            const auto squared_subsegment_length = subsegment_length * subsegment_length;
            if (projected_segment.num_elements > 1U)
            {
               const auto squared_dist1 =
                  geometry::squared_euclidean_distance(projected_segment.state[0U], projected_segment.state[1U]);

               if (squared_dist1 > squared_subsegment_length)
               {
                  ArrayWrapper<geometry::Point2D_T> preliminary_subvertices;
                  get_subvertices_positions(preliminary_subvertices, projected_segment.state[1U], projected_segment.state[0U],
                                            subsegment_length, false, false);

                  const auto subvertices_rend_it = preliminary_subvertices.rend();
                  for (auto it = preliminary_subvertices.rbegin(); it != subvertices_rend_it; it++)
                  {
                     const auto &new_state = *it;

                     if (dc_contour.get_num_of_vertices() >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
                     {
                        break;
                     }

                     if (sg::dc::common::is_vertex_in_critical_region(critical_region, new_state))
                     {
                        add_vertex(dc_contour, new_state, false, false, true);
                        f_leftover[num_leftovers++] = true;
                     }
                  }
               }

               for (auto i = 1U; i < projected_segment.num_elements; i++)
               {
                  if (dc_contour.get_num_of_vertices() >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
                  {
                     break;
                  }
                  add_vertex_and_assign_past_data(dc_contour, projected_segment.assigned_old_subsegment[i],
                                                  projected_segment.state[i], false, false, projected_segment.f_critical[i]);
                  f_leftover[num_leftovers++] = projected_segment.f_leftover[i];
               }
            }

            if (projected_segment.num_elements > 0U)
            {
               const auto squared_dist2 =
                  geometry::squared_euclidean_distance(projected_segment.state[projected_segment.num_elements - 1U], end_state);

               if (squared_dist2 > squared_subsegment_length)
               {
                  ArrayWrapper<geometry::Point2D_T> closing_subvertices;
                  get_subvertices_positions(closing_subvertices, projected_segment.state[projected_segment.num_elements - 1U],
                                            end_state, subsegment_length, false, true);

                  std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> f_in_region;

                  for (auto i = 0U; i < closing_subvertices.get_num_elements(); i++)
                  {
                     f_in_region.set(i, sg::dc::common::is_vertex_in_critical_region(critical_region, closing_subvertices[i]));
                  }

                  if (closing_subvertices.get_num_elements() > 0U)
                  {
                     for (auto it = closing_subvertices.begin();
                          it != std::next(closing_subvertices.begin(), closing_subvertices.get_num_elements() - 1U); it++)
                     {
                        if (dc_contour.get_num_of_vertices() >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
                        {
                           break;
                        }

                        const auto interpolated_subvertex_idx = static_cast<uint16_t>(std::distance(closing_subvertices.begin(), it));
                        const auto &new_state = *it;

                        if (f_in_region[interpolated_subvertex_idx])
                        {
                           if (f_in_region[interpolated_subvertex_idx + 1U])
                           {
                              add_vertex(dc_contour, new_state, false, false, true);
                              f_leftover[num_leftovers++] = true;
                           }
                           else
                           {
                              add_vertex(dc_contour, new_state, false, false, false);
                           }
                        }
                     }
                  }
               }
            }
            if (f_end_vertex_in_region && (dc_contour.get_num_of_vertices() < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR))
            {
               add_vertex(dc_contour, end_state, false, true, true);
            }
            else
            {
               add_vertex(dc_contour, end_state, false, true, false);

               auto &last_subsegment         = dc_contour.subsegments.back();
               last_subsegment.past_data     = Past_Data_T();
               last_subsegment.subsegment_id = INVALID_SEGMENT_ID;
            }
         }
      }

      static void add_vertex_and_assign_past_data(DC_Contour_T &dc_contour,
                                                  const DC_Contour_T::SubsegmentList::iterator &assigned_old_subsegment_it,
                                                  const geometry::Point2D_T &v,
                                                  const bool f_beg,
                                                  const bool f_end,
                                                  const bool f_critical)
      {
         add_vertex(dc_contour, v, f_beg, f_end, f_critical);

         if ((assigned_old_subsegment_it != nullptr) && (!dc_contour.subsegments.empty()))
         {
            dc_contour.subsegments.back().subsegment_id = assigned_old_subsegment_it->subsegment_id;
            dc_contour.subsegments.back().past_data     = assigned_old_subsegment_it->past_data;
         }
      }

      static void add_vertex(DC_Contour_T &dc_contour,
                             const geometry::Point2D_T &vertex,
                             const bool is_segment_begin,
                             const bool is_segment_end,
                             const bool f_critical)
      {
         if (is_segment_begin)
         {
            Subsegment_T new_subsegment;
            new_subsegment.begin_vertex            = vertex;
            new_subsegment.begin_vertex.f_critical = f_critical;
            (void) dc_contour.subsegments.push_back(new_subsegment);
         }
         else if (is_segment_end)
         {
            dc_contour.subsegments.back().end_vertex            = vertex;
            dc_contour.subsegments.back().end_vertex.f_critical = f_critical;
         }
         else
         {
            dc_contour.subsegments.back().end_vertex            = vertex;
            dc_contour.subsegments.back().end_vertex.f_critical = f_critical;
            Subsegment_T new_subsegment;
            new_subsegment.begin_vertex            = vertex;
            new_subsegment.begin_vertex.f_critical = f_critical;
            (void) dc_contour.subsegments.push_back(new_subsegment);
         }
      }
   }
}
