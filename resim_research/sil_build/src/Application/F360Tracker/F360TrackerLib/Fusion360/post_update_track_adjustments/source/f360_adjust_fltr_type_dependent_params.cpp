/*===================================================================================*\
* FILE:  f360_adjust_fltr_type_dependent_params.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Adjust_Fltr_Type_Dependet_Params() function implementation
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include <algorithm>
#include "f360_iterator.h"
#include "f360_math.h"
#include "f360_adjust_fltr_type_dependent_params.h"
#include "f360_get_wall_time.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ctca_states.h"

namespace f360_variant_A
{  
   static F360_Trk_Fltr_Type_T Determine_Fltr_Type(
      const F360_Object_Track_T &obj_track,
      const F360_Calibrations_T &calib
   );
   
   static CONF9_T Map_Overall_Conf_To_Speed_Conf(const CONF3_T conf_overall);

   /*===========================================================================*\
   * FUNCTION: Adjust_Fltr_Type_Dependet_Params()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T &tracker_Info,
   *  F360_Object_Track_T (&obj_tracks)[NUMBER_OF_OBJECT_TRACKS],
   *  const F360_Calibrations_T &calib,
   *  F360_TRKR_TIMING_INFO_T &timing_info
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
   * This function adjust all object params that are correlated with filtration type.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Adjust_Fltr_Type_Dependent_Params(
      const F360_Tracker_Info_T &tracker_Info,
      F360_Object_Track_T (&obj_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T &calib,
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      const float32_t start_time = get_wall_time();

      for (int32_t loop_index = 0; loop_index < tracker_Info.num_active_objs; loop_index++)
      {
         const int32_t obj_trk_idx = tracker_Info.active_obj_ids[loop_index] - 1;

         if (obj_trk_idx >= 0)
         {
            const F360_Trk_Fltr_Type_T new_fltr_type = Determine_Fltr_Type(obj_tracks[obj_trk_idx], calib);
            const F360_Trk_Fltr_Type_T old_fltr_type = obj_tracks[obj_trk_idx].trk_fltr_type;

            if (old_fltr_type != new_fltr_type)
            {
               // Switch from CCA to CTCA

               obj_tracks[obj_trk_idx].trk_fltr_type = new_fltr_type;

               float32_t new_obj_errcov[STATE_DIMENSION][STATE_DIMENSION] = {};

               // Set position cov info
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X];
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y];
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_X] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_X];
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y];

               // Set velocity and acceleration info
               float32_t temp_errcov[2][2] = { {obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX], obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY]}, {obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX], obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY]} };
               float32_t temp_errcov_rot[2][2];
               Rotate_2D_Covariance_Matrix(obj_tracks[obj_trk_idx].vcs_heading.Cos(), -obj_tracks[obj_trk_idx].vcs_heading.Sin(), temp_errcov, temp_errcov_rot);
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = temp_errcov_rot[0][0];

               temp_errcov[0][0] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX];
               temp_errcov[0][1] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY];
               temp_errcov[1][0] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX];
               temp_errcov[1][1] = obj_tracks[obj_trk_idx].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY];
               Rotate_2D_Covariance_Matrix(obj_tracks[obj_trk_idx].vcs_heading.Cos(), -obj_tracks[obj_trk_idx].vcs_heading.Sin(), temp_errcov, temp_errcov_rot);
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = temp_errcov_rot[0][0];

               // Set heading and curvature;
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = 0.01F;
               new_obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = 0.001F;

               (void)std::copy(cmn::begin(new_obj_errcov), cmn::end(new_obj_errcov), cmn::begin(obj_tracks[obj_trk_idx].errcov));
               
              
               obj_tracks[obj_trk_idx].conf_speed = Map_Overall_Conf_To_Speed_Conf(obj_tracks[obj_trk_idx].conf_overall);
            }

             // Adjust_Fltr_Type_CTCA_To_CCA is a Switch from CTCA to CCA but it is only called from object motion classification.
         }
      }

      timing_info.adjust_fltr_type_dependent_params = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Fltr_Type()
   *===========================================================================
   * RETURN VALUE:
   * F360_Trk_Fltr_Type_T
   *
   * PARAMETERS:
   *  const F360_Object_Track_T &obj_track,
   *  const F360_Calibrations_T &calib
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
   * This function determines filter type. We only allow CCA objects to
   * switch to CTCA objects and not vice versa.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * Caller to this function must be aware that we only allow switching from CCA to CTCA.
   * If this changes the calling functions must be modified to correctly handle state and
   * covariance modifications of the object in the case where it switches from CTCA to CCA model.
   *
   \*===========================================================================*/
   static F360_Trk_Fltr_Type_T Determine_Fltr_Type(
      const F360_Object_Track_T &obj_track,
      const F360_Calibrations_T &calib
   )
   {
      F360_Trk_Fltr_Type_T result = obj_track.trk_fltr_type; // As default don't chnage filter type

      const float32_t abs_speed = std::abs(obj_track.speed);
      const bool obj_is_moving_fast_enough_for_ctca = (obj_track.f_moving && (abs_speed > calib.fast_moving_thresh));

      if ((F360_TRACKER_TRKFLTR_CCA == obj_track.trk_fltr_type) && obj_is_moving_fast_enough_for_ctca) // In order to switch to CTCA from CCA the object must have some significant speed
      {
         const float32_t k_max_accepted_vel_error = 0.4F;
         const float32_t k_max_accepted_vel_error_safe = std::min(k_max_accepted_vel_error, calib.fast_moving_thresh); // [m/s]. The min function is used for code safety purposes to ensure that k_max_accepted_vel_error / abs_speed is <= 1 (such that asin is always well defined below)
         const float32_t k_max_accepted_heading_pnt_disagree = F360_Asinf(k_max_accepted_vel_error_safe / abs_speed); // Note: No risk of division with zero. We have checked speed is larger than zero before entering here. Also no risk of (k_max_accepted_vel_error_safe / abs_speed being outside) the [-1, 1] interval where asin is well difined becasue we have prevpusly made sure that k_max_accepted_vel_error_safe is always smaller than or equal to abs_speed

         // To swicth to CTCA from CCA some minimum time sine initialization must have passed and heading must have converged 
         // (i.e. curvature and heading pointing disagreement must be small).

         // Handling of the reversing case where heading and pointing are in opposite directions
         float32_t obj_hdg_ptng_disagmt = obj_track.hdg_ptng_disagmt;
         if (obj_hdg_ptng_disagmt > F360_PI_2)
         {
            obj_hdg_ptng_disagmt = obj_hdg_ptng_disagmt - F360_PI;
         }
         else if (obj_hdg_ptng_disagmt < -F360_PI_2)
         {
            obj_hdg_ptng_disagmt = obj_hdg_ptng_disagmt + F360_PI;
         }
         else
         {
            // Do nothing (for MISRA)
         }

         const float32_t k_cca_to_ctca_time_since_init_th = 2.0F;
         const float32_t k_cca_to_ctca_curvature_th = 0.018F;
         const bool f_switch_from_cca_to_ctca = ((obj_track.time_since_initialization > k_cca_to_ctca_time_since_init_th) &&
            (std::abs(obj_track.curvature) < k_cca_to_ctca_curvature_th) &&
            (std::abs(obj_hdg_ptng_disagmt) < k_max_accepted_heading_pnt_disagree));
      
         if (f_switch_from_cca_to_ctca)
         {
            result = F360_TRACKER_TRKFLTR_CTCA;
         }
      }

      return result;
   }
   /*===========================================================================*\
   * FUNCTION: Adjust_Fltr_Type_CTCA_To_CCA()
   *===========================================================================
   * RETURN VALUE:
   * Void
   *
   * PARAMETERS:
   *  F360_Object_Track_T &obj_track
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
   * This function switches filter type from CTCA to CCA
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * The user should be aware that CTCA is only allowed to be switched to CCA by
   * the moveable reset logic (See DEX-2890 for details)
   *
   \*===========================================================================*/
   void Adjust_Fltr_Type_CTCA_To_CCA(F360_Object_Track_T &obj_track)
   {
      // Switch from CTCA to CCA
      obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

      float32_t new_obj_errcov[STATE_DIMENSION][STATE_DIMENSION] = {};

      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] =
         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y] =
         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_X] =
         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_X];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] =
         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y];

      // error covariance matrix of heading and speed from CTCA filter type
      float32_t temp_errcov_hs[2][2] = {{obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H],
                                         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S]},
                                        {obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H],
                                         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S]}};

      // [vx, vy].T = [v*cos(theta), v*sin(theta)].T
      // calculate the jacobian matrix of the right hand side relative to theta and v
      const float32_t speed                = obj_track.speed;
      const Angle vcs_heading              = obj_track.vcs_heading;
      const float32_t jacobian_vx_vy[2][2] = {{-speed * vcs_heading.Sin(), vcs_heading.Cos()},
                                              {speed * vcs_heading.Cos(), vcs_heading.Sin()}};

      float32_t temp_mat_vx_vy[2][2] = {};
      float32_t temp_var_vx_vy[2][2] = {};
      F360_Matmul_MxN_NxP(jacobian_vx_vy, temp_errcov_hs, temp_mat_vx_vy);
      F360_Matmul_MxN_PxN_Transpose(temp_mat_vx_vy, jacobian_vx_vy, temp_var_vx_vy);

      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = temp_var_vx_vy[0][0];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = temp_var_vx_vy[0][1];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = temp_var_vx_vy[1][0];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = temp_var_vx_vy[1][1];

      // similarly for ax, ay
      const float32_t acc                  = obj_track.tang_accel;
      const float32_t jacobian_ax_ay[2][2] = {{-acc * vcs_heading.Sin(), vcs_heading.Cos()},
                                              {acc * vcs_heading.Cos(), vcs_heading.Sin()}};

      // error covariance matrix of heading and acceleration from CTCA filter type
      float32_t temp_errcov_ha[2][2] = {{obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H],
                                         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A]},
                                        {obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H],
                                         obj_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A]}};

      float32_t temp_mat_ax_ay[2][2] = {};
      float32_t temp_var_ax_ay[2][2] = {};
      F360_Matmul_MxN_NxP(jacobian_ax_ay, temp_errcov_ha, temp_mat_ax_ay);
      F360_Matmul_MxN_PxN_Transpose(temp_mat_ax_ay, jacobian_ax_ay, temp_var_ax_ay);
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = temp_var_ax_ay[0][0];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] = temp_var_ax_ay[0][1];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX] = temp_var_ax_ay[1][0];
      new_obj_errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = temp_var_ax_ay[1][1];

      (void) std::copy(cmn::begin(new_obj_errcov), cmn::end(new_obj_errcov), cmn::begin(obj_track.errcov));

      // Confidence mapping
      obj_track.conf_speed = Map_Overall_Conf_To_Speed_Conf(obj_track.conf_overall);

      // Assume no heading pointing disagreement for a stationary but moveable object.
   }

   /*===========================================================================*\
   * FUNCTION: Map_Overall_Conf_To_Speed_Conf()
   *===========================================================================
   * RETURN VALUE:
   * CONF9_T
   *
   * PARAMETERS:
   * const CONF3_T conf_overall
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
   * This function maps overall confidence to speed confidence.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static CONF9_T Map_Overall_Conf_To_Speed_Conf(const CONF3_T conf_overall)
   {
      CONF9_T speed_conf = CONF9_NONE;

      switch (conf_overall)
      {
         case CONF3_HIGH:
            speed_conf = CONF9_HIGH;
            break;
         case CONF3_MED:
            speed_conf = CONF9_MED4;
            break;
         case CONF3_LOW:
            speed_conf = CONF9_LOW4;
            break;
         case CONF3_NONE:
         default:
            speed_conf = CONF9_NONE;
            break;
      }

      return speed_conf;
   }
}
