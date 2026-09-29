/*===========================================================================*\
* FILE: f360_convert_object_prev_vcs_to_current_vcs.cpp
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function definition of Convert_Object_Prev_Vcs_To_Current_Vcs().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006
***/

#include "f360_convert_object_prev_vcs_to_current_vcs.h"
#include "f360_math_func.h"
#include "f360_norm_heading_angle.h"
#include "f360_trk_fltr_cca_states.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Convert_Object_Prev_Vcs_To_Current_Vcs()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info
   * const F360_Host_Props_T & host_p
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
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
   * Function transforms objects properties and covariance matrix from previous 
   * VCS position and orientation, VCS_(t-1), to current VCS position and orientation, 
   * VCS_(t0).
   * See DFD-285 for deriviation of this transform function.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Convert_Object_Prev_Vcs_To_Current_Vcs(
      const F360_Tracker_Info_T & tracker_info,
      const F360_Host_Props_T & host_p,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      for (uint32_t i = 0U; i < static_cast<uint32_t>(tracker_info.num_active_objs); i++)
      {
         const uint32_t active_obj_idx = static_cast<uint32_t>(tracker_info.active_obj_ids[i]) - 1U;

         Convert_Object_Properties(
            host_p,
            object_tracks[active_obj_idx]);

         Convert_Object_Covariance(
            host_p,
            object_tracks[active_obj_idx]);

         // Convert pseudo heading state vector
         float32_t new_state_vec[6];

         new_state_vec[0] = object_tracks[active_obj_idx].pseudo_hdg_state_vec[0];

         new_state_vec[1] = host_p.cos_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x) +
            host_p.sin_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_y);

         new_state_vec[2] = -host_p.sin_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x) +
            host_p.cos_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_y);

         new_state_vec[3] = host_p.cos_delta_pointing * host_p.cos_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[3] - 2.0F * object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] * host_p.delta_position_x + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x * host_p.delta_position_x) +
            host_p.sin_delta_pointing * host_p.sin_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[4] - 2.0F * object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] * host_p.delta_position_y + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_y * host_p.delta_position_y) +
            2.0F * host_p.cos_delta_pointing * host_p.sin_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[5] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] * host_p.delta_position_x - object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] * host_p.delta_position_y + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x * host_p.delta_position_y);

         new_state_vec[4] = host_p.sin_delta_pointing * host_p.sin_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[3] - 2.0F * object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] * host_p.delta_position_x + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x * host_p.delta_position_x) +
            host_p.cos_delta_pointing * host_p.cos_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[4] - 2.0F * object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] * host_p.delta_position_y + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_y * host_p.delta_position_y) -
            2.0F * host_p.cos_delta_pointing * host_p.sin_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[5] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] * host_p.delta_position_x - object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] * host_p.delta_position_y + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x * host_p.delta_position_y);

         new_state_vec[5] = host_p.cos_delta_pointing * host_p.cos_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[5] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] * host_p.delta_position_x - object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] * host_p.delta_position_y + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x * host_p.delta_position_y) -
            host_p.sin_delta_pointing * host_p.sin_delta_pointing * (object_tracks[active_obj_idx].pseudo_hdg_state_vec[5] - object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] * host_p.delta_position_x - object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] * host_p.delta_position_y + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x * host_p.delta_position_y) +
            host_p.cos_delta_pointing * host_p.sin_delta_pointing * (-object_tracks[active_obj_idx].pseudo_hdg_state_vec[3] + 2.0F * object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] * host_p.delta_position_x - object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_x * host_p.delta_position_x + object_tracks[active_obj_idx].pseudo_hdg_state_vec[4] - 2.0F * object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] * host_p.delta_position_y + object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] * host_p.delta_position_y * host_p.delta_position_y);

         object_tracks[active_obj_idx].pseudo_hdg_state_vec[0] = new_state_vec[0];
         object_tracks[active_obj_idx].pseudo_hdg_state_vec[1] = new_state_vec[1];
         object_tracks[active_obj_idx].pseudo_hdg_state_vec[2] = new_state_vec[2];
         object_tracks[active_obj_idx].pseudo_hdg_state_vec[3] = new_state_vec[3];
         object_tracks[active_obj_idx].pseudo_hdg_state_vec[4] = new_state_vec[4];
         object_tracks[active_obj_idx].pseudo_hdg_state_vec[5] = new_state_vec[5];
      }
   }

   /*===========================================================================*\
   * FUNCTION: Convert_Object_Properties()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Host_Props_T & host_p
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
   * Function transforms objects properties from previous VCS position and orientation,
   * VCS_(t-1), to current VCS position and orientation VCS_(t0).
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Convert_Object_Properties(
      const F360_Host_Props_T & host_p,
      F360_Object_Track_T& obj)
   {
      const Point pos_in_prev_vcs = obj.vcs_position;
      const Point pos_in_curr_vcs = Transform_Point_From_Prev_To_Current_Vcs(pos_in_prev_vcs, host_p);

      // Velocity, velocities only needs to be rotated, no translation part
      float32_t x_vel;
      float32_t y_vel;
      F360_Rotate_2D_Vector(
         obj.vcs_velocity.longitudinal,
         obj.vcs_velocity.lateral,
         host_p.cos_delta_pointing,
         -host_p.sin_delta_pointing,
         x_vel,
         y_vel);

      // Acceleration - Needs only to be rotated, no translation part
      float32_t x_acc;
      float32_t y_acc;
      F360_Rotate_2D_Vector(
         obj.vcs_accel.longitudinal,
         obj.vcs_accel.lateral,
         host_p.cos_delta_pointing,
         -host_p.sin_delta_pointing,
         x_acc,
         y_acc);

      obj.vcs_accel.longitudinal = x_acc;
      obj.vcs_accel.lateral = y_acc;

      // Transform previous object center VCS position for crude "position change heading" calculation, only applicable for CTCA and CCA objects
      obj.prev_vcs_center_pos = Transform_Point_From_Prev_To_Current_Vcs(obj.prev_vcs_center_pos, host_p);
      obj.filtered_pos_diff_heading = Normalize_Heading_Angle(obj.filtered_pos_diff_heading - host_p.delta_pointing, 0.0F);

      // Heading and pointing
      const Angle heading = (obj.vcs_heading - host_p.delta_pointing).Normalize();
      // If object is non_moveable, set pointing equal to heading
      const Angle pointing = (obj.movable_prob > 0.5F) ? ((obj.bbox.Get_Orientation() - host_p.delta_pointing).Normalize()) : heading;

      // Update object properties
      obj.vcs_position = pos_in_curr_vcs;
      obj.vcs_velocity.longitudinal = x_vel;
      obj.vcs_velocity.lateral = y_vel;
      obj.vcs_heading = heading;
      obj.Set_Bbox_Orientation(pointing);
   }

   /*===========================================================================*\
   * FUNCTION: Convert_Object_Covariance()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Host_Props_T & host_p
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
   * Function transforms objects covariance matrix from previous VCS position/orientation,
   * VCS_(t-1) to current VCS position/orientation VCS_(t0).
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Convert_Object_Covariance(
      const F360_Host_Props_T& host_p,
      F360_Object_Track_T& obj)
   {
    
      // Fill transformation matrix depending on object type
      if (obj.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA)
      {
         /* Rotate covariance matrix P_rotated = R *P * R'
         * Rotation matrix is R = [ cos(delta_pnt), sin(delta_pnt), 0, 0, 0, 0;
         *                          -sin(delta_pnt), cos(delta_pnt), 0, 0, 0, 0;
         *                                   0,               0,      1, 0, 0, 0;   
         *                                   0,               0,      0, 1, 0, 0;
         *                                   0,               0,      0, 0, 1, 0;
         *                                   0,               0,      0, 0, 0, 1] 
         * Compute R * P * R'. To optimize for runtime divide the R and P matrices into
         * R = [R0, 0;        P = [P0,  P1;
         *      0,  I]             P1', P2]
         * and do
         * R * P * R' = [R0*P0*R0', R0*P1;
         *                P1'*R0',   P2  ] // Note that P1'*R0' = (R0*P1)' which is also utilized */

         const float32_t cos_pnt = host_p.cos_delta_pointing;
         const float32_t sin_pnt = host_p.sin_delta_pointing;
         const float32_t sq_cos_pnt = cos_pnt * cos_pnt;
         const float32_t sq_sin_pnt = sin_pnt * sin_pnt;
         const float32_t cos_sin_pnt = cos_pnt * sin_pnt;

         // Compute R0*P0*R0'
         const float32_t element00 = sq_cos_pnt * obj.errcov[0][0] + sq_sin_pnt * obj.errcov[1][1] + 2.0F * cos_sin_pnt * obj.errcov[0][1];
         const float32_t element01 = -cos_sin_pnt * obj.errcov[0][0] + cos_sin_pnt * obj.errcov[1][1] + (sq_cos_pnt - sq_sin_pnt) * obj.errcov[0][1];
         const float32_t element11 = sq_sin_pnt * obj.errcov[0][0] + sq_cos_pnt * obj.errcov[1][1] - 2.0F * cos_sin_pnt * obj.errcov[0][1];

         // Compute R0*P1
         const float32_t element02 = cos_pnt * obj.errcov[0][2] + sin_pnt * obj.errcov[1][2];
         const float32_t element03 = cos_pnt * obj.errcov[0][3] + sin_pnt * obj.errcov[1][3];
         const float32_t element04 = cos_pnt * obj.errcov[0][4] + sin_pnt * obj.errcov[1][4];
         const float32_t element05 = cos_pnt * obj.errcov[0][5] + sin_pnt * obj.errcov[1][5];
         const float32_t element12 = -sin_pnt * obj.errcov[0][2] + cos_pnt * obj.errcov[1][2];
         const float32_t element13 = -sin_pnt * obj.errcov[0][3] + cos_pnt * obj.errcov[1][3];
         const float32_t element14 = -sin_pnt * obj.errcov[0][4] + cos_pnt * obj.errcov[1][4];
         const float32_t element15 = -sin_pnt * obj.errcov[0][5] + cos_pnt * obj.errcov[1][5];

         // Fill errcov with new values
         obj.errcov[0][0] = element00;
         obj.errcov[0][1] = element01;
         obj.errcov[1][0] = obj.errcov[0][1];
         obj.errcov[0][2] = element02;
         obj.errcov[2][0] = obj.errcov[0][2];
         obj.errcov[0][3] = element03;
         obj.errcov[3][0] = obj.errcov[0][3];
         obj.errcov[0][4] = element04;
         obj.errcov[4][0] = obj.errcov[0][4];
         obj.errcov[0][5] = element05;
         obj.errcov[5][0] = obj.errcov[0][5];
         obj.errcov[1][1] = element11;
         obj.errcov[1][2] = element12;
         obj.errcov[2][1] = obj.errcov[1][2];
         obj.errcov[1][3] = element13;
         obj.errcov[3][1] = obj.errcov[1][3];
         obj.errcov[1][4] = element14;
         obj.errcov[4][1] = obj.errcov[1][4];
         obj.errcov[1][5] = element15;
         obj.errcov[5][1] = obj.errcov[1][5];
      }
      else
      {
         /* Rotate covariance matrix P_rotated = R *P * R'
         * Rotation matrix is R = [ cos(delta_pnt),       0,               0,       sin(delta_pnt),       0,             0;
         *                                0,        cos(delta_pnt),        0,             0,        sin(delta_pnt),       0;
         *                                0,              0,         cos(delta_pnt),      0,              0,        sin(delta_pnt);
         *                         -sin(delta_pnt),       0,               0,       cos(delta_pnt),       0,             0;
         *                                0,       -sin(delta_pnt),        0,             0,        cos(delta_pnt),       0;
         *                                0,              0,        -sin(delta_pnt),      0,              0,        cos(delta_pnt)]
         * To optimize for runtime utilize the structure of the R matrix (which contains a lot of 0:os) */

          // Pre - multiply with R
         float32_t RP[STATE_DIMENSION][STATE_DIMENSION];
         for (uint32_t row_idx1 = 0U; row_idx1 < 3U; row_idx1++)
         {
            for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
            {
               const uint32_t row_idx2 = row_idx1 + 3U;
               RP[row_idx1][col_idx] = host_p.cos_delta_pointing * obj.errcov[row_idx1][col_idx] + host_p.sin_delta_pointing * obj.errcov[row_idx2][col_idx];
               RP[row_idx2][col_idx] = -host_p.sin_delta_pointing * obj.errcov[row_idx1][col_idx] + host_p.cos_delta_pointing * obj.errcov[row_idx2][col_idx];
            }
         }

         // Post multiply with R
         for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
         {
            for (uint32_t col_idx1 = 0U; col_idx1 < 3U; col_idx1++)
            {
               const uint32_t col_idx2 = col_idx1 + 3U;
               obj.errcov[row_idx][col_idx1] = RP[row_idx][col_idx1] * host_p.cos_delta_pointing + RP[row_idx][col_idx2] * host_p.sin_delta_pointing;
               obj.errcov[row_idx][col_idx2] = -RP[row_idx][col_idx1] * host_p.sin_delta_pointing + RP[row_idx][col_idx2] * host_p.cos_delta_pointing;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Transform_Point_From_Prev_To_Current_Vcs()
   * ===========================================================================
   * RETURN VALUE:
   * Point current_vcs_pos
   *
   * PARAMETERS:
   * const Point& prev_vcs_pos
   * const F360_Host_Props_T& host_p
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
   * Function transforms a point from previous VCS position,
   * VCS_(t-1) to current VCS position VCS_(t0).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Point Transform_Point_From_Prev_To_Current_Vcs(
      const Point& prev_vcs_pos,
      const F360_Host_Props_T& host_p)
   {
      /*
      Position
      Point position is here in VCS_(t-1)
      The delta movement of host is expressed in VCS_(t-1)
      So compensate for the position shift due to host motion by translating the distance moved
      */
      const float32_t long_pos_translated = prev_vcs_pos.x - host_p.delta_position_x;
      const float32_t lat_pos_translated = prev_vcs_pos.y - host_p.delta_position_y;

      // Translated position is shifted to correct origin but still orientated in VCS_(t-1)
      // Rotate to align the position in VCS_(t0)
      Point current_vcs_pos = {};
      F360_Rotate_2D_Vector(
         long_pos_translated,
         lat_pos_translated,
         host_p.cos_delta_pointing,
         -host_p.sin_delta_pointing,
         current_vcs_pos.x,
         current_vcs_pos.y);

      return current_vcs_pos;
   }
}

