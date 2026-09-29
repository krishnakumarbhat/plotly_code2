/*===========================================================================*\
* FILE: f360_inputs_preprocessing.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Inputs_Preprocessing function definition which calls functions that compute and
*   evaluate detections, host and sensors properties.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_inputs_preprocessing.h"
#include "f360_host_props_update.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Inputs_Preprocessing
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Core_Info_T& core_info,
   *  const F360_Host_T& host,
   *  F360_Host_Props_T& host_props,
   *  F360_TRKR_TIMING_INFO_T& timing_info
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Inputs_Preprocessing calls functions that compute and
   * evaluate detections, host and sensors properties.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Inputs_Preprocessing(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      F360_Host_Props_T& host_props,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();
      Host_Props_Update(core_info.elapsed_time_s, host, host_props);

      timing_info.inputs_preprocessing = get_wall_time() - start_time;
   }
}
