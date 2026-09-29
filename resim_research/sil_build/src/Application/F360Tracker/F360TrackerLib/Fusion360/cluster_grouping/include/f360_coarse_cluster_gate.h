/*===================================================================================*\
* FILE:  f360_coarse_cluster_gate.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of the function Coarse_Cluster_Gate() which can be used
* as a crude check if two clusters are positioned close by to each other
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#ifndef F360_COARSE_CLUSTER_GATE_H
#define F360_COARSE_CLUSTER_GATE_H

#include "f360_reuse.h"
#include "f360_cluster.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   bool Coarse_Cluster_Gate(
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &cluster1,
      const F360_Cluster_T &cluster2
   );
}
#endif 
