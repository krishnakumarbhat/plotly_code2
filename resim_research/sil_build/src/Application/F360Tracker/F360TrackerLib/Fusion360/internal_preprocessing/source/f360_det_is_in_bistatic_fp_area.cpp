/*===================================================================================*\
* FILE: f360_det_is_in_bistatic_fp_area.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of function Is_Det_In_Bistatic_FP_Area();
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_det_is_in_bistatic_fp_area.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Det_In_Bistatic_FP_Area()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_in_fp_area - flag indicating whether detection is placed in
   * bistatic FP area
   *
   * PARAMETERS:
   * const Point& det_pos - detection VCS position
   * const F360_Calibrations_T& calib - tracker calibrations
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Is_Det_In_Bistatic_FP_Area(
      const Point& det_pos,
      const F360_Calibrations_T& calib)
   {
      const bool f_is_in_fp_area = ((calib.k_bistatic_cond_assoc_area_min_lat < det_pos.y) &&
                                            (det_pos.y < calib.k_bistatic_cond_assoc_area_max_lat) &&
                                            (calib.k_bistatic_cond_assoc_area_min_lon < det_pos.x) &&
                                            (det_pos.x < calib.k_bistatic_cond_assoc_area_max_lon));

      return f_is_in_fp_area;
   }
}
