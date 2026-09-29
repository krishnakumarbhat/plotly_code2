/*=============================================================================================*\
* FILE: sg_time_update.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions for time update function
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_time_update.h"

#include "sg_time_update_contours.h"
#include "sg_time_update_detections.h"

namespace sg
{
   void time_update(const float elapsed_time,
                    const RSPP_Host_T &host,
                    const HostProps &host_properties,
                    const Time_Update_Calibrations_T &calibrations,
                    const ContourStorage &contours,
                    DetectionStorage &detections)
   {
      time_update_detections(elapsed_time, host.speed, host_properties, calibrations.noise, detections);
      time_update_contours(elapsed_time, host.speed, host_properties, calibrations.noise, contours);
   }
}