//
// File: SteeringWheelAngleBiasEstimation.h
//
// Code generated for Simulink model 'SteeringWheelAngleBiasEstimation'.
//
// Model version                  : 1.932
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:57:58 2024
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
#ifndef RTW_HEADER_SteeringWheelAngleBiasEstimation_h_
#define RTW_HEADER_SteeringWheelAngleBiasEstimation_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef SteeringWheelAngleBiasEstimation_COMMON_INCLUDES_
# define SteeringWheelAngleBiasEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // SteeringWheelAngleBiasEstimation_COMMON_INCLUDES_ 

#include "SteeringWheelAngleBiasEstimation_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'SteeringWheelAngleBiasEstimation' 
typedef struct {
  real32_T wheel_speed_filter_den[5];  // '<S22>/wheel speed filter co-efficients' 
  real32_T wheel_speed_filter_num[5];  // '<S22>/wheel speed filter co-efficients' 
  real32_T diff_denom[3];              // '<S22>/vel to accel co-efficients'
  real32_T diff_num[3];                // '<S22>/vel to accel co-efficients'
  real32_T host_speed_filter_den[5];   // '<S22>/host speed filter co-efficients' 
  real32_T host_speed_filter_num[5];   // '<S22>/host speed filter co-efficients' 
  real32_T accel_filter_den[5];        // '<S22>/accel filter co-efficients'
  real32_T accel_filter_num[5];        // '<S22>/accel filter co-efficients'
  real32_T wheelspeedfilterright_states[4];// '<S31>/wheel speed filter right'
  real32_T wheelspeedfilterleft_states[4];// '<S31>/wheel speed filter left'
  real32_T veltoaccelfilter_states[4]; // '<S24>/vel to accel filter'
  real32_T veltoaccelfilter_states_m[2];// '<S25>/vel to accel filter'
  real32_T accelfilter_states[4];      // '<S23>/accel filter'
  real32_T UnitDelay1_DSTATE;          // '<S11>/Unit Delay1'
  real32_T UnitDelay2_DSTATE;          // '<S11>/Unit Delay2'
  real32_T UnitDelay5_DSTATE;          // '<S11>/Unit Delay5'
  real32_T UnitDelay2_DSTATE_p;        // '<S10>/Unit Delay2'
  real32_T UnitDelay1_DSTATE_n;        // '<S10>/Unit Delay1'
  real32_T UnitDelay5_DSTATE_c;        // '<S10>/Unit Delay5'
  real32_T wheelspeedfilterright_tmp;  // '<S31>/wheel speed filter right'
  real32_T wheelspeedfilterleft_tmp;   // '<S31>/wheel speed filter left'
  real32_T veltoaccelfilter_tmp;       // '<S24>/vel to accel filter'
  real32_T veltoaccelfilter_tmp_d;     // '<S25>/vel to accel filter'
  real32_T accelfilter_tmp;            // '<S23>/accel filter'
  real32_T matured_time;               // '<S9>/MATLAB Function'
  real32_T dematured_time;             // '<S9>/MATLAB Function'
  struct {
    uint_T is_c17_YawRateBasedSWABiasEstimation:2;// '<S11>/compensated vehicle steering angle quality factor' 
    uint_T is_c17_YawRateBasedSWABiasEstimation_b:2;// '<S10>/compensated vehicle steering angle quality factor' 
    uint_T is_c8_SteeringWheelAngleBiasEstimation:2;// '<S5>/Chart'
    uint_T is_c2_SteeringWheelAngleBiasEstimation:2;// '<S1>/Chart'
    uint_T is_active_c17_YawRateBasedSWABiasEstimation:1;// '<S11>/compensated vehicle steering angle quality factor' 
    uint_T is_active_c17_YawRateBasedSWABiasEstimation_o:1;// '<S10>/compensated vehicle steering angle quality factor' 
    uint_T is_active_c8_SteeringWheelAngleBiasEstimation:1;// '<S5>/Chart'
    uint_T is_active_c2_SteeringWheelAngleBiasEstimation:1;// '<S1>/Chart'
    uint_T f_initialize_swa_bias_estimation:1;// '<S5>/Chart'
    uint_T UnitDelay3_DSTATE:1;        // '<S11>/Unit Delay3'
    uint_T UnitDelay3_DSTATE_m:1;      // '<S10>/Unit Delay3'
    uint_T prev_final_fault_flag:1;    // '<S9>/MATLAB Function'
  } bitsForTID0;

  uint16_T UnitDelay6_DSTATE;          // '<S11>/Unit Delay6'
  uint16_T UnitDelay6_DSTATE_d;        // '<S10>/Unit Delay6'
  enum_quality_factor_T comp_veh_steering_angle_qf;// '<S11>/compensated vehicle steering angle quality factor' 
  enum_quality_factor_T comp_veh_steering_angle_qf_b;// '<S10>/compensated vehicle steering angle quality factor' 
  enum_quality_factor_T comp_veh_steering_angle_qf_m;// '<S1>/Chart'
  uint8_T UnitDelay_DSTATE;            // '<S11>/Unit Delay'
  uint8_T UnitDelay4_DSTATE;           // '<S11>/Unit Delay4'
  uint8_T UnitDelay_DSTATE_a;          // '<S10>/Unit Delay'
  uint8_T UnitDelay4_DSTATE_h;         // '<S10>/Unit Delay4'
} DW_SteeringWheelAngleBiasEstimation_T;

// Class declaration for model SteeringWheelAngleBiasEstimation
class SteeringWheelAngleBiasEstimationModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_filt_veh_speed_over_ground, const real32_T
            *rtu_comp_yaw_rate_filtered, const real32_T *rtu_comp_lat_accel,
            const real32_T *rtu_raw_steering_angle_deg, const
            enum_quality_factor_T *rtu_filt_veh_speed_over_ground_qf, const
            enum_quality_factor_T *rtu_raw_speed_qf, const enum_quality_factor_T
            *rtu_comp_yaw_rate_qf, const boolean_T
            *rtu_f_yaw_rate_stop_bias_converged, const enum_quality_factor_T
            *rtu_comp_lat_accel_qf, const enum_quality_factor_T
            *rtu_raw_steering_angle_qf, const real32_T
            *rtu_wheel_lin_speed_mps_fl, const real32_T
            *rtu_wheel_lin_speed_mps_fr, const real32_T *rtu_PressureValue_RHF,
            const real32_T *rtu_PressureValue_LHF, const real32_T
            *rtu_k_veh_track, const real32_T *rtu_nvm_last_rem_swa_bias_deg,
            const boolean_T *rtu_nvm_f_last_rem_swa_bias, const real32_T
            *rtu_steering_angle_bias_deg_external, const enum_quality_factor_T
            *rtu_steering_angle_bias_qf_external, boolean_T
            *rty_f_consider_swa_bias_conv_out, real32_T
            *rty_swa_bias_converged_out, real32_T *rty_comp_steering_angle_deg,
            enum_quality_factor_T *rty_comp_steering_angle_qf, boolean_T
            *rty_f_consider_swa_bias_conv_internal);

  // Constructor
  SteeringWheelAngleBiasEstimationModelClass();

  // Destructor
  ~SteeringWheelAngleBiasEstimationModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_SteeringWheelAngleBiasEstimation_T SteeringWheelAngleBiasEstimation_DW;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Data Type Conversion' : Eliminate redundant data type conversion


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
//  '<Root>' : 'SteeringWheelAngleBiasEstimation'
//  '<S1>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation'
//  '<S2>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds'
//  '<S3>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Chart'
//  '<S4>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Compare To Constant2'
//  '<S5>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem'
//  '<S6>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/If Action Subsystem'
//  '<S7>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/If Action Subsystem1'
//  '<S8>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/If Action Subsystem2'
//  '<S9>'   : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Maturation_DeMaturation_Block'
//  '<S10>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation'
//  '<S11>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1'
//  '<S12>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/Compare To Constant'
//  '<S13>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/Compare To Constant1'
//  '<S14>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/Compare To Constant2'
//  '<S15>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/Compare To Constant3'
//  '<S16>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/Compare To Constant4'
//  '<S17>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/Compare To Constant5'
//  '<S18>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/Compare To Constant6'
//  '<S19>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/If Action Subsystem'
//  '<S20>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/CalculateYawRateUsingWheelSpeeds/If Action Subsystem1'
//  '<S21>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/Chart'
//  '<S22>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/Subsystem'
//  '<S23>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/acceleration filter'
//  '<S24>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/host speed filter'
//  '<S25>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/vel to accel filter'
//  '<S26>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/wheel_speed_filter'
//  '<S27>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/Subsystem/accel filter co-efficients'
//  '<S28>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/Subsystem/host speed filter co-efficients'
//  '<S29>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/Subsystem/vel to accel co-efficients'
//  '<S30>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/Subsystem/wheel speed filter co-efficients'
//  '<S31>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Filter subsystem/wheel_speed_filter/Wheel speed filter'
//  '<S32>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S33>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/2DecRoundingOff'
//  '<S34>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/Compare To Constant'
//  '<S35>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/Compare To Constant1'
//  '<S36>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/Compare To Constant2'
//  '<S37>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/Compare To Constant3'
//  '<S38>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/Degrees to Radians'
//  '<S39>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/NVM interaction'
//  '<S40>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/SWABiasOperatingCondition'
//  '<S41>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/SWABias_Level1Convergence'
//  '<S42>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/SWABias_Level2Convergence'
//  '<S43>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation/compensated vehicle steering angle quality factor'
//  '<S44>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/2DecRoundingOff'
//  '<S45>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/Compare To Constant'
//  '<S46>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/Compare To Constant1'
//  '<S47>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/Compare To Constant2'
//  '<S48>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/Compare To Constant3'
//  '<S49>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/Degrees to Radians'
//  '<S50>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/NVM interaction'
//  '<S51>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/SWABiasOperatingCondition'
//  '<S52>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/SWABias_Level1Convergence'
//  '<S53>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/SWABias_Level2Convergence'
//  '<S54>'  : 'SteeringWheelAngleBiasEstimation/SteeringWheelAngleBiasEstimation/YawRateBasedSWABiasEstimation1/compensated vehicle steering angle quality factor'

#endif                                 // RTW_HEADER_SteeringWheelAngleBiasEstimation_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
