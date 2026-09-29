/*===================================================================================*\
* FILE: f360_update_extended_bbox_offsets_for_object_in_dead_zone.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_UPDATE_EXTENDED_BBOX_OFFSETS_FOR_OBJECT_IN_DEAD_ZONE_H
#define F360_UPDATE_EXTENDED_BBOX_OFFSETS_FOR_OBJECT_IN_DEAD_ZONE_H

#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_dead_zone.h"

namespace f360_variant_A
{
   void Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(
      const F360_Calibrations_T& calib,
      const float32_t host_speed,
      const Dead_Zone_T& dead_zone,
      const F360_Object_Track_T& object,
      float32_t& rear_offset,
      float32_t& front_offset);
}

#endif
