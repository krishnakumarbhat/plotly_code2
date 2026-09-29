/*===================================================================================*\
* FILE: sg_input.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG input datatypes.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_INPUT_H
#define SG_INPUT_H

#include "f360_log_types.h"
#include "rspp_detection_list.h"
#include "rspp_host.h"
#include "rspp_radar_sensor.h"
#include "sg_rot_rspp_chooser.h"


namespace sg
{
   using SG_Input_Detections_T = rspp::RSPP_Detection_List_T;

   struct SG_Input_T
   {
      const uint64_t timestamp_us;
      const rot::F360_Detection_Log_Output_T &rot_detections;
      const rspp::RSPP_Detection_List_T &rspp_detections;
      const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS];
      const RSPP_Host_T &host;

      SG_Input_T() = delete;
   };
}

#endif
