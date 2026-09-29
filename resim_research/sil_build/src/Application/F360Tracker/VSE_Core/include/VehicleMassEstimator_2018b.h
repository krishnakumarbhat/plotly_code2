//
// File: VehicleMassEstimator_2018b.h
//
// Code generated for Simulink model 'VehicleMassEstimator_2018b'.
//
// Model version                  : 1.364
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:00 2024
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
#ifndef RTW_HEADER_VehicleMassEstimator_2018b_h_
#define RTW_HEADER_VehicleMassEstimator_2018b_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef VehicleMassEstimator_2018b_COMMON_INCLUDES_
# define VehicleMassEstimator_2018b_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // VehicleMassEstimator_2018b_COMMON_INCLUDES_ 

#include "VehicleMassEstimator_2018b_types.h"
#include <stddef.h>

// Block signals and states (default storage) for system '<S3>/Highpass Filter3' 
typedef struct {
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T gobj_0;// '<S3>/Highpass Filter3' 
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T gobj_1;// '<S3>/Highpass Filter3' 
  dsp_HighpassFilter_VehicleMassEstimator_2018b_T obj;// '<S3>/Highpass Filter3' 
  real_T HighpassFilter3;              // '<S3>/Highpass Filter3'
  struct {
    uint_T objisempty:1;               // '<S3>/Highpass Filter3'
    uint_T isInitialized:1;            // '<S3>/Highpass Filter3'
  } bitsForTID0;
} DW_HighpassFilter3_VehicleMassEstimator_2018b_T;

// Block signals and states (default storage) for system '<S3>/Lowpass Filter2'
typedef struct {
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T gobj_0;// '<S3>/Lowpass Filter2' 
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T gobj_1;// '<S3>/Lowpass Filter2' 
  dsp_LowpassFilter_VehicleMassEstimator_2018b_T obj;// '<S3>/Lowpass Filter2'
  real_T LowpassFilter2;               // '<S3>/Lowpass Filter2'
  struct {
    uint_T objisempty:1;               // '<S3>/Lowpass Filter2'
    uint_T isInitialized:1;            // '<S3>/Lowpass Filter2'
  } bitsForTID0;
} DW_LowpassFilter2_VehicleMassEstimator_2018b_T;

// Block signals and states (default storage) for model 'VehicleMassEstimator_2018b' 
typedef struct {
  DW_LowpassFilter2_VehicleMassEstimator_2018b_T LowpassFilter;// '<S3>/Lowpass Filter2' 
  DW_HighpassFilter3_VehicleMassEstimator_2018b_T HighpassFilter1;// '<S3>/Highpass Filter3' 
  DW_LowpassFilter2_VehicleMassEstimator_2018b_T LowpassFilter1;// '<S3>/Lowpass Filter2' 
  DW_HighpassFilter3_VehicleMassEstimator_2018b_T HighpassFilter2;// '<S3>/Highpass Filter3' 
  DW_LowpassFilter2_VehicleMassEstimator_2018b_T LowpassFilter2;// '<S3>/Lowpass Filter2' 
  DW_HighpassFilter3_VehicleMassEstimator_2018b_T HighpassFilter3;// '<S3>/Highpass Filter3' 
  real_T x_ekf[4];                     // '<S3>/MassEstimation'
  real_T P_ekf[16];                    // '<S3>/MassEstimation'
  real_T prev_speed;                   // '<S3>/MassEstimation'
  real_T count;                        // '<S3>/MassEstimation'
  real_T running_av;                   // '<S3>/MassEstimation'
  real_T prev_speed_filt;              // '<S3>/MassEstimation'
  real_T acc_filt_debounce;            // '<S3>/MassEstimation'
  real_T brake_filt_debounce;          // '<S3>/MassEstimation'
  real_T P_rls;                        // '<S3>/MassEstimation'
  real_T w_rls;                        // '<S3>/MassEstimation'
  real_T x_ekf2;                       // '<S3>/MassEstimation'
  real_T P_ekf2;                       // '<S3>/MassEstimation'
  real_T converged_mass;               // '<S3>/MassEstimation'
  real_T count_acc_sample;             // '<S3>/MassEstimation'
  real_T count_brake_sample;           // '<S3>/MassEstimation'
  real_T running_av_acc;               // '<S3>/MassEstimation'
  real_T running_av_brake;             // '<S3>/MassEstimation'
  real32_T Delay1_DSTATE;              // '<S4>/Delay1'
  struct {
    uint_T prev_speed_not_empty:1;     // '<S3>/MassEstimation'
    uint_T converge_flag:1;            // '<S3>/MassEstimation'
  } bitsForTID0;

  uint8_T icLoad;                      // '<S4>/Delay1'
} DW_VehicleMassEstimator_2018b_T;

// Class declaration for model VehicleMassEstimator_2018b
class VehicleMassEstimator_2018bModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_VsVCAN_Nm_EngTrq, const real32_T
            *rtu_filtered_speed_over_ground, const real32_T
            *rtu_Wheel_Running_Radii, const real32_T *rtu_VsVCAN_Nm_BrkTrq,
            const real32_T *rtu_raw_long_accel, const real32_T
            *rtu_Comp_YawRate_Filtered, const real32_T *rtu_GearReductionRatio,
            real32_T *rty_VsVSE_Kg_TrlrMass, boolean_T *rty_VsVSE_b_TrlrMassConv);

  // Initial conditions function
  void init();

  // model start function
  void start();

  // Constructor
  VehicleMassEstimator_2018bModelClass();

  // Destructor
  ~VehicleMassEstimator_2018bModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_VehicleMassEstimator_2018b_T VehicleMassEstimator_2018b_DW;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Cast To Double8' : Unused code path elimination


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
//  '<Root>' : 'VehicleMassEstimator_2018b'
//  '<S1>'   : 'VehicleMassEstimator_2018b/Vehicle Mass Estimation'
//  '<S2>'   : 'VehicleMassEstimator_2018b/Vehicle Mass Estimation/ForgettingFactor'
//  '<S3>'   : 'VehicleMassEstimator_2018b/Vehicle Mass Estimation/MassEstimation'
//  '<S4>'   : 'VehicleMassEstimator_2018b/Vehicle Mass Estimation/Moving_Average_Leaky_Integrator'
//  '<S5>'   : 'VehicleMassEstimator_2018b/Vehicle Mass Estimation/MassEstimation/MassEstimation'

#endif                                 // RTW_HEADER_VehicleMassEstimator_2018b_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
