/*=============================================================================================*\
* FILE: sg_cluster_detections.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for cluster_detections.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_CLUSTER_DETECTIONS_H
#define SG_CLUSTER_DETECTIONS_H

#include "sg_calibrations.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief    clusters detections in DetectionStorage.
    *
    * @param[in, out]           detections
    * @param[in]                calibrations
    **/
   void cluster_detections(DetectionStorage &detections, const Cluster_Detections_Calibrations_T &calibrations);
}
#endif