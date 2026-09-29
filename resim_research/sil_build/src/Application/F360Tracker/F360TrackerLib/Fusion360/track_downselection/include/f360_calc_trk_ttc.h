/*===================================================================================*\
* FILE: f360_calc_trk_ttc.h
*====================================================================================
* Copyright (C) 2020 Aptiv. All Rights Reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Calc_Trk_Ttc()
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_CALC_TRK_TTC_H
#define F360_CALC_TRK_TTC_H

#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_host.h"

namespace f360_variant_A
{
   float32_t Calc_Trk_Ttc(
      const F360_Object_Track_T& obj_trk,
      const F360_Host_T& host,
      const float32_t cos_host_vcs_sideslip,
      const float32_t sin_host_vcs_sideslip,
      const F360_Calibrations_T& calib,
      const float32_t range);
}
#endif
