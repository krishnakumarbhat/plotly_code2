/*===================================================================================*\
* FILE: f360_det_is_in_bistatic_fp_area.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of function Is_Det_In_Bistatic_FP_Area();
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DET_IS_IN_BISTATIC_FP_AREA_H
#define DET_IS_IN_BISTATIC_FP_AREA_H

#include "f360_point.h"
#include "f360_calibrations.h"

namespace f360_variant_A 
{
   bool Is_Det_In_Bistatic_FP_Area(
      const Point& det_pos,
      const F360_Calibrations_T& calib);
}
#endif
