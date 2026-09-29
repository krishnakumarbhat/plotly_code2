#ifndef VSE_INTERNAL_DATA_LOG_T_H
#define VSE_INTERNAL_DATA_LOG_T_H
/*===================================================================================*\
 * FILE: VSEInternalDataLog.h
 *====================================================================================
 * Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*==========================================================================================*/

#include "stdint.h"
#include "VSEOutputLog.h"

/*===========================================================================*\
 * Exported local (file scope) Constants
\*===========================================================================*/

#define VSE_INTERNAL_DATA_LOG_STREAM_NUM     16
#define VSE_INTERNAL_DATA_LOG_STREAM_VERSION  1
#define VSE_OUTPUT_BUFFER_LENGTH              5

#ifndef __TASKING__
#  pragma pack(push)
#  pragma pack(4)
#endif

/*===========================================================================*\
 * Exported Type Declarations
\*===========================================================================*/
typedef struct VSE_RESIM_Internals_Tag
{
  float yaw_rate_bias1;
  float yaw_rate_bias2;
  float yaw_rate_bias_fast_bias1;
  float yaw_rate_bias_fast_bias2;
  float comp_yaw_rate_diff_filt;
  float yaw_rate_bias_diff;
  float ignition_time;
  float CurvKalmanFilterC0;
  float CurvKalmanFilterC1;
  int32_t road_type;
  uint8_t f_yaw_rate_bias_converged;
  uint8_t f_stop_bias_converged;
  uint8_t f_yaw_rate_bias_shift;
  uint8_t f_yaw_rate_steady;
  uint8_t f_input_invalid_persistent;
  uint8_t f_execution_period_error_persistent;
  uint8_t f_bias_was_accurate;
  uint8_t reserved;
} VSE_RESIM_Internals_T;

typedef struct VSE_Internals_Data_Log_Tag
{
   VSE_Output_Log_T VSE_output_buffer[VSE_OUTPUT_BUFFER_LENGTH];
   VSE_RESIM_Internals_T VSE_internals;
   uint32_t k_host_signal_latency_ms;
   uint32_t buffer_size;
} VSE_Internal_Data_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************
LOGSIZE_ASSERT(VSE_Internal_Data_Log_T, 696U);
LOGSIZE_ASSERT(VSE_Output_Log_T, 128U);
LOGSIZE_ASSERT(VSE_RESIM_Internals_T, 48U);

#ifndef __TASKING__
#  pragma pack(pop)
#else
#endif

#endif
