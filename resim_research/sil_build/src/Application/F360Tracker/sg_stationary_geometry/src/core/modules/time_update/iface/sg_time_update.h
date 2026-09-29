/*=============================================================================================*\
* FILE: sg_time_update.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration for time update function
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_TIME_UPDATE_H
#define SG_TIME_UPDATE_H

#include "rspp_host.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"
#include "sg_host_props.h"

namespace sg
{
   /**
    * @brief       Update detections and contours based on estimated host motion.
    *
    * @param[in]   elapsed_time
    * @param[in]   host
    * @param[in]   host_properties
    * @param[in]   calibrations
    * @param[in]   contours
    * @param[in, out]   detections
    *
    * @return      N/A
    **/
   void time_update(const float elapsed_time,
                    const RSPP_Host_T &host,
                    const HostProps &host_properties,
                    const Time_Update_Calibrations_T &calibrations,
                    const ContourStorage &contours,
                    DetectionStorage &detections);
}
#endif
