/*=========================================================================
 *  FILE: f360_safety_control_logic.h
 *=========================================================================
 * Copyright (C) 2020 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------
 *
 *  DESCRIPTION:
 *    This file contains SafetyControlLogic class declaration.
 *
 *========================================================================*/
#ifndef F360_SAFETY_CONTROL_LOGIC_VARIANT_A_H
#define F360_SAFETY_CONTROL_LOGIC_VARIANT_A_H
/******************************
 * Includes
 *******************************/
#include "../Types/f360_core_info.h"
#include "../Types/f360_host.h"
#include "../Types/f360_radar_sensor.h"
#include "../Logging/f360_log_types.h"
#include "f360_input_diagnostics.h"
#include "f360_output_diagnostics.h"
#include "rspp_detection_list.h"
#include "../Logging/TrackerInfoLog.h"

/******************************
 * Class definition
 *******************************/
namespace f360_variant_A
{
   class SafetyControlLogic
   {
   public:
      enum CYCLE_FAULT_STATUS : uint8_t
      {
         FAULT_PRESENT_STATUS = (60),
         FAULT_PARTIAL_PRESENT_STATUS = (105),
         FAULT_NOT_PRESENT_STATUS = (195)
      };

      struct SCL_Output_T
      {
         CYCLE_FAULT_STATUS core_info_fault_status;
         CYCLE_FAULT_STATUS host_info_fault_status;
         CYCLE_FAULT_STATUS raw_detection_fault_status;
         CYCLE_FAULT_STATUS sensors_calibs_fault_status[MAX_NUMBER_OF_SENSORS];
         CYCLE_FAULT_STATUS sensors_fault_status[MAX_NUMBER_OF_SENSORS];
         CYCLE_FAULT_STATUS object_track_fault_status;
         CYCLE_FAULT_STATUS angle_jump_fault_status;
         CYCLE_FAULT_STATUS overall_fault_status;
         bool should_reset;
         uint8_t padding[3]; // tasking compiler padding
      };

      SafetyControlLogic(Input_Diagnostics &, Output_Diagnostics &);
      virtual ~SafetyControlLogic() = default;


      virtual SCL_Output_T evaluate_cycle(
          const F360_Core_Info_T &core_info,
          const F360_Host_T &host,
          const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
          const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
          const F360_Object_Log_Output_T &obj_log,
          const Tracker_Info_Log_T &r_tracker_info); // function evaluates faults from Input_Diagnostics, Output_Diagnostics.

      virtual const SCL_Output_T &get_scl_status() const;       // function gets scl fault status
      virtual const Input_Faults_T &get_input_status() const;   // function gets input fault status
      virtual const Output_Faults_T &get_output_status() const; // function gets output fault status

      virtual void initialize(); // function to clear internal module state.

   private:
      // Structure for calibrations used in SCL module
      const struct SCL_Calibrations_T
      {
      public:
         SCL_Calibrations_T(){};
         uint8_t get_max_non_critical_fault_count() const;
         uint8_t get_max_fault_status_count() const;

      private:
         const uint8_t max_non_critical_fault_count = 3U;
         const uint8_t max_fault_status_count = 10U;
      } calib;

      // references to external objects
      Input_Diagnostics &inp_diag;  // Reference to input diagnostics object
      Output_Diagnostics &out_diag; // Reference to output diagnostics object

      SCL_Output_T scl_status;
      Input_Faults_T input_faults;
      Output_Faults_T output_faults;

      void update_non_critical_fault_info(
          const bool f_fault,
          uint8_t &r_fault_counter,
          uint8_t &r_fault_remain_counter,
          CYCLE_FAULT_STATUS & fault_status_to_be_updated) const;

      void update_critical_output_fault_info(
          Output_Faults_T const& all_output_faults,
          uint8_t &r_fault_remain_counter,
          CYCLE_FAULT_STATUS & fault_status_to_be_updated,
          bool &r_critical_fault_present) const;

      void determine_faults_status(Input_Faults_T const &input_status, Output_Faults_T const &output_status);             // Function determines critical fault status.
      void check_core_info_faults(Core_Info_Faults_T const &r_core_info);                                                 // Function determines critical fault status of F360_Core_Info_T.
      void check_host_info_faults(Host_Info_Faults_T const &r_host_info);                                                 // Function determines critical fault status of F360_Host_Info_T.
      void check_raw_detection_faults(Raw_Detection_Faults_T const &r_raw_detection);                                     // Function determines critical fault status of RSPP_Detection_List_T.
      void check_radar_sensor_calib_faults(Radar_Sensor_Calib_Faults_T const (&r_sensors_calibs)[MAX_NUMBER_OF_SENSORS]); // Function determines critical fault status of Radar_Sensor_Calib_Faults_T.
      void check_radar_sensor_faults(Radar_Sensor_Faults_T const (&r_sensors)[MAX_NUMBER_OF_SENSORS]);                    // Function determines critical fault status of Radar_Sensor_Faults_T.
      void check_output_faults(Output_Faults_T const& r_output_faults);                                       // Function determines critical fault status of Object_List_Faults_T.
      void compute_overall_fault_status(void);                                                                            // Function that computes the overall fault status based on the separate faults

      // Core Info
      uint8_t core_info_fault_counter;        // Counter to check faults of Core_Info_Faults_T
      uint8_t core_info_fault_remain_counter; // Counter to check Core_Info_Faults_T Present for 10 tracker indices before changing to No fault status

      // Host info
      uint8_t vehicle_index_fault_counter;                         // Counter to check host fault vehicle_index_no_increase
      uint8_t vehicle_index_fault_remain_counter;                  // Counter to check if host vehicle_index_no_increase is present for 10 tracker indices before changing to No fault status
      uint8_t host_speed_fault_counter;                            // Counter to check fault host_speed_invalid
      uint8_t host_speed_fault_remain_counter;                     // Counter to check if host_speed_invalid is present for 10 tracker indices before changing to No fault status
      uint8_t host_yawrate_fault_counter;                          // Counter to check fault host_yawrate_invalid
      uint8_t host_yawrate_fault_remain_counter;                   // Counter to check if host_yawrate_invalid is present for 10 tracker indices before changing to No fault status
      uint8_t host_longitudinal_acceleration_fault_counter;        // Counter to check fault host_longitudinal_acceleration_invalid
      uint8_t host_longitudinal_acceleration_fault_remain_counter; // Counter to check if host_longitudinal_acceleration_invalid is present for 10 tracker indices before changing to No fault status
      uint8_t host_lateral_acceleration_fault_counter;             // Counter to check fault host_lateral_acceleration_invalid
      uint8_t host_lateral_acceleration_fault_remain_counter;      // Counter to check if host_lateral_acceleration_invalid is present for 10 tracker indices before changing to No fault status

      // Radar Raw Detection
      uint8_t detection_range_fault_counter;                       // Counter to check range_is_invalid fault
      uint8_t detection_range_fault_remain_counter;                // Counter to check if range_is_invalid is present for 10 tracker indices before changing to No fault status
      uint8_t range_rate_fault_fault_counter;                      // Counter to check range_rate_is_invalid fault
      uint8_t range_rate_fault_remain_counter;                     // Counter to check if range_rate_is_invalid is present for 10 tracker indices before changing to No fault status
      uint8_t azimuth_angle_faultfault_counter;                    // Counter to check azimuth_is_invalid fault
      uint8_t azimuth_angle_fault_remain_counter;                  // Counter to check if azimuth_is_invalid is present for 10 tracker indices before changing to No fault status
      uint8_t elevation_angle_fault_counter;                       // Counter to check elevation_is_invalid fault
      uint8_t elevation_angle_fault_remain_counter;                // Counter to check if elevation_is_invalid is present for 10 tracker indices before changing to No fault status
 
      // Radar Sensor Calib
      uint8_t mounting_pos_fault_counter[MAX_NUMBER_OF_SENSORS];           // Counter to check fault mounting_pos_is_invalid
      uint8_t mounting_pos_fault_remain_counter[MAX_NUMBER_OF_SENSORS];    // Counter to check mounting_pos_is_invalid for 10 tracker indices before changing to No fault status
      uint8_t polarity_fault_counter[MAX_NUMBER_OF_SENSORS];               // Counter to check fault polarity_is_invalid
      uint8_t polarity_fault_remain_counter[MAX_NUMBER_OF_SENSORS];        // Counter to check polarity_is_invalid for 10 tracker indices before changing to No fault status
      uint8_t boresight_angle_fault_counter[MAX_NUMBER_OF_SENSORS];        // Counter to check fault azimuth_boresight_angle_is_invalid
      uint8_t boresight_angle_fault_remain_counter[MAX_NUMBER_OF_SENSORS]; // Counter to check polarity_is_invalid for 10 tracker indices before changing to No fault status

      // Radar Sensor
      uint8_t look_index_fault_counter[MAX_NUMBER_OF_SENSORS];                      // Array for look_index_no_increase
      uint8_t look_index_fault_remain_counter[MAX_NUMBER_OF_SENSORS];               // Counter array to check look_index_no_increasefor Present 10 tracker indices before changing to No fault status
      uint8_t sensor_trk_ts_divergence_fault_counter[MAX_NUMBER_OF_SENSORS];        // Array for sensor_vs_tracker_timestamp_divergence
      uint8_t sensor_trk_ts_divergence_fault_remain_counter[MAX_NUMBER_OF_SENSORS]; // Counter array to check sensor_vs_tracker_timestamp_divergence Present for 10 tracker indices before changing to No fault status

      // Object Tracks
      uint8_t object_track_fault_remain_counter; // Counter to check object list output diagnostics fault for 10 tracker indices before changing to No fault status

   };
}
#endif
