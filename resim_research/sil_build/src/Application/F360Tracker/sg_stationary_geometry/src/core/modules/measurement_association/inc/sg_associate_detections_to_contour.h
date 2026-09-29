/*=============================================================================================*\
* FILE: sg_associate_detections_to_contour.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the functions provided by the algorithm associate detections to contour.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_ASSOCIATE_DETECTIONS_TO_CONTOUR_H
#define SG_ASSOCIATE_DETECTIONS_TO_CONTOUR_H

#include "sg_calibrations.h"
#include "sg_constants.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"
#include "sg_measurement_association_utils.h"

namespace sg
{
   /**
    * @brief              Associates detections to contour.
    *
    * @param[in,out]      contours
    * @param[in,out]      detections
    * @param[in]          measurement_association_calibrations
    * @param[in]          common_calibrations
    **/
   void associate_detections_to_contour(const ContourStorage &contours,
                                        const DetectionStorage &detections,
                                        const Measurement_Association_Calibrations_T &measurement_association_calibrations,
                                        const Common_Calibrations_T &common_calibrations);

   /**
    * @brief    Set the most frequent cluster id of associated detections to contour.
    *
    * @param[in,out]    contour
    * @param[in]        detections
    * @param[in]        min_dets_assoc_update_cluster_id
    *
    **/
   static inline void set_dominant_cluster_id(Contour_T &contour,
                                              const DetectionStorage &detections,
                                              const uint16_t min_dets_assoc_update_cluster_id)
   {
      const std::pair<uint16_t, uint16_t> most_freq_cluster_id = get_most_frequent_cluster_id(detections, contour.unique_id());

      if (min_dets_assoc_update_cluster_id <= most_freq_cluster_id.first)
      {
         contour.cluster_id = most_freq_cluster_id.second;
      }
      else
      {
         contour.cluster_id = INVALID_CLUSTER_ID;
      }
   }

}

#endif
