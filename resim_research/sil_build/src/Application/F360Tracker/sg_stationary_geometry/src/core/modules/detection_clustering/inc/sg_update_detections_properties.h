/*=============================================================================================*\
* FILE: sg_update_detections_properties.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for update_detections_properties and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_UPDATE_DETECTIONS_PROPERTIES_H
#define SG_UPDATE_DETECTIONS_PROPERTIES_H

#include <bitset>

#include "sg_calibrations.h"
#include "sg_detection_storage.h"
#include "sg_math.h"

namespace sg
{
   /**
    * @brief    Updates detection properties like cluster_id, num_neighbors and existence probability.
    *
    * @param[in, out]    detections - detections being updated
    * @param[in]         calibrations - calibration parameters
    *
    * @return         N/A
    **/
   void update_detections_properties(DetectionStorage &detections, const Existence_Probability_Calibrations_T &calibrations);

   /**
    * @brief    Determine destination cluster based on requested cluster id.
    *           Updated detection is not guaranteed to have the same cluster_id as requested. The function
    *           only ensures that detections with the same requested cluster_id are in the same cluster
    *
    * @param[in, out]    destination_cluster_its - list of iterators to destination clusters for each detection
    * @param[in, out]    detections - detections storage
    * @param[in]         idx - index of current detection
    * @param[in]         requested_cluster_id - requested new cluster_id
    *
    * @return         N/A
    **/
   void determine_destination_cluster(std::array<ClusterList::iterator, SG_MAX_NUM_INTERNAL_DETS> &destination_cluster_its,
                                      DetectionStorage &detections,
                                      const uint16_t idx,
                                      const uint16_t requested_cluster_id);

   /**
    * @brief    Move detections between clusters based on new assignment
    *
    * @param[in, out]    det_storage - DetectionStorage container
    * @param[in]         detection_its - iterator for all detections across clusters
    * @param[in]         destination_cluster_its - iterators to destination clusters for every detection
    *
    * @return         N/A
    **/
   void update_cluster_assignment(DetectionStorage &det_storage,
                                  const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_its,
                                  const std::array<ClusterList::iterator, SG_MAX_NUM_INTERNAL_DETS> &destination_cluster_its);

   /**
    * @brief    Update detection existence probability based on number of historic neighbors and age.
    *
    * @param[in, out]    detection - detection to be updated
    * @param[in]         calibrations - calibration parameters
    *
    * @return         N/A
    **/
   inline void update_existence_probability(Detection_T &detection, const Existence_Probability_Calibrations_T &calibrations)
   {
      const float weighted_sum = (calibrations.neighbors_factor * detection.cumulated_num_neighbors
                                  + calibrations.age_factor * static_cast<float>(detection.age));

      detection.existence_probability = logistic_function_value(weighted_sum, calibrations.alpha);
   }
}
#endif
