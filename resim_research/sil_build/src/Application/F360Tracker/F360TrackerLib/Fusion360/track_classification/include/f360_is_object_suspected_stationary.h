/*===================================================================================*\
* FILE:  f360_is_object_suspected_stationary.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of Is_Object_Suspected_Stationary() function.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef CHECK_IF_OBJECT_IS_SUSPECTED_STATIONARY_H
#define CHECK_IF_OBJECT_IS_SUSPECTED_STATIONARY_H

#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_host.h"
#include "f360_calibrations.h"
#include "f360_occlusion_types.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   bool Is_Object_Suspected_Stationary(
      F360_Object_Track_T& object,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS]);
}

#endif
