//
// File: TrailerDetection.h
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
#ifndef RTW_HEADER_TrailerDetection_h_
#define RTW_HEADER_TrailerDetection_h_
#include <string.h>
#include <stddef.h>
#ifndef TrailerDetection_COMMON_INCLUDES_
# define TrailerDetection_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // TrailerDetection_COMMON_INCLUDES_

#include "TrailerDetection_types.h"
#include <stddef.h>

// Block signals and states (default storage) for model 'TrailerDetection'
typedef struct {
  struct {
    uint_T Delay_DSTATE:1;             // '<S13>/Delay'
    uint_T Delay_DSTATE_g:1;           // '<S18>/Delay'
  } bitsForTID0;
} DW_TrailerDetection_T;

// Class declaration for model TrailerDetection
class TrailerDetectionModelClass {
  // public data and function members
 public:
  // model step function
  void step(const uint8_T *rtu_VsVCAN_TrlrConnectionSts, const uint8_T
            *rtu_VsVCAN_ITBM_TrlrStat, const boolean_T
            *rtu_VsPROXI_CANNode63_TTM, const boolean_T
            *rtu_VsPROXI_CANNode95_ITBM_ITCM, const uint8_T
            *rtu_VsTracker_TrlrDetect, const uint8_T
            *rtu_VeTracker_TrlrDetectConfLvl, const uint8_T
            *rtu_VeTracker_TrlrLenConfLvl, const uint8_T
            *rtu_VeTracker_TrlrWidthConfLvl, const real32_T
            *rtu_VsTracker_m_TrlrLen, const real32_T *rtu_VsTracker_m_TrlrWidth,
            const uint8_T *rtu_VsTracker_RadarDetectionSts, const int32_T
            *rtu_VsTracker_RadarDetectionTimer, const int32_T
            *rtu_VsTracker_StationaryTimer, const boolean_T
            *rtu_VsVSE_b_Stationary, enum_Presence_Status
            *rty_VsVSE_TrlrDetectSts, uint8_T *rty_VeVSE_TrlrDetectStsConfLvl,
            real32_T *rty_VsVSE_m_TrlrLen, uint8_T *rty_VeVSE_TrlrLenConfLvl,
            real32_T *rty_VsVSE_m_TrlrWidth, uint8_T *rty_VeVSE_TrlrWidthConfLvl,
            boolean_T *rty_VeVSE_RadarDetectionSts, real32_T
            *rty_VsVSE_s_RadarDetectionTimer, real32_T
            *rty_VsVSE_s_StationaryTimer);

  // Constructor
  TrailerDetectionModelClass();

  // Destructor
  ~TrailerDetectionModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW_TrailerDetection_T TrailerDetection_DW;
};

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
//  '<Root>' : 'TrailerDetection'
//  '<S1>'   : 'TrailerDetection/TrailerDetectionUnit'
//  '<S2>'   : 'TrailerDetection/TrailerDetectionUnit/MATLAB Function1'
//  '<S3>'   : 'TrailerDetection/TrailerDetectionUnit/MATLAB Function7'
//  '<S4>'   : 'TrailerDetection/TrailerDetectionUnit/MATLAB Function8'
//  '<S5>'   : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status'
//  '<S6>'   : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status'
//  '<S7>'   : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Subsystem'
//  '<S8>'   : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Trailer_HW_Connection_Status'
//  '<S9>'   : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Subsystem/If Action Subsystem2'
//  '<S10>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Subsystem/If Action Subsystem3'
//  '<S11>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Subsystem/If Action Subsystem4'
//  '<S12>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Subsystem/No trailer present'
//  '<S13>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Subsystem/Subsystem'
//  '<S14>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Subsystem/trailer present'
//  '<S15>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Trailer_HW_Connection_Status/Connected'
//  '<S16>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Trailer_HW_Connection_Status/Present'
//  '<S17>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Trailer_HW_Connection_Status/Present To Constant5'
//  '<S18>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Trailer_HW_Connection_Status/StationaryStateHoldLogic'
//  '<S19>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Connection_Status/Trailer_HW_Connection_Status/TRAR_PRSNT'
//  '<S20>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/Compare'
//  '<S21>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/Compare1'
//  '<S22>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/Compare2'
//  '<S23>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/Compare3'
//  '<S24>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/Compare4'
//  '<S25>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/Compare5'
//  '<S26>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/If Action Subsystem'
//  '<S27>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/If Action Subsystem1'
//  '<S28>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/If Action Subsystem2'
//  '<S29>'  : 'TrailerDetection/TrailerDetectionUnit/Trailer_Presence_Status/If Action Subsystem3'

#endif                                 // RTW_HEADER_TrailerDetection_h_

//
// File trailer for generated code.
//
// [EOF]
//
