//
// File: VehicleCurvatureAndSideslipEstimation_Simulink.cpp
//
// Code generated for Simulink model 'VehicleCurvatureAndSideslipEstimation_Simulink'.
//
// Model version                  : 1.484
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 18:58:38 2024
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
#include "VehicleCurvatureAndSideslipEstimation_Simulink.h"
#include "VehicleCurvatureAndSideslipEstimation_Simulink_private.h"
#include "LookUp_real32_T_real32_T_Aptiv.h"

// Named constants for Chart: '<S3>/trigger for init'
#define VehicleCurvatureAndSideslipEstimation_Simulink_IN_Trigger ((uint8_T)1U)
#define VehicleCurvatureAndSideslipEstimation_Simulink_IN_disable ((uint8_T)2U)

// System initialize for referenced model: 'VehicleCurvatureAndSideslipEstimation_Simulink'
void VehicleCurvatureAndSideslipEstimation_SimulinkModelClass::init(void)
{
  // Chart: '<S3>/trigger for init'
  VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.is_c9_VehicleCurvatureAndSideslipEstimation_Simulink
    = VehicleCurvatureAndSideslipEstimation_Simulink_IN_Trigger;
}

// Output and update for referenced model: 'VehicleCurvatureAndSideslipEstimation_Simulink'
void VehicleCurvatureAndSideslipEstimation_SimulinkModelClass::step(const
  uint64_T *rtu_system_timer_get_64bit_current_value, const real32_T
  *rtu_veh_speed_compensated, const real32_T *rtu_comp_yaw_rate_filtered, const
  real32_T *rtu_k_dist_front_to_rear_axle, const real32_T
  *rtu_k_vehicleCfg_width, const boolean_T *rtu_var_rr_corner_comp_true, const
  real32_T *rtu_k_rear_cornering_compliance, const boolean_T *rtu_f_reverse,
  real32_T *rty_curvature_rear_axle, real32_T *rty_VCS_long_velocity, real32_T
  *rty_sensor_long_velocity, real32_T *rty_VCS_sideslip, real32_T
  *rty_sensor_sideslip, real32_T *rty_sideslip_rear_axle, real32_T
  *rty_VCS_lat_velocity, real32_T *rty_sensor_lat_velocity, real32_T
  *rty_CurvKalmanFilterC0, real32_T *rty_CurvKalmanFilterC1)
{
  // local block i/o variables
  real32_T rtb_LookupTableDynamic;
  real32_T x[6];
  int32_T iyz;
  real32_T s;
  real32_T ylast;
  int32_T k;
  real32_T rtb_Product;
  int32_T i;
  uint32_T qY;
  real32_T Memory_PreviousInput_tmp;

  // Switch: '<S1>/Switch' incorporates:
  //   Constant: '<S1>/Constant'
  //   Constant: '<S1>/Constant1'

  if (*rtu_f_reverse) {
    i = -1;
  } else {
    i = 1;
  }

  // End of Switch: '<S1>/Switch'

  // Product: '<S1>/Product'
  rtb_Product = (*rtu_veh_speed_compensated) * ((real32_T)i);

  // Chart: '<S3>/trigger for init'
  if (((uint32_T)
       VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.is_c9_VehicleCurvatureAndSideslipEstimation_Simulink)
      == VehicleCurvatureAndSideslipEstimation_Simulink_IN_Trigger) {
    if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.initialization)
    {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.is_c9_VehicleCurvatureAndSideslipEstimation_Simulink
        = VehicleCurvatureAndSideslipEstimation_Simulink_IN_disable;
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.initialization
        = false;
    } else {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.initialization
        = true;
    }
  }

  // End of Chart: '<S3>/trigger for init'

  // Outputs for Enabled SubSystem: '<S3>/Init' incorporates:
  //   EnablePort: '<S6>/Enable'

  if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.initialization)
  {
    // MATLAB Function: '<S6>/Variable sidelsip values generation' incorporates:
    //   BusCreator: '<S4>/Bus Creator2'

    for (i = 0; i < 6; i++) {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.lat_acc[i] =
        VehicleCurvatureAndSideslipEstimation_Simulink_ConstB.lat_acc[i];
      x[i] = VehicleCurvatureAndSideslipEstimation_Simulink_DW.lat_acc[i];
    }

    for (i = 0; i < 5; i++) {
      x[i] = x[i + 1] - x[i];
    }

    s = 0.0F;
    i = -1;
    iyz = 0;
    ylast = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.rear_slip_ang[0] = 0.0F;
    for (k = 0; k < 5; k++) {
      iyz++;
      i++;
      s += ((ylast +
             VehicleCurvatureAndSideslipEstimation_Simulink_ConstB.rr_corner_comp
             [iyz]) / 2.0F) * x[i];
      ylast =
        VehicleCurvatureAndSideslipEstimation_Simulink_ConstB.rr_corner_comp[iyz];
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.rear_slip_ang[iyz] = s;
    }

    // End of MATLAB Function: '<S6>/Variable sidelsip values generation'

    // MATLAB Function: '<S6>/vehicle state struct init'
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_rear_axle_to_vcs
      = *rtu_k_dist_front_to_rear_axle;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_radar_to_rear_axle
      = *rtu_k_dist_front_to_rear_axle;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.vcs_radar_lat
      = (*rtu_k_vehicleCfg_width) / 2.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.dist_based_curv_speed_thresh
      = 0.05F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.dist_based_curv_discrete_distance_interval
      = 0.25F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.curv_kalman_gain_1
      = 0.245100394F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.curv_kalman_gain_2
      = 0.137377188F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.delta_T
      = 0.01F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.DistBasedCurv =
      0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.CurvDist = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.CurvIntegYaw =
      0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.CurvKalmanFilterC0
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.CurvKalmanFilterC1
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.curvature_rear_axle
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.sideslip_rear_axle
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.VCS_sideslip =
      0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.sensor_sideslip =
      0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.prev_time =
      *rtu_system_timer_get_64bit_current_value;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.VCS_long_velocity
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.VCS_lat_velocity
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.sensor_long_velocity
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.sensor_lat_velocity
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state.f_CurvKalmanFilterInitialized
      = false;
  }

  // End of Outputs for SubSystem: '<S3>/Init'

  // MATLAB Function: '<S3>/MATLAB Function3'
  if (!VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.time_step_number_not_empty)
  {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.time_step_number = 0U;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.bitsForTID0.time_step_number_not_empty
      = true;
  } else {
    qY = VehicleCurvatureAndSideslipEstimation_Simulink_DW.time_step_number +
      /*MW:OvSatOk*/ 1U;
    if (qY < VehicleCurvatureAndSideslipEstimation_Simulink_DW.time_step_number)
    {
      qY = MAX_uint32_T;
    }

    VehicleCurvatureAndSideslipEstimation_Simulink_DW.time_step_number = qY;
  }

  // Switch: '<S3>/Switch' incorporates:
  //   MATLAB Function: '<S3>/MATLAB Function3'

  if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.time_step_number == 0U)
  {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput =
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_state;
  }

  // End of Switch: '<S3>/Switch'

  // MATLAB Function: '<S3>/RearAxleCurvature' incorporates:
  //   BusCreator: '<S1>/Bus Creator'

  s = ((real32_T)((uint64_T)((*rtu_system_timer_get_64bit_current_value) -
         VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.prev_time)))
    * 1.0E-6F;
  VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.prev_time
    = *rtu_system_timer_get_64bit_current_value;
  if ((s < (0.5F *
            VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.delta_T))
      || (s > (6.0F *
               VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.delta_T)))
  {
    s =
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.delta_T;
  }

  VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvDist
    += rtb_Product * s;
  if (fabsf(rtb_Product) >
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.dist_based_curv_speed_thresh)
  {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvIntegYaw
      += (*rtu_comp_yaw_rate_filtered) * s;
  }

  if (fabsf
      (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvDist)
      >=
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.dist_based_curv_discrete_distance_interval)
  {
    s =
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvIntegYaw
      /
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvDist;
    if (fabsf(s) > 0.2F) {
      if (s < 0.0F) {
        s = -1.0F;
      } else {
        if (s > 0.0F) {
          s = 1.0F;
        }
      }

      s *= 0.2F;
    }

    if (!VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.f_CurvKalmanFilterInitialized)
    {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC0
        = s;
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC1
        = 0.0F;
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.f_CurvKalmanFilterInitialized
        = true;
    } else {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC0
        +=
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvDist
        * VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC1;
      s -=
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC0;
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC0
        +=
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.curv_kalman_gain_1
        * s;
      if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvDist
          >= 0.0F) {
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC1
          +=
          VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.curv_kalman_gain_2
          * s;
      } else {
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC1
          -=
          VehicleCurvatureAndSideslipEstimation_Simulink_DW.curvature_calibrations.curv_kalman_gain_2
          * s;
      }
    }

    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvDist
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvIntegYaw
      = 0.0F;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle
      =
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC0;
  }

  // End of MATLAB Function: '<S3>/RearAxleCurvature'

  // If: '<S10>/If'
  if (*rtu_var_rr_corner_comp_true) {
    // Outputs for IfAction SubSystem: '<S10>/SideSlip Table' incorporates:
    //   ActionPort: '<S15>/Action Port'

    // Product: '<S15>/Divide' incorporates:
    //   BusCreator: '<S1>/Bus Creator'

    s = (rtb_Product * rtb_Product) *
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle;

    // Signum: '<S15>/Sign1'
    if (s < 0.0F) {
      ylast = -1.0F;
    } else if (s > 0.0F) {
      ylast = 1.0F;
    } else {
      ylast = s;
    }

    // End of Signum: '<S15>/Sign1'

    // Abs: '<S15>/Abs'
    s = fabsf(s);

    // MinMax: '<S15>/MinMax'
    rtb_LookupTableDynamic =
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.lat_acc[0];
    for (i = 0; i < 5; i++) {
      rtb_LookupTableDynamic = fmaxf(rtb_LookupTableDynamic,
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.lat_acc[i + 1]);
    }

    // End of MinMax: '<S15>/MinMax'

    // Switch: '<S15>/Switch2' incorporates:
    //   RelationalOperator: '<S15>/Relational Operator'

    if (s > rtb_LookupTableDynamic) {
      s = rtb_LookupTableDynamic;
    }

    // End of Switch: '<S15>/Switch2'

    // S-Function (sfix_look1_dyn): '<S15>/Lookup Table Dynamic'
    // Dynamic Look-Up Table Block: '<S15>/Lookup Table Dynamic'
    //  Input0  Data Type:  Floating Point real32_T
    //  Input1  Data Type:  Floating Point real32_T
    //  Input2  Data Type:  Floating Point real32_T
    //  Output0 Data Type:  Floating Point real32_T
    //  Lookup Method: Linear_Endpoint
    //

    LookUp_real32_T_real32_T_Aptiv( &(rtb_LookupTableDynamic),
      &VehicleCurvatureAndSideslipEstimation_Simulink_DW.rear_slip_ang[0], s,
      &VehicleCurvatureAndSideslipEstimation_Simulink_DW.lat_acc[0], 5U);

    // Signum: '<S15>/Sign' incorporates:
    //   BusCreator: '<S1>/Bus Creator'

    if (rtb_Product < 0.0F) {
      s = -1.0F;
    } else if (rtb_Product > 0.0F) {
      s = 1.0F;
    } else {
      s = rtb_Product;
    }

    // End of Signum: '<S15>/Sign'

    // Switch: '<S15>/Switch' incorporates:
    //   Constant: '<S15>/Constant'
    //   Constant: '<S16>/Constant'
    //   Product: '<S15>/Divide1'
    //   RelationalOperator: '<S16>/Compare'
    //   Sum: '<S15>/Add'

    if (s >= 0.0F) {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Switch =
        (rtb_LookupTableDynamic * s) * ylast;
    } else {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Switch =
        ((rtb_LookupTableDynamic * s) * ylast) + 3.14159274F;
    }

    // End of Switch: '<S15>/Switch'
    // End of Outputs for SubSystem: '<S10>/SideSlip Table'
  }

  // End of If: '<S10>/If'

  // SignalConversion: '<Root>/TmpSignal ConversionAtCurvKalmanFilterC0Inport1'
  *rty_CurvKalmanFilterC0 =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC0;

  // SignalConversion: '<Root>/TmpSignal ConversionAtCurvKalmanFilterC1Inport1'
  *rty_CurvKalmanFilterC1 =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.CurvKalmanFilterC1;

  // MATLAB Function: '<S10>/MATLAB Function4' incorporates:
  //   BusCreator: '<S1>/Bus Creator'

  if (*rtu_var_rr_corner_comp_true) {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle
      = VehicleCurvatureAndSideslipEstimation_Simulink_DW.Switch;
  } else if (rtb_Product >= 0.0F) {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle
      =
      -(((VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle
          * rtb_Product) * rtb_Product) * (*rtu_k_rear_cornering_compliance));
  } else {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle
      =
      (((VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle
         * rtb_Product) * rtb_Product) * (*rtu_k_rear_cornering_compliance)) +
      3.14159274F;
  }

  // End of MATLAB Function: '<S10>/MATLAB Function4'

  // MATLAB Function: '<S3>/ComputeSideslip' incorporates:
  //   BusCreator: '<S1>/Bus Creator'

  if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle
      > 3.14159274F) {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle
      -= 6.28318548F;
  } else {
    if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle
        < -3.14159274F) {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle
        += 6.28318548F;
    }
  }

  s = cosf
    (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle);
  ylast = sinf
    (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle);
  if (rtb_Product >= 0.0F) {
    Memory_PreviousInput_tmp =
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle
      * VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_rear_axle_to_vcs;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_sideslip
      = atan2f(Memory_PreviousInput_tmp * s, (Memory_PreviousInput_tmp * ylast)
               + 1.0F) +
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_sideslip
      = atan2f
      (((VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_radar_to_rear_axle
         * s) +
        (VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.vcs_radar_lat
         * ylast)) *
       VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle,
       (((VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_radar_to_rear_axle
          * ylast) -
         (VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.vcs_radar_lat
          * s)) *
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle)
       + 1.0F) +
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle;
  } else {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_sideslip
      = atan2f
      (((-VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle)
        * VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_rear_axle_to_vcs)
       * s, 1.0F -
       ((VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle
         * VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_rear_axle_to_vcs)
        * ylast)) +
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle;
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_sideslip
      = atan2f
      (((VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_radar_to_rear_axle
         * s) +
        (VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.vcs_radar_lat
         * ylast)) *
       (-VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle),
       1.0F -
       (((VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_radar_to_rear_axle
          * ylast) -
         (VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.vcs_radar_lat
          * s)) *
        VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle))
      + VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle;
  }

  if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_sideslip
      > 3.14159274F) {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_sideslip
      -= 6.28318548F;
  } else {
    if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_sideslip
        < -3.14159274F) {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_sideslip
        += 6.28318548F;
    }
  }

  if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_sideslip
      > 3.14159274F) {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_sideslip
      -= 6.28318548F;
  } else {
    if (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_sideslip
        < -3.14159274F) {
      VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_sideslip
        += 6.28318548F;
    }
  }

  if (fabsf
      (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle)
      < 1.57079637F) {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_long_velocity
      = rtb_Product * cosf
      (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle);
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_lat_velocity
      = (rtb_Product * sinf
         (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle))
      + ((*rtu_comp_yaw_rate_filtered) *
         VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_rear_axle_to_vcs);
  } else {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_long_velocity
      = (-rtb_Product) * cosf
      (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle);
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_lat_velocity
      = (rtb_Product * sinf
         (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle))
      + ((*rtu_comp_yaw_rate_filtered) *
         VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_rear_axle_to_vcs);
  }

  if (fabsf
      (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle)
      < 1.57079637F) {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_long_velocity
      = rtb_Product * cosf
      (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle);
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_lat_velocity
      = (rtb_Product * sinf
         (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle))
      + ((*rtu_comp_yaw_rate_filtered) *
         VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_radar_to_rear_axle);
  } else {
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_long_velocity
      = (-rtb_Product) * cosf
      (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle);
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_lat_velocity
      = (rtb_Product * sinf
         (VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle))
      + ((*rtu_comp_yaw_rate_filtered) *
         VehicleCurvatureAndSideslipEstimation_Simulink_DW.veh_calibrations.dist_radar_to_rear_axle);
  }

  // End of MATLAB Function: '<S3>/ComputeSideslip'

  // SignalConversion: '<Root>/TmpSignal ConversionAtVCS_lat_velocityInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_VCS_lat_velocity =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_lat_velocity;

  // SignalConversion: '<Root>/TmpSignal ConversionAtVCS_long_velocityInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_VCS_long_velocity =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_long_velocity;

  // SignalConversion: '<Root>/TmpSignal ConversionAtVCS_sideslipInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_VCS_sideslip =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.VCS_sideslip;

  // SignalConversion: '<Root>/TmpSignal ConversionAtcurvature_rear_axleInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_curvature_rear_axle =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.curvature_rear_axle;

  // SignalConversion: '<Root>/TmpSignal ConversionAtsensor_lat_velocityInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_sensor_lat_velocity =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_lat_velocity;

  // SignalConversion: '<Root>/TmpSignal ConversionAtsensor_long_velocityInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_sensor_long_velocity =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_long_velocity;

  // SignalConversion: '<Root>/TmpSignal ConversionAtsensor_sideslipInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_sensor_sideslip =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sensor_sideslip;

  // SignalConversion: '<Root>/TmpSignal ConversionAtsideslip_rear_axleInport1' incorporates:
  //   BusCreator: '<S3>/Bus Creator1'

  *rty_sideslip_rear_axle =
    VehicleCurvatureAndSideslipEstimation_Simulink_DW.Memory_PreviousInput.sideslip_rear_axle;
}

// Constructor
VehicleCurvatureAndSideslipEstimation_SimulinkModelClass::
  VehicleCurvatureAndSideslipEstimation_SimulinkModelClass()
{
  // Currently there is no constructor body generated.
}

// Destructor
VehicleCurvatureAndSideslipEstimation_SimulinkModelClass::
  ~VehicleCurvatureAndSideslipEstimation_SimulinkModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
