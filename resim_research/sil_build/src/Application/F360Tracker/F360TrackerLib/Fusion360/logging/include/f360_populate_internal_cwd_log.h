#ifndef F360_POPULATE_INTERNAL_CWD_LOG_H
#define F360_POPULATE_INTERNAL_CWD_LOG_H
/*===================================================================================*\
* FILE:  f360_populate_internal_cwd_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/

#include "f360_internal_cwd.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_cwd_data.h"
#include "T360_Types.h"

namespace f360_variant_A
{
   void Populate_Internal_CWD_Data(
      CWD_Data_T &cwd_data,
      const F360_Internal_CWD_T(&cwd_log)[MAX_NUMBER_OF_SENSORS]);

   void Populate_Internal_CWD_Log_Data(
      F360_Internal_CWD_T(&cwd_log)[MAX_NUMBER_OF_SENSORS],
      const CWD_Data_T& cwd_data);
}

#endif 
