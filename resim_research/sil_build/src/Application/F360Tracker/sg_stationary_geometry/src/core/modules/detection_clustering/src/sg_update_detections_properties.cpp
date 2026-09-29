/*=============================================================================================*\
* FILE: sg_update_detections_properties.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential  Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for update_detections_properties and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_update_detections_properties.h"

namespace sg
{
   void update_detections_properties(DetectionStorage &detections, const Existence_Probability_Calibrations_T &calibrations)
   {
      std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> detection_random_access_its;
      std::fill(detection_random_access_its.begin(), detection_random_access_its.end(), DetectionList::iterator());
      std::array<ClusterList::iterator, SG_MAX_NUM_INTERNAL_DETS> destination_cluster_its;
      std::fill(destination_cluster_its.begin(), destination_cluster_its.end(), ClusterList::iterator());

      uint16_t idx               = 0U;
      const auto detecion_end_it = detections.end();
      for (auto detection_it = detections.begin(); detection_it != detecion_end_it; ++detection_it)
      {
         if (detection_it->f_subset)
         {
            update_existence_probability(*detection_it, calibrations);
            detection_random_access_its[idx] = detection_it;
            determine_destination_cluster(destination_cluster_its, detections, idx, detection_it->temp_cluster_id);
         }
         idx++;
      }
      assert(idx <= SG_MAX_NUM_INTERNAL_DETS);
      (void) idx; // MISRA

      update_cluster_assignment(detections, detection_random_access_its, destination_cluster_its);
   }

   void determine_destination_cluster(std::array<ClusterList::iterator, SG_MAX_NUM_INTERNAL_DETS> &destination_cluster_its,
                                      DetectionStorage &detections,
                                      const uint16_t idx,
                                      const uint16_t requested_cluster_id)
   {
      if (destination_cluster_its[idx] == nullptr)
      {
         const auto destination_cluster_it = detections.find_cluster_or_create_new(requested_cluster_id);

         // Assign the same destination cluster for each detection with the same requested cluster id
         uint16_t i = 0U;
         for (const auto &det : detections)
         {
            if (det.temp_cluster_id == requested_cluster_id)
            {
               destination_cluster_its[i] = destination_cluster_it;
            }
            ++i;
         }
         (void) i; // MISRA
      }
   }

   void update_cluster_assignment(DetectionStorage &det_storage,
                                  const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_its,
                                  const std::array<ClusterList::iterator, SG_MAX_NUM_INTERNAL_DETS> &destination_cluster_its)
   {
      const auto total_number_of_detections = det_storage.size();

      for (size_t idx = 0U; idx < total_number_of_detections; idx++)
      {
         if (detection_its[idx] != nullptr)
         {
            det_storage.move_detection(destination_cluster_its[idx], detection_its[idx]);
         }
      }
      det_storage.remove_empty_clusters();
   }
}
