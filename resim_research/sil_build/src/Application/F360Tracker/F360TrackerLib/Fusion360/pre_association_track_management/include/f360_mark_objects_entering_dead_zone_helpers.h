/*===================================================================================*\
* FILE: f360_mark_objects_entering_dead_zone_helpers.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of supporting functions used by Mark_Objects_Enterning_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef MARK_OBJECTS_ENTERING_DEAD_ZONE_HELPERS_H
#define MARK_OBJECTS_ENTERING_DEAD_ZONE_HELPERS_H

#include "f360_dead_zone.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_bounding_box.h"

namespace f360_variant_A
{
   bool Is_Center_In_Dead_Zone(
      const float32_t center_long_pos,
      const Interval<float32_t>& zone);

   bool Is_Rear_In_Zone(
      const BboxCorners& bbox,
      const Interval<float32_t>& zone);

   bool Is_Front_In_Zone(
      const BboxCorners& bbox,
      const Interval<float32_t>& zone);

   F360_Dead_Zone_Status_T Determine_Object_Dead_Zone_Status(
      const Dead_Zone_T& dead_zone,
      const F360_Object_Track_T& object,
      const float32_t k_dead_zone_max_obj_vcs_lat_pos,
      const float32_t k_dead_zone_max_obj_vcs_heading);
}

#endif
