/*===================================================================================*\
* FILE: dc_project_points_on_line.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definitions of functions used to project old subvertices to a new segment.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_project_points_on_line.h"

#include <cmath>

#include "dc_is_vertex_in_critical_region.h"
#include "geometry/geo_angle.h"
#include "geometry/geo_distance.h"
#include "geometry/geo_is_inside.h"
#include "geometry/geo_rotate.h"

namespace sg
{
   namespace dc
   {
      void project_points_on_line(UpdatedSegment &projected_segment,
                                  OldLeftovers &old_leftovers,
                                  std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &f_projected_old,
                                  Contour_T::VertexList::iterator primary_vertex_iter,
                                  const bool f_primary_vertex_critical,
                                  const bool f_secondary_vertex_critical,
                                  const DC_Contour_T::SubsegmentList::iterator partner_segment_iter,
                                  const DCContourStorage::ContourList::iterator partner_contour_iter,
                                  const CriticalRegion &critical_region,
                                  const DC_Contour_T::SubsegmentList &old_subsegments,
                                  const Drivability_Classification_Calibrations_T &cfg)
      {
         const auto iter_begin = primary_vertex_iter;
         ++primary_vertex_iter;
         const auto iter_end = primary_vertex_iter;

         const auto &new_point1 = *iter_begin;
         const auto &new_point2 = *iter_end;

         const auto &old_point1 = (*partner_segment_iter).begin_vertex;
         const auto &old_point2 = (*partner_segment_iter).end_vertex;

         // create lines which contain new and old segments
         const geometry::Line_T new_line{new_point1.position, new_point2.position};
         const geometry::Line_T old_line{old_point1.position, old_point2.position};

         // we want to project point to the direction of the point of the same id
         // old_point1 has the same id as new_point1

         // angle in radians
         auto signed_rad = 0.0F;

         if (!(((std::abs(new_line.a) < std::numeric_limits<float>::epsilon())
                && (std::abs(old_line.a) < std::numeric_limits<float>::epsilon()))
               || ((std::abs(new_line.b) < std::numeric_limits<float>::epsilon())
                   && (std::abs(old_line.b) < std::numeric_limits<float>::epsilon()))))
         {
            const geometry::Angle2D_T angle{old_point2.position - old_point1.position, new_point2.position - new_point1.position};
            signed_rad = angle.signed_rad();
         }

         ArrayWrapper<geometry::Point2D_T, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> subvertices;
         if (std::fabs(signed_rad) < cfg.angle_between_segments_threshold)
         {
            calculate_perpendicular_lines_intersections(subvertices, partner_segment_iter, (*iter_begin).segment_id,
                                                        old_subsegments, new_line);
         }
         else
         {
            // compute intersection point of two lines
            const auto intersection_point = new_line.intersection(old_line);
            calculate_rotated_subvertices(subvertices, partner_segment_iter, (*iter_begin).segment_id, old_subsegments, signed_rad,
                                          intersection_point);
         }

         create_segment_from_intersection_positions(projected_segment, old_leftovers, f_projected_old, subvertices,
                                                    f_primary_vertex_critical, f_secondary_vertex_critical, old_subsegments,
                                                    partner_segment_iter, partner_contour_iter, iter_begin, critical_region, cfg);
      }

      void calculate_rotated_subvertices(ArrayWrapper<geometry::Point2D_T, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &subvertices,
                                         DC_Contour_T::SubsegmentList::iterator partner_segment_iter,
                                         const uint32_t current_segment_id,
                                         const DC_Contour_T::SubsegmentList &old_subsegments,
                                         const float angle,
                                         const geometry::Point2D_T &intersection_point)
      {
         auto reached_end = true;
         for (; partner_segment_iter != old_subsegments.end(); ++partner_segment_iter)
         {
            const auto &subsegment = *partner_segment_iter;
            if ((subsegment.segment_id != current_segment_id)
                || (subvertices.get_num_elements() == SG_MAX_NUM_SUBVERTICES_PER_CONTOUR))
            {
               reached_end = false;
               break;
            }

            auto point = subsegment.begin_vertex.position;
            sg::geometry::rotate(point, angle, intersection_point);

            subvertices.add_element(point);
         }

         if (reached_end)
         {
            const auto &subsegment = old_subsegments.back();

            auto point = subsegment.end_vertex.position;
            sg::geometry::rotate(point, angle, intersection_point);

            subvertices.add_element(point);
         }
      }

      void calculate_perpendicular_lines_intersections(ArrayWrapper<geometry::Point2D_T, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &subvertices,
                                                       DC_Contour_T::SubsegmentList::iterator partner_segment_iter,
                                                       const uint32_t current_segment_id,
                                                       const DC_Contour_T::SubsegmentList &old_subsegments,
                                                       const geometry::Line_T &new_line)
      {
         auto reached_end = true;
         for (; partner_segment_iter != old_subsegments.end(); ++partner_segment_iter)
         {
            const auto &subsegment = *partner_segment_iter;
            if ((subsegment.segment_id != current_segment_id)
                || (subvertices.get_num_elements() == SG_MAX_NUM_SUBVERTICES_PER_CONTOUR))
            {
               reached_end = false;
               break;
            }

            const auto &old_begin_pos     = subsegment.begin_vertex.position;
            const auto intersection_point = new_line.orthogonal_projection(old_begin_pos);

            subvertices.add_element(intersection_point);
         }

         if (reached_end)
         {
            const auto &subsegment = old_subsegments.back();

            const auto &old_begin_pos     = subsegment.end_vertex.position;
            const auto intersection_point = new_line.orthogonal_projection(old_begin_pos);

            subvertices.add_element(intersection_point);
         }
      }

      void create_segment_from_intersection_positions(UpdatedSegment &projected_segment,
                                                      OldLeftovers &old_leftovers,
                                                      std::bitset<SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &f_projected_old,
                                                      const ArrayWrapper<geometry::Point2D_T, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &subvertices,
                                                      const bool f_primary_vertex_critical,
                                                      const bool f_secondary_vertex_critical,
                                                      const DC_Contour_T::SubsegmentList &old_subsegments,
                                                      const DC_Contour_T::SubsegmentList::iterator partner_segment_iter,
                                                      const DCContourStorage::ContourList::iterator partner_contour_iter,
                                                      const Contour_T::VertexList::iterator primary_vertex_iter,
                                                      const CriticalRegion &critical_region,
                                                      const Drivability_Classification_Calibrations_T &cfg)
      {
         const auto current_segment_id = (*primary_vertex_iter).segment_id;
         (void) current_segment_id; // MISRA
         const auto squared_dist_threshold = cfg.projection_distance_threshold * cfg.projection_distance_threshold;
         (void) squared_dist_threshold; // MISRA

         projected_segment.state[projected_segment.num_elements] = (*primary_vertex_iter).position;
         (void) projected_segment.f_critical.set(static_cast<size_t>(projected_segment.num_elements), f_primary_vertex_critical);

         auto secondary_vertex_iter = primary_vertex_iter;
         secondary_vertex_iter++;

         if (f_primary_vertex_critical)
         {
            projected_segment.assigned_old_subsegment[projected_segment.num_elements] = nullptr;
            (void) projected_segment.f_leftover.set(static_cast<size_t>(projected_segment.num_elements), true);
         }
         else if (f_secondary_vertex_critical)
         {
            projected_segment.assigned_old_subsegment[projected_segment.num_elements] = nullptr;
            (void) projected_segment.f_leftover.set(static_cast<size_t>(projected_segment.num_elements), false);
         }
         else
         {
            // MISRA
         }
         projected_segment.num_elements++;

         uint16_t state_counter{0U};

         auto subsegment_idx = std::distance(old_subsegments.begin(), partner_segment_iter); // can be potentially optimized
         for (auto sub_iter = partner_segment_iter; sub_iter != old_subsegments.end(); sub_iter++)
         {
            const auto &subsegment = *sub_iter;
            if ((subsegment.segment_id != current_segment_id)
                || (projected_segment.num_elements >= SG_MAX_NUM_SUBVERTICES_PER_CONTOUR))
            {
               break;
            }
            const auto &new_subvertex_position  = subvertices[state_counter];
            const auto f_new_subvertex_critical = common::is_vertex_in_critical_region(
               critical_region, new_subvertex_position, f_primary_vertex_critical && f_secondary_vertex_critical);
            const auto f_new_subvertex_on_segment =
               is_point_on_segment(new_subvertex_position, (*primary_vertex_iter).position, (*secondary_vertex_iter).position);
            (void) f_new_subvertex_on_segment; // MISRA

            auto next_subsegment = sub_iter;
            next_subsegment++;
            (void) next_subsegment; // MISRA
            if ((!f_secondary_vertex_critical)
                && ((next_subsegment == old_subsegments.end()) || (next_subsegment->segment_id != current_segment_id))
                && f_new_subvertex_critical && f_new_subvertex_on_segment)
            {
               projected_segment.state[projected_segment.num_elements]                   = new_subvertex_position;
               projected_segment.assigned_old_subsegment[projected_segment.num_elements] = nullptr;

               (void) projected_segment.f_critical.set(static_cast<size_t>(projected_segment.num_elements), true);
               (void) projected_segment.f_leftover.set(static_cast<size_t>(projected_segment.num_elements), false);
               projected_segment.num_elements++;
               break;
            }

            const auto dist_to_begin = geometry::squared_euclidean_distance((*primary_vertex_iter).position, new_subvertex_position);
            const auto dist_to_end = geometry::squared_euclidean_distance((*secondary_vertex_iter).position, new_subvertex_position);
            (void) dist_to_begin; // MISRA
            (void) dist_to_end;   // MISRA

            if (f_new_subvertex_critical && f_new_subvertex_on_segment && (dist_to_begin > squared_dist_threshold)
                && (dist_to_end > squared_dist_threshold))
            {
               projected_segment.state[projected_segment.num_elements] = new_subvertex_position;
               (void) projected_segment.f_critical.set(static_cast<size_t>(projected_segment.num_elements), true);

               projected_segment.assigned_old_subsegment[projected_segment.num_elements] = sub_iter;
               (void) projected_segment.f_leftover.set(static_cast<size_t>(projected_segment.num_elements),
                                                       subsegment.subsegment_id == INVALID_SEGMENT_ID);

               (void) f_projected_old.set(static_cast<size_t>(subsegment_idx), true);
               projected_segment.num_elements++;
            }
            else if (subsegment.subsegment_id != INVALID_SEGMENT_ID)
            {
               if (old_leftovers.num_old_leftovers < SG_MAX_NUM_SUBVERTICES_PER_CONTOUR)
               {
                  old_leftovers.subsegment_leftovers[old_leftovers.num_old_leftovers] = sub_iter;
                  old_leftovers.contour_leftovers[old_leftovers.num_old_leftovers]    = partner_contour_iter;
                  old_leftovers.num_old_leftovers++;
                  (void) f_projected_old.set(static_cast<size_t>(subsegment_idx), true);
               }
            }
            else
            {
               // MISRA
            }
            state_counter++;
            subsegment_idx++;
         }
         (void) state_counter;  // MISRA
         (void) subsegment_idx; // MISRA
      }

      bool is_point_on_segment(const geometry::Point2D_T &point,
                               const geometry::Point2D_T &segment_begin,
                               const geometry::Point2D_T &segment_end)
      {
         auto f_on_subsegment = true;
         const auto tolerance = 10e-4F;

         const auto diff_segment           = segment_end - segment_begin;
         const auto diff_point_segment_beg = point - segment_begin;

         const auto cross_product = diff_point_segment_beg.y * diff_segment.x - diff_point_segment_beg.x * diff_segment.y;

         // check if point and segment are aligned
         if (std::fabs(cross_product) > tolerance)
         {
            f_on_subsegment = false;
         }
         else
         {
            const auto dot_product = diff_segment * diff_point_segment_beg;

            if (dot_product < 0.0F)
            {
               f_on_subsegment = false;
            }
            else
            {
               const auto squared_segment_length = geometry::squared_euclidean_distance(segment_end, segment_begin);

               if ((dot_product - squared_segment_length) > tolerance)
               {
                  f_on_subsegment = false;
               }
            }
         }
         return f_on_subsegment;
      }
   }
}
