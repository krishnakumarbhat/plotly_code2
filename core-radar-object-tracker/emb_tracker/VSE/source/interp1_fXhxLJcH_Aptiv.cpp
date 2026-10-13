//
// File: interp1_fXhxLJcH_Aptiv.cpp
//
// Code generated for Simulink model 'VehicleCurvatureAndSideslipEstimation_Simulink'.
//
// Model version                  : 1.447
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:11 2023
//
#include "interp1_fXhxLJcH_Aptiv.h"
#include "asr_s32_Aptiv.h"
#include "rtwtypes.h"

// Function for MATLAB Function: '<S3>/MATLAB Function4'
real32_T interp1_fXhxLJcH_Aptiv(const real32_T varargin_1[6], const real32_T varargin_2[6], real32_T varargin_3)
{
   real32_T Vq;
   real32_T y[6];
   real32_T x[6];
   real32_T xtmp;
   int32_T low_i;
   int32_T low_ip1;
   int32_T high_i;
   int32_T mid_i;
   for (low_i = 0; low_i < 6; low_i++)
   {
      y[low_i] = varargin_2[low_i];
      x[low_i] = varargin_1[low_i];
   }

   if (varargin_1[1] < varargin_1[0])
   {
      xtmp = x[0];
      x[0] = x[5];
      x[5] = xtmp;
      xtmp = y[0];
      y[0] = y[5];
      y[5] = xtmp;
      xtmp = x[1];
      x[1] = x[4];
      x[4] = xtmp;
      xtmp = y[1];
      y[1] = y[4];
      y[4] = xtmp;
      xtmp = x[2];
      x[2] = x[3];
      x[3] = xtmp;
      xtmp = y[2];
      y[2] = y[3];
      y[3] = xtmp;
   }

   if (varargin_3 > x[5])
   {
      Vq = (((varargin_3 - x[5]) / (x[5] - x[4])) * (y[5] - y[4])) + y[5];
   }
   else if (varargin_3 < x[0])
   {
      Vq = (((varargin_3 - x[0]) / (x[1] - x[0])) * (y[1] - y[0])) + y[0];
   }
   else
   {
      low_i   = 1;
      low_ip1 = 2;
      high_i  = 6;
      while (high_i > low_ip1)
      {
         mid_i = asr_s32_Aptiv(low_i + high_i, 1U);
         if (varargin_3 >= x[mid_i - 1])
         {
            low_i   = mid_i;
            low_ip1 = mid_i + 1;
         }
         else
         {
            high_i = mid_i;
         }
      }

      xtmp = x[low_i - 1];
      xtmp = (varargin_3 - xtmp) / (x[low_i] - xtmp);
      if (xtmp == 0.0F)
      {
         Vq = y[low_i - 1];
      }
      else if (xtmp == 1.0F)
      {
         Vq = y[low_i];
      }
      else
      {
         Vq = y[low_i - 1];
         if (Vq != y[low_i])
         {
            Vq = ((1.0F - xtmp) * Vq) + (xtmp * y[low_i]);
         }
      }
   }

   return Vq;
}

//
// File trailer for generated code.
//
// [EOF]
//
