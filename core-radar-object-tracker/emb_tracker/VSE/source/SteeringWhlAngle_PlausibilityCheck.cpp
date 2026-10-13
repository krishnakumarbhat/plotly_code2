//
// File: SteeringWhlAngle_PlausibilityCheck.cpp
//
// Code generated for Simulink model 'SteeringWhlAngle_PlausibilityCheck'.
//
// Model version                  : 1.108
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:05 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 64-bit (LLP64)
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
#include "SteeringWhlAngle_PlausibilityCheck.h"
#include "SteeringWhlAngle_PlausibilityCheck_private.h"

// Named constants for Chart: '<S1>/Chart'
#define SteeringWhlAngle_PlausibilityCheck_IN_Initialize ((uint8_T)1U)
#define SteeringWhlAngle_PlausibilityCheck_IN_Running    ((uint8_T)2U)

// Output and update for referenced model: 'SteeringWhlAngle_PlausibilityCheck'
void SteeringWhlAngle_PlausibilityCheckModelClass::step(const real32_T *rtu_raw_steering_angle_deg,
                                                        const enum_quality_factor_T *rtu_raw_steering_angle_qf_in,
                                                        enum_quality_factor_T *rty_raw_steering_angle_qf_out)
{
   real32_T numAccum;
   boolean_T rtb_Confi_Level_SteeringWhlAngleRate;
   boolean_T rtb_Confi_Level_SteeringWhlAngle;

   // Chart: '<S1>/Chart'
   if (((uint32_T)SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.is_active_c2_SteeringWhlAngle_PlausibilityCheck) == 0U)
   {
      SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.is_active_c2_SteeringWhlAngle_PlausibilityCheck = 1;
      SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.is_c2_SteeringWhlAngle_PlausibilityCheck =
         SteeringWhlAngle_PlausibilityCheck_IN_Initialize;
      SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.f_initialize_steering_whl_angle_plausibility = true;
   }
   else if (((uint32_T)SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.is_c2_SteeringWhlAngle_PlausibilityCheck) ==
            SteeringWhlAngle_PlausibilityCheck_IN_Initialize)
   {
      if (SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.f_initialize_steering_whl_angle_plausibility)
      {
         SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.is_c2_SteeringWhlAngle_PlausibilityCheck =
            SteeringWhlAngle_PlausibilityCheck_IN_Running;
         SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.f_initialize_steering_whl_angle_plausibility = false;
      }
   }
   else
   {
      SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.f_initialize_steering_whl_angle_plausibility = false;
   }

   // End of Chart: '<S1>/Chart'

   // Outputs for Enabled SubSystem: '<S1>/Initialization' incorporates:
   //   EnablePort: '<S3>/Enable'

   if (SteeringWhlAngle_PlausibilityCheck_DW.bitsForTID0.f_initialize_steering_whl_angle_plausibility)
   {
      // MATLAB Function: '<S3>/FindDifferentiatorCoeff'
      SteeringWhlAngle_PlausibilityCheck_DW.diff_num[0]   = 2.69821119F;
      SteeringWhlAngle_PlausibilityCheck_DW.diff_denom[0] = 1.0F;
      SteeringWhlAngle_PlausibilityCheck_DW.diff_num[1]   = 2.69821119F;
      SteeringWhlAngle_PlausibilityCheck_DW.diff_denom[1] = -0.142980486F;
      SteeringWhlAngle_PlausibilityCheck_DW.diff_num[2]   = -2.69821119F;
      SteeringWhlAngle_PlausibilityCheck_DW.diff_denom[2] = -0.730178893F;
      SteeringWhlAngle_PlausibilityCheck_DW.diff_num[3]   = -2.69821119F;
      SteeringWhlAngle_PlausibilityCheck_DW.diff_denom[3] = 0.412801594F;
   }

   // End of Outputs for SubSystem: '<S1>/Initialization'

   // DiscreteTransferFcn: '<S5>/Differentiator'
   SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_tmp =
      (((*rtu_raw_steering_angle_deg) -
        (SteeringWhlAngle_PlausibilityCheck_DW.diff_denom[1] * SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[0])) -
       (SteeringWhlAngle_PlausibilityCheck_DW.diff_denom[2] * SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[1])) -
      (SteeringWhlAngle_PlausibilityCheck_DW.diff_denom[3] * SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[2]);
   numAccum =
      (((SteeringWhlAngle_PlausibilityCheck_DW.diff_num[0] * SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_tmp) +
        (SteeringWhlAngle_PlausibilityCheck_DW.diff_num[1] * SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[0])) +
       (SteeringWhlAngle_PlausibilityCheck_DW.diff_num[2] * SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[1])) +
      (SteeringWhlAngle_PlausibilityCheck_DW.diff_num[3] * SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[2]);

   // Logic: '<S5>/Logical Operator' incorporates:
   //   Constant: '<S10>/Constant'
   //   Constant: '<S11>/Constant'
   //   DiscreteTransferFcn: '<S5>/Differentiator'
   //   RelationalOperator: '<S10>/Compare'
   //   RelationalOperator: '<S11>/Compare'

   rtb_Confi_Level_SteeringWhlAngleRate = ((numAccum <= 2000.0F) && (numAccum >= -2000.0F));

   // Logic: '<S4>/Logical Operator' incorporates:
   //   Constant: '<S8>/Constant'
   //   Constant: '<S9>/Constant'
   //   RelationalOperator: '<S8>/Compare'
   //   RelationalOperator: '<S9>/Compare'

   rtb_Confi_Level_SteeringWhlAngle = (((*rtu_raw_steering_angle_deg) <= 720.0F) && ((*rtu_raw_steering_angle_deg) >= -720.0F));

   // MATLAB Function: '<S1>/Updated_QualityFactor'
   if (((uint32_T)(*rtu_raw_steering_angle_qf_in)) != ACCURATED)
   {
      *rty_raw_steering_angle_qf_out = *rtu_raw_steering_angle_qf_in;
   }
   else if (rtb_Confi_Level_SteeringWhlAngle && rtb_Confi_Level_SteeringWhlAngleRate)
   {
      *rty_raw_steering_angle_qf_out = 3U;
   }
   else if ((!rtb_Confi_Level_SteeringWhlAngle) && rtb_Confi_Level_SteeringWhlAngleRate)
   {
      *rty_raw_steering_angle_qf_out = TEMP_UNDEFINED;
   }
   else if (rtb_Confi_Level_SteeringWhlAngle && (!rtb_Confi_Level_SteeringWhlAngleRate))
   {
      *rty_raw_steering_angle_qf_out = NOT_ACCURATED;
   }
   else
   {
      *rty_raw_steering_angle_qf_out = UNDEFINED;
   }

   // End of MATLAB Function: '<S1>/Updated_QualityFactor'

   // Update for DiscreteTransferFcn: '<S5>/Differentiator'
   SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[2] = SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[1];
   SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[1] = SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[0];
   SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_states[0] = SteeringWhlAngle_PlausibilityCheck_DW.Differentiator_tmp;
}

// Constructor
SteeringWhlAngle_PlausibilityCheckModelClass::SteeringWhlAngle_PlausibilityCheckModelClass()
{
   // Currently there is no constructor body generated.
}

// Destructor
SteeringWhlAngle_PlausibilityCheckModelClass::~SteeringWhlAngle_PlausibilityCheckModelClass()
{
   // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
