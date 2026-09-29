/*===================================================================================*\
* FILE: f360_clear_cluster.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Clear_Cluster
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_CLEAR_CLUSTER_H
#define F360_CLEAR_CLUSTER_H

#include "f360_cluster.h"

namespace f360_variant_A
{
   void Clear_Cluster(
      F360_Cluster_T & cluster);
}

#endif
