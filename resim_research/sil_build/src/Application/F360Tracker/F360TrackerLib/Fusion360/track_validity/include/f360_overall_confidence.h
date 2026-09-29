#ifndef F360_OVERALL_CONFIDENCE_H
#define F360_OVERALL_CONFIDENCE_H
/*===================================================================================*\
* FILE: f360_overall_confidence.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains function declarations for functions related to the overall confidence.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   void Overall_Confidence(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS], 
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Tracker_Info_T& tracker_info, 
      const F360_Calibrations_T& calib);
}
#endif
