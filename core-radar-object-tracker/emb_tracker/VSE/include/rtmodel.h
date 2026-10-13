//
// File: rtmodel.h
//
// Code generated for Simulink model 'BMW_VSE_Master_Model'.
//
// Model version                  : 1.495
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:40 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 64-bit (LLP64)
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
#ifndef RTW_HEADER_rtmodel_h_
   #define RTW_HEADER_rtmodel_h_
   #include "BMW_VSE_Master_Model.h"

//
//  ROOT_IO_FORMAT: 0 (Individual arguments)
//  ROOT_IO_FORMAT: 1 (Structure reference)
//  ROOT_IO_FORMAT: 2 (Part of model data structure)

   #define ROOT_IO_FORMAT 1

   // Macros generated for backwards compatibility
   #ifndef rtmGetErrorStatus
      #define rtmGetErrorStatus(rtm) ((void *)0)
   #endif

   #ifndef rtmSetErrorStatus
      #define rtmSetErrorStatus(rtm, val) ((void)0)
   #endif

   #ifndef rtmGetStopRequested
      #define rtmGetStopRequested(rtm) ((void *)0)
   #endif

   #define MODEL_CLASSNAME BMW_VSE_Master_ModelModelClass
   #define MODEL_STEPNAME  step
#endif // RTW_HEADER_rtmodel_h_

//
// File trailer for generated code.
//
// [EOF]
//
