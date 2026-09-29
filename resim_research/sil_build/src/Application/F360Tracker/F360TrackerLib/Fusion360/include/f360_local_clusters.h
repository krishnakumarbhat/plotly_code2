/*===================================================================================*\
* FILE: f360_local_clusters.h
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
*   This file contains F360_Local_Clusters_Tag structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): TypesFusion360.h
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
#ifndef F360_LOCAL_CLUSTERS_H
#define F360_LOCAL_CLUSTERS_H

#include "f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   typedef struct F360_Local_Clusters_Tag
   {
      uint8_t num_dets_in_clusters[MAX_TRACKER_POSN_CLUSTERS];
      int16_t array_of_det_idxs_in_clusters[MAX_NUMBER_OF_DETECTIONS];
      uint16_t array_of_first_det_idx_in_clusters[MAX_TRACKER_POSN_CLUSTERS];
      uint16_t num_of_associated_dets;
      uint16_t num_clusters;
   }F360_Local_Clusters_T;
}
#endif
