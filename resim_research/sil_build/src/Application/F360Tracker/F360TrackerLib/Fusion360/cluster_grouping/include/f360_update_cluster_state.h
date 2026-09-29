/*===================================================================================*\
* FILE:  f360_update_cluster_state.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the declaration of the function Update_Cluster_State() which can be
* used to update the states of clusters over time when new detections are added or old
* detections are removed.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#ifndef F360_UPDATE_CLUSTER_STATE_H
#define F360_UPDATE_CLUSTER_STATE_H

#include "f360_reuse.h"
#include "f360_cluster.h"
#include "f360_host_props.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_detection_hist.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   void Update_Cluster_State(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Hist_T & det_hist,
      F360_Cluster_T & cluster);
}

#endif
