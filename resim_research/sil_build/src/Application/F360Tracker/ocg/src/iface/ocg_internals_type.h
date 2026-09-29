/*===================================================================================*\
* FILE: ocg_internals_type.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential  Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains internals stream definitions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef OCG_INTERNALS_TYPE_H
#define OCG_INTERNALS_TYPE_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

#include "ocg_reuse.h"
#include "ocg_underdrivability_type.h"

namespace ocg
{

    struct UD_Height_States_T
    {
        float mean_elevation;
        float mean_sq_elevation;
        float num_dets;
    };

    struct UD_RCS_States_T
    {
        float mean_range;
        float mean_sq_range;
        float mean_rcs;
        float mean_sq_rcs;
        float rcs_range;
        float num_dets;
    };

    struct OCG_Cell_Internal_T
    {
        uint16_t global_zone_idx;
        UD_Height_States_T state_height_can_pass;
        UD_Height_States_T state_height_is_likely_to_pass;
        UD_Height_States_T state_height_can_not_pass_upper;
        UD_Height_States_T state_height_can_not_pass_lower;

        UD_RCS_States_T state_RCS_slope_can_pass;
        UD_RCS_States_T state_RCS_slope_is_likely_to_pass;
        UD_RCS_States_T state_RCS_slope_can_not_pass_upper;
        UD_RCS_States_T state_RCS_slope_can_not_pass_lower;

        // Probabilities for different hypotheses in different range zones - height
        float p_height_can_pass;
        float p_height_is_likely_to_pass;
        float p_height_can_not_pass_upper;
        float p_height_can_not_pass_lower;

        // Probabilities for different hypotheses in different range zones - RCS slope
        float p_RCS_slope_can_pass;
        float p_RCS_slope_is_likely_to_pass;
        float p_RCS_slope_can_not_pass_upper;
        float p_RCS_slope_can_not_pass_lower;

        // Probabilities for different hypotheses considering both height and RCS slope together in different range zones
        float p_can_pass;
        float p_is_likely_to_pass;
        float p_can_not_pass;
    };

    struct OCG_Internal_Props_T
    {
        OCG_Position_T ogcs_host_rear_axle_initial_position;
        float host_travel_distance;
        uint64_t timestamp_us;
        uint64_t prev_timestamp_us;
        uint16_t circular_buffer_idx;
    };
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
