/*=============================================================================================*\
* FILE: sg_initialize_contours_helpers.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for initilize_contours helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_INITIALIZE_CONTOURS_HELPERS_H
#define SG_INITIALIZE_CONTOURS_HELPERS_H

#include <array>
#include <bitset>

#include "sg_calibrations.h"
#include "sg_cluster_detections_helpers.h"
#include "sg_contour.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"
#include "sg_init_aliases.h"

namespace sg
{
   /**
    * @brief   Returns mask indicating which detections should be used for creation of a new contour.
    *
    * @param[in]        contours
    * @param[in]        current_cluster_ptr
    * @param[in]        calibrations
    * @param[in]        azimuth_epsilon
    *
    * @return  true if any detection f_subset was marked
    **/
   bool collect_dets_for_a_new_contour(const ContourStorage &contours,
                                       const Cluster *const current_cluster_ptr,
                                       const Contour_Initialization_Calibrations_T &calibrations,
                                       const float azimuth_epsilon);

   /**
    * @brief   Adjust result of dbscan by removing unclustered dets.
    *
    * @param[in, out]   current_cluster_ptr
    *
    * @return  N/A
    **/
   void adjust_results_of_dbscan(const Cluster *const current_cluster_ptr);

   /**
    * @brief   Modifies contours_mask to indicate contours, that are part of current cluster.
    *
    * @param[out]       contours_mask
    * @param[in]        contours
    * @param[in]        current_cluster_id
    *
    * @return  N/A
    **/
   void get_contours_of_cluster_mask(std::bitset<SG_MAX_NUM_CONTOURS> &contours_mask,
                                     const ContourStorage &contours,
                                     const uint16_t current_cluster_id);

   /**
    * @brief   Checks if contour creation is possible. Selects candidates for contour creation
    *          by modifying subset_of_dets_mask.
    *
    * @param[in, out]   current_cluster_ptr
    * @param[in]        contours
    * @param[in]        contours_of_cluster_mask
    * @param[in]        calibrations
    * @param[in]        azimuth_epsilon
    *
    * @return  flag if contour creation is possible
    **/
   bool check_if_contour_creation_is_possible(const Cluster *const current_cluster_ptr,
                                              const ContourStorage &contours,
                                              const std::bitset<SG_MAX_NUM_CONTOURS> &contours_of_cluster_mask,
                                              const Contour_Initialization_Calibrations_T &calibrations,
                                              const float azimuth_epsilon);

   /**
    * @brief   Checks if detections belong to moving objects. Use evaluate
    *          detctions look diversity
    *
    * @param [in]          contours
    * @param [in]          num_of_unassociated_dets
    * @param [in]          current_cluster_ptr
    * @param [in]          contours_of_cluster_mask
    * @param [in]          calibrations
    *
    * @return  flag if detections do not belong to moving object
    **/
   bool check_if_detections_belong_to_moving_objects(const ContourStorage &contours,
                                                     const size_t num_of_unassociated_dets,
                                                     const Cluster *const current_cluster_ptr,
                                                     const std::bitset<SG_MAX_NUM_CONTOURS> &contours_of_cluster_mask,
                                                     const Contour_Initialization_Calibrations_T &calibrations);

   /**
    * @brief   Returns true if the detections comes equally from all sensor looks
    *
    * @param [in]         current_cluster_ptr
    * @param [in]         look_threshold
    *
    * @return  flag is true if detection look diversity is valid
    **/
   bool evaluate_detections_look_diversity(const Cluster *const current_cluster_ptr, const float look_threshold);

   /**
    * @brief   Finds and returns most frequent/common value (mode) inside std::array.
    *          If there isn't any value other than 0U inside array, 0U is returned.
    *
    * @param [in]         arr
    *
    * @return  most frequent element of the provided array
    **/
   template <typename T, std::size_t N>
   T find_most_frequent_nonzero_value(const std::array<T, N> &arr)
   {
      static_assert(N <= std::numeric_limits<std::size_t>::max(), "std::array size must be lower than maximum value of "
                                                                  "std::size_t.");
      std::array<T, N> sorted_array = arr;
      std::sort(sorted_array.begin(), sorted_array.end());

      T most_frequent_value     = 0U;
      std::size_t current_count = 0U;
      std::size_t max_count     = 0U;

      for (std::size_t i = 0U; i < N; ++i)
      {
         if (sorted_array[i] == 0U)
         {
            continue;
         }

         if ((i > 0U) && (sorted_array[i] == sorted_array[i - 1U]))
         {
            current_count++;
         }
         else
         {
            current_count = 1U;
         }

         if (current_count > max_count)
         {
            max_count           = current_count;
            most_frequent_value = sorted_array[i];
         }
      }
      (void) current_count; // MISRA
      (void) max_count;     // MISRA

      return most_frequent_value;
   }

   /**
    * @brief   Limits number of vertices to max value provided as input parameter.
    *
    * @param   vertices
    * @param   max_num_vertices
    * @param   host_position
    *
    * @return  N/A
    **/
   void cut_off_far_vertices(Contour_T::VertexList &vertices, const uint16_t max_num_vertices, const geometry::Point2D_T host_position);

   /**
    * @brief   Calculates minimal distance of subcontour (starting from given index) to host.
    *
    * @param   vertices
    * @param   max_num_vertices
    * @param   start_idx
    * @param   host_position
    *
    * @return  host to subcontour distance
    **/
   float calc_subcontour_to_host_distance(const Contour_T::VertexList &vertices,
                                          const uint16_t max_num_vertices,
                                          const uint16_t start_idx,
                                          const geometry::Point2D_T host_position);

   /**
    * @brief   Assigns segment_id to each vertex's (except the last) segment_id.
    *
    * @param[in, out]   vertices
    * @param[in, out]   segment_id_handler
    *
    * @return  N/A
    **/
   void assign_segment_ids(const Contour_T::VertexList &vertices, IdHandlerIncremental<uint32_t> &segment_id_handler);

   /**
    * @brief    determines the detections within a radius around the current detection
    *
    * @tparam            size
    *
    * @param[in, out]    neighbors
    * @param[in]         current_cluster_ptr
    * @param[in]         current_detection
    * @param[in]         cluster_radius
    *
    * @return            number of neighbors detected in circular region
    **/
   template <size_t size>
   uint8_t get_dets_in_circular_region(EmbeddedList<DetectionList::iterator, size> &neighbors,
                                       const Cluster *const current_cluster_ptr,
                                       const Detection_T &current_detection,
                                       const float cluster_radius)
   {
      static constexpr bool f_use_squeezed_position = false;
      uint8_t num_neighbors                         = 0U;
      const geometry::Point2D_T circle_center       = get_2D_det_position(current_detection, f_use_squeezed_position);
      const geometry::Circle_T circular_region{circle_center, cluster_radius};

      const auto dets_end = current_cluster_ptr->end();
      for (auto neighbor_candidate_detection_it = current_cluster_ptr->begin(); neighbor_candidate_detection_it != dets_end;
           neighbor_candidate_detection_it++)
      {
         if (neighbor_candidate_detection_it->f_subset)
         {
            // If the current detection is a new detection, count neighbors from within all detections,
            // otherwise count only from within the new ones as the old ones have already cumulated their neighbors.
            // This speeds up runtime.
            if (neighbor_candidate_detection_it->f_new || current_detection.f_new)
            {
               const geometry::Point2D_T candidate_point_position =
                  get_2D_det_position(*neighbor_candidate_detection_it, f_use_squeezed_position);

               if ((num_neighbors < SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS)
                   && geometry::is_inside(circular_region, candidate_point_position))
               {
                  const auto result = neighbors.push_back(neighbor_candidate_detection_it);
                  (void) result; // MISRA: result of operation can only be checked in Debug mode
                  assert(result != nullptr);
                  // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
                  num_neighbors++;
               }

               (void) candidate_point_position; // MISRA
            }
         }
      }
      return num_neighbors;
   }

   /**
    * @brief    Move new_neighbors to current neigbors if they are not already there
    *
    * @param[in]           new_neighbors
    * @param[in, out]      current_neighbors
    *
    * @return              N/A
    **/
   void add_new_neighbors_to_current_neighbors(const NewNeighborIterators &new_neighbors, NeighborIterators &current_neighbors);

}
#endif
