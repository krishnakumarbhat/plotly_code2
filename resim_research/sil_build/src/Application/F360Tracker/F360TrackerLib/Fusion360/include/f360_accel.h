/*===================================================================================*\
* FILE: f360_accel.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
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
*   This file contains F360_Accel_T  structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): TypesFusion360.h
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
#ifndef F360_ACCEL_H
#define F360_ACCEL_H

#include "f360_reuse.h"
#include "f360_math.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   typedef struct F360_VCS_Accel_Tag
   {
      float32_t longitudinal;
      float32_t lateral;
   } F360_VCS_Accel_T;

   inline bool operator==(const F360_VCS_Accel_T& acc_1, const F360_VCS_Accel_T& acc_2)
   {
      return (
         (std::abs(acc_1.longitudinal - acc_2.longitudinal) < F360_EPSILON) &&
         (std::abs(acc_1.lateral - acc_2.lateral) < F360_EPSILON)
         );
   }

   static_assert(8 == sizeof(F360_VCS_Accel_T), "sizeof(F360_VCS_Accel_T) not as expected. Remember to align padding if needed");
}
#endif
