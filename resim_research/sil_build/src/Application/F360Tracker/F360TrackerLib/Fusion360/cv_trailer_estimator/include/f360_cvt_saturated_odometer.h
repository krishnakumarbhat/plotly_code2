#ifndef F360_CVT_SATURATED_ODOMETER_H
#define F360_CVT_SATURATED_ODOMETER_H
/******************************************************************************
* Copyright 2025 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_cvt_types.h"
#include "f360_calibrations.h"
namespace f360_variant_A
{

   void Update_Saturated_Odometer(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Input_Data_T& cvt_input,
      F360_CVT_State_T& cvt_state);

}
#endif
