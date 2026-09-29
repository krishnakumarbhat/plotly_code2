/*=============================================================================================*\
* FILE: sg_merge_contours_helpers.h
* ====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the declaration of merge_contours helpers function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_MERGE_CONTOURS_HELPERS_H
#define SG_MERGE_CONTOURS_HELPERS_H

#include "geometry/geo_distance.h"
#include "geometry/geo_length.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"

namespace sg
{
   /**
    * @brief            Function returns all squared distances between two contours to be merged
    *
    * @param[in, out]   current_begin_to_partner_begin_dist_sq
    * @param[in, out]   current_begin_to_partner_end_dist_sq
    * @param[in, out]   current_end_to_partner_begin_dist_sq
    * @param[in, out]   current_end_to_partner_end_dist_sq
    * @param[in]        current_contour
    * @param[in]        partner_contour
    * @param[in]        merge_squeeze_factor
    * @param[in]        curvature_rear
    *
    * @return           N/A
    **/
   void get_all_merge_distances(float &current_begin_to_partner_begin_dist_sq,
                                float &current_begin_to_partner_end_dist_sq,
                                float &current_end_to_partner_begin_dist_sq,
                                float &current_end_to_partner_end_dist_sq,
                                const Contour_T &current_contour,
                                const Contour_T &partner_contour,
                                const float merge_squeeze_factor,
                                const float curvature_rear);

   /**
    * @brief            Calculate cumulative angle between segments to be merged,
    *                   description here: https://confluence.asux.aptiv.com/pages/viewpage.action?pageId=705929486
    *
    * @param[in]        first_contour_last_segment
    * @param[in]        second_contour_first_segment
    * @param[in]        max_cumulated_angle_threshold
    * @param[in]        max_merge_distance_to_avoid_cumulated_angle
    *
    **/
   bool check_cumulative_angle(const sg::geometry::Segment2D_T &first_contour_last_segment,
                               const sg::geometry::Segment2D_T &second_contour_first_segment,
                               const float max_cumulated_angle_threshold,
                               const float max_merge_distance_to_ignore_cumulated_angle);

   /**
    * @brief             Calculate angles between segments to be merged,
    *                    description here: https://confluence.asux.aptiv.com/pages/viewpage.action?pageId=705929486
    *
    * @param[in]         current_contour
    * @param[in]         partner_contour
    * @param[in]         current_contour_segment
    * @param[in]         partner_contour_segment
    * @param[in]         calibrations
    *
    **/
   bool check_all_merge_angles(const Contour_T &current_contour,
                               const Contour_T &partner_contour,
                               const geometry::Segment2D_T &current_contour_segment,
                               const geometry::Segment2D_T &partner_contour_segment,
                               const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations);

   /**
    * @brief            Check all possibilities with short and
    *                   single segment contours then calculate angles if needed,
    *                   more informations here: https://confluence.asux.aptiv.com/pages/viewpage.action?pageId=705929486
    *
    * @param[in]         current_contour
    * @param[in]         partner_contour
    * @param[in]         current_contour_segment
    * @param[in]         partner_contour_segment
    * @param[in]         calibrations
    *
    **/
   bool check_angles_for_short_contours(const Contour_T &current_contour,
                                        const Contour_T &partner_contour,
                                        const geometry::Segment2D_T &current_contour_segment,
                                        const geometry::Segment2D_T &partner_contour_segment,
                                        const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations);
}
#endif
