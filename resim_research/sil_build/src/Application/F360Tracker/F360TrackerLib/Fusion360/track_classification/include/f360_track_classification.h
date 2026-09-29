/*===================================================================================*\
* FILE: f360_track_classification.h
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
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
*   This is the function prototype for the object classification module.
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/

#ifndef OBJECT_CLASSIFICATION_H
#define OBJECT_CLASSIFICATION_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_globals.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_timing_info.h"
#include "f360_calibrations.h"
#include "f360_radar_sensor.h"
#include "f360_host_props.h"
#include "f360_occlusion_types.h"
#include "ocg_occupancy_grid_types.h"
#include "f360_sg_interface.h"

namespace f360_variant_A
{
   void Track_Classification(
      const F360_Host_T & host,
      const F360_Host_Props_T& host_props,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const sg::SG_Output_T* const p_sg_output,
      const F360_Globals_T & globals,
      const rspp_variant_A::RSPP_Detection_List_T & dets_raw,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T & calib,
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
}

#endif
