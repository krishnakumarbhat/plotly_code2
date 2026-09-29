/*===================================================================================*\
* FILE: f360_priority_update_tracks.h
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
* This file contains function signature of calculate_priority()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): f360_calculate_priority.h
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
#ifndef F360_PRIORITY_UPDATE_TRACKS_H
#define F360_PRIORITY_UPDATE_TRACKS_H

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_host.h"

#include "f360_constants.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
namespace f360_variant_A
{
   void Update_Track_Priority(
      const F360_Host_T &host_props,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T &tracker_info);
}
#endif
