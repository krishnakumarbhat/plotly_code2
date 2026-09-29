//
// File: WheelRadiusSpeedAndLongSlipEstimation.cpp
//
// Code generated for Simulink model 'WheelRadiusSpeedAndLongSlipEstimation'.
//
// Model version                  : 1.480
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:57 2024
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
#include "WheelRadiusSpeedAndLongSlipEstimation.h"
#include "WheelRadiusSpeedAndLongSlipEstimation_private.h"

// Named constants for MATLAB Function: '<S10>/AccumulateAngleFromFrontLeftWheelSpeed' 
#define WheelRadiusSpeedAndLongSlipEstimation_wheel_rad_est_sampling_time (0.01F)

// Named constants for Chart: '<S1>/Chart'
#define WheelRadiusSpeedAndLongSlipEstimation_IN_Initialize ((uint8_T)1U)
#define WheelRadiusSpeedAndLongSlipEstimation_IN_Running ((uint8_T)2U)

//
// Output and update for atomic system:
//    '<S10>/AccumulateAngleFromFrontLeftWheelSpeed'
//    '<S11>/AccumulateAngleFromFrontRightWheelSpeed'
//    '<S12>/AccumulateAngleFromRearLeftWheelSpeed'
//    '<S13>/AccumulateAngleFromRearRightWheelSpeed'
//
void
  WheelRadiusSpeedAndLongSlipEstimation_AccumulateAngleFromFrontLeftWheelSpeed
  (real32_T rtu_wheel_speed_radpsec, boolean_T rtu_f_low_acceleration, boolean_T
   rtu_f_low_curvature, real32_T *rty_accumulated_angle,
   DW_AccumulateAngleFromFrontLeftWheelSpeed_WheelRadiusSpeedAndLongSlipEstimation_T
   *localDW)
{
  if (rtu_f_low_acceleration && rtu_f_low_curvature) {
    localDW->accum_angle += rtu_wheel_speed_radpsec *
      WheelRadiusSpeedAndLongSlipEstimation_wheel_rad_est_sampling_time;
  } else {
    localDW->accum_angle = 0.0F;
  }

  *rty_accumulated_angle = localDW->accum_angle;
}

//
// System initialize for atomic system:
//    '<S42>/FindTireRadius'
//    '<S43>/FindTireRadius'
//    '<S44>/FindTireRadius'
//    '<S45>/FindTireRadius'
//
void WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_Init
  (DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_T *localDW)
{
  localDW->bitsForTID0.f_est_rad_conv_time_start = true;
}

//
// Output and update for atomic system:
//    '<S42>/FindTireRadius'
//    '<S43>/FindTireRadius'
//    '<S44>/FindTireRadius'
//    '<S45>/FindTireRadius'
//
void WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius(real32_T
  rtu_gps_accumumulated_dist, real32_T rtu_raw_accumulated_dist, const boolean_T
  *rtu_f_new_GPS_data, real32_T rtu_accumulated_angle, const
  TireRadEst_Constants *rtu_TireRadEst_Constants, real32_T *rty_raw_rad_conv,
  real32_T *rty_est_rad_conv, boolean_T *rty_f_raw_rad_conv_out, boolean_T
  *rty_f_est_rad_conv_out,
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_T *localDW)
{
  real32_T temp_estimated_tire_radius;
  real32_T temp_raw_tire_radius;
  if (!localDW->bitsForTID0.curr_time_not_empty) {
    localDW->curr_time = 0.0F;
    localDW->bitsForTID0.curr_time_not_empty = true;
  } else {
    localDW->curr_time += 0.01F;
  }

  if (((*rtu_f_new_GPS_data) && (rtu_gps_accumumulated_dist > 0.0F)) &&
      (rtu_accumulated_angle > 0.0F)) {
    temp_estimated_tire_radius = rtu_gps_accumumulated_dist /
      rtu_accumulated_angle;
    localDW->last_estimated_tire_radius = temp_estimated_tire_radius;
  } else {
    temp_estimated_tire_radius = 0.0F;
  }

  if ((rtu_raw_accumulated_dist > 0.0F) && (rtu_accumulated_angle > 0.0F)) {
    temp_raw_tire_radius = rtu_raw_accumulated_dist / rtu_accumulated_angle;
    localDW->last_raw_tire_radius = temp_raw_tire_radius;
  } else {
    temp_raw_tire_radius = 0.0F;
  }

  if (!localDW->bitsForTID0.f_raw_rad_converged_temp) {
    if (((localDW->last_raw_tire_radius > 0.0F) && (fabsf
          (localDW->running_avg_raw_tire_radius - localDW->last_raw_tire_radius)
          < rtu_TireRadEst_Constants->k_raw_rad_tolerance)) &&
        (!localDW->bitsForTID0.f_raw_rad_conv_time_start)) {
      localDW->raw_rad_conv_time_start = localDW->curr_time;
      localDW->bitsForTID0.f_raw_rad_conv_time_start = true;
    } else {
      if ((localDW->bitsForTID0.f_raw_rad_conv_time_start) &&
          ((localDW->last_raw_tire_radius <= 0.0F) || (fabsf
            (localDW->running_avg_raw_tire_radius -
             localDW->last_raw_tire_radius) >=
            rtu_TireRadEst_Constants->k_raw_rad_tolerance))) {
        localDW->raw_rad_conv_time_start = 0.0F;
        localDW->bitsForTID0.f_raw_rad_conv_time_start = false;
      }
    }

    if ((localDW->bitsForTID0.f_raw_rad_conv_time_start) && ((localDW->curr_time
          - localDW->raw_rad_conv_time_start) >=
         rtu_TireRadEst_Constants->k_raw_rad_mature_time)) {
      localDW->bitsForTID0.f_raw_rad_converged_temp = true;
    }

    if ((temp_raw_tire_radius > 0.0F) &&
        (!localDW->bitsForTID0.f_raw_rad_converged_temp)) {
      localDW->running_avg_raw_tire_radius =
        (localDW->running_avg_raw_tire_radius + temp_raw_tire_radius) / 2.0F;
    }
  }

  if (!localDW->bitsForTID0.f_est_rad_converged_temp) {
    if (((localDW->last_estimated_tire_radius > 0.0F) && (fabsf
          (localDW->running_avg_estimated_tire_radius -
           localDW->last_estimated_tire_radius) <
          rtu_TireRadEst_Constants->k_est_rad_tolerance)) &&
        (!localDW->bitsForTID0.f_est_rad_conv_time_start)) {
      localDW->est_rad_conv_time_start = localDW->curr_time;
      localDW->bitsForTID0.f_est_rad_conv_time_start = true;
    } else {
      if ((localDW->bitsForTID0.f_est_rad_conv_time_start) &&
          ((localDW->last_estimated_tire_radius <= 0.0F) || (fabsf
            (localDW->running_avg_estimated_tire_radius -
             localDW->last_estimated_tire_radius) >=
            rtu_TireRadEst_Constants->k_est_rad_tolerance))) {
        localDW->est_rad_conv_time_start = 0.0F;
        localDW->bitsForTID0.f_est_rad_conv_time_start = false;
      }
    }

    if ((localDW->bitsForTID0.f_est_rad_conv_time_start) && ((localDW->curr_time
          - localDW->est_rad_conv_time_start) >=
         rtu_TireRadEst_Constants->k_est_rad_mature_time)) {
      localDW->bitsForTID0.f_est_rad_converged_temp = true;
    }

    if ((temp_estimated_tire_radius > 0.0F) &&
        (!localDW->bitsForTID0.f_est_rad_converged_temp)) {
      localDW->running_avg_estimated_tire_radius =
        (localDW->running_avg_estimated_tire_radius + temp_estimated_tire_radius)
        / 2.0F;
    }
  }

  if ((localDW->bitsForTID0.f_raw_rad_converged_temp) && (fabsf
       (localDW->running_avg_raw_tire_radius - localDW->raw_rad_conv_persistent)
       > rtu_TireRadEst_Constants->k_raw_rad_tolerance)) {
    localDW->bitsForTID0.f_raw_rad_converged_temp = false;
    localDW->raw_rad_conv_persistent = localDW->running_avg_raw_tire_radius;
    localDW->bitsForTID0.f_raw_rad_converged = true;
  }

  if ((localDW->bitsForTID0.f_est_rad_converged_temp) && (fabsf
       (localDW->running_avg_estimated_tire_radius -
        localDW->est_rad_conv_persistent) >
       rtu_TireRadEst_Constants->k_est_rad_tolerance)) {
    localDW->bitsForTID0.f_est_rad_converged_temp = false;
    localDW->est_rad_conv_persistent = localDW->running_avg_raw_tire_radius;
    localDW->bitsForTID0.f_est_rad_converged = true;
  }

  *rty_raw_rad_conv = localDW->raw_rad_conv_persistent;
  *rty_est_rad_conv = localDW->est_rad_conv_persistent;
  *rty_f_raw_rad_conv_out = localDW->bitsForTID0.f_raw_rad_converged;
  *rty_f_est_rad_conv_out = localDW->bitsForTID0.f_est_rad_converged;
}

//
// System initialize for atomic system:
//    '<S55>/FindTireRadius'
//    '<S56>/FindTireRadius'
//    '<S57>/FindTireRadius'
//
void WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_h_Init
  (DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_e_T *localDW)
{
  localDW->bitsForTID0.f_radar_est_rad_conv_time_start = true;
}

//
// Output and update for atomic system:
//    '<S55>/FindTireRadius'
//    '<S56>/FindTireRadius'
//    '<S57>/FindTireRadius'
//
void WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_i(real32_T
  rtu_radar_comp_accumulated_dist, real32_T rtu_accumulated_angle, const
  TireRadEst_Constants *rtu_TireRadEst_Constants, real32_T
  *rty_radar_est_rad_conv, boolean_T *rty_f_radar_est_rad_conv_out,
  DW_FindTireRadius_WheelRadiusSpeedAndLongSlipEstimation_e_T *localDW)
{
  real32_T temp_radar_est_tire_radius;
  if (!localDW->bitsForTID0.curr_time_not_empty) {
    localDW->curr_time = 0.0F;
    localDW->bitsForTID0.curr_time_not_empty = true;
  } else {
    localDW->curr_time += 0.01F;
  }

  if ((rtu_radar_comp_accumulated_dist > 0.0F) && (rtu_accumulated_angle > 0.0F))
  {
    temp_radar_est_tire_radius = rtu_radar_comp_accumulated_dist /
      rtu_accumulated_angle;
    localDW->last_radar_estimated_tire_radius = temp_radar_est_tire_radius;
  } else {
    temp_radar_est_tire_radius = 0.0F;
  }

  if (!localDW->bitsForTID0.f_radar_est_rad_converged_temp) {
    if (((localDW->last_radar_estimated_tire_radius > 0.0F) && (fabsf
          (localDW->running_avg_radar_estimated_tire_radius -
           localDW->last_radar_estimated_tire_radius) <
          rtu_TireRadEst_Constants->k_raw_rad_tolerance)) &&
        (!localDW->bitsForTID0.f_radar_est_rad_conv_time_start)) {
      localDW->radar_est_rad_conv_time_start = localDW->curr_time;
      localDW->bitsForTID0.f_radar_est_rad_conv_time_start = true;
    } else {
      if ((localDW->bitsForTID0.f_radar_est_rad_conv_time_start) &&
          ((localDW->last_radar_estimated_tire_radius <= 0.0F) || (fabsf
            (localDW->running_avg_radar_estimated_tire_radius -
             localDW->last_radar_estimated_tire_radius) >=
            rtu_TireRadEst_Constants->k_raw_rad_tolerance))) {
        localDW->radar_est_rad_conv_time_start = 0.0F;
        localDW->bitsForTID0.f_radar_est_rad_conv_time_start = false;
      }
    }

    if ((localDW->bitsForTID0.f_radar_est_rad_conv_time_start) &&
        ((localDW->curr_time - localDW->radar_est_rad_conv_time_start) >=
         rtu_TireRadEst_Constants->k_raw_rad_mature_time)) {
      localDW->bitsForTID0.f_radar_est_rad_converged_temp = true;
    }

    if ((temp_radar_est_tire_radius > 0.0F) &&
        (!localDW->bitsForTID0.f_radar_est_rad_converged_temp)) {
      localDW->running_avg_radar_estimated_tire_radius =
        (localDW->running_avg_radar_estimated_tire_radius +
         temp_radar_est_tire_radius) / 2.0F;
    }
  }

  if ((localDW->bitsForTID0.f_radar_est_rad_converged_temp) && (fabsf
       (localDW->running_avg_radar_estimated_tire_radius -
        localDW->radar_est_rad_conv_persistent) >
       rtu_TireRadEst_Constants->k_est_rad_tolerance)) {
    localDW->bitsForTID0.f_radar_est_rad_converged_temp = false;
    localDW->radar_est_rad_conv_persistent =
      localDW->running_avg_radar_estimated_tire_radius;
    localDW->bitsForTID0.f_radar_est_rad_converged = true;
  }

  *rty_radar_est_rad_conv = localDW->radar_est_rad_conv_persistent;
  *rty_f_radar_est_rad_conv_out = localDW->bitsForTID0.f_radar_est_rad_converged;
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_release
  (dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_LPHPFilterBase_releaseImpl
  (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_release(obj->FilterObj);
  obj->NumChannels = -1;
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_releaseWrapper
  (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  if (obj->isSetupComplete) {
    WheelRadiusSpeedAndLongSlipEstimation_LPHPFilterBase_releaseImpl(obj);
  }
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_release_m
  (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  if (obj->isInitialized == 1) {
    WheelRadiusSpeedAndLongSlipEstimation_SystemCore_releaseWrapper(obj);
  }
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_delete_a
  (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_release_m(obj);
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_matlabCodegenHandle_matlabCodegenDestructor_g
  (dsp_LowpassFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    WheelRadiusSpeedAndLongSlipEstimation_SystemCore_delete_a(obj);
  }
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_delete
  (dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  WheelRadiusSpeedAndLongSlipEstimation_SystemCore_release(obj);
}

void WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimation_matlabCodegenHandle_matlabCodegenDestructor
  (dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    WheelRadiusSpeedAndLongSlipEstimation_SystemCore_delete(obj);
  }
}

// System initialize for referenced model: 'WheelRadiusSpeedAndLongSlipEstimation'
void WheelRadiusSpeedAndLongSlipEstimationModelClass::init(void)
{
  // InitializeConditions for MATLABSystem: '<S6>/Lowpass Filter'
  if (WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->isInitialized == 1)
  {
    // System object Initialization function: dsp.BiquadFilter
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [0] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [0] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [1] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [1] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [2] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [2] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [3] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [3] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
  }

  // End of InitializeConditions for MATLABSystem: '<S6>/Lowpass Filter'

  // SystemInitialize for MATLAB Function: '<S7>/LockStartingGPSPosition'
  WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_QF_locked = UNDEFINED;

  // SystemInitialize for MATLAB Function: '<S42>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_Init
    (&WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius);

  // SystemInitialize for MATLAB Function: '<S54>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_conv_time_start
    = true;

  // SystemInitialize for MATLAB Function: '<S55>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_h_Init
    (&WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_i);

  // SystemInitialize for MATLAB Function: '<S43>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_Init
    (&WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_e);

  // SystemInitialize for MATLAB Function: '<S56>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_h_Init
    (&WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_o);

  // SystemInitialize for MATLAB Function: '<S44>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_Init
    (&WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_m);

  // SystemInitialize for MATLAB Function: '<S45>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_Init
    (&WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_k);

  // SystemInitialize for MATLAB Function: '<S57>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_h_Init
    (&WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_f);
}

// Start for referenced model: 'WheelRadiusSpeedAndLongSlipEstimation'
void WheelRadiusSpeedAndLongSlipEstimationModelClass::start(void)
{
  dspcodegen_BiquadFilter_WheelRadiusSpeedAndLongSlipEstimation_T *iobj_0;
  int32_T i;
  static const real32_T tmp[6] = { 0.149377525F, -0.271013767F, 0.149377525F,
    0.105591848F, 0.105591848F, 0.0F };

  // Start for MATLABSystem: '<S6>/Lowpass Filter'
  WheelRadiusSpeedAndLongSlipEstimation_DW.gobj_1.matlabCodegenIsDeleted = true;
  WheelRadiusSpeedAndLongSlipEstimation_DW.gobj_0.matlabCodegenIsDeleted = true;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.matlabCodegenIsDeleted = true;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.isInitialized = 0;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.NumChannels = -1;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.matlabCodegenIsDeleted = false;
  WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.objisempty = true;
  iobj_0 = &WheelRadiusSpeedAndLongSlipEstimation_DW.gobj_0;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.isSetupComplete = false;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.isInitialized = 1;
  WheelRadiusSpeedAndLongSlipEstimation_DW.gobj_0.isInitialized = 0;

  // System object Constructor function: dsp.BiquadFilter
  iobj_0->cSFunObject.P0_ICRTP = 0.0F;
  for (i = 0; i < 6; i++) {
    iobj_0->cSFunObject.P1_RTP1COEFF[i] = tmp[i];
  }

  iobj_0->cSFunObject.P2_RTP2COEFF[0] = -1.83956015F;
  iobj_0->cSFunObject.P2_RTP2COEFF[1] = 0.877620876F;
  iobj_0->cSFunObject.P2_RTP2COEFF[2] = -0.84607476F;
  iobj_0->cSFunObject.P2_RTP2COEFF[3] = 0.0F;
  iobj_0->cSFunObject.P3_RTP3COEFF[0] = 0.0F;
  iobj_0->cSFunObject.P3_RTP3COEFF[1] = 0.0F;
  iobj_0->cSFunObject.P3_RTP3COEFF[2] = 0.0F;
  iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[0] = false;
  iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[1] = false;
  iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[2] = false;
  iobj_0->cSFunObject.P5_IC2RTP = 0.0F;
  WheelRadiusSpeedAndLongSlipEstimation_DW.gobj_0.matlabCodegenIsDeleted = false;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj =
    &WheelRadiusSpeedAndLongSlipEstimation_DW.gobj_0;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.NumChannels = 1;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.isSetupComplete = true;

  // End of Start for MATLABSystem: '<S6>/Lowpass Filter'
}

// Output and update for referenced model: 'WheelRadiusSpeedAndLongSlipEstimation'
void WheelRadiusSpeedAndLongSlipEstimationModelClass::step(const real32_T
  *rtu_raw_speed_mps, const enum_quality_factor_T *rtu_raw_speed_qf, const
  real32_T *rtu_WheelSpeed_FR_RPM, const real32_T *rtu_WheelSpeed_FL_RPM, const
  real32_T *rtu_WheelSpeed_RR_RPM, const real32_T *rtu_WheelSpeed_RL_RPM, const
  real32_T *rtu_comp_yaw_rate_filtered, const real_T *rtu_GPS_Lat_mas, const
  real_T *rtu_GPS_Long_mas, const enum_quality_factor_T *rtu_GPS_QF, const
  boolean_T *rtu_f_new_GPS_data, const real32_T *rtu_radar_compensated_speed_mps,
  const enum_quality_factor_T *rtu_radar_compensated_speed_qf, const real32_T
  *rtu_default_front_tire_radius_m, const real32_T
  *rtu_default_rear_tire_radius_m, real32_T
  *rty_front_left_estimated_tire_radius, tire_radius_estimation_mode_T
  *rty_front_left_estimated_tire_radius_mode, real32_T
  *rty_front_right_estimated_tire_radius, tire_radius_estimation_mode_T
  *rty_front_right_estimated_tire_radius_mode, real32_T
  *rty_rear_left_estimated_tire_radius, tire_radius_estimation_mode_T
  *rty_rear_left_estimated_tire_radius_mode, real32_T
  *rty_rear_right_estimated_tire_radius, tire_radius_estimation_mode_T
  *rty_rear_right_estimated_tire_radius_mode, real32_T *rty_gps_comp_factor,
  boolean_T *rty_f_gps_comp_factor)
{
  // local block i/o variables
  real32_T rtb_distance_along_earth;
  enum_quality_factor_T rtb_FindDistFrom_GPSLatLongDiff_o2;
  real32_T rtb_Switch2_ex;
  boolean_T rtb_f_radar_est_rad_conv_out_k;
  boolean_T rtb_f_est_rad_conv_out;
  boolean_T rtb_f_raw_rad_conv_out;
  real32_T rtb_raw_rad_conv;
  boolean_T rtb_Switch1;
  boolean_T rtb_Switch_cr;
  boolean_T rtb_f_est_rad_conv_out_e;
  real32_T rtb_raw_rad_conv_o;
  boolean_T rtb_f_raw_rad_conv_out_h;
  real32_T rtb_est_rad_conv_e;
  real32_T rtb_raw_rad_conv_n;
  TireRadEst_Constants
    rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1;
  enum_quality_factor_T rtb_GPS_QF_start;
  real_T rtb_GPS_long_start;
  real_T rtb_GPS_lat_start;

  // Chart: '<S1>/Chart'
  if (((uint32_T)
       WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.is_active_c5_WheelRadiusSpeedAndLongSlipEstimation)
      == 0U) {
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.is_active_c5_WheelRadiusSpeedAndLongSlipEstimation
      = 1;
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.is_c5_WheelRadiusSpeedAndLongSlipEstimation
      = WheelRadiusSpeedAndLongSlipEstimation_IN_Initialize;
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_initialize_speed_gps_comp
      = true;
  } else if (((uint32_T)
              WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.is_c5_WheelRadiusSpeedAndLongSlipEstimation)
             == WheelRadiusSpeedAndLongSlipEstimation_IN_Initialize) {
    if (WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_initialize_speed_gps_comp)
    {
      WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.is_c5_WheelRadiusSpeedAndLongSlipEstimation
        = WheelRadiusSpeedAndLongSlipEstimation_IN_Running;
      WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_initialize_speed_gps_comp
        = false;
    }
  } else {
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_initialize_speed_gps_comp
      = false;
  }

  // End of Chart: '<S1>/Chart'

  // Outputs for Enabled SubSystem: '<S1>/Speed_To_Accel_TF_Coeff' incorporates:
  //   EnablePort: '<S9>/Enable'

  if (WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_initialize_speed_gps_comp)
  {
    // MATLAB Function: '<S9>/Find_Speed_To_Accel_TF'
    WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[0] = 1.10854352F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.den_discrete[0] = 1.0F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[1] = 1.10854352F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.den_discrete[1] = -0.778631747F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[2] = -1.10854352F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.den_discrete[2] = -0.977829158F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[3] = -1.10854352F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.den_discrete[3] = 0.800802648F;
  }

  // End of Outputs for SubSystem: '<S1>/Speed_To_Accel_TF_Coeff'

  // DiscreteFilter: '<S1>/Discrete Filter'
  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_tmp =
    (((*rtu_raw_speed_mps) -
      (WheelRadiusSpeedAndLongSlipEstimation_DW.den_discrete[1] *
       WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[0])) -
     (WheelRadiusSpeedAndLongSlipEstimation_DW.den_discrete[2] *
      WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[1])) -
    (WheelRadiusSpeedAndLongSlipEstimation_DW.den_discrete[3] *
     WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[2]);

  // Switch: '<S6>/Switch3' incorporates:
  //   Constant: '<S6>/Constant7'
  //   Memory: '<S6>/Memory'

  if (WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.Memory_PreviousInput)
  {
    rtb_raw_rad_conv_n = 1.1F;
  } else {
    rtb_raw_rad_conv_n = 1.0F;
  }

  // End of Switch: '<S6>/Switch3'

  // Switch: '<S6>/Switch' incorporates:
  //   Abs: '<S6>/Abs'
  //   DiscreteFilter: '<S1>/Discrete Filter'
  //   RelationalOperator: '<S6>/Less Than'

  rtb_Switch_cr = (fabsf
                   ((((WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[0] *
                       WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_tmp)
                      + (WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[1]
    * WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[0])) +
                     (WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[2] *
                      WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states
                      [1])) +
                    (WheelRadiusSpeedAndLongSlipEstimation_DW.num_discrete[3] *
                     WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states
                     [2])) < rtb_raw_rad_conv_n);

  // Switch: '<S6>/Switch2' incorporates:
  //   Abs: '<S6>/Abs2'
  //   Constant: '<S41>/Constant'
  //   Constant: '<S6>/Constant4'
  //   Product: '<S6>/Divide'
  //   RelationalOperator: '<S41>/Compare'

  if (fabsf(*rtu_radar_compensated_speed_mps) > 0.5F) {
    rtb_Switch2_ex = (*rtu_comp_yaw_rate_filtered) /
      (*rtu_radar_compensated_speed_mps);
  } else {
    rtb_Switch2_ex = 0.0F;
  }

  // End of Switch: '<S6>/Switch2'

  // MATLABSystem: '<S6>/Lowpass Filter'
  if (WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->isInitialized != 1)
  {
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->isSetupComplete =
      false;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->isInitialized = 1;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->isSetupComplete =
      true;

    // System object Initialization function: dsp.BiquadFilter
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [0] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [0] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [1] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [1] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [2] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [2] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
      [3] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
      [3] =
      WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
  }

  // System object Outputs function: dsp.BiquadFilter
  rtb_raw_rad_conv_n =
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
    [0] * rtb_Switch2_ex;
  rtb_raw_rad_conv_n +=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
    [1] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [0];
  rtb_raw_rad_conv_n +=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
    [2] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [1];
  rtb_raw_rad_conv_n -=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
    [0] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [0];
  rtb_raw_rad_conv_n -=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
    [1] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [1];
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [1] =
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [0];
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [0] = rtb_Switch2_ex;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [1] =
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [0];
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [0] = rtb_raw_rad_conv_n;
  rtb_Switch2_ex = rtb_raw_rad_conv_n;
  rtb_raw_rad_conv_n *=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
    [3];
  rtb_raw_rad_conv_n +=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
    [4] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [2];
  rtb_raw_rad_conv_n +=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
    [5] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [3];
  rtb_raw_rad_conv_n -=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
    [2] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [2];
  rtb_raw_rad_conv_n -=
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
    [3] *
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [3];
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [3] =
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [2];
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES
    [2] = rtb_Switch2_ex;
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [3] =
    WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [2];
  WheelRadiusSpeedAndLongSlipEstimation_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES
    [2] = rtb_raw_rad_conv_n;

  // Abs: '<S6>/Abs1' incorporates:
  //   MATLABSystem: '<S6>/Lowpass Filter'

  rtb_Switch2_ex = fabsf(rtb_raw_rad_conv_n);

  // Memory: '<S6>/Memory1'
  rtb_f_radar_est_rad_conv_out_k =
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.Memory1_PreviousInput;

  // Switch: '<S6>/Switch4' incorporates:
  //   Constant: '<S6>/Constant10'
  //   Memory: '<S6>/Memory1'

  if (WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.Memory1_PreviousInput)
  {
    rtb_raw_rad_conv_n = 0.0006F;
  } else {
    rtb_raw_rad_conv_n = 0.0005F;
  }

  // End of Switch: '<S6>/Switch4'

  // Switch: '<S6>/Switch1' incorporates:
  //   RelationalOperator: '<S6>/Less Than1'

  rtb_Switch1 = (rtb_Switch2_ex < rtb_raw_rad_conv_n);

  // DiscreteIntegrator: '<S3>/Discrete-Time Integrator' incorporates:
  //   Constant: '<S18>/Constant'
  //   Logic: '<S3>/Logical Operator'
  //   RelationalOperator: '<S18>/Compare'
  //   RelationalOperator: '<S19>/Compare'
  //   RelationalOperator: '<S20>/Compare'

  if (((((uint32_T)(*rtu_raw_speed_qf)) != ACCURATED) || (!rtb_Switch_cr)) ||
      (!rtb_Switch1)) {
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_DSTATE =
      0.0F;
  }

  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator = (0.01F *
    (*rtu_raw_speed_mps)) +
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_DSTATE;

  // End of DiscreteIntegrator: '<S3>/Discrete-Time Integrator'

  // MATLAB Function: '<S10>/AccumulateAngleFromFrontLeftWheelSpeed' incorporates:
  //   Gain: '<S1>/REVpMIN_TO_RADpSEC1'

  WheelRadiusSpeedAndLongSlipEstimation_AccumulateAngleFromFrontLeftWheelSpeed
    (0.104719758F * (*rtu_WheelSpeed_FR_RPM), rtb_Switch_cr, rtb_Switch1,
     &rtb_Switch2_ex,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_AccumulateAngleFromFrontLeftWheelSpeed);

  // MATLAB Function: '<S7>/LockStartingGPSPosition'
  if ((rtb_Switch_cr && rtb_Switch1) &&
      (!WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_gps_distance_accum_started))
  {
    WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_long_locked = *rtu_GPS_Long_mas;
    WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_lat_locked = *rtu_GPS_Lat_mas;
    WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_QF_locked = *rtu_GPS_QF;
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_gps_distance_accum_started
      = true;
  }

  if ((!rtb_Switch_cr) || (!rtb_Switch1)) {
    WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_long_locked = 0.0;
    WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_lat_locked = 0.0;
    WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_QF_locked = UNDEFINED;
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_gps_distance_accum_started
      = false;
  }

  rtb_GPS_lat_start = WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_lat_locked;
  rtb_GPS_long_start = WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_long_locked;
  rtb_GPS_QF_start = WheelRadiusSpeedAndLongSlipEstimation_DW.GPS_QF_locked;

  // End of MATLAB Function: '<S7>/LockStartingGPSPosition'

  // ModelReference: '<S7>/FindDistFrom_GPSLatLongDiff'
  FindDistFrom_GPSLatLongDiffMDLOBJ1.step(&rtb_GPS_lat_start,
    &rtb_GPS_long_start, rtu_GPS_Lat_mas, rtu_GPS_Long_mas, &rtb_GPS_QF_start,
    rtu_GPS_QF, &rtb_distance_along_earth, &rtb_FindDistFrom_GPSLatLongDiff_o2);

  // BusCreator: '<S42>/BusConversion_InsertedFor_FindTireRadius_at_inport_4' incorporates:
  //   Constant: '<S42>/Constant'
  //   Constant: '<S42>/Constant1'
  //   Constant: '<S42>/Constant2'
  //   Constant: '<S42>/Constant3'
  //   Constant: '<S42>/Constant4'
  //   Constant: '<S42>/Constant5'

  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_mature_time
    = 3.0F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_mature_time
    = 1.5F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_min
    = 0.98F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_max
    = 1.02F;

  // MATLAB Function: '<S42>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius(rtb_distance_along_earth,
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator,
    rtu_f_new_GPS_data, rtb_Switch2_ex,
    &rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1,
    &rtb_raw_rad_conv_n, &WheelRadiusSpeedAndLongSlipEstimation_DW.Merge,
    &rtb_f_radar_est_rad_conv_out_k, &rtb_f_est_rad_conv_out_e,
    &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius);

  // DiscreteIntegrator: '<S53>/Discrete-Time Integrator' incorporates:
  //   Constant: '<S58>/Constant'
  //   Logic: '<S53>/Logical Operator'
  //   RelationalOperator: '<S58>/Compare'
  //   RelationalOperator: '<S59>/Compare'
  //   RelationalOperator: '<S60>/Compare'

  if (((((uint32_T)(*rtu_radar_compensated_speed_qf)) != ACCURATED) ||
       (!rtb_Switch_cr)) || (!rtb_Switch1)) {
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_DSTATE_n =
      0.0F;
  }

  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_e = (0.01F * (*
    rtu_radar_compensated_speed_mps)) +
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_DSTATE_n;

  // End of DiscreteIntegrator: '<S53>/Discrete-Time Integrator'

  // MATLAB Function: '<S54>/FindTireRadius' incorporates:
  //   BusCreator: '<S54>/BusConversion_InsertedFor_FindTireRadius_at_inport_2'
  //   Constant: '<S54>/Constant'
  //   Constant: '<S54>/Constant1'
  //   Constant: '<S54>/Constant2'

  if (!WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.curr_time_not_empty)
  {
    WheelRadiusSpeedAndLongSlipEstimation_DW.curr_time = 0.0F;
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.curr_time_not_empty =
      true;
  } else {
    WheelRadiusSpeedAndLongSlipEstimation_DW.curr_time += 0.01F;
  }

  if ((WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_e > 0.0F)
      && (rtb_Switch2_ex > 0.0F)) {
    rtb_Switch2_ex =
      WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_e /
      rtb_Switch2_ex;
    WheelRadiusSpeedAndLongSlipEstimation_DW.last_radar_estimated_tire_radius =
      rtb_Switch2_ex;
  } else {
    rtb_Switch2_ex = 0.0F;
  }

  if (!WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_converged_temp)
  {
    if (((WheelRadiusSpeedAndLongSlipEstimation_DW.last_radar_estimated_tire_radius
          > 0.0F) && (fabsf
                      (WheelRadiusSpeedAndLongSlipEstimation_DW.running_avg_radar_estimated_tire_radius
                       - WheelRadiusSpeedAndLongSlipEstimation_DW.last_radar_estimated_tire_radius)
                      < 0.0005F)) &&
        (!WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_conv_time_start))
    {
      WheelRadiusSpeedAndLongSlipEstimation_DW.radar_est_rad_conv_time_start =
        WheelRadiusSpeedAndLongSlipEstimation_DW.curr_time;
      WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_conv_time_start
        = true;
    } else {
      if ((WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_conv_time_start)
          &&
          ((WheelRadiusSpeedAndLongSlipEstimation_DW.last_radar_estimated_tire_radius
            <= 0.0F) || (fabsf
                         (WheelRadiusSpeedAndLongSlipEstimation_DW.running_avg_radar_estimated_tire_radius
                          - WheelRadiusSpeedAndLongSlipEstimation_DW.last_radar_estimated_tire_radius)
                         >= 0.0005F))) {
        WheelRadiusSpeedAndLongSlipEstimation_DW.radar_est_rad_conv_time_start =
          0.0F;
        WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_conv_time_start
          = false;
      }
    }

    if ((WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_conv_time_start)
        && ((WheelRadiusSpeedAndLongSlipEstimation_DW.curr_time -
             WheelRadiusSpeedAndLongSlipEstimation_DW.radar_est_rad_conv_time_start)
            >= 3.0F)) {
      WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_converged_temp
        = true;
    }

    if ((rtb_Switch2_ex > 0.0F) &&
        (!WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_converged_temp))
    {
      WheelRadiusSpeedAndLongSlipEstimation_DW.running_avg_radar_estimated_tire_radius
        =
        (WheelRadiusSpeedAndLongSlipEstimation_DW.running_avg_radar_estimated_tire_radius
         + rtb_Switch2_ex) / 2.0F;
    }
  }

  if ((WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_converged_temp)
      && (fabsf
          (WheelRadiusSpeedAndLongSlipEstimation_DW.running_avg_radar_estimated_tire_radius
           - WheelRadiusSpeedAndLongSlipEstimation_DW.radar_est_rad_conv_persistent)
          > 0.0005F)) {
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_converged_temp
      = false;
    WheelRadiusSpeedAndLongSlipEstimation_DW.radar_est_rad_conv_persistent =
      WheelRadiusSpeedAndLongSlipEstimation_DW.running_avg_radar_estimated_tire_radius;
    WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_converged
      = true;
  }

  // If: '<S21>/If' incorporates:
  //   Constant: '<S21>/Constant'
  //   Constant: '<S21>/Constant1'
  //   Constant: '<S21>/Constant2'
  //   Inport: '<S25>/In2'
  //   Inport: '<S26>/In1'
  //   Inport: '<S26>/In2'
  //   Inport: '<S27>/In1'
  //   Inport: '<S27>/In2'
  //   MATLAB Function: '<S54>/FindTireRadius'

  if (rtb_f_est_rad_conv_out_e) {
    // Outputs for IfAction SubSystem: '<S21>/If Action Subsystem' incorporates:
    //   ActionPort: '<S25>/Action Port'

    *rty_front_left_estimated_tire_radius_mode = GPS_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S21>/If Action Subsystem'
  } else if
      (WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.f_radar_est_rad_converged)
  {
    // Outputs for IfAction SubSystem: '<S21>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S26>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge =
      WheelRadiusSpeedAndLongSlipEstimation_DW.radar_est_rad_conv_persistent;
    *rty_front_left_estimated_tire_radius_mode = RADAR_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S21>/If Action Subsystem1'
  } else {
    // Outputs for IfAction SubSystem: '<S21>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S27>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge =
      *rtu_default_front_tire_radius_m;
    *rty_front_left_estimated_tire_radius_mode = DEFAULT_RADIUS;

    // End of Outputs for SubSystem: '<S21>/If Action Subsystem2'
  }

  // End of If: '<S21>/If'

  // MATLAB Function: '<S11>/AccumulateAngleFromFrontRightWheelSpeed' incorporates:
  //   Gain: '<S1>/REVpMIN_TO_RADpSEC6'

  WheelRadiusSpeedAndLongSlipEstimation_AccumulateAngleFromFrontLeftWheelSpeed
    (0.104719758F * (*rtu_WheelSpeed_FL_RPM), rtb_Switch_cr, rtb_Switch1,
     &rtb_Switch2_ex,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_AccumulateAngleFromFrontRightWheelSpeed);

  // BusCreator: '<S55>/BusConversion_InsertedFor_FindTireRadius_at_inport_2' incorporates:
  //   Constant: '<S55>/Constant'
  //   Constant: '<S55>/Constant1'
  //   Constant: '<S55>/Constant2'
  //   Constant: '<S55>/Constant3'
  //   Constant: '<S55>/Constant4'
  //   Constant: '<S55>/Constant5'

  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_mature_time
    = 3.0F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_mature_time
    = 1.5F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_min
    = 0.98F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_max
    = 1.02F;

  // MATLAB Function: '<S55>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_i
    (WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_e,
     rtb_Switch2_ex,
     &rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_j,
     &rtb_f_radar_est_rad_conv_out_k,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_i);

  // BusCreator: '<S43>/BusConversion_InsertedFor_FindTireRadius_at_inport_4' incorporates:
  //   Constant: '<S43>/Constant'
  //   Constant: '<S43>/Constant1'
  //   Constant: '<S43>/Constant2'
  //   Constant: '<S43>/Constant3'
  //   Constant: '<S43>/Constant4'
  //   Constant: '<S43>/Constant5'

  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_mature_time
    = 3.0F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_mature_time
    = 1.5F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_min
    = 0.98F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_max
    = 1.02F;

  // MATLAB Function: '<S43>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius(rtb_distance_along_earth,
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator,
    rtu_f_new_GPS_data, rtb_Switch2_ex,
    &rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1,
    &rtb_raw_rad_conv_n, &rtb_est_rad_conv_e, &rtb_f_raw_rad_conv_out_h,
    &rtb_f_est_rad_conv_out_e,
    &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_e);

  // If: '<S22>/If' incorporates:
  //   Constant: '<S22>/Constant'
  //   Constant: '<S22>/Constant1'
  //   Constant: '<S22>/Constant2'
  //   Inport: '<S29>/In1'
  //   Inport: '<S29>/In2'
  //   Inport: '<S30>/In2'
  //   Inport: '<S31>/In1'
  //   Inport: '<S31>/In2'

  if (rtb_f_est_rad_conv_out_e) {
    // Outputs for IfAction SubSystem: '<S22>/If Action Subsystem' incorporates:
    //   ActionPort: '<S29>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_j = rtb_est_rad_conv_e;
    *rty_front_right_estimated_tire_radius_mode = GPS_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S22>/If Action Subsystem'
  } else if (rtb_f_radar_est_rad_conv_out_k) {
    // Outputs for IfAction SubSystem: '<S22>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S30>/Action Port'

    *rty_front_right_estimated_tire_radius_mode = RADAR_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S22>/If Action Subsystem1'
  } else {
    // Outputs for IfAction SubSystem: '<S22>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S31>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_j =
      *rtu_default_front_tire_radius_m;
    *rty_front_right_estimated_tire_radius_mode = DEFAULT_RADIUS;

    // End of Outputs for SubSystem: '<S22>/If Action Subsystem2'
  }

  // End of If: '<S22>/If'

  // MATLAB Function: '<S12>/AccumulateAngleFromRearLeftWheelSpeed' incorporates:
  //   Gain: '<S1>/REVpMIN_TO_RADpSEC7'

  WheelRadiusSpeedAndLongSlipEstimation_AccumulateAngleFromFrontLeftWheelSpeed
    (0.104719758F * (*rtu_WheelSpeed_RR_RPM), rtb_Switch_cr, rtb_Switch1,
     &rtb_Switch2_ex,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_AccumulateAngleFromRearLeftWheelSpeed);

  // BusCreator: '<S56>/BusConversion_InsertedFor_FindTireRadius_at_inport_2' incorporates:
  //   Constant: '<S56>/Constant'
  //   Constant: '<S56>/Constant1'
  //   Constant: '<S56>/Constant2'
  //   Constant: '<S56>/Constant3'
  //   Constant: '<S56>/Constant4'
  //   Constant: '<S56>/Constant5'

  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_mature_time
    = 3.0F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_mature_time
    = 1.5F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_min
    = 0.98F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_max
    = 1.02F;

  // MATLAB Function: '<S56>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_i
    (WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_e,
     rtb_Switch2_ex,
     &rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_k,
     &rtb_f_radar_est_rad_conv_out_k,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_o);

  // BusCreator: '<S44>/BusConversion_InsertedFor_FindTireRadius_at_inport_4' incorporates:
  //   Constant: '<S44>/Constant'
  //   Constant: '<S44>/Constant1'
  //   Constant: '<S44>/Constant2'
  //   Constant: '<S44>/Constant3'
  //   Constant: '<S44>/Constant4'
  //   Constant: '<S44>/Constant5'

  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_mature_time
    = 3.0F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_mature_time
    = 1.5F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_min
    = 0.98F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_max
    = 1.02F;

  // MATLAB Function: '<S44>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius(rtb_distance_along_earth,
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator,
    rtu_f_new_GPS_data, rtb_Switch2_ex,
    &rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1,
    &rtb_raw_rad_conv_o, &rtb_est_rad_conv_e, &rtb_f_raw_rad_conv_out_h,
    &rtb_f_est_rad_conv_out_e,
    &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_m);

  // If: '<S23>/If' incorporates:
  //   Constant: '<S23>/Constant'
  //   Constant: '<S23>/Constant1'
  //   Constant: '<S23>/Constant2'
  //   Inport: '<S33>/In1'
  //   Inport: '<S33>/In2'
  //   Inport: '<S34>/In2'
  //   Inport: '<S35>/In1'
  //   Inport: '<S35>/In2'

  if (rtb_f_est_rad_conv_out_e) {
    // Outputs for IfAction SubSystem: '<S23>/If Action Subsystem' incorporates:
    //   ActionPort: '<S33>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_k = rtb_est_rad_conv_e;
    *rty_rear_left_estimated_tire_radius_mode = GPS_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S23>/If Action Subsystem'
  } else if (rtb_f_radar_est_rad_conv_out_k) {
    // Outputs for IfAction SubSystem: '<S23>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S34>/Action Port'

    *rty_rear_left_estimated_tire_radius_mode = RADAR_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S23>/If Action Subsystem1'
  } else {
    // Outputs for IfAction SubSystem: '<S23>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S35>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_k =
      *rtu_default_rear_tire_radius_m;
    *rty_rear_left_estimated_tire_radius_mode = DEFAULT_RADIUS;

    // End of Outputs for SubSystem: '<S23>/If Action Subsystem2'
  }

  // End of If: '<S23>/If'

  // MATLAB Function: '<S13>/AccumulateAngleFromRearRightWheelSpeed' incorporates:
  //   Gain: '<S1>/REVpMIN_TO_RADpSEC8'

  WheelRadiusSpeedAndLongSlipEstimation_AccumulateAngleFromFrontLeftWheelSpeed
    (0.104719758F * (*rtu_WheelSpeed_RL_RPM), rtb_Switch_cr, rtb_Switch1,
     &rtb_Switch2_ex,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_AccumulateAngleFromRearRightWheelSpeed);

  // BusCreator: '<S45>/BusConversion_InsertedFor_FindTireRadius_at_inport_4' incorporates:
  //   Constant: '<S45>/Constant'
  //   Constant: '<S45>/Constant1'
  //   Constant: '<S45>/Constant2'
  //   Constant: '<S45>/Constant3'
  //   Constant: '<S45>/Constant4'
  //   Constant: '<S45>/Constant5'

  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_mature_time
    = 3.0F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_mature_time
    = 1.5F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_min
    = 0.98F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_max
    = 1.02F;

  // MATLAB Function: '<S45>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius(rtb_distance_along_earth,
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator,
    rtu_f_new_GPS_data, rtb_Switch2_ex,
    &rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1,
    &rtb_raw_rad_conv, &rtb_raw_rad_conv_n, &rtb_f_raw_rad_conv_out,
    &rtb_f_est_rad_conv_out,
    &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_k);

  // Gain: '<S7>/Divide_by_two2' incorporates:
  //   Sum: '<S7>/Add2'

  rtb_raw_rad_conv_o = (rtb_raw_rad_conv_o + rtb_raw_rad_conv) * 0.5F;

  // If: '<S46>/If' incorporates:
  //   Constant: '<S46>/Constant'
  //   Switch: '<S46>/Switch'

  if (rtb_raw_rad_conv_o != 0.0F) {
    // Outputs for IfAction SubSystem: '<S46>/If Action Subsystem' incorporates:
    //   ActionPort: '<S52>/Action Port'

    // Product: '<S52>/Divide' incorporates:
    //   Gain: '<S7>/Divide_by_two3'
    //   Sum: '<S7>/Add3'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Divide = ((rtb_est_rad_conv_e +
      rtb_raw_rad_conv_n) * 0.5F) / rtb_raw_rad_conv_o;

    // End of Outputs for SubSystem: '<S46>/If Action Subsystem'
    *rty_gps_comp_factor = WheelRadiusSpeedAndLongSlipEstimation_DW.Divide;
  } else {
    *rty_gps_comp_factor = 1.0F;
  }

  // End of If: '<S46>/If'

  // BusCreator: '<S57>/BusConversion_InsertedFor_FindTireRadius_at_inport_2' incorporates:
  //   Constant: '<S57>/Constant'
  //   Constant: '<S57>/Constant1'
  //   Constant: '<S57>/Constant2'
  //   Constant: '<S57>/Constant3'
  //   Constant: '<S57>/Constant4'
  //   Constant: '<S57>/Constant5'

  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_tolerance
    = 0.0005F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_raw_rad_mature_time
    = 3.0F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_est_rad_mature_time
    = 1.5F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_min
    = 0.98F;
  rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1.k_comp_factor_max
    = 1.02F;

  // MATLAB Function: '<S57>/FindTireRadius'
  WheelRadiusSpeedAndLongSlipEstimation_FindTireRadius_i
    (WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_e,
     rtb_Switch2_ex,
     &rtb_BusConversion_InsertedFor_FindTireRadius_at_inport_4_BusCreator1,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_jc,
     &rtb_f_radar_est_rad_conv_out_k,
     &WheelRadiusSpeedAndLongSlipEstimation_DW.sf_FindTireRadius_f);

  // If: '<S24>/If' incorporates:
  //   Constant: '<S24>/Constant'
  //   Constant: '<S24>/Constant1'
  //   Constant: '<S24>/Constant2'
  //   Inport: '<S37>/In1'
  //   Inport: '<S37>/In2'
  //   Inport: '<S38>/In2'
  //   Inport: '<S39>/In1'
  //   Inport: '<S39>/In2'

  if (rtb_f_est_rad_conv_out) {
    // Outputs for IfAction SubSystem: '<S24>/If Action Subsystem' incorporates:
    //   ActionPort: '<S37>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_jc = rtb_raw_rad_conv_n;
    *rty_rear_right_estimated_tire_radius_mode = GPS_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S24>/If Action Subsystem'
  } else if (rtb_f_radar_est_rad_conv_out_k) {
    // Outputs for IfAction SubSystem: '<S24>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S38>/Action Port'

    *rty_rear_right_estimated_tire_radius_mode = RADAR_BASED_RADIUS;

    // End of Outputs for SubSystem: '<S24>/If Action Subsystem1'
  } else {
    // Outputs for IfAction SubSystem: '<S24>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S39>/Action Port'

    WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_jc =
      *rtu_default_rear_tire_radius_m;
    *rty_rear_right_estimated_tire_radius_mode = DEFAULT_RADIUS;

    // End of Outputs for SubSystem: '<S24>/If Action Subsystem2'
  }

  // End of If: '<S24>/If'

  // Logic: '<S46>/Logical Operator' incorporates:
  //   Logic: '<S7>/Logical Operator'
  //   Logic: '<S7>/Logical Operator1'

  *rty_f_gps_comp_factor = ((rtb_f_raw_rad_conv_out_h && rtb_f_raw_rad_conv_out)
    && (rtb_f_est_rad_conv_out_e && rtb_f_est_rad_conv_out));

  // Product: '<S21>/Divide1' incorporates:
  //   Constant: '<S21>/Constant4'

  rtb_raw_rad_conv_n = (*rtu_default_front_tire_radius_m) * 0.03F;

  // Sum: '<S21>/Add2'
  rtb_Switch2_ex = (*rtu_default_front_tire_radius_m) + rtb_raw_rad_conv_n;

  // Switch: '<S28>/Switch2' incorporates:
  //   RelationalOperator: '<S28>/LowerRelop1'

  if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge > rtb_Switch2_ex) {
    *rty_front_left_estimated_tire_radius = rtb_Switch2_ex;
  } else {
    // Sum: '<S21>/Add3'
    rtb_Switch2_ex = (*rtu_default_front_tire_radius_m) - rtb_raw_rad_conv_n;

    // Switch: '<S28>/Switch' incorporates:
    //   RelationalOperator: '<S28>/UpperRelop'

    if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge < rtb_Switch2_ex) {
      *rty_front_left_estimated_tire_radius = rtb_Switch2_ex;
    } else {
      *rty_front_left_estimated_tire_radius =
        WheelRadiusSpeedAndLongSlipEstimation_DW.Merge;
    }

    // End of Switch: '<S28>/Switch'
  }

  // End of Switch: '<S28>/Switch2'

  // Product: '<S22>/Divide1' incorporates:
  //   Constant: '<S22>/Constant4'

  rtb_raw_rad_conv_n = (*rtu_default_front_tire_radius_m) * 0.03F;

  // Sum: '<S22>/Add2'
  rtb_Switch2_ex = (*rtu_default_front_tire_radius_m) + rtb_raw_rad_conv_n;

  // Switch: '<S32>/Switch2' incorporates:
  //   RelationalOperator: '<S32>/LowerRelop1'

  if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_j > rtb_Switch2_ex) {
    *rty_front_right_estimated_tire_radius = rtb_Switch2_ex;
  } else {
    // Sum: '<S22>/Add3'
    rtb_Switch2_ex = (*rtu_default_front_tire_radius_m) - rtb_raw_rad_conv_n;

    // Switch: '<S32>/Switch' incorporates:
    //   RelationalOperator: '<S32>/UpperRelop'

    if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_j < rtb_Switch2_ex) {
      *rty_front_right_estimated_tire_radius = rtb_Switch2_ex;
    } else {
      *rty_front_right_estimated_tire_radius =
        WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_j;
    }

    // End of Switch: '<S32>/Switch'
  }

  // End of Switch: '<S32>/Switch2'

  // Product: '<S23>/Divide1' incorporates:
  //   Constant: '<S23>/Constant4'

  rtb_raw_rad_conv_n = (*rtu_default_rear_tire_radius_m) * 0.03F;

  // Sum: '<S23>/Add2'
  rtb_Switch2_ex = (*rtu_default_rear_tire_radius_m) + rtb_raw_rad_conv_n;

  // Switch: '<S36>/Switch2' incorporates:
  //   RelationalOperator: '<S36>/LowerRelop1'

  if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_k > rtb_Switch2_ex) {
    *rty_rear_left_estimated_tire_radius = rtb_Switch2_ex;
  } else {
    // Sum: '<S23>/Add3'
    rtb_Switch2_ex = (*rtu_default_rear_tire_radius_m) - rtb_raw_rad_conv_n;

    // Switch: '<S36>/Switch' incorporates:
    //   RelationalOperator: '<S36>/UpperRelop'

    if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_k < rtb_Switch2_ex) {
      *rty_rear_left_estimated_tire_radius = rtb_Switch2_ex;
    } else {
      *rty_rear_left_estimated_tire_radius =
        WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_k;
    }

    // End of Switch: '<S36>/Switch'
  }

  // End of Switch: '<S36>/Switch2'

  // Product: '<S24>/Divide1' incorporates:
  //   Constant: '<S24>/Constant4'

  rtb_raw_rad_conv_n = (*rtu_default_rear_tire_radius_m) * 0.03F;

  // Sum: '<S24>/Add2'
  rtb_Switch2_ex = (*rtu_default_rear_tire_radius_m) + rtb_raw_rad_conv_n;

  // Switch: '<S40>/Switch2' incorporates:
  //   RelationalOperator: '<S40>/LowerRelop1'

  if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_jc > rtb_Switch2_ex) {
    *rty_rear_right_estimated_tire_radius = rtb_Switch2_ex;
  } else {
    // Sum: '<S24>/Add3'
    rtb_Switch2_ex = (*rtu_default_rear_tire_radius_m) - rtb_raw_rad_conv_n;

    // Switch: '<S40>/Switch' incorporates:
    //   RelationalOperator: '<S40>/UpperRelop'

    if (WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_jc < rtb_Switch2_ex) {
      *rty_rear_right_estimated_tire_radius = rtb_Switch2_ex;
    } else {
      *rty_rear_right_estimated_tire_radius =
        WheelRadiusSpeedAndLongSlipEstimation_DW.Merge_jc;
    }

    // End of Switch: '<S40>/Switch'
  }

  // End of Switch: '<S40>/Switch2'

  // Update for DiscreteFilter: '<S1>/Discrete Filter'
  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[2] =
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[1];
  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[1] =
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[0];
  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_states[0] =
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteFilter_tmp;

  // Update for Memory: '<S6>/Memory'
  WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.Memory_PreviousInput =
    rtb_Switch_cr;

  // Update for Memory: '<S6>/Memory1'
  WheelRadiusSpeedAndLongSlipEstimation_DW.bitsForTID0.Memory1_PreviousInput =
    rtb_Switch1;

  // Update for DiscreteIntegrator: '<S3>/Discrete-Time Integrator'
  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_DSTATE =
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator;

  // Update for DiscreteIntegrator: '<S53>/Discrete-Time Integrator'
  WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_DSTATE_n =
    WheelRadiusSpeedAndLongSlipEstimation_DW.DiscreteTimeIntegrator_e;
}

// Constructor
WheelRadiusSpeedAndLongSlipEstimationModelClass::
  WheelRadiusSpeedAndLongSlipEstimationModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
WheelRadiusSpeedAndLongSlipEstimationModelClass::
  ~WheelRadiusSpeedAndLongSlipEstimationModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
