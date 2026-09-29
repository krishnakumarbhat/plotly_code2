/*===========================================================================*\
* FILE: f360_longi_stat_curve_init.h
*============================================================================
* Copyright (C) 2020 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Longi_Stat_Curve_Init() declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_LONGI_STAT_CURVE_INIT_H
#define F360_LONGI_STAT_CURVE_INIT_H

#include "f360_longi_stat_curve.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   void F360_Longi_Stat_Curve_Init(F360_Longi_Stat_Curve_T(&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]);
}
        

#endif
