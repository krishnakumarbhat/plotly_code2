//
// File: TrailerDetection_types.h
//
// Code generated for Simulink model 'TrailerDetection'.
//
// Model version                  : 1.1026
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:58:13 2024
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
#ifndef RTW_HEADER_TrailerDetection_types_h_
#define RTW_HEADER_TrailerDetection_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_enum_TrackerTrailerPresence_Status_
#define DEFINED_TYPEDEF_FOR_enum_TrackerTrailerPresence_Status_

typedef uint8_T enum_TrackerTrailerPresence_Status;

// enum enum_TrackerTrailerPresence_Status
#define No_Trailer                     ((enum_TrackerTrailerPresence_Status)0U) // Default value 
#define Trailer_Present                ((enum_TrackerTrailerPresence_Status)1U)
#define Unknown                        ((enum_TrackerTrailerPresence_Status)2U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_TrailerConnection27_Status_
#define DEFINED_TYPEDEF_FOR_enum_TrailerConnection27_Status_

typedef uint8_T enum_TrailerConnection27_Status;

// enum enum_TrailerConnection27_Status
#define Not_Connected                  ((enum_TrailerConnection27_Status)0U) // Default value 
#define Connected                      ((enum_TrailerConnection27_Status)1U)
#define Not_Used                       ((enum_TrailerConnection27_Status)2U)
#define SNA2                           ((enum_TrailerConnection27_Status)3U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_ITBMTrlrStat29_Status_
#define DEFINED_TYPEDEF_FOR_enum_ITBMTrlrStat29_Status_

typedef uint8_T enum_ITBMTrlrStat29_Status;

// enum enum_ITBMTrlrStat29_Status
#define No_TRLR                        ((enum_ITBMTrlrStat29_Status)0U) // Default value 
#define TRLR_PRSNT                     ((enum_ITBMTrlrStat29_Status)1U)
#define TRLR_DCONN                     ((enum_ITBMTrlrStat29_Status)2U)
#define SNA1                           ((enum_ITBMTrlrStat29_Status)3U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_Presence_Status_
#define DEFINED_TYPEDEF_FOR_enum_Presence_Status_

typedef uint8_T enum_Presence_Status;

// enum enum_Presence_Status
#define Neither_Connected              ((enum_Presence_Status)0U) // Default value 
#define HW_Connected_Only              ((enum_Presence_Status)1U)
#define RADAR_Connected_Only           ((enum_Presence_Status)2U)
#define Both_Connected                 ((enum_Presence_Status)3U)
#define Undefined                      ((enum_Presence_Status)4U)
#endif
#endif                                 // RTW_HEADER_TrailerDetection_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
