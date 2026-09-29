/*=============================================================================================*\
* FILE: sg_position_measurement_update.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration for position_measurement_update function.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_POSITION_MEASUREMENT_UPDATE_H
#define SG_POSITION_MEASUREMENT_UPDATE_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"
#include "sg_measurement_update_utils.h"

namespace sg
{
   /**
    * @brief   Perform the position measurement update.
    *
    * @param [in]       detections
    * @param [in,out]   contours
    * @param [in,out]   dets_and_contours_data_for_length_measurement
    * @param [in,out]   num_contours_to_length_measurement_update
    * @param [in]       position_forgetting_factor
    *
    * @return  N/A
    **/
   void position_measurement_update(DetectionStorage &detections,
                                    ContourStorage &contours,
                                    std::array<ContourAssociatedDets, SG_MAX_NUM_CONTOURS> &dets_and_contours_data_for_length_measurement,
                                    uint16_t &num_contours_to_length_measurement_update,
                                    const float position_forgetting_factor,
                                    const float stiffness,
                                    const float regularity_threshold);

}
#endif