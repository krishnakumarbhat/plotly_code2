//
// File: YawRateEstimationUsingSteeringWheelAngle_private.h
//
// Code generated for Simulink model 'YawRateEstimationUsingSteeringWheelAngle'.
//
// Model version                  : 1.141
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:40 2024
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
#ifndef RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_private_h_
#define RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_private_h_
#include "rtwtypes.h"

// Constant parameters (default storage)
typedef struct {
  // Expression: RoadWheel_vs_SteeringWheel_Table.Table
  //  Referenced by: '<S4>/RoadWheelAngle_vs_SteeringWheelAngle'

  real32_T RoadWheelAngle_vs_SteeringWheelAngle_tableData[31];

  // Expression: RoadWheel_vs_SteeringWheel_Table.Breakpoints(1)
  //  Referenced by: '<S4>/RoadWheelAngle_vs_SteeringWheelAngle'

  real32_T RoadWheelAngle_vs_SteeringWheelAngle_bp01Data[31];
} ConstP_YawRateEstimationUsingSteeringWheelAngle_T;

// Constant parameters (default storage)
extern const ConstP_YawRateEstimationUsingSteeringWheelAngle_T
  YawRateEstimationUsingSteeringWheelAngle_ConstP;

#endif                                 // RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_private_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
