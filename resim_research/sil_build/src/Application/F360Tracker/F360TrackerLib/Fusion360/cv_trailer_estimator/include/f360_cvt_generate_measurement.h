#ifndef F360_CVT_GENERATE_MEASUREMENT_H
#define F360_CVT_GENERATE_MEASUREMENT_H
/******************************************************************************
 * Copyright 2024 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include "f360_reuse.h"
#include "f360_cvt_types.h"

namespace f360_variant_A
{
   float32_t Calc_Range_Rate_Gate(
      const float32_t host_yaw_rate,
      const bool f_reversing);

   void Find_Valid_Detections(
      const F360_CVT_Input_Data_T& cvt_input,
      const F360_CVT_State_T& cvt_state,
      const float32_t range_rate_gate,
      int32_t(&valid_det_idx)[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER],
      int32_t& counter_valid_det);

   void Find_Approximate_Lines(
      const F360_CVT_Detection_Info_T(&detections)[CVT_MAX_NUMBER_OF_DETECTIONS],
      const float32_t radar_vcs_latpos,
      const int32_t n_valid_dets,
      const int32_t(&valid_det_idxs)[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER],
      Trailer_Measurement_Info_T& primary_measurement,
      Trailer_Measurement_Info_T& secondary_measurement);

   void Generate_Measurement(
      const F360_CVT_Input_Data_T& cvt_input,
      F360_CVT_State_T& cvt_state);
}
#endif
