//
// File: VehicleMassEstimator_2018b_types.h
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
#ifndef RTW_HEADER_VehicleMassEstimator_2018b_types_h_
#define RTW_HEADER_VehicleMassEstimator_2018b_types_h_
#include "rtwtypes.h"
#ifndef struct_md70bfa7194d76bd242e324e642d8cde5
#define struct_md70bfa7194d76bd242e324e642d8cde5

struct md70bfa7194d76bd242e324e642d8cde5
{
  int32_T S0_isInitialized;
  real_T W0_ZERO_STATES[10];
  real_T W1_POLE_STATES[10];
  int32_T W2_PreviousNumChannels;
  real_T P0_ICRTP;
  real_T P1_RTP1COEFF[15];
  real_T P2_RTP2COEFF[10];
  real_T P3_RTP3COEFF[6];
  boolean_T P4_RTP_COEFF3_BOOL[6];
  real_T P5_IC2RTP;
};

#endif                                 //struct_md70bfa7194d76bd242e324e642d8cde5

#ifndef typedef_dsp_BiquadFilter_0_VehicleMassEstimator_2018b_T
#define typedef_dsp_BiquadFilter_0_VehicleMassEstimator_2018b_T

typedef struct md70bfa7194d76bd242e324e642d8cde5
  dsp_BiquadFilter_0_VehicleMassEstimator_2018b_T;

#endif                                 //typedef_dsp_BiquadFilter_0_VehicleMassEstimator_2018b_T

#ifndef struct_md0L5ze8JYTAeut2dDnCC2EG
#define struct_md0L5ze8JYTAeut2dDnCC2EG

struct md0L5ze8JYTAeut2dDnCC2EG
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  dsp_BiquadFilter_0_VehicleMassEstimator_2018b_T cSFunObject;
};

#endif                                 //struct_md0L5ze8JYTAeut2dDnCC2EG

#ifndef typedef_dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T
#define typedef_dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T

typedef struct md0L5ze8JYTAeut2dDnCC2EG
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T;

#endif                                 //typedef_dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T

#ifndef typedef_cell_wrap_VehicleMassEstimator_2018b_T
#define typedef_cell_wrap_VehicleMassEstimator_2018b_T

typedef struct {
  uint32_T f1[8];
} cell_wrap_VehicleMassEstimator_2018b_T;

#endif                                 //typedef_cell_wrap_VehicleMassEstimator_2018b_T

#ifndef struct_mddU3Hemon2fJG2VxxOyVddC
#define struct_mddU3Hemon2fJG2VxxOyVddC

struct mddU3Hemon2fJG2VxxOyVddC
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  cell_wrap_VehicleMassEstimator_2018b_T inputVarSize;
  int32_T NumChannels;
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *FilterObj;
};

#endif                                 //struct_mddU3Hemon2fJG2VxxOyVddC

#ifndef typedef_dsp_HighpassFilter_VehicleMassEstimator_2018b_T
#define typedef_dsp_HighpassFilter_VehicleMassEstimator_2018b_T

typedef struct mddU3Hemon2fJG2VxxOyVddC
  dsp_HighpassFilter_VehicleMassEstimator_2018b_T;

#endif                                 //typedef_dsp_HighpassFilter_VehicleMassEstimator_2018b_T

#ifndef struct_md8d9b4f3bb7147d3026472b7611b75084
#define struct_md8d9b4f3bb7147d3026472b7611b75084

struct md8d9b4f3bb7147d3026472b7611b75084
{
  int32_T S0_isInitialized;
  real_T W0_ZERO_STATES[2];
  real_T W1_POLE_STATES[2];
  int32_T W2_PreviousNumChannels;
  real_T P0_ICRTP;
  real_T P1_RTP1COEFF[3];
  real_T P2_RTP2COEFF[2];
  real_T P3_RTP3COEFF[2];
  boolean_T P4_RTP_COEFF3_BOOL[2];
  real_T P5_IC2RTP;
};

#endif                                 //struct_md8d9b4f3bb7147d3026472b7611b75084

#ifndef typedef_dsp_BiquadFilter_0_VehicleMassEstimator_2018b_e_T
#define typedef_dsp_BiquadFilter_0_VehicleMassEstimator_2018b_e_T

typedef struct md8d9b4f3bb7147d3026472b7611b75084
  dsp_BiquadFilter_0_VehicleMassEstimator_2018b_e_T;

#endif                                 //typedef_dsp_BiquadFilter_0_VehicleMassEstimator_2018b_e_T

#ifndef struct_mdb7XKqlqllZZCxzFEPdCRFH
#define struct_mdb7XKqlqllZZCxzFEPdCRFH

struct mdb7XKqlqllZZCxzFEPdCRFH
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  dsp_BiquadFilter_0_VehicleMassEstimator_2018b_e_T cSFunObject;
};

#endif                                 //struct_mdb7XKqlqllZZCxzFEPdCRFH

#ifndef typedef_dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T
#define typedef_dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T

typedef struct mdb7XKqlqllZZCxzFEPdCRFH
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T;

#endif                                 //typedef_dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T

#ifndef struct_md768Zxq8Ejg3dWku7GR26HG
#define struct_md768Zxq8Ejg3dWku7GR26HG

struct md768Zxq8Ejg3dWku7GR26HG
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  cell_wrap_VehicleMassEstimator_2018b_T inputVarSize;
  int32_T NumChannels;
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *FilterObj;
};

#endif                                 //struct_md768Zxq8Ejg3dWku7GR26HG

#ifndef typedef_dsp_LowpassFilter_VehicleMassEstimator_2018b_T
#define typedef_dsp_LowpassFilter_VehicleMassEstimator_2018b_T

typedef struct md768Zxq8Ejg3dWku7GR26HG
  dsp_LowpassFilter_VehicleMassEstimator_2018b_T;

#endif                                 //typedef_dsp_LowpassFilter_VehicleMassEstimator_2018b_T
#endif                                 // RTW_HEADER_VehicleMassEstimator_2018b_types_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
