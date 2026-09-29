/*===================================================================================*\
* FILE: f360_clear_cluster.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Clear_Cluster
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include <cstring>
#include "f360_clear_cluster.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Clear_Cluster
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Cluster_T & cluster - Cluster to be cleared
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function clears all the fields of a cluster
   *
   \*===========================================================================*/

   void Clear_Cluster(
      F360_Cluster_T & cluster)
   {
      const int16_t id = cluster.id;
      (void)memset(&cluster, 0, sizeof(F360_Cluster_T));
      cluster.id = id;
      cluster.time_since_measurement = -1.0F;
      cluster.time_since_cluster_updated = -1.0F;
   }
}
