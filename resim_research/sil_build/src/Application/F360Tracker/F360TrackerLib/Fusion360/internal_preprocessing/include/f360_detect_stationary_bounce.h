/*===========================================================================*\
* FILE: f360_detect_stationary_bounce.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains functions declarations: Detect_Stationary_Bounce_Detections() and Mark_Stationary_Bounce_Detections_For_Track().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_DETECT_STATIONARY_BOUNCE_H
#define F360_DETECT_STATIONARY_BOUNCE_H

#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   void Detect_Stationary_Bounce_Detections(
      const F360_Calibrations_T &calib,
      const F360_Host_T &host,
      const F360_Tracker_Info_T &tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   );

   void Mark_Stationary_Bounce_Detections_For_Track(
      const F360_Calibrations_T &calib,
      const F360_Host_T &host,
      const F360_Object_Track_T &curr_trk,
      const uint32_t num_det_valid,
      const int32_t first_sort_idx,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   );
}
#endif
