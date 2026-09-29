#ifndef F360_POST_ESTIMATE_CLOUD_ONLY_INIT_COUNTERMEASURE_H
#define F360_POST_ESTIMATE_CLOUD_ONLY_INIT_COUNTERMEASURE_H
/******************************************************************************
* Copyright 2025 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_post_estimate_cloud_only_init_countermeasure.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Post_Estimate_Cloud_Only_Init_Countermeasure()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/
#include "f360_reuse.h"
#include "f360_conf.h"
#include "f360_cluster.h"
#include "f360_host.h"
#include "f360_track_init.h"

namespace f360_variant_A
{
    void Post_Estimate_Cloud_Only_Init_Countermeasure(
        const F360_Host_T& host,
        const F360_Cluster_T& cluster,
        const CONF3_T posdiff_confidence,
        const float32_t longvel_estimate,
        F360_Track_Init_T& init_type);
}

#endif
