/*===================================================================================*\
* FILE: ocg_process_detections.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Process_Detections() function implementation
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef OCG_PROCESS_DETECTIONS_H
#define OCG_PROCESS_DETECTIONS_H

#include "ocg_occupancy_grid_types.h"

namespace ocg
{
void Process_Detections(const OCG_Host_T &host, const OCG_Sensor_T &sensor, const OCG_Detection_List_T &detection_list,
                        OCG_Detection_Properties_T (&det_props)[OCG_NUM_DETS]);
}
#endif
