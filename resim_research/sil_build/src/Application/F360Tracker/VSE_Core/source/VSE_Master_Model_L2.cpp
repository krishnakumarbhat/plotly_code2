//
// File: VSE_Master_Model_L2.cpp
//
// Code generated for Simulink model 'VSE_Master_Model_L2'.
//
// Model version                  : 1.794
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 19:00:41 2024
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
#include "VSE_Master_Model_L2.h"
#include "VSE_Master_Model_L2_private.h"

// Named constants for Chart: '<S4>/Initialization_Trigger'
#define VSE_Master_Model_L2_IN_Initialize ((uint8_T)1U)
#define VSE_Master_Model_L2_IN_Running ((uint8_T)2U)

// Named constants for Chart: '<S3>/Chart'
#define VSE_Master_Model_L2_IN_Initialize_l ((uint8_T)1U)
#define VSE_Master_Model_L2_IN_Running_d ((uint8_T)2U)

//
// Output and update for enable system:
//    '<S4>/Initialization'
//    '<S5>/Initialization'
//    '<S14>/Initialization'
//    '<S15>/Initialization'
//    '<S17>/Initialization'
//
void VSE_Master_Model_L2_Initialization(boolean_T rtu_Enable, real32_T
  rty_diff_num[4], real32_T rty_diff_den[4])
{
  // Outputs for Enabled SubSystem: '<S4>/Initialization' incorporates:
  //   EnablePort: '<S42>/Enable'

  if (rtu_Enable) {
    // MATLAB Function: '<S42>/MATLAB Function'
    rty_diff_num[0] = 0.724336267F;
    rty_diff_den[0] = 1.0F;
    rty_diff_num[1] = 0.724336267F;
    rty_diff_den[1] = -0.822694957F;
    rty_diff_num[2] = -0.724336267F;
    rty_diff_den[2] = -0.98551327F;
    rty_diff_num[3] = -0.724336267F;
    rty_diff_den[3] = 0.837181628F;
  }

  // End of Outputs for SubSystem: '<S4>/Initialization'
}

//
// Output and update for atomic system:
//    '<S4>/Initialization_Trigger'
//    '<S5>/Initialization_Trigger'
//    '<S14>/Initialization_Trigger'
//    '<S15>/Initialization_Trigger'
//    '<S17>/Initialization_Trigger'
//
void VSE_Master_Model_L2_Initialization_Trigger(boolean_T *rty_f_initialize,
  DW_Initialization_Trigger_VSE_Master_Model_L2_T *localDW)
{
  // Chart: '<S4>/Initialization_Trigger'
  if (((uint32_T)localDW->bitsForTID0.is_active_c5_Initialization_Trigger) == 0U)
  {
    localDW->bitsForTID0.is_active_c5_Initialization_Trigger = 1;
    localDW->bitsForTID0.is_c5_Initialization_Trigger =
      VSE_Master_Model_L2_IN_Initialize;
    *rty_f_initialize = true;
  } else if (((uint32_T)localDW->bitsForTID0.is_c5_Initialization_Trigger) ==
             VSE_Master_Model_L2_IN_Initialize) {
    if (*rty_f_initialize) {
      localDW->bitsForTID0.is_c5_Initialization_Trigger =
        VSE_Master_Model_L2_IN_Running;
      *rty_f_initialize = false;
    }
  } else {
    *rty_f_initialize = false;
  }

  // End of Chart: '<S4>/Initialization_Trigger'
}

//
// Output and update for atomic system:
//    '<S66>/MATLAB Function'
//    '<S69>/MATLAB Function'
//    '<S72>/MATLAB Function'
//    '<S75>/MATLAB Function'
//    '<S78>/MATLAB Function'
//    '<S81>/MATLAB Function'
//    '<S84>/MATLAB Function'
//
void VSE_Master_Model_L2_MATLABFunction(boolean_T rtu_fault_flag, real32_T
  rtu_fault_maturation_time_seconds, real32_T
  rtu_fault_dematuration_time_seconds, boolean_T *rty_final_fault,
  DW_MATLABFunction_VSE_Master_Model_L2_T *localDW)
{
  boolean_T tmp;
  boolean_T tmp_0;
  tmp = !localDW->bitsForTID0.prev_final_fault_flag;
  if (rtu_fault_flag && tmp) {
    localDW->matured_time += 0.01F;
  } else {
    if (!rtu_fault_flag) {
      localDW->matured_time = 0.0F;
    }
  }

  tmp_0 = !rtu_fault_flag;
  if (tmp_0 && (localDW->bitsForTID0.prev_final_fault_flag)) {
    localDW->dematured_time += 0.01F;
  } else {
    if (!localDW->bitsForTID0.prev_final_fault_flag) {
      localDW->dematured_time = 0.0F;
    }
  }

  *rty_final_fault = (((rtu_fault_flag && tmp) && (localDW->matured_time >=
    rtu_fault_maturation_time_seconds)) || ((tmp_0 ||
    (localDW->bitsForTID0.prev_final_fault_flag)) && (((rtu_fault_flag || tmp) ||
    (localDW->dematured_time < rtu_fault_dematuration_time_seconds)) && ((tmp_0 &&
    (localDW->bitsForTID0.prev_final_fault_flag)) || rtu_fault_flag))));
  localDW->bitsForTID0.prev_final_fault_flag = *rty_final_fault;
}

// Model step function
void VSE_Master_Model_L2ModelClass::step()
{
  // local block i/o variables
  real32_T rtb_VsTracker_VehSpdCompFac;
  real32_T rtb_Model_o1;
  real32_T rtb_yawRate;
  real32_T rtb_Model6_o3;
  real32_T rtb_Yaw_Rate_Compensated_Unfiltered;
  real32_T rtb_Yaw_Rate_Raw_Bias;
  real32_T rtb_yaw_rate_bias1;
  real32_T rtb_f_yaw_stop_bias_converged;
  real32_T rtb_yaw_rate_bias_fast_bias1;
  real32_T rtb_yaw_rate_bias_fast_bias2;
  real32_T rtb_comp_yaw_rate_diff_filt;
  real32_T rtb_yaw_rate_bias_diff;
  real32_T rtb_ignition_time;
  real32_T rtb_curvature_rear_axle;
  real32_T rtb_VCS_long_velocity;
  real32_T rtb_sensor_long_velocity;
  real32_T rtb_VCS_sideslip;
  real32_T rtb_sensor_sideslip;
  real32_T rtb_sideslip_rear_axle;
  real32_T rtb_VCS_lat_velocity;
  real32_T rtb_sensor_lat_velocity;
  real32_T rtb_CurvKalmanFilterC0;
  real32_T rtb_CurvKalmanFilterC1;
  real32_T rtb_Acceleration_VCS_o1;
  real32_T rtb_Acceleration_VCS_o2;
  real32_T rtb_Model3_o1;
  real32_T rtb_Model3_o2;
  real32_T rtb_WheelRadiusEstimation_o1;
  real32_T rtb_WheelRadiusEstimation_o3;
  real32_T rtb_WheelRadiusEstimation_o5;
  real32_T rtb_WheelRadiusEstimation_o7;
  real32_T rtb_WheelRadiusEstimation_o9;
  real32_T rtb_signal_value_filtered;
  real32_T rtb_trailer_length;
  real32_T rtb_trailer_width;
  real32_T rtb_Model5_o8;
  real32_T rtb_Model5_o9;
  real32_T rtb_Model9_o1;
  real32_T rtb_Model9_o2;
  real32_T rtb_Gain;
  enum_road_type_T rtb_road_type;
  uint8_T rtb_Trailer_Detection_ConfLvl;
  uint8_T rtb_trailer_length_ConfLvl;
  uint8_T rtb_trailer_width_ConfLvl;
  enum_quality_factor_T rtb_Merge;
  enum_quality_factor_T rtb_VeTracker_VehSpdCompFac_QF;
  enum_quality_factor_T rtb_Model_o2;
  enum_quality_factor_T rtb_qf;
  enum_quality_factor_T rtb_Model6_o4;
  enum_quality_factor_T rtb_Yaw_Rate_Bias_QF;
  enum_quality_factor_T rtb_Acceleration_VCS_o3;
  enum_quality_factor_T rtb_Acceleration_VCS_o4;
  enum_quality_factor_T rtb_Model3_o3;
  enum_quality_factor_T rtb_Model3_o4;
  boolean_T rtb_f_stationary;
  boolean_T rtb_f_Yaw_Rate_Bias_Converged;
  boolean_T rtb_f_stop_bias_converged;
  boolean_T rtb_f_yaw_rate_bias_shift;
  boolean_T rtb_f_Yaw_Rate_Bias_Converged_j;
  boolean_T rtb_f_stop_bias_converged_n;
  boolean_T rtb_f_yaw_rate_steady;
  boolean_T rtb_f_input_invalid_persistent;
  boolean_T rtb_f_execution_period_error_persistent;
  boolean_T rtb_f_bias_was_accurate;
  boolean_T rtb_f_yaw_stop_bias_converged_d;
  boolean_T rtb_f_yaw_stop_bias_converged_m;
  boolean_T rtb_WheelRadiusEstimation_o10;
  boolean_T rtb_f_mass_converged;
  boolean_T rtb_Model5_o7;
  real32_T numAccum;
  real32_T denAccum;
  real32_T denAccum_0;
  real32_T denAccum_1;
  real32_T denAccum_2;
  real32_T denAccum_3;
  real_T denAccum_4;
  real_T denAccum_5;
  int32_T j;
  int32_T denIdx;
  boolean_T rtb_LogicalOperator;
  boolean_T rtb_LogicalOperator_b2;
  boolean_T rtb_LogicalOperator_ov;
  real_T rtb_Abs;
  real_T rtb_Switch;
  enum_quality_factor_T rtb_Merge_n;
  enum_quality_factor_T rtb_Merge_j;
  enum_quality_factor_T rtb_Merge_kq;
  enum_quality_factor_T rtb_Merge_b;
  static const real_T tmp[8] = { 2.7763343090014743E-5, 0.00013881671545007372,
    0.00024987008781013266, 0.00013881671545007372, -0.00013881671545007372,
    -0.00024987008781013266, -0.00013881671545007372, -2.7763343090014743E-5 };

  static const real_T tmp_0[8] = { 1.0, -4.4174748855531041, 6.8371489541927746,
    -2.55917168747596, -4.72189665753505, 6.4186206346112051,
    -3.1152434123881356, 0.55803482268744786 };

  static const real_T tmp_1[5] = { 0.0097355705758097114, -0.032135368264455524,
    0.045449987251035384, -0.032135368264455511, 0.0097355705758096975 };

  static const real_T tmp_2[5] = { 1.0, -3.5729428344521863, 4.8079147193565426,
    -2.8863252161892525, 0.65200372315863975 };

  uint32_T qY;

  // RelationalOperator: '<S29>/Compare' incorporates:
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S46>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector6_at_outport_1'

  rtb_Merge_b = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLatAccelQF;

  // Switch: '<S22>/Switch' incorporates:
  //   Constant: '<S29>/Constant'
  //   Delay: '<S22>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S29>/Compare'

  if (((uint32_T)rtb_Merge_b) != UNDEFINED) {
    VSE_Master_Model_L2_DW.Delay_DSTATE =
      VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps2_RawLatAccel;
  }

  // End of Switch: '<S22>/Switch'

  // Logic: '<S44>/Logical Operator' incorporates:
  //   Constant: '<S44>/Constant'
  //   Constant: '<S44>/Constant1'
  //   Delay: '<S22>/Delay'
  //   RelationalOperator: '<S44>/Equal'
  //   RelationalOperator: '<S44>/Equal1'

  rtb_LogicalOperator = ((VSE_Master_Model_L2_DW.Delay_DSTATE >= -15.0F) &&
    (VSE_Master_Model_L2_DW.Delay_DSTATE <= 15.0F));

  // RelationalOperator: '<S28>/Compare' incorporates:
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S91>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector3_at_outport_3'

  rtb_Merge_kq = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawVehSpeedQF;

  // Switch: '<S21>/Switch' incorporates:
  //   Constant: '<S28>/Constant'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S28>/Compare'

  if (((uint32_T)rtb_Merge_kq) != UNDEFINED) {
    VSE_Master_Model_L2_DW.Delay_DSTATE_i =
      VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_RawVehSpeed;
  }

  // End of Switch: '<S21>/Switch'

  // Logic: '<S89>/Logical Operator' incorporates:
  //   Constant: '<S89>/Constant'
  //   Constant: '<S89>/Constant1'
  //   Delay: '<S21>/Delay'
  //   RelationalOperator: '<S89>/Equal'
  //   RelationalOperator: '<S89>/Equal1'

  rtb_LogicalOperator_b2 = ((VSE_Master_Model_L2_DW.Delay_DSTATE_i >= 0.0F) &&
    (VSE_Master_Model_L2_DW.Delay_DSTATE_i <= 140.0F));

  // Chart: '<S16>/Chart'
  if (((uint32_T)
       VSE_Master_Model_L2_DW.bitsForTID0.is_active_c3_VSE_Master_Model_L2) ==
      0U) {
    VSE_Master_Model_L2_DW.bitsForTID0.is_active_c3_VSE_Master_Model_L2 = 1;
    VSE_Master_Model_L2_DW.bitsForTID0.is_c3_VSE_Master_Model_L2 =
      VSE_Master_Model_L2_IN_Initialize_l;
    VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_vse = true;
    VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex = 0U;
  } else if (((uint32_T)
              VSE_Master_Model_L2_DW.bitsForTID0.is_c3_VSE_Master_Model_L2) ==
             VSE_Master_Model_L2_IN_Initialize_l) {
    if (VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_vse) {
      VSE_Master_Model_L2_DW.bitsForTID0.is_c3_VSE_Master_Model_L2 =
        VSE_Master_Model_L2_IN_Running_d;
      VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_vse = false;
      qY = VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex + /*MW:OvSatOk*/ 1U;
      if (qY < VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex) {
        qY = MAX_uint32_T;
      }

      VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex = qY;
    }
  } else {
    VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_vse = false;
    qY = VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex + /*MW:OvSatOk*/ 1U;
    if (qY < VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex) {
      qY = MAX_uint32_T;
    }

    VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehIndex = qY;
  }

  // End of Chart: '<S16>/Chart'

  // Chart: '<S14>/Initialization_Trigger'
  VSE_Master_Model_L2_Initialization_Trigger
    (&VSE_Master_Model_L2_DW.f_initialize_f,
     &VSE_Master_Model_L2_DW.sf_Initialization_Trigger_a);

  // Outputs for Enabled SubSystem: '<S14>/Initialization'
  VSE_Master_Model_L2_Initialization(VSE_Master_Model_L2_DW.f_initialize_f,
    VSE_Master_Model_L2_DW.diff_num_m, VSE_Master_Model_L2_DW.diff_denom_g);

  // End of Outputs for SubSystem: '<S14>/Initialization'

  // DiscreteFilter: '<S14>/Discrete Filter' incorporates:
  //   Delay: '<S21>/Delay'

  denAccum = ((VSE_Master_Model_L2_DW.Delay_DSTATE_i -
               (VSE_Master_Model_L2_DW.diff_denom_g[1] *
                VSE_Master_Model_L2_DW.DiscreteFilter_states[0])) -
              (VSE_Master_Model_L2_DW.diff_denom_g[2] *
               VSE_Master_Model_L2_DW.DiscreteFilter_states[1])) -
    (VSE_Master_Model_L2_DW.diff_denom_g[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states[2]);
  numAccum = (((VSE_Master_Model_L2_DW.diff_num_m[0] * denAccum) +
               (VSE_Master_Model_L2_DW.diff_num_m[1] *
                VSE_Master_Model_L2_DW.DiscreteFilter_states[0])) +
              (VSE_Master_Model_L2_DW.diff_num_m[2] *
               VSE_Master_Model_L2_DW.DiscreteFilter_states[1])) +
    (VSE_Master_Model_L2_DW.diff_num_m[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states[2]);

  // Logic: '<S90>/Logical Operator' incorporates:
  //   Constant: '<S90>/Constant1'
  //   Constant: '<S90>/Constant2'
  //   DiscreteFilter: '<S14>/Discrete Filter'
  //   RelationalOperator: '<S90>/Equal'
  //   RelationalOperator: '<S90>/Equal1'

  rtb_LogicalOperator_ov = ((numAccum >= -1.0E+6F) && (numAccum <= 1.0E+6F));

  // If: '<S86>/If' incorporates:
  //   Constant: '<S86>/Constant'
  //   Constant: '<S86>/Constant1'
  //   Constant: '<S86>/Constant2'
  //   Constant: '<S86>/Constant3'
  //   Constant: '<S91>/Constant'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<S92>/In1'
  //   Inport: '<S93>/In1'
  //   Inport: '<S94>/In1'
  //   Inport: '<S95>/In1'
  //   Inport: '<S96>/In1'
  //   RelationalOperator: '<S91>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector3_at_outport_3'

  if (((uint32_T)rtb_Merge_kq) != ACCURATED) {
    // Outputs for IfAction SubSystem: '<S86>/If Action Subsystem' incorporates:
    //   ActionPort: '<S92>/Action Port'

    rtb_Merge = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawVehSpeedQF;

    // End of Outputs for SubSystem: '<S86>/If Action Subsystem'
  } else if (rtb_LogicalOperator_b2 && rtb_LogicalOperator_ov) {
    // Outputs for IfAction SubSystem: '<S86>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S93>/Action Port'

    rtb_Merge = ACCURATED;

    // End of Outputs for SubSystem: '<S86>/If Action Subsystem1'
  } else if ((!rtb_LogicalOperator_b2) && rtb_LogicalOperator_ov) {
    // Outputs for IfAction SubSystem: '<S86>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S94>/Action Port'

    rtb_Merge = TEMP_UNDEFINED;

    // End of Outputs for SubSystem: '<S86>/If Action Subsystem2'
  } else if (rtb_LogicalOperator_b2 && (!rtb_LogicalOperator_ov)) {
    // Outputs for IfAction SubSystem: '<S86>/If Action Subsystem3' incorporates:
    //   ActionPort: '<S95>/Action Port'

    rtb_Merge = NOT_ACCURATED;

    // End of Outputs for SubSystem: '<S86>/If Action Subsystem3'
  } else {
    // Outputs for IfAction SubSystem: '<S86>/If Action Subsystem4' incorporates:
    //   ActionPort: '<S96>/Action Port'

    rtb_Merge = UNDEFINED;

    // End of Outputs for SubSystem: '<S86>/If Action Subsystem4'
  }

  // End of If: '<S86>/If'

  // ModelReference: '<S1>/Model8' incorporates:
  //   Inport: '<Root>/Tracker_VSE'

  Model8MDLOBJ9.step
    (&VSE_Master_Model_L2_U.Tracker_VSE_j.VsTracker_VehSpdCompFac_1,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VeTracker_VehSpdCompFac_1_QF,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VsTracker_VehSpdCompFac_2,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VeTracker_VehSpdCompFac_2_QF,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VsTracker_VehSpdCompFac_3,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VeTracker_VehSpdCompFac_3_QF,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VsTracker_VehSpdCompFac_4,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VeTracker_VehSpdCompFac_4_QF,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VsTracker_VehSpdCompFac_5,
     &VSE_Master_Model_L2_U.Tracker_VSE_j.VeTracker_VehSpdCompFac_5_QF,
     &rtb_VsTracker_VehSpdCompFac, &rtb_VeTracker_VehSpdCompFac_QF);

  // ModelReference: '<S1>/Model' incorporates:
  //   Constant: '<S1>/default_f_use_gps_comp_factor'
  //   Constant: '<S1>/default_gps_comp_factor'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/LAST_KEY_CYCLE'

  ModelMDLOBJ2.step(&VSE_Master_Model_L2_DW.Delay_DSTATE_i, &rtb_Merge,
                    &rtb_VsTracker_VehSpdCompFac,
                    &rtb_VeTracker_VehSpdCompFac_QF,
                    &VSE_Master_Model_L2_U.LAST_KEY_CYCLE_e.NsTracker_LastRemSpdCompFac,
                    &VSE_Master_Model_L2_ConstP.pooled10,
                    &VSE_Master_Model_L2_ConstP.pooled17, &rtb_Model_o1,
                    &rtb_Model_o2);

  // Chart: '<S4>/Initialization_Trigger'
  VSE_Master_Model_L2_Initialization_Trigger
    (&VSE_Master_Model_L2_DW.f_initialize_a,
     &VSE_Master_Model_L2_DW.sf_Initialization_Trigger);

  // Outputs for Enabled SubSystem: '<S4>/Initialization'
  VSE_Master_Model_L2_Initialization(VSE_Master_Model_L2_DW.f_initialize_a,
    VSE_Master_Model_L2_DW.diff_num_e, VSE_Master_Model_L2_DW.diff_denom_gs);

  // End of Outputs for SubSystem: '<S4>/Initialization'

  // DiscreteFilter: '<S4>/Discrete Filter' incorporates:
  //   Delay: '<S22>/Delay'

  denAccum_0 = ((VSE_Master_Model_L2_DW.Delay_DSTATE -
                 (VSE_Master_Model_L2_DW.diff_denom_gs[1] *
                  VSE_Master_Model_L2_DW.DiscreteFilter_states_k[0])) -
                (VSE_Master_Model_L2_DW.diff_denom_gs[2] *
                 VSE_Master_Model_L2_DW.DiscreteFilter_states_k[1])) -
    (VSE_Master_Model_L2_DW.diff_denom_gs[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_k[2]);
  numAccum = (((VSE_Master_Model_L2_DW.diff_num_e[0] * denAccum_0) +
               (VSE_Master_Model_L2_DW.diff_num_e[1] *
                VSE_Master_Model_L2_DW.DiscreteFilter_states_k[0])) +
              (VSE_Master_Model_L2_DW.diff_num_e[2] *
               VSE_Master_Model_L2_DW.DiscreteFilter_states_k[1])) +
    (VSE_Master_Model_L2_DW.diff_num_e[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_k[2]);

  // Logic: '<S45>/Logical Operator' incorporates:
  //   Constant: '<S45>/Constant1'
  //   Constant: '<S45>/Constant2'
  //   DiscreteFilter: '<S4>/Discrete Filter'
  //   RelationalOperator: '<S45>/Equal'
  //   RelationalOperator: '<S45>/Equal1'

  rtb_LogicalOperator_b2 = ((numAccum >= -50.0F) && (numAccum <= 50.0F));

  // If: '<S41>/If' incorporates:
  //   Constant: '<S41>/Constant'
  //   Constant: '<S41>/Constant1'
  //   Constant: '<S41>/Constant2'
  //   Constant: '<S41>/Constant3'
  //   Constant: '<S46>/Constant'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<S47>/In1'
  //   Inport: '<S48>/In1'
  //   Inport: '<S49>/In1'
  //   Inport: '<S50>/In1'
  //   Inport: '<S51>/In1'
  //   RelationalOperator: '<S46>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector6_at_outport_1'

  if (((uint32_T)rtb_Merge_b) != ACCURATED) {
    // Outputs for IfAction SubSystem: '<S41>/If Action Subsystem' incorporates:
    //   ActionPort: '<S47>/Action Port'

    rtb_Merge_kq = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLatAccelQF;

    // End of Outputs for SubSystem: '<S41>/If Action Subsystem'
  } else if (rtb_LogicalOperator && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S41>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S48>/Action Port'

    rtb_Merge_kq = ACCURATED;

    // End of Outputs for SubSystem: '<S41>/If Action Subsystem1'
  } else if ((!rtb_LogicalOperator) && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S41>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S49>/Action Port'

    rtb_Merge_kq = TEMP_UNDEFINED;

    // End of Outputs for SubSystem: '<S41>/If Action Subsystem2'
  } else if (rtb_LogicalOperator && (!rtb_LogicalOperator_b2)) {
    // Outputs for IfAction SubSystem: '<S41>/If Action Subsystem3' incorporates:
    //   ActionPort: '<S50>/Action Port'

    rtb_Merge_kq = NOT_ACCURATED;

    // End of Outputs for SubSystem: '<S41>/If Action Subsystem3'
  } else {
    // Outputs for IfAction SubSystem: '<S41>/If Action Subsystem4' incorporates:
    //   ActionPort: '<S51>/Action Port'

    rtb_Merge_kq = UNDEFINED;

    // End of Outputs for SubSystem: '<S41>/If Action Subsystem4'
  }

  // End of If: '<S41>/If'

  // RelationalOperator: '<S27>/Compare' incorporates:
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S116>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector3_at_outport_1'

  rtb_Merge_b = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawYawRateQF;

  // Switch: '<S20>/Switch' incorporates:
  //   Constant: '<S27>/Constant'
  //   Delay: '<S20>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S27>/Compare'

  if (((uint32_T)rtb_Merge_b) != UNDEFINED) {
    VSE_Master_Model_L2_DW.Delay_DSTATE_n =
      VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_rps_RawYawRate;
  }

  // End of Switch: '<S20>/Switch'

  // Logic: '<S114>/Logical Operator' incorporates:
  //   Constant: '<S114>/Constant'
  //   Constant: '<S114>/Constant1'
  //   Delay: '<S20>/Delay'
  //   RelationalOperator: '<S114>/Equal'
  //   RelationalOperator: '<S114>/Equal1'

  rtb_LogicalOperator = ((VSE_Master_Model_L2_DW.Delay_DSTATE_n >= -3.0543263F) &&
    (VSE_Master_Model_L2_DW.Delay_DSTATE_n <= 3.0543263F));

  // Chart: '<S5>/Initialization_Trigger'
  VSE_Master_Model_L2_Initialization_Trigger
    (&VSE_Master_Model_L2_DW.f_initialize_d,
     &VSE_Master_Model_L2_DW.sf_Initialization_Trigger_l);

  // Chart: '<S17>/Initialization_Trigger'
  VSE_Master_Model_L2_Initialization_Trigger
    (&VSE_Master_Model_L2_DW.f_initialize,
     &VSE_Master_Model_L2_DW.sf_Initialization_Trigger_ah);

  // Outputs for Enabled SubSystem: '<S17>/Initialization'
  VSE_Master_Model_L2_Initialization(VSE_Master_Model_L2_DW.f_initialize,
    VSE_Master_Model_L2_DW.diff_num_b, VSE_Master_Model_L2_DW.diff_denom_k);

  // End of Outputs for SubSystem: '<S17>/Initialization'

  // DiscreteFilter: '<S17>/Discrete Filter' incorporates:
  //   Delay: '<S20>/Delay'

  denAccum_1 = ((VSE_Master_Model_L2_DW.Delay_DSTATE_n -
                 (VSE_Master_Model_L2_DW.diff_denom_k[1] *
                  VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[0])) -
                (VSE_Master_Model_L2_DW.diff_denom_k[2] *
                 VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[1])) -
    (VSE_Master_Model_L2_DW.diff_denom_k[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[2]);
  numAccum = (((VSE_Master_Model_L2_DW.diff_num_b[0] * denAccum_1) +
               (VSE_Master_Model_L2_DW.diff_num_b[1] *
                VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[0])) +
              (VSE_Master_Model_L2_DW.diff_num_b[2] *
               VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[1])) +
    (VSE_Master_Model_L2_DW.diff_num_b[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[2]);

  // Logic: '<S115>/Logical Operator' incorporates:
  //   Constant: '<S115>/Constant1'
  //   Constant: '<S115>/Constant2'
  //   DiscreteFilter: '<S17>/Discrete Filter'
  //   RelationalOperator: '<S115>/Equal'
  //   RelationalOperator: '<S115>/Equal1'

  rtb_LogicalOperator_b2 = ((numAccum >= -3.0543263F) && (numAccum <= 3.0543263F));

  // If: '<S111>/If' incorporates:
  //   Constant: '<S111>/Constant'
  //   Constant: '<S111>/Constant1'
  //   Constant: '<S111>/Constant2'
  //   Constant: '<S111>/Constant3'
  //   Constant: '<S116>/Constant'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<S117>/In1'
  //   Inport: '<S118>/In1'
  //   Inport: '<S119>/In1'
  //   Inport: '<S120>/In1'
  //   Inport: '<S121>/In1'
  //   RelationalOperator: '<S116>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector3_at_outport_1'

  if (((uint32_T)rtb_Merge_b) != ACCURATED) {
    // Outputs for IfAction SubSystem: '<S111>/If Action Subsystem' incorporates:
    //   ActionPort: '<S117>/Action Port'

    rtb_Merge_j = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawYawRateQF;

    // End of Outputs for SubSystem: '<S111>/If Action Subsystem'
  } else if (rtb_LogicalOperator && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S111>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S118>/Action Port'

    rtb_Merge_j = ACCURATED;

    // End of Outputs for SubSystem: '<S111>/If Action Subsystem1'
  } else if ((!rtb_LogicalOperator) && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S111>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S119>/Action Port'

    rtb_Merge_j = TEMP_UNDEFINED;

    // End of Outputs for SubSystem: '<S111>/If Action Subsystem2'
  } else if (rtb_LogicalOperator && (!rtb_LogicalOperator_b2)) {
    // Outputs for IfAction SubSystem: '<S111>/If Action Subsystem3' incorporates:
    //   ActionPort: '<S120>/Action Port'

    rtb_Merge_j = NOT_ACCURATED;

    // End of Outputs for SubSystem: '<S111>/If Action Subsystem3'
  } else {
    // Outputs for IfAction SubSystem: '<S111>/If Action Subsystem4' incorporates:
    //   ActionPort: '<S121>/Action Port'

    rtb_Merge_j = UNDEFINED;

    // End of Outputs for SubSystem: '<S111>/If Action Subsystem4'
  }

  // End of If: '<S111>/If'

  // RelationalOperator: '<S25>/Compare' incorporates:
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S103>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector1_at_outport_1'

  rtb_Merge_b = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawSteeringAngleQF;

  // Switch: '<S18>/Switch' incorporates:
  //   Constant: '<S25>/Constant'
  //   Delay: '<S18>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S25>/Compare'

  if (((uint32_T)rtb_Merge_b) != UNDEFINED) {
    VSE_Master_Model_L2_DW.Delay_DSTATE_f =
      VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_deg_RawSteeringAngle;
  }

  // End of Switch: '<S18>/Switch'

  // Logic: '<S101>/Logical Operator' incorporates:
  //   Constant: '<S101>/Constant'
  //   Constant: '<S101>/Constant1'
  //   Delay: '<S18>/Delay'
  //   RelationalOperator: '<S101>/Equal'
  //   RelationalOperator: '<S101>/Equal1'

  rtb_LogicalOperator = ((VSE_Master_Model_L2_DW.Delay_DSTATE_f >= -720.0F) &&
    (VSE_Master_Model_L2_DW.Delay_DSTATE_f <= 720.0F));

  // Chart: '<S15>/Initialization_Trigger'
  VSE_Master_Model_L2_Initialization_Trigger
    (&VSE_Master_Model_L2_DW.f_initialize_o,
     &VSE_Master_Model_L2_DW.sf_Initialization_Trigger_lx);

  // Outputs for Enabled SubSystem: '<S15>/Initialization'
  VSE_Master_Model_L2_Initialization(VSE_Master_Model_L2_DW.f_initialize_o,
    VSE_Master_Model_L2_DW.diff_num_f, VSE_Master_Model_L2_DW.diff_denom_a);

  // End of Outputs for SubSystem: '<S15>/Initialization'

  // DiscreteFilter: '<S15>/Discrete Filter' incorporates:
  //   Delay: '<S18>/Delay'

  denAccum_2 = ((VSE_Master_Model_L2_DW.Delay_DSTATE_f -
                 (VSE_Master_Model_L2_DW.diff_denom_a[1] *
                  VSE_Master_Model_L2_DW.DiscreteFilter_states_e[0])) -
                (VSE_Master_Model_L2_DW.diff_denom_a[2] *
                 VSE_Master_Model_L2_DW.DiscreteFilter_states_e[1])) -
    (VSE_Master_Model_L2_DW.diff_denom_a[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_e[2]);
  numAccum = (((VSE_Master_Model_L2_DW.diff_num_f[0] * denAccum_2) +
               (VSE_Master_Model_L2_DW.diff_num_f[1] *
                VSE_Master_Model_L2_DW.DiscreteFilter_states_e[0])) +
              (VSE_Master_Model_L2_DW.diff_num_f[2] *
               VSE_Master_Model_L2_DW.DiscreteFilter_states_e[1])) +
    (VSE_Master_Model_L2_DW.diff_num_f[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_e[2]);

  // Logic: '<S102>/Logical Operator' incorporates:
  //   Constant: '<S102>/Constant1'
  //   Constant: '<S102>/Constant2'
  //   DiscreteFilter: '<S15>/Discrete Filter'
  //   RelationalOperator: '<S102>/Equal'
  //   RelationalOperator: '<S102>/Equal1'

  rtb_LogicalOperator_b2 = ((numAccum >= -2000.0F) && (numAccum <= 2000.0F));

  // If: '<S98>/If' incorporates:
  //   Constant: '<S103>/Constant'
  //   Constant: '<S98>/Constant'
  //   Constant: '<S98>/Constant1'
  //   Constant: '<S98>/Constant2'
  //   Constant: '<S98>/Constant3'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<S104>/In1'
  //   Inport: '<S105>/In1'
  //   Inport: '<S106>/In1'
  //   Inport: '<S107>/In1'
  //   Inport: '<S108>/In1'
  //   RelationalOperator: '<S103>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector1_at_outport_1'

  if (((uint32_T)rtb_Merge_b) != ACCURATED) {
    // Outputs for IfAction SubSystem: '<S98>/If Action Subsystem' incorporates:
    //   ActionPort: '<S104>/Action Port'

    rtb_Merge_n = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawSteeringAngleQF;

    // End of Outputs for SubSystem: '<S98>/If Action Subsystem'
  } else if (rtb_LogicalOperator && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S98>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S105>/Action Port'

    rtb_Merge_n = ACCURATED;

    // End of Outputs for SubSystem: '<S98>/If Action Subsystem1'
  } else if ((!rtb_LogicalOperator) && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S98>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S106>/Action Port'

    rtb_Merge_n = TEMP_UNDEFINED;

    // End of Outputs for SubSystem: '<S98>/If Action Subsystem2'
  } else if (rtb_LogicalOperator && (!rtb_LogicalOperator_b2)) {
    // Outputs for IfAction SubSystem: '<S98>/If Action Subsystem3' incorporates:
    //   ActionPort: '<S107>/Action Port'

    rtb_Merge_n = NOT_ACCURATED;

    // End of Outputs for SubSystem: '<S98>/If Action Subsystem3'
  } else {
    // Outputs for IfAction SubSystem: '<S98>/If Action Subsystem4' incorporates:
    //   ActionPort: '<S108>/Action Port'

    rtb_Merge_n = UNDEFINED;

    // End of Outputs for SubSystem: '<S98>/If Action Subsystem4'
  }

  // End of If: '<S98>/If'

  // Switch: '<S23>/Switch' incorporates:
  //   Constant: '<S30>/Constant'
  //   Delay: '<S23>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S30>/Compare'

  if (((uint32_T)VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLatAccelSecondaryQF)
      != UNDEFINED) {
    VSE_Master_Model_L2_DW.Delay_DSTATE_ne =
      VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps2_RawLatAccelSecondary;
  }

  // End of Switch: '<S23>/Switch'

  // Delay: '<S1>/Delay2'
  rtb_LogicalOperator = VSE_Master_Model_L2_DW.bitsForTID0.Delay2_DSTATE;

  // ModelReference: '<S1>/Model1' incorporates:
  //   Delay: '<S18>/Delay'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S1>/Delay1'
  //   Delay: '<S23>/Delay'
  //   Inport: '<Root>/LAST_KEY_CYCLE'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'

  Model1MDLOBJ3.step(&rtb_Model_o1, &VSE_Master_Model_L2_DW.Delay_DSTATE_o,
                     &VSE_Master_Model_L2_DW.Delay_DSTATE_ne,
                     &VSE_Master_Model_L2_DW.Delay_DSTATE_f, &rtb_Model_o2,
                     &rtb_Merge, &VSE_Master_Model_L2_DW.Delay1_DSTATE,
                     &rtb_LogicalOperator,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLatAccelSecondaryQF,
                     &rtb_Merge_n,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_WheelLinSpd_FL,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_WheelLinSpd_FR,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_psi_TirePressure_FR,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_psi_TirePressure_FL,
                     &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_FrontTrackWidth,
                     &VSE_Master_Model_L2_U.LAST_KEY_CYCLE_e.NsVSE_deg_LastRemSWABias,
                     &VSE_Master_Model_L2_U.LAST_KEY_CYCLE_e.NsVSE_b_LastRemSWABiasFlag,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_deg_SteeringAngleBiasExt,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_SteeringAngleBiasExtQF,
                     &VSE_Master_Model_L2_DW.f_SWA_Bias_deg_Conv_out,
                     &VSE_Master_Model_L2_DW.SWA_Bias_deg_Conv_Out,
                     &VSE_Master_Model_L2_DW.CompSteeringAngle_deg_SAE,
                     &VSE_Master_Model_L2_DW.CompSteeringAngle_QF,
                     &VSE_Master_Model_L2_DW.f_SWA_Bias_deg_Conv_internal);

  // ModelReference: '<S1>/Model6' incorporates:
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'

  Model6MDLOBJ8.step(&VSE_Master_Model_L2_DW.Delay_DSTATE_i,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehReverse,
                     &VSE_Master_Model_L2_DW.CompSteeringAngle_deg_SAE,
                     &VSE_Master_Model_L2_DW.CompSteeringAngle_QF,
                     &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_WheelBase,
                     &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_SteeringGearRatio,
                     &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_UndersteeringCoeff,
                     &rtb_yawRate, &rtb_qf, &rtb_Model6_o3, &rtb_Model6_o4);

  // RelationalOperator: '<S31>/Compare' incorporates:
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S58>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector6_at_outport_3'

  rtb_Merge_b = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLongAccelQF;

  // Switch: '<S24>/Switch' incorporates:
  //   Constant: '<S31>/Constant'
  //   Delay: '<S24>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   RelationalOperator: '<S31>/Compare'

  if (!(((uint32_T)rtb_Merge_b) == UNDEFINED)) {
    VSE_Master_Model_L2_DW.Delay_DSTATE_p =
      VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps2_RawLongAccel;
  }

  // End of Switch: '<S24>/Switch'

  // Logic: '<S56>/Logical Operator' incorporates:
  //   Constant: '<S56>/Constant'
  //   Constant: '<S56>/Constant1'
  //   Delay: '<S24>/Delay'
  //   RelationalOperator: '<S56>/Equal'
  //   RelationalOperator: '<S56>/Equal1'

  rtb_LogicalOperator = ((VSE_Master_Model_L2_DW.Delay_DSTATE_p >= -15.0F) &&
    (VSE_Master_Model_L2_DW.Delay_DSTATE_p <= 15.0F));

  // Outputs for Enabled SubSystem: '<S5>/Initialization'
  VSE_Master_Model_L2_Initialization(VSE_Master_Model_L2_DW.f_initialize_d,
    VSE_Master_Model_L2_DW.diff_num_mm, VSE_Master_Model_L2_DW.diff_denom_j);

  // End of Outputs for SubSystem: '<S5>/Initialization'

  // DiscreteFilter: '<S5>/Discrete Filter' incorporates:
  //   Delay: '<S24>/Delay'

  denAccum_3 = ((VSE_Master_Model_L2_DW.Delay_DSTATE_p -
                 (VSE_Master_Model_L2_DW.diff_denom_j[1] *
                  VSE_Master_Model_L2_DW.DiscreteFilter_states_f[0])) -
                (VSE_Master_Model_L2_DW.diff_denom_j[2] *
                 VSE_Master_Model_L2_DW.DiscreteFilter_states_f[1])) -
    (VSE_Master_Model_L2_DW.diff_denom_j[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_f[2]);
  numAccum = (((VSE_Master_Model_L2_DW.diff_num_mm[0] * denAccum_3) +
               (VSE_Master_Model_L2_DW.diff_num_mm[1] *
                VSE_Master_Model_L2_DW.DiscreteFilter_states_f[0])) +
              (VSE_Master_Model_L2_DW.diff_num_mm[2] *
               VSE_Master_Model_L2_DW.DiscreteFilter_states_f[1])) +
    (VSE_Master_Model_L2_DW.diff_num_mm[3] *
     VSE_Master_Model_L2_DW.DiscreteFilter_states_f[2]);

  // Logic: '<S57>/Logical Operator' incorporates:
  //   Constant: '<S57>/Constant1'
  //   Constant: '<S57>/Constant2'
  //   DiscreteFilter: '<S5>/Discrete Filter'
  //   RelationalOperator: '<S57>/Equal'
  //   RelationalOperator: '<S57>/Equal1'

  rtb_LogicalOperator_b2 = ((numAccum >= -50.0F) && (numAccum <= 50.0F));

  // If: '<S53>/If' incorporates:
  //   Constant: '<S53>/Constant'
  //   Constant: '<S53>/Constant1'
  //   Constant: '<S53>/Constant2'
  //   Constant: '<S53>/Constant3'
  //   Constant: '<S58>/Constant'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<S59>/In1'
  //   Inport: '<S60>/In1'
  //   Inport: '<S61>/In1'
  //   Inport: '<S62>/In1'
  //   Inport: '<S63>/In1'
  //   RelationalOperator: '<S58>/Compare'
  //   SignalConversion: '<S2>/SigConversion_InsertedFor_Bus Selector6_at_outport_3'

  if (((uint32_T)rtb_Merge_b) != ACCURATED) {
    // Outputs for IfAction SubSystem: '<S53>/If Action Subsystem' incorporates:
    //   ActionPort: '<S59>/Action Port'

    rtb_Merge_b = VSE_Master_Model_L2_U.VCAN_VSE_p.VeVCAN_RawLongAccelQF;

    // End of Outputs for SubSystem: '<S53>/If Action Subsystem'
  } else if (rtb_LogicalOperator && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S53>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S60>/Action Port'

    rtb_Merge_b = ACCURATED;

    // End of Outputs for SubSystem: '<S53>/If Action Subsystem1'
  } else if ((!rtb_LogicalOperator) && rtb_LogicalOperator_b2) {
    // Outputs for IfAction SubSystem: '<S53>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S61>/Action Port'

    rtb_Merge_b = TEMP_UNDEFINED;

    // End of Outputs for SubSystem: '<S53>/If Action Subsystem2'
  } else if (rtb_LogicalOperator && (!rtb_LogicalOperator_b2)) {
    // Outputs for IfAction SubSystem: '<S53>/If Action Subsystem3' incorporates:
    //   ActionPort: '<S62>/Action Port'

    rtb_Merge_b = NOT_ACCURATED;

    // End of Outputs for SubSystem: '<S53>/If Action Subsystem3'
  } else {
    // Outputs for IfAction SubSystem: '<S53>/If Action Subsystem4' incorporates:
    //   ActionPort: '<S63>/Action Port'

    rtb_Merge_b = UNDEFINED;

    // End of Outputs for SubSystem: '<S53>/If Action Subsystem4'
  }

  // End of If: '<S53>/If'

  // ModelReference: '<S1>/Model2' incorporates:
  //   Delay: '<S20>/Delay'
  //   Delay: '<S22>/Delay'
  //   Delay: '<S24>/Delay'
  //   Inport: '<Root>/VCAN_VSE'

  Model2MDLOBJ4.step(&VSE_Master_Model_L2_DW.Delay_DSTATE_n, &rtb_Merge_j,
                     &VSE_Master_Model_L2_DW.Delay_DSTATE, &rtb_Merge_kq,
                     &VSE_Master_Model_L2_DW.Delay_DSTATE_p, &rtb_Merge_b,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_WheelLinSpd_FL,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_WheelLinSpd_FR,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_WheelLinSpd_FL,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_mps_WheelLinSpd_FR,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehStationary,
                     &rtb_f_stationary);

  // ModelReference: '<S1>/Model7' incorporates:
  //   Delay: '<S1>/Delay'
  //   Delay: '<S1>/Delay1'
  //   Delay: '<S20>/Delay'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/Vs_us_SystemTime'

  Model7MDLOBJ13.step(&VSE_Master_Model_L2_DW.Delay_DSTATE_n, &rtb_Merge_j,
                      &rtb_yawRate, &rtb_qf,
                      &VSE_Master_Model_L2_DW.Delay_DSTATE_i, &rtb_Merge,
                      &rtb_f_stationary, &VSE_Master_Model_L2_U.Vs_us_SystemTime,
                      &rtb_Yaw_Rate_Compensated_Unfiltered,
                      &VSE_Master_Model_L2_DW.Delay_DSTATE_o,
                      &rtb_Yaw_Rate_Raw_Bias, &rtb_Yaw_Rate_Bias_QF,
                      &VSE_Master_Model_L2_DW.Delay1_DSTATE,
                      &rtb_f_Yaw_Rate_Bias_Converged, &rtb_f_stop_bias_converged,
                      &rtb_yaw_rate_bias1, &rtb_f_yaw_stop_bias_converged,
                      &rtb_yaw_rate_bias_fast_bias1,
                      &rtb_yaw_rate_bias_fast_bias2,
                      &rtb_comp_yaw_rate_diff_filt, &rtb_yaw_rate_bias_diff,
                      &rtb_road_type, &rtb_f_yaw_rate_bias_shift,
                      &rtb_f_Yaw_Rate_Bias_Converged_j,
                      &rtb_f_stop_bias_converged_n, &rtb_f_yaw_rate_steady,
                      &rtb_f_input_invalid_persistent,
                      &rtb_f_execution_period_error_persistent,
                      &rtb_f_bias_was_accurate, &rtb_ignition_time,
                      &rtb_f_yaw_stop_bias_converged_d,
                      &rtb_f_yaw_stop_bias_converged_m);

  // ModelReference: '<S1>/Model4' incorporates:
  //   Constant: '<S1>/Constant21'
  //   Delay: '<S1>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'
  //   Inport: '<Root>/Vs_us_SystemTime'

  Model4MDLOBJ6.step(&VSE_Master_Model_L2_U.Vs_us_SystemTime, &rtb_Model_o1,
                     &VSE_Master_Model_L2_DW.Delay_DSTATE_o,
                     &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_DistRearAxleToVCS,
                     &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_VehWidth,
                     &VSE_Master_Model_L2_ConstP.pooled17,
                     &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_RearCorneringCompliance,
                     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehReverse,
                     &rtb_curvature_rear_axle, &rtb_VCS_long_velocity,
                     &rtb_sensor_long_velocity, &rtb_VCS_sideslip,
                     &rtb_sensor_sideslip, &rtb_sideslip_rear_axle,
                     &rtb_VCS_lat_velocity, &rtb_sensor_lat_velocity,
                     &rtb_CurvKalmanFilterC0, &rtb_CurvKalmanFilterC1);

  // ModelReference: '<S1>/Acceleration_VCS' incorporates:
  //   Delay: '<S1>/Delay'
  //   Delay: '<S1>/Delay1'

  Acceleration_VCSMDLOBJ1.step(&rtb_VCS_lat_velocity, &rtb_VCS_long_velocity,
    &VSE_Master_Model_L2_DW.Delay_DSTATE_o, &rtb_Model_o2,
    &VSE_Master_Model_L2_DW.Delay1_DSTATE, &rtb_Acceleration_VCS_o1,
    &rtb_Acceleration_VCS_o2, &rtb_Acceleration_VCS_o3, &rtb_Acceleration_VCS_o4);

  // ModelReference: '<S1>/Model3' incorporates:
  //   Delay: '<S22>/Delay'

  Model3MDLOBJ5.step(&VSE_Master_Model_L2_DW.Delay_DSTATE, &rtb_Merge_kq,
                     &rtb_Model3_o1, &rtb_Model3_o2, &rtb_Model3_o3,
                     &rtb_Model3_o4);

  // Chart: '<S3>/Chart'
  if (((uint32_T)VSE_Master_Model_L2_DW.bitsForTID0.is_active_c4_JerkEstimator) ==
      0U) {
    VSE_Master_Model_L2_DW.bitsForTID0.is_active_c4_JerkEstimator = 1;
    VSE_Master_Model_L2_DW.bitsForTID0.is_c4_JerkEstimator =
      VSE_Master_Model_L2_IN_Initialize_l;
    VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_JerkEstimation = true;
  } else if (((uint32_T)VSE_Master_Model_L2_DW.bitsForTID0.is_c4_JerkEstimator) ==
             VSE_Master_Model_L2_IN_Initialize_l) {
    if (VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_JerkEstimation) {
      VSE_Master_Model_L2_DW.bitsForTID0.is_c4_JerkEstimator =
        VSE_Master_Model_L2_IN_Running_d;
      VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_JerkEstimation = false;
    }
  } else {
    VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_JerkEstimation = false;
  }

  // End of Chart: '<S3>/Chart'

  // Outputs for Enabled SubSystem: '<S3>/Filter Generator' incorporates:
  //   EnablePort: '<S34>/Enable'

  // Outputs for Enabled SubSystem: '<S3>/Differentiator Generator' incorporates:
  //   EnablePort: '<S33>/Enable'

  if (VSE_Master_Model_L2_DW.bitsForTID0.f_initialize_JerkEstimation) {
    // MATLAB Function: '<S33>/FindDifferentiatorCoeff'
    memcpy(&VSE_Master_Model_L2_DW.diff_num[0], &tmp[0], (sizeof(real_T)) << 3U);
    memcpy(&VSE_Master_Model_L2_DW.diff_denom[0], &tmp_0[0], (sizeof(real_T)) <<
           3U);

    // MATLAB Function: '<S34>/FindFilterCoeff'
    for (j = 0; j < 5; j++) {
      VSE_Master_Model_L2_DW.filter_discrete_num[j] = tmp_1[j];
      VSE_Master_Model_L2_DW.filter_discrete_denom[j] = tmp_2[j];
    }

    // End of MATLAB Function: '<S34>/FindFilterCoeff'
  }

  // End of Outputs for SubSystem: '<S3>/Differentiator Generator'
  // End of Outputs for SubSystem: '<S3>/Filter Generator'

  // DiscreteTransferFcn: '<S35>/LowPass Filter' incorporates:
  //   DataTypeConversion: '<S3>/Data Type Conversion'
  //   Delay: '<S24>/Delay'

  denAccum_4 = (((((real_T)VSE_Master_Model_L2_DW.Delay_DSTATE_p) -
                  (VSE_Master_Model_L2_DW.filter_discrete_denom[1] *
                   VSE_Master_Model_L2_DW.LowPassFilter_states[0])) -
                 (VSE_Master_Model_L2_DW.filter_discrete_denom[2] *
                  VSE_Master_Model_L2_DW.LowPassFilter_states[1])) -
                (VSE_Master_Model_L2_DW.filter_discrete_denom[3] *
                 VSE_Master_Model_L2_DW.LowPassFilter_states[2])) -
    (VSE_Master_Model_L2_DW.filter_discrete_denom[4] *
     VSE_Master_Model_L2_DW.LowPassFilter_states[3]);
  rtb_Abs = ((((VSE_Master_Model_L2_DW.filter_discrete_num[0] * denAccum_4) +
               (VSE_Master_Model_L2_DW.filter_discrete_num[1] *
                VSE_Master_Model_L2_DW.LowPassFilter_states[0])) +
              (VSE_Master_Model_L2_DW.filter_discrete_num[2] *
               VSE_Master_Model_L2_DW.LowPassFilter_states[1])) +
             (VSE_Master_Model_L2_DW.filter_discrete_num[3] *
              VSE_Master_Model_L2_DW.LowPassFilter_states[2])) +
    (VSE_Master_Model_L2_DW.filter_discrete_num[4] *
     VSE_Master_Model_L2_DW.LowPassFilter_states[3]);

  // Switch: '<S35>/Switch' incorporates:
  //   Abs: '<S35>/Abs'
  //   Constant: '<S35>/Constant'
  //   Constant: '<S39>/Constant'
  //   DiscreteTransferFcn: '<S35>/LowPass Filter'
  //   RelationalOperator: '<S39>/Compare'

  if (fabs(rtb_Abs) <= 1.0E-10) {
    rtb_Abs = 0.0;
  }

  // End of Switch: '<S35>/Switch'

  // DataTypeConversion: '<S2>/Data Type Conversion1' incorporates:
  //   Inport: '<Root>/VCAN_VSE'

  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_VehStationary =
    VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehStationary ? 1U : 0U;

  // DiscreteTransferFcn: '<S36>/Differentiator' incorporates:
  //   DataTypeConversion: '<S3>/Data Type Conversion'
  //   Delay: '<S24>/Delay'

  denAccum_5 = (real_T)VSE_Master_Model_L2_DW.Delay_DSTATE_p;
  denIdx = 1;
  for (j = 0; j < 7; j++) {
    denAccum_5 -= VSE_Master_Model_L2_DW.diff_denom[denIdx] *
      VSE_Master_Model_L2_DW.Differentiator_states[j];
    denIdx++;
  }

  rtb_Switch = VSE_Master_Model_L2_DW.diff_num[0] * denAccum_5;
  denIdx = 1;
  for (j = 0; j < 7; j++) {
    rtb_Switch += VSE_Master_Model_L2_DW.diff_num[denIdx] *
      VSE_Master_Model_L2_DW.Differentiator_states[j];
    denIdx++;
  }

  // Switch: '<S36>/Switch' incorporates:
  //   Abs: '<S36>/Abs'
  //   Constant: '<S36>/Constant'
  //   Constant: '<S40>/Constant'
  //   DiscreteTransferFcn: '<S36>/Differentiator'
  //   RelationalOperator: '<S40>/Compare'

  if (fabs(rtb_Switch) <= 1.0E-10) {
    rtb_Switch = 0.0;
  }

  // End of Switch: '<S36>/Switch'

  // DataTypeConversion: '<S36>/Data Type Conversion'
  rtb_Gain = (real32_T)rtb_Switch;

  // DataTypeConversion: '<S2>/Data Type Conversion' incorporates:
  //   Inport: '<Root>/VCAN_VSE'

  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_VehReverseSts =
    VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehReverse ? 1U : 0U;

  // BusCreator: '<S1>/Bus Creator' incorporates:
  //   Constant: '<S10>/Constant'
  //   Constant: '<S10>/Constant1'
  //   Constant: '<S77>/Constant'
  //   MATLAB Function: '<S78>/MATLAB Function'
  //   Outport: '<Root>/VSE_Output'
  //   RelationalOperator: '<S77>/Compare'

  VSE_Master_Model_L2_MATLABFunction(((uint32_T)rtb_Merge) != ACCURATED, 0.15F,
    0.0F, &VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_RawSpdPlausibilityFault,
    &VSE_Master_Model_L2_DW.sf_MATLABFunction_m);

  // MATLAB Function: '<S84>/MATLAB Function' incorporates:
  //   Constant: '<S12>/Constant'
  //   Constant: '<S12>/Constant1'
  //   Constant: '<S83>/Constant'
  //   RelationalOperator: '<S83>/Compare'

  VSE_Master_Model_L2_MATLABFunction(((uint32_T)rtb_Merge_j) != ACCURATED, 0.15F,
    0.0F, &rtb_LogicalOperator, &VSE_Master_Model_L2_DW.sf_MATLABFunction_i);

  // BusCreator: '<S1>/Bus Creator' incorporates:
  //   Constant: '<S11>/Constant'
  //   Constant: '<S11>/Constant1'
  //   Constant: '<S65>/Constant'
  //   Constant: '<S68>/Constant'
  //   Constant: '<S6>/Constant'
  //   Constant: '<S6>/Constant1'
  //   Constant: '<S71>/Constant'
  //   Constant: '<S74>/Constant'
  //   Constant: '<S7>/Constant'
  //   Constant: '<S7>/Constant1'
  //   Constant: '<S80>/Constant'
  //   Constant: '<S8>/Constant'
  //   Constant: '<S8>/Constant1'
  //   Constant: '<S9>/Constant'
  //   Constant: '<S9>/Constant1'
  //   Delay: '<S1>/Delay1'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/Vs_us_SystemTime'
  //   MATLAB Function: '<S66>/MATLAB Function'
  //   MATLAB Function: '<S69>/MATLAB Function'
  //   MATLAB Function: '<S72>/MATLAB Function'
  //   MATLAB Function: '<S75>/MATLAB Function'
  //   MATLAB Function: '<S81>/MATLAB Function'
  //   Outport: '<Root>/VSE_Output'
  //   RelationalOperator: '<S65>/Compare'
  //   RelationalOperator: '<S68>/Compare'
  //   RelationalOperator: '<S71>/Compare'
  //   RelationalOperator: '<S74>/Compare'
  //   RelationalOperator: '<S80>/Compare'

  VSE_Master_Model_L2_MATLABFunction(((uint32_T)rtb_Merge_kq) != ACCURATED,
    0.15F, 0.0F,
    &VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_RawLatAccelPlausibilityFault,
    &VSE_Master_Model_L2_DW.sf_MATLABFunction_g);
  VSE_Master_Model_L2_MATLABFunction(((uint32_T)rtb_Merge_b) != ACCURATED, 0.15F,
    0.0F,
    &VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_RawLongAccelPlausibilityFault,
    &VSE_Master_Model_L2_DW.sf_MATLABFunction_f);
  VSE_Master_Model_L2_MATLABFunction(((uint32_T)rtb_Merge_n) != ACCURATED, 0.15F,
    0.0F,
    &VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_RawSteeringAnglePlausibilityFault,
    &VSE_Master_Model_L2_DW.sf_MATLABFunction_n);
  VSE_Master_Model_L2_MATLABFunction(((uint32_T)rtb_Model_o2) != ACCURATED,
    0.15F, 0.0F,
    &VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_CompSpdPlausibilityFault,
    &VSE_Master_Model_L2_DW.sf_MATLABFunction);
  VSE_Master_Model_L2_MATLABFunction(((uint32_T)
    VSE_Master_Model_L2_DW.Delay1_DSTATE) != ACCURATED, 0.15F, 0.0F,
    &VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_CompYawRatePlausibilityFault,
    &VSE_Master_Model_L2_DW.sf_MATLABFunction_p);
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_us_Timestamp =
    VSE_Master_Model_L2_U.Vs_us_SystemTime;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_VehRawSpd =
    VSE_Master_Model_L2_DW.Delay_DSTATE_i;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_VehRawSpdQF = rtb_Merge;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_SpdCompFactor =
    rtb_VsTracker_VehSpdCompFac;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_SpdCompFactorQF =
    rtb_VeTracker_VehSpdCompFac_QF;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_VehFiltSpdOverGround = rtb_Model_o1;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_VehFiltSpdOverGroundQF = rtb_Model_o2;

  // Switch: '<S13>/Switch' incorporates:
  //   Constant: '<S13>/Constant'
  //   Constant: '<S13>/Constant1'
  //   Inport: '<Root>/VCAN_VSE'

  if (VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_b_VehReverse) {
    j = -1;
  } else {
    j = 1;
  }

  // End of Switch: '<S13>/Switch'

  // BusCreator: '<S1>/Bus Creator' incorporates:
  //   Constant: '<S1>/Constant10'
  //   Constant: '<S1>/Constant2'
  //   Constant: '<S1>/Constant5'
  //   Constant: '<S1>/Constant6'
  //   Constant: '<S1>/Constant7'
  //   Constant: '<S1>/Constant8'
  //   Constant: '<S1>/Constant9'
  //   DataTypeConversion: '<S35>/Data Type Conversion'
  //   Delay: '<S18>/Delay'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S1>/Delay1'
  //   Delay: '<S20>/Delay'
  //   Delay: '<S22>/Delay'
  //   Delay: '<S24>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'
  //   Outport: '<Root>/VSE_Output'
  //   Product: '<S13>/Product'

  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_VehFiltSignedSpdOverGround =
    rtb_Model_o1 * ((real32_T)j);
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_RawLatAccel =
    VSE_Master_Model_L2_DW.Delay_DSTATE;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_RawLatAccelQF = rtb_Merge_kq;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_RawLongAccel =
    VSE_Master_Model_L2_DW.Delay_DSTATE_p;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_RawLongAccelQF = rtb_Merge_b;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rps_RawYawRate =
    VSE_Master_Model_L2_DW.Delay_DSTATE_n;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_RawYawRateQF = rtb_Merge_j;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_deg_RawSteeringAngle =
    VSE_Master_Model_L2_DW.Delay_DSTATE_f;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_RawSteeringAngleQF = rtb_Merge_n;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_deg_RoadWhlAngle = rtb_Model6_o3;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_RoadWhlAngleQF = rtb_Model6_o4;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rps_YawRateSA = rtb_yawRate;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_YawRateSAQF = rtb_qf;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rps_YawRateBias = rtb_Yaw_Rate_Raw_Bias;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_YawRateBiasQF = rtb_Yaw_Rate_Bias_QF;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_YawStopBiasConverged =
    rtb_f_yaw_stop_bias_converged_m;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rps_CompYawRateUnfilt =
    rtb_Yaw_Rate_Compensated_Unfiltered;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rps_CompYawRateFilt =
    VSE_Master_Model_L2_DW.Delay_DSTATE_o;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_CompYawRateQF =
    VSE_Master_Model_L2_DW.Delay1_DSTATE;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_CurvatureRearAxle =
    rtb_curvature_rear_axle;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rad_VCSSideslip = rtb_VCS_sideslip;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_VCSLongVel = rtb_VCS_long_velocity;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_VCSLatVel = rtb_VCS_lat_velocity;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rad_SensorSideslip =
    rtb_sensor_sideslip;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_SensorLongVel =
    rtb_sensor_long_velocity;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_SensorLatVel =
    rtb_sensor_lat_velocity;
  VSE_Master_Model_L2_Y.VSE_Output.KsVSE_m_DistRearAxleToVCS =
    VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_DistRearAxleToVCS;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_VCSLatAccel =
    rtb_Acceleration_VCS_o1;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_VCSLatAccelQF = rtb_Acceleration_VCS_o3;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_VCSLongAccel =
    rtb_Acceleration_VCS_o2;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_VCSLongAccelQF =
    rtb_Acceleration_VCS_o4;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_CompLatAccel = rtb_Model3_o2;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_CompLatAccelQF = rtb_Model3_o4;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_CompLongAccel = (real32_T)rtb_Abs;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_CompLongAccelQF = rtb_Merge_b;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps3_LatJerk = rtb_Model3_o1;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_LatJerkQF = rtb_Model3_o3;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps3_LongJerk = rtb_Gain;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_LongJerkQF = rtb_Merge_b;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_deg_BankAngle = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_BankAngleQF =
    VSE_Master_Model_L2_ConstB.DataTypeConversion5;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_deg_GradeAngle = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_GradeAngleQF =
    VSE_Master_Model_L2_ConstB.DataTypeConversion6;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_deg_CompSteeringAngle =
    VSE_Master_Model_L2_DW.CompSteeringAngle_deg_SAE;
  VSE_Master_Model_L2_Y.VSE_Output.VeVSE_CompSteeringAngleQF =
    VSE_Master_Model_L2_DW.CompSteeringAngle_QF;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_deg_SWABiasConvergedToNVM =
    VSE_Master_Model_L2_DW.SWA_Bias_deg_Conv_Out;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_SWABiasConvergedToNVMFlag =
    VSE_Master_Model_L2_DW.f_SWA_Bias_deg_Conv_out;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_SWABiasConvergedInternalFlag =
    VSE_Master_Model_L2_DW.f_SWA_Bias_deg_Conv_internal;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_dps_SteeringAngleRateFilt =
    VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_dps_RawSteeringAngleRate;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_rad_SideslipRearAxle =
    rtb_sideslip_rear_axle;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_COGLongVel = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps_COGLatVel = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_COGLongAccel = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_mps2_COGLatAccel = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_VehMotionDirection = 0U;
  VSE_Master_Model_L2_Y.VSE_Output.VsVSE_b_RawYawRatePlausibilityFault =
    rtb_LogicalOperator;
  VSE_Master_Model_L2_Y.VSE_Output.CsVSE_MajorVer = 19U;
  VSE_Master_Model_L2_Y.VSE_Output.CsVSE_MinorVer = 1U;
  VSE_Master_Model_L2_Y.VSE_Output.CsVSE_FieldVer = 1U;
  VSE_Master_Model_L2_Y.VSE_Output.CsVSE_CalibrationVer = 2U;

  // ModelReference: '<S1>/WheelRadiusEstimation' incorporates:
  //   Constant: '<S1>/Constant1'
  //   Constant: '<S1>/Constant11'
  //   Constant: '<S1>/Constant13'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'

  WheelRadiusEstimationMDLOBJ12.step(&VSE_Master_Model_L2_DW.Delay_DSTATE_i,
    &rtb_Merge, &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_rpm_WheelRotSpd_FR,
    &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_rpm_WheelRotSpd_FL,
    &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_rpm_WheelRotSpd_FR,
    &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_rpm_WheelRotSpd_FL,
    &VSE_Master_Model_L2_DW.Delay_DSTATE_o, &VSE_Master_Model_L2_ConstP.pooled4,
    &VSE_Master_Model_L2_ConstP.pooled4, &VSE_Master_Model_L2_ConstB.Cast,
    (boolean_T*)&VSE_Master_Model_L2_ConstB.CastToBoolean, &rtb_Model_o1,
    &rtb_Model_o2,
    &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_FrontWhlRadiusStaticLoaded,
    &VSE_Master_Model_L2_U.Vehicle_Parameters_f.KsVSE_m_FrontWhlRadiusStaticLoaded,
    &rtb_WheelRadiusEstimation_o1,
    &VSE_Master_Model_L2_DW.WheelRadiusEstimation_o2,
    &rtb_WheelRadiusEstimation_o3,
    &VSE_Master_Model_L2_DW.WheelRadiusEstimation_o4,
    &rtb_WheelRadiusEstimation_o5,
    &VSE_Master_Model_L2_DW.WheelRadiusEstimation_o6,
    &rtb_WheelRadiusEstimation_o7,
    &VSE_Master_Model_L2_DW.WheelRadiusEstimation_o8,
    &rtb_WheelRadiusEstimation_o9, &rtb_WheelRadiusEstimation_o10);

  // Gain: '<S1>/Gain' incorporates:
  //   Sum: '<S1>/Sum'

  rtb_Gain = (((rtb_WheelRadiusEstimation_o1 + rtb_WheelRadiusEstimation_o3) +
               rtb_WheelRadiusEstimation_o5) + rtb_WheelRadiusEstimation_o7) *
    0.25F;

  // ModelReference: '<S1>/TrailerMassEstimation' incorporates:
  //   Constant: '<S1>/Constant15'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S24>/Delay'
  //   Inport: '<Root>/VCAN_VSE'

  TrailerMassEstimationMDLOBJ11.step
    (&VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_Nm_SumTrqStatic, &rtb_Model_o1,
     &rtb_Gain, &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_Nm_BrkTrq,
     &VSE_Master_Model_L2_DW.Delay_DSTATE_p,
     &VSE_Master_Model_L2_DW.Delay_DSTATE_o,
     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_TrqAmpTrans,
     &rtb_signal_value_filtered, &rtb_f_mass_converged);

  // ModelReference: '<S1>/Model5' incorporates:
  //   Inport: '<Root>/PROXI'
  //   Inport: '<Root>/Tracker_VSE_Trailer_Signals'
  //   Inport: '<Root>/VCAN_VSE'

  Model5MDLOBJ7.step
    (&VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_TrailerConnectionSts,
     &VSE_Master_Model_L2_U.VCAN_VSE_p.VsVCAN_ITBM_TrlrStat,
     &VSE_Master_Model_L2_U.PROXI_h.CsPROXI_CANNode63_TTM,
     &VSE_Master_Model_L2_U.PROXI_h.CsPROXI_CANNode95_ITBM,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_TrailerPresence,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_TrailerPresenceConfLvl,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_TrailerLengthConfLvl,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_TrailerWidthConfLvl,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_m_TrailerLength,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_m_TrailerWidth,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_RadarDetectSts,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_ms_RadarDetectTimer,
     &VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_ms_StationaryTimer,
     &rtb_f_stationary, &VSE_Master_Model_L2_DW.Trailer_Detection,
     &rtb_Trailer_Detection_ConfLvl, &rtb_trailer_length,
     &rtb_trailer_length_ConfLvl, &rtb_trailer_width, &rtb_trailer_width_ConfLvl,
     &rtb_Model5_o7, &rtb_Model5_o8, &rtb_Model5_o9);

  // ModelReference: '<S1>/Model9' incorporates:
  //   Inport: '<Root>/Tracker_VSE_Trailer_Signals'

  Model9MDLOBJ10.step
    (&VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_rps_TrailerAngleRate,
     &rtb_Model9_o1, &rtb_Model9_o2);

  // BusCreator: '<S1>/Bus Creator2' incorporates:
  //   Inport: '<Root>/Tracker_VSE_Trailer_Signals'
  //   Outport: '<Root>/VSE_Output_Trailer'

  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_Kg_TrlrMass =
    rtb_signal_value_filtered;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_Kg_HostVehMass = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_m_TrlrLen = rtb_trailer_length;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_m_TrlrWidth = rtb_trailer_width;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_Kgm2_HostVehIzz = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_Kgm2_TrlrIzz = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_N_HitchFx = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_N_HitchFy = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_Nm_HitchMz = 0.0F;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_rad_HitchAng =
    VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_rad_TrailerAngle;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_rps_HitchAngSpd =
    VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_rps_TrailerAngleRate;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_m_TrlrHostVehGap =
    VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_m_TrailerHVGap;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_TrlrDetectSts =
    VSE_Master_Model_L2_DW.Trailer_Detection;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_TrlrDetectStsConfLvl =
    rtb_Trailer_Detection_ConfLvl;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_b_TrlrMassConv =
    rtb_f_mass_converged;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_b_HostVehMassConv = false;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_TrlrLenConfLvl =
    rtb_trailer_length_ConfLvl;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_TrlrWidthConfLvl =
    rtb_trailer_width_ConfLvl;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_b_HostVehIzzConv = false;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_b_TrlrIzzConv = false;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_HitchFxConfLvl = 3U;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_HitchFyConfLvl = 3U;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_HitchMzConfLvl = 3U;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_HitchAngConfLvl =
    VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_TrailerAngleConfLvl;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_HitchAngSpdConfLvl =
    VSE_Master_Model_L2_U.Tracker_VSE_Trailer_Signals_g.VsTracker_TrailerAngleRateConfLvl;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VeVSE_b_RadarDetectionSts =
    rtb_Model5_o7;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_s_RadarDetectionTimer =
    rtb_Model5_o8;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_s_StationaryTimer =
    rtb_Model5_o9;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_rps_TrlrOscMag = rtb_Model9_o1;
  VSE_Master_Model_L2_Y.VSE_Output_Trailer.VsVSE_hz_TrlrOscFreq = rtb_Model9_o2;

  // Outport: '<Root>/VSE_Resim_Output' incorporates:
  //   BusCreator: '<S1>/Bus Creator1'

  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_YawRateBiasConverged =
    rtb_f_Yaw_Rate_Bias_Converged_j;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_YawRateStopBiasConverged =
    rtb_f_stop_bias_converged_n;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_rps_YawRateBias1 =
    rtb_yaw_rate_bias1;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_rps_YawRateBias2 =
    rtb_f_yaw_stop_bias_converged;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_rps_CompYawRateDiffFilt =
    rtb_comp_yaw_rate_diff_filt;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_rps_YawRateBiasDiff =
    rtb_yaw_rate_bias_diff;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_rps_YawRateBiasFast1 =
    rtb_yaw_rate_bias_fast_bias1;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_rps_YawRateBiasFast2 =
    rtb_yaw_rate_bias_fast_bias2;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VeVSE_RoadType = rtb_road_type;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_YawRateBiasShift =
    rtb_f_yaw_rate_bias_shift;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_YawRateSteady =
    rtb_f_yaw_rate_steady;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_ExecutionPeriodErrorPresistent =
    rtb_f_execution_period_error_persistent;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_InputInvalidPersistent =
    rtb_f_input_invalid_persistent;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_BiasWasAccurate =
    rtb_f_bias_was_accurate;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_s_IgnitionTime =
    rtb_ignition_time;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_b_YawStopBiasConverged =
    rtb_f_yaw_stop_bias_converged_d;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_CurvKalmanFilterC0 =
    rtb_CurvKalmanFilterC0;
  VSE_Master_Model_L2_Y.VSE_Resim_Output.VsVSE_CurvKalmanFilterC1 =
    rtb_CurvKalmanFilterC1;

  // Update for DiscreteFilter: '<S14>/Discrete Filter'
  VSE_Master_Model_L2_DW.DiscreteFilter_states[2] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states[1];
  VSE_Master_Model_L2_DW.DiscreteFilter_states[1] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states[0];
  VSE_Master_Model_L2_DW.DiscreteFilter_states[0] = denAccum;

  // Update for DiscreteFilter: '<S4>/Discrete Filter'
  VSE_Master_Model_L2_DW.DiscreteFilter_states_k[2] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_k[1];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_k[1] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_k[0];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_k[0] = denAccum_0;

  // Update for DiscreteFilter: '<S17>/Discrete Filter'
  VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[2] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[1];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[1] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[0];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_ku[0] = denAccum_1;

  // Update for DiscreteFilter: '<S15>/Discrete Filter'
  VSE_Master_Model_L2_DW.DiscreteFilter_states_e[2] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_e[1];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_e[1] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_e[0];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_e[0] = denAccum_2;

  // Update for Delay: '<S1>/Delay2'
  VSE_Master_Model_L2_DW.bitsForTID0.Delay2_DSTATE = rtb_f_stop_bias_converged;

  // Update for DiscreteFilter: '<S5>/Discrete Filter'
  VSE_Master_Model_L2_DW.DiscreteFilter_states_f[2] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_f[1];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_f[1] =
    VSE_Master_Model_L2_DW.DiscreteFilter_states_f[0];
  VSE_Master_Model_L2_DW.DiscreteFilter_states_f[0] = denAccum_3;

  // Update for DiscreteTransferFcn: '<S35>/LowPass Filter'
  VSE_Master_Model_L2_DW.LowPassFilter_states[3] =
    VSE_Master_Model_L2_DW.LowPassFilter_states[2];
  VSE_Master_Model_L2_DW.LowPassFilter_states[2] =
    VSE_Master_Model_L2_DW.LowPassFilter_states[1];
  VSE_Master_Model_L2_DW.LowPassFilter_states[1] =
    VSE_Master_Model_L2_DW.LowPassFilter_states[0];
  VSE_Master_Model_L2_DW.LowPassFilter_states[0] = denAccum_4;

  // Update for DiscreteTransferFcn: '<S36>/Differentiator'
  for (j = 0; j < 6; j++) {
    VSE_Master_Model_L2_DW.Differentiator_states[6 - j] =
      VSE_Master_Model_L2_DW.Differentiator_states[5 - j];
  }

  VSE_Master_Model_L2_DW.Differentiator_states[0] = denAccum_5;

  // End of Update for DiscreteTransferFcn: '<S36>/Differentiator'
}

// Model initialize function
void VSE_Master_Model_L2ModelClass::initialize()
{
  // Start for ModelReference: '<S1>/WheelRadiusEstimation' incorporates:
  //   Constant: '<S1>/Constant1'
  //   Constant: '<S1>/Constant11'
  //   Constant: '<S1>/Constant13'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'

  WheelRadiusEstimationMDLOBJ12.start();

  // Start for ModelReference: '<S1>/TrailerMassEstimation' incorporates:
  //   Constant: '<S1>/Constant15'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S24>/Delay'
  //   Inport: '<Root>/VCAN_VSE'

  TrailerMassEstimationMDLOBJ11.start();

  // SystemInitialize for ModelReference: '<S1>/Model' incorporates:
  //   Constant: '<S1>/default_f_use_gps_comp_factor'
  //   Constant: '<S1>/default_gps_comp_factor'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/LAST_KEY_CYCLE'

  ModelMDLOBJ2.init();

  // SystemInitialize for ModelReference: '<S1>/Model4' incorporates:
  //   Constant: '<S1>/Constant21'
  //   Delay: '<S1>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'
  //   Inport: '<Root>/Vs_us_SystemTime'

  Model4MDLOBJ6.init();

  // SystemInitialize for ModelReference: '<S1>/WheelRadiusEstimation' incorporates:
  //   Constant: '<S1>/Constant1'
  //   Constant: '<S1>/Constant11'
  //   Constant: '<S1>/Constant13'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S21>/Delay'
  //   Inport: '<Root>/VCAN_VSE'
  //   Inport: '<Root>/Vehicle_Parameters'

  WheelRadiusEstimationMDLOBJ12.init();

  // SystemInitialize for ModelReference: '<S1>/TrailerMassEstimation' incorporates:
  //   Constant: '<S1>/Constant15'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S24>/Delay'
  //   Inport: '<Root>/VCAN_VSE'

  TrailerMassEstimationMDLOBJ11.init();

  // SystemInitialize for ModelReference: '<S1>/Model9' incorporates:
  //   Inport: '<Root>/Tracker_VSE_Trailer_Signals'

  Model9MDLOBJ10.init();
}

// Constructor
VSE_Master_Model_L2ModelClass::VSE_Master_Model_L2ModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
VSE_Master_Model_L2ModelClass::~VSE_Master_Model_L2ModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
