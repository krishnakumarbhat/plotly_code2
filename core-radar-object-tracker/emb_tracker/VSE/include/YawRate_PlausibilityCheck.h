//
// File: YawRate_PlausibilityCheck.h
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
#ifndef RTW_HEADER_YawRate_PlausibilityCheck_h_
   #define RTW_HEADER_YawRate_PlausibilityCheck_h_
   #include <stddef.h>
   #include <string.h>
   #ifndef YawRate_PlausibilityCheck_COMMON_INCLUDES_
      #define YawRate_PlausibilityCheck_COMMON_INCLUDES_
      #include "rtwtypes.h"
   #endif // YawRate_PlausibilityCheck_COMMON_INCLUDES_

   #include "YawRate_PlausibilityCheck_types.h"
   #include <stddef.h>

// Block signals and states (default storage) for model 'YawRate_PlausibilityCheck'
typedef struct
{
   real32_T diff_num[4];              // '<S6>/FindDifferentiatorCoeff'
   real32_T diff_denom[4];            // '<S6>/FindDifferentiatorCoeff'
   real32_T Differentiator_states[3]; // '<S5>/Differentiator'
   real32_T Differentiator_tmp;       // '<S5>/Differentiator'
   struct
   {
      uint_T is_c4_YawRate_PlausibilityCheck:2;        // '<S1>/Chart'
      uint_T is_active_c4_YawRate_PlausibilityCheck:1; // '<S1>/Chart'
      uint_T f_initialize_yaw_rate_plausibility:1;     // '<S1>/Chart'
   } bitsForTID0;
} DW_YawRate_PlausibilityCheck_T;

// Class declaration for model YawRate_PlausibilityCheck
class YawRate_PlausibilityCheckModelClass
{
   // public data and function members
 public:
   // model step function
   void step(const real32_T *rtu_raw_yaw_rate_rps, const enum_quality_factor_T *rtu_raw_yaw_rate_qf_in,
             enum_quality_factor_T *rty_raw_yaw_rate_qf_out);

   // Constructor
   YawRate_PlausibilityCheckModelClass();

   // Destructor
   ~YawRate_PlausibilityCheckModelClass();

   // private data and function members
 private:
   // Block signals and states
   DW_YawRate_PlausibilityCheck_T YawRate_PlausibilityCheck_DW;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S2>/Scope' : Unused code path elimination
//  Block '<S10>/Gain' : Unused code path elimination
//  Block '<S5>/Scope2' : Unused code path elimination
//  Block '<S1>/Scope' : Unused code path elimination

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
//  '<Root>' : 'YawRate_PlausibilityCheck'
//  '<S1>'   : 'YawRate_PlausibilityCheck/Subsystem'
//  '<S2>'   : 'YawRate_PlausibilityCheck/Subsystem/AngularAccelRangeCheck'
//  '<S3>'   : 'YawRate_PlausibilityCheck/Subsystem/Chart'
//  '<S4>'   : 'YawRate_PlausibilityCheck/Subsystem/DecideQualityFactor'
//  '<S5>'   : 'YawRate_PlausibilityCheck/Subsystem/FindAngularAccel'
//  '<S6>'   : 'YawRate_PlausibilityCheck/Subsystem/Initialization'
//  '<S7>'   : 'YawRate_PlausibilityCheck/Subsystem/YawRateRangeCheck'
//  '<S8>'   : 'YawRate_PlausibilityCheck/Subsystem/AngularAccelRangeCheck/Radians to Degrees'
//  '<S9>'   : 'YawRate_PlausibilityCheck/Subsystem/DecideQualityFactor/DecideQualityFactor'
//  '<S10>'  : 'YawRate_PlausibilityCheck/Subsystem/FindAngularAccel/Radians to Degrees1'
//  '<S11>'  : 'YawRate_PlausibilityCheck/Subsystem/Initialization/FindDifferentiatorCoeff'
//  '<S12>'  : 'YawRate_PlausibilityCheck/Subsystem/YawRateRangeCheck/Radians to Degrees'

#endif // RTW_HEADER_YawRate_PlausibilityCheck_h_

//
// File trailer for generated code.
//
// [EOF]
//
