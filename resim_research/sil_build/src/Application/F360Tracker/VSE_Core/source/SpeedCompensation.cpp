//
// File: SpeedCompensation.cpp
//
// Code generated for Simulink model 'SpeedCompensation'.
//
// Model version                  : 1.541
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:57:34 2024
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
#include "SpeedCompensation.h"
#include "SpeedCompensation_private.h"

// Named constants for MATLAB Function: '<S4>/MATLAB Function'
#define SpeedCompensation_input_rate_within_limit_maturation_time (0.05F)
#define SpeedCompensation_lower_speed_rate_lim (-10.0F)
#define SpeedCompensation_speed_comp_vse_sampling_time (0.01F)
#define SpeedCompensation_upper_speed_rate_lim (8.0F)

// System initialize for referenced model: 'SpeedCompensation'
void SpeedCompensationModelClass::init(void)
{
  // InitializeConditions for UnitDelay: '<S3>/Unit Delay2'
  SpeedCompensation_DW.UnitDelay2_DSTATE = 10.0F;

  // InitializeConditions for UnitDelay: '<S3>/Unit Delay1'
  SpeedCompensation_DW.UnitDelay1_DSTATE = ACCURATED;

  // InitializeConditions for Delay: '<S3>/Resettable Delay'
  SpeedCompensation_DW.icLoad = 1U;
}

// Output and update for referenced model: 'SpeedCompensation'
void SpeedCompensationModelClass::step(const real32_T *rtu_raw_speed_mps, const
  enum_quality_factor_T *rtu_raw_speed_qf, const real32_T
  *rtu_VehicleCompFactor_KA, const enum_quality_factor_T
  *rtu_VehicleCompFactor_KA_QF, const real32_T *rtu_nvm_last_rem_speed_comp,
  const real32_T *rtu_gps_comp_factor, const boolean_T
  *rtu_f_use_gps_comp_factor, real32_T *rty_filt_veh_speed_over_ground,
  enum_quality_factor_T *rty_filt_veh_speed_over_ground_qf)
{
  real32_T rate;
  real32_T input_rate;
  boolean_T rtb_Compare_c;
  real32_T rtb_Add;
  boolean_T guard1 = false;

  // MATLAB Function: '<S4>/MATLAB Function'
  if (!SpeedCompensation_DW.bitsForTID0.last_val_out_not_empty) {
    SpeedCompensation_DW.last_val_out = *rtu_raw_speed_mps;
    SpeedCompensation_DW.bitsForTID0.last_val_out_not_empty = true;
    SpeedCompensation_DW.last_val_in = *rtu_raw_speed_mps;
  }

  rate = ((*rtu_raw_speed_mps) - SpeedCompensation_DW.last_val_out) /
    SpeedCompensation_speed_comp_vse_sampling_time;
  input_rate = ((*rtu_raw_speed_mps) - SpeedCompensation_DW.last_val_in) /
    SpeedCompensation_speed_comp_vse_sampling_time;
  guard1 = false;
  if (rate > SpeedCompensation_upper_speed_rate_lim) {
    rate = 0.08F + SpeedCompensation_DW.last_val_out;
    guard1 = true;
  } else if (rate < SpeedCompensation_lower_speed_rate_lim) {
    rate = -0.099999994F + SpeedCompensation_DW.last_val_out;
    guard1 = true;
  } else {
    rate = *rtu_raw_speed_mps;
    SpeedCompensation_DW.last_val_out = *rtu_raw_speed_mps;
    SpeedCompensation_DW.last_val_in = *rtu_raw_speed_mps;
    SpeedCompensation_DW.time_check_input_rate_within_limit = 0.0F;
  }

  if (guard1) {
    if ((input_rate < SpeedCompensation_upper_speed_rate_lim) && (input_rate >
         SpeedCompensation_lower_speed_rate_lim)) {
      SpeedCompensation_DW.time_check_input_rate_within_limit +=
        SpeedCompensation_speed_comp_vse_sampling_time;
    } else {
      SpeedCompensation_DW.time_check_input_rate_within_limit = 0.0F;
    }

    if (SpeedCompensation_DW.time_check_input_rate_within_limit >
        SpeedCompensation_input_rate_within_limit_maturation_time) {
      rate = (0.760942817F * SpeedCompensation_DW.last_val_out) + (0.239057183F *
        (*rtu_raw_speed_mps));
    }

    SpeedCompensation_DW.last_val_out = rate;
    SpeedCompensation_DW.last_val_in = *rtu_raw_speed_mps;
  }

  // End of MATLAB Function: '<S4>/MATLAB Function'

  // RelationalOperator: '<S6>/Compare' incorporates:
  //   Constant: '<S6>/Constant'

  rtb_Compare_c = (((uint32_T)(*rtu_VehicleCompFactor_KA_QF)) == ACCURATED);

  // Switch: '<S3>/Switch3' incorporates:
  //   Constant: '<S3>/Constant2'
  //   Constant: '<S3>/Constant3'
  //   Constant: '<S9>/Constant'
  //   Logic: '<S3>/Logical Operator'
  //   RelationalOperator: '<S9>/Compare'
  //   Sum: '<S3>/Add2'
  //   UnitDelay: '<S3>/Unit Delay1'
  //   UnitDelay: '<S3>/Unit Delay2'

  if ((((uint32_T)SpeedCompensation_DW.UnitDelay1_DSTATE) != ACCURATED) &&
      rtb_Compare_c) {
    SpeedCompensation_DW.UnitDelay2_DSTATE = 0.0F;
  } else {
    SpeedCompensation_DW.UnitDelay2_DSTATE += 0.01F;
  }

  // End of Switch: '<S3>/Switch3'

  // Saturate: '<S3>/Saturation'
  if (SpeedCompensation_DW.UnitDelay2_DSTATE > 10.0F) {
    SpeedCompensation_DW.UnitDelay2_DSTATE = 10.0F;
  } else {
    if (SpeedCompensation_DW.UnitDelay2_DSTATE < 0.0F) {
      SpeedCompensation_DW.UnitDelay2_DSTATE = 0.0F;
    }
  }

  // End of Saturate: '<S3>/Saturation'

  // Delay: '<S3>/Resettable Delay'
  if (((int32_T)SpeedCompensation_DW.icLoad) != 0) {
    SpeedCompensation_DW.ResettableDelay_DSTATE = *rtu_nvm_last_rem_speed_comp;
  }

  // Switch: '<S3>/Switch2' incorporates:
  //   Constant: '<S8>/Constant'
  //   Logic: '<S3>/Logical Operator1'
  //   RelationalOperator: '<S8>/Compare'
  //   Switch: '<S3>/Switch4'
  //   UnitDelay: '<S3>/Unit Delay2'

  if ((SpeedCompensation_DW.UnitDelay2_DSTATE >= 10.0F) && rtb_Compare_c) {
    input_rate = *rtu_VehicleCompFactor_KA;
  } else {
    if (rtb_Compare_c) {
      // Switch: '<S3>/Switch4'
      input_rate = *rtu_VehicleCompFactor_KA;
    } else {
      // Switch: '<S3>/Switch4' incorporates:
      //   Delay: '<S3>/Resettable Delay'

      input_rate = SpeedCompensation_DW.ResettableDelay_DSTATE;
    }

    // Sum: '<S3>/Add' incorporates:
    //   Delay: '<S3>/Resettable Delay'

    rtb_Add = input_rate - SpeedCompensation_DW.ResettableDelay_DSTATE;

    // Switch: '<S3>/Switch1' incorporates:
    //   Abs: '<S3>/Abs'
    //   Constant: '<S3>/Constant1'
    //   Constant: '<S7>/Constant'
    //   Delay: '<S3>/Resettable Delay'
    //   Product: '<S3>/Divide'
    //   RelationalOperator: '<S7>/Compare'
    //   Sum: '<S3>/Add1'

    if (fabsf(rtb_Add) > 5.0E-5F) {
      // Signum: '<S3>/Sign'
      if (rtb_Add < 0.0F) {
        rtb_Add = -1.0F;
      } else {
        if (rtb_Add > 0.0F) {
          rtb_Add = 1.0F;
        }
      }

      // End of Signum: '<S3>/Sign'
      input_rate = (5.0E-5F * rtb_Add) +
        SpeedCompensation_DW.ResettableDelay_DSTATE;
    }

    // End of Switch: '<S3>/Switch1'
  }

  // End of Switch: '<S3>/Switch2'

  // Switch: '<S2>/Switch1' incorporates:
  //   Constant: '<S5>/Constant'
  //   Logic: '<S2>/Logical Operator1'
  //   Product: '<S2>/Product1'
  //   Product: '<S3>/Product'
  //   RelationalOperator: '<S5>/Compare'
  //   Switch: '<S1>/Switch1'

  if ((*rtu_f_use_gps_comp_factor) && (rate < 15.0F)) {
    *rty_filt_veh_speed_over_ground = (*rtu_gps_comp_factor) * rate;
  } else {
    *rty_filt_veh_speed_over_ground = input_rate * rate;
  }

  // End of Switch: '<S2>/Switch1'

  // Inport: '<Root>/raw_speed_qf'
  *rty_filt_veh_speed_over_ground_qf = *rtu_raw_speed_qf;

  // Update for UnitDelay: '<S3>/Unit Delay1'
  SpeedCompensation_DW.UnitDelay1_DSTATE = *rtu_VehicleCompFactor_KA_QF;

  // Update for Delay: '<S3>/Resettable Delay'
  SpeedCompensation_DW.icLoad = 0U;
  SpeedCompensation_DW.ResettableDelay_DSTATE = input_rate;
}

// Constructor
SpeedCompensationModelClass::SpeedCompensationModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
SpeedCompensationModelClass::~SpeedCompensationModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
