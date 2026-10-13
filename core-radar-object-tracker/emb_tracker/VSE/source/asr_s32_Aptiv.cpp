//
// File: asr_s32_Aptiv.cpp
//
// Code generated for Simulink model 'VehicleCurvatureAndSideslipEstimation_Simulink'.
//
// Model version                  : 1.447
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:11 2023
//
#include "asr_s32_Aptiv.h"
#include "rtwtypes.h"

int32_T asr_s32_Aptiv(int32_T u, uint32_T n)
{
   int32_T y;
   if (u >= 0)
   {
      y = (int32_T)((uint32_T)(((uint32_T)u) >> n));
   }
   else
   {
      y = (-((int32_T)((uint32_T)(((uint32_T)((int32_T)(-1 - u))) >> n)))) - 1;
   }

   return y;
}

//
// File trailer for generated code.
//
// [EOF]
//
