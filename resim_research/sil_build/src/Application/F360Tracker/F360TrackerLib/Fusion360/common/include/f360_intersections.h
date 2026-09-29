/*===================================================================================*\
* FILE: f360_intersections.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of Intersection functionality.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_INTERSECTIONS_H
#define F360_INTERSECTIONS_H


#include "f360_object_track.h"
#include "f360_point.h"
#include "f360_circle.h"

namespace f360_variant_A
{
   const uint8_t Max_Num_Intersections_Circle_Rectangle = 8U;

   uint32_t Find_Intersections_in_TCS(
      const Circle &circle_tcs,
      const F360_Object_Track_T &object,
      Point(&intersections_tcs)[Max_Num_Intersections_Circle_Rectangle]
   );

   bool Determine_Segments_Intersection_Limited(
      const float32_t(&end_point_A1)[2],
      const float32_t(&end_point_A2)[2],
      const float32_t(&end_point_B1)[2],
      const float32_t(&end_point_B2)[2],
      float32_t & result_x,
      float32_t & result_y);
}

#endif
