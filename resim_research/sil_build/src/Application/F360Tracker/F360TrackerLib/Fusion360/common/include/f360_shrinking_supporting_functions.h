/******************************************************************************
* Copyright 2025 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE:  f360_shrinking_supporting_functions.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declarations of:
* Downselect_Detections()
* Determine_Positions_In_Set_Of_Dets_To_Clear()
* Count_Unique_Tsm_And_Update_Num_Of_Dets_For_Each()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/
#ifndef F360_SHRINKING_SUPPORTING_FUNCTIONS_H
#define F360_SHRINKING_SUPPORTING_FUNCTIONS_H

#include "f360_object_track.h"
#include "f360_host.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   float32_t Get_CIPV_Long_Pos(
     const F360_Tracker_Info_T& tracker_info,
     const F360_Host_T& host,
     const F360_Calibrations_T& calib);

   bool Determine_CIPV_Status(
      const F360_Object_Track_T& object,
      const float32_t cipv_long_pos,
      const F360_Calibrations_T& calib);

   bool In_Special_Zone_For_No_Shrinking(
       const Point& obj_vcs_pos,
       const F360_Tracker_Variant_T& tracker_variant_type);
}
#endif
