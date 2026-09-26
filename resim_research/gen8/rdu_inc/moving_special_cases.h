#ifndef MOVING_SPECIAL_CASES_H
   #define MOVING_SPECIAL_CASES_H
   /*===========================================================================*/
   /**
    * @file moving_special_cases.h
    * @brief Stage 2: Moving Special Cases Detection API
    *
    * This module identifies ambiguous detections from Stage 1
    * (Stationary_Moving_Classifier) that represent moving targets folding onto
    * the stationary velocity profile. It uses a 2-scan detection association
    * approach with analytical intersection/proximity detection.
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Aptiv Sensitive Business – Restricted Aptiv information. Do not disclose
    **/
   /*==========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/
   #include "rdu_math.h"
   #include <math.h>
   #include <stdbool.h>
   #include <stdint.h>
   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "doppler_unfolding.h" /* RDU_Data_T, RDU_Params_T, RDU_MOTION_STATUS_*,
                                  MAX_NUM_DETECTS, RDU_K_MIN/MAX, float32_t   */

   #ifdef __cplusplus
extern "C"
{
   #endif /* __cplusplus */

   /*===========================================================================*
    * Exported Preprocessor #define Constants
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/

   /* -------------------------------------------------------------------------
    * Stage 2 Algorithm Parameters
    * Size: 44 bytes. One-time allocation, reused across all scans.
    * -------------------------------------------------------------------------*/

   /* -------------------------------------------------------------------------
    * Stage 2 Internal Working Memory
    *
    * Memory breakdown (MAX_NUM_DETECTS = 512, RDU_S2_MAX_POTENTIAL_MATCHES = 16):
    *   Filtered-det index list  :  ~1.0 KB  (512 × 2 bytes + counter)
    *   Phase 1 candidate buffer :    384 B  (16 × 24 bytes + counter)
    *   TOTAL                    :  ~1.4 KB  (all temporary per scan)
    *
    * Previous-scan raw detection data (range, azimuth, range-rate) is read
    * directly from the RDU_Buffer via p_rdu_buffer->prev_scan_det_data
    * when p_rdu_buffer->has_valid_prev_scan is true.
    * Output / classification flags are in RDU_Stage2_Output_temp_Data_T, not here.
    * This mirrors Stage 1 practice: RDU_Internals_T holds only derived
    * computation state, never raw input copies or module outputs.
    * -------------------------------------------------------------------------*/
   /* ========== Stage 2 Internal Working Memory ========== */
   /* These fields are NOT logged in the output stream. They are used
      internally during RDU_Stage2_Process() for candidate tracking and
      motion-status classification. Reused across scans. */
   typedef struct RDU_Stage2_Internal_Flags_Tag
   {
      bool was_ambiguous[MAX_NUM_DETECTS] __attribute__((aligned(16)));
      bool is_updated_by_stage2[MAX_NUM_DETECTS] __attribute__((aligned(16)));
      bool is_stationary_updated[MAX_NUM_DETECTS] __attribute__((aligned(16)));
      bool could_be_zero_rr[MAX_NUM_DETECTS] __attribute__((aligned(16)));
      float32_t stage2_sorted_range[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t stage2_sorted_vel[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t stage2_sorted_theta[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t stage2_sorted_cos_theta[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t stage2_sorted_sin_theta[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      bool stage2_sorted_was_ambiguous[MAX_NUM_DETECTS] __attribute__((aligned(16)));
      uint16_t stage2_sorted_original_idx[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t min_predicted_range; /**< Min range for current scan predictions */
      float32_t max_predicted_range; /**< Max range for current scan predictions */
      uint16_t stage2_num_sorted_ambiguous;
   } RDU_Stage2_Internal_Flags_T;

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/

   /**
    * @brief  Clear per-scan Stage 2 working memory and output fields.
    *
    * Resets all per-scan temporary fields in p_stage2_internals and all
    * per-detection output fields in p_stage2_output.  Previous-scan data is
    * managed externally via the IPC ping-pong buffer.
    *
    * Called automatically by RDU_Stage2_Process() at the start of each scan.
    *
    * @param[out] p_stage2_internals  Non-NULL pointer to Stage 2 working memory.
    * @param[out] p_stage2_output     Non-NULL pointer to Stage 2 output struct.
    * @param[in]  num_detections      Number of current-scan detections
    *                                 (bounds the per-detection clear loops).
    *
    * @return true  Clear successful.
    * @return false Either pointer is NULL.
    */
   bool RDU_Stage2_Internals_Clear(RDU_Internals_T *p_stage2_internals, RDU_Stage2_Output_Data_T *p_stage2_output_data,
                                   uint16_t num_detections);

   /**
    * @brief  Main Stage 2 processing entry point.
    *
    * Operates on the current scan data in p_rdu_data.  Previous-scan data is
    * taken from the persistent cache inside p_stage2_internals (populated
    * automatically at the end of each successful call).
    *
    * On the very first call (prev_scan_valid == false) the function skips all
    * matching and only populates the cache.
    *
    *
    * @param[in,out] p_rdu_data          Current-scan RDU data (motion_status updated).
    * @param[in]     p_rdu_params        Stage 1 parameters (vua, perspective_angle, …).
    * @param[in]     p_stage2_params     Stage 2 tuning parameters.
    * @param[in,out] p_stage2_internals  Working memory + inter-scan cache.
    *
    * @pre   Stage 1 (Stationary_Moving_Classifier_Process) has completed for the
    *        current scan – rrr_motion_status, classification_confidence,
    *        moving_det_thold, vx_scs, vy_scs in p_rdu_output_data are valid.
    * @post  p_rdu_data->p_rdu_output_data->rrr_motion_status[] updated.
    * @post  All Stage 2 output fields populated in p_rdu_data->p_stage2_output_data.
    *
    * @return true  Processing completed (including first-scan cache-only case).
    * @return false Any input pointer is NULL.
    */
   bool RDU_Stage2_Process(RDU_Data_T *p_rdu_data, RDU_Params_T *p_rdu_params, RDU_Internals_T *p_stage2_internals,
                           RDU_Buffer_T *p_rdu_buffer);

   /**
    * @brief  Analytical intersection between a predicted moving trajectory and a
    *         K_stat-shifted stationary Doppler profile.
    *
    * Solves A·cos(θ₂) + B·sin(θ₂) = C using R·cos(θ₂ − φ) = C.
    * Returns up to 2 solutions within the sensor FOV.
    *
    * @param[in]  r1              Previous-scan range                  [m]
    * @param[in]  theta1          Previous-scan azimuth                [rad]
    * @param[in]  r2              Predicted current-scan range         [m]
    * @param[in]  dt              Scan-to-scan time interval           [s]
    * @param[in]  vxs             Sensor velocity X (SCS)             [m/s]
    * @param[in]  vys             Sensor velocity Y (SCS)             [m/s]
    * @param[in]  K_stat          Stationary profile wrapping factor   [–]
    * @param[in]  vua_current     Velocity unambiguity, current scan   [m/s]
    * @param[in]  fov_rad         Half field-of-view                   [rad]
    * @param[out] p_solutions     Array of at least 2 floats for results [rad]
    * @param[out] p_num_solutions Number of valid solutions (0, 1, or 2).
    *
    * @return true   At least one solution is within the FOV.
    * @return false  No intersection, or degenerate geometry (R ≈ 0).
    */
   bool RDU_Stage2_Find_Intersection(float32_t r1, float32_t r2, float32_t dt, float32_t vxs, float32_t vys, int8_t K_stat,
                                     float32_t vua_current, const RDU_Prev_Scan_Det_T *p_prev_scan_det, uint16_t prev_det_idx,
                                     float32_t fov_rad, float32_t *p_solutions, uint8_t *p_num_solutions);

   /**
    * @brief  Find the azimuth of minimum distance between the moving trajectory
    *         and the stationary Doppler profile (proximity case).
    *
    * Uses the derivative condition d/dθ[rr₂(θ) − rr_ref(θ)] = 0, which is
    * independent of K_stat (the constant shift does not move the minimum
    * location).  The caller computes the shifted distance for the specific K_stat.
    *
    * @param[in]  r1                 Previous-scan range            [m]
    * @param[in]  theta1             Previous-scan azimuth          [rad]
    * @param[in]  r2                 Predicted current-scan range   [m]
    * @param[in]  dt                 Scan-to-scan time interval     [s]
    * @param[in]  vxs                Sensor velocity X (SCS)       [m/s]
    * @param[in]  vys                Sensor velocity Y (SCS)       [m/s]
    * @param[in]  fov_rad            Half field-of-view             [rad]
    * @param[out] p_closest_azimuth  Azimuth of minimum distance    [rad]
    * @param[out] p_min_distance     Distance at that azimuth (K_stat=0) [m/s]
    *
    * @return true   Valid minimum found within FOV.
    * @return false  Degenerate geometry or minimum outside FOV.
    */
   bool RDU_Stage2_Find_Minimum_Distance(float32_t r1, float32_t r2, float32_t dt, float32_t vxs, float32_t vys,
                                         const RDU_Prev_Scan_Det_T *p_prev_scan_det, uint16_t prev_det_idx, float32_t fov_rad,
                                         float32_t *p_closest_azimuth, float32_t *p_min_distance);

   /**
    * @brief  Check whether the absolute velocity of a matched target pair is
    *         within the maximum realistic limit.
    *
    * Converts relative (sensor-frame) position change to absolute world velocity
    * by adding the sensor's own velocity vector.
    *
    * @param[in]  r1            Previous-scan range                [m]
    * @param[in]  theta1        Previous-scan azimuth              [rad]
    * @param[in]  r2_actual     Current-scan range (matched)       [m]
    * @param[in]  theta2_actual Current-scan azimuth (matched)     [rad]
    * @param[in]  dt            Scan-to-scan time interval         [s]
    * @param[in]  vxs           Sensor velocity X (SCS)           [m/s]
    * @param[in]  vys           Sensor velocity Y (SCS)           [m/s]
    * @param[in]  max_velocity  Maximum realistic speed threshold  [m/s]
    * @param[out] p_abs_speed   Computed absolute speed magnitude  [m/s]
    *
    * @return true   Absolute speed ≤ max_velocity.
    * @return false  Absolute speed > max_velocity.
    */
   bool RDU_Stage2_Check_Absolute_Velocity(float32_t r1, float32_t r2_actual, float32_t dt, float32_t vxs, float32_t vys,
                                           float32_t max_velocity, const RDU_Internals_T *p_rdu_internals,
                                           const RDU_Prev_Scan_Det_T *p_prev_scan_det, uint16_t prev_det_idx,
                                           uint16_t curr_det_idx, float32_t *p_abs_speed);

   /**
    * @brief  Compute a composite quality score for a single potential match.
    *
    * Lower score indicates a better match.  Priority hierarchy:
    *  1. Intersection (type 1) beats Proximity (type 2) via intersection_bonus_weight penalty.
    *  2. Minimise Euclidean position error (polar approximation: sqrt(Δr² + (Δθ·r)²)).
    *  3. Minimise absolute velocity (fine-tuning via velocity_weight).
    *  4. Minimise RR prediction error (via rr_diff_weight).
    *
    * Mirrors MATLAB: calculate_match_quality_score.m
    *
    * @param[in] detection_type     RDU_S2_DET_TYPE_INTERSECTION (1) or _PROXIMITY (2).
    * @param[in] range_error        |r_predicted − r_actual|                    [m].
    * @param[in] azimuth_error      |θ_predicted − θ_actual|                    [rad].
    * @param[in] range_current      Actual current-scan range (for arc → m conversion) [m].
    * @param[in] abs_speed          Absolute target speed magnitude               [m/s].
    * @param[in] rr_diff            |rr_curr_unwrapped − rr_predicted|           [m/s].
    * @param[in] p_stage2_params    Stage 2 parameters (weights).
    *
    * @return  Composite quality score (≥ 0).  Lower is better.
    */
   float32_t RDU_Stage2_Calculate_Match_Quality_Score(uint8_t detection_type, float32_t range_error, float32_t azimuth_error,
                                                      float32_t range_current, float32_t abs_speed, float32_t rr_diff,
                                                      const RDU_Params_T *p_stage2_params);

   /**
    * @brief  Compute Stage 2 classification confidence for matched detections.
    *
    * Uses Mahalanobis distance in 3D prediction error space (range, azimuth, RR):
    *   d = sqrt(e_r²/σ_r² + e_θ²/σ_θ² + e_rr²/σ_rr²)
    * with σ = threshold/3 (3-sigma interpretation, consistent with Stage 1).
    *
    * When rr_diff_norm_factor = 0 (Gen8 single-VUA: vwrap_prev == vwrap_curr),
    * falls back to 2D (range + azimuth only).
    *
    * Confidence shape (mirrored exponential, same as Stage 1 moving branch):
    *   confidence = 0.5 × exp(−0.5 × max(0, 3−d) × q_type)
    * where q_type = 1.0 for intersection, proximity_confidence_factor for proximity.
    *
    * Output range: [0.5×exp(−1.5), 0.5]  (intersection)
    *               [0.5×exp(−1.5×p), 0.5] (proximity, p = proximity_confidence_factor)
    *
    * Mirrors MATLAB: compute_stage2_confidence.m
    *
    * @param[out] p_confidence_out      Array to receive confidence values (length num_matches).
    * @param[in]  p_candidates          Validated candidate entries (is_valid == 1U only).
    * @param[in]  num_matches           Number of valid entries.
    * @param[in]  rr_diff_norm_factor   0.5 × scaling × |vwrap_curr − vwrap_prev|. 0 for Gen8.
    * @param[in]  p_stage2_params       Stage 2 parameters.
    */
   void RDU_Stage2_Compute_Confidence(float32_t *p_confidence_out, const RDU_Stage2_Candidate_T *p_candidates,
                                      uint16_t num_matches, float32_t rr_diff_norm_factor, const RDU_Params_T *p_stage2_params);

   /**
    * @brief  Update rrr_motion_status for AMBIGUOUS detections and finalise output flags.
    *
    * Decision logic (applied only to detections where was_ambiguous == true):
    *  - is_updated_by_stage2 == true                     → MOVING_SPECIAL (3)
    *  - is_updated_by_stage2 == false
    *      && enable_zero_rr_check && |rr| ≤ T[i]         → unchanged AMBIGUOUS (2);
    *                                                         could_be_zero_rr set true
    *  - is_updated_by_stage2 == false (all other cases)  → STATIONARY (0);
    *                                                         is_stationary_updated set true
    *
    * When enable_zero_rr_check is false (default), ALL unmatched AMBIGUOUS
    * detections are classified STATIONARY and is_stationary_updated is set.
    *
    * MOVING (1) and INVALID (−1) detections are never modified.
    *
    * @param[in,out] p_rdu_data       RDU data (rrr_motion_status updated in-place).
    * @param[in]     p_stage2_params  Stage 2 parameters (enable_zero_rr_check flag).
    * @param[in,out] p_stage2_output  Stage 2 output struct (could_be_zero_rr and
    *                                 is_stationary_updated written here).
    * @param[in]     num_detections   Number of detections to process.
    *
    * @return true   Update completed.
    * @return false  Any pointer is NULL.
    */
   bool RDU_Stage2_Update_Motion_Status(RDU_Data_T *p_rdu_data, const RDU_Params_T *p_stage2_params,
                                        RDU_Stage2_Output_Data_T *p_rdu_stage2_output_data, uint16_t num_detections,
                                        RDU_Internals_T *p_rdu_internals);

   /*===========================================================================*
    * Exported Inline Function Definitions
    *===========================================================================*/

   /** @brief π as a float32 literal – avoids double-promotion and M_PI portability issues */
   #ifndef M_PI_F
      #define M_PI_F (3.14159265358979323846f)
   #endif

   /**
    * @brief  Wrap an angle to [−π, +π].
    * @param  angle  Input angle in radians.
    * @return Angle normalised to [−π, +π].
    */
   static inline float32_t RDU_Stage2_Normalize_Angle(float32_t angle)
   {
      /* fmodf can return a negative result when the dividend is negative.
       * Adding 2π before the modulo and clamping ensures the intermediate
       * value is non-negative before the final shift into (−π, +π].      */
      float32_t r = fmodf(angle + M_PI_F, 2.0f * M_PI_F);
      if (r < 0.0f)
      {
         r += 2.0f * M_PI_F;
      }
      return r - M_PI_F;
   }

   #ifdef __cplusplus
} /* extern "C" */
   #endif /* __cplusplus */

/** @} doxygen end group rdu_stage2 */
#endif /* MOVING_SPECIAL_CASES_H */
/* END OF FILE -------------------------------------------------------------- */
