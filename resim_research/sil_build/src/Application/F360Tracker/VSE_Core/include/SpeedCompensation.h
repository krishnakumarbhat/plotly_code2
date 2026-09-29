//
// File: SpeedCompensation.h
//
// Code generated for Simulink model 'SpeedCompensation'.
//
// Model version                  : 1.541
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:57:34 2024
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
#ifndef RTW_HEADER_SpeedCompensation_h_
#define RTW_HEADER_SpeedCompensation_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef SpeedCompensation_COMMON_INCLUDES_
# define SpeedCompensation_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // SpeedCompensation_COMMON_INCLUDES_

#include "SpeedCompensation_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'SpeedCompensation'
typedef struct {
  real32_T UnitDelay2_DSTATE;          // '<S3>/Unit Delay2'
  real32_T ResettableDelay_DSTATE;     // '<S3>/Resettable Delay'
  real32_T last_val_out;               // '<S4>/MATLAB Function'
  real32_T last_val_in;                // '<S4>/MATLAB Function'
  real32_T time_check_input_rate_within_limit;// '<S4>/MATLAB Function'
  struct {
    uint_T last_val_out_not_empty:1;   // '<S4>/MATLAB Function'
  } bitsForTID0;

  enum_quality_factor_T UnitDelay1_DSTATE;// '<S3>/Unit Delay1'
  uint8_T icLoad;                      // '<S3>/Resettable Delay'
} DW_SpeedCompensation_T;

// Class declaration for model SpeedCompensation
class SpeedCompensationModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real32_T *rtu_raw_speed_mps, const enum_quality_factor_T
            *rtu_raw_speed_qf, const real32_T *rtu_VehicleCompFactor_KA, const
            enum_quality_factor_T *rtu_VehicleCompFactor_KA_QF, const real32_T
            *rtu_nvm_last_rem_speed_comp, const real32_T *rtu_gps_comp_factor,
            const boolean_T *rtu_f_use_gps_comp_factor, real32_T
            *rty_filt_veh_speed_over_ground, enum_quality_factor_T
            *rty_filt_veh_speed_over_ground_qf);

  // Initial conditions function
  void init();

  // Constructor
  SpeedCompensationModelClass();

  // Destructor
  ~SpeedCompensationModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_SpeedCompensation_T SpeedCompensation_DW;
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
//  '<Root>' : 'SpeedCompensation'
//  '<S1>'   : 'SpeedCompensation/SpeedCompensation'
//  '<S2>'   : 'SpeedCompensation/SpeedCompensation/GPSCompensation'
//  '<S3>'   : 'SpeedCompensation/SpeedCompensation/RadarCompensation'
//  '<S4>'   : 'SpeedCompensation/SpeedCompensation/SpeedRateLimiterFilter'
//  '<S5>'   : 'SpeedCompensation/SpeedCompensation/GPSCompensation/Compare To Constant2'
//  '<S6>'   : 'SpeedCompensation/SpeedCompensation/RadarCompensation/Compare To Constant2'
//  '<S7>'   : 'SpeedCompensation/SpeedCompensation/RadarCompensation/Compare To Constant3'
//  '<S8>'   : 'SpeedCompensation/SpeedCompensation/RadarCompensation/Compare To Constant4'
//  '<S9>'   : 'SpeedCompensation/SpeedCompensation/RadarCompensation/Compare To Constant5'
//  '<S10>'  : 'SpeedCompensation/SpeedCompensation/SpeedRateLimiterFilter/MATLAB Function'

#endif                                 // RTW_HEADER_SpeedCompensation_h_

//
// File trailer for generated code.
//
// [EOF]
//
