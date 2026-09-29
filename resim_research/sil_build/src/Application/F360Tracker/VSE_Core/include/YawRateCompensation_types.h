//
// File: YawRateCompensation_types.h
//
// Code generated for Simulink model 'YawRateCompensation'.
//
// Model version                  : 1.143
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:27 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 64-bit (LP64)
// Code generation objectives:
//    1. RAM efficiency
//    2. ROM efficiency
//    3. Safety precaution
//    4. Execution efficiency
//    5. MISRA C:2012 guidelines
//    6. Traceability
//    7. Debugging
// Validation result: Not run
//
#ifndef RTW_HEADER_YawRateCompensation_types_h_
#define RTW_HEADER_YawRateCompensation_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_enum_quality_factor_T_
#define DEFINED_TYPEDEF_FOR_enum_quality_factor_T_

typedef uint8_T enum_quality_factor_T;

// enum enum_quality_factor_T
#define UNDEFINED                      ((enum_quality_factor_T)0U) // Default value 
#define TEMP_UNDEFINED                 ((enum_quality_factor_T)1U)
#define NOT_ACCURATED                  ((enum_quality_factor_T)2U)
#define ACCURATED                      ((enum_quality_factor_T)3U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_YC_INPUT_T_
#define DEFINED_TYPEDEF_FOR_YC_INPUT_T_

typedef struct {
  real32_T Yaw_Rate_Raw;
  enum_quality_factor_T Yaw_Rate_Raw_QF;
  real32_T Yaw_Rate_Reference;
  enum_quality_factor_T Yaw_Rate_Reference_QF;
  real32_T System_Vehicle_Velocity;
  enum_quality_factor_T System_Vehicle_Velocity_QF;
  boolean_T f_Vehicle_Stationary;
  uint64_T curr_run_time;
} YC_INPUT_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_road_type_T_
#define DEFINED_TYPEDEF_FOR_enum_road_type_T_

typedef int32_T enum_road_type_T;

// enum enum_road_type_T
#define UNKNOWN_ROAD                   (0)                       // Default value 
#define STRAIGHT_ROAD                  (1)
#define CURVED_ROAD                    (2)
#define INTERMEDIATE_ROAD              (3)
#endif

#ifndef DEFINED_TYPEDEF_FOR_YC_INTERNAL_RESIM_T_
#define DEFINED_TYPEDEF_FOR_YC_INTERNAL_RESIM_T_

typedef struct {
  real32_T yaw_rate_bias1;
  real32_T yaw_rate_bias2;
  real32_T yaw_rate_bias_fast_bias1;
  real32_T yaw_rate_bias_fast_bias2;
  real32_T comp_yaw_rate_diff_filt;
  real32_T yaw_rate_bias_diff;
  enum_road_type_T road_type;
  boolean_T f_yaw_rate_bias_shift;
  boolean_T f_Yaw_Rate_Bias_Converged;
  boolean_T f_stop_bias_converged;
  boolean_T f_yaw_rate_steady;
  boolean_T f_input_invalid_persistent;
  boolean_T f_execution_period_error_persistent;
  boolean_T f_bias_was_accurate;
  real32_T ignition_time;
  boolean_T f_yaw_stop_bias_converged;
} YC_INTERNAL_RESIM_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_YC_OUTPUT_T_
#define DEFINED_TYPEDEF_FOR_YC_OUTPUT_T_

typedef struct {
  real32_T Yaw_Rate_Compensated_Unfiltered;
  real32_T Yaw_Rate_Compensated_Filtered;
  real32_T Yaw_Rate_Raw_Bias;
  enum_quality_factor_T Yaw_Rate_Compensated_QF;
  enum_quality_factor_T Yaw_Rate_Bias_QF;
} YC_OUTPUT_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_yaw_rate_bias_accuracy_T_
#define DEFINED_TYPEDEF_FOR_enum_yaw_rate_bias_accuracy_T_

typedef int32_T enum_yaw_rate_bias_accuracy_T;

// enum enum_yaw_rate_bias_accuracy_T
#define BIAS_UNDEFINED                 (0)                       // Default value 
#define BIAS_NOT_ACCURATE              (1)
#define BIAS_ACCURATE                  (2)
#endif

#ifndef DEFINED_TYPEDEF_FOR_YC_INTERNAL_T_
#define DEFINED_TYPEDEF_FOR_YC_INTERNAL_T_

typedef struct {
  real32_T yaw_rate_error;
  real32_T yaw_rate_bias1;
  real32_T yaw_rate_bias2;
  real32_T ignition_time;
  real32_T yaw_rate_ref_bias_time;
  real32_T yaw_rate_ref_bias;
  real32_T yaw_rate_steady_fast;
  real32_T yaw_rate_steady_slow;
  real32_T yaw_rate_steady_timer;
  boolean_T f_yaw_rate_steady;
  boolean_T f_valid_stop_yaw_rate;
  boolean_T f_stop_bias_converged;
  real32_T yaw_rate_bias_stop_inc_step;
  boolean_T f_yaw_rate_bias_shift;
  real32_T filt_raw_curvature;
  real32_T filt_ref_curvature;
  real32_T raw_curvature_bias;
  real32_T ref_curvature_bias;
  enum_road_type_T road_type;
  real32_T yaw_rate_select;
  real32_T yaw_rate_filtered;
  real32_T yaw_rate_ref_filtered;
  boolean_T f_vehicle_stop;
  real32_T comp_yaw_rate_difference;
  real32_T comp_yaw_rate_diff_filt;
  real32_T yaw_rate_bias_fast_bias1;
  real32_T yaw_rate_bias_fast_bias2;
  real32_T yaw_rate_bias_fast;
  enum_yaw_rate_bias_accuracy_T bias_accuracy;
  boolean_T f_bias_was_accurate;
  real32_T yaw_rate_bias_diff;
  boolean_T f_Yaw_Rate_Bias_Converged;
  real32_T yaw_rate_delta_ABS;
  boolean_T f_execution_period_error;
  boolean_T f_execution_period_error_persistent;
  boolean_T f_execution_period_error_start;
  real32_T exec_error_sustain_period;
  uint64_T prev_run_time;
  uint64_T execution_period_error_start_time;
  boolean_T f_input_invalid;
  boolean_T f_input_invalid_persistent;
  boolean_T f_input_invalid_start;
  real32_T input_invalid_sustain_period;
  uint64_T input_invalid_start_time;
  boolean_T f_Enable_Fast_Yaw_Bias_Filter;
} YC_INTERNAL_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_YC_CALS_T_
#define DEFINED_TYPEDEF_FOR_YC_CALS_T_

typedef struct {
  real32_T k_yaw_rate_time_constant;
  real32_T k_yaw_rate_bias_time_constant;
  real32_T k_yaw_rate_cal_time_const_stop;
  real32_T k_yaw_rate_cal_time_const_stop_fast;
  real32_T k_yaw_rate_cal_time_const_stop_slow;
  real32_T k_yaw_rate_steady_fast_time_const;
  real32_T k_yaw_rate_steady_slow_time_const;
  real32_T k_yaw_rate_steady_slower_time_const;
  real32_T k_yaw_rate_delta_steady_max;
  real32_T k_yaw_rate_steady_time_min;
  real32_T k_curv_fast_tc;
  real32_T k_curv_road_straight_ref_tc;
  real32_T k_curv_road_unknown_ref_tc;
  real32_T k_yaw_rate_reference_bias_tc;
  real32_T k_curv_road_unknown_tc;
  real32_T k_yaw_rate_ref_bias_stable_time;
  real32_T k_curv_road_straight_tc;
  real32_T k_curv_road_unknown;
  boolean_T k_yaw_rate_bias_ref_enable;
  real32_T k_enable_fast_yaw_bias_time;
  real32_T k_Yaw_Washout_Max_Rate;
  real32_T k_Yaw_Rate_Washout_Max_Rate_2;
  real32_T k_yaw_rate_washout_time_constant;
  real32_T k_Yaw_Rate_Washout_Time_Constant_Fast;
  real32_T k_yaw_rate_washout_time_constant_stop;
  real32_T k_Yaw_Rate_Washout_Time_Constant_Straight;
  real32_T k_ref_yaw_rate_washout_time_constant;
  real32_T k_ref_yaw_rate_washout_time_constant_straight;
  real32_T k_ref_yaw_rate_washout_time_constant_fast;
  boolean_T k_yaw_rate_bias_stop_enable;
  real32_T k_yaw_bias_enable_speed;
  boolean_T k_yaw_rate_bias_remove;
  real32_T k_curv_bias_high;
  real32_T k_curv_bias_low;
  real32_T k_yaw_rate_bias_high;
  real32_T k_curv_speed_min;
  real32_T k_yaw_rate_ref_max;
  real32_T k_yaw_rate_bias_threshold;
  real32_T k_curv_yaw_max;
  real32_T k_yaw_rate_error_max;
  real32_T k_curv_road_turn;
  real32_T k_curv_road_straight;
  boolean_T k_yaw_rate_bias2_enabled;
  real32_T k_yaw_rate_washout_min_speed;
  boolean_T C_use_veh_stationary;
  real32_T k_Yaw_Rate_Processing_Period;
  real32_T k_YAW_RATE_DELTA_ABS_LIMIT;
  real32_T k_MAX_YAW_RATE_BIAS;
  real32_T k_MIN_YAW_RATE_BIAS;
  real32_T k_EPSILON_ZERO_VEH_SPEED;
  real32_T k_MAX_YAW_RATE_REF_BIAS_TIME;
  real32_T k_MIN_CURVATURE_BIAS;
  real32_T k_MAX_CURVATURE_BIAS;
  real32_T k_MIN_FILT_RAW_CURVATURE;
  real32_T k_MAX_FILT_RAW_CURVATURE;
  real32_T k_yaw_rate_cal_stop_bias_inc;
  real32_T k_yaw_rate_cal_stop_bias_inc_slow;
  real32_T k_yaw_rate_cal_bias_step;
  real32_T k_calibration_version_main;
  real32_T k_calibration_version_sub;
  real32_T k_discard_invalid_sensor_data_time;
  real32_T k_stop_valid_yaw_rate_max_diff;
  boolean_T C_use_ref_yaw;
  real32_T k_comp_and_ref_diff_low;
  real32_T k_comp_and_ref_diff_mid;
  real32_T k_max_comp_and_ref_diff;
  real32_T k_comp_and_ref_diff_transition;
  real32_T k_max_yaw_comp_threshold;
  real32_T k_min_ref_yaw_speed_threshold;
  real32_T k_comp_and_ref_diff_time_constant;
  real32_T k_yaw_rate_bias_diff_drift_low;
  real32_T k_yaw_rate_bias_diff_drift_mid;
  real32_T k_max_yaw_rate_bias_diff_drift;
  real32_T k_yaw_rate_bias_diff_qf_mid;
  real32_T k_yaw_rate_bias_diff_qf_high;
  real32_T k_max_yaw_rate_bias_diff_qf;
  real32_T k_max_yaw_rate_raw_input;
  real32_T k_min_yaw_rate_raw_input;
  enum_quality_factor_T k_max_yaw_rate_qf_input;
  enum_quality_factor_T k_min_yaw_rate_qf_input;
  real32_T k_max_yaw_rate_ref_input;
  real32_T k_min_yaw_rate_ref_input;
  enum_quality_factor_T k_max_yaw_rate_ref_qf_input;
  enum_quality_factor_T k_min_yaw_rate_ref_qf_input;
  real32_T k_max_veh_vel_input;
  real32_T k_min_veh_vel_input;
  boolean_T k_max_f_veh_stationary;
  boolean_T k_min_f_veh_stationary;
  real32_T k_max_input_invalid_duration;
  real32_T k_max_execution_period_error_range;
  real32_T k_max_execution_period_error_sustain;
} YC_CALS_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_YC_PARAMS_T_
#define DEFINED_TYPEDEF_FOR_YC_PARAMS_T_

typedef struct {
  boolean_T f_gain_calculated;
  real32_T system_yaw_rate_gain;
  real32_T yaw_rate_bias_gain;
  real32_T yaw_rate_error_gain;
  real32_T yaw_rate_error_gain_fast;
  real32_T yaw_rate_error_gain_slow;
  real32_T yaw_rate_washout_gain;
  real32_T yaw_rate_washout_fast_gain;
  real32_T yaw_rate_washout_stop_gain;
  real32_T yaw_rate_washout_straight_gain;
  real32_T ref_yaw_rate_washout_straight_gain;
  real32_T ref_yaw_rate_washout_gain;
  real32_T ref_yaw_rate_washout_fast_gain;
  real32_T yaw_steady_fast_gain;
  real32_T yaw_steady_slow_gain;
  real32_T yaw_steady_slower_gain;
  real32_T curv_fast_gain;
  real32_T curv_road_straight_ref_gain;
  real32_T curv_road_unknown_ref_gain;
  real32_T yaw_rate_reference_bias_gain;
  real32_T curv_road_unknown_gain;
  real32_T curv_road_straight_gain;
  real32_T filt_raw_curvature;
  real32_T filt_raw_curvature_ref;
  real32_T comp_yaw_diff_gain;
} YC_PARAMS_T;

#endif

// Custom Type definition for MATLAB Function: '<S4>/MATLAB Function'
#ifndef struct_tag_sCra9MzdSEdpPTjYUGHpkSD
#define struct_tag_sCra9MzdSEdpPTjYUGHpkSD

struct tag_sCra9MzdSEdpPTjYUGHpkSD
{
  real32_T Yaw_Rate_Raw;
  enum_quality_factor_T Yaw_Rate_Raw_QF;
  real32_T Yaw_Rate_Reference;
  enum_quality_factor_T Yaw_Rate_Reference_QF;
  real32_T System_Vehicle_Velocity;
  enum_quality_factor_T System_Vehicle_Velocity_QF;
  boolean_T f_Vehicle_Stationary;
  real32_T curr_run_time;
};

#endif                                 //struct_tag_sCra9MzdSEdpPTjYUGHpkSD

#ifndef typedef_sCra9MzdSEdpPTjYUGHpkSD_YawRateCompensation_T
#define typedef_sCra9MzdSEdpPTjYUGHpkSD_YawRateCompensation_T

typedef struct tag_sCra9MzdSEdpPTjYUGHpkSD
  sCra9MzdSEdpPTjYUGHpkSD_YawRateCompensation_T;

#endif                                 //typedef_sCra9MzdSEdpPTjYUGHpkSD_YawRateCompensation_T
#endif                                 // RTW_HEADER_YawRateCompensation_types_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
