/*=============================================================================================*\
* FILE: sg_measurement_update_utils.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions for measurement update contours function.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_measurement_update_contours.h"

#include <array>

#include "sg_length_measurement_update.h"
#include "sg_measurement_update_utils.h"
#include "sg_position_measurement_update.h"

namespace sg
{
   void measurement_update_contours(ContourStorage &contours,
                                    DetectionStorage &detections,
                                    const Measurement_Update_Calibrations_T &calibrations,
                                    const float min_segment_length)
   {
      uint16_t num_contours_to_length_measurement_update = 0U;
      std::array<ContourAssociatedDets, SG_MAX_NUM_CONTOURS> dets_and_contours_data_for_length_measurement{};

      position_measurement_update(detections, contours, dets_and_contours_data_for_length_measurement,
                                  num_contours_to_length_measurement_update, calibrations.position_forgetting_factor,
                                  calibrations.regularity_weight, calibrations.regularity_threshold);

      length_measurement_update(dets_and_contours_data_for_length_measurement, num_contours_to_length_measurement_update,
                                calibrations, min_segment_length);
   }
}
