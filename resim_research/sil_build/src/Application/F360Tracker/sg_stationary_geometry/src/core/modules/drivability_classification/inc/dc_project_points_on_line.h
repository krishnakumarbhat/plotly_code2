/*===================================================================================*\
* FILE: dc_project_points_on_line.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of project_points_on_line function and its inner functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_PROJECT_POINTS_ON_LINE
#define DC_PROJECT_POINTS_ON_LINE

#include <bitset>

#include "dc_array_wrapper.h"
#include "dc_common.h"
#include "dc_contour_storage.h"
#include "dc_critical_region.h"
#include "geometry/geo_line.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      /*=============================================================================================*\
       * @brief       Function projects subvertices from old segment to a new segment.
       *
       * @param[out]  projected_segment - segment with projected vertices
       * @param[out]  old_leftovers - list of iterators to subsegments that were not projected
       * @param[out]  f_projected_old - map if old subsegment was projected
       * @param[in]   primary_vertex_iter - iterator to the first vertex of the new segment
       * @param[in]   f_primary_vertex_critical - is primary_vertex critical
       * @param[in]   f_secondary_vertex_critical - is secondary_vertex critical
       * @param[in]   partner_segment_iter - iterator to the old segment that has matching id with new segment
       * @param[in]   partner_contour_iter - iterator to the contour of partner_segment
       * @param[in]   critical_region - critical region
       * @param[in]   old_subsegments - list of subsegments from the previous iteration
       * @param[in]   cfg - configuration
       *=============================================================================================*/
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
                                  const Drivability_Classification_Calibrations_T &cfg);

      /*=============================================================================================*\
       * @brief       Function calculates rotation of subvertices from the old segment to the new segment
       *
       * @param[out]  subvertices - calculated subvertices
       * @param[out]  partner_segment_iter - iterator to the old segment that has matching id with new segment
       * @param[in]   current_segment_id - id of the current segment
       * @param[in]   old_subsegments - list of subsegments from the previous iteration
       * @param[in]   angle - angle by which to rotate
       * @param[in]   intersection_point - point around which to rotate
       *=============================================================================================*/
      void calculate_rotated_subvertices(ArrayWrapper<geometry::Point2D_T, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &subvertices,
                                         DC_Contour_T::SubsegmentList::iterator partner_segment_iter,
                                         const uint32_t current_segment_id,
                                         const DC_Contour_T::SubsegmentList &old_subsegments,
                                         const float angle,
                                         const geometry::Point2D_T &intersection_point);

      /*=============================================================================================*\
       * @brief       Function calculates perpendicular lines intersections
       *
       * @param[out]  subvertices - calculated subvertices
       * @param[out]  partner_segment_iter - iterator to the old segment that has matching id with new segment
       * @param[in]   current_segment_id - id of the current segment
       * @param[in]   old_subsegments - list of subsegments from the previous iteration
       * @param[in]   new_line - line with which to find intersections
       *=============================================================================================*/
      void calculate_perpendicular_lines_intersections(ArrayWrapper<geometry::Point2D_T, SG_MAX_NUM_SUBVERTICES_PER_CONTOUR> &subvertices,
                                                       DC_Contour_T::SubsegmentList::iterator partner_segment_iter,
                                                       const uint32_t current_segment_id,
                                                       const DC_Contour_T::SubsegmentList &old_subsegments,
                                                       const geometry::Line_T &new_line);

      /*=============================================================================================*\
       * @brief       Function projects subvertices from old segment to a new segment.
       *
       * @param[out]  projected_segment - segment with projected vertices
       * @param[out]  old_leftovers - list of iterators to subsegments that were not projected
       * @param[out]  f_projected_old - map if old subsegment was projected
       * @param[out]  subvertices - calculated subvertices
       * @param[in]   primary_vertex_critical - is primary_vertex critical
       * @param[in]   secondary_vertex_critical - is secondary_vertex critical
       * @param[in]   old_subsegment - list of subsegments from the previous iteration
       * @param[in]   partner_segment_iter - iterator to the old segment that has matching id with new segment
       * @param[in]   partner_contour_iter - iterator to the contour of partner_segment
       * @param[in]   primary_vertex_iter - iterator to the first vertex of the new segment
       * @param[in]   critical_region - critical region
       * @param[in]   cfg - configuration
       *=============================================================================================*/
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
                                                      const Drivability_Classification_Calibrations_T &cfg);

      /*=============================================================================================*\
       * @brief       Function checks if point lies on a segment.
       *
       * @param[in]   point - 2 dimensional point to check
       * @param[in]   segment_begin - first vertex of segment, 2 dimensional point
       * @param[in]   segment_end - second (last) vertex of segment, 2 dimensional point
       *
       * @return      bool - is point on segment
       *=============================================================================================*/
      bool is_point_on_segment(const geometry::Point2D_T &point,
                               const geometry::Point2D_T &segment_begin,
                               const geometry::Point2D_T &segment_end);
   }
}

#endif
