//
// File: diag_hB9Vh4NX_Aptiv.cpp
//
// Code generated for Simulink model 'VehicleMassEstimator_2018b'.
//
// Model version                  : 1.364
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:00 2024
//
#include "rtwtypes.h"
#include <string.h>
#include "diag_hB9Vh4NX_Aptiv.h"

// Function for MATLAB Function: '<S3>/MassEstimation'
void diag_hB9Vh4NX_Aptiv(const real_T v[3], real_T d[9])
{
  memset(&d[0], 0, 9U * (sizeof(real_T)));
  d[0] = v[0];
  d[4] = v[1];
  d[8] = v[2];
}

//
// File trailer for generated code.
//
// [EOF]
//
