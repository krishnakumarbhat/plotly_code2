#ifndef F360_CHECK_IF_OBJECTS_CAN_OVERLAP_SIGNIFICANTLY_H
#define F360_CHECK_IF_OBJECTS_CAN_OVERLAP_SIGNIFICANTLY_H
/*===================================================================================*\
* FILE: f360_check_if_objects_can_overlap_significantly.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains class declaration of functions
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_reuse.h"
#include "f360_point.h"

namespace f360_variant_A
{
   bool Check_If_Objects_Can_Overlap_Significantly(
      const Point& obj1_center_position_vcs,
      const float32_t obj1_length,
      const Point& obj2_center_position_vcs,
      const float32_t obj2_length);
}

#endif
