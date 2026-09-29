/*=============================================================================================*\
* FILE: sg_determine_drivability_class.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of function for determination of drivability.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_DETERMINE_DRIVABILITY_CLASS_H
#define SG_DETERMINE_DRIVABILITY_CLASS_H

#include "geometry/geo_point.h"
#include "sg_calibrations.h"
#include "sg_drivability_class.h"

namespace sg
{
   /**
    * @brief          Determine detection's drivability
    *
    * @param[in]      view ranges - view ranges used in SG
    * @param[in]      position - position of detection (in ISO)
    * @return         drivability - drivability class of detection
    **/
   SG_Drivability_Class_T sg_determine_drivability_class(const View_Ranges_T &view_ranges, const geometry::Point3D_T position);

   /**
    * @brief          Check if detection is within given limits
    *
    * @param[in]      position - position of detection (in ISO)
    * @param[in]      range - limits of the region
    * @return         result - if detection is within given region limits
    **/
   inline bool is_in_view_range(const geometry::Point3D_T &position, const View_Range_T &range)
   {
      bool result                             = false;
      const bool is_within_longitudinal_range = (range.longitudinal.min <= position.x) && (position.x <= range.longitudinal.max);
      const bool is_within_lateral_range      = (range.lateral.min <= position.y) && (position.y <= range.lateral.max);
      const bool is_within_vertical_range =
         ((range.vertical.overground.min <= position.z) && (position.z <= range.vertical.overground.max))
         || ((range.vertical.underground.min <= position.z) && (position.z <= range.vertical.underground.max));
      if (is_within_longitudinal_range && is_within_lateral_range && is_within_vertical_range)
      {
         result = true;
      }
      (void) is_within_lateral_range;  // MISRA
      (void) is_within_vertical_range; // MISRA
      return result;
   }
}

#endif