//
// File: SpeedCompensationArbitration.h
//
// Code generated for Simulink model 'SpeedCompensationArbitration'.
//
// Model version                  : 1.77
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:57:40 2024
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
#ifndef RTW_HEADER_SpeedCompensationArbitration_h_
#define RTW_HEADER_SpeedCompensationArbitration_h_
#include <string.h>
#include <stddef.h>
#ifndef SpeedCompensationArbitration_COMMON_INCLUDES_
# define SpeedCompensationArbitration_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // SpeedCompensationArbitration_COMMON_INCLUDES_ 

#include "SpeedCompensationArbitration_types.h"
#include <stddef.h>

// Class declaration for model SpeedCompensationArbitration
class SpeedCompensationArbitrationModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_VsTracker_VehSpdCompFac_1, const
            enum_quality_factor_T *rtu_VeTracker_VehSpdCompFac_1_QF, const
            real32_T *rtu_VsTracker_VehSpdCompFac_2, const enum_quality_factor_T
            *rtu_VeTracker_VehSpdCompFac_2_QF, const real32_T
            *rtu_VsTracker_VehSpdCompFac_3, const enum_quality_factor_T
            *rtu_VeTracker_VehSpdCompFac_3_QF, const real32_T
            *rtu_VsTracker_VehSpdCompFac_4, const enum_quality_factor_T
            *rtu_VeTracker_VehSpdCompFac_4_QF, const real32_T
            *rtu_VsTracker_VehSpdCompFac_5, const enum_quality_factor_T
            *rtu_VeTracker_VehSpdCompFac_5_QF, real32_T
            *rty_VsTracker_VehSpdCompFac, enum_quality_factor_T
            *rty_VeTracker_VehSpdCompFac_QF);

  // Constructor
  SpeedCompensationArbitrationModelClass();

  // Destructor
  ~SpeedCompensationArbitrationModelClass();

  // private data and function members
 private:
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
//  '<Root>' : 'SpeedCompensationArbitration'
//  '<S1>'   : 'SpeedCompensationArbitration/SpeedCompensationArbitration'

#endif                                 // RTW_HEADER_SpeedCompensationArbitration_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
