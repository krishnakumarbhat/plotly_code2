/*=============================================================================================*\
* FILE: sg_get_unoccluded_dets.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for get_unoccluded_dets and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_GET_UNOCCLUDED_DETS_H
#define SG_GET_UNOCCLUDED_DETS_H

#include <bitset>

#include "sg_calibrations.h"
#include "sg_constants.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief   Returns information about detections not occluded by contours.
    *
    * @param[in, out]   current_cluster_ptr
    * @param[in]        contours
    * @param[in]        calibrations
    * @param[in]        contours_mask
    * @param[in]        azimuth_epsilon
    *
    * @return  number of unoccluded dets
    **/
   std::size_t get_unoccluded_dets(const Cluster *const current_cluster_ptr,
                                   const ContourStorage &contours,
                                   const Contour_Initialization_Calibrations_T &calibrations,
                                   const std::bitset<SG_MAX_NUM_CONTOURS> &contours_mask,
                                   const float azimuth_epsilon);

   /**
    * @brief   Calculates azimuth and range of detections regarding to provided mask.
    *
    * @param[in, out]   dets_azimuth
    * @param[in, out]   dets_range
    * @param[in]        current_cluster_ptr
    *
    * @return  N/A
    **/
   void calc_detections_azimuth_and_range(float (&dets_azimuth)[SG_MAX_NUM_INTERNAL_DETS],
                                          float (&dets_range)[SG_MAX_NUM_INTERNAL_DETS],
                                          const Cluster *const current_cluster_ptr);

   /**
    * @brief   Calculates azimuth and range of vertices..
    *
    * @param[in, out]   vertices
    * @param[in]        vertices_azimuth array
    * @param[in]        vertices_range array
    *
    * @return  N/A
    **/
   void calc_vertices_azimuth_and_range(const Contour_T::VertexList &vertices,
                                        float (&vertices_azimuth)[SG_MAX_NUM_VERTICES],
                                        float (&vertices_range)[SG_MAX_NUM_VERTICES]);

   /**
    * @brief   Returns mask of detections that have azimuth in provided limits.
    *
    * @param[in, out]   dets_azimuth_within_limits_mask
    * @param[in]        current_cluster_ptr
    * @param[in]        dets_azimuth array
    * @param[in]        azimuth_limits
    * @param[in]        azimuth_margin_threshold
    *
    * @return  N/A
    **/
   void dets_azimuth_within_limit(std::bitset<SG_MAX_NUM_INTERNAL_DETS> &dets_azimuth_within_limits_mask,
                                  const Cluster *const current_cluster_ptr,
                                  const float (&dets_azimuth)[SG_MAX_NUM_INTERNAL_DETS],
                                  const std::pair<float, float> azimuth_limits,
                                  const float azimuth_margin_threshold);

   /**
    * @brief   Returns mask of detections that have range in provided limit.
    *
    * @param[in, out]   current_cluster_ptr
    * @param[in, out]   dets_range_within_limits_mask
    * @param[in]        dets_range array
    * @param[in]        range_limit
    * @param[in]        range_margin
    *
    * @return  N/A
    **/
   void dets_range_within_limit(const Cluster *const current_cluster_ptr,
                                std::bitset<SG_MAX_NUM_INTERNAL_DETS> &dets_range_within_limits_mask,
                                const float (&dets_range)[SG_MAX_NUM_INTERNAL_DETS],
                                const float range_limit,
                                const float range_margin);

}
#endif
