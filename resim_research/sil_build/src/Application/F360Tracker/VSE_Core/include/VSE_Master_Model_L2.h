//
// File: VSE_Master_Model_L2.h
//
// Code generated for Simulink model 'VSE_Master_Model_L2'.
//
// Model version                  : 1.794
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 19:00:41 2024
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
#ifndef RTW_HEADER_VSE_Master_Model_L2_h_
#define RTW_HEADER_VSE_Master_Model_L2_h_
#include <math.h>
#include <string.h>
#ifndef VSE_Master_Model_L2_COMMON_INCLUDES_
# define VSE_Master_Model_L2_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // VSE_Master_Model_L2_COMMON_INCLUDES_

#include "VSE_Master_Model_L2_types.h"

// Child system includes
#include "YawRateEstimationUsingSteeringWheelAngle.h"
#include "YawRateCompensation.h"
#include "WheelRadiusSpeedAndLongSlipEstimation.h"
#include "VehicleMassEstimator_2018b.h"
#include "VehicleCurvatureAndSideslipEstimation_Simulink.h"
#include "TrailerOscillationEstimation_2018b.h"
#include "TrailerDetection.h"
#include "SteeringWheelAngleBiasEstimation.h"
#include "SpeedCompensationArbitration.h"
#include "SpeedCompensation.h"
#include "LateralJerkEstimation.h"
#include "DtrmnVehStationary.h"
#include "Acceleration_VCS.h"

// Macros for accessing real-time model data structure

// Block signals and states (default storage) for system '<S4>/Initialization_Trigger' 
typedef struct {
  struct {
    uint_T is_c5_Initialization_Trigger:2;// '<S4>/Initialization_Trigger'
    uint_T is_active_c5_Initialization_Trigger:1;// '<S4>/Initialization_Trigger' 
  } bitsForTID0;
} DW_Initialization_Trigger_VSE_Master_Model_L2_T;

// Block signals and states (default storage) for system '<S66>/MATLAB Function' 
typedef struct {
  real32_T matured_time;               // '<S66>/MATLAB Function'
  real32_T dematured_time;             // '<S66>/MATLAB Function'
  struct {
    uint_T prev_final_fault_flag:1;    // '<S66>/MATLAB Function'
  } bitsForTID0;
} DW_MATLABFunction_VSE_Master_Model_L2_T;

// Block signals and states (default storage) for system '<Root>'
typedef struct {
  DW_Initialization_Trigger_VSE_Master_Model_L2_T sf_Initialization_Trigger_ah;// '<S17>/Initialization_Trigger' 
  DW_Initialization_Trigger_VSE_Master_Model_L2_T sf_Initialization_Trigger_lx;// '<S15>/Initialization_Trigger' 
  DW_Initialization_Trigger_VSE_Master_Model_L2_T sf_Initialization_Trigger_a;// '<S14>/Initialization_Trigger' 
  DW_MATLABFunction_VSE_Master_Model_L2_T sf_MATLABFunction_i;// '<S84>/MATLAB Function' 
  DW_MATLABFunction_VSE_Master_Model_L2_T sf_MATLABFunction_n;// '<S81>/MATLAB Function' 
  DW_MATLABFunction_VSE_Master_Model_L2_T sf_MATLABFunction_m;// '<S78>/MATLAB Function' 
  DW_MATLABFunction_VSE_Master_Model_L2_T sf_MATLABFunction_f;// '<S75>/MATLAB Function' 
  DW_MATLABFunction_VSE_Master_Model_L2_T sf_MATLABFunction_g;// '<S72>/MATLAB Function' 
  DW_MATLABFunction_VSE_Master_Model_L2_T sf_MATLABFunction_p;// '<S69>/MATLAB Function' 
  DW_MATLABFunction_VSE_Master_Model_L2_T sf_MATLABFunction;// '<S66>/MATLAB Function' 
  DW_Initialization_Trigger_VSE_Master_Model_L2_T sf_Initialization_Trigger_l;// '<S5>/Initialization_Trigger' 
  DW_Initialization_Trigger_VSE_Master_Model_L2_T sf_Initialization_Trigger;// '<S4>/Initialization_Trigger' 
  real_T filter_discrete_num[5];       // '<S34>/FindFilterCoeff'
  real_T filter_discrete_denom[5];     // '<S34>/FindFilterCoeff'
  real_T diff_num[8];                  // '<S33>/FindDifferentiatorCoeff'
  real_T diff_denom[8];                // '<S33>/FindDifferentiatorCoeff'
  real_T LowPassFilter_states[4];      // '<S35>/LowPass Filter'
  real_T Differentiator_states[7];     // '<S36>/Differentiator'
  real32_T diff_num_b[4];              // '<S112>/MATLAB Function'
  real32_T diff_denom_k[4];            // '<S112>/MATLAB Function'
  real32_T diff_num_f[4];              // '<S99>/MATLAB Function'
  real32_T diff_denom_a[4];            // '<S99>/MATLAB Function'
  real32_T diff_num_m[4];              // '<S87>/MATLAB Function'
  real32_T diff_denom_g[4];            // '<S87>/MATLAB Function'
  real32_T diff_num_mm[4];             // '<S54>/MATLAB Function'
  real32_T diff_denom_j[4];            // '<S54>/MATLAB Function'
  real32_T diff_num_e[4];              // '<S42>/MATLAB Function'
  real32_T diff_denom_gs[4];           // '<S42>/MATLAB Function'
  real32_T DiscreteFilter_states[3];   // '<S14>/Discrete Filter'
  real32_T DiscreteFilter_states_k[3]; // '<S4>/Discrete Filter'
  real32_T DiscreteFilter_states_ku[3];// '<S17>/Discrete Filter'
  real32_T DiscreteFilter_states_e[3]; // '<S15>/Discrete Filter'
  real32_T DiscreteFilter_states_f[3]; // '<S5>/Discrete Filter'
  real32_T SWA_Bias_deg_Conv_Out;      // '<S1>/Model1'
  real32_T CompSteeringAngle_deg_SAE;  // '<S1>/Model1'
  real32_T Delay_DSTATE;               // '<S22>/Delay'
  real32_T Delay_DSTATE_i;             // '<S21>/Delay'
  real32_T Delay_DSTATE_n;             // '<S20>/Delay'
  real32_T Delay_DSTATE_f;             // '<S18>/Delay'
  real32_T Delay_DSTATE_o;             // '<S1>/Delay'
  real32_T Delay_DSTATE_ne;            // '<S23>/Delay'
  real32_T Delay_DSTATE_p;             // '<S24>/Delay'
  struct {
    uint_T is_c3_VSE_Master_Model_L2:2;// '<S16>/Chart'
    uint_T is_c4_JerkEstimator:2;      // '<S3>/Chart'
    uint_T is_active_c3_VSE_Master_Model_L2:1;// '<S16>/Chart'
    uint_T is_active_c4_JerkEstimator:1;// '<S3>/Chart'
    uint_T f_initialize_JerkEstimation:1;// '<S3>/Chart'
    uint_T Delay2_DSTATE:1;            // '<S1>/Delay2'
    uint_T f_initialize_vse:1;         // '<S16>/Chart'
  } bitsForTID0;

  tire_radius_estimation_mode_T WheelRadiusEstimation_o2;// '<S1>/WheelRadiusEstimation' 
  tire_radius_estimation_mode_T WheelRadiusEstimation_o4;// '<S1>/WheelRadiusEstimation' 
  tire_radius_estimation_mode_T WheelRadiusEstimation_o6;// '<S1>/WheelRadiusEstimation' 
  tire_radius_estimation_mode_T WheelRadiusEstimation_o8;// '<S1>/WheelRadiusEstimation' 
  enum_quality_factor_T CompSteeringAngle_QF;// '<S1>/Model1'
  enum_Presence_Status Trailer_Detection;// '<S1>/Model5'
  enum_quality_factor_T Delay1_DSTATE; // '<S1>/Delay1'
  boolean_T f_SWA_Bias_deg_Conv_out;   // '<S1>/Model1'
  boolean_T f_SWA_Bias_deg_Conv_internal;// '<S1>/Model1'
  boolean_T f_initialize;              // '<S17>/Initialization_Trigger'
  boolean_T f_initialize_o;            // '<S15>/Initialization_Trigger'
  boolean_T f_initialize_f;            // '<S14>/Initialization_Trigger'
  boolean_T f_initialize_d;            // '<S5>/Initialization_Trigger'
  boolean_T f_initialize_a;            // '<S4>/Initialization_Trigger'
} DW_VSE_Master_Model_L2_T;

// Invariant block signals (default storage)
typedef const struct tag_ConstB_VSE_Master_Model_L2_T {
  enum_quality_factor_T DataTypeConversion5;// '<S1>/Data Type Conversion5'
  enum_quality_factor_T DataTypeConversion6;// '<S1>/Data Type Conversion6'
  enum_quality_factor_T Cast;          // '<S1>/Cast'
  boolean_T CastToBoolean;             // '<S1>/Cast To Boolean'
} ConstB_VSE_Master_Model_L2_T;

// Constant parameters (default storage)
typedef struct {
  // Pooled Parameter (Expression: 1)
  //  Referenced by:
  //    '<S1>/Constant1'
  //    '<S1>/Constant11'

  real_T pooled4;

  // Pooled Parameter (Mixed Expressions)
  //  Referenced by:
  //    '<S1>/Constant15'
  //    '<S1>/Constant2'
  //    '<S1>/Constant5'
  //    '<S1>/Delay'
  //    '<S6>/Constant1'
  //    '<S7>/Constant1'
  //    '<S8>/Constant1'
  //    '<S9>/Constant1'
  //    '<S10>/Constant1'
  //    '<S11>/Constant1'
  //    '<S12>/Constant1'
  //    '<S18>/Delay'
  //    '<S20>/Delay'
  //    '<S21>/Delay'
  //    '<S22>/Delay'
  //    '<S23>/Delay'
  //    '<S24>/Delay'
  //    '<S42>/diff_num'
  //    '<S42>/diff_den'
  //    '<S54>/diff_num'
  //    '<S54>/diff_den'
  //    '<S87>/diff_num'
  //    '<S87>/diff_den'
  //    '<S89>/Constant'
  //    '<S99>/diff_num'
  //    '<S99>/diff_den'
  //    '<S112>/diff_num'
  //    '<S112>/diff_den'

  real32_T pooled6;

  // Pooled Parameter (Expression: 1)
  //  Referenced by:
  //    '<S1>/Constant13'
  //    '<S1>/default_gps_comp_factor'
  //    '<S13>/Constant1'

  real32_T pooled10;

  // Pooled Parameter (Mixed Expressions)
  //  Referenced by:
  //    '<S1>/Constant21'
  //    '<S1>/default_f_use_gps_comp_factor'
  //    '<S1>/Delay2'

  boolean_T pooled17;
} ConstP_VSE_Master_Model_L2_T;

// External inputs (root inport signals with default storage)
typedef struct {
  VCAN_VSE VCAN_VSE_p;                 // '<Root>/VCAN_VSE'
  Tracker_VSE Tracker_VSE_j;           // '<Root>/Tracker_VSE'
  Vehicle_Parameters Vehicle_Parameters_f;// '<Root>/Vehicle_Parameters'
  uint64_T Vs_us_SystemTime;           // '<Root>/Vs_us_SystemTime'
  LAST_KEY_CYCLE LAST_KEY_CYCLE_e;     // '<Root>/LAST_KEY_CYCLE'
  Tracker_VSE_Trailer_Signals Tracker_VSE_Trailer_Signals_g;// '<Root>/Tracker_VSE_Trailer_Signals' 
  PROXI PROXI_h;                       // '<Root>/PROXI'
} ExtU_VSE_Master_Model_L2_T;

// External outputs (root outports fed by signals with default storage)
typedef struct {
  VSE_OUT VSE_Output;                  // '<Root>/VSE_Output'
  VSE_RESIM_T VSE_Resim_Output;        // '<Root>/VSE_Resim_Output'
  VSE_OUT_Trailer VSE_Output_Trailer;  // '<Root>/VSE_Output_Trailer'
} ExtY_VSE_Master_Model_L2_T;

extern const ConstB_VSE_Master_Model_L2_T VSE_Master_Model_L2_ConstB;// constant block i/o 

// Constant parameters (default storage)
extern const ConstP_VSE_Master_Model_L2_T VSE_Master_Model_L2_ConstP;

// Class declaration for model VSE_Master_Model_L2
class VSE_Master_Model_L2ModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU_VSE_Master_Model_L2_T VSE_Master_Model_L2_U;

  // External outputs
  ExtY_VSE_Master_Model_L2_T VSE_Master_Model_L2_Y;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  VSE_Master_Model_L2ModelClass();

  // Destructor
  ~VSE_Master_Model_L2ModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_VSE_Master_Model_L2_T VSE_Master_Model_L2_DW;

  // model instance variable for '<S1>/Acceleration_VCS'
  AccelerationRearAxleModelClass Acceleration_VCSMDLOBJ1;

  // model instance variable for '<S1>/Model'
  SpeedCompensationModelClass ModelMDLOBJ2;

  // model instance variable for '<S1>/Model1'
  SteeringWheelAngleBiasEstimationModelClass Model1MDLOBJ3;

  // model instance variable for '<S1>/Model2'
  DtrmnVehStationaryModelClass Model2MDLOBJ4;

  // model instance variable for '<S1>/Model3'
  LateralJerkEstimationModelClass Model3MDLOBJ5;

  // model instance variable for '<S1>/Model4'
  VehicleCurvatureAndSideslipEstimation_SimulinkModelClass Model4MDLOBJ6;

  // model instance variable for '<S1>/Model5'
  TrailerDetectionModelClass Model5MDLOBJ7;

  // model instance variable for '<S1>/Model6'
  YawRateEstimationUsingSteeringWheelAngleModelClass Model6MDLOBJ8;

  // model instance variable for '<S1>/Model8'
  SpeedCompensationArbitrationModelClass Model8MDLOBJ9;

  // model instance variable for '<S1>/Model9'
  TrailerOscillationEstimation_2018bModelClass Model9MDLOBJ10;

  // model instance variable for '<S1>/TrailerMassEstimation'
  VehicleMassEstimator_2018bModelClass TrailerMassEstimationMDLOBJ11;

  // model instance variable for '<S1>/WheelRadiusEstimation'
  WheelRadiusSpeedAndLongSlipEstimationModelClass WheelRadiusEstimationMDLOBJ12;

  // model instance variable for '<S1>/Model7'
  YawRateCompensationModelClass Model7MDLOBJ13;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S26>/Compare' : Unused code path elimination
//  Block '<S26>/Constant' : Unused code path elimination
//  Block '<S19>/Delay' : Unused code path elimination
//  Block '<S19>/Switch' : Unused code path elimination


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
//  '<Root>' : 'VSE_Master_Model_L2'
//  '<S1>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2'
//  '<S2>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper'
//  '<S3>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator'
//  '<S4>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck'
//  '<S5>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck'
//  '<S6>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompSpeed_Fault_Based_On_QF'
//  '<S7>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompYawRate_Fault_Based_On_QF'
//  '<S8>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLatAccel_Fault_Based_On_QF'
//  '<S9>'   : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLongAccel_Fault_Based_On_QF'
//  '<S10>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSpeed_Fault_Based_On_QF'
//  '<S11>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSteering_Fault_Based_On_QF'
//  '<S12>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawYawRate_Fault_Based_On_QF'
//  '<S13>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Signed_Speed_Calculation'
//  '<S14>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck'
//  '<S15>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck'
//  '<S16>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/VehicleIndexCalculation'
//  '<S17>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck'
//  '<S18>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based'
//  '<S19>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based1'
//  '<S20>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based2'
//  '<S21>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based4'
//  '<S22>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based6'
//  '<S23>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based7'
//  '<S24>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based8'
//  '<S25>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based/Compare To Constant'
//  '<S26>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based1/Compare To Constant'
//  '<S27>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based2/Compare To Constant'
//  '<S28>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based4/Compare To Constant'
//  '<S29>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based6/Compare To Constant'
//  '<S30>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based7/Compare To Constant'
//  '<S31>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/InputWrapper/Hold_Last_Value_Condition_Based8/Compare To Constant'
//  '<S32>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Chart'
//  '<S33>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Differentiator Generator'
//  '<S34>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Filter Generator'
//  '<S35>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Lateral Acceleration Filter'
//  '<S36>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Lateral Jerk Estimator'
//  '<S37>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Differentiator Generator/FindDifferentiatorCoeff'
//  '<S38>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Filter Generator/FindFilterCoeff'
//  '<S39>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Lateral Acceleration Filter/Compare To Constant'
//  '<S40>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Jerk_Estimator/Lateral Jerk Estimator/Compare To Constant'
//  '<S41>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/DecideQualityFactor'
//  '<S42>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/Initialization'
//  '<S43>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/Initialization_Trigger'
//  '<S44>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/SignalRangeCheck'
//  '<S45>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/SignalRateCheck'
//  '<S46>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/DecideQualityFactor/Compare To Constant'
//  '<S47>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem'
//  '<S48>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem1'
//  '<S49>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem2'
//  '<S50>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem3'
//  '<S51>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem4'
//  '<S52>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LateralAcceleration_PlausibilityCheck/Initialization/MATLAB Function'
//  '<S53>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/DecideQualityFactor'
//  '<S54>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/Initialization'
//  '<S55>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/Initialization_Trigger'
//  '<S56>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/SignalRangeCheck'
//  '<S57>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/SignalRateCheck'
//  '<S58>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/DecideQualityFactor/Compare To Constant'
//  '<S59>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem'
//  '<S60>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem1'
//  '<S61>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem2'
//  '<S62>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem3'
//  '<S63>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/DecideQualityFactor/If Action Subsystem4'
//  '<S64>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/LongitudinalAcceleration_PlausibilityCheck/Initialization/MATLAB Function'
//  '<S65>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompSpeed_Fault_Based_On_QF/Compare To Constant'
//  '<S66>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompSpeed_Fault_Based_On_QF/Maturation_DeMaturation_Block'
//  '<S67>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompSpeed_Fault_Based_On_QF/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S68>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompYawRate_Fault_Based_On_QF/Compare To Constant'
//  '<S69>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompYawRate_Fault_Based_On_QF/Maturation_DeMaturation_Block'
//  '<S70>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_CompYawRate_Fault_Based_On_QF/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S71>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLatAccel_Fault_Based_On_QF/Compare To Constant'
//  '<S72>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLatAccel_Fault_Based_On_QF/Maturation_DeMaturation_Block'
//  '<S73>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLatAccel_Fault_Based_On_QF/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S74>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLongAccel_Fault_Based_On_QF/Compare To Constant'
//  '<S75>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLongAccel_Fault_Based_On_QF/Maturation_DeMaturation_Block'
//  '<S76>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawLongAccel_Fault_Based_On_QF/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S77>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSpeed_Fault_Based_On_QF/Compare To Constant'
//  '<S78>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSpeed_Fault_Based_On_QF/Maturation_DeMaturation_Block'
//  '<S79>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSpeed_Fault_Based_On_QF/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S80>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSteering_Fault_Based_On_QF/Compare To Constant'
//  '<S81>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSteering_Fault_Based_On_QF/Maturation_DeMaturation_Block'
//  '<S82>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawSteering_Fault_Based_On_QF/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S83>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawYawRate_Fault_Based_On_QF/Compare To Constant'
//  '<S84>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawYawRate_Fault_Based_On_QF/Maturation_DeMaturation_Block'
//  '<S85>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Set_RawYawRate_Fault_Based_On_QF/Maturation_DeMaturation_Block/MATLAB Function'
//  '<S86>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/DecideQualityFactor'
//  '<S87>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/Initialization'
//  '<S88>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/Initialization_Trigger'
//  '<S89>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/SignalRangeCheck'
//  '<S90>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/SignalRateCheck'
//  '<S91>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/DecideQualityFactor/Compare To Constant'
//  '<S92>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/DecideQualityFactor/If Action Subsystem'
//  '<S93>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/DecideQualityFactor/If Action Subsystem1'
//  '<S94>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/DecideQualityFactor/If Action Subsystem2'
//  '<S95>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/DecideQualityFactor/If Action Subsystem3'
//  '<S96>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/DecideQualityFactor/If Action Subsystem4'
//  '<S97>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/Speed_PlausibilityCheck/Initialization/MATLAB Function'
//  '<S98>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/DecideQualityFactor'
//  '<S99>'  : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/Initialization'
//  '<S100>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/Initialization_Trigger'
//  '<S101>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/SignalRangeCheck'
//  '<S102>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/SignalRateCheck'
//  '<S103>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/DecideQualityFactor/Compare To Constant'
//  '<S104>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/DecideQualityFactor/If Action Subsystem'
//  '<S105>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/DecideQualityFactor/If Action Subsystem1'
//  '<S106>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/DecideQualityFactor/If Action Subsystem2'
//  '<S107>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/DecideQualityFactor/If Action Subsystem3'
//  '<S108>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/DecideQualityFactor/If Action Subsystem4'
//  '<S109>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/SteeringWhlAngle_PlausibilityCheck/Initialization/MATLAB Function'
//  '<S110>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/VehicleIndexCalculation/Chart'
//  '<S111>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/DecideQualityFactor'
//  '<S112>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/Initialization'
//  '<S113>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/Initialization_Trigger'
//  '<S114>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/SignalRangeCheck'
//  '<S115>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/SignalRateCheck'
//  '<S116>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/DecideQualityFactor/Compare To Constant'
//  '<S117>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/DecideQualityFactor/If Action Subsystem'
//  '<S118>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/DecideQualityFactor/If Action Subsystem1'
//  '<S119>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/DecideQualityFactor/If Action Subsystem2'
//  '<S120>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/DecideQualityFactor/If Action Subsystem3'
//  '<S121>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/DecideQualityFactor/If Action Subsystem4'
//  '<S122>' : 'VSE_Master_Model_L2/VSE_Master_Model_L2/YawRate_PlausibilityCheck/Initialization/MATLAB Function'

#endif                                 // RTW_HEADER_VSE_Master_Model_L2_h_

//
// File trailer for generated code.
//
// [EOF]
//
