//
// File: YawRateCompensation.h
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
#ifndef RTW_HEADER_YawRateCompensation_h_
#define RTW_HEADER_YawRateCompensation_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef YawRateCompensation_COMMON_INCLUDES_
# define YawRateCompensation_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // YawRateCompensation_COMMON_INCLUDES_

#include "YawRateCompensation_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'YawRateCompensation'
typedef struct {
  YC_CALS_T YC_Cals;                   // '<S4>/MATLAB Function1'
  YC_CALS_T UnitDelay1_DSTATE;         // '<S1>/Unit Delay1'
  YC_INTERNAL_T YC_internal;           // '<S4>/MATLAB Function1'
  YC_INTERNAL_T UnitDelay2_DSTATE;     // '<S1>/Unit Delay2'
  YC_PARAMS_T YC_Params;               // '<S4>/MATLAB Function1'
  YC_PARAMS_T UnitDelay_DSTATE;        // '<S1>/Unit Delay'
  YC_OUTPUT_T YC_output;               // '<S4>/MATLAB Function1'
  YC_OUTPUT_T UnitDelay3_DSTATE;       // '<S1>/Unit Delay3'
  struct {
    uint_T is_c1_YawRateCompensation:2;// '<S1>/Chart'
    uint_T is_active_c1_YawRateCompensation:1;// '<S1>/Chart'
    uint_T f_initialize_yaw_rate_comp:1;// '<S1>/Chart'
    uint_T UnitDelay4_DSTATE:1;        // '<S1>/Unit Delay4'
  } bitsForTID0;
} DW_YawRateCompensation_T;

// Class declaration for model YawRateCompensation
class YawRateCompensationModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_raw_yaw_rate_rps, const enum_quality_factor_T
            *rtu_raw_yaw_rate_qf, const real32_T *rtu_Yaw_Rate_SA, const
            enum_quality_factor_T *rtu_Yaw_Rate_SA_QF, const real32_T
            *rtu_filt_veh_speed_over_ground, const enum_quality_factor_T
            *rtu_filt_veh_speed_over_ground_qf, const boolean_T *rtu_stationary,
            const uint64_T *rtu_System_Timer_Get_64bit_Current_Value, real32_T
            *rty_comp_yaw_rate_unfiltered, real32_T *rty_comp_yaw_rate_filtered,
            real32_T *rty_yaw_rate_bias, enum_quality_factor_T
            *rty_yaw_rate_bias_qf, enum_quality_factor_T *rty_comp_yaw_rate_qf,
            boolean_T *rty_f_yaw_rate_bias_converged, boolean_T
            *rty_f_yaw_rate_stop_bias_converged, real32_T
            *rty_YC_Internal_Resim_Signals_yaw_rate_bias1, real32_T
            *rty_YC_Internal_Resim_Signals_yaw_rate_bias2, real32_T
            *rty_YC_Internal_Resim_Signals_yaw_rate_bias_fast_bias1, real32_T
            *rty_YC_Internal_Resim_Signals_yaw_rate_bias_fast_bias2, real32_T
            *rty_YC_Internal_Resim_Signals_comp_yaw_rate_diff_filt, real32_T
            *rty_YC_Internal_Resim_Signals_yaw_rate_bias_diff, enum_road_type_T *
            rty_YC_Internal_Resim_Signals_road_type, boolean_T
            *rty_YC_Internal_Resim_Signals_f_yaw_rate_bias_shift, boolean_T
            *rty_YC_Internal_Resim_Signals_f_Yaw_Rate_Bias_Converged, boolean_T *
            rty_YC_Internal_Resim_Signals_f_stop_bias_converged, boolean_T
            *rty_YC_Internal_Resim_Signals_f_yaw_rate_steady, boolean_T
            *rty_YC_Internal_Resim_Signals_f_input_invalid_persistent, boolean_T
            *rty_YC_Internal_Resim_Signals_f_execution_period_error_persistent,
            boolean_T *rty_YC_Internal_Resim_Signals_f_bias_was_accurate,
            real32_T *rty_YC_Internal_Resim_Signals_ignition_time, boolean_T
            *rty_YC_Internal_Resim_Signals_f_yaw_stop_bias_converged, boolean_T *
            rty_f_yaw_rate_stop_bias_converged_atleast_once);

  // Constructor
  YawRateCompensationModelClass();

  // Destructor
  ~YawRateCompensationModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_YawRateCompensation_T YawRateCompensation_DW;

  // private member function(s) for subsystem '<Root>/TmpModelReferenceSubsystem'
  void YawRateCompensation_determineGain(const YC_CALS_T *YC_Cals, YC_PARAMS_T
    *YC_Params);
  void YawRateCompensation_determineYawRateInputValid(const YC_INPUT_T YC_input,
    YC_INTERNAL_T *YC_internal, const YC_CALS_T *YC_Cals);
  void YawRateCompensation_calsInit(YC_CALS_T *YC_Cals, YC_PARAMS_T *YC_Params);
  void YawRateCompensation_determineExecutionPeriodValid(uint64_T curr_run_time,
    YC_INTERNAL_T *YC_internal, const YC_CALS_T *YC_Cals);
  void YawRateCompensation_determineYawRateSteady(const YC_INPUT_T input,
    YC_INTERNAL_T *internal, const YC_CALS_T *Cals, real32_T
    Params_yaw_steady_fast_gain, real32_T Params_yaw_steady_slow_gain, real32_T
    Params_yaw_steady_slower_gain);
  void YawRateCompensation_determineRoadType(const YC_OUTPUT_T output, const
    YC_INPUT_T input, YC_INTERNAL_T *internal, real32_T Params_curv_fast_gain,
    real32_T Params_curv_road_straight_ref_gain, real32_T
    Params_curv_road_unknown_ref_gain, real32_T Params_curv_road_unknown_gain,
    real32_T Params_curv_road_straight_gain, const YC_CALS_T *Cals);
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'YawRateCompensation'
//  '<S1>'   : 'YawRateCompensation/YawRateCompensation'
//  '<S2>'   : 'YawRateCompensation/YawRateCompensation/Chart'
//  '<S3>'   : 'YawRateCompensation/YawRateCompensation/Elapsed_Time'
//  '<S4>'   : 'YawRateCompensation/YawRateCompensation/Initialization'
//  '<S5>'   : 'YawRateCompensation/YawRateCompensation/MATLAB Function'
//  '<S6>'   : 'YawRateCompensation/YawRateCompensation/YawRateCompensation'
//  '<S7>'   : 'YawRateCompensation/YawRateCompensation/Initialization/MATLAB Function'
//  '<S8>'   : 'YawRateCompensation/YawRateCompensation/Initialization/MATLAB Function1'

#endif                                 // RTW_HEADER_YawRateCompensation_h_

//
// File trailer for generated code.
//
// [EOF]
//
