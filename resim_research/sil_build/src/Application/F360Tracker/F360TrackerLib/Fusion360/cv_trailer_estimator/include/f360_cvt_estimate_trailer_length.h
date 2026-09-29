#ifndef F360_CVT_ESTIMATE_TRAILER_LENGTH_H
#define F360_CVT_ESTIMATE_TRAILER_LENGTH_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_cvt_types.h"
#include "f360_reuse.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Run_Length_Filter(
      const F360_Calibrations_T& calibrations,
      const bool valid_measurement_ekf_1,
      const bool valid_measurement_ekf_2,
      F360_CVT_State_T& cvt_state);
}

#endif
