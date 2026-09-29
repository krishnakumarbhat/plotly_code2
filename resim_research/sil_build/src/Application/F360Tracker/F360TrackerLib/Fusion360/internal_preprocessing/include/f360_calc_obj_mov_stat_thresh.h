/*===================================================================================*\
* FILE: f360_calc_obj_mov_stat_thresh.h
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
*   This file contains function signature of funcions to calculate the threshold as a
*   function of host speed
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): calcObjMovStatThresh.m.m
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
#ifndef CALC_OBJ_MOV_STAT_THRESH_H
#define CALC_OBJ_MOV_STAT_THRESH_H

#include "f360_reuse.h"
#include "f360_timing_info.h"
#include "f360_host.h"

namespace f360_variant_A
{
	float32_t Calc_Obj_Mov_Stat_Thresh(float32_t const host_vcs_speed,
	   F360_TRKR_TIMING_INFO_T* const timing_info);
}

#endif
