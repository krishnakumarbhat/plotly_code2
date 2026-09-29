/*===================================================================================*\
* FILE: f360_matrix_dimension.h
*====================================================================================
* Copyright 2023 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   F360 matrix dimension defitions
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
#ifndef F360_MATRIX_DIMENSION_H
#define F360_MATRIX_DIMENSION_H
#include "f360_reuse.h"

namespace f360_variant_A
{
   enum F360_2D_COV_IDX : uint8_t{
    F360_2D_IDX_X = 0,
    F360_2D_IDX_Y = 1,
    F360_2D_IDX_DIMENSION = 2
   };
}
#endif
