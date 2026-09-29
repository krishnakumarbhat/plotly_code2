//
// File: VehicleMassEstimator_2018b.cpp
//
// Code generated for Simulink model 'VehicleMassEstimator_2018b'.
//
// Model version                  : 1.364
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:00 2024
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
#include "VehicleMassEstimator_2018b.h"
#include "VehicleMassEstimator_2018b_private.h"
#include "diag_OhOSGesU_Aptiv.h"
#include "diag_hB9Vh4NX_Aptiv.h"
#include "mean_S0i0JSrU_Aptiv.h"

// Forward declaration for local functions
static void VehicleMassEstimator_2018b_SystemCore_release
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_LPHPFilterBase_releaseImpl
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_releaseWrapper
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_release_m
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_delete_a
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj);
static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor_g
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_delete
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *obj);
static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *obj);

// Forward declaration for local functions
static void VehicleMassEstimator_2018b_SystemCore_release_mt2
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *obj);
static void VehicleMassEstimator_2018b_LPHPFilterBase_releaseImpl_n
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_releaseWrapper_fhr
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_release_mt2c
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_delete_asm
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj);
static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor_goq
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_delete_as
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *obj);
static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor_go
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *obj);
static void VehicleMassEstimator_2018b_SystemCore_release
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

static void VehicleMassEstimator_2018b_LPHPFilterBase_releaseImpl
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  VehicleMassEstimator_2018b_SystemCore_release(obj->FilterObj);
  obj->NumChannels = -1;
}

static void VehicleMassEstimator_2018b_SystemCore_releaseWrapper
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (obj->isSetupComplete) {
    VehicleMassEstimator_2018b_LPHPFilterBase_releaseImpl(obj);
  }
}

static void VehicleMassEstimator_2018b_SystemCore_release_m
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (obj->isInitialized == 1) {
    VehicleMassEstimator_2018b_SystemCore_releaseWrapper(obj);
  }
}

static void VehicleMassEstimator_2018b_SystemCore_delete_a
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  VehicleMassEstimator_2018b_SystemCore_release_m(obj);
}

static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor_g
  (dsp_HighpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    VehicleMassEstimator_2018b_SystemCore_delete_a(obj);
  }
}

static void VehicleMassEstimator_2018b_SystemCore_delete
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *obj)
{
  VehicleMassEstimator_2018b_SystemCore_release(obj);
}

static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    VehicleMassEstimator_2018b_SystemCore_delete(obj);
  }
}

//
// System initialize for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void VehicleMassEstimator_2018b_HighpassFilter3_Init
  (DW_HighpassFilter3_VehicleMassEstimator_2018b_T *localDW)
{
  int32_T i;

  // InitializeConditions for MATLABSystem: '<S3>/Highpass Filter3'
  if (localDW->obj.FilterObj->isInitialized == 1) {
    // System object Initialization function: dsp.BiquadFilter
    for (i = 0; i < 10; i++) {
      localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[i] =
        localDW->obj.FilterObj->cSFunObject.P0_ICRTP;
      localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[i] =
        localDW->obj.FilterObj->cSFunObject.P5_IC2RTP;
    }
  }

  // End of InitializeConditions for MATLABSystem: '<S3>/Highpass Filter3'
}

//
// Start for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void VehicleMassEstimator_2018b_HighpassFilter3_Start
  (DW_HighpassFilter3_VehicleMassEstimator_2018b_T *localDW)
{
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_T *iobj_0;
  int32_T i;
  static const real_T tmp[15] = { 0.48589159504618851, -0.971371803559309,
    0.48589159504618845, 1.0000504747650509, -1.9999999999999998,
    1.0000504747650507, 1.0004825747265893, -1.9999999999999998,
    1.0004825747265891, 1.0002833073259745, -1.9999999999999998,
    1.000283307325974, 1.8995826782843195, -3.7973795330721849,
    1.899582678284319 };

  static const real_T tmp_0[10] = { -1.9952920332277544, 0.99635772519831789,
    -1.9198626590861436, 0.92271934187728055, -1.9988358965426647,
    0.99982032600855275, -1.9831722797151403, 0.98452561587419907,
    -1.9980846498307336, 0.9990846297351833 };

  // Start for MATLABSystem: '<S3>/Highpass Filter3'
  localDW->gobj_1.matlabCodegenIsDeleted = true;
  localDW->gobj_0.matlabCodegenIsDeleted = true;
  localDW->obj.matlabCodegenIsDeleted = true;
  localDW->obj.isInitialized = 0;
  localDW->obj.NumChannels = -1;
  localDW->obj.matlabCodegenIsDeleted = false;
  localDW->bitsForTID0.objisempty = true;
  iobj_0 = &localDW->gobj_0;
  localDW->obj.isSetupComplete = false;
  localDW->obj.isInitialized = 1;
  localDW->gobj_0.isInitialized = 0;

  // System object Constructor function: dsp.BiquadFilter
  iobj_0->cSFunObject.P0_ICRTP = 0.0;
  for (i = 0; i < 15; i++) {
    iobj_0->cSFunObject.P1_RTP1COEFF[i] = tmp[i];
  }

  for (i = 0; i < 10; i++) {
    iobj_0->cSFunObject.P2_RTP2COEFF[i] = tmp_0[i];
  }

  for (i = 0; i < 6; i++) {
    iobj_0->cSFunObject.P3_RTP3COEFF[i] = 0.0;
  }

  for (i = 0; i < 6; i++) {
    iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[i] = false;
  }

  iobj_0->cSFunObject.P5_IC2RTP = 0.0;
  localDW->gobj_0.matlabCodegenIsDeleted = false;
  localDW->obj.FilterObj = &localDW->gobj_0;
  localDW->obj.NumChannels = 1;
  localDW->obj.isSetupComplete = true;

  // End of Start for MATLABSystem: '<S3>/Highpass Filter3'
}

//
// Output and update for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void VehicleMassEstimator_2018b_HighpassFilter3(real_T rtu_0,
  DW_HighpassFilter3_VehicleMassEstimator_2018b_T *localDW)
{
  real_T numAccum;
  real_T stageIn;
  int32_T i;

  // MATLABSystem: '<S3>/Highpass Filter3'
  if (localDW->obj.FilterObj->isInitialized != 1) {
    localDW->obj.FilterObj->isSetupComplete = false;
    localDW->obj.FilterObj->isInitialized = 1;
    localDW->obj.FilterObj->isSetupComplete = true;

    // System object Initialization function: dsp.BiquadFilter
    for (i = 0; i < 10; i++) {
      localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[i] =
        localDW->obj.FilterObj->cSFunObject.P0_ICRTP;
      localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[i] =
        localDW->obj.FilterObj->cSFunObject.P5_IC2RTP;
    }
  }

  // System object Outputs function: dsp.BiquadFilter
  numAccum = localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] * rtu_0;
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[1];
  localDW->HighpassFilter3 = numAccum - (localDW->
    obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] * localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[0]);
  localDW->HighpassFilter3 -= localDW->obj.FilterObj->cSFunObject.P2_RTP2COEFF[1]
    * localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[1];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] = localDW->
    obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] = rtu_0;
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[1] = localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[0] =
    localDW->HighpassFilter3;
  stageIn = localDW->HighpassFilter3;
  numAccum = localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
    localDW->HighpassFilter3;
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[3];
  localDW->HighpassFilter3 = numAccum - (localDW->
    obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] * localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[2]);
  localDW->HighpassFilter3 -= localDW->obj.FilterObj->cSFunObject.P2_RTP2COEFF[3]
    * localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[3];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[3] = localDW->
    obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[2] = stageIn;
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[3] = localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[2] =
    localDW->HighpassFilter3;
  stageIn = localDW->HighpassFilter3;
  numAccum = localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[6] *
    localDW->HighpassFilter3;
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[7] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[8] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[5];
  localDW->HighpassFilter3 = numAccum - (localDW->
    obj.FilterObj->cSFunObject.P2_RTP2COEFF[4] * localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[4]);
  localDW->HighpassFilter3 -= localDW->obj.FilterObj->cSFunObject.P2_RTP2COEFF[5]
    * localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[5];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[5] = localDW->
    obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[4] = stageIn;
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[5] = localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[4];
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[4] =
    localDW->HighpassFilter3;
  stageIn = localDW->HighpassFilter3;
  numAccum = localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[9] *
    localDW->HighpassFilter3;
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[10] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[11] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[7];
  localDW->HighpassFilter3 = numAccum - (localDW->
    obj.FilterObj->cSFunObject.P2_RTP2COEFF[6] * localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[6]);
  localDW->HighpassFilter3 -= localDW->obj.FilterObj->cSFunObject.P2_RTP2COEFF[7]
    * localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[7];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[7] = localDW->
    obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[6] = stageIn;
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[7] = localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[6];
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[6] =
    localDW->HighpassFilter3;
  stageIn = localDW->HighpassFilter3;
  numAccum = localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[12] *
    localDW->HighpassFilter3;
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[13] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[14] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[9];
  localDW->HighpassFilter3 = numAccum - (localDW->
    obj.FilterObj->cSFunObject.P2_RTP2COEFF[8] * localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[8]);
  localDW->HighpassFilter3 -= localDW->obj.FilterObj->cSFunObject.P2_RTP2COEFF[9]
    * localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[9];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[9] = localDW->
    obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[8] = stageIn;
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[9] = localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[8];
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[8] =
    localDW->HighpassFilter3;

  // End of MATLABSystem: '<S3>/Highpass Filter3'
}

static void VehicleMassEstimator_2018b_SystemCore_release_mt2
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

static void VehicleMassEstimator_2018b_LPHPFilterBase_releaseImpl_n
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  VehicleMassEstimator_2018b_SystemCore_release_mt2(obj->FilterObj);
  obj->NumChannels = -1;
}

static void VehicleMassEstimator_2018b_SystemCore_releaseWrapper_fhr
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (obj->isSetupComplete) {
    VehicleMassEstimator_2018b_LPHPFilterBase_releaseImpl_n(obj);
  }
}

static void VehicleMassEstimator_2018b_SystemCore_release_mt2c
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (obj->isInitialized == 1) {
    VehicleMassEstimator_2018b_SystemCore_releaseWrapper_fhr(obj);
  }
}

static void VehicleMassEstimator_2018b_SystemCore_delete_asm
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  VehicleMassEstimator_2018b_SystemCore_release_mt2c(obj);
}

static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor_goq
  (dsp_LowpassFilter_VehicleMassEstimator_2018b_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    VehicleMassEstimator_2018b_SystemCore_delete_asm(obj);
  }
}

static void VehicleMassEstimator_2018b_SystemCore_delete_as
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *obj)
{
  VehicleMassEstimator_2018b_SystemCore_release_mt2(obj);
}

static void
  VehicleMassEstimator_2018b_matlabCodegenHandle_matlabCodegenDestructor_go
  (dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    VehicleMassEstimator_2018b_SystemCore_delete_as(obj);
  }
}

//
// System initialize for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void VehicleMassEstimator_2018b_LowpassFilter2_Init
  (DW_LowpassFilter2_VehicleMassEstimator_2018b_T *localDW)
{
  // InitializeConditions for MATLABSystem: '<S3>/Lowpass Filter2'
  if (localDW->obj.FilterObj->isInitialized == 1) {
    // System object Initialization function: dsp.BiquadFilter
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
      localDW->obj.FilterObj->cSFunObject.P0_ICRTP;
    localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[0] =
      localDW->obj.FilterObj->cSFunObject.P5_IC2RTP;
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
      localDW->obj.FilterObj->cSFunObject.P0_ICRTP;
    localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
      localDW->obj.FilterObj->cSFunObject.P5_IC2RTP;
  }

  // End of InitializeConditions for MATLABSystem: '<S3>/Lowpass Filter2'
}

//
// Start for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void VehicleMassEstimator_2018b_LowpassFilter2_Start
  (DW_LowpassFilter2_VehicleMassEstimator_2018b_T *localDW)
{
  dspcodegen_BiquadFilter_VehicleMassEstimator_2018b_m_T *iobj_0;

  // Start for MATLABSystem: '<S3>/Lowpass Filter2'
  localDW->gobj_1.matlabCodegenIsDeleted = true;
  localDW->gobj_0.matlabCodegenIsDeleted = true;
  localDW->obj.matlabCodegenIsDeleted = true;
  localDW->obj.isInitialized = 0;
  localDW->obj.NumChannels = -1;
  localDW->obj.matlabCodegenIsDeleted = false;
  localDW->bitsForTID0.objisempty = true;
  iobj_0 = &localDW->gobj_0;
  localDW->obj.isSetupComplete = false;
  localDW->obj.isInitialized = 1;
  localDW->gobj_0.isInitialized = 0;

  // System object Constructor function: dsp.BiquadFilter
  iobj_0->cSFunObject.P0_ICRTP = 0.0;
  iobj_0->cSFunObject.P1_RTP1COEFF[0] = 0.013328106976106513;
  iobj_0->cSFunObject.P1_RTP1COEFF[1] = -0.0098750623475528064;
  iobj_0->cSFunObject.P1_RTP1COEFF[2] = 0.013328106976106509;
  iobj_0->cSFunObject.P2_RTP2COEFF[0] = -1.81812479993625;
  iobj_0->cSFunObject.P2_RTP2COEFF[1] = 0.83500283018636567;
  iobj_0->cSFunObject.P3_RTP3COEFF[0] = 0.0;
  iobj_0->cSFunObject.P3_RTP3COEFF[1] = 0.0;
  iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[0] = false;
  iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[1] = false;
  iobj_0->cSFunObject.P5_IC2RTP = 0.0;
  localDW->gobj_0.matlabCodegenIsDeleted = false;
  localDW->obj.FilterObj = &localDW->gobj_0;
  localDW->obj.NumChannels = 1;
  localDW->obj.isSetupComplete = true;
}

//
// Output and update for atomic system:
//    synthesized block
//    synthesized block
//    synthesized block
//
void VehicleMassEstimator_2018b_LowpassFilter2(real_T rtu_0,
  DW_LowpassFilter2_VehicleMassEstimator_2018b_T *localDW)
{
  real_T numAccum;

  // MATLABSystem: '<S3>/Lowpass Filter2'
  if (localDW->obj.FilterObj->isInitialized != 1) {
    localDW->obj.FilterObj->isSetupComplete = false;
    localDW->obj.FilterObj->isInitialized = 1;
    localDW->obj.FilterObj->isSetupComplete = true;

    // System object Initialization function: dsp.BiquadFilter
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
      localDW->obj.FilterObj->cSFunObject.P0_ICRTP;
    localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[0] =
      localDW->obj.FilterObj->cSFunObject.P5_IC2RTP;
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
      localDW->obj.FilterObj->cSFunObject.P0_ICRTP;
    localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
      localDW->obj.FilterObj->cSFunObject.P5_IC2RTP;
  }

  // System object Outputs function: dsp.BiquadFilter
  numAccum = localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] * rtu_0;
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
  numAccum += localDW->obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
    localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[1];
  localDW->LowpassFilter2 = numAccum - (localDW->
    obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] * localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[0]);
  localDW->LowpassFilter2 -= localDW->obj.FilterObj->cSFunObject.P2_RTP2COEFF[1]
    * localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[1];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] = localDW->
    obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
  localDW->obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] = rtu_0;
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[1] = localDW->
    obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
  localDW->obj.FilterObj->cSFunObject.W1_POLE_STATES[0] =
    localDW->LowpassFilter2;

  // End of MATLABSystem: '<S3>/Lowpass Filter2'
}

// System initialize for referenced model: 'VehicleMassEstimator_2018b'
void VehicleMassEstimator_2018bModelClass::init(void)
{
  // InitializeConditions for Delay: '<S4>/Delay1'
  VehicleMassEstimator_2018b_DW.icLoad = 1U;
  VehicleMassEstimator_2018b_LowpassFilter2_Init
    (&VehicleMassEstimator_2018b_DW.LowpassFilter);
  VehicleMassEstimator_2018b_HighpassFilter3_Init
    (&VehicleMassEstimator_2018b_DW.HighpassFilter1);
  VehicleMassEstimator_2018b_LowpassFilter2_Init
    (&VehicleMassEstimator_2018b_DW.LowpassFilter1);
  VehicleMassEstimator_2018b_HighpassFilter3_Init
    (&VehicleMassEstimator_2018b_DW.HighpassFilter2);
  VehicleMassEstimator_2018b_LowpassFilter2_Init
    (&VehicleMassEstimator_2018b_DW.LowpassFilter2);
  VehicleMassEstimator_2018b_HighpassFilter3_Init
    (&VehicleMassEstimator_2018b_DW.HighpassFilter3);

  // SystemInitialize for MATLAB Function: '<S3>/MassEstimation'
  VehicleMassEstimator_2018b_DW.x_ekf[0] = 1000.0;
  VehicleMassEstimator_2018b_DW.x_ekf[1] = 0.0;
  VehicleMassEstimator_2018b_DW.x_ekf[2] = 0.0;
  VehicleMassEstimator_2018b_DW.x_ekf[3] = 0.0;
  VehicleMassEstimator_2018b_DW.P_rls = 0.1;
  VehicleMassEstimator_2018b_DW.w_rls = 500.0;
  VehicleMassEstimator_2018b_DW.x_ekf2 = 500.0;
}

// Start for referenced model: 'VehicleMassEstimator_2018b'
void VehicleMassEstimator_2018bModelClass::start(void)
{
  VehicleMassEstimator_2018b_LowpassFilter2_Start
    (&VehicleMassEstimator_2018b_DW.LowpassFilter);
  VehicleMassEstimator_2018b_HighpassFilter3_Start
    (&VehicleMassEstimator_2018b_DW.HighpassFilter1);
  VehicleMassEstimator_2018b_LowpassFilter2_Start
    (&VehicleMassEstimator_2018b_DW.LowpassFilter1);
  VehicleMassEstimator_2018b_HighpassFilter3_Start
    (&VehicleMassEstimator_2018b_DW.HighpassFilter2);

  // Start for MATLABSystem: '<S3>/Lowpass Filter2'
  VehicleMassEstimator_2018b_LowpassFilter2_Start
    (&VehicleMassEstimator_2018b_DW.LowpassFilter2);

  // Start for MATLABSystem: '<S3>/Highpass Filter3'
  VehicleMassEstimator_2018b_HighpassFilter3_Start
    (&VehicleMassEstimator_2018b_DW.HighpassFilter3);
}

// Output and update for referenced model: 'VehicleMassEstimator_2018b'
void VehicleMassEstimator_2018bModelClass::step(const real32_T
  *rtu_VsVCAN_Nm_EngTrq, const real32_T *rtu_filtered_speed_over_ground, const
  real32_T *rtu_Wheel_Running_Radii, const real32_T *rtu_VsVCAN_Nm_BrkTrq, const
  real32_T *rtu_raw_long_accel, const real32_T *rtu_Comp_YawRate_Filtered, const
  real32_T *rtu_GearReductionRatio, real32_T *rty_VsVSE_Kg_TrlrMass, boolean_T
  *rty_VsVSE_b_TrlrMassConv)
{
  // local block i/o variables
  real_T rtb_Multiply;
  real_T rtb_CastToDouble2;
  real_T rtb_CastToDouble7;
  real_T diff_speed_filt;
  real_T mean_radii;
  real_T radius_of_curv;
  int32_T collect_acc_sample;
  int32_T collect_brake_sample;
  real_T R[9];
  real_T H[12];
  real_T S[9];
  real_T K[12];
  real_T y[12];
  int32_T r2;
  int32_T r3;
  real_T maxval;
  real_T a21;
  int8_T b_I[16];
  static const real_T b[4] = { 1000.0, 0.1, 1.0, 100.0 };

  static const real_T Q[16] = { 1.0000000000000001E-7, 0.0, 0.0, 0.0, 0.0,
    0.0001, 0.0, 0.0, 0.0, 0.0, 1.0000000000000001E-7, 0.0, 0.0, 0.0, 0.0,
    1.0000000000000002E-6 };

  static const real_T c[3] = { 1.0, 30.0, 1500.0 };

  real_T rtb_Gain1;
  real32_T rtb_Saturation;
  real32_T rtb_forgetting_factor;
  real_T rtu_Wheel_Running_Radii_0[4];
  real_T H_0[12];
  real_T b_I_0[16];
  real_T b_I_1[16];
  real_T error_idx_1;
  real_T error_idx_2;
  real_T error_idx_1_tmp;
  real_T error_idx_1_tmp_0;
  real_T error_idx_1_tmp_1;
  int32_T H_tmp;
  int32_T K_tmp;
  int32_T K_tmp_0;
  int32_T K_tmp_1;
  int32_T K_tmp_2;
  int32_T K_tmp_3;

  // Gain: '<S1>/Gain1' incorporates:
  //   DataTypeConversion: '<S1>/Cast To Double9'

  rtb_Gain1 = 0.15915494309189535 * ((real_T)(*rtu_GearReductionRatio));

  // Product: '<S1>/Multiply' incorporates:
  //   DataTypeConversion: '<S1>/Cast To Double1'

  rtb_Multiply = rtb_Gain1 * ((real_T)(*rtu_VsVCAN_Nm_EngTrq));
  VehicleMassEstimator_2018b_LowpassFilter2(rtb_Multiply,
    &VehicleMassEstimator_2018b_DW.LowpassFilter);
  VehicleMassEstimator_2018b_HighpassFilter3
    (VehicleMassEstimator_2018b_DW.LowpassFilter.LowpassFilter2,
     &VehicleMassEstimator_2018b_DW.HighpassFilter1);

  // DataTypeConversion: '<S1>/Cast To Double2'
  rtb_CastToDouble2 = (real_T)(*rtu_filtered_speed_over_ground);
  VehicleMassEstimator_2018b_LowpassFilter2(rtb_CastToDouble2,
    &VehicleMassEstimator_2018b_DW.LowpassFilter1);
  VehicleMassEstimator_2018b_HighpassFilter3
    (VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2,
     &VehicleMassEstimator_2018b_DW.HighpassFilter2);

  // DataTypeConversion: '<S1>/Cast To Double7'
  rtb_CastToDouble7 = (real_T)(*rtu_VsVCAN_Nm_BrkTrq);

  // MATLABSystem: '<S3>/Lowpass Filter2'
  VehicleMassEstimator_2018b_LowpassFilter2(rtb_CastToDouble7,
    &VehicleMassEstimator_2018b_DW.LowpassFilter2);

  // MATLABSystem: '<S3>/Highpass Filter3'
  VehicleMassEstimator_2018b_HighpassFilter3
    (VehicleMassEstimator_2018b_DW.LowpassFilter2.LowpassFilter2,
     &VehicleMassEstimator_2018b_DW.HighpassFilter3);

  // MATLAB Function: '<S3>/MassEstimation'
  if (!VehicleMassEstimator_2018b_DW.bitsForTID0.prev_speed_not_empty) {
    VehicleMassEstimator_2018b_DW.prev_speed =
      VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2;
    VehicleMassEstimator_2018b_DW.bitsForTID0.prev_speed_not_empty = true;
    VehicleMassEstimator_2018b_DW.prev_speed_filt =
      VehicleMassEstimator_2018b_DW.HighpassFilter2.HighpassFilter3;
    diag_OhOSGesU_Aptiv(b, VehicleMassEstimator_2018b_DW.P_ekf);
    VehicleMassEstimator_2018b_DW.P_ekf2 = 1000.0;
  }

  // SignalConversion: '<S5>/TmpSignal ConversionAt SFunction Inport5' incorporates:
  //   MATLAB Function: '<S3>/MassEstimation'

  rtu_Wheel_Running_Radii_0[0] = (real_T)(*rtu_Wheel_Running_Radii);
  rtu_Wheel_Running_Radii_0[1] = (real_T)(*rtu_Wheel_Running_Radii);
  rtu_Wheel_Running_Radii_0[2] = (real_T)(*rtu_Wheel_Running_Radii);
  rtu_Wheel_Running_Radii_0[3] = (real_T)(*rtu_Wheel_Running_Radii);

  // MATLAB Function: '<S3>/MassEstimation' incorporates:
  //   DataTypeConversion: '<S1>/Cast To Double5'
  //   DataTypeConversion: '<S1>/Cast To Double6'

  mean_radii = mean_S0i0JSrU_Aptiv(rtu_Wheel_Running_Radii_0);
  if (mean_radii < 0.1) {
    mean_radii = 0.1;
  }

  if (rtb_Gain1 < 0.0001) {
    mean_radii = 0.0001;
  }

  if (VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2 > 25.0) {
    error_idx_2 = (VehicleMassEstimator_2018b_DW.LowpassFilter.LowpassFilter2 -
                   VehicleMassEstimator_2018b_DW.LowpassFilter2.LowpassFilter2) /
      mean_radii;
  } else {
    error_idx_2 = ((2.0 *
                    VehicleMassEstimator_2018b_DW.LowpassFilter.LowpassFilter2)
                   - VehicleMassEstimator_2018b_DW.LowpassFilter2.LowpassFilter2)
      / mean_radii;
  }

  maxval = (VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2 -
            VehicleMassEstimator_2018b_DW.prev_speed) / 0.01;
  mean_radii = (VehicleMassEstimator_2018b_DW.HighpassFilter1.HighpassFilter3 -
                VehicleMassEstimator_2018b_DW.HighpassFilter3.HighpassFilter3) /
    mean_radii;
  diff_speed_filt =
    (VehicleMassEstimator_2018b_DW.HighpassFilter2.HighpassFilter3 -
     VehicleMassEstimator_2018b_DW.prev_speed_filt) / 0.01;
  radius_of_curv = fabs((real_T)(*rtu_Comp_YawRate_Filtered));
  if (radius_of_curv > 0.0001) {
    radius_of_curv = VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2
      / radius_of_curv;
  } else {
    radius_of_curv = 10000.0;
  }

  if ((((maxval > 0.3) &&
        (VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2 > 2.0)) &&
       (VehicleMassEstimator_2018b_DW.LowpassFilter2.LowpassFilter2 < 1.0)) &&
      (rtb_Gain1 < 10.0)) {
    if ((VehicleMassEstimator_2018b_DW.LowpassFilter.LowpassFilter2 / rtb_Gain1)
        > 100.0) {
      if (radius_of_curv > 1000.0) {
        VehicleMassEstimator_2018b_DW.acc_filt_debounce += 0.01;
      } else {
        VehicleMassEstimator_2018b_DW.acc_filt_debounce = 0.0;
      }
    } else {
      VehicleMassEstimator_2018b_DW.acc_filt_debounce = 0.0;
    }
  } else {
    VehicleMassEstimator_2018b_DW.acc_filt_debounce = 0.0;
  }

  if (VehicleMassEstimator_2018b_DW.acc_filt_debounce > 0.3) {
    collect_acc_sample = 1;
    VehicleMassEstimator_2018b_DW.count_acc_sample++;
  } else {
    collect_acc_sample = 0;
  }

  if (((((maxval < -0.3) &&
         (VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2 > 2.0)) &&
        (VehicleMassEstimator_2018b_DW.LowpassFilter2.LowpassFilter2 > 500.0)) &&
       (VehicleMassEstimator_2018b_DW.LowpassFilter.LowpassFilter2 < 10.0)) &&
      (radius_of_curv > 1000.0)) {
    VehicleMassEstimator_2018b_DW.brake_filt_debounce += 0.01;
  } else {
    VehicleMassEstimator_2018b_DW.brake_filt_debounce = 0.0;
  }

  if (VehicleMassEstimator_2018b_DW.brake_filt_debounce > 0.3) {
    collect_brake_sample = 1;
    VehicleMassEstimator_2018b_DW.count_brake_sample++;
  } else {
    collect_brake_sample = 0;
  }

  if ((collect_acc_sample == 1) || (collect_brake_sample == 1)) {
    rtb_Gain1 = error_idx_2 / maxval;
    if (rtb_Gain1 > 140000.0) {
      rtb_Gain1 = 140000.0;
    }

    if (rtb_Gain1 < 70.0) {
      rtb_Gain1 = 70.0;
    }

    VehicleMassEstimator_2018b_DW.running_av =
      ((VehicleMassEstimator_2018b_DW.running_av *
        VehicleMassEstimator_2018b_DW.count) + rtb_Gain1) /
      (VehicleMassEstimator_2018b_DW.count + 1.0);
    VehicleMassEstimator_2018b_DW.count++;
    VehicleMassEstimator_2018b_DW.prev_speed =
      VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2;
    VehicleMassEstimator_2018b_DW.prev_speed_filt =
      VehicleMassEstimator_2018b_DW.HighpassFilter2.HighpassFilter3;
  } else {
    VehicleMassEstimator_2018b_DW.prev_speed =
      VehicleMassEstimator_2018b_DW.LowpassFilter1.LowpassFilter2;
    VehicleMassEstimator_2018b_DW.prev_speed_filt =
      VehicleMassEstimator_2018b_DW.HighpassFilter2.HighpassFilter3;
  }

  if (collect_acc_sample == 1) {
    rtb_Gain1 = error_idx_2 / maxval;
    if (rtb_Gain1 > 140000.0) {
      rtb_Gain1 = 140000.0;
    }

    if (rtb_Gain1 < 70.0) {
      rtb_Gain1 = 70.0;
    }

    VehicleMassEstimator_2018b_DW.running_av_acc =
      (((VehicleMassEstimator_2018b_DW.count_acc_sample - 1.0) *
        VehicleMassEstimator_2018b_DW.running_av_acc) + rtb_Gain1) /
      VehicleMassEstimator_2018b_DW.count_acc_sample;
  }

  if (collect_brake_sample == 1) {
    rtb_Gain1 = error_idx_2 / maxval;
    if (rtb_Gain1 > 140000.0) {
      rtb_Gain1 = 140000.0;
    }

    if (rtb_Gain1 < 70.0) {
      rtb_Gain1 = 70.0;
    }

    VehicleMassEstimator_2018b_DW.running_av_brake =
      (((VehicleMassEstimator_2018b_DW.count_brake_sample - 1.0) *
        VehicleMassEstimator_2018b_DW.running_av_brake) + rtb_Gain1) /
      VehicleMassEstimator_2018b_DW.count_brake_sample;
  }

  if ((((VehicleMassEstimator_2018b_DW.count >= 5000.0) &&
        (VehicleMassEstimator_2018b_DW.count_acc_sample > 2000.0)) &&
       (VehicleMassEstimator_2018b_DW.count_brake_sample > 2000.0)) &&
      (!VehicleMassEstimator_2018b_DW.bitsForTID0.converge_flag)) {
    VehicleMassEstimator_2018b_DW.bitsForTID0.converge_flag = true;
    VehicleMassEstimator_2018b_DW.converged_mass =
      VehicleMassEstimator_2018b_DW.running_av;
  }

  if (VehicleMassEstimator_2018b_DW.bitsForTID0.converge_flag) {
    rtb_Gain1 = VehicleMassEstimator_2018b_DW.converged_mass;
    *rty_VsVSE_b_TrlrMassConv = true;
  } else {
    rtb_Gain1 = VehicleMassEstimator_2018b_DW.running_av;
    *rty_VsVSE_b_TrlrMassConv = false;
  }

  for (collect_brake_sample = 0; collect_brake_sample < 16; collect_brake_sample
       ++) {
    VehicleMassEstimator_2018b_DW.P_ekf[collect_brake_sample] +=
      Q[collect_brake_sample];
  }

  if (VehicleMassEstimator_2018b_DW.running_av != 0.0) {
    diag_hB9Vh4NX_Aptiv(c, R);
    radius_of_curv = VehicleMassEstimator_2018b_DW.running_av -
      VehicleMassEstimator_2018b_DW.x_ekf[0];
    error_idx_1 = (((((real_T)(*rtu_Comp_YawRate_Filtered)) * ((real_T)
      (*rtu_Comp_YawRate_Filtered))) * 1.504) + (((real_T)(*rtu_raw_long_accel))
      - maxval)) - ((9.81 * sin(VehicleMassEstimator_2018b_DW.x_ekf[1])) +
                    VehicleMassEstimator_2018b_DW.x_ekf[2]);
    error_idx_2 -= ((((VehicleMassEstimator_2018b_DW.x_ekf[0] * 9.81) * sin
                      (VehicleMassEstimator_2018b_DW.x_ekf[1])) +
                     (VehicleMassEstimator_2018b_DW.x_ekf[0] * maxval)) -
                    (((((real_T)(*rtu_Comp_YawRate_Filtered)) * ((real_T)
      (*rtu_Comp_YawRate_Filtered))) * VehicleMassEstimator_2018b_DW.x_ekf[0]) *
                     1.504)) + VehicleMassEstimator_2018b_DW.x_ekf[3];
    H[0] = 1.0;
    H[3] = 0.0;
    H[6] = 0.0;
    H[9] = 0.0;
    H[1] = 0.0;
    H[4] = 9.81 * cos(VehicleMassEstimator_2018b_DW.x_ekf[1]);
    H[7] = 1.0;
    H[10] = 0.0;
    H[2] = ((9.81 * sin(VehicleMassEstimator_2018b_DW.x_ekf[1])) + maxval) -
      ((((real_T)(*rtu_Comp_YawRate_Filtered)) * ((real_T)
         (*rtu_Comp_YawRate_Filtered))) * 1.504);
    H[5] = (VehicleMassEstimator_2018b_DW.x_ekf[0] * 9.81) * cos
      (VehicleMassEstimator_2018b_DW.x_ekf[1]);
    H[8] = 0.0;
    H[11] = 1.0;
  } else {
    memset(&R[0], 0, 9U * (sizeof(real_T)));
    R[0] = 1000.0;
    R[4] = 30.0;
    R[8] = 1500.0;
    radius_of_curv = VehicleMassEstimator_2018b_DW.running_av -
      VehicleMassEstimator_2018b_DW.x_ekf[0];
    error_idx_1_tmp = sin(VehicleMassEstimator_2018b_DW.x_ekf[1]);
    error_idx_1_tmp_0 = ((real_T)(*rtu_Comp_YawRate_Filtered)) * ((real_T)
      (*rtu_Comp_YawRate_Filtered));
    a21 = 9.81 * error_idx_1_tmp;
    error_idx_1_tmp_1 = error_idx_1_tmp_0 * 1.504;
    error_idx_1 = (error_idx_1_tmp_1 + (((real_T)(*rtu_raw_long_accel)) - maxval))
      - (a21 + VehicleMassEstimator_2018b_DW.x_ekf[2]);
    error_idx_2 -= ((((VehicleMassEstimator_2018b_DW.x_ekf[0] * 9.81) *
                      error_idx_1_tmp) + (VehicleMassEstimator_2018b_DW.x_ekf[0]
      * maxval)) - ((error_idx_1_tmp_0 * VehicleMassEstimator_2018b_DW.x_ekf[0])
                    * 1.504)) + VehicleMassEstimator_2018b_DW.x_ekf[3];
    H[0] = 1.0;
    H[3] = 0.0;
    H[6] = 0.0;
    H[9] = 0.0;
    H[1] = 0.0;
    error_idx_1_tmp = cos(VehicleMassEstimator_2018b_DW.x_ekf[1]);
    H[4] = 9.81 * error_idx_1_tmp;
    H[7] = 1.0;
    H[10] = 0.0;
    H[2] = (a21 + maxval) - error_idx_1_tmp_1;
    H[5] = (VehicleMassEstimator_2018b_DW.x_ekf[0] * 9.81) * error_idx_1_tmp;
    H[8] = 0.0;
    H[11] = 1.0;
  }

  for (collect_brake_sample = 0; collect_brake_sample < 3; collect_brake_sample
       ++) {
    for (r2 = 0; r2 < 4; r2++) {
      collect_acc_sample = (3 * r2) + collect_brake_sample;
      K[r2 + (collect_brake_sample << 2)] = H[collect_acc_sample];
      r3 = collect_brake_sample + (3 * r2);
      H_0[r3] = 0.0;
      H_tmp = (r2 << 2);
      H_0[r3] = H_0[collect_acc_sample] +
        (VehicleMassEstimator_2018b_DW.P_ekf[H_tmp] * H[collect_brake_sample]);
      H_0[r3] = (VehicleMassEstimator_2018b_DW.P_ekf[H_tmp + 1] *
                 H[collect_brake_sample + 3]) + H_0[collect_acc_sample];
      H_0[r3] = (VehicleMassEstimator_2018b_DW.P_ekf[H_tmp + 2] *
                 H[collect_brake_sample + 6]) + H_0[collect_acc_sample];
      H_0[r3] = (VehicleMassEstimator_2018b_DW.P_ekf[H_tmp + 3] *
                 H[collect_brake_sample + 9]) + H_0[collect_acc_sample];
    }
  }

  for (collect_brake_sample = 0; collect_brake_sample < 3; collect_brake_sample
       ++) {
    for (r2 = 0; r2 < 3; r2++) {
      collect_acc_sample = (collect_brake_sample << 2);
      S[r2 + (3 * collect_brake_sample)] = ((((K[collect_acc_sample + 1] *
        H_0[r2 + 3]) + (K[collect_acc_sample] * H_0[r2])) +
        (K[collect_acc_sample + 2] * H_0[r2 + 6])) + (K[collect_acc_sample + 3] *
        H_0[r2 + 9])) + R[(3 * collect_brake_sample) + r2];
    }

    for (r2 = 0; r2 < 4; r2++) {
      collect_acc_sample = r2 + (collect_brake_sample << 2);
      y[collect_acc_sample] = 0.0;
      r3 = (collect_brake_sample << 2) + r2;
      y[collect_acc_sample] = y[r3] + (K[collect_brake_sample << 2] *
        VehicleMassEstimator_2018b_DW.P_ekf[r2]);
      y[collect_acc_sample] = (K[(collect_brake_sample << 2) + 1] *
        VehicleMassEstimator_2018b_DW.P_ekf[r2 + 4]) + y[r3];
      y[collect_acc_sample] = (K[(collect_brake_sample << 2) + 2] *
        VehicleMassEstimator_2018b_DW.P_ekf[r2 + 8]) + y[r3];
      y[collect_acc_sample] = (K[(collect_brake_sample << 2) + 3] *
        VehicleMassEstimator_2018b_DW.P_ekf[r2 + 12]) + y[r3];
    }
  }

  collect_brake_sample = 0;
  r2 = 1;
  r3 = 2;
  maxval = fabs(S[0]);
  a21 = fabs(S[1]);
  if (a21 > maxval) {
    maxval = a21;
    collect_brake_sample = 1;
    r2 = 0;
  }

  if (fabs(S[2]) > maxval) {
    collect_brake_sample = 2;
    r2 = 1;
    r3 = 0;
  }

  S[r2] /= S[collect_brake_sample];
  S[r3] /= S[collect_brake_sample];
  S[3 + r2] -= S[3 + collect_brake_sample] * S[r2];
  S[3 + r3] -= S[3 + collect_brake_sample] * S[r3];
  S[6 + r2] -= S[6 + collect_brake_sample] * S[r2];
  S[6 + r3] -= S[6 + collect_brake_sample] * S[r3];
  if (fabs(S[3 + r3]) > fabs(S[3 + r2])) {
    collect_acc_sample = r2;
    r2 = r3;
    r3 = collect_acc_sample;
  }

  S[3 + r3] /= S[3 + r2];
  S[6 + r3] -= S[3 + r3] * S[6 + r2];
  for (H_tmp = 0; H_tmp < 4; H_tmp++) {
    collect_acc_sample = (collect_brake_sample << 2);
    K_tmp_3 = H_tmp + collect_acc_sample;
    K[K_tmp_3] = y[H_tmp] / S[collect_brake_sample];
    collect_acc_sample += H_tmp;
    K_tmp = (r2 << 2);
    K_tmp_0 = H_tmp + K_tmp;
    K[K_tmp_0] = y[4 + H_tmp] - (K[collect_acc_sample] * S[3 +
      collect_brake_sample]);
    K_tmp_1 = (r3 << 2);
    K_tmp_2 = H_tmp + K_tmp_1;
    K[K_tmp_2] = y[8 + H_tmp] - (K[collect_acc_sample] * S[6 +
      collect_brake_sample]);
    K_tmp += H_tmp;
    K[K_tmp_0] = K[K_tmp] / S[3 + r2];
    K_tmp_1 += H_tmp;
    K[K_tmp_2] = K[K_tmp_1] - (K[K_tmp] * S[6 + r2]);
    K[K_tmp_2] = K[K_tmp_1] / S[6 + r3];
    K[K_tmp_0] = K[K_tmp] - (K[K_tmp_1] * S[3 + r3]);
    K[K_tmp_3] = K[collect_acc_sample] - (K[K_tmp_1] * S[r3]);
    K[K_tmp_3] = K[collect_acc_sample] - (K[K_tmp] * S[r2]);
    VehicleMassEstimator_2018b_DW.x_ekf[H_tmp] += ((K[H_tmp + 4] * error_idx_1)
      + (K[H_tmp] * radius_of_curv)) + (K[H_tmp + 8] * error_idx_2);
  }

  for (collect_brake_sample = 0; collect_brake_sample < 16; collect_brake_sample
       ++) {
    b_I[collect_brake_sample] = 0;
  }

  b_I[0] = 1;
  b_I[5] = 1;
  b_I[10] = 1;
  b_I[15] = 1;
  for (collect_brake_sample = 0; collect_brake_sample < 4; collect_brake_sample
       ++) {
    for (r2 = 0; r2 < 4; r2++) {
      collect_acc_sample = (r2 << 2);
      b_I_0[collect_brake_sample + collect_acc_sample] = ((real_T)
        b_I[collect_acc_sample + collect_brake_sample]) - (((H[(3 * r2) + 1] *
        K[collect_brake_sample + 4]) + (H[3 * r2] * K[collect_brake_sample])) +
        (H[(3 * r2) + 2] * K[collect_brake_sample + 8]));
    }

    for (r2 = 0; r2 < 4; r2++) {
      collect_acc_sample = (r2 << 2);
      r3 = collect_brake_sample + collect_acc_sample;
      b_I_1[r3] = 0.0;
      H_tmp = collect_acc_sample + collect_brake_sample;
      b_I_1[r3] = b_I_1[H_tmp] +
        (VehicleMassEstimator_2018b_DW.P_ekf[collect_acc_sample] *
         b_I_0[collect_brake_sample]);
      b_I_1[r3] = (VehicleMassEstimator_2018b_DW.P_ekf[collect_acc_sample + 1] *
                   b_I_0[collect_brake_sample + 4]) + b_I_1[H_tmp];
      b_I_1[r3] = (VehicleMassEstimator_2018b_DW.P_ekf[collect_acc_sample + 2] *
                   b_I_0[collect_brake_sample + 8]) + b_I_1[H_tmp];
      b_I_1[r3] = (VehicleMassEstimator_2018b_DW.P_ekf[collect_acc_sample + 3] *
                   b_I_0[collect_brake_sample + 12]) + b_I_1[H_tmp];
    }
  }

  memcpy(&VehicleMassEstimator_2018b_DW.P_ekf[0], &b_I_1[0], (sizeof(real_T)) <<
         4U);
  VehicleMassEstimator_2018b_DW.P_ekf2 += 1.0E-5;
  maxval = (VehicleMassEstimator_2018b_DW.P_ekf2 * diff_speed_filt) /
    (((diff_speed_filt * VehicleMassEstimator_2018b_DW.P_ekf2) * diff_speed_filt)
     + 5000.0);
  VehicleMassEstimator_2018b_DW.x_ekf2 += (mean_radii - (diff_speed_filt *
    VehicleMassEstimator_2018b_DW.x_ekf2)) * maxval;
  VehicleMassEstimator_2018b_DW.P_ekf2 *= 1.0 - (maxval * diff_speed_filt);
  maxval = (VehicleMassEstimator_2018b_DW.P_rls * diff_speed_filt) /
    (((diff_speed_filt * VehicleMassEstimator_2018b_DW.P_rls) * diff_speed_filt)
     + 1.0);
  VehicleMassEstimator_2018b_DW.P_rls -= (maxval * diff_speed_filt) *
    VehicleMassEstimator_2018b_DW.P_rls;
  VehicleMassEstimator_2018b_DW.w_rls += (mean_radii - (diff_speed_filt *
    VehicleMassEstimator_2018b_DW.w_rls)) * maxval;

  // Saturate: '<S1>/Saturation' incorporates:
  //   DataTypeConversion: '<S1>/Data Type Conversion'
  //   MATLAB Function: '<S3>/MassEstimation'

  if (((real32_T)rtb_Gain1) > 1.0E+6F) {
    rtb_Saturation = 1.0E+6F;
  } else if (((real32_T)rtb_Gain1) < 10.0F) {
    rtb_Saturation = 10.0F;
  } else {
    rtb_Saturation = (real32_T)rtb_Gain1;
  }

  // End of Saturate: '<S1>/Saturation'

  // MATLAB Function: '<S1>/ForgettingFactor'
  if (*rty_VsVSE_b_TrlrMassConv) {
    rtb_forgetting_factor = 0.99F;
  } else {
    rtb_forgetting_factor = 0.9999F;
  }

  // End of MATLAB Function: '<S1>/ForgettingFactor'

  // Delay: '<S4>/Delay1'
  if (((int32_T)VehicleMassEstimator_2018b_DW.icLoad) != 0) {
    VehicleMassEstimator_2018b_DW.Delay1_DSTATE = rtb_Saturation;
  }

  // Sum: '<S4>/Add4' incorporates:
  //   Constant: '<S4>/Constant2'
  //   Delay: '<S4>/Delay1'
  //   Product: '<S4>/Multiply2'
  //   Product: '<S4>/Multiply3'
  //   Sum: '<S4>/Add3'

  *rty_VsVSE_Kg_TrlrMass = ((1.0F - rtb_forgetting_factor) * rtb_Saturation) +
    (rtb_forgetting_factor * VehicleMassEstimator_2018b_DW.Delay1_DSTATE);

  // Update for Delay: '<S4>/Delay1'
  VehicleMassEstimator_2018b_DW.icLoad = 0U;
  VehicleMassEstimator_2018b_DW.Delay1_DSTATE = *rty_VsVSE_Kg_TrlrMass;
}

// Constructor
VehicleMassEstimator_2018bModelClass::VehicleMassEstimator_2018bModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
VehicleMassEstimator_2018bModelClass::~VehicleMassEstimator_2018bModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
