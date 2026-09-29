/*=============================================================================================*\
* FILE: sg_determine_drivability_class.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of function for determination of drivability.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_determine_drivability_class.h"

namespace sg
{
   SG_Drivability_Class_T sg_determine_drivability_class(const View_Ranges_T &view_ranges, const geometry::Point3D_T position)
   {
      SG_Drivability_Class_T drivability;
      if (is_in_view_range(position, view_ranges.nondrivable))
      {
         drivability = SG_Drivability_Class_T::NONDRIVABLE;
      }
      else if (is_in_view_range(position, view_ranges.overdrivable))
      {
         drivability = SG_Drivability_Class_T::OVERDRIVABLE;
      }
      else if (is_in_view_range(position, view_ranges.underdrivable))
      {
         drivability = SG_Drivability_Class_T::UNDERDRIVABLE;
      }
      else
      {
         drivability = SG_Drivability_Class_T::UNCLASSIFIED;
      }
      return drivability;
   }
}