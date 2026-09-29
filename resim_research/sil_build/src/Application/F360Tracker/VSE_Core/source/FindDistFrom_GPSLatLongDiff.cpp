//
// File: FindDistFrom_GPSLatLongDiff.cpp
//
// Code generated for Simulink model 'FindDistFrom_GPSLatLongDiff'.
//
// Model version                  : 1.87
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:13 2024
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
#include "FindDistFrom_GPSLatLongDiff.h"
#include "FindDistFrom_GPSLatLongDiff_private.h"

// Output and update for referenced model: 'FindDistFrom_GPSLatLongDiff'
void FindDistFrom_GPSLatLongDiffModelClass::step(const real_T *rtu_GPS_Lat_1_mas,
  const real_T *rtu_GPS_Long_1_mas, const real_T *rtu_GPS_Lat_2_mas, const
  real_T *rtu_GPS_Long_2_mas, const enum_quality_factor_T *rtu_GPS_Point1_QF,
  const enum_quality_factor_T *rtu_GPS_Point2_QF, real32_T *rty_dist_along_earth,
  enum_quality_factor_T *rty_dist_along_earth_qf)
{
  real_T x;
  real_T b_x;
  real_T rtb_Gain1;
  real_T rtb_Gain1_h;
  boolean_T rtb_f_correct_distance;

  // Gain: '<S4>/Gain1' incorporates:
  //   Gain: '<S1>/MilliArcSeconds_to_Degrees'

  rtb_Gain1 = (2.7777777777777776E-7 * (*rtu_GPS_Lat_1_mas)) *
    0.017453292519943295;

  // Gain: '<S6>/Gain1' incorporates:
  //   Gain: '<S1>/MilliArcSeconds_to_Degrees2'

  rtb_Gain1_h = (2.7777777777777776E-7 * (*rtu_GPS_Lat_2_mas)) *
    0.017453292519943295;

  // MATLAB Function: '<S1>/MATLAB Function' incorporates:
  //   Gain: '<S1>/MilliArcSeconds_to_Degrees1'
  //   Gain: '<S1>/MilliArcSeconds_to_Degrees3'
  //   Gain: '<S5>/Gain1'
  //   Gain: '<S7>/Gain1'

  x = sin((rtb_Gain1 - rtb_Gain1_h) / 2.0);
  b_x = sin((((2.7777777777777776E-7 * (*rtu_GPS_Long_1_mas)) *
              0.017453292519943295) - ((2.7777777777777776E-7 *
    (*rtu_GPS_Long_2_mas)) * 0.017453292519943295)) / 2.0);
  rtb_Gain1 = ((cos(rtb_Gain1) * cos(rtb_Gain1_h)) * (b_x * b_x)) + (x * x);
  if ((rtb_Gain1 >= 0.0) && (rtb_Gain1 <= 1.0)) {
    *rty_dist_along_earth = (real32_T)((real_T)(1.27562E+7 * asin(sqrt(rtb_Gain1))));
    rtb_f_correct_distance = true;
  } else {
    *rty_dist_along_earth = 0.0F;
    rtb_f_correct_distance = false;
  }

  // End of MATLAB Function: '<S1>/MATLAB Function'

  // Switch: '<S1>/Switch' incorporates:
  //   Constant: '<S1>/Constant'
  //   Constant: '<S1>/Constant1'
  //   Constant: '<S2>/Constant'
  //   Constant: '<S3>/Constant'
  //   Logic: '<S1>/Logical Operator'
  //   RelationalOperator: '<S2>/Compare'
  //   RelationalOperator: '<S3>/Compare'

  if (((((uint32_T)(*rtu_GPS_Point1_QF)) == ACCURATED) && (((uint32_T)
         (*rtu_GPS_Point2_QF)) == ACCURATED)) && rtb_f_correct_distance) {
    *rty_dist_along_earth_qf = ACCURATED;
  } else {
    *rty_dist_along_earth_qf = UNDEFINED;
  }

  // End of Switch: '<S1>/Switch'
}

// Constructor
FindDistFrom_GPSLatLongDiffModelClass::FindDistFrom_GPSLatLongDiffModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
FindDistFrom_GPSLatLongDiffModelClass::~FindDistFrom_GPSLatLongDiffModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
