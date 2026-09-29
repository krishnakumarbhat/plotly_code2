//
// File: SpeedCompensationArbitration.cpp
//
// Code generated for Simulink model 'SpeedCompensationArbitration'.
//
// Model version                  : 1.77
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:57:40 2024
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
#include "SpeedCompensationArbitration.h"
#include "SpeedCompensationArbitration_private.h"

// Output and update for referenced model: 'SpeedCompensationArbitration'
void SpeedCompensationArbitrationModelClass::step(const real32_T
  *rtu_VsTracker_VehSpdCompFac_1, const enum_quality_factor_T
  *rtu_VeTracker_VehSpdCompFac_1_QF, const real32_T
  *rtu_VsTracker_VehSpdCompFac_2, const enum_quality_factor_T
  *rtu_VeTracker_VehSpdCompFac_2_QF, const real32_T
  *rtu_VsTracker_VehSpdCompFac_3, const enum_quality_factor_T
  *rtu_VeTracker_VehSpdCompFac_3_QF, const real32_T
  *rtu_VsTracker_VehSpdCompFac_4, const enum_quality_factor_T
  *rtu_VeTracker_VehSpdCompFac_4_QF, const real32_T
  *rtu_VsTracker_VehSpdCompFac_5, const enum_quality_factor_T
  *rtu_VeTracker_VehSpdCompFac_5_QF, real32_T *rty_VsTracker_VehSpdCompFac,
  enum_quality_factor_T *rty_VeTracker_VehSpdCompFac_QF)
{
  int8_T rtb_TempFac1;
  int8_T rtb_TempFac;
  int8_T rtb_TempFac3;
  int8_T rtb_TempFac4;
  int8_T rtb_TempFac5;
  int8_T rtb_SumofElements1;
  real32_T rtu_VsTracker_VehSpdCompFac_1_0;
  real32_T rtu_VsTracker_VehSpdCompFac_2_0;
  real32_T rtu_VsTracker_VehSpdCompFac_3_0;
  real32_T rtu_VsTracker_VehSpdCompFac_4_0;
  real32_T rtu_VsTracker_VehSpdCompFac_5_0;

  // Outputs for Atomic SubSystem: '<Root>/SpeedCompensationArbitration'
  // DataTypeConversion: '<S1>/Data Type Conversion' incorporates:
  //   Constant: '<S1>/Constant3'
  //   RelationalOperator: '<S1>/Relational Operator'

  rtb_TempFac1 = (int8_T)((((uint32_T)(*rtu_VeTracker_VehSpdCompFac_1_QF)) ==
    ACCURATED) ? 1 : 0);

  // DataTypeConversion: '<S1>/Data Type Conversion1' incorporates:
  //   Constant: '<S1>/Constant6'
  //   RelationalOperator: '<S1>/Relational Operator1'

  rtb_TempFac = (int8_T)((((uint32_T)(*rtu_VeTracker_VehSpdCompFac_2_QF)) ==
    ACCURATED) ? 1 : 0);

  // DataTypeConversion: '<S1>/Data Type Conversion2' incorporates:
  //   Constant: '<S1>/Constant9'
  //   RelationalOperator: '<S1>/Relational Operator2'

  rtb_TempFac3 = (int8_T)((((uint32_T)(*rtu_VeTracker_VehSpdCompFac_3_QF)) ==
    ACCURATED) ? 1 : 0);

  // DataTypeConversion: '<S1>/Data Type Conversion3' incorporates:
  //   Constant: '<S1>/Constant14'
  //   RelationalOperator: '<S1>/Relational Operator3'

  rtb_TempFac4 = (int8_T)((((uint32_T)(*rtu_VeTracker_VehSpdCompFac_4_QF)) ==
    ACCURATED) ? 1 : 0);

  // DataTypeConversion: '<S1>/Data Type Conversion4' incorporates:
  //   Constant: '<S1>/Constant18'
  //   RelationalOperator: '<S1>/Relational Operator4'

  rtb_TempFac5 = (int8_T)((((uint32_T)(*rtu_VeTracker_VehSpdCompFac_5_QF)) ==
    ACCURATED) ? 1 : 0);

  // Sum: '<S1>/Sum of Elements1'
  rtb_SumofElements1 = (int8_T)((((rtb_TempFac1 + rtb_TempFac) + rtb_TempFac3) +
    rtb_TempFac4) + rtb_TempFac5);

  // Switch: '<S1>/Switch3' incorporates:
  //   Constant: '<S1>/Constant5'
  //   RelationalOperator: '<S1>/Relational Operator8'

  if (rtb_SumofElements1 != 0) {
    // Switch: '<S1>/Switch1' incorporates:
    //   Constant: '<S1>/Constant17'

    *rty_VeTracker_VehSpdCompFac_QF = ACCURATED;

    // Saturate: '<S1>/Saturation1' incorporates:
    //   Switch: '<S1>/Switch5'

    if ((*rtu_VsTracker_VehSpdCompFac_1) > 1.05F) {
      rtu_VsTracker_VehSpdCompFac_1_0 = 1.05F;
    } else if ((*rtu_VsTracker_VehSpdCompFac_1) < 0.95F) {
      rtu_VsTracker_VehSpdCompFac_1_0 = 0.95F;
    } else {
      rtu_VsTracker_VehSpdCompFac_1_0 = *rtu_VsTracker_VehSpdCompFac_1;
    }

    // End of Saturate: '<S1>/Saturation1'

    // Saturate: '<S1>/Saturation2' incorporates:
    //   Switch: '<S1>/Switch5'

    if ((*rtu_VsTracker_VehSpdCompFac_2) > 1.05F) {
      rtu_VsTracker_VehSpdCompFac_2_0 = 1.05F;
    } else if ((*rtu_VsTracker_VehSpdCompFac_2) < 0.95F) {
      rtu_VsTracker_VehSpdCompFac_2_0 = 0.95F;
    } else {
      rtu_VsTracker_VehSpdCompFac_2_0 = *rtu_VsTracker_VehSpdCompFac_2;
    }

    // End of Saturate: '<S1>/Saturation2'

    // Saturate: '<S1>/Saturation3' incorporates:
    //   Switch: '<S1>/Switch5'

    if ((*rtu_VsTracker_VehSpdCompFac_3) > 1.05F) {
      rtu_VsTracker_VehSpdCompFac_3_0 = 1.05F;
    } else if ((*rtu_VsTracker_VehSpdCompFac_3) < 0.95F) {
      rtu_VsTracker_VehSpdCompFac_3_0 = 0.95F;
    } else {
      rtu_VsTracker_VehSpdCompFac_3_0 = *rtu_VsTracker_VehSpdCompFac_3;
    }

    // End of Saturate: '<S1>/Saturation3'

    // Saturate: '<S1>/Saturation4' incorporates:
    //   Switch: '<S1>/Switch5'

    if ((*rtu_VsTracker_VehSpdCompFac_4) > 1.05F) {
      rtu_VsTracker_VehSpdCompFac_4_0 = 1.05F;
    } else if ((*rtu_VsTracker_VehSpdCompFac_4) < 0.95F) {
      rtu_VsTracker_VehSpdCompFac_4_0 = 0.95F;
    } else {
      rtu_VsTracker_VehSpdCompFac_4_0 = *rtu_VsTracker_VehSpdCompFac_4;
    }

    // End of Saturate: '<S1>/Saturation4'

    // Saturate: '<S1>/Saturation5' incorporates:
    //   Switch: '<S1>/Switch5'

    if ((*rtu_VsTracker_VehSpdCompFac_5) > 1.05F) {
      rtu_VsTracker_VehSpdCompFac_5_0 = 1.05F;
    } else if ((*rtu_VsTracker_VehSpdCompFac_5) < 0.95F) {
      rtu_VsTracker_VehSpdCompFac_5_0 = 0.95F;
    } else {
      rtu_VsTracker_VehSpdCompFac_5_0 = *rtu_VsTracker_VehSpdCompFac_5;
    }

    // End of Saturate: '<S1>/Saturation5'

    // Switch: '<S1>/Switch5' incorporates:
    //   Product: '<S1>/Divide'
    //   Product: '<S1>/Multiply'
    //   Product: '<S1>/Multiply1'
    //   Product: '<S1>/Multiply2'
    //   Product: '<S1>/Multiply3'
    //   Product: '<S1>/Multiply4'
    //   Sum: '<S1>/Sum of Elements'

    *rty_VsTracker_VehSpdCompFac = (((((rtu_VsTracker_VehSpdCompFac_1_0 *
      ((real32_T)rtb_TempFac1)) + (rtu_VsTracker_VehSpdCompFac_2_0 * ((real32_T)
      rtb_TempFac))) + (rtu_VsTracker_VehSpdCompFac_3_0 * ((real32_T)
      rtb_TempFac3))) + (rtu_VsTracker_VehSpdCompFac_4_0 * ((real32_T)
      rtb_TempFac4))) + (rtu_VsTracker_VehSpdCompFac_5_0 * ((real32_T)
      rtb_TempFac5))) / ((real32_T)rtb_SumofElements1);
  } else {
    // Switch: '<S1>/Switch1' incorporates:
    //   Constant: '<S1>/Constant2'

    *rty_VeTracker_VehSpdCompFac_QF = NOT_ACCURATED;

    // Switch: '<S1>/Switch5' incorporates:
    //   Constant: '<S1>/DefaultSpeedCompensation'

    *rty_VsTracker_VehSpdCompFac = 1.0F;
  }

  // End of Switch: '<S1>/Switch3'
  // End of Outputs for SubSystem: '<Root>/SpeedCompensationArbitration'
}

// Constructor
SpeedCompensationArbitrationModelClass::SpeedCompensationArbitrationModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
SpeedCompensationArbitrationModelClass::~SpeedCompensationArbitrationModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
