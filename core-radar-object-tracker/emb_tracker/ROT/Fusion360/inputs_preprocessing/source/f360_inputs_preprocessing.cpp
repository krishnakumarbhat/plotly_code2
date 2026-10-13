/*===========================================================================*\
* FILE: f360_inputs_preprocessing.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Inputs_Preprocessing function definition which calls functions that compute and
*   evaluate detections, host and sensors properties.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_inputs_preprocessing.h"
#include "f360_clear_detections_props.h"
#include "f360_detection_props_update.h"
#include "f360_get_wall_time.h"
#include "f360_host_props_update.h"
#include "f360_sensor_props_update.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"

namespace f360_variant_A
{
void Inputs_Preprocessing(const F360_Core_Info_T &core_info, const F360_Host_T &host,
                          const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS], const F360_Detection_List_T &detect_list,
                          F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS],
                          F360_Detection_Props_List_T &detect_props_list, F360_Host_Props_T &host_props,
                          F360_TRKR_TIMING_INFO_T &timing_info)
{
   const float32_t start_time = get_wall_time();

   // Update host properties
   Host_Props_Update(core_info.elapsed_time_s, host, host_props);

   // Update sensor properties
   Sensor_Props_Update(core_info.time_us, sensors, sensors_props);

   // Update detection properties
   Clear_Detections_Props(detect_props_list.detection_props);
   Detection_Props_Update(host, sensors, sensors_props, detect_list, detect_props_list);

   // Sort detections in VCS longitudinal order and update detection list with sorting info
   Sort_Detections_Vcs_Long(detect_list.number_of_valid_detections, detect_props_list);

   timing_info.inputs_preprocessing = get_wall_time() - start_time;
}

} // namespace f360_variant_A
