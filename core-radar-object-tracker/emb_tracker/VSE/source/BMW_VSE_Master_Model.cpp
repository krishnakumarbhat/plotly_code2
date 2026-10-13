//
// File: BMW_VSE_Master_Model.cpp
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
#include "BMW_VSE_Master_Model.h"
#include "BMW_VSE_Master_Model_private.h"

// Named constants for Chart: '<S4>/Chart'
#define BMW_VSE_Master_Model_IN_Initialize ((uint8_T)1U)
#define BMW_VSE_Master_Model_IN_Running    ((uint8_T)2U)

// Model step function
void BMW_VSE_Master_ModelModelClass::step()
{
   // local block i/o variables
   real32_T rtb_SpeedCompensation_o1;
   real32_T rtb_yawRate;
   real32_T rtb_YawRateEstimationUsingSteeringWheelAngle_o3;
   real32_T rtb_Yaw_Rate_Compensated_Unfiltered;
   real32_T rtb_Yaw_Rate_Compensated_Filtered;
   real32_T rtb_Yaw_Rate_Raw_Bias;
   real32_T rtb_yaw_rate_bias1;
   real32_T rtb_yaw_rate_bias2;
   real32_T rtb_yaw_rate_bias_fast_bias1;
   real32_T rtb_yaw_rate_bias_fast_bias2;
   real32_T rtb_comp_yaw_rate_diff_filt;
   real32_T rtb_yaw_rate_bias_diff;
   real32_T rtb_ignition_time;
   real32_T rtb_curvature_rear_axle;
   real32_T rtb_VCS_long_velocity;
   real32_T rtb_sensor_long_velocity;
   real32_T rtb_VCS_sideslip;
   real32_T rtb_sensor_sideslip;
   real32_T rtb_sideslip_rear_axle;
   real32_T rtb_VCS_lat_velocity;
   real32_T rtb_sensor_lat_velocity;
   real32_T rtb_CurvKalmanFilterC0;
   real32_T rtb_CurvKalmanFilterC1;
   real32_T rtb_Acceleration_VCS_o1;
   real32_T rtb_Acceleration_VCS_o2;
   enum_road_type_T rtb_road_type;
   enum_quality_factor_T rtb_raw_speed_qf_out;
   enum_quality_factor_T rtb_SpeedCompensation_o2;
   enum_quality_factor_T rtb_raw_yaw_rate_qf_out;
   enum_quality_factor_T rtb_Steering_Whl_Angle_qf_out;
   enum_quality_factor_T rtb_qf;
   enum_quality_factor_T rtb_YawRateEstimationUsingSteeringWheelAngle_o4;
   enum_quality_factor_T rtb_Yaw_Rate_Bias_QF;
   enum_quality_factor_T rtb_Yaw_Rate_Compensated_QF;
   enum_quality_factor_T rtb_Acceleration_VCS_o3;
   enum_quality_factor_T rtb_Acceleration_VCS_o4;
   boolean_T rtb_f_Yaw_Rate_Bias_Converged;
   boolean_T rtb_f_stop_bias_converged;
   boolean_T rtb_f_yaw_rate_bias_shift;
   boolean_T rtb_f_yaw_rate_steady;
   boolean_T rtb_f_input_invalid_persistent;
   boolean_T rtb_f_execution_period_error_persistent;
   boolean_T rtb_f_bias_was_accurate;
   uint32_T qY;
   int32_T tmp;

   // Chart: '<S4>/Chart'
   if (((uint32_T)BMW_VSE_Master_Model_DW.bitsForTID0.is_active_c2_BMW_VSE_Master_Model) == 0U)
   {
      BMW_VSE_Master_Model_DW.bitsForTID0.is_active_c2_BMW_VSE_Master_Model = 1;
      BMW_VSE_Master_Model_DW.bitsForTID0.is_c2_BMW_VSE_Master_Model        = BMW_VSE_Master_Model_IN_Initialize;
      BMW_VSE_Master_Model_DW.bitsForTID0.f_initialize_vse                  = true;
      BMW_VSE_Master_Model_Y.VSE_Output.veh_index                           = 0U;
   }
   else if (((uint32_T)BMW_VSE_Master_Model_DW.bitsForTID0.is_c2_BMW_VSE_Master_Model) == BMW_VSE_Master_Model_IN_Initialize)
   {
      if (BMW_VSE_Master_Model_DW.bitsForTID0.f_initialize_vse)
      {
         BMW_VSE_Master_Model_DW.bitsForTID0.is_c2_BMW_VSE_Master_Model = BMW_VSE_Master_Model_IN_Running;
         BMW_VSE_Master_Model_DW.bitsForTID0.f_initialize_vse           = false;
         qY = BMW_VSE_Master_Model_Y.VSE_Output.veh_index + /*MW:OvSatOk*/ 1U;
         if (qY < BMW_VSE_Master_Model_Y.VSE_Output.veh_index)
         {
            qY = MAX_uint32_T;
         }

         BMW_VSE_Master_Model_Y.VSE_Output.veh_index = qY;
      }
   }
   else
   {
      BMW_VSE_Master_Model_DW.bitsForTID0.f_initialize_vse = false;
      qY                                                   = BMW_VSE_Master_Model_Y.VSE_Output.veh_index + /*MW:OvSatOk*/ 1U;
      if (qY < BMW_VSE_Master_Model_Y.VSE_Output.veh_index)
      {
         qY = MAX_uint32_T;
      }

      BMW_VSE_Master_Model_Y.VSE_Output.veh_index = qY;
   }

   // End of Chart: '<S4>/Chart'

   // ModelReference: '<S1>/Speed_PlausibilityCheck' incorporates:
   //   Inport: '<Root>/VCAN_VSE'

   Speed_PlausibilityCheckMDLOBJ3.step(&BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_speed_qf,
                                       &BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_speed_mps, &rtb_raw_speed_qf_out);

   // ModelReference: '<S1>/SpeedCompensation' incorporates:
   //   Constant: '<S1>/default_f_use_gps_comp_factor'
   //   Constant: '<S1>/default_gps_comp_factor'
   //   Inport: '<Root>/Tracker_VSE'
   //   Inport: '<Root>/VCAN_VSE'

   SpeedCompensationMDLOBJ2.step(&BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_speed_mps, &rtb_raw_speed_qf_out,
                                 &BMW_VSE_Master_Model_U.Tracker_VSE_j.VehSpeedCompFac_KA,
                                 &BMW_VSE_Master_Model_U.Tracker_VSE_j.VehSpeedCompFac_KA_QF, &BMW_VSE_Master_Model_ConstP.pooled1,
                                 &BMW_VSE_Master_Model_ConstP.pooled2, &rtb_SpeedCompensation_o1, &rtb_SpeedCompensation_o2);

   // ModelReference: '<S1>/YawRate_PlausibilityCheck' incorporates:
   //   Inport: '<Root>/VCAN_VSE'

   YawRate_PlausibilityCheckMDLOBJ8.step(&BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_yaw_rate_rps,
                                         &BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_yaw_rate_qf, &rtb_raw_yaw_rate_qf_out);

   // ModelReference: '<S1>/SteeringWhlAngle_PlausibilityCheck' incorporates:
   //   Inport: '<Root>/VCAN_VSE'

   SteeringWhlAngle_PlausibilityCheckMDLOBJ4.step(&BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_steering_angle_deg,
                                                  &BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_steering_angle_qf,
                                                  &rtb_Steering_Whl_Angle_qf_out);

   // ModelReference: '<S1>/YawRateEstimationUsingSteeringWheelAngle' incorporates:
   //   Inport: '<Root>/VCAN_VSE'
   //   Inport: '<Root>/Vehicle_Parameters'

   YawRateEstimationUsingSteeringWheelAngleMDLOBJ7.step(
      &rtb_SpeedCompensation_o1, &BMW_VSE_Master_Model_U.VCAN_VSE_p.f_reverse,
      &BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_steering_angle_deg, &rtb_Steering_Whl_Angle_qf_out,
      &BMW_VSE_Master_Model_U.Vehicle_Parameters_f.k_wheel_base,
      &BMW_VSE_Master_Model_U.Vehicle_Parameters_f.k_steering_gear_ratio,
      &BMW_VSE_Master_Model_U.Vehicle_Parameters_f.k_understeer_coefficient, &rtb_yawRate, &rtb_qf,
      &rtb_YawRateEstimationUsingSteeringWheelAngle_o3, &rtb_YawRateEstimationUsingSteeringWheelAngle_o4);

   // ModelReference: '<S1>/YawRateCompensation' incorporates:
   //   Inport: '<Root>/System_Time_Micro_Sec'
   //   Inport: '<Root>/VCAN_VSE'

   YawRateCompensationMDLOBJ6.step(
      &BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_yaw_rate_rps, &rtb_raw_yaw_rate_qf_out, &rtb_yawRate, &rtb_qf,
      &rtb_SpeedCompensation_o1, &rtb_SpeedCompensation_o2, &BMW_VSE_Master_Model_U.VCAN_VSE_p.f_stationary,
      &BMW_VSE_Master_Model_U.System_Time_Micro_Sec, &rtb_Yaw_Rate_Compensated_Unfiltered, &rtb_Yaw_Rate_Compensated_Filtered,
      &rtb_Yaw_Rate_Raw_Bias, &rtb_Yaw_Rate_Bias_QF, &rtb_Yaw_Rate_Compensated_QF, &rtb_f_Yaw_Rate_Bias_Converged,
      &rtb_f_stop_bias_converged, &rtb_yaw_rate_bias1, &rtb_yaw_rate_bias2, &rtb_yaw_rate_bias_fast_bias1,
      &rtb_yaw_rate_bias_fast_bias2, &rtb_comp_yaw_rate_diff_filt, &rtb_yaw_rate_bias_diff, &rtb_road_type,
      &rtb_f_yaw_rate_bias_shift, &rtb_f_yaw_rate_steady, &rtb_f_input_invalid_persistent,
      &rtb_f_execution_period_error_persistent, &rtb_f_bias_was_accurate, &rtb_ignition_time);

   // ModelReference: '<S1>/VehicleCurvatureAndSideslipEstimation' incorporates:
   //   Constant: '<S1>/Constant4'
   //   Inport: '<Root>/System_Time_Micro_Sec'
   //   Inport: '<Root>/VCAN_VSE'
   //   Inport: '<Root>/Vehicle_Parameters'

   VehicleCurvatureAndSideslipEstimationMDLOBJ5.step(
      &BMW_VSE_Master_Model_U.System_Time_Micro_Sec, &rtb_SpeedCompensation_o1, &rtb_Yaw_Rate_Compensated_Filtered,
      &BMW_VSE_Master_Model_U.Vehicle_Parameters_f.k_dist_rear_axle_to_vcs,
      &BMW_VSE_Master_Model_U.Vehicle_Parameters_f.k_vehicle_width, &BMW_VSE_Master_Model_ConstP.pooled2,
      &BMW_VSE_Master_Model_U.Vehicle_Parameters_f.k_rear_cornering_compliance, &BMW_VSE_Master_Model_U.VCAN_VSE_p.f_reverse,
      &rtb_curvature_rear_axle, &rtb_VCS_long_velocity, &rtb_sensor_long_velocity, &rtb_VCS_sideslip, &rtb_sensor_sideslip,
      &rtb_sideslip_rear_axle, &rtb_VCS_lat_velocity, &rtb_sensor_lat_velocity, &rtb_CurvKalmanFilterC0, &rtb_CurvKalmanFilterC1);

   // DataTypeConversion: '<S1>/Data Type Conversion1' incorporates:
   //   Inport: '<Root>/VCAN_VSE'

   BMW_VSE_Master_Model_Y.VSE_Output.stationary = BMW_VSE_Master_Model_U.VCAN_VSE_p.f_stationary ? 1U : 0U;

   // ModelReference: '<S1>/Acceleration_VCS'
   Acceleration_VCSMDLOBJ1.step(&rtb_VCS_lat_velocity, &rtb_VCS_long_velocity, &rtb_Yaw_Rate_Compensated_Filtered,
                                &rtb_SpeedCompensation_o2, &rtb_Yaw_Rate_Compensated_QF, &rtb_Acceleration_VCS_o1,
                                &rtb_Acceleration_VCS_o2, &rtb_Acceleration_VCS_o3, &rtb_Acceleration_VCS_o4);

   // BusCreator: '<S1>/Bus Creator' incorporates:
   //   Inport: '<Root>/System_Time_Micro_Sec'
   //   Inport: '<Root>/Tracker_VSE'
   //   Inport: '<Root>/VCAN_VSE'
   //   Outport: '<Root>/VSE_Output'

   BMW_VSE_Master_Model_Y.VSE_Output.timestamp_us                  = BMW_VSE_Master_Model_U.System_Time_Micro_Sec;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_speed_mps                 = BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_speed_mps;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_speed_qf                  = rtb_raw_speed_qf_out;
   BMW_VSE_Master_Model_Y.VSE_Output.speed_compensation_factor     = BMW_VSE_Master_Model_U.Tracker_VSE_j.VehSpeedCompFac_KA;
   BMW_VSE_Master_Model_Y.VSE_Output.speed_compensation_factor_qf  = BMW_VSE_Master_Model_U.Tracker_VSE_j.VehSpeedCompFac_KA_QF;
   BMW_VSE_Master_Model_Y.VSE_Output.filt_veh_speed_over_ground    = rtb_SpeedCompensation_o1;
   BMW_VSE_Master_Model_Y.VSE_Output.filt_veh_speed_over_ground_qf = rtb_SpeedCompensation_o2;

   // Switch: '<S3>/Switch' incorporates:
   //   Constant: '<S3>/Constant'
   //   Constant: '<S3>/Constant1'
   //   Inport: '<Root>/VCAN_VSE'

   if (BMW_VSE_Master_Model_U.VCAN_VSE_p.f_reverse)
   {
      tmp = -1;
   }
   else
   {
      tmp = 1;
   }

   // End of Switch: '<S3>/Switch'

   // BusCreator: '<S1>/Bus Creator' incorporates:
   //   Inport: '<Root>/VCAN_VSE'
   //   Inport: '<Root>/Vehicle_Parameters'
   //   Outport: '<Root>/VSE_Output'
   //   Product: '<S3>/Product'

   BMW_VSE_Master_Model_Y.VSE_Output.signed_filt_veh_speed_over_ground = rtb_SpeedCompensation_o1 * ((real32_T)tmp);
   BMW_VSE_Master_Model_Y.VSE_Output.raw_lat_accel                     = BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_lat_accel;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_lat_accel_qf                  = BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_lat_accel_qf;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_long_accel                    = BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_long_accel;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_long_accel_qf                 = BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_long_accel_qf;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_yaw_rate_rps                  = BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_yaw_rate_rps;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_yaw_rate_qf                   = rtb_raw_yaw_rate_qf_out;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_steering_angle_deg            = BMW_VSE_Master_Model_U.VCAN_VSE_p.raw_steering_angle_deg;
   BMW_VSE_Master_Model_Y.VSE_Output.raw_steering_angle_qf             = rtb_Steering_Whl_Angle_qf_out;
   BMW_VSE_Master_Model_Y.VSE_Output.road_wheel_angle_deg              = rtb_YawRateEstimationUsingSteeringWheelAngle_o3;
   BMW_VSE_Master_Model_Y.VSE_Output.road_wheel_angle_qf               = rtb_YawRateEstimationUsingSteeringWheelAngle_o4;
   BMW_VSE_Master_Model_Y.VSE_Output.yaw_rate_sa                       = rtb_yawRate;
   BMW_VSE_Master_Model_Y.VSE_Output.yaw_rate_sa_qf                    = rtb_qf;
   BMW_VSE_Master_Model_Y.VSE_Output.yaw_rate_raw_bias                 = rtb_Yaw_Rate_Raw_Bias;
   BMW_VSE_Master_Model_Y.VSE_Output.yaw_rate_bias_qf                  = rtb_Yaw_Rate_Bias_QF;
   BMW_VSE_Master_Model_Y.VSE_Output.comp_yaw_rate_unfiltered          = rtb_Yaw_Rate_Compensated_Unfiltered;
   BMW_VSE_Master_Model_Y.VSE_Output.comp_yaw_rate_filtered            = rtb_Yaw_Rate_Compensated_Filtered;
   BMW_VSE_Master_Model_Y.VSE_Output.comp_yaw_rate_qf                  = rtb_Yaw_Rate_Compensated_QF;
   BMW_VSE_Master_Model_Y.VSE_Output.curvature_rear_axle               = rtb_curvature_rear_axle;
   BMW_VSE_Master_Model_Y.VSE_Output.sideslip_rear_axle                = rtb_sideslip_rear_axle;
   BMW_VSE_Master_Model_Y.VSE_Output.vcs_sideslip                      = rtb_VCS_sideslip;
   BMW_VSE_Master_Model_Y.VSE_Output.vcs_long_velocity                 = rtb_VCS_long_velocity;
   BMW_VSE_Master_Model_Y.VSE_Output.vcs_lat_velocity                  = rtb_VCS_lat_velocity;
   BMW_VSE_Master_Model_Y.VSE_Output.sensor_sideslip                   = rtb_sensor_sideslip;
   BMW_VSE_Master_Model_Y.VSE_Output.sensor_long_velocity              = rtb_sensor_long_velocity;
   BMW_VSE_Master_Model_Y.VSE_Output.sensor_lat_velocity               = rtb_sensor_lat_velocity;
   BMW_VSE_Master_Model_Y.VSE_Output.k_dist_rear_axle_to_vcs = BMW_VSE_Master_Model_U.Vehicle_Parameters_f.k_dist_rear_axle_to_vcs;
   BMW_VSE_Master_Model_Y.VSE_Output.vcs_lat_accel           = rtb_Acceleration_VCS_o1;
   BMW_VSE_Master_Model_Y.VSE_Output.vcs_long_accel          = rtb_Acceleration_VCS_o2;
   BMW_VSE_Master_Model_Y.VSE_Output.vcs_lat_accel_qf        = rtb_Acceleration_VCS_o3;
   BMW_VSE_Master_Model_Y.VSE_Output.vcs_long_accel_qf       = rtb_Acceleration_VCS_o4;

   // Comput Acceleration rear axis
   BMW_VSE_Master_Model_Y.VSE_Output.accel_rear_axle =
      BMW_VSE_Master_Model_Y.VSE_Output.vcs_long_accel +
      (BMW_VSE_Master_Model_Y.VSE_Output.comp_yaw_rate_filtered * BMW_VSE_Master_Model_Y.VSE_Output.comp_yaw_rate_filtered *
       BMW_VSE_Master_Model_Y.VSE_Output.k_dist_rear_axle_to_vcs);

   // Outport: '<Root>/VSE_Resim' incorporates:
   //   BusCreator: '<S1>/Bus Creator1'

   BMW_VSE_Master_Model_Y.VSE_Resim.f_yaw_rate_bias_converged           = rtb_f_stop_bias_converged;
   BMW_VSE_Master_Model_Y.VSE_Resim.f_stop_bias_converged               = rtb_f_Yaw_Rate_Bias_Converged;
   BMW_VSE_Master_Model_Y.VSE_Resim.yaw_rate_bias1                      = rtb_yaw_rate_bias1;
   BMW_VSE_Master_Model_Y.VSE_Resim.yaw_rate_bias2                      = rtb_yaw_rate_bias2;
   BMW_VSE_Master_Model_Y.VSE_Resim.yaw_rate_bias_fast_bias1            = rtb_yaw_rate_bias_fast_bias1;
   BMW_VSE_Master_Model_Y.VSE_Resim.yaw_rate_bias_fast_bias2            = rtb_yaw_rate_bias_fast_bias2;
   BMW_VSE_Master_Model_Y.VSE_Resim.comp_yaw_rate_diff_filt             = rtb_comp_yaw_rate_diff_filt;
   BMW_VSE_Master_Model_Y.VSE_Resim.yaw_rate_bias_diff                  = rtb_yaw_rate_bias_diff;
   BMW_VSE_Master_Model_Y.VSE_Resim.road_type                           = rtb_road_type;
   BMW_VSE_Master_Model_Y.VSE_Resim.f_yaw_rate_bias_shift               = rtb_f_yaw_rate_bias_shift;
   BMW_VSE_Master_Model_Y.VSE_Resim.f_yaw_rate_steady                   = rtb_f_yaw_rate_steady;
   BMW_VSE_Master_Model_Y.VSE_Resim.f_input_invalid_persistent          = rtb_f_input_invalid_persistent;
   BMW_VSE_Master_Model_Y.VSE_Resim.f_execution_period_error_persistent = rtb_f_execution_period_error_persistent;
   BMW_VSE_Master_Model_Y.VSE_Resim.f_bias_was_accurate                 = rtb_f_bias_was_accurate;
   BMW_VSE_Master_Model_Y.VSE_Resim.ignition_time                       = rtb_ignition_time;
   BMW_VSE_Master_Model_Y.VSE_Resim.CurvKalmanFilterC0                  = rtb_CurvKalmanFilterC0;
   BMW_VSE_Master_Model_Y.VSE_Resim.CurvKalmanFilterC1                  = rtb_CurvKalmanFilterC1;
}

// Model initialize function
void BMW_VSE_Master_ModelModelClass::initialize()
{
   // SystemInitialize for ModelReference: '<S1>/VehicleCurvatureAndSideslipEstimation' incorporates:
   //   Constant: '<S1>/Constant4'
   //   Inport: '<Root>/System_Time_Micro_Sec'
   //   Inport: '<Root>/VCAN_VSE'
   //   Inport: '<Root>/Vehicle_Parameters'

   VehicleCurvatureAndSideslipEstimationMDLOBJ5.init();
}

// Constructor
BMW_VSE_Master_ModelModelClass::BMW_VSE_Master_ModelModelClass()
{
   // Currently there is no constructor body generated.
}

// Destructor
BMW_VSE_Master_ModelModelClass::~BMW_VSE_Master_ModelModelClass()
{
   // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
