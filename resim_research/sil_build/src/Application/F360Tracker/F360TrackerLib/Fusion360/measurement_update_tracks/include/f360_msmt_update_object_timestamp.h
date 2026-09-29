/*===================================================================================*\
* FILE: f360_msmt_update_object_timestamp.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains function definition of Msmt_Update_Object_Timestamp()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_MSMT_UPDATE_OBJECT_TIMESTAMP_H
#define F360_MSMT_UPDATE_OBJECT_TIMESTAMP_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Msmt_Update_Object_Timestamp(
      const float32_t elapsed_time_s,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T & object_track);

   void Msmt_Update_Object_Timestamp_Newest_Det(
      const float32_t elapsed_time_s,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object_track);

   void Find_Newest_Det_Timestamp(
      const F360_Object_Track_T& object_track,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      float32_t & timestamp_newest,
      bool& f_valid_det_found);
}

#endif
