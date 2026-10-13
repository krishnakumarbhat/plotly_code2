//
// File: BMW_VSE_Master_Model_types.h
//
// Code generated for Simulink model 'BMW_VSE_Master_Model'.
//
// Model version                  : 1.495
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:40 2023
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
#ifndef RTW_HEADER_BMW_VSE_Master_Model_types_h_
   #define RTW_HEADER_BMW_VSE_Master_Model_types_h_
   #include "rtwtypes.h"
   #ifndef DEFINED_TYPEDEF_FOR_enum_quality_factor_T_
      #define DEFINED_TYPEDEF_FOR_enum_quality_factor_T_

typedef uint8_T enum_quality_factor_T;

      // enum enum_quality_factor_T
      #define BMW_VSE_QF_UNDEFINED      ((enum_quality_factor_T)0U) // Default value
      #define BMW_VSE_QF_TEMP_UNDEFINED ((enum_quality_factor_T)1U)
      #define BMW_VSE_QF_NOT_ACCURATED  ((enum_quality_factor_T)2U)
      #define BMW_VSE_QF_ACCURATED      ((enum_quality_factor_T)3U)
   #endif

   #ifndef DEFINED_TYPEDEF_FOR_VCAN_VSE_
      #define DEFINED_TYPEDEF_FOR_VCAN_VSE_

typedef struct
{
   real32_T raw_speed_mps;
   enum_quality_factor_T raw_speed_qf;
   real32_T raw_yaw_rate_rps;
   enum_quality_factor_T raw_yaw_rate_qf;
   boolean_T f_stationary;
   boolean_T f_reverse;
   real32_T raw_steering_angle_deg;
   enum_quality_factor_T raw_steering_angle_qf;
   real32_T raw_lat_accel;
   enum_quality_factor_T raw_lat_accel_qf;
   real32_T raw_long_accel;
   enum_quality_factor_T raw_long_accel_qf;
} VCAN_VSE;

   #endif

   #ifndef DEFINED_TYPEDEF_FOR_Tracker_VSE_
      #define DEFINED_TYPEDEF_FOR_Tracker_VSE_

typedef struct
{
   real32_T VehSpeedCompFac_KA;
   enum_quality_factor_T VehSpeedCompFac_KA_QF;
} Tracker_VSE;

   #endif

   #ifndef DEFINED_TYPEDEF_FOR_Vehicle_Parameters_
      #define DEFINED_TYPEDEF_FOR_Vehicle_Parameters_

typedef struct
{
   real32_T k_wheel_base;
   real32_T k_dist_rear_axle_to_vcs;
   real32_T k_long_dist_radar_to_rear_axle;
   real32_T k_lat_dist_radar_to_rear_axle;
   real32_T k_steering_gear_ratio;
   real32_T k_understeer_coefficient;
   real32_T k_rear_cornering_compliance;
   real32_T k_vehicle_width;
   real32_T k_cog_x;
   real32_T k_cog_y;
} Vehicle_Parameters;

   #endif

   #ifndef DEFINED_TYPEDEF_FOR_VSE_OUT_
      #define DEFINED_TYPEDEF_FOR_VSE_OUT_

typedef struct
{
   uint64_T timestamp_us;
   uint32_T veh_index;
   real32_T raw_speed_mps;
   enum_quality_factor_T raw_speed_qf;
   real32_T speed_compensation_factor;
   enum_quality_factor_T speed_compensation_factor_qf;
   real32_T filt_veh_speed_over_ground;
   enum_quality_factor_T filt_veh_speed_over_ground_qf;
   real32_T signed_filt_veh_speed_over_ground;
   real32_T raw_lat_accel;
   enum_quality_factor_T raw_lat_accel_qf;
   real32_T raw_long_accel;
   enum_quality_factor_T raw_long_accel_qf;
   real32_T raw_yaw_rate_rps;
   enum_quality_factor_T raw_yaw_rate_qf;
   real32_T raw_steering_angle_deg;
   enum_quality_factor_T raw_steering_angle_qf;
   real32_T road_wheel_angle_deg;
   enum_quality_factor_T road_wheel_angle_qf;
   real32_T yaw_rate_sa;
   enum_quality_factor_T yaw_rate_sa_qf;
   real32_T yaw_rate_raw_bias;
   enum_quality_factor_T yaw_rate_bias_qf;
   real32_T comp_yaw_rate_unfiltered;
   real32_T comp_yaw_rate_filtered;
   enum_quality_factor_T comp_yaw_rate_qf;
   real32_T curvature_rear_axle;
   real32_T sideslip_rear_axle;
   real32_T vcs_sideslip;
   real32_T vcs_long_velocity;
   real32_T vcs_lat_velocity;
   real32_T sensor_sideslip;
   real32_T sensor_long_velocity;
   real32_T sensor_lat_velocity;
   uint8_T stationary;
   real32_T k_dist_rear_axle_to_vcs;
   real32_T vcs_lat_accel;
   real32_T vcs_long_accel;
   enum_quality_factor_T vcs_lat_accel_qf;
   enum_quality_factor_T vcs_long_accel_qf;
   real32_T accel_rear_axle;
} VSE_OUT;

   #endif

   #ifndef DEFINED_TYPEDEF_FOR_enum_road_type_T_
      #define DEFINED_TYPEDEF_FOR_enum_road_type_T_

typedef int32_T enum_road_type_T;

      // enum enum_road_type_T
      #define UNKNOWN_ROAD  (0) // Default value
      #define STRAIGHT_ROAD (1)
      #define CURVED_ROAD   (2)
   #endif

   #ifndef DEFINED_TYPEDEF_FOR_VSE_RESIM_
      #define DEFINED_TYPEDEF_FOR_VSE_RESIM_

typedef struct
{
   boolean_T f_yaw_rate_bias_converged;
   boolean_T f_stop_bias_converged;
   real32_T yaw_rate_bias1;
   real32_T yaw_rate_bias2;
   real32_T yaw_rate_bias_fast_bias1;
   real32_T yaw_rate_bias_fast_bias2;
   real32_T comp_yaw_rate_diff_filt;
   real32_T yaw_rate_bias_diff;
   enum_road_type_T road_type;
   boolean_T f_yaw_rate_bias_shift;
   boolean_T f_yaw_rate_steady;
   boolean_T f_input_invalid_persistent;
   boolean_T f_execution_period_error_persistent;
   boolean_T f_bias_was_accurate;
   real32_T ignition_time;
   real32_T CurvKalmanFilterC0;
   real32_T CurvKalmanFilterC1;
} VSE_RESIM;

   #endif

   #ifndef DEFINED_TYPEDEF_FOR_enum_yaw_rate_bias_accuracy_T_
      #define DEFINED_TYPEDEF_FOR_enum_yaw_rate_bias_accuracy_T_

typedef int32_T enum_yaw_rate_bias_accuracy_T;

      // enum enum_yaw_rate_bias_accuracy_T
      #define BIAS_UNDEFINED    (0) // Default value
      #define BIAS_NOT_ACCURATE (1)
      #define BIAS_ACCURATE     (2)
   #endif

   #ifndef DEFINED_TYPEDEF_FOR_RoadWheel_vs_SteeringWheel_Table_
      #define DEFINED_TYPEDEF_FOR_RoadWheel_vs_SteeringWheel_Table_

typedef struct
{
   real32_T SteeringWheelAngle_Deg[31];
   real32_T RoadWheelAngle_Deg[31];
} RoadWheel_vs_SteeringWheel_Table;

   #endif
#endif // RTW_HEADER_BMW_VSE_Master_Model_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
