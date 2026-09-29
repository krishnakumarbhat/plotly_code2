//
// File: VehicleCurvatureAndSideslipEstimation_Simulink_types.h
//
// Code generated for Simulink model 'VehicleCurvatureAndSideslipEstimation_Simulink'.
//
// Model version                  : 1.484
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:58:38 2024
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
#ifndef RTW_HEADER_VehicleCurvatureAndSideslipEstimation_Simulink_types_h_
#define RTW_HEADER_VehicleCurvatureAndSideslipEstimation_Simulink_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_CURVATURE_AND_SIDESLIP_INPUT_T_
#define DEFINED_TYPEDEF_FOR_CURVATURE_AND_SIDESLIP_INPUT_T_

typedef struct {
  uint64_T time;
  real32_T host_speed_compensated;
  real32_T host_yawrate;
} CURVATURE_AND_SIDESLIP_INPUT_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_RR_CORNER_COMP_TABLE_
#define DEFINED_TYPEDEF_FOR_RR_CORNER_COMP_TABLE_

typedef struct {
  real32_T lat_acc[6];
  real32_T rr_corner_comp[6];
} RR_CORNER_COMP_TABLE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CURVATURE_AND_SIDESLIP_OUTPUT_T_
#define DEFINED_TYPEDEF_FOR_CURVATURE_AND_SIDESLIP_OUTPUT_T_

typedef struct {
  real32_T curvature_rear_axle;
  real32_T sideslip_rear_axle;
  real32_T VCS_sideslip;
  real32_T sensor_sideslip;
  real32_T VCS_long_velocity;
  real32_T VCS_lat_velocity;
  real32_T sensor_long_velocity;
  real32_T sensor_lat_velocity;
} CURVATURE_AND_SIDESLIP_OUTPUT_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CURVATURE_AND_SIDESLIP_STATE_T_
#define DEFINED_TYPEDEF_FOR_CURVATURE_AND_SIDESLIP_STATE_T_

typedef struct {
  real32_T DistBasedCurv;
  real32_T CurvDist;
  real32_T CurvIntegYaw;
  real32_T CurvKalmanFilterC0;
  real32_T CurvKalmanFilterC1;
  real32_T curvature_rear_axle;
  real32_T sideslip_rear_axle;
  real32_T VCS_sideslip;
  real32_T sensor_sideslip;
  uint64_T prev_time;
  real32_T VCS_long_velocity;
  real32_T VCS_lat_velocity;
  real32_T sensor_long_velocity;
  real32_T sensor_lat_velocity;
  boolean_T f_CurvKalmanFilterInitialized;
} CURVATURE_AND_SIDESLIP_STATE_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_VEH_CALS_T_
#define DEFINED_TYPEDEF_FOR_VEH_CALS_T_

typedef struct {
  real32_T dist_rear_axle_to_vcs;
  real32_T dist_radar_to_rear_axle;
  real32_T vcs_radar_lat;
} VEH_CALS_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CURVATURE_CALS_T_
#define DEFINED_TYPEDEF_FOR_CURVATURE_CALS_T_

typedef struct {
  // don't integrate yaw rate below this speed [m/sec]
  real32_T dist_based_curv_speed_thresh;

  // raw curv. measurements are calculated in discrete - distance, not discrete - time.This is distance between calculation instants 
  real32_T dist_based_curv_discrete_distance_interval;

  // Steady-State Kalman gains for discrete-distance curvature filter based on distance interval 
  real32_T curv_kalman_gain_1;

  // Steady-State Kalman gains for discrete-distance curvature filter based on distance interval 
  real32_T curv_kalman_gain_2;
  real32_T delta_T;
} CURVATURE_CALS_T;

#endif
#endif                                 // RTW_HEADER_VehicleCurvatureAndSideslipEstimation_Simulink_types_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
