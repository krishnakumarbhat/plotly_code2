/*===========================================================================*\
* FILE: f360_determine_dets_on_edge_of_fov.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains declarations of functions Mark_Detection_On_Edge_Of_FOV() and Is_Detection_In_FOV_Limit()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_DETERMINE_DETS_ON_EDGE_OF_FOV_H
#define F360_DETERMINE_DETS_ON_EDGE_OF_FOV_H

#include "f360_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_radar_sensor_props.h"
#include "f360_reuse.h"
namespace f360_variant_A
{
void Mark_Detection_On_Edge_Of_FOV(const F360_Detection_T &detection, const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
                                   const F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS],
                                   F360_Detection_Props_T &detection_prop);

bool Is_Detection_In_FOV_Limit(const float32_t (&interior_fov)[F360_DET_NUM_LOOK_ID], const F360_Det_Look_ID_T look_id,
                               const float32_t detection_az);
} // namespace f360_variant_A
#endif
