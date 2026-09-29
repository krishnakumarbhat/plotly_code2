/*===================================================================================*\
* FILE:  f360_calulate_priority.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function signatures to compute cluster and object priorities.
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#ifndef F360_CALCULATE_PRIORITY_H
#define F360_CALCULATE_PRIORITY_H

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_cluster.h"

namespace f360_variant_A
{
   float32_t Calculate_Priority_For_Cluster(
      const F360_Host_T& host,
      const F360_Cluster_T& cluster);

   float32_t Calculate_Priority(
      const F360_Host_T & host_props,
      const float32_t movable_prob,
      const float32_t confidence,
      const float32_t longitudal_pos,
      const float32_t lateral_pos);
}
#endif
