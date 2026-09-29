#ifndef F360_DETERMINE_FINAL_VEL_ESTIMATE_H
#define F360_DETERMINE_FINAL_VEL_ESTIMATE_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_determine_final_vel_estimate.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Determine_Final_Vel_Estimate()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_reuse.h"
#include "f360_track_init.h"
#include "f360_conf.h"

namespace f360_variant_A
{
   F360_Track_Init_T Determine_Final_Vel_Estimate(
      const float32_t posdiff_longvel,
      const float32_t posdiff_latvel,
      const float32_t cloud_longvel,
      const float32_t cloud_latvel,
      const CONF3_T posdiff_confidence,
      CONF3_T cloud_confidence,
      float32_t& longvel_estimate,
      float32_t& latvel_estimate);
}

#endif
