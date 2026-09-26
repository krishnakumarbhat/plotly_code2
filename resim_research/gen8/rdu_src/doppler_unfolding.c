/*===========================================================================*/
/**
 * @file doppler_unfolding.c
 *
 * @brief Implementation of doppler unfolding and RDU initialization.
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
#include <stdint.h>
#include <string.h>
/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "api/profiling_helpers.h"
#include "doppler_unfolding.h"
#include "ipc_dsp.h"
#include "moving_special_cases.h"
#include "radar_math.h"
#include "radar_sw_config.h"
#include "rdu_math.h"
#include "smc_cal.h"
#include "stationary_moving_classifier.h"
#include "two_cycle_unfolding.h"
/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/
#define REAR_CORNERING_COMPLIANCE (-0.0053f) /* Rear cornering compliance [m/rad] for sideslip estimation */
/** @brief Degrees-to-radians conversion */
#define RDU_S2_DEG2RAD(deg) ((deg) * (M_PI_F / 180.0f))
#ifndef US_2_MS
   #define US_2_MS (0.001F)
#endif
/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/
typedef struct RDU_Internal_Mem_Block_Tag
{
   RDU_Params_T rdu_algo_params;
   RDU_Internals_T rdu_internals;
} RDU_Internal_Mem_Block_T;

typedef enum
{
   RDU_STAGE_1 = 0,
   RDU_STAGE_2,
   RDU_STAGE_3,
   RDU_NUM_STAGES
} RDU_Processing_Stage_T;

typedef struct
{
   RDU_Processing_Stage_T stage;
   ProfilingStruct_t profilingStruct;
   uint32_t count;
} RDU_ProfilingInfo_T;

/*===========================================================================*
 * Exported Const Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/
static RDU_Internal_Mem_Info_T RDU_Internal_Mem_Info[RDU_MP_MAX_NUM_MEMORY_LOC];
static RDU_Internal_Mem_Block_T *P_G_Rdu_Mem_Block = NULL;
static RDU_Params_T *P_G_Rdu_Algo_Params           = NULL;
static RDU_Internals_T *P_G_Rdu_Internals          = NULL;

RDU_ProfilingInfo_T __attribute__((section(".sram0.bss.rdu"))) RDU_ProfilingInfo[RDU_NUM_STAGES] = {
   {RDU_STAGE_1, {0}, 0},
   {RDU_STAGE_2, {0}, 0},
   {RDU_STAGE_3, {0}, 0},
};

static bool rdu_profiling_initialized = false;

#ifndef RDU_SIL_ENABLE
static __attribute__((aligned(32))) __attribute__((section(".sram0.bss.rdu"))) RDU_Buffer_T RDU_Buffer;
#else
static __attribute__((aligned(32))) RDU_Buffer_T RDU_Buffer;
#endif
/*===========================================================================*
 * Local Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/
static bool RDU_Params_Init(RDU_Data_T *p_rdu_data);
static bool RDU_Internals_Init(RDU_Data_T *p_rdu_data);

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/

/******************************************************************************
 * Name:  RDU_Params_Init
 *   This function initializes RDU Algorithm parameters
 *
 * Shared Variables: none
 *
 * Parameters:  none
 *
 * Return Value: none
 *
 ******************************************************************************/
static bool RDU_Params_Init(RDU_Data_T *p_rdu_data)
{
   bool ret_value = false;
   int16_t i = 0, j = 0;
   /* Check if memory allocated */
   if (NULL != P_G_Rdu_Algo_Params && NULL != p_rdu_data)
   {
      /* Initialize host_motion_covariance_matrix (3x3) element-wise */
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[0][0] = 1e-8F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[0][1] = 0.0F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[0][2] = 0.0F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[1][0] = 0.0F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[1][1] = 0.01F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[1][2] = 0.0F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[2][0] = 0.0F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[2][1] = 0.0F;
      P_G_Rdu_Algo_Params->host_motion_covariance_matrix[2][2] = 1e-6F; /* refer improvedDetMovingThreshold2.m */
      P_G_Rdu_Algo_Params->std_theta                           = 0.019198622554540634F;
      P_G_Rdu_Algo_Params->std_phi                             = 0.01745329238474369F;
      P_G_Rdu_Algo_Params->std_range_rate                      = 0.10999999940395355F;
      P_G_Rdu_Algo_Params->enable_delta_velocity_correction    = true;
      P_G_Rdu_Algo_Params->vel_correction_threshold            = 0.5000000f;
      P_G_Rdu_Algo_Params->min_range_rate                      = -30.0F;
      P_G_Rdu_Algo_Params->vua[0]                              = rdu_absf(k_vwrapping_smc[0] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->vua[1]                              = rdu_absf(k_vwrapping_smc[1] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->vua[2]                              = rdu_absf(k_vwrapping_smc[2] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->vua[3]                              = rdu_absf(k_vwrapping_smc[3] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->tikh_vx                             = 0.01F;
      P_G_Rdu_Algo_Params->tikh_vy                             = 0.01F;
      P_G_Rdu_Algo_Params->var_theta                           = P_G_Rdu_Algo_Params->std_theta * P_G_Rdu_Algo_Params->std_theta;
      P_G_Rdu_Algo_Params->var_phi                             = P_G_Rdu_Algo_Params->std_phi * P_G_Rdu_Algo_Params->std_phi;
      P_G_Rdu_Algo_Params->var_range_rate          = P_G_Rdu_Algo_Params->std_range_rate * P_G_Rdu_Algo_Params->std_range_rate;
      P_G_Rdu_Algo_Params->perspective_angle       = AF_FOV_DEG; /* from radar_sw_config.h */
      P_G_Rdu_Algo_Params->minimum_range_threshold = 0.5000000F; /* Initialize with default value */
      P_G_Rdu_Algo_Params->k_vua                   = 2;          /* Initialize with default value */

      /* Stage2 related params */
      const int8_t k_values_moving[RDU_S2_NUM_K_MOVING]         = {-2, -1, -1, 0, 0, 0, 1, 1, 2};
      const int8_t k_values_stationary[RDU_S2_NUM_K_STATIONARY] = {-1, -1, 0, -1, 0, 1, 0, 1, 1};
      (void)memcpy(P_G_Rdu_Algo_Params->K_values_moving, k_values_moving, sizeof(k_values_moving));
      (void)memcpy(P_G_Rdu_Algo_Params->K_values_stationary, k_values_stationary, sizeof(k_values_stationary));
      P_G_Rdu_Algo_Params->num_K_moving                   = RDU_S2_NUM_K_MOVING;
      P_G_Rdu_Algo_Params->num_K_stationary               = RDU_S2_NUM_K_STATIONARY;
      P_G_Rdu_Algo_Params->max_realistic_velocity         = 45.0F;                /* m/s   */
      P_G_Rdu_Algo_Params->range_prediction_threshold     = 0.30F;                /* m     */
      P_G_Rdu_Algo_Params->angle_prediction_threshold_rad = RDU_S2_DEG2RAD(0.4F); /* rad */
      P_G_Rdu_Algo_Params->confidence_threshold           = 0.2f;                 /* –     */
      P_G_Rdu_Algo_Params->intersection_bonus_weight      = 1000.0f;              /* promotes intersection over proximity */
      P_G_Rdu_Algo_Params->velocity_weight                = 0.1f;                 /* fine-tuning term                     */
      P_G_Rdu_Algo_Params->rr_diff_weight                 = 1.0f;                 /* RR prediction error term [1/(m/s)]   */
      P_G_Rdu_Algo_Params->rr_diff_scaling_factor         = 0.6f;                 /* scales the RR acceptance threshold   */
      P_G_Rdu_Algo_Params->proximity_confidence_factor    = 0.6f;                 /* q_type for proximity matches         */
      P_G_Rdu_Algo_Params->enable_zero_rr_check           = false;
      P_G_Rdu_Algo_Params->max_close_detections           = (uint8_t)RDU_S2_DEFAULT_MAX_CLOSE_DETECTIONS;
      P_G_Rdu_Algo_Params->use_within_interval_constraint = true;
      P_G_Rdu_Algo_Params->max_within_interval            = 3U;

      /* Stage 3 related params - read radar position from IPC one-time buffer if available */
      uint8_t sensor_posn = 0U;
      sensor_posn         = p_rdu_data->radar_position - (uint8_t)1U;
      /* NOTE: Angles to be in radian */
      if (sensor_posn < RDU_MAX_RADAR_SENSOR_POS)
      {
         P_G_Rdu_Algo_Params->sensor_mounting.sensor_mount_loc =
            k_sensor_mount_location_smc; /* sensor_pos is index required to read SMC values*/
         P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_x_posn     = k_sensor_mount_pos_x_list_smc[sensor_posn];
         P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_y_posn     = k_sensor_mount_pos_y_list_smc[sensor_posn];
         P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_z_posn     = k_sensor_mount_pos_z_list_smc[sensor_posn];
         P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_roll  = k_sensor_mount_ornt_roll_list_smc[sensor_posn];
         P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_yaw   = k_sensor_mount_ornt_yaw_list_smc[sensor_posn];
         P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_pitch = k_sensor_mount_ornt_pitch_list_smc[sensor_posn];
         P_G_Rdu_Algo_Params->sensor_mounting.sensor_height         = k_sensor_mount_height_list_smc[sensor_posn];
         P_G_Rdu_Algo_Params->sensor_mounting.sensor_polarity       = k_sensor_mount_polarity_list_smc[sensor_posn];

         /* Compute m_transform_matrix from mounting parameters (equivalent to MATLAB pre_calc_mat with centershift=0). */
         const float32_t x_mount     = P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_x_posn;
         const float32_t y_mount     = P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_y_posn;
         const float32_t az_pol      = P_G_Rdu_Algo_Params->sensor_mounting.sensor_polarity;
         const float32_t theta_mount = P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_yaw;

         const float32_t sin_bs = rm_sinf(theta_mount);
         const float32_t cos_bs = rm_sinf(theta_mount + RADAR_PI_BY_2);

         const float32_t r00 = cos_bs;
         const float32_t r01 = sin_bs;
         const float32_t r10 = -rm_sinf(az_pol * theta_mount);
         const float32_t r11 = az_pol * cos_bs;

         const float32_t l0 = -y_mount;
         const float32_t l1 = x_mount;

         P_G_Rdu_Algo_Params->m_transform_matrix[0][0] = r00 * l0 + r01 * l1;
         P_G_Rdu_Algo_Params->m_transform_matrix[0][1] = r00;
         P_G_Rdu_Algo_Params->m_transform_matrix[0][2] = r01;
         P_G_Rdu_Algo_Params->m_transform_matrix[1][0] = r10 * l0 + r11 * l1; // check against spec
         P_G_Rdu_Algo_Params->m_transform_matrix[1][1] = r10;
         P_G_Rdu_Algo_Params->m_transform_matrix[1][2] = r11;
      }
      /* sensor_motion_covariance_mat = M * H * M' */
      float32_t A[2][3] = {0};
      for (i = 0; i < 2; ++i)
      {
         for (j = 0; j < 3; ++j)
         {
            A[i][j] = P_G_Rdu_Algo_Params->m_transform_matrix[i][0] * P_G_Rdu_Algo_Params->host_motion_covariance_matrix[0][j] +
                      P_G_Rdu_Algo_Params->m_transform_matrix[i][1] * P_G_Rdu_Algo_Params->host_motion_covariance_matrix[1][j] +
                      P_G_Rdu_Algo_Params->m_transform_matrix[i][2] * P_G_Rdu_Algo_Params->host_motion_covariance_matrix[2][j];
         }
      }

      for (i = 0; i < 2; ++i)
      {
         for (j = 0; j < 2; ++j)
         {
            {
               P_G_Rdu_Algo_Params->sensor_motion_covariance_matrix[i][j] =
                  A[i][0] * P_G_Rdu_Algo_Params->m_transform_matrix[j][0] +
                  A[i][1] * P_G_Rdu_Algo_Params->m_transform_matrix[j][1] +
                  A[i][2] * P_G_Rdu_Algo_Params->m_transform_matrix[j][2];
            }
         }
      }

      P_G_Rdu_Algo_Params->euclidean_max_dist_m  = 5.0F;
      P_G_Rdu_Algo_Params->num_neighbors         = 3;
      P_G_Rdu_Algo_Params->radar_cycle_time_s    = 0.0500000f;
      P_G_Rdu_Algo_Params->hi_conf               = 0.9F;
      P_G_Rdu_Algo_Params->max_rdot_mps          = 90.0F;
      P_G_Rdu_Algo_Params->min_rdot_mps          = -90.0F;
      P_G_Rdu_Algo_Params->minimum_rangerate_mps = -30.0F;
      P_G_Rdu_Algo_Params->max_eps_range_m       = 3;
      P_G_Rdu_Algo_Params->vwrapping_mps[0]      = rdu_absf(k_vwrapping_smc[0] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->vwrapping_mps[1]      = rdu_absf(k_vwrapping_smc[1] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->vwrapping_mps[2]      = rdu_absf(k_vwrapping_smc[2] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->vwrapping_mps[3]      = rdu_absf(k_vwrapping_smc[3] * CONVERT_P21_TO_FLOAT);
      P_G_Rdu_Algo_Params->rwrapping_m[0]        = rdu_absf(k_rwrapping_smc[0] * CONVERT_P31_TO_FLOAT);
      P_G_Rdu_Algo_Params->rwrapping_m[1]        = rdu_absf(k_rwrapping_smc[1] * CONVERT_P31_TO_FLOAT);
      P_G_Rdu_Algo_Params->rwrapping_m[2]        = rdu_absf(k_rwrapping_smc[2] * CONVERT_P31_TO_FLOAT);
      P_G_Rdu_Algo_Params->rwrapping_m[3]        = rdu_absf(k_rwrapping_smc[3] * CONVERT_P31_TO_FLOAT);

      /* Calculation of Rot matrix:
         roll -> Az polarity in rad
         pitch -> El in rad
         yaw -> Boresight in rad
         Rx                  = @(roll)[1,0,0; 0,cos(roll),-sin(roll); 0,sin(roll),cos(roll)];
         Ry                  = @(pitch)[cos(pitch),0,sin(pitch); 0,1,0; -sin(pitch),0,cos(pitch)];
         Rz                  = @(yaw)[cos(yaw),-sin(yaw),0; sin(yaw),cos(yaw),0;  0,0,1];
         Rot_M               = Rz(yaw)*Ry(pitch)*Rx(roll);
      */
      float32_t Rx[3][3] = {{1, 0, 0},
                            {0, rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_roll + RADAR_PI_BY_2),
                             -rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_roll)},
                            {0, rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_roll),
                             rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_roll + RADAR_PI_BY_2)}};
      float32_t Ry[3][3] = {{rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_pitch + RADAR_PI_BY_2), 0,
                             rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_pitch)},
                            {0, 1, 0},
                            {-rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_pitch), 0,
                             rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_pitch + RADAR_PI_BY_2)}};

      float32_t Rz[3][3]    = {{rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_yaw + RADAR_PI_BY_2),
                             -rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_yaw), 0},
                            {rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_yaw),
                             rm_sinf(P_G_Rdu_Algo_Params->sensor_mounting.iso_sensor_ornt_yaw + RADAR_PI_BY_2), 0},
                            {0, 0, 1}};
      float32_t Rzy[3][3]   = {0};
      float32_t Rot_M[3][3] = {0};
      /* Rzy = Rz * Ry */
      for (i = 0; i < 3; ++i)
      {
         for (j = 0; j < 3; ++j)
         {
            Rzy[i][j] = Rz[i][0] * Ry[0][j] + Rz[i][1] * Ry[1][j] + Rz[i][2] * Ry[2][j];
         }
      }
      /* Rot_M = Rzy * Rx */
      for (i = 0; i < 3; ++i)
      {
         for (j = 0; j < 3; ++j)
         {
            Rot_M[i][j] = Rzy[i][0] * Rx[0][j] + Rzy[i][1] * Rx[1][j] + Rzy[i][2] * Rx[2][j];
         }
      }
      memcpy(P_G_Rdu_Algo_Params->sensor_rot_matrix, Rot_M, sizeof(Rot_M));

      ret_value = true;
   }
   else
   {
      /* Do Nothing */
   }

   return ret_value;
}

/******************************************************************************
 * Name:  RDU_Internals_Init
 *   This function initializes RDU internal data and populates detection arrays
 *   from IPC D2M stream to avoid repeated extraction in classifier functions.
 *
 * Shared Variables: none
 *
 * Parameters:  [in] p_rdu_data - Pointer to RDU data (contains detection stream)
 *
 * Return Value: bool
 *
 ******************************************************************************/
static bool RDU_Internals_Init(RDU_Data_T *p_rdu_data)
{
   bool ret_value = false;

   /* Check if memory allocated */
   if (NULL != P_G_Rdu_Internals)
   {
      /* Clear whole structure to get rid of garbage value */
      memset(P_G_Rdu_Internals, 0, sizeof(RDU_Internals_T));

      P_G_Rdu_Internals->host_motion_vector.host_velocity = p_rdu_data->p_vse_data->vse_output.raw_speed_mps;
      P_G_Rdu_Internals->host_motion_vector.host_yaw_rate = p_rdu_data->p_vse_data->vse_output.raw_yaw_rate_rps;
      P_G_Rdu_Internals->host_motion_vector.host_sideslip =
         0.0f; /* REAR_CORNERING_COMPLIANCE * p_rdu_data->p_vse_data->veh_params.veh_speed *
                  p_rdu_data->p_vse_data->veh_params.veh_yaw; */
      /* side slip hardcoded to zero after discussing with RSys*/

      /* Populate detection arrays from IPC D2M stream */
      uint32_t num_dets = p_rdu_data->p_det_data->num_af_det;
      for (uint32_t i = 0; i < num_dets; i++)
      {
         P_G_Rdu_Internals->cur_range[i] = p_rdu_data->p_det_data->af_dets[i].ran;
         P_G_Rdu_Internals->cur_vel[i]   = p_rdu_data->p_det_data->af_dets[i].vel;
         P_G_Rdu_Internals->cur_theta[i] = p_rdu_data->p_det_data->af_dets[i].theta;
         P_G_Rdu_Internals->cur_phi[i]   = p_rdu_data->p_det_data->af_dets[i].phi;
      }

      ret_value = true;
   }
   else
   {
      /* Do Nothing */
   }

   return ret_value;
}

void Rdu_Buffer_Update_After_S1(RDU_Data_T *p_rdu_data, RDU_Buffer_T *p_rdu_buffer)
{
   /* TODO: replace memcpy with vector_copy() from spbb bbe_helpers.h */
   /* Stage 1 outputs Copy from IPC to RDU buffer*/
   memcpy((void *)p_rdu_buffer->present_scan_s1_output.rrr_motion_status,
          (void *)p_rdu_data->p_rdu_stage1_output_data->stage1_rrr_motion_status,
          sizeof(p_rdu_data->p_rdu_stage1_output_data->stage1_rrr_motion_status));
   memcpy((void *)p_rdu_buffer->present_scan_s1_output.K_factor, (void *)p_rdu_data->p_rdu_stage1_output_data->stage1_wrapping_k,
          sizeof(p_rdu_data->p_rdu_stage1_output_data->stage1_wrapping_k));
   memcpy((void *)p_rdu_buffer->present_scan_s1_output.range_rate_unamb, (void *)P_G_Rdu_Internals->stage1_unamb_range_rate,
          sizeof(P_G_Rdu_Internals->stage1_unamb_range_rate));
}

void Rdu_Buffer_Update_After_S2(RDU_Data_T *p_rdu_data, RDU_Buffer_T *p_rdu_buffer)
{
   /* TODO: replace memcpy with vector_copy() from spbb bbe_helpers.h */
   /* Stage 2 outputs Copy from IPC to RDU Buffer*/
   memcpy((void *)p_rdu_buffer->present_scan_s2_output.rrr_motion_status,
          (void *)p_rdu_data->p_rdu_stage2_output_data->stage2_rrr_motion_status,
          sizeof(p_rdu_data->p_rdu_stage2_output_data->stage2_rrr_motion_status));
   memcpy((void *)p_rdu_buffer->present_scan_s2_output.K_factor, (void *)p_rdu_data->p_rdu_stage1_output_data->stage1_wrapping_k,
          sizeof(p_rdu_data->p_rdu_stage1_output_data->stage1_wrapping_k));
   memcpy((void *)p_rdu_buffer->present_scan_s2_output.range_rate_unamb, (void *)P_G_Rdu_Internals->stage1_unamb_range_rate,
          sizeof(P_G_Rdu_Internals->stage1_unamb_range_rate));
}

void Rdu_Buffer_Update_After_S3(RDU_Internals_T *p_rdu_internals, RDU_Buffer_T *p_rdu_buffer, RDU_Data_T *p_rdu_data)
{
   /* TODO: replace memcpy with vector_copy() from spbb bbe_helpers.h */
   /* 1) Copy current scan det data linearized data to previous scan det data for next cycle
    * SIN(EL),COS(EL),SIN(AZ),COS(AZ) of present detections to be made as previous detections.
    *    AZ (theta): cos/sin already computed in S1 and stored in RDU_Internals. */
   memcpy(&p_rdu_buffer->prev_scan_det_data.prev_range, &p_rdu_internals->cur_range,
          MAX_NUM_DETECTS * sizeof(p_rdu_internals->cur_range[0]));
   memcpy(&p_rdu_buffer->prev_scan_det_data.prev_vel, &p_rdu_internals->cur_vel,
          MAX_NUM_DETECTS * sizeof(p_rdu_internals->cur_vel[0]));
   memcpy(&p_rdu_buffer->prev_scan_det_data.prev_theta, &p_rdu_internals->cur_theta,
          MAX_NUM_DETECTS * sizeof(p_rdu_internals->cur_theta[0]));
   memcpy(&p_rdu_buffer->prev_scan_det_data.prev_phi, &p_rdu_internals->cur_phi,
          MAX_NUM_DETECTS * sizeof(p_rdu_internals->cur_phi[0]));
   memcpy(&p_rdu_buffer->prev_scan_det_data.prev_cos_az, &p_rdu_internals->cos_theta,
          MAX_NUM_DETECTS * sizeof(p_rdu_internals->cos_theta[0]));
   memcpy(&p_rdu_buffer->prev_scan_det_data.prev_sin_az, &p_rdu_internals->sin_theta,
          MAX_NUM_DETECTS * sizeof(p_rdu_internals->sin_theta[0]));
   p_rdu_buffer->prev_scan_det_data.num_prev_scan_dets = p_rdu_data->p_det_data->num_af_det;

   /* 2) STAGE 2 OUTPUT OF PRESENT DETECTIONS TO BE MADE AS PREV DETECTIONS */
   memcpy(&p_rdu_buffer->prev_scan_s2_output, &p_rdu_buffer->present_scan_s2_output, sizeof(RDU_S2_Output_T));

   /* 3) STAGE 1 OUTPUT OF PRESENT DETECTIONS TO BE MADE AS PREV DETECTIONS */
   memcpy(&p_rdu_buffer->prev_scan_s1_output, &p_rdu_buffer->present_scan_s1_output, sizeof(RDU_S1_Output_T));

   /* 4) HOST MOTION VECTOR OF PRESENT SCAN TO BE MADE AS PREV SCAN */
   memcpy(&p_rdu_buffer->prev_scan_host_motion_vector, &p_rdu_internals->host_motion_vector, sizeof(Host_Motion_Vector_T));

   /* Mark buffer as containing valid previous scan data for the next scan cycle */
   p_rdu_buffer->has_valid_prev_scan = true;
}

/******************************************************************************
 * Name:  Doppler_Unfolding_Process
 *   This function is single API for Execution of RDU Algorithm
 *
 * Shared Variables: none
 *
 * Parameters:  none
 *
 * Return Value: none
 *
 ******************************************************************************/

RDU_Status_T Doppler_Unfolding_Process(RDU_Data_T *p_rdu_data)
{
   RDU_Status_T ret_value = RDU_STATUS_S1_FAILURE;

   if (!rdu_profiling_initialized)
   {
      for (uint8_t i = 0U; i < RDU_NUM_STAGES; i++)
      {
         Profiling_Helpers_Init(&RDU_ProfilingInfo[i].profilingStruct, true);
         RDU_ProfilingInfo[i].count = 0U;
      }
      rdu_profiling_initialized = true;
   }

   /* Initialize parameters - check for failure */
   if (!RDU_Params_Init(p_rdu_data) || !RDU_Internals_Init(p_rdu_data))
   {
      ret_value = RDU_STATUS_INIT_FAILURE;
   }
   vector_clear((void *)p_rdu_data->p_rdu_stage1_output_data, sizeof(RDU_Stage1_Output_Data_T));
   vector_clear((void *)p_rdu_data->p_rdu_stage2_output_data, sizeof(RDU_Stage2_Output_Data_T));
   vector_clear((void *)p_rdu_data->p_rdu_stage3_output_data, sizeof(RDU_Stage3_Output_Data_T));

   /* Stage 1 Execution - Stationary Moving Classifier */

   Profiling_Helpers_Init(&RDU_ProfilingInfo[RDU_STAGE_1].profilingStruct, false);

   bool s1_ok = Stationary_Moving_Classifier_Process(p_rdu_data, P_G_Rdu_Algo_Params, P_G_Rdu_Internals);

   Profiling_Helpers_Update(&RDU_ProfilingInfo[RDU_STAGE_1].profilingStruct);
   RDU_ProfilingInfo[RDU_STAGE_1].count++;

   if (s1_ok)
   {
      ret_value = RDU_STATUS_S1_SUCCESS;

      /* Copy IPC Stage 1 output (rrr_motion_status, k_factor, unamb RR, confidence) into RDU buffer/output.*/
      Rdu_Buffer_Update_After_S1(p_rdu_data, &RDU_Buffer);
#ifdef ENABLE_RDU_TESTING
      xthal_dcache_block_writeback((void *)Get_SP_Post_Proc_IPC_D2M_Msg_Buffer(), sizeof(D2M_Msg_T));
      xthal_dcache_block_invalidate((void *)Get_SP_Post_Proc_IPC_D2M_Msg_Buffer(), sizeof(D2M_Msg_T));
#endif
      /* Stage 2 Execution - Moving Special Cases Detection
       * Only runs from the 2nd scan onward: when RDU_Buffer.has_valid_prev_scan is true.
       * On the first scan, the RDU_Buffer does not yet contain valid previous scan data.
       * Runs after Stage 1 has populated rrr_motion_status, classification_confidence,
       * moving_det_thold, vx_scs, vy_scs in p_rdu_output_data.              */
      if (!RDU_Buffer.has_valid_prev_scan)
      {
         /* First scan / alias-guard skip: Stage 2 not run.
          * Zero the Stage 2 output slot so R52 sees a clean "no Stage 2 activity"
          * frame rather than stale content from a previous use of this ping-pong slot.
          * (At power-on the D2M buffer is already zeroed by BSS init; this guard
          * handles all subsequent skip cases defensively.)                         */
         if (p_rdu_data->p_rdu_stage2_output_data != NULL)
         {
            (void)memset(p_rdu_data->p_rdu_stage2_output_data, 0, sizeof(RDU_Stage2_Output_Data_T));
         }
      }

      Profiling_Helpers_Init(&RDU_ProfilingInfo[RDU_STAGE_2].profilingStruct, false);

      bool s2_ok = RDU_Stage2_Process(p_rdu_data, P_G_Rdu_Algo_Params, P_G_Rdu_Internals, &RDU_Buffer);

      Profiling_Helpers_Update(&RDU_ProfilingInfo[RDU_STAGE_2].profilingStruct);
      RDU_ProfilingInfo[RDU_STAGE_2].count++;

      if (s2_ok)
      {
         ret_value = RDU_STATUS_S2_SUCCESS;
         ret_value = RDU_STATUS_S2_SUCCESS;
      }
      else
      {
         ret_value = RDU_STATUS_S2_FAILURE; /* Stage 1 succeeded; Stage 2 non-fatal failure */
         ret_value = RDU_STATUS_S2_FAILURE; /* Stage 1 succeeded; Stage 2 non-fatal failure */
      }

#ifdef ENABLE_RDU_TESTING
      xthal_dcache_block_writeback((void *)Get_SP_Post_Proc_IPC_D2M_Msg_Buffer(), sizeof(D2M_Msg_T));
      xthal_dcache_block_invalidate((void *)Get_SP_Post_Proc_IPC_D2M_Msg_Buffer(), sizeof(D2M_Msg_T));
#endif

      /* Copy IPC Stage 2 output (rrr_motion_status, k_factor, unamb RR) into RDU buffer/output.
       * Runs every scan so present scan S2 data is available as previous data next cycle. */
      Rdu_Buffer_Update_After_S2(p_rdu_data, &RDU_Buffer);

      /* Stage 3: Two-cycle unfolding disambiguation */

      Profiling_Helpers_Init(&RDU_ProfilingInfo[RDU_STAGE_3].profilingStruct, false);

      Two_Cycle_Unfolding_Process(p_rdu_data, P_G_Rdu_Algo_Params, P_G_Rdu_Internals, &RDU_Buffer);

      Profiling_Helpers_Update(&RDU_ProfilingInfo[RDU_STAGE_3].profilingStruct);
      RDU_ProfilingInfo[RDU_STAGE_3].count++;

      /* Update buffer after S3: copy present scan data as previous scan data.
       * Also sets has_valid_prev_scan = true after the first scan completes. */
      Rdu_Buffer_Update_After_S3(P_G_Rdu_Internals, &RDU_Buffer, p_rdu_data);
#ifdef ENABLE_RDU_TESTING
      xthal_dcache_block_writeback((void *)Get_SP_Post_Proc_IPC_D2M_Msg_Buffer(), sizeof(D2M_Msg_T));
#endif
      ret_value = RDU_STATUS_S3_SUCCESS; /* TODO add condition to check Stage 3 success */

      /* Map profiling max times to rdu_stream timing data */
      if (p_rdu_data->p_rdu_timing_data != NULL)
      {
         p_rdu_data->p_rdu_timing_data->stage1_inst_time =
            (float32_t)RDU_ProfilingInfo[RDU_STAGE_1].profilingStruct.lastRuntime * (float)US_2_MS;
         p_rdu_data->p_rdu_timing_data->stage2_inst_time =
            (float32_t)RDU_ProfilingInfo[RDU_STAGE_2].profilingStruct.lastRuntime * (float)US_2_MS;
         p_rdu_data->p_rdu_timing_data->stage3_inst_time =
            (float32_t)RDU_ProfilingInfo[RDU_STAGE_3].profilingStruct.lastRuntime * (float)US_2_MS;
         p_rdu_data->p_rdu_timing_data->rdu_total_inst_time =
            (float32_t)(RDU_ProfilingInfo[RDU_STAGE_1].profilingStruct.lastRuntime +
                        RDU_ProfilingInfo[RDU_STAGE_2].profilingStruct.lastRuntime +
                        RDU_ProfilingInfo[RDU_STAGE_3].profilingStruct.lastRuntime) *
            (float)US_2_MS;
         p_rdu_data->p_rdu_timing_data->stage1_max_time =
            (float32_t)RDU_ProfilingInfo[RDU_STAGE_1].profilingStruct.maxRuntime * (float)US_2_MS;
         p_rdu_data->p_rdu_timing_data->stage2_max_time =
            (float32_t)RDU_ProfilingInfo[RDU_STAGE_2].profilingStruct.maxRuntime * (float)US_2_MS;
         p_rdu_data->p_rdu_timing_data->stage3_max_time =
            (float32_t)RDU_ProfilingInfo[RDU_STAGE_3].profilingStruct.maxRuntime * (float)US_2_MS;
         p_rdu_data->p_rdu_timing_data->rdu_total_max_time =
            (float32_t)(RDU_ProfilingInfo[RDU_STAGE_1].profilingStruct.maxRuntime +
                        RDU_ProfilingInfo[RDU_STAGE_2].profilingStruct.maxRuntime +
                        RDU_ProfilingInfo[RDU_STAGE_3].profilingStruct.maxRuntime) *
            (float)US_2_MS;
      }
   }

   return ret_value;
}

/******************************************************************************
 * Name:  Get_RDU_Internal_memory_pointers
 *   This function will get the memory pointer to all RDU structures
 *
 * Shared Variables: None
 *
 * Parameters: void
 *
 * Return Value: RDU_Internal_Mem_Info_T* - Pointer to memory info array
 *
 ******************************************************************************/
RDU_Internal_Mem_Info_T *Get_RDU_Internal_memory_pointers(void)
{
   /* Clear memory details before assign */
   (void)memset(RDU_Internal_Mem_Info, 0U, sizeof(RDU_Internal_Mem_Info));

   /* Slot 0: Stage 1 algorithm parameters */
   RDU_Internal_Mem_Info[0].size     = sizeof(RDU_Params_T);
   RDU_Internal_Mem_Info[0].addr_ptr = (void **)&P_G_Rdu_Algo_Params;

   /* Slot 1: Stage 1 internal working memory */
   RDU_Internal_Mem_Info[1].size     = sizeof(RDU_Internals_T);
   RDU_Internal_Mem_Info[1].addr_ptr = (void **)&P_G_Rdu_Internals;

   return RDU_Internal_Mem_Info;
}

/******************************************************************************
 * Name:  RDU_Handler_Memory_Init
 *   This function will allocate memory from mempool for all RDU structures
 *
 * Shared Variables: None
 *
 * Parameters: [in] mem_pool - Pointer to memory pool structure
 *
 * Return Value: Memory_Pool_Return_T - MEM_POOL_SUCCESS or MEM_POOL_FAILURE
 *
 ******************************************************************************/
Memory_Pool_Return_T RDU_Handler_Memory_Init(Mem_Pool_T *mem_pool)
{
   Memory_Pool_Return_T retVal = MEM_POOL_FAILURE;

   if (NULL == mem_pool)
   {
      return retVal;
   }

#ifdef RDU_SIL_ENABLE
   /* SIL only: reset pool size so allocation starts fresh each scan.
    * On target, the memory pool is initialised once at startup and
    * never reset — this guard ensures embedded behaviour is unchanged. */
   mem_pool->size = 0U;
#endif

   retVal = Mem_Pool_Alloc(mem_pool, sizeof(*P_G_Rdu_Mem_Block), (void **)&P_G_Rdu_Mem_Block);

   if (MEM_POOL_SUCCESS == retVal)
   {
      P_G_Rdu_Algo_Params = &P_G_Rdu_Mem_Block->rdu_algo_params;
      P_G_Rdu_Internals   = &P_G_Rdu_Mem_Block->rdu_internals;
   }
   else
   {
      P_G_Rdu_Mem_Block   = NULL;
      P_G_Rdu_Algo_Params = NULL;
      P_G_Rdu_Internals   = NULL;
   }

   return retVal;
}

/******************************************************************************
 * Name:  Doppler_Unfolding_Init
 *   This function Initializes IPC Buffers addresses used as input and output for RDU
 *
 * Shared Variables: none
 *
 * Parameters: [in,out] p_rdu_input_data - Pointer to RDU Input structure
 *             [in] p_d2m_data - Pointer to D2M message structure
 *
 * Return Value: RDU_Status_T
 *
 ******************************************************************************/
RDU_Status_T Doppler_Unfolding_Init(RDU_Data_T *p_rdu_input_data, D2M_Msg_T *p_d2m_data, M2D_Msg_T *p_m2d_data, uint8_t radar_posn)
{
   RDU_Status_T ret_value = RDU_STATUS_INIT_FAILURE;

   if ((p_rdu_input_data == NULL) || (p_d2m_data == NULL) || (p_m2d_data == NULL))
   {
      /* Do Nothing */
   }
   else
   {
      p_rdu_input_data->p_det_data               = &p_d2m_data->payload.det_data;
      p_rdu_input_data->p_vse_data               = &p_m2d_data->payload.veh_data;
      p_rdu_input_data->radar_position           = radar_posn;
      p_rdu_input_data->p_rdu_stage1_output_data = &p_d2m_data->payload.rdu_stream_data.rdu_stage1_output_data;
      p_rdu_input_data->p_rdu_stage2_output_data = &p_d2m_data->payload.rdu_stream_data.rdu_stage2_output_data;
      p_rdu_input_data->p_rdu_stage3_output_data = &p_d2m_data->payload.rdu_stream_data.rdu_stage3_output_data;
      p_rdu_input_data->p_rdu_timing_data        = &p_d2m_data->payload.rdu_stream_data.rdu_timing_data;

      ret_value = RDU_STATUS_INIT_SUCCESS;
      ret_value = RDU_STATUS_INIT_SUCCESS;
   }

   return ret_value;
}
/* END OF FILE -------------------------------------------------------------- */
