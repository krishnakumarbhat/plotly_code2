/*===================================================================================*\
* FILE: f360_input_diagnostics.cpp
* ====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* Implementation of a class responsible for diagnostics and fault detection of tracker
*   input signals.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


/******************************
* Includes
*******************************/
#include "f360_input_diagnostics.h"
#include "f360_math.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calibrations_T()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * None.
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Constructor of InputDiagnostics::Calibrations_T class
   * Configures default values of calibration parameters
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Input_Diagnostics::Calibrations_T::Calibrations_T()
   {
      min_allowed_core_info_elapsed_time_s = 0.01F;
      max_allowed_core_info_elapsed_time_s = 0.15F;
      max_allowed_sensor_vs_tracker_timestamp_diff_us = 300000LL;
      min_allowed_sensor_vs_tracker_timestamp_diff_us = 0LL;
      max_allowed_rollover_look_index = 3U;
      min_allowed_rollover_prev_look_index = 65532U;
      max_allowed_lateral_mounting_position_abs_vcs = 1.5F;
      max_allowed_azimuth_boresight_angle_abs_vcs = F360_PI;
      valid_polarity_abs = 1;
      min_allowed_detection_range_m = -2.0F;
      max_allowed_detection_range_m = 400.0F;
      min_allowed_range_rate_ms = -70.0F;
      max_allowed_range_rate_ms = 40.0F;
      min_allowed_raw_azimuth_angle = F360_DEG2RAD(-90.0F);
      max_allowed_raw_azimuth_angle = F360_DEG2RAD(90.0F);
      min_allowed_raw_elevation_angle = F360_DEG2RAD(-45.0F);
      max_allowed_raw_elevation_angle = F360_DEG2RAD(45.0F);
   }


   /*===========================================================================*\
   * FUNCTION: Input_Diagnostics()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * None.
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Input Diagnostics object default constructor
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   #if defined(__TMS320C6X__)
   #pragma FUNC_EXT_CALLED
   #endif
   Input_Diagnostics::Input_Diagnostics() : previous_cycle_info{ }
   {
   }

   /*===========================================================================*\
   * FUNCTION: Execute()
   *===========================================================================
   * RETURN VALUE:
   * Input_Faults_T faults_status - list of detected input signal faults
   *
   * PARAMETERS:
   * F360_Core_Info_T& core_info - core_info signal
   * F360_Host_T& host - host signal
   * F360_Radar_Sensor_T (&sensors)[] - sensor information
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Run diagnostics for each input signal, update internal state with information about
   * previous signals state and return detected faults. If previous signal information is
   * not available (after reset or during the first run) no faults are reported.
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   #if defined(__TMS320C6X__)
   #pragma FUNC_EXT_CALLED
   #endif
   Input_Faults_T f360_variant_A::Input_Diagnostics::Execute(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      Input_Faults_T faults_status = { };

      if (previous_cycle_info.valid)
      {
         faults_status.core_info = Check_F360_Core_Info(core_info);
         faults_status.host_info = Check_F360_Host_Info(host);
         faults_status.raw_detection = Check_F360_Radar_Raw_Detection(raw_detect_list);
         for (uint32_t idx = 0U; idx < MAX_NUMBER_OF_SENSORS; ++idx)
         {
            faults_status.sensors_calibs[idx] = Check_F360_Radar_Sensor_Calib(sensors[idx]);
            faults_status.sensors[idx] = Check_F360_Radar_Sensor(sensors[idx], core_info, previous_cycle_info.sensors[idx]);
         }
      }

      Update_Previous_Signals(core_info, host, sensors);

      return faults_status;
   }


   /*===========================================================================*\
   * FUNCTION: Initialize()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * None.
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Objective of Initialize() is to clear internal module state.
   * Information about previous signals states is wiped and marked as not valid.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Input_Diagnostics::Initialize()
   {
      previous_cycle_info = { };
   }

   /*===========================================================================*\
   * FUNCTION: Check_F360_Core_Info()
   *===========================================================================
   * RETURN VALUE:
   * Core_Info_Faults_T faults - faults detected in core_info signal
   *
   * PARAMETERS:
   * F360_Core_Info_T& core_info - core_info signal
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Detect and return faults detected in core_info signal
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Core_Info_Faults_T Input_Diagnostics::Check_F360_Core_Info(const F360_Core_Info_T& core_info) const
   {
      Core_Info_Faults_T faults = { };

      faults.time_us_no_increase = (core_info.time_us <= previous_cycle_info.core_info.time_us);
      faults.cnt_loops_no_increase = (core_info.cnt_loops <= previous_cycle_info.core_info.cnt_loops);
      faults.elapsed_time_above_upper_limit = (core_info.elapsed_time_s > calib.max_allowed_core_info_elapsed_time_s);
      faults.elapsed_time_below_lower_limit = (core_info.elapsed_time_s < calib.min_allowed_core_info_elapsed_time_s);

      return faults;
   }

   /*===========================================================================*\
   * FUNCTION: Check_F360_Host_Info()
   *===========================================================================
   * RETURN VALUE:
   * Host_Info_Faults_T faults - faults detected in host signal
   *
   * PARAMETERS:
   * F360_Host_T& host - host signal
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Detect and return faults detected in host signal
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Host_Info_Faults_T Input_Diagnostics::Check_F360_Host_Info(const F360_Host_T& host) const
   {
      Host_Info_Faults_T faults = { };

      faults.vehicle_index_no_increase = (host.vehicle_index <= previous_cycle_info.host_info.vehicle_index);
      faults.host_speed_invalid                     = (F360_QF_ACCURATE != host.speed_qf);
      faults.host_longitudinal_acceleration_invalid = (F360_QF_ACCURATE != host.long_accel_qf);
      faults.host_lateral_acceleration_invalid = (F360_QF_ACCURATE != host.lat_accel_qf);

      static const float32_t MAX_VALUE_VSE_SIGNALS = 5.0F;
      faults.host_yawrate_invalid = (std::abs(host.curvature_rear) > MAX_VALUE_VSE_SIGNALS)
         || (std::abs(host.vcs_sideslip) > MAX_VALUE_VSE_SIGNALS)
         || (F360_QF_ACCURATE != host.yaw_rate_qf);

      return faults;
   }

    /*===========================================================================*\
    * FUNCTION: Check_F360_Radar_Raw_Detection()
    *===========================================================================
    * RETURN VALUE:
    * Raw_Detection_Faults_T faults  - faults detected in raw_detect_list signal
    *
    * PARAMETERS:
    * RSPP_Detection_List_T& raw_detect_list - single sensor
    *
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * Detect and return faults detected in sensor_calib signal
    *
    * PRECONDITIONS:
    * All the pointers should point to valid structures.
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   Raw_Detection_Faults_T Input_Diagnostics::Check_F360_Radar_Raw_Detection(const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list) const
   {
       Raw_Detection_Faults_T faults = {};

       for (uint32_t idx = 0U; idx < MAX_NUMBER_OF_DETECTIONS; ++idx)
       {
           faults.range_is_invalid = faults.range_is_invalid || ((raw_detect_list.detections[idx].raw.range < calib.min_allowed_detection_range_m) || (raw_detect_list.detections[idx].raw.range> calib.max_allowed_detection_range_m));
           faults.range_rate_is_invalid = faults.range_rate_is_invalid ||((raw_detect_list.detections[idx].raw.range_rate < calib.min_allowed_range_rate_ms) || (raw_detect_list.detections[idx].raw.range_rate > calib.max_allowed_range_rate_ms));
           faults.azimuth_is_invalid = faults.azimuth_is_invalid || ((raw_detect_list.detections[idx].raw.azimuth < calib.min_allowed_raw_azimuth_angle) || (raw_detect_list.detections[idx].raw.azimuth > calib.max_allowed_raw_azimuth_angle));
           faults.elevation_is_invalid = faults.elevation_is_invalid || ((raw_detect_list.detections[idx].raw.elevation < calib.min_allowed_raw_elevation_angle) || (raw_detect_list.detections[idx].raw.elevation > calib.max_allowed_raw_elevation_angle));
       }
       return faults;
   }

   /*===========================================================================*\
   * FUNCTION: Check_F360_Radar_Sensor_Calib()
   *===========================================================================
   * RETURN VALUE:
   * Radar_Sensor_Calib_Faults_T faults  - faults detected in sensor_calib signal
   *
   * PARAMETERS:
   * F360_Radar_Sensor_Calib_T& sensor - single sensor
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Detect and return faults detected in sensor_calib signal
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Radar_Sensor_Calib_Faults_T Input_Diagnostics::Check_F360_Radar_Sensor_Calib(const F360_Radar_Sensor_T& sensor) const
   {
      Radar_Sensor_Calib_Faults_T faults = {};

      if (sensor.variable.is_valid)
      {
         faults.mounting_pos_is_invalid = (fabsf(sensor.constant.mounting_position.vcs_position.lateral)>calib.max_allowed_lateral_mounting_position_abs_vcs);
         faults.polarity_is_invalid = !((sensor.constant.polarity == calib.valid_polarity_abs) || (sensor.constant.polarity == -calib.valid_polarity_abs));
         faults.boresight_angle_is_invalid = (fabsf(sensor.constant.mounting_position.vcs_boresight_azimuth_angle) > calib.max_allowed_azimuth_boresight_angle_abs_vcs);
      }

      return faults;
   }

   /*===========================================================================*\
   * FUNCTION: Check_F360_Radar_Sensor()
   *===========================================================================
   * RETURN VALUE:
   * Radar_Sensor_Faults_T faults  - faults detected in radar_sensor signal
   *
   * PARAMETERS:
   * F360_Radar_Sensor_T& sensor - single sensor information signal
   * F360_Core_Info_T& core_info - core_info signal
   * Radar_Sensor_Reduced_T& previous_cycle_sensor - previous cycle info for single sensor
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Detect and return faults detected in radar_sensor signal
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Radar_Sensor_Faults_T Input_Diagnostics::Check_F360_Radar_Sensor(const F360_Radar_Sensor_T& sensor,
      const F360_Core_Info_T& core_info,
      const Radar_Sensor_Reduced_T& previous_cycle_sensor) const
   {
      Radar_Sensor_Faults_T faults = { };

      if ((sensor.variable.is_valid) && (sensor.variable.timestamp_us != 0U))
      {
         faults.look_index_no_increase = Check_Sensor_Look_Index_No_Increase(sensor, previous_cycle_sensor);
         const int64_t timestamp_difference = (static_cast<int64_t>(core_info.time_us) - static_cast<int64_t>(sensor.variable.timestamp_us));
         faults.sensor_vs_tracker_timestamp_divergence = (timestamp_difference > calib.max_allowed_sensor_vs_tracker_timestamp_diff_us) ||
                                                         (timestamp_difference < calib.min_allowed_sensor_vs_tracker_timestamp_diff_us);
      }

      return faults;
   }

   /*===========================================================================*\
   * FUNCTION: Check_Sensor_Look_Index_Rollover()
   *===========================================================================
   * RETURN VALUE:
   * bool status - true if sensor look_index rollover has been detected
   *
   * PARAMETERS:
   * F360_Radar_Sensor_T& sensor - single sensor information signal
   * Radar_Sensor_Reduced_T& previous_cycle_sensor - previous cycle info for single sensor
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determine if sensor look_index signal has rolled-over.
   * Normal operating signal behaviour: ... -> 65534 -> 65535 -> 0 -> 1 -> ...
   * After reset look_index value should be equal to zero or one (if allowed sensor lookindex drop has occured)
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Input_Diagnostics::Check_Sensor_Look_Index_Rollover(const F360_Radar_Sensor_T& sensor,
      const Radar_Sensor_Reduced_T& previous_cycle_sensor) const
   {
      return ((sensor.variable.look_index < calib.max_allowed_rollover_look_index) && (calib.min_allowed_rollover_prev_look_index < previous_cycle_sensor.look_index));
   }

   /*===========================================================================*\
   * FUNCTION: Check_Sensor_Look_Index_No_Increase()
   *===========================================================================
   * RETURN VALUE:
   * bool status - true if sensor look_index has not increased
   *
   * PARAMETERS:
   * F360_Radar_Sensor_T& sensor - single sensor information signal
   * Radar_Sensor_Reduced_T& previous_cycle_sensor - previous cycle info for single sensor
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determine if sensor look_index has not been increased compared to previous signal value
   * Check is not executed during look_index rollover.
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Input_Diagnostics::Check_Sensor_Look_Index_No_Increase(const F360_Radar_Sensor_T& sensor,
      const Radar_Sensor_Reduced_T& previous_cycle_sensor) const
   {
      // Check signal only when rollover has not been detected
      const bool sensor_look_index_rollover = Check_Sensor_Look_Index_Rollover(sensor, previous_cycle_sensor);

      return ((!sensor_look_index_rollover) && (sensor.variable.look_index <= previous_cycle_sensor.look_index));
   }

   /*===========================================================================*\
   * FUNCTION: Update_Previous_Signals()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Core_Info_T& core_info - core info signal
   * F360_Host_T& host - host signal
   * F360_Radar_Sensor_T (&sensors)[] - radar sensor information signal
   * const rspp_variant_A::RSPP_Detection_List_T - radar raw detection signal
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Update internal state of module with information about previous execution
   * signals state
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Input_Diagnostics::Update_Previous_Signals(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      previous_cycle_info.core_info.cnt_loops = core_info.cnt_loops;
      previous_cycle_info.core_info.time_us = core_info.time_us;
      previous_cycle_info.host_info.vehicle_index = host.vehicle_index;
      for (uint32_t idx = 0U; idx < MAX_NUMBER_OF_SENSORS; ++idx)
      {
         previous_cycle_info.sensors[idx].look_index = sensors[idx].variable.look_index;
         previous_cycle_info.sensors[idx].timestamp_us = sensors[idx].variable.timestamp_us;
      }

      previous_cycle_info.valid = true;
   }

}



