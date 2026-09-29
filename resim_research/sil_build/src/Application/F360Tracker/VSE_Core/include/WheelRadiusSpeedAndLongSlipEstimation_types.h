//
// File: WheelRadiusSpeedAndLongSlipEstimation_types.h
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
#ifndef RTW_HEADER_WheelRadiusSpeedAndLongSlipEstimation_types_h_
#define RTW_HEADER_WheelRadiusSpeedAndLongSlipEstimation_types_h_
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

#ifndef DEFINED_TYPEDEF_FOR_TireRadius_Info_
#define DEFINED_TYPEDEF_FOR_TireRadius_Info_

typedef struct {
  // Radius estimated using GPS
  real32_T gps_est_rad;

  // Flag to indicate if GPS estimated radius has converged
  boolean_T f_gps_est_rad_conv;

  // Radius Estimated using Radar Compensated Speed
  real32_T radar_est_rad;

  // Flag to indiciate if Radius Estimated using Radar Compensated Speed has converged 
  boolean_T f_radar_est_rad_conv;

  // Default radius of tire
  real32_T default_rad;
} TireRadius_Info;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tire_radius_estimation_mode_T_
#define DEFINED_TYPEDEF_FOR_tire_radius_estimation_mode_T_

typedef uint8_T tire_radius_estimation_mode_T;

// enum tire_radius_estimation_mode_T
#define DEFAULT_RADIUS                 ((tire_radius_estimation_mode_T)0U) // Default value 
#define GPS_BASED_RADIUS               ((tire_radius_estimation_mode_T)1U)
#define RADAR_BASED_RADIUS             ((tire_radius_estimation_mode_T)2U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_TireRadEst_Constants_
#define DEFINED_TYPEDEF_FOR_TireRadEst_Constants_

typedef struct {
  real32_T k_raw_rad_tolerance;
  real32_T k_est_rad_tolerance;
  real32_T k_raw_rad_mature_time;
  real32_T k_est_rad_mature_time;
  real32_T k_comp_factor_min;
  real32_T k_comp_factor_max;
} TireRadEst_Constants;

#endif

#ifndef struct_mdb77c711c6299a461b6beffa6be5a0cca
#define struct_mdb77c711c6299a461b6beffa6be5a0cca

struct mdb77c711c6299a461b6beffa6be5a0cca
{
  int32_T S0_isInitialized;
  real32_T W0_ZERO_STATES[4];
  real32_T W1_POLE_STATES[4];
  int32_T W2_PreviousNumChannels;
  real32_T P0_ICRTP;
  real32_T P1_RTP1COEFF[6];
  real32_T P2_RTP2COEFF[4];
  real32_T P3_RTP3COEFF[3];
  boolean_T P4_RTP_COEFF3_BOOL[3];
  real32_T P5_IC2RTP;
};

#endif                                 //struct_mdb77c711c6299a461b6beffa6be5a0cca

#ifndef typedef_dsp_BiquadFilter_0_WheelRadiusSpeedAndLongSlipEstimation_T
#define typedef_dsp_BiquadFilter_0_WheelRadiusSpeedAndLongSlipEstimation_T

typedef struct mdb77c711c6299a461b6beffa6be5a0cca
  dsp_BiquadFilter_0_WheelRadiusSpeedAndLongSlipEstimation_T;

#endif                                 //typedef_dsp_BiquadFilter_0_WheelRadiusSpeedAndLongSlipEstimation_T

#ifndef struct_mdyECKyHbRISTQsHS23k5KnD
#define struct_mdyECKyHbRISTQsHS23k5KnD

struct mdyECKyHbRISTQsHS23k5KnD
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  dsp_BiquadFilter_0_WheelRadiusSpeedAndLongSlipEstimation_T cSFunObject;
};

#endif                                 //struct_mdyECKyHbRISTQsHS23k5KnD

#ifndef typedef_dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T
#define typedef_dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T

typedef struct mdyECKyHbRISTQsHS23k5KnD
  dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T;

#endif                                 //typedef_dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T

#ifndef typedef_cell_wrap_WheelRadiusSpeedAndLongSlipEstimation_T
#define typedef_cell_wrap_WheelRadiusSpeedAndLongSlipEstimation_T

typedef struct {
  uint32_T f1[8];
} cell_wrap_WheelRadiusSpeedAndLongSlipEstimation_T;

#endif                                 //typedef_cell_wrap_WheelRadiusSpeedAndLongSlipEstimation_T

#ifndef struct_mdETnhbX6VG1gUW5HVZDCjmH
#define struct_mdETnhbX6VG1gUW5HVZDCjmH

struct mdETnhbX6VG1gUW5HVZDCjmH
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  cell_wrap_WheelRadiusSpeedAndLongSlipEstimation_T inputVarSize;
  int32_T NumChannels;
  dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *FilterObj;
};

#endif                                 //struct_mdETnhbX6VG1gUW5HVZDCjmH

#ifndef typedef_dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T
#define typedef_dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T

typedef struct mdETnhbX6VG1gUW5HVZDCjmH
  dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T;

#endif                                 //typedef_dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T
#endif                                 // RTW_HEADER_WheelRadiusSpeedAndLongSlipEstimation_types_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
