/*===================================================================================*\
* FILE:  f360_object_motion_classification_helpers.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential – Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of supporting functions used in Object_Motion_Classification().
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef OBJECT_MOTION_CLASSIFICATION_HELPERS_H
#define OBJECT_MOTION_CLASSIFICATION_HELPERS_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   bool Object_Motion_Status_Should_Be_Updated(
      const F360_Object_Track_T& object,
      const float32_t k_coasted_track_max_time_from_update);

   void Update_Object_Properties(
      F360_Object_Track_T& object,
      const F360_Globals_T& globals);

   int32_t Get_Min_Num_Consec_Moving(
      const F360_Object_Track_T& object,
      const float32_t host_yaw_rate_rad,
      const F360_Calibrations_T& calib);

}

#endif
