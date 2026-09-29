#ifndef F360_CVT_TWO_LINK_EKF_H
#define F360_CVT_TWO_LINK_EKF_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_reuse.h"
#include "f360_cvt_types.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void KF_Time_Update_2link(
      const F360_Calibrations_T& calibrations,
      const bool f_reversing,
      const bool f_update_valid,
      const bool f_radar_left_side,
      const float32_t elapsed_time,
      const float32_t host_speed,
      const float32_t host_yawrate,
      float32_t(&ekf_state)[5],
      float32_t(&ekf_state_errcov)[5][5]);

   void KF_Measurement_Update_2msmt(
      const Trailer_Measurement_Info_T& measurement_link1,
      const Trailer_Measurement_Info_T& measurement_link2,
      const float32_t b1,
      float32_t(&ekf_state)[5],
      float32_t(&ekf_state_errcov)[5][5]);

   void KF_Measurement_Update_1msmt(
      const Trailer_Measurement_Info_T& measurement,
      const float32_t b1,
      float32_t(&ekf_state)[5],
      float32_t(&ekf_state_errcov)[5][5]);

   void Apply_State_Constraints(
      const F360_CVT_State_Constraints_T& state_constraints,
      float32_t(&KF_state)[5],
      float32_t(&KF_errcov)[5][5]);

   void Two_Link_Trailer_EKF(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Input_Data_T& cvt_input,
      const float32_t angle_gate,
      F360_CVT_State_T& cvt_state,
      bool& f_updated);
}
#endif
