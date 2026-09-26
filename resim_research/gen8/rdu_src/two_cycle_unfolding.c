/*===========================================================================*/
/**
 * @file two_cycle_unfolding.c
 * @brief Implementation of stage 3 - two-cycle unfolding of RDU.
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
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <float.h>
#include <string.h>
/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#ifdef BBE_ENABLE
   #include "api/bbe_helpers.h"
   #include "api/profiling_helpers.h" /* ProfilingStruct_t, Profiling_Helpers_Init/Update */
   #include <xtensa/tie/xt_bben_scalarfp.h>
#endif

#include "radar_math.h"
#include "rdu_math.h"
#include "two_cycle_unfolding.h"
/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/

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
void Convert_Detections_To_VCS(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                               RDU_Buffer_T *p_rdu_buffer);
void Convert_Spherical_Coordinates_To_Cartesian_VCS(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params,
                                                    RDU_Internals_T *p_rdu_internals, RDU_Buffer_T *p_rdu_buffer);
void Cartesian_N_Knn(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                     RDU_Buffer_T *p_rdu_buffer);
/**
 * @brief Inserts one candidate into a sorted top-K squared-distance list (ascending).
 *
 * Maintains knn_d / knn_idx / knn_rr / knn_az in sorted order after each
 * insertion.  All inner loops are straight-line with no early-exit branches so
 * the compiler fully unrolls them when k_max is a compile-time constant.
 *
 * The caller is responsible for the pre-check  new_d < knn_d[k_max-1]  before
 * calling this function (to avoid needless attribute look-ups on miss).
 *
 * @param[in,out] knn_d    Sorted squared-distance array [k_max], ascending
 * @param[in,out] knn_idx  Sorted moving-buffer index array [k_max]
 * @param[in,out] knn_rr   Sorted prev-scan range-rate array [k_max] [m/s]
 * @param[in,out] knn_az   Sorted prev-scan azimuth array [k_max] [deg]
 * @param[in]     k_max    Active neighbour slot count (≤ RDU_MAX_K_NEIGHBOURS)
 * @param[in]     new_d    Squared Euclidean distance of the new candidate [m²]
 * @param[in]     new_idx  Moving-buffer index of the new candidate
 * @param[in]     new_rr   Prev-scan range rate of the new candidate [m/s]
 * @param[in]     new_az   Prev-scan azimuth of the new candidate [deg]
 */
static inline void insert_into_topk_n(float32_t *knn_d, uint16_t *knn_idx, float32_t *knn_rr, float32_t *knn_az, uint8_t k_max,
                                      float32_t new_d, uint16_t new_idx, float32_t new_rr, float32_t new_az)
{
   int16_t pos = (int16_t)k_max; /* insertion position; stays k_max if no slot improved */
   int16_t k;

   /* Linear scan for insertion position — K is small (≤ RDU_MAX_K_NEIGHBOURS) */
   for (k = 0; k < (int16_t)k_max; k++)
   {
      if (new_d < knn_d[k])
      {
         pos = k;
         break;
      }
   }
   if (pos == (int16_t)k_max)
   {
      return;
   } /* candidate does not improve any slot */

   /* Shift elements [pos .. k_max-2] one place right to make room at pos */
   for (k = (int16_t)k_max - 1; k > pos; k--)
   {
      knn_d[k]   = knn_d[k - 1];
      knn_idx[k] = knn_idx[k - 1];
      knn_rr[k]  = knn_rr[k - 1];
      knn_az[k]  = knn_az[k - 1];
   }
   knn_d[pos]   = new_d;
   knn_idx[pos] = new_idx;
   knn_rr[pos]  = new_rr;
   knn_az[pos]  = new_az;
}

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/

/**
 * @brief This function filters moving detections of present cycle
 *
 * Detections which are moving are copied to RDU_Buffer's present_scan_s2_mov_det
 * structure based on stage 2 classification results.
 *
 * @param[in,out] p_rdu_data      Pointer to RDU data structure
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return void
 *
 * @ref Matlab function reference: run_algorithm_single() of two-cycle unfolding
 */
void Filter_Moving_Dets(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                        RDU_Buffer_T *p_rdu_buffer)
{
   uint16_t moving_det_count = 0;

   int32_t num_af_dets = p_rdu_data->p_det_data->num_af_det;

   p_rdu_buffer->present_scan_s2_mov_det.scan_idx  = p_rdu_data->p_det_data->det_list_property.scanindex;
   p_rdu_buffer->present_scan_s2_mov_det.look_type = (uint8_t)p_rdu_data->p_det_data->det_list_property.look_type;

#ifdef BBE_ENABLE
   uint16_t i;
   uint16_t __attribute__((aligned(32))) idx_values[XCHAL_BBEN_SIMD_WIDTH] = {0};
   uint16_t set_flag_count;
   uint16_t vec_width = XCHAL_BBEN_SIMD_WIDTH; /*  */
                                               //   uint16_t rem                = num_af_dets % vec_width;
   uint16_t padded_num_af_dets = ((num_af_dets + vec_width - 1) / vec_width) * vec_width;
   uint16_t num_vec            = padded_num_af_dets / vec_width;

   xb_vecNx16 *p_rrr_status  = (xb_vecNx16 *)&p_rdu_data->p_rdu_stage2_output_data->stage2_rrr_motion_status[0];
   xb_vecNx16 *p_mov_det_idx = (xb_vecNx16 *)&p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[0];
   xb_vecNx16U *p_idx_values = (xb_vecNx16U *)&idx_values[0];
   xb_vecNx16U v_idx_values;
   xb_vecNx16 v_status_moving_cast         = (xb_vecNx16)(RDU_MOTION_STATUS_MOVING);
   xb_vecNx16 v_status_moving_special_cast = (xb_vecNx16)(RDU_MOTION_STATUS_MOVING_SPECIAL);
   valign valign_mov_det_idx;
   xb_vecNx16 v_rrr_status;
   vboolN v_moving_flag;
   vboolN v_moving_special_flag;
   uint16_t base_idx = 0;
   xb_vecNx16U v_idx;
   xb_vecNx16 vec_rrr_status_16bit_new;
   xb_vecNx16 vec_lo_8bit_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
   int8_t status;
   for (i = 0U; i < num_vec; i++)
   {
      BBE_LAVNX16_XP(v_rrr_status, BBE_LANX16_PP(p_rrr_status), p_rrr_status, 16);
      vec_rrr_status_16bit_new = BBE_SELNX16I((v_rrr_status >> 8U), v_rrr_status, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask;
      v_moving_flag            = BBE_EQNX16(vec_rrr_status_16bit_new, v_status_moving_cast);
      v_moving_special_flag    = BBE_EQNX16(vec_rrr_status_16bit_new, v_status_moving_special_cast);
      BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, BBE_ORBN(v_moving_special_flag, v_moving_flag));
      if (set_flag_count > 0U)
      {
         base_idx           = i * vec_width;
         v_idx_values       = BBE_LVNX16U_I(p_idx_values, 0U);
         v_idx              = BBE_ADDNX16U(v_idx_values, (xb_vecNx16U)base_idx);
         valign_mov_det_idx = BBE_ZALIGN();
         BBE_SAVNX16_XP(v_idx, valign_mov_det_idx, p_mov_det_idx, set_flag_count);
         BBE_SANX16POS_FP(valign_mov_det_idx, p_mov_det_idx); /*to flush the value*/
         moving_det_count += (set_flag_count >> 1U);
      }
   }
   for (i = 0; i < num_af_dets; i++)
   {
      status = p_rdu_data->p_rdu_stage2_output_data->stage2_rrr_motion_status[i];
      if (status == RDU_MOTION_STATUS_STATIONARY)
      {
         p_rdu_data->p_rdu_stage3_output_data->stage3_unfolding_confidence[i] = 1.0F;
      }
   }
   p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets = moving_det_count;

#else
   int8_t status;
   uint16_t *p_moving_det_indices = &p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[0];
   int16_t i                      = 0;
   for (i = 0; i < num_af_dets; i++)
   {
      status = p_rdu_data->p_rdu_stage2_output_data->stage2_rrr_motion_status[i];
      if ((status == RDU_MOTION_STATUS_MOVING) || (status == RDU_MOTION_STATUS_MOVING_SPECIAL))
      {
         p_moving_det_indices[moving_det_count] = i;
         moving_det_count++;
      }
      else
      {
         /*do nothing*/
      }
      if (status == RDU_MOTION_STATUS_STATIONARY)
      {
         p_rdu_data->p_rdu_stage3_output_data->stage3_unfolding_confidence[i] = 1.0F;
      }
      else
      {
         /*do nothing*/
      }
   }

   p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets = moving_det_count;
#endif
   /* Convert Spherical coordinates to Cartesian VCS */
   Convert_Spherical_Coordinates_To_Cartesian_VCS(p_rdu_data, p_rdu_params, p_rdu_internals, p_rdu_buffer);
}

/**
 * @brief This function output motion compensated VCS positions of prev scan dets
 *
 *  Ego motion is translational and rotational between two scans. Below are the steps for ego motion compensation of previous scan
 * detections in VCS,
 *
 *  - Translation motion compensation: subtract the distance travelled by the ego vehicle from the previous scan to the current
 * scan. (Vx,Vy,Vz)-(distance_travelled,0,0) : as vehicle has moved along x-axis subtract distance_travelled along the x-axis
 *
 *  - Rotational motion compensation: rotate the previous scan detections in VCS by the angle rotated by the ego vehicle using yaw
 * angle from yawrate. Muliply by rotation matrix [1, yaw_angle, 0; -yaw_angle, 1, 0; 0, 0, 1], with low angle approximation
 *
 * @param[in,out] p_rdu_data      Pointer to RDU data structure
 * @param[in] p_rdu_params    Pointer to RDU parameters structure
 * @param[in,out] p_rdu_internals Pointer to RDU internals structure
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return void
 *
 * @ref Matlab function reference: compensate_ego_motion_fast() of two-cycle unfolding
 */
void Apply_Ego_Motion_Compensation_To_Prev_Scan_Dets(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params,
                                                     RDU_Internals_T *p_rdu_internals, RDU_Buffer_T *p_rdu_buffer)
{
   (void)p_rdu_data;
   int16_t num_prev_mov_dets = (int16_t)p_rdu_buffer->prev_scan_s2_mov_det.num_moving_dets;
   uint16_t i                = 0;
   /* For EGO Compensation of prev dets, velocity of prev scan is used and yaw rate of current scan is used */
   float32_t vel      = p_rdu_buffer->prev_scan_host_motion_vector.host_velocity;
   float32_t yaw_rate = p_rdu_internals->host_motion_vector.host_yaw_rate;
   float32_t dt       = (float32_t)(p_rdu_buffer->present_scan_s2_mov_det.scan_idx - p_rdu_buffer->prev_scan_s2_mov_det.scan_idx) *
                  p_rdu_params->radar_cycle_time_s;
   float32_t distance_travelled = XT_MUL_S(vel, dt);
   float32_t yaw_angle          = XT_MUL_S(yaw_rate, dt);

#ifdef BBE_ENABLE
   uint16_t vec_width               = BBE_BATCH;
   uint16_t padded_num_dets         = ((num_prev_mov_dets + vec_width - 1) / vec_width) * vec_width;
   uint16_t num_vec                 = padded_num_dets / vec_width;
   xb_vecN_2xf32 vec_dist_travelled = (xb_vecN_2xf32)distance_travelled;
   xb_vecN_2xf32 vec_yaw_angle      = (xb_vecN_2xf32)yaw_angle;
   xb_vecN_2xf32 v_det_pos_x, v_det_pos_y;
   xb_vecN_2xf32 *p_det_pos_x = (xb_vecN_2xf32 *)&p_rdu_buffer->prev_scan_s2_mov_det.det_pos_x[0];
   xb_vecN_2xf32 *p_det_pos_y = (xb_vecN_2xf32 *)&p_rdu_buffer->prev_scan_s2_mov_det.det_pos_y[0];
   xb_vecN_2xf32 v_temp1, v_temp2;
   for (i = 0; i < num_vec; i++)
   {
      v_det_pos_x = BBE_LVN_2XF32_I(p_det_pos_x, 0);
      v_det_pos_y = BBE_LVN_2XF32_I(p_det_pos_y, 0);
      v_temp1     = BBE_SUBN_2XF32(v_det_pos_x, vec_dist_travelled); /* (vx - distance_travelled) */
      v_temp2     = v_det_pos_y;
      BBE_MULSN_2XF32(v_temp2, v_temp1, vec_yaw_angle); /* vy - (vx - distance_travelled) * yaw_angle */
      BBE_SVN_2XF32_IP(v_temp2, p_det_pos_y, 32U);
      BBE_MULAN_2XF32(v_temp1, v_det_pos_y, vec_yaw_angle); /* (vx - distance_travelled) + (vy * yaw_angle) */
      BBE_SVN_2XF32_IP(v_temp1, p_det_pos_x, 32U);
   }
#else
   float32_t vx                                          = 0.0F;
   float32_t vy                                          = 0.0F;
   for (i = 0; i < num_prev_mov_dets; i++)
   {
      vx = p_rdu_buffer->prev_scan_s2_mov_det.det_pos_x[i];
      vy = p_rdu_buffer->prev_scan_s2_mov_det.det_pos_y[i];
      /*
         Final output of ego motion compensation is [x-distance + yaw_angle.*y ; -yaw_angle.*(x-distance) + y; z];
      */
      p_rdu_buffer->prev_scan_s2_mov_det.det_pos_x[i] = (vx - distance_travelled) + (vy * yaw_angle);
      p_rdu_buffer->prev_scan_s2_mov_det.det_pos_y[i] = (-(vx - distance_travelled) * (yaw_angle)) + (vy);
   }
#endif
}

/**
 * @brief This function output the cartesian coordinates of detections
 *
 * Conversion of detections from sensor's spherical coordinates to Cartesian coordinates
 * in VCS. Here, the default VCS is ISO.
 * Following are the steps for conversion of detections from sensor spherical coordinates to Cartesian coordinates in VCS:
 *
 *    1) Coversion of detections from sensor's spherical coordinates to cartesian coordinates (with sensor as reference)
 *         z =  r .* sin(elev);
 *         x =  r .* cos(elev) .* cos(az);
 *         y =  r .* cos(elev) .* sin(az);
 *
 *      2) Use Rotation Matrix for angle compensation in cartesian coordinates of detection w.r.t sensor
 *
 *      3) Add offset of sensor mounting position to Detections in cartesian coordinates to convert to VCS coordinates
 *
 * @param[in,out] p_rdu_data      Pointer to RDU data structure
 * @param[in] p_rdu_params    Pointer to RDU parameters structure
 * @param[in,out] p_rdu_internals Pointer to RDU internals structure
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return void
 *
 * @ref Matlab function reference: sensor_spherical_to_cartesian_VCS() of two-cycle unfolding
 */
void Convert_Spherical_Coordinates_To_Cartesian_VCS(RDU_Data_T *restrict p_rdu_data, RDU_Params_T *restrict p_rdu_params,
                                                    RDU_Internals_T *restrict p_rdu_internals, RDU_Buffer_T *restrict p_rdu_buffer)
{
   (void)p_rdu_data;
   int16_t num_cur_mov_dets = (int16_t)p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets;
   uint16_t i;
   uint16_t det_idx = 0U;
   float32_t vx     = 0.0F;
   float32_t vy     = 0.0F;
   float32_t vz     = 0.0F;
   float32_t range  = 0.0F;
   /* Rotation Matrix - cache in locals so the compiler can keep them in registers */
   const float32_t Rot_M_00 = p_rdu_params->sensor_rot_matrix[0][0];
   const float32_t Rot_M_01 = p_rdu_params->sensor_rot_matrix[0][1];
   const float32_t Rot_M_02 = p_rdu_params->sensor_rot_matrix[0][2];
   const float32_t Rot_M_10 = p_rdu_params->sensor_rot_matrix[1][0];
   const float32_t Rot_M_11 = p_rdu_params->sensor_rot_matrix[1][1];
   const float32_t Rot_M_12 = p_rdu_params->sensor_rot_matrix[1][2];
   const float32_t Rot_M_20 = p_rdu_params->sensor_rot_matrix[2][0];
   const float32_t Rot_M_21 = p_rdu_params->sensor_rot_matrix[2][1];
   const float32_t Rot_M_22 = p_rdu_params->sensor_rot_matrix[2][2];
   const float32_t mount_x  = p_rdu_params->sensor_mounting.iso_sensor_x_posn;
   const float32_t mount_y  = p_rdu_params->sensor_mounting.iso_sensor_y_posn;

#ifdef BBE_ENABLE
   uint16_t vec_width                 = BBE_BATCH;
   uint16_t num_vec                   = num_cur_mov_dets / vec_width;
   float32_t vec_range_arr[BBE_BATCH] = {0.0F};
   float32_t vec_phi_arr[BBE_BATCH]   = {0.0F};
   float32_t vec_cos_theta[BBE_BATCH] = {0.0F};
   float32_t vec_sin_theta[BBE_BATCH] = {0.0F};
   uint16_t *p_mov_det_idx            = &p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[0];

   xb_vecN_2xf32 v_1_by_2pi = (xb_vecN_2xf32)(XT_DIV_S(1.0F, 2.0F * RADAR_PI));
   xb_vecN_2xf32 v_int_2_float;
   xb_vecN_2x32v v_float_2_int;
   xb_vecN_2xf32 v_sin_phi, v_cos_phi;
   xb_vecN_2xf32 *p_phi       = (xb_vecN_2xf32 *)&vec_phi_arr[0];
   xb_vecN_2xf32 *p_range     = (xb_vecN_2xf32 *)&vec_range_arr[0];
   xb_vecN_2xf32 *p_cos_theta = (xb_vecN_2xf32 *)&vec_cos_theta[0];
   xb_vecN_2xf32 *p_sin_theta = (xb_vecN_2xf32 *)&vec_sin_theta[0];
   xb_vecN_2xf32 *p_det_pos_x = (xb_vecN_2xf32 *)&p_rdu_buffer->present_scan_s2_mov_det.det_pos_x[0];
   xb_vecN_2xf32 *p_det_pos_y = (xb_vecN_2xf32 *)&p_rdu_buffer->present_scan_s2_mov_det.det_pos_y[0];
   xb_vecN_2xf32 *p_det_pos_z = (xb_vecN_2xf32 *)&p_rdu_buffer->present_scan_s2_mov_det.det_pos_z[0];
   xb_vecN_2xf32 v_phi, v_ran, v_cos_theta, v_sin_theta;
   xb_vecN_2xf32 v_acc;
   xb_vecN_2xf32 v_temp1, v_temp2, v_ran_sq;
   xb_vecN_2xf32 v_vx, v_vy, v_vz;
   xb_vecN_2xf32 v_c;
   uint16_t idx0, idx1, idx2, idx3, idx4, idx5, idx6, idx7;

   for (i = 0; i < num_vec; i++)
   {
      /*load 8 indices - sequential pointer walk, no loop-carried dependency */
      idx0 = *p_mov_det_idx++;
      idx1 = *p_mov_det_idx++;
      idx2 = *p_mov_det_idx++;
      idx3 = *p_mov_det_idx++;
      idx4 = *p_mov_det_idx++;
      idx5 = *p_mov_det_idx++;
      idx6 = *p_mov_det_idx++;
      idx7 = *p_mov_det_idx++;

      vec_range_arr[0] = p_rdu_internals->cur_range[idx0];
      vec_range_arr[1] = p_rdu_internals->cur_range[idx1];
      vec_range_arr[2] = p_rdu_internals->cur_range[idx2];
      vec_range_arr[3] = p_rdu_internals->cur_range[idx3];
      vec_range_arr[4] = p_rdu_internals->cur_range[idx4];
      vec_range_arr[5] = p_rdu_internals->cur_range[idx5];
      vec_range_arr[6] = p_rdu_internals->cur_range[idx6];
      vec_range_arr[7] = p_rdu_internals->cur_range[idx7];

      vec_phi_arr[0] = p_rdu_internals->cur_phi[idx0];
      vec_phi_arr[1] = p_rdu_internals->cur_phi[idx1];
      vec_phi_arr[2] = p_rdu_internals->cur_phi[idx2];
      vec_phi_arr[3] = p_rdu_internals->cur_phi[idx3];
      vec_phi_arr[4] = p_rdu_internals->cur_phi[idx4];
      vec_phi_arr[5] = p_rdu_internals->cur_phi[idx5];
      vec_phi_arr[6] = p_rdu_internals->cur_phi[idx6];
      vec_phi_arr[7] = p_rdu_internals->cur_phi[idx7];

      vec_cos_theta[0] = p_rdu_internals->cos_theta[idx0];
      vec_cos_theta[1] = p_rdu_internals->cos_theta[idx1];
      vec_cos_theta[2] = p_rdu_internals->cos_theta[idx2];
      vec_cos_theta[3] = p_rdu_internals->cos_theta[idx3];
      vec_cos_theta[4] = p_rdu_internals->cos_theta[idx4];
      vec_cos_theta[5] = p_rdu_internals->cos_theta[idx5];
      vec_cos_theta[6] = p_rdu_internals->cos_theta[idx6];
      vec_cos_theta[7] = p_rdu_internals->cos_theta[idx7];

      vec_sin_theta[0] = p_rdu_internals->sin_theta[idx0];
      vec_sin_theta[1] = p_rdu_internals->sin_theta[idx1];
      vec_sin_theta[2] = p_rdu_internals->sin_theta[idx2];
      vec_sin_theta[3] = p_rdu_internals->sin_theta[idx3];
      vec_sin_theta[4] = p_rdu_internals->sin_theta[idx4];
      vec_sin_theta[5] = p_rdu_internals->sin_theta[idx5];
      vec_sin_theta[6] = p_rdu_internals->sin_theta[idx6];
      vec_sin_theta[7] = p_rdu_internals->sin_theta[idx7];

      /* load arr to vec */
      v_phi       = BBE_LVN_2XF32_I(p_phi, 0U);
      v_ran       = BBE_LVN_2XF32_I(p_range, 0U);
      v_cos_theta = BBE_LVN_2XF32_I(p_cos_theta, 0U);
      v_sin_theta = BBE_LVN_2XF32_I(p_sin_theta, 0U);

      /* calculate sin(phi) :
        x = x *(1/2*pi)
        x = abs(x + 0.25f)
       x = -0.5f + x - (float)(int)x
       x = x*x
       x = (((((6.565280458e+00f) * x + (-2.599369394e+01f)) * x + (6.017455525e+01f)
            ) * x + (-8.545097926e+01f)
            ) * x + (6.493916301e+01f)
            ) * x + (-1.973920539e+01f)
            ) * x +
            (9.999999918e-01f);
       */
      v_temp1       = BBE_MULN_2XF32(v_phi, v_1_by_2pi);                             /* x = x *(1/2*pi) */
      v_temp2       = BBE_ABSN_2XF32(BBE_ADDN_2XF32(v_temp1, (xb_vecN_2xf32)0.25F)); /* x = abs(x + 0.25f) */
      v_float_2_int = xb_vecN_2xf32_rtor_xb_vecN_2x32v(v_temp2);                     /* (int)x */
      v_int_2_float = xb_vecN_2x32v_rtor_xb_vecN_2xf32(v_float_2_int);               /* (float)(int)x */
      v_temp1       = BBE_SUBN_2XF32(v_temp2, (xb_vecN_2xf32)0.5f);                  /* x = -0.5f + x  */
      v_temp2       = BBE_SUBN_2XF32(v_temp1, v_int_2_float);                        /* x = x - (float)(int)x */
      v_ran_sq      = BBE_MULN_2XF32(v_temp2, v_temp2);                              /* x_sq = x*x */
      /* p = c6 */
      v_acc = (xb_vecN_2xf32)(6.565280458e+00f);
      /* p = p*x + c5 */
      v_c = (xb_vecN_2xf32)(-2.599369394e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      /* p = p*x + c4 */
      v_c = (xb_vecN_2xf32)(6.017455525e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      /* p = p*x + c3 */
      v_c = (xb_vecN_2xf32)(-8.545097926e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      /* p = p*x + c2 */
      v_c = (xb_vecN_2xf32)(6.493916301e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      /* p = p*x + c1 */
      v_c = (xb_vecN_2xf32)(-1.973920539e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      /* p = p*x + c0 */
      v_c = (xb_vecN_2xf32)(9.999999918e-01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_sin_phi = v_c;

      /* calculate cos(phi) = sin( phi + pi/2 ) */
      v_temp2       = BBE_ADDN_2XF32(v_phi, (xb_vecN_2xf32)RADAR_PI_BY_2);
      v_temp1       = BBE_MULN_2XF32(v_temp2, v_1_by_2pi);                           /* x = x *(1/2*pi) */
      v_temp2       = BBE_ABSN_2XF32(BBE_ADDN_2XF32(v_temp1, (xb_vecN_2xf32)0.25F)); /* x = abs(x + 0.25f) */
      v_float_2_int = xb_vecN_2xf32_rtor_xb_vecN_2x32v(v_temp2);                     /* (int)x */
      v_int_2_float = xb_vecN_2x32v_rtor_xb_vecN_2xf32(v_float_2_int);               /* (float)(int)x */
      v_temp1       = BBE_SUBN_2XF32(v_temp2, (xb_vecN_2xf32)0.5f);                  /* x = -0.5f + x  */
      v_temp2       = BBE_SUBN_2XF32(v_temp1, v_int_2_float);                        /* x = x - (float)(int)x */
      v_ran_sq      = BBE_MULN_2XF32(v_temp2, v_temp2);                              /* x_sq = x*x */
      v_acc         = (xb_vecN_2xf32)(6.565280458e+00f);
      v_c           = (xb_vecN_2xf32)(-2.599369394e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      v_c   = (xb_vecN_2xf32)(6.017455525e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      v_c   = (xb_vecN_2xf32)(-8.545097926e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      v_c   = (xb_vecN_2xf32)(6.493916301e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      v_c   = (xb_vecN_2xf32)(-1.973920539e+01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_acc = v_c;
      v_c   = (xb_vecN_2xf32)(9.999999918e-01f);
      BBE_MULAN_2XF32(v_c, v_acc, v_ran_sq);
      v_cos_phi = v_c;

      v_vz    = BBE_MULN_2XF32(v_ran, v_sin_phi); /* vz = range * sin(phi)  */
      v_temp1 = BBE_MULN_2XF32(v_ran, v_cos_phi);
      v_vx    = BBE_MULN_2XF32(v_temp1, v_cos_theta); /* vx =  range * cos(phi) * cos(theta) */
      v_vy    = BBE_MULN_2XF32(v_temp1, v_sin_theta); /* vy = range * cos(phi) * sin(theta) */

      /* compute pos in x,y,z and store */
      /* pos_x = (vx * Rot_M_00) + (vy * Rot_M_01) + (vz * Rot_M_02) + mount_x */
      v_acc = BBE_MULN_2XF32(v_vx, (xb_vecN_2xf32)Rot_M_00);
      BBE_MULAN_2XF32(v_acc, v_vy, (xb_vecN_2xf32)Rot_M_01);
      BBE_MULAN_2XF32(v_acc, v_vz, (xb_vecN_2xf32)Rot_M_02);
      v_temp1 = BBE_ADDN_2XF32(v_acc, (xb_vecN_2xf32)mount_x);
      BBE_SVN_2XF32_IP(v_temp1, p_det_pos_x, 32U);

      /* pos_y = (vx * Rot_M_10) + (vy * Rot_M_11) + (vz * Rot_M_12) + mount_y */
      v_acc = BBE_MULN_2XF32(v_vx, (xb_vecN_2xf32)Rot_M_10);
      BBE_MULAN_2XF32(v_acc, v_vy, (xb_vecN_2xf32)Rot_M_11);
      BBE_MULAN_2XF32(v_acc, v_vz, (xb_vecN_2xf32)Rot_M_12);
      v_temp1 = BBE_ADDN_2XF32(v_acc, (xb_vecN_2xf32)mount_y);
      BBE_SVN_2XF32_IP(v_temp1, p_det_pos_y, 32U);

      /* pos_z = (vx * Rot_M_20) + (vy * Rot_M_21) + (vz * Rot_M_22) */
      v_acc = BBE_MULN_2XF32(v_vx, (xb_vecN_2xf32)Rot_M_20);
      BBE_MULAN_2XF32(v_acc, v_vy, (xb_vecN_2xf32)Rot_M_21);
      BBE_MULAN_2XF32(v_acc, v_vz, (xb_vecN_2xf32)Rot_M_22);
      BBE_SVN_2XF32_IP(v_acc, p_det_pos_z, 32U);
   }
   for (i = (num_vec)*8; i < num_cur_mov_dets; i++)
   {
      det_idx = p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[i];
      range   = p_rdu_internals->cur_range[det_idx];

      float32_t sin_phi   = rm_sinf(p_rdu_internals->cur_phi[det_idx]);
      float32_t cos_theta = p_rdu_internals->cos_theta[det_idx];
      vz                  = range * sin_phi;
      vx                  = range * rm_sinf(p_rdu_internals->cur_phi[det_idx] + RADAR_PI_BY_2) * cos_theta;
      float32_t cos_phi   = rm_sinf(p_rdu_internals->cur_phi[det_idx] + RADAR_PI_BY_2);
      float32_t sin_theta = p_rdu_internals->sin_theta[det_idx];
      vy                  = range * cos_phi * sin_theta;

      p_rdu_buffer->present_scan_s2_mov_det.det_pos_x[i] = (vx * Rot_M_00) + (vy * Rot_M_01) + (vz * Rot_M_02) + mount_x;
      p_rdu_buffer->present_scan_s2_mov_det.det_pos_y[i] = (vx * Rot_M_10) + (vy * Rot_M_11) + (vz * Rot_M_12) + mount_y;
      p_rdu_buffer->present_scan_s2_mov_det.det_pos_z[i] = (vx * Rot_M_20) + (vy * Rot_M_21) + (vz * Rot_M_22);
      /* height from ground (z mount offset) is not added per spec */
   }
#else
   for (i = 0; i < (uint16_t)num_cur_mov_dets; i++)
   {
      det_idx = p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[i];
      range   = p_rdu_internals->cur_range[det_idx];

      float32_t sin_phi   = rm_sinf(p_rdu_internals->cur_phi[det_idx]);
      float32_t cos_theta = p_rdu_internals->cos_theta[det_idx];
      vz                  = range * sin_phi;
      vx                  = range * rm_sinf(p_rdu_internals->cur_phi[det_idx] + RADAR_PI_BY_2) * cos_theta;
      float32_t cos_phi   = rm_sinf(p_rdu_internals->cur_phi[det_idx] + RADAR_PI_BY_2);
      float32_t sin_theta = p_rdu_internals->sin_theta[det_idx];
      vy                  = range * cos_phi * sin_theta;

      p_rdu_buffer->present_scan_s2_mov_det.det_pos_x[i] = (vx * Rot_M_00) + (vy * Rot_M_01) + (vz * Rot_M_02) + mount_x;
      p_rdu_buffer->present_scan_s2_mov_det.det_pos_y[i] = (vx * Rot_M_10) + (vy * Rot_M_11) + (vz * Rot_M_12) + mount_y;
      p_rdu_buffer->present_scan_s2_mov_det.det_pos_z[i] = (vx * Rot_M_20) + (vy * Rot_M_21) + (vz * Rot_M_22);
      /* height from ground (z mount offset) is not added per spec */
   }
#endif
}

/**
 * @brief Identifies the N-nearest neighbours between two scans for configurable K.
 *
 * Generalises Cartesian_Knn() to support any number of neighbours up to
 * RDU_MAX_K_NEIGHBOURS, reading the desired count from
 * p_rdu_params->num_neighbors.
 *
 * @param[in,out] p_rdu_data      Pointer to RDU data structure
 * @param[in]     p_rdu_params    Pointer to RDU parameters structure
 * @param[in,out] p_rdu_internals Pointer to RDU internals structure
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return void
 *
 * @ref Matlab function reference: cartesian_KNN() of Associate_detections_between_scans()
 */
void Cartesian_N_Knn(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                     RDU_Buffer_T *p_rdu_buffer)
{
   (void)p_rdu_data;
   const int16_t num_cur_mov_dets  = (int16_t)p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets;
   const int16_t num_prev_mov_dets = (int16_t)p_rdu_buffer->prev_scan_s2_mov_det.num_moving_dets;
   const float32_t max_dist_sq     = p_rdu_params->euclidean_max_dist_m * p_rdu_params->euclidean_max_dist_m;
   const uint8_t k_max =
      (p_rdu_params->num_neighbors < (uint8_t)RDU_MAX_K_NEIGHBOURS) ? p_rdu_params->num_neighbors : (uint8_t)RDU_MAX_K_NEIGHBOURS;

   const float32_t *restrict p_prev_pos_x  = &p_rdu_buffer->prev_scan_s2_mov_det.det_pos_x[0];
   const float32_t *restrict p_prev_pos_y  = &p_rdu_buffer->prev_scan_s2_mov_det.det_pos_y[0];
   const uint16_t *restrict p_prev_mov_idx = &p_rdu_buffer->prev_scan_s2_mov_det.moving_det_idx[0];
   const float32_t *restrict p_prev_vel    = p_rdu_buffer->prev_scan_det_data.prev_vel;
   const float32_t *restrict p_prev_theta  = p_rdu_buffer->prev_scan_det_data.prev_theta;

   float32_t knn_d[RDU_MAX_K_NEIGHBOURS];  /* sorted squared distances (ascending) */
   uint16_t knn_idx[RDU_MAX_K_NEIGHBOURS]; /* sorted moving-buffer indices */
   float32_t knn_rr[RDU_MAX_K_NEIGHBOURS]; /* sorted prev-scan range rates [m/s] */
   float32_t knn_az[RDU_MAX_K_NEIGHBOURS]; /* sorted prev-scan azimuths [deg] */

   int16_t i, j;
   uint16_t n_mov_det_idx;
   uint16_t prev_global_idx;
   float32_t cur_x, cur_y;
   float32_t dx, dy, dist_sq;
   float32_t prev_rr_j, prev_az_j;
   float32_t knn_worst; /* register cache of knn_d[k_max-1] */
   int16_t num_nb;
   uint8_t k;
   Cart_Knn_Output_T *p_knn;

   for (i = 0; i < num_cur_mov_dets; i++)
   {
      n_mov_det_idx = p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[i];
      p_knn         = &p_rdu_internals->cart_knn_output[i];

      p_knn->cur_scan_detidx = n_mov_det_idx;
      p_knn->cur_rr          = p_rdu_internals->cur_vel[n_mov_det_idx];
      p_knn->cur_az_deg      = p_rdu_internals->cur_phi[n_mov_det_idx];

      cur_x = p_rdu_buffer->present_scan_s2_mov_det.det_pos_x[i];
      cur_y = p_rdu_buffer->present_scan_s2_mov_det.det_pos_y[i];

      /* Initialise the K-slot sorted list */
      for (k = 0U; k < k_max; k++)
      {
         knn_d[k]   = FLT_MAX;
         knn_idx[k] = UINT16_MAX;
         knn_rr[k]  = 0.0F;
         knn_az[k]  = 0.0F;
      }
      num_nb    = 0;
      knn_worst = FLT_MAX;

      for (j = 0; j < num_prev_mov_dets; j++)
      {
         dx      = cur_x - p_prev_pos_x[j];
         dy      = cur_y - p_prev_pos_y[j];
         dist_sq = (dx * dx) + (dy * dy);

         if (dist_sq > max_dist_sq)
         {
            continue;
         } /* outside search radius */
         if (dist_sq >= knn_worst)
         {
            continue;
         } /* farther than current worst slot */

         /* Fetch prev-scan attributes only on confirmed insertion */
         prev_global_idx = p_prev_mov_idx[j];
         prev_rr_j       = p_prev_vel[prev_global_idx];
         prev_az_j       = p_prev_theta[prev_global_idx];

         insert_into_topk_n(knn_d, knn_idx, knn_rr, knn_az, k_max, dist_sq, (uint16_t)j, prev_rr_j, prev_az_j);
         knn_worst = knn_d[(uint8_t)(k_max - 1U)];
         if (num_nb < (int16_t)k_max)
         {
            num_nb++;
         }
      }

      p_knn->num_neighbours = (int8_t)num_nb;
      p_knn->f_matched      = (num_nb > 0);
      for (k = 0U; k < k_max; k++)
      {
         p_knn->euclidean_dist[k]   = (num_nb > (int16_t)k) ? rdu_sqrtf(knn_d[k]) : FLT_MAX;
         p_knn->prev_scan_detidx[k] = knn_idx[k];
         p_knn->prev_scan_rr[k]     = knn_rr[k];
         p_knn->prev_scan_az_deg[k] = knn_az[k];
      }
   }
}

#ifdef BBE_ENABLE
void Cartesian_N_Knn_Opt(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                         RDU_Buffer_T *p_rdu_buffer)
{
   (void)p_rdu_data;
   const int16_t num_cur_mov_dets  = (int16_t)p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets;
   const int16_t num_prev_mov_dets = (int16_t)p_rdu_buffer->prev_scan_s2_mov_det.num_moving_dets;
   const float32_t max_dist_sq     = p_rdu_params->euclidean_max_dist_m * p_rdu_params->euclidean_max_dist_m;
   const uint8_t k_max =
      (p_rdu_params->num_neighbors < (uint8_t)RDU_MAX_K_NEIGHBOURS) ? p_rdu_params->num_neighbors : (uint8_t)RDU_MAX_K_NEIGHBOURS;

   const uint16_t *restrict p_prev_mov_idx = &p_rdu_buffer->prev_scan_s2_mov_det.moving_det_idx[0];
   const float32_t *restrict p_prev_vel    = p_rdu_buffer->prev_scan_det_data.prev_vel;
   const float32_t *restrict p_prev_theta  = p_rdu_buffer->prev_scan_det_data.prev_theta;

   float32_t knn_d[RDU_MAX_K_NEIGHBOURS];  /* sorted squared distances (ascending) */
   uint16_t knn_idx[RDU_MAX_K_NEIGHBOURS]; /* sorted moving-buffer indices */
   float32_t knn_rr[RDU_MAX_K_NEIGHBOURS]; /* sorted prev-scan range rates [m/s] */
   float32_t knn_az[RDU_MAX_K_NEIGHBOURS]; /* sorted prev-scan azimuths [deg] */

   int16_t i, j;
   uint16_t n_mov_det_idx;
   // float32_t cur_x, cur_y;
   uint16_t prev_global_idx;
   float32_t batch_rr, batch_az;
   float32_t knn_worst;
   int16_t num_nb;
   uint8_t k, m, extract_cnt;
   uint16_t batch_idx, base_idx;
   uint16_t lane_cnt_local;
   Cart_Knn_Output_T *restrict p_knn;

   /* BBE32 SIMD variables */
   xb_vecN_2xf32 v_cur_x, v_cur_y;
   xb_vecN_2xf32 v_prev_x, v_prev_y, v_rem_prev_x, v_rem_prev_y;
   xb_vecN_2xf32 *p_prev_pos_x;
   xb_vecN_2xf32 *p_prev_pos_y;
   xb_vecN_2xf32 v_dx, v_dy, v_dist_sq;
   xb_vecN_2xf32 v_temp1, v_temp2, vtemp3;
   vboolN_2 f_neighbour;
   vboolN_2 f_remaining;
   vboolN_2 f_used_in_batch;
   vboolN_2 f_min_m;
   vboolN_2 f_det_mask;
   vboolN_2 f_false = BBE_OLTN_2XF32((xb_vecN_2xf32)1.0F, (xb_vecN_2xf32)1.0F);
   vboolN f_min_N;
   vboolN f_min_N_local;
   float idxs[BBE_BATCH]                                       = {0.0F, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F};
   uint16_t __attribute__((aligned(32))) idx_values[BBE_BATCH] = {0U};
   uint16_t __attribute__((aligned(32))) idx_values_batch[XCHAL_BBEN_SIMD_WIDTH] = {0U};
   float32_t __attribute__((aligned(32))) temp_buff[BBE_BATCH]                   = {0.0F};
   xb_vecN_2xf32 *p_idxs                                                         = (xb_vecN_2xf32 *)&idx_values[0];
   xb_vecN_2xf32 *p_temp_buff                                                    = (xb_vecN_2xf32 *)&temp_buff[0];
   uint16_t set_flag_count;
   float32_t dmin_m;

   const uint16_t vec_width        = BBE_BATCH; /* = 8 */
   const uint16_t rem              = (uint16_t)num_prev_mov_dets % vec_width;
   const uint16_t num_full_batches = (uint16_t)((uint16_t)num_prev_mov_dets >> BBE_BATCH_LOG2);
   const uint16_t base_prev_idx    = (uint16_t)(num_prev_mov_dets - (int16_t)rem);

   const xb_vecN_2xf32 v_max_dist_sq = (xb_vecN_2xf32)max_dist_sq;

   p_idxs = (xb_vecN_2xf32 *)&idxs[0];
   BBE_LVN_2XF32_IP(vtemp3, p_idxs, 32U);
   f_det_mask = BBE_OLTN_2XF32(BBE_ADDN_2XF32(vtemp3, uint32_rtor_xb_vecN_2xf32((uint32_t)base_prev_idx)),
                               uint32_rtor_xb_vecN_2xf32((uint32_t)num_prev_mov_dets));

   if (rem > 0U)
   {
      p_temp_buff = (xb_vecN_2xf32 *)&temp_buff[0];
      memcpy((void *)&temp_buff[0], (void *)&p_rdu_buffer->prev_scan_s2_mov_det.det_pos_x[base_prev_idx], rem * sizeof(float32_t));
      BBE_LVN_2XF32_XP(v_rem_prev_x, p_temp_buff, (int32_t)(rem * sizeof(float32_t)));

      p_temp_buff = (xb_vecN_2xf32 *)&temp_buff[0];
      memcpy((void *)&temp_buff[0], (void *)&p_rdu_buffer->prev_scan_s2_mov_det.det_pos_y[base_prev_idx], rem * sizeof(float32_t));
      BBE_LVN_2XF32_XP(v_rem_prev_y, p_temp_buff, (int32_t)(rem * sizeof(float32_t)));
   }

   for (i = 0; i < num_cur_mov_dets; i++)
   {
      n_mov_det_idx = p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[i];
      p_knn         = &p_rdu_internals->cart_knn_output[i];

      p_knn->cur_scan_detidx = n_mov_det_idx;
      p_knn->cur_rr          = p_rdu_internals->cur_vel[n_mov_det_idx];
      p_knn->cur_az_deg      = p_rdu_internals->cur_phi[n_mov_det_idx];

      v_cur_x = (xb_vecN_2xf32)p_rdu_buffer->present_scan_s2_mov_det.det_pos_x[i];
      v_cur_y = (xb_vecN_2xf32)p_rdu_buffer->present_scan_s2_mov_det.det_pos_y[i];

      /* Initialise the K-slot sorted list */
      for (k = 0U; k < k_max; k++)
      {
         knn_d[k]   = FLT_MAX;
         knn_idx[k] = UINT16_MAX;
         knn_rr[k]  = 0.0F;
         knn_az[k]  = 0.0F;
      }
      num_nb    = 0;
      knn_worst = FLT_MAX;

      /* Reset prev-det vector pointers for this current detection */
      p_prev_pos_x = (xb_vecN_2xf32 *)&p_rdu_buffer->prev_scan_s2_mov_det.det_pos_x[0];
      p_prev_pos_y = (xb_vecN_2xf32 *)&p_rdu_buffer->prev_scan_s2_mov_det.det_pos_y[0];

      for (j = 0; j < (int16_t)num_full_batches; j++)
      {
         BBE_LVN_2XF32_IP(v_prev_x, p_prev_pos_x, 32U);
         BBE_LVN_2XF32_IP(v_prev_y, p_prev_pos_y, 32U);

         v_dx      = BBE_SUBN_2XF32(v_cur_x, v_prev_x);
         v_dy      = BBE_SUBN_2XF32(v_cur_y, v_prev_y);
         v_dist_sq = BBE_MULN_2XF32(v_dx, v_dx); /* dx^2 */
         BBE_MULAN_2XF32(v_dist_sq, v_dy, v_dy); /* dx^2 + dy^2 */

         f_neighbour = BBE_OLTN_2XF32(v_dist_sq, v_max_dist_sq);
         f_neighbour = BBE_OLEN_2XF32T(v_dist_sq, (xb_vecN_2xf32)knn_worst, f_neighbour);

         f_min_N = BBE_JOINBN(f_false, f_neighbour);
         BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, f_min_N);
         set_flag_count >>= 1U;

         if (set_flag_count > 0U)
         {
            /* Extract up to k_max (≤ 8 = vec_width) minimums from this batch */
            extract_cnt     = (uint8_t)((set_flag_count < (uint16_t)k_max) ? set_flag_count : (uint16_t)k_max);
            f_used_in_batch = f_false;
            base_idx        = (uint16_t)((uint16_t)j * vec_width);

            for (m = 0U; m < extract_cnt; m++)
            {
               f_remaining = BBE_XORBN_2(f_used_in_batch, f_neighbour);
               BBE_RBMINNUMN_2XF32T(f_min_m, dmin_m, v_dist_sq, f_remaining);
               f_used_in_batch = BBE_ORBN_2(f_used_in_batch, f_min_m);

               if (dmin_m < knn_worst)
               {
                  f_min_N_local = BBE_JOINBN(f_false, f_min_m);
                  BBE_SQZN(((vsaN *)idx_values_batch)[0], lane_cnt_local, f_min_N_local);
                  batch_idx       = base_idx + idx_values_batch[0];
                  prev_global_idx = p_prev_mov_idx[batch_idx];
                  batch_rr        = p_prev_vel[prev_global_idx];
                  batch_az        = p_prev_theta[prev_global_idx];

                  insert_into_topk_n(knn_d, knn_idx, knn_rr, knn_az, k_max, dmin_m, batch_idx, batch_rr, batch_az);
                  knn_worst = knn_d[(uint8_t)(k_max - 1U)];
                  if (num_nb < (int16_t)k_max)
                  {
                     num_nb++;
                  }
               }
            }
         }
      }

      if (rem > 0U)
      {
         v_dx      = BBE_SUBN_2XF32(v_cur_x, v_rem_prev_x);
         v_temp1   = BBE_MULN_2XF32(v_dx, v_dx);
         v_dy      = BBE_SUBN_2XF32(v_cur_y, v_rem_prev_y);
         v_temp2   = BBE_MULN_2XF32(v_dy, v_dy);
         v_dist_sq = BBE_ADDN_2XF32(v_temp1, v_temp2);

         /* Apply validity mask to suppress padding lanes */
         f_neighbour = BBE_OLTN_2XF32T(v_dist_sq, v_max_dist_sq, f_det_mask);
         f_neighbour = BBE_OLEN_2XF32T(v_dist_sq, (xb_vecN_2xf32)knn_worst, f_neighbour);

         f_min_N = BBE_JOINBN(f_false, f_neighbour);
         BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, f_min_N);
         set_flag_count >>= 1U;

         if (set_flag_count > 0U)
         {
            extract_cnt     = (uint8_t)((set_flag_count < (uint16_t)k_max) ? set_flag_count : (uint16_t)k_max);
            f_used_in_batch = f_false;

            for (m = 0U; m < extract_cnt; m++)
            {
               f_remaining = BBE_XORBN_2(f_used_in_batch, f_neighbour);
               BBE_RBMINNUMN_2XF32T(f_min_m, dmin_m, v_dist_sq, f_remaining);
               f_used_in_batch = BBE_ORBN_2(f_used_in_batch, f_min_m);

               if (dmin_m < knn_worst)
               {
                  f_min_N_local = BBE_JOINBN(f_false, f_min_m);
                  BBE_SQZN(((vsaN *)idx_values_batch)[0], lane_cnt_local, f_min_N_local);
                  batch_idx       = base_prev_idx + idx_values_batch[0];
                  prev_global_idx = p_prev_mov_idx[batch_idx];
                  batch_rr        = p_prev_vel[prev_global_idx];
                  batch_az        = p_prev_theta[prev_global_idx];

                  insert_into_topk_n(knn_d, knn_idx, knn_rr, knn_az, k_max, dmin_m, batch_idx, batch_rr, batch_az);
                  knn_worst = knn_d[(uint8_t)(k_max - 1U)];
                  if (num_nb < (int16_t)k_max)
                  {
                     num_nb++;
                  }
               }
            }
         }
      }

      /* Write-back to struct — once per current detection, after full j-loop */
      p_knn->num_neighbours = (int8_t)num_nb;
      p_knn->f_matched      = (num_nb > 0);
      for (k = 0U; k < k_max; k++)
      {
         p_knn->euclidean_dist[k]   = (num_nb > (int16_t)k) ? rdu_sqrtf(knn_d[k]) : FLT_MAX;
         p_knn->prev_scan_detidx[k] = knn_idx[k];
         p_knn->prev_scan_rr[k]     = knn_rr[k];
         p_knn->prev_scan_az_deg[k] = knn_az[k];
      }
   }
}
#endif

/**
 * @brief This function does disambiguation of associated detections
 *
 * This functions linear programming based disambiguation to find the
 * best association and Unfolding factor between present scan moving
 * detections and previous scan moving detections.
 *
 * @param[in,out] p_rdu_data      Pointer to RDU data structure
 * @param[in] p_rdu_params    Pointer to RDU parameters structure
 * @param[in,out] p_rdu_internals Pointer to RDU internals structure
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return bool - true if solution found, false otherwise
 *
 * @ref Matlab function reference: linprog_disambiguate_rr() of two cycle unfolding
 */
bool Linprog_Disambiguate(int16_t k2_min, int16_t k2_max, int16_t k1_min, int16_t k1_max, float32_t W1, float32_t W2,
                          float32_t delta_w, float32_t dt_half, float32_t rr_diff_amb, float32_t range_residual,
                          float32_t max_eps_range, int16_t *p_interval, float32_t *p_delta_rr, float32_t *p_eps_range,
                          int16_t *p_within_interval, float32_t *p_min_val, float32_t eps_threshold)
{
   static const float32_t one_third = 1.0F / 3.0F;

   /*  MAX_WITHIN_INTERVAL is max(K1_span, K2_span), capped to [1, 20] */
   const int16_t k1_span = k1_max - k1_min;
   const int16_t k2_span = k2_max - k2_min;
   int16_t max_within    = (k1_span > k2_span) ? k1_span : k2_span;
   if (max_within < 1)
   {
      max_within = 1;
   }
   if (max_within > 20)
   {
      max_within = 20;
   }

   /* solution value accumulator */
   float32_t best_obj       = FLT_MAX;
   bool found               = false;
   int16_t best_k2          = 0;
   int16_t best_k1          = 0;
   float32_t best_delta_rr  = 0.0F;
   float32_t best_eps_range = 0.0F;
   bool best_solution_found = false;

   int16_t within, k1, k2;
   float32_t threshold;
   int16_t k1_lo, k1_hi;

   /* computed per k2 iteration */
   float32_t db_k2_term;
   float32_t w2_k2;
   float32_t w1_k2;
   float32_t neg_w2_k2;
   float32_t abs_k2;
   float32_t w1_third_k2;

   /* k1 iteration accumulator */
   float32_t w1_k1;

   /* Per-k1 computed values */
   float32_t eps_rr;
   float32_t neg_wrap_sum;
   float32_t nws_diff;
   float32_t nws_sum;
   float32_t eps_range;
   float32_t obj_val;

   for (within = 0; (within <= max_within) && !found; within++)
   {
      threshold = (0.5F * delta_w) + (1.5F * (float32_t)within * W1);

      for (k2 = k2_min; (k2 <= k2_max) && !best_solution_found; k2++)
      {
         k1_lo = (int16_t)XT_MAX(k2 - within, (int32_t)k1_min);
         k1_hi = (int16_t)XT_MIN(k2 + within, (int32_t)k1_max);

         /* k2-invariant */
         db_k2_term  = rr_diff_amb + ((float32_t)k2 * delta_w);
         w2_k2       = W2 * (float32_t)k2;
         w1_k2       = W1 * (float32_t)k2;
         neg_w2_k2   = -w2_k2;
         abs_k2      = rdu_absf((float32_t)k2);
         w1_third_k2 = one_third * w1_k2;

         w1_k1 = XT_MUL_S(W1, (float32_t)k1_lo);

         for (k1 = k1_lo; (k1 <= k1_hi) && !best_solution_found; k1++, w1_k1 += W1)
         {
            /* Check 1: |db_k2_term - (1/3)*(w1_k2 - w1_k1)| > threshold */
            if (rdu_absf(db_k2_term - (w1_third_k2 - (one_third * w1_k1))) > threshold)
            {
               continue;
            }

            /* Check 2: eps_rr = |(neg_w2_k2 + w1_k1) - rr_diff_amb| > eps_threshold */
            eps_rr = rdu_absf((neg_w2_k2 + w1_k1) - rr_diff_amb);
            if (eps_rr > eps_threshold)
            {
               continue;
            }

            /* Check 3: eps_range > max_eps_range */
            neg_wrap_sum = neg_w2_k2 - w1_k1;
            XT_ADDSUB_S(nws_diff, nws_sum, neg_wrap_sum, eps_rr);
            {
               float32_t eps_r1 = rdu_absf((dt_half * nws_diff) + range_residual);
               float32_t eps_r2 = rdu_absf((dt_half * nws_sum) + range_residual);
               eps_range        = (eps_r1 > eps_r2) ? eps_r1 : eps_r2;
            }
            if (eps_range > max_eps_range)
            {
               continue;
            }

            obj_val = eps_rr + abs_k2 + rdu_absf((float32_t)k1) + eps_range;
            if (obj_val < best_obj)
            {
               best_obj       = obj_val;
               best_k2        = k2;
               best_k1        = k1;
               best_delta_rr  = eps_rr;
               best_eps_range = eps_range;
               found          = true;
               if ((best_delta_rr < 1e-10F) & (best_eps_range < 1e-10F))
               {
                  best_solution_found = true;
               }
            }
         }
      }
   }

   /* Unified write-back — single path, no duplicate block */
   if (found)
   {
      *p_interval        = best_k2;
      *p_delta_rr        = best_delta_rr;
      *p_within_interval = (int16_t)(best_k2 - best_k1);
      *p_eps_range       = best_eps_range;
      *p_min_val         = best_obj;
   }
   return found;
}

/**
 * @brief This function does disambiguation of associated detections in Cartesian KNN
 *
 * This functions takes cartesian KNN output and applies linear programming based
 * disambiguation to find the best association and Unfolding factor between present
 * scan moving detections and previous scan moving detections.
 *
 * @param[in,out] p_rdu_data      Pointer to RDU data structure
 * @param[in] p_rdu_params    Pointer to RDU parameters structure
 * @param[in,out] p_rdu_internals Pointer to RDU internals structure
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return void
 *
 * @ref Matlab function reference: run_algorithm_single(), linprog_disambiguate_rr() of two cycle unfolding
 */
void Stage3_Disambiguation(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                           RDU_Buffer_T *p_rdu_buffer)
{
   /* Cache deep pointer chains — eliminates N-level dereference on every access */
   RDU_Stage3_Output_Data_T *restrict p_out = p_rdu_data->p_rdu_stage3_output_data;
   uint16_t *restrict p_prev_mov_buf        = &p_rdu_buffer->prev_scan_s2_mov_det.moving_det_idx[0];

   const uint8_t cur_look  = p_rdu_buffer->present_scan_s2_mov_det.look_type;
   const uint8_t prev_look = p_rdu_buffer->prev_scan_s2_mov_det.look_type;

   const float32_t W2            = p_rdu_params->vwrapping_mps[cur_look];
   const float32_t W1            = p_rdu_params->vwrapping_mps[prev_look];
   const float32_t delta_w       = rdu_absf(W1 - W2);
   const float32_t eps_threshold = 0.5F * delta_w;
   const float32_t dt = (float32_t)(p_rdu_buffer->present_scan_s2_mov_det.scan_idx - p_rdu_buffer->prev_scan_s2_mov_det.scan_idx) *
                        p_rdu_params->radar_cycle_time_s;

   const bool dt_delta_valid = (dt >= 1e-3f) & (delta_w >= 1e-6f);

   /* Pre-compute LP invariants once — passed to every Linprog_Disambiguate_Opt_new call */
   const float32_t lp_min_rdot      = p_rdu_params->min_rdot_mps - p_rdu_params->minimum_rangerate_mps;
   const float32_t lp_max_rdot      = p_rdu_params->max_rdot_mps - p_rdu_params->minimum_rangerate_mps;
   const int16_t lp_k2_min          = (int16_t)rdu_floorf(lp_min_rdot / W2);
   const int16_t lp_k2_max          = (int16_t)rdu_floorf(lp_max_rdot / W2);
   const int16_t lp_k1_min          = (int16_t)rdu_floorf(lp_min_rdot / W1);
   const int16_t lp_k1_max          = (int16_t)rdu_floorf(lp_max_rdot / W1);
   const float32_t lp_max_eps_range = p_rdu_params->max_eps_range_m;
   const float32_t lp_dt_half       = 0.5F * dt;

   uint16_t cur_det_idx;
   uint16_t prev_mov_idx;
   uint16_t prev_det_idx;
   int8_t stage2_status;
   float32_t rr_amb_cur, range_cur, rr_prev, range_prev, range_diff;
   int16_t interval, within_interval;
   float32_t delta_rr, eps_range, min_val;
   bool solution_found;
   bool has_solution;
   float32_t best_min_val;
   float32_t best_delta_rr;
   int16_t best_interval;
   float32_t best_eps_range;
   int16_t best_within_interval;
   float32_t rr_unamb, conf, conf_r;
   float32_t lp_rr_diff_amb, lp_range_residual;
   uint8_t num_nb;

   uint16_t i, j;

   const uint16_t num_mov_dets = p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets;
   const Cart_Knn_Output_T *restrict p_knn_i;

   for (i = 0U; i < num_mov_dets; i++)
   {
      cur_det_idx   = p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[i];
      stage2_status = p_rdu_data->p_rdu_stage2_output_data->stage2_rrr_motion_status[cur_det_idx];

      p_knn_i = &p_rdu_internals->cart_knn_output[i];

      rr_amb_cur = p_knn_i->cur_rr;
      range_cur  = p_rdu_internals->cur_range[cur_det_idx];

      has_solution         = false;
      best_min_val         = FLT_MAX;
      best_delta_rr        = FLT_MAX;
      best_interval        = 0;
      best_eps_range       = FLT_MAX;
      best_within_interval = 0;

      if (!p_knn_i->f_matched)
      {
         if (stage2_status == RDU_MOTION_STATUS_MOVING_SPECIAL)
         {
            p_out->stage3_rrr_motion_status[cur_det_idx] = RDU_MOTION_STATUS_AMBIGUOUS; // if no match found make it AMBIGUOUS
            p_out->stage3_classification_confidence[cur_det_idx] = 0.5F;
         }
         continue;
      }

      if (stage2_status == RDU_MOTION_STATUS_MOVING)
      {
         p_out->stage3_rrr_motion_status[cur_det_idx] =
            RDU_MOTION_STATUS_MOVING_ASSOCIATED; // MOVING -> ASSOCIATED ; if matched in stage 3
      }
      else if (stage2_status == RDU_MOTION_STATUS_MOVING_SPECIAL)
      {
         p_out->stage3_rrr_motion_status[cur_det_idx] =
            RDU_MOTION_STATUS_MOVING_SPECIAL; // MOVING_SPECIAL -> MOVING_SPECIAL ; even if matched in stage 3
      }

      num_nb = (uint8_t)p_knn_i->num_neighbours;
      for (j = 0U; j < (uint16_t)num_nb; j++)
      {
         prev_mov_idx = p_knn_i->prev_scan_detidx[j];
         if (prev_mov_idx == UINT16_MAX)
         {
            continue;
         }

         prev_det_idx = p_prev_mov_buf[prev_mov_idx];
         rr_prev      = p_knn_i->prev_scan_rr[j];
         range_prev   = p_rdu_buffer->prev_scan_det_data.prev_range[prev_det_idx];
         range_diff   = range_cur - range_prev;

         lp_rr_diff_amb    = rr_amb_cur - rr_prev;
         lp_range_residual = range_diff - (lp_dt_half * (rr_amb_cur + rr_prev));
         solution_found =
            dt_delta_valid && Linprog_Disambiguate(lp_k2_min, lp_k2_max, lp_k1_min, lp_k1_max, W1, W2, delta_w, lp_dt_half,
                                                   lp_rr_diff_amb, lp_range_residual, lp_max_eps_range, &interval, &delta_rr,
                                                   &eps_range, &within_interval, &min_val, eps_threshold);

         if (solution_found && (min_val < best_min_val)) /* Best solution is one with min obj*/
         {
            best_delta_rr        = delta_rr;
            best_interval        = interval;
            best_eps_range       = eps_range;
            best_within_interval = within_interval;
            best_min_val         = min_val;
            has_solution         = true;
         }
      }

      if (has_solution)
      {
         rr_unamb = rr_amb_cur + ((float32_t)best_interval * W2);
         conf     = 1.0F - (best_delta_rr / eps_threshold);
         conf     = XT_MAX_S(conf, 0.0F);
         conf_r   = 1.0F - ((float32_t)XT_ABS_S(best_eps_range) / lp_max_eps_range);
         conf_r   = XT_MAX_S(conf_r, 0.0F);

         p_out->stage3_rdu_unamb_range_rate[cur_det_idx] = rr_unamb;
         p_out->stage3_wrapping_k[cur_det_idx]           = (int8_t)best_interval;
         p_out->stage3_unfolding_confidence[cur_det_idx] = conf * conf_r;
         p_rdu_internals->stage3_eps_range[cur_det_idx]  = best_eps_range; /* moved from output tag */
         p_out->stage3_delta_rr[cur_det_idx]             = best_delta_rr;
         p_out->stage3_within_interval[cur_det_idx]      = best_within_interval;
         p_rdu_internals->best_obj[cur_det_idx]          = best_min_val;
      }
   }
}

#ifdef BBE_ENABLE
bool Linprog_Disambiguate_Det_Vec(uint16_t num_nb, int16_t k2_min, int16_t k2_max, int16_t k1_min, int16_t k1_max, float32_t W1,
                                  float32_t W2, float32_t delta_w, float32_t dt_half, float32_t *p_vec_rr_diff_amb,
                                  float32_t *p_vec_range_residual, float32_t max_eps_range, int16_t *p_interval,
                                  float32_t *p_delta_rr, float32_t *p_eps_range, int16_t *p_within_interval, float32_t *p_min_obj,
                                  float32_t eps_threshold)
{
   static const float32_t one_third = 1.0F / 3.0F;

   const int16_t k1_span = k1_max - k1_min;
   const int16_t k2_span = k2_max - k2_min;
   int16_t max_within    = (k1_span > k2_span) ? k1_span : k2_span;
   if (max_within < 1)
   {
      max_within = 1;
   }
   if (max_within > 20)
   {
      max_within = 20;
   }

   /* Loop variables */
   int16_t within, k1, k2;

   /* derived variables */
   float32_t threshold;
   int16_t k1_lo, k1_hi;

   /* k2-invariant scalars */
   float32_t w2_k2;
   float32_t w1_k2;
   float32_t neg_w2_k2;
   float32_t abs_k2;
   float32_t w1_third_k2;

   /* k1 iteration accumulator */
   float32_t w1_k1;

   uint16_t __attribute__((aligned(32))) idx_values[XCHAL_BBEN_SIMD_WIDTH];
   uint16_t set_flag_count;

   bool solution_found = false;

   xb_vecN_2xf32 *p_rr_diff_amb    = (xb_vecN_2xf32 *)p_vec_rr_diff_amb;
   xb_vecN_2xf32 *p_range_residual = (xb_vecN_2xf32 *)p_vec_range_residual;

   xb_vecN_2xf32 v_rr_diff_amb;
   xb_vecN_2xf32 v_range_residual;
   xb_vecN_2xf32 v_db_k2_term;
   xb_vecN_2xf32 v_threshold;
   xb_vecN_2xf32 v_eps_rr;
   xb_vecN_2xf32 v_eps_range;
   xb_vecN_2xf32 v_obj_val;
   xb_vecN_2xf32 v_temp1, v_temp2;

   /* FIX 1: v_best_obj is a per-lane vector  updated via BBE_MOVN_2XF32T,
    * never broadcast from a single scalar winner. */
   xb_vecN_2xf32 v_best_obj = (xb_vecN_2xf32)FLT_MAX;

   /* FIX 2: per-lane tracking  each neighbour independently records its best (k2,k1) */
   xb_vecN_2xf32 v_per_lane_best_eps_rr    = (xb_vecN_2xf32)FLT_MAX;
   xb_vecN_2xf32 v_per_lane_best_eps_range = (xb_vecN_2xf32)0.0F;
   xb_vecN_2xf32 v_per_lane_best_k2        = (xb_vecN_2xf32)0.0F;
   xb_vecN_2xf32 v_per_lane_best_k1        = (xb_vecN_2xf32)0.0F;
   vboolN_2 f_per_lane_found;

   vboolN_2 f_pos_sol;
   vboolN_2 f_temp;
   vboolN_2 f_sol_found_nb;
   vboolN_2 f_false = BBE_OLTN_2XF32((xb_vecN_2xf32)1.0F, (xb_vecN_2xf32)1.0F);
   f_per_lane_found = f_false;

   vboolN f_nb_flag_N;
   vboolN_2 f_nb, f_n_2_temp;

   xb_vecNx16U vec_nb_idx = BBE_SEQNX16U();
   f_nb_flag_N            = vec_nb_idx < (xb_vecNx16U)(num_nb);
   BBE_EXTRACTBN(f_n_2_temp, f_nb, f_nb_flag_N);
   vboolN_2 f_active = f_nb; /* lanes still searching for their first (minimum-within-level) solution */

   v_rr_diff_amb    = BBE_LVN_2XF32_I(p_rr_diff_amb, 0U);
   v_range_residual = BBE_LVN_2XF32_I(p_range_residual, 0U);

   for (within = 0; within <= max_within; within++)
   {
      /* Rebuild active mask: valid lanes that have not yet found a solution.
       * Once a lane finds a solution at within=w, it must not update for within>w
       * (matches reference scalar LP which exits within loop at first successful within). */
      f_active = f_nb & ~f_per_lane_found;
      BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, BBE_JOINBN(f_false, f_active));
      if (set_flag_count == 0U)
      {
         break;
      } /* all valid lanes have solutions */

      threshold      = (0.5F * delta_w) + (1.5F * (float32_t)within * W1);
      f_sol_found_nb = f_false;
      v_threshold    = BBE_ADDN_2XF32((xb_vecN_2xf32)(0.5F * delta_w),
                                      BBE_MULN_2XF32((xb_vecN_2xf32)(1.5F * (float32_t)within), (xb_vecN_2xf32)W1));

      for (k2 = k2_min; k2 <= k2_max; k2++)
      {
         k1_lo = (int16_t)XT_MAX(k2 - within, (int32_t)k1_min);
         k1_hi = (int16_t)XT_MIN(k2 + within, (int32_t)k1_max);

         v_db_k2_term = v_rr_diff_amb + ((xb_vecN_2xf32)delta_w * (xb_vecN_2xf32)(float32_t)k2);
         w2_k2        = XT_MUL_S(W2, (float32_t)k2);
         w1_k2        = XT_MUL_S(W1, (float32_t)k2);
         neg_w2_k2    = -w2_k2;
         abs_k2       = XT_ABS_S((float32_t)k2);
         w1_third_k2  = one_third * w1_k2;

         w1_k1 = XT_MUL_S(W1, (float32_t)k1_lo);

         for (k1 = k1_lo; k1 <= k1_hi; k1++)
         {
            f_pos_sol = f_false;

            /* Check 1 */
            v_temp1   = BBE_ABSN_2XF32(v_db_k2_term - ((xb_vecN_2xf32)(w1_third_k2 - (one_third * w1_k1))));
            f_temp    = ~(v_threshold < v_temp1); /* MATLAB Condition > continue */
            f_pos_sol = f_temp | f_pos_sol;

            BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, BBE_JOINBN(f_false, f_pos_sol));
            if (set_flag_count > 0U)
            {
               /* Check 2 */
               v_eps_rr  = BBE_ABSN_2XF32((xb_vecN_2xf32)(neg_w2_k2 + w1_k1) - v_rr_diff_amb);
               f_temp    = ~((xb_vecN_2xf32)eps_threshold < v_eps_rr); /* MATLAB Condition > continue */
               f_pos_sol = f_temp & f_pos_sol;

               BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, BBE_JOINBN(f_false, f_pos_sol));
               if (set_flag_count > 0U)
               {
                  /* Check 3 */
                  v_temp1 = (xb_vecN_2xf32)(neg_w2_k2 - w1_k1);
                  BBE_ADDSUBN_2XF32(v_temp2, v_temp1, v_temp1, v_eps_rr);
                  v_eps_range = BBE_MAXN_2XF32(BBE_ABSN_2XF32((v_temp2 * (xb_vecN_2xf32)dt_half) + v_range_residual),
                                               BBE_ABSN_2XF32((v_temp1 * (xb_vecN_2xf32)dt_half) + v_range_residual));
                  f_temp      = ~((xb_vecN_2xf32)max_eps_range < v_eps_range); /* MATLAB Condition > continue */
                  f_pos_sol   = f_temp & f_pos_sol;

                  BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, BBE_JOINBN(f_false, f_pos_sol));
                  if (set_flag_count > 0U)
                  {
                     /* Objective  addition order matches reference: eps_rr + |k2| + |k1| + eps_range */
                     v_obj_val = v_eps_rr + ((xb_vecN_2xf32)abs_k2) + BBE_ABSN_2XF32((xb_vecN_2xf32)(float32_t)k1) + v_eps_range;

                     /* FIX 1: gate by per-lane best_obj (not a scalar broadcast) */
                     f_temp    = v_obj_val < v_best_obj;
                     f_pos_sol = f_pos_sol & f_temp;
                     f_pos_sol = f_pos_sol & f_active; /* only lanes still searching (valid + no solution yet) */

                     BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, BBE_JOINBN(f_false, f_pos_sol));
                     if (set_flag_count > 0U)
                     {
                        /* FIX 1: per-lane best_obj update  no broadcast */
                        v_best_obj = BBE_MOVN_2XF32T(v_obj_val, v_best_obj, f_pos_sol);
                        /* FIX 2: per-lane tracking of best eps_rr, eps_range, k2, k1 */
                        v_per_lane_best_eps_rr    = BBE_MOVN_2XF32T(v_eps_rr, v_per_lane_best_eps_rr, f_pos_sol);
                        v_per_lane_best_eps_range = BBE_MOVN_2XF32T(v_eps_range, v_per_lane_best_eps_range, f_pos_sol);
                        v_per_lane_best_k2        = BBE_MOVN_2XF32T((xb_vecN_2xf32)(float32_t)k2, v_per_lane_best_k2, f_pos_sol);
                        v_per_lane_best_k1        = BBE_MOVN_2XF32T((xb_vecN_2xf32)(float32_t)k1, v_per_lane_best_k1, f_pos_sol);
                        f_per_lane_found          = f_per_lane_found | f_pos_sol;
                        solution_found            = true;

                        /* Note: scalar best_solution_found removed, it would exit k2/k1 loops
                         * when any ONE lane finds a perfect solution, starving other active lanes.
                         * Correctness requires all active lanes to complete their k2/k1 search. */
                     }
                     /* v_best_obj already updated per-lane no broadcast needed */
                  }
               }
            }
            w1_k1 += W1;
         }
      }
   }

   if (solution_found)
   {
      /* FIX 2: post-loop reduction  pick neighbour with minimum best-eps_rr
       * (matches reference criterion: pick j by min delta_rr). */
      vboolN_2 f_valid_found        = f_per_lane_found & f_nb;
      xb_vecN_2xf32 v_gated_min_obj = BBE_MOVN_2XF32T(v_best_obj, (xb_vecN_2xf32)FLT_MAX, f_valid_found);
      vboolN_2 f_win;
      float32_t win_min_obj;
      BBE_RBMINNUMN_2XF32T(f_win, win_min_obj, v_gated_min_obj, f_valid_found);
      BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, BBE_JOINBN(f_false, f_win));
      if (set_flag_count > 0U)
      {
         float32_t __attribute__((aligned(32))) k2_arr_out[BBE_BATCH];
         float32_t __attribute__((aligned(32))) k1_arr_out[BBE_BATCH];
         float32_t __attribute__((aligned(32))) er_arr_out[BBE_BATCH];
         float32_t __attribute__((aligned(32))) min_val_out[BBE_BATCH];
         float32_t __attribute__((aligned(32))) eps_rr_arr_out[BBE_BATCH];
         BBE_SVN_2XF32_I(v_per_lane_best_k2, (xb_vecN_2xf32 *)k2_arr_out, 0);
         BBE_SVN_2XF32_I(v_per_lane_best_k1, (xb_vecN_2xf32 *)k1_arr_out, 0);
         BBE_SVN_2XF32_I(v_per_lane_best_eps_range, (xb_vecN_2xf32 *)er_arr_out, 0);
         BBE_SVN_2XF32_I(v_best_obj, (xb_vecN_2xf32 *)min_val_out, 0);
         BBE_SVN_2XF32_I(v_per_lane_best_eps_rr, (xb_vecN_2xf32 *)eps_rr_arr_out, 0);
         const uint16_t win_lane = idx_values[0];
         *p_interval             = (int16_t)k2_arr_out[win_lane];
         *p_delta_rr             = eps_rr_arr_out[win_lane];
         *p_within_interval      = (int16_t)((int16_t)k2_arr_out[win_lane] - (int16_t)k1_arr_out[win_lane]);
         *p_min_obj              = win_min_obj;
         *p_eps_range            = er_arr_out[win_lane];
      }
   }
   return solution_found;
}

void Stage3_Disambiguation_Det_Vec(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                                   RDU_Buffer_T *p_rdu_buffer)
{
   // RDU_Stage2_Output_Data_T *restrict p_stage2_out =p_rdu_data->p_rdu_stage2_output_data;
   RDU_Stage3_Output_Data_T *restrict p_out = p_rdu_data->p_rdu_stage3_output_data;
   /* Access detection data directly from internals and buffer */
   const uint16_t *restrict p_prev_mov_buf = &p_rdu_buffer->prev_scan_s2_mov_det.moving_det_idx[0];

   const uint8_t cur_look  = p_rdu_buffer->present_scan_s2_mov_det.look_type;
   const uint8_t prev_look = p_rdu_buffer->prev_scan_s2_mov_det.look_type;

   const float32_t W2            = p_rdu_params->vwrapping_mps[cur_look];
   const float32_t W1            = p_rdu_params->vwrapping_mps[prev_look];
   const float32_t delta_w       = rdu_absf(W1 - W2);
   const float32_t eps_threshold = 0.5F * delta_w;
   const float32_t dt = (float32_t)(p_rdu_buffer->present_scan_s2_mov_det.scan_idx - p_rdu_buffer->prev_scan_s2_mov_det.scan_idx) *
                        p_rdu_params->radar_cycle_time_s;

   const bool dt_delta_valid = (dt >= 1e-3f) & (delta_w >= 1e-6f);

   /* Pre-compute LP invariants once and passed to every Linprog_Disambiguate_Opt_new call */
   const float32_t lp_min_rdot      = p_rdu_params->min_rdot_mps - p_rdu_params->minimum_rangerate_mps;
   const float32_t lp_max_rdot      = p_rdu_params->max_rdot_mps - p_rdu_params->minimum_rangerate_mps;
   const int16_t lp_k2_min          = (int16_t)rdu_floorf(lp_min_rdot / W2);
   const int16_t lp_k2_max          = (int16_t)rdu_floorf(lp_max_rdot / W2);
   const int16_t lp_k1_min          = (int16_t)rdu_floorf(lp_min_rdot / W1);
   const int16_t lp_k1_max          = (int16_t)rdu_floorf(lp_max_rdot / W1);
   const float32_t lp_max_eps_range = p_rdu_params->max_eps_range_m;
   const float32_t lp_dt_half       = 0.5F * dt;

   uint16_t i, j;
   uint16_t cur_det_idx;
   uint16_t prev_mov_idx;
   uint16_t prev_det_idx;
   int8_t stage2_status;
   float32_t rr_amb_cur, range_cur, rr_prev, range_prev, range_diff;
   int16_t interval, within_interval;
   float32_t delta_rr, eps_range, min_val;
   bool solution_found;
   float32_t rr_unamb, conf, conf_r;
   uint8_t valid_nb = 0U;

   const uint16_t num_mov_dets = p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets;
   const Cart_Knn_Output_T *restrict p_knn_i;

   /* Array of neighbours' unambiguous range rate differences */
   float32_t __attribute__((aligned(32))) cart_nb_rr_diff_unamb[8]  = {0.0F};
   float32_t __attribute__((aligned(32))) cart_nb_range_residual[8] = {0.0F};

   for (i = 0U; i < num_mov_dets; i++)
   {
      cur_det_idx   = p_rdu_buffer->present_scan_s2_mov_det.moving_det_idx[i];
      stage2_status = p_rdu_data->p_rdu_stage2_output_data->stage2_rrr_motion_status[cur_det_idx];

      p_knn_i = &p_rdu_internals->cart_knn_output[i];

      rr_amb_cur = p_knn_i->cur_rr;
      range_cur  = p_rdu_internals->cur_range[cur_det_idx];

      delta_rr        = FLT_MAX;
      interval        = 0;
      eps_range       = FLT_MAX;
      within_interval = 0;
      solution_found  = false;
      valid_nb        = 0;

      if (!p_knn_i->f_matched)
      {
         if (stage2_status == RDU_MOTION_STATUS_MOVING_SPECIAL)
         {
            p_out->stage3_rrr_motion_status[cur_det_idx]         = RDU_MOTION_STATUS_AMBIGUOUS;
            p_out->stage3_classification_confidence[cur_det_idx] = 0.5F;
         }
         continue;
      }

      if (stage2_status == RDU_MOTION_STATUS_MOVING)
      {
         p_out->stage3_rrr_motion_status[cur_det_idx] =
            RDU_MOTION_STATUS_MOVING_ASSOCIATED; // MOVING -> ASSOCIATED ; if matched in stage 3
      }
      else if (stage2_status == RDU_MOTION_STATUS_MOVING_SPECIAL)
      {
         p_out->stage3_rrr_motion_status[cur_det_idx] =
            RDU_MOTION_STATUS_MOVING_SPECIAL; // MOVING_SPECIAL -> MOVING_SPECIAL ; even if matched in stage 3
      }

      const uint8_t num_nb = (uint8_t)p_knn_i->num_neighbours;

      for (j = 0U; j < (uint16_t)num_nb; j++)
      {
         prev_mov_idx = p_knn_i->prev_scan_detidx[j];
         if (prev_mov_idx == UINT16_MAX)
         {
            continue;
         }
         prev_det_idx                    = p_prev_mov_buf[prev_mov_idx];
         rr_prev                         = p_knn_i->prev_scan_rr[j];
         cart_nb_rr_diff_unamb[valid_nb] = rr_amb_cur - rr_prev;

         range_prev                       = p_rdu_buffer->prev_scan_det_data.prev_range[prev_det_idx];
         range_diff                       = range_cur - range_prev;
         cart_nb_range_residual[valid_nb] = range_diff - (lp_dt_half * (rr_amb_cur + rr_prev));
         valid_nb++;
      }

      solution_found = dt_delta_valid && Linprog_Disambiguate_Det_Vec(
                                            (int16_t)valid_nb, lp_k2_min, lp_k2_max, lp_k1_min, lp_k1_max, W1, W2, delta_w,
                                            lp_dt_half, &cart_nb_rr_diff_unamb[0], &cart_nb_range_residual[0], lp_max_eps_range,
                                            &interval, &delta_rr, &eps_range, &within_interval, &min_val, eps_threshold);

      if (solution_found)
      {
         rr_unamb = rr_amb_cur + (float32_t)XT_MUL_S((float32_t)interval, W2);
         conf     = 1.0F - (delta_rr / eps_threshold);
         conf     = XT_MAX_S(conf, 0.0F);
         conf_r   = 1.0F - ((float32_t)XT_ABS_S(eps_range) / lp_max_eps_range);
         conf_r   = XT_MAX_S(conf_r, 0.0F);

         p_out->stage3_rdu_unamb_range_rate[cur_det_idx] = rr_unamb;
         p_out->stage3_wrapping_k[cur_det_idx]           = (int8_t)interval;
         p_out->stage3_unfolding_confidence[cur_det_idx] = conf * conf_r;
         p_rdu_internals->stage3_eps_range[cur_det_idx]  = eps_range; /* moved from output tag */
         p_out->stage3_delta_rr[cur_det_idx]             = delta_rr;
         p_out->stage3_within_interval[cur_det_idx]      = within_interval;
         p_rdu_internals->best_obj[cur_det_idx]          = min_val;
      }
   }
}
#endif

/**
 * @brief This function copies present scan moving dets data to previous scan
 * moving det buffer
 *
 *
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return void
 *
 */
void Update_Prev_Scan_Mov_Data(RDU_Buffer_T *p_rdu_buffer)
{
   /* Copy present scan moving detections to previous scan moving detections buffer */
   memcpy((void *)&p_rdu_buffer->prev_scan_s2_mov_det, (void *)&p_rdu_buffer->present_scan_s2_mov_det, sizeof(RDU_S2_Mov_Det_T));
}

/**
 * @brief Two-cycle unfolding process (stage 3) for RDU.
 *
 * Main entry point that performs the two-cycle unfolding stage of the
 * RDU algorithm.
 *
 * @param[in,out] p_rdu_data      Pointer to RDU data structure
 * @param[in] p_rdu_params    Pointer to RDU parameters structure
 * @param[in,out] p_rdu_internals Pointer to RDU internals structure
 * @param[in,out] p_rdu_buffer    Pointer to RDU buffer structure (holds
 *                                present and previous scan data)
 *
 * @return void
 *
 * @ref Matlab function reference: run_algorithm_single() of two-cycle unfolding
 */
void Two_Cycle_Unfolding_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_rdu_internals,
                                 RDU_Buffer_T *p_rdu_buffer)
{
   RDU_Stage1_Output_Data_T *p_stage1_out = p_rdu_data->p_rdu_stage1_output_data;
   RDU_Stage3_Output_Data_T *p_out        = p_rdu_data->p_rdu_stage3_output_data;
   RDU_Stage2_Output_Data_T *p_stage2_out = p_rdu_data->p_rdu_stage2_output_data;

   /* Seed Stage3 from Stage2 before refinement */
   (void)vector_copy_unaligned(&p_out->stage3_rdu_unamb_range_rate[0], &p_rdu_internals->stage1_unamb_range_rate[0],
                               AF_MAX_NUM_DET * sizeof(p_rdu_internals->stage1_unamb_range_rate[0]));
   (void)vector_copy_unaligned(&p_out->stage3_rrr_motion_status[0], &p_stage2_out->stage2_rrr_motion_status[0],
                               AF_MAX_NUM_DET * sizeof(p_stage2_out->stage2_rrr_motion_status[0]));
   (void)vector_copy_unaligned(&p_out->stage3_classification_confidence[0], &p_stage2_out->stage2_classification_confidence[0],
                               AF_MAX_NUM_DET * sizeof(p_stage2_out->stage2_classification_confidence[0]));
   (void)vector_copy_unaligned(&p_out->stage3_wrapping_k[0], &p_stage1_out->stage1_wrapping_k[0],
                               AF_MAX_NUM_DET * sizeof(p_stage1_out->stage1_wrapping_k[0]));

   /* Execute stage 3 with prev stage 2 output in RDU Buffer */
   /* 1) Filter moving detections based on stage 2 classification (VCS coordinates of moving dets to be computed)
    * 2) Make present detections as past dets under following conditions and return from function:
    *    a) no prev valid detections list i.e first scan (if (!p_rdu_buffer->has_valid_prev_scan) )
    *    b) no moving dets in present scan or past scan
    *    c) scan_idx diff converted to time diff  < 1e-3s
    * 3) Call cartesian KNN with present scan moving and prev scan moving dets as input
    * 4) Call lin prog disambiguation function with cartesian KNN output
    * 5) Update stage 3 output to rrr motion status, unamb rr, K factor, confidence in IPC
    * 6) Update RDU buffer of prev scan with present scan detections for next cycle and return from function
    */

   /* clear required IPC fields */
#ifdef BBE_ENABLE
   (void)vector_clear((void *)p_rdu_internals->stage3_eps_range, sizeof(p_rdu_internals->stage3_eps_range));
   (void)vector_clear((void *)p_out->stage3_delta_rr, sizeof(p_out->stage3_delta_rr));
   (void)vector_clear((void *)p_out->stage3_within_interval, sizeof(p_out->stage3_within_interval));
   (void)vector_clear((void *)p_out->stage3_unfolding_confidence, sizeof(p_out->stage3_unfolding_confidence));
#else
   (void)memset((void *)p_rdu_internals->stage3_eps_range, 0, sizeof(p_rdu_internals->stage3_eps_range));
   (void)memset((void *)p_out->stage3_delta_rr, 0, sizeof(p_out->stage3_delta_rr));
   (void)memset((void *)p_out->stage3_within_interval, 0, sizeof(p_out->stage3_within_interval));
   (void)memset((void *)p_out->stage3_unfolding_confidence, 0, sizeof(p_out->stage3_unfolding_confidence));
#endif

#ifdef ENABLE_RDU_TESTING
   xthal_dcache_block_invalidate((void *)p_rdu_params, sizeof(RDU_Params_T));
   xthal_dcache_block_invalidate((void *)p_rdu_internals, sizeof(RDU_Internals_T));
   xthal_dcache_block_invalidate((void *)p_rdu_buffer, sizeof(RDU_Buffer_T));
#endif

   Filter_Moving_Dets(p_rdu_data, p_rdu_params, p_rdu_internals, p_rdu_buffer);

   /*logging stage 3 prev and curr scan and lookid*/
   if (p_rdu_buffer->has_valid_prev_scan)
   {
      p_out->stage3_scan_idx_prev_scan = p_rdu_buffer->prev_scan_s2_mov_det.scan_idx;
      p_out->stage3_look_id_prev_scan  = (uint8_t)p_rdu_buffer->prev_scan_s2_mov_det.look_type;
   }
   else
   {
      /* first/invalid previous scan fallback */
      p_out->stage3_scan_idx_prev_scan = INVALID_SCAN_IDX;
      p_out->stage3_look_id_prev_scan  = INVALID_LOOK_ID;
   }

   p_out->stage3_scan_idx_curr_scan = p_rdu_buffer->present_scan_s2_mov_det.scan_idx;
   p_out->stage3_look_id_curr_scan  = (uint8_t)p_rdu_buffer->present_scan_s2_mov_det.look_type;

   bool skip_stage3      = false;
   int32_t scan_interval = p_rdu_buffer->present_scan_s2_mov_det.scan_idx - p_rdu_buffer->prev_scan_s2_mov_det.scan_idx;
   /* Check for conditions to skip stage 3 */
   bool condition_a = !p_rdu_buffer->has_valid_prev_scan; /* condition a) */
   bool condition_b = (p_rdu_buffer->present_scan_s2_mov_det.num_moving_dets == 0) ||
                      (p_rdu_buffer->prev_scan_s2_mov_det.num_moving_dets == 0);  /* condition b) */
   bool condition_c = (scan_interval * p_rdu_params->radar_cycle_time_s) < 1e-3f; /* condition c) */
   skip_stage3      = condition_a || condition_b || condition_c;

   if (skip_stage3)
   {
      Update_Prev_Scan_Mov_Data(p_rdu_buffer);
   }
   else
   {
      /* Apply velocity compensation to prev scan detections in VCS */
      Apply_Ego_Motion_Compensation_To_Prev_Scan_Dets(p_rdu_data, p_rdu_params, p_rdu_internals, p_rdu_buffer);

      /* Cartesian KNN based Association */
#ifdef BBE_ENABLE
      Cartesian_N_Knn_Opt(p_rdu_data, p_rdu_params, p_rdu_internals, p_rdu_buffer);
#else
      Cartesian_N_Knn(p_rdu_data, p_rdu_params, p_rdu_internals, p_rdu_buffer);
#endif
      /* Disambiguation */
#ifdef BBE_ENABLE
      Stage3_Disambiguation_Det_Vec(p_rdu_data, p_rdu_params, p_rdu_internals, p_rdu_buffer);
#else
      Stage3_Disambiguation(p_rdu_data, p_rdu_params, p_rdu_internals, p_rdu_buffer);
#endif
      Update_Prev_Scan_Mov_Data(p_rdu_buffer);
   }

#ifdef ENABLE_RDU_TESTING
   xthal_dcache_block_writeback((void *)p_rdu_params, sizeof(RDU_Params_T));
   xthal_dcache_block_writeback((void *)p_rdu_internals, sizeof(RDU_Internals_T));
   xthal_dcache_block_writeback((void *)p_rdu_buffer, sizeof(RDU_Buffer_T));
#endif
}

/* END OF FILE -------------------------------------------------------------- */
