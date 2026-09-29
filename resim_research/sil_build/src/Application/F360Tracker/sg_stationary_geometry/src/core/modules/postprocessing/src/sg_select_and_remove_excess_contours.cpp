/*=============================================================================================*\
* FILE: sg_select_and_remove_excess_contours.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions for select_and_remove_excess_contours function
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_select_and_remove_excess_contours.h"

#include <cfloat>
#include <cmath>

#include "geometry/geo_distance.h"
#include "geometry/geo_projection.h"
#include "sg_extract_contoured_cluster_ids.h"

namespace sg
{
   void select_and_remove_excess_contours(
      ContourStorage &contours,
      const HostProps &host_properties,
      const Contour_Postprocessing_Calibrations_T::Select_And_Remove_Excess_Contours_T &contour_postprocessing_calibrations,
      const Common_Calibrations_T &common_calibrations)
   {
      std::array<uint16_t, SG_MAX_NUM_CONTOURS> contoured_cluster_ids;
      const auto num_clusters = extract_contoured_cluster_ids(contours, contoured_cluster_ids);

      // Check if there are any excess contours
      if (num_clusters >= contour_postprocessing_calibrations.max_num_contoured_clusters)
      {
         const auto longitudinal_view_ranges = {std::fabs(common_calibrations.view_ranges.nondrivable.longitudinal.min),
                                                std::fabs(common_calibrations.view_ranges.nondrivable.longitudinal.max),
                                                std::fabs(common_calibrations.view_ranges.overdrivable.longitudinal.min),
                                                std::fabs(common_calibrations.view_ranges.overdrivable.longitudinal.max),
                                                std::fabs(common_calibrations.view_ranges.underdrivable.longitudinal.min),
                                                std::fabs(common_calibrations.view_ranges.underdrivable.longitudinal.max)};

         const auto lateral_view_ranges = {std::fabs(common_calibrations.view_ranges.nondrivable.lateral.min),
                                           std::fabs(common_calibrations.view_ranges.nondrivable.lateral.max),
                                           std::fabs(common_calibrations.view_ranges.overdrivable.lateral.min),
                                           std::fabs(common_calibrations.view_ranges.overdrivable.lateral.max),
                                           std::fabs(common_calibrations.view_ranges.underdrivable.lateral.min),
                                           std::fabs(common_calibrations.view_ranges.underdrivable.lateral.max)};

         const auto max_longitudinal_view_range = std::max(longitudinal_view_ranges);
         const auto max_lateral_view_range      = std::max(lateral_view_ranges);

         const auto max_view_range =
            sqrtf(max_longitudinal_view_range * max_longitudinal_view_range + max_lateral_view_range * max_lateral_view_range);

         auto cluster_distances =
            get_cluster_to_host_distances(contours, contoured_cluster_ids, num_clusters, host_properties.position, max_view_range);

         sort_by_distance(cluster_distances, num_clusters);

         remove_excess_contours(contours, cluster_distances, num_clusters,
                                contour_postprocessing_calibrations.max_num_contoured_clusters);
      }
   }

   std::array<std::pair<uint16_t, float>, SG_MAX_NUM_CONTOURS>
   get_cluster_to_host_distances(const ContourStorage &contours,
                                 const std::array<uint16_t, SG_MAX_NUM_CONTOURS> &contoured_cluster_ids,
                                 const uint16_t num_clusters,
                                 const geometry::Point2D_T &host_position,
                                 const float max_view_range)
   {
      std::array<std::pair<uint16_t, float>, SG_MAX_NUM_CONTOURS> cluster_distances;
      std::fill(cluster_distances.begin(), cluster_distances.end(), std::pair<uint16_t, float>(INVALID_CLUSTER_ID, max_view_range));

      for (const auto &contour : contours)
      {
         const auto current_distance = get_contour_to_host_distance(contour, host_position);

         for (auto i = 0U; i < num_clusters; i++)
         {
            if (contoured_cluster_ids[i] == contour.cluster_id)
            {
               cluster_distances[i].first  = contour.cluster_id;
               cluster_distances[i].second = std::min(cluster_distances[i].second, current_distance);
               break;
            }
         }
         (void) current_distance; // MISRA
      }
      return cluster_distances;
   }

   void sort_by_distance(std::array<std::pair<uint16_t, float>, SG_MAX_NUM_CONTOURS> &cluster_distances, const uint16_t num_clusters)
   {
      std::sort(cluster_distances.begin(), cluster_distances.begin() + static_cast<std::ptrdiff_t>(num_clusters),
                [](const auto &lhs, const auto &rhs) { return lhs.second < rhs.second; });
   }

   void remove_excess_contours(ContourStorage &contours,
                               const std::array<std::pair<uint16_t, float>, SG_MAX_NUM_CONTOURS> &cluster_distances,
                               const uint16_t num_clusters,
                               const uint16_t max_num_contoured_clusters)
   {
      for (uint16_t idx = max_num_contoured_clusters - 1U; idx <= num_clusters; idx++)
      {
         for (auto cont_it = contours.begin(); cont_it != contours.end(); cont_it++)
         {
            if (cont_it->cluster_id == cluster_distances[idx].first)
            {
               (void) contours.erase(cont_it);
            }
         }
      }
   }

   float get_contour_to_host_distance(const Contour_T &contour, const geometry::Point2D_T &host_position)
   {
      // Check if contour has at least one segment
      assert(contour.vertices.size() > 1U);

      // get closest vertex
      float min_distance = FLT_MAX;
      Contour_T::VertexList::iterator min_distance_it{};

      const auto contour_vertices_end_it = contour.vertices.end();

      for (auto vertex_it = contour.vertices.begin(); vertex_it != contour_vertices_end_it; vertex_it++)
      {
         const float host_distance_x = vertex_it->position.x - host_position.x;
         const float host_distance_y = vertex_it->position.y - host_position.y;

         const float current_distance =
            geometry::euclidean_distance(geometry::Point2D_T{0.0F, 0.0F}, geometry::Point2D_T{host_distance_x, host_distance_y});
         if (current_distance < min_distance)
         {
            min_distance_it = vertex_it;
            min_distance    = current_distance;
         }
      }
      (void) min_distance; // MISRA

      auto distance_pre = FLT_MAX;

      if (min_distance_it != contour.vertices.begin())
      {
         const auto &min_distance_vertex = *min_distance_it;

         // calculate footpoints of segment before closest vertex
         auto &prev_min_distance_vertex = *std::prev(min_distance_it);

         geometry::Point2D_T first_point_pre{prev_min_distance_vertex.position.x, prev_min_distance_vertex.position.y};
         geometry::Point2D_T second_point_pre{min_distance_vertex.position.x, min_distance_vertex.position.y};

         const geometry::Segment2D_T segment_pre{first_point_pre, second_point_pre};
         const auto foot_point_pre =
            geometry::make_projection(segment_pre, geometry::Point2D_T{host_position.x, host_position.y}, true);

         distance_pre = geometry::euclidean_distance(geometry::Point2D_T{host_position.x, host_position.y}, foot_point_pre.point);
      }

      auto distance_post = FLT_MAX;

      if (min_distance_it != --contour.vertices.end())
      {
         const auto &min_distance_vertex = *min_distance_it;

         // calculate footpoints of segment after closest vertex
         auto &next_min_distance_vertex = *std::next(min_distance_it);

         geometry::Point2D_T first_point_post{min_distance_vertex.position.x, min_distance_vertex.position.y};
         geometry::Point2D_T second_point_post{next_min_distance_vertex.position.x, next_min_distance_vertex.position.y};

         const geometry::Segment2D_T segment_post{first_point_post, second_point_post};
         const auto foot_point_post =
            geometry::make_projection(segment_post, geometry::Point2D_T{host_position.x, host_position.y}, true);

         distance_post = geometry::euclidean_distance(geometry::Point2D_T{host_position.x, host_position.y}, foot_point_post.point);
      }

      return (distance_pre < distance_post) ? distance_pre : distance_post;
   }
}
