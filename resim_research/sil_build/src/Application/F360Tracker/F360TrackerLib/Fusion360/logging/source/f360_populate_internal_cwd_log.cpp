/*===================================================================================*\
* FILE:  f360_populate_internal_cwd_log.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/
#include "f360_populate_internal_cwd_log.h"

namespace f360_variant_A
{
   void Populate_Internal_CWD_Data(
      CWD_Data_T& cwd_data,
      const F360_Internal_CWD_T(&cwd_log)[MAX_NUMBER_OF_SENSORS])
   {
      for (uint32_t sens_idx = 0U; sens_idx < MAX_NUMBER_OF_SENSORS; ++sens_idx)
      {
         cwd_data.buffer_index[sens_idx] = 0;
         for (int8_t sample_idx = 0; sample_idx < cwd_buffer_size; sample_idx++)
         {
            if (cwd_log[sens_idx].measurement_is_valid[sample_idx] > 0U)
            {
               cwd_data.circular_buffer[sens_idx][sample_idx] = cwd_log[sens_idx].measurement_lateral_position[sample_idx];
            }
         }
      }
   }

   void Populate_Internal_CWD_Log_Data(
      F360_Internal_CWD_T(&cwd_log)[MAX_NUMBER_OF_SENSORS],
      const CWD_Data_T& cwd_data)
   {
      for (uint32_t sens_idx = 0U; sens_idx < MAX_NUMBER_OF_SENSORS; sens_idx++)
      {
         cwd_log[sens_idx].mounting_loc = cwd_data.mount_loc[sens_idx];
         for (int8_t sample_idx = 0; sample_idx < cwd_buffer_size; sample_idx++)
         {
            const int8_t unrolled_idx = (cwd_data.buffer_index[sens_idx] + sample_idx) % cwd_buffer_size;
            cwd_log[sens_idx].measurement_lateral_position[sample_idx] = cwd_data.circular_buffer[sens_idx][unrolled_idx];
            cwd_log[sens_idx].measurement_is_valid[sample_idx] = (cwd_data.circular_buffer[sens_idx][unrolled_idx] > 0.0F) ? 1U : 0U;
         }
      }
   }
}
