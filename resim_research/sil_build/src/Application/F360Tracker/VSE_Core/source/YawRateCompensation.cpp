//
// File: YawRateCompensation.cpp
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
#include "YawRateCompensation.h"
#include "YawRateCompensation_private.h"

// Named constants for Chart: '<S1>/Chart'
#define YawRateCompensation_IN_Initialize ((uint8_T)1U)
#define YawRateCompensation_IN_Running ((uint8_T)2U)

// Function for MATLAB Function: '<S1>/YawRateCompensation'
void YawRateCompensationModelClass::YawRateCompensation_determineGain(const
  YC_CALS_T *YC_Cals, YC_PARAMS_T *YC_Params)
{
  if (!YC_Params->f_gain_calculated) {
    YC_Params->system_yaw_rate_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period + YC_Cals->k_yaw_rate_time_constant);
    YC_Params->yaw_rate_bias_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_bias_time_constant);
    YC_Params->yaw_rate_error_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_cal_time_const_stop);
    YC_Params->yaw_rate_error_gain_fast = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_cal_time_const_stop_fast);
    YC_Params->yaw_rate_error_gain_slow = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_cal_time_const_stop_slow);
    YC_Params->yaw_rate_washout_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_washout_time_constant);
    YC_Params->yaw_rate_washout_fast_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_Yaw_Rate_Washout_Time_Constant_Fast);
    YC_Params->yaw_rate_washout_stop_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_washout_time_constant_stop);
    YC_Params->yaw_rate_washout_straight_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_Yaw_Rate_Washout_Time_Constant_Straight);
    YC_Params->ref_yaw_rate_washout_gain = YC_Cals->k_Yaw_Rate_Processing_Period
      / (YC_Cals->k_Yaw_Rate_Processing_Period +
         YC_Cals->k_ref_yaw_rate_washout_time_constant);
    YC_Params->ref_yaw_rate_washout_straight_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_ref_yaw_rate_washout_time_constant_straight);
    YC_Params->ref_yaw_rate_washout_fast_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_ref_yaw_rate_washout_time_constant_fast);
    YC_Params->yaw_steady_fast_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_steady_fast_time_const);
    YC_Params->yaw_steady_slow_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_steady_slow_time_const);
    YC_Params->yaw_steady_slower_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_steady_slower_time_const);
    YC_Params->curv_fast_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period + YC_Cals->k_curv_fast_tc);
    YC_Params->curv_road_straight_ref_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_curv_road_straight_ref_tc);
    YC_Params->curv_road_unknown_ref_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_curv_road_unknown_ref_tc);
    YC_Params->yaw_rate_reference_bias_gain =
      YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_yaw_rate_reference_bias_tc);
    YC_Params->curv_road_unknown_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period + YC_Cals->k_curv_road_unknown_tc);
    YC_Params->curv_road_straight_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period + YC_Cals->k_curv_road_straight_tc);
    YC_Params->filt_raw_curvature = YC_Cals->k_curv_road_unknown;
    YC_Params->filt_raw_curvature_ref = YC_Cals->k_curv_road_unknown;
    YC_Params->comp_yaw_diff_gain = YC_Cals->k_Yaw_Rate_Processing_Period /
      (YC_Cals->k_Yaw_Rate_Processing_Period +
       YC_Cals->k_comp_and_ref_diff_time_constant);
    YC_Params->f_gain_calculated = true;
  }
}

// Function for MATLAB Function: '<S1>/YawRateCompensation'
void YawRateCompensationModelClass::
  YawRateCompensation_determineYawRateInputValid(const YC_INPUT_T YC_input,
  YC_INTERNAL_T *YC_internal, const YC_CALS_T *YC_Cals)
{
  real32_T period_tmp;
  if (((((((((((((YC_input.Yaw_Rate_Raw < YC_Cals->k_min_yaw_rate_raw_input) ||
                 (YC_input.Yaw_Rate_Raw > YC_Cals->k_max_yaw_rate_raw_input)) ||
                (YC_input.Yaw_Rate_Raw_QF < YC_Cals->k_min_yaw_rate_qf_input)) ||
               (YC_input.Yaw_Rate_Raw_QF > YC_Cals->k_max_yaw_rate_qf_input)) ||
              ((YC_Cals->C_use_ref_yaw) && (YC_input.Yaw_Rate_Reference <
                YC_Cals->k_min_yaw_rate_ref_input))) || ((YC_Cals->C_use_ref_yaw)
              && (YC_input.Yaw_Rate_Reference >
                  YC_Cals->k_max_yaw_rate_ref_input))) ||
            ((YC_Cals->C_use_ref_yaw) && (YC_input.Yaw_Rate_Reference_QF <
              YC_Cals->k_min_yaw_rate_ref_qf_input))) ||
           ((YC_Cals->C_use_ref_yaw) && (YC_input.Yaw_Rate_Reference_QF >
             YC_Cals->k_max_yaw_rate_ref_qf_input))) || (((int32_T)
            (YC_input.f_Vehicle_Stationary ? 1 : 0)) < ((int32_T)
            (YC_Cals->k_min_f_veh_stationary ? 1 : 0)))) || (((int32_T)
           (YC_input.f_Vehicle_Stationary ? 1 : 0)) > ((int32_T)
           (YC_Cals->k_max_f_veh_stationary ? 1 : 0)))) ||
        (YC_input.System_Vehicle_Velocity > YC_Cals->k_max_veh_vel_input)) ||
       (YC_input.System_Vehicle_Velocity < YC_Cals->k_min_veh_vel_input)) ||
      (((uint32_T)YC_input.System_Vehicle_Velocity_QF) != ACCURATED)) {
    YC_internal->f_input_invalid = true;
    if (!YC_internal->f_input_invalid_start) {
      YC_internal->f_input_invalid_start = true;
      YC_internal->input_invalid_start_time = YC_input.curr_run_time;
      YC_internal->input_invalid_sustain_period = 0.0F;
    }

    if (!YC_internal->f_input_invalid_persistent) {
      period_tmp = ((real32_T)((uint64_T)(YC_input.curr_run_time -
        YC_internal->input_invalid_start_time))) * 0.001F;
      YC_internal->input_invalid_sustain_period = period_tmp;
      if ((period_tmp < 0.0F) || (period_tmp >
           YC_Cals->k_max_input_invalid_duration)) {
        YC_internal->f_input_invalid_persistent = true;
      }
    }
  } else {
    YC_internal->f_input_invalid = false;
    YC_internal->f_input_invalid_persistent = false;
    YC_internal->f_input_invalid_start = false;
    YC_internal->input_invalid_sustain_period = 0.0F;
    YC_internal->input_invalid_start_time = 0UL;
  }
}

// Function for MATLAB Function: '<S4>/MATLAB Function1'
void YawRateCompensationModelClass::YawRateCompensation_calsInit(YC_CALS_T
  *YC_Cals, YC_PARAMS_T *YC_Params)
{
  YC_Cals->k_calibration_version_main = 3.0F;
  YC_Cals->k_calibration_version_sub = 5.0F;
  YC_Cals->C_use_veh_stationary = true;
  YC_Cals->C_use_ref_yaw = true;
  YC_Cals->k_yaw_rate_bias_ref_enable = false;
  YC_Cals->k_Yaw_Rate_Processing_Period = 0.01F;
  YC_Cals->k_discard_invalid_sensor_data_time = 0.0F;
  YC_Cals->k_yaw_rate_time_constant = 0.05F;
  YC_Cals->k_yaw_rate_bias_time_constant = 0.297F;
  YC_Cals->k_yaw_rate_cal_time_const_stop = 0.3F;
  YC_Cals->k_yaw_rate_cal_time_const_stop_fast = 0.1F;
  YC_Cals->k_yaw_rate_cal_time_const_stop_slow = 0.8F;
  YC_Cals->k_yaw_rate_steady_fast_time_const = 0.15F;
  YC_Cals->k_yaw_rate_steady_slow_time_const = 0.3F;
  YC_Cals->k_yaw_rate_steady_slower_time_const = 4.5F;
  YC_Cals->k_yaw_rate_delta_steady_max = 0.00174520072F;
  YC_Cals->k_yaw_rate_steady_time_min = 0.5F;
  YC_Cals->k_curv_fast_tc = 5.0F;
  YC_Cals->k_curv_road_straight_ref_tc = 200.0F;
  YC_Cals->k_curv_road_unknown_ref_tc = 400.0F;
  YC_Cals->k_yaw_rate_reference_bias_tc = 2.0F;
  YC_Cals->k_curv_road_unknown_tc = 600.0F;
  YC_Cals->k_yaw_rate_ref_bias_stable_time = 20.0F;
  YC_Cals->k_curv_road_straight_tc = 300.0F;
  YC_Cals->k_curv_road_unknown = 0.000349040143F;
  YC_Cals->k_enable_fast_yaw_bias_time = 600.0F;
  YC_Cals->k_Yaw_Washout_Max_Rate = 0.0261780098F;
  YC_Cals->k_Yaw_Rate_Washout_Max_Rate_2 = 0.0174520072F;
  YC_Cals->k_yaw_rate_washout_time_constant = 600.0F;
  YC_Cals->k_Yaw_Rate_Washout_Time_Constant_Fast = 150.0F;
  YC_Cals->k_yaw_rate_washout_time_constant_stop = 5.0F;
  YC_Cals->k_yaw_rate_bias_stop_enable = true;
  YC_Cals->k_yaw_bias_enable_speed = 3.0F;
  YC_Cals->k_yaw_rate_bias_remove = false;
  YC_Cals->k_curv_bias_high = 0.000523560215F;
  YC_Cals->k_curv_bias_low = 8.72600358E-5F;
  YC_Cals->k_yaw_rate_bias_high = 0.00349040143F;
  YC_Cals->k_curv_speed_min = 7.0F;
  YC_Cals->k_yaw_rate_ref_max = 0.104712039F;
  YC_Cals->k_yaw_rate_bias_threshold = 0.00174520072F;
  YC_Cals->k_curv_yaw_max = 0.0523560196F;
  YC_Cals->k_yaw_rate_error_max = 0.0610820241F;
  YC_Cals->k_curv_road_turn = 0.000523560215F;
  YC_Cals->k_curv_road_straight = 0.000174520072F;
  YC_Cals->k_yaw_rate_bias2_enabled = true;
  YC_Cals->k_yaw_rate_washout_min_speed = 10.0F;
  YC_Cals->k_Yaw_Rate_Washout_Time_Constant_Straight = 300.0F;
  YC_Cals->k_YAW_RATE_DELTA_ABS_LIMIT = 0.174520075F;
  YC_Cals->k_MAX_YAW_RATE_BIAS = 0.0610820241F;
  YC_Cals->k_MIN_YAW_RATE_BIAS = -0.0610820241F;
  YC_Cals->k_EPSILON_ZERO_VEH_SPEED = 0.001F;
  YC_Cals->k_MAX_YAW_RATE_REF_BIAS_TIME = 60.0F;
  YC_Cals->k_MIN_CURVATURE_BIAS = -0.00261780107F;
  YC_Cals->k_MAX_CURVATURE_BIAS = 0.00261780107F;
  YC_Cals->k_MIN_FILT_RAW_CURVATURE = -0.00261780107F;
  YC_Cals->k_MAX_FILT_RAW_CURVATURE = 0.00261780107F;
  YC_Cals->k_yaw_rate_cal_stop_bias_inc = 0.000872600358F;
  YC_Cals->k_yaw_rate_cal_stop_bias_inc_slow = 0.000174520072F;
  YC_Cals->k_yaw_rate_cal_bias_step = 0.00174520072F;
  YC_Cals->k_stop_valid_yaw_rate_max_diff = 0.00523560215F;
  YC_Cals->k_comp_and_ref_diff_low = 0.0218150094F;
  YC_Cals->k_comp_and_ref_diff_mid = 0.0261780098F;
  YC_Cals->k_max_comp_and_ref_diff = 0.0349040143F;
  YC_Cals->k_comp_and_ref_diff_transition = 0.000872600358F;
  YC_Cals->k_max_yaw_comp_threshold = 0.0349040143F;
  YC_Cals->k_min_ref_yaw_speed_threshold = 10.0F;
  YC_Cals->k_comp_and_ref_diff_time_constant = 30.0F;
  YC_Cals->k_yaw_rate_bias_diff_drift_low = 0.00174520072F;
  YC_Cals->k_yaw_rate_bias_diff_drift_mid = 0.00261780107F;
  YC_Cals->k_max_yaw_rate_bias_diff_drift = 0.00436300179F;
  YC_Cals->k_yaw_rate_bias_diff_qf_mid = 0.00261780107F;
  YC_Cals->k_yaw_rate_bias_diff_qf_high = 0.00349040143F;
  YC_Cals->k_max_yaw_rate_bias_diff_qf = 0.00610820251F;
  YC_Cals->k_ref_yaw_rate_washout_time_constant = 150.0F;
  YC_Cals->k_ref_yaw_rate_washout_time_constant_straight = 100.0F;
  YC_Cals->k_ref_yaw_rate_washout_time_constant_fast = 75.0F;
  YC_Cals->k_max_yaw_rate_raw_input = 1.75F;
  YC_Cals->k_min_yaw_rate_raw_input = -1.75F;
  YC_Cals->k_max_yaw_rate_qf_input = ACCURATED;
  YC_Cals->k_min_yaw_rate_qf_input = UNDEFINED;
  YC_Cals->k_max_yaw_rate_ref_input = 1.75F;
  YC_Cals->k_min_yaw_rate_ref_input = -1.75F;
  YC_Cals->k_max_yaw_rate_ref_qf_input = ACCURATED;
  YC_Cals->k_min_yaw_rate_ref_qf_input = UNDEFINED;
  YC_Cals->k_max_veh_vel_input = 100.0F;
  YC_Cals->k_min_veh_vel_input = -20.0F;
  YC_Cals->k_max_f_veh_stationary = true;
  YC_Cals->k_min_f_veh_stationary = false;
  YC_Cals->k_max_input_invalid_duration = 10000.0F;
  YC_Cals->k_max_execution_period_error_range = 0.2F;
  YC_Cals->k_max_execution_period_error_sustain = 10000.0F;
  YC_Params->f_gain_calculated = false;
  YC_Params->system_yaw_rate_gain = 0.0F;
  YC_Params->yaw_rate_bias_gain = 0.0F;
  YC_Params->yaw_rate_error_gain = 0.0F;
  YC_Params->yaw_rate_error_gain_fast = 0.0F;
  YC_Params->yaw_rate_error_gain_slow = 0.0F;
  YC_Params->yaw_rate_washout_gain = 0.0F;
  YC_Params->yaw_rate_washout_fast_gain = 0.0F;
  YC_Params->yaw_rate_washout_stop_gain = 0.0F;
  YC_Params->ref_yaw_rate_washout_gain = 0.0F;
  YC_Params->ref_yaw_rate_washout_straight_gain = 0.0F;
  YC_Params->ref_yaw_rate_washout_fast_gain = 0.0F;
  YC_Params->yaw_steady_fast_gain = 0.0F;
  YC_Params->yaw_steady_slow_gain = 0.0F;
  YC_Params->yaw_steady_slower_gain = 0.0F;
  YC_Params->curv_fast_gain = 0.0F;
  YC_Params->curv_road_straight_ref_gain = 0.0F;
  YC_Params->curv_road_unknown_ref_gain = 0.0F;
  YC_Params->yaw_rate_reference_bias_gain = 0.0F;
  YC_Params->curv_road_unknown_gain = 0.0F;
  YC_Params->curv_road_straight_gain = 0.0F;
  YC_Params->yaw_rate_washout_straight_gain = 0.0F;
  YC_Params->filt_raw_curvature = 0.0F;
  YC_Params->filt_raw_curvature_ref = 0.0F;
}

// Function for MATLAB Function: '<S1>/YawRateCompensation'
void YawRateCompensationModelClass::
  YawRateCompensation_determineExecutionPeriodValid(uint64_T curr_run_time,
  YC_INTERNAL_T *YC_internal, const YC_CALS_T *YC_Cals)
{
  real32_T exec_duration;
  real32_T period_tmp;
  exec_duration = ((real32_T)((uint64_T)(curr_run_time -
    YC_internal->prev_run_time))) * 0.001F;
  period_tmp = YC_Cals->k_Yaw_Rate_Processing_Period * 1000.0F;
  if ((fabsf(exec_duration - period_tmp) / period_tmp) >
      YC_Cals->k_max_execution_period_error_range) {
    if (!YC_internal->f_execution_period_error_start) {
      YC_internal->execution_period_error_start_time = curr_run_time;
      YC_internal->f_execution_period_error_start = true;
      YC_internal->exec_error_sustain_period = 0.0F;
    }

    if (!YC_internal->f_execution_period_error_persistent) {
      period_tmp = ((real32_T)((uint64_T)(curr_run_time -
        YC_internal->execution_period_error_start_time))) * 0.001F;
      YC_internal->exec_error_sustain_period = period_tmp;
      if (((period_tmp < 0.0F) || (exec_duration < 0.0F)) || (period_tmp >
           YC_Cals->k_max_execution_period_error_sustain)) {
        YC_internal->f_execution_period_error_persistent = true;
      }
    }
  } else {
    YC_internal->f_execution_period_error = false;
    YC_internal->f_execution_period_error_persistent = false;
    YC_internal->f_execution_period_error_start = false;
    YC_internal->execution_period_error_start_time = 0UL;
    YC_internal->exec_error_sustain_period = 0.0F;
  }
}

// Function for MATLAB Function: '<S1>/YawRateCompensation'
void YawRateCompensationModelClass::YawRateCompensation_determineYawRateSteady(
  const YC_INPUT_T input, YC_INTERNAL_T *internal, const YC_CALS_T *Cals,
  real32_T Params_yaw_steady_fast_gain, real32_T Params_yaw_steady_slow_gain,
  real32_T Params_yaw_steady_slower_gain)
{
  real32_T filtered_value_tmp;
  if (((uint32_T)input.Yaw_Rate_Raw_QF) == ACCURATED) {
    filtered_value_tmp = ((1.0F - Params_yaw_steady_fast_gain) *
                          internal->yaw_rate_steady_fast) + (input.Yaw_Rate_Raw *
      Params_yaw_steady_fast_gain);
    internal->yaw_rate_steady_fast = filtered_value_tmp;
    if ((internal->f_yaw_rate_steady) && (internal->f_vehicle_stop)) {
      internal->yaw_rate_steady_slow = ((1.0F - Params_yaw_steady_slower_gain) *
        internal->yaw_rate_steady_slow) + (input.Yaw_Rate_Raw *
        Params_yaw_steady_slower_gain);
    } else {
      internal->yaw_rate_steady_slow = ((1.0F - Params_yaw_steady_slow_gain) *
        internal->yaw_rate_steady_slow) + (input.Yaw_Rate_Raw *
        Params_yaw_steady_slow_gain);
    }

    filtered_value_tmp = fminf(fabsf(filtered_value_tmp -
      internal->yaw_rate_steady_slow), Cals->k_YAW_RATE_DELTA_ABS_LIMIT);
    internal->yaw_rate_delta_ABS = filtered_value_tmp;
    if (internal->yaw_rate_steady_timer >= Cals->k_yaw_rate_steady_time_min) {
      internal->yaw_rate_steady_timer = Cals->k_yaw_rate_steady_time_min;
    } else {
      internal->yaw_rate_steady_timer += Cals->k_Yaw_Rate_Processing_Period;
    }

    if ((filtered_value_tmp <= Cals->k_yaw_rate_delta_steady_max) &&
        (internal->yaw_rate_steady_timer >= Cals->k_yaw_rate_steady_time_min)) {
      internal->f_yaw_rate_steady = true;
    } else if ((filtered_value_tmp > Cals->k_yaw_rate_delta_steady_max) &&
               (internal->f_yaw_rate_steady)) {
      internal->yaw_rate_steady_timer = 0.0F;
      internal->f_yaw_rate_steady = false;
    } else {
      internal->f_yaw_rate_steady = false;
    }

    internal->f_valid_stop_yaw_rate = true;
    if ((((internal->f_yaw_rate_steady) && (internal->f_stop_bias_converged)) &&
         (internal->f_vehicle_stop)) && (fabsf(input.Yaw_Rate_Raw -
          internal->yaw_rate_steady_slow) > Cals->k_stop_valid_yaw_rate_max_diff))
    {
      internal->f_valid_stop_yaw_rate = false;
    }
  } else {
    internal->f_yaw_rate_steady = false;
  }
}

// Function for MATLAB Function: '<S1>/YawRateCompensation'
void YawRateCompensationModelClass::YawRateCompensation_determineRoadType(const
  YC_OUTPUT_T output, const YC_INPUT_T input, YC_INTERNAL_T *internal, real32_T
  Params_curv_fast_gain, real32_T Params_curv_road_straight_ref_gain, real32_T
  Params_curv_road_unknown_ref_gain, real32_T Params_curv_road_unknown_gain,
  real32_T Params_curv_road_straight_gain, const YC_CALS_T *Cals)
{
  real32_T curvature_reference;
  real32_T varargin_1;
  if (!internal->f_yaw_rate_bias_shift) {
    if (fabsf(output.Yaw_Rate_Raw_Bias) > Cals->k_yaw_rate_bias_high) {
      internal->yaw_rate_select = output.Yaw_Rate_Compensated_Unfiltered;
    } else {
      internal->yaw_rate_select = input.Yaw_Rate_Raw;
    }
  } else {
    internal->yaw_rate_select = output.Yaw_Rate_Compensated_Unfiltered;
  }

  if ((Cals->C_use_ref_yaw) && (((uint32_T)input.Yaw_Rate_Reference_QF) ==
       ACCURATED)) {
    curvature_reference = fabsf(internal->filt_ref_curvature -
      internal->ref_curvature_bias);
    if (curvature_reference > Cals->k_curv_road_turn) {
      internal->road_type = CURVED_ROAD;
    } else if (curvature_reference <= Cals->k_curv_road_straight) {
      if (fabsf(internal->filt_raw_curvature - internal->raw_curvature_bias) <=
          Cals->k_curv_road_straight) {
        internal->road_type = STRAIGHT_ROAD;
      } else {
        internal->road_type = INTERMEDIATE_ROAD;
      }
    } else {
      internal->road_type = INTERMEDIATE_ROAD;
    }

    if ((input.System_Vehicle_Velocity < Cals->k_curv_speed_min) || (((uint32_T)
          input.Yaw_Rate_Raw_QF) != ACCURATED)) {
      if (internal->yaw_rate_select >= 0.0F) {
        internal->filt_raw_curvature = Cals->k_curv_road_unknown;
        internal->filt_ref_curvature = Cals->k_curv_road_unknown;
      } else {
        internal->filt_raw_curvature = -Cals->k_curv_road_unknown;
        internal->filt_ref_curvature = -Cals->k_curv_road_unknown;
      }
    } else {
      curvature_reference = internal->raw_curvature_bias;
      varargin_1 = internal->filt_raw_curvature;
      if (fabsf(internal->yaw_rate_select) <= Cals->k_curv_yaw_max) {
        switch (internal->road_type) {
         case STRAIGHT_ROAD:
          curvature_reference = fminf(fmaxf(((1.0F -
            Params_curv_road_straight_gain) * internal->raw_curvature_bias) +
            ((internal->yaw_rate_select / input.System_Vehicle_Velocity) *
             Params_curv_road_straight_gain), Cals->k_MIN_CURVATURE_BIAS),
            Cals->k_MAX_CURVATURE_BIAS);
          break;

         case UNKNOWN_ROAD:
          curvature_reference = fminf(fmaxf(((1.0F -
            Params_curv_road_unknown_gain) * internal->raw_curvature_bias) +
            ((internal->yaw_rate_select / input.System_Vehicle_Velocity) *
             Params_curv_road_unknown_gain), Cals->k_MIN_CURVATURE_BIAS),
            Cals->k_MAX_CURVATURE_BIAS);
          break;

         default:
          // no actions
          break;
        }

        varargin_1 = fmaxf(fminf(((1.0F - Params_curv_fast_gain) *
          internal->filt_raw_curvature) + ((internal->yaw_rate_select /
          input.System_Vehicle_Velocity) * Params_curv_fast_gain),
          Cals->k_MAX_FILT_RAW_CURVATURE), Cals->k_MIN_FILT_RAW_CURVATURE);
      }

      internal->filt_raw_curvature = varargin_1;
      internal->raw_curvature_bias = curvature_reference;
      curvature_reference = internal->ref_curvature_bias;
      varargin_1 = internal->filt_ref_curvature;
      if (fabsf(input.Yaw_Rate_Reference) <= Cals->k_curv_yaw_max) {
        switch (internal->road_type) {
         case STRAIGHT_ROAD:
          curvature_reference = fminf(fmaxf(((1.0F -
            Params_curv_road_straight_ref_gain) * internal->ref_curvature_bias)
            + ((input.Yaw_Rate_Reference / input.System_Vehicle_Velocity) *
               Params_curv_road_straight_ref_gain), Cals->k_MIN_CURVATURE_BIAS),
            Cals->k_MAX_CURVATURE_BIAS);
          break;

         case UNKNOWN_ROAD:
          curvature_reference = fminf(fmaxf(((1.0F -
            Params_curv_road_unknown_ref_gain) * internal->ref_curvature_bias) +
            ((input.Yaw_Rate_Reference / input.System_Vehicle_Velocity) *
             Params_curv_road_unknown_ref_gain), Cals->k_MIN_CURVATURE_BIAS),
            Cals->k_MAX_CURVATURE_BIAS);
          break;

         default:
          // no actions
          break;
        }

        varargin_1 = fmaxf(fminf(((1.0F - Params_curv_fast_gain) *
          internal->filt_ref_curvature) + ((input.Yaw_Rate_Reference /
          input.System_Vehicle_Velocity) * Params_curv_fast_gain),
          Cals->k_MAX_FILT_RAW_CURVATURE), Cals->k_MIN_FILT_RAW_CURVATURE);
      }

      internal->filt_ref_curvature = varargin_1;
      internal->ref_curvature_bias = curvature_reference;
    }
  } else {
    internal->road_type = UNKNOWN_ROAD;
  }
}

// Output and update for referenced model: 'YawRateCompensation'
void YawRateCompensationModelClass::step(const real32_T *rtu_raw_yaw_rate_rps,
  const enum_quality_factor_T *rtu_raw_yaw_rate_qf, const real32_T
  *rtu_Yaw_Rate_SA, const enum_quality_factor_T *rtu_Yaw_Rate_SA_QF, const
  real32_T *rtu_filt_veh_speed_over_ground, const enum_quality_factor_T
  *rtu_filt_veh_speed_over_ground_qf, const boolean_T *rtu_stationary, const
  uint64_T *rtu_System_Timer_Get_64bit_Current_Value, real32_T
  *rty_comp_yaw_rate_unfiltered, real32_T *rty_comp_yaw_rate_filtered, real32_T *
  rty_yaw_rate_bias, enum_quality_factor_T *rty_yaw_rate_bias_qf,
  enum_quality_factor_T *rty_comp_yaw_rate_qf, boolean_T
  *rty_f_yaw_rate_bias_converged, boolean_T *rty_f_yaw_rate_stop_bias_converged,
  real32_T *rty_YC_Internal_Resim_Signals_yaw_rate_bias1, real32_T
  *rty_YC_Internal_Resim_Signals_yaw_rate_bias2, real32_T
  *rty_YC_Internal_Resim_Signals_yaw_rate_bias_fast_bias1, real32_T
  *rty_YC_Internal_Resim_Signals_yaw_rate_bias_fast_bias2, real32_T
  *rty_YC_Internal_Resim_Signals_comp_yaw_rate_diff_filt, real32_T
  *rty_YC_Internal_Resim_Signals_yaw_rate_bias_diff, enum_road_type_T
  *rty_YC_Internal_Resim_Signals_road_type, boolean_T
  *rty_YC_Internal_Resim_Signals_f_yaw_rate_bias_shift, boolean_T
  *rty_YC_Internal_Resim_Signals_f_Yaw_Rate_Bias_Converged, boolean_T
  *rty_YC_Internal_Resim_Signals_f_stop_bias_converged, boolean_T
  *rty_YC_Internal_Resim_Signals_f_yaw_rate_steady, boolean_T
  *rty_YC_Internal_Resim_Signals_f_input_invalid_persistent, boolean_T
  *rty_YC_Internal_Resim_Signals_f_execution_period_error_persistent, boolean_T *
  rty_YC_Internal_Resim_Signals_f_bias_was_accurate, real32_T
  *rty_YC_Internal_Resim_Signals_ignition_time, boolean_T
  *rty_YC_Internal_Resim_Signals_f_yaw_stop_bias_converged, boolean_T
  *rty_f_yaw_rate_stop_bias_converged_atleast_once)
{
  real32_T Yaw_Rate_Filt_Enable_Limit;
  enum_quality_factor_T b;
  real32_T limited_yaw_rate_bias;
  enum_yaw_rate_bias_accuracy_T i;
  boolean_T j;
  YC_INPUT_T rtb_BusCreator;
  static const YC_OUTPUT_T tmp = { 0.0F,// Yaw_Rate_Compensated_Unfiltered
    0.0F,                              // Yaw_Rate_Compensated_Filtered
    0.0F,                              // Yaw_Rate_Raw_Bias
    1U,                                // Yaw_Rate_Compensated_QF
    0U                                 // Yaw_Rate_Bias_QF
  };

  static const YC_INTERNAL_T tmp_0 = { 0.0F,// yaw_rate_error
    0.0F,                              // yaw_rate_bias1
    0.0F,                              // yaw_rate_bias2
    0.0F,                              // ignition_time
    0.0F,                              // yaw_rate_ref_bias_time
    0.0F,                              // yaw_rate_ref_bias
    0.0F,                              // yaw_rate_steady_fast
    0.0F,                              // yaw_rate_steady_slow
    0.0F,                              // yaw_rate_steady_timer
    false,                             // f_yaw_rate_steady
    false,                             // f_valid_stop_yaw_rate
    false,                             // f_stop_bias_converged
    0.0F,                              // yaw_rate_bias_stop_inc_step
    false,                             // f_yaw_rate_bias_shift
    0.0F,                              // filt_raw_curvature
    0.0F,                              // filt_ref_curvature
    0.0F,                              // raw_curvature_bias
    0.0F,                              // ref_curvature_bias
    0,                                 // road_type
    0.0F,                              // yaw_rate_select
    0.0F,                              // yaw_rate_filtered
    0.0F,                              // yaw_rate_ref_filtered
    false,                             // f_vehicle_stop
    0.0F,                              // comp_yaw_rate_difference
    0.0F,                              // comp_yaw_rate_diff_filt
    0.0F,                              // yaw_rate_bias_fast_bias1
    0.0F,                              // yaw_rate_bias_fast_bias2
    0.0F,                              // yaw_rate_bias_fast
    0,                                 // bias_accuracy
    false,                             // f_bias_was_accurate
    0.0F,                              // yaw_rate_bias_diff
    false,                             // f_Yaw_Rate_Bias_Converged
    0.0F,                              // yaw_rate_delta_ABS
    false,                             // f_execution_period_error
    false,                             // f_execution_period_error_persistent
    false,                             // f_execution_period_error_start
    0.0F,                              // exec_error_sustain_period
    0UL,                               // prev_run_time
    0UL,                               // execution_period_error_start_time
    false,                             // f_input_invalid
    false,                             // f_input_invalid_persistent
    false,                             // f_input_invalid_start
    0.0F,                              // input_invalid_sustain_period
    0UL,                               // input_invalid_start_time
    false                              // f_Enable_Fast_Yaw_Bias_Filter
  };

  static const YC_CALS_T tmp_1 = { 0.05F,// k_yaw_rate_time_constant
    0.297F,                            // k_yaw_rate_bias_time_constant
    0.3F,                              // k_yaw_rate_cal_time_const_stop
    0.1F,                              // k_yaw_rate_cal_time_const_stop_fast
    0.6F,                              // k_yaw_rate_cal_time_const_stop_slow
    0.3F,                              // k_yaw_rate_steady_fast_time_const
    1.125F,                            // k_yaw_rate_steady_slow_time_const
    4.5F,                              // k_yaw_rate_steady_slower_time_const
    0.00174520072F,                    // k_yaw_rate_delta_steady_max
    1.0F,                              // k_yaw_rate_steady_time_min
    5.0F,                              // k_curv_fast_tc
    200.0F,                            // k_curv_road_straight_ref_tc
    400.0F,                            // k_curv_road_unknown_ref_tc
    2.0F,                              // k_yaw_rate_reference_bias_tc
    600.0F,                            // k_curv_road_unknown_tc
    20.0F,                             // k_yaw_rate_ref_bias_stable_time
    300.0F,                            // k_curv_road_straight_tc
    0.000349040143F,                   // k_curv_road_unknown
    false,                             // k_yaw_rate_bias_ref_enable
    600.0F,                            // k_enable_fast_yaw_bias_time
    0.0261780098F,                     // k_Yaw_Washout_Max_Rate
    0.0174520072F,                     // k_Yaw_Rate_Washout_Max_Rate_2
    600.0F,                            // k_yaw_rate_washout_time_constant
    150.0F,                            // k_Yaw_Rate_Washout_Time_Constant_Fast
    5.0F,                              // k_yaw_rate_washout_time_constant_stop
    300.0F,                            // k_Yaw_Rate_Washout_Time_Constant_Straight 
    150.0F,                            // k_ref_yaw_rate_washout_time_constant
    100.0F,                            // k_ref_yaw_rate_washout_time_constant_straight 
    75.0F,                             // k_ref_yaw_rate_washout_time_constant_fast 
    true,                              // k_yaw_rate_bias_stop_enable
    3.0F,                              // k_yaw_bias_enable_speed
    false,                             // k_yaw_rate_bias_remove
    0.000523560215F,                   // k_curv_bias_high
    8.72600358E-5F,                    // k_curv_bias_low
    0.2F,                              // k_yaw_rate_bias_high
    7.0F,                              // k_curv_speed_min
    0.104712039F,                      // k_yaw_rate_ref_max
    0.00174520072F,                    // k_yaw_rate_bias_threshold
    0.0523560196F,                     // k_curv_yaw_max
    0.0610820241F,                     // k_yaw_rate_error_max
    0.000523560215F,                   // k_curv_road_turn
    0.000174520072F,                   // k_curv_road_straight
    true,                              // k_yaw_rate_bias2_enabled
    10.0F,                             // k_yaw_rate_washout_min_speed
    true,                              // C_use_veh_stationary
    0.025F,                            // k_Yaw_Rate_Processing_Period
    0.174520075F,                      // k_YAW_RATE_DELTA_ABS_LIMIT
    0.0610820241F,                     // k_MAX_YAW_RATE_BIAS
    -0.0610820241F,                    // k_MIN_YAW_RATE_BIAS
    0.001F,                            // k_EPSILON_ZERO_VEH_SPEED
    60.0F,                             // k_MAX_YAW_RATE_REF_BIAS_TIME
    -0.15F,                            // k_MIN_CURVATURE_BIAS
    0.15F,                             // k_MAX_CURVATURE_BIAS
    -0.15F,                            // k_MIN_FILT_RAW_CURVATURE
    0.15F,                             // k_MAX_FILT_RAW_CURVATURE
    0.000872600358F,                   // k_yaw_rate_cal_stop_bias_inc
    0.000174520072F,                   // k_yaw_rate_cal_stop_bias_inc_slow
    0.00174520072F,                    // k_yaw_rate_cal_bias_step
    3.0F,                              // k_calibration_version_main
    3.0F,                              // k_calibration_version_sub
    0.0F,                              // k_discard_invalid_sensor_data_time
    0.00523560215F,                    // k_stop_valid_yaw_rate_max_diff
    true,                              // C_use_ref_yaw
    0.0218150094F,                     // k_comp_and_ref_diff_low
    0.0261780098F,                     // k_comp_and_ref_diff_mid
    0.0349040143F,                     // k_max_comp_and_ref_diff
    0.000872600358F,                   // k_comp_and_ref_diff_transition
    0.0349040143F,                     // k_max_yaw_comp_threshold
    10.0F,                             // k_min_ref_yaw_speed_threshold
    30.0F,                             // k_comp_and_ref_diff_time_constant
    0.00174520072F,                    // k_yaw_rate_bias_diff_drift_low
    0.00261780107F,                    // k_yaw_rate_bias_diff_drift_mid
    0.00436300179F,                    // k_max_yaw_rate_bias_diff_drift
    0.00261780107F,                    // k_yaw_rate_bias_diff_qf_mid
    0.00349040143F,                    // k_yaw_rate_bias_diff_qf_high
    0.00610820251F,                    // k_max_yaw_rate_bias_diff_qf
    0.0305410121F,                     // k_max_yaw_rate_raw_input
    -0.0305410121F,                    // k_min_yaw_rate_raw_input
    3U,                                // k_max_yaw_rate_qf_input
    0U,                                // k_min_yaw_rate_qf_input
    0.0305410121F,                     // k_max_yaw_rate_ref_input
    -0.0305410121F,                    // k_min_yaw_rate_ref_input
    3U,                                // k_max_yaw_rate_ref_qf_input
    0U,                                // k_min_yaw_rate_ref_qf_input
    100.0F,                            // k_max_veh_vel_input
    -20.0F,                            // k_min_veh_vel_input
    true,                              // k_max_f_veh_stationary
    false,                             // k_min_f_veh_stationary
    10000.0F,                          // k_max_input_invalid_duration
    0.2F,                              // k_max_execution_period_error_range
    10000.0F                           // k_max_execution_period_error_sustain
  };

  static const YC_PARAMS_T tmp_2 = { false,// f_gain_calculated
    0.0F,                              // system_yaw_rate_gain
    0.0F,                              // yaw_rate_bias_gain
    0.0F,                              // yaw_rate_error_gain
    0.0F,                              // yaw_rate_error_gain_fast
    0.0F,                              // yaw_rate_error_gain_slow
    0.0F,                              // yaw_rate_washout_gain
    0.0F,                              // yaw_rate_washout_fast_gain
    0.0F,                              // yaw_rate_washout_stop_gain
    0.0F,                              // yaw_rate_washout_straight_gain
    0.0F,                              // ref_yaw_rate_washout_straight_gain
    0.0F,                              // ref_yaw_rate_washout_gain
    0.0F,                              // ref_yaw_rate_washout_fast_gain
    0.0F,                              // yaw_steady_fast_gain
    0.0F,                              // yaw_steady_slow_gain
    0.0F,                              // yaw_steady_slower_gain
    0.0F,                              // curv_fast_gain
    0.0F,                              // curv_road_straight_ref_gain
    0.0F,                              // curv_road_unknown_ref_gain
    0.0F,                              // yaw_rate_reference_bias_gain
    0.0F,                              // curv_road_unknown_gain
    0.0F,                              // curv_road_straight_gain
    0.0F,                              // filt_raw_curvature
    0.0F,                              // filt_raw_curvature_ref
    0.0F                               // comp_yaw_diff_gain
  };

  real32_T yaw_rate_bias_removed_tmp;
  real32_T b_filtered_value_tmp;
  real32_T c_filtered_value_tmp;
  enum_road_type_T tmp_3;
  boolean_T guard1 = false;

  // BusCreator: '<S1>/Bus Creator'
  rtb_BusCreator.Yaw_Rate_Raw = *rtu_raw_yaw_rate_rps;
  rtb_BusCreator.Yaw_Rate_Raw_QF = *rtu_raw_yaw_rate_qf;
  rtb_BusCreator.Yaw_Rate_Reference = *rtu_Yaw_Rate_SA;
  rtb_BusCreator.Yaw_Rate_Reference_QF = *rtu_Yaw_Rate_SA_QF;
  rtb_BusCreator.System_Vehicle_Velocity = *rtu_filt_veh_speed_over_ground;
  rtb_BusCreator.System_Vehicle_Velocity_QF = *rtu_filt_veh_speed_over_ground_qf;
  rtb_BusCreator.f_Vehicle_Stationary = *rtu_stationary;
  rtb_BusCreator.curr_run_time = *rtu_System_Timer_Get_64bit_Current_Value;

  // Chart: '<S1>/Chart'
  if (((uint32_T)
       YawRateCompensation_DW.bitsForTID0.is_active_c1_YawRateCompensation) ==
      0U) {
    YawRateCompensation_DW.bitsForTID0.is_active_c1_YawRateCompensation = 1;
    YawRateCompensation_DW.bitsForTID0.is_c1_YawRateCompensation =
      YawRateCompensation_IN_Initialize;
    YawRateCompensation_DW.bitsForTID0.f_initialize_yaw_rate_comp = true;
  } else if (((uint32_T)
              YawRateCompensation_DW.bitsForTID0.is_c1_YawRateCompensation) ==
             YawRateCompensation_IN_Initialize) {
    if (YawRateCompensation_DW.bitsForTID0.f_initialize_yaw_rate_comp) {
      YawRateCompensation_DW.bitsForTID0.is_c1_YawRateCompensation =
        YawRateCompensation_IN_Running;
      YawRateCompensation_DW.bitsForTID0.f_initialize_yaw_rate_comp = false;
    }
  } else {
    YawRateCompensation_DW.bitsForTID0.f_initialize_yaw_rate_comp = false;
  }

  // End of Chart: '<S1>/Chart'

  // Outputs for Enabled SubSystem: '<S1>/Initialization' incorporates:
  //   EnablePort: '<S4>/Enable'

  if (YawRateCompensation_DW.bitsForTID0.f_initialize_yaw_rate_comp) {
    // MATLAB Function: '<S4>/MATLAB Function'
    memcpy(&YawRateCompensation_DW.YC_output, &tmp, sizeof(YC_OUTPUT_T));
    memcpy(&YawRateCompensation_DW.YC_internal, &tmp_0, sizeof(YC_INTERNAL_T));
    memcpy(&YawRateCompensation_DW.YC_Cals, &tmp_1, sizeof(YC_CALS_T));
    YawRateCompensation_DW.YC_Params = tmp_2;

    // MATLAB Function: '<S4>/MATLAB Function1'
    YawRateCompensation_calsInit(&YawRateCompensation_DW.YC_Cals,
      &YawRateCompensation_DW.YC_Params);
    YawRateCompensation_DW.YC_internal.yaw_rate_error = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_bias1 = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_bias2 = 0.0F;
    YawRateCompensation_DW.YC_internal.ignition_time = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_ref_bias_time = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_ref_bias = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_steady_fast = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_steady_slow = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_steady_timer = 0.0F;
    YawRateCompensation_DW.YC_internal.f_yaw_rate_steady = false;
    YawRateCompensation_DW.YC_internal.f_valid_stop_yaw_rate = false;
    YawRateCompensation_DW.YC_internal.f_stop_bias_converged = false;
    YawRateCompensation_DW.YC_internal.yaw_rate_bias_stop_inc_step =
      YawRateCompensation_DW.YC_Cals.k_yaw_rate_cal_stop_bias_inc;
    YawRateCompensation_DW.YC_internal.f_yaw_rate_bias_shift = false;
    YawRateCompensation_DW.YC_internal.filt_raw_curvature = 0.0F;
    YawRateCompensation_DW.YC_internal.filt_ref_curvature = 0.0F;
    YawRateCompensation_DW.YC_internal.raw_curvature_bias = 0.0F;
    YawRateCompensation_DW.YC_internal.ref_curvature_bias = 0.0F;
    YawRateCompensation_DW.YC_internal.road_type = UNKNOWN_ROAD;
    YawRateCompensation_DW.YC_internal.yaw_rate_select = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_filtered = 0.0F;
    YawRateCompensation_DW.YC_internal.yaw_rate_ref_filtered = 0.0F;
    YawRateCompensation_DW.YC_internal.f_vehicle_stop = false;
    YawRateCompensation_DW.YC_internal.f_Yaw_Rate_Bias_Converged = false;
    YawRateCompensation_DW.YC_output.Yaw_Rate_Compensated_Unfiltered = 0.0F;
    YawRateCompensation_DW.YC_output.Yaw_Rate_Compensated_Filtered = 0.0F;
    YawRateCompensation_DW.YC_output.Yaw_Rate_Raw_Bias = 0.0F;
    YawRateCompensation_DW.YC_output.Yaw_Rate_Compensated_QF = UNDEFINED;
  }

  // End of Outputs for SubSystem: '<S1>/Initialization'

  // Switch: '<S1>/Switch' incorporates:
  //   Switch: '<S1>/Switch1'
  //   Switch: '<S1>/Switch2'
  //   Switch: '<S1>/Switch3'
  //   UnitDelay: '<S1>/Unit Delay'
  //   UnitDelay: '<S1>/Unit Delay1'
  //   UnitDelay: '<S1>/Unit Delay2'
  //   UnitDelay: '<S1>/Unit Delay3'

  if (YawRateCompensation_DW.bitsForTID0.f_initialize_yaw_rate_comp) {
    YawRateCompensation_DW.UnitDelay3_DSTATE = YawRateCompensation_DW.YC_output;
    YawRateCompensation_DW.UnitDelay2_DSTATE =
      YawRateCompensation_DW.YC_internal;
    YawRateCompensation_DW.UnitDelay1_DSTATE = YawRateCompensation_DW.YC_Cals;
    YawRateCompensation_DW.UnitDelay_DSTATE = YawRateCompensation_DW.YC_Params;
  }

  // End of Switch: '<S1>/Switch'

  // MATLAB Function: '<S1>/YawRateCompensation' incorporates:
  //   UnitDelay: '<S1>/Unit Delay'
  //   UnitDelay: '<S1>/Unit Delay1'
  //   UnitDelay: '<S1>/Unit Delay2'
  //   UnitDelay: '<S1>/Unit Delay3'

  YawRateCompensation_determineGain(&YawRateCompensation_DW.UnitDelay1_DSTATE,
    &YawRateCompensation_DW.UnitDelay_DSTATE);
  YawRateCompensation_determineYawRateInputValid(rtb_BusCreator,
    &YawRateCompensation_DW.UnitDelay2_DSTATE,
    &YawRateCompensation_DW.UnitDelay1_DSTATE);
  YawRateCompensation_determineExecutionPeriodValid
    (*rtu_System_Timer_Get_64bit_Current_Value,
     &YawRateCompensation_DW.UnitDelay2_DSTATE,
     &YawRateCompensation_DW.UnitDelay1_DSTATE);
  YawRateCompensation_DW.UnitDelay2_DSTATE.ignition_time +=
    YawRateCompensation_DW.UnitDelay1_DSTATE.k_Yaw_Rate_Processing_Period;
  if (YawRateCompensation_DW.UnitDelay2_DSTATE.ignition_time > 1800.0F) {
    YawRateCompensation_DW.UnitDelay2_DSTATE.ignition_time = 1800.0F;
  }

  if (!YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_remove) {
    limited_yaw_rate_bias =
      YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias;
  } else if (YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias >
             YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_threshold)
  {
    limited_yaw_rate_bias =
      YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias -
      YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_threshold;
  } else if (YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias <
             (-YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_threshold))
  {
    limited_yaw_rate_bias =
      YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias +
      YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_threshold;
  } else {
    limited_yaw_rate_bias = 0.0F;
  }

  yaw_rate_bias_removed_tmp = (*rtu_raw_yaw_rate_rps) - limited_yaw_rate_bias;
  YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_Unfiltered =
    yaw_rate_bias_removed_tmp;
  Yaw_Rate_Filt_Enable_Limit = (1.0F -
    YawRateCompensation_DW.UnitDelay_DSTATE.system_yaw_rate_gain) *
    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_Filtered;
  limited_yaw_rate_bias = Yaw_Rate_Filt_Enable_Limit +
    (YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_Unfiltered *
     YawRateCompensation_DW.UnitDelay_DSTATE.system_yaw_rate_gain);
  YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_Filtered =
    Yaw_Rate_Filt_Enable_Limit + (yaw_rate_bias_removed_tmp *
    YawRateCompensation_DW.UnitDelay_DSTATE.system_yaw_rate_gain);
  if (YawRateCompensation_DW.UnitDelay2_DSTATE.ignition_time >
      YawRateCompensation_DW.UnitDelay1_DSTATE.k_discard_invalid_sensor_data_time)
  {
    if ((!YawRateCompensation_DW.UnitDelay2_DSTATE.f_input_invalid) &&
        ((*rtu_filt_veh_speed_over_ground) >= 0.0F)) {
      YawRateCompensation_DW.UnitDelay2_DSTATE.f_vehicle_stop =
        (((*rtu_filt_veh_speed_over_ground) <
          YawRateCompensation_DW.UnitDelay1_DSTATE.k_EPSILON_ZERO_VEH_SPEED) &&
         ((!YawRateCompensation_DW.UnitDelay1_DSTATE.C_use_veh_stationary) ||
          (*rtu_stationary)));
      YawRateCompensation_determineYawRateSteady(rtb_BusCreator,
        &YawRateCompensation_DW.UnitDelay2_DSTATE,
        &YawRateCompensation_DW.UnitDelay1_DSTATE,
        YawRateCompensation_DW.UnitDelay_DSTATE.yaw_steady_fast_gain,
        YawRateCompensation_DW.UnitDelay_DSTATE.yaw_steady_slow_gain,
        YawRateCompensation_DW.UnitDelay_DSTATE.yaw_steady_slower_gain);
      if ((YawRateCompensation_DW.UnitDelay2_DSTATE.ignition_time <
           YawRateCompensation_DW.UnitDelay1_DSTATE.k_enable_fast_yaw_bias_time)
          || (YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_bias_shift)) {
        YawRateCompensation_DW.UnitDelay2_DSTATE.f_Enable_Fast_Yaw_Bias_Filter =
          true;
        Yaw_Rate_Filt_Enable_Limit =
          YawRateCompensation_DW.UnitDelay1_DSTATE.k_Yaw_Washout_Max_Rate;
      } else {
        YawRateCompensation_DW.UnitDelay2_DSTATE.f_Enable_Fast_Yaw_Bias_Filter =
          false;
        Yaw_Rate_Filt_Enable_Limit =
          YawRateCompensation_DW.UnitDelay1_DSTATE.k_Yaw_Rate_Washout_Max_Rate_2;
      }

      YawRateCompensation_determineRoadType
        (YawRateCompensation_DW.UnitDelay3_DSTATE, rtb_BusCreator,
         &YawRateCompensation_DW.UnitDelay2_DSTATE,
         YawRateCompensation_DW.UnitDelay_DSTATE.curv_fast_gain,
         YawRateCompensation_DW.UnitDelay_DSTATE.curv_road_straight_ref_gain,
         YawRateCompensation_DW.UnitDelay_DSTATE.curv_road_unknown_ref_gain,
         YawRateCompensation_DW.UnitDelay_DSTATE.curv_road_unknown_gain,
         YawRateCompensation_DW.UnitDelay_DSTATE.curv_road_straight_gain,
         &YawRateCompensation_DW.UnitDelay1_DSTATE);
      if ((YawRateCompensation_DW.UnitDelay1_DSTATE.C_use_ref_yaw) &&
          (((uint32_T)(*rtu_Yaw_Rate_SA_QF)) == ACCURATED)) {
        b_filtered_value_tmp = ((1.0F -
          YawRateCompensation_DW.UnitDelay_DSTATE.system_yaw_rate_gain) *
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_filtered) +
          ((*rtu_Yaw_Rate_SA) *
           YawRateCompensation_DW.UnitDelay_DSTATE.system_yaw_rate_gain);
        YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_filtered =
          b_filtered_value_tmp;
        c_filtered_value_tmp = ((1.0F -
          YawRateCompensation_DW.UnitDelay_DSTATE.system_yaw_rate_gain) *
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_filtered) +
          ((*rtu_raw_yaw_rate_rps) *
           YawRateCompensation_DW.UnitDelay_DSTATE.system_yaw_rate_gain);
        YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_filtered =
          c_filtered_value_tmp;
        if (((*rtu_filt_veh_speed_over_ground) >=
             YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_bias_enable_speed) &&
            (fabsf(b_filtered_value_tmp) <=
             YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_ref_max)) {
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_bias_time =
            fminf
            (YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_bias_time +
             YawRateCompensation_DW.UnitDelay1_DSTATE.k_Yaw_Rate_Processing_Period,
             YawRateCompensation_DW.UnitDelay1_DSTATE.k_MAX_YAW_RATE_REF_BIAS_TIME);
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_bias = ((1.0F -
            YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_reference_bias_gain)
            * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_bias) +
            ((c_filtered_value_tmp - b_filtered_value_tmp) *
             YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_reference_bias_gain);
        }
      }

      if (((uint32_T)(*rtu_raw_yaw_rate_qf)) == ACCURATED) {
        if (YawRateCompensation_DW.UnitDelay2_DSTATE.f_vehicle_stop) {
          if (YawRateCompensation_DW.UnitDelay2_DSTATE.f_stop_bias_converged) {
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error = ((1.0F -
              YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_error_gain_slow) *
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error) +
              (yaw_rate_bias_removed_tmp *
               YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_error_gain_slow);
          } else {
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error = ((1.0F -
              YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_error_gain_fast) *
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error) +
              (yaw_rate_bias_removed_tmp *
               YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_error_gain_fast);
          }

          if (((YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_steady) &&
               (fabsf(*rtu_raw_yaw_rate_rps) <
                YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_error_max)) &&
              (YawRateCompensation_DW.UnitDelay2_DSTATE.f_valid_stop_yaw_rate))
          {
            if (fabsf(YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error) >=
                YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_cal_bias_step)
            {
              if (YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error > 0.0F)
              {
                YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 = fmaxf
                  (fminf
                   (YawRateCompensation_DW.UnitDelay1_DSTATE.k_MAX_YAW_RATE_BIAS,
                    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias +
                    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_stop_inc_step),
                   YawRateCompensation_DW.UnitDelay1_DSTATE.k_MIN_YAW_RATE_BIAS);
                YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2 = 0.0F;
              } else {
                YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 = fmaxf
                  (fminf
                   (YawRateCompensation_DW.UnitDelay1_DSTATE.k_MAX_YAW_RATE_BIAS,
                    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias -
                    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_stop_inc_step),
                   YawRateCompensation_DW.UnitDelay1_DSTATE.k_MIN_YAW_RATE_BIAS);
                YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2 = 0.0F;
              }

              YawRateCompensation_DW.UnitDelay2_DSTATE.f_stop_bias_converged =
                false;
              YawRateCompensation_DW.UnitDelay2_DSTATE.f_Yaw_Rate_Bias_Converged
                = false;
            } else {
              YawRateCompensation_DW.UnitDelay2_DSTATE.f_stop_bias_converged =
                true;
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_stop_inc_step
                =
                YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_cal_stop_bias_inc_slow;
              YawRateCompensation_DW.UnitDelay2_DSTATE.f_Yaw_Rate_Bias_Converged
                = true;
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 = ((1.0F -
                YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_stop_gain)
                * YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias) +
                ((*rtu_raw_yaw_rate_rps) *
                 YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_stop_gain);
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2 = 0.0F;
            }

            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1 =
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1;
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2 =
              0.0F;
          }
        } else {
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error = ((1.0F -
            YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_error_gain) *
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error) +
            (yaw_rate_bias_removed_tmp *
             YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_error_gain);
          if (((((YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_ref_enable)
                 &&
                 (YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_bias_time
                  >=
                  YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_ref_bias_stable_time))
                &&
                (!YawRateCompensation_DW.UnitDelay2_DSTATE.f_Yaw_Rate_Bias_Converged))
               && (YawRateCompensation_DW.UnitDelay1_DSTATE.C_use_ref_yaw)) &&
              (((uint32_T)(*rtu_Yaw_Rate_SA_QF)) == ACCURATED)) {
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 =
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_bias;
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2 = 0.0F;
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1 =
              YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1;
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2 =
              0.0F;
            YawRateCompensation_DW.UnitDelay2_DSTATE.f_Yaw_Rate_Bias_Converged =
              true;
          } else {
            if (((*rtu_filt_veh_speed_over_ground) >=
                 YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_washout_min_speed)
                && (fabsf
                    (YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_select) <=
                    Yaw_Rate_Filt_Enable_Limit)) {
              tmp_3 = YawRateCompensation_DW.UnitDelay2_DSTATE.road_type;
              if ((tmp_3 != CURVED_ROAD) && (tmp_3 != UNKNOWN_ROAD)) {
                j =
                  !YawRateCompensation_DW.UnitDelay2_DSTATE.f_Enable_Fast_Yaw_Bias_Filter;
                if ((tmp_3 == INTERMEDIATE_ROAD) && j) {
                  yaw_rate_bias_removed_tmp = ((1.0F -
                    YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_gain)
                    * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1) +
                    ((*rtu_raw_yaw_rate_rps) *
                     YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_gain);
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 =
                    yaw_rate_bias_removed_tmp;
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2 =
                    ((1.0F -
                      YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_gain)
                     * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2)
                    + (((*rtu_raw_yaw_rate_rps) - yaw_rate_bias_removed_tmp) *
                       YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_gain);
                  yaw_rate_bias_removed_tmp = ((1.0F -
                    YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_gain)
                    * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1)
                    + ((*rtu_raw_yaw_rate_rps) *
                       YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_gain);
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1
                    = yaw_rate_bias_removed_tmp;
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2
                    = ((1.0F -
                        YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_gain)
                       * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2)
                    + (((*rtu_raw_yaw_rate_rps) - yaw_rate_bias_removed_tmp) *
                       YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_gain);
                } else if ((tmp_3 == STRAIGHT_ROAD) && j) {
                  yaw_rate_bias_removed_tmp = ((1.0F -
                    YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_straight_gain)
                    * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1) +
                    ((*rtu_raw_yaw_rate_rps) *
                     YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_straight_gain);
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 =
                    yaw_rate_bias_removed_tmp;
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2 =
                    ((1.0F -
                      YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_straight_gain)
                     * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2)
                    + (((*rtu_raw_yaw_rate_rps) - yaw_rate_bias_removed_tmp) *
                       YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_straight_gain);
                  yaw_rate_bias_removed_tmp = ((1.0F -
                    YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_straight_gain)
                    * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1)
                    + ((*rtu_raw_yaw_rate_rps) *
                       YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_straight_gain);
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1
                    = yaw_rate_bias_removed_tmp;
                  YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2
                    = ((1.0F -
                        YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_straight_gain)
                       * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2)
                    + (((*rtu_raw_yaw_rate_rps) - yaw_rate_bias_removed_tmp) *
                       YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_straight_gain);
                } else {
                  if ((tmp_3 != CURVED_ROAD) &&
                      (YawRateCompensation_DW.UnitDelay2_DSTATE.f_Enable_Fast_Yaw_Bias_Filter))
                  {
                    yaw_rate_bias_removed_tmp = ((1.0F -
                      YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_fast_gain)
                      * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1)
                      + ((*rtu_raw_yaw_rate_rps) *
                         YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_fast_gain);
                    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 =
                      yaw_rate_bias_removed_tmp;
                    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2 =
                      ((1.0F -
                        YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_fast_gain)
                       * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2)
                      + (((*rtu_raw_yaw_rate_rps) - yaw_rate_bias_removed_tmp) *
                         YawRateCompensation_DW.UnitDelay_DSTATE.yaw_rate_washout_fast_gain);
                    yaw_rate_bias_removed_tmp = ((1.0F -
                      YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_fast_gain)
                      * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1)
                      + ((*rtu_raw_yaw_rate_rps) *
                         YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_fast_gain);
                    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1
                      = yaw_rate_bias_removed_tmp;
                    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2
                      = ((1.0F -
                          YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_fast_gain)
                         * YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2)
                      + (((*rtu_raw_yaw_rate_rps) - yaw_rate_bias_removed_tmp) *
                         YawRateCompensation_DW.UnitDelay_DSTATE.ref_yaw_rate_washout_fast_gain);
                  }
                }
              }
            }
          }
        }
      } else {
        YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_error = 0.0F;
      }

      if (YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias2_enabled) {
        YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias =
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1 -
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2;
        YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast =
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1 -
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2;
      } else {
        YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias =
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1;
        YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast =
          YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1;
      }

      YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_diff =
        YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast -
        YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias;
      if ((YawRateCompensation_DW.UnitDelay1_DSTATE.C_use_ref_yaw) &&
          (((uint32_T)(*rtu_Yaw_Rate_SA_QF)) == ACCURATED)) {
        if (((*rtu_filt_veh_speed_over_ground) >=
             YawRateCompensation_DW.UnitDelay1_DSTATE.k_min_ref_yaw_speed_threshold)
            && (fabsf(limited_yaw_rate_bias) <=
                YawRateCompensation_DW.UnitDelay1_DSTATE.k_max_yaw_comp_threshold))
        {
          YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_difference =
            limited_yaw_rate_bias -
            YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_ref_filtered;
          YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt =
            ((1.0F - YawRateCompensation_DW.UnitDelay_DSTATE.comp_yaw_diff_gain)
             * YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt)
            + (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_difference
               * YawRateCompensation_DW.UnitDelay_DSTATE.comp_yaw_diff_gain);
        }

        yaw_rate_bias_removed_tmp = fabsf
          (YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_diff);
        if (yaw_rate_bias_removed_tmp >=
            YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_diff_drift_mid)
        {
          if (fabsf(YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_diff)
              >=
              YawRateCompensation_DW.UnitDelay1_DSTATE.k_max_yaw_rate_bias_diff_drift)
          {
            YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_bias_shift =
              true;
          } else {
            if (((fabsf
                  (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt)
                  >=
                  YawRateCompensation_DW.UnitDelay1_DSTATE.k_comp_and_ref_diff_mid)
                 && (fabsf(limited_yaw_rate_bias) <=
                     YawRateCompensation_DW.UnitDelay1_DSTATE.k_max_yaw_comp_threshold))
                && (((*rtu_filt_veh_speed_over_ground) >=
                     YawRateCompensation_DW.UnitDelay1_DSTATE.k_min_ref_yaw_speed_threshold)
                    &&
                    (((YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_diff
                       >= 0.0F) &&
                      (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt
                       >= 0.0F)) ||
                     ((YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_diff
                       < 0.0F) &&
                      (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt
                       < 0.0F))))) {
              YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_bias_shift =
                true;
            }
          }
        } else if (fabsf
                   (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt)
                   <
                   YawRateCompensation_DW.UnitDelay1_DSTATE.k_comp_and_ref_diff_low)
        {
          YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_bias_shift = false;
        } else {
          if (yaw_rate_bias_removed_tmp <
              YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_diff_drift_low)
          {
            YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_bias_shift =
              false;
          }
        }
      } else {
        YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_bias_shift = false;
      }
    }

    guard1 = false;
    if (((YawRateCompensation_DW.UnitDelay1_DSTATE.C_use_ref_yaw) && (((uint32_T)
           (*rtu_Yaw_Rate_SA_QF)) == ACCURATED)) && (fabsf
         (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt) >
         YawRateCompensation_DW.UnitDelay1_DSTATE.k_max_comp_and_ref_diff)) {
      guard1 = true;
    } else {
      yaw_rate_bias_removed_tmp = fabsf
        (YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_diff);
      if (yaw_rate_bias_removed_tmp >
          YawRateCompensation_DW.UnitDelay1_DSTATE.k_max_yaw_rate_bias_diff_qf)
      {
        guard1 = true;
      } else if ((((!YawRateCompensation_DW.UnitDelay1_DSTATE.C_use_ref_yaw) ||
                   (((uint32_T)(*rtu_Yaw_Rate_SA_QF)) != ACCURATED)) || (fabsf
                   (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt)
        <= (YawRateCompensation_DW.UnitDelay1_DSTATE.k_comp_and_ref_diff_low -
            YawRateCompensation_DW.UnitDelay1_DSTATE.k_comp_and_ref_diff_transition)))
                 && (yaw_rate_bias_removed_tmp <=
                     YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_diff_qf_mid))
      {
        i = BIAS_ACCURATE;
        j = true;
      } else if ((((YawRateCompensation_DW.UnitDelay1_DSTATE.C_use_ref_yaw) &&
                   (((uint32_T)(*rtu_Yaw_Rate_SA_QF)) == ACCURATED)) && (fabsf
                   (YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt)
        > (YawRateCompensation_DW.UnitDelay1_DSTATE.k_comp_and_ref_diff_low +
           YawRateCompensation_DW.UnitDelay1_DSTATE.k_comp_and_ref_diff_transition)))
                 || (yaw_rate_bias_removed_tmp >
                     YawRateCompensation_DW.UnitDelay1_DSTATE.k_yaw_rate_bias_diff_qf_high))
      {
        i = BIAS_NOT_ACCURATE;
        j = YawRateCompensation_DW.UnitDelay2_DSTATE.f_bias_was_accurate;
      } else {
        i = YawRateCompensation_DW.UnitDelay2_DSTATE.bias_accuracy;
        j = YawRateCompensation_DW.UnitDelay2_DSTATE.f_bias_was_accurate;
      }
    }

    if (guard1) {
      i = BIAS_UNDEFINED;
      j = YawRateCompensation_DW.UnitDelay2_DSTATE.f_bias_was_accurate;
    }

    YawRateCompensation_DW.UnitDelay2_DSTATE.bias_accuracy = i;
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_bias_was_accurate = j;
    if ((((uint32_T)(*rtu_raw_yaw_rate_qf)) == TEMP_UNDEFINED) || (!j)) {
      b = TEMP_UNDEFINED;
    } else if ((((((((uint32_T)(*rtu_raw_yaw_rate_qf)) == UNDEFINED) ||
                   (YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias >
                    YawRateCompensation_DW.UnitDelay1_DSTATE.k_MAX_YAW_RATE_BIAS))
                  || (YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias
                      <
                      YawRateCompensation_DW.UnitDelay1_DSTATE.k_MIN_YAW_RATE_BIAS))
                 || ((((uint32_T)(*rtu_raw_yaw_rate_qf)) == ACCURATED) && (i ==
        BIAS_UNDEFINED))) ||
                (YawRateCompensation_DW.UnitDelay2_DSTATE.f_input_invalid_persistent))
               ||
               (YawRateCompensation_DW.UnitDelay2_DSTATE.f_execution_period_error_persistent))
    {
      b = UNDEFINED;
    } else if ((((uint32_T)(*rtu_raw_yaw_rate_qf)) == ACCURATED) && (i ==
                BIAS_ACCURATE)) {
      b = ACCURATED;
    } else {
      b = NOT_ACCURATED;
    }

    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Bias_QF = b;
    if ((((uint32_T)(*rtu_raw_yaw_rate_qf)) == TEMP_UNDEFINED) || (((uint32_T)b)
         == TEMP_UNDEFINED)) {
      YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_QF =
        TEMP_UNDEFINED;
    } else if ((((uint32_T)(*rtu_raw_yaw_rate_qf)) == UNDEFINED) || (((uint32_T)
                 b) == UNDEFINED)) {
      YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_QF =
        UNDEFINED;
    } else if ((((uint32_T)(*rtu_raw_yaw_rate_qf)) == NOT_ACCURATED) ||
               (((uint32_T)b) == NOT_ACCURATED)) {
      YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_QF =
        NOT_ACCURATED;
    } else {
      YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_QF =
        ACCURATED;
    }

    YawRateCompensation_DW.UnitDelay2_DSTATE.prev_run_time =
      *rtu_System_Timer_Get_64bit_Current_Value;
  }

  // End of MATLAB Function: '<S1>/YawRateCompensation'

  // MATLAB Function: '<S1>/MATLAB Function' incorporates:
  //   UnitDelay: '<S1>/Unit Delay2'
  //   UnitDelay: '<S1>/Unit Delay4'

  *rty_f_yaw_rate_stop_bias_converged_atleast_once =
    YawRateCompensation_DW.bitsForTID0.UnitDelay4_DSTATE;
  if ((YawRateCompensation_DW.UnitDelay2_DSTATE.f_stop_bias_converged) &&
      (!YawRateCompensation_DW.bitsForTID0.UnitDelay4_DSTATE)) {
    *rty_f_yaw_rate_stop_bias_converged_atleast_once = true;
  }

  // End of MATLAB Function: '<S1>/MATLAB Function'

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_yaw_rate_bias1 =
    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias1;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_f_stop_bias_converged =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_stop_bias_converged;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_f_yaw_rate_steady =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_steady;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_f_input_invalid_persistent =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_input_invalid_persistent;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_f_execution_period_error_persistent =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_execution_period_error_persistent;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_f_bias_was_accurate =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_bias_was_accurate;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_ignition_time =
    YawRateCompensation_DW.UnitDelay2_DSTATE.ignition_time;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' 
  *rty_YC_Internal_Resim_Signals_f_yaw_stop_bias_converged =
    *rty_f_yaw_rate_stop_bias_converged_atleast_once;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_yaw_rate_bias2 =
    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias2;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_yaw_rate_bias_fast_bias1 =
    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias1;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_yaw_rate_bias_fast_bias2 =
    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_fast_bias2;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_comp_yaw_rate_diff_filt =
    YawRateCompensation_DW.UnitDelay2_DSTATE.comp_yaw_rate_diff_filt;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_yaw_rate_bias_diff =
    YawRateCompensation_DW.UnitDelay2_DSTATE.yaw_rate_bias_diff;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_road_type =
    YawRateCompensation_DW.UnitDelay2_DSTATE.road_type;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_f_yaw_rate_bias_shift =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_yaw_rate_bias_shift;

  // SignalConversion: '<Root>/BusConversion_InsertedFor_YC_Internal_Resim_Signals_at_inport_0' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_YC_Internal_Resim_Signals_f_Yaw_Rate_Bias_Converged =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_Yaw_Rate_Bias_Converged;

  // SignalConversion: '<Root>/TmpSignal ConversionAtcomp_yaw_rate_filteredInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay3'

  *rty_comp_yaw_rate_filtered =
    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_Filtered;

  // SignalConversion: '<Root>/TmpSignal ConversionAtcomp_yaw_rate_qfInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay3'

  *rty_comp_yaw_rate_qf =
    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_QF;

  // SignalConversion: '<Root>/TmpSignal ConversionAtcomp_yaw_rate_unfilteredInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay3'

  *rty_comp_yaw_rate_unfiltered =
    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Compensated_Unfiltered;

  // SignalConversion: '<Root>/TmpSignal ConversionAtf_yaw_rate_bias_convergedInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_f_yaw_rate_bias_converged =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_Yaw_Rate_Bias_Converged;

  // SignalConversion: '<Root>/TmpSignal ConversionAtf_yaw_rate_stop_bias_convergedInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay2'

  *rty_f_yaw_rate_stop_bias_converged =
    YawRateCompensation_DW.UnitDelay2_DSTATE.f_stop_bias_converged;

  // SignalConversion: '<Root>/TmpSignal ConversionAtyaw_rate_biasInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay3'

  *rty_yaw_rate_bias =
    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Raw_Bias;

  // SignalConversion: '<Root>/TmpSignal ConversionAtyaw_rate_bias_qfInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay3'

  *rty_yaw_rate_bias_qf =
    YawRateCompensation_DW.UnitDelay3_DSTATE.Yaw_Rate_Bias_QF;

  // Update for UnitDelay: '<S1>/Unit Delay4'
  YawRateCompensation_DW.bitsForTID0.UnitDelay4_DSTATE =
    *rty_f_yaw_rate_stop_bias_converged_atleast_once;
}

// Constructor
YawRateCompensationModelClass::YawRateCompensationModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
YawRateCompensationModelClass::~YawRateCompensationModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
