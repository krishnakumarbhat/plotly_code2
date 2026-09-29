//
// File: LateralJerkEstimation.h
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
#ifndef RTW_HEADER_LateralJerkEstimation_h_
#define RTW_HEADER_LateralJerkEstimation_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef LateralJerkEstimation_COMMON_INCLUDES_
# define LateralJerkEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // LateralJerkEstimation_COMMON_INCLUDES_ 

#include "LateralJerkEstimation_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'LateralJerkEstimation'
typedef struct {
  real_T filter_discrete_num[5];       // '<S4>/FindFilterCoeff'
  real_T filter_discrete_denom[5];     // '<S4>/FindFilterCoeff'
  real_T diff_num[9];                  // '<S3>/FindDifferentiatorCoeff'
  real_T diff_denom[9];                // '<S3>/FindDifferentiatorCoeff'
  real_T LowPassFilter_states[4];      // '<S5>/LowPass Filter'
  real_T Differentiator_states[8];     // '<S6>/Differentiator'
  real_T LowPassFilter_tmp;            // '<S5>/LowPass Filter'
  real_T Differentiator_tmp;           // '<S6>/Differentiator'
  struct {
    uint_T is_c4_LateralJerkEstimation:2;// '<S1>/Chart'
    uint_T is_active_c4_LateralJerkEstimation:1;// '<S1>/Chart'
    uint_T f_initialize_LateralJerkEstimation:1;// '<S1>/Chart'
  } bitsForTID0;
} DW_LateralJerkEstimation_T;

// Class declaration for model LateralJerkEstimation
class LateralJerkEstimationModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_Lateral_Acceleration, const
            enum_quality_factor_T *rtu_Raw_Lateral_Acceleration_qf, real32_T
            *rty_Lateral_Jerk, real32_T *rty_Filt_Lateral_Acceleration,
            enum_quality_factor_T *rty_Lateral_Jerk_qf, enum_quality_factor_T
            *rty_Filt_Lateral_Acceleration_qf);

  // Constructor
  LateralJerkEstimationModelClass();

  // Destructor
  ~LateralJerkEstimationModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_LateralJerkEstimation_T LateralJerkEstimation_DW;
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
//  '<Root>' : 'LateralJerkEstimation'
//  '<S1>'   : 'LateralJerkEstimation/Lateral Jerk Estimator'
//  '<S2>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Chart'
//  '<S3>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Differentiator Generator'
//  '<S4>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Filter Generator'
//  '<S5>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Lateral Acceleration Filter'
//  '<S6>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Lateral Jerk Estimator'
//  '<S7>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Differentiator Generator/FindDifferentiatorCoeff'
//  '<S8>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Filter Generator/FindFilterCoeff'
//  '<S9>'   : 'LateralJerkEstimation/Lateral Jerk Estimator/Lateral Acceleration Filter/Compare To Constant'
//  '<S10>'  : 'LateralJerkEstimation/Lateral Jerk Estimator/Lateral Jerk Estimator/Compare To Constant'

#endif                                 // RTW_HEADER_LateralJerkEstimation_h_

//
// File trailer for generated code.
//
// [EOF]
//
