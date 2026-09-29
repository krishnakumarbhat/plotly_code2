/*===================================================================================*\
* FILE: f360_calculate_jacobian_CTCA.h
*====================================================================================
* Copyright 2020 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function signature of calculateJacobianCTCA() function
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_CALC_JACOBIAN_H
#define F360_CALC_JACOBIAN_H

#include "f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   void F360_Calculate_Jacobian_CTCA(
      const float32_t cos_psi,
      const float32_t sin_psi,
      const float32_t velocity,
      const float32_t curvature,
      const float32_t acceleration,
      const float32_t dT,
      const float32_t (&tcs_vec_from_center_rear_to_ref_pnt)[2],
      float32_t (&jacobian)[STATE_DIMENSION][STATE_DIMENSION]);
}
#endif
