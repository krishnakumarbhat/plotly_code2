/*===================================================================================*\
* FILE: f360_determine_association_hypothesis.h
*====================================================================================
* Copyright (C) 2020 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Determine_Association_Hypothesis() declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_DETERMINE_ASSOCIATION_HYPOTHESIS_H
#define F360_DETERMINE_ASSOCIATION_HYPOTHESIS_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "rspp_detection.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_detection_association_support_functions.h"

namespace f360_variant_A
{
   void Determine_Association_Hypothesis(
      const F360_Calibrations_T & calibrations,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t host_vcs_speed,
      const uint32_t (&det_idx_array)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t num_dets_in_array,
      const int32_t obj_idx,
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      float32_t (&det_score_array)[MAX_NUMBER_OF_DETECTIONS],
      float32_t (&det_rdot_comp_array)[MAX_NUMBER_OF_DETECTIONS]
   );

}
#endif
