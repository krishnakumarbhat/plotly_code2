#ifndef F360_ESTIMATE_VELOCITY_BY_POSITION_CHANGE_H
#define F360_ESTIMATE_VELOCITY_BY_POSITION_CHANGE_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_estimate_velocity_by_position_change.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Estimate_Velocity_By_Position_Change()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_reuse.h"
#include "f360_conf.h"
#include "f360_detection_hist.h"
#include "f360_cluster.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   CONF3_T Estimate_Velocity_By_Position_Change(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Cluster_T& cluster,
      float32_t& longvel_by_position,
      float32_t& latvel_by_position);
}

#endif
