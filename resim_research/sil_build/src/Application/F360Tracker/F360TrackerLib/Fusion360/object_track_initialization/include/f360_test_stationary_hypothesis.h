#ifndef F360_TEST_STATIONARY_HYPOTHESIS_H
#define F360_TEST_STATIONARY_HYPOTHESIS_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_test_stationary_hypothesis.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Test_Stationary_Hypothesis()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_reuse.h"
#include "f360_track_init.h"
#include "f360_calibrations.h"
#include "f360_detection_hist.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_cluster.h"
#include "f360_host.h"

namespace f360_variant_A
{
   F360_Track_Init_T Test_Stationary_Hypothesis(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Cluster_T& cluster,
      float32_t& longvel_estimate,
      float32_t& latvel_estimate);
}

#endif
