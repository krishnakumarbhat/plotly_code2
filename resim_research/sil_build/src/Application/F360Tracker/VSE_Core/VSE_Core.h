#ifndef VSE_CORE_H
#define VSE_CORE_H

/*=========================================================================
*  FILE: VSE_Core.h
*=========================================================================
* Copyright © 2020 Aptiv. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
*
*  DESCRIPTION:
*    This file contains VSE_CORE wrapper class declaration.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

/*=========================================================================
*------------------------------------------------------------------------------
*
* class:        VSE_CORE
*
* Description:  Wrapper for the Core VSE
*
* Deviations from standards: None
*
*========================================================================*/

#include "VSE_Master_Model_L2.h"
#include "VSEInternalDataLog.h"
#include "f360_host_calib.h"
#include "f360_host_raw.h"
#include "vse_utilities.h"
#include "stdint.h"

namespace vse_core
{
   class VSE_CORE
   {
   public:
      VSE_CORE();
      ~VSE_CORE();

      void Initialize(const f360_variant_A::F360_Host_Calib_T& r_host_calib);
      void Step(const uint64_t timestamp_us, const float speed_correction_factor, const f360_variant_A::F360_Host_Raw_T& r_host_raw);

      VSE_OUT Get_VSE_Output(void);
      VSE_OUT Get_VSE_Output(const uint64_t timestamp_us);

      void Log_VSE_Output(VSE_Output_Log_T& r_VSE_Output_log);
      void Log_Internals(VSE_Internal_Data_Log_T& r_VSE_internals_log);

      void Init_State_From_Log(const VSE_Internal_Data_Log_T& r_VSE_internals);
      void Update_VSE_Buffer(const VSE_OUT& r_VSE_Output);

   private:
      uint64_t raw_host_signal_latency_us;
      uint32_t vehicle_index;
      VSE_Master_Model_L2ModelClass BMW_VSE;
      VSE_buffer vse_output_buffer;
   };
}

#endif /* VSE_CORE_H */
