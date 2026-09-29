//
// File: YawRateEstimationUsingSteeringWheelAngle_types.h
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
#ifndef RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_types_h_
#define RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_enum_quality_factor_T_
#define DEFINED_TYPEDEF_FOR_enum_quality_factor_T_

typedef uint8_T enum_quality_factor_T;

// enum enum_quality_factor_T
#define UNDEFINED                      ((enum_quality_factor_T)0U) // Default value 
#define TEMP_UNDEFINED                 ((enum_quality_factor_T)1U)
#define NOT_ACCURATED                  ((enum_quality_factor_T)2U)
#define ACCURATED                      ((enum_quality_factor_T)3U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_host_veh_T_
#define DEFINED_TYPEDEF_FOR_host_veh_T_

typedef struct {
  real32_T vehicle_base;
  real32_T steering_gear_ratio;
} host_veh_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_RoadWheel_vs_SteeringWheel_Table_
#define DEFINED_TYPEDEF_FOR_RoadWheel_vs_SteeringWheel_Table_

typedef struct {
  real32_T SteeringWheelAngle_Deg[31];
  real32_T RoadWheelAngle_Deg[31];
} RoadWheel_vs_SteeringWheel_Table;

#endif

#ifndef DEFINED_TYPEDEF_FOR_yawrate_est_str_T_
#define DEFINED_TYPEDEF_FOR_yawrate_est_str_T_

typedef struct {
  real32_T yawRate;
  enum_quality_factor_T qf;
} yawrate_est_str_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_yaw_est_cals_T_
#define DEFINED_TYPEDEF_FOR_yaw_est_cals_T_

typedef struct {
  real32_T k_understeer_coefficient;
  real32_T k_execution_time;
  real32_T k_SA_time_constant;
  real32_T k_SA_min_speed;
  real32_T k_SA_max_yaw;
  int32_T k_yawEst_version_main;
  int32_T k_yawEst_version_sub;
} yaw_est_cals_T;

#endif
#endif                                 // RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_types_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
