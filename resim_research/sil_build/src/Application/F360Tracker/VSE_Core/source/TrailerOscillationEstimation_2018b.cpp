//
// File: TrailerOscillationEstimation_2018b.cpp
//
// Code generated for Simulink model 'TrailerOscillationEstimation_2018b'.
//
// Model version                  : 1.406
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:58:25 2024
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
#include "TrailerOscillationEstimation_2018b.h"
#include "TrailerOscillationEstimation_2018b_private.h"

// System initialize for referenced model: 'TrailerOscillationEstimation_2018b'
void TrailerOscillationEstimation_2018bModelClass::init(void)
{
  int32_T i;

  // InitializeConditions for Delay: '<S2>/Delay14'
  for (i = 0; i < 10; i++) {
    TrailerOscillationEstimation_2018b_DW.Delay14_DSTATE[i] = 1.0F;
  }

  // End of InitializeConditions for Delay: '<S2>/Delay14'

  // InitializeConditions for Delay: '<S2>/Delay11'
  TrailerOscillationEstimation_2018b_DW.Delay11_DSTATE = 1.0F;

  // SystemInitialize for MATLAB Function: '<S3>/OscilationDetection'
  TrailerOscillationEstimation_2018b_DW.head = 1U;
}

// Output and update for referenced model: 'TrailerOscillationEstimation_2018b'
void TrailerOscillationEstimation_2018bModelClass::step(const real32_T
  *rtu_HitchAngleRate, real32_T *rty_VsVSE_rps_TrlrOscMag, real32_T
  *rty_VsVSE_hz_TrlrOscFreq)
{
  real32_T cos_w;
  real32_T sin_w;
  uint8_T Xk_mag_max_ind;
  real32_T Xk_mag_max;
  real32_T left_weight;
  real32_T rtb_Delay14[10];
  real32_T rtb_xw_img[10];
  boolean_T rtb_trigger_flag;
  int32_T k;
  uint32_T tmp;
  real32_T rtb_Delay15;

  // MATLAB Function: '<S2>/OscilationDetection1' incorporates:
  //   Delay: '<S2>/Delay11'

  TrailerOscillationEstimation_2018b_DW.Delay11_DSTATE++;
  for (k = 0; k < 10; k++) {
    // Delay: '<S2>/Delay14'
    left_weight = TrailerOscillationEstimation_2018b_DW.Delay14_DSTATE[k];

    // Delay: '<S2>/Delay15'
    rtb_Delay15 = TrailerOscillationEstimation_2018b_DW.Delay15_DSTATE[k];

    // Delay: '<S2>/Delay16'
    Xk_mag_max = TrailerOscillationEstimation_2018b_DW.Delay16_DSTATE[k];

    // MATLAB Function: '<S2>/OscilationDetection1' incorporates:
    //   Constant: '<S2>/ForgettingFactor1'
    //   Constant: '<S2>/k_N_of_Npoint_DFT'
    //   Delay: '<S2>/Delay14'
    //   Delay: '<S2>/Delay15'
    //   Delay: '<S2>/Delay16'
    //   Delay: '<S2>/Delay17'

    TrailerOscillationEstimation_2018b_DW.Delay14_DSTATE[k] = 1.0F;
    TrailerOscillationEstimation_2018b_DW.Delay15_DSTATE[k] = 0.0F;
    TrailerOscillationEstimation_2018b_DW.Delay16_DSTATE[k] = 0.0F;
    sin_w = ((1.0F + ((real32_T)k)) * 6.28318548F) / 1000.0F;
    cos_w = cosf(sin_w);
    sin_w = sinf(sin_w);
    TrailerOscillationEstimation_2018b_DW.Delay14_DSTATE[k] = (cos_w *
      left_weight) - (sin_w * rtb_Delay15);
    TrailerOscillationEstimation_2018b_DW.Delay15_DSTATE[k] = (cos_w *
      rtb_Delay15) + (left_weight * sin_w);
    TrailerOscillationEstimation_2018b_DW.Delay16_DSTATE[k] = (0.999F *
      Xk_mag_max) + ((*rtu_HitchAngleRate) *
                     TrailerOscillationEstimation_2018b_DW.Delay14_DSTATE[k]);
    rtb_xw_img[k] = (0.999F *
                     TrailerOscillationEstimation_2018b_DW.Delay17_DSTATE[k]) +
      ((*rtu_HitchAngleRate) *
       TrailerOscillationEstimation_2018b_DW.Delay15_DSTATE[k]);
  }

  // MATLAB Function: '<S3>/periodic_pulse'
  rtb_trigger_flag = false;
  if (((int32_T)TrailerOscillationEstimation_2018b_DW.timer_count) < 5) {
    k = (int32_T)((uint32_T)(((uint32_T)
      TrailerOscillationEstimation_2018b_DW.timer_count) + 1U));
    if (((uint32_T)k) > 255U) {
      k = 255;
    }

    TrailerOscillationEstimation_2018b_DW.timer_count = (uint8_T)k;
  } else {
    TrailerOscillationEstimation_2018b_DW.timer_count = 1U;
    rtb_trigger_flag = true;
  }

  // End of MATLAB Function: '<S3>/periodic_pulse'

  // MATLAB Function: '<S3>/OscilationDetection' incorporates:
  //   Constant: '<S3>/k_damping_factor_SDFT'
  //   Constant: '<S3>/k_rps_amp_noise_threshold'
  //   Constant: '<S3>/k_sample_time_reduced_buffer'

  rtb_Delay15 = 0.0F;
  if (rtb_trigger_flag) {
    k = ((int32_T)TrailerOscillationEstimation_2018b_DW.head) - 1;
    TrailerOscillationEstimation_2018b_DW.x_n_minus_N_prev =
      TrailerOscillationEstimation_2018b_DW.queue_xn[k];
    TrailerOscillationEstimation_2018b_DW.queue_xn[k] = *rtu_HitchAngleRate;
    tmp = ((uint32_T)((uint16_T)(((uint32_T)
              TrailerOscillationEstimation_2018b_DW.head) - ((uint32_T)
              ((uint16_T)((((uint32_T)TrailerOscillationEstimation_2018b_DW.head)
      / 100U) * 100U)))))) + 1U;
    if (tmp > 65535U) {
      tmp = 65535U;
    }

    TrailerOscillationEstimation_2018b_DW.head = (uint16_T)tmp;
    Xk_mag_max_ind = 1U;
    Xk_mag_max = 0.0F;
    for (k = 0; k < 10; k++) {
      sin_w = ((1.0F + ((real32_T)k)) * 6.28318548F) / 100.0F;
      cos_w = cosf(sin_w);
      left_weight = (((*rtu_HitchAngleRate) - (0.990047693F *
        TrailerOscillationEstimation_2018b_DW.x_n_minus_N_prev)) + ((1.9998F *
        cos_w) * TrailerOscillationEstimation_2018b_DW.s_n_real_prev[k])) -
        (0.999799967F *
         TrailerOscillationEstimation_2018b_DW.s_n_real_prev_prev[k]);
      cos_w = left_weight - ((0.9999F * cos_w) *
        TrailerOscillationEstimation_2018b_DW.s_n_real_prev[k]);
      sin_w = (sinf(sin_w) * -0.9999F) *
        TrailerOscillationEstimation_2018b_DW.s_n_real_prev[k];
      TrailerOscillationEstimation_2018b_DW.Xk_mag_prev[k] = sqrtf((cos_w *
        cos_w) + (sin_w * sin_w));
      if ((TrailerOscillationEstimation_2018b_DW.Xk_mag_prev[k] > Xk_mag_max) &&
          ((1 + k) <= 5)) {
        Xk_mag_max = TrailerOscillationEstimation_2018b_DW.Xk_mag_prev[k];
        Xk_mag_max_ind = (uint8_T)((int32_T)(1 + k));
      }

      rtb_Delay14[k] = left_weight;
    }

    for (k = 0; k < 5; k++) {
      rtb_Delay15 += TrailerOscillationEstimation_2018b_DW.Xk_mag_prev[k] *
        TrailerOscillationEstimation_2018b_DW.Xk_mag_prev[k];
    }

    rtb_Delay15 = (2.0F * sqrtf(rtb_Delay15)) / 100.0F;
    left_weight = 0.0F;
    sin_w = 0.0F;
    if (((int32_T)Xk_mag_max_ind) != 1) {
      left_weight = TrailerOscillationEstimation_2018b_DW.Xk_mag_prev[((int32_T)
        Xk_mag_max_ind) - 2];
    }

    if (((int32_T)Xk_mag_max_ind) != 5) {
      sin_w = TrailerOscillationEstimation_2018b_DW.Xk_mag_prev[Xk_mag_max_ind];
    }

    cos_w = (left_weight + Xk_mag_max) + sin_w;
    TrailerOscillationEstimation_2018b_DW.freq_estim_prev = ((((((((real32_T)
      Xk_mag_max_ind) - 1.0F) / 100.0F) / 0.05F) * left_weight) / cos_w) +
      ((((((real32_T)Xk_mag_max_ind) / 100.0F) / 0.05F) * Xk_mag_max) / cos_w))
      + (((((((real32_T)Xk_mag_max_ind) + 1.0F) / 100.0F) / 0.05F) * sin_w) /
         cos_w);
    if (rtb_Delay15 < 0.003F) {
      TrailerOscillationEstimation_2018b_DW.freq_estim_prev = 0.0F;
    }

    for (k = 0; k < 10; k++) {
      TrailerOscillationEstimation_2018b_DW.s_n_real_prev_prev[k] =
        TrailerOscillationEstimation_2018b_DW.s_n_real_prev[k];
      TrailerOscillationEstimation_2018b_DW.s_n_real_prev[k] = rtb_Delay14[k];
    }

    TrailerOscillationEstimation_2018b_DW.amplitude_estim_prev = rtb_Delay15;
  } else {
    rtb_Delay15 = TrailerOscillationEstimation_2018b_DW.amplitude_estim_prev;
  }

  // Switch: '<S1>/Switch' incorporates:
  //   MATLAB Function: '<S3>/OscilationDetection'

  *rty_VsVSE_rps_TrlrOscMag = rtb_Delay15;

  // Switch: '<S1>/Switch1' incorporates:
  //   MATLAB Function: '<S3>/OscilationDetection'

  *rty_VsVSE_hz_TrlrOscFreq =
    TrailerOscillationEstimation_2018b_DW.freq_estim_prev;

  // Update for Delay: '<S2>/Delay17'
  for (k = 0; k < 10; k++) {
    TrailerOscillationEstimation_2018b_DW.Delay17_DSTATE[k] = rtb_xw_img[k];
  }

  // End of Update for Delay: '<S2>/Delay17'
}

// Constructor
TrailerOscillationEstimation_2018bModelClass::
  TrailerOscillationEstimation_2018bModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
TrailerOscillationEstimation_2018bModelClass::
  ~TrailerOscillationEstimation_2018bModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
