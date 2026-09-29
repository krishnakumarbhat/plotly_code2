/*===================================================================================*\
* FILE: f360_occlusion_ut_helpers.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declarations of additional helper functions for occlusion testing
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_OCCLUSION_UT_HELPERS_H
#define F360_OCCLUSION_UT_HELPERS_H

#include "f360_occlusion.h"

namespace f360_variant_A
{
   void Set_Base_Object_Parameters(
      F360_Object_Track_T& object);

   void Set_Left_Rear_Sensor(
      F360_Radar_Sensor_T& sensor);

   void Set_Left_Front_Sensor(
      F360_Radar_Sensor_T& sensor);

   void Set_Right_Rear_Sensor(
      F360_Radar_Sensor_T& sensor);

   void Set_Right_Front_Sensor(
      F360_Radar_Sensor_T& sensor);

   void Set_Common_Sensor_Parameters(
      F360_Radar_Sensor_T& sensor);
}

#endif
