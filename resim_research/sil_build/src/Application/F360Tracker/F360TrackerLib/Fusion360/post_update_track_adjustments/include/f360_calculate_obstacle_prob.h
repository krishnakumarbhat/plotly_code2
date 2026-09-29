#ifndef F360_CALCULATE_OBSTACLE_PROB
#define F360_CALCULATE_OBSTACLE_PROB
/*===================================================================================*\
* FILE: f360_calculate_obstacle_prob.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declarations of the following functions.
*   Calculate_Properties_For_Obstacle_Prob()
*   Calculate_Obstacle_Prob()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_host.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
    void Calculate_Properties_For_Obstacle_Prob(
        const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
        const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
        const F360_Calibrations_T& calib,
        F360_Object_Track_T& obj
    );

    void Calculate_And_Filter_Obstacle_Prob(
        const F360_Host_T& host,
        F360_Object_Track_T& obj
    );

    float32_t Calculate_Instantaneous_Obstacle_Prob(
        const F360_Object_Track_T& obj,
        const float32_t& range
    );
}
#endif
