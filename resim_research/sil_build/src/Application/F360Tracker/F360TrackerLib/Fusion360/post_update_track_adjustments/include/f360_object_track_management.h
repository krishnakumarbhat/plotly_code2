/*===================================================================================*\
* FILE:  f360_object_track_management.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* N/A
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_OBJECT_TRACK_MANAGEMENT_H
#define F360_OBJECT_TRACK_MANAGEMENT_H

#include "f360_detection_props.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_timing_info.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Obj_Trk_Status_Book_Keeping(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T &calibs,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T &timing_info
   );
}
#endif
