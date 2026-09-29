//
// File: FindDistFrom_GPSLatLongDiff.h
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
#ifndef RTW_HEADER_FindDistFrom_GPSLatLongDiff_h_
#define RTW_HEADER_FindDistFrom_GPSLatLongDiff_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef FindDistFrom_GPSLatLongDiff_COMMON_INCLUDES_
# define FindDistFrom_GPSLatLongDiff_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // FindDistFrom_GPSLatLongDiff_COMMON_INCLUDES_ 

#include "FindDistFrom_GPSLatLongDiff_types.h"
#include <stddef.h>

// Class declaration for model FindDistFrom_GPSLatLongDiff
class FindDistFrom_GPSLatLongDiffModelClass {
  // public data and function members
 public:
  // model step function
  void step(const real_T *rtu_GPS_Lat_1_mas, const real_T *rtu_GPS_Long_1_mas,
            const real_T *rtu_GPS_Lat_2_mas, const real_T *rtu_GPS_Long_2_mas,
            const enum_quality_factor_T *rtu_GPS_Point1_QF, const
            enum_quality_factor_T *rtu_GPS_Point2_QF, real32_T
            *rty_dist_along_earth, enum_quality_factor_T
            *rty_dist_along_earth_qf);

  // Constructor
  FindDistFrom_GPSLatLongDiffModelClass();

  // Destructor
  ~FindDistFrom_GPSLatLongDiffModelClass();

  // private data and function members
 private:
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Scope' : Unused code path elimination


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
//  '<Root>' : 'FindDistFrom_GPSLatLongDiff'
//  '<S1>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff'
//  '<S2>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff/Compare To Constant'
//  '<S3>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff/Compare To Constant1'
//  '<S4>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff/Degrees to Radians'
//  '<S5>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff/Degrees to Radians1'
//  '<S6>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff/Degrees to Radians2'
//  '<S7>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff/Degrees to Radians3'
//  '<S8>'   : 'FindDistFrom_GPSLatLongDiff/FindDistFrom_GPSLatLongDiff/MATLAB Function'

#endif                                 // RTW_HEADER_FindDistFrom_GPSLatLongDiff_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
