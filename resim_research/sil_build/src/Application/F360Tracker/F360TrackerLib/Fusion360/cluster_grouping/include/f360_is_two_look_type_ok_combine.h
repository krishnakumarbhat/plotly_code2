/*===================================================================================*\
* FILE: f360_is_two_look_type_ok_combine.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: AIT-69%
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
*   The file contains the defination of is_two_look_type_ok_combine function
*
* ABBREVIATIONS:
*
*
* TRACEABILITY INFO:
*   Design Document(s): isTwoLookTypeOkCombine.m
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
#ifndef F360_IS_TWO_LOOK_TYPE_OK_COMBINE_H
#define F360_IS_TWO_LOOK_TYPE_OK_COMBINE_H

#include "f360_reuse.h"
#include "f360_globals.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Is_Two_Look_Type_Ok_Combine_For_Zero_Intervals(
      const F360_Calibrations_T &calib,
      const float32_t &rdot_diff,
      bool &f_success);

   void Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calib,
      const float32_t rdot_dealiased,
      const float32_t rdot_2,
      const float32_t speed_interval,
      bool &f_success,
      float32_t &index);

   void Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calib,
      const float32_t rdot_1,
      const float32_t rdot_2,
      const float32_t interval_1,
      const float32_t interval_2,
      bool &f_success,
      float32_t &index_1,
      float32_t &index_2);
}
#endif
