/*===================================================================================*\
* FILE: f360_handle_bistatic.h
*====================================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of function Handle_Bistatic_Dets.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards" [06-Sep-2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_HANDLE_BISTATIC_H
#define F360_HANDLE_BISTATIC_H

#include "f360_constants.h"
#include "f360_radar_sensor_props.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Handle_Bistatic_Dets(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Calibrations_T &calib,
      const F360_Radar_Sensor_Props_T &sensor_prop,
      F360_Detection_Props_T &detection_prop
   );
}
#endif

