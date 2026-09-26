/*===========================================================================*/
/**
 * @file moving_special_cases.c
 * @brief Stage 2: Moving Special Cases Detection – Implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitive Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Implements the Stage 2 Doppler unfolding algorithm that resolves AMBIGUOUS
 * detections produced by Stage 1 (Stationary_Moving_Classifier).  A detection
 * is AMBIGUOUS when it cannot be reliably assigned as STATIONARY or MOVING
 * because it falls inside the stationary Doppler profile.
 *
 * Stage 2 uses a 2-scan detection association approach:
 *  • For each low-confidence MOVING detection in the previous scan, the
 *    algorithm predicts where the target could appear in the current scan for
 *    each of 5 × 5 = 25 (K_mov, K_stat) wrapping-factor combinations.
 *  • Predicted locations are compared against current AMBIGUOUS detections
 *    using analytical intersection or proximity geometry.
 *  • Confirmed matches are validated by an absolute-velocity magnitude check.
 *  • Verified detections are reclassified as MOVING_SPECIAL (3).
 *
 * Reuse from Stage 1
 * ------------------
 *  • RDU_Data_T / RDU_Params_T / RDU_Internals_T       – unchanged structs
 *  • RDU_MOTION_STATUS_* defines                        – unchanged enumerations
 *  • MAX_NUM_DETECTS, RDU_K_MIN/MAX                     – unchanged limits
 *  • p_rdu_output_data->vx_scs / vy_scs                 – Stage 1 SCS velocity
 *  • p_rdu_output_data->rrr_motion_status[]             – updated in-place
 *  • p_rdu_output_data->classification_confidence[]     – read (filter criterion)
 *  • p_rdu_internals->stage1_moving_det_thold[]              – read (T threshold)
 *  • p_rdu_params->vua / perspective_angle              – reused sensor params
 *
 * MATLAB reference: @Moving_Special_Cases/run_algorithm_single.m
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s): Stage2_C_Implementation_Layout.md
 *
 *   - Applicable Standards:
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 *
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

#include "doppler_unfolding.h"
#include "moving_special_cases.h"
#include "radar_math.h"

#ifdef BBE_ENABLE
   #include "api/bbe_helpers.h"
   #include <xtensa/tie/xt_bbe32.h>
   #include <xtensa/tie/xt_bben.h>
   #include <xtensa/tie/xt_bben_scalarfp.h>
#endif
/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/** @brief π as a float32 literal (avoids double-promotion warnings) */
#ifndef M_PI_F
   #define M_PI_F (3.14159265358979323846f)
#endif

/** @brief Degrees-to-radians conversion */
#define RDU_S2_DEG2RAD(deg) ((deg) * (M_PI_F / 180.0f))
#define RDU_S2_VEC_WIDTH    (8U)

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/

typedef struct __attribute__((aligned(32))) RDU_Stage2_Current_Search_Tag
{
   const float32_t *az_curr;
   const float32_t *cos_curr;
   const float32_t *sin_curr;
   const float32_t *r_curr;
   const float32_t *rr_curr;
   const RDU_Internals_T *p_rdu_internals;
   const bool *p_was_ambiguous;
   const RDU_Prev_Scan_Det_T *p_prev_scan_det;
   uint16_t prev_det_idx;
   uint16_t n_curr;
   float32_t dt;
   float32_t r1_v;
   float32_t r2_predicted;
   uint16_t range_first;
   uint16_t range_last;
   float32_t rr1_unwrapped;
   float32_t closest_az;
   float32_t vxs;
   float32_t vys;
   float32_t vua_current;
   float32_t rr_diff_norm_factor;
   int8_t K_stat;
   uint8_t detection_type;
   bool apply_rr_gate;
   const RDU_Params_T *p_rdu_params;
} RDU_Stage2_Current_Search_T;

#ifndef RDU_SIL_ENABLE
static __attribute__((aligned(32))) __attribute__((section(".sram0.bss.rdu")))
RDU_Stage2_Internal_Flags_T RDU_Stage2_Internal_Flags;
#else
static __attribute__((aligned(32))) RDU_Stage2_Internal_Flags_T RDU_Stage2_Internal_Flags;
#endif
/*===========================================================================*
 * Function Definitions
 *===========================================================================*/

#ifdef BBE_ENABLE
static void RDU_Stage2_Init_Current_Search_Batch(RDU_Stage2_Current_Search_T *p_search_batch, uint16_t batch_count,
                                                 const float32_t *az_curr, const float32_t *cos_curr, const float32_t *sin_curr,
                                                 const float32_t *r_curr, const float32_t *rr_curr, const bool *p_was_ambiguous,
                                                 const RDU_Prev_Scan_Det_T *p_prev_scan_det, const uint16_t *p_prev_det_idx_batch,
                                                 uint16_t n_curr, float32_t dt, const float32_t *p_r1_v,
                                                 const float32_t *p_r2_predicted, const uint16_t *p_range_first_batch,
                                                 const uint16_t *p_range_last_batch, float32_t vxs, float32_t vys,
                                                 float32_t vua_current, float32_t rr_diff_norm_factor, int8_t K_stat,
                                                 bool apply_rr_gate, const RDU_Params_T *p_rdu_params,
                                                 const RDU_Internals_T *p_rdu_internals, const float32_t *rr1_unwrapped);
   #if 0
static void RDU_Stage2_Try_Build_Candidate_Batch(const RDU_Stage2_Current_Search_T *p_search, uint16_t base_curr_det_idx,
                                                 uint16_t batch_count, bool *p_is_candidate, float32_t *p_range_error,
                                                 float32_t *p_azimuth_error, float32_t *p_rr_diff, float32_t *p_abs_speed);
static void RDU_Stage2_Check_Absolute_Velocity_Batch(const RDU_Stage2_Current_Search_T *p_search, uint16_t batch_count,
                                                     const float32_t *p_range_buf, const float32_t *p_curr_cos_buf,
                                                     const float32_t *p_curr_sin_buf, const float32_t *p_gate_mask,
                                                     bool *p_is_candidate, float32_t *p_abs_speed);
   #endif
static void RDU_Stage2_Process_Prev_Batch(RDU_Internals_T *p_stage2_internals, const RDU_Prev_Scan_Det_T *p_prev_scan_det,
                                          const uint16_t *p_valid_idx, uint16_t batch_start, uint16_t batch_count,
                                          const float32_t *az_curr, const float32_t *cos_curr, const float32_t *sin_curr,
                                          const float32_t *r_curr, const float32_t *rr_curr, const bool *p_was_ambiguous,
                                          uint16_t n_curr, float32_t dt, float32_t vxs, float32_t vys, float32_t vua_current,
                                          float32_t vua_prev, float32_t rr_diff_norm_factor, bool apply_rr_gate,
                                          const RDU_Params_T *p_rdu_params, const RDU_Internals_T *p_rdu_internals,
                                          float32_t fov_rad, float32_t T_max);
static void RDU_Stage2_Find_Intersection_Batch(const float32_t *p_r1_v, const float32_t *p_r2_predicted,
                                               const float32_t *p_cos_theta1, const float32_t *p_sin_theta1, uint16_t batch_count,
                                               float32_t dt, float32_t vxs, float32_t vys, int8_t K_stat, float32_t vua_current,
                                               float32_t fov_rad, bool *p_has_intersection, float32_t (*p_solutions)[2],
                                               uint8_t *p_num_solutions);
static void RDU_Stage2_Find_Minimum_Distance_Batch(const RDU_Stage2_Current_Search_T *p_search_batch, const bool *p_enable_lane,
                                                   uint16_t batch_count, float32_t fov_rad, bool *p_has_minimum,
                                                   float32_t *p_closest_azimuth, float32_t *p_min_distance);

static void RDU_Stage2_Normalize_Angle_Batch(const float32_t *p_angle_in, uint16_t batch_count, float32_t *p_angle_out);
static void RDU_Stage2_Collect_Search_Matches_Batch(RDU_Internals_T *p_stage2_internals,
                                                    RDU_Stage2_Current_Search_T *p_search_batch, const uint16_t *p_prev_idx_batch,
                                                    const uint16_t *p_prev_det_idx_batch, const float32_t *p_r1_v,
                                                    const float32_t *p_r2_predicted, const float32_t *p_cos_theta1,
                                                    const float32_t *p_sin_theta1, uint16_t batch_count, int8_t K_mov,
                                                    float32_t fov_rad, float32_t T_max, const bool *within_range_limit);
#endif

bool RDU_Stage2_Find_Intersection(float32_t r1, float32_t r2, float32_t dt, float32_t vxs, float32_t vys, int8_t K_stat,
                                  float32_t vua_current, const RDU_Prev_Scan_Det_T *p_prev_scan_det, uint16_t prev_det_idx,
                                  float32_t fov_rad, float32_t *p_solutions, uint8_t *p_num_solutions);

void RDU_Stage2_Compute_Confidence(float32_t *p_confidence_out, const RDU_Stage2_Candidate_T *p_candidates, uint16_t num_matches,
                                   float32_t rr_diff_norm_factor, const RDU_Params_T *p_stage2_params);
#ifndef BBE_ENABLE
static void RDU_Stage2_Process_K_Stat(RDU_Internals_T *p_stage2_internals, const float32_t *az_curr, const float32_t *r_curr,
                                      const float32_t *cos_curr, const float32_t *sin_curr, const float32_t *rr_curr,
                                      const bool *p_was_ambiguous, const RDU_Prev_Scan_Det_T *p_prev_scan_det,
                                      uint16_t prev_det_idx, uint16_t n_curr, float32_t dt, float32_t r1_v, float32_t r2_predicted,
                                      float32_t vxs, float32_t vys, float32_t vua_current, float32_t rr_diff_norm_factor,
                                      bool apply_rr_gate, const RDU_Params_T *p_rdu_params, int8_t K_mov, int8_t K_stat,
                                      uint16_t prev_idx, float32_t fov_rad, float32_t T_max, float32_t rr1_unwrapped);
#endif

static bool RDU_Stage2_Try_Build_Candidate(const RDU_Stage2_Current_Search_T *p_search, uint16_t curr_det_idx,
                                           RDU_Stage2_Candidate_T *p_candidate);

static uint16_t RDU_Stage2_Build_Sorted_Ambiguous(RDU_Internals_T *p_stage2_internals, uint16_t n_curr);
static bool RDU_Stage2_Find_Range_Window_Binary_Search(const float32_t *p_sorted_range, uint16_t count, float32_t predicted_range,
                                                       float32_t range_threshold, uint16_t *p_first, uint16_t *p_last);

static uint16_t RDU_Stage2_Mark_Ambiguous_Detections(const int8_t *ms_curr, const float32_t *moving_det_thold, uint16_t n_curr,
                                                     float32_t *p_t_max)
{
#ifdef BBE_ENABLE
   uint16_t i;
   uint16_t num_ambiguous = 0U;
   uint16_t vec_width     = XCHAL_BBEN_SIMD_WIDTH;
   uint16_t num_full_vec  = n_curr / vec_width;
   // uint16_t rem           = n_curr % vec_width;
   uint16_t __attribute__((aligned(32))) idx_values[XCHAL_BBEN_SIMD_WIDTH] = {0U};
   uint16_t set_flag_count;
   float32_t t_max = 0.0f;

   xb_vecNx16 *p_rrr_status       = (xb_vecNx16 *)ms_curr;
   xb_vecNx16 *p_was_ambiguous    = (xb_vecNx16 *)&RDU_Stage2_Internal_Flags.was_ambiguous[0];
   xb_vecN_2xf32 *p_mov_det_thold = (xb_vecN_2xf32 *)moving_det_thold;
   xb_vecNx16 v_status_ambiguous  = (xb_vecNx16)(RDU_MOTION_STATUS_AMBIGUOUS);
   xb_vecN_2xf32 v_mov_det_thold_lo;
   xb_vecN_2xf32 v_mov_det_thold_hi;
   xb_vecNx16 v_rrr_status;
   xb_vecNx16 vec_rrr_status_16bit_new;
   xb_vecNx16 vec_lo_8bit_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
   xb_vecNx16 vec_const        = BBE_MOVVINT16(1);
   xb_vecNx16 v_temp_new1;
   xb_vecNx16 v_temp_new2;
   vboolN v_ambiguous_flag;
   vboolN_2 v_amb_flag_lo;
   vboolN_2 v_amb_flag_hi;
   vboolN_2 v_unused;
   vboolN_2 v_temp_mask;
   valign was_amb_align;
   float32_t temp_t_max = 0.0f;

   for (i = 0U; i < num_full_vec; i++)
   {
      BBE_LAVNX16_XP(v_rrr_status, BBE_LANX16_PP(p_rrr_status), p_rrr_status, 16U);
      vec_rrr_status_16bit_new = BBE_SELNX16I((v_rrr_status >> 8U), v_rrr_status, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask;
      v_ambiguous_flag         = BBE_EQNX16(vec_rrr_status_16bit_new, v_status_ambiguous);
      BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, v_ambiguous_flag);

      vec_rrr_status_16bit_new = BBE_MOVNX16T(vec_const, BBE_ZERONX16(), v_ambiguous_flag);
      v_temp_new1              = BBE_SELNX16I(vec_rrr_status_16bit_new, vec_rrr_status_16bit_new, BBE_SELI_EXTRACT_1_OF_2_OFF_0);
      v_temp_new2              = BBE_SELNX16I(vec_rrr_status_16bit_new, vec_rrr_status_16bit_new, BBE_SELI_EXTRACT_1_OF_2_OFF_1);
      vec_rrr_status_16bit_new = (v_temp_new2 << 8U) | v_temp_new1;
      was_amb_align            = BBE_ZALIGN();
      BBE_SAVRNX16_XP(vec_rrr_status_16bit_new, was_amb_align, p_was_ambiguous, 16U);
      BBE_SANX16POS_FP(was_amb_align, p_was_ambiguous);

      BBE_EXTRACTBN(v_unused, v_amb_flag_lo, v_ambiguous_flag);
      BBE_LVN_2XF32_IP(v_mov_det_thold_lo, p_mov_det_thold, 32U);
      BBE_RBMAXNUMN_2XF32T(v_temp_mask, temp_t_max, v_mov_det_thold_lo, v_amb_flag_lo);
      t_max = XT_MAX_S(temp_t_max, t_max);

      BBE_EXTRACTBN(v_amb_flag_hi, v_unused, v_ambiguous_flag);
      BBE_LVN_2XF32_IP(v_mov_det_thold_hi, p_mov_det_thold, 32U);
      BBE_RBMAXNUMN_2XF32T(v_temp_mask, temp_t_max, v_mov_det_thold_hi, v_amb_flag_hi);
      t_max = XT_MAX_S(temp_t_max, t_max);

      num_ambiguous = (uint16_t)(num_ambiguous + (set_flag_count >> 1U));
   }

   for (i = (uint16_t)(num_full_vec * vec_width); i < n_curr; i++)
   {
      bool is_ambiguous                          = (ms_curr[i] == (int8_t)RDU_MOTION_STATUS_AMBIGUOUS);
      RDU_Stage2_Internal_Flags.was_ambiguous[i] = is_ambiguous;

      if (is_ambiguous)
      {
         float32_t t_i = moving_det_thold[i];
         num_ambiguous++;
         if (t_i > t_max)
         {
            t_max = t_i;
         }
      }
   }
#else
   uint16_t i;
   uint16_t num_ambiguous = 0U;
   float32_t t_max        = 0.0f;

   for (i = 0U; i < n_curr; i++)
   {
      bool is_ambiguous                          = (ms_curr[i] == (int8_t)RDU_MOTION_STATUS_AMBIGUOUS);
      RDU_Stage2_Internal_Flags.was_ambiguous[i] = is_ambiguous;

      if (is_ambiguous)
      {
         float32_t t_i = moving_det_thold[i];
         num_ambiguous++;
         if (t_i > t_max)
         {
            t_max = t_i;
         }
      }
   }
#endif

   *p_t_max = t_max;

   return num_ambiguous;
}

static uint16_t RDU_Stage2_Build_Sorted_Ambiguous(RDU_Internals_T *p_stage2_internals, uint16_t n_curr)
{
   uint16_t source_idx;
   uint16_t sorted_count = 0U;

   for (source_idx = 0U; (source_idx < n_curr) && (sorted_count < MAX_NUM_DETECTS); source_idx++)
   {
      uint16_t insert_idx;

      if (!RDU_Stage2_Internal_Flags.was_ambiguous[source_idx])
      {
         continue;
      }

      insert_idx = sorted_count;
      while ((insert_idx > 0U) &&
             (RDU_Stage2_Internal_Flags.stage2_sorted_range[insert_idx - 1U] > p_stage2_internals->cur_range[source_idx]))
      {
         RDU_Stage2_Internal_Flags.stage2_sorted_range[insert_idx] =
            RDU_Stage2_Internal_Flags.stage2_sorted_range[insert_idx - 1U];
         RDU_Stage2_Internal_Flags.stage2_sorted_vel[insert_idx] = RDU_Stage2_Internal_Flags.stage2_sorted_vel[insert_idx - 1U];
         RDU_Stage2_Internal_Flags.stage2_sorted_theta[insert_idx] =
            RDU_Stage2_Internal_Flags.stage2_sorted_theta[insert_idx - 1U];
         RDU_Stage2_Internal_Flags.stage2_sorted_cos_theta[insert_idx] =
            RDU_Stage2_Internal_Flags.stage2_sorted_cos_theta[insert_idx - 1U];
         RDU_Stage2_Internal_Flags.stage2_sorted_sin_theta[insert_idx] =
            RDU_Stage2_Internal_Flags.stage2_sorted_sin_theta[insert_idx - 1U];
         RDU_Stage2_Internal_Flags.stage2_sorted_was_ambiguous[insert_idx] =
            RDU_Stage2_Internal_Flags.stage2_sorted_was_ambiguous[insert_idx - 1U];
         RDU_Stage2_Internal_Flags.stage2_sorted_original_idx[insert_idx] =
            RDU_Stage2_Internal_Flags.stage2_sorted_original_idx[insert_idx - 1U];
         insert_idx--;
      }

      RDU_Stage2_Internal_Flags.stage2_sorted_range[insert_idx]         = p_stage2_internals->cur_range[source_idx];
      RDU_Stage2_Internal_Flags.stage2_sorted_vel[insert_idx]           = p_stage2_internals->cur_vel[source_idx];
      RDU_Stage2_Internal_Flags.stage2_sorted_theta[insert_idx]         = p_stage2_internals->cur_theta[source_idx];
      RDU_Stage2_Internal_Flags.stage2_sorted_cos_theta[insert_idx]     = p_stage2_internals->cos_theta[source_idx];
      RDU_Stage2_Internal_Flags.stage2_sorted_sin_theta[insert_idx]     = p_stage2_internals->sin_theta[source_idx];
      RDU_Stage2_Internal_Flags.stage2_sorted_was_ambiguous[insert_idx] = true;
      RDU_Stage2_Internal_Flags.stage2_sorted_original_idx[insert_idx]  = source_idx;
      sorted_count++;
   }

   RDU_Stage2_Internal_Flags.stage2_num_sorted_ambiguous = sorted_count;
   return sorted_count;
}

static bool RDU_Stage2_Find_Range_Window_Binary_Search(const float32_t *p_sorted_range, uint16_t count, float32_t predicted_range,
                                                       float32_t range_threshold, uint16_t *p_first, uint16_t *p_last)
{
   float32_t lower;
   float32_t upper;

   uint16_t lo;
   uint16_t hi;
   uint16_t mid;

   if ((p_sorted_range == NULL) || (p_first == NULL) || (p_last == NULL) || (count == 0U))
   {
      return false;
   }

   lower = predicted_range - range_threshold;
   upper = predicted_range + range_threshold;

   /*------------------------------------------------------------
    * Find first element >= lower (lower_bound)
    *-----------------------------------------------------------*/
   lo = 0U;
   hi = count;

   while (lo < hi)
   {
      mid = (uint16_t)(lo + ((hi - lo) >> 1U));

      if (p_sorted_range[mid] < lower)
      {
         lo = (uint16_t)(mid + 1U);
      }
      else
      {
         hi = mid;
      }
   }

   if (lo >= count)
   {
      return false;
   }

   *p_first = lo;

   /*------------------------------------------------------------
    * Find first element > upper (upper_bound)
    *-----------------------------------------------------------*/
   lo = 0U;
   hi = count;

   while (lo < hi)
   {
      mid = (uint16_t)(lo + ((hi - lo) >> 1U));

      if (p_sorted_range[mid] <= upper)
      {
         lo = (uint16_t)(mid + 1U);
      }
      else
      {
         hi = mid;
      }
   }

   if (lo == 0U)
   {
      return false;
   }

   *p_last = (uint16_t)(lo - 1U);

   return (*p_first <= *p_last);
}
static uint16_t RDU_Stage2_Collect_Valid_Previous_Detections(uint16_t *p_valid_idx, uint16_t max_valid, int32_t n_prev,
                                                             const RDU_Buffer_T *p_rdu_buffer, float32_t conf_thresh)
{
#ifdef BBE_ENABLE
   uint16_t i;
   uint16_t num_valid                                                      = 0U;
   uint16_t n_prev_u                                                       = (n_prev > 0) ? (uint16_t)n_prev : 0U;
   uint16_t vec_width                                                      = XCHAL_BBEN_SIMD_WIDTH;
   uint16_t num_full_vec                                                   = n_prev_u / vec_width;
   uint16_t __attribute__((aligned(32))) idx_values[XCHAL_BBEN_SIMD_WIDTH] = {0U};
   uint16_t set_flag_count;

   xb_vecNx16 *p_rrr_status    = (xb_vecNx16 *)&p_rdu_buffer->prev_scan_s1_output.rrr_motion_status[0];
   xb_vecN_2xf32 *p_confidence = (xb_vecN_2xf32 *)&p_rdu_buffer->prev_scan_s1_output.confidence[0];
   xb_vecNx16 v_status_moving  = (xb_vecNx16)(RDU_MOTION_STATUS_MOVING);
   xb_vecN_2xf32 v_conf_thresh = (xb_vecN_2xf32)conf_thresh;
   xb_vecNx16 v_rrr_status;
   xb_vecNx16 vec_rrr_status_16bit_new;
   xb_vecNx16 vec_lo_8bit_mask = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
   xb_vecN_2xf32 v_confidence_lo;
   xb_vecN_2xf32 v_confidence_hi;
   vboolN v_moving_flag;
   vboolN v_conf_gating;
   vboolN_2 v_conf_lo;
   vboolN_2 v_conf_hi;

   for (i = 0U; (i < num_full_vec) && (num_valid < max_valid); i++)
   {
      uint16_t selected_count;
      uint16_t selected_idx;
      uint16_t base_index = (uint16_t)(i * vec_width);

      BBE_LAVNX16_XP(v_rrr_status, BBE_LANX16_PP(p_rrr_status), p_rrr_status, 16U);
      vec_rrr_status_16bit_new = BBE_SELNX16I((v_rrr_status >> 8U), v_rrr_status, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask;
      v_moving_flag            = BBE_EQNX16(vec_rrr_status_16bit_new, v_status_moving);

      BBE_LAN_2XF32_IP(v_confidence_lo, BBE_LAN_2XF32_PP(p_confidence), p_confidence);
      v_conf_lo = BBE_OLTN_2XF32(v_confidence_lo, v_conf_thresh);
      BBE_LAN_2XF32_IP(v_confidence_hi, BBE_LAN_2XF32_PP(p_confidence), p_confidence);
      v_conf_hi     = BBE_OLTN_2XF32(v_confidence_hi, v_conf_thresh);
      v_conf_gating = BBE_ANDBN(BBE_JOINBN_2(v_conf_hi, v_conf_lo), v_moving_flag);

      BBE_SQZN(((vsaN *)idx_values)[0], set_flag_count, v_conf_gating);
      selected_count = (uint16_t)(set_flag_count >> 1U);

      for (selected_idx = 0U; (selected_idx < selected_count) && (num_valid < max_valid); selected_idx++)
      {
         p_valid_idx[num_valid] = (uint16_t)(base_index + idx_values[selected_idx]);
         num_valid++;
      }
   }

   for (i = (uint16_t)(num_full_vec * vec_width); (i < n_prev_u) && (num_valid < max_valid); i++)
   {
      bool is_moving         = (p_rdu_buffer->prev_scan_s1_output.rrr_motion_status[i] == (int8_t)RDU_MOTION_STATUS_MOVING);
      bool is_low_confidence = (p_rdu_buffer->prev_scan_s1_output.confidence[i] < conf_thresh);

      if (is_moving && is_low_confidence)
      {
         p_valid_idx[num_valid] = i;
         num_valid++;
      }
   }

   return num_valid;
#else
   uint16_t i;
   uint16_t num_valid = 0U;
   uint16_t n_prev_u  = (n_prev > 0) ? (uint16_t)n_prev : 0U;

   for (i = 0U; (i < n_prev_u) && (num_valid < max_valid); i++)
   {
      bool is_moving         = (p_rdu_buffer->prev_scan_s1_output.rrr_motion_status[i] == (int8_t)RDU_MOTION_STATUS_MOVING);
      bool is_low_confidence = (p_rdu_buffer->prev_scan_s1_output.confidence[i] < conf_thresh);

      if (is_moving && is_low_confidence)
      {
         p_valid_idx[num_valid] = i;
         num_valid++;
      }
   }

   return num_valid;
#endif
}

static void RDU_Stage2_Recompute_Worst_Top_Candidate(const RDU_Stage2_Candidate_T *p_top_candidates, uint8_t top_count,
                                                     float32_t *p_worst_score, uint8_t *p_worst_pos)
{
   uint8_t i;

   *p_worst_pos   = 0U;
   *p_worst_score = p_top_candidates[0].quality_score;

   for (i = 1U; i < top_count; i++)
   {
      if (p_top_candidates[i].quality_score > *p_worst_score)
      {
         *p_worst_score = p_top_candidates[i].quality_score;
         *p_worst_pos   = i;
      }
   }
}

static bool RDU_Stage2_Try_Build_Candidate(const RDU_Stage2_Current_Search_T *p_search, uint16_t curr_det_idx,
                                           RDU_Stage2_Candidate_T *p_candidate)
{
   uint16_t original_curr_det_idx = RDU_Stage2_Internal_Flags.stage2_sorted_original_idx[curr_det_idx];
   bool is_candidate              = p_search->p_was_ambiguous[curr_det_idx];
   float32_t range_error          = 0.0f;
   float32_t azimuth_error        = 0.0f;
   float32_t rr_diff              = 0.0f;
   float32_t abs_speed            = 0.0f;

   if (is_candidate)
   {
      range_error   = rdu_absf(p_search->r_curr[curr_det_idx] - p_search->r2_predicted);
      azimuth_error = rdu_absf(p_search->az_curr[curr_det_idx] - p_search->closest_az);
      is_candidate  = (azimuth_error < p_search->p_rdu_params->angle_prediction_threshold_rad) &&
                     (range_error < p_search->p_rdu_params->range_prediction_threshold);
   }

   if (is_candidate)
   {
      /* Match the BBE path: unwrap the current range rate and compare it with
       * the previous unwrapped range rate for this search candidate. */
      float32_t rr_current_unwrapped = p_search->rr_curr[curr_det_idx] + (float32_t)p_search->K_stat * p_search->vua_current;
      rr_diff                        = rdu_absf(rr_current_unwrapped - p_search->rr1_unwrapped);
      is_candidate                   = (!p_search->apply_rr_gate) || (rr_diff < p_search->rr_diff_norm_factor);
   }

   if (is_candidate)
   {
      is_candidate = RDU_Stage2_Check_Absolute_Velocity(
         p_search->r1_v, p_search->r_curr[curr_det_idx], p_search->dt, p_search->vxs, p_search->vys,
         p_search->p_rdu_params->max_realistic_velocity, p_search->p_rdu_internals, p_search->p_prev_scan_det,
         p_search->prev_det_idx, original_curr_det_idx, &abs_speed);
   }

   if (is_candidate)
   {
      p_candidate->curr_det_idx   = original_curr_det_idx;
      p_candidate->detection_type = p_search->detection_type;
      p_candidate->quality_score =
         RDU_Stage2_Calculate_Match_Quality_Score(p_search->detection_type, range_error, azimuth_error,
                                                  p_search->r_curr[curr_det_idx], abs_speed, rr_diff, p_search->p_rdu_params);
      p_candidate->abs_speed     = abs_speed;
      p_candidate->range_error   = range_error;
      p_candidate->azimuth_error = azimuth_error;
      p_candidate->rr_diff       = rr_diff;
   }

   return is_candidate;
}

/* These below functions are not being used yet keeping it for future reference */
#if 0
static void RDU_Stage2_Try_Build_Candidate_Batch(const RDU_Stage2_Current_Search_T *p_search, uint16_t base_curr_det_idx,
                                                 uint16_t batch_count, bool *p_is_candidate, float32_t *p_range_error,
                                                 float32_t *p_azimuth_error, float32_t *p_rr_diff, float32_t *p_abs_speed)
{
   float32_t __attribute__((aligned(32))) mask_buf[RDU_S2_VEC_WIDTH] = {0.0f};
   uint32_t ambiguous_mask_arr[RDU_S2_VEC_WIDTH]                     = {0U};
   float32_t prev_cos_az;
   float32_t prev_sin_az;
   xb_vecN_2xf32 v_one;
   xb_vecN_2xf32 v_zero;
   xb_vecN_2xf32 v_az;
   xb_vecN_2xf32 v_range;
   xb_vecN_2xf32 v_rr;
   xb_vecN_2xf32 v_curr_cos;
   xb_vecN_2xf32 v_curr_sin;

   xb_vecN_2xf32 v_range_error;
   xb_vecN_2xf32 v_azimuth_error;
   xb_vecN_2xf32 v_rr_diff;
   vboolN_2 stage1_mask;
   vboolN_2 stage2_mask;
   uint16_t lane;

   for (lane = 0U; lane < batch_count; lane++)
   {
      uint16_t curr_det_idx    = (uint16_t)(base_curr_det_idx + lane);
      ambiguous_mask_arr[lane] = p_search->p_was_ambiguous[curr_det_idx] ? 0xFU : 0x0U;
      p_is_candidate[lane]     = false;
      p_range_error[lane]      = 0.0f;
      p_azimuth_error[lane]    = 0.0f;
      p_rr_diff[lane]          = 0.0f;
      p_abs_speed[lane]        = 0.0f;
   }

   prev_cos_az = p_search->p_prev_scan_det->prev_cos_az[p_search->prev_det_idx];
   prev_sin_az = p_search->p_prev_scan_det->prev_sin_az[p_search->prev_det_idx];

   v_az            = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_search->az_curr + base_curr_det_idx), 0);
   v_range         = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_search->r_curr + base_curr_det_idx), 0);
   v_rr            = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_search->rr_curr + base_curr_det_idx), 0);
   v_curr_cos      = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_search->cos_curr + base_curr_det_idx), 0);
   v_curr_sin      = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)(p_search->sin_curr + base_curr_det_idx), 0);
   v_zero          = BBE_ZERON_2XF32();
   v_one           = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(1.0f), 0);
   v_range_error   = BBE_ABSN_2XF32(BBE_SUBN_2XF32(v_range, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(p_search->r2_predicted), 0)));
   v_azimuth_error = BBE_ABSN_2XF32(BBE_SUBN_2XF32(v_az, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(p_search->closest_az), 0)));
   stage1_mask =
      ((*((xb_vecN_2x32Uv *)ambiguous_mask_arr) != BBE_ZERON_2X32U()) &
       BBE_OLEN_2XF32(v_azimuth_error,
                      BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(p_search->p_rdu_params->angle_prediction_threshold_rad), 0)) &
       BBE_OLEN_2XF32(v_range_error,
                      BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(p_search->p_rdu_params->range_prediction_threshold), 0)));
   /* Less then equal to error for az and range as per spec */
   BBE_SVN_2XF32_I(v_range_error, (xb_vecN_2xf32 *)p_range_error, 0);
   BBE_SVN_2XF32_I(v_azimuth_error, (xb_vecN_2xf32 *)p_azimuth_error, 0);

   // rr_diff_2d    = abs(rr_win(:)' + Kstat_vwrap_cands - rr_cands);  condition is rr_diff_2d    <  rr_diff_norm_factor;
   // rr_win is current range rate, rr_cands is predicted rr from prev idx loop
   // % |rr_meas_unwrapped ( # candidates x # current dets) - rr_prev_unwrapped (# candidates x 1)|, per (candidate, detection)
   // pair [m/s]

   BBE_MULAN_2XF32(v_rr, (xb_vecN_2xf32)((float32_t)p_search->K_stat), (xb_vecN_2xf32)p_search->vua_current);

   v_rr_diff = BBE_ABSN_2XF32(BBE_SUBN_2XF32(v_rr, (xb_vecN_2xf32)p_search->rr1_unwrapped));
   BBE_SVN_2XF32_I(v_rr_diff, (xb_vecN_2xf32 *)p_rr_diff, 0);

   stage2_mask = stage1_mask;
   if (p_search->apply_rr_gate)
   {
      stage2_mask =
         stage1_mask & BBE_OLTN_2XF32(v_rr_diff, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(p_search->rr_diff_norm_factor), 0));
   }

   BBE_SVN_2XF32_I(BBE_MOVN_2XF32T(v_one, v_zero, stage2_mask), (xb_vecN_2xf32 *)mask_buf, 0);

   RDU_Stage2_Check_Absolute_Velocity_Batch(p_search, batch_count, &p_search->r_curr[base_curr_det_idx],
                                            &p_search->cos_curr[base_curr_det_idx], &p_search->sin_curr[base_curr_det_idx],
                                            mask_buf, p_is_candidate, p_abs_speed);
}

static void RDU_Stage2_Check_Absolute_Velocity_Batch(const RDU_Stage2_Current_Search_T *p_search, uint16_t batch_count,
                                                     const float32_t *p_range_buf, const float32_t *p_curr_cos_buf,
                                                     const float32_t *p_curr_sin_buf, const float32_t *p_gate_mask,
                                                     bool *p_is_candidate, float32_t *p_abs_speed)
{
   float32_t prev_cos_az = p_search->p_prev_scan_det->prev_cos_az[p_search->prev_det_idx];
   float32_t prev_sin_az = p_search->p_prev_scan_det->prev_sin_az[p_search->prev_det_idx];
   float32_t x1          = p_search->r1_v * prev_cos_az;
   float32_t y1          = p_search->r1_v * prev_sin_az;
   xb_vecN_2xf32 v_range;
   xb_vecN_2xf32 v_curr_cos;
   xb_vecN_2xf32 v_curr_sin;
   xb_vecN_2xf32 v_x2;
   xb_vecN_2xf32 v_y2;
   xb_vecN_2xf32 v_vx_rel;
   xb_vecN_2xf32 v_vy_rel;
   xb_vecN_2xf32 v_vx_abs;
   xb_vecN_2xf32 v_vy_abs;
   xb_vecN_2xf32 v_abs_speed;
   uint16_t lane;

   v_range    = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)p_range_buf, 0);
   v_curr_cos = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)p_curr_cos_buf, 0);
   v_curr_sin = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)p_curr_sin_buf, 0);

   v_x2        = BBE_MULN_2XF32(v_range, v_curr_cos);
   v_y2        = BBE_MULN_2XF32(v_range, v_curr_sin);
   v_vx_rel    = BBE_MULN_2XF32(BBE_SUBN_2XF32(v_x2, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(x1), 0)),
                                BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(1.0f / p_search->dt), 0));
   v_vy_rel    = BBE_MULN_2XF32(BBE_SUBN_2XF32(v_y2, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(y1), 0)),
                                BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(1.0f / p_search->dt), 0));
   v_vx_abs    = BBE_ADDN_2XF32(v_vx_rel, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(p_search->vxs), 0));
   v_vy_abs    = BBE_ADDN_2XF32(v_vy_rel, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(p_search->vys), 0));
   v_abs_speed = BBE_SQRTN_2XF32(BBE_ADDN_2XF32(BBE_MULN_2XF32(v_vx_abs, v_vx_abs), BBE_MULN_2XF32(v_vy_abs, v_vy_abs)));
   BBE_SVN_2XF32_I(v_abs_speed, (xb_vecN_2xf32 *)p_abs_speed, 0);

   for (lane = 0U; lane < batch_count; lane++)
   {
      p_is_candidate[lane] = ((p_gate_mask[lane] > 0.5f) && (p_abs_speed[lane] <= p_search->p_rdu_params->max_realistic_velocity));
   }
}

#endif
static void RDU_Stage2_Insert_Top_Candidate(RDU_Stage2_Candidate_T *p_top_candidates, uint8_t top_cap, uint8_t *p_top_count,
                                            float32_t *p_worst_score, uint8_t *p_worst_pos,
                                            const RDU_Stage2_Candidate_T *p_candidate)
{
   if (top_cap != 0U)
   {
      if (*p_top_count < top_cap)
      {
         p_top_candidates[*p_top_count] = *p_candidate;
         (*p_top_count)++;
         RDU_Stage2_Recompute_Worst_Top_Candidate(p_top_candidates, *p_top_count, p_worst_score, p_worst_pos);
      }
      else if (p_candidate->quality_score < *p_worst_score)
      {
         p_top_candidates[*p_worst_pos] = *p_candidate;
         RDU_Stage2_Recompute_Worst_Top_Candidate(p_top_candidates, *p_top_count, p_worst_score, p_worst_pos);
      }
   }
}

static void RDU_Stage2_Append_Top_Candidates(RDU_Internals_T *p_stage2_internals, const RDU_Stage2_Candidate_T *p_top_candidates,
                                             uint8_t top_count, uint16_t prev_local_idx, uint16_t prev_det_idx, int8_t K_mov)
{
   uint8_t i;

   for (i = 0U; i < top_count; i++)
   {
      if (p_stage2_internals->num_candidates < (uint16_t)RDU_S2_MAX_POTENTIAL_MATCHES)
      {
         RDU_Stage2_Candidate_T *cand = &p_stage2_internals->candidates[p_stage2_internals->num_candidates];
         *cand                        = p_top_candidates[i];
         cand->prev_local_idx         = prev_local_idx;
         cand->prev_det_idx           = prev_det_idx;
         cand->K_moving_value         = K_mov;
         cand->is_valid               = 1U;
         p_stage2_internals->num_candidates++;
      }
   }
}

static void RDU_Stage2_Collect_Top_Matches(RDU_Internals_T *p_stage2_internals, const RDU_Stage2_Current_Search_T *p_search,
                                           uint16_t prev_local_idx, uint16_t prev_det_idx, int8_t K_mov)
{
   uint8_t top_count     = 0U;
   uint8_t top_cap       = (p_search->p_rdu_params->max_close_detections < 8U) ? p_search->p_rdu_params->max_close_detections : 8U;
   uint8_t worst_pos     = 0U;
   float32_t worst_score = 0.0f;
   RDU_Stage2_Candidate_T top_candidates[8U];

   if (0U == top_cap)
   {
      return;
   }

#if 0
   uint16_t range_first = p_search->range_first;
   uint16_t range_last = p_search->range_last;
   uint16_t base_curr_det_idx;

   base_curr_det_idx = (uint16_t)(range_first - (range_first % RDU_S2_VEC_WIDTH));

   for (; base_curr_det_idx <= range_last; base_curr_det_idx = (uint16_t)(base_curr_det_idx + RDU_S2_VEC_WIDTH))
   {
      uint16_t lane;
      uint16_t batch_count =
         (uint16_t)(((base_curr_det_idx + RDU_S2_VEC_WIDTH) <= p_search->n_curr) ? RDU_S2_VEC_WIDTH :
                                                                                   (p_search->n_curr - base_curr_det_idx));
      bool candidate_mask[RDU_S2_VEC_WIDTH]                                  = {false};
      float32_t __attribute__((aligned(32))) range_error[RDU_S2_VEC_WIDTH]   = {0.0f};
      float32_t __attribute__((aligned(32))) azimuth_error[RDU_S2_VEC_WIDTH] = {0.0f};
      float32_t __attribute__((aligned(32))) rr_diff[RDU_S2_VEC_WIDTH]       = {0.0f};
      float32_t __attribute__((aligned(32))) abs_speed[RDU_S2_VEC_WIDTH]     = {0.0f};

      RDU_Stage2_Try_Build_Candidate_Batch(p_search, base_curr_det_idx, batch_count, candidate_mask, range_error, azimuth_error,
                                           rr_diff, abs_speed);

      for (lane = 0U; lane < batch_count; lane++)
      {
         uint16_t curr_det_idx = (uint16_t)(base_curr_det_idx + lane);

         /* The first vector may contain entries before range_first and the
          * last vector may contain entries after range_last.  They are valid
          * vector lanes but not part of this prediction window. */
         if (candidate_mask[lane] && (curr_det_idx >= range_first) && (curr_det_idx <= range_last))
         {
            RDU_Stage2_Candidate_T candidate;

            candidate.curr_det_idx   = p_search->p_rdu_internals->stage2_sorted_original_idx[curr_det_idx];
            candidate.detection_type = p_search->detection_type;
            candidate.quality_score  = RDU_Stage2_Calculate_Match_Quality_Score(
                p_search->detection_type, range_error[lane], azimuth_error[lane], p_search->r_curr[curr_det_idx], abs_speed[lane],
                rr_diff[lane], p_search->p_rdu_params);
            candidate.abs_speed     = abs_speed[lane];
            candidate.range_error   = range_error[lane];
            candidate.azimuth_error = azimuth_error[lane];
            candidate.rr_diff       = rr_diff[lane];

            RDU_Stage2_Insert_Top_Candidate(top_candidates, top_cap, &top_count, &worst_score, &worst_pos, &candidate);
         }
      }
   }
#endif
   uint16_t j;

   /* range_first/range_last were already computed by the caller before solution
    * finding started -- reuse them instead of re-running the binary search. */
   for (j = p_search->range_first; j <= p_search->range_last; j++)
   {
      RDU_Stage2_Candidate_T candidate;
      if (RDU_Stage2_Try_Build_Candidate(p_search, j, &candidate))
      {
         RDU_Stage2_Insert_Top_Candidate(top_candidates, top_cap, &top_count, &worst_score, &worst_pos, &candidate);
      }
   }

   RDU_Stage2_Append_Top_Candidates(p_stage2_internals, top_candidates, top_count, prev_local_idx, prev_det_idx, K_mov);
}

static bool RDU_Stage2_Is_K_Stat_Allowed(const RDU_Params_T *p_rdu_params, int8_t K_mov, int8_t K_stat)
{
   bool is_allowed = true;

   if (p_rdu_params->use_within_interval_constraint)
   {
      int8_t diff = (int8_t)(K_stat - K_mov);
      if (diff < (int8_t)0)
      {
         diff = (int8_t)(-diff);
      }
      is_allowed = (diff <= (int8_t)p_rdu_params->max_within_interval);
   }

   return is_allowed;
}

static void RDU_Stage2_Init_Current_Search(
   RDU_Stage2_Current_Search_T *p_search, const float32_t *az_curr, const float32_t *cos_curr, const float32_t *sin_curr,
   const float32_t *r_curr, const float32_t *rr_curr, const bool *p_was_ambiguous, const RDU_Prev_Scan_Det_T *p_prev_scan_det,
   uint16_t prev_det_idx, uint16_t n_curr, float32_t dt, float32_t r1_v, float32_t r2_predicted, uint16_t range_first,
   uint16_t range_last, float32_t vxs, float32_t vys, float32_t vua_current, float32_t rr_diff_norm_factor, int8_t K_stat,
   bool apply_rr_gate, const RDU_Params_T *p_rdu_params, const RDU_Internals_T *p_rdu_internals, const float32_t rr1_unwrapped)
{
   p_search->az_curr             = az_curr;
   p_search->cos_curr            = cos_curr;
   p_search->sin_curr            = sin_curr;
   p_search->r_curr              = r_curr;
   p_search->p_rdu_internals     = p_rdu_internals;
   p_search->rr_curr             = rr_curr;
   p_search->p_was_ambiguous     = p_was_ambiguous;
   p_search->p_prev_scan_det     = p_prev_scan_det;
   p_search->prev_det_idx        = prev_det_idx;
   p_search->n_curr              = n_curr;
   p_search->dt                  = dt;
   p_search->r1_v                = r1_v;
   p_search->r2_predicted        = r2_predicted;
   p_search->range_first         = range_first;
   p_search->range_last          = range_last;
   p_search->rr1_unwrapped       = rr1_unwrapped;
   p_search->closest_az          = 0.0f;
   p_search->vxs                 = vxs;
   p_search->vys                 = vys;
   p_search->vua_current         = vua_current;
   p_search->rr_diff_norm_factor = rr_diff_norm_factor;
   p_search->K_stat              = K_stat;
   p_search->detection_type      = 0U;
   p_search->apply_rr_gate       = apply_rr_gate;
   p_search->p_rdu_params        = p_rdu_params;
}
#ifndef BBE_ENABLE
static void RDU_Stage2_Collect_Search_Matches(RDU_Internals_T *p_stage2_internals, RDU_Stage2_Current_Search_T *p_search,
                                              uint16_t prev_idx, int8_t K_mov, float32_t fov_rad, float32_t T_max)
{
   float32_t closest_az = 0.0f;
   float32_t min_dist   = 0.0f;
   float32_t solutions[2];
   uint8_t num_solutions = 0U;

   (void)RDU_Stage2_Find_Intersection(p_search->r1_v, p_search->r2_predicted, p_search->dt, p_search->vxs, p_search->vys,
                                      p_search->K_stat, p_search->vua_current, p_search->p_prev_scan_det, p_search->prev_det_idx,
                                      fov_rad, solutions, &num_solutions);

   if (num_solutions > 0U)
   {
      uint8_t sol_i;

      for (sol_i = 0U; sol_i < num_solutions; sol_i++)
      {
         p_search->closest_az     = solutions[sol_i];
         p_search->detection_type = RDU_S2_DET_TYPE_INTERSECTION;
         RDU_Stage2_Collect_Top_Matches(p_stage2_internals, p_search, prev_idx, p_search->prev_det_idx, K_mov);
      }
   }
   else
   {
      if (RDU_Stage2_Find_Minimum_Distance(p_search->r1_v, p_search->r2_predicted, p_search->dt, p_search->vxs, p_search->vys,
                                           p_search->p_prev_scan_det, p_search->prev_det_idx, fov_rad, &closest_az, &min_dist) &&
          (min_dist <= T_max))
      {
         p_search->closest_az     = closest_az;
         p_search->detection_type = RDU_S2_DET_TYPE_PROXIMITY;
         RDU_Stage2_Collect_Top_Matches(p_stage2_internals, p_search, prev_idx, p_search->prev_det_idx, K_mov);
      }
   }
}
#endif
#ifdef BBE_ENABLE
static void RDU_Stage2_Init_Current_Search_Batch(RDU_Stage2_Current_Search_T *p_search_batch, uint16_t batch_count,
                                                 const float32_t *az_curr, const float32_t *cos_curr, const float32_t *sin_curr,
                                                 const float32_t *r_curr, const float32_t *rr_curr, const bool *p_was_ambiguous,
                                                 const RDU_Prev_Scan_Det_T *p_prev_scan_det, const uint16_t *p_prev_det_idx_batch,
                                                 uint16_t n_curr, float32_t dt, const float32_t *p_r1_v,
                                                 const float32_t *p_r2_predicted, const uint16_t *p_range_first_batch,
                                                 const uint16_t *p_range_last_batch, float32_t vxs, float32_t vys,
                                                 float32_t vua_current, float32_t rr_diff_norm_factor, int8_t K_stat,
                                                 bool apply_rr_gate, const RDU_Params_T *p_rdu_params,
                                                 const RDU_Internals_T *p_rdu_internals, const float32_t *p_rr1_unwrapped)
{
   uint16_t lane;

   for (lane = 0U; lane < batch_count; lane++)
   {
      RDU_Stage2_Init_Current_Search(&p_search_batch[lane], az_curr, cos_curr, sin_curr, r_curr, rr_curr, p_was_ambiguous,
                                     p_prev_scan_det, p_prev_det_idx_batch[lane], n_curr, dt, p_r1_v[lane], p_r2_predicted[lane],
                                     p_range_first_batch[lane], p_range_last_batch[lane], vxs, vys, vua_current,
                                     rr_diff_norm_factor, K_stat, apply_rr_gate, p_rdu_params, p_rdu_internals,
                                     p_rr1_unwrapped[lane]);
   }
}

static void RDU_Stage2_Normalize_Angle_Batch(const float32_t *p_angle_in, uint16_t batch_count, float32_t *p_angle_out)
{
   float32_t angle_buf[RDU_S2_VEC_WIDTH] = {0.0f};
   xb_vecN_2xf32 v_angle;
   xb_vecN_2xf32 v_shifted;
   xb_vecN_2xf32 v_wrapped;
   xb_vecN_2xf32 v_two_pi;
   xb_vecN_2xf32 v_pi;
   uint16_t lane;

   for (lane = 0U; lane < batch_count; lane++)
   {
      angle_buf[lane] = p_angle_in[lane];
   }

   v_angle   = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)angle_buf, 0);
   v_pi      = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(M_PI_F), 0);
   v_two_pi  = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(2.0f * M_PI_F), 0);
   v_shifted = BBE_ADDN_2XF32(v_angle, v_pi);
   v_wrapped = BBE_SUBN_2XF32(
      v_shifted, BBE_MULN_2XF32(v_two_pi, BBE_FIFLOORN_2XF32(BBE_MULN_2XF32(
                                             v_shifted, BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(1.0f / (2.0f * M_PI_F)), 0)))));
   v_wrapped = BBE_SUBN_2XF32(v_wrapped, v_pi);
   BBE_SVN_2XF32_I(v_wrapped, (xb_vecN_2xf32 *)p_angle_out, 0);
}

static void RDU_Stage2_Find_Intersection_Batch(const float32_t *p_r1_v, const float32_t *p_r2_predicted,
                                               const float32_t *p_cos_theta1, const float32_t *p_sin_theta1, uint16_t batch_count,
                                               float32_t dt, float32_t vxs, float32_t vys, int8_t K_stat, float32_t vua_current,
                                               float32_t fov_rad, bool *p_has_intersection, float32_t (*p_solutions)[2],
                                               uint8_t *p_num_solutions)
{
   float32_t __attribute__((aligned(32))) A_buf[RDU_S2_VEC_WIDTH]             = {0.0f};
   float32_t __attribute__((aligned(32))) B_buf[RDU_S2_VEC_WIDTH]             = {0.0f};
   float32_t __attribute__((aligned(32))) C_buf[RDU_S2_VEC_WIDTH]             = {0.0f};
   float32_t __attribute__((aligned(32))) sol0_buf[RDU_S2_VEC_WIDTH]          = {0.0f};
   float32_t __attribute__((aligned(32))) sol1_buf[RDU_S2_VEC_WIDTH]          = {0.0f};
   float32_t __attribute__((aligned(32))) solution_mask_buf[RDU_S2_VEC_WIDTH] = {0.0f};
   float32_t __attribute__((aligned(32))) R_buf[RDU_S2_VEC_WIDTH]             = {0.0f};
   bool __attribute__((aligned(32))) has_real_solution[RDU_S2_VEC_WIDTH]      = {false};
   xb_vecN_2xf32 v_r1;
   xb_vecN_2xf32 v_r2_predicted;
   xb_vecN_2xf32 v_cos_theta1;
   xb_vecN_2xf32 v_sin_theta1;
   xb_vecN_2xf32 v_A;
   xb_vecN_2xf32 v_abs_C;
   xb_vecN_2xf32 v_B;
   xb_vecN_2xf32 v_C;
   xb_vecN_2xf32 v_C_over_R;
   xb_vecN_2xf32 v_eps;
   xb_vecN_2xf32 v_phi;
   xb_vecN_2xf32 v_one;
   xb_vecN_2xf32 v_R;
   xb_vecN_2xf32 v_sol0;
   xb_vecN_2xf32 v_sol1;
   xb_vecN_2xf32 v_dt_vxs;
   xb_vecN_2xf32 v_dt_vys;
   xb_vecN_2xf32 v_dt_kstat_vua;
   xb_vecN_2xf32 v_zero;
   xb_vecN_2xf32 v_safe_R;
   vboolN_2 pred_has_real_solution;
   vboolN_2 pred_has_valid_r;
   uint16_t lane;

   for (lane = 0U; lane < batch_count; lane++)
   {
      p_has_intersection[lane] = false;
      p_num_solutions[lane]    = 0U;
      p_solutions[lane][0]     = 0.0f;
      p_solutions[lane][1]     = 0.0f;
   }

   v_r1           = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)p_r1_v, 0);
   v_r2_predicted = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)p_r2_predicted, 0);
   v_cos_theta1   = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)p_cos_theta1, 0);
   v_sin_theta1   = BBE_LVN_2XF32_I((const xb_vecN_2xf32 *)p_sin_theta1, 0);
   v_dt_vxs       = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(dt * vxs), 0);
   v_dt_vys       = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(dt * vys), 0);
   v_dt_kstat_vua = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(dt * (float32_t)K_stat * vua_current), 0);
   v_A            = BBE_SUBN_2XF32(BBE_MULN_2XF32(v_r1, v_cos_theta1), v_dt_vxs);
   v_B            = BBE_SUBN_2XF32(BBE_MULN_2XF32(v_r1, v_sin_theta1), v_dt_vys);
   v_C            = BBE_SUBN_2XF32(v_r2_predicted, v_dt_kstat_vua);
   v_R            = BBE_SQRTN_2XF32(BBE_ADDN_2XF32(BBE_MULN_2XF32(v_A, v_A), BBE_MULN_2XF32(v_B, v_B)));
   BBE_SVN_2XF32_I(v_A, (xb_vecN_2xf32 *)A_buf, 0);
   BBE_SVN_2XF32_I(v_B, (xb_vecN_2xf32 *)B_buf, 0);
   BBE_SVN_2XF32_I(v_C, (xb_vecN_2xf32 *)C_buf, 0);
   BBE_SVN_2XF32_I(v_R, (xb_vecN_2xf32 *)R_buf, 0);
   v_phi = bbe_vec_atan2f(v_B, v_A);

   v_zero                 = BBE_ZERON_2XF32();
   v_one                  = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(1.0f), 0);
   v_eps                  = BBE_REPN_2XF32(BBE_MOVN_2XF32_FROMF32(RDU_S2_EPSILON), 0);
   pred_has_valid_r       = BBE_OLTN_2XF32(v_eps, v_R);
   v_safe_R               = BBE_MOVN_2XF32T(v_R, v_eps, pred_has_valid_r);
   v_abs_C                = BBE_ABSN_2XF32(v_C);
   pred_has_real_solution = pred_has_valid_r & BBE_OLTN_2XF32(v_abs_C, v_R);
   v_C_over_R             = BBE_MULN_2XF32(v_C, BBE_RECIPN_2XF32(v_safe_R));
   v_C_over_R             = BBE_MOVN_2XF32T(v_C_over_R, v_zero, pred_has_real_solution);
   BBE_SVN_2XF32_I(BBE_MOVN_2XF32T(v_one, v_zero, pred_has_real_solution), (xb_vecN_2xf32 *)solution_mask_buf, 0);

   for (lane = 0U; lane < batch_count; lane++)
   {
      has_real_solution[lane] = (solution_mask_buf[lane] > 0.5f);
   }

   v_C = bbe_vec_acosf(v_C_over_R);

   v_sol0 = BBE_ADDN_2XF32(v_phi, v_C);
   v_sol1 = BBE_SUBN_2XF32(v_phi, v_C);
   BBE_SVN_2XF32_I(v_sol0, (xb_vecN_2xf32 *)sol0_buf, 0);
   BBE_SVN_2XF32_I(v_sol1, (xb_vecN_2xf32 *)sol1_buf, 0);
   RDU_Stage2_Normalize_Angle_Batch(sol0_buf, batch_count, sol0_buf);
   RDU_Stage2_Normalize_Angle_Batch(sol1_buf, batch_count, sol1_buf);

   for (lane = 0U; lane < batch_count; lane++)
   {
      if ((R_buf[lane] >= RDU_S2_EPSILON) && has_real_solution[lane])
      {
         float32_t sol0 = sol0_buf[lane];
         float32_t sol1 = sol1_buf[lane];

         if (rdu_absf(sol0) <= fov_rad)
         {
            p_solutions[lane][p_num_solutions[lane]] = sol0;
            p_num_solutions[lane]++;
         }

         if (rdu_absf(sol1) <= fov_rad)
         {
            p_solutions[lane][p_num_solutions[lane]] = sol1;
            p_num_solutions[lane]++;
         }

         p_has_intersection[lane] = (p_num_solutions[lane] > 0U);
      }
   }
}

static void RDU_Stage2_Find_Minimum_Distance_Batch(const RDU_Stage2_Current_Search_T *p_search_batch, const bool *p_enable_lane,
                                                   uint16_t batch_count, float32_t fov_rad, bool *p_has_minimum,
                                                   float32_t *p_closest_azimuth, float32_t *p_min_distance)
{
   uint16_t lane;

   for (lane = 0U; lane < batch_count; lane++)
   {
      p_has_minimum[lane]     = false;
      p_closest_azimuth[lane] = 0.0f;
      p_min_distance[lane]    = 0.0f;

      if (p_enable_lane[lane])
      {
         p_has_minimum[lane] = RDU_Stage2_Find_Minimum_Distance(
            p_search_batch[lane].r1_v, p_search_batch[lane].r2_predicted, p_search_batch[lane].dt, p_search_batch[lane].vxs,
            p_search_batch[lane].vys, p_search_batch[lane].p_prev_scan_det, p_search_batch[lane].prev_det_idx, fov_rad,
            &p_closest_azimuth[lane], &p_min_distance[lane]);
      }
   }
}

static void RDU_Stage2_Collect_Search_Matches_Batch(RDU_Internals_T *p_stage2_internals,
                                                    RDU_Stage2_Current_Search_T *p_search_batch, const uint16_t *p_prev_idx_batch,
                                                    const uint16_t *p_prev_det_idx_batch, const float32_t *p_r1_v,
                                                    const float32_t *p_r2_predicted, const float32_t *p_cos_theta1,
                                                    const float32_t *p_sin_theta1, uint16_t batch_count, int8_t K_mov,
                                                    float32_t fov_rad, float32_t T_max, const bool *within_range_limit)
{
   bool has_intersection[RDU_S2_VEC_WIDTH]  = {false};
   bool proximity_enabled[RDU_S2_VEC_WIDTH] = {false};
   bool has_minimum[RDU_S2_VEC_WIDTH]       = {false};
   float32_t closest_az[RDU_S2_VEC_WIDTH]   = {0.0f};
   float32_t min_dist[RDU_S2_VEC_WIDTH]     = {0.0f};
   float32_t solutions[RDU_S2_VEC_WIDTH][2] = {{0.0f}};
   uint8_t num_solutions[RDU_S2_VEC_WIDTH]  = {0U};
   uint16_t lane;

   RDU_Stage2_Find_Intersection_Batch(p_r1_v, p_r2_predicted, p_cos_theta1, p_sin_theta1, batch_count, p_search_batch[0].dt,
                                      p_search_batch[0].vxs, p_search_batch[0].vys, p_search_batch[0].K_stat,
                                      p_search_batch[0].vua_current, fov_rad, has_intersection, solutions, num_solutions);

   for (lane = 0U; lane < batch_count; lane++)
   {
      uint8_t sol_i;
      if (within_range_limit[lane])
      {
         if (has_intersection[lane])
         {
            for (sol_i = 0U; sol_i < num_solutions[lane]; sol_i++)
            {
               p_search_batch[lane].closest_az     = solutions[lane][sol_i];
               p_search_batch[lane].detection_type = RDU_S2_DET_TYPE_INTERSECTION;
               RDU_Stage2_Collect_Top_Matches(p_stage2_internals, &p_search_batch[lane], p_prev_idx_batch[lane],
                                              p_prev_det_idx_batch[lane], K_mov);
            }
         }
         else
         {
            proximity_enabled[lane] = true;
         }
      }
   }

   RDU_Stage2_Find_Minimum_Distance_Batch(p_search_batch, proximity_enabled, batch_count, fov_rad, has_minimum, closest_az,
                                          min_dist);

   for (lane = 0U; lane < batch_count; lane++)
   {
      if (within_range_limit[lane])
      {
         if (has_minimum[lane] && (min_dist[lane] <= T_max))
         {
            p_search_batch[lane].closest_az     = closest_az[lane];
            p_search_batch[lane].detection_type = RDU_S2_DET_TYPE_PROXIMITY;
            RDU_Stage2_Collect_Top_Matches(p_stage2_internals, &p_search_batch[lane], p_prev_idx_batch[lane],
                                           p_prev_det_idx_batch[lane], K_mov);
         }
      }
   }
}

static void RDU_Stage2_Process_Prev_Batch(RDU_Internals_T *p_stage2_internals, const RDU_Prev_Scan_Det_T *p_prev_scan_det,
                                          const uint16_t *p_valid_idx, uint16_t batch_start, uint16_t batch_count,
                                          const float32_t *az_curr, const float32_t *cos_curr, const float32_t *sin_curr,
                                          const float32_t *r_curr, const float32_t *rr_curr, const bool *p_was_ambiguous,
                                          uint16_t n_curr, float32_t dt, float32_t vxs, float32_t vys, float32_t vua_current,
                                          float32_t vua_prev, float32_t rr_diff_norm_factor, bool apply_rr_gate,
                                          const RDU_Params_T *p_rdu_params, const RDU_Internals_T *p_rdu_internals,
                                          float32_t fov_rad, float32_t T_max)
{
   float32_t r1_buf[RDU_S2_VEC_WIDTH]            = {0.0f};
   float32_t rr1_buf[RDU_S2_VEC_WIDTH]           = {0.0f};
   float32_t r2_predicted_buf[RDU_S2_VEC_WIDTH]  = {0.0f};
   float32_t cos_theta1_buf[RDU_S2_VEC_WIDTH]    = {0.0f};
   float32_t sin_theta1_buf[RDU_S2_VEC_WIDTH]    = {0.0f};
   float32_t rr1_unwrapped_buf[RDU_S2_VEC_WIDTH] = {0.0f};
   uint16_t prev_det_idx_batch[RDU_S2_VEC_WIDTH] = {0U};
   uint16_t prev_idx_batch[RDU_S2_VEC_WIDTH]     = {0U};
   uint16_t range_first_buf[RDU_S2_VEC_WIDTH]    = {0U};
   uint16_t range_last_buf[RDU_S2_VEC_WIDTH]     = {0U};
   bool within_range_limit[RDU_S2_VEC_WIDTH]     = {false};
   RDU_Stage2_Current_Search_T search_batch[RDU_S2_VEC_WIDTH];
   uint16_t lane;
   uint16_t k_mov_i;

   for (lane = 0U; lane < batch_count; lane++)
   {
      uint16_t prev_local_idx = (uint16_t)(batch_start + lane);
      uint16_t prev_det_idx   = p_valid_idx[prev_local_idx];

      prev_det_idx_batch[lane] = prev_det_idx;
      prev_idx_batch[lane]     = prev_local_idx;
      r1_buf[lane]             = p_prev_scan_det->prev_range[prev_det_idx];
      rr1_buf[lane]            = p_prev_scan_det->prev_vel[prev_det_idx];
      cos_theta1_buf[lane]     = p_prev_scan_det->prev_cos_az[prev_det_idx];
      sin_theta1_buf[lane]     = p_prev_scan_det->prev_sin_az[prev_det_idx];
   }

   for (k_mov_i = 0U; k_mov_i < p_rdu_params->num_K_moving; k_mov_i++)
   {
      int8_t K_mov  = p_rdu_params->K_values_moving[k_mov_i];
      int8_t K_stat = p_rdu_params->K_values_stationary[k_mov_i];

      if (!RDU_Stage2_Is_K_Stat_Allowed(p_rdu_params, K_mov, K_stat))
      {
         continue;
      }

      bool any_within_range = false;

      for (lane = 0U; lane < batch_count; lane++)
      {
         rr1_unwrapped_buf[lane]  = rr1_buf[lane] + (float32_t)K_mov * vua_prev;
         r2_predicted_buf[lane]   = r1_buf[lane] + rr1_unwrapped_buf[lane] * dt;
         within_range_limit[lane] = (r2_predicted_buf[lane] >= RDU_Stage2_Internal_Flags.min_predicted_range) &&
                                    (r2_predicted_buf[lane] <= RDU_Stage2_Internal_Flags.max_predicted_range);
         if (within_range_limit[lane])
         {
            within_range_limit[lane] = RDU_Stage2_Find_Range_Window_Binary_Search(
               RDU_Stage2_Internal_Flags.stage2_sorted_range, RDU_Stage2_Internal_Flags.stage2_num_sorted_ambiguous,
               r2_predicted_buf[lane], p_rdu_params->range_prediction_threshold, &range_first_buf[lane], &range_last_buf[lane]);
         }
         any_within_range = any_within_range || within_range_limit[lane];
      }

      /* Skip solution finding entirely for this K_stat when no lane has a candidate window. */
      if (!any_within_range)
      {
         continue;
      }

      RDU_Stage2_Init_Current_Search_Batch(
         search_batch, batch_count, az_curr, cos_curr, sin_curr, r_curr, rr_curr, p_was_ambiguous, p_prev_scan_det,
         prev_det_idx_batch, n_curr, dt, r1_buf, r2_predicted_buf, range_first_buf, range_last_buf, vxs, vys, vua_current,
         rr_diff_norm_factor, K_stat, apply_rr_gate, p_rdu_params, p_rdu_internals, rr1_unwrapped_buf);
      RDU_Stage2_Collect_Search_Matches_Batch(p_stage2_internals, search_batch, prev_idx_batch, prev_det_idx_batch, r1_buf,
                                              r2_predicted_buf, cos_theta1_buf, sin_theta1_buf, batch_count, K_mov, fov_rad, T_max,
                                              within_range_limit);
   }
}

#endif
static void RDU_Stage2_Deduplicate_Candidates(RDU_Stage2_Candidate_T *p_candidates, uint16_t num_cands, bool by_prev)
{
   uint16_t match_i;
   uint16_t match_j;

   for (match_i = 0U; match_i < num_cands; match_i++)
   {
      RDU_Stage2_Candidate_T *cand_i = &p_candidates[match_i];
      uint16_t key_i;

      if (cand_i->is_valid == 0U)
      {
         continue;
      }

      key_i = by_prev ? cand_i->prev_local_idx : cand_i->curr_det_idx;

      for (match_j = (uint16_t)(match_i + 1U); match_j < num_cands; match_j++)
      {
         RDU_Stage2_Candidate_T *cand_j = &p_candidates[match_j];
         uint16_t key_j                 = by_prev ? cand_j->prev_local_idx : cand_j->curr_det_idx;

         if ((cand_j->is_valid != 0U) && (key_j == key_i))
         {
            if (cand_j->quality_score < cand_i->quality_score)
            {
               cand_i->is_valid = 0U;
               break;
            }

            cand_j->is_valid = 0U;
         }
      }
   }
}
#ifndef BBE_ENABLE
static void RDU_Stage2_Process_K_Stat(RDU_Internals_T *p_stage2_internals, const float32_t *az_curr, const float32_t *r_curr,
                                      const float32_t *cos_curr, const float32_t *sin_curr, const float32_t *rr_curr,
                                      const bool *p_was_ambiguous, const RDU_Prev_Scan_Det_T *p_prev_scan_det,
                                      uint16_t prev_det_idx, uint16_t n_curr, float32_t dt, float32_t r1_v, float32_t r2_predicted,
                                      float32_t vxs, float32_t vys, float32_t vua_current, float32_t rr_diff_norm_factor,
                                      bool apply_rr_gate, const RDU_Params_T *p_rdu_params, int8_t K_mov, int8_t K_stat,
                                      uint16_t prev_idx, float32_t fov_rad, float32_t T_max, float32_t rr1_unwrapped)
{
   uint16_t range_first;
   uint16_t range_last;

   /* Skip solution finding entirely when the predicted range has no candidate window. */
   if (RDU_Stage2_Is_K_Stat_Allowed(p_rdu_params, K_mov, K_stat) &&
       RDU_Stage2_Find_Range_Window_Binary_Search(RDU_Stage2_Internal_Flags.stage2_sorted_range,
                                                  RDU_Stage2_Internal_Flags.stage2_num_sorted_ambiguous, r2_predicted,
                                                  p_rdu_params->range_prediction_threshold, &range_first, &range_last))
   {
      RDU_Stage2_Current_Search_T search;

      RDU_Stage2_Init_Current_Search(&search, az_curr, cos_curr, sin_curr, r_curr, rr_curr, p_was_ambiguous, p_prev_scan_det,
                                     prev_det_idx, n_curr, dt, r1_v, r2_predicted, range_first, range_last, vxs, vys, vua_current,
                                     rr_diff_norm_factor, K_stat, apply_rr_gate, p_rdu_params, p_stage2_internals, rr1_unwrapped);
      RDU_Stage2_Collect_Search_Matches(p_stage2_internals, &search, prev_idx, K_mov, fov_rad, T_max);
   }
}
#endif

/******************************************************************************
 * Name:  RDU_Stage2_Internals_Clear
 *
 * @brief Reset all per-scan temporary fields; leave prev-scan cache intact.
 ******************************************************************************/
bool RDU_Stage2_Internals_Clear(RDU_Internals_T *p_stage2_internals, RDU_Stage2_Output_Data_T *p_stage2_output_data,
                                uint16_t num_detections)
{
   bool is_success = true;
   uint16_t i;
   (void)num_detections;

   if ((NULL == p_stage2_internals) || (NULL == p_stage2_output_data))
   {
      is_success = false;
   }

   if (is_success)
   {
      /* Clear Stage 2 output fields for current-scan detections */
      for (i = 0U; i < MAX_NUM_DETECTS; i++)
      {
         RDU_Stage2_Internal_Flags.was_ambiguous[i]                = false;
         RDU_Stage2_Internal_Flags.is_updated_by_stage2[i]         = false;
         RDU_Stage2_Internal_Flags.is_stationary_updated[i]        = false;
         RDU_Stage2_Internal_Flags.could_be_zero_rr[i]             = false;
         p_stage2_output_data->stage2_classification_confidence[i] = 0.0f;
      }

      /* Clear working memory counters */
      p_stage2_internals->num_valid_dets2test               = 0U;
      p_stage2_internals->num_candidates                    = 0U;
      RDU_Stage2_Internal_Flags.stage2_num_sorted_ambiguous = 0U;

      /* Clear cached range bounds (will be recomputed after sorted range is populated) */
      RDU_Stage2_Internal_Flags.min_predicted_range = 0.0f;
      RDU_Stage2_Internal_Flags.max_predicted_range = 0.0f;
   }

   return is_success;
}

/******************************************************************************
 * Name:  RDU_Stage2_Compute_Range_Bounds
 *
 * @brief Cache the min/max predicted range bounds for the current scan.
 * These depend only on the sorted range and prediction threshold, so they
 * are computed once and reused across all K_mov/K_stat combinations.
 ******************************************************************************/
void RDU_Stage2_Compute_Range_Bounds(RDU_Internals_T *p_stage2_internals, const RDU_Params_T *p_rdu_params)
{
   if ((NULL != p_stage2_internals) && (NULL != p_rdu_params) && (RDU_Stage2_Internal_Flags.stage2_num_sorted_ambiguous > 0U))
   {
      RDU_Stage2_Internal_Flags.min_predicted_range =
         RDU_Stage2_Internal_Flags.stage2_sorted_range[0U] - p_rdu_params->range_prediction_threshold;
      RDU_Stage2_Internal_Flags.max_predicted_range =
         RDU_Stage2_Internal_Flags.stage2_sorted_range[RDU_Stage2_Internal_Flags.stage2_num_sorted_ambiguous - 1U] +
         p_rdu_params->range_prediction_threshold;
   }
}

/******************************************************************************
 * Name:  RDU_Stage2_Check_Absolute_Velocity
 *
 * @brief Compute absolute target speed from consecutive scan positions and
 *        check against maximum realistic limit.
 *
 *   Relative velocity:   vx_rel = (x2 − x1) / dt,  vy_rel = (y2 − y1) / dt
 *   Absolute velocity:   vx_abs = vx_rel + vxs,     vy_abs = vy_rel + vys
 *   Speed magnitude:     v = sqrt(vx_abs² + vy_abs²)
 ******************************************************************************/
bool RDU_Stage2_Check_Absolute_Velocity(float32_t r1, float32_t r2_actual, float32_t dt, float32_t vxs, float32_t vys,
                                        float32_t max_velocity, const RDU_Internals_T *p_rdu_internals,
                                        const RDU_Prev_Scan_Det_T *p_prev_scan_det, uint16_t prev_det_idx, uint16_t curr_det_idx,
                                        float32_t *p_abs_speed)
{
   bool is_success = false;

   if ((NULL != p_abs_speed) && (NULL != p_rdu_internals) && /* guard: cached curr trig */
       (NULL != p_prev_scan_det) &&                          /* guard: cached prev trig */
       (dt >= RDU_S2_EPSILON))
   {
      float32_t prev_cos_az = p_prev_scan_det->prev_cos_az[prev_det_idx];
      float32_t prev_sin_az = p_prev_scan_det->prev_sin_az[prev_det_idx];
      float32_t curr_cos_az = p_rdu_internals->cos_theta[curr_det_idx];
      float32_t curr_sin_az = p_rdu_internals->sin_theta[curr_det_idx];

      /* Cartesian positions relative to sensor */
      float32_t x1 = r1 * prev_cos_az;
      float32_t y1 = r1 * prev_sin_az;
      float32_t x2 = r2_actual * curr_cos_az;
      float32_t y2 = r2_actual * curr_sin_az;

      /* Relative velocity (sensor frame) */
      float32_t vx_rel = (x2 - x1) / dt;
      float32_t vy_rel = (y2 - y1) / dt;

      /* Absolute velocity (world frame) = relative + sensor own velocity */
      float32_t vx_abs = vx_rel + vxs;
      float32_t vy_abs = vy_rel + vys;

      *p_abs_speed = rdu_sqrtf(vx_abs * vx_abs + vy_abs * vy_abs);

      is_success = (*p_abs_speed <= max_velocity);
   }

   return is_success;
}

/******************************************************************************
 * Name:  RDU_Stage2_Find_Intersection
 *
 * @brief Solve  A·cos(θ₂) + B·sin(θ₂) = C  for θ₂ ∈ [−FOV, +FOV].
 *
 * where
 *   A = r1·cos(θ₁) − dt·vxs
 *   B = r1·sin(θ₁) − dt·vys
 *   C = r2_pred   − dt·K_stat·vua
 *
 * Using R·cos(θ₂ − φ) = C  ⇒  θ₂ = φ ± acos(C/R).
 ******************************************************************************/
bool RDU_Stage2_Find_Intersection(float32_t r1, float32_t r2, float32_t dt, float32_t vxs, float32_t vys, int8_t K_stat,
                                  float32_t vua_current, const RDU_Prev_Scan_Det_T *p_prev_scan_det, uint16_t prev_det_idx,
                                  float32_t fov_rad, float32_t *p_solutions, uint8_t *p_num_solutions)
{
   bool is_success = false;

   if ((NULL != p_solutions) && (NULL != p_num_solutions) && (NULL != p_prev_scan_det))
   {
      *p_num_solutions = 0U;

      float32_t cos_t1 = p_prev_scan_det->prev_cos_az[prev_det_idx];
      float32_t sin_t1 = p_prev_scan_det->prev_sin_az[prev_det_idx];
      float32_t A      = r1 * cos_t1 - dt * vxs;
      float32_t B      = r1 * sin_t1 - dt * vys;
      float32_t C      = r2 - dt * (float32_t)K_stat * vua_current;
      float32_t R      = rdu_sqrtf(A * A + B * B);

      if (R >= RDU_S2_EPSILON)
      {
         float32_t C_over_R = C * rdu_finv(R);

         if (rdu_absf(C_over_R) <= 1.0f)
         {
            float32_t phi          = rm_atan2f(B, A);
            float32_t angle_offset = rdu_acosf(C_over_R); /* acosf(C_over_R) = PI/2 - rm_asinf(C_over_R) */

            float32_t sol[2];
            sol[0] = RDU_Stage2_Normalize_Angle(phi + angle_offset);
            sol[1] = RDU_Stage2_Normalize_Angle(phi - angle_offset);

            for (uint8_t s = 0U; s < 2U; s++)
            {
               if (rdu_absf(sol[s]) <= fov_rad)
               {
                  p_solutions[*p_num_solutions] = sol[s];
                  (*p_num_solutions)++;
               }
            }

            is_success = (*p_num_solutions > 0U);
         }
      }
   }

   return is_success;
}

/******************************************************************************
 * Name:  RDU_Stage2_Find_Minimum_Distance
 *
 * @brief Find the azimuth θ₂ where d/dθ[rr₂(θ) − rr_ref(θ)] = 0  (proximity).
 *
 * The minimum azimuth is K_stat-independent (the constant shift K_stat·vua
 * does not affect the location of the minimum, only its value).
 *
 *   numerator   = r1·sin(θ₁)/dt − vys
 *   denominator = r1·cos(θ₁)/dt − vxs
 *   θ_closest   = atan2(numerator, denominator)
 *
 * p_min_distance is the K_stat=0 distance at that azimuth; the caller adds
 * the K_stat correction when needed.
 ******************************************************************************/
bool RDU_Stage2_Find_Minimum_Distance(float32_t r1, float32_t r2, float32_t dt, float32_t vxs, float32_t vys,
                                      const RDU_Prev_Scan_Det_T *p_prev_scan_det, uint16_t prev_det_idx, float32_t fov_rad,
                                      float32_t *p_closest_azimuth, float32_t *p_min_distance)
{
   if ((NULL == p_closest_azimuth) || (NULL == p_min_distance) || (NULL == p_prev_scan_det) || (dt < RDU_S2_EPSILON))
   {
      return false;
   }

   float32_t cos_t1    = p_prev_scan_det->prev_cos_az[prev_det_idx];
   float32_t sin_t1    = p_prev_scan_det->prev_sin_az[prev_det_idx];
   float32_t r1_cos_t1 = r1 * cos_t1;
   float32_t r1_sin_t1 = r1 * sin_t1;
   float32_t num_val   = r1_sin_t1 / dt - vys;
   float32_t den_val   = r1_cos_t1 / dt - vxs;
   float32_t closest_az;

   if (rdu_absf(den_val) < RDU_S2_EPSILON)
   {
      if (rdu_absf(num_val) < RDU_S2_EPSILON)
      {
         closest_az = rm_atan2f(sin_t1, cos_t1); /* Degenerate - reconstruct angle from cached trig */
      }
      else
      {
         return false; /* Undefined minimum inside FOV */
      }
   }
   else
   {
      closest_az = rm_atan2f(num_val, den_val);
   }

   if (rdu_absf(closest_az) > fov_rad)
   {
      return false; /* Minimum outside sensor FOV */
   }

   {
      float32_t cos_closest_az = rm_sinf((M_PI_F / 2.0f) + closest_az);
      float32_t sin_closest_az = rm_sinf(closest_az);
      float32_t cos_delta_az   = cos_closest_az * cos_t1 + sin_closest_az * sin_t1;
      float32_t rr2_at         = (r2 - r1 * cos_delta_az) / dt;
      float32_t rr_ref_at      = -cos_closest_az * vxs - sin_closest_az * vys;

      *p_closest_azimuth = closest_az;
      *p_min_distance    = rdu_absf(rr2_at - rr_ref_at);
   }

   return true;
}

/******************************************************************************
 * Name:  RDU_Stage2_Update_Motion_Status
 *
 * @brief Update rrr_motion_status for AMBIGUOUS detections only.
 *
 * With enable_zero_rr_check = false (MATLAB default):
 *   is_updated_by_stage2 = true                         → MOVING_SPECIAL (3)
 *   is_ambiguous & !is_updated_by_stage2                → STATIONARY     (0)
 *
 * With enable_zero_rr_check = true:
 *   is_updated_by_stage2 = true                         → MOVING_SPECIAL (3)
 *   is_ambiguous & !is_updated_by_stage2 & |rr| > T[i]  → STATIONARY     (0)
 *   is_ambiguous & !is_updated_by_stage2 & |rr| ≤ T[i]  → AMBIGUOUS      (2) [no change]
 ******************************************************************************/
bool RDU_Stage2_Update_Motion_Status(RDU_Data_T *p_rdu_data, const RDU_Params_T *p_stage2_params,
                                     RDU_Stage2_Output_Data_T *p_stage2_output_data, uint16_t num_detections,
                                     RDU_Internals_T *p_rdu_internals)
{
   bool is_success = true;
   uint16_t i;

   if ((NULL == p_rdu_data) || (NULL == p_stage2_params) || (NULL == p_stage2_output_data) || (NULL == p_rdu_internals))
   {
      is_success = false;
   }

   if (is_success)
   {
      int8_t *ms                     = p_rdu_data->p_rdu_stage1_output_data->stage1_rrr_motion_status;
      int8_t *p_stage2_motion_status = p_stage2_output_data->stage2_rrr_motion_status;
      const float32_t *T             = p_rdu_internals->stage1_moving_det_thold;

      /* Use pre-populated velocity array from RDU_Internals */
      const float32_t *rr = &p_rdu_internals->cur_vel[0];

      for (i = 0U; i < num_detections; i++)
      {
         p_stage2_motion_status[i] = ms[i]; /* Init with motion status from Stage 1 */

         /* Only process detections that were AMBIGUOUS at Stage 2 entry */
         if (!RDU_Stage2_Internal_Flags.was_ambiguous[i])
         {
            continue;
         }

         if (RDU_Stage2_Internal_Flags.is_updated_by_stage2[i])
         {
            /* Confirmed moving special case – always promote regardless of zero-RR check */
            p_stage2_motion_status[i] = (int8_t)RDU_MOTION_STATUS_MOVING_SPECIAL;
            ;
         }
         else if (p_stage2_params->enable_zero_rr_check && (rdu_absf(rr[i]) <= T[i]))
         {
            /* Near-zero RR – cannot decide; leave as AMBIGUOUS.
             * Record in output for downstream diagnostics.        */
            RDU_Stage2_Internal_Flags.could_be_zero_rr[i] = true;
         }
         else
         {
            /* Unmatched, not near-zero RR → reclassify as STATIONARY */
            p_stage2_motion_status[i]                          = (int8_t)RDU_MOTION_STATUS_STATIONARY;
            RDU_Stage2_Internal_Flags.is_stationary_updated[i] = true;
         }
      }
   }

   return is_success;
}

bool RDU_Stage2_Update_Motion_Status_Vec(RDU_Data_T *p_rdu_data, const RDU_Params_T *p_stage2_params,
                                         RDU_Stage2_Output_Data_T *p_stage2_output, uint16_t num_detections,
                                         RDU_Internals_T *p_rdu_internals)
{
#if BBE_ENABLE
   bool ret_value = false;
   if ((NULL != p_rdu_data) && (NULL != p_stage2_params) && (NULL != p_stage2_output) && (NULL != p_rdu_internals))
   {
      uint16_t i;
      uint16_t vec_width             = XCHAL_BBEN_SIMD_WIDTH;
      uint16_t num_full_vec          = num_detections / vec_width;
      uint16_t rem                   = num_detections % vec_width;
      int8_t *ms                     = &p_rdu_data->p_rdu_stage1_output_data->stage1_rrr_motion_status[0];
      int8_t *p_stage2_motion_status = &p_stage2_output->stage2_rrr_motion_status[0];
      const float32_t *T             = &p_rdu_internals->stage1_moving_det_thold[0];
      const float32_t *rr            = &p_rdu_internals->cur_vel[0];
      /* input vectors */
      xb_vecN_2xf32 *p_moving_det_thold   = (xb_vecN_2xf32 *)&p_rdu_internals->stage1_moving_det_thold[0];
      xb_vecN_2xf32 *p_cur_vel            = (xb_vecN_2xf32 *)&p_rdu_internals->cur_vel[0];
      xb_vecNx16U *p_is_updated_by_stage2 = (xb_vecNx16U *)&RDU_Stage2_Internal_Flags.is_updated_by_stage2[0];
      xb_vecNx16U *p_was_ambiguous        = (xb_vecNx16U *)&RDU_Stage2_Internal_Flags.was_ambiguous[0];
      xb_vecN_2xf32 v_mov_det_thold;
      xb_vecN_2xf32 v_cur_vel;

      /* output vectors */
      xb_vecNx16 *p_rrr_status_load        = (xb_vecNx16 *)&p_rdu_data->p_rdu_stage1_output_data->stage1_rrr_motion_status[0];
      xb_vecNx16 *p_rrr_status_store       = (xb_vecNx16 *)&p_stage2_output->stage2_rrr_motion_status[0];
      xb_vecNx16U *p_could_be_zero_rr      = (xb_vecNx16U *)&RDU_Stage2_Internal_Flags.could_be_zero_rr[0];
      xb_vecNx16U *p_is_stationary_updated = (xb_vecNx16U *)&RDU_Stage2_Internal_Flags.is_stationary_updated[0];

      xb_vecNx16 v_rrr_status;
      xb_vecNx16U v_is_updated_by_stage2;
      xb_vecNx16U v_was_amb;
      xb_vecNx16U v_temp1_16bitU, v_temp2_16bitU, v_temp3_16bitU;
      xb_vecNx16 v_temp1_16bit, v_temp2_16_bit;
      xb_vecNx16U v_could_be_zero_rr, v_is_stationary_updated;
      vboolN f_is_stationary_updated;
      vboolN f_was_ambiguous;
      vboolN f_is_updated_by_stage2;
      vboolN_2 f_could_be_zero_rr_1;
      vboolN_2 f_could_be_zero_rr_2;
      vboolN f_could_be_zero_rr;
      vboolN_2 f_temp1;
      vboolN_2 f_temp2;
      xb_vecNx16 vec_rrr_status_16bit_new;
      xb_vecNx16 vec_lo_8bit_mask        = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
      xb_vecNx16U vec_lo_8bit_mask_u     = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);
      valign align_rrr_ms                = BBE_LANX16_PP(p_rrr_status_load);
      valign align_was_amb               = BBE_LANX16U_PP(p_was_ambiguous);
      valign align_is_stationary_updated = BBE_LANX16U_PP(p_is_stationary_updated);
      valign align_could_be_zero_rr      = BBE_LANX16U_PP(p_could_be_zero_rr);

      for (i = 0U; i < num_full_vec; i++)
      {
         BBE_LAVNX16U_XP(v_was_amb, align_was_amb, p_was_ambiguous, 16U);
         v_temp1_16bitU  = BBE_SELNX16UI((v_was_amb >> 8U), v_was_amb, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask_u;
         f_was_ambiguous = v_temp1_16bitU != BBE_ZERONX16U();

         BBE_LAVNX16U_XP(v_is_updated_by_stage2, BBE_LANX16U_PP(p_is_updated_by_stage2), p_is_updated_by_stage2, 16U);
         v_temp1_16bitU =
            BBE_SELNX16UI((v_is_updated_by_stage2 >> 8U), v_is_updated_by_stage2, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask_u;
         f_is_updated_by_stage2 = v_temp1_16bitU != BBE_ZERONX16U();

         BBE_LAVNX16_XP(v_rrr_status, BBE_LANX16_PP(p_rrr_status_load), p_rrr_status_load, 16U);
         vec_rrr_status_16bit_new = BBE_SELNX16I((v_rrr_status >> 8U), v_rrr_status, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask;
         v_rrr_status             = BBE_MOVNX16UT((xb_vecNx16U)RDU_MOTION_STATUS_MOVING_SPECIAL, vec_rrr_status_16bit_new,
                                                  f_is_updated_by_stage2 & f_was_ambiguous);

         BBE_LAN_2XF32_IP(v_mov_det_thold, BBE_LAN_2XF32_PP(p_moving_det_thold), p_moving_det_thold);
         BBE_LAN_2XF32_IP(v_cur_vel, BBE_LAN_2XF32_PP(p_cur_vel), p_cur_vel);
         BBE_EXTRACTBN(f_temp2, f_temp1, f_was_ambiguous);
         f_could_be_zero_rr_1 = (BBE_ABSN_2XF32(v_cur_vel) <= v_mov_det_thold) & f_temp1;
         BBE_EXTRACTBN(f_temp2, f_temp1, ~f_is_updated_by_stage2);
         f_could_be_zero_rr_1 = f_could_be_zero_rr_1 & f_temp1;

         BBE_LAN_2XF32_IP(v_mov_det_thold, BBE_LAN_2XF32_PP(p_moving_det_thold), p_moving_det_thold);
         BBE_LAN_2XF32_IP(v_cur_vel, BBE_LAN_2XF32_PP(p_cur_vel), p_cur_vel);
         BBE_EXTRACTBN(f_temp2, f_temp1, f_was_ambiguous);
         f_could_be_zero_rr_2 = (BBE_ABSN_2XF32(v_cur_vel) <= v_mov_det_thold) & f_temp2;
         BBE_EXTRACTBN(f_temp2, f_temp1, ~f_is_updated_by_stage2);
         f_could_be_zero_rr_2 = f_could_be_zero_rr_2 & f_temp2;

         f_could_be_zero_rr      = BBE_JOINBN_2(f_could_be_zero_rr_2, f_could_be_zero_rr_1);
         f_could_be_zero_rr      = f_could_be_zero_rr & xtbool_rtor_vboolN((xtbool)p_stage2_params->enable_zero_rr_check);
         f_is_stationary_updated = f_was_ambiguous & ~f_is_updated_by_stage2 & ~f_could_be_zero_rr;
         v_rrr_status            = BBE_MOVNX16UT((xb_vecNx16U)RDU_MOTION_STATUS_STATIONARY, v_rrr_status, f_is_stationary_updated);
         v_could_be_zero_rr      = BBE_MOVNX16UT((xb_vecNx16U)(1U), BBE_ZERONX16U(), f_could_be_zero_rr);
         v_is_stationary_updated = BBE_MOVNX16UT((xb_vecNx16U)(1U), BBE_ZERONX16U(), f_is_stationary_updated);

         v_temp1_16bit            = BBE_SELNX16I(v_rrr_status, v_rrr_status, BBE_SELI_EXTRACT_1_OF_2_OFF_0);
         v_temp2_16_bit           = BBE_SELNX16I(v_rrr_status, v_rrr_status, BBE_SELI_EXTRACT_1_OF_2_OFF_1);
         vec_rrr_status_16bit_new = (v_temp2_16_bit << 8U) | v_temp1_16bit;
         BBE_SAVRNX16_XP(vec_rrr_status_16bit_new, align_rrr_ms, p_rrr_status_store, 16U);
         BBE_SANX16POS_FP(align_rrr_ms, p_rrr_status_store);

         v_temp1_16bitU = BBE_SELNX16UI(v_is_stationary_updated, v_is_stationary_updated, BBE_SELI_EXTRACT_1_OF_2_OFF_0);
         v_temp2_16bitU = BBE_SELNX16UI(v_is_stationary_updated, v_is_stationary_updated, BBE_SELI_EXTRACT_1_OF_2_OFF_1);
         v_temp3_16bitU = (v_temp2_16bitU << 8U) | v_temp1_16bitU;
         BBE_SAVRNX16U_XP(v_temp3_16bitU, align_is_stationary_updated, p_is_stationary_updated, 16U);
         BBE_SANX16UPOS_FP(align_is_stationary_updated, p_is_stationary_updated);

         v_temp1_16bitU = BBE_SELNX16UI(v_could_be_zero_rr, v_could_be_zero_rr, BBE_SELI_EXTRACT_1_OF_2_OFF_0);
         v_temp2_16bitU = BBE_SELNX16UI(v_could_be_zero_rr, v_could_be_zero_rr, BBE_SELI_EXTRACT_1_OF_2_OFF_1);
         v_temp3_16bitU = (v_temp2_16bitU << 8U) | v_temp1_16bitU;
         BBE_SAVRNX16U_XP(v_temp3_16bitU, align_could_be_zero_rr, p_could_be_zero_rr, 16U);
         BBE_SANX16UPOS_FP(align_could_be_zero_rr, p_could_be_zero_rr);
      }

      if (rem != 0U)
      {
         /* replaced "C" code as tail handling was showing mismatch with BBE instructions, when tail length is odd */
         for (i = num_full_vec * vec_width; i < num_detections; i++)
         {
            p_stage2_motion_status[i] = ms[i];

            /* Only process detections that were AMBIGUOUS at Stage 2 entry */
            if (!RDU_Stage2_Internal_Flags.was_ambiguous[i])
            {
               continue;
            }

            if (RDU_Stage2_Internal_Flags.is_updated_by_stage2[i])
            {
               /* Confirmed moving special case – always promote regardless of zero-RR check */
               p_stage2_motion_status[i] = (int8_t)RDU_MOTION_STATUS_MOVING_SPECIAL;
            }
            else if (p_stage2_params->enable_zero_rr_check && (rdu_absf(rr[i]) <= T[i]))
            {
               /* Near-zero RR – cannot decide; leave as AMBIGUOUS.
                * Record in output for downstream diagnostics.        */
               RDU_Stage2_Internal_Flags.could_be_zero_rr[i] = true;
            }
            else
            {
               /* Unmatched, not near-zero RR → reclassify as STATIONARY */
               p_stage2_motion_status[i]                          = (int8_t)RDU_MOTION_STATUS_STATIONARY;
               RDU_Stage2_Internal_Flags.is_stationary_updated[i] = true;
            }
         }
      }
      ret_value = true;
   }
   return ret_value;
#else
   return RDU_Stage2_Update_Motion_Status(p_rdu_data, p_stage2_params, p_stage2_output, num_detections, p_rdu_internals);
#endif
}

/******************************************************************************
 * Name:  RDU_Stage2_Compute_Confidence
 *
 * @brief Compute Stage 2 classification confidence for each final matched
 *        candidate.  Mirrors MATLAB: compute_stage2_confidence.m
 *
 * Method (Mahalanobis-based Gaussian):
 *   sigma_r  = range_prediction_threshold / 3
 *   sigma_az = angle_prediction_threshold_rad / 3
 *   sigma_rr = rr_diff_norm_factor / 3  (0 → 2-D fallback, Gen8)
 *
 *   if sigma_rr > eps:  d = sqrt( (r_err/σ_r)² + (az_err/σ_az)² + (rr_diff/σ_rr)² )
 *   else:               d = sqrt( (r_err/σ_r)² + (az_err/σ_az)² )
 *
 *   q_type = 1.0  (INTERSECTION) or proximity_confidence_factor (PROXIMITY)
 *   confidence = 0.5 * exp(-0.5 * max(0, 3 - d) * q_type)
 *   clamped to [0.0, 0.5]
 ******************************************************************************/
void RDU_Stage2_Compute_Confidence(float32_t *p_confidence_out, const RDU_Stage2_Candidate_T *p_candidates, uint16_t num_matches,
                                   float32_t rr_diff_norm_factor, const RDU_Params_T *p_stage2_params)
{
#ifdef BBE_ENABLE
   uint16_t i;
   uint16_t vec_width    = XCHAL_BBEN_SIMD_WIDTH >> 1U;
   uint16_t num_full_vec = num_matches / vec_width;
   uint16_t rem          = num_matches % vec_width;

   static const float32_t one_third = 1.0F / 3.0F;

   xb_vecN_2xf32 v_cand_ran_err, v_cand_az_err, v_cand_rr_diff;
   xb_vecN_2xf32 v_d, v_nr, v_naz, v_nrr, v_q_type;
   xb_vecN_2xf32 v_sigma_r_inv, v_sigma_az_inv, v_sigma_rr_inv;
   xb_vecN_2xf32 v_margin, v_arg, v_conf;
   vboolN_2 v_temp1;

   xb_vecN_2xf32 *p_conf_out = (xb_vecN_2xf32 *)(p_confidence_out);

   float32_t __attribute__((aligned(32))) cand_ran_err_arr[XCHAL_BBEN_SIMD_WIDTH >> 1U] = {0.0F};
   float32_t __attribute__((aligned(32))) cand_az_err_arr[XCHAL_BBEN_SIMD_WIDTH >> 1U]  = {0.0F};
   float32_t __attribute__((aligned(32))) cand_rr_diff_arr[XCHAL_BBEN_SIMD_WIDTH >> 1U] = {0.0F};
   uint8_t __attribute__((aligned(16))) cand_det_type[XCHAL_BBEN_SIMD_WIDTH >> 1U]      = {0U};

   v_sigma_r_inv = BBE_DIVN_2XF32((xb_vecN_2xf32)1.0F, (xb_vecN_2xf32)(p_stage2_params->range_prediction_threshold * one_third));
   v_sigma_az_inv =
      BBE_DIVN_2XF32((xb_vecN_2xf32)1.0F, (xb_vecN_2xf32)(p_stage2_params->angle_prediction_threshold_rad * one_third));
   v_sigma_rr_inv =
      BBE_MOVN_2XF32T(BBE_ZERON_2XF32(), BBE_DIVN_2XF32((xb_vecN_2xf32)1.0F, (xb_vecN_2xf32)(rr_diff_norm_factor * one_third)),
                      xtbool_rtor_vboolN_2((rr_diff_norm_factor <= 0)));
   vboolN_2 v_sigma_r_gt_eps  = (v_sigma_r_inv > (xb_vecN_2xf32)RDU_S2_EPSILON);
   vboolN_2 v_sigma_az_gt_eps = (v_sigma_az_inv > (xb_vecN_2xf32)RDU_S2_EPSILON);
   vboolN_2 v_sigma_rr_gt_eps = (v_sigma_rr_inv > (xb_vecN_2xf32)RDU_S2_EPSILON);
   vboolN_2 f_intersection_N_2;
   vboolN f_intersection_N;

   xb_vecNx16U *p_cand_det_type = (xb_vecNx16U *)&cand_det_type[0];
   xb_vecNx16U v_cand_det_type;
   xb_vecNx16U vec_lo_8bit_mask_u = BBE_MOVVINX16U(BBE_MOVVI_LOWER_CHAR);

   for (i = 0U; i < num_full_vec; i++)
   {
      /* Load candidate errors into vectors */
      for (uint16_t j = 0; j < vec_width; j++)
      {
         cand_ran_err_arr[j] = p_candidates[i * vec_width + j].range_error;
         cand_az_err_arr[j]  = p_candidates[i * vec_width + j].azimuth_error;
         cand_rr_diff_arr[j] = p_candidates[i * vec_width + j].rr_diff;
         cand_det_type[j]    = p_candidates[i * vec_width + j].detection_type;
      }

      v_cand_ran_err = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)cand_ran_err_arr, 0U);
      v_cand_az_err  = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)cand_az_err_arr, 0U);
      v_cand_rr_diff = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)cand_rr_diff_arr, 0U);

      v_nr  = BBE_MULN_2XF32(v_cand_ran_err, v_sigma_r_inv);
      v_nr  = BBE_MOVN_2XF32T(v_nr, (xb_vecN_2xf32)(0.0F), v_sigma_r_gt_eps);
      v_naz = BBE_MULN_2XF32(v_cand_az_err, v_sigma_az_inv);
      v_naz = BBE_MOVN_2XF32T(v_naz, (xb_vecN_2xf32)(0.0F), v_sigma_az_gt_eps);
      v_nrr = BBE_MULN_2XF32(v_cand_rr_diff, v_sigma_rr_inv);
      v_nrr = BBE_MOVN_2XF32T(v_nrr, (xb_vecN_2xf32)(0.0F), v_sigma_rr_gt_eps);

      v_d = BBE_MULN_2XF32(v_nr, v_nr);
      BBE_MULAN_2XF32(v_d, v_naz, v_naz);
      BBE_MULAN_2XF32T(v_d, v_nrr, v_nrr, v_sigma_rr_gt_eps);
      v_d = BBE_SQRTN_2XF32(v_d);

      /* load detection type */

      BBE_LAVNX16U_XP(v_cand_det_type, BBE_LANX16U_PP(p_cand_det_type), p_cand_det_type, vec_width);
      v_cand_det_type  = BBE_SELNX16UI((v_cand_det_type >> 8U), v_cand_det_type, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask_u;
      f_intersection_N = (v_cand_det_type == (xb_vecNx16U)(RDU_S2_DET_TYPE_INTERSECTION));
      BBE_EXTRACTBN(v_temp1, f_intersection_N_2, f_intersection_N);
      v_q_type =
         BBE_MOVN_2XF32T((xb_vecN_2xf32)(1.0F), (xb_vecN_2xf32)(p_stage2_params->proximity_confidence_factor), f_intersection_N_2);

      v_margin = (xb_vecN_2xf32)(3.0F) - (v_d);
      v_arg    = BBE_MAXN_2XF32(BBE_MULN_2XF32(v_margin, v_q_type), (xb_vecN_2xf32)0.0F);
      //    	v_arg = BBE_MULN_2XF32(v_margin, v_q_type);

      v_arg = BBE_MULN_2XF32(v_arg, (xb_vecN_2xf32)(-0.5F));

      v_conf = (xb_vecN_2xf32)(0.5F) * bbe_vec_expf(v_arg);

      v_conf = BBE_MOVN_2XF32T((xb_vecN_2xf32)0.0F, v_conf, (v_conf < (xb_vecN_2xf32)(0.0F)));
      v_conf = BBE_MOVN_2XF32T((xb_vecN_2xf32)0.5F, v_conf, (v_conf > (xb_vecN_2xf32)(0.5F)));

      BBE_SVN_2XF32_IP(v_conf, p_conf_out, 32U);
   }
   if (rem != 0)
   {
      uint16_t rem_base = num_full_vec * vec_width;
      /* Load candidate errors into vectors */
      for (uint16_t m = 0; m < rem; m++)
      {
         cand_ran_err_arr[m] = p_candidates[rem_base + m].range_error;
         cand_az_err_arr[m]  = p_candidates[rem_base + m].azimuth_error;
         cand_rr_diff_arr[m] = p_candidates[rem_base + m].rr_diff;
         cand_det_type[m]    = p_candidates[rem_base + m].detection_type;
      }

      v_cand_ran_err = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)cand_ran_err_arr, 0U);
      v_cand_az_err  = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)cand_az_err_arr, 0U);
      v_cand_rr_diff = BBE_LVN_2XF32_I((xb_vecN_2xf32 *)cand_rr_diff_arr, 0U);

      v_nr  = BBE_MULN_2XF32(v_cand_ran_err, v_sigma_r_inv);
      v_nr  = BBE_MOVN_2XF32T(v_nr, (xb_vecN_2xf32)(0.0F), v_sigma_r_gt_eps);
      v_naz = BBE_MULN_2XF32(v_cand_az_err, v_sigma_az_inv);
      v_naz = BBE_MOVN_2XF32T(v_naz, (xb_vecN_2xf32)(0.0F), v_sigma_az_gt_eps);
      v_nrr = BBE_MULN_2XF32(v_cand_rr_diff, v_sigma_rr_inv);
      v_nrr = BBE_MOVN_2XF32T(v_nrr, (xb_vecN_2xf32)(0.0F), v_sigma_rr_gt_eps);

      v_d = BBE_MULN_2XF32(v_nr, v_nr);
      BBE_MULAN_2XF32(v_d, v_naz, v_naz);
      BBE_MULAN_2XF32T(v_d, v_nrr, v_nrr, v_sigma_rr_gt_eps);
      v_d = BBE_SQRTN_2XF32(v_d);

      /* load detection type */

      BBE_LAVNX16U_XP(v_cand_det_type, BBE_LANX16U_PP(p_cand_det_type), p_cand_det_type, vec_width);
      v_cand_det_type  = BBE_SELNX16UI((v_cand_det_type >> 8U), v_cand_det_type, BBE_SELI_INTERLEAVE_1_LO) & vec_lo_8bit_mask_u;
      f_intersection_N = v_cand_det_type == (xb_vecNx16U)(RDU_S2_DET_TYPE_INTERSECTION);
      BBE_EXTRACTBN(v_temp1, f_intersection_N_2, f_intersection_N);
      v_q_type =
         BBE_MOVN_2XF32T((xb_vecN_2xf32)(1.0F), (xb_vecN_2xf32)(p_stage2_params->proximity_confidence_factor), f_intersection_N_2);

      v_margin = (xb_vecN_2xf32)(3.0F) - (v_d);
      v_arg    = BBE_MAXN_2XF32(BBE_MULN_2XF32(v_margin, v_q_type), (xb_vecN_2xf32)0.0F);
      //    	v_arg = BBE_MULN_2XF32(v_margin, v_q_type);

      v_arg = BBE_MULN_2XF32(v_arg, (xb_vecN_2xf32)(-0.5F));

      v_conf = (xb_vecN_2xf32)(0.5F) * bbe_vec_expf(v_arg);

      v_conf = BBE_MOVN_2XF32T((xb_vecN_2xf32)0.0F, v_conf, (v_conf < (xb_vecN_2xf32)(0.0F)));
      v_conf = BBE_MOVN_2XF32T((xb_vecN_2xf32)0.5F, v_conf, (v_conf > (xb_vecN_2xf32)(0.5F)));

      BBE_SVN_2XF32_XP(v_conf, p_conf_out, sizeof(float32_t) * rem);
   }
#else
   float32_t sigma_r  = p_stage2_params->range_prediction_threshold / 3.0f;
   float32_t sigma_az = p_stage2_params->angle_prediction_threshold_rad / 3.0f;
   float32_t sigma_rr = (rr_diff_norm_factor > RDU_S2_EPSILON) ? (rr_diff_norm_factor / 3.0f) : 0.0f;
   bool use_3d        = (sigma_rr > RDU_S2_EPSILON);

   uint16_t k;
   for (k = 0U; k < num_matches; k++)
   {
      const RDU_Stage2_Candidate_T *c = &p_candidates[k];

      float32_t nr  = (sigma_r > RDU_S2_EPSILON) ? (c->range_error / sigma_r) : 0.0f;
      float32_t naz = (sigma_az > RDU_S2_EPSILON) ? (c->azimuth_error / sigma_az) : 0.0f;
      float32_t d;

      if (use_3d)
      {
         float32_t nrr = c->rr_diff / sigma_rr;
         d             = rdu_sqrtf(nr * nr + naz * naz + nrr * nrr);
      }
      else
      {
         d = rdu_sqrtf(nr * nr + naz * naz); /* 2-D fallback – Gen8 single-VUA */
      }

      float32_t q_type =
         (c->detection_type == (uint8_t)RDU_S2_DET_TYPE_INTERSECTION) ? 1.0f : p_stage2_params->proximity_confidence_factor;

      float32_t margin = 3.0f - d;
      float32_t arg    = (margin > 0.0f) ? (margin * q_type) : 0.0f;
      float32_t conf   = 0.5f * rdu_expf(-0.5f * arg);

      /* Clamp to [0.0, 0.5] */
      if (conf < 0.0f)
      {
         conf = 0.0f;
      }
      else if (conf > 0.5f)
      {
         conf = 0.5f;
      }

      p_confidence_out[k] = conf;
   }
#endif
}

/******************************************************************************
 * Name:  RDU_Stage2_Calculate_Match_Quality_Score
 *
 * @brief Composite quality score for a single potential match entry.
 *
 * Score = type_penalty + position_error + abs_speed * velocity_weight
 *                      + rr_diff * rr_diff_weight
 * where:
 *   type_penalty   = 0 for INTERSECTION, intersection_bonus_weight for PROXIMITY
 *   position_error = sqrt(range_error² + (azimuth_error * range_current)²)
 *   rr_diff        = |rr_curr_unwrapped − rr_predicted|
 *
 * Mirrors MATLAB: calculate_match_quality_score.m
 ******************************************************************************/
float32_t RDU_Stage2_Calculate_Match_Quality_Score(uint8_t detection_type, float32_t range_error, float32_t azimuth_error,
                                                   float32_t range_current, float32_t abs_speed, float32_t rr_diff,
                                                   const RDU_Params_T *p_stage2_params)
{
   /* Type penalty: 0 for intersection, intersection_bonus_weight for proximity */
   float32_t type_penalty =
      (detection_type == (uint8_t)RDU_S2_DET_TYPE_INTERSECTION) ? 0.0f : p_stage2_params->intersection_bonus_weight;

   /* Euclidean position error (polar → Cartesian approximation) */
   float32_t lateral_err    = azimuth_error * range_current;
   float32_t position_error = rdu_sqrtf(range_error * range_error + lateral_err * lateral_err);

   return (type_penalty + position_error + (abs_speed * p_stage2_params->velocity_weight) +
           (rr_diff * p_stage2_params->rr_diff_weight));
}

/******************************************************************************
 * Name:  RDU_Stage2_Process
 *
 * @brief Main Stage 2 processing entry point.
 *
 * All data access uses the canonical Gen8 field names:
 *   Range       : p_rdu_data->p_det_data->af_data.ran[]
 *   Azimuth     : p_rdu_data->p_det_data->af_data.theta[]
 *   Range rate  : p_rdu_data->p_det_data->af_data.vel[]
 *   Num dets    : p_rdu_data->p_det_data->af_data.num_af_det
 *   Scan index  : p_rdu_data->p_det_data->det_list_property.scanindex
 *   Mot. status : p_rdu_data->p_rdu_output_data->rrr_motion_status[]
 *   Confidence  : p_rdu_data->p_rdu_output_data->classification_confidence[]
 *   Threshold T : p_rdu_internals->stage1_moving_det_thold[]
 *   vx_scs/vy_s : p_rdu_data->p_rdu_output_data->vx_scs / vy_scs
 *   VUA         : p_rdu_params->vua  (single-value, Gen8 – reused as-is)
 *   FOV         : p_rdu_params->perspective_angle  [degrees]
 ******************************************************************************/
bool RDU_Stage2_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_stage2_internals,
                        RDU_Buffer_T *p_rdu_buffer)
{
   bool is_success = true;

   /* Null check */
   if ((NULL == p_rdu_data) || (NULL == p_rdu_params) || (NULL == p_stage2_internals) || (NULL == p_rdu_buffer) ||
       (NULL == p_rdu_data->p_rdu_stage1_output_data) || (NULL == p_rdu_data->p_rdu_stage2_output_data))
   {
      is_success = false;
   }

   if (is_success)
   {
      RDU_Stage2_Output_Data_T *p_stage2_output_data = p_rdu_data->p_rdu_stage2_output_data;

      const RDU_Prev_Scan_Det_T *p_prev_scan_det_trig = &p_rdu_buffer->prev_scan_det_data;

      uint16_t match_i;

      /* Convenience pointers into current-scan data (all from Stage 1 outputs) */
      int32_t n32     = p_rdu_data->p_det_data->num_af_det;
      uint16_t n_curr = (n32 > 0) ? (uint16_t)n32 : 0U;

      /* Use pre-populated arrays from RDU_Internals (filled during init) */
      int8_t *ms_curr                        = p_rdu_data->p_rdu_stage1_output_data->stage1_rrr_motion_status;
      float32_t *p_classification_confidence = p_rdu_data->p_rdu_stage1_output_data->stage1_classification_confidence;
      float32_t vxs                          = p_rdu_data->p_rdu_stage1_output_data->stage1_vx_scs;
      float32_t vys                          = p_rdu_data->p_rdu_stage1_output_data->stage1_vy_scs;
      uint16_t scan_idx                      = p_rdu_data->p_det_data->det_list_property.scanindex;

      /* Stage2 metadata: always update current-scan info */
      p_stage2_output_data->stage2_scan_idx_curr_scan = scan_idx;

      /* Default previous-scan info (overwritten when prev scan is valid) */
      p_stage2_output_data->stage2_scan_idx_prev_scan = INVALID_SCAN_IDX;
      p_stage2_output_data->stage2_look_id_prev_scan  = INVALID_LOOK_ID;

      /* Sensor parameters (reused from Stage 1 without modification).
       * Gen8 currently carries a single VUA calibration value, so previous-scan
       * and current-scan VUA resolve to the same source until previous-scan VUA
       * is plumbed through the ping-pong data path. */
      float32_t fov_rad = RDU_S2_DEG2RAD(p_rdu_params->perspective_angle); /* rad */

      /* Step 0: Clear per-scan working memory and Stage 2 output fields */
      (void)RDU_Stage2_Internals_Clear(p_stage2_internals, p_stage2_output_data, n_curr);
      /* Seed Stage2 outputs from Stage1 every scan (inclusive baseline for Stage3) */
      for (uint16_t i = 0U; i < n_curr; i++)
      {
         p_stage2_output_data->stage2_rrr_motion_status[i]         = ms_curr[i];
         p_stage2_output_data->stage2_classification_confidence[i] = p_classification_confidence[i];
      }
      /* Process only when previous scan IPC ping-pong data is available.
       * The RDU_Buffer maintains the has_valid_prev_scan flag which tracks
       * whether a complete previous scan has been processed. */
      if (p_rdu_buffer->has_valid_prev_scan)
      {
         uint16_t prev_look = p_rdu_buffer->prev_scan_s2_mov_det.look_type;
         uint16_t curr_look = p_rdu_data->p_det_data->det_list_property.look_type;

         float32_t vua_current = p_rdu_params->vua[curr_look]; /* m/s */
         float32_t vua_prev    = p_rdu_params->vua[prev_look]; /* m/s */

         p_stage2_output_data->stage2_scan_idx_prev_scan = p_rdu_buffer->prev_scan_s2_mov_det.scan_idx;
         p_stage2_output_data->stage2_look_id_prev_scan  = prev_look;
         p_stage2_output_data->stage2_look_id_curr_scan  = curr_look;

         /* Step 1: Compute dt */
         int32_t scan_delta = (int32_t)scan_idx - (int32_t)p_rdu_buffer->prev_scan_s2_mov_det.scan_idx;
         float32_t dt       = (float32_t)scan_delta * p_rdu_params->radar_cycle_time_s;

         if (dt > RDU_S2_EPSILON) /* Guard against negative / zero dt */
         {
            /* Step 2: Mark AMBIGUOUS detections in current scan; count them and compute T_max
             * (mirrors MATLAB: T = max(samples_current_scan.T(curr_det_ambiguous_indices));
             *  entire matching block is gated on num_ambiguous > 0, same as MATLAB)         */
            float32_t T_max = 0.0f;
            uint16_t num_ambiguous =
               RDU_Stage2_Mark_Ambiguous_Detections(ms_curr, p_stage2_internals->stage1_moving_det_thold, n_curr, &T_max);
            uint16_t sorted_num_ambiguous = RDU_Stage2_Build_Sorted_Ambiguous(p_stage2_internals, n_curr);

            /* Gate entire matching on num_ambiguous > 0 – mirrors MATLAB:
             * "if s>1 && num_ambiguous > 0".  When no AMBIGUOUS dets exist
             * there is nothing to reclassify and T_max has no meaning.     */
            if ((num_ambiguous > 0U) && (sorted_num_ambiguous > 0U))
            {
               /* Step 3: Filter low-confidence MOVING dets from previous scan (IPC ping-pong).
                * Confidence threshold is capped at 0.5 (MATLAB: min(confidence_threshold, 0.5)). */
               float32_t conf_thresh = (p_rdu_params->confidence_threshold < 0.5f) ? p_rdu_params->confidence_threshold : 0.5f;

               int32_t n_prev     = p_rdu_buffer->prev_scan_det_data.num_prev_scan_dets;
               uint16_t num_valid = RDU_Stage2_Collect_Valid_Previous_Detections(
                  p_stage2_internals->valid_dets2test_idx, (uint16_t)MAX_NUM_DETECTS, n_prev, p_rdu_buffer, conf_thresh);
               p_stage2_internals->num_valid_dets2test = num_valid;

               /* RR diff acceptance threshold (MATLAB: find_and_score_matches.m):
                *   rr_diff_norm_factor = 0.5 * rr_diff_scaling_factor * |vua_curr - vua_prev|
                * For Gen8 single-VUA (vua_prev == vua_current), this is 0.0 and the RR
                * gate is skipped entirely (otherwise it would reject all matches).   */
               float32_t rr_diff_norm_factor = 0.5f * p_rdu_params->rr_diff_scaling_factor * rdu_absf(vua_current - vua_prev);
               bool apply_rr_gate            = (rr_diff_norm_factor > RDU_S2_EPSILON);

               /* Compute and cache the min/max predicted range bounds for this scan.
                * These depend only on the sorted range and prediction threshold, so they
                * are computed once here and reused across all K_mov/K_stat combinations. */
               RDU_Stage2_Compute_Range_Bounds(p_stage2_internals, p_rdu_params);

               /* Step 5: Main K × K matching loop */
               if (num_valid > 0U)
               {
#ifdef BBE_ENABLE
                  uint16_t prev_batch_start;

                  for (prev_batch_start = 0U; prev_batch_start < num_valid;
                       prev_batch_start = (uint16_t)(prev_batch_start + RDU_S2_VEC_WIDTH))
                  {
                     uint16_t batch_count =
                        (uint16_t)(((prev_batch_start + RDU_S2_VEC_WIDTH) <= num_valid) ? RDU_S2_VEC_WIDTH :
                                                                                          (num_valid - prev_batch_start));

                     RDU_Stage2_Process_Prev_Batch(
                        p_stage2_internals, p_prev_scan_det_trig, p_stage2_internals->valid_dets2test_idx, prev_batch_start,
                        batch_count, RDU_Stage2_Internal_Flags.stage2_sorted_theta,
                        RDU_Stage2_Internal_Flags.stage2_sorted_cos_theta, RDU_Stage2_Internal_Flags.stage2_sorted_sin_theta,
                        RDU_Stage2_Internal_Flags.stage2_sorted_range, RDU_Stage2_Internal_Flags.stage2_sorted_vel,
                        RDU_Stage2_Internal_Flags.stage2_sorted_was_ambiguous, sorted_num_ambiguous, dt, vxs, vys, vua_current,
                        vua_prev, rr_diff_norm_factor, apply_rr_gate, p_rdu_params, p_stage2_internals, fov_rad, T_max);
                  }
#else
                  uint16_t prev_idx;
                  uint16_t k_mov_i;
                  const float32_t *az_curr           = RDU_Stage2_Internal_Flags.stage2_sorted_theta;
                  const float32_t *cos_curr          = RDU_Stage2_Internal_Flags.stage2_sorted_cos_theta;
                  const float32_t *sin_curr          = RDU_Stage2_Internal_Flags.stage2_sorted_sin_theta;
                  const float32_t *r_curr            = RDU_Stage2_Internal_Flags.stage2_sorted_range;
                  const float32_t *rr_curr           = RDU_Stage2_Internal_Flags.stage2_sorted_vel;
                  const bool *p_sorted_was_ambiguous = RDU_Stage2_Internal_Flags.stage2_sorted_was_ambiguous;

                  float32_t min_predicted_range = r_curr[0U] - p_rdu_params->range_prediction_threshold;
                  float32_t max_predicted_range = r_curr[sorted_num_ambiguous - 1U] + p_rdu_params->range_prediction_threshold;

                  for (prev_idx = 0U; prev_idx < num_valid; prev_idx++)
                  {
                     /* Access raw previous-scan data directly via the IPC
                      * ping-pong pointer – no local copies maintained.       */
                     uint16_t prev_gbl_i = p_stage2_internals->valid_dets2test_idx[prev_idx];
                     float32_t r1_v      = p_rdu_buffer->prev_scan_det_data.prev_range[prev_gbl_i];
                     float32_t rr1_v     = p_rdu_buffer->prev_scan_det_data.prev_vel[prev_gbl_i];

                     for (k_mov_i = 0U; k_mov_i < p_rdu_params->num_K_moving; k_mov_i++)
                     {
                        int8_t K_mov            = p_rdu_params->K_values_moving[k_mov_i];
                        float32_t rr1_unwrapped = rr1_v + (float32_t)K_mov * vua_prev;
                        float32_t r2_predicted  = r1_v + rr1_unwrapped * dt;

                        bool within_range_limit = (r2_predicted >= min_predicted_range) && (r2_predicted <= max_predicted_range);

                        if (!within_range_limit)
                        {
                           continue;
                        }

                        int8_t K_stat = p_rdu_params->K_values_stationary[k_mov_i];

                        RDU_Stage2_Process_K_Stat(p_stage2_internals, az_curr, r_curr, cos_curr, sin_curr, rr_curr,
                                                  p_sorted_was_ambiguous, p_prev_scan_det_trig, prev_gbl_i, sorted_num_ambiguous,
                                                  dt, r1_v, r2_predicted, vxs, vys, vua_current, rr_diff_norm_factor,
                                                  apply_rr_gate, p_rdu_params, K_mov, K_stat, prev_idx, fov_rad, T_max,
                                                  rr1_unwrapped);

                     } /* end K_mov loop */
                  }    /* end prev_idx loop */
#endif

                  /* -- Step 6 (Phase 2): Best-match selection via two-pass quality-score
                   * deduplication.  Mirrors MATLAB: validateAndSelectBestMatches().
                   *
                   * Pass A – per current detection: for each curr_det_idx, keep only the
                   *          candidate with the lowest quality_score; eliminate the rest.
                   * Pass B – per previous detection: among Pass-A survivors, for each
                   *          prev_local_idx keep only the candidate with the lowest score.
                   * Together these enforce strict one-to-one matching.                      */

                  uint16_t num_cands = p_stage2_internals->num_candidates;

                  RDU_Stage2_Deduplicate_Candidates(p_stage2_internals->candidates, num_cands, false);
                  RDU_Stage2_Deduplicate_Candidates(p_stage2_internals->candidates, num_cands, true);

                  /* Apply final best matches: only is_valid == 1U entries */
                  for (match_i = 0U; match_i < num_cands; match_i++)
                  {
                     RDU_Stage2_Candidate_T *cand = &p_stage2_internals->candidates[match_i];
                     if (cand->is_valid == 0U)
                     {
                        continue;
                     }

                     uint16_t curr_idx = cand->curr_det_idx;

                     RDU_Stage2_Internal_Flags.is_updated_by_stage2[curr_idx] = true;
                  }

                  /* -- Step 6b: Compute Stage 2 classification confidence for each final
                   * match (mirrors MATLAB: compute_stage2_confidence.m).
                   * Collect is_valid candidates into a contiguous local array, call the
                   * confidence function, then scatter results back to the IPC output.   */
                  {
                     RDU_Stage2_Candidate_T valid_cands[RDU_S2_MAX_POTENTIAL_MATCHES];
                     float32_t conf_vals[RDU_S2_MAX_POTENTIAL_MATCHES];
                     uint16_t n_valid = 0U;
                     // float32_t *p_classification_confidence = p_rdu_data->p_rdu_output_data->classification_confidence;

                     for (match_i = 0U; match_i < num_cands; match_i++)
                     {
                        if (p_stage2_internals->candidates[match_i].is_valid != 0U)
                        {
                           valid_cands[n_valid] = p_stage2_internals->candidates[match_i];
                           n_valid++;
                        }
                     }
                     /* all potential matches*/
                     p_stage2_output_data->stage2_potential_matches_count = num_cands;
                     /* To send count of all potential matches identified including one-to-many matches if any*/

                     RDU_Stage2_Compute_Confidence(conf_vals, valid_cands, n_valid, rr_diff_norm_factor, p_rdu_params);

                     for (uint16_t vi = 0U; vi < n_valid; vi++)
                     {
                        uint16_t curr_idx        = valid_cands[vi].curr_det_idx;
                        uint16_t prev_idx_global = p_stage2_internals->valid_dets2test_idx[valid_cands[vi].prev_local_idx];

                        /* per-match arrays: index by vi */
                        p_stage2_output_data->stage2_curr_stat_det_idx[vi]     = curr_idx;
                        p_stage2_output_data->stage2_prev_mov_det_idx[vi]      = prev_idx_global;
                        p_stage2_output_data->stage2_prev_mov_det_k_values[vi] = valid_cands[vi].K_moving_value;
                        p_stage2_output_data->stage2_prev_mov_det_type[vi]     = valid_cands[vi].detection_type;
                        /*Update Confidence value*/
                        p_stage2_output_data->stage2_classification_confidence[curr_idx] = conf_vals[vi];
                     }
                  }

               } /* end if (num_valid > 0) */

            } /* end if (num_ambiguous > 0) */

            /* -- Step 7: Update motion status and finalise all Stage 2 output flags --
             * Runs unconditionally (mirrors MATLAB – motion status update is outside
             * the num_ambiguous guard; no-op when no was_ambiguous[] flags are set). */
            (void)RDU_Stage2_Update_Motion_Status_Vec(p_rdu_data, p_rdu_params, p_stage2_output_data, n_curr, p_stage2_internals);

         } /* end if (dt > epsilon) */
      }
   }
   return is_success;
}

/* END OF FILE -------------------------------------------------------------- */
