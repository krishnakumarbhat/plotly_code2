//
// File: VSE_Master_Model_L2_private.h
//
// Code generated for Simulink model 'VSE_Master_Model_L2'.
//
// Model version                  : 1.794
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 19:00:41 2024
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
#ifndef RTW_HEADER_VSE_Master_Model_L2_private_h_
#define RTW_HEADER_VSE_Master_Model_L2_private_h_
#include "rtwtypes.h"
#include "VSE_Master_Model_L2.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

// Skipping ulong/long check: insufficient preprocessor integer range.

// Skipping ulong_long/long_long check: insufficient preprocessor integer range. 
extern void VSE_Master_Model_L2_Initialization(boolean_T rtu_Enable, real32_T
  rty_diff_num[4], real32_T rty_diff_den[4]);
extern void VSE_Master_Model_L2_Initialization_Trigger(boolean_T
  *rty_f_initialize, DW_Initialization_Trigger_VSE_Master_Model_L2_T *localDW);
extern void VSE_Master_Model_L2_MATLABFunction(boolean_T rtu_fault_flag,
  real32_T rtu_fault_maturation_time_seconds, real32_T
  rtu_fault_dematuration_time_seconds, boolean_T *rty_final_fault,
  DW_MATLABFunction_VSE_Master_Model_L2_T *localDW);

#endif                                 // RTW_HEADER_VSE_Master_Model_L2_private_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
