//
// File: LateralJerkEstimation.cpp
//
// Code generated for Simulink model 'LateralJerkEstimation'.
//
// Model version                  : 1.355
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:57:23 2024
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
#include "LateralJerkEstimation.h"
#include "LateralJerkEstimation_private.h"

// Named constants for Chart: '<S1>/Chart'
#define LateralJerkEstimation_IN_Initialize ((uint8_T)1U)
#define LateralJerkEstimation_IN_Running ((uint8_T)2U)

// Output and update for referenced model: 'LateralJerkEstimation'
void LateralJerkEstimationModelClass::step(const real32_T
  *rtu_Lateral_Acceleration, const enum_quality_factor_T
  *rtu_Raw_Lateral_Acceleration_qf, real32_T *rty_Lateral_Jerk, real32_T
  *rty_Filt_Lateral_Acceleration, enum_quality_factor_T *rty_Lateral_Jerk_qf,
  enum_quality_factor_T *rty_Filt_Lateral_Acceleration_qf)
{
  int32_T j;
  int32_T denIdx;
  real_T rtb_Abs;
  static const real_T tmp[9] = { 0.00456979326481199, -0.0239216615486164,
    0.0550095356425841, -0.0684109653052692, 0.0424378518633043,
    -0.000466222799784696, -0.0190351874386231, 0.0128395886483602,
    -0.00302270967772132 };

  static const real_T tmp_0[9] = { 1.0, -6.99425629357504, 21.5755709725485,
    -38.3524990416117, 42.9801932125239, -31.0998105951794, 14.1900027564518,
    -3.73241941610446, 0.433220712500325 };

  static const real_T tmp_1[5] = { 0.0097355705758097114, -0.032135368264455524,
    0.045449987251035384, -0.032135368264455511, 0.0097355705758096975 };

  static const real_T tmp_2[5] = { 1.0, -3.5729428344521863, 4.8079147193565426,
    -2.8863252161892525, 0.65200372315863975 };

  // Chart: '<S1>/Chart'
  if (((uint32_T)
       LateralJerkEstimation_DW.bitsForTID0.is_active_c4_LateralJerkEstimation) ==
      0U) {
    LateralJerkEstimation_DW.bitsForTID0.is_active_c4_LateralJerkEstimation = 1;
    LateralJerkEstimation_DW.bitsForTID0.is_c4_LateralJerkEstimation =
      LateralJerkEstimation_IN_Initialize;
    LateralJerkEstimation_DW.bitsForTID0.f_initialize_LateralJerkEstimation =
      true;
  } else if (((uint32_T)
              LateralJerkEstimation_DW.bitsForTID0.is_c4_LateralJerkEstimation) ==
             LateralJerkEstimation_IN_Initialize) {
    if (LateralJerkEstimation_DW.bitsForTID0.f_initialize_LateralJerkEstimation)
    {
      LateralJerkEstimation_DW.bitsForTID0.is_c4_LateralJerkEstimation =
        LateralJerkEstimation_IN_Running;
      LateralJerkEstimation_DW.bitsForTID0.f_initialize_LateralJerkEstimation =
        false;
    }
  } else {
    LateralJerkEstimation_DW.bitsForTID0.f_initialize_LateralJerkEstimation =
      false;
  }

  // End of Chart: '<S1>/Chart'

  // Outputs for Enabled SubSystem: '<S1>/Filter Generator' incorporates:
  //   EnablePort: '<S4>/Enable'

  // Outputs for Enabled SubSystem: '<S1>/Differentiator Generator' incorporates:
  //   EnablePort: '<S3>/Enable'

  if (LateralJerkEstimation_DW.bitsForTID0.f_initialize_LateralJerkEstimation) {
    // MATLAB Function: '<S3>/FindDifferentiatorCoeff'
    memcpy(&LateralJerkEstimation_DW.diff_num[0], &tmp[0], 9U * (sizeof(real_T)));
    memcpy(&LateralJerkEstimation_DW.diff_denom[0], &tmp_0[0], 9U * (sizeof
            (real_T)));

    // MATLAB Function: '<S4>/FindFilterCoeff'
    for (j = 0; j < 5; j++) {
      LateralJerkEstimation_DW.filter_discrete_num[j] = tmp_1[j];
      LateralJerkEstimation_DW.filter_discrete_denom[j] = tmp_2[j];
    }

    // End of MATLAB Function: '<S4>/FindFilterCoeff'
  }

  // End of Outputs for SubSystem: '<S1>/Differentiator Generator'
  // End of Outputs for SubSystem: '<S1>/Filter Generator'

  // DiscreteTransferFcn: '<S5>/LowPass Filter' incorporates:
  //   DataTypeConversion: '<S1>/Data Type Conversion'

  LateralJerkEstimation_DW.LowPassFilter_tmp = (((((real_T)
    (*rtu_Lateral_Acceleration)) -
    (LateralJerkEstimation_DW.filter_discrete_denom[1] *
     LateralJerkEstimation_DW.LowPassFilter_states[0])) -
    (LateralJerkEstimation_DW.filter_discrete_denom[2] *
     LateralJerkEstimation_DW.LowPassFilter_states[1])) -
    (LateralJerkEstimation_DW.filter_discrete_denom[3] *
     LateralJerkEstimation_DW.LowPassFilter_states[2])) -
    (LateralJerkEstimation_DW.filter_discrete_denom[4] *
     LateralJerkEstimation_DW.LowPassFilter_states[3]);
  rtb_Abs = ((((LateralJerkEstimation_DW.filter_discrete_num[0] *
                LateralJerkEstimation_DW.LowPassFilter_tmp) +
               (LateralJerkEstimation_DW.filter_discrete_num[1] *
                LateralJerkEstimation_DW.LowPassFilter_states[0])) +
              (LateralJerkEstimation_DW.filter_discrete_num[2] *
               LateralJerkEstimation_DW.LowPassFilter_states[1])) +
             (LateralJerkEstimation_DW.filter_discrete_num[3] *
              LateralJerkEstimation_DW.LowPassFilter_states[2])) +
    (LateralJerkEstimation_DW.filter_discrete_num[4] *
     LateralJerkEstimation_DW.LowPassFilter_states[3]);

  // Switch: '<S5>/Switch' incorporates:
  //   Abs: '<S5>/Abs'
  //   Constant: '<S5>/Constant'
  //   Constant: '<S9>/Constant'
  //   DiscreteTransferFcn: '<S5>/LowPass Filter'
  //   RelationalOperator: '<S9>/Compare'

  if (fabs(rtb_Abs) <= 1.0E-10) {
    rtb_Abs = 0.0;
  }

  // End of Switch: '<S5>/Switch'

  // DataTypeConversion: '<S5>/Data Type Conversion'
  *rty_Filt_Lateral_Acceleration = (real32_T)rtb_Abs;

  // DiscreteTransferFcn: '<S6>/Differentiator'
  LateralJerkEstimation_DW.Differentiator_tmp = (real_T)
    (*rtu_Lateral_Acceleration);
  denIdx = 1;
  for (j = 0; j < 8; j++) {
    LateralJerkEstimation_DW.Differentiator_tmp -=
      LateralJerkEstimation_DW.diff_denom[denIdx] *
      LateralJerkEstimation_DW.Differentiator_states[j];
    denIdx++;
  }

  rtb_Abs = LateralJerkEstimation_DW.diff_num[0] *
    LateralJerkEstimation_DW.Differentiator_tmp;
  denIdx = 1;
  for (j = 0; j < 8; j++) {
    rtb_Abs += LateralJerkEstimation_DW.diff_num[denIdx] *
      LateralJerkEstimation_DW.Differentiator_states[j];
    denIdx++;
  }

  // Switch: '<S6>/Switch' incorporates:
  //   Abs: '<S6>/Abs'
  //   Constant: '<S10>/Constant'
  //   Constant: '<S6>/Constant'
  //   DiscreteTransferFcn: '<S6>/Differentiator'
  //   RelationalOperator: '<S10>/Compare'

  if (fabs(rtb_Abs) <= 1.0E-10) {
    rtb_Abs = 0.0;
  }

  // End of Switch: '<S6>/Switch'

  // DataTypeConversion: '<S6>/Data Type Conversion'
  *rty_Lateral_Jerk = (real32_T)rtb_Abs;

  // Inport: '<Root>/Raw_Lateral_Acceleration_qf'
  *rty_Filt_Lateral_Acceleration_qf = *rtu_Raw_Lateral_Acceleration_qf;

  // SignalConversion: '<Root>/TmpSignal ConversionAtLateral_Jerk_qfInport1'
  *rty_Lateral_Jerk_qf = *rty_Filt_Lateral_Acceleration_qf;

  // Update for DiscreteTransferFcn: '<S5>/LowPass Filter'
  LateralJerkEstimation_DW.LowPassFilter_states[3] =
    LateralJerkEstimation_DW.LowPassFilter_states[2];
  LateralJerkEstimation_DW.LowPassFilter_states[2] =
    LateralJerkEstimation_DW.LowPassFilter_states[1];
  LateralJerkEstimation_DW.LowPassFilter_states[1] =
    LateralJerkEstimation_DW.LowPassFilter_states[0];
  LateralJerkEstimation_DW.LowPassFilter_states[0] =
    LateralJerkEstimation_DW.LowPassFilter_tmp;

  // Update for DiscreteTransferFcn: '<S6>/Differentiator'
  for (j = 0; j < 7; j++) {
    LateralJerkEstimation_DW.Differentiator_states[7 - j] =
      LateralJerkEstimation_DW.Differentiator_states[6 - j];
  }

  LateralJerkEstimation_DW.Differentiator_states[0] =
    LateralJerkEstimation_DW.Differentiator_tmp;

  // End of Update for DiscreteTransferFcn: '<S6>/Differentiator'
}

// Constructor
LateralJerkEstimationModelClass::LateralJerkEstimationModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
LateralJerkEstimationModelClass::~LateralJerkEstimationModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
