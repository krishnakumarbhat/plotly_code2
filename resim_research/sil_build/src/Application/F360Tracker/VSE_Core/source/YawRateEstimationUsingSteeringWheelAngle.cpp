//
// File: YawRateEstimationUsingSteeringWheelAngle.cpp
//
// Code generated for Simulink model 'YawRateEstimationUsingSteeringWheelAngle'.
//
// Model version                  : 1.141
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:59:40 2024
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
#include "YawRateEstimationUsingSteeringWheelAngle.h"
#include "YawRateEstimationUsingSteeringWheelAngle_private.h"
#include "look1_iflf_binlx_Aptiv.h"

// Named constants for Chart: '<S1>/Chart'
#define YawRateEstimationUsingSteeringWheelAngle_IN_Initialize ((uint8_T)1U)
#define YawRateEstimationUsingSteeringWheelAngle_IN_Running ((uint8_T)2U)

// Output and update for referenced model: 'YawRateEstimationUsingSteeringWheelAngle'
void YawRateEstimationUsingSteeringWheelAngleModelClass::step(const real32_T
  *rtu_filt_veh_speed_over_ground, const boolean_T *rtu_f_reverse, const
  real32_T *rtu_comp_steering_angle_deg, const enum_quality_factor_T
  *rtu_comp_steering_angle_qf, const real32_T *rtu_k_wheel_base, const real32_T *
  rtu_steering_ratio_on_center, const real32_T *rtu_k_understeer_coefficient,
  real32_T *rty_yaw_rate_sa, enum_quality_factor_T *rty_yaw_rate_sa_qf, real32_T
  *rty_road_wheel_angle_deg, enum_quality_factor_T *rty_road_wheel_angle_qf)
{
  real32_T k;
  real32_T rtb_Product;
  static const yawrate_est_str_T tmp = { 0.0F,// yawRate
    0U                                 // qf
  };

  static const host_veh_T tmp_0 = { 2.84988F,// vehicle_base
    14.8F                              // steering_gear_ratio
  };

  static const yaw_est_cals_T tmp_1 = { 0.0053F,// k_understeer_coefficient
    0.005F,                            // k_execution_time
    0.00125F,                          // k_SA_time_constant
    0.5F,                              // k_SA_min_speed
    1.745F,                            // k_SA_max_yaw
    2,                                 // k_yawEst_version_main
    1                                  // k_yawEst_version_sub
  };

  int32_T rtu_f_reverse_0;

  // Switch: '<S1>/Switch1' incorporates:
  //   Constant: '<S1>/Constant1'
  //   Constant: '<S1>/Constant2'

  if (*rtu_f_reverse) {
    rtu_f_reverse_0 = -1;
  } else {
    rtu_f_reverse_0 = 1;
  }

  // End of Switch: '<S1>/Switch1'

  // Product: '<S1>/Product'
  rtb_Product = (*rtu_filt_veh_speed_over_ground) * ((real32_T)rtu_f_reverse_0);

  // Inport: '<Root>/comp_steering_angle_qf'
  *rty_road_wheel_angle_qf = *rtu_comp_steering_angle_qf;

  // Chart: '<S1>/Chart'
  if (((uint32_T)
       YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.is_active_c6_YawRateEstimationUsingSteeringWheelAngle)
      == 0U) {
    YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.is_active_c6_YawRateEstimationUsingSteeringWheelAngle
      = 1;
    YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.is_c6_YawRateEstimationUsingSteeringWheelAngle
      = YawRateEstimationUsingSteeringWheelAngle_IN_Initialize;
    YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.f_initialize_yaw_rate_estimation
      = true;
  } else if (((uint32_T)
              YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.is_c6_YawRateEstimationUsingSteeringWheelAngle)
             == YawRateEstimationUsingSteeringWheelAngle_IN_Initialize) {
    if (YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.f_initialize_yaw_rate_estimation)
    {
      YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.is_c6_YawRateEstimationUsingSteeringWheelAngle
        = YawRateEstimationUsingSteeringWheelAngle_IN_Running;
      YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.f_initialize_yaw_rate_estimation
        = false;
    }
  } else {
    YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.f_initialize_yaw_rate_estimation
      = false;
  }

  // End of Chart: '<S1>/Chart'

  // Outputs for Enabled SubSystem: '<S1>/Initialization' incorporates:
  //   EnablePort: '<S5>/Enable'

  // Switch: '<S1>/Switch' incorporates:
  //   Switch: '<S1>/Switch2'
  //   Switch: '<S1>/Switch3'
  //   UnitDelay: '<S1>/Unit Delay'
  //   UnitDelay: '<S1>/Unit Delay2'

  if (YawRateEstimationUsingSteeringWheelAngle_DW.bitsForTID0.f_initialize_yaw_rate_estimation)
  {
    // MATLAB Function: '<S5>/call_defYawEstStructures'
    memcpy(&YawRateEstimationUsingSteeringWheelAngle_DW.yawrate_est_str, &tmp,
           sizeof(yawrate_est_str_T));
    YawRateEstimationUsingSteeringWheelAngle_DW.host_veh = tmp_0;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals = tmp_1;

    // MATLAB Function: '<S5>/call_initializeYawEstStruct'
    YawRateEstimationUsingSteeringWheelAngle_DW.host_veh.vehicle_base =
      *rtu_k_wheel_base;
    YawRateEstimationUsingSteeringWheelAngle_DW.host_veh.steering_gear_ratio =
      *rtu_steering_ratio_on_center;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals.k_understeer_coefficient
      = *rtu_k_understeer_coefficient;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals.k_execution_time =
      0.01F;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals.k_SA_time_constant =
      0.00125F;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals.k_SA_min_speed =
      0.5F;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals.k_SA_max_yaw =
      1.745F;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals.k_yawEst_version_main
      = 2;
    YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals.k_yawEst_version_sub
      = 1;
    YawRateEstimationUsingSteeringWheelAngle_DW.yawrate_est_str.yawRate = 0.0F;
    YawRateEstimationUsingSteeringWheelAngle_DW.yawrate_est_str.qf =
      TEMP_UNDEFINED;
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE =
      YawRateEstimationUsingSteeringWheelAngle_DW.yawrate_est_str;
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay3_DSTATE =
      YawRateEstimationUsingSteeringWheelAngle_DW.host_veh;
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE =
      YawRateEstimationUsingSteeringWheelAngle_DW.yaw_est_cals;
  }

  // End of Switch: '<S1>/Switch'
  // End of Outputs for SubSystem: '<S1>/Initialization'

  // Lookup_n-D: '<S4>/RoadWheelAngle_vs_SteeringWheelAngle'
  *rty_road_wheel_angle_deg = look1_iflf_binlx_Aptiv
    (*rtu_comp_steering_angle_deg,
     YawRateEstimationUsingSteeringWheelAngle_ConstP.RoadWheelAngle_vs_SteeringWheelAngle_bp01Data,
     YawRateEstimationUsingSteeringWheelAngle_ConstP.RoadWheelAngle_vs_SteeringWheelAngle_tableData,
     30U);

  // MATLAB Function: '<S1>/MATLAB Function' incorporates:
  //   Gain: '<S3>/Gain1'
  //   UnitDelay: '<S1>/Unit Delay'
  //   UnitDelay: '<S1>/Unit Delay2'
  //   UnitDelay: '<S1>/Unit Delay3'

  if ((fabsf
       (YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay3_DSTATE.steering_gear_ratio)
       <= 1.0E-6F) || (fabsf
                       (YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay3_DSTATE.vehicle_base)
                       < 1.0E-6F)) {
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.yawRate = 0.0F;
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.qf = UNDEFINED;
  } else {
    k =
      YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_execution_time
      /
      (YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_execution_time
       + YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_SA_time_constant);
    k = ((1.0F - k) *
         YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.yawRate) +
      (k * (((0.0174532924F * (*rtu_comp_steering_angle_deg)) * rtb_Product) /
            (((YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay3_DSTATE.steering_gear_ratio
               * YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_understeer_coefficient)
              * (rtb_Product * rtb_Product)) +
             (YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay3_DSTATE.steering_gear_ratio
              * YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay3_DSTATE.vehicle_base))));
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.yawRate = k;
    if (((uint32_T)(*rty_road_wheel_angle_qf)) == ACCURATED) {
      if (fabsf(rtb_Product) >=
          YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_SA_min_speed)
      {
        if (fabsf(k) <=
            YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_SA_max_yaw)
        {
          YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.qf =
            ACCURATED;
        } else {
          YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.qf =
            NOT_ACCURATED;
        }
      } else {
        YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.qf =
          NOT_ACCURATED;
      }
    } else {
      YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.qf =
        *rty_road_wheel_angle_qf;
    }

    if (k >
        YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_SA_max_yaw)
    {
      YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.yawRate =
        YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_SA_max_yaw;
    } else {
      if (k <
          (-YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_SA_max_yaw))
      {
        YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.yawRate =
          -YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay2_DSTATE.k_SA_max_yaw;
      }
    }
  }

  // End of MATLAB Function: '<S1>/MATLAB Function'

  // SignalConversion: '<Root>/TmpSignal ConversionAtyaw_rate_saInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay'

  *rty_yaw_rate_sa =
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.yawRate;

  // SignalConversion: '<Root>/TmpSignal ConversionAtyaw_rate_sa_qfInport1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay'

  *rty_yaw_rate_sa_qf =
    YawRateEstimationUsingSteeringWheelAngle_DW.UnitDelay_DSTATE.qf;
}

// Constructor
YawRateEstimationUsingSteeringWheelAngleModelClass::
  YawRateEstimationUsingSteeringWheelAngleModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
YawRateEstimationUsingSteeringWheelAngleModelClass::
  ~YawRateEstimationUsingSteeringWheelAngleModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
