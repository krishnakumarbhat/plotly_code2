/*=============================================================================================*\
* FILE: sg_initialize_contours.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for initialize_contours.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_initialize_contours.h"

#include <utility>

#include "sg_sort_clusters_by_distance.h"
#include "sg_try_initialize_contour.h"

namespace sg
{
   void initialize_contours(const Contour_Initialization_Calibrations_T &contour_initialization_calibrations,
                            const Common_Calibrations_T &common_calibrations,
                            DetectionStorage &detections,
                            ContourStorage &contours)
   {
      const geometry::Point2D_T host_position{0.0F, 0.0F};
      std::array<Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> cluster_ptrs_sorted_by_distance{};
      cluster_ptrs_sorted_by_distance.fill(nullptr);

      const auto num_clusters     = sort_clusters_by_distance(detections, host_position, cluster_ptrs_sorted_by_distance);
      bool f_break_initialization = contours.size() == contours.capacity();

      for (std::size_t idx = 0U; idx < num_clusters; idx++)
      {
         if (f_break_initialization)
         {
            break;
         }

         const auto current_cluster_ptr = cluster_ptrs_sorted_by_distance[idx];
         auto result = try_initialize_contour(contours, current_cluster_ptr, contour_initialization_calibrations, common_calibrations);

         if (result.first == InitializationResult::CREATED)
         {
            (void) contours.push_back(std::move(result.second)); // FZD-822: Handle situation when new contour couldn't be added
         }

         f_break_initialization = (result.first == InitializationResult::OUT_OF_MAX_NUM_VERTICES)
                                  || (contours.size() == contours.capacity());
      }
      (void) f_break_initialization; // MISRA
   }
}
