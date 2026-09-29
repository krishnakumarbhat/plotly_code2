/*=============================================================================================*\
* FILE: sg_time_update_detections.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for time update detections functions
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_TIME_UPDATE_DETECTIONS_H
#define SG_TIME_UPDATE_DETECTIONS_H

#include "sg_calibrations.h"
#include "sg_detection_storage.h"
#include "sg_host_props.h"

namespace sg
{
   /**
    * @brief       Update detections position and covariance based on estimated host motion.
    *
    * @param[in]   elapsed_time
    * @param[in]   host_speed
    * @param[in]   host_properties
    * @param[in]   process_noise
    * @param[in, out]   detections
    *
    * @return      N/A
    **/
   void time_update_detections(const float elapsed_time,
                               const float host_speed,
                               const HostProps &host_properties,
                               const Time_Update_Calibrations_T::Process_Noise_T &process_noise,
                               DetectionStorage &detections);
}
#endif