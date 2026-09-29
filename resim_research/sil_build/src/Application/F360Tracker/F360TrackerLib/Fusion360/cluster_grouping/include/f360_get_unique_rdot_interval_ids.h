/*===================================================================================*\
* FILE: f360_get_unique_rdot_interval_ids.h
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
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
*   This file contains function signature of get unique rdot interval ids
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): getUniqueRdotIntervalIds.m
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
#ifndef F360_GET_UNIQUE_RDOT_INTERVAL_IDS_H
#define F360_GET_UNIQUE_RDOT_INTERVAL_IDS_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   void Get_Unique_Rdot_Interval_Ids(
      const float32_t (&unique_rdot_interval_widths)[MAX_NUM_UNIQUE_RDOT_INTERVAL],
      const int32_t num_unique_rdot_interval_widths,
      const int32_t ndets,
      const float32_t (&rdot_interval_width)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
      int32_t (&rdot_interval_ids)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
      int32_t (&unique_rdot_interval_ids)[MAX_NUM_UNIQUE_RDOT_INTERVAL],
      int32_t & num_unique_rdot_intervals
   );
}
#endif
