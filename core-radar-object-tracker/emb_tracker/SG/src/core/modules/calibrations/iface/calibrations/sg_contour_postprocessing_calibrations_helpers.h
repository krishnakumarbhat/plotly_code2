/*===========================================================================*\
* FILE: sg_contour_postprocessing_calibrations_helpers.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains contour postprocessing calibrations helpers.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_CONTOUR_POSTPROCESSING_CALIBRATIONS_HELPERS_H
#define SG_CONTOUR_POSTPROCESSING_CALIBRATIONS_HELPERS_H

#include "sg_reuse.h"

namespace sg
{
struct DeclutterContoursCalibs
{
   // get_occluded_contours
   float contour_occlusion_threshold{};
   // get_occluded_vertices
   float contour_occlusion_azimuth_margin{};
   float contour_occlusion_range_margin[2]{};
   uint8_t occluders_num_threshold{};
};

struct MergeContoursCalibs
{
   // merge_contours
   // description for merge contours calibrations:
   // https://confluence.asux.aptiv.com/pages/viewpage.action?pageId=705929486
   float merge_distance_threshold{};
   float merge_max_angle{};
   float merge_max_cumulative_angle{};
   float merge_max_single_segment_length{};               // for single segment contours whose length is smaller than
                                                          // this threshold, the cumulative angle is ignored
   float max_merge_distance_to_ignore_cumulative_angle{}; // if vertex distance between merge partners is smaller than
                                                          // this threshold, cumulative angle requirements are not
                                                          // necessary
   float merge_longitudinal_squeeze_factor{};
};

struct SimplifyContoursCalibs
{
   // presimplify_contours
   float presimplify_host_length{};
   float presimplify_min_angle{};
   float presimplify_max_angle{};
   // simplify_contours
   float simplify_min_turning_angle{};
   float simplify_length_range[2]{};
   float simplify_angle{};

   uint8_t simplify_type{};
   uint8_t presimplify_type{};
};

struct RemoveVerticesCalibs
{
   // remove_vertices
   float remove_vertices_front_limit{};
   float remove_vertices_rear_limit{};
   float remove_vertices_left_limit{};
   float remove_vertices_right_limit{};
   float accepted_uncertainty{};
   uint16_t max_num_cycles_no_update{};
};

struct SelectAndRemoveExcessContoursCalibs
{
   uint16_t max_num_contoured_clusters{};
};
} // namespace sg
#endif
