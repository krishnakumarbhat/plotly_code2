/******************************************************************************
* Copyright 2025 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_cvt_two_link_ekf.h"
#include "f360_math_func.h"
#include "f360_LinearSolvers.h"

namespace f360_variant_A
{
   /*===========================================================================
    * FUNCTION: KF_Time_Update_2link
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Calibrations_T& calibrations
    * const bool f_reversing
    * const bool f_update_valid
    * const bool f_radar_left_side
    * const float32_t elapsed_time
    * const float32_t host_speed
    * const float32_t host_yawrate
    * float32_t(&ekf_state)[5]
    * float32_t(&ekf_state_errcov)[5][5]
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function executes the time update, i.e., predition step for the EKF
    *
    * PRECONDITIONS:
    *  To be called in EKF main function Two_Link_Trailer_EKF().
    *
    * POSTCONDITIONS:
    * None
    *
    *=========================================================================*/
   void KF_Time_Update_2link(
      const F360_Calibrations_T& calibrations,
      const bool f_reversing,
      const bool f_update_valid,
      const bool f_radar_left_side,
      const float32_t elapsed_time,
      const float32_t host_speed,
      const float32_t host_yawrate,
      float32_t(&ekf_state)[5],
      float32_t(&ekf_state_errcov)[5][5])
   {
      // Handle state
      const float32_t x1 = ekf_state[0];
      const float32_t x2 = ekf_state[1];
      const float32_t x3 = ekf_state[2];
      const float32_t x4 = ekf_state[3];
      const float32_t x5 = ekf_state[4];

      float32_t x5_scaled;
      if ((f_radar_left_side && (x4 < 0.0F)) ||
         ((!f_radar_left_side) && (x4 > 0.0F)))
      {
         x5_scaled = x5 * F360_Linear_Equation_With_Saturation(fabsf(x4), 0.0F, F360_DEG2RAD(90.0F), 1.0F, 0.7F);
      }
      else
      {
         x5_scaled = x5;
      }

      if (f_reversing)
      {
         ekf_state[0] = x1 * 0.9F;
         ekf_state[1] = x2;
         ekf_state[2] = x3;
         ekf_state[3] = x4 * 0.9F;
         ekf_state[4] = x5;

         // Leave errcov unchanged
      }
      else if (host_speed > calibrations.k_cvt_slow_speed_threshold)  // Only do time-update when moving forward
      {
         const float32_t t = elapsed_time;
         ekf_state[0] = x1 - t * (host_yawrate + (host_speed * F360_Sinf(x1)) / x3 + (host_yawrate * x2 * F360_Cosf(x1)) / x3);
         ekf_state[1] = x2;
         ekf_state[2] = x3;
         ekf_state[3] = x4 - t * (host_yawrate - (host_speed * F360_Sinf(x1 - x4) * F360_Cosf(x1)) / x5_scaled + (host_yawrate * x2 * F360_Sinf(x1 - x4) * F360_Sinf(x1)) / x5_scaled);
         ekf_state[4] = x5;

         // Only change the errcov if the update is valid
         if (f_update_valid)
         {
            // Handle state errcov
            float32_t F[5][5];
            (void)memset(&F[0][0], 0, sizeof(F));

            F[0][0] = 1.0F - t * ((host_speed * F360_Cosf(x1)) / x3 - (host_yawrate * x2 * F360_Sinf(x1)) / x3);
            F[0][1] = -(host_yawrate * t * F360_Cosf(x1)) / x3;
            F[0][2] = (t * (host_speed * F360_Sinf(x1) + host_yawrate * x2 * F360_Cosf(x1))) / (x3 * x3);
            F[1][1] = 1.0F;
            F[2][2] = 1.0F;
            F[3][0] = (t * (host_speed * F360_Cosf(2.0F * x1 - x4) - host_yawrate * x2 * F360_Sinf(2.0F * x1 - x4))) / x5;
            F[3][1] = -(host_yawrate * t * F360_Sinf(x1 - x4) * F360_Sinf(x1)) / x5;
            F[3][3] = 1.0F - t * ((host_speed * F360_Cosf(x1 - x4) * F360_Cosf(x1)) / x5 - (host_yawrate * x2 * F360_Cosf(x1 - x4) * F360_Sinf(x1)) / x5);
            F[3][4] = -(t * F360_Sinf(x1 - x4) * (host_speed * F360_Cosf(x1) - host_yawrate * x2 * F360_Sinf(x1))) / (x5 * x5);
            F[4][4] = 1.0F;

            //[1 - t * ((host_speed * cos(x1)) / x3 - (host_yawrate * x2 * sin(x1)) / x3), -(host_yawrate * t * cos(x1)) / x3, (t * (host_speed * sin(x1) + host_yawrate * x2 * cos(x1))) / x3 ^ 2, 0, 0]
            //[0, 1, 0, 0, 0]
            //[0, 0, 1, 0, 0]
            //[(t * (host_speed * cos(2 * x1 - x4) - host_yawrate * x2 * sin(2 * x1 - x4))) / x5, -(host_yawrate * t * sin(x1 - x4) * sin(x1)) / x5, 0, 1 - t * ((host_speed * cos(x1 - x4) * cos(x1)) / x5 - (host_yawrate * x2 * cos(x1 - x4) * sin(x1)) / x5), -(t * sin(x1 - x4) * (host_speed * cos(x1) - host_yawrate * x2 * sin(x1))) / x5 ^ 2]
            //[0, 0, 0, 0, 1]

            float32_t temp[5][5] = {};
            F360_Matmul_MxN_NxP(F, ekf_state_errcov, temp);
            F360_Matmul_MxN_PxN_Transpose(temp, F, ekf_state_errcov);

            const float32_t Q_diag[5] = { 0.002F, 0.00005F, 0.001F, 0.001F, 0.0005F };
            ekf_state_errcov[0][0] += Q_diag[0];
            ekf_state_errcov[1][1] += Q_diag[1];
            ekf_state_errcov[2][2] += Q_diag[2];
            ekf_state_errcov[3][3] += Q_diag[3];
            ekf_state_errcov[4][4] += Q_diag[4];
         }
      }
      else
      {
         // Do nothing, maintain previous state and errcov
      }

   }


   /*===========================================================================
    * FUNCTION: KF_Measurement_Update_2msmt
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const Trailer_Measurement_Info_T& measurement_link1,
    * const Trailer_Measurement_Info_T& measurement_link2,
    * const float32_t b1,
    * float32_t(&ekf_state)[5],
    * float32_t(&ekf_state_errcov)[5][5]
    * 
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function executes the measurement update step for the EKF with two line measurements
    *
    * PRECONDITIONS:
    *  To be called in EKF main function Two_Link_Trailer_EKF(), when there are two lines measured.
    *
    * POSTCONDITIONS:
    * None
    *
    *=========================================================================*/
   void KF_Measurement_Update_2msmt(
      const Trailer_Measurement_Info_T& measurement_link1,
      const Trailer_Measurement_Info_T& measurement_link2,
      const float32_t b1,
      float32_t(&ekf_state)[5],
      float32_t(&ekf_state_errcov)[5][5])
   {
      float32_t H[4][5];
      (void)memset(&H[0][0], 0, sizeof(H));
      const float32_t x1 = ekf_state[0];
      const float32_t x2 = ekf_state[1];
      const float32_t x3 = ekf_state[2];
      const float32_t x4 = ekf_state[3];

      H[0][0] = 1.0F;
      H[1][1] = -1.0F;
      H[2][3] = 1.0F;
      H[3][0] = (x3 * F360_Cosf(x1 - x4)) / F360_Sinf(x4);
      H[3][1] = -1.0F;
      H[3][2] = F360_Sinf(x1 - x4) / F360_Sinf(x4);
      H[3][3] = -(x3 * F360_Sinf(x1)) / (F360_Sinf(x4) * F360_Sinf(x4));

      const float32_t Hx[4] = {
         x1,
         (-b1 - x2),
         x4,
         -(b1 * F360_Sinf(x4) + x2 * F360_Sinf(x4) - x3 * F360_Sinf(x1 - x4)) / F360_Sinf(x4)
      };

      float32_t HT[5][4];
      F360_Transpose_2D(H, HT);

      float32_t P_HT[5][4];
      F360_Matmul_MxN_NxP(ekf_state_errcov, HT, P_HT);
      float32_t H_P_HT[4][4];
      F360_Matmul_MxN_NxP(H, P_HT, H_P_HT);

      const float32_t R_diag[4] = { 2.0F, 10.0F, 2.0F, 10.0F };
      for (int32_t i = 0; i < 4; i++)
      {
         H_P_HT[i][i] += R_diag[i];
      }

      float32_t H_PT[4][5];
      F360_Transpose_2D(P_HT, H_PT);
      // Kalman gain is K := P H'/(H*P_prior*H' + R)
      // Matrix Division does B'*inv(A). P H' = (H P')'
      float32_t K_kalman_gain[5][4] = {};
      Matrix_Division(H_P_HT, H_PT, 4U, 5U, K_kalman_gain);

      // State update 
      // y = z - Hx
      const float32_t y[4][1] = {
         { measurement_link1.trailer_angle_vcs - Hx[0] },
         { measurement_link1.trailer_intersect_vcs_long - Hx[1] },
         { measurement_link2.trailer_angle_vcs - Hx[2] },
         { measurement_link2.trailer_intersect_vcs_long - Hx[3] }
      };

      float32_t Ky[5][1];
      F360_Matmul_MxN_NxP(K_kalman_gain, y, Ky);
      for (int32_t i = 0; i < 5; i++)
      {
         ekf_state[i] += Ky[i][0];
      }

      // State errcov update (eye(5) - K * H) * P_prior;
      float32_t K_H[5][5];
      F360_Matmul_MxN_NxP(K_kalman_gain, H, K_H);
      float32_t I_KH[5][5] = {};
      for (int32_t i = 0; i < 5; i++)
      {
         I_KH[i][i] = 1.0F;
         for (int32_t j = 0; j < 5; j++)
         {
            I_KH[i][j] -= K_H[i][j];
         }
      }

      float32_t P_post[5][5];
      F360_Matmul_MxN_NxP(I_KH, ekf_state_errcov, P_post);
      (void)memcpy(ekf_state_errcov, &P_post[0][0], sizeof(ekf_state_errcov));
   }


   /*===========================================================================
    * FUNCTION: KF_Measurement_Update_1msmt
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const Trailer_Measurement_Info_T& measurement_link1,
    * const Trailer_Measurement_Info_T& measurement_link2,
    * const float32_t b1,
    * float32_t(&ekf_state)[5],
    * float32_t(&ekf_state_errcov)[5][5]
    * 
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function executes the measurement update step for the EKF with one line measurement
    *
    * PRECONDITIONS:
    *  To be called in EKF main function Two_Link_Trailer_EKF(), when there is only 1 line measured and confirmed.
    *
    * POSTCONDITIONS:
    * None
    *
    *=========================================================================*/
   void KF_Measurement_Update_1msmt(
      const Trailer_Measurement_Info_T& measurement,
      const float32_t b1,
      float32_t(&ekf_state)[5],
      float32_t(&ekf_state_errcov)[5][5])
   {
      float32_t H[2][5];
      (void)memset(&H[0][0], 0, sizeof(H));
      const float32_t x1 = ekf_state[0];
      const float32_t x2 = ekf_state[1];
      const float32_t x3 = ekf_state[2];
      const float32_t x4 = ekf_state[3];

      H[0][3] = 1.0F;
      H[1][0] = (x3 * F360_Cosf(x1 - x4)) / F360_Sinf(x4);
      H[1][1] = -1.0F;
      H[1][2] = F360_Sinf(x1 - x4) / F360_Sinf(x4);
      H[1][3] = -(x3 * F360_Sinf(x1)) / (F360_Sinf(x4) * F360_Sinf(x4));

      const float32_t Hx[2] = {
         x4,
        -(b1 * F360_Sinf(x4) + x2 * F360_Sinf(x4) - x3 * F360_Sinf(x1 - x4)) / F360_Sinf(x4)
      };

      float32_t HT[5][2];
      F360_Transpose_2D(H, HT);

      float32_t P_HT[5][2];
      F360_Matmul_MxN_NxP(ekf_state_errcov, HT, P_HT);
      float32_t H_P_HT[2][2];
      F360_Matmul_MxN_NxP(H, P_HT, H_P_HT);

      const float32_t R_diag[2] = { 2.0F, 10.0F };
      for (int32_t i = 0; i < 2; i++)
      {
         H_P_HT[i][i] += R_diag[i];
      }

      float32_t S_inv[2][2];
      (void)F360_matinv_2x2(H_P_HT, S_inv);

      float32_t K_kalman_gain[5][2] = {};
      F360_Matmul_MxN_NxP(P_HT, S_inv, K_kalman_gain);

      // State update 
      const float32_t y[2][1] = {
         { measurement.trailer_angle_vcs - Hx[0] },
         { measurement.trailer_intersect_vcs_long - Hx[1] }
      };
      float32_t Ky[5][1];
      F360_Matmul_MxN_NxP(K_kalman_gain, y, Ky);
      for (int32_t i = 0; i < 5; i++)
      {
         ekf_state[i] += Ky[i][0];
      }

      // State errcov update (eye(5) - K * H) * P_prior;
      float32_t K_H[5][5];
      F360_Matmul_MxN_NxP(K_kalman_gain, H, K_H);
      float32_t I_KH[5][5] = {};
      for (int32_t i = 0; i < 5; i++)
      {
         I_KH[i][i] = 1.0F;
         for (int32_t j = 0; j < 5; j++)
         {
            I_KH[i][j] -= K_H[i][j];
         }
      }

      float32_t P_post[5][5];
      F360_Matmul_MxN_NxP(I_KH, ekf_state_errcov, P_post);
      (void)memcpy(ekf_state_errcov, &P_post[0][0], sizeof(ekf_state_errcov));
   }


   /*===========================================================================
    * FUNCTION: Apply_State_Constraints
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_CVT_State_Constraints_T& state_constraints,
    * float32_t(&KF_state)[5],
    * float32_t(&KF_errcov)[5][5]
    * 
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function constrains the state space of the EKF
    *
    * PRECONDITIONS:
    *  To be called in EKF main function Two_Link_Trailer_EKF(), after the time-update and measurement update.
    *
    * POSTCONDITIONS:
    * None
    *
    *=========================================================================*/
   void Apply_State_Constraints(
      const F360_CVT_State_Constraints_T& state_constraints,
      float32_t(&KF_state)[5],
      float32_t(&KF_errcov)[5][5])
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

      if ((KF_state[3] > state_constraints.max_x3) || (KF_state[3] < state_constraints.min_x3))
      {
         KF_state[3] = fminf(state_constraints.max_x3, fmaxf(state_constraints.min_x3, KF_state[3]));
         KF_errcov[3][3] = fminf(KF_errcov[3][3] * 1.1F, 1.0F);
      }

      if ((KF_state[4] > state_constraints.max_x4) || (KF_state[4] < state_constraints.min_x4))
      {
         KF_state[4] = fminf(state_constraints.max_x4, fmaxf(state_constraints.min_x4, KF_state[4]));
         KF_errcov[4][4] = fminf(KF_errcov[4][4] * 1.1F, 1.0F);
      }
   }

   /*===========================================================================
    * FUNCTION: Two_Link_Trailer_EKF
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Calibrations_T& calibrations,
    * const F360_CVT_Input_Data_T& cvt_input,
    * const float32_t angle_gate,
    * F360_CVT_State_T& cvt_state,
    * bool& f_updated
    * 
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function executes the Extended Kalman Filter (EKF) for the two-trailer model
    *
    * PRECONDITIONS:
    *  To be called in CVT_Execute(), after generate_measurement() is done.
    *
    * POSTCONDITIONS:
    * None
    *
    *=========================================================================*/
   void Two_Link_Trailer_EKF(
      const F360_Calibrations_T& calibrations,
      const F360_CVT_Input_Data_T& cvt_input,
      const float32_t angle_gate,
      F360_CVT_State_T& cvt_state,
      bool& f_updated)
   {
      constexpr float32_t k_min_abs_angle_link1 = F360_DEG2RAD(2.0F);
      constexpr float32_t k_min_abs_angle_link2 = F360_DEG2RAD(5.0F);
      const float32_t intersect_max = 0.0F;
      const float32_t intersect_min = cvt_input.host_rear_axle_vcs_longpos - cvt_state.two_link.ekf_state[1] - cvt_state.two_link.ekf_state[2] - 5.0F;
      const float32_t elapsed_time = 0.05F;
      const bool f_reversing = cvt_input.host_speed < calibrations.k_cvt_reversing_speed_threshold;
      const bool f_radar_left_side = cvt_state.radar_vcs_latpos < 0.0F;

      bool f_msmt_confirmed = false;
      float32_t ekf_state[5];  // tmp copy of the EKF state
      float32_t ekf_state_errcov[5][5];  // tmp copy of the EKF state error covariance
      (void)memcpy(&ekf_state[0], cvt_state.two_link.ekf_state, sizeof(ekf_state));
      (void)memcpy(&ekf_state_errcov[0][0], cvt_state.two_link.ekf_state_errcov, sizeof(ekf_state_errcov));

      if (cvt_state.primary_measurement.f_msmt_valid && cvt_state.secondary_measurement.f_msmt_valid)
      {
         // Measurements valid from two trailer links
         bool f_state_ok;
         bool f_angle_msmt_ok;
         if (f_radar_left_side)
         {
            f_state_ok = (cvt_state.two_link.ekf_state[3] > k_min_abs_angle_link2) && (cvt_state.two_link.ekf_state[0] > k_min_abs_angle_link1);
            f_angle_msmt_ok = (cvt_state.primary_measurement.trailer_angle_vcs > k_min_abs_angle_link2) && (cvt_state.secondary_measurement.trailer_angle_vcs > k_min_abs_angle_link1);
         }
         else
         {
            f_state_ok = (cvt_state.two_link.ekf_state[3] < -k_min_abs_angle_link2) && (cvt_state.two_link.ekf_state[0] < -k_min_abs_angle_link1);
            f_angle_msmt_ok = (cvt_state.primary_measurement.trailer_angle_vcs < -k_min_abs_angle_link2) && (cvt_state.secondary_measurement.trailer_angle_vcs < -k_min_abs_angle_link1);
         }

         f_msmt_confirmed = f_state_ok && f_angle_msmt_ok &&
            (fabsf(cvt_state.primary_measurement.trailer_angle_vcs - cvt_state.two_link.ekf_state[3]) < angle_gate) &&
            (fabsf(cvt_state.secondary_measurement.trailer_angle_vcs - cvt_state.two_link.ekf_state[0]) < angle_gate) &&
            (cvt_state.primary_measurement.trailer_intersect_vcs_long < intersect_max) &&
            (cvt_state.primary_measurement.trailer_intersect_vcs_long > intersect_min) &&
            (cvt_state.secondary_measurement.trailer_intersect_vcs_long < intersect_max) &&
            (cvt_state.secondary_measurement.trailer_intersect_vcs_long > intersect_min);

         KF_Time_Update_2link(calibrations, f_reversing, f_msmt_confirmed, f_radar_left_side, elapsed_time, cvt_input.host_speed, cvt_input.host_yawrate, ekf_state, ekf_state_errcov);

         if (f_msmt_confirmed && (!f_reversing))
         {
            KF_Measurement_Update_2msmt(cvt_state.secondary_measurement, cvt_state.primary_measurement, -cvt_input.host_rear_axle_vcs_longpos, ekf_state, ekf_state_errcov);
         }
      }
      else if (cvt_state.primary_measurement.f_msmt_valid)
      {
         // Measurements valid from one trailer link, assumed to be the rearmost link
         bool f_state_ok;
         bool f_angle_msmt_ok;
         if (f_radar_left_side)
         {
            f_state_ok = (cvt_state.two_link.ekf_state[3] > k_min_abs_angle_link2) && (cvt_state.two_link.ekf_state[0] > k_min_abs_angle_link1);
            f_angle_msmt_ok = (cvt_state.primary_measurement.trailer_angle_vcs > k_min_abs_angle_link2);
         }
         else
         {
            f_state_ok = (cvt_state.two_link.ekf_state[3] < -k_min_abs_angle_link2) && (cvt_state.two_link.ekf_state[0] < -k_min_abs_angle_link1);
            f_angle_msmt_ok = (cvt_state.primary_measurement.trailer_angle_vcs < -k_min_abs_angle_link2);
         }

         f_msmt_confirmed = f_state_ok && f_angle_msmt_ok &&
            (fabsf(cvt_state.primary_measurement.trailer_angle_vcs - cvt_state.two_link.ekf_state[3]) < angle_gate) &&
            (cvt_state.primary_measurement.trailer_intersect_vcs_long < intersect_max) &&
            (cvt_state.primary_measurement.trailer_intersect_vcs_long > intersect_min);

         KF_Time_Update_2link(calibrations, f_reversing, f_msmt_confirmed, f_radar_left_side, elapsed_time, cvt_input.host_speed, cvt_input.host_yawrate, ekf_state, ekf_state_errcov);

         if (f_msmt_confirmed && (!f_reversing))
         {
            KF_Measurement_Update_1msmt(cvt_state.primary_measurement, -cvt_input.host_rear_axle_vcs_longpos, ekf_state, ekf_state_errcov);
         }
      }
      else
      {
         // No valid measurements
         KF_Time_Update_2link(calibrations, f_reversing, f_msmt_confirmed, f_radar_left_side, elapsed_time, cvt_input.host_speed, cvt_input.host_yawrate, ekf_state, ekf_state_errcov);
      }

      // Ensure constraints are repected
      Apply_State_Constraints(cvt_state.state_constraints, ekf_state, ekf_state_errcov);

      // Copy back state and errcov after the time-update and measurement-update steps
      (void)memcpy(&cvt_state.two_link.ekf_state[0], &ekf_state[0], sizeof(cvt_state.two_link.ekf_state));
      (void)memcpy(&cvt_state.two_link.ekf_state_errcov[0][0], &ekf_state_errcov[0][0], sizeof(cvt_state.two_link.ekf_state_errcov));
      f_updated = f_msmt_confirmed && (!f_reversing);
   }
}
