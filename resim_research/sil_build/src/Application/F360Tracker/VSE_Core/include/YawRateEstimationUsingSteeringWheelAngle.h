//
// File: YawRateEstimationUsingSteeringWheelAngle.h
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
#ifndef RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_h_
#define RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef YawRateEstimationUsingSteeringWheelAngle_COMMON_INCLUDES_
# define YawRateEstimationUsingSteeringWheelAngle_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // YawRateEstimationUsingSteeringWheelAngle_COMMON_INCLUDES_ 

#include "YawRateEstimationUsingSteeringWheelAngle_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'YawRateEstimationUsingSteeringWheelAngle' 
typedef struct {
  yaw_est_cals_T yaw_est_cals;         // '<S5>/call_initializeYawEstStruct'
  yaw_est_cals_T UnitDelay2_DSTATE;    // '<S1>/Unit Delay2'
  host_veh_T host_veh;                 // '<S5>/call_initializeYawEstStruct'
  host_veh_T UnitDelay3_DSTATE;        // '<S1>/Unit Delay3'
  yawrate_est_str_T yawrate_est_str;   // '<S5>/call_initializeYawEstStruct'
  yawrate_est_str_T UnitDelay_DSTATE;  // '<S1>/Unit Delay'
  struct {
    uint_T is_c6_YawRateEstimationUsingSteeringWheelAngle:2;// '<S1>/Chart'
    uint_T is_active_c6_YawRateEstimationUsingSteeringWheelAngle:1;// '<S1>/Chart' 
    uint_T f_initialize_yaw_rate_estimation:1;// '<S1>/Chart'
  } bitsForTID0;
} DW_YawRateEstimationUsingSteeringWheelAngle_T;

// Class declaration for model YawRateEstimationUsingSteeringWheelAngle
class YawRateEstimationUsingSteeringWheelAngleModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_filt_veh_speed_over_ground, const boolean_T
            *rtu_f_reverse, const real32_T *rtu_comp_steering_angle_deg, const
            enum_quality_factor_T *rtu_comp_steering_angle_qf, const real32_T
            *rtu_k_wheel_base, const real32_T *rtu_steering_ratio_on_center,
            const real32_T *rtu_k_understeer_coefficient, real32_T
            *rty_yaw_rate_sa, enum_quality_factor_T *rty_yaw_rate_sa_qf,
            real32_T *rty_road_wheel_angle_deg, enum_quality_factor_T
            *rty_road_wheel_angle_qf);

  // Constructor
  YawRateEstimationUsingSteeringWheelAngleModelClass();

  // Destructor
  ~YawRateEstimationUsingSteeringWheelAngleModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_YawRateEstimationUsingSteeringWheelAngle_T
    YawRateEstimationUsingSteeringWheelAngle_DW;
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
//  '<Root>' : 'YawRateEstimationUsingSteeringWheelAngle'
//  '<S1>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle'
//  '<S2>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/Chart'
//  '<S3>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/Degrees to Radians'
//  '<S4>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/Find_Real_Steering_Ratio'
//  '<S5>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/Initialization'
//  '<S6>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/MATLAB Function'
//  '<S7>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/Find_Real_Steering_Ratio/MATLAB Function'
//  '<S8>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/Initialization/call_defYawEstStructures'
//  '<S9>'   : 'YawRateEstimationUsingSteeringWheelAngle/YawRateEstimationUsingSteeringWheelAngle/Initialization/call_initializeYawEstStruct'

#endif                                 // RTW_HEADER_YawRateEstimationUsingSteeringWheelAngle_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
