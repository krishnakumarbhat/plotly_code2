/*=============================================================================================*\
* FILE: sg_declutter_contours_helpers.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the declaration of declutter_contours helpers function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_DECLUTTER_CONTOURS_HELPERS_H
#define SG_DECLUTTER_CONTOURS_HELPERS_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"

namespace sg
{
   static constexpr uint16_t MAX_NUM_CLUSTERS_FOR_DECLUTTER =
      std::min(static_cast<uint16_t>(SG_MAX_NUM_CONTOURS / 2U), SG_MAX_NUM_INTERNAL_CLUSTERS);

   /**
    * @brief            Structure for contour properties.
    *
    **/
   struct ContourProps
   {
      uint16_t vertices_number;
      std::array<float, SG_MAX_NUM_VERTICES_PER_CONTOUR> vertices_azimuth;
      std::array<float, SG_MAX_NUM_VERTICES_PER_CONTOUR> vertices_range;
   };
   /**
    * @brief            Function selects contours only from clusters that have more than one contour.
    *
    * @param[in/out]    contours_for_decluttering
    * @param[in/out]    num_contours_in_cluster
    * @param[in]        contours
    *
    **/
   void select_contours_for_decluttering(std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                                         std::array<uint16_t, MAX_NUM_CLUSTERS_FOR_DECLUTTER> &num_contours_in_cluster,
                                         const ContourStorage &contours);

   /**
    * @brief            Function calculates azimuth and range of input vertices.
    *
    * @param[in]        vertices
    * @param[in/out]    vertices_azimuth
    * @param[in/out]    vertices_range
    *
    **/
   void calc_vertices_azimuth_and_range(const Contour_T::VertexList &vertices,
                                        std::array<float, SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_azimuth,
                                        std::array<float, SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_range);

   /**
    * @brief            Function finds number of occluders for each contour from contours_for_decluttering.
    *
    * @param[in/out]    num_of_occluders
    * @param[in]        contours_for_decluttering
    * @param[in]        num_contours_in_cluster
    * @param[in]        calibrations
    * @param[in]        azimuth_epsilon
    *
    **/
   void mark_occluded_contours(std::array<uint16_t, SG_MAX_NUM_CONTOURS> &num_of_occluders,
                               const std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                               const std::array<uint16_t, MAX_NUM_CLUSTERS_FOR_DECLUTTER> &num_contours_in_clusters,
                               const Contour_Postprocessing_Calibrations_T::Declutter_Contours_T &calibrations,
                               const float azimuth_epsilon);

   /**
    * @brief            Based on number of occluders function removes contours from ContoursStorage.
    *
    * @param[in/out]    contours
    * @param[in]        contours_for_decluttering
    * @param[in]        num_of_occluders
    * @param[in]        occluders_num_threshold
    *
    **/
   void remove_occluded_contours(ContourStorage &contours,
                                 const std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                                 const std::array<uint16_t, SG_MAX_NUM_CONTOURS> &num_of_occluders,
                                 const uint8_t occluders_num_threshold);

   /**
    * @brief            Function calculates azimuth and range of vertices of contours selectet for decluttering.
    *
    * @param[in/out]    contours_props
    * @param[in]        contours_for_decluttering
    * @param[in]        num_contours_in_clusters
    *
    **/
   void calc_contours_properties(std::array<ContourProps, SG_MAX_NUM_CONTOURS> &contours_props,
                                 const std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                                 const std::array<uint16_t, MAX_NUM_CLUSTERS_FOR_DECLUTTER> &num_contours_in_clusters);

   /**
    * @brief            Function checks if occlusion relation occuours between two contours.
    *
    * @param[in]        tested_contour
    * @param[in]        potential_occluder
    * @param[in]        calibrations
    * @param[in]        azimuth_epsilon
    *
    * @return           occlusion result
    **/
   bool occlusion_assessment(const ContourProps &tested_contour,
                             const ContourProps &potential_occluder,
                             const Contour_Postprocessing_Calibrations_T::Declutter_Contours_T &calibrations,
                             const float azimuth_epsilon);
}
#endif
