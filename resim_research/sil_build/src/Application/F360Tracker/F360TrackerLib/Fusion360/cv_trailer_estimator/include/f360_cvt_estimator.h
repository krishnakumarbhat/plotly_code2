#ifndef F360_CVT_ESTIMATOR_H
#define F360_CVT_ESTIMATOR_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_cvt_types.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void CVT_Reset(
      F360_CVT_State_T& cvt_state
   );

   void CVT_Initialize(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Initialization_Data_T& init_data,
      F360_CVT_State_T& cvt_state
   );

   void CVT_Execute(
      const F360_Calibrations_T& calibrations,
      F360_CVT_Input_Data_T& cvt_input,
      F360_CVT_State_T& cvt_state
   );
}
#endif
