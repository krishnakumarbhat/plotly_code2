/*===================================================================================*\
* FILE:  f360_check_if_object_is_cross_moving.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential – Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of Is_Object_Cross_Moving() function.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef F360_CHECK_IF_OBJECT_IS_CROSS_MOVING_H
#define F360_CHECK_IF_OBJECT_IS_CROSS_MOVING_H

#include "f360_object_track.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   bool Is_Object_Cross_Moving(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib);
}

#endif



