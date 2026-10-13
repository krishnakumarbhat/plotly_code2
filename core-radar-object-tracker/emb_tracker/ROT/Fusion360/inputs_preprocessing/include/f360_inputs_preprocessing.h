/*===========================================================================*\
* FILE: inputs_preprocessing.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Inputs_Preprocessing function declaration.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef INPUTS_PREPROCESSING_H
#define INPUTS_PREPROCESSING_H

#include "f360_constants.h"
#include "f360_core_info.h"
#include "f360_detection_list.h"
#include "f360_detection_props_list.h"
#include "f360_host.h"
#include "f360_host_props.h"
#include "f360_radar_sensor.h"
#include "f360_radar_sensor_props.h"
#include "f360_reuse.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  Inputs_Preprocessing
 *   This function performs preprocessing of tracker inputs including host and sensor properties update.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   core_info       - Struct containing core tracker information such as elapsed time.
 *   host            - Struct containing host information such as position and velocity.
 *   sensors         - Array of structs containing sensor information.
 *   detect_list       - Struct containing raw detection list to be updated.
 *   sensors_props   - Array of structs containing sensor properties to be updated.
 *   detect_props_list - Struct containing detection properties list to be updated.
 *   host_props      - Struct containing host properties to be updated.
 *   timing_info     - Struct containing timing information to be updated.
 *
 * Return Value: None
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
void Inputs_Preprocessing(const F360_Core_Info_T &core_info, const F360_Host_T &host,
                          const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS], const F360_Detection_List_T &detect_list,
                          F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS],
                          F360_Detection_Props_List_T &detect_props_list, F360_Host_Props_T &host_props,
                          F360_TRKR_TIMING_INFO_T &timing_info);

} // namespace f360_variant_A

#endif // F360_INPUTS_PREPROCESSING_H
