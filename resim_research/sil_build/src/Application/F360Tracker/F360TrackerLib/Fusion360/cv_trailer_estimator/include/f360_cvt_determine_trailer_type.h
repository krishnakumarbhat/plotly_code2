#ifndef F360_CVT_DETERMINE_TRAILER_TYPE_H
#define F360_CVT_DETERMINE_TRAILER_TYPE_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_reuse.h"
#include "f360_cvt_types.h"

namespace f360_variant_A
{
   void Determine_Trailer_Type(
      const float32_t host_speed,
      const float32_t vcs_sideslip,
      const F360_CVT_Detection_Info_T(&detections)[CVT_MAX_NUMBER_OF_DETECTIONS],
      F360_CVT_State_T& cvt_state);
}
#endif
