/*===================================================================================*\
* FILE: f360_kill_cluster.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains function signature of kill_cluster()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): f360_kill_obj_trk.h
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_KILL_CLUSTER_H
#define F360_KILL_CLUSTER_H

#include "f360_cluster.h"
#include "f360_detection_hist.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void kill_cluster(
      F360_Cluster_T & cluster,
      F360_Detection_Hist_T & det_hist,
      F360_Tracker_Info_T & tracker_info
   );
}
#endif
