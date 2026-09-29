/*===================================================================================*\
* FILE: f360_common_object_output.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_COMMON_OBJECT_OUTPUT struct
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_COMMON_OBJECT_OUTPUT_H
#define F360_COMMON_OBJECT_OUTPUT_H

#include "f360_rot_object_log.h"
#include "f360_constants.h"
#include "f360_object_track.h"
#include "rspp_detection_list.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "f360_functional_safety_faults_log.h"

namespace f360_variant_A
{
    void Set_ROT_Object_Output(
       const F360_Host_T& host_info,
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       const rspp_variant_A::RSPP_Detection_List_T& det_list,
       const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
       ROT_Object_Output_T(&rot_obj_output)[NUMBER_OF_REDUCED_OBJECT_TRACKS]);

    void Set_ROT_Scl_Faults(const Functional_Safety_Faults_Log_T& functional_safety_faults_log,
       All_SCL_Faults_T& fault_status);
}

#endif
