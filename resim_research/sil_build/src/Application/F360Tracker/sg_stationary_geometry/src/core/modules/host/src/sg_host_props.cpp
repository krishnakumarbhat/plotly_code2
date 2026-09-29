/*===================================================================================*\
* FILE: sg_host_props.cpp
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions which updates host properties with calculated value.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "sg_host_props.h"

#include "sg_math.h"
#include "sg_matrix_operations.h"

void sg::calculate_host_properties(const float elapsed_time, const RSPP_Host_T &host, HostProps &host_properties)
{
   // Calculate pointing angle delta from previous tracker iteration
   host_properties.delta_pointing     = host.yaw_rate_rad * elapsed_time;
   host_properties.cos_delta_pointing = cosf(host_properties.delta_pointing);
   host_properties.sin_delta_pointing = sinf(host_properties.delta_pointing);

   /* Calculate distance that host has moved from previous tracker iteration in x (denoted as delta_position.x) and
    *  y (denoted as delta_position.y) direction for a coordinate system aligned with the heading angle from previous tracker
    * iteration. Note that these calculations assumes that host is driving on a circle */
   sg::Matrix<float, 2, 1> deltaXY{};

   if (std::abs(host.yaw_rate_rad) > 0.0001F)
   {
      const float curve_radius = host.vcs_speed / host.yaw_rate_rad;

      deltaXY[0][0] = curve_radius * host_properties.sin_delta_pointing;
      deltaXY[1][0] = curve_radius * (1.0F - host_properties.cos_delta_pointing);
   }
   else
   {
      // Same calculation as in if statement but using small angle approximation of sin and cos
      deltaXY[0][0] = host.vcs_speed * elapsed_time;
      deltaXY[1][0] = 0.5F * host.yaw_rate_rad * host.vcs_speed * (elapsed_time * elapsed_time);
   }
   /* In host props we store distance that host has moved from previous tracker given in the old VCS coordinate system
   from previous scan so we need to do a rotation of deltaXY (from old heading aligned to old pointing aligned system)
   Calculation of delta_pos_x and delta_pos_y assumes a circular motion--> constant sideslip-->
   previous vcs_sideslip is not required to obtain heading angle from previous tracker iteration */
   const float c_prev_side_slip     = cosf(host.vcs_sideslip);
   const float s_prev_side_slip     = sinf(host.vcs_sideslip);
   host_properties.delta_position.x = c_prev_side_slip * deltaXY[0][0] - s_prev_side_slip * deltaXY[1][0];
   host_properties.delta_position.y = s_prev_side_slip * deltaXY[0][0] + c_prev_side_slip * deltaXY[1][0];

   /* Heading angle in previous tracker iteration
   Calculation of delta_pos_x and delta_pos_y assumes a circular motion --> constant sideslip -->
   previous vcs_sideslip is not required to obtain heading angle from previous tracker iteration
   Note: host_properties->heading_angle is filled with host pointing angle */
   const float prev_heading_angle = host_properties.heading_angle + host.vcs_sideslip;

   const float cangle = cosf(prev_heading_angle);
   const float sangle = sinf(prev_heading_angle);

   // Initialize Rotation matrix to rotate delta_pos_x and delta_pos_y to WCS
   const sg::Matrix<float, 2U, 2U> rot_mat{{{cangle, -sangle}, {sangle, cangle}}};

   // Rotational transformation of delta_pos_x and delta_pos_y to WCS
   const sg::Matrix<float, 2U, 1U> pos_inc_wcs = rot_mat * deltaXY;

   // Host position update
   host_properties.position.x += pos_inc_wcs[0][0];
   host_properties.position.y += pos_inc_wcs[1][0];

   // Calculate cumulative pointing angle, i.e. pointing angle for current tracker iteration
   // Note: host_properties->heading_angle is filled with host pointing angle
   host_properties.heading_angle = sg::normalize_angle(host_properties.heading_angle + host_properties.delta_pointing, 0.0F);

   // Heading angle in current tracker iteration
   const float heading_angle = host_properties.heading_angle + host.vcs_sideslip;

   // TODO: Remove the calculation of host_properties->position_inc_cov and replace the usage of it with the
   // equivalent variable from SCM module.
   float rot_mat_jacobian[2][2];
   const float cheading_angle = cosf(heading_angle);
   const float sheading_angle = sinf(heading_angle);

   // Initialize Rotation matrix
   rot_mat_jacobian[0][0] = cheading_angle;
   rot_mat_jacobian[0][1] = -sheading_angle;
   rot_mat_jacobian[1][0] = sheading_angle;
   rot_mat_jacobian[1][1] = cheading_angle;

   const float speed_var_factor_dx = 0.01F * 0.01F;
   const float speed_var_factor_dy = 0.001F * 0.001F;
   const float speed_var_bias      = 0.3F * 0.3F;

   // Position incrementation uncertainty estimation.
   /* TODO: it's very simple and very inconsistent uncertainty estimation.
    *  Host processing need to be re-factored to enable precise uncertainty propagation.
    *  Since propagation is highly nonlinear consider Monte Carlo propagation. */
   const float vx_var = ((host.vcs_speed * host.vcs_speed * speed_var_factor_dx) + speed_var_bias);
   const float vy_var = ((host.vcs_speed * host.vcs_speed * speed_var_factor_dy) + speed_var_bias);

   float vcs_vel_cov[2][2];
   vcs_vel_cov[sg::COV_2D_IDX_X][sg::COV_2D_IDX_X] = vx_var;
   vcs_vel_cov[sg::COV_2D_IDX_X][sg::COV_2D_IDX_Y] = 0.0F;
   vcs_vel_cov[sg::COV_2D_IDX_Y][sg::COV_2D_IDX_X] = 0.0F;
   vcs_vel_cov[sg::COV_2D_IDX_Y][sg::COV_2D_IDX_Y] = vy_var;

   sg::propagate_uncertainty(rot_mat_jacobian, vcs_vel_cov, host_properties.vel_cov);

   // Position incrementation differs only by dt.
   host_properties.position_inc_cov[sg::COV_2D_IDX_X][sg::COV_2D_IDX_X] = host_properties.vel_cov[sg::COV_2D_IDX_X][sg::COV_2D_IDX_X];
   host_properties.position_inc_cov[sg::COV_2D_IDX_X][sg::COV_2D_IDX_Y] = host_properties.vel_cov[sg::COV_2D_IDX_X][sg::COV_2D_IDX_Y];
   host_properties.position_inc_cov[sg::COV_2D_IDX_Y][sg::COV_2D_IDX_X] = host_properties.vel_cov[sg::COV_2D_IDX_Y][sg::COV_2D_IDX_X];
   host_properties.position_inc_cov[sg::COV_2D_IDX_Y][sg::COV_2D_IDX_Y] = host_properties.vel_cov[sg::COV_2D_IDX_Y][sg::COV_2D_IDX_Y];

   sg::propagate_uncertainty(elapsed_time, host_properties.position_inc_cov);

   // Update cos and sin of host pointing angle
   // Note: host_properties->heading_angle is filled with host pointing angle
   host_properties.cos_heading = cosf(host_properties.heading_angle);
   host_properties.sin_heading = sinf(host_properties.heading_angle);
}

void sg::reset_host_properties(HostProps &host_properties)
{
   host_properties = {};
}
