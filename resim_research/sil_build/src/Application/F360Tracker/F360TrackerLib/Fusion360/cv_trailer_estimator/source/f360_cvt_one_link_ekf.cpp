/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include <cmath>
#include "f360_cvt_one_link_ekf.h"
#include "f360_math_func.h"
namespace f360_variant_A
{
  /*===========================================================================*\
   * FUNCTION: Calc_Jacobian
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t t,
   * const float32_t host_speed,
   * const float32_t host_yawrate,
   * const float32_t(&x)[3],
   * float32_t(&F)[3][3]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculate the jacobian used by the time-update step in EKF
   \*===========================================================================*/
   void Calc_Jacobian(
      const float32_t t,
      const float32_t host_speed,
      const float32_t host_yawrate,
      const float32_t(&x)[3],
      float32_t(&F)[3][3])
   {
      F[0][0] = 1.0F - t * ((host_speed * F360_Cosf(x[0])) / x[2] - (host_yawrate * x[1] * F360_Sinf(x[0])) / x[2]);
      F[0][1] = -(host_yawrate * t * F360_Cosf(x[0])) / x[2];
      F[0][2] = (t * (host_speed * F360_Sinf(x[0]) + host_yawrate * x[1] * F360_Cosf(x[0]))) / (x[2] * x[2]);
      F[1][0] = 0.0F;
      F[1][1] = 1.0F;
      F[1][2] = 0.0F;
      F[2][0] = 0.0F;
      F[2][1] = 0.0F;
      F[2][2] = 1.0F;
   }

  /*===========================================================================*\
   * FUNCTION: KF_Time_Update
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const bool f_valid_msmt,
   * const float32_t elapsed_time,
   * const float32_t host_speed,
   * const float32_t host_yawrate,
   * float32_t(&x)[3],
   * float32_t(&P)[3][3]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Execute the time update, or predition, step for the EKF
   \*===========================================================================*/
   void KF_Time_Update(
      const F360_Calibrations_T& calibrations,
      const bool f_valid_msmt,
      const float32_t elapsed_time,
      const float32_t host_speed,
      const float32_t host_yawrate,
      float32_t(&x)[3], // 0: trailer angle, 1: COR to joint, 2: 1/(joint2wheels)
      float32_t(&P)[3][3])
   {
      const float32_t k_stationary_threshold =  calibrations.k_cvt_slow_speed_threshold;

      if (fabsf(host_speed) > k_stationary_threshold)
      {
         const float32_t trailer_yawrate = (-host_speed * F360_Sinf(x[0]) / x[2]) - (x[1] * F360_Cosf(x[0]) / x[2] * host_yawrate);
         const float32_t trailer_angle_delta = (trailer_yawrate - host_yawrate) * elapsed_time;

         if (host_speed > k_stationary_threshold)
         {
            // update state vector
            x[0] += trailer_angle_delta;

            if (f_valid_msmt)
            {
               const float32_t Q_diag[3] = { 2.0F, 0.001F, 0.01F };

               // update errcov: FPF' + Q
               float32_t F[3][3] = {};
               float32_t temp[3][3] = {};

               Calc_Jacobian(elapsed_time, host_speed, host_yawrate, x, F);
               F360_Matmul_MxN_NxP(F, P, temp);
               F360_Matmul_MxN_PxN_Transpose(temp, F, P);

               P[0][0] += Q_diag[0];
               P[1][1] += Q_diag[1];
               P[2][2] += Q_diag[2];
            }
         }
         else if(host_speed < calibrations.k_cvt_reversing_speed_threshold) 
         {
            // reversing - do not update P, multiply trailer_angle_delta with "magic" gain
            x[0] *= 0.9F;
         }
         else
         {
            // stationary - do not update
         }
      }
      else
      {
         //Stationary host - do nothing
      }
   }


  /*===========================================================================*\
   * FUNCTION: KF_Measurement_Update
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t angle_msmt,
   * const float32_t intersect_msmt,
   * float32_t(&x)[3],
   * float32_t(&P)[3][3]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Execute the measurement update step for the EKF
   \*===========================================================================*/
   void KF_Measurement_Update(
      const float32_t angle_msmt,
      const float32_t intersect_msmt,
      float32_t(&x)[3],
      float32_t(&P)[3][3])
   {
      const float32_t R[2] = { 50.0F, 40.0F };

      // K = P*H' / (H*P*H' + R)
      float32_t K[3][2];
      K[0][0] = (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      K[0][1] = (P[0][1] * R[0]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      K[1][0] = (P[1][0] * R[1]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      K[1][1] = (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[1][1] * R[0]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      K[2][0] = (P[1][1] * P[2][0] - P[1][0] * P[2][1] + P[2][0] * R[1]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      K[2][1] = (P[0][0] * P[2][1] - P[0][1] * P[2][0] + P[2][1] * R[0]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);

      // Calculate state update: K * (z - H(x))
      const float32_t angle_innov = angle_msmt - x[0];
      const float32_t intersect_innov = intersect_msmt - x[1];
      x[0] += K[0][0] * angle_innov + K[0][1] * intersect_innov;
      x[1] += K[1][0] * angle_innov + K[1][1] * intersect_innov;
      x[2] += K[2][0] * angle_innov + K[2][1] * intersect_innov;

      // Calculate state error covariance update: (I - K*H)*P
      float32_t temp[3][3];
      temp[0][0] = (R[0] * (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1])) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[0][1] = (P[0][1] * R[0] * R[1]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[0][2] = (R[0] * (P[0][2] * P[1][1] - P[0][1] * P[1][2] + P[0][2] * R[1])) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[1][0] = (P[1][0] * R[0] * R[1]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[1][1] = (R[1] * (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[1][1] * R[0])) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[1][2] = (R[1] * (P[0][0] * P[1][2] - P[0][2] * P[1][0] + P[1][2] * R[0])) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[2][0] = (R[0] * (P[1][1] * P[2][0] - P[1][0] * P[2][1] + P[2][0] * R[1])) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[2][1] = (R[1] * (P[0][0] * P[2][1] - P[0][1] * P[2][0] + P[2][1] * R[0])) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);
      temp[2][2] = (P[0][0] * P[1][1] * P[2][2] - P[0][0] * P[1][2] * P[2][1] - P[0][1] * P[1][0] * P[2][2] + P[0][1] * P[1][2] * P[2][0] + P[0][2] * P[1][0] * P[2][1] - P[0][2] * P[1][1] * P[2][0] + P[0][0] * P[2][2] * R[1] - P[0][2] * P[2][0] * R[1] + P[1][1] * P[2][2] * R[0] - P[1][2] * P[2][1] * R[0] + P[2][2] * R[0] * R[1]) / (P[0][0] * P[1][1] - P[0][1] * P[1][0] + P[0][0] * R[1] + P[1][1] * R[0] + R[0] * R[1]);

      for (int32_t i = 0; i < 3; i++)
      {
         for (int32_t j = 0; j < 3; j++)
         {
            P[i][j] = temp[i][j];
         }
      }
   }

   /*===========================================================================*\
    * FUNCTION: Apply_State_Constraints
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_CVT_State_Constraints_T& state_constraints,
    * float32_t(&KF_state)[3],
    * float32_t(&KF_errcov)[3][3])
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * Constrain the state space of the EKF
   \*===========================================================================*/
   void Apply_State_Constraints(
      const F360_CVT_State_Constraints_T& state_constraints,
      float32_t(&KF_state)[3],
      float32_t(&KF_errcov)[3][3])
   {
      if ((KF_state[0] > state_constraints.max_x0) || (KF_state[0] < state_constraints.min_x0))
      {
         KF_state[0] = fminf(state_constraints.max_x0, fmaxf(state_constraints.min_x0, KF_state[0]));
         KF_errcov[0][0] = fminf(KF_errcov[0][0] * 1.1F, 0.5F);
      }

      if ((KF_state[1] > state_constraints.max_x1) || (KF_state[1] < state_constraints.min_x1))
      {
         KF_state[1] = fminf(state_constraints.max_x1, fmaxf(state_constraints.min_x1, KF_state[1]));
         KF_errcov[1][1] = fminf(KF_errcov[1][1] * 1.1F, 1.0F);
      }

      if ((KF_state[2] > state_constraints.max_x2) || (KF_state[2] < state_constraints.min_x2))
      {
         KF_state[2] = fminf(state_constraints.max_x2, fmaxf(state_constraints.min_x2, KF_state[2]));
         KF_errcov[2][2] = fminf(KF_errcov[2][2] * 1.1F, 1.0F);
      }
   }

   /*===========================================================================*\
    * FUNCTION: One_Trailer_EKF
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_CVT_Input_Data_T& cvt_input,
    * const float32_t angle_gate,
    * F360_CVT_State_T& cvt_state,
    * bool& f_updated
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * Execute the Extended Kalman Filter (EKF) for the one-trailer model
   \*===========================================================================*/
   void One_Trailer_EKF(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Input_Data_T& cvt_input,
      const float32_t angle_gate,
      F360_CVT_State_T& cvt_state,
      bool& f_updated)
   {
      const float32_t msmt_theta = cvt_state.primary_measurement.trailer_angle_vcs;
      const float32_t msmt_a1 = -(cvt_state.primary_measurement.trailer_intersect_vcs_long - cvt_input.host_rear_axle_vcs_longpos);

      // the minimum absolute angle required for both the measured line and the estimated trailer angle state for measurement update
      constexpr float32_t k_min_abs_angle = F360_DEG2RAD(4.5F);

      const bool f_msmt_confirmed = cvt_state.primary_measurement.f_msmt_valid &&
         ((fabsf(cvt_state.one_link.ekf_state[0]) > k_min_abs_angle) ||
         (fabsf(msmt_theta) > k_min_abs_angle)) &&
         ((cvt_state.one_link.ekf_state[0] + angle_gate) > msmt_theta) &&
         ((cvt_state.one_link.ekf_state[0] - angle_gate) < msmt_theta) &&
         (msmt_a1 < (cvt_state.state_constraints.max_x1 + 2.0F)) &&
         (msmt_a1 > (cvt_state.state_constraints.min_x1 - 2.0F));

      const float32_t elapsed_time = 0.05F;
      KF_Time_Update(calibrations, f_msmt_confirmed, elapsed_time, cvt_input.host_speed, cvt_input.host_yawrate,
         cvt_state.one_link.ekf_state, cvt_state.one_link.ekf_state_errcov);

      const bool f_reversing = (cvt_input.host_speed < calibrations.k_cvt_reversing_speed_threshold);
      if (f_msmt_confirmed && (!f_reversing))
      {
         KF_Measurement_Update(msmt_theta, msmt_a1, cvt_state.one_link.ekf_state, cvt_state.one_link.ekf_state_errcov);
      }

      Apply_State_Constraints(cvt_state.state_constraints, cvt_state.one_link.ekf_state, cvt_state.one_link.ekf_state_errcov);

      f_updated = f_msmt_confirmed && (!f_reversing);  // Regardless of the measurement, no measurement update will be done during reversing
   }
}
