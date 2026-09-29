/*=============================================================================================*\
* FILE: sg_length_measurement_update.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration for length_measurement_update function.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_LENGTH_MEASUREMENT_UPDATE_H
#define SG_LENGTH_MEASUREMENT_UPDATE_H

#include "sg_calibrations.h"
#include "sg_measurement_update_utils.h"

namespace sg
{
   /**
    * @brief   Function updates only first and last segment in contour.
    *
    * @param [in]       dets_and_contours_data_for_length_measurement
    * @param [in]       num_contours
    * @param [in]       calibrations
    * @param [in]       min_segment_length
    *
    * @return  N/A
    **/
   void length_measurement_update(const std::array<sg::ContourAssociatedDets, SG_MAX_NUM_CONTOURS> &dets_and_contours_data_for_length_measurement,
                                  const uint16_t num_contours,
                                  const Measurement_Update_Calibrations_T &calibrations,
                                  const float min_segment_length);
}
#endif
