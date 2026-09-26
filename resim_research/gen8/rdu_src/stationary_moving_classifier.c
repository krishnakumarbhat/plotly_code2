/*===========================================================================*/
/**
 * @file stationary_moving_classifier.c
 *
 * @brief Implementation of the stationary and moving detection classifier.
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 *
 * @section ABBR ABBREVIATIONS:
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "reuse.h"
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "doppler_unfolding.h"
#include "radar_math.h"
#include "rdu_math.h"
#include "stationary_moving_classifier.h"
#ifdef BBE_ENABLE
   #include "api/bbe_helpers.h"
   #include <xtensa/tie/xt_bbe32.h>
   #include <xtensa/tie/xt_bben.h>
#endif
/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/
#define RDU_DET_EPSILON (1e-10f)

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/
#define VEC_WIDTH (8U)
#define VEC_SIZE  (32U)
/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Const Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Function Prototypes
 *===========================================================================*/
static inline float32_t Mod_Vua(float32_t rr, float32_t min_rr, float32_t vua, float32_t vua_inv);
static inline float32_t Get_Current_Scan_Vua(const RDU_Data_T *p_rdu_data, const RDU_Params_T *p_rdu_params);
/* Helper / sub-function prototypes from stationary_moving_classifier.c */
void Stationary_Moving_Classifier_Precalcs(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals);
void Transform_Host_Vel_To_Scs(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals);
void Compute_Ref_RR(uint16_t num_af_dets, float32_t vx_scs, float32_t vy_scs, float32_t *restrict p_reference_rr,
                    float32_t *restrict p_cos_theta, float32_t *restrict p_sin_theta);
void Calc_Improved_Moving_Det_Thold(uint16_t num_af_dets, float32_t vx_scs, float32_t vy_scs, float32_t *restrict p_cos_theta,
                                    float32_t *restrict p_sin_theta, float32_t *restrict p_moving_det_thold,
                                    RDU_Params_T *p_rdu_params);
uint16_t Apply_Stationary_Threshold(uint16_t num_af_dets, float32_t *restrict p_reference_rr,
                                    float32_t *restrict p_reference_rr_mod, bool *restrict p_f_stationary_det,
                                    bool *restrict p_f_valid_det, float32_t *restrict p_range_rate_diff,
                                    float32_t *restrict p_range_rate, float32_t *restrict p_moving_det_thold,
                                    RDU_Params_T *p_rdu_params, float32_t vua);
bool Compute_Delta_Velocity_Correction(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals);
void Stationary_Det_Classifier(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals);
void Stationary_Det_Classifier_Vel_Correction(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params,
                                              RDU_Internals_T *p_rdu_internals);
void Check_Stationary_Classification_Boundary_Cases(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params,
                                                    RDU_Internals_T *p_rdu_internals);
void Compute_Classification_Confidence_Values(RDU_Data_T *p_rdu_data, RDU_Internals_T *p_rdu_internals);
#ifdef BBE_ENABLE
static inline void Mod_Vua_Vec(const float32_t *p_rr, float32_t min_rr, float32_t vua, float32_t vua_inv, float32_t *p_result);
#endif

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/

/******************************************************************************
 * Name:  Stationary_Moving_Classifier_Precalcs
 *   This function does precalculations required inside stationary moving
 * classifier algorithm
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 * Return Value: none
 *
 * Matlab Function Reference: run_algorithm_single() of stationary moving classfier
 *
 ******************************************************************************/
void Stationary_Moving_Classifier_Precalcs(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
{
   uint32_t num_dets               = p_rdu_data->p_det_data->num_af_det;
   float32_t perspective_angle_rad = RDU_DEG2RAD(p_rdu_params->perspective_angle);
   float32_t az_abs                = 0.0f;
   float32_t *restrict p_theta     = &p_rdu_internals->cur_theta[0];
   bool *restrict p_valid_det_flag = &p_rdu_internals->valid_det_flag[0];
   float32_t *restrict p_cos_theta = &p_rdu_internals->cos_theta[0];
   float32_t *restrict p_sin_theta = &p_rdu_internals->sin_theta[0];
   float32_t *restrict p_range     = &p_rdu_internals->cur_range[0];

#ifdef BBE_ENABLE
   const uint16_t vec_width            = VEC_WIDTH;
   const uint16_t rem                  = num_dets & (vec_width - 1U);
   const uint16_t num_vec              = num_dets / vec_width;
   float32_t *restrict p_theta_aligned = &p_rdu_internals->reference_rr_temp[0];
   xb_vecN_2xf32 *p_theta_out_ptr      = (xb_vecN_2xf32 *)p_theta_aligned;

   for (uint16_t i_vec = 0U; i_vec < num_vec; i_vec++)
   {
      const uint16_t base_idx = i_vec * vec_width;
      xb_vecN_2xf32 *restrict p_theta_align;
      p_theta_align        = (xb_vecN_2xf32 *)(&p_theta[base_idx]);
      valign v_theta_align = BBE_LAN_2XF32_PP(p_theta_align);
      xb_vecN_2xf32 v_theta;
      BBE_LAN_2XF32_IP(v_theta, v_theta_align, p_theta_align);
      BBE_SVN_2XF32_IP(v_theta, p_theta_out_ptr, VEC_SIZE);
   }

   if (rem != 0U)
   {
      const uint16_t base_idx = num_vec * vec_width;
      xb_vecN_2xf32 *restrict p_theta_align;
      p_theta_align        = (xb_vecN_2xf32 *)(&p_theta[base_idx]);
      valign v_theta_align = BBE_LAN_2XF32_PP(p_theta_align);
      valign v_theta_store = BBE_ZALIGN();
      xb_vecN_2xf32 v_theta;
      BBE_LAVN_2XF32_XP(v_theta, v_theta_align, p_theta_align, rem * sizeof(float32_t));
      BBE_SAVN_2XF32_XP(v_theta, v_theta_store, p_theta_out_ptr, rem * sizeof(float32_t));
      BBE_SAN_2XF32POS_FP(v_theta_store, p_theta_out_ptr);
   }

   rm_dot_sin_phs(p_theta_aligned, p_sin_theta, 0.0F, num_dets);
   rm_dot_sin_phs(p_theta_aligned, p_cos_theta, RADAR_PI_BY_2, num_dets);
   for (uint32_t i = 0U; i < num_dets; i++)
   {
      az_abs              = rdu_absf(p_theta[i]);
      p_valid_det_flag[i] = (p_range[i] > p_rdu_params->minimum_range_threshold) && (az_abs < perspective_angle_rad);
   }
#else
   for (uint32_t i = 0; i < num_dets; i++)
   {
      az_abs              = rdu_absf(p_theta[i]);
      p_valid_det_flag[i] = (p_range[i] > p_rdu_params->minimum_range_threshold) && (az_abs < perspective_angle_rad);
      p_cos_theta[i]      = rm_sinf(p_theta[i] + RADAR_PI_BY_2); // rm_cosf(p_theta[i]) = rm_sinf(p_theta[i] + 90 deg);
      p_sin_theta[i]      = rm_sinf(p_theta[i]);
   }
#endif
}

/******************************************************************************
 * Name:  Transform_Host_Vel_To_Scs
 *   This function does transformation of Host Velocity vector to  Velocity in
 * Sensor Co-ordinate System (SCS)
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 * Return Value: bool
 *
 * Matlab Function Reference: get_refspeed_scs()
 *
 ******************************************************************************/
void Transform_Host_Vel_To_Scs(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
{
   p_rdu_data->p_rdu_stage1_output_data->stage1_vx_scs =
      p_rdu_params->m_transform_matrix[0][0] * p_rdu_internals->host_motion_vector.host_yaw_rate +
      p_rdu_params->m_transform_matrix[0][1] * p_rdu_internals->host_motion_vector.host_velocity +
      p_rdu_params->m_transform_matrix[0][2] * p_rdu_internals->host_motion_vector.host_sideslip;
   p_rdu_data->p_rdu_stage1_output_data->stage1_vy_scs =
      p_rdu_params->m_transform_matrix[1][0] * p_rdu_internals->host_motion_vector.host_yaw_rate +
      p_rdu_params->m_transform_matrix[1][1] * p_rdu_internals->host_motion_vector.host_velocity +
      p_rdu_params->m_transform_matrix[1][2] * p_rdu_internals->host_motion_vector.host_sideslip;

   /* Stage1 host-motion logging */
   p_rdu_data->p_rdu_stage1_output_data->stage1_longitudinal_host_velocity = p_rdu_internals->host_motion_vector.host_velocity;
   p_rdu_data->p_rdu_stage1_output_data->stage1_lateral_host_velocity      = p_rdu_internals->host_motion_vector.host_sideslip;
   p_rdu_data->p_rdu_stage1_output_data->stage1_yawrate                    = p_rdu_internals->host_motion_vector.host_yaw_rate;
}

/******************************************************************************
 * Name:  Compute_Ref_RR
 *   This function reference range rate for each detection based on host
 * velocity and angle of the detections
 *
 * Shared Variables: none
 *
 * Parameters:  [in] num_af_dets - Number of AF detections
 *              [in] vx_scs - Velocity in SCS along x-axis
 *              [in] vy_scs - Velocity in SCS along y-axis
 *              [out] p_reference_rr - Pointer to reference range rate array
 *              [in] p_cos_theta - Pointer to array of cosine of theta values
 *              [in] p_sin_theta - Pointer to array of sine of theta values
 *
 * Return Value: bool
 *
 * Matlab Function Reference: get_rr_ref()
 *
 ******************************************************************************/
void Compute_Ref_RR(uint16_t num_af_dets, float32_t vx_scs, float32_t vy_scs, float32_t *restrict p_reference_rr,
                    float32_t *restrict p_cos_theta, float32_t *restrict p_sin_theta)
{
#ifdef BBE_ENABLE
   uint16_t vec_width = VEC_WIDTH; /* BBE32 VFPU width for float32 */
   uint16_t num_vec   = num_af_dets / vec_width;
   uint16_t rem       = num_af_dets % vec_width;

   xb_vecN_2xf32 vx_vec = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vx_scs), 0);
   xb_vecN_2xf32 vy_vec = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vy_scs), 0);

   xb_vecN_2xf32 *p_out_ptr       = (xb_vecN_2xf32 *)p_reference_rr;
   const xb_vecN_2xf32 *p_cos_ptr = (const xb_vecN_2xf32 *)p_cos_theta;
   const xb_vecN_2xf32 *p_sin_ptr = (const xb_vecN_2xf32 *)p_sin_theta;
   xb_vecN_2xf32 cos_vec, sin_vec, sum, result;

   for (uint16_t i = 0U; i < num_vec; i++)
   {
      BBE_LVN_2XF32_IP(cos_vec, p_cos_ptr, VEC_SIZE);
      BBE_LVN_2XF32_IP(sin_vec, p_sin_ptr, VEC_SIZE);

      sum = BBE_MULN_2XF32(cos_vec, vx_vec);
      BBE_MULAN_2XF32(sum, sin_vec, vy_vec);
      result = BBE_NEGN_2XF32(sum);
      BBE_SVN_2XF32_IP(result, p_out_ptr, VEC_SIZE);
   }

   if (rem != 0U)
   {
      BBE_LVN_2XF32_IP(cos_vec, p_cos_ptr, VEC_SIZE);
      BBE_LVN_2XF32_IP(sin_vec, p_sin_ptr, VEC_SIZE);

      sum = BBE_MULN_2XF32(cos_vec, vx_vec);
      BBE_MULAN_2XF32(sum, sin_vec, vy_vec);
      result                = BBE_NEGN_2XF32(sum);
      valign v_result_store = BBE_ZALIGN();
      BBE_SAVN_2XF32_XP(result, v_result_store, p_out_ptr, rem * sizeof(float32_t));
      BBE_SAN_2XF32POS_FP(v_result_store, p_out_ptr);
   }
#else
   for (uint16_t i = 0; i < num_af_dets; i++)
   {
      p_reference_rr[i] = -(p_cos_theta[i] * vx_scs + p_sin_theta[i] * vy_scs);
   }
#endif
}

/******************************************************************************
 * Name:  Calc_Improved_Moving_Det_Thold
 *   This function computes improved moving detection threshold
 *
 * Shared Variables: none
 *
 * Parameters:  [in] num_af_dets - Number of AF detections
 *              [in] vx_scs - Velocity in SCS along x-axis
 *              [in] vy_scs - Velocity in SCS along y-axis
 *              [in] p_cos_theta - Pointer to array of cosine of theta values
 *              [in] p_sin_theta - Pointer to array of sine of theta values
 *              [out] p_moving_det_thold - Pointer to array of moving detection thresholds
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *
 * Return Value: bool
 *
 * Matlab Function Reference: improvedDetMovingThreshold2()
 ******************************************************************************/
void Calc_Improved_Moving_Det_Thold(uint16_t num_af_dets, float32_t vx_scs, float32_t vy_scs, float32_t *restrict p_cos_theta,
                                    float32_t *restrict p_sin_theta, float32_t *restrict p_moving_det_thold,
                                    RDU_Params_T *p_rdu_params)
{
#ifdef BBE_ENABLE
   uint16_t vec_width     = VEC_WIDTH;
   uint16_t rem           = num_af_dets & (vec_width - 1U);
   uint16_t num_vec_iters = num_af_dets / vec_width;

   float32_t cov_00         = p_rdu_params->sensor_motion_covariance_matrix[0][0];
   float32_t cov_01         = p_rdu_params->sensor_motion_covariance_matrix[0][1];
   float32_t cov_11         = p_rdu_params->sensor_motion_covariance_matrix[1][1];
   float32_t var_theta      = p_rdu_params->var_theta;
   float32_t var_range_rate = p_rdu_params->var_range_rate;

   const xb_vecN_2xf32 *p_cos_ptr = (const xb_vecN_2xf32 *)p_cos_theta;
   const xb_vecN_2xf32 *p_sin_ptr = (const xb_vecN_2xf32 *)p_sin_theta;
   xb_vecN_2xf32 *p_thold_ptr     = (xb_vecN_2xf32 *)p_moving_det_thold;

   xb_vecN_2xf32 cov_00_vec    = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(cov_00), 0);
   xb_vecN_2xf32 cov_01_vec    = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(cov_01), 0);
   xb_vecN_2xf32 cov_11_vec    = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(cov_11), 0);
   xb_vecN_2xf32 var_theta_vec = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(var_theta), 0);
   xb_vecN_2xf32 var_rr_vec    = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(var_range_rate), 0);
   xb_vecN_2xf32 vx_vec        = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vx_scs), 0);
   xb_vecN_2xf32 vy_vec        = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vy_scs), 0);
   xb_vecN_2xf32 three_vec     = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(3.0F), 0);

   xb_vecN_2xf32 cos_vec;
   xb_vecN_2xf32 sin_vec;

   for (uint16_t i = 0U; i < num_vec_iters; i++)
   {
      BBE_LVN_2XF32_IP(cos_vec, p_cos_ptr, VEC_SIZE);
      BBE_LVN_2XF32_IP(sin_vec, p_sin_ptr, VEC_SIZE);

      xb_vecN_2xf32 cos_sq  = BBE_MULN_2XF32(cos_vec, cos_vec);
      xb_vecN_2xf32 sin_sq  = BBE_MULN_2XF32(sin_vec, sin_vec);
      xb_vecN_2xf32 cos_sin = BBE_MULN_2XF32(cos_vec, sin_vec);

      xb_vecN_2xf32 sensor_cov_rr = BBE_MULN_2XF32(cos_sq, cov_00_vec);
      BBE_MULAN_2XF32(sensor_cov_rr, cos_sin, cov_01_vec);
      BBE_MULAN_2XF32(sensor_cov_rr, cos_sin, cov_01_vec);
      BBE_MULAN_2XF32(sensor_cov_rr, sin_sq, cov_11_vec);

      xb_vecN_2xf32 u    = BBE_SUBN_2XF32(BBE_MULN_2XF32(sin_vec, vx_vec), BBE_MULN_2XF32(cos_vec, vy_vec));
      xb_vecN_2xf32 u_sq = BBE_MULN_2XF32(u, u);
      BBE_MULAN_2XF32(sensor_cov_rr, u_sq, var_theta_vec);

      xb_vecN_2xf32 v_sum   = BBE_ADDN_2XF32(sensor_cov_rr, var_rr_vec);
      xb_vecN_2xf32 v_sqrt  = BBE_SQRTN_2XF32(v_sum);
      xb_vecN_2xf32 v_thold = BBE_MULN_2XF32(v_sqrt, three_vec);
      BBE_SVN_2XF32_IP(v_thold, p_thold_ptr, VEC_SIZE);
   }

   if (rem != 0U)
   {
      BBE_LVN_2XF32_IP(cos_vec, p_cos_ptr, VEC_SIZE);
      BBE_LVN_2XF32_IP(sin_vec, p_sin_ptr, VEC_SIZE);

      xb_vecN_2xf32 cos_sq  = BBE_MULN_2XF32(cos_vec, cos_vec);
      xb_vecN_2xf32 sin_sq  = BBE_MULN_2XF32(sin_vec, sin_vec);
      xb_vecN_2xf32 cos_sin = BBE_MULN_2XF32(cos_vec, sin_vec);

      xb_vecN_2xf32 sensor_cov_rr = BBE_MULN_2XF32(cos_sq, cov_00_vec);
      BBE_MULAN_2XF32(sensor_cov_rr, cos_sin, cov_01_vec);
      BBE_MULAN_2XF32(sensor_cov_rr, cos_sin, cov_01_vec);
      BBE_MULAN_2XF32(sensor_cov_rr, sin_sq, cov_11_vec);

      xb_vecN_2xf32 u    = BBE_SUBN_2XF32(BBE_MULN_2XF32(sin_vec, vx_vec), BBE_MULN_2XF32(cos_vec, vy_vec));
      xb_vecN_2xf32 u_sq = BBE_MULN_2XF32(u, u);
      BBE_MULAN_2XF32(sensor_cov_rr, u_sq, var_theta_vec);

      xb_vecN_2xf32 v_sum   = BBE_ADDN_2XF32(sensor_cov_rr, var_rr_vec);
      xb_vecN_2xf32 v_sqrt  = BBE_SQRTN_2XF32(v_sum);
      xb_vecN_2xf32 v_thold = BBE_MULN_2XF32(v_sqrt, three_vec);
      valign v_thold_store  = BBE_ZALIGN();
      BBE_SAVN_2XF32_XP(v_thold, v_thold_store, p_thold_ptr, rem * sizeof(float32_t));
      BBE_SAN_2XF32POS_FP(v_thold_store, p_thold_ptr);
   }
#else
   float32_t temp_acc                     = 0.0F;
   float32_t sensor_cov_rr                = 0.0F;
   float32_t sensor_motion_cov_matrix_0_0 = p_rdu_params->sensor_motion_covariance_matrix[0][0];
   float32_t sensor_motion_cov_matrix_0_1 = p_rdu_params->sensor_motion_covariance_matrix[0][1];
   float32_t sensor_motion_cov_matrix_1_1 = p_rdu_params->sensor_motion_covariance_matrix[1][1];
   float32_t sigma_var_t                  = p_rdu_params->var_theta;
   float32_t sigma_var_rr                 = p_rdu_params->var_range_rate;

   for (uint16_t i = 0U; i < num_af_dets; i++)
   {
      sensor_cov_rr = (p_cos_theta[i] * p_cos_theta[i] * sensor_motion_cov_matrix_0_0) +
                      (2.0F * p_cos_theta[i] * p_sin_theta[i] * sensor_motion_cov_matrix_0_1) +
                      (p_sin_theta[i] * p_sin_theta[i] * sensor_motion_cov_matrix_1_1);
      temp_acc              = (p_sin_theta[i] * vx_scs - p_cos_theta[i] * vy_scs);
      p_moving_det_thold[i] = 3.0F * rdu_sqrtf(sensor_cov_rr + (sigma_var_t * temp_acc * temp_acc) + sigma_var_rr);
   }
#endif
}

/******************************************************************************
 * Name:  Mod_Vua
 *   This function returns Modulo of range rate values based on velocity unambiguity
 *
 * Shared Variables: none
 *
 * Parameters:  [in] rr - range rate value
 *              [in] min_rr - minimum range rate value
 *              [in] vua - velocity unambiguity
 *
 * Return Value: float32_t
 *
 * matlab Function Reference: Mod_Vua()
 *
 ******************************************************************************/
static inline float32_t Mod_Vua(float32_t rr, float32_t min_rr, float32_t vua, float32_t vua_inv)
{
   /* rr_mod= rr - vua.*floor((rr-min_rr)./vua); */
   return rr - vua * rdu_floorf((rr - min_rr) * vua_inv);
}

static inline float32_t Get_Current_Scan_Vua(const RDU_Data_T *p_rdu_data, const RDU_Params_T *p_rdu_params)
{
   return p_rdu_params->vua[p_rdu_data->p_det_data->det_list_property.look_type];
}

#ifdef BBE_ENABLE
static inline void Mod_Vua_Vec(const float32_t *p_rr, float32_t min_rr, float32_t vua, float32_t vua_inv, float32_t *p_result)
{
   xb_vecN_2xf32 v_rr;
   const xb_vecN_2xf32 *p_rr_ptr = (const xb_vecN_2xf32 *)p_rr;
   xb_vecN_2xf32 *p_result_ptr   = (xb_vecN_2xf32 *)p_result;
   BBE_LVN_2XF32_IP(v_rr, p_rr_ptr, VEC_SIZE);

   xb_vecN_2xf32 v_min_rr  = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(min_rr), 0);
   xb_vecN_2xf32 v_vua     = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vua), 0);
   xb_vecN_2xf32 v_vua_inv = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vua_inv), 0);

   xb_vecN_2xf32 diff         = BBE_SUBN_2XF32(v_rr, v_min_rr);
   xb_vecN_2xf32 scaled       = BBE_MULN_2XF32(diff, v_vua_inv);
   xb_vecN_2xf32 floor_result = BBE_FIFLOORN_2XF32(scaled);
   xb_vecN_2xf32 product      = BBE_MULN_2XF32(v_vua, floor_result);
   xb_vecN_2xf32 result       = BBE_SUBN_2XF32(v_rr, product);

   BBE_SVN_2XF32_IP(result, p_result_ptr, VEC_SIZE);
}
#endif

/******************************************************************************
 * Name:  Apply_Stationary_Threshold
 *   This function Classifies detections as stationary based on threshold comparison
 *
 * Shared Variables: none
 *
 * Parameters:  [in] num_af_dets - Number of AF detections
 *              [in] p_reference_rr - Pointer to reference range rate array
 *              [in] p_reference_rr_mod - Pointer to modulo of reference range rate array
 *              [in] p_f_valid_det - Pointer to array of valid detection flag
 *              [in] p_range_rate_diff - Pointer to array of range rate differences
 *              [in] p_range_rate - Pointer to array of range rates
 *              [in] p_moving_det_thold - Pointer to array of moving detection thresholds
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *
 * Return Value: bool
 *
 * Matlab Function Reference: apply_stationary_threshold()
 *
 *     % Calculate the absolute differences used for classification
 *     rr_differences = zeros(size(rr_all));
 *     rr_differences(valid_dets) = abs(rr_all(valid_dets) - rr_ref_mod(valid_dets));
 *
 *     % Apply threshold comparison for classification
 *     is_stationary = rr_differences(valid_dets) < T(valid_dets);
 *     num_stationaries = nnz(is_stationary);
 *
 ******************************************************************************/
uint16_t Apply_Stationary_Threshold(uint16_t num_af_dets, float32_t *restrict p_reference_rr,
                                    float32_t *restrict p_reference_rr_mod, bool *restrict p_f_stationary_det,
                                    bool *restrict p_f_valid_det, float32_t *restrict p_range_rate_diff,
                                    float32_t *restrict p_range_rate, float32_t *restrict p_moving_det_thold,
                                    RDU_Params_T *p_rdu_params, float32_t vua)
{
#ifdef BBE_ENABLE
   uint16_t num_stationary_dets = 0U;
   uint16_t vec_width           = VEC_WIDTH;
   uint16_t num_vec_iters       = num_af_dets / vec_width;
   uint16_t rem                 = num_af_dets & (vec_width - 1U);
   float32_t vua_inv            = rdu_finv(vua);

   xb_vecN_2xf32 *p_rr_ref_ptr   = (xb_vecN_2xf32 *)p_reference_rr_mod;
   xb_vecN_2xf32 *p_thold_ptr    = (xb_vecN_2xf32 *)p_moving_det_thold;
   xb_vecN_2xf32 *p_diff_out_ptr = (xb_vecN_2xf32 *)p_range_rate_diff;

   xb_vecN_2xf32 v_range_rate;
   xb_vecN_2xf32 v_reference_rr_mod;
   xb_vecN_2xf32 v_moving_thold;
   xb_vecN_2xf32 v_range_rate_diff_vec;

   for (uint16_t i = 0U; i < num_vec_iters * vec_width; i += vec_width)
   {
      Mod_Vua_Vec(&p_reference_rr[i], p_rdu_params->min_range_rate, vua, vua_inv, &p_reference_rr_mod[i]);
   }

   for (uint16_t i = num_vec_iters * vec_width; i < num_af_dets; i++)
   {
      p_reference_rr_mod[i] = Mod_Vua(p_reference_rr[i], p_rdu_params->min_range_rate, vua, vua_inv);
   }

   for (uint16_t i_vec = 0U; i_vec < num_vec_iters; i_vec++)
   {
      uint16_t base_idx                      = i_vec * vec_width;
      xb_vecNx16U *restrict vec_p_valid_flag = (xb_vecNx16U *)(&p_f_valid_det[base_idx]);
      valign bb_valid_align                  = BBE_LANX16U_PP(vec_p_valid_flag);
      xb_vecNx16U vec_valid_flag_8bit;
      BBE_LAVNX16U_XP(vec_valid_flag_8bit, bb_valid_align, vec_p_valid_flag, 8);

      xb_vecNx16U vec_lo_8bit_sel_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
      vec_valid_flag_8bit =
         BBE_SELNX16UI((vec_valid_flag_8bit >> 8U), vec_valid_flag_8bit, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_sel_mask;
      xb_vecN_2x32Uv vec_valid_flag_32bit =
         BBE_SELN_2X32UI(BBE_ZERON_2X32U(), BBE_MOVN_2X32U_FROMNX16(vec_valid_flag_8bit), BBE_SELI_INTERLEAVE_1_LO);
      vboolN_2 valid_mask = vec_valid_flag_32bit != BBE_ZERON_2X32U();

      // Loading range rate values since it is in unaligned IPC data stream
      xb_vecN_2xf32 *restrict p_ralign;
      p_ralign                  = (xb_vecN_2xf32 *)(&p_range_rate[base_idx]);
      valign v_range_rate_align = BBE_LAN_2XF32_PP(p_ralign);
      BBE_LAN_2XF32_IP(v_range_rate, v_range_rate_align, p_ralign);

      BBE_LVN_2XF32_IP(v_reference_rr_mod, p_rr_ref_ptr, VEC_SIZE);
      BBE_LVN_2XF32_IP(v_moving_thold, p_thold_ptr, VEC_SIZE);
      v_range_rate_diff_vec = BBE_ZERON_2X32U(); // Newly added

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         uint16_t idx            = base_idx + lane;
         p_f_stationary_det[idx] = false;
      }
      xb_vecN_2xf32 v_diff_raw = BBE_SUBN_2XF32(v_range_rate, v_reference_rr_mod);
      BBE_ABSN_2XF32T(v_range_rate_diff_vec, v_diff_raw, valid_mask);
      BBE_SVN_2XF32_IP(v_range_rate_diff_vec, p_diff_out_ptr, VEC_SIZE);

      vboolN_2 is_stationary_vec       = BBE_OLTN_2XF32(v_range_rate_diff_vec, v_moving_thold);
      vboolN_2 is_valid_stationary_vec = BBE_ANDBN_2(is_stationary_vec, valid_mask);
      vboolN a                         = BBE_MOVN_FROMN_2(is_valid_stationary_vec);
      uint32_t stationary_count;
      vselN selector;
      BBE_SQZN(selector, stationary_count, a);
      xb_vecN_2xf32 v01 = BBE_MOVN_2XF32T((xb_vecN_2xf32)1.0f, BBE_ZERON_2XF32(), is_valid_stationary_vec);
      num_stationary_dets += stationary_count >> 2;
      float32_t stat01_buf[8U];
      BBE_SVN_2XF32_I(v01, (xb_vecN_2xf32 *)stat01_buf, 0);

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t idx = base_idx + lane;
         if (idx < num_af_dets)
         {
            const bool is_stationary = (stat01_buf[lane] > 0.5f);
            p_f_stationary_det[idx]  = (is_stationary);
         }
      }
   }

   if (rem != 0U)
   {
      const uint16_t base_idx = num_vec_iters * vec_width;
      uint32_t __attribute__((aligned(32))) valid_mask_arr[VEC_WIDTH];

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t idx   = base_idx + lane;
         const bool in_bounds = (idx < num_af_dets);
         valid_mask_arr[lane] = (in_bounds && (p_f_valid_det[idx] == true)) ? 0xFU : 0x0U;
      }

      vboolN_2 valid_mask = *((xb_vecN_2x32Uv *)valid_mask_arr) != BBE_ZERON_2X32U();

      xb_vecN_2xf32 *restrict p_ralign;
      p_ralign                  = (xb_vecN_2xf32 *)(&p_range_rate[base_idx]);
      valign v_range_rate_align = BBE_LAN_2XF32_PP(p_ralign);
      BBE_LAN_2XF32_IP(v_range_rate, v_range_rate_align, p_ralign);

      BBE_LVN_2XF32_IP(v_reference_rr_mod, p_rr_ref_ptr, VEC_SIZE);
      BBE_LVN_2XF32_IP(v_moving_thold, p_thold_ptr, VEC_SIZE);
      v_range_rate_diff_vec = BBE_ZERON_2X32U();

      for (uint16_t lane = 0U; lane < rem; lane++)
      {
         p_f_stationary_det[base_idx + lane] = false;
      }

      xb_vecN_2xf32 v_diff_raw = BBE_SUBN_2XF32(v_range_rate, v_reference_rr_mod);
      BBE_ABSN_2XF32T(v_range_rate_diff_vec, v_diff_raw, valid_mask);
      valign v_diff_store = BBE_ZALIGN();
      BBE_SAVN_2XF32_XP(v_range_rate_diff_vec, v_diff_store, p_diff_out_ptr, rem * sizeof(float32_t));
      BBE_SAN_2XF32POS_FP(v_diff_store, p_diff_out_ptr);

      vboolN_2 is_stationary_vec       = BBE_OLTN_2XF32(v_range_rate_diff_vec, v_moving_thold);
      vboolN_2 is_valid_stationary_vec = BBE_ANDBN_2(is_stationary_vec, valid_mask);
      vboolN a                         = BBE_MOVN_FROMN_2(is_valid_stationary_vec);
      uint32_t stationary_count;
      vselN selector;
      BBE_SQZN(selector, stationary_count, a);
      xb_vecN_2xf32 v01 = BBE_MOVN_2XF32T((xb_vecN_2xf32)1.0f, BBE_ZERON_2XF32(), is_valid_stationary_vec);
      num_stationary_dets += stationary_count >> 2;
      float32_t stat01_buf[8U];
      BBE_SVN_2XF32_I(v01, (xb_vecN_2xf32 *)stat01_buf, 0);

      for (uint16_t lane = 0U; lane < rem; lane++)
      {
         const bool is_stationary            = (stat01_buf[lane] > 0.5f);
         p_f_stationary_det[base_idx + lane] = is_stationary;
      }
   }

   return num_stationary_dets;
#else
   uint16_t num_stationary_dets = 0U;
   float32_t vua_inv            = rdu_finv(vua);
   for (uint16_t i = 0; i < num_af_dets; i++)
   {
      p_reference_rr_mod[i] = Mod_Vua(p_reference_rr[i], p_rdu_params->min_range_rate, vua, vua_inv);
      p_range_rate_diff[i]  = 0.0F;
      p_f_stationary_det[i] = false;
      if (p_f_valid_det[i] == true)
      {
         p_range_rate_diff[i]  = rdu_absf(p_range_rate[i] - p_reference_rr_mod[i]);
         p_f_stationary_det[i] = (p_range_rate_diff[i] < p_moving_det_thold[i]) ? true : false;
         num_stationary_dets += (p_f_stationary_det[i] == true) ? 1 : 0;
      }
   }
   return num_stationary_dets;
#endif
}

/******************************************************************************
 * Name:  Compute_Delta_Velocity_Correction
 *   This function computes velocity correction factor for low vehicle speed case
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 *
 * Return Value: bool
 *
 ******************************************************************************/
bool Compute_Delta_Velocity_Correction(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
{
#ifdef BBE_ENABLE
   bool ret_value               = true;
   const uint16_t num_dets      = p_rdu_data->p_det_data->num_af_det;
   const uint16_t num_stat_dets = p_rdu_internals->num_stationary_dets;
   const float32_t min_rr       = p_rdu_params->min_range_rate;
   const float32_t vua          = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);

   if (num_stat_dets == 0U)
   {
      p_rdu_internals->v_xy_delta.vx = 0.0f;
      p_rdu_internals->v_xy_delta.vy = 0.0f;
   }
   else
   {
      float32_t A2[2][2];
      float32_t Atrr[2];
      A2[0][0] = 0.0f;
      A2[0][1] = 0.0f;
      A2[1][0] = 0.0f;
      A2[1][1] = 0.0f;
      Atrr[0]  = 0.0f;
      Atrr[1]  = 0.0f;

      const float32_t *p_vel              = &p_rdu_internals->cur_vel[0];
      const float32_t *p_reference_rr_mod = &p_rdu_internals->reference_rr_mod[0];
      const float32_t *p_threshold        = &p_rdu_internals->stage1_moving_det_thold[0];
      const bool *p_f_stationary_det      = &p_rdu_internals->f_stationary_det[0];
      const float32_t *p_cos_theta        = &p_rdu_internals->cos_theta[0];
      const float32_t *p_sin_theta        = &p_rdu_internals->sin_theta[0];

      const uint16_t vec_width        = VEC_WIDTH;
      const uint16_t rem              = num_dets & (vec_width - 1U);
      const uint16_t num_vec_iters    = num_dets / vec_width;
      const float32_t min_rr_plus_vua = min_rr + vua;

      xb_vecN_2xf32 v_acc_cos2    = BBE_ZERON_2XF32();
      xb_vecN_2xf32 v_acc_sin2    = BBE_ZERON_2XF32();
      xb_vecN_2xf32 v_acc_cos_sin = BBE_ZERON_2XF32();
      xb_vecN_2xf32 v_acc_cos_rr  = BBE_ZERON_2XF32();
      xb_vecN_2xf32 v_acc_sin_rr  = BBE_ZERON_2XF32();

      xb_vecN_2xf32 v_min_rr          = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(min_rr), 0);
      xb_vecN_2xf32 v_min_rr_plus_vua = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(min_rr_plus_vua), 0);
      xb_vecN_2xf32 v_vua             = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vua), 0);
      xb_vecN_2xf32 v_neg_vua         = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(-vua), 0);

      for (uint16_t i_vec = 0; i_vec < num_vec_iters; i_vec++)
      {
         const uint32_t idx = i_vec * vec_width;
         // Load unaligned velocity values since it is in unaligned IPC data stream
         xb_vecN_2xf32 *restrict p_valign;
         p_valign           = (xb_vecN_2xf32 *)(&p_vel[idx]);
         valign v_vel_align = BBE_LAN_2XF32_PP(p_valign);
         xb_vecN_2xf32 v_vel;
         BBE_LAN_2XF32_IP(v_vel, v_vel_align, p_valign);
         xb_vecN_2xf32 v_reference_rr_mod = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_reference_rr_mod[idx], 0);
         xb_vecN_2xf32 v_threshold        = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_threshold[idx], 0);
         xb_vecN_2xf32 v_cos_theta        = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_cos_theta[idx], 0);
         xb_vecN_2xf32 v_sin_theta        = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_sin_theta[idx], 0);

         xb_vecNx16U *restrict vec_p_stat_flag = (xb_vecNx16U *)(&p_f_stationary_det[idx]);
         valign bb_stat_align                  = BBE_LANX16U_PP(vec_p_stat_flag);
         xb_vecNx16U vec_stat_flag_8bit;
         BBE_LAVNX16U_XP(vec_stat_flag_8bit, bb_stat_align, vec_p_stat_flag, 8);
         xb_vecNx16U vec_lo_8bit_sel_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
         vec_stat_flag_8bit =
            BBE_SELNX16UI((vec_stat_flag_8bit >> 8U), vec_stat_flag_8bit, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_sel_mask;

         xb_vecN_2x32Uv vec_stat_flag_32bit =
            BBE_SELN_2X32UI(BBE_ZERON_2X32U(), BBE_MOVN_2X32U_FROMNX16(vec_stat_flag_8bit), BBE_SELI_INTERLEAVE_1_LO);
         vboolN_2 stat_mask        = vec_stat_flag_32bit != BBE_ZERON_2X32U();
         vboolN_2 active_stat_mask = stat_mask;

         xb_vecN_2xf32 v_rr_delta = BBE_SUBN_2XF32(v_vel, v_reference_rr_mod);

         xb_vecN_2xf32 v_lower_bound = BBE_ADDN_2XF32(v_min_rr, v_threshold);
         vboolN_2 bb_is_lower        = BBE_OLTN_2XF32(v_vel, v_lower_bound);
         xb_vecN_2xf32 v_upper_bound = BBE_SUBN_2XF32(v_min_rr_plus_vua, v_threshold);
         vboolN_2 bb_is_upper        = BBE_OLTN_2XF32(v_upper_bound, v_vel);

         xb_vecN_2xf32 v_rr_delta_plus_vua = BBE_ADDN_2XF32(v_rr_delta, v_vua);
         xb_vecN_2xf32 v_abs_rr_delta      = BBE_ABSN_2XF32(v_rr_delta);
         xb_vecN_2xf32 v_abs_rr_delta_plus = BBE_ABSN_2XF32(v_rr_delta_plus_vua);
         vboolN_2 bb_choose_plus           = BBE_OLTN_2XF32(v_abs_rr_delta_plus, v_abs_rr_delta);
         vboolN_2 bb_lower_and_choose      = bb_is_lower & bb_choose_plus & active_stat_mask;

         xb_vecN_2xf32 v_rr_delta_minus_vua = BBE_ADDN_2XF32(v_rr_delta, v_neg_vua);
         xb_vecN_2xf32 v_abs_rr_delta_minus = BBE_ABSN_2XF32(v_rr_delta_minus_vua);
         vboolN_2 bb_choose_minus           = BBE_OLTN_2XF32(v_abs_rr_delta_minus, v_abs_rr_delta);
         vboolN_2 bb_upper_and_choose       = bb_is_upper & bb_choose_minus & active_stat_mask;

         BBE_MOVN_2XF32T(v_rr_delta_plus_vua, v_rr_delta, bb_lower_and_choose);
         BBE_MOVN_2XF32T(v_rr_delta_minus_vua, v_rr_delta, bb_upper_and_choose);

         v_cos_theta = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_cos_theta, (~active_stat_mask));
         v_sin_theta = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_sin_theta, (~active_stat_mask));
         v_rr_delta  = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_rr_delta, (~active_stat_mask));

         BBE_MULAN_2XF32(v_acc_cos2, v_cos_theta, v_cos_theta);
         BBE_MULAN_2XF32(v_acc_sin2, v_sin_theta, v_sin_theta);
         BBE_MULAN_2XF32(v_acc_cos_sin, v_cos_theta, v_sin_theta);
         BBE_MULAN_2XF32(v_acc_cos_rr, v_cos_theta, v_rr_delta);
         BBE_MULAN_2XF32(v_acc_sin_rr, v_sin_theta, v_rr_delta);
      }

      if (rem != 0U)
      {
         const uint16_t idx = num_vec_iters * vec_width;
         uint32_t __attribute__((aligned(32))) stat_mask_arr[VEC_WIDTH];

         for (uint16_t lane = 0U; lane < vec_width; lane++)
         {
            const uint16_t lane_idx = idx + lane;
            stat_mask_arr[lane]     = ((lane_idx < num_dets) && (p_f_stationary_det[lane_idx] == true)) ? 0xFU : 0x0U;
         }

         vboolN_2 active_stat_mask = *((xb_vecN_2x32Uv *)stat_mask_arr) != BBE_ZERON_2X32U();

         xb_vecN_2xf32 *restrict p_valign;
         p_valign           = (xb_vecN_2xf32 *)(&p_vel[idx]);
         valign v_vel_align = BBE_LAN_2XF32_PP(p_valign);
         xb_vecN_2xf32 v_vel;
         BBE_LAN_2XF32_IP(v_vel, v_vel_align, p_valign);
         xb_vecN_2xf32 v_reference_rr_mod = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_reference_rr_mod[idx], 0);
         xb_vecN_2xf32 v_threshold        = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_threshold[idx], 0);
         xb_vecN_2xf32 v_cos_theta        = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_cos_theta[idx], 0);
         xb_vecN_2xf32 v_sin_theta        = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)&p_sin_theta[idx], 0);

         xb_vecN_2xf32 v_rr_delta = BBE_SUBN_2XF32(v_vel, v_reference_rr_mod);

         xb_vecN_2xf32 v_lower_bound = BBE_ADDN_2XF32(v_min_rr, v_threshold);
         vboolN_2 bb_is_lower        = BBE_OLTN_2XF32(v_vel, v_lower_bound);
         xb_vecN_2xf32 v_upper_bound = BBE_SUBN_2XF32(v_min_rr_plus_vua, v_threshold);
         vboolN_2 bb_is_upper        = BBE_OLTN_2XF32(v_upper_bound, v_vel);

         xb_vecN_2xf32 v_rr_delta_plus_vua = BBE_ADDN_2XF32(v_rr_delta, v_vua);
         xb_vecN_2xf32 v_abs_rr_delta      = BBE_ABSN_2XF32(v_rr_delta);
         xb_vecN_2xf32 v_abs_rr_delta_plus = BBE_ABSN_2XF32(v_rr_delta_plus_vua);
         vboolN_2 bb_choose_plus           = BBE_OLTN_2XF32(v_abs_rr_delta_plus, v_abs_rr_delta);
         vboolN_2 bb_lower_and_choose      = bb_is_lower & bb_choose_plus & active_stat_mask;

         xb_vecN_2xf32 v_rr_delta_minus_vua = BBE_ADDN_2XF32(v_rr_delta, v_neg_vua);
         xb_vecN_2xf32 v_abs_rr_delta_minus = BBE_ABSN_2XF32(v_rr_delta_minus_vua);
         vboolN_2 bb_choose_minus           = BBE_OLTN_2XF32(v_abs_rr_delta_minus, v_abs_rr_delta);
         vboolN_2 bb_upper_and_choose       = bb_is_upper & bb_choose_minus & active_stat_mask;

         BBE_MOVN_2XF32T(v_rr_delta_plus_vua, v_rr_delta, bb_lower_and_choose);
         BBE_MOVN_2XF32T(v_rr_delta_minus_vua, v_rr_delta, bb_upper_and_choose);

         v_cos_theta = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_cos_theta, (~active_stat_mask));
         v_sin_theta = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_sin_theta, (~active_stat_mask));
         v_rr_delta  = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_rr_delta, (~active_stat_mask));

         BBE_MULAN_2XF32(v_acc_cos2, v_cos_theta, v_cos_theta);
         BBE_MULAN_2XF32(v_acc_sin2, v_sin_theta, v_sin_theta);
         BBE_MULAN_2XF32(v_acc_cos_sin, v_cos_theta, v_sin_theta);
         BBE_MULAN_2XF32(v_acc_cos_rr, v_cos_theta, v_rr_delta);
         BBE_MULAN_2XF32(v_acc_sin_rr, v_sin_theta, v_rr_delta);
      }

      float32_t temp_vec[VEC_WIDTH];
      BBE_SVN_2XF32_I(v_acc_cos2, (xb_vecN_2xf32 *)temp_vec, 0);
      for (uint16_t k = 0; k < vec_width; k++)
      {
         A2[0][0] += temp_vec[k];
      }
      BBE_SVN_2XF32_I(v_acc_sin2, (xb_vecN_2xf32 *)temp_vec, 0);
      for (uint16_t k = 0; k < vec_width; k++)
      {
         A2[1][1] += temp_vec[k];
      }
      BBE_SVN_2XF32_I(v_acc_cos_sin, (xb_vecN_2xf32 *)temp_vec, 0);
      for (uint16_t k = 0; k < vec_width; k++)
      {
         A2[0][1] += temp_vec[k];
         A2[1][0] += temp_vec[k];
      }
      BBE_SVN_2XF32_I(v_acc_cos_rr, (xb_vecN_2xf32 *)temp_vec, 0);
      for (uint16_t k = 0; k < vec_width; k++)
      {
         Atrr[0] += temp_vec[k];
      }
      BBE_SVN_2XF32_I(v_acc_sin_rr, (xb_vecN_2xf32 *)temp_vec, 0);
      for (uint16_t k = 0; k < vec_width; k++)
      {
         Atrr[1] += temp_vec[k];
      }

      Atrr[0] = -Atrr[0];
      Atrr[1] = -Atrr[1];
      A2[0][0] += p_rdu_params->tikh_vx;
      A2[1][1] += p_rdu_params->tikh_vy;

      const float32_t a   = A2[0][0];
      const float32_t b   = A2[0][1];
      const float32_t c   = A2[1][0];
      const float32_t d   = A2[1][1];
      const float32_t det = a * d - b * c;
      if (rdu_absf(det) < RDU_DET_EPSILON)
      {
         p_rdu_internals->v_xy_delta.vx = 0.0f;
         p_rdu_internals->v_xy_delta.vy = 0.0f;
         ret_value                      = false;
      }
      else
      {
         const float32_t inv_det        = rdu_finv(det);
         p_rdu_internals->v_xy_delta.vx = inv_det * (d * Atrr[0] - b * Atrr[1]);
         p_rdu_internals->v_xy_delta.vy = inv_det * (-c * Atrr[0] + a * Atrr[1]);
      }
   }
   return ret_value;
#else
   uint16_t num_dets, num_stat_dets;
   float32_t A2[2][2];
   float32_t Atrr[2];
   float32_t min_rr, vua;
   float32_t a, b, c, d, det, inv_det;
   float32_t rr_delta, rr_measured, threshold;
   float32_t cos_t, sin_t;
   float32_t vua_adjusted_rr;
   bool is_upper_boundary, is_lower_boundary;

   num_dets       = p_rdu_data->p_det_data->num_af_det;
   num_stat_dets  = p_rdu_internals->num_stationary_dets;
   bool ret_value = true;

   /* Initialize accumulators for normal equations */
   A2[0][0] = 0.0f;
   A2[0][1] = 0.0f;
   A2[1][0] = 0.0f;
   A2[1][1] = 0.0f;
   Atrr[0]  = 0.0f;
   Atrr[1]  = 0.0f;
   min_rr   = p_rdu_params->min_range_rate;
   vua      = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);

   /* if no stationary detections */
   if (num_stat_dets == 0U)
   {
      p_rdu_internals->v_xy_delta.vx = 0.0f;
      p_rdu_internals->v_xy_delta.vy = 0.0f;
   }
   else
   {
      for (uint16_t i = 0; i < num_dets; i++)
      {
         /* Process only stationary detections */
         if (p_rdu_internals->f_stationary_det[i] == true)
         {
            rr_delta    = p_rdu_internals->cur_vel[i] - p_rdu_internals->reference_rr_mod[i];
            rr_measured = p_rdu_internals->cur_vel[i];
            threshold   = p_rdu_internals->stage1_moving_det_thold[i];

            is_upper_boundary = (rr_measured > (min_rr + vua - threshold));
            is_lower_boundary = (rr_measured < (min_rr + threshold));

            if (is_lower_boundary)
            {
               /* rr_cases_lower_boundary = [rr_delta, rr_delta + vua] */
               vua_adjusted_rr = rr_delta + vua;
               if (rdu_absf(vua_adjusted_rr) < rdu_absf(rr_delta))
               {
                  rr_delta = vua_adjusted_rr;
               }
            }
            else
            {
               /* not a boundary case */
            }
            if (is_upper_boundary)
            {
               /* rr_cases_upper_boundary = [rr_delta, rr_delta - vua] */
               vua_adjusted_rr = rr_delta - vua;
               if (rdu_absf(vua_adjusted_rr) < rdu_absf(rr_delta))
               {
                  rr_delta = vua_adjusted_rr;
               }
            }
            else
            {
               /* not boundary case */
            }

            /*
             * A' * A = [sum(cos^2), sum(cos*sin); sum(cos*sin), sum(sin^2)]
             * -A' * rr_delta = [-sum(cos * rr); -sum(sin * rr)] */

            cos_t = p_rdu_internals->cos_theta[i];
            sin_t = p_rdu_internals->sin_theta[i];

            A2[0][0] += cos_t * cos_t;
            A2[0][1] += cos_t * sin_t;
            A2[1][0] += cos_t * sin_t;
            A2[1][1] += sin_t * sin_t;

            Atrr[0] += cos_t * rr_delta;
            Atrr[1] += sin_t * rr_delta;
         }
      }

      /* -A' * rr_delta */
      Atrr[0] = -Atrr[0];
      Atrr[1] = -Atrr[1];

      /* A2 = (A' * A) + diag([tikh_vx, tikh_vy]) */
      A2[0][0] += p_rdu_params->tikh_vx;
      A2[1][1] += p_rdu_params->tikh_vy;

      /* Finding inverse of 2x2 matrix
       * A2 = [a b; c d], inv(A2) = (1/det) * [d, -b; -c, a]
       * det = a*d - b*c
       */
      a   = A2[0][0];
      b   = A2[0][1];
      c   = A2[1][0];
      d   = A2[1][1];
      det = a * d - b * c;
      /* Check for small value as determinant */
      if (rdu_absf(det) < RDU_DET_EPSILON)
      {
         p_rdu_internals->v_xy_delta.vx = 0.0f;
         p_rdu_internals->v_xy_delta.vy = 0.0f;
         ret_value                      = false;
      }
      else
      {
         inv_det = rdu_finv(det);
         /* v_xy_delta = inv(A2) * Atrr */
         p_rdu_internals->v_xy_delta.vx = inv_det * (d * Atrr[0] - b * Atrr[1]);
         p_rdu_internals->v_xy_delta.vy = inv_det * (-c * Atrr[0] + a * Atrr[1]);
      }
   }

   return ret_value;
#endif
}

/******************************************************************************
 * Name:  Stationary_Det_Classifier
 *   This function computes Reference RR, Moving detection threaholds,
 * applies threshold comparison and classifies detections as
 * stationary or not
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 *
 * Return Value: bool
 *
 * Matlab Function Reference: Mod_Vua_stationary_classification()
 ******************************************************************************/
void Stationary_Det_Classifier(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
{
   float32_t vx_scs                       = p_rdu_data->p_rdu_stage1_output_data->stage1_vx_scs;
   float32_t vy_scs                       = p_rdu_data->p_rdu_stage1_output_data->stage1_vy_scs;
   uint16_t num_af_dets                   = p_rdu_data->p_det_data->num_af_det;
   float32_t *restrict p_moving_det_thold = &p_rdu_internals->stage1_moving_det_thold[0];
   float32_t *restrict p_cos_theta        = &p_rdu_internals->cos_theta[0];
   float32_t *restrict p_sin_theta        = &p_rdu_internals->sin_theta[0];
   float32_t *restrict p_reference_rr     = &p_rdu_internals->reference_rr[0];
   float32_t *restrict p_reference_rr_mod = &p_rdu_internals->reference_rr_mod[0];
   bool *restrict p_f_valid_det           = &p_rdu_internals->valid_det_flag[0];
   float32_t *restrict p_range_rate_diff  = &p_rdu_internals->range_rate_diff[0];
   float32_t *restrict p_range_rate       = &p_rdu_internals->cur_vel[0];
   bool *restrict p_f_stationary_det      = &p_rdu_internals->f_stationary_det[0];
   float32_t vua                          = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);

   (void)Compute_Ref_RR(num_af_dets, vx_scs, vy_scs, p_reference_rr, p_cos_theta, p_sin_theta);
   (void)Calc_Improved_Moving_Det_Thold(num_af_dets, vx_scs, vy_scs, p_cos_theta, p_sin_theta, p_moving_det_thold, p_rdu_params);
   p_rdu_internals->num_stationary_dets =
      Apply_Stationary_Threshold(num_af_dets, p_reference_rr, p_reference_rr_mod, p_f_stationary_det, p_f_valid_det,
                                 p_range_rate_diff, p_range_rate, p_moving_det_thold, p_rdu_params, vua);
}

/******************************************************************************
 * Name:  Stationary_Det_Classifier_Vel_Correction
 *   This function computes velocity correction factor for low vehicle speed case
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 *
 * Return Value: bool
 *
 * Matlab Code reference :
 *    [is_stationary_temp_cand,rr_ref_temp_cand,rr_ref_temp_cand_mod,T_temp,num_stationaries_temp_cand,rr_differences_temp_cand]=obj.mod_vua_stationary_classification(rr_all,valid_dets,cos_theta,sin_theta,refspeed_scs_temp_cand,minrr,vua);
 *
 *    if num_stationaries_temp_cand>num_stationaries&& norm(delta_vxy)<obj.paras.accepted_correction_threshold
 *        rr_ref=rr_ref_temp_cand;
 *        rr_ref_mod=rr_ref_temp_cand_mod;
 *        refspeed_scs=refspeed_scs_temp_cand;
 *        is_stationary(valid_dets)=is_stationary_temp_cand(valid_dets);
 *        T=T_temp;
 *        rr_differences=rr_differences_temp_cand;
 *    end
 ******************************************************************************/
void Stationary_Det_Classifier_Vel_Correction(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
{
   float32_t vx_scs     = p_rdu_data->p_rdu_stage1_output_data->stage1_vx_scs;
   float32_t vy_scs     = p_rdu_data->p_rdu_stage1_output_data->stage1_vy_scs;
   uint16_t num_af_dets = p_rdu_data->p_det_data->num_af_det;

   uint16_t num_stationary_dets_temp = 0U;

   float32_t *restrict p_moving_det_thold = &p_rdu_internals->moving_det_thold_temp[0];
   float32_t *restrict p_reference_rr     = &p_rdu_internals->reference_rr_temp[0];
   float32_t *restrict p_reference_rr_mod = &p_rdu_internals->reference_rr_mod_temp[0];
   float32_t *restrict p_range_rate_diff  = &p_rdu_internals->range_rate_diff_temp[0];
   bool *restrict p_f_stationary_det      = &p_rdu_internals->f_stationary_det_temp[0];
   float32_t *restrict p_range_rate       = &p_rdu_internals->cur_vel[0];
   float32_t *restrict p_cos_theta        = &p_rdu_internals->cos_theta[0];
   float32_t *restrict p_sin_theta        = &p_rdu_internals->sin_theta[0];
   bool *restrict p_f_valid_det           = &p_rdu_internals->valid_det_flag[0];
   float32_t vua                          = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);

   vx_scs = p_rdu_data->p_rdu_stage1_output_data->stage1_vx_scs + p_rdu_internals->v_xy_delta.vx;
   vy_scs = p_rdu_data->p_rdu_stage1_output_data->stage1_vy_scs + p_rdu_internals->v_xy_delta.vy;

   (void)Compute_Ref_RR(num_af_dets, vx_scs, vy_scs, p_reference_rr, p_cos_theta, p_sin_theta);
   (void)Calc_Improved_Moving_Det_Thold(num_af_dets, vx_scs, vy_scs, p_cos_theta, p_sin_theta, p_moving_det_thold, p_rdu_params);
   num_stationary_dets_temp =
      Apply_Stationary_Threshold(num_af_dets, p_reference_rr, p_reference_rr_mod, p_f_stationary_det, p_f_valid_det,
                                 p_range_rate_diff, p_range_rate, p_moving_det_thold, p_rdu_params, vua);

   /* Update only if number of stationary detections increased */
   if (num_stationary_dets_temp > p_rdu_internals->num_stationary_dets)
   {
      p_rdu_internals->num_stationary_dets                = num_stationary_dets_temp;
      p_rdu_data->p_rdu_stage1_output_data->stage1_vx_scs = vx_scs; /* corrected velocity vector */
      p_rdu_data->p_rdu_stage1_output_data->stage1_vy_scs = vy_scs; /* corrected velocity vector */
#if BBE_ENABLE
      p_rdu_internals->num_stationary_dets                = num_stationary_dets_temp;
      p_rdu_data->p_rdu_stage1_output_data->stage1_vx_scs = vx_scs; /* corrected velocity vector */
      p_rdu_data->p_rdu_stage1_output_data->stage1_vy_scs = vy_scs;
      /* corrected velocity vector */ /* Copy temporary results to main output structure */
      (void)vector_copy_unaligned(&p_rdu_internals->stage1_moving_det_thold[0], &p_rdu_internals->moving_det_thold_temp[0],
                                  num_af_dets * sizeof(float32_t));
      (void)vector_copy_unaligned(&p_rdu_internals->range_rate_diff[0], &p_rdu_internals->range_rate_diff_temp[0],
                                  num_af_dets * sizeof(float32_t));
      (void)vector_copy_unaligned(&p_rdu_internals->reference_rr[0], &p_rdu_internals->reference_rr_temp[0],
                                  num_af_dets * sizeof(float32_t));
      (void)vector_copy_unaligned(&p_rdu_internals->reference_rr_mod[0], &p_rdu_internals->reference_rr_mod_temp[0],
                                  num_af_dets * sizeof(float32_t));

#else
      memcpy(&p_rdu_internals->stage1_moving_det_thold[0], &p_rdu_internals->moving_det_thold_temp[0],
             num_af_dets * sizeof(float32_t));
      memcpy(&p_rdu_internals->range_rate_diff[0], &p_rdu_internals->range_rate_diff_temp[0], num_af_dets * sizeof(float32_t));
      memcpy(&p_rdu_internals->reference_rr[0], &p_rdu_internals->reference_rr_temp[0], num_af_dets * sizeof(float32_t));
      memcpy(&p_rdu_internals->reference_rr_mod[0], &p_rdu_internals->reference_rr_mod_temp[0], num_af_dets * sizeof(float32_t));
#endif
      memcpy(&p_rdu_internals->f_stationary_det[0], &p_rdu_internals->f_stationary_det_temp[0], num_af_dets * sizeof(bool));
   }
}

/******************************************************************************
 * Name:  Check_Stationary_Classification_Boundary_Cases
 *   This function identifies and handles boundary cases that occur when the
 * reference range rate is near the velocity unambiguity boundaries
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 *
 * Return Value: bool
 *
 * Matlab Function Reference: check_stationary_mod_boundary_cases()
 * ******************************************************************************/
void Check_Stationary_Classification_Boundary_Cases(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params,
                                                    RDU_Internals_T *p_rdu_internals)
{
#ifdef BBE_ENABLE
   float32_t min_rr  = p_rdu_params->min_range_rate;
   float32_t vua     = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);
   float32_t vua_inv = rdu_finv(vua);

   float32_t *restrict p_reference_rr         = &p_rdu_internals->reference_rr[0];
   float32_t *restrict p_reference_rr_mod     = &p_rdu_internals->reference_rr_mod[0];
   float32_t *restrict p_range_rate           = &p_rdu_internals->cur_vel[0];
   bool *restrict p_valid_det_flag            = &p_rdu_internals->valid_det_flag[0];
   float32_t *restrict p_range_rate_diff      = &p_rdu_internals->range_rate_diff[0];
   float32_t *restrict p_moving_det_threshold = &p_rdu_internals->stage1_moving_det_thold[0];
   bool *restrict p_f_stationary_det          = &p_rdu_internals->f_stationary_det[0];

   uint16_t num_af_dets = p_rdu_data->p_det_data->num_af_det;
   uint16_t vec_width   = VEC_WIDTH;
   uint16_t rem         = num_af_dets & (vec_width - 1U);
   uint16_t num_vec     = num_af_dets / vec_width;

   xb_vecN_2xf32 v_min_rr  = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(min_rr), 0);
   xb_vecN_2xf32 v_vua     = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vua), 0);
   xb_vecN_2xf32 v_vua_inv = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vua_inv), 0);

   for (uint16_t i_vec = 0U; i_vec < num_vec; i_vec++)
   {
      uint16_t base_idx          = i_vec * vec_width;
      xb_vecN_2xf32 v_ref_rr     = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_reference_rr + base_idx), 0);
      xb_vecN_2xf32 v_ref_rr_mod = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_reference_rr_mod + base_idx), 0);
      // Do aligned load of the velocity vector since it is in unaligned IPC data stream
      xb_vecN_2xf32 *restrict p_valign;
      p_valign          = (xb_vecN_2xf32 *)(&p_range_rate[base_idx]);
      valign v_rr_align = BBE_LAN_2XF32_PP(p_valign);
      xb_vecN_2xf32 v_rr;
      BBE_LAN_2XF32_IP(v_rr, v_rr_align, p_valign);
      xb_vecN_2xf32 v_threshold = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_moving_det_threshold + base_idx), 0);

      xb_vecNx16U *restrict vec_p_valid_flag = (xb_vecNx16U *)(&p_valid_det_flag[base_idx]);
      valign bb_valid_align                  = BBE_LANX16U_PP(vec_p_valid_flag);
      xb_vecNx16U vec_valid_flag_8bit;
      BBE_LAVNX16U_XP(vec_valid_flag_8bit, bb_valid_align, vec_p_valid_flag, 8);
      xb_vecNx16U vec_lo_8bit_sel_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
      vec_valid_flag_8bit =
         BBE_SELNX16UI((vec_valid_flag_8bit >> 8U), vec_valid_flag_8bit, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_sel_mask;
      xb_vecN_2x32Uv vec_valid_flag_32bit =
         BBE_SELN_2X32UI(BBE_ZERON_2X32U(), BBE_MOVN_2X32U_FROMNX16(vec_valid_flag_8bit), BBE_SELI_INTERLEAVE_1_LO);
      vboolN_2 valid_mask = vec_valid_flag_32bit != BBE_ZERON_2X32U();

      xb_vecN_2xf32 ref_minus_min = BBE_SUBN_2XF32(v_ref_rr, v_min_rr);
      xb_vecN_2xf32 upper_bound   = BBE_ADDN_2XF32(ref_minus_min, v_threshold);
      xb_vecN_2xf32 upper_div     = BBE_MULN_2XF32(upper_bound, v_vua_inv);
      xb_vecN_2xf32 upper_floor   = BBE_FIFLOORN_2XF32(upper_div);
      xb_vecN_2xf32 lower_bound   = BBE_SUBN_2XF32(ref_minus_min, v_threshold);
      xb_vecN_2xf32 lower_div     = BBE_MULN_2XF32(lower_bound, v_vua_inv);
      xb_vecN_2xf32 lower_floor   = BBE_FIFLOORN_2XF32(lower_div);
      vboolN_2 boundary_mask      = BBE_OLTN_2XF32(lower_floor, upper_floor);
      vboolN_2 process_mask       = valid_mask & boundary_mask;

      xb_vecN_2xf32 ref_mod_minus_vua = BBE_SUBN_2XF32(v_ref_rr_mod, v_vua);
      float32_t rr_minus_buf[8U];
      float32_t mod_minus_buf[8U];
      BBE_SVN_2XF32_I(ref_mod_minus_vua, (xb_vecN_2xf32 *)rr_minus_buf, 0);
      Mod_Vua_Vec(rr_minus_buf, min_rr, vua, vua_inv, mod_minus_buf);
      xb_vecN_2xf32 mod_result_minus = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)mod_minus_buf, 0);
      xb_vecN_2xf32 diff_minus       = BBE_SUBN_2XF32(v_rr, mod_result_minus);
      xb_vecN_2xf32 rr_diff_minus;
      BBE_ABSN_2XF32T(rr_diff_minus, diff_minus, process_mask);
      vboolN_2 is_stat_minus = BBE_OLTN_2XF32T(rr_diff_minus, v_threshold, process_mask);

      xb_vecN_2xf32 ref_mod_plus_vua = BBE_ADDN_2XF32(v_ref_rr_mod, v_vua);
      float32_t rr_plus_buf[8U];
      float32_t mod_plus_buf[8U];
      BBE_SVN_2XF32_I(ref_mod_plus_vua, (xb_vecN_2xf32 *)rr_plus_buf, 0);
      Mod_Vua_Vec(rr_plus_buf, min_rr, vua, vua_inv, mod_plus_buf);
      xb_vecN_2xf32 mod_result_plus = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)mod_plus_buf, 0);
      xb_vecN_2xf32 diff_plus       = BBE_SUBN_2XF32(v_rr, mod_result_plus);
      xb_vecN_2xf32 rr_diff_plus;
      BBE_ABSN_2XF32T(rr_diff_plus, diff_plus, process_mask);
      vboolN_2 is_stat_plus = BBE_OLTN_2XF32T(rr_diff_plus, v_threshold, process_mask);
      //  OR logic
      xb_vecN_2xf32 v01 = BBE_MOVN_2XF32T((xb_vecN_2xf32)1.0f, BBE_ZERON_2XF32(), is_stat_minus);
      xb_vecN_2xf32 v02 = BBE_MOVN_2XF32T((xb_vecN_2xf32)1.0f, BBE_ZERON_2XF32(), is_stat_plus);
      float32_t stat01_buf[8U];
      float32_t stat02_buf[8U];
      BBE_SVN_2XF32_I(v01, (xb_vecN_2xf32 *)stat01_buf, 0);
      BBE_SVN_2XF32_I(v02, (xb_vecN_2xf32 *)stat02_buf, 0);
      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t idx       = base_idx + lane;
         const bool is_stationary = (stat01_buf[lane] > 0.5f) || (stat02_buf[lane] > 0.5f);
         p_f_stationary_det[idx]  = p_f_stationary_det[idx] || (is_stationary);
      }
      //
      xb_vecN_2xf32 v_rr_diff_orig = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_range_rate_diff + base_idx), 0);
      vboolN_2 use_minus           = process_mask & is_stat_minus;
      BBE_MOVN_2XF32T(v_rr_diff_orig, rr_diff_minus, use_minus);
      vboolN_2 use_plus = process_mask & is_stat_plus;

      BBE_MOVN_2XF32T(v_rr_diff_orig, rr_diff_plus, use_plus);
      BBE_SVN_2XF32_I(v_rr_diff_orig, (xb_vecN_2xf32 *)(p_range_rate_diff + base_idx), 0);
   }

   if (rem != 0U)
   {
      const uint16_t base_idx = num_vec * vec_width;
      uint32_t __attribute__((aligned(32))) valid_mask_arr[VEC_WIDTH];

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t idx   = base_idx + lane;
         valid_mask_arr[lane] = ((idx < num_af_dets) && (p_valid_det_flag[idx] == true)) ? 0xFU : 0x0U;
      }

      xb_vecN_2xf32 v_ref_rr     = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_reference_rr + base_idx), 0);
      xb_vecN_2xf32 v_ref_rr_mod = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_reference_rr_mod + base_idx), 0);
      xb_vecN_2xf32 *restrict p_valign;
      p_valign          = (xb_vecN_2xf32 *)(&p_range_rate[base_idx]);
      valign v_rr_align = BBE_LAN_2XF32_PP(p_valign);
      xb_vecN_2xf32 v_rr;
      BBE_LAN_2XF32_IP(v_rr, v_rr_align, p_valign);
      xb_vecN_2xf32 v_threshold = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_moving_det_threshold + base_idx), 0);
      vboolN_2 valid_mask       = *((xb_vecN_2x32Uv *)valid_mask_arr) != BBE_ZERON_2X32U();

      xb_vecN_2xf32 ref_minus_min = BBE_SUBN_2XF32(v_ref_rr, v_min_rr);
      xb_vecN_2xf32 upper_bound   = BBE_ADDN_2XF32(ref_minus_min, v_threshold);
      xb_vecN_2xf32 upper_div     = BBE_MULN_2XF32(upper_bound, v_vua_inv);
      xb_vecN_2xf32 upper_floor   = BBE_FIFLOORN_2XF32(upper_div);
      xb_vecN_2xf32 lower_bound   = BBE_SUBN_2XF32(ref_minus_min, v_threshold);
      xb_vecN_2xf32 lower_div     = BBE_MULN_2XF32(lower_bound, v_vua_inv);
      xb_vecN_2xf32 lower_floor   = BBE_FIFLOORN_2XF32(lower_div);
      vboolN_2 boundary_mask      = BBE_OLTN_2XF32(lower_floor, upper_floor);
      vboolN_2 process_mask       = valid_mask & boundary_mask;

      xb_vecN_2xf32 ref_mod_minus_vua = BBE_SUBN_2XF32(v_ref_rr_mod, v_vua);
      float32_t rr_minus_buf[8U];
      float32_t mod_minus_buf[8U];
      BBE_SVN_2XF32_I(ref_mod_minus_vua, (xb_vecN_2xf32 *)rr_minus_buf, 0);
      Mod_Vua_Vec(rr_minus_buf, min_rr, vua, vua_inv, mod_minus_buf);
      xb_vecN_2xf32 mod_result_minus = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)mod_minus_buf, 0);
      xb_vecN_2xf32 diff_minus       = BBE_SUBN_2XF32(v_rr, mod_result_minus);
      xb_vecN_2xf32 rr_diff_minus;
      BBE_ABSN_2XF32T(rr_diff_minus, diff_minus, process_mask);
      vboolN_2 is_stat_minus = BBE_OLTN_2XF32T(rr_diff_minus, v_threshold, process_mask);

      xb_vecN_2xf32 ref_mod_plus_vua = BBE_ADDN_2XF32(v_ref_rr_mod, v_vua);
      float32_t rr_plus_buf[8U];
      float32_t mod_plus_buf[8U];
      BBE_SVN_2XF32_I(ref_mod_plus_vua, (xb_vecN_2xf32 *)rr_plus_buf, 0);
      Mod_Vua_Vec(rr_plus_buf, min_rr, vua, vua_inv, mod_plus_buf);
      xb_vecN_2xf32 mod_result_plus = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)mod_plus_buf, 0);
      xb_vecN_2xf32 diff_plus       = BBE_SUBN_2XF32(v_rr, mod_result_plus);
      xb_vecN_2xf32 rr_diff_plus;
      BBE_ABSN_2XF32T(rr_diff_plus, diff_plus, process_mask);
      vboolN_2 is_stat_plus = BBE_OLTN_2XF32T(rr_diff_plus, v_threshold, process_mask);
      xb_vecN_2xf32 v01     = BBE_MOVN_2XF32T((xb_vecN_2xf32)1.0f, BBE_ZERON_2XF32(), is_stat_minus);
      xb_vecN_2xf32 v02     = BBE_MOVN_2XF32T((xb_vecN_2xf32)1.0f, BBE_ZERON_2XF32(), is_stat_plus);
      float32_t stat01_buf[8U];
      float32_t stat02_buf[8U];
      BBE_SVN_2XF32_I(v01, (xb_vecN_2xf32 *)stat01_buf, 0);
      BBE_SVN_2XF32_I(v02, (xb_vecN_2xf32 *)stat02_buf, 0);
      for (uint16_t lane = 0U; lane < rem; lane++)
      {
         const bool is_stationary            = (stat01_buf[lane] > 0.5f) || (stat02_buf[lane] > 0.5f);
         p_f_stationary_det[base_idx + lane] = p_f_stationary_det[base_idx + lane] || is_stationary;
      }

      xb_vecN_2xf32 v_rr_diff_orig = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_range_rate_diff + base_idx), 0);
      vboolN_2 use_minus           = process_mask & is_stat_minus;
      BBE_MOVN_2XF32T(v_rr_diff_orig, rr_diff_minus, use_minus);
      vboolN_2 use_plus = process_mask & is_stat_plus;
      BBE_MOVN_2XF32T(v_rr_diff_orig, rr_diff_plus, use_plus);
      BBE_SVN_2XF32T_I(v_rr_diff_orig, (xb_vecN_2xf32 *)(p_range_rate_diff + base_idx), 0, valid_mask);
   }
#else
   float32_t min_rr                           = p_rdu_params->min_range_rate;
   float32_t vua                              = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);
   float32_t *restrict p_reference_rr         = &p_rdu_internals->reference_rr[0];
   float32_t *restrict p_reference_rr_mod     = &p_rdu_internals->reference_rr_mod[0];
   float32_t *restrict p_range_rate           = &p_rdu_internals->cur_vel[0];
   bool *restrict p_valid_det_flag            = &p_rdu_internals->valid_det_flag[0];
   float32_t *restrict p_range_rate_diff      = &p_rdu_internals->range_rate_diff[0];
   float32_t *restrict p_moving_det_threshold = &p_rdu_internals->stage1_moving_det_thold[0];
   bool *restrict p_f_stationary_det          = &p_rdu_internals->f_stationary_det[0];
   float32_t rr_diff_minus                    = 0.0F;
   float32_t rr_diff_plus                     = 0.0F;
   bool is_boundary_case                      = false;
   bool is_stationary_minus                   = false;
   bool is_stationary_plus                    = false;

   float32_t vua_inv = rdu_finv(vua);

   uint16_t num_af_dets = p_rdu_data->p_det_data->num_af_det;

   for (uint16_t i = 0; i < num_af_dets; i++)
   {
      is_boundary_case = (rdu_floorf((p_reference_rr[i] - min_rr + p_moving_det_threshold[i]) * vua_inv) >
                          rdu_floorf((p_reference_rr[i] - min_rr - p_moving_det_threshold[i]) * vua_inv));
      if ((p_valid_det_flag[i] == true) && (is_boundary_case == true))
      {
         /* Check rr_ref_mod - vua interval */
         rr_diff_minus       = rdu_absf(p_range_rate[i] - Mod_Vua((p_reference_rr_mod[i] - vua), min_rr, vua, vua_inv));
         is_stationary_minus = (rr_diff_minus < p_moving_det_threshold[i]) ? true : false;

         /* Check rr_ref_mod + vua interval */
         rr_diff_plus       = rdu_absf(p_range_rate[i] - Mod_Vua((p_reference_rr_mod[i] + vua), min_rr, vua, vua_inv));
         is_stationary_plus = (rr_diff_plus < p_moving_det_threshold[i]) ? true : false;

         /* Update stationary classification (OR logic) */
         p_f_stationary_det[i] = p_f_stationary_det[i] || is_stationary_minus || is_stationary_plus;

         /* Update differences for detections that passed in the boundary intervals */
         if (is_stationary_plus == true)
         {
            p_range_rate_diff[i] = rr_diff_plus;
         }
         else if (is_stationary_minus == true)
         {
            p_range_rate_diff[i] = rr_diff_minus;
         }
         else
         {
            /* not a boundary case */
         }
      }
   }
#endif
}

/******************************************************************************
 * Name:  Compute_Classification_Confidence_Values
 *   This function computes confidence score of classification based on
 * range rate differences and thresholds
 *
 * Shared Variables: none
 *
 * Parameters:  [in/out] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 *
 * Return Value: bool
 *
 * Matlab Function Reference: compute_classification_confidence()
 ******************************************************************************/
void Compute_Classification_Confidence_Values(RDU_Data_T *p_rdu_data, RDU_Internals_T *p_rdu_internals)
{
#ifdef BBE_ENABLE
   const uint16_t num_af_dets       = p_rdu_data->p_det_data->num_af_det;
   float32_t *p_confidence          = &p_rdu_data->p_rdu_stage1_output_data->stage1_classification_confidence[0];
   const float32_t *p_threshold     = &p_rdu_internals->stage1_moving_det_thold[0];
   bool *restrict p_valid           = &p_rdu_internals->valid_det_flag[0];
   const float32_t *p_diff          = &p_rdu_internals->range_rate_diff[0];
   const bool *p_f_stationary       = &p_rdu_internals->f_stationary_det[0];
   static const float32_t one_sixth = 1.0F / 6.0F; /* precompute at compile time */

   float32_t z_adj = 0.0F;

   const uint16_t vec_width     = 8U;
   const uint16_t rem           = num_af_dets & (vec_width - 1U);
   const uint16_t num_vec_iters = num_af_dets / vec_width;

   const xb_vecN_2xf32 v_three     = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(3.0f), 0);
   const xb_vecN_2xf32 v_one       = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(1.0f), 0);
   const xb_vecN_2xf32 v_one_sixth = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(one_sixth), 0);

   for (uint16_t i_vec = 0U; i_vec < num_vec_iters; i_vec++)
   {
      const uint16_t idx = i_vec * vec_width;
      // Stationary mask conversion
      xb_vecNx16U *restrict vec_p_stat_flag = (xb_vecNx16U *)(&p_f_stationary[idx]);
      valign bb_stat_align                  = BBE_LANX16U_PP(vec_p_stat_flag);
      xb_vecNx16U vec_stat_flag_8bit;
      BBE_LAVNX16U_XP(vec_stat_flag_8bit, bb_stat_align, vec_p_stat_flag, 8);
      xb_vecNx16U vec_lo_8bit_sel_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
      vec_stat_flag_8bit =
         BBE_SELNX16UI((vec_stat_flag_8bit >> 8U), vec_stat_flag_8bit, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_sel_mask;
      xb_vecN_2x32Uv vec_stat_flag_32bit =
         BBE_SELN_2X32UI(BBE_ZERON_2X32U(), BBE_MOVN_2X32U_FROMNX16(vec_stat_flag_8bit), BBE_SELI_INTERLEAVE_1_LO);
      vboolN_2 stat_mask = vec_stat_flag_32bit != BBE_ZERON_2X32U();

      uint32_t __attribute__((aligned(32))) active_mask_arr[VEC_WIDTH];
      float32_t __attribute__((aligned(32))) threshold_safe_buf[VEC_WIDTH];
      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t base_idx  = idx + lane;
         const bool lane_active   = (base_idx < num_af_dets) && (p_valid[base_idx] == true) && (p_threshold[base_idx] != 0.0F);
         active_mask_arr[lane]    = lane_active ? 0xFU : 0x0U;
         threshold_safe_buf[lane] = lane_active ? p_threshold[base_idx] : 1.0F;
      }

      vboolN_2 active_mask = *((xb_vecN_2x32Uv *)active_mask_arr) != BBE_ZERON_2X32U();

      vboolN_2 active_stat_mask = BBE_ANDBN_2(stat_mask, active_mask);

      xb_vecN_2xf32 v_conf = (xb_vecN_2xf32)p_confidence[idx];
      v_conf               = BBE_ZERON_2XF32();

      xb_vecN_2xf32 v_diff      = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_diff + idx), 0);
      xb_vecN_2xf32 v_threshold = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)threshold_safe_buf, 0);
      xb_vecN_2xf32 v_num       = BBE_MULN_2XF32(v_diff, v_three);
      xb_vecN_2xf32 v_z         = BBE_DIVN_2XF32(v_num, v_threshold);

      BBE_SUBN_2XF32T(v_conf, v_one, BBE_MULN_2XF32(v_z, v_one_sixth), active_stat_mask); // Stationary: 1 - 1/6*z
      vboolN_2 v_z_mask_1  = BBE_OLTN_2XF32(v_three, v_z);                                // z > 3
      vboolN_2 moving_mask = BBE_ANDBN_2(~stat_mask, active_mask);
      // For moving detections:
      // (1) z >= 3: v_z_mask_1 & moving_mask → use exp decay (handled below in scalar tail)
      // (2) z < 3: (~v_z_mask_1) & moving_mask → set to 0.5
      BBE_MOVN_2XF32T(v_conf, BBE_MOVN_2XF32_FROMF32(0.5f), (~v_z_mask_1) & moving_mask);
      // (3) Optionally, for z >= 3 & moving, you may want to set a marker or handle in scalar tail (or keep as is for now)

      float32_t conf_buf[8U];
      float32_t z_buf[8U];
      BBE_SVN_2XF32_I(v_conf, (xb_vecN_2xf32 *)conf_buf, 0);
      BBE_SVN_2XF32_I(v_z, (xb_vecN_2xf32 *)z_buf, 0);

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t base_idx = idx + lane;
         if ((base_idx >= num_af_dets) || (p_valid[base_idx] == false) || (p_threshold[base_idx] == 0.0F))
         {
            if (base_idx < num_af_dets)
            {
               p_confidence[base_idx] = 0.0F;
            }
         }
         else if (p_f_stationary[base_idx])
         {
            p_confidence[base_idx] = conf_buf[lane];
         }
         else
         {
            if (z_buf[lane] < 3.0f)
            {
               p_confidence[base_idx] = 0.5f;
            }
            else
            {
               z_adj                  = z_buf[lane] - 3.0f;
               p_confidence[base_idx] = 0.5f * rdu_expf(-0.5f * z_adj);
            }
         }
      }
   }

   if (rem != 0U)
   {
      const uint16_t idx = num_vec_iters * vec_width;
      uint32_t __attribute__((aligned(32))) stat_mask_arr[VEC_WIDTH];
      uint32_t __attribute__((aligned(32))) active_mask_arr[VEC_WIDTH];
      float32_t __attribute__((aligned(32))) threshold_safe_buf[VEC_WIDTH];
      float32_t __attribute__((aligned(32))) diff_safe_buf[VEC_WIDTH];

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t base_idx  = idx + lane;
         const bool in_range      = (base_idx < num_af_dets);
         const bool is_stationary = in_range && (p_f_stationary[base_idx] == true);
         const bool lane_active   = in_range && (p_valid[base_idx] == true) && (p_threshold[base_idx] != 0.0F);
         stat_mask_arr[lane]      = is_stationary ? 0xFU : 0x0U;
         active_mask_arr[lane]    = lane_active ? 0xFU : 0x0U;
         threshold_safe_buf[lane] = lane_active ? p_threshold[base_idx] : 1.0F;
         diff_safe_buf[lane]      = in_range ? p_diff[base_idx] : 0.0F;
      }

      vboolN_2 stat_mask        = *((xb_vecN_2x32Uv *)stat_mask_arr) != BBE_ZERON_2X32U();
      vboolN_2 active_mask      = *((xb_vecN_2x32Uv *)active_mask_arr) != BBE_ZERON_2X32U();
      vboolN_2 active_stat_mask = BBE_ANDBN_2(stat_mask, active_mask);

      xb_vecN_2xf32 v_conf      = BBE_ZERON_2XF32();
      xb_vecN_2xf32 v_diff      = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)diff_safe_buf, 0);
      xb_vecN_2xf32 v_threshold = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)threshold_safe_buf, 0);
      xb_vecN_2xf32 v_num       = BBE_MULN_2XF32(v_diff, v_three);
      xb_vecN_2xf32 v_z         = BBE_DIVN_2XF32(v_num, v_threshold);

      BBE_SUBN_2XF32T(v_conf, v_one, BBE_MULN_2XF32(v_z, v_one_sixth), active_stat_mask);
      vboolN_2 v_z_mask_1  = BBE_OLTN_2XF32(v_three, v_z);
      vboolN_2 moving_mask = BBE_ANDBN_2(~stat_mask, active_mask);
      BBE_MOVN_2XF32T(v_conf, BBE_MOVN_2XF32_FROMF32(0.5f), (~v_z_mask_1) & moving_mask);

      float32_t conf_buf[8U];
      float32_t z_buf[8U];
      BBE_SVN_2XF32_I(v_conf, (xb_vecN_2xf32 *)conf_buf, 0);
      BBE_SVN_2XF32_I(v_z, (xb_vecN_2xf32 *)z_buf, 0);

      for (uint16_t lane = 0U; lane < rem; lane++)
      {
         const uint16_t base_idx = idx + lane;
         if ((p_valid[base_idx] == false) || (p_threshold[base_idx] == 0.0F))
         {
            p_confidence[base_idx] = 0.0F;
         }
         else if (p_f_stationary[base_idx])
         {
            p_confidence[base_idx] = conf_buf[lane];
         }
         else if (z_buf[lane] < 3.0f)
         {
            p_confidence[base_idx] = 0.5f;
         }
         else
         {
            z_adj                  = z_buf[lane] - 3.0f;
            p_confidence[base_idx] = 0.5f * rdu_expf(-0.5f * z_adj);
         }
      }
   }
#else
   uint16_t num_af_dets             = p_rdu_data->p_det_data->num_af_det;
   float32_t threshold              = 0.0F;
   float32_t z_score                = 0.0F;
   float32_t confidence             = 0.0F;
   float32_t z_adj                  = 0.0F;
   static const float32_t one_sixth = 1.0F / 6.0F; /* precompute at compile time */
   for (uint16_t i = 0; i < num_af_dets; i++)
   {
      /* Initialize confidence to 0 for all detections */
      p_rdu_data->p_rdu_stage1_output_data->stage1_classification_confidence[i] = 0.0F;

      /* Only compute confidence for valid detections */

      threshold = p_rdu_internals->stage1_moving_det_thold[i];
      if (p_rdu_internals->valid_det_flag[i] == false || threshold == 0.0F)
      {
         continue;
      }
      z_score    = p_rdu_internals->range_rate_diff[i] * 3.0F * rdu_finv(threshold);
      confidence = 0.0F;

      if (p_rdu_internals->f_stationary_det[i] == true)
      {
         /* Stationary detection confidence: linear mapping from 1.0 (z=0) to 0.5 (z=3) */
         confidence = 1.0F - one_sixth * z_score; /* equivalent to 1.0 - 0.5 * (z_score / 3) */
      }
      else
      {
         /* Moving detection confidence: exponential decay from 0.5 (z=3) towards 0.0 */
         z_adj      = (z_score > 3.0F) ? (z_score - 3.0F) : 0.0F;
         confidence = 0.5F * rdu_expf(-0.5F * z_adj);
      }
      p_rdu_data->p_rdu_stage1_output_data->stage1_classification_confidence[i] = confidence;
   }
#endif
}

/******************************************************************************
 * Name:  Stationary_Unfolding
 *   This function unfolded RR of stationary detections
 *
 *
 * Shared Variables: none
 *
 * Parameters:  [in/out] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 *
 * Return Value: bool
 *
 * Matlab Function Reference: run_algorithm_single() from stationary_unfolding_classic
 ******************************************************************************/
void Stationary_Unfolding(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
{
#if BBE_ENABLE
   RDU_Stage1_Output_Data_T *restrict p_rdu_output_data = p_rdu_data->p_rdu_stage1_output_data;

   // AF_Det_T *restrict p_af_data                  = &p_rdu_data->p_det_data->af_data;
   uint16_t num_af_dets                       = p_rdu_data->p_det_data->num_af_det;
   bool *restrict p_f_stationary_det          = &p_rdu_internals->f_stationary_det[0];
   float32_t *restrict p_reference_rr         = &p_rdu_internals->reference_rr[0];
   bool *restrict p_valid_det_flag            = &p_rdu_internals->valid_det_flag[0];
   float32_t *restrict p_vel                  = &p_rdu_internals->cur_vel[0];
   int8_t *restrict p_k_factor                = &p_rdu_output_data->stage1_wrapping_k[0];
   float32_t *restrict p_rdu_unamb_range_rate = &p_rdu_internals->stage1_unamb_range_rate[0];
   int8_t *restrict p_rrr_motion_status       = &p_rdu_output_data->stage1_rrr_motion_status[0];

   uint16_t scan_idx                  = p_rdu_data->p_det_data->det_list_property.scanindex;
   uint16_t look_id                   = p_rdu_data->p_det_data->det_list_property.look_type;
   p_rdu_output_data->stage1_scan_idx = scan_idx;
   p_rdu_output_data->stage1_look_id  = look_id;

   /* Classic disambiguation without pattern  */
   float32_t vua_recip = 0.0F;
   float32_t vua       = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);
   vua_recip           = rdu_finv(vua);

   const uint16_t vec_width     = 8U;
   const uint16_t rem           = num_af_dets & (vec_width - 1U);
   const uint16_t num_vec_iters = num_af_dets / vec_width;

   xb_vecN_2xf32 K_min         = BBE_REPN_2XF32((float32_t)RDU_K_MIN, 0);
   xb_vecN_2xf32 K_max         = BBE_REPN_2XF32((float32_t)RDU_K_MAX, 0);
   xb_vecN_2xf32 vua_recip_vec = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vua_recip), 0);
   xb_vecN_2xf32 v_vua         = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(vua), 0);

   for (uint16_t i = 0; i < num_af_dets; i++)
   {
      p_k_factor[i]             = 0;
      p_rdu_unamb_range_rate[i] = 0.0F;
      p_rrr_motion_status[i]    = RDU_MOTION_STATUS_INVALID; /* default status is -1 : INVALID */
   }

   xb_vecN_2xf32 *p_rdu_unamb_rr = (xb_vecN_2xf32 *)p_rdu_unamb_range_rate;
   for (uint16_t i_vec = 0U; i_vec < num_vec_iters; i_vec++)
   {
      const uint16_t base_idx = i_vec * vec_width;
      xb_vecN_2xf32 v_ref_rr  = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_reference_rr + base_idx), 0);

      xb_vecN_2xf32 *restrict p_valign;
      p_valign           = (xb_vecN_2xf32 *)(&p_vel[base_idx]);
      valign v_vel_align = BBE_LAN_2XF32_PP(p_valign);
      xb_vecN_2xf32 v_vel;
      BBE_LAN_2XF32_IP(v_vel, v_vel_align, p_valign);

      // Stationary mask conversion
      xb_vecNx16U *restrict vec_p_stat_flag = (xb_vecNx16U *)(&p_f_stationary_det[base_idx]);
      valign bb_stat_align                  = BBE_LANX16U_PP(vec_p_stat_flag);
      xb_vecNx16U vec_stat_flag_8bit;
      BBE_LAVNX16U_XP(vec_stat_flag_8bit, bb_stat_align, vec_p_stat_flag, 8);
      xb_vecNx16U vec_lo_8bit_sel_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
      vec_stat_flag_8bit =
         BBE_SELNX16UI((vec_stat_flag_8bit >> 8U), vec_stat_flag_8bit, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_sel_mask;
      xb_vecN_2x32Uv vec_stat_flag_32bit =
         BBE_SELN_2X32UI(BBE_ZERON_2X32U(), BBE_MOVN_2X32U_FROMNX16(vec_stat_flag_8bit), BBE_SELI_INTERLEAVE_1_LO);
      vboolN_2 stat_mask        = vec_stat_flag_32bit != BBE_ZERON_2X32U();
      vboolN_2 active_stat_mask = stat_mask;

      xb_vecN_2xf32 v_diff    = BBE_SUBN_2XF32(v_ref_rr, v_vel);
      xb_vecN_2xf32 v_k_float = BBE_FIROUNDN_2XF32(BBE_MULN_2XF32(v_diff, vua_recip_vec));

      vboolN_2 k_below_min_mask = BBE_OLTN_2XF32(v_k_float, K_min);
      vboolN_2 k_above_max_mask = BBE_OLTN_2XF32(K_max, v_k_float);
      xb_vecN_2xf32 v_k_rounded = BBE_MOVN_2XF32T(K_max, v_k_float, k_above_max_mask & active_stat_mask);
      v_k_rounded               = BBE_MOVN_2XF32T(K_min, v_k_rounded, k_below_min_mask & active_stat_mask);
      v_k_rounded = BBE_MOVN_2XF32T(v_k_float, v_k_rounded, ~(k_below_min_mask | k_above_max_mask) & active_stat_mask);
      v_k_rounded = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_k_rounded, ~active_stat_mask);

      // Compute unambiguous range rate
      xb_vecN_2xf32 v_k_vua = BBE_MULN_2XF32(v_k_rounded, v_vua);
      xb_vecN_2xf32 v_unamb = BBE_ADDN_2XF32(v_vel, v_k_vua);
      BBE_SVN_2XF32T_IP(v_unamb, p_rdu_unamb_rr, VEC_SIZE, active_stat_mask);

      float32_t k_buf[VEC_WIDTH];
      BBE_SVN_2XF32_I(v_k_rounded, (xb_vecN_2xf32 *)k_buf, 0);

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t idx = base_idx + lane;

         if ((idx < num_af_dets) && (p_f_stationary_det[idx] == true))
         {
            p_k_factor[idx] = (int8_t)k_buf[lane];
         }
      }
   }

   if (rem != 0U)
   {
      const uint16_t base_idx = num_vec_iters * vec_width;
      uint32_t __attribute__((aligned(32))) stat_mask_arr[VEC_WIDTH];

      for (uint16_t lane = 0U; lane < vec_width; lane++)
      {
         const uint16_t idx  = base_idx + lane;
         stat_mask_arr[lane] = ((idx < num_af_dets) && (p_f_stationary_det[idx] == true)) ? 0xFU : 0x0U;
      }

      vboolN_2 active_stat_mask = *((xb_vecN_2x32Uv *)stat_mask_arr) != BBE_ZERON_2X32U();
      xb_vecN_2xf32 v_ref_rr    = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_reference_rr + base_idx), 0);

      xb_vecN_2xf32 *restrict p_valign;
      p_valign           = (xb_vecN_2xf32 *)(&p_vel[base_idx]);
      valign v_vel_align = BBE_LAN_2XF32_PP(p_valign);
      xb_vecN_2xf32 v_vel;
      BBE_LAN_2XF32_IP(v_vel, v_vel_align, p_valign);

      xb_vecN_2xf32 v_diff    = BBE_SUBN_2XF32(v_ref_rr, v_vel);
      xb_vecN_2xf32 v_k_float = BBE_FIROUNDN_2XF32(BBE_MULN_2XF32(v_diff, vua_recip_vec));

      vboolN_2 k_below_min_mask = BBE_OLTN_2XF32(v_k_float, K_min);
      vboolN_2 k_above_max_mask = BBE_OLTN_2XF32(K_max, v_k_float);
      xb_vecN_2xf32 v_k_rounded = BBE_MOVN_2XF32T(K_max, v_k_float, k_above_max_mask & active_stat_mask);
      v_k_rounded               = BBE_MOVN_2XF32T(K_min, v_k_rounded, k_below_min_mask & active_stat_mask);
      v_k_rounded = BBE_MOVN_2XF32T(v_k_float, v_k_rounded, ~(k_below_min_mask | k_above_max_mask) & active_stat_mask);
      v_k_rounded = BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), v_k_rounded, ~active_stat_mask);

      xb_vecN_2xf32 v_k_vua = BBE_MULN_2XF32(v_k_rounded, v_vua);
      xb_vecN_2xf32 v_unamb = BBE_ADDN_2XF32(v_vel, v_k_vua);
      BBE_SVN_2XF32T_IP(v_unamb, p_rdu_unamb_rr, VEC_SIZE, active_stat_mask);

      float32_t k_buf[VEC_WIDTH];
      BBE_SVN_2XF32_I(v_k_rounded, (xb_vecN_2xf32 *)k_buf, 0);

      for (uint16_t lane = 0U; lane < rem; lane++)
      {
         if (p_f_stationary_det[base_idx + lane] == true)
         {
            p_k_factor[base_idx + lane] = (int8_t)k_buf[lane];
         }
      }
   }

   for (uint16_t i = 0; i < num_af_dets; i++)
      /* Motion status of individual detections updated here */
      if (p_valid_det_flag[i] && !p_f_stationary_det[i])
      {
         p_rrr_motion_status[i] = RDU_MOTION_STATUS_MOVING;
      }
      else if (p_f_stationary_det[i])
      {
         p_rrr_motion_status[i] = RDU_MOTION_STATUS_AMBIGUOUS;
      }
      else
      {
         /* keep INVALID */
      }
#else
   RDU_Stage1_Output_Data_T *restrict p_rdu_output_data = p_rdu_data->p_rdu_stage1_output_data;
   uint16_t num_af_dets                                 = p_rdu_data->p_det_data->num_af_det;
   bool *restrict p_f_stationary_det                    = &p_rdu_internals->f_stationary_det[0];
   float32_t *restrict p_reference_rr                   = &p_rdu_internals->reference_rr[0];
   bool *restrict p_valid_det_flag                      = &p_rdu_internals->valid_det_flag[0];
   float32_t *restrict p_vel                            = &p_rdu_internals->cur_vel[0];
   int8_t *restrict p_k_factor                          = &p_rdu_output_data->stage1_wrapping_k[0];
   float32_t *restrict p_rdu_unamb_range_rate           = &p_rdu_internals->stage1_unamb_range_rate[0];
   int8_t *restrict p_rrr_motion_status                 = &p_rdu_output_data->stage1_rrr_motion_status[0];
   uint16_t scan_idx                                    = p_rdu_data->p_det_data->det_list_property.scanindex;
   uint16_t look_id                                     = p_rdu_data->p_det_data->det_list_property.look_type;
   p_rdu_output_data->stage1_scan_idx                   = scan_idx;
   p_rdu_output_data->stage1_look_id                    = look_id;

   /* Classic disambiguation without pattern  */
   float32_t vua_recip = 0.0F;
   float32_t k_float   = 0.0F;
   float32_t vua       = Get_Current_Scan_Vua(p_rdu_data, p_rdu_params);
   vua_recip           = rdu_finv(vua);
   for (uint16_t i = 0; i < num_af_dets; i++)
   {
      p_k_factor[i]             = 0;
      p_rdu_unamb_range_rate[i] = 0.0F;
      p_rrr_motion_status[i]    = RDU_MOTION_STATUS_INVALID; /* default status is -1 : INVALID */
      if (p_f_stationary_det[i] == true)
      {
         /* Compute K-factor with clamping to valid range [-2, 2] */
         k_float = rdu_roundf((p_reference_rr[i] - p_vel[i]) * vua_recip);
         if (k_float < (float32_t)RDU_K_MIN)
         {
            p_k_factor[i] = RDU_K_MIN;
         }
         else if (k_float > (float32_t)RDU_K_MAX)
         {
            p_k_factor[i] = RDU_K_MAX;
         }
         else
         {
            p_k_factor[i] = (int8_t)k_float;
         }
         p_rdu_unamb_range_rate[i] = p_vel[i] + (p_k_factor[i] * vua);
      }
      else
      {
         // do nothing
      }
      /* Motion status of individual detections updated here */
      if (p_valid_det_flag[i] && !p_f_stationary_det[i])
      {
         p_rrr_motion_status[i] = RDU_MOTION_STATUS_MOVING;
      }
      else if (p_f_stationary_det[i])
      {
         p_rrr_motion_status[i] = RDU_MOTION_STATUS_AMBIGUOUS;
      }
      else
      {
         /* keep INVALID */
      }
   }
#endif
}

/******************************************************************************
 * Name:  Stationary_Moving_Classifier_Process
 *   This function is Single API that does calls to all sub-functions required for
 * Stationary moving classifier aka stage 1
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data structure
 *              [in] p_rdu_params - Pointer to RDU parameters structure
 *              [in] p_rdu_internals - Pointer to RDU internals structure
 *
 * Return Value: none
 *
 * Matlab function Reference: run_stationary_moving_classifier()
 *
 ******************************************************************************/
#if defined(ENABLE_RDU_TESTING)
bool __attribute__((used))
Stationary_Moving_Classifier_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
#else
bool Stationary_Moving_Classifier_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals)
#endif
{
   bool ret_value = false;

   if (p_rdu_data != NULL && p_rdu_params != NULL && p_rdu_internals != NULL)
   {
      float32_t norm_delta_vxy = 0.0f;
      /* reset per-scan internal flag */
      p_rdu_internals->f_vel_corrected = false;

      /* Pre-calculations for stationary moving classifier */
      Stationary_Moving_Classifier_Precalcs(p_rdu_data, p_rdu_params, p_rdu_internals);

      /* (1) Stationary classification using adaptive Threshold and modulo function */
      (void)Transform_Host_Vel_To_Scs(p_rdu_data, p_rdu_params, p_rdu_internals);
      (void)Stationary_Det_Classifier(p_rdu_data, p_rdu_params, p_rdu_internals);

      /* (2) Low speed delta-velocity correction with refined stationary classification */
      if (p_rdu_params->enable_delta_velocity_correction == true)
      {
         p_rdu_internals->f_vel_corrected = Compute_Delta_Velocity_Correction(p_rdu_data, p_rdu_params, p_rdu_internals);

         norm_delta_vxy = rdu_sqrtf(p_rdu_internals->v_xy_delta.vx * p_rdu_internals->v_xy_delta.vx +
                                    p_rdu_internals->v_xy_delta.vy * p_rdu_internals->v_xy_delta.vy);

         if (p_rdu_internals->f_vel_corrected && norm_delta_vxy < p_rdu_params->vel_correction_threshold)
         {
            (void)Stationary_Det_Classifier_Vel_Correction(p_rdu_data, p_rdu_params, p_rdu_internals);
         }
      }

      /* (3) Stationary classification special case handling (at border of range rate interval) */
      (void)Check_Stationary_Classification_Boundary_Cases(p_rdu_data, p_rdu_params, p_rdu_internals);
      /* (4) Calculate classification confidence values using updated differences from boundary case handling */
      (void)Compute_Classification_Confidence_Values(p_rdu_data, p_rdu_internals);

      /* UnFolding after classification */
      (void)Stationary_Unfolding(p_rdu_data, p_rdu_params, p_rdu_internals);

      ret_value = true;
   }

   return ret_value;
}
/* END OF FILE -------------------------------------------------------------- */
