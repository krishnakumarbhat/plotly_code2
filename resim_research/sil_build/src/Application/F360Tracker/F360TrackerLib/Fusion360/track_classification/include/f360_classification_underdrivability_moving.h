#ifndef CLASSIFICATION_UNDERDRIVABILITY_MOVING_H
#define CLASSIFICATION_UNDERDRIVABILITY_MOVING_H

/*===================================================================================*\
 * FILE:  f360_classification_underdrivability_moving.cpp
 *====================================================================================
 * Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 * This file contains the implementation of Determine_Underdrivability_For_Movable().
 *
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 **/

#include "rspp_detection_list.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_timing_info.h"
#include "f360_host.h"

namespace f360_variant_A
{
   void Assign_Underdrivability_Status_To_Moving_Object(
      const F360_Calibrations_T & calib,
      const F360_Host_T &host,
      F360_Object_Track_T &object,
      F360_TRKR_TIMING_INFO_T& timing_info);
}

#endif
