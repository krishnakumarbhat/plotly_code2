//
// File: SteeringWheelAngleBiasEstimation.cpp
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
#include "SteeringWheelAngleBiasEstimation.h"
#include "SteeringWheelAngleBiasEstimation_private.h"

// Named constants for MATLAB Function: '<S10>/NVM interaction'
#define SteeringWheelAngleBiasEstimation_FALSE (false)
#define SteeringWheelAngleBiasEstimation_TRUE (true)
#define SteeringWheelAngleBiasEstimation_k_SWABias_high_threshold (12.0F)

// Named constants for Chart: '<S1>/Chart'
#define SteeringWheelAngleBiasEstimation_IN_Initialize ((uint8_T)1U)
#define SteeringWheelAngleBiasEstimation_IN_Running ((uint8_T)2U)
#define SteeringWheelAngleBiasEstimation_IN_not_converged ((uint8_T)3U)

//
// Output and update for action system:
//    '<S1>/If Action Subsystem'
//    '<S1>/If Action Subsystem1'
//
void SteeringWheelAngleBiasEstimation_IfActionSubsystem(boolean_T rtu_In1,
  real32_T rtu_In1_n, real32_T rtu_In1_c, enum_quality_factor_T rtu_In1_l,
  boolean_T rtu_In1_f, boolean_T *rty_Out1, real32_T *rty_Out1_n, real32_T
  *rty_Out1_c, enum_quality_factor_T *rty_Out1_l, boolean_T *rty_Out1_f)
{
  // SignalConversion: '<S6>/BusConversion_InsertedFor_Out1_at_inport_0'
  *rty_Out1 = rtu_In1;

  // SignalConversion: '<S6>/BusConversion_InsertedFor_Out1_at_inport_0'
  *rty_Out1_n = rtu_In1_n;

  // SignalConversion: '<S6>/BusConversion_InsertedFor_Out1_at_inport_0'
  *rty_Out1_c = rtu_In1_c;

  // SignalConversion: '<S6>/BusConversion_InsertedFor_Out1_at_inport_0'
  *rty_Out1_l = rtu_In1_l;

  // SignalConversion: '<S6>/BusConversion_InsertedFor_Out1_at_inport_0'
  *rty_Out1_f = rtu_In1_f;
}

//
// Output and update for atomic system:
//    '<S10>/NVM interaction'
//    '<S11>/NVM interaction'
//
void SteeringWheelAngleBiasEstimation_NVMinteraction(real32_T
  rtu_swa_bias_converged, boolean_T rtu_f_consider_swa_bias_conv_in, const
  real32_T *rtu_last_rem_swa_bias, const boolean_T *rtu_f_last_rem_swa_bias,
  boolean_T *rty_f_consider_swa_bias_conv_out, real32_T
  *rty_swa_bias_converged_out)
{
  if (rtu_f_consider_swa_bias_conv_in) {
    *rty_f_consider_swa_bias_conv_out = SteeringWheelAngleBiasEstimation_TRUE;
    *rty_swa_bias_converged_out = rtu_swa_bias_converged;
  } else if ((*rtu_f_last_rem_swa_bias) && (fabsf(*rtu_last_rem_swa_bias) <
              SteeringWheelAngleBiasEstimation_k_SWABias_high_threshold)) {
    *rty_f_consider_swa_bias_conv_out = SteeringWheelAngleBiasEstimation_TRUE;
    *rty_swa_bias_converged_out = *rtu_last_rem_swa_bias;
  } else {
    *rty_f_consider_swa_bias_conv_out = SteeringWheelAngleBiasEstimation_FALSE;
    *rty_swa_bias_converged_out = *rtu_last_rem_swa_bias;
  }
}

//
// Output and update for atomic system:
//    '<S10>/SWABias_Level1Convergence'
//    '<S11>/SWABias_Level1Convergence'
//
void SteeringWheelAngleBiasEstimation_SWABias_Level1Convergence(real32_T
  rtu_SWA_Bias_deg, boolean_T rtu_f_consider_SWA_Bias, real32_T
  rtu_SWA_Bias_deg_Avg_In, uint8_T rtu_Counter_In, uint8_T
  rtu_k_HAS_SWABias_TempSamples, real32_T *rty_SWA_bias_deg_Avg_Temp, boolean_T *
  rty_f_consider_SWA_Bias_deg_Avg_temp, real32_T *rty_SWA_Bias_deg_Avg_Out,
  uint8_T *rty_Counter_Out)
{
  real32_T SWA_Bias_deg_Avg_Out;
  uint8_T Counter_Out;
  if (rtu_f_consider_SWA_Bias) {
    SWA_Bias_deg_Avg_Out = ((rtu_SWA_Bias_deg_Avg_In * ((real32_T)rtu_Counter_In))
      + rtu_SWA_Bias_deg) / (((real32_T)rtu_Counter_In) + 1.0F);
    Counter_Out = (uint8_T)(((uint32_T)rtu_Counter_In) + 1U);
  } else {
    SWA_Bias_deg_Avg_Out = rtu_SWA_Bias_deg_Avg_In;
    Counter_Out = rtu_Counter_In;
  }

  if (Counter_Out >= rtu_k_HAS_SWABias_TempSamples) {
    *rty_SWA_bias_deg_Avg_Temp = SWA_Bias_deg_Avg_Out;
    *rty_f_consider_SWA_Bias_deg_Avg_temp = true;
    Counter_Out = 0U;
  } else {
    *rty_SWA_bias_deg_Avg_Temp = 0.0F;
    *rty_f_consider_SWA_Bias_deg_Avg_temp = false;
  }

  *rty_SWA_Bias_deg_Avg_Out = SWA_Bias_deg_Avg_Out;
  *rty_Counter_Out = Counter_Out;
}

//
// Output and update for atomic system:
//    '<S10>/SWABias_Level2Convergence'
//    '<S11>/SWABias_Level2Convergence'
//
void SteeringWheelAngleBiasEstimation_SWABias_Level2Convergence(real32_T
  rtu_SWA_Bias_deg_Conv_In, boolean_T rtu_f_SWA_Bias_deg_Conv_In, real32_T
  rtu_SWA_bias_deg_Avg_Temp, boolean_T rtu_f_consider_SWA_Bias_deg_Avg_Temp,
  uint8_T rtu_Counter_In, real32_T rtu_SWA_Bias_deg_Conv_OutFinalIn, uint16_T
  rtu_SWA_Bias_ConvCountIn, uint8_T rtu_k_HAS_SWABias_ConvSamples, real32_T
  rtu_k_HAS_SWABias_NewBiasInFac, real32_T rtu_k_HAS_SWABias_NewBiasDecFac,
  real32_T *rty_SWA_Bias_deg_Conv_Out, boolean_T *rty_f_SWA_Bias_deg_Conv_out,
  uint8_T *rty_Counter_Out, real32_T *rty_SWA_Bias_deg_Conv_OutFinal, uint16_T
  *rty_SWA_Bias_ConvCount)
{
  real32_T SWA_Bias_deg_Conv_Out;
  if (rtu_f_consider_SWA_Bias_deg_Avg_Temp) {
    SWA_Bias_deg_Conv_Out = ((rtu_SWA_Bias_deg_Conv_In * ((real32_T)
      rtu_Counter_In)) + rtu_SWA_bias_deg_Avg_Temp) / (((real32_T)rtu_Counter_In)
      + 1.0F);
    *rty_Counter_Out = (uint8_T)(((uint32_T)rtu_Counter_In) + 1U);
  } else {
    SWA_Bias_deg_Conv_Out = rtu_SWA_Bias_deg_Conv_In;
    *rty_Counter_Out = rtu_Counter_In;
  }

  if (rtu_Counter_In >= rtu_k_HAS_SWABias_ConvSamples) {
    *rty_f_SWA_Bias_deg_Conv_out = true;
    *rty_SWA_Bias_ConvCount = (uint16_T)(((uint32_T)rtu_SWA_Bias_ConvCountIn) +
      1U);
    if (rtu_f_SWA_Bias_deg_Conv_In) {
      if (((SWA_Bias_deg_Conv_Out >= 0.0F) && (rtu_SWA_Bias_deg_Conv_OutFinalIn >=
            0.0F)) || ((SWA_Bias_deg_Conv_Out < 0.0F) &&
                       (rtu_SWA_Bias_deg_Conv_OutFinalIn < 0.0F))) {
        if (fabsf(SWA_Bias_deg_Conv_Out) > fabsf
            (rtu_SWA_Bias_deg_Conv_OutFinalIn)) {
          *rty_SWA_Bias_deg_Conv_OutFinal = ((1.0F -
            rtu_k_HAS_SWABias_NewBiasInFac) * rtu_SWA_Bias_deg_Conv_OutFinalIn)
            + (rtu_k_HAS_SWABias_NewBiasInFac * SWA_Bias_deg_Conv_Out);
        } else {
          *rty_SWA_Bias_deg_Conv_OutFinal = ((1.0F -
            rtu_k_HAS_SWABias_NewBiasDecFac) * rtu_SWA_Bias_deg_Conv_OutFinalIn)
            + (rtu_k_HAS_SWABias_NewBiasDecFac * SWA_Bias_deg_Conv_Out);
        }
      } else {
        *rty_SWA_Bias_deg_Conv_OutFinal = ((1.0F -
          rtu_k_HAS_SWABias_NewBiasDecFac) * rtu_SWA_Bias_deg_Conv_OutFinalIn) +
          (rtu_k_HAS_SWABias_NewBiasDecFac * SWA_Bias_deg_Conv_Out);
      }
    } else {
      *rty_SWA_Bias_deg_Conv_OutFinal = SWA_Bias_deg_Conv_Out;
    }

    *rty_Counter_Out = 0U;
  } else {
    *rty_f_SWA_Bias_deg_Conv_out = rtu_f_SWA_Bias_deg_Conv_In;
    *rty_SWA_Bias_deg_Conv_OutFinal = rtu_SWA_Bias_deg_Conv_OutFinalIn;
    *rty_SWA_Bias_ConvCount = rtu_SWA_Bias_ConvCountIn;
  }

  *rty_SWA_Bias_deg_Conv_Out = SWA_Bias_deg_Conv_Out;
}

// Output and update for referenced model: 'SteeringWheelAngleBiasEstimation'
void SteeringWheelAngleBiasEstimationModelClass::step(const real32_T
  *rtu_filt_veh_speed_over_ground, const real32_T *rtu_comp_yaw_rate_filtered,
  const real32_T *rtu_comp_lat_accel, const real32_T *rtu_raw_steering_angle_deg,
  const enum_quality_factor_T *rtu_filt_veh_speed_over_ground_qf, const
  enum_quality_factor_T *rtu_raw_speed_qf, const enum_quality_factor_T
  *rtu_comp_yaw_rate_qf, const boolean_T *rtu_f_yaw_rate_stop_bias_converged,
  const enum_quality_factor_T *rtu_comp_lat_accel_qf, const
  enum_quality_factor_T *rtu_raw_steering_angle_qf, const real32_T
  *rtu_wheel_lin_speed_mps_fl, const real32_T *rtu_wheel_lin_speed_mps_fr, const
  real32_T *rtu_PressureValue_RHF, const real32_T *rtu_PressureValue_LHF, const
  real32_T *rtu_k_veh_track, const real32_T *rtu_nvm_last_rem_swa_bias_deg,
  const boolean_T *rtu_nvm_f_last_rem_swa_bias, const real32_T
  *rtu_steering_angle_bias_deg_external, const enum_quality_factor_T
  *rtu_steering_angle_bias_qf_external, boolean_T
  *rty_f_consider_swa_bias_conv_out, real32_T *rty_swa_bias_converged_out,
  real32_T *rty_comp_steering_angle_deg, enum_quality_factor_T
  *rty_comp_steering_angle_qf, boolean_T *rty_f_consider_swa_bias_conv_internal)
{
  real32_T numAccum;
  real32_T rtb_swa_bias_converged_out;
  real32_T rtb_swa_bias_converged_out_e;
  real32_T rtb_Gain;
  boolean_T rtb_f_use_yawrate_using_wheel_speeds;
  boolean_T rtb_Compare_mv;
  boolean_T rtb_f_consider_swa_bias_conv_out_k;
  boolean_T rtb_f_do_not_use_yawrate;
  boolean_T rtb_f_consider_SWA_Bias;
  boolean_T rtb_f_consider_SWA_Bias_a;
  enum_quality_factor_T rtb_Switch;
  static const real32_T tmp[5] = { 0.0288372524F, -0.0951865837F, 0.134625152F,
    -0.0951865837F, 0.0288372524F };

  static const real32_T tmp_0[5] = { 1.0F, -3.44273305F, 4.47926712F,
    -2.60689211F, 0.572284937F };

  static const real32_T tmp_1[5] = { 0.0842187703F, -0.277991F, 0.393170774F,
    -0.277991F, 0.0842187703F };

  static const real32_T tmp_2[5] = { 1.0F, -3.28342938F, 4.10065699F,
    -2.30170918F, 0.490107685F };

  static const real32_T tmp_3[5] = { 0.0288942661F, -0.110054433F, 0.162455484F,
    -0.110054433F, 0.0288942661F };

  static const real32_T tmp_4[5] = { 1.0F, -3.72233772F, 5.20511723F,
    -3.24018526F, 0.757540941F };

  int32_T i;
  boolean_T tmp_5;
  boolean_T tmp_6;

  // Chart: '<S5>/Chart'
  if (((uint32_T)
       SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c8_SteeringWheelAngleBiasEstimation)
      == 0U) {
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c8_SteeringWheelAngleBiasEstimation
      = 1;
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c8_SteeringWheelAngleBiasEstimation
      = SteeringWheelAngleBiasEstimation_IN_Initialize;
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.f_initialize_swa_bias_estimation
      = true;
  } else if (((uint32_T)
              SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c8_SteeringWheelAngleBiasEstimation)
             == SteeringWheelAngleBiasEstimation_IN_Initialize) {
    if (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.f_initialize_swa_bias_estimation)
    {
      SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c8_SteeringWheelAngleBiasEstimation
        = SteeringWheelAngleBiasEstimation_IN_Running;
      SteeringWheelAngleBiasEstimation_DW.bitsForTID0.f_initialize_swa_bias_estimation
        = false;
    }
  } else {
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.f_initialize_swa_bias_estimation
      = false;
  }

  // End of Chart: '<S5>/Chart'

  // Outputs for Enabled SubSystem: '<S5>/Subsystem' incorporates:
  //   EnablePort: '<S22>/Enable'

  if (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.f_initialize_swa_bias_estimation)
  {
    for (i = 0; i < 5; i++) {
      // MATLAB Function: '<S22>/wheel speed filter co-efficients'
      SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[i] = tmp[i];
      SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[i] = tmp_0[i];

      // MATLAB Function: '<S22>/host speed filter co-efficients'
      SteeringWheelAngleBiasEstimation_DW.host_speed_filter_num[i] = tmp_1[i];
      SteeringWheelAngleBiasEstimation_DW.host_speed_filter_den[i] = tmp_2[i];

      // MATLAB Function: '<S22>/accel filter co-efficients'
      SteeringWheelAngleBiasEstimation_DW.accel_filter_num[i] = tmp_3[i];
      SteeringWheelAngleBiasEstimation_DW.accel_filter_den[i] = tmp_4[i];
    }

    // MATLAB Function: '<S22>/vel to accel co-efficients'
    SteeringWheelAngleBiasEstimation_DW.diff_num[0] = 150.0F;
    SteeringWheelAngleBiasEstimation_DW.diff_denom[0] = 1.0F;
    SteeringWheelAngleBiasEstimation_DW.diff_num[1] = -200.0F;
    SteeringWheelAngleBiasEstimation_DW.diff_denom[1] = 0.0F;
    SteeringWheelAngleBiasEstimation_DW.diff_num[2] = 50.0F;
    SteeringWheelAngleBiasEstimation_DW.diff_denom[2] = 0.0F;
  }

  // End of Outputs for SubSystem: '<S5>/Subsystem'

  // DiscreteFilter: '<S31>/wheel speed filter right'
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_tmp =
    ((((*rtu_wheel_lin_speed_mps_fr) -
       (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[1] *
        SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[0])) -
      (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[2] *
       SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[1])) -
     (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[3] *
      SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[2])) -
    (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[4] *
     SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[3]);
  numAccum = ((((SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[0] *
                 SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_tmp)
                + (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[1]
                   * SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states
                   [0])) +
               (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[2] *
                SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[
                1])) +
              (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[3] *
               SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states
               [2])) +
    (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[4] *
     SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[3]);

  // DiscreteFilter: '<S31>/wheel speed filter left'
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_tmp =
    ((((*rtu_wheel_lin_speed_mps_fl) -
       (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[1] *
        SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[0])) -
      (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[2] *
       SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[1])) -
     (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[3] *
      SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[2])) -
    (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_den[4] *
     SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[3]);
  rtb_swa_bias_converged_out =
    ((((SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[0] *
        SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_tmp) +
       (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[1] *
        SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[0])) +
      (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[2] *
       SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[1])) +
     (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[3] *
      SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[2])) +
    (SteeringWheelAngleBiasEstimation_DW.wheel_speed_filter_num[4] *
     SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[3]);

  // If: '<S2>/If' incorporates:
  //   Abs: '<S2>/Abs'
  //   Constant: '<S12>/Constant'
  //   Constant: '<S13>/Constant'
  //   Constant: '<S14>/Constant'
  //   Constant: '<S15>/Constant'
  //   Constant: '<S16>/Constant'
  //   Constant: '<S17>/Constant'
  //   Constant: '<S18>/Constant'
  //   Logic: '<S2>/Logical Operator'
  //   RelationalOperator: '<S12>/Compare'
  //   RelationalOperator: '<S13>/Compare'
  //   RelationalOperator: '<S14>/Compare'
  //   RelationalOperator: '<S15>/Compare'
  //   RelationalOperator: '<S16>/Compare'
  //   RelationalOperator: '<S17>/Compare'
  //   RelationalOperator: '<S18>/Compare'
  //   Sum: '<S2>/Subtract'

  if ((((((((*rtu_PressureValue_LHF) >= 15.0F) && ((*rtu_PressureValue_LHF) <=
            55.0F)) && ((*rtu_PressureValue_RHF) >= 15.0F)) &&
         ((*rtu_PressureValue_RHF) <= 55.0F)) && ((*rtu_k_veh_track) > 1.0E-6F))
       && (fabsf((*rtu_PressureValue_RHF) - (*rtu_PressureValue_LHF)) <= 4.0F)) &&
      (((uint32_T)(*rtu_raw_speed_qf)) == ACCURATED)) {
    // Outputs for IfAction SubSystem: '<S2>/If Action Subsystem' incorporates:
    //   ActionPort: '<S19>/Action Port'

    // SignalConversion: '<S19>/OutportBuffer_InsertedFor_f_use_yawrate_using_wheel_speeds_at_inport_0' incorporates:
    //   Constant: '<S19>/Constant'

    rtb_f_use_yawrate_using_wheel_speeds = true;

    // Product: '<S19>/Divide' incorporates:
    //   DiscreteFilter: '<S31>/wheel speed filter left'
    //   DiscreteFilter: '<S31>/wheel speed filter right'
    //   Sum: '<S26>/Subtract'

    rtb_swa_bias_converged_out_e = (rtb_swa_bias_converged_out - numAccum) /
      (*rtu_k_veh_track);

    // End of Outputs for SubSystem: '<S2>/If Action Subsystem'
  } else {
    // Outputs for IfAction SubSystem: '<S2>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S20>/Action Port'

    // SignalConversion: '<S20>/OutportBuffer_InsertedFor_yawrate_using_wheel_speeds_at_inport_0' incorporates:
    //   Constant: '<S20>/Constant1'

    rtb_swa_bias_converged_out_e = 0.0F;

    // SignalConversion: '<S20>/OutportBuffer_InsertedFor_f_use_yawrate_using_wheel_speeds_at_inport_0' incorporates:
    //   Constant: '<S20>/Constant'

    rtb_f_use_yawrate_using_wheel_speeds = false;

    // End of Outputs for SubSystem: '<S2>/If Action Subsystem1'
  }

  // End of If: '<S2>/If'

  // Gain: '<S5>/Gain' incorporates:
  //   DiscreteFilter: '<S31>/wheel speed filter left'
  //   DiscreteFilter: '<S31>/wheel speed filter right'
  //   Sum: '<S5>/Add'

  rtb_Gain = (numAccum + rtb_swa_bias_converged_out) * 0.5F;

  // DiscreteFilter: '<S24>/vel to accel filter'
  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_tmp =
    ((((*rtu_filt_veh_speed_over_ground) -
       (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_den[1] *
        SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[0])) -
      (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_den[2] *
       SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[1])) -
     (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_den[3] *
      SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[2])) -
    (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_den[4] *
     SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[3]);

  // DiscreteFilter: '<S25>/vel to accel filter' incorporates:
  //   DiscreteFilter: '<S24>/vel to accel filter'

  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_tmp_d =
    ((((((SteeringWheelAngleBiasEstimation_DW.host_speed_filter_num[0] *
          SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_tmp) +
         (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_num[1] *
          SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[0])) +
        (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_num[2] *
         SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[1])) +
       (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_num[3] *
        SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[2])) +
      (SteeringWheelAngleBiasEstimation_DW.host_speed_filter_num[4] *
       SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[3])) -
     (SteeringWheelAngleBiasEstimation_DW.diff_denom[1] *
      SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states_m[0])) -
    (SteeringWheelAngleBiasEstimation_DW.diff_denom[2] *
     SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states_m[1]);

  // DiscreteFilter: '<S23>/accel filter' incorporates:
  //   DiscreteFilter: '<S25>/vel to accel filter'

  SteeringWheelAngleBiasEstimation_DW.accelfilter_tmp =
    ((((((SteeringWheelAngleBiasEstimation_DW.diff_num[0] *
          SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_tmp_d) +
         (SteeringWheelAngleBiasEstimation_DW.diff_num[1] *
          SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states_m[0])) +
        (SteeringWheelAngleBiasEstimation_DW.diff_num[2] *
         SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states_m[1])) -
       (SteeringWheelAngleBiasEstimation_DW.accel_filter_den[1] *
        SteeringWheelAngleBiasEstimation_DW.accelfilter_states[0])) -
      (SteeringWheelAngleBiasEstimation_DW.accel_filter_den[2] *
       SteeringWheelAngleBiasEstimation_DW.accelfilter_states[1])) -
     (SteeringWheelAngleBiasEstimation_DW.accel_filter_den[3] *
      SteeringWheelAngleBiasEstimation_DW.accelfilter_states[2])) -
    (SteeringWheelAngleBiasEstimation_DW.accel_filter_den[4] *
     SteeringWheelAngleBiasEstimation_DW.accelfilter_states[3]);
  numAccum = ((((SteeringWheelAngleBiasEstimation_DW.accel_filter_num[0] *
                 SteeringWheelAngleBiasEstimation_DW.accelfilter_tmp) +
                (SteeringWheelAngleBiasEstimation_DW.accel_filter_num[1] *
                 SteeringWheelAngleBiasEstimation_DW.accelfilter_states[0])) +
               (SteeringWheelAngleBiasEstimation_DW.accel_filter_num[2] *
                SteeringWheelAngleBiasEstimation_DW.accelfilter_states[1])) +
              (SteeringWheelAngleBiasEstimation_DW.accel_filter_num[3] *
               SteeringWheelAngleBiasEstimation_DW.accelfilter_states[2])) +
    (SteeringWheelAngleBiasEstimation_DW.accel_filter_num[4] *
     SteeringWheelAngleBiasEstimation_DW.accelfilter_states[3]);
  SteeringWheelAngleBiasEstimation_DW.accelfilter_states[3] =
    SteeringWheelAngleBiasEstimation_DW.accelfilter_states[2];
  SteeringWheelAngleBiasEstimation_DW.accelfilter_states[2] =
    SteeringWheelAngleBiasEstimation_DW.accelfilter_states[1];
  SteeringWheelAngleBiasEstimation_DW.accelfilter_states[1] =
    SteeringWheelAngleBiasEstimation_DW.accelfilter_states[0];
  SteeringWheelAngleBiasEstimation_DW.accelfilter_states[0] =
    SteeringWheelAngleBiasEstimation_DW.accelfilter_tmp;

  // Switch: '<S2>/Switch' incorporates:
  //   Constant: '<S2>/Constant'
  //   Constant: '<S2>/Constant1'

  if (rtb_f_use_yawrate_using_wheel_speeds) {
    rtb_Switch = ACCURATED;
  } else {
    rtb_Switch = UNDEFINED;
  }

  // End of Switch: '<S2>/Switch'

  // MATLAB Function: '<S11>/SWABiasOperatingCondition' incorporates:
  //   Constant: '<S11>/Constant10'
  //   Constant: '<S11>/Constant11'
  //   Constant: '<S11>/Constant2'
  //   Constant: '<S11>/Constant3'
  //   Constant: '<S11>/Constant4'
  //   Constant: '<S45>/Constant'
  //   Constant: '<S46>/Constant'
  //   Constant: '<S47>/Constant'
  //   Constant: '<S48>/Constant'
  //   DiscreteFilter: '<S23>/accel filter'
  //   Logic: '<S11>/Logical Operator'
  //   RelationalOperator: '<S45>/Compare'
  //   RelationalOperator: '<S46>/Compare'
  //   RelationalOperator: '<S47>/Compare'
  //   RelationalOperator: '<S48>/Compare'

  rtb_swa_bias_converged_out = 0.0F;
  rtb_f_consider_SWA_Bias = false;
  if (((((((uint32_T)(*rtu_raw_speed_qf)) == ACCURATED) && (((uint32_T)
           rtb_Switch) == ACCURATED)) && rtb_f_use_yawrate_using_wheel_speeds) &&
       (((uint32_T)(*rtu_comp_lat_accel_qf)) == ACCURATED)) && (((uint32_T)
        (*rtu_raw_steering_angle_qf)) == ACCURATED)) {
    if (rtb_Gain > 0.67F) {
      rtb_swa_bias_converged_out_e /= rtb_Gain;
    } else {
      rtb_swa_bias_converged_out_e /= 0.67F;
    }

    if ((((fabsf(*rtu_comp_lat_accel) < 0.5F) && (fabsf(numAccum) < 1.0F)) &&
         (rtb_Gain > 6.95F)) && (fabsf(rtb_swa_bias_converged_out_e) <=
         0.0001429F)) {
      rtb_swa_bias_converged_out = *rtu_raw_steering_angle_deg;
      rtb_f_consider_SWA_Bias = true;
    }
  }

  // End of MATLAB Function: '<S11>/SWABiasOperatingCondition'

  // MATLAB Function: '<S11>/SWABias_Level1Convergence' incorporates:
  //   Constant: '<S11>/Constant12'
  //   UnitDelay: '<S11>/Unit Delay'
  //   UnitDelay: '<S11>/Unit Delay1'

  SteeringWheelAngleBiasEstimation_SWABias_Level1Convergence
    (rtb_swa_bias_converged_out, rtb_f_consider_SWA_Bias,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay1_DSTATE,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay_DSTATE, 100,
     &rtb_swa_bias_converged_out_e, &rtb_f_use_yawrate_using_wheel_speeds,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay1_DSTATE,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay_DSTATE);

  // MATLAB Function: '<S11>/SWABias_Level2Convergence' incorporates:
  //   Constant: '<S11>/Constant13'
  //   Constant: '<S11>/Constant14'
  //   Constant: '<S11>/Constant15'
  //   UnitDelay: '<S11>/Unit Delay2'
  //   UnitDelay: '<S11>/Unit Delay3'
  //   UnitDelay: '<S11>/Unit Delay4'
  //   UnitDelay: '<S11>/Unit Delay5'
  //   UnitDelay: '<S11>/Unit Delay6'

  SteeringWheelAngleBiasEstimation_SWABias_Level2Convergence
    (SteeringWheelAngleBiasEstimation_DW.UnitDelay2_DSTATE,
     SteeringWheelAngleBiasEstimation_DW.bitsForTID0.UnitDelay3_DSTATE,
     rtb_swa_bias_converged_out_e, rtb_f_use_yawrate_using_wheel_speeds,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay4_DSTATE,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay5_DSTATE,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay6_DSTATE, 30, 0.25F, 0.5F,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay2_DSTATE,
     &rtb_f_consider_SWA_Bias,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay4_DSTATE,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay5_DSTATE,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay6_DSTATE);

  // Rounding: '<S44>/Floor' incorporates:
  //   Gain: '<S44>/C_100_SINGLE'
  //   UnitDelay: '<S11>/Unit Delay5'

  rtb_swa_bias_converged_out_e = roundf(100.0F *
    SteeringWheelAngleBiasEstimation_DW.UnitDelay5_DSTATE);

  // MATLAB Function: '<S11>/NVM interaction' incorporates:
  //   Gain: '<S44>/1byC_100_SINGLE'

  SteeringWheelAngleBiasEstimation_NVMinteraction(0.01F *
    rtb_swa_bias_converged_out_e, rtb_f_consider_SWA_Bias,
    rtu_nvm_last_rem_swa_bias_deg, rtu_nvm_f_last_rem_swa_bias,
    &rtb_f_use_yawrate_using_wheel_speeds, &rtb_swa_bias_converged_out);

  // MATLAB Function: '<S10>/SWABiasOperatingCondition' incorporates:
  //   Constant: '<S10>/Constant10'
  //   Constant: '<S10>/Constant11'
  //   Constant: '<S10>/Constant2'
  //   Constant: '<S10>/Constant3'
  //   Constant: '<S10>/Constant4'
  //   Constant: '<S34>/Constant'
  //   Constant: '<S35>/Constant'
  //   Constant: '<S36>/Constant'
  //   Constant: '<S37>/Constant'
  //   DiscreteFilter: '<S23>/accel filter'
  //   Logic: '<S10>/Logical Operator'
  //   RelationalOperator: '<S34>/Compare'
  //   RelationalOperator: '<S35>/Compare'
  //   RelationalOperator: '<S36>/Compare'
  //   RelationalOperator: '<S37>/Compare'

  rtb_Gain = 0.0F;
  rtb_f_consider_SWA_Bias_a = false;
  if (((((((uint32_T)(*rtu_filt_veh_speed_over_ground_qf)) == ACCURATED) &&
         (((uint32_T)(*rtu_comp_yaw_rate_qf)) == ACCURATED)) &&
        (*rtu_f_yaw_rate_stop_bias_converged)) && (((uint32_T)
         (*rtu_comp_lat_accel_qf)) == ACCURATED)) && (((uint32_T)
        (*rtu_raw_steering_angle_qf)) == ACCURATED)) {
    if ((*rtu_filt_veh_speed_over_ground) > 0.67F) {
      rtb_swa_bias_converged_out_e = (*rtu_comp_yaw_rate_filtered) /
        (*rtu_filt_veh_speed_over_ground);
    } else {
      rtb_swa_bias_converged_out_e = (*rtu_comp_yaw_rate_filtered) / 0.67F;
    }

    if ((((fabsf(*rtu_comp_lat_accel) < 0.5F) && (fabsf(numAccum) < 1.0F)) && ((*
           rtu_filt_veh_speed_over_ground) > 6.95F)) && (fabsf
         (rtb_swa_bias_converged_out_e) <= 0.0001429F)) {
      rtb_Gain = *rtu_raw_steering_angle_deg;
      rtb_f_consider_SWA_Bias_a = true;
    }
  }

  // End of MATLAB Function: '<S10>/SWABiasOperatingCondition'

  // MATLAB Function: '<S10>/SWABias_Level1Convergence' incorporates:
  //   Constant: '<S10>/Constant12'
  //   UnitDelay: '<S10>/Unit Delay'
  //   UnitDelay: '<S10>/Unit Delay1'

  SteeringWheelAngleBiasEstimation_SWABias_Level1Convergence(rtb_Gain,
    rtb_f_consider_SWA_Bias_a,
    SteeringWheelAngleBiasEstimation_DW.UnitDelay1_DSTATE_n,
    SteeringWheelAngleBiasEstimation_DW.UnitDelay_DSTATE_a, 100,
    &rtb_swa_bias_converged_out_e, &rtb_f_consider_swa_bias_conv_out_k,
    &SteeringWheelAngleBiasEstimation_DW.UnitDelay1_DSTATE_n,
    &SteeringWheelAngleBiasEstimation_DW.UnitDelay_DSTATE_a);

  // MATLAB Function: '<S10>/SWABias_Level2Convergence' incorporates:
  //   Constant: '<S10>/Constant13'
  //   Constant: '<S10>/Constant14'
  //   Constant: '<S10>/Constant15'
  //   UnitDelay: '<S10>/Unit Delay2'
  //   UnitDelay: '<S10>/Unit Delay3'
  //   UnitDelay: '<S10>/Unit Delay4'
  //   UnitDelay: '<S10>/Unit Delay5'
  //   UnitDelay: '<S10>/Unit Delay6'

  SteeringWheelAngleBiasEstimation_SWABias_Level2Convergence
    (SteeringWheelAngleBiasEstimation_DW.UnitDelay2_DSTATE_p,
     SteeringWheelAngleBiasEstimation_DW.bitsForTID0.UnitDelay3_DSTATE_m,
     rtb_swa_bias_converged_out_e, rtb_f_consider_swa_bias_conv_out_k,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay4_DSTATE_h,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay5_DSTATE_c,
     SteeringWheelAngleBiasEstimation_DW.UnitDelay6_DSTATE_d, 30, 0.25F, 0.5F,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay2_DSTATE_p,
     &rtb_f_consider_SWA_Bias_a,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay4_DSTATE_h,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay5_DSTATE_c,
     &SteeringWheelAngleBiasEstimation_DW.UnitDelay6_DSTATE_d);

  // Chart: '<S1>/Chart'
  if (((uint32_T)
       SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c2_SteeringWheelAngleBiasEstimation)
      == 0U) {
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c2_SteeringWheelAngleBiasEstimation
      = 1;
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c2_SteeringWheelAngleBiasEstimation
      = SteeringWheelAngleBiasEstimation_IN_Initialize;
    SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_m =
      NOT_ACCURATED;
  } else {
    switch
      (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c2_SteeringWheelAngleBiasEstimation)
    {
     case SteeringWheelAngleBiasEstimation_IN_Initialize:
      if ((((uint32_T)(*rtu_raw_steering_angle_qf)) == ACCURATED) &&
          rtb_f_consider_SWA_Bias_a) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c2_SteeringWheelAngleBiasEstimation
          = SteeringWheelAngleBiasEstimation_IN_Running;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_m =
          ACCURATED;
      }
      break;

     case SteeringWheelAngleBiasEstimation_IN_Running:
      if (((uint32_T)(*rtu_raw_steering_angle_qf)) != ACCURATED) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c2_SteeringWheelAngleBiasEstimation
          = SteeringWheelAngleBiasEstimation_IN_not_converged;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_m =
          *rtu_raw_steering_angle_qf;
      }
      break;

     default:
      if (((uint32_T)(*rtu_raw_steering_angle_qf)) == ACCURATED) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c2_SteeringWheelAngleBiasEstimation
          = SteeringWheelAngleBiasEstimation_IN_Running;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_m =
          ACCURATED;
      } else {
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_m =
          *rtu_raw_steering_angle_qf;
      }
      break;
    }
  }

  // End of Chart: '<S1>/Chart'

  // RelationalOperator: '<S4>/Compare' incorporates:
  //   Constant: '<S4>/Constant'

  rtb_Compare_mv = (((uint32_T)(*rtu_steering_angle_bias_qf_external)) ==
                    ACCURATED);

  // Rounding: '<S33>/Floor' incorporates:
  //   Gain: '<S33>/C_100_SINGLE'
  //   UnitDelay: '<S10>/Unit Delay5'

  rtb_swa_bias_converged_out_e = roundf(100.0F *
    SteeringWheelAngleBiasEstimation_DW.UnitDelay5_DSTATE_c);

  // MATLAB Function: '<S10>/NVM interaction' incorporates:
  //   Gain: '<S33>/1byC_100_SINGLE'

  SteeringWheelAngleBiasEstimation_NVMinteraction(0.01F *
    rtb_swa_bias_converged_out_e, rtb_f_consider_SWA_Bias_a,
    rtu_nvm_last_rem_swa_bias_deg, rtu_nvm_f_last_rem_swa_bias,
    &rtb_f_consider_swa_bias_conv_out_k, &rtb_swa_bias_converged_out_e);

  // Logic: '<S1>/NOT'
  rtb_f_do_not_use_yawrate = !rtb_f_consider_swa_bias_conv_out_k;

  // MATLAB Function: '<S9>/MATLAB Function' incorporates:
  //   Constant: '<S1>/Constant1'
  //   Constant: '<S1>/Constant2'

  tmp_5 = !SteeringWheelAngleBiasEstimation_DW.bitsForTID0.prev_final_fault_flag;
  if (rtb_f_do_not_use_yawrate && tmp_5) {
    SteeringWheelAngleBiasEstimation_DW.matured_time += 0.01F;
  } else {
    if (!rtb_f_do_not_use_yawrate) {
      SteeringWheelAngleBiasEstimation_DW.matured_time = 0.0F;
    }
  }

  tmp_6 = !rtb_f_do_not_use_yawrate;
  if (tmp_6 &&
      (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.prev_final_fault_flag)) {
    SteeringWheelAngleBiasEstimation_DW.dematured_time += 0.01F;
  } else {
    if (!SteeringWheelAngleBiasEstimation_DW.bitsForTID0.prev_final_fault_flag)
    {
      SteeringWheelAngleBiasEstimation_DW.dematured_time = 0.0F;
    }
  }

  rtb_f_do_not_use_yawrate = (((rtb_f_do_not_use_yawrate && tmp_5) &&
    (SteeringWheelAngleBiasEstimation_DW.matured_time >= 3.0F)) || ((tmp_6 ||
    (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.prev_final_fault_flag)) &&
    (((rtb_f_do_not_use_yawrate || tmp_5) ||
      (SteeringWheelAngleBiasEstimation_DW.dematured_time < 0.0F)) && ((tmp_6 &&
    (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.prev_final_fault_flag)) ||
    rtb_f_do_not_use_yawrate))));
  SteeringWheelAngleBiasEstimation_DW.bitsForTID0.prev_final_fault_flag =
    rtb_f_do_not_use_yawrate;

  // End of MATLAB Function: '<S9>/MATLAB Function'

  // Chart: '<S11>/compensated vehicle steering angle quality factor' incorporates:
  //   RelationalOperator: '<S46>/Compare'

  if (((uint32_T)
       SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c17_YawRateBasedSWABiasEstimation)
      == 0U) {
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c17_YawRateBasedSWABiasEstimation
      = 1;
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation
      = SteeringWheelAngleBiasEstimation_IN_Initialize;
    SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf =
      NOT_ACCURATED;
  } else {
    switch
      (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation)
    {
     case SteeringWheelAngleBiasEstimation_IN_Initialize:
      if ((((((uint32_T)(*rtu_raw_speed_qf)) == ACCURATED) && (((uint32_T)
              (*rtu_raw_steering_angle_qf)) == ACCURATED)) && (((uint32_T)
             rtb_Switch) == ACCURATED)) && rtb_f_consider_SWA_Bias) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation
          = SteeringWheelAngleBiasEstimation_IN_Running;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf =
          ACCURATED;
      }
      break;

     case SteeringWheelAngleBiasEstimation_IN_Running:
      if (((((uint32_T)(*rtu_raw_speed_qf)) != ACCURATED) || (((uint32_T)
             (*rtu_raw_steering_angle_qf)) != ACCURATED)) || (((uint32_T)
            rtb_Switch) != ACCURATED)) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation
          = SteeringWheelAngleBiasEstimation_IN_not_converged;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf =
          *rtu_raw_steering_angle_qf;
      }
      break;

     default:
      if (((((uint32_T)(*rtu_raw_speed_qf)) == ACCURATED) && (((uint32_T)
             (*rtu_raw_steering_angle_qf)) == ACCURATED)) && (((uint32_T)
            rtb_Switch) == ACCURATED)) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation
          = SteeringWheelAngleBiasEstimation_IN_Running;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf =
          ACCURATED;
      } else {
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf =
          *rtu_raw_steering_angle_qf;
      }
      break;
    }
  }

  // End of Chart: '<S11>/compensated vehicle steering angle quality factor'

  // Chart: '<S10>/compensated vehicle steering angle quality factor'
  if (((uint32_T)
       SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c17_YawRateBasedSWABiasEstimation_o)
      == 0U) {
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_active_c17_YawRateBasedSWABiasEstimation_o
      = 1;
    SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation_b
      = SteeringWheelAngleBiasEstimation_IN_Initialize;
    SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_b =
      NOT_ACCURATED;
  } else {
    switch
      (SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation_b)
    {
     case SteeringWheelAngleBiasEstimation_IN_Initialize:
      if ((((((uint32_T)(*rtu_filt_veh_speed_over_ground_qf)) == ACCURATED) &&
            (((uint32_T)(*rtu_raw_steering_angle_qf)) == ACCURATED)) &&
           (((uint32_T)(*rtu_comp_yaw_rate_qf)) == ACCURATED)) &&
          rtb_f_consider_SWA_Bias_a) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation_b
          = SteeringWheelAngleBiasEstimation_IN_Running;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_b =
          ACCURATED;
      }
      break;

     case SteeringWheelAngleBiasEstimation_IN_Running:
      if (((((uint32_T)(*rtu_filt_veh_speed_over_ground_qf)) != ACCURATED) ||
           (((uint32_T)(*rtu_raw_steering_angle_qf)) != ACCURATED)) ||
          (((uint32_T)(*rtu_comp_yaw_rate_qf)) != ACCURATED)) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation_b
          = SteeringWheelAngleBiasEstimation_IN_not_converged;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_b =
          *rtu_raw_steering_angle_qf;
      }
      break;

     default:
      if (((((uint32_T)(*rtu_filt_veh_speed_over_ground_qf)) == ACCURATED) &&
           (((uint32_T)(*rtu_raw_steering_angle_qf)) == ACCURATED)) &&
          (((uint32_T)(*rtu_comp_yaw_rate_qf)) == ACCURATED)) {
        SteeringWheelAngleBiasEstimation_DW.bitsForTID0.is_c17_YawRateBasedSWABiasEstimation_b
          = SteeringWheelAngleBiasEstimation_IN_Running;
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_b =
          ACCURATED;
      } else {
        SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_b =
          *rtu_raw_steering_angle_qf;
      }
      break;
    }
  }

  // End of Chart: '<S10>/compensated vehicle steering angle quality factor'

  // If: '<S1>/If' incorporates:
  //   Logic: '<S1>/Logical Operator1'
  //   Sum: '<S10>/Add'
  //   Sum: '<S11>/Add'

  if ((!rtb_f_do_not_use_yawrate) || ((!rtb_Compare_mv) &&
       (!rtb_f_use_yawrate_using_wheel_speeds))) {
    // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem' incorporates:
    //   ActionPort: '<S6>/Action Port'

    SteeringWheelAngleBiasEstimation_IfActionSubsystem
      (rtb_f_consider_swa_bias_conv_out_k, rtb_swa_bias_converged_out_e,
       (*rtu_raw_steering_angle_deg) - rtb_swa_bias_converged_out_e,
       SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_b,
       rtb_f_consider_SWA_Bias_a, rty_f_consider_swa_bias_conv_out,
       rty_swa_bias_converged_out, rty_comp_steering_angle_deg,
       rty_comp_steering_angle_qf, rty_f_consider_swa_bias_conv_internal);

    // End of Outputs for SubSystem: '<S1>/If Action Subsystem'
  } else if (rtb_Compare_mv) {
    // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S8>/Action Port'

    // SignalConversion: '<S8>/BusConversion_InsertedFor_Out1_at_inport_0'
    *rty_f_consider_swa_bias_conv_out = true;

    // SignalConversion: '<S8>/BusConversion_InsertedFor_Out1_at_inport_0'
    *rty_swa_bias_converged_out = *rtu_steering_angle_bias_deg_external;

    // SignalConversion: '<S8>/BusConversion_InsertedFor_Out1_at_inport_0' incorporates:
    //   Sum: '<S1>/Add'

    *rty_comp_steering_angle_deg = (*rtu_raw_steering_angle_deg) -
      (*rtu_steering_angle_bias_deg_external);

    // SignalConversion: '<S8>/BusConversion_InsertedFor_Out1_at_inport_0'
    *rty_comp_steering_angle_qf =
      SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf_m;

    // SignalConversion: '<S8>/BusConversion_InsertedFor_Out1_at_inport_0'
    *rty_f_consider_swa_bias_conv_internal = rtb_f_consider_SWA_Bias_a;

    // End of Outputs for SubSystem: '<S1>/If Action Subsystem2'
  } else {
    // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S7>/Action Port'

    SteeringWheelAngleBiasEstimation_IfActionSubsystem
      (rtb_f_use_yawrate_using_wheel_speeds, rtb_swa_bias_converged_out,
       (*rtu_raw_steering_angle_deg) - rtb_swa_bias_converged_out,
       SteeringWheelAngleBiasEstimation_DW.comp_veh_steering_angle_qf,
       rtb_f_consider_SWA_Bias, rty_f_consider_swa_bias_conv_out,
       rty_swa_bias_converged_out, rty_comp_steering_angle_deg,
       rty_comp_steering_angle_qf, rty_f_consider_swa_bias_conv_internal);

    // End of Outputs for SubSystem: '<S1>/If Action Subsystem1'
  }

  // End of If: '<S1>/If'

  // Update for DiscreteFilter: '<S31>/wheel speed filter right'
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[3] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[2];
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[2] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[1];
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[1] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[0];
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_states[0] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterright_tmp;

  // Update for DiscreteFilter: '<S31>/wheel speed filter left'
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[3] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[2];
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[2] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[1];
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[1] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[0];
  SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_states[0] =
    SteeringWheelAngleBiasEstimation_DW.wheelspeedfilterleft_tmp;

  // Update for DiscreteFilter: '<S24>/vel to accel filter'
  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[3] =
    SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[2];
  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[2] =
    SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[1];
  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[1] =
    SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[0];
  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states[0] =
    SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_tmp;

  // Update for DiscreteFilter: '<S25>/vel to accel filter'
  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states_m[1] =
    SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states_m[0];
  SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_states_m[0] =
    SteeringWheelAngleBiasEstimation_DW.veltoaccelfilter_tmp_d;

  // Update for UnitDelay: '<S11>/Unit Delay3'
  SteeringWheelAngleBiasEstimation_DW.bitsForTID0.UnitDelay3_DSTATE =
    rtb_f_consider_SWA_Bias;

  // Update for UnitDelay: '<S10>/Unit Delay3'
  SteeringWheelAngleBiasEstimation_DW.bitsForTID0.UnitDelay3_DSTATE_m =
    rtb_f_consider_SWA_Bias_a;
}

// Constructor
SteeringWheelAngleBiasEstimationModelClass::
  SteeringWheelAngleBiasEstimationModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
SteeringWheelAngleBiasEstimationModelClass::
  ~SteeringWheelAngleBiasEstimationModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
