/*===========================================================================*\
* FILE: f360_mark_azimuth_range_rate_outliers.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Mark_Azimuth_Range_Rate_Outliers()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_MARK_AZIMUTH_RANGE_RATE_OUTLIERS_H
#define F360_MARK_AZIMUTH_RANGE_RATE_OUTLIERS_H

#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Mark_Azimuth_Range_Rate_Outliers(
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      const float32_t dist_to_rear_axle,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);
}


#endif
