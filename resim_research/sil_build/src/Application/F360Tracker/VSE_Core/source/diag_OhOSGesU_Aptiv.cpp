//
// File: diag_OhOSGesU_Aptiv.cpp
//
// Code generated for Simulink model 'VehicleMassEstimator_2018b'.
//
// Model version                  : 1.364
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:00 2024
//
#include "rtwtypes.h"
#include <string.h>
#include "diag_OhOSGesU_Aptiv.h"

// Function for MATLAB Function: '<S3>/MassEstimation'
void diag_OhOSGesU_Aptiv(const real_T v[4], real_T d[16])
{
  memset(&d[0], 0, (sizeof(real_T)) << 4U);
  d[0] = v[0];
  d[5] = v[1];
  d[10] = v[2];
  d[15] = v[3];
}

//
// File trailer for generated code.
//
// [EOF]
//
