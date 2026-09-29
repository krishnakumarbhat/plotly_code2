/*=============================================================================================*\
* FILE: sg_downselect_input_detections.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains an algorithm for downselection input detections.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_DOWNSELECT_INPUT_DETECTIONS_H
#define SG_DOWNSELECT_INPUT_DETECTIONS_H

#include <bitset>

#include "sg_calibrations.h"
#include "sg_constants.h"
#include "sg_input.h"

namespace sg
{
   /**
    * @brief          Downselect input detections based on chosen criteria.
    *
    * @param[in,out]  nondrivable_detections_mask
    * @param[in,out]  underdrivable_detections_mask
    * @param[in]      calibrations
    * @param[in]      view_ranges
    * @param[in]      input_detections
    * @param[in]      host_speed
    **/
   void downselect_input_detections(std::bitset<SG_MAX_NUM_INPUT_DETS> &nondrivable_detections_mask,
                                    std::bitset<SG_MAX_NUM_INPUT_DETS> &underdrivable_detections_mask,
                                    const Downselect_Input_Detections_Calibrations_T &calibrations,
                                    const View_Ranges_T &view_ranges,
                                    const SG_Input_Detections_T &input_detections,
                                    const float &host_speed);
}

#endif
