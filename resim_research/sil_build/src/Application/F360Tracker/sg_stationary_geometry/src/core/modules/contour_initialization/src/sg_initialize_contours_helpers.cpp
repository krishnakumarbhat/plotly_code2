/*=============================================================================================*\
* FILE: sg_initialize_contours_helpers.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitiions for initilize_contours helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_initialize_contours_helpers.h"

#include <algorithm>
#include <limits>

#include "sg_contour.h"
#include "sg_dbscan.h"
#include "sg_extract_contoured_cluster_ids.h"
#include "sg_get_unoccluded_dets.h"
#include "sg_math.h"

namespace sg
{
   bool collect_dets_for_a_new_contour(const ContourStorage &contours,
                                       const Cluster *const current_cluster_ptr,
                                       const Contour_Initialization_Calibrations_T &calibrations,
                                       const float azimuth_epsilon)
   {
      std::bitset<SG_MAX_NUM_CONTOURS> contours_of_cluster_mask;

      get_contours_of_cluster_mask(contours_of_cluster_mask, contours, current_cluster_ptr->unique_id());
      const bool f_success         = check_if_contour_creation_is_possible(current_cluster_ptr, contours, contours_of_cluster_mask,
                                                                           calibrations, azimuth_epsilon);
      std::size_t any_det_f_subset = false;
      const auto current_cluster_end = current_cluster_ptr->end();

      if (f_success)
      {
         dbscan(current_cluster_ptr, calibrations.sub_cluster_radius, calibrations.sub_min_cluster_points);
         adjust_results_of_dbscan(current_cluster_ptr);

         static_assert(SG_MAX_NUM_INTERNAL_DETS <= std::numeric_limits<std::size_t>::max(), "Possible number of internal "
                                                                                            "detections is too high for current "
                                                                                            "implementation");
         const std::size_t non_zero_elements_count =
            static_cast<std::size_t>(std::count_if(current_cluster_ptr->begin(), current_cluster_ptr->end(),
                                                   [](const Detection_T &det) { return det.temp_cluster_id > 0U; }));
         if (non_zero_elements_count > 0U)
         {
            std::array<uint16_t, SG_MAX_NUM_INTERNAL_DETS> sub_dets_cluster_ids{};
            std::size_t detection_idx = 0U;
            for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
            {
               if (detection_idx < SG_MAX_NUM_INTERNAL_DETS) // MISRA
               {
                  sub_dets_cluster_ids[detection_idx] = det_it->temp_cluster_id;
               }
               else
               {
                  assert(false);
               }
               ++detection_idx;
            }
            (void) detection_idx; // MISRA

            const uint16_t chosen_subcluster_id = find_most_frequent_nonzero_value(sub_dets_cluster_ids);
            for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
            {
               det_it->f_subset = (det_it->temp_cluster_id == chosen_subcluster_id);
               if (det_it->f_subset)
               {
                  any_det_f_subset = true;
               }
            }

            (void) chosen_subcluster_id; // MISRA
         }
      }
      else
      {
         any_det_f_subset = false;
      }
      return any_det_f_subset;
   }

   void adjust_results_of_dbscan(const Cluster *const current_cluster_ptr)
   {
      const auto current_cluster_end = current_cluster_ptr->end();
      for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
      {
         if ((det_it->f_subset) && (det_it->temp_cluster_id == INVALID_CLUSTER_ID))
         {
            det_it->f_subset = false;
         }
      }
   }

   void get_contours_of_cluster_mask(std::bitset<SG_MAX_NUM_CONTOURS> &contours_mask,
                                     const ContourStorage &contours,
                                     const uint16_t current_cluster_id)
   {
      std::size_t i              = 0U;
      const auto contours_end_it = contours.end();

      for (auto contour_it = contours.begin(); contour_it != contours_end_it; ++contour_it)
      {
         if (contour_it->cluster_id == current_cluster_id)
         {
            contours_mask[i] = true;
         }
         i++;
      }
      (void) i; // MISRA
   }

   bool check_if_contour_creation_is_possible(const Cluster *const current_cluster_ptr,
                                              const ContourStorage &contours,
                                              const std::bitset<SG_MAX_NUM_CONTOURS> &contours_of_cluster_mask,
                                              const Contour_Initialization_Calibrations_T &calibrations,
                                              const float azimuth_epsilon)
   {
      bool f_success                             = true;
      const std::size_t num_of_unassociated_dets = std::count_if(current_cluster_ptr->begin(), current_cluster_ptr->end(),
                                                                 [](const auto &det)
                                                                 { return det.contour_id == INVALID_CONTOUR_ID; });
      const auto current_cluster_end             = current_cluster_ptr->end();

      if (check_if_detections_belong_to_moving_objects(contours, num_of_unassociated_dets, current_cluster_ptr,
                                                       contours_of_cluster_mask, calibrations))
      {
         f_success = false;
         for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
         {
            det_it->f_subset = (det_it->contour_id == INVALID_CONTOUR_ID);
         }
      }
      // if there are contours and some unassociated detections in the cluster,
      //  check if the number of unoccluded is sufficient
      else if ((contours_of_cluster_mask.any()) && (num_of_unassociated_dets > 0U))
      {
         const std::size_t num_of_unoccluded_dets =
            get_unoccluded_dets(current_cluster_ptr, contours, calibrations, contours_of_cluster_mask, azimuth_epsilon);

         if (num_of_unoccluded_dets < calibrations.min_dets_for_contour_init)
         {
            f_success = false;
         }
      }
      //  otherwise check if a new contour can be created on this cluster
      else
      {
         std::size_t f_subset_count = 0U;
         for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
         {
            if (det_it->contour_id == INVALID_CONTOUR_ID)
            {
               det_it->f_subset = true;
               ++f_subset_count;
            }
            else
            {
               det_it->f_subset = false;
            }
         }

         std::array<uint16_t, SG_MAX_NUM_CONTOURS> cluster_ids{};
         std::fill(cluster_ids.begin(), cluster_ids.end(), INVALID_CLUSTER_ID);
         const auto num_contoured_clusters = extract_contoured_cluster_ids(contours, cluster_ids);

         // if min number of dets is not met or we reached the limit of contoured clusters - continue
         if ((calibrations.max_num_init_contoured_clusters <= num_contoured_clusters)
             || (f_subset_count < calibrations.min_dets_for_first_contour_init))
         {
            f_success = false;
         }

         (void) f_subset_count; // MISRA
      }
      return f_success;
   }

   bool check_if_detections_belong_to_moving_objects(const ContourStorage &contours,
                                                     const size_t num_of_unassociated_dets,
                                                     const Cluster *const current_cluster_ptr,
                                                     const std::bitset<SG_MAX_NUM_CONTOURS> &contours_of_cluster_mask,
                                                     const Contour_Initialization_Calibrations_T &calibrations)
   {
      bool f_check_detections_look_diversity = true;

      if (num_of_unassociated_dets > calibrations.min_num_dets_for_stationary_hypothesis)
      {
         f_check_detections_look_diversity = false;
      }
      else if (contours_of_cluster_mask.count() == 0U)
      {
         float min_pos_x                = std::numeric_limits<float>::max();
         float max_pos_x                = std::numeric_limits<float>::min();
         std::size_t dets_mask_idx      = 0U;
         const auto current_cluster_end = current_cluster_ptr->end();
         for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
         {
            if (det_it->contour_id == INVALID_CONTOUR_ID)
            {
               min_pos_x = std::min(det_it->position.x, min_pos_x);
               max_pos_x = std::max(det_it->position.x, max_pos_x);
            }
            dets_mask_idx++;
         }
         (void) dets_mask_idx; // MISRA

         if ((max_pos_x - min_pos_x) > calibrations.min_dets_span_for_stationary_hypothesis)
         {
            f_check_detections_look_diversity = false;
         }
      }
      else if (contours_of_cluster_mask.count() > 0U)
      {
         std::size_t contour_mask_idx       = 0U;
         uint16_t cumulated_num_of_vertices = 0U;
         for (auto &current_contour : contours)
         {
            if (contours_of_cluster_mask[contour_mask_idx])
            {
               cumulated_num_of_vertices += current_contour.size();
            }
            ++contour_mask_idx;

            (void) current_contour; // MISRA
         }
         (void) contour_mask_idx;          // MISRA
         (void) cumulated_num_of_vertices; // MSIRA

         if (cumulated_num_of_vertices > calibrations.min_num_vertices_for_stationary_hypothesis)
         {
            f_check_detections_look_diversity = false;
         }
      }
      else
      {
         // MISRA
      }

      const bool f_aliased_moving =
         f_check_detections_look_diversity
            ? (!evaluate_detections_look_diversity(current_cluster_ptr, calibrations.sensor_look_diversity_threshold))
            : false;

      return f_aliased_moving;
   }

   bool evaluate_detections_look_diversity(const Cluster *const current_cluster_ptr, const float look_threshold)
   {
      std::array<uint16_t, 4U> cnt_unique{};

      const auto current_cluster_end = current_cluster_ptr->end();
      for (auto det_it = current_cluster_ptr->begin(); det_it != current_cluster_end; ++det_it)
      {
         if ((0 <= det_it->look_id) && (det_it->contour_id == INVALID_CONTOUR_ID))
         {
            cnt_unique[static_cast<uint8_t>(det_it->look_id)]++; // TODO https://jiraprod.aptiv.com/browse/FZD-1666
         }
      }

      bool f_diverse = false;

      const auto cnt_look_ids_1_and_3 = cnt_unique[1U] + cnt_unique[3U];
      const auto cnt_look_ids_0_and_2 = cnt_unique[0U] + cnt_unique[2U];

      if ((cnt_look_ids_1_and_3 == 0U) || (cnt_look_ids_0_and_2 == 0))
      {
         f_diverse = false;
      }
      else
      {
         const float log_ratio =
            std::abs(std::log(static_cast<float>(cnt_look_ids_0_and_2) / static_cast<float>(cnt_look_ids_1_and_3)));

         if (log_ratio <= look_threshold)
         {
            f_diverse = true;
         }
      }

      (void) cnt_look_ids_0_and_2; // MISRA

      return f_diverse;
   }

   inline static void cut_off_vertices(Contour_T::VertexList &vertices,
                                       sg::Contour_T::VertexList::iterator begin_it,
                                       const sg::Contour_T::VertexList::iterator end_it)
   {
      for (; begin_it != end_it; ++begin_it)
      {
         (void) vertices.erase(begin_it);
      }
   }

   void cut_off_far_vertices(Contour_T::VertexList &vertices, const uint16_t max_num_vertices, const geometry::Point2D_T host_position)
   {
      const uint16_t excess_vertices_number = static_cast<uint16_t>(vertices.size()) - max_num_vertices;
      const float distance_begin            = calc_subcontour_to_host_distance(vertices, max_num_vertices, 0U, host_position);
      const float distance_end = calc_subcontour_to_host_distance(vertices, max_num_vertices, excess_vertices_number, host_position);

      if (distance_begin < distance_end)
      {
         static_assert(std::numeric_limits<decltype(max_num_vertices)>::max() <= std::numeric_limits<std::ptrdiff_t>::max(),
                       "Possible maximum value is too high.");
         const auto begin_it = std::next(vertices.begin(), static_cast<std::ptrdiff_t>(max_num_vertices));
         const auto end_it   = vertices.end();
         cut_off_vertices(vertices, begin_it, end_it);
      }
      else
      {
         static_assert(std::numeric_limits<decltype(excess_vertices_number)>::max() <= std::numeric_limits<std::ptrdiff_t>::max(),
                       "Possible maximum value is too high.");
         const auto begin_it = vertices.begin();
         const auto end_it   = std::next(vertices.begin(), static_cast<std::ptrdiff_t>(excess_vertices_number));
         cut_off_vertices(vertices, begin_it, end_it);
      }
   }

   float calc_subcontour_to_host_distance(const Contour_T::VertexList &vertices,
                                          const uint16_t max_num_vertices,
                                          const uint16_t start_idx,
                                          const geometry::Point2D_T host_position)
   {
      float vertex_distances[SG_MAX_NUM_VERTICES_PER_CONTOUR]{};

      uint16_t stop_idx = start_idx + max_num_vertices;

      if (stop_idx > vertices.size())
      {
         stop_idx = static_cast<uint16_t>(vertices.size());
      }

      static_assert(std::numeric_limits<decltype(start_idx)>::max() <= std::numeric_limits<std::ptrdiff_t>::max(), "Possible "
                                                                                                                   "maximum value "
                                                                                                                   "is to high.");
      static_assert(std::numeric_limits<decltype(stop_idx)>::max() <= std::numeric_limits<std::ptrdiff_t>::max(), "Possible "
                                                                                                                  "maximum value "
                                                                                                                  "is to high.");

      auto start_it      = std::next(vertices.begin(), static_cast<std::ptrdiff_t>(start_idx));
      const auto stop_it = std::next(vertices.begin(), static_cast<std::ptrdiff_t>(stop_idx));

      uint16_t vertex_idx{};
      for (; start_it != stop_it; ++start_it)
      {
         if (vertex_idx < SG_MAX_NUM_VERTICES_PER_CONTOUR) // MISRA ("Guarantee that container indices and iterators are within the
                                                           // valid range.")
         {
            vertex_distances[vertex_idx] =
               calculate_point_distance(start_it->position.x - host_position.x, start_it->position.y - host_position.y);
            ++vertex_idx;
         }
         else
         {
            break;
         }
      }

      return *std::min_element(&vertex_distances[0U], &vertex_distances[vertex_idx]);
   }

   void assign_segment_ids(const Contour_T::VertexList &vertices, IdHandlerIncremental<uint32_t> &segment_id_handler)
   {
      const auto end_it = std::prev(vertices.end());

      for (auto vertex_it = vertices.begin(); vertex_it != end_it; ++vertex_it)
      {
         vertex_it->segment_id = segment_id_handler.get_id();
      }
   }

   void add_new_neighbors_to_current_neighbors(const NewNeighborIterators &new_neighbors, NeighborIterators &current_neighbors)
   {
      for (auto &new_neighbor_it : new_neighbors)
      {
         if (current_neighbors.full())
         {
            break;
         }
         // add only new_neighbors which are not already present in curr_neighbors
         const bool f_is_in_curr_neighbor = std::any_of(current_neighbors.begin(), current_neighbors.end(),
                                                        [&new_neighbor_it](const auto &curr_neighbor_intern_it)
                                                        { return new_neighbor_it->unique_id == curr_neighbor_intern_it->unique_id; });

         if (!f_is_in_curr_neighbor)
         {
            (void) current_neighbors.push_back(new_neighbor_it);
            // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
         }
         (void) new_neighbor_it; // MISRA
      }
   }
}
