/*=============================================================================================*\
* FILE: sg_cluster_detections_helpers.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for helper functions used in detection clustering.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_CLUSTER_DETECTIONS_HELPERS_H
#define SG_CLUSTER_DETECTIONS_HELPERS_H

#include <bitset>
#include <tuple>

#include "geometry/geo_is_inside.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief    selects either the squeezed or unsqueezed 2D position (x & y, without z) from the given detection
    *
    * @param[in]      detection
    * @param[in]      f_use_squeezed_position
    *
    * @return         2D point position in selected coordinates
    **/
   inline geometry::Point2D_T get_2D_det_position(const Detection_T &detection, const bool f_use_squeezed_position)
   {
      return f_use_squeezed_position ? geometry::Point2D_T(detection.position_squeezed.x, detection.position_squeezed.y)
                                     : geometry::Point2D_T(detection.position.x, detection.position.y);
   }

   /**
    * @brief     returns min and max x position given by current detection and cluster radius
    *
    * @param[in]     current_det
    * @param[in]     cluster_radius
    *
    * @return        min and max value
    **/

   std::tuple<float, float> get_relevant_x_position_interval(const Detection_T &current_det, const float cluster_radius);

   /**
    * @brief    determines the detections within a radius around the current detection
    *
    * @tparam            size
    *
    * @param[in, out]    neighbors
    * @param[in]         detections
    * @param[in]         current_detection
    * @param[in]         cluster_radius
    *
    * @return            number of neighbors detected in circular region
    **/
   template <size_t size>
   uint8_t get_dets_in_circular_region(EmbeddedList<DetectionCache::collection_data_type::const_iterator, size> &neighbors,
                                       const DetectionStorage &detections,
                                       const Detection_T &current_detection,
                                       const float cluster_radius)
   {
      uint8_t num_neighbors                   = 0U;
      const geometry::Point2D_T circle_center = current_detection.position_squeezed;
      const geometry::Circle_T circular_region{circle_center, cluster_radius};

      float min_det_pos_x{};
      float max_det_pos_x{};
      std::tie(min_det_pos_x, max_det_pos_x) = get_relevant_x_position_interval(current_detection, cluster_radius);
      auto start_idx                         = detections.begin(min_det_pos_x);
      const auto end_idx                     = detections.end(max_det_pos_x);

      // Loop over sorted detections with given range
      for (; start_idx != end_idx; start_idx++)
      {
         const auto &candidate_det = **start_idx;
         if (candidate_det.f_subset)
         {
            // If the current detection is a new detection, count neighbors from within all detections,
            // otherwise count only from within the new ones as the old ones have already cumulated their neighbors.
            // This speeds up runtime.
            if (candidate_det.f_new || current_detection.f_new)
            {
               const geometry::Point2D_T candidate_point_position = candidate_det.position_squeezed;

               if ((num_neighbors < SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS)
                   && geometry::is_inside(circular_region, candidate_point_position))
               {
                  const auto result = neighbors.push_back(start_idx);
                  (void) result; // MISRA: result of operation can only be checked in Debug mode
                  assert(result != nullptr);
                  // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
                  num_neighbors++;
               }
               (void) candidate_point_position;
            }
         }
      }

      return num_neighbors;
   }

   /**
    * @brief          Helper function for creating a mask for detections of a certain drivability class
    *
    * @param[in, out]   detections
    * @param[in]        drivability_class
    *
    * @return         true if any detection has current drivability class
    **/
   bool mark_detections_with_drivability_class(const DetectionStorage &detections, const SG_Drivability_Class_T &drivability_class);
}
#endif
