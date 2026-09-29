/*===================================================================================*\
* FILE:  f360_update_object_orientation_uncertainty.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Update_Object_Orientation_Uncertainty() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#ifndef UPDATE_OBJECT_ORIENTATION_UNCERTAINTY_H
#define UPDATE_OBJECT_ORIENTATION_UNCERTAINTY_H

#include "f360_object_track.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Update_Object_Orientation_Uncertainty(
      const F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
}

#endif
