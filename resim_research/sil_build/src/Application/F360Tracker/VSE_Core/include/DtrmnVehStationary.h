//
// File: DtrmnVehStationary.h
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
#ifndef RTW_HEADER_DtrmnVehStationary_h_
#define RTW_HEADER_DtrmnVehStationary_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef DtrmnVehStationary_COMMON_INCLUDES_
# define DtrmnVehStationary_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // DtrmnVehStationary_COMMON_INCLUDES_

#include "DtrmnVehStationary_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'DtrmnVehStationary'
typedef struct {
  real32_T diff_num[4];                // '<S3>/FindDifferentiatorCoeff'
  real32_T diff_denom[4];              // '<S3>/FindDifferentiatorCoeff'
  real32_T Differentiator_states[9];   // '<S4>/Differentiator'
  real32_T Differentiator_tmp[3];      // '<S4>/Differentiator'
  uint32_T count;                      // '<S16>/Delay_set'
  struct {
    uint_T is_c1_DtrmnVehStationary:2; // '<S1>/Chart'
    uint_T is_active_c1_DtrmnVehStationary:1;// '<S1>/Chart'
    uint_T f_initialize_DtrmnVehStationary:1;// '<S1>/Chart'
  } bitsForTID0;
} DW_DtrmnVehStationary_T;

// Class declaration for model DtrmnVehStationary
class DtrmnVehStationaryModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_raw_yaw_rate_rps, const enum_quality_factor_T
            *rtu_raw_yaw_rate_qf, const real32_T *rtu_raw_lat_accel, const
            enum_quality_factor_T *rtu_raw_lat_accel_qf, const real32_T
            *rtu_raw_long_accel, const enum_quality_factor_T
            *rtu_raw_long_accel_qf, const real32_T
            *rtu_wheel_lin_speed_mps_front_left, const real32_T
            *rtu_wheel_lin_speed_mps_front_right, const real32_T
            *rtu_wheel_lin_speed_mps_rear_left, const real32_T
            *rtu_wheel_lin_speed_mps_rear_right, const boolean_T
            *rtu_external_stationary_flag, boolean_T *rty_f_stationary);

  // Constructor
  DtrmnVehStationaryModelClass();

  // Destructor
  ~DtrmnVehStationaryModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_DtrmnVehStationary_T DtrmnVehStationary_DW;
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
//  '<Root>' : 'DtrmnVehStationary'
//  '<S1>'   : 'DtrmnVehStationary/DtrmnVehStationary'
//  '<S2>'   : 'DtrmnVehStationary/DtrmnVehStationary/Chart'
//  '<S3>'   : 'DtrmnVehStationary/DtrmnVehStationary/Differentiator Generator'
//  '<S4>'   : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary'
//  '<S5>'   : 'DtrmnVehStationary/DtrmnVehStationary/Differentiator Generator/FindDifferentiatorCoeff'
//  '<S6>'   : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant1'
//  '<S7>'   : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant10'
//  '<S8>'   : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant2'
//  '<S9>'   : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant3'
//  '<S10>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant4'
//  '<S11>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant5'
//  '<S12>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant6'
//  '<S13>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant7'
//  '<S14>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant8'
//  '<S15>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Compare To Constant9'
//  '<S16>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Delay_set1'
//  '<S17>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/DocBlock'
//  '<S18>'  : 'DtrmnVehStationary/DtrmnVehStationary/DtrmnVehStationary/Delay_set1/Delay_set'

#endif                                 // RTW_HEADER_DtrmnVehStationary_h_

//
// File trailer for generated code.
//
// [EOF]
//
