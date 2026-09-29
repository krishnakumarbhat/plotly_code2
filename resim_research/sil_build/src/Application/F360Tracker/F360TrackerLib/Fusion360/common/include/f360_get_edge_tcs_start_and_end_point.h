/*===================================================================================*\
* FILE: f360_get_edge_tcs_start_and_end_point.h
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains function declaration of Get_Edge_TCS_Start_And_End_Point().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#ifndef F360_GET_EDGE_TCS_START_AND_END_POINT_H
#define F360_GET_EDGE_TCS_START_AND_END_POINT_H

#include "f360_object_track.h"
#include "f360_bounding_box.h"
#include "f360_object_sides.h"

namespace f360_variant_A
{
   bool Get_Edge_TCS_Start_And_End_Point(
      const F360_Object_Track_T & obj,
      const F360_Object_Sides_T edge,
      Point & start_point_tcs,
      Point & end_point_tcs);
}
#endif
