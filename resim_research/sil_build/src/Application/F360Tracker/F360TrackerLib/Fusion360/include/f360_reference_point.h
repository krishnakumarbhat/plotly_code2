#ifndef F360_REFERENCE_POINT_H
#define F360_REFERENCE_POINT_H
/*===================================================================================*\
* FILE: f360_reference_point.h
*====================================================================================
*Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains enum definitions for object reference point.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
   enum F360_Reference_Point_T : uint8_t
   {
      F360_REFERENCE_POINT_CENTER = 0,
      F360_REFERENCE_POINT_FRONT_LEFT = 1,
      F360_REFERENCE_POINT_FRONT = 2,
      F360_REFERENCE_POINT_FRONT_RIGHT = 3,
      F360_REFERENCE_POINT_RIGHT = 4,
      F360_REFERENCE_POINT_REAR_RIGHT = 5,
      F360_REFERENCE_POINT_REAR = 6,
      F360_REFERENCE_POINT_REAR_LEFT = 7,
      F360_REFERENCE_POINT_LEFT = 8
   };
}
#endif
