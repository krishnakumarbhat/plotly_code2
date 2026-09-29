/*===========================================================================*\
* FILE: f360_identify_relevant_low_azimuth_confidence_dets.h
*============================================================================
* Copyright (C) 2019-2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of functions in f360_identify_relevant_low_azimuth_confidence_dets.cpp
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef IDENTIFY_RELEVANT_LOW_AZIMUTH_CONFIDENCE_DETS
#define IDENTIFY_RELEVANT_LOW_AZIMUTH_CONFIDENCE_DETS

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_radar_sensor_props.h"
#include "f360_host.h"
#include "f360_host_props.h"

namespace f360_variant_A
{
   void Handle_Low_Az_Conf_Detections(
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      F360_Detection_Props_T& det_prop);
}
#endif


