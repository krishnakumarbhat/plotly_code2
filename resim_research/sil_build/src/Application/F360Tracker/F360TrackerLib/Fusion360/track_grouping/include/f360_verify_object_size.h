/*===================================================================================*\
* FILE: f360_verify_object_size.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declarations of Verify_Object_Size function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_VERIFY_OBJECT_SIZE_H
#define F360_VERIFY_OBJECT_SIZE_H

#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_dimensions.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   bool Verify_Object_Size(
      const F360_Object_Track_T & obj_to_keep,
      const F360_Object_Track_T & obj_to_kill,
      const F360_Calibrations_T & calibs,
      const F360_Tracker_Info_T & tracker_info,
      const F360_Dimensions_T & initial_merged_obj_dimensions);
}
#endif
