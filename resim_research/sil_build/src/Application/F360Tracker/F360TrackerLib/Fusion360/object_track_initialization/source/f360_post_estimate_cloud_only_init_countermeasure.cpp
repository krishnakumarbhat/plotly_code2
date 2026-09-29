/******************************************************************************
* Copyright 2025 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_post_estimate_cloud_only_init_countermeasure.cpp
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Post_Estimate_Cloud_Only_Init_Countermeasure()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_post_estimate_cloud_only_init_countermeasure.h"
#include "f360_math_func.h"

namespace f360_variant_A
{

    /*===========================================================================*\
    * FUNCTION: Post_Estimate_Cloud_Only_Init_Countermeasure()
    *===========================================================================
    * RETURN VALUE:
    * NONE
    *
    * PARAMETERS:
    * const F360_Host_T& host,
    * const F360_Cluster_T& cluster,
    * const CONF3_T cloud_confidence,
    * const CONF3_T posdiff_confidence,
    * const float32_t longvel_estimate,
    * F360_Track_Init_T& init_type
    *
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * After moving object initialization conditions are estimated, this function stops cloud only init if
    * certain conditions are met. This is to stop potential angle jump dets to initialize ghost objects.
    * Sometimes angle jump dets can be reported by the radar and pass through angle jump checks in the tracker, but
    * these dets have defined range rate properties which can allow them to form ghost objects from cloud only init. This function
    * checks for certain conditions which can predict a potential angle jump based object and inhibit initializing it
    * with cloud only init. The conditions are set to reduce scope so that the impact of countermeasure is only on angle jump objects
    * and if other objects are impacted then they are not for critical objects which can be initialized a bit later (~0.1s) using other schemes.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
    void Post_Estimate_Cloud_Only_Init_Countermeasure(
        const F360_Host_T& host,
        const F360_Cluster_T& cluster,
        const CONF3_T posdiff_confidence,
        const float32_t longvel_estimate,
        F360_Track_Init_T& init_type)
    {
        constexpr float32_t lat_thres_funnel_longfactor = 0.05F;
        constexpr float32_t lat_thres_funnel_offset = 1.0F;

        const bool f_candidate_far_away = (cluster.vcs_position_x > 40.0F); // To reduce scope of countermeasure to act only on far away objects which if incorrectly identified, still have enough time to be initialized using other schemes
        const bool f_highway_scenario = (host.speed > 25.0F); // To reduce scope of countermeasure to act in highway scenarios where angle jump ghosts can have larger feature function impacts
        const bool f_candidate_driving_fast_along_host_dir = (longvel_estimate > 20.0F); // To ensure highway scenario and that no object which could have a large relative velocity towards host is inhibited from init
        const bool f_candidate_slower_than_host = (longvel_estimate < host.speed); // To ensure countermeasure is only active in overtaking cases where anglejump ghost objects can stay in closer critical region for feature functions
        const bool f_candidate_latpos_not_in_hostpath = (fabsf(cluster.vcs_position_y) > (lat_thres_funnel_offset + lat_thres_funnel_longfactor * cluster.vcs_position_x)); // position of object outside host path which is denoted by a funnel with increasing y threshold when x position is farther along with an offset
        if (f_candidate_far_away &&
            f_highway_scenario &&
            f_candidate_driving_fast_along_host_dir &&
            f_candidate_slower_than_host &&
            f_candidate_latpos_not_in_hostpath &&
            (F360_TRACK_INIT_CLOUD == init_type) &&
            (CONF3_NONE == posdiff_confidence))
        {
            init_type = F360_TRACK_INIT_INVALID;
        }
    }
}
