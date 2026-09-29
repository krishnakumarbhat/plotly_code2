/*===================================================================================*\
* FILE: sg_cluster_list.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Cluster List - the container for Cluster objects.
*   Cluster objects are stored in ClusterList which has type of EmbeddedList.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CLUSTER_LIST_H
#define SG_CLUSTER_LIST_H

#include "embedded_list.h"
#include "sg_cluster.h"
#include "sg_constants.h"

namespace sg
{
   using ClusterList = EmbeddedList<Cluster, SG_MAX_NUM_INTERNAL_CLUSTERS>;
}

#endif
