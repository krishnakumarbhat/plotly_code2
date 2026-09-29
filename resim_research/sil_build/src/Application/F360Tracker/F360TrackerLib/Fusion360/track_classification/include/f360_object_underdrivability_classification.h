#ifndef OBJECT_UNDERDRIVABILITY_CLASSIFICATION_H
#define OBJECT_UNDERDRIVABILITY_CLASSIFICATION_H

/*===================================================================================*\
 * FILE:  f360_object_underdrivability_classification.h
 *====================================================================================
 * Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 * This file contains the declaration of Object_Underdriviability_Classification().
 *
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 **/

#include "f360_tracker_info.h"
#include "ocg_occupancy_grid_types.h"
#include "f360_sg_interface.h"
#include "f360_host.h"
#include "f360_host_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Object_Underdrivability_Classification(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_Props_T& host_props,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const sg::SG_Output_T* const p_sg_output,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info);
}

#endif
