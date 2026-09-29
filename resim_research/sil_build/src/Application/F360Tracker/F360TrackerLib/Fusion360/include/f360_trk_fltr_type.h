/*===================================================================================*\
* FILE: F360_trk_fltr_type.h
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
*   This file contains F360_Trk_Fltr_Type_T   enum declaration
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
#ifndef F360_TRK_FLTR_TYPE_H
#define F360_TRK_FLTR_TYPE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   enum F360_Trk_Fltr_Type_T : uint8_t
   {
      F360_TRACKER_TRKFLTR_INVALID = 0,
      F360_TRACKER_TRKFLTR_CCV = 1,
      F360_TRACKER_TRKFLTR_CTCA = 2,
      F360_TRACKER_TRKFLTR_CCA = 3
   };
}
#endif
