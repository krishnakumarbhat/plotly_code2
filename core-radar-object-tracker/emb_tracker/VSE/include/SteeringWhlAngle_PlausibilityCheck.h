//
// File: SteeringWhlAngle_PlausibilityCheck.h
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
#ifndef RTW_HEADER_SteeringWhlAngle_PlausibilityCheck_h_
   #define RTW_HEADER_SteeringWhlAngle_PlausibilityCheck_h_
   #include <stddef.h>
   #include <string.h>
   #ifndef SteeringWhlAngle_PlausibilityCheck_COMMON_INCLUDES_
      #define SteeringWhlAngle_PlausibilityCheck_COMMON_INCLUDES_
      #include "rtwtypes.h"
   #endif // SteeringWhlAngle_PlausibilityCheck_COMMON_INCLUDES_

   #include "SteeringWhlAngle_PlausibilityCheck_types.h"
   #include <stddef.h>

// Block signals and states (default storage) for model 'SteeringWhlAngle_PlausibilityCheck'
typedef struct
{
   real32_T diff_num[4];              // '<S3>/FindDifferentiatorCoeff'
   real32_T diff_denom[4];            // '<S3>/FindDifferentiatorCoeff'
   real32_T Differentiator_states[3]; // '<S5>/Differentiator'
   real32_T Differentiator_tmp;       // '<S5>/Differentiator'
   struct
   {
      uint_T is_c2_SteeringWhlAngle_PlausibilityCheck:2;        // '<S1>/Chart'
      uint_T is_active_c2_SteeringWhlAngle_PlausibilityCheck:1; // '<S1>/Chart'
      uint_T f_initialize_steering_whl_angle_plausibility:1;    // '<S1>/Chart'
   } bitsForTID0;
} DW_SteeringWhlAngle_PlausibilityCheck_T;

// Class declaration for model SteeringWhlAngle_PlausibilityCheck
class SteeringWhlAngle_PlausibilityCheckModelClass
{
   // public data and function members
 public:
   // model step function
   void step(const real32_T *rtu_raw_steering_angle_deg, const enum_quality_factor_T *rtu_raw_steering_angle_qf_in,
             enum_quality_factor_T *rty_raw_steering_angle_qf_out);

   // Constructor
   SteeringWhlAngle_PlausibilityCheckModelClass();

   // Destructor
   ~SteeringWhlAngle_PlausibilityCheckModelClass();

   // private data and function members
 private:
   // Block signals and states
   DW_SteeringWhlAngle_PlausibilityCheck_T SteeringWhlAngle_PlausibilityCheck_DW;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S5>/Scope' : Unused code path elimination

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
//  '<Root>' : 'SteeringWhlAngle_PlausibilityCheck'
//  '<S1>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck'
//  '<S2>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/Chart'
//  '<S3>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/Initialization'
//  '<S4>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/RangeCheck'
//  '<S5>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/RangeRateCheck'
//  '<S6>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/Updated_QualityFactor'
//  '<S7>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/Initialization/FindDifferentiatorCoeff'
//  '<S8>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/RangeCheck/Compare To Constant'
//  '<S9>'   : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/RangeCheck/Compare To Constant1'
//  '<S10>'  : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/RangeRateCheck/Compare To Constant'
//  '<S11>'  : 'SteeringWhlAngle_PlausibilityCheck/SWA_PlausibilityCheck/RangeRateCheck/Compare To Constant1'

#endif // RTW_HEADER_SteeringWhlAngle_PlausibilityCheck_h_

//
// File trailer for generated code.
//
// [EOF]
//
