/*=============================================================================================*\
* FILE: sg_combine_detections.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains an algorithm for combining input and old detections.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_COMBINE_DETECTIONS_H
#define SG_COMBINE_DETECTIONS_H

#include <bitset>

#include "sg_calibrations.h"
#include "sg_constants.h"
#include "sg_detection_storage.h"
#include "sg_host_props.h"
#include "sg_input.h"

namespace sg
{ /**
   * @brief             Function to combine internal and input detections.
   *
   * @param[in,out]     detections - internal detections storage
   * @param[in]         importance_calibrations - set of importance calibrations used in this step
   * @param[in]         common_calibrations - set of calibrations used in multiple processing steps
   * @param[in]         host - host data
   * @param[in]         sensors - sensors data
   * @param[in]         input_detections - list of input detections
   * @param[in]         nondrivable_detections_mask - mask of nondrivable detections
   * @param[in]         underdrivable_detections_mask - mask of underdrivable detections
   **/
   void combine_detections(DetectionStorage &detections,
                           const Importance_Calibrations_T &importance_calibrations,
                           const Common_Calibrations_T &common_calibrations,
                           const RSPP_Host_T &host,
                           const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                           const SG_Input_Detections_T &input_detections,
                           const std::bitset<SG_MAX_NUM_INPUT_DETS> &nondrivable_detections_mask,
                           const std::bitset<SG_MAX_NUM_INPUT_DETS> &underdrivable_detections_mask);
}

#endif