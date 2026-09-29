/*===================================================================================*\
* FILE:  f360_time_update_object_track_CCA.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains funcions to implement the prediction step of the Kalman Filter for
* the Cartesian Constant Velocity motion model.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#include "f360_time_update_object_tracks_CCA.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_get_wall_time.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_iterator.h"
#include "f360_handle_spd_and_acc_when_stopping.h"
#include "f360_norm_heading_angle.h"

namespace f360_variant_A
{
   static void Update_P_Matrix_CCA(
      const float32_t T,
      const float32_t(&P_before)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&P_updated)[STATE_DIMENSION][STATE_DIMENSION]);

   static void Rotate_Q_From_TCS_To_VCS(
      const float32_t(&Q_TCS)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t cos_obj_pnt,
      const float32_t sin_obj_pnt,
      float32_t(&Q_VCS)[STATE_DIMENSION][STATE_DIMENSION]);

   static void Compute_Fast_Moving_CCA_Q(
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const float32_t T4,
      const float32_t T5,
      float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION]);

   /*===========================================================================*\
   * FUNCTION: Time_Update_Object_Tracks_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t elapsed_time_s,
   * const F360_Tracker_Info_T &tracker_info,
   * const F360_Calibrations_T & calibrations,
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_TRKR_TIMING_INFO_T & timing_info
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
   * This function performs a Kalman filter time update for CCA objects.
   * Between two tracker iterations host has possibly moved causing a translation and rotation of the VCS.
   * Time update therefore consists of two steps:
               1) KF Predict/time update the state vector in the old VCS without accounting for host motion 
               2) Account for host motion by transform the predicted states from the old VCS system to the new.
   * This function only considers the first step. The transformation of the state vector from the old VCS to the
   * new VCS is performed outside of this function.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   *
   \*===========================================================================*/
   void Time_Update_Object_Tracks_CCA(
      const float32_t T,
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T & calibrations,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();

      const float32_t T2 = T*T;
      const float32_t T3 = T2*T;
      const float32_t T4 = T3*T;
      const float32_t T5 = T4*T;

      float32_t default_low_speed_Q[STATE_DIMENSION][STATE_DIMENSION];
      Compute_Q_For_Slow_Moving_Object(calibrations, T, T2, T3, T4, T5, default_low_speed_Q);

      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
         F360_Object_Track_T& obj = object_tracks[obj_idx];
         
         if (obj.trk_fltr_type == F360_TRACKER_TRKFLTR_CCA)
         {
            // Store the position from previous tracker iteration
            obj.prev_vcs_center_pos.x = obj.bbox.Get_Center().x;
            obj.prev_vcs_center_pos.y = obj.bbox.Get_Center().y;
            obj.prev_predicted_vcs_y_pos = obj.predicted_vcs_position.y;

            // Time update of the object states
            const float32_t dx = T * obj.vcs_velocity.longitudinal + 0.5F * T2 * obj.vcs_accel.longitudinal;
            const float32_t dy = T * obj.vcs_velocity.lateral + 0.5F * T2 * obj.vcs_accel.lateral;
            obj.vcs_position.Translate(dx, dy);
            obj.bbox.Translate(dx, dy);

            const F360_VCS_Velocity_T prev_vel = obj.vcs_velocity;
            obj.vcs_velocity.longitudinal =
               obj.vcs_velocity.longitudinal +
               T * obj.vcs_accel.longitudinal;

            obj.vcs_velocity.lateral =
               obj.vcs_velocity.lateral +
               T * obj.vcs_accel.lateral;

            /* We are assuming constant acceleration so the states vcs_accel.longitudinal and vcs_accel.lateral 
              are unchanged by the time update */

            // Transform the cartesian representation of velocity into polar representaion (speed and heading)
            obj.speed = F360_Get_Hypotenuse(obj.vcs_velocity.longitudinal, obj.vcs_velocity.lateral);

            // Compute tangential acceleration for object
            obj.tang_accel = obj.vcs_accel.longitudinal * obj.vcs_heading.Cos() + obj.vcs_accel.lateral * obj.vcs_heading.Sin();

            if (obj.movable_prob > 0.5F)
            {
               // Handle when object stops from hard break
               Handle_Spd_And_Acc_When_Stopping_CCA(prev_vel, calibrations, obj);

               // Update object heading, bbox pointing, heading rate and curvature
               // The new heading is only calculated for moveable objects
               obj.vcs_heading = Angle{F360_Atan2f(obj.vcs_velocity.lateral, obj.vcs_velocity.longitudinal)}.Normalize(); // Heading corresponds to the direction of the velocity vector

               if (obj.f_moving && (obj.speed > calibrations.k_cca_min_speed_to_update_pnt)) // Note: For CCA the speed is always positive so we don't need to take abs
               {
                  Time_Update_Pointing_Heading_Rate_CCA(T, T2, T3, calibrations, obj);

                  // Compute object curvature
                  obj.curvature = obj.heading_rate / obj.speed;
               }
               else
               {
                  // BBox of stationary objects should not rotate. Therefore:
                  //    - Freeze object pointing (i.e. don't update).
                  //    - Set heading rate and curvature to 0
                  obj.heading_rate = 0.0F;
                  obj.curvature = 0.0F;
               }
            }
            else
            {
               // Update object heading and bbox pointing for non_moveable cca object
               obj.vcs_heading = Angle{ F360_Atan2f(obj.vcs_velocity.lateral, obj.vcs_velocity.longitudinal) }.Normalize(); // Heading corresponds to the direction of the velocity vector
               obj.Set_Bbox_Orientation(obj.vcs_heading);
               obj.heading_rate = 0.0F;
               obj.curvature = 0.0F;
            }

            // Time update of the object state covariance matrix
            float32_t Q_CCA[STATE_DIMENSION][STATE_DIMENSION];
            Compute_Process_Noise_Covariance_CCA(obj, calibrations, T, T2, T3, T4, T5, default_low_speed_Q, Q_CCA);

            float32_t P_before[STATE_DIMENSION][STATE_DIMENSION];
            (void)std::copy(cmn::begin(obj.errcov), cmn::end(obj.errcov), cmn::begin(P_before));
            Update_P_Matrix_CCA(T, P_before, Q_CCA, obj.errcov);
         }
      }

      timing_info.time_update_obj_trks_cca = get_wall_time() - start_time;
   }


   /*===========================================================================*\
   * FUNCTION: Time_Update_Pointing_Heading_Rate_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t T,
   * const F360_Calibrations_T& calibs,
   * F360_Object_Track_T& obj
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
   * This function performs a time update of the heading, pointing and heading rate of an CCA object.
   *
   * The heading is set to equal the direction of the object velocity vector.
   * The pointing and heading rate are updated through a KF filter that takes the object heading as a measurement.
   * The intention with this KF filter for pointing and heading rate is to prevent the bounding box of slow moving
   * CCA objects to rotate too fast around its own axis while still enabling to let object velocity vector change
   * unrestrictedly.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   *
   \*===========================================================================*/
   void Time_Update_Pointing_Heading_Rate_CCA(
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj)
   {
      // Extract object info
      const Angle prev_pointing = obj.bbox.Get_Orientation();
      const float32_t prev_heading_rate = obj.heading_rate;
      const float32_t prev_P[2][2] = { {obj.cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][1]}, {obj.cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][1]} };

      // Time update object pointing (orientation of bounding box/filtered direction of velocity vector) and heading rate. (Note: Constant heading rate is assumed so nothing to be done for heading rate)
      const Angle new_pointing = (prev_pointing + T * prev_heading_rate).Normalize();
      obj.bbox.Set_Orientation(new_pointing);
      obj.Update_Bbox_Center();

      // Compute new heading pointing disagreement angle
      obj.hdg_ptng_disagmt = (new_pointing - obj.vcs_heading).Normalize(0.0F).Value();

      // Time update of pointing and heading rate covariance matrix
      if (obj.cca_pnt_filter_cov[0][0] < calibs.init_cca_pnt_filter_cov[0][0])
      {
         // If object is coasting for a long time then heading covariance grows.
         // Limit this growth by preventing updating P matrix in time update if 
         // heading covariance grows too large.

         float32_t scale_factor;
         const float32_t k_speed_th_to_slow_down_cca_filter = 4.0F;
         if (std::abs(obj.speed) > k_speed_th_to_slow_down_cca_filter)
         {
            scale_factor = 1.0F;
         }
         else
         {
            // Slow down filter in slow speeds
            scale_factor = obj.speed / k_speed_th_to_slow_down_cca_filter;
         }

         scale_factor *= scale_factor * scale_factor * scale_factor;
         float32_t q_cca_heading_rate = 0.05F;

         if (std::abs(obj.heading_rate / obj.speed) > 0.03F)
         {
            q_cca_heading_rate = q_cca_heading_rate * 100.0F;
         }
         else
         {
            q_cca_heading_rate = q_cca_heading_rate;
         }

         q_cca_heading_rate *= scale_factor;
         
         const float32_t q_cca_pnt = 0.0F * scale_factor;
         const float32_t q[2][2] = { {0.333F * T3 * q_cca_heading_rate + T * q_cca_pnt, 0.5F * T2 * q_cca_heading_rate}, {0.5F * T2 * q_cca_heading_rate, T * q_cca_heading_rate} };

         const float32_t new_cross_cov = prev_P[0][1] + T * prev_P[1][1];
         obj.cca_pnt_filter_cov[0][0] = prev_P[0][0] + prev_P[0][1] * T + new_cross_cov * T + q[0][0];
         obj.cca_pnt_filter_cov[0][1] = new_cross_cov + q[0][1];
         obj.cca_pnt_filter_cov[1][0] = obj.cca_pnt_filter_cov[0][1];
         obj.cca_pnt_filter_cov[1][1] = prev_P[1][1] + q[1][1];
      }
   }


   /*===========================================================================*\
   * FUNCTION: Update_P_Matrix_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t T,
   * const float32_t(&P_before)[STATE_DIMENSION][STATE_DIMENSION],
   * const float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION],
   * float32_t(&P_updated)[STATE_DIMENSION][STATE_DIMENSION]
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
   * This function performs the error covariance update step of the KF time update.
   * I.e. it is executing the following matrix multiplication:
   *    P_updated = F*P_before*F' + Q
   * where
   * F corresponds to the transition model
   * P_before corresponds to the non-time updated error covariance matrix
   * Q corresponds to the process npise covariance matrix
   * P_updated corresponds to the time updated error covariance matrix
   * 
   * The function has been run time optimized where the matrix multiplications have been
   * analytically unwrapped offline and the unwrapped equations are implemented in the code.
   *
   * PRECONDITIONS:
   * The function assumes that the F matrix has the following structure:
   *    F = [1 T T^2/2 0 0 0
   *         0 1 T     0 0 0
   *         0 0 1     0 0 0
   *         0 0 0     1 T T^2/2
   *         0 0 0     0 1 T
   *         0 0 0     0 0 1]
   *
   * The function assumes that P_before and Q correspond to covariance matrices
   * (especially the symmetric propery of covariance matrices are utilized)
   *
   * POSTCONDITIONS:
   *
   \*===========================================================================*/
   static void Update_P_Matrix_CCA(
      const float32_t T,
      const float32_t(&P_before)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&P_updated)[STATE_DIMENSION][STATE_DIMENSION])
   {
      // Assuming state vector defined as [x, x_dot, x_ddot, y, y_dot, y_ddot]'

      const float32_t x0 = 0.5F*T*T;
      const float32_t x1 = P_before[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VX] + P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] * T + P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VX] * x0;
      const float32_t x2 = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] * x0;
      const float32_t x3 = P_before[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AX] + P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AX] * T + x2;
      const float32_t x4 = P_before[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VY] + P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] * T + P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY] * x0;
      const float32_t x5 = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] * x0;
      const float32_t x6 = P_before[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AY] + P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AY] * T + x5;
      const float32_t x7 = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VX] * T;
      const float32_t x8 = P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] + x7;
      const float32_t x9 = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] * T;
      const float32_t x10 = P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AX] + x9;
      const float32_t x11 = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY] * T;
      const float32_t x12 = P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] + x11;
      const float32_t x13 = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] * T;
      const float32_t x14 = P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AY] + x13;
      const float32_t x15 = P_before[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VY] + P_before[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] * T + P_before[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_VY] * x0;
      const float32_t x16 = P_before[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] * x0;
      const float32_t x17 = P_before[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AY] + P_before[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AY] * T + x16;
      const float32_t x18 = P_before[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_VY] * T;
      const float32_t x19 = P_before[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] + x18;
      const float32_t x20 = P_before[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] * T;
      const float32_t x21 = P_before[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AY] + x20;

      P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] = P_before[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] + P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_X] * T + P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_X] * x0 + Q[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] + T*x1 + x0*x3;
      P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VX] = Q[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VX] + T*x3 + x1;
      P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AX] = Q[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AX] + x3;
      P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y] = P_before[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y] + P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_Y] * T + P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y] * x0 + Q[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y] + T*x4 + x0*x6;
      P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VY] = Q[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VY] + T*x6 + x4;
      P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AY] = Q[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AY] + x6;
      
      P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_X] = P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VX];
      P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = Q[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] + T*x10 + x8;
      P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AX] = Q[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AX] + x10;
      P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_Y] = P_before[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_Y] + P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y] * T + Q[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_Y] + T*x12 + x0*x14;
      P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = Q[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] + T*x14 + x12;
      P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AY] = Q[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AY] + x14;
      
      P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_X] = P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AX];
      P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VX] = P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AX];
      P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] + Q[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX];
      P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y] = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y] + Q[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y] + x11 + x5;
      P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY] = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY] + Q[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY] + x13;
      P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] = P_before[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] + Q[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY];
      
      P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_X] = P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y];
      P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VX] = P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_Y];
      P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AX] = P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y];
      P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] = P_before[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] + P_before[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_Y] * T + P_before[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_Y] * x0 + Q[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] + T*x15 + x0*x17;
      P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VY] = Q[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VY] + T*x17 + x15;
      P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AY] = Q[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AY] + x17;
      
      P_updated[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_X] = P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VY];
      P_updated[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY];
      P_updated[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AX] = P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY];
      P_updated[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_Y] = P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VY];
      P_updated[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = Q[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] + T*x21 + x19;
      P_updated[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AY] = Q[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AY] + x21;
      
      P_updated[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_X] = P_updated[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AY];
      P_updated[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_VX] = P_updated[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AY];
      P_updated[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX] = P_updated[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY];
      P_updated[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_Y] = P_updated[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AY];
      P_updated[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_VY] = P_updated[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AY];
      P_updated[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = P_before[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] + Q[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY];
   }


   /*===========================================================================*\
   * FUNCTION: Compute_Process_Noise_Covariance_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj,
   * const F360_Calibrations_T& calibs,
   * const float32_t T,
   * const float32_t T2,
   * const float32_t T3,
   * const float32_t T4,
   * const float32_t T5,
   * float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION]
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
   * This function computes the KF process noise covaraince matrix (Q).
   * The tuning is done in object TCS to enable objects to have a faster para-responce
   * than orth reponse. The initial TCS tuning is then rotated into VCS.
   * 
   * The Q matrix equations are obtained from discretizing a time continous model with 
   * assumed indepent proces noise on all states.
   *
   * PRECONDITIONS:
   * 
   * POSTCONDITIONS:
   *
   \*===========================================================================*/
   void Compute_Process_Noise_Covariance_CCA(
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const float32_t T4,
      const float32_t T5,
      const float32_t(&default_low_speed_Q)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION])
   {

      if ((std::abs(obj.speed) < calibs.k_cca_low_speed_th_to_ramp_down_proceess_noise))
      {
         (void)std::copy(cmn::begin(default_low_speed_Q), cmn::end(default_low_speed_Q), cmn::begin(Q));
      }
      else
      {
         Compute_Fast_Moving_CCA_Q(obj, calibs, T, T2, T3, T4, T5, Q);
      }
   }

   /*===========================================================================*\
    * FUNCTION: Compute_Q_For_Slow_Moving_Old_Object()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Calibrations_T& calibs,
    * const float32_t T,
    * const float32_t T2,
    * const float32_t T3,
    * const float32_t T4,
    * const float32_t T5,
    * float32_t(&default_low_speed_Q)[STATE_DIMENSION][STATE_DIMENSION]
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
    * This function computes the process noise covariance matrix which is used for slow moving
    * (speed below calibs.k_cca_low_speed_th_to_ramp_down_proceess_noise) CCA objects
    * 
    * PRECONDITIONS:
    *
    * POSTCONDITIONS:
    *
   \*===========================================================================*/
   void Compute_Q_For_Slow_Moving_Object(
      const F360_Calibrations_T& calibs,
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const float32_t T4,
      const float32_t T5,
      float32_t(&default_low_speed_Q)[STATE_DIMENSION][STATE_DIMENSION])
   {
      const float32_t q_pos = calibs.q_cca_acc_low_speed * (0.050F) * T5 + calibs.q_cca_vel_low_speed * (0.333F) * T3 + calibs.q_cca_pos_low_speed * T;
      const float32_t q_pos_vel = calibs.q_cca_acc_low_speed * (0.125F) * T4 + calibs.q_cca_vel_low_speed * (0.500F) * T2;
      const float32_t q_pos_acc = calibs.q_cca_acc_low_speed * (0.167F) * T3;
      const float32_t q_vel = calibs.q_cca_acc_low_speed * (0.333F) * T3 + calibs.q_cca_vel_low_speed * T;
      const float32_t q_vel_acc = calibs.q_cca_acc_low_speed * (0.500F) * T2;
      const float32_t q_acc = calibs.q_cca_acc_low_speed * T;

      const float32_t low_speed_Q_tmp[STATE_DIMENSION][STATE_DIMENSION] = { {q_pos, q_pos_vel, q_pos_acc, 0.0F, 0.0F, 0.0F},
         {q_pos_vel, q_vel, q_vel_acc, 0.0F, 0.0F, 0.0F},
         {q_pos_acc, q_vel_acc, q_acc, 0.0F, 0.0F, 0.0F},
         {0.0F, 0.0F, 0.0F, q_pos, q_pos_vel, q_pos_acc},
         {0.0F, 0.0F, 0.0F, q_pos_vel, q_vel, q_vel_acc},
         {0.0F, 0.0F, 0.0F, q_pos_acc, q_vel_acc, q_acc} };

      (void)std::copy(cmn::begin(low_speed_Q_tmp), cmn::end(low_speed_Q_tmp), cmn::begin(default_low_speed_Q));
   }

   /*===========================================================================*\
    * FUNCTION: Rotate_Q_From_TCS_To_VCS()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const float32_t(&Q_TCS)[STATE_DIMENSION][STATE_DIMENSION],
    * const float32_t cos_obj_pnt,
    * const float32_t sin_obj_pnt,
    * float32_t(&Q_VCS)[STATE_DIMENSION][STATE_DIMENSION]
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
    * This function computes the rotation of the process noice covariance from TCS coordinate
    * system into VCS coordinte system for CCA model.
    * I.e the function computes Q_VCS = R * Q_TCS * R' where
    *    R = [cos(obj_pnt)      0             0       -sin(obj_pnt)       0             0;
                   0       cos(obj_pnt)       0             0       -sin(obj_pnt)       0;
                   0       0             cos(obj_pnt)       0             0       -sin(obj_pnt);
              sin(obj_pnt)      0             0        cos(obj_pnt)       0             0;
                   0       sin(obj_pnt)       0             0        cos(obj_pnt)       0;
                   0       0             sin(obj_pnt)       0             0       cos(obj_pnt)].
    *
    * The function is implemented with run time optimization in mind where the known structure
    * of the R matrix (which contains a lot of zeroes) is utilized
    *
    * PRECONDITIONS:
    * The rotatioin matrix R from TCS to VCS has the structure given from the above description.
    *
    * POSTCONDITIONS:
    *
   \*===========================================================================*/
   static void Rotate_Q_From_TCS_To_VCS(
      const float32_t(&Q_TCS)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t cos_obj_pnt,
      const float32_t sin_obj_pnt,
      float32_t(&Q_VCS)[STATE_DIMENSION][STATE_DIMENSION])
   {

      // Pre multiply with rotation matrix
      float32_t temp[STATE_DIMENSION][STATE_DIMENSION];
      for (uint32_t row_idx1 = 0U; row_idx1 < 3U; row_idx1++)
      {
         for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
         {
            const uint32_t row_idx2 = row_idx1 + 3U;
            temp[row_idx1][col_idx] = cos_obj_pnt * Q_TCS[row_idx1][col_idx] - sin_obj_pnt * Q_TCS[row_idx2][col_idx];
            temp[row_idx2][col_idx] = sin_obj_pnt * Q_TCS[row_idx1][col_idx] + cos_obj_pnt * Q_TCS[row_idx2][col_idx];
         }
      }

      // Post multiply with transpose of rotation matrix
      for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
      {
         for (uint32_t col_idx1 = 0U; col_idx1 < 3U; col_idx1++)
         {
            const uint32_t col_idx2 = col_idx1 + 3U;
            Q_VCS[row_idx][col_idx1] = temp[row_idx][col_idx1] * cos_obj_pnt - temp[row_idx][col_idx2] * sin_obj_pnt;
            Q_VCS[row_idx][col_idx2] = temp[row_idx][col_idx1] * sin_obj_pnt + temp[row_idx][col_idx2] * cos_obj_pnt;
         }
      }
   }

   /*===========================================================================*\
 * FUNCTION: Compute_Fast_Moving_CCA_Q()
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const F360_Object_Track_T& obj,
 * const F360_Calibrations_T& calibs,
 * const float32_t T,
 * const float32_t T2,
 * const float32_t T3,
 * const float32_t T4,
 * const float32_t T5,
 * float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION]
 *
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * This function computes the process noise covariance matrix whitch is used for fast
 * moving (speed above calibs.k_cca_low_speed_th_to_ramp_down_proceess_noise) CCA objects.
 *
 * PRECONDITIONS:
 *
 * POSTCONDITIONS:
 *
\*===========================================================================*/

   static void Compute_Fast_Moving_CCA_Q(
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const float32_t T4,
      const float32_t T5,
      float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION])
   {
      const float32_t q_cca_pos_para_high_speed = 2.0F;
      const float32_t q_cca_vel_para_high_speed = 2.0F;
      const float32_t q_cca_acc_para_high_speed = 20.0F;
      const float32_t q_cca_pos_orth_high_speed = 2.0F;
      const float32_t q_cca_vel_orth_high_speed = 2.0F;
      const float32_t q_cca_acc_orth_high_speed = 0.6F;


      // Ramp down noise levels for relatively slow moving objects
      float32_t scale_factor = std::min(1.0F, std::max(0.0F, (obj.speed - calibs.k_cca_low_speed_th_to_ramp_down_proceess_noise) / (calibs.k_cca_high_speed_th_to_ramp_down_proceess_noise - calibs.k_cca_low_speed_th_to_ramp_down_proceess_noise))); // Note for CCA object speed is always positive so we don't need to take abs
      scale_factor = scale_factor * scale_factor;
      const float32_t q_cca_pos_para = calibs.q_cca_pos_low_speed + scale_factor * (q_cca_pos_para_high_speed - calibs.q_cca_pos_low_speed);
      const float32_t q_cca_vel_para = calibs.q_cca_vel_low_speed + scale_factor * (q_cca_vel_para_high_speed - calibs.q_cca_vel_low_speed);
      const float32_t q_cca_acc_para_low_acc = calibs.q_cca_acc_low_speed + scale_factor * (q_cca_acc_para_high_speed - calibs.q_cca_acc_low_speed);
      const float32_t q_cca_pos_orth = calibs.q_cca_pos_low_speed + scale_factor * (q_cca_pos_orth_high_speed - calibs.q_cca_pos_low_speed);
      const float32_t q_cca_vel_orth = calibs.q_cca_vel_low_speed + scale_factor * (q_cca_vel_orth_high_speed - calibs.q_cca_vel_low_speed);
      const float32_t q_cca_acc_orth_low_acc = calibs.q_cca_acc_low_speed + scale_factor * (q_cca_acc_orth_high_speed - calibs.q_cca_acc_low_speed);

      // In addition to the speed dependency the tuning for acceleration is also dependent on the acceleration.
      // For large accelerations the q noise is ramped up
      const float32_t small_acc_th = 0.5F;
      const float32_t large_acc_th = 2.0F;
      const float32_t q_para_acc_large_acc = 50.0F;
      const float32_t q_orth_acc_large_acc = 50.0F;
      const float32_t cos_hdg = obj.vcs_heading.Cos();
      const float32_t sin_hdg = obj.vcs_heading.Sin();
      const float32_t abs_para_acc = fabsf(cos_hdg * obj.vcs_accel.longitudinal + sin_hdg * obj.vcs_accel.lateral);
      const float32_t abs_orth_acc = fabsf(-sin_hdg * obj.vcs_accel.longitudinal + cos_hdg * obj.vcs_accel.lateral);
      const float32_t q_cca_acc_para = F360_Linear_Equation_With_Saturation(abs_para_acc, small_acc_th, large_acc_th, q_cca_acc_para_low_acc, q_para_acc_large_acc);
      const float32_t q_cca_acc_orth = F360_Linear_Equation_With_Saturation(abs_orth_acc, small_acc_th, large_acc_th, q_cca_acc_orth_low_acc, q_orth_acc_large_acc);

      // Setup process noise covariance matrix in TCS
      /*
      * using the following formulas:
      * 
      * q_pos     = q_cca_acc * (0.050F) * T5 + q_cca_vel * (0.333F) * T3 + q_cca_pos * T
      * q_pos_vel = q_cca_acc * (0.125F) * T4 + q_cca_vel * (0.500F) * T2
      * q_pos_vel = q_cca_acc * (0.167F) * T3
      * 
      * q_vel     = q_cca_acc * (0.333F) * T3 + q_cca_vel * T
      * q_vel_acc = q_cca_acc * (0.500F) * T2
      * q_acc     = q_cca_acc * T
      */

      const float32_t Q_CCA_para_orth[STATE_DIMENSION][STATE_DIMENSION] = {
         {q_cca_acc_para * (0.050F) * T5 + q_cca_vel_para * (0.333F) * T3 +
            q_cca_pos_para * T,
            q_cca_acc_para * (0.125F) * T4 + q_cca_vel_para * (0.500F) * T2,
            q_cca_acc_para * (0.167F) * T3, 0.0F, 0.0F, 0.0F},
         {q_cca_acc_para * (0.125F) * T4 + q_cca_vel_para * (0.500F) * T2,
            q_cca_acc_para * (0.333F) * T3 + q_cca_vel_para * T,
            q_cca_acc_para * (0.500F) * T2, 0.0F, 0.0F, 0.0F},
         {q_cca_acc_para * (0.167F) * T3, q_cca_acc_para * (0.500F) * T2,
            q_cca_acc_para * T, 0.0F, 0.0F, 0.0F},
         {0.0F, 0.0F, 0.0F,
            q_cca_acc_orth * (0.050F) * T5 + q_cca_vel_orth * (0.333F) * T3 +
            q_cca_pos_orth * T,
            q_cca_acc_orth * (0.125F) * T4 + q_cca_vel_orth * (0.5F) * T2,
            q_cca_acc_orth * (0.167F) * T3},
         {0.0F, 0.0F, 0.0F,
            q_cca_acc_orth * (0.125F) * T4 + q_cca_vel_orth * (0.500F) * T2,
            q_cca_acc_orth * (0.333F) * T3 + q_cca_vel_orth * T,
            q_cca_acc_orth * (0.500F) * T2},
         {0.0F, 0.0F, 0.0F, q_cca_acc_orth * (0.167F) * T3,
            q_cca_acc_orth * (0.500F) * T2, q_cca_acc_orth * T} };

      Rotate_Q_From_TCS_To_VCS(Q_CCA_para_orth, obj.bbox.Get_Orientation().Cos(), obj.bbox.Get_Orientation().Sin(), Q);
   }
}
