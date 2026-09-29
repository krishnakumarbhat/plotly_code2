//
// File: VehicleCurvatureAndSideslipEstimation_Simulink.h
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
#ifndef RTW_HEADER_VehicleCurvatureAndSideslipEstimation_Simulink_h_
#define RTW_HEADER_VehicleCurvatureAndSideslipEstimation_Simulink_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef VehicleCurvatureAndSideslipEstimation_Simulink_COMMON_INCLUDES_
# define VehicleCurvatureAndSideslipEstimation_Simulink_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // VehicleCurvatureAndSideslipEstimation_Simulink_COMMON_INCLUDES_ 

#include "VehicleCurvatureAndSideslipEstimation_Simulink_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'VehicleCurvatureAndSideslipEstimation_Simulink' 
typedef struct {
  CURVATURE_AND_SIDESLIP_STATE_T veh_state;// '<S6>/vehicle state struct init'
  CURVATURE_AND_SIDESLIP_STATE_T Memory_PreviousInput;// '<S3>/Memory'
  CURVATURE_CALS_T curvature_calibrations;// '<S6>/vehicle state struct init'
  VEH_CALS_T veh_calibrations;         // '<S6>/vehicle state struct init'
  real32_T lat_acc[6];                 // '<S6>/Variable sidelsip values generation' 
  real32_T rear_slip_ang[6];           // '<S6>/Variable sidelsip values generation' 
  real32_T Switch;                     // '<S15>/Switch'
  uint32_T time_step_number;           // '<S3>/MATLAB Function3'
  struct {
    uint_T is_c9_VehicleCurvatureAndSideslipEstimation_Simulink:2;// '<S3>/trigger for init' 
    uint_T initialization:1;           // '<S3>/trigger for init'
    uint_T time_step_number_not_empty:1;// '<S3>/MATLAB Function3'
  } bitsForTID0;
} DW_VehicleCurvatureAndSideslipEstimation_Simulink_T;

// Invariant block signals for model 'VehicleCurvatureAndSideslipEstimation_Simulink' 
typedef const struct
  tag_ConstB_VehicleCurvatureAndSideslipEstimation_Simulink_c_T {
  real32_T lat_acc[6];                 // '<S4>/Gain3'
  real32_T rr_corner_comp[6];          // '<S4>/Gain5'
} ConstB_VehicleCurvatureAndSideslipEstimation_Simulink_h_T;

// Class declaration for model VehicleCurvatureAndSideslipEstimation_Simulink
class VehicleCurvatureAndSideslipEstimation_SimulinkModelClass {
  // public data and function members
 public:
  // model step function
  void step(const uint64_T *rtu_system_timer_get_64bit_current_value, const
            real32_T *rtu_veh_speed_compensated, const real32_T
            *rtu_comp_yaw_rate_filtered, const real32_T
            *rtu_k_dist_front_to_rear_axle, const real32_T
            *rtu_k_vehicleCfg_width, const boolean_T
            *rtu_var_rr_corner_comp_true, const real32_T
            *rtu_k_rear_cornering_compliance, const boolean_T *rtu_f_reverse,
            real32_T *rty_curvature_rear_axle, real32_T *rty_VCS_long_velocity,
            real32_T *rty_sensor_long_velocity, real32_T *rty_VCS_sideslip,
            real32_T *rty_sensor_sideslip, real32_T *rty_sideslip_rear_axle,
            real32_T *rty_VCS_lat_velocity, real32_T *rty_sensor_lat_velocity,
            real32_T *rty_CurvKalmanFilterC0, real32_T *rty_CurvKalmanFilterC1);

  // Initial conditions function
  void init();

  // Constructor
  VehicleCurvatureAndSideslipEstimation_SimulinkModelClass();

  // Destructor
  ~VehicleCurvatureAndSideslipEstimation_SimulinkModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_VehicleCurvatureAndSideslipEstimation_Simulink_T
    VehicleCurvatureAndSideslipEstimation_Simulink_DW;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S4>/Switch' : Eliminated due to constant selection input
//  Block '<S4>/Bus Creator1' : Unused code path elimination
//  Block '<S4>/Constant' : Unused code path elimination
//  Block '<S4>/Gain' : Unused code path elimination
//  Block '<S4>/Gain2' : Unused code path elimination
//  Block '<S4>/Gain4' : Unused code path elimination
//  Block '<S4>/lat_acc_2pass' : Unused code path elimination
//  Block '<S4>/rr_corner_comp_2pass' : Unused code path elimination


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
//  '<Root>' : 'VehicleCurvatureAndSideslipEstimation_Simulink'
//  '<S1>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1'
//  '<S2>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Elapsed_Time'
//  '<S3>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function'
//  '<S4>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Rear cornering compliance table from FCA'
//  '<S5>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/ComputeSideslip'
//  '<S6>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/Init'
//  '<S7>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/MATLAB Function1'
//  '<S8>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/MATLAB Function3'
//  '<S9>'   : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/RearAxleCurvature'
//  '<S10>'  : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/Sideslip_Rearaxle'
//  '<S11>'  : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/trigger for init'
//  '<S12>'  : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/Init/Variable sidelsip values generation'
//  '<S13>'  : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/Init/vehicle state struct init'
//  '<S14>'  : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/Sideslip_Rearaxle/MATLAB Function4'
//  '<S15>'  : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/Sideslip_Rearaxle/SideSlip Table'
//  '<S16>'  : 'VehicleCurvatureAndSideslipEstimation_Simulink/Subsystem1/Main function/Sideslip_Rearaxle/SideSlip Table/Compare To Zero'

#endif                                 // RTW_HEADER_VehicleCurvatureAndSideslipEstimation_Simulink_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
