/*===================================================================================*\
* FILE: f360_check_if_point_is_inside_box.h
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains declarations of functions that checks if a point is inside a 
*   defined box. 
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef CHECK_IF_POINT_IS_INSIDE_BOX_H
#define CHECK_IF_POINT_IS_INSIDE_BOX_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"
#include "f360_bounding_box.h"

namespace f360_variant_A
{
   bool Check_If_Point_Is_Inside_Box_In_Same_CS(
      const float32_t x_point,
      const float32_t y_point,
      const float32_t (&box)[2][2]
   );

   bool Check_If_Vcs_Point_Is_Inside_Bounding_Box(
      const float32_t x_point_vcs,
      const float32_t y_point_vcs,
      const F360_Object_Track_T& obj
   );

   bool Check_If_Point_Is_Inside_Extended_Bounding_Box(
      const Point & point_vcs,
      const F360_Object_Track_T & obj
   );

   bool Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(
      const Point &point_vcs,
      const F360_Object_Track_T &obj,
      const bool f_water_spray,
      const F360_Calibrations_T &calib
   );

   void Determine_Extended_Bounding_Box(
      const F360_Object_Track_T & obj,
      BoundingBox & box);
}
#endif
