/*===========================================================================*\
* FILE: f360_nearby_wheel_spins.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Detect_Near_By_Wheel_Spins() declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_NEAR_BY_WHEEL_SPINS_H
#define F360_NEAR_BY_WHEEL_SPINS_H

#include "f360_detection_props.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   void Detect_Nearby_Wheel_Spins(
      const rspp_variant_A::RSPP_Detection_List_T & raw_detections,
      const F360_Calibrations_T &calibrations,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS]
   );
}
#endif
