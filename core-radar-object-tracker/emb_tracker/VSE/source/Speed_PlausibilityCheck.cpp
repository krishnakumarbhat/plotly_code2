//
// File: Speed_PlausibilityCheck.cpp
//
// Code generated for Simulink model 'Speed_PlausibilityCheck'.
//
// Model version                  : 1.169
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:01 2023
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
#include "Speed_PlausibilityCheck.h"
#include "Speed_PlausibilityCheck_private.h"

// Named constants for Chart: '<S1>/Chart'
#define Speed_PlausibilityCheck_IN_Initialize ((uint8_T)1U)
#define Speed_PlausibilityCheck_IN_Running    ((uint8_T)2U)

// Output and update for referenced model: 'Speed_PlausibilityCheck'
void Speed_PlausibilityCheckModelClass::step(const enum_quality_factor_T *rtu_raw_speed_qf_in, const real32_T *rtu_raw_speed_mps,
                                             enum_quality_factor_T *rty_raw_speed_qf_out)
{
   real32_T numAccum;
   boolean_T rtb_LogicalOperator;
   boolean_T rtb_LogicalOperator_e;

   // Chart: '<S1>/Chart'
   if (((uint32_T)Speed_PlausibilityCheck_DW.bitsForTID0.is_active_c1_Speed_PlausibilityCheck) == 0U)
   {
      Speed_PlausibilityCheck_DW.bitsForTID0.is_active_c1_Speed_PlausibilityCheck = 1;
      Speed_PlausibilityCheck_DW.bitsForTID0.is_c1_Speed_PlausibilityCheck        = Speed_PlausibilityCheck_IN_Initialize;
      Speed_PlausibilityCheck_DW.bitsForTID0.f_initialize_speed_plausibility      = true;
   }
   else if (((uint32_T)Speed_PlausibilityCheck_DW.bitsForTID0.is_c1_Speed_PlausibilityCheck) ==
            Speed_PlausibilityCheck_IN_Initialize)
   {
      if (Speed_PlausibilityCheck_DW.bitsForTID0.f_initialize_speed_plausibility)
      {
         Speed_PlausibilityCheck_DW.bitsForTID0.is_c1_Speed_PlausibilityCheck   = Speed_PlausibilityCheck_IN_Running;
         Speed_PlausibilityCheck_DW.bitsForTID0.f_initialize_speed_plausibility = false;
      }
   }
   else
   {
      Speed_PlausibilityCheck_DW.bitsForTID0.f_initialize_speed_plausibility = false;
   }

   // End of Chart: '<S1>/Chart'

   // Outputs for Enabled SubSystem: '<S1>/Initialization' incorporates:
   //   EnablePort: '<S4>/Enable'

   if (Speed_PlausibilityCheck_DW.bitsForTID0.f_initialize_speed_plausibility)
   {
      // MATLAB Function: '<S4>/FindDifferentiatorCoeff'
      Speed_PlausibilityCheck_DW.diff_num[0]   = 2.69821119F;
      Speed_PlausibilityCheck_DW.diff_denom[0] = 1.0F;
      Speed_PlausibilityCheck_DW.diff_num[1]   = 2.69821119F;
      Speed_PlausibilityCheck_DW.diff_denom[1] = -0.142980486F;
      Speed_PlausibilityCheck_DW.diff_num[2]   = -2.69821119F;
      Speed_PlausibilityCheck_DW.diff_denom[2] = -0.730178893F;
      Speed_PlausibilityCheck_DW.diff_num[3]   = -2.69821119F;
      Speed_PlausibilityCheck_DW.diff_denom[3] = 0.412801594F;
   }

   // End of Outputs for SubSystem: '<S1>/Initialization'

   // DiscreteTransferFcn: '<S1>/Differentiator'
   Speed_PlausibilityCheck_DW.Differentiator_tmp =
      (((*rtu_raw_speed_mps) - (Speed_PlausibilityCheck_DW.diff_denom[1] * Speed_PlausibilityCheck_DW.Differentiator_states[0])) -
       (Speed_PlausibilityCheck_DW.diff_denom[2] * Speed_PlausibilityCheck_DW.Differentiator_states[1])) -
      (Speed_PlausibilityCheck_DW.diff_denom[3] * Speed_PlausibilityCheck_DW.Differentiator_states[2]);
   numAccum = (((Speed_PlausibilityCheck_DW.diff_num[0] * Speed_PlausibilityCheck_DW.Differentiator_tmp) +
                (Speed_PlausibilityCheck_DW.diff_num[1] * Speed_PlausibilityCheck_DW.Differentiator_states[0])) +
               (Speed_PlausibilityCheck_DW.diff_num[2] * Speed_PlausibilityCheck_DW.Differentiator_states[1])) +
              (Speed_PlausibilityCheck_DW.diff_num[3] * Speed_PlausibilityCheck_DW.Differentiator_states[2]);

   // Logic: '<S6>/Logical Operator' incorporates:
   //   Constant: '<S6>/Constant1'
   //   Constant: '<S6>/Constant2'
   //   DiscreteTransferFcn: '<S1>/Differentiator'
   //   RelationalOperator: '<S6>/Equal'
   //   RelationalOperator: '<S6>/Equal1'

   rtb_LogicalOperator = ((numAccum >= -10.0F) && (numAccum <= 8.0F));

   // Logic: '<S5>/Logical Operator' incorporates:
   //   Constant: '<S5>/Constant'
   //   Constant: '<S5>/Constant1'
   //   RelationalOperator: '<S5>/Equal'
   //   RelationalOperator: '<S5>/Equal1'

   rtb_LogicalOperator_e = (((*rtu_raw_speed_mps) >= 0.0F) && ((*rtu_raw_speed_mps) <= 150.0F));

   // Switch: '<S1>/Switch' incorporates:
   //   Constant: '<S3>/Constant'
   //   RelationalOperator: '<S3>/Compare'

   if (((uint32_T)(*rtu_raw_speed_qf_in)) == ACCURATED)
   {
      // MATLAB Function: '<S1>/call_updateSpeedQF'
      if (rtb_LogicalOperator_e && rtb_LogicalOperator)
      {
         *rty_raw_speed_qf_out = ACCURATED;
      }
      else if ((!rtb_LogicalOperator_e) && rtb_LogicalOperator)
      {
         *rty_raw_speed_qf_out = TEMP_UNDEFINED;
      }
      else if ((!rtb_LogicalOperator) && rtb_LogicalOperator_e)
      {
         *rty_raw_speed_qf_out = NOT_ACCURATED;
      }
      else
      {
         *rty_raw_speed_qf_out = UNDEFINED;
      }

      // End of MATLAB Function: '<S1>/call_updateSpeedQF'
   }
   else
   {
      *rty_raw_speed_qf_out = *rtu_raw_speed_qf_in;
   }

   // End of Switch: '<S1>/Switch'

   // Update for DiscreteTransferFcn: '<S1>/Differentiator'
   Speed_PlausibilityCheck_DW.Differentiator_states[2] = Speed_PlausibilityCheck_DW.Differentiator_states[1];
   Speed_PlausibilityCheck_DW.Differentiator_states[1] = Speed_PlausibilityCheck_DW.Differentiator_states[0];
   Speed_PlausibilityCheck_DW.Differentiator_states[0] = Speed_PlausibilityCheck_DW.Differentiator_tmp;
}

// Constructor
Speed_PlausibilityCheckModelClass::Speed_PlausibilityCheckModelClass()
{
   // Currently there is no constructor body generated.
}

// Destructor
Speed_PlausibilityCheckModelClass::~Speed_PlausibilityCheckModelClass()
{
   // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
