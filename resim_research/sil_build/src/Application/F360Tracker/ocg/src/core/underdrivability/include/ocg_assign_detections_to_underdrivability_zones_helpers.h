/*===================================================================================*\
* FILE: ocg_assign_detections_to_underdrivability_zones_helpers.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Assign_Detections_To_Underdrivability_Zones() helper functions declarations
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef ASSIGN_DETECTIONS_TO_UNDERDRIVABILITY_ZONES_HELPERS_H
#define ASSIGN_DETECTIONS_TO_UNDERDRIVABILITY_ZONES_HELPERS_H

#include "ocg_underdrivability.h"
#include "rspp_detection.h"
#include "ocg_calibrations.h"
#include "ocg_reuse.h"

namespace ocg
{
    void Update_Zone_Innovation(
        OCG_Zones_Innovation_T &zone_innovation,
        const OCG_Calibrations_T &calib,
        const float &det_vcs_rng,
        const float &det_vcs_elev,
        const rspp_variant_A::Raw_Detection_T &det,
        const bool &is_ground_detection,
        const float lateral_weight);

    float Calc_Lateral_Distance_To_Curved_Host_Path(
        const float curvature_rear,
        const float small_curvature_th,
        const float vcs_position_x,
        const float vcs_position_y);

    bool Is_Detection_Valid(
        const rspp_variant_A::RSPP_Detection_T &det,
        const OCG_Calibrations_T &calib,
        const rspp_variant_A::F360_Radar_Sensor_T (&sensors)[rspp_variant_A::MAX_NUMBER_OF_SENSORS],
        const float closest_dist_to_host_path,
        const float max_lat_offset_from_host_curv);

    void Calc_Det_Elevation_Params(
        const rspp_variant_A::RSPP_Detection_T &det,
        float &det_vcs_rng,
        float &det_vcs_height,
        float &det_vcs_elev);

    uint32_t Calc_In_Which_Zone_Det_Is_Located(
        const OCG_Underdrivability_Internal_T &in_underdrivability,
        const float det_vcs_long_posn);

    void Compensate_Ground_Detection(
        const OCG_Calibrations_T &calib,
        float &det_height,
        float &det_elev,
        bool &is_ground_detection,
        const float &det_rng);

}
#endif
