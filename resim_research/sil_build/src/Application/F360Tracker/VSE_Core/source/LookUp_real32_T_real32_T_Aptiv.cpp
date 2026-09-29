//
// File: LookUp_real32_T_real32_T_Aptiv.cpp
//
// Code generated for Simulink model 'VehicleCurvatureAndSideslipEstimation_Simulink'.
//
// Model version                  : 1.484
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:58:38 2024
//
#include "rtwtypes.h"
#include "BINARYSEARCH_real32_T_Aptiv.h"
#include "LookUp_real32_T_real32_T_Aptiv.h"

// Lookup Utility LookUp_real32_T_real32_T_Aptiv
void LookUp_real32_T_real32_T_Aptiv(real32_T *pY, const real32_T *pYData,
  real32_T u, const real32_T *pUData, uint32_T iHi)
{
  uint32_T iLeft;
  uint32_T iRght;
  BINARYSEARCH_real32_T_Aptiv( &(iLeft), &(iRght), u, pUData, iHi);

  {
    real32_T lambda;
    if (pUData[iRght] > pUData[iLeft] ) {
      real32_T num;
      real32_T den;
      den = pUData[iRght];
      den -= pUData[iLeft];
      num = u;
      num -= pUData[iLeft];
      lambda = num / den;
    } else {
      lambda = 0.0F;
    }

    {
      real32_T yLeftCast;
      real32_T yRghtCast;
      yLeftCast = pYData[iLeft];
      yRghtCast = pYData[iRght];
      yLeftCast += lambda * ( yRghtCast - yLeftCast );
      (*pY) = yLeftCast;
    }
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
