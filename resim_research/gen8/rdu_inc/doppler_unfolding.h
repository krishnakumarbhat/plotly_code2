#ifndef DOPPLER_UNFOLDING_H
#define DOPPLER_UNFOLDING_H
/*===========================================================================*/
/**
 * @file doppler_unfolding.h
 * @brief Doppler unfolding public API for the RDU module.
 *
 * This header declares the public types and functions used by the Doppler
 * unfolding stage in the RDU processing pipeline.
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
 *
 * @section ABBR ABBREVIATIONS:
 *   -
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
 *
 * @defgroup template Provide API description and define/delete next line
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <stdint.h>
/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "ipc_data.h"
#include "mem_pool.h"
#include "radar_look_types.h"

#ifdef __cplusplus
extern "C"
{      /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */

   /*===========================================================================*
    * Exported Preprocessor #define Constants
    *===========================================================================*/

#define RDU_K_MIN                (-2)
#define RDU_K_MAX                (2)
#define RDU_K_RANGE              (RDU_K_MAX - RDU_K_MIN + 1)
#define MAX_NUM_DETECTS          (AF_MAX_NUM_DET)
#define RDU_MAX_K_NEIGHBOURS     (3U) /* Maximum Neighbours in Cartesian KNN association algorithm */
#define RDU_MAX_RADAR_SENSOR_POS (14U)
   /*===========================================================================*
    * Exported Preprocessor #define MACROS
    *===========================================================================*/

/* RDU Motion status - represented in signed Int8 */
#define RDU_MOTION_STATUS_INVALID           (-1)
#define RDU_MOTION_STATUS_STATIONARY        (0)
#define RDU_MOTION_STATUS_MOVING            (1)
#define RDU_MOTION_STATUS_AMBIGUOUS         (2)
#define RDU_MOTION_STATUS_MOVING_SPECIAL    (3)
#define RDU_MOTION_STATUS_MOVING_ASSOCIATED (4)

/** @brief Memory pool slots: [0] RDU_Params_T, [1] RDU_Internals_T,
 *                            [2] RDU_Stage2_Params_T, [3] RDU_Stage2_Internals_T */
#define RDU_MP_MAX_NUM_MEMORY_LOC (4U) /**< Maximum number of RDU memory locations */

/*Stage 2 Macro*/
#define RDU_S2_MAX_POTENTIAL_MATCHES (16U)
/** @brief Number of K values tested for moving detection unwrapping */
#define RDU_S2_NUM_K_MOVING (9U)

/** @brief Number of K values tested for stationary profile shifting */
#define RDU_S2_NUM_K_STATIONARY (9U)

/** @brief Detection type: trajectory intersects with shifted stationary profile */
#define RDU_S2_DET_TYPE_INTERSECTION (1U)

/** @brief Detection type: trajectory in proximity to shifted stationary profile */
#define RDU_S2_DET_TYPE_PROXIMITY (2U)

/**
 * @brief Default per-call candidate cap (max_close_detections parameter default).
 * @note  Matches MATLAB default_paras.m: paras.max_close_detections = 1
 *        (strict one-to-one matching: find_and_score_matches returns at most
 *         1 match per prev_idx, making Pass B deduplication redundant).
 */
#define RDU_S2_DEFAULT_MAX_CLOSE_DETECTIONS (1U)

/**
 * @brief Hard cap on total candidate entries in the Phase 1 flat buffer.
 *
 * Set to 16 to bound pool memory use to 16 × 24 = 384 bytes.
 * Phase 1 stops appending once this limit is reached; the top-N scoring
 * per call (max_close_detections=1) still applies within each individual call.
 */

/** @brief Numerical guard for near-zero denominators / degenerate geometry */
#define RDU_S2_EPSILON (1.0e-10f)

/*Invalid values use when no prev scan info is available*/
#define INVALID_SCAN_IDX (UINT16_MAX)
#define INVALID_LOOK_ID  ((uint8_t)0xFFU)
   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   typedef struct RDU_Internal_Mem_Info_Tag
   {
      void **addr_ptr;
      uint32_t size;
   } RDU_Internal_Mem_Info_T;

   typedef enum RDU_Status_Tag
   {
      RDU_STATUS_PRE_INIT     = -1,
      RDU_STATUS_INIT_FAILURE = 0,
      RDU_STATUS_INIT_SUCCESS = 1,
      RDU_STATUS_S1_SUCCESS   = 2,
      RDU_STATUS_S1_FAILURE   = 3,
      RDU_STATUS_PF_NO_ERROR  = 4,
      RDU_STATUS_S2_SUCCESS   = 5, /* Stage 1 + Stage 2 both completed successfully */
      RDU_STATUS_S2_FAILURE   = 6, /* Stage 1 ok; Stage 2 failed */
      RDU_STATUS_S3_SUCCESS   = 7,
      RDU_STATUS_S3_FAILURE   = 8,
   } RDU_Status_T;

   typedef struct Host_Motion_Vector_Tag
   {
      float32_t host_yaw_rate;
      float32_t host_velocity;
      float32_t host_sideslip;
   } Host_Motion_Vector_T;

   typedef struct Speed_in_SCS_Tag
   {
      float32_t vx;
      float32_t vy;
   } Speed_in_SCS_T;

   typedef struct Sensor_Mounting_Tag
   {
      uint8_t sensor_mount_loc;
      float32_t iso_sensor_x_posn;
      float32_t iso_sensor_y_posn;
      float32_t iso_sensor_z_posn;
      float32_t iso_sensor_ornt_roll;
      float32_t iso_sensor_ornt_yaw;
      float32_t iso_sensor_ornt_pitch;
      float32_t sensor_height;
      float32_t sensor_polarity;
   } Sensor_Mounting_T;

   typedef struct Cartesian_Knn_Output_Tag
   {
      float32_t cur_rr;                                 /* Range Rate of Current moving det */
      float32_t cur_az_deg;                             /* Azimuth angle of Current moving det */
      float32_t prev_scan_rr[RDU_MAX_K_NEIGHBOURS];     /* Range Rate of Previous scan moving dets */
      float32_t prev_scan_az_deg[RDU_MAX_K_NEIGHBOURS]; /* Azimuth angle of Previous scan moving dets */
      float32_t euclidean_dist[RDU_MAX_K_NEIGHBOURS];   /* Euclidean distance between Current and Previous scan moving dets */
      uint16_t prev_scan_detidx[RDU_MAX_K_NEIGHBOURS];  /* Indices of Previous scan moving dets */
      uint16_t cur_scan_detidx;                         /* Index of Current scan moving det */
      int8_t num_neighbours;                            /* Number of Neighbours identified */
      bool f_matched; /* Flag to indicate if at least one neighbour is within the euclidean distance threshold */
   } Cart_Knn_Output_T;

   typedef struct RDU_Stage2_Candidate_Tag
   {
      uint16_t prev_local_idx;
      uint16_t prev_det_idx;
      uint16_t curr_det_idx;
      uint8_t detection_type;
      int8_t K_moving_value;
      uint8_t is_valid;
      uint8_t _pad;
      float32_t quality_score;
      float32_t abs_speed;
      float32_t range_error;
      float32_t azimuth_error;
      float32_t rr_diff;
   } RDU_Stage2_Candidate_T;

   typedef struct RDU_Data_Tag
   {
      Detection_Stream_T *p_det_data;
      Vse_Stream_T *p_vse_data;
      uint8_t radar_position;
      RDU_Stage3_Output_Data_T *p_rdu_stage3_output_data;
      RDU_Stage2_Output_Data_T *p_rdu_stage2_output_data;
      RDU_Stage1_Output_Data_T *p_rdu_stage1_output_data;
      RDU_Timing_Data_T *p_rdu_timing_data;
   } RDU_Data_T;

   typedef struct RDU_Params_Tag
   {
      Sensor_Mounting_T sensor_mounting;
      float32_t __attribute__((aligned(32))) m_transform_matrix[2][3];
      float32_t __attribute__((aligned(32))) host_motion_covariance_matrix[3][3];
      float32_t __attribute__((aligned(32))) sensor_motion_covariance_matrix[2][2];
      float32_t sensor_rot_matrix[3][3];
      float32_t vel_correction_threshold;
      float32_t std_theta;
      float32_t std_phi;
      float32_t std_range_rate;
      float32_t var_theta;
      float32_t var_phi;
      float32_t var_range_rate;
      float32_t min_range_rate;
      float32_t __attribute__((aligned(32))) vua[4];
      float32_t tikh_vx;
      float32_t tikh_vy;
      bool enable_delta_velocity_correction;
      float32_t perspective_angle;       /**< Field of view angle [degrees] */
      float32_t minimum_range_threshold; /**< Minimum range for valid detections [m] */
      int32_t k_vua;
      /*stage 2 parameters*/
      /* K-value sets for dual-wrapping search (reuse RDU_K_MIN/MAX convention) */
      int8_t K_values_moving[RDU_S2_NUM_K_MOVING];
      int8_t K_values_stationary[RDU_S2_NUM_K_STATIONARY];
      uint8_t num_K_moving;     /**< Active entries in K_values_moving    */
      uint8_t num_K_stationary; /**< Active entries in K_values_stationary */

      /* Matching thresholds */
      float32_t max_realistic_velocity;         /**< Max absolute target speed [m/s]  – default 45.0  */
      float32_t range_prediction_threshold;     /**< Range match window [m]           – default 0.25  */
      float32_t angle_prediction_threshold_rad; /**< Azimuth match window [rad]       – default 0.004363 (0.25°) */
      float32_t confidence_threshold;           /**< Stage 1 confidence below which a MOVING det. is eligible – default 0.3 */

      /* Timing */
      float32_t radar_cycle_time_s; /**< Nominal scan-to-scan interval [s] – default 0.05 */

      /* Phase 2 best-match selection quality score weights */
      float32_t intersection_bonus_weight; /**< Quality-score penalty for proximity vs intersection match
                                                  (lower score = better); default 1000.0                          */
      float32_t velocity_weight;           /**< Weight of absolute velocity term in quality score;
                                                  default 0.1                                                      */
      float32_t rr_diff_weight;            /**< Weight of RR prediction error in quality score [1/(m/s)];
                                                  default 1.0 (MATLAB default_paras.m)                            */
      float32_t rr_diff_scaling_factor;    /**< Scaling factor for RR diff acceptance threshold.
                                                  rr_diff_norm_factor = 0.5 * rr_diff_scaling_factor
                                                    * |vwrap_curr − vwrap_prev|
                                                  Default 0.8 (MATLAB default_paras.m).
                                                  For Gen8 single-VUA: norm_factor = 0, gate skipped.            */

      /* Stage 2 confidence computation */
      float32_t proximity_confidence_factor; /**< Confidence scaling for proximity (type 2) vs intersection
                                                    (type 1) matches. Applied as q_type in Mahalanobis distance.
                                                    default 0.6 (MATLAB default_paras.m)                            */

      /* Near-zero range-rate handling */
      bool enable_zero_rr_check; /**< When true, AMBIGUOUS dets with |rr| ≤ T stay AMBIGUOUS
                                        instead of being reclassified STATIONARY; default false          */
      /* Per-candidate match count cap */
      uint8_t max_close_detections; /**< Maximum current-scan matches per (prev, K_mov, K_stat) call.
                                           Default 1 (MATLAB) – strict one-to-one matching;
                                           makes Pass B deduplication redundant.
                                           Total candidates capped at RDU_S2_MAX_POTENTIAL_MATCHES (16). */

      /* Within-interval K_stat constraint (mirrors MATLAB compute_K_ranges.m) */
      bool use_within_interval_constraint; /**< When true, only test K_stat values where
                                                  |K_stat − K_mov| ≤ max_within_interval.
                                                  For Gen8 (single VUA, vua_prev == vua_current)
                                                  auto-computed interval is 0, reducing 25
                                                  K-combinations to 5.  default: true               */
      uint8_t max_within_interval;         /**< Maximum |K_stat − K_mov| to test.
                                                  MATLAB (compute_K_ranges.m, use_auto_K_computation=true):
                                                    max_within_interval = max(|K_stat_grid − K_mov_grid|)
                                                  For Gen8 single-VUA (vwrapping_prev==vwrapping_current):
                                                    K_mov_grid == K_stat_grid  ⇒  auto-computed value = 0.
                                                  The fallback value of 3 in default_paras.m applies only
                                                  when use_auto_K_computation=false (multi-VUA sensors).
                                                  Gen8 embedded value: 0U (= MATLAB auto-computed result). */
      float32_t vwrapping_mps[NUM_LOOKS];
      float32_t rwrapping_m[NUM_LOOKS];
      float32_t minimum_rangerate_mps;
      float32_t min_rdot_mps;
      float32_t max_rdot_mps;
      float32_t max_eps_range_m;
      float32_t hi_conf;
      float32_t euclidean_max_dist_m;
      uint8_t num_neighbors;
   } RDU_Params_T;

   typedef struct RDU_Internals_Tag
   {
      Host_Motion_Vector_T host_motion_vector;
      Speed_in_SCS_T speed_in_scs;
      Speed_in_SCS_T v_xy_delta;
      float32_t __attribute__((aligned(32))) cur_range[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) cur_vel[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) cur_theta[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) cur_phi[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) cos_theta[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) sin_theta[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) reference_rr[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) reference_rr_mod[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) stage1_moving_det_thold[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) moving_det_thold_temp[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) reference_rr_temp[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) reference_rr_mod_temp[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) range_rate_diff[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) range_rate_diff_temp[MAX_NUM_DETECTS];
      float32_t __attribute__((aligned(32))) best_obj[MAX_NUM_DETECTS]; /* stage 3 Best obj of all neighbours */
      float32_t __attribute__((aligned(32))) stage1_unamb_range_rate[MAX_NUM_DETECTS];
      /* Stage3 debug/aux output kept in internals (output tag no longer owns eps_range) */
      float32_t __attribute__((aligned(32))) stage3_eps_range[MAX_NUM_DETECTS];
      bool valid_det_flag[MAX_NUM_DETECTS]; /* flag to determine that range and az are within limits */
      bool f_stationary_det[MAX_NUM_DETECTS];
      bool f_stationary_det_temp[MAX_NUM_DETECTS];
      uint16_t num_stationary_dets; /*added here as ouput struct dont logg it anymore*/
      /* Stage1 internal-only flag */
      bool f_vel_corrected;
      uint16_t valid_dets2test_idx[MAX_NUM_DETECTS];                   /**< Global indices of valid detections to test */
      uint16_t num_valid_dets2test;                                    /**< Number of valid entries               */
      RDU_Stage2_Candidate_T candidates[RDU_S2_MAX_POTENTIAL_MATCHES]; /**< Phase 1 candidates  */
      uint16_t num_candidates;                                         /**< Active entries in candidates[]                 */
      Cart_Knn_Output_T cart_knn_output[MAX_NUM_DETECTS];              /* S3 - Disambiguation internal variables */
   } RDU_Internals_T;

   typedef struct RDU_S2_Output_Tag
   {
      float32_t range_rate_unamb[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      ;
      int8_t rrr_motion_status[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      ;
      int8_t K_factor[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      ;
      bool is_moving[MAX_NUM_DETECTS];
   } RDU_S2_Output_T;

   typedef struct RDU_S1_Output_Tag
   {
      float32_t range_rate_unamb[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t confidence[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      int8_t rrr_motion_status[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      int8_t K_factor[MAX_NUM_DETECTS] __attribute__((aligned(32)));
   } RDU_S1_Output_T;

   typedef struct RDU_S2_Mov_Det_Tag
   {
      float32_t det_pos_x[MAX_NUM_DETECTS] __attribute__((aligned(32))); /* VCS X position [m] */
      float32_t det_pos_y[MAX_NUM_DETECTS] __attribute__((aligned(32))); /* VCS Y position [m] */
      float32_t det_pos_z[MAX_NUM_DETECTS] __attribute__((aligned(32))); /* VCS Z position [m] */
      uint16_t moving_det_idx[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      uint16_t num_moving_dets;
      uint16_t scan_idx;
      uint8_t look_type;
   } RDU_S2_Mov_Det_T;

   typedef struct RDU_Prev_Scan_Det_Tag
   {
      float32_t prev_range[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t prev_vel[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t prev_theta[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t prev_phi[MAX_NUM_DETECTS] __attribute__((aligned(32)));
      float32_t prev_cos_az[MAX_NUM_DETECTS] __attribute__((aligned(32))); /* cos(azimuth) of prev scan detections */
      float32_t prev_sin_az[MAX_NUM_DETECTS] __attribute__((aligned(32))); /* sin(azimuth) of prev scan detections */
      float32_t prev_cos_el[MAX_NUM_DETECTS] __attribute__((aligned(32))); /*  cos(elevation) of prev scan detections */
      float32_t prev_sin_el[MAX_NUM_DETECTS] __attribute__((aligned(32))); /*  sin(elevation) of prev scan detections */
      uint16_t num_prev_scan_dets;
   } RDU_Prev_Scan_Det_T;

   typedef struct RDU_Buffer_Tag
   {
      RDU_Prev_Scan_Det_T prev_scan_det_data;   /* Required detection data from previous scan */
      RDU_S1_Output_T prev_scan_s1_output;      /* Stage 1 output from previous scan */
      RDU_S1_Output_T present_scan_s1_output;   /* Stage 1 output from current scan (populated from IPC) */
      RDU_S2_Output_T prev_scan_s2_output;      /* Stage 2 output from previous scan */
      RDU_S2_Output_T present_scan_s2_output;   /* Stage 2 output from current scan (populated from IPC) */
      RDU_S2_Mov_Det_T prev_scan_s2_mov_det;    /* Stage 2 moving detection output from previous scan */
      RDU_S2_Mov_Det_T present_scan_s2_mov_det; /* Stage 2 moving detection output from current scan (populated from IPC) */
      Host_Motion_Vector_T prev_scan_host_motion_vector; /* Host motion vector from previous scan */
      bool has_valid_prev_scan;                          /* true once at least one scan has been processed into buffer */
   } RDU_Buffer_T;

   /*===========================================================================*
    * Exported Const Object Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/
   Memory_Pool_Return_T RDU_Handler_Memory_Init(Mem_Pool_T *mem_pool);
   RDU_Status_T Doppler_Unfolding_Init(RDU_Data_T *p_rdu_input_data, D2M_Msg_T *p_d2m_data, M2D_Msg_T *p_m2d_data,
                                       uint8_t radar_posn);
   RDU_Status_T Doppler_Unfolding_Process(RDU_Data_T *p_rdu_data);
   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/** @} doxygen end group */
#endif /* DOPPLER_UNFOLDING_H */
