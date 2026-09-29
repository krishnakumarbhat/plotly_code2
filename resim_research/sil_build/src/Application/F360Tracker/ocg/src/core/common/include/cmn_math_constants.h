/*===================================================================================*\
* FILE: cmn_math_contants.h
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
*   This file contains function signature Matlab related build in functions
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
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef OCG_CMN_CONSTANTS_H
#define OCG_CMN_CONSTANTS_H

#include "ocg_reuse.h"

namespace ocg
{
   namespace cmn
   {

      static constexpr float OCG_SQRT1_2 = 0.707106781186547524401F; // 1/(square root of 2)
      static constexpr float OCG_MIN_PROBABILITY = 0.0F;
      static constexpr float OCG_MAX_PROBABILITY = 1.0F;
      static constexpr float OCG_EPSILON = 1.19e-07F;
      static constexpr float OCG_MIN_DENOMINATOR = OCG_EPSILON;


   } // cmn
} // ocg

#endif // OCG_CMN_CONSTANTS_H
