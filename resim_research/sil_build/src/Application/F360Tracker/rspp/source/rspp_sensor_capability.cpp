/*===========================================================================*/
/**
 * @file rspp_sensor_capability.cpp
 *
 * @brief Sensor capability and uncertainty propagation implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of sensor capability functions including uncertainty propagation
 * calculations for range rate compensation, velocity transformations, and
 * detection measurements. Provides covariance matrix computations using
 * Jacobian linearization methods.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
  *   - None.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_math.h"
#include <cstring>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_sensor_capability.h"
#include "rspp_math_func.h"
#include "rspp_constants.h"
#include "rspp_detection.h"
#include "rspp_radar_sensor.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Using Namespaces
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{
   /*===========================================================================*
    * Local Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Static Variable Definitions
    *===========================================================================*/

   /*===========================================================================*
    * Function Definitions
    *===========================================================================*/
   void RSPP_Get_Uncertainty_Of_Compensated_Range_Rate(
       const float32_t cos_det_az,
       const float32_t sin_det_az,
       const float32_t (&sens_vel)[2],
       const float32_t var_det_rng_rate,
       const float32_t var_det_az,
       const float32_t (&cov_sens_vel)[2][2],
       float32_t &var_comp_rng_rate)
   {
      float32_t det_cov[4][4] = {0.0F};
      float32_t jacobian[1][4];
      float32_t temp_mat[1][4];
      float32_t temp_var_comp_rng_rate[1][1];

      /* Equation for computing compensated range rate is:
      comp_rng_rate = rng_rate + sens_vel_x*cos(az) + sens_vel_y*sin(az)
      Assume uncertainty in rng_rate, sens_vel and az. Linearize the above
      equation and assume normal distributed variables to get the uncertainty
      in comp_range_rate */

      // Setup covariance of [rng_rate, az, sens_vel_x, sens_vel_y]^T
      det_cov[0][0] = var_det_rng_rate;
      det_cov[1][1] = var_det_az;
      det_cov[2][2] = cov_sens_vel[0][0];
      det_cov[2][3] = cov_sens_vel[0][1];
      det_cov[3][2] = cov_sens_vel[1][0];
      det_cov[3][3] = cov_sens_vel[1][1];

      // Compute jacobian (linearization)
      jacobian[0][0] = 1.0F;                                                 // d comp_rng_rate / d rng_rate
      jacobian[0][1] = -sens_vel[0] * sin_det_az + sens_vel[1] * cos_det_az; // d comp_rng_rate / d az
      jacobian[0][2] = cos_det_az;                                           // d comp_rng_rate / d sens_vel_x
      jacobian[0][3] = sin_det_az;                                           // d comp_rng_rate / d sens_vel_x

      // Propagate uncertainty
      RSPP_Matmul_MxN_NxP(jacobian, det_cov, temp_mat);
      RSPP_Matmul_MxN_PxN_Transpose(temp_mat, jacobian, temp_var_comp_rng_rate);

      var_comp_rng_rate = temp_var_comp_rng_rate[0][0];
   }

   float32_t RSPP_Compute_Raw_Host_Speed_Uncertainty(
       const RSPP_Host_T &host,
       const float32_t max_otg_speed)
   {
      /* Use the following equations and assume uncertainty in speed_correction_factor,
         raw_speed and raw_yaw_rate:
            - host->speed = (speed_correction_factor)*raw_speed
         Propagate the uncertainties to host->speed by
         linearizing the above equations and assuming normal distributed variables. */
      float32_t raw_speed;
      const float32_t speed_correction_factor = host.speed_correction_factor;
      const float32_t speed_compensation_factor = speed_correction_factor;

      if (RSPP_EPSILON > std::abs(speed_compensation_factor))
      {
         raw_speed = max_otg_speed;
      }
      else
      {
         raw_speed = host.speed / speed_compensation_factor;
      }

      /* Modeled uncertainties of raw measured host signals.
         Can be considered to be tuning variables of the sensor capability
         module. */
      const float32_t raw_speed_var = 1e-4F; // [(m/s)^]
      const float32_t speed_correction_factor_var = 1e-5F;

      /* Propagate raw uncertainties */
      const float32_t host_speed_var = speed_compensation_factor * speed_compensation_factor * raw_speed_var +
                                       raw_speed * raw_speed * speed_correction_factor_var;
      return host_speed_var;
   }

   void RSPP_Compute_Raw_Detection_Uncertainty(
       const float32_t det_az,
       const float32_t fov_min_az_rad,
       const float32_t fov_max_az_rad,
       const float32_t (&interior_fov)[RSPP_DET_NUM_LOOK_ID],
       const RSPP_Det_Look_ID_T look_id,
       const RSPP_Sensor_Type_T sensor_type,
       float32_t &azimuth_var)
   {
      // Variables for modeling azimuth variance
      const float32_t frac_az = 0.04F;
      const float32_t az_safety_margin = RSPP_deg2rad(5.0F);
      float32_t std_vec[7];           // NUM_AZ_BREAKPOINTS
      float32_t az_breakpoint_vec[7]; // NUM_AZ_BREAKPOINTS
      float32_t max_interior_fov;
      float32_t min_interior_fov;
      float32_t azimuth_std;

      // Get limits for interior FOV and maximum possible FOV.
      RSPP_Get_Limits_For_FOV(RSPP_Get_Range_Type(look_id), interior_fov, min_interior_fov, max_interior_fov);

      // Set model parameters depending on sensor type
      RSPP_Compute_Azimuth_Breakpoints(fov_min_az_rad, az_safety_margin, min_interior_fov,
                                       frac_az, max_interior_fov, fov_max_az_rad, az_breakpoint_vec);

      RSPP_Compute_Azimuth_Std_Vec_Based_On_Sensor_Type(sensor_type, std_vec);

      // Compute azimuth std
      azimuth_std = RSPP_Piecewise_Linear_Equation(det_az, az_breakpoint_vec, std_vec);

      // Compute variances
      azimuth_var = azimuth_std * azimuth_std;
   }

   void RSPP_Get_Limits_For_FOV(
       const RSPP_Det_Range_Type_T range_type,
       const float32_t (&interior_fov)[RSPP_DET_NUM_LOOK_ID],
       float32_t &min_interior_fov,
       float32_t &max_interior_fov)
   {
      if (RSPP_DET_RANGE_TYPE_MEDIUM == range_type)
      {
         min_interior_fov = interior_fov[RSPP_DET_LOOK_ID_2];
         max_interior_fov = interior_fov[RSPP_DET_LOOK_ID_3];
      }
      else
      {
         min_interior_fov = interior_fov[RSPP_DET_LOOK_ID_0];
         max_interior_fov = interior_fov[RSPP_DET_LOOK_ID_1];
      }
   }

   void RSPP_Compute_Azimuth_Std_Vec_Based_On_Sensor_Type(
       const RSPP_Sensor_Type_T &sensor_type,
       float32_t (&std_vec)[7])
   {
      if ((RSPP_SENSOR_TYPE_MRR360_RADAR == sensor_type) ||
          (RSPP_SENSOR_TYPE_MRR3_RADAR == sensor_type))
      {
         std_vec[0] = RSPP_deg2rad(1.0F);
         std_vec[1] = RSPP_deg2rad(1.0F);
         std_vec[2] = RSPP_deg2rad(0.5F);
         std_vec[3] = RSPP_deg2rad(0.5F);
         std_vec[4] = RSPP_deg2rad(0.5F);
         std_vec[5] = RSPP_deg2rad(1.0F);
         std_vec[6] = RSPP_deg2rad(1.0F);
      }
      else
      {
         std_vec[0] = RSPP_deg2rad(2.0F);
         std_vec[1] = RSPP_deg2rad(2.0F);
         std_vec[2] = RSPP_deg2rad(1.0F);
         std_vec[3] = RSPP_deg2rad(1.0F);
         std_vec[4] = RSPP_deg2rad(1.0F);
         std_vec[5] = RSPP_deg2rad(2.0F);
         std_vec[6] = RSPP_deg2rad(2.0F);
      }
   }

   void RSPP_Compute_Azimuth_Breakpoints(
       const float32_t &min_fov,
       const float32_t &az_safety_margin,
       const float32_t &min_interior_fov,
       const float32_t &frac_az,
       const float32_t &max_interior_fov,
       const float32_t &max_fov,
       float32_t (&az_breakpoint_vec)[7])
   {
      az_breakpoint_vec[0] = min_fov - az_safety_margin;
      az_breakpoint_vec[1] = min_interior_fov;
      az_breakpoint_vec[2] = (1.0F - frac_az) * min_interior_fov;
      az_breakpoint_vec[3] = 0.0F;
      az_breakpoint_vec[4] = (1.0F - frac_az) * max_interior_fov;
      az_breakpoint_vec[5] = max_interior_fov;
      az_breakpoint_vec[6] = max_fov + az_safety_margin;
   }

   void RSPP_Get_Host_Velocity_Uncertainty(
       const RSPP_Host_T &host,
       const float32_t host_speed_var,
       const float32_t host_yaw_rate_var,
       const float32_t (&translation_vec)[2],
       float32_t (&velocity_cov)[2][2])
   {
      /* Use the following rigid body mechanics equations:
            v_B = v_A + w x r_A\B
         where
            v_B = velocity in point B
            v_A = velocity in point A
            w = yaw rate of rigid body
            r_A\B = vector from point A to point B

         For host we have:
            v_A = velocity vector at center of rear axle =
                = speed*[cos(rear_side_slip), sin(rear_sideslip)]^T
         where
            rear_sideslip = -rear_compliance_factor*speed*yaw_rate
         and where
            r_A\B = [dist_rear_axle_to_vcs,]^T + translation_vec
         Assume uncertainty in speed, yawrate and rear_cornering_compliance
         and propagate to host velocity vector. */

      // Extract host properties
      const float32_t speed = host.speed;
      const float32_t speed_sq = speed * speed;
      const float32_t yaw_rate = host.yaw_rate_rad;
      const float32_t dist_rear_axle_to_vcs = host.dist_rear_axle_to_vcs_m;
      const float32_t rear_cornering_compliance = host.rear_cornering_compliance;
      const float32_t rear_sideslip = -rear_cornering_compliance * speed * yaw_rate;
      const float32_t cos_rear_sideslip = RSPP_Cosf(rear_sideslip);
      const float32_t sin_rear_sideslip = RSPP_Sinf(rear_sideslip);

      /* Set maximum bias for host rear cornering compliance factor and
         host distance from rear axle to VCS. Can be considered to be
         tuning variables of the sensor capability module. */
      const float32_t max_bias_rear_cornering_complience = 0.005F;

      // Matrices for variance propagation
      float32_t jacobian[2][3];
      float32_t temp_mat[2][3];
      float32_t host_cov_matrix[3][3] = {0.0F};

      // Set covariance matrix for "raw" host signals
      host_cov_matrix[0][0] = host_speed_var;
      host_cov_matrix[1][1] = host_yaw_rate_var;
      host_cov_matrix[2][2] = max_bias_rear_cornering_complience * max_bias_rear_cornering_complience;

      // Compute the Jacobian
      // Derivative of longitudinal velocity w.r.t. host speed
      jacobian[0][0] = cos_rear_sideslip + rear_cornering_compliance * yaw_rate * speed * sin_rear_sideslip;
      // Derivative of longitudinal velocity w.r.t. yaw_rate
      jacobian[0][1] = rear_cornering_compliance * speed_sq * sin_rear_sideslip - translation_vec[1];
      // Derivative of longitudinal velocity w.r.t. rear cornering compliance factor
      jacobian[0][2] = yaw_rate * speed_sq * sin_rear_sideslip;

      // Derivative of lateral velocity w.r.t. host speed
      jacobian[1][0] = sin_rear_sideslip - rear_cornering_compliance * yaw_rate * speed * cos_rear_sideslip;
      // Derivative of lateral velocity w.r.t. yaw_rate
      jacobian[1][1] = -rear_cornering_compliance * speed_sq * cos_rear_sideslip + dist_rear_axle_to_vcs + translation_vec[0];
      // Derivative of lateral velocity w.r.t. rear cornering compliance factor
      jacobian[1][2] = -yaw_rate * speed_sq * cos_rear_sideslip;

      // Propagate uncertainty of "raw" host signals to uncertainty of host velocity
      RSPP_Matmul_MxN_NxP(jacobian, host_cov_matrix, temp_mat);
      RSPP_Matmul_MxN_PxN_Transpose(temp_mat, jacobian, velocity_cov);
   }
}

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  wzfkqj      Tobias Almroth
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 *-----------------------------------------------------------------------------
 *
 *  File history can be traced by URL:
 *  "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
