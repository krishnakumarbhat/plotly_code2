//
// File: TrailerOscillationEstimation_2018b.h
//
// Code generated for Simulink model 'TrailerOscillationEstimation_2018b'.
//
// Model version                  : 1.406
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:58:25 2024
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
#ifndef RTW_HEADER_TrailerOscillationEstimation_2018b_h_
#define RTW_HEADER_TrailerOscillationEstimation_2018b_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef TrailerOscillationEstimation_2018b_COMMON_INCLUDES_
# define TrailerOscillationEstimation_2018b_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // TrailerOscillationEstimation_2018b_COMMON_INCLUDES_ 

#include "TrailerOscillationEstimation_2018b_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'TrailerOscillationEstimation_2018b' 
typedef struct {
  real32_T Delay14_DSTATE[10];         // '<S2>/Delay14'
  real32_T Delay15_DSTATE[10];         // '<S2>/Delay15'
  real32_T Delay16_DSTATE[10];         // '<S2>/Delay16'
  real32_T Delay17_DSTATE[10];         // '<S2>/Delay17'
  real32_T s_n_real_prev_prev[10];     // '<S3>/OscilationDetection'
  real32_T s_n_real_prev[10];          // '<S3>/OscilationDetection'
  real32_T queue_xn[100];              // '<S3>/OscilationDetection'
  real32_T Xk_mag_prev[10];            // '<S3>/OscilationDetection'
  real32_T Delay11_DSTATE;             // '<S2>/Delay11'
  real32_T amplitude_estim_prev;       // '<S3>/OscilationDetection'
  real32_T freq_estim_prev;            // '<S3>/OscilationDetection'
  real32_T x_n_minus_N_prev;           // '<S3>/OscilationDetection'
  uint16_T head;                       // '<S3>/OscilationDetection'
  uint8_T timer_count;                 // '<S3>/periodic_pulse'
} DW_TrailerOscillationEstimation_2018b_T;

// Class declaration for model TrailerOscillationEstimation_2018b
class TrailerOscillationEstimation_2018bModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_HitchAngleRate, real32_T
            *rty_VsVSE_rps_TrlrOscMag, real32_T *rty_VsVSE_hz_TrlrOscFreq);

  // Initial conditions function
  void init();

  // Constructor
  TrailerOscillationEstimation_2018bModelClass();

  // Destructor
  ~TrailerOscillationEstimation_2018bModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_TrailerOscillationEstimation_2018b_T TrailerOscillationEstimation_2018b_DW;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S2>/Constant1' : Unused code path elimination
//  Block '<S2>/Constant47' : Unused code path elimination
//  Block '<S2>/GreaterThan1' : Unused code path elimination
//  Block '<S2>/GreaterThan10' : Unused code path elimination
//  Block '<S2>/GreaterThan2' : Unused code path elimination
//  Block '<S2>/GreaterThan3' : Unused code path elimination
//  Block '<S2>/GreaterThan4' : Unused code path elimination
//  Block '<S2>/GreaterThan5' : Unused code path elimination
//  Block '<S2>/GreaterThan6' : Unused code path elimination
//  Block '<S2>/GreaterThan7' : Unused code path elimination
//  Block '<S2>/GreaterThan8' : Unused code path elimination
//  Block '<S2>/GreaterThan9' : Unused code path elimination
//  Block '<S2>/Logical Operator34' : Unused code path elimination
//  Block '<S2>/Logical Operator40' : Unused code path elimination
//  Block '<S2>/Multiply' : Unused code path elimination
//  Block '<S2>/Data Type Conversion' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion1' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion2' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion3' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion4' : Eliminate redundant data type conversion
//  Block '<S1>/Constant2' : Unused code path elimination
//  Block '<S1>/Constant4' : Unused code path elimination
//  Block '<S2>/SwaySum' : Unused code path elimination


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
//  '<Root>' : 'TrailerOscillationEstimation_2018b'
//  '<S1>'   : 'TrailerOscillationEstimation_2018b/TrailerOscillationDetectEstim'
//  '<S2>'   : 'TrailerOscillationEstimation_2018b/TrailerOscillationDetectEstim/Curernt_Model_LC_implementation'
//  '<S3>'   : 'TrailerOscillationEstimation_2018b/TrailerOscillationDetectEstim/Subsystem'
//  '<S4>'   : 'TrailerOscillationEstimation_2018b/TrailerOscillationDetectEstim/Curernt_Model_LC_implementation/OscilationDetection1'
//  '<S5>'   : 'TrailerOscillationEstimation_2018b/TrailerOscillationDetectEstim/Subsystem/OscilationDetection'
//  '<S6>'   : 'TrailerOscillationEstimation_2018b/TrailerOscillationDetectEstim/Subsystem/periodic_pulse'

#endif                                 // RTW_HEADER_TrailerOscillationEstimation_2018b_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
