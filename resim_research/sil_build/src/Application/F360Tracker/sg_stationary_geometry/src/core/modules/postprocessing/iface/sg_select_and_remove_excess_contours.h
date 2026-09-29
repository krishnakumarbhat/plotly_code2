/*=============================================================================================*\
* FILE: sg_select_and_remove_excess_contours.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration for select_and_remove_excess_contours function
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_SELECT_AND_REMOVE_EXCESS_CONTOURS_H
#define SG_SELECT_AND_REMOVE_EXCESS_CONTOURS_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_host_props.h"


namespace sg
{
   /**
    * @brief          Function to remove contours that are further away from the host than
    *                 others and exceed the limit of the permissible amount of clusters that
    *                 are contoured.
    *
    *                 At the end of a cycle, the function "select_and_remove_excess_contours"
    *                 is invoked to remove cluster contours that are beyond the excess limit.
    *                 This frees up space for new contours to be initialized in the next cycle,
    *                 e.g. when clusters have been moved closer to the host.
    *                 The removal is based on the contour to host distance. The contours are
    *                 sorted w.r.t. their host distance and the further ones that also exceed
    *                 the set limit are removed.
    *
    * @param[in, out] contours
    * @param[in]      host_properties
    * @param[in]      contour_postprocessing_calibrations
    * @param[in]      common_calibrations
    **/
   void select_and_remove_excess_contours(
      ContourStorage &contours,
      const HostProps &host_properties,
      const Contour_Postprocessing_Calibrations_T::Select_And_Remove_Excess_Contours_T &contour_postprocessing_calibrations,
      const Common_Calibrations_T &common_calibrations);

   /**
    * @brief          A function to calculate the distance to the contoured cluster. It returns the
    *                 distance from closest contour in the cluster and its cluster id as pairs array.
    *
    * @param[in]      contours
    * @param[in]      contoured_cluster_ids
    * @param[in]      num_clusters
    * @param[in]      host_position
    * @param[in]      max_view_range
    * @param[out]     cluster_distances
    **/
   std::array<std::pair<uint16_t, float>, SG_MAX_NUM_CONTOURS>
   get_cluster_to_host_distances(const ContourStorage &contours,
                                 const std::array<uint16_t, SG_MAX_NUM_CONTOURS> &contoured_cluster_ids,
                                 const uint16_t num_clusters,
                                 const geometry::Point2D_T &host_position,
                                 const float max_view_range);

   /**
    * @brief          Sorts cluster distances array ascending by distance.
    *
    * @param[in,out]  cluster_distances
    * @param[in]      num_clusters
    **/
   void sort_by_distance(std::array<std::pair<uint16_t, float>, SG_MAX_NUM_CONTOURS> &cluster_distances, const uint16_t num_clusters);

   /**
    * @brief          Removes all contours on excessed contoured cluster.
    *
    * @param[in,out]  contours
    * @param[in]      cluster_distances
    * @param[in]      num_clusters
    * @param[in]      max_num_contoured_clusters
    **/
   void remove_excess_contours(ContourStorage &contours,
                               const std::array<std::pair<uint16_t, float>, SG_MAX_NUM_CONTOURS> &cluster_distances,
                               const uint16_t num_clusters,
                               const uint16_t max_num_contoured_clusters);

   /**
   * @brief          A function to calculate the minimal distance between the host and a given
                     contour. First the closest vertex is determined. Then the footpoint
                     distance to the adjacent two segments (if it is a middle vertex) is
                     calculated. Of these two footpoints, the closest is selected and output
                     together with the host to footpoint distance.
   *
   * @param[in]      contour
   * @param[in]      host_position
   * @param[out]     distance
   **/
   float get_contour_to_host_distance(const Contour_T &contour, const geometry::Point2D_T &host_position);
}
#endif
