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
                                            const geometry::Point2D_T &v, const bool f_beg, const bool f_end,
                                            const bool f_critical);

static void add_vertex(DC_Contour_T &dc_contour, const geometry::Point2D_T &vertex, const bool is_segment_begin,
                       const bool is_segment_end, const bool f_critical);

void amend_segment(DC_Contour_T &dc_contour, std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &f_leftover,
                   const UpdatedSegment &projected_segment, const geometry::Point2D_T &end_state,
                   const CriticalRegion &critical_region, const bool f_both_vertices_in_region, const float subsegment_length)
{
   if (dc_contour.get_num_of_vertices() < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
   {
      const auto squared_subsegment_length = subsegment_length * subsegment_length;

      auto squared_dist = std::numeric_limits<float>::min();

      if (projected_segment.num_elements > 1U)
      {
         // calculate distance between first (primary) vertex of segment and first projected subvertex on this segment
         squared_dist = geometry::squared_euclidean_distance(projected_segment.state[0U], projected_segment.state[1U]);
      }

      ArrayWrapper<geometry::Point2D_T> preliminary_subvertices;
      ArrayWrapper<geometry::Point2D_T> closing_subvertices;

      if (squared_dist > squared_subsegment_length)
      {
         // get subvertices between first (primary) vertex of segment and first projected subvertex on this segment
         get_subvertices_positions(preliminary_subvertices, projected_segment.state[1U], projected_segment.state[0U],
                                   subsegment_length, false, false);
      }

      if (projected_segment.num_elements > 0U)
      {
         // get subvertices between last projected subvertex and second (primary) vertex of segment (end_state)
         // including the second (primary) vertex
         get_subvertices_positions(closing_subvertices, projected_segment.state[projected_segment.num_elements - 1U], end_state,
                                   subsegment_length, false, true);
      }

      const uint16_t num_vertices_to_process =
         preliminary_subvertices.get_num_elements() + projected_segment.num_elements + closing_subvertices.get_num_elements();

      // index where projected subvertices start
      // (except the first one because projected_segment always includes first primary vertex)
      const uint16_t projected_subvertices_first_idx = preliminary_subvertices.get_num_elements() + 1U;
      (void)projected_subvertices_first_idx; // MISRA

      // index where closing subvertices start
      const uint16_t closing_subvertices_first_idx = preliminary_subvertices.get_num_elements() + projected_segment.num_elements;
      (void)closing_subvertices_first_idx; // MISRA

      bool f_current_subvertex_in_region = false;
      (void)f_current_subvertex_in_region; // MISRA
      bool f_next_subvertex_in_region = false;
      (void)f_next_subvertex_in_region; // MISRA

      auto num_leftovers = 0U;
      (void)num_leftovers; // MISRA
      for (uint16_t curr_idx = 0U; curr_idx < num_vertices_to_process; curr_idx++)
      {
         if (dc_contour.get_num_of_vertices() >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
         {
            break;
         }

         // find state of current subvertex
         const sg::geometry::Point2D_T *current_state = nullptr;
         (void)current_state;      // MISRA
         auto f_projected = false; // is subvertex a result of projection (from project_points_on_line) or interpolation
         (void)f_projected;        // MISRA
         if (curr_idx == 0U)       // first subvertex is always included as the first element of projected_segment
         {
            f_projected   = true;
            current_state = &projected_segment.state[0U];
         }
         else if (curr_idx < projected_subvertices_first_idx) // current subvertex is in preleminary_vertices
         {
            current_state = &preliminary_subvertices[preliminary_subvertices.get_num_elements() - curr_idx];
         }
         else if (curr_idx < closing_subvertices_first_idx) // current subvertex is in projected subvertices
         {
            f_projected   = true;
            current_state = &projected_segment.state[curr_idx - projected_subvertices_first_idx + 1U];
         }
         else // current subvertex is in closing subvertices
         {
            current_state = &closing_subvertices[curr_idx - closing_subvertices_first_idx];
         }

         // is it the first subvertex of segment (primary vertex)
         const auto f_first_subvertex = curr_idx == 0U;

         // is it the last subvertex of segment (secondary primary vertex)
         const auto f_last_subvertex = curr_idx == (num_vertices_to_process - 1U);

         // if it's the first subvertex, check if it's in region
         // we will reuse f_next_subvertex_in_region in next iterations
         if (f_first_subvertex)
         {
            f_current_subvertex_in_region =
               sg::dc::common::is_vertex_in_critical_region(critical_region, *current_state, f_both_vertices_in_region);
         }
         else
         {
            // we can use criticality of the next subvertex in the next iteration
            f_current_subvertex_in_region = f_next_subvertex_in_region;
         }

         // if it's the last subsegment, there is no next subvertex
         const sg::geometry::Point2D_T *next_state = nullptr;
         (void)next_state;
         if (!f_last_subvertex)
         {
            const uint16_t next_idx = curr_idx + 1U;

            // find state of the next subvertex
            if (next_idx < projected_subvertices_first_idx) // next subvertex is in preliminary_subvertices
            {
               next_state = &preliminary_subvertices[preliminary_subvertices.get_num_elements() - next_idx];
            }
            else if (next_idx < closing_subvertices_first_idx) // next subvertex is in projected subvertices
            {
               next_state = &projected_segment.state[next_idx - projected_subvertices_first_idx + 1U];
            }
            else // next subvertex is in closing subvertices
            {
               next_state = &closing_subvertices[next_idx - closing_subvertices_first_idx];
            }

            f_next_subvertex_in_region =
               sg::dc::common::is_vertex_in_critical_region(critical_region, *next_state, f_both_vertices_in_region);
         }
         else
         {
            f_next_subvertex_in_region = false;
         }

         if (f_current_subvertex_in_region || f_last_subvertex || f_first_subvertex)
         {
            if (f_projected && f_next_subvertex_in_region)

            {
               uint32_t projected_idx{0U};
               if (curr_idx > 0U)
               {
                  projected_idx = static_cast<uint32_t>(curr_idx) - static_cast<uint32_t>(projected_subvertices_first_idx) + 1U;
               }

               add_vertex_and_assign_past_data(dc_contour, projected_segment.assigned_old_subsegment[projected_idx],
                                               *current_state, f_first_subvertex, f_last_subvertex, f_current_subvertex_in_region);
               f_leftover[num_leftovers++] = projected_segment.f_leftover[projected_idx];
            }
            else
            {
               add_vertex(dc_contour, *current_state, f_first_subvertex, f_last_subvertex, f_current_subvertex_in_region);

               if (!f_last_subvertex)
               {
                  f_leftover[num_leftovers++] = f_current_subvertex_in_region && f_next_subvertex_in_region;
               }
            }

            // if number of subsegments is maximum allowed
            if ((dc_contour.get_num_of_vertices() >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR) && (next_state != nullptr))
            {
               // add *next_state as last subvertex of this contour
               add_vertex(dc_contour, *next_state, false, true, f_next_subvertex_in_region);
            }
         }
      }
   }
}

static void add_vertex_and_assign_past_data(DC_Contour_T &dc_contour,
                                            const DC_Contour_T::SubsegmentList::iterator &assigned_old_subsegment_it,
                                            const geometry::Point2D_T &v, const bool f_beg, const bool f_end,
                                            const bool f_critical)
{
   add_vertex(dc_contour, v, f_beg, f_end, f_critical);

   if ((assigned_old_subsegment_it != nullptr) && (!dc_contour.subsegments.empty()))
   {
      dc_contour.subsegments.back().subsegment_id = assigned_old_subsegment_it->subsegment_id;
      dc_contour.subsegments.back().past_data     = assigned_old_subsegment_it->past_data;
   }
}

static void add_vertex(DC_Contour_T &dc_contour, const geometry::Point2D_T &vertex, const bool is_segment_begin,
                       const bool is_segment_end, const bool f_critical)
{
   if (is_segment_begin)
   {
      Subsegment_T new_subsegment;
      new_subsegment.begin_vertex            = vertex;
      new_subsegment.begin_vertex.f_critical = f_critical;
      (void)dc_contour.subsegments.push_back(new_subsegment);
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
      (void)dc_contour.subsegments.push_back(new_subsegment);
   }
}
} // namespace dc
} // namespace sg
