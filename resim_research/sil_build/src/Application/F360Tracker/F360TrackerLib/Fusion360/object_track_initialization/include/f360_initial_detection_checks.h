#ifndef F360_INITIAL_DETECTION_CHECKS_H
#define F360_INITIAL_DETECTION_CHECKS_H
/*===========================================================================*\
* FILE: f360_initial_detection_checks.h
*============================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Initial_Detection_Checks()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [September 06, 2020]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reuse.h"
#include "f360_detection_hist.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_cluster.h"

namespace f360_variant_A
{
   bool Initial_Detection_Checks(
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Cluster_T& cluster);
}

#endif
