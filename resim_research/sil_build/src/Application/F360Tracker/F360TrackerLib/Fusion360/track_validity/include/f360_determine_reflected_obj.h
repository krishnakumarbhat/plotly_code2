/*===================================================================================*\
* FILE: f360_determine_reflected_obj.h
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This is the main function for the vehicle processing module.
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
#ifndef F360_DETERMINE_REFLECTED_OBJ_H
#define F360_DETERMINE_REFLECTED_OBJ_H

#include "f360_reuse.h"
#include "f360_tracker_info.h"
#include "f360_host.h"
#include "f360_static_env_poly_types.h"
#include "f360_calibrations.h"
#include "f360_radar_sensor.h"
#include "f360_trailer_detector_flt_fus_output.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   void Determine_Reflected_Obj(
      const F360_Tracker_Info_T & tracker_info,
      const int32_t obj_idx,
      const F360_Host_T & host,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T & calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Trailer_Estimator_Output_T& trailer_data,
      const bool f_marked_as_multipath,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   );
}
#endif
