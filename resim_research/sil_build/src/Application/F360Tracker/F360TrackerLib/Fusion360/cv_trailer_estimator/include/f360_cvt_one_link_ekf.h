#ifndef F360_CVT_ONE_LINK_EKF_H
#define F360_CVT_ONE_LINK_EKF_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_reuse.h"
#include "f360_cvt_types.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void One_Trailer_EKF(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Input_Data_T& cvt_input,
      const float32_t angle_gate,
      F360_CVT_State_T& cvt_state,
      bool& f_updated);

   void Calc_Jacobian(
      const float32_t t,
      const float32_t host_speed,
      const float32_t host_yawrate,
      const float32_t(&x)[3],
      float32_t(&F)[3][3]);

   void KF_Time_Update(
      const F360_Calibrations_T& calibrations,
      const bool f_valid_msmt,
      const float32_t elapsed_time,
      const float32_t host_speed,
      const float32_t host_yawrate,
      float32_t(&x)[3],
      float32_t(&P)[3][3]);

   void KF_Measurement_Update(
      const float32_t angle_msmt,
      const float32_t intersect_msmt,
      float32_t(&x)[3],
      float32_t(&P)[3][3]);

   void Apply_State_Constraints(
      const F360_CVT_State_Constraints_T& state_constraints,
      float32_t(&KF_state)[3],
      float32_t(&KF_errcov)[3][3]);
}
#endif
