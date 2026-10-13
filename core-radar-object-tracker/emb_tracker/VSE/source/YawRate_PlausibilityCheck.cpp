//
// File: YawRate_PlausibilityCheck.cpp
//
// Code generated for Simulink model 'YawRate_PlausibilityCheck'.
//
// Model version                  : 1.205
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:31 2023
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
#include "YawRate_PlausibilityCheck.h"
#include "YawRate_PlausibilityCheck_private.h"

// Named constants for Chart: '<S1>/Chart'
#define YawRate_PlausibilityCheck_IN_Initialize ((uint8_T)1U)
#define YawRate_PlausibilityCheck_IN_Running    ((uint8_T)2U)

// Output and update for referenced model: 'YawRate_PlausibilityCheck'
void YawRate_PlausibilityCheckModelClass::step(const real32_T *rtu_raw_yaw_rate_rps,
                                               const enum_quality_factor_T *rtu_raw_yaw_rate_qf_in,
                                               enum_quality_factor_T *rty_raw_yaw_rate_qf_out)
{
   real32_T rtb_Gain;
   boolean_T rtb_LogicalOperator;
   boolean_T rtb_LogicalOperator_p;

   // Chart: '<S1>/Chart'
   if (((uint32_T)YawRate_PlausibilityCheck_DW.bitsForTID0.is_active_c4_YawRate_PlausibilityCheck) == 0U)
   {
      YawRate_PlausibilityCheck_DW.bitsForTID0.is_active_c4_YawRate_PlausibilityCheck = 1;
      YawRate_PlausibilityCheck_DW.bitsForTID0.is_c4_YawRate_PlausibilityCheck        = YawRate_PlausibilityCheck_IN_Initialize;
      YawRate_PlausibilityCheck_DW.bitsForTID0.f_initialize_yaw_rate_plausibility     = true;
   }
   else if (((uint32_T)YawRate_PlausibilityCheck_DW.bitsForTID0.is_c4_YawRate_PlausibilityCheck) ==
            YawRate_PlausibilityCheck_IN_Initialize)
   {
      if (YawRate_PlausibilityCheck_DW.bitsForTID0.f_initialize_yaw_rate_plausibility)
      {
         YawRate_PlausibilityCheck_DW.bitsForTID0.is_c4_YawRate_PlausibilityCheck    = YawRate_PlausibilityCheck_IN_Running;
         YawRate_PlausibilityCheck_DW.bitsForTID0.f_initialize_yaw_rate_plausibility = false;
      }
   }
   else
   {
      YawRate_PlausibilityCheck_DW.bitsForTID0.f_initialize_yaw_rate_plausibility = false;
   }

   // End of Chart: '<S1>/Chart'

   // Outputs for Enabled SubSystem: '<S1>/Initialization' incorporates:
   //   EnablePort: '<S6>/Enable'

   if (YawRate_PlausibilityCheck_DW.bitsForTID0.f_initialize_yaw_rate_plausibility)
   {
      // MATLAB Function: '<S6>/FindDifferentiatorCoeff'
      YawRate_PlausibilityCheck_DW.diff_num[0]   = 2.69821119F;
      YawRate_PlausibilityCheck_DW.diff_denom[0] = 1.0F;
      YawRate_PlausibilityCheck_DW.diff_num[1]   = 2.69821119F;
      YawRate_PlausibilityCheck_DW.diff_denom[1] = -0.142980486F;
      YawRate_PlausibilityCheck_DW.diff_num[2]   = -2.69821119F;
      YawRate_PlausibilityCheck_DW.diff_denom[2] = -0.730178893F;
      YawRate_PlausibilityCheck_DW.diff_num[3]   = -2.69821119F;
      YawRate_PlausibilityCheck_DW.diff_denom[3] = 0.412801594F;
   }

   // End of Outputs for SubSystem: '<S1>/Initialization'

   // DiscreteTransferFcn: '<S5>/Differentiator'
   YawRate_PlausibilityCheck_DW.Differentiator_tmp =
      (((*rtu_raw_yaw_rate_rps) -
        (YawRate_PlausibilityCheck_DW.diff_denom[1] * YawRate_PlausibilityCheck_DW.Differentiator_states[0])) -
       (YawRate_PlausibilityCheck_DW.diff_denom[2] * YawRate_PlausibilityCheck_DW.Differentiator_states[1])) -
      (YawRate_PlausibilityCheck_DW.diff_denom[3] * YawRate_PlausibilityCheck_DW.Differentiator_states[2]);

   // Gain: '<S8>/Gain' incorporates:
   //   DiscreteTransferFcn: '<S5>/Differentiator'

   rtb_Gain = ((((YawRate_PlausibilityCheck_DW.diff_num[0] * YawRate_PlausibilityCheck_DW.Differentiator_tmp) +
                 (YawRate_PlausibilityCheck_DW.diff_num[1] * YawRate_PlausibilityCheck_DW.Differentiator_states[0])) +
                (YawRate_PlausibilityCheck_DW.diff_num[2] * YawRate_PlausibilityCheck_DW.Differentiator_states[1])) +
               (YawRate_PlausibilityCheck_DW.diff_num[3] * YawRate_PlausibilityCheck_DW.Differentiator_states[2])) *
              57.2957802F;

   // Logic: '<S2>/Logical Operator' incorporates:
   //   Constant: '<S2>/Constant'
   //   Constant: '<S2>/Constant1'
   //   RelationalOperator: '<S2>/Less Than'
   //   RelationalOperator: '<S2>/Less Than1'

   rtb_LogicalOperator = ((100.0F < rtb_Gain) || (rtb_Gain < -100.0F));

   // Gain: '<S12>/Gain'
   rtb_Gain = 57.2957802F * (*rtu_raw_yaw_rate_rps);

   // Logic: '<S7>/Logical Operator' incorporates:
   //   Constant: '<S7>/Constant'
   //   Constant: '<S7>/Constant1'
   //   RelationalOperator: '<S7>/GreaterThan'
   //   RelationalOperator: '<S7>/Less Than'

   rtb_LogicalOperator_p = ((100.0F < rtb_Gain) || (rtb_Gain < -100.0F));

   // MATLAB Function: '<S4>/DecideQualityFactor'
   if (((uint32_T)(*rtu_raw_yaw_rate_qf_in)) != ACCURATED)
   {
      *rty_raw_yaw_rate_qf_out = *rtu_raw_yaw_rate_qf_in;
   }
   else if (rtb_LogicalOperator_p && rtb_LogicalOperator)
   {
      *rty_raw_yaw_rate_qf_out = UNDEFINED;
   }
   else if (rtb_LogicalOperator_p && (!rtb_LogicalOperator))
   {
      *rty_raw_yaw_rate_qf_out = TEMP_UNDEFINED;
   }
   else if ((!rtb_LogicalOperator_p) && rtb_LogicalOperator)
   {
      *rty_raw_yaw_rate_qf_out = NOT_ACCURATED;
   }
   else
   {
      *rty_raw_yaw_rate_qf_out = ACCURATED;
   }

   // End of MATLAB Function: '<S4>/DecideQualityFactor'

   // Update for DiscreteTransferFcn: '<S5>/Differentiator'
   YawRate_PlausibilityCheck_DW.Differentiator_states[2] = YawRate_PlausibilityCheck_DW.Differentiator_states[1];
   YawRate_PlausibilityCheck_DW.Differentiator_states[1] = YawRate_PlausibilityCheck_DW.Differentiator_states[0];
   YawRate_PlausibilityCheck_DW.Differentiator_states[0] = YawRate_PlausibilityCheck_DW.Differentiator_tmp;
}

// Constructor
YawRate_PlausibilityCheckModelClass::YawRate_PlausibilityCheckModelClass()
{
   // Currently there is no constructor body generated.
}

// Destructor
YawRate_PlausibilityCheckModelClass::~YawRate_PlausibilityCheckModelClass()
{
   // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
