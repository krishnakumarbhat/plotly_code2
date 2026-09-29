/*===================================================================================*\
* FILE: f360_object_based_multibounce_detector.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains declarations of functions related to object based 
*    multibounce detector.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_OBJECT_BASED_MULTIBOUNCE_DETECTOR_INTERNALS_H
#define F360_OBJECT_BASED_MULTIBOUNCE_DETECTOR_INTERNALS_H

#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_bounding_box.h"


namespace f360_variant_A
{
   struct Bounce_Origin
   {
      Point position;            // position if found
      bool f_found;      // true - found, false - not found
   };

   BoundingBox Create_Area_Of_Correct_Detections(
      const F360_Object_Track_T &obj_track,
      const float32_t area_width);

   bool Is_Det_Object_Based_Multibounce(
      const F360_VCS_Velocity_T &sensor_velocity,
      const Point &sensor_mount_pos_vcs,
      const F360_Detection_Props_T &det_prop,
      const rspp_variant_A::RSPP_Detection_T &det_raw,
      const F360_VCS_Velocity_T &obj_velocity_vcs,
      const BoundingBox &restricted_area,
      const F360_Calibrations_T& calibs);

   Bounce_Origin Find_Bounce_Origin_Point(
      const Point &detection_pos_vcs,
      const Point &sensor_mount_pos_vcs,
      const BoundingBox &restricted_area);

   uint16_t Estimate_Num_Bounces(
      const Point &sensor_mount_pos_vcs,
      const Point &bounce_origin_point,
      const float32_t det_range);
}
#endif
