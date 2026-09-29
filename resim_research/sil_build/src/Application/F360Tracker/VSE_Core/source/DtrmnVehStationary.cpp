//
// File: DtrmnVehStationary.cpp
//
// Code generated for Simulink model 'DtrmnVehStationary'.
//
// Model version                  : 1.382
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:57:14 2024
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
#include "DtrmnVehStationary.h"
#include "DtrmnVehStationary_private.h"

// Named constants for Chart: '<S1>/Chart'
#define DtrmnVehStationary_IN_Initialize ((uint8_T)1U)
#define DtrmnVehStationary_IN_Running  ((uint8_T)2U)

// Output and update for referenced model: 'DtrmnVehStationary'
void DtrmnVehStationaryModelClass::step(const real32_T *rtu_raw_yaw_rate_rps,
  const enum_quality_factor_T *rtu_raw_yaw_rate_qf, const real32_T
  *rtu_raw_lat_accel, const enum_quality_factor_T *rtu_raw_lat_accel_qf, const
  real32_T *rtu_raw_long_accel, const enum_quality_factor_T
  *rtu_raw_long_accel_qf, const real32_T *rtu_wheel_lin_speed_mps_front_left,
  const real32_T *rtu_wheel_lin_speed_mps_front_right, const real32_T
  *rtu_wheel_lin_speed_mps_rear_left, const real32_T
  *rtu_wheel_lin_speed_mps_rear_right, const boolean_T
  *rtu_external_stationary_flag, boolean_T *rty_f_stationary)
{
  int32_T k;
  int32_T memOffset;
  real32_T rtb_TmpSignalConversionAtDifferentiatorInport1[3];
  real32_T rtb_Differentiator[3];
  real32_T Differentiator_tmp_tmp;
  real32_T Differentiator_tmp_tmp_0;

  // Chart: '<S1>/Chart'
  if (((uint32_T)
       DtrmnVehStationary_DW.bitsForTID0.is_active_c1_DtrmnVehStationary) == 0U)
  {
    DtrmnVehStationary_DW.bitsForTID0.is_active_c1_DtrmnVehStationary = 1;
    DtrmnVehStationary_DW.bitsForTID0.is_c1_DtrmnVehStationary =
      DtrmnVehStationary_IN_Initialize;
    DtrmnVehStationary_DW.bitsForTID0.f_initialize_DtrmnVehStationary = true;
  } else if (((uint32_T)
              DtrmnVehStationary_DW.bitsForTID0.is_c1_DtrmnVehStationary) ==
             DtrmnVehStationary_IN_Initialize) {
    if (DtrmnVehStationary_DW.bitsForTID0.f_initialize_DtrmnVehStationary) {
      DtrmnVehStationary_DW.bitsForTID0.is_c1_DtrmnVehStationary =
        DtrmnVehStationary_IN_Running;
      DtrmnVehStationary_DW.bitsForTID0.f_initialize_DtrmnVehStationary = false;
    }
  } else {
    DtrmnVehStationary_DW.bitsForTID0.f_initialize_DtrmnVehStationary = false;
  }

  // End of Chart: '<S1>/Chart'

  // Outputs for Enabled SubSystem: '<S1>/Differentiator Generator' incorporates:
  //   EnablePort: '<S3>/Enable'

  if (DtrmnVehStationary_DW.bitsForTID0.f_initialize_DtrmnVehStationary) {
    // MATLAB Function: '<S3>/FindDifferentiatorCoeff'
    DtrmnVehStationary_DW.diff_num[0] = 0.724336267F;
    DtrmnVehStationary_DW.diff_denom[0] = 1.0F;
    DtrmnVehStationary_DW.diff_num[1] = 0.724336267F;
    DtrmnVehStationary_DW.diff_denom[1] = -0.822694957F;
    DtrmnVehStationary_DW.diff_num[2] = -0.724336267F;
    DtrmnVehStationary_DW.diff_denom[2] = -0.98551327F;
    DtrmnVehStationary_DW.diff_num[3] = -0.724336267F;
    DtrmnVehStationary_DW.diff_denom[3] = 0.837181628F;
  }

  // End of Outputs for SubSystem: '<S1>/Differentiator Generator'

  // SignalConversion: '<S4>/TmpSignal ConversionAtDifferentiatorInport1'
  rtb_TmpSignalConversionAtDifferentiatorInport1[0] = *rtu_raw_yaw_rate_rps;
  rtb_TmpSignalConversionAtDifferentiatorInport1[1] = *rtu_raw_lat_accel;
  rtb_TmpSignalConversionAtDifferentiatorInport1[2] = *rtu_raw_long_accel;

  // DiscreteTransferFcn: '<S4>/Differentiator'
  for (k = 0; k < 3; k++) {
    memOffset = k * 3;
    Differentiator_tmp_tmp =
      DtrmnVehStationary_DW.Differentiator_states[memOffset + 1];
    Differentiator_tmp_tmp_0 =
      DtrmnVehStationary_DW.Differentiator_states[memOffset + 2];
    DtrmnVehStationary_DW.Differentiator_tmp[k] =
      ((rtb_TmpSignalConversionAtDifferentiatorInport1[k] -
        (DtrmnVehStationary_DW.diff_denom[1] *
         DtrmnVehStationary_DW.Differentiator_states[memOffset])) -
       (Differentiator_tmp_tmp * DtrmnVehStationary_DW.diff_denom[2])) -
      (Differentiator_tmp_tmp_0 * DtrmnVehStationary_DW.diff_denom[3]);
    rtb_Differentiator[k] = (((DtrmnVehStationary_DW.diff_num[0] *
      DtrmnVehStationary_DW.Differentiator_tmp[k]) +
      (DtrmnVehStationary_DW.diff_num[1] *
       DtrmnVehStationary_DW.Differentiator_states[memOffset])) +
      (Differentiator_tmp_tmp * DtrmnVehStationary_DW.diff_num[2])) +
      (Differentiator_tmp_tmp_0 * DtrmnVehStationary_DW.diff_num[3]);
  }

  // End of DiscreteTransferFcn: '<S4>/Differentiator'

  // Outputs for Atomic SubSystem: '<S4>/Delay_set1'
  // Chart: '<S16>/Delay_set' incorporates:
  //   Abs: '<S4>/Abs1'
  //   Abs: '<S4>/Abs3'
  //   Abs: '<S4>/Abs5'
  //   Constant: '<S11>/Constant'
  //   Constant: '<S12>/Constant'
  //   Constant: '<S13>/Constant'
  //   Constant: '<S15>/Constant'
  //   Constant: '<S4>/Constant5'
  //   Constant: '<S6>/Constant'
  //   Constant: '<S7>/Constant'
  //   Constant: '<S8>/Constant'
  //   Logic: '<S4>/Logical Operator'
  //   Logic: '<S4>/Logical Operator1'
  //   Logic: '<S4>/Logical Operator2'
  //   RelationalOperator: '<S11>/Compare'
  //   RelationalOperator: '<S12>/Compare'
  //   RelationalOperator: '<S13>/Compare'
  //   RelationalOperator: '<S15>/Compare'
  //   RelationalOperator: '<S6>/Compare'
  //   RelationalOperator: '<S7>/Compare'
  //   RelationalOperator: '<S8>/Compare'
  //
  //  Block description for '<S16>/Delay_set':
  //   _FCA1PX.Clib.000_57_

  // _FCA1PX.Clib.111_2_
  if ((((fabsf(rtb_Differentiator[0]) <= 0.01F) && (fabsf(rtb_Differentiator[2])
         <= 0.5F)) && (fabsf(rtb_Differentiator[1]) <= 0.5F)) &&
      (((((*rtu_wheel_lin_speed_mps_front_left) <= 0.01F) &&
         ((*rtu_wheel_lin_speed_mps_front_right) <= 0.01F)) &&
        ((*rtu_wheel_lin_speed_mps_rear_left) <= 0.01F)) &&
       ((*rtu_wheel_lin_speed_mps_rear_right) <= 0.01F))) {
    if (DtrmnVehStationary_DW.count >= 1000U) {
      *rty_f_stationary = true;
    } else {
      *rty_f_stationary = false;
      DtrmnVehStationary_DW.count += 10U;
    }
  } else {
    *rty_f_stationary = false;
    DtrmnVehStationary_DW.count = 0U;
  }

  // End of Chart: '<S16>/Delay_set'
  // End of Outputs for SubSystem: '<S4>/Delay_set1'

  // Switch: '<S4>/Switch' incorporates:
  //   Constant: '<S10>/Constant'
  //   Constant: '<S14>/Constant'
  //   Constant: '<S9>/Constant'
  //   Logic: '<S4>/Logical Operator3'
  //   RelationalOperator: '<S10>/Compare'
  //   RelationalOperator: '<S14>/Compare'
  //   RelationalOperator: '<S9>/Compare'

  if (((((uint32_T)(*rtu_raw_yaw_rate_qf)) != ACCURATED) || (((uint32_T)
         (*rtu_raw_lat_accel_qf)) != ACCURATED)) || (((uint32_T)
        (*rtu_raw_long_accel_qf)) != ACCURATED)) {
    *rty_f_stationary = *rtu_external_stationary_flag;
  }

  // End of Switch: '<S4>/Switch'

  // Update for DiscreteTransferFcn: '<S4>/Differentiator'
  for (k = 0; k < 3; k++) {
    memOffset = k * 3;
    DtrmnVehStationary_DW.Differentiator_states[memOffset + 2] =
      DtrmnVehStationary_DW.Differentiator_states[memOffset + 1];
    DtrmnVehStationary_DW.Differentiator_states[memOffset + 1] =
      DtrmnVehStationary_DW.Differentiator_states[memOffset];
    DtrmnVehStationary_DW.Differentiator_states[memOffset] =
      DtrmnVehStationary_DW.Differentiator_tmp[k];
  }

  // End of Update for DiscreteTransferFcn: '<S4>/Differentiator'
}

// Constructor
DtrmnVehStationaryModelClass::DtrmnVehStationaryModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
DtrmnVehStationaryModelClass::~DtrmnVehStationaryModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
