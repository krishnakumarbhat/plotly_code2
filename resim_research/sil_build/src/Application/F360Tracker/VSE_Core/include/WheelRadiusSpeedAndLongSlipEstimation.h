//
// File: WheelRadiusSpeedAndLongSlipEstimation.h
//
// Code generated for Simulink model 'WheelRadiusSpeedAndLongSlipEstimation'.
//
// Model version                  : 1.480
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:57 2024
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
#ifndef RTW_HEADER_WheelRadiusSpeedAndLongSlipEstimation_h_
#define RTW_HEADER_WheelRadiusSpeedAndLongSlipEstimation_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef WheelRadiusSpeedAndLongSlipEstimation_COMMON_INCLUDES_
# define WheelRadiusSpeedAndLongSlipEstimation_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // WheelRadiusSpeedAndLongSlipEstimation_COMMON_INCLUDES_ 

#include "WheelRadiusSpeedAndLongSlipEstimation_types.h"

// Child system includes
#include "FindDistFrom_GPSLatLongDiff.h"
#include <stddef.h>

// Block signals and states (default storage) for system '<S10>/AccumulateAngleFromFrontLeftWheelSpeed' 
typedef struct {
  real32_T accum_angle;                // '<S10>/AccumulateAngleFromFrontLeftWheelSpeed' 
} DW_AccumulateAngleFromFrontLeftWheelSpeed_WheelRadiusSpeedAndLongSlipEstimation_T;

// Block signals and states (default storage) for system '<S42>/FindTireRadius'
typedef struct {
  real32_T curr_time;                  // '<S42>/FindTireRadius'
  real32_T raw_rad_conv_time_start;    // '<S42>/FindTireRadius'
  real32_T est_rad_conv_time_start;    // '<S42>/FindTireRadius'
  real32_T running_avg_raw_tire_radius;// '<S42>/FindTireRadius'
  real32_T running_avg_estimated_tire_radius;// '<S42>/FindTireRadius'
  real32_T last_raw_tire_radius;       // '<S42>/FindTireRadius'
  real32_T last_estimated_tire_radius; // '<S42>/FindTireRadius'
  real32_T raw_rad_conv_persistent;    // '<S42>/FindTireRadius'
  real32_T est_rad_conv_persistent;    // '<S42>/FindTireRadius'
  struct {
    uint_T curr_time_not_empty:1;      // '<S42>/FindTireRadius'
    uint_T f_raw_rad_converged_temp:1; // '<S42>/FindTireRadius'
    uint_T f_est_rad_converged_temp:1; // '<S42>/FindTireRadius'
    uint_T f_raw_rad_converged:1;      // '<S42>/FindTireRadius'
    uint_T f_est_rad_converged:1;      // '<S42>/FindTireRadius'
    uint_T f_raw_rad_conv_time_start:1;// '<S42>/FindTireRadius'
    uint_T f_est_rad_conv_time_start:1;// '<S42>/FindTireRadius'
  } bitsForTID0;
} DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_T;

// Block signals and states (default storage) for system '<S55>/FindTireRadius'
typedef struct {
  real32_T curr_time;                  // '<S55>/FindTireRadius'
  real32_T radar_est_rad_conv_time_start;// '<S55>/FindTireRadius'
  real32_T running_avg_radar_estimated_tire_radius;// '<S55>/FindTireRadius'
  real32_T last_radar_estimated_tire_radius;// '<S55>/FindTireRadius'
  real32_T radar_est_rad_conv_persistent;// '<S55>/FindTireRadius'
  struct {
    uint_T curr_time_not_empty:1;      // '<S55>/FindTireRadius'
    uint_T f_radar_est_rad_converged_temp:1;// '<S55>/FindTireRadius'
    uint_T f_radar_est_rad_converged:1;// '<S55>/FindTireRadius'
    uint_T f_radar_est_rad_conv_time_start:1;// '<S55>/FindTireRadius'
  } bitsForTID0;
} DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_e_T;

// Block signals and states (default storage) for model 'WheelRadiusSpeedAndLongSlipEstimation' 
typedef struct {
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_e_T
    sf_FindTireRadius_f;               // '<S57>/FindTireRadius'
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_e_T
    sf_FindTireRadius_o;               // '<S56>/FindTireRadius'
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_e_T
    sf_FindTireRadius_i;               // '<S55>/FindTireRadius'
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_T sf_FindTireRadius_k;// '<S45>/FindTireRadius' 
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_T sf_FindTireRadius_m;// '<S44>/FindTireRadius' 
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_T sf_FindTireRadius_e;// '<S43>/FindTireRadius' 
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_T sf_FindTireRadius;// '<S42>/FindTireRadius' 
  DW_AccumulateAngleFromFrontLeftWheelSpeed_WheelRadiusSpeedAndLongSlipEstimation_T
    sf_AccumulateAngleFromRearRightWheelSpeed;// '<S13>/AccumulateAngleFromRearRightWheelSpeed' 
  DW_AccumulateAngleFromFrontLeftWheelSpeed_WheelRadiusSpeedAndLongSlipEstimation_T
    sf_AccumulateAngleFromRearLeftWheelSpeed;// '<S12>/AccumulateAngleFromRearLeftWheelSpeed' 
  DW_AccumulateAngleFromFrontLeftWheelSpeed_WheelRadiusSpeedAndLongSlipEstimation_T
    sf_AccumulateAngleFromFrontRightWheelSpeed;// '<S11>/AccumulateAngleFromFrontRightWheelSpeed' 
  DW_AccumulateAngleFromFrontLeftWheelSpeed_WheelRadiusSpeedAndLongSlipEstimation_T
    sf_AccumulateAngleFromFrontLeftWheelSpeed;// '<S10>/AccumulateAngleFromFrontLeftWheelSpeed' 
  dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T gobj_0;// '<S6>/Lowpass Filter' 
  dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T gobj_1;// '<S6>/Lowpass Filter' 
  dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T obj;// '<S6>/Lowpass Filter' 
  real_T GPS_long_locked;              // '<S7>/LockStartingGPSPosition'
  real_T GPS_lat_locked;               // '<S7>/LockStartingGPSPosition'
  real32_T num_discrete[4];            // '<S9>/Find_Speed_To_Accel_TF'
  real32_T den_discrete[4];            // '<S9>/Find_Speed_To_Accel_TF'
  real32_T DiscreteFilter_states[3];   // '<S1>/Discrete Filter'
  real32_T DiscreteTimeIntegrator;     // '<S3>/Discrete-Time Integrator'
  real32_T DiscreteTimeIntegrator_e;   // '<S53>/Discrete-Time Integrator'
  real32_T Merge;                      // '<S21>/Merge'
  real32_T Merge_j;                    // '<S22>/Merge'
  real32_T Merge_k;                    // '<S23>/Merge'
  real32_T Merge_jc;                   // '<S24>/Merge'
  real32_T Divide;                     // '<S52>/Divide'
  real32_T DiscreteTimeIntegrator_DSTATE;// '<S3>/Discrete-Time Integrator'
  real32_T DiscreteTimeIntegrator_DSTATE_n;// '<S53>/Discrete-Time Integrator'
  real32_T DiscreteFilter_tmp;         // '<S1>/Discrete Filter'
  real32_T curr_time;                  // '<S54>/FindTireRadius'
  real32_T radar_est_rad_conv_time_start;// '<S54>/FindTireRadius'
  real32_T running_avg_radar_estimated_tire_radius;// '<S54>/FindTireRadius'
  real32_T last_radar_estimated_tire_radius;// '<S54>/FindTireRadius'
  real32_T radar_est_rad_conv_persistent;// '<S54>/FindTireRadius'
  struct {
    uint_T is_c5_WheelRadiusSpeedAndLongSlipEstimation:2;// '<S1>/Chart'
    uint_T is_active_c5_WheelRadiusSpeedAndLongSlipEstimation:1;// '<S1>/Chart'
    uint_T f_initialize_speed_gps_comp:1;// '<S1>/Chart'
    uint_T Memory_PreviousInput:1;     // '<S6>/Memory'
    uint_T Memory1_PreviousInput:1;    // '<S6>/Memory1'
    uint_T curr_time_not_empty:1;      // '<S54>/FindTireRadius'
    uint_T f_radar_est_rad_converged_temp:1;// '<S54>/FindTireRadius'
    uint_T f_radar_est_rad_converged:1;// '<S54>/FindTireRadius'
    uint_T f_radar_est_rad_conv_time_start:1;// '<S54>/FindTireRadius'
    uint_T f_gps_distance_accum_started:1;// '<S7>/LockStartingGPSPosition'
    uint_T objisempty:1;               // '<S6>/Lowpass Filter'
    uint_T isInitialized:1;            // '<S6>/Lowpass Filter'
  } bitsForTID0;

  enum_quality_factor_T GPS_QF_locked; // '<S7>/LockStartingGPSPosition'
} DW_WheelRadiusSpeedAndLongSlipEstimation_T;

// Class declaration for model WheelRadiusSpeedAndLongSlipEstimation
class WheelRadiusSpeedAndLongSlipEstimationModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_raw_speed_mps, const enum_quality_factor_T
            *rtu_raw_speed_qf, const real32_T *rtu_WheelSpeed_FR_RPM, const
            real32_T *rtu_WheelSpeed_FL_RPM, const real32_T
            *rtu_WheelSpeed_RR_RPM, const real32_T *rtu_WheelSpeed_RL_RPM, const
            real32_T *rtu_comp_yaw_rate_filtered, const real_T *rtu_GPS_Lat_mas,
            const real_T *rtu_GPS_Long_mas, const enum_quality_factor_T
            *rtu_GPS_QF, const boolean_T *rtu_f_new_GPS_data, const real32_T
            *rtu_radar_compensated_speed_mps, const enum_quality_factor_T
            *rtu_radar_compensated_speed_qf, const real32_T
            *rtu_default_front_tire_radius_m, const real32_T
            *rtu_default_rear_tire_radius_m, real32_T
            *rty_front_left_estimated_tire_radius, tire_radius_estimation_mode_T
            *rty_front_left_estimated_tire_radius_mode, real32_T
            *rty_front_right_estimated_tire_radius,
            tire_radius_estimation_mode_T
            *rty_front_right_estimated_tire_radius_mode, real32_T
            *rty_rear_left_estimated_tire_radius, tire_radius_estimation_mode_T *
            rty_rear_left_estimated_tire_radius_mode, real32_T
            *rty_rear_right_estimated_tire_radius, tire_radius_estimation_mode_T
            *rty_rear_right_estimated_tire_radius_mode, real32_T
            *rty_gps_comp_factor, boolean_T *rty_f_gps_comp_factor);

  // Initial conditions function
  void init();

  // model start function
  void start();

  // Constructor
  WheelRadiusSpeedAndLongSlipEstimationModelClass();

  // Destructor
  ~WheelRadiusSpeedAndLongSlipEstimationModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_WheelRadiusSpeedAndLongSlipEstimation_T
    WheelRadiusSpeedAndLongSlipEstimation_DW;

  // private member function(s) for subsystem '<Root>/TmpModelReferenceSubsystem'
  void WheelRadiusSpeedAndLongSlipEstimation_SystemCore_release
    (dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);
  void WheelRadiusSpeedAndLongSlipEstimation_LPHPFilterBase_releaseImpl
    (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);
  void WheelRadiusSpeedAndLongSlipEstimation_SystemCore_releaseWrapper
    (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);
  void WheelRadiusSpeedAndLongSlipEstimation_SystemCore_release_m
    (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);
  void WheelRadiusSpeedAndLongSlipEstimation_SystemCore_delete_a
    (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);
  void
    WheelRadiusSpeedAndLongSlipEstimation_matlabCodegenHandle_matlabCodegenDestructor_g
    (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);
  void WheelRadiusSpeedAndLongSlipEstimation_SystemCore_delete
    (dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);
  void
    WheelRadiusSpeedAndLongSlipEstimation_matlabCodegenHandle_matlabCodegenDestructor
    (dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj);

  // model instance variable for '<S7>/FindDistFrom_GPSLatLongDiff'
  FindDistFrom_GPSLatLongDiffModelClass FindDistFrom_GPSLatLongDiffMDLOBJ1;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S28>/Data Type Duplicate' : Unused code path elimination
//  Block '<S28>/Data Type Propagation' : Unused code path elimination
//  Block '<S32>/Data Type Duplicate' : Unused code path elimination
//  Block '<S32>/Data Type Propagation' : Unused code path elimination
//  Block '<S36>/Data Type Duplicate' : Unused code path elimination
//  Block '<S36>/Data Type Propagation' : Unused code path elimination
//  Block '<S40>/Data Type Duplicate' : Unused code path elimination
//  Block '<S40>/Data Type Propagation' : Unused code path elimination


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
//  '<Root>' : 'WheelRadiusSpeedAndLongSlipEstimation'
//  '<S1>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation'
//  '<S2>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates'
//  '<S3>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateDistanceFromRawSpeed'
//  '<S4>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/Chart'
//  '<S5>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator'
//  '<S6>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FindIfAccelAndYawRateAreLow'
//  '<S7>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation'
//  '<S8>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed'
//  '<S9>'   : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/Speed_To_Accel_TF_Coeff'
//  '<S10>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromFrontLeftWheelSpeed'
//  '<S11>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromFrontRightWheelSpeed'
//  '<S12>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromRearLeftWheelSpeed'
//  '<S13>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromRearRightWheelSpeed'
//  '<S14>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromFrontLeftWheelSpeed/AccumulateAngleFromFrontLeftWheelSpeed'
//  '<S15>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromFrontRightWheelSpeed/AccumulateAngleFromFrontRightWheelSpeed'
//  '<S16>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromRearLeftWheelSpeed/AccumulateAngleFromRearLeftWheelSpeed'
//  '<S17>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateAngleFromWheelRotationRates/AccumulateAngleFromRearRightWheelSpeed/AccumulateAngleFromRearRightWheelSpeed'
//  '<S18>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateDistanceFromRawSpeed/Compare To Constant'
//  '<S19>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateDistanceFromRawSpeed/Compare To Constant1'
//  '<S20>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/AccumulateDistanceFromRawSpeed/Compare To Constant2'
//  '<S21>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Left Tire Radius Arbitration'
//  '<S22>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Right Tire Radius Arbitration'
//  '<S23>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Left Tire Radius Arbitration'
//  '<S24>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Right Tire Radius Arbitration'
//  '<S25>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Left Tire Radius Arbitration/If Action Subsystem'
//  '<S26>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Left Tire Radius Arbitration/If Action Subsystem1'
//  '<S27>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Left Tire Radius Arbitration/If Action Subsystem2'
//  '<S28>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Left Tire Radius Arbitration/Saturation Dynamic1'
//  '<S29>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Right Tire Radius Arbitration/If Action Subsystem'
//  '<S30>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Right Tire Radius Arbitration/If Action Subsystem1'
//  '<S31>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Right Tire Radius Arbitration/If Action Subsystem2'
//  '<S32>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Front Right Tire Radius Arbitration/Saturation Dynamic1'
//  '<S33>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Left Tire Radius Arbitration/If Action Subsystem'
//  '<S34>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Left Tire Radius Arbitration/If Action Subsystem1'
//  '<S35>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Left Tire Radius Arbitration/If Action Subsystem2'
//  '<S36>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Left Tire Radius Arbitration/Saturation Dynamic1'
//  '<S37>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Right Tire Radius Arbitration/If Action Subsystem'
//  '<S38>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Right Tire Radius Arbitration/If Action Subsystem1'
//  '<S39>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Right Tire Radius Arbitration/If Action Subsystem2'
//  '<S40>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FinalRadiusEstimationArbitrator/Rear Right Tire Radius Arbitration/Saturation Dynamic1'
//  '<S41>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/FindIfAccelAndYawRateAreLow/Compare To Constant2'
//  '<S42>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindFrontLeftTireRadius'
//  '<S43>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindFrontRightTireRadius'
//  '<S44>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindRearLeftTireRadius'
//  '<S45>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindRearRightTireRadius'
//  '<S46>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/GPSCompFactorCalculation'
//  '<S47>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/LockStartingGPSPosition'
//  '<S48>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindFrontLeftTireRadius/FindTireRadius'
//  '<S49>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindFrontRightTireRadius/FindTireRadius'
//  '<S50>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindRearLeftTireRadius/FindTireRadius'
//  '<S51>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/FindRearRightTireRadius/FindTireRadius'
//  '<S52>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusAndGPSCompFactorCalculation/GPSCompFactorCalculation/If Action Subsystem'
//  '<S53>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/AccumulateDistanceFromRadarCompSpeed'
//  '<S54>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindFrontLeftTireRadius'
//  '<S55>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindFrontRightTireRadius'
//  '<S56>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindRearLeftTireRadius'
//  '<S57>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindRearRightTireRadius'
//  '<S58>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/AccumulateDistanceFromRadarCompSpeed/Compare To Constant'
//  '<S59>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/AccumulateDistanceFromRadarCompSpeed/Compare To Constant1'
//  '<S60>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/AccumulateDistanceFromRadarCompSpeed/Compare To Constant2'
//  '<S61>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindFrontLeftTireRadius/FindTireRadius'
//  '<S62>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindFrontRightTireRadius/FindTireRadius'
//  '<S63>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindRearLeftTireRadius/FindTireRadius'
//  '<S64>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/RadiusFromRadarCompSpeed/FindRearRightTireRadius/FindTireRadius'
//  '<S65>'  : 'WheelRadiusSpeedAndLongSlipEstimation/WheelRadiusSpeedAndLongSlipEstimation/Speed_To_Accel_TF_Coeff/Find_Speed_To_Accel_TF'

#endif                                 // RTW_HEADER_WheelRadiusSpeedAndLongSlipEstimation_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
