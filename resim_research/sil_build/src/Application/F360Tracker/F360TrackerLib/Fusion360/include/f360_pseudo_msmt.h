/*===================================================================================*\
* FILE: f360_pseudo_msmt.h
*====================================================================================
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: Jan Buc
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains enum F360_PSEUDO_MSMT declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): f360_pseudo_msmt.h.m
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
#ifndef enum_F360_PSEUDO_MSMT
#define enum_F360_PSEUDO_MSMT

#include "f360_reuse.h"

namespace f360_variant_A
{
   enum F360_PSEUDO_MSMT : uint8_t
   {
      F360_PSEUDO_MSMT_POS_X = 0,
      F360_PSEUDO_MSMT_POS_Y = 1,
   };
}
#endif
