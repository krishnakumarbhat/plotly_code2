#ifndef F360_CVT_RUNNER_H
#define F360_CVT_RUNNER_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/

#include "f360_calibrations.h"
#include "f360_constants.h"
#include "f360_host.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "f360_cvt_estimator.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Run_CV_Trailer_Estimator(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_CVT_State_T& cvt_state,
      F360_TRKR_TIMING_INFO_T& timing_info);

}
#endif
