#ifndef F360_SELECT_PRIORITIZED_DETECTIONS_H
#define F360_SELECT_PRIORITIZED_DETECTIONS_H
/*===================================================================================\
 * FILE: f360_select_prioritized_detections.h
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains function definition for Select_Prioritized_Detections.
 *
 * Applicable Standards (in order of precedence: highest first):
 *   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *   ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===================================================================================*/

#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_host.h"
#include "f360_look_ID.h"

namespace f360_variant_A
{
   void Select_Prioritized_Detections(
      const rspp_variant_A::RSPP_Detection_List_T& det_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );
}

#endif
