//
// File: Speed_PlausibilityCheck.h
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
#ifndef RTW_HEADER_Speed_PlausibilityCheck_h_
   #define RTW_HEADER_Speed_PlausibilityCheck_h_
   #include <stddef.h>
   #include <string.h>
   #ifndef Speed_PlausibilityCheck_COMMON_INCLUDES_
      #define Speed_PlausibilityCheck_COMMON_INCLUDES_
      #include "rtwtypes.h"
   #endif // Speed_PlausibilityCheck_COMMON_INCLUDES_

   #include "Speed_PlausibilityCheck_types.h"
   #include <stddef.h>

// Block signals and states (default storage) for model 'Speed_PlausibilityCheck'
typedef struct
{
   real32_T diff_num[4];              // '<S4>/FindDifferentiatorCoeff'
   real32_T diff_denom[4];            // '<S4>/FindDifferentiatorCoeff'
   real32_T Differentiator_states[3]; // '<S1>/Differentiator'
   real32_T Differentiator_tmp;       // '<S1>/Differentiator'
   struct
   {
      uint_T is_c1_Speed_PlausibilityCheck:2;        // '<S1>/Chart'
      uint_T is_active_c1_Speed_PlausibilityCheck:1; // '<S1>/Chart'
      uint_T f_initialize_speed_plausibility:1;      // '<S1>/Chart'
   } bitsForTID0;
} DW_Speed_PlausibilityCheck_T;

// Class declaration for model Speed_PlausibilityCheck
class Speed_PlausibilityCheckModelClass
{
   // public data and function members
 public:
   // model step function
   void step(const enum_quality_factor_T *rtu_raw_speed_qf_in, const real32_T *rtu_raw_speed_mps,
             enum_quality_factor_T *rty_raw_speed_qf_out);

   // Constructor
   Speed_PlausibilityCheckModelClass();

   // Destructor
   ~Speed_PlausibilityCheckModelClass();

   // private data and function members
 private:
   // Block signals and states
   DW_Speed_PlausibilityCheck_T Speed_PlausibilityCheck_DW;
};

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
//  '<Root>' : 'Speed_PlausibilityCheck'
//  '<S1>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck'
//  '<S2>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck/Chart'
//  '<S3>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck/Compare To Constant'
//  '<S4>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck/Initialization'
//  '<S5>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck/SpeedRangeCheck'
//  '<S6>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck/Subsystem'
//  '<S7>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck/call_updateSpeedQF'
//  '<S8>'   : 'Speed_PlausibilityCheck/Speed_PlausibilityCheck/Initialization/FindDifferentiatorCoeff'

#endif // RTW_HEADER_Speed_PlausibilityCheck_h_

//
// File trailer for generated code.
//
// [EOF]
//
