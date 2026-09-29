/*=========================================================================
*  FILE: State_Manager.cpp
*=========================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
*
*  DESCRIPTION:
*    This file contains State Manager class methods definitions.
*
*
*=========================================================================
*------------------------------------------------------------------------------
*
* class:        State_Manager
*
* Description:  Manages tracker states, executes tracker, calls
*        SafetyLogic. Basing on information recieved from SafetyLogic
*        changes/keeps tracker state. Execute function should be
*        called inside wrapper.
*
*========================================================================*/

/******************************
* Includes
*******************************/

#include "State_Manager.h"
namespace f360_variant_A
{
   /*=========================================================================
    * Method         State_Manager::State_Manager
    *
    * Description    Constructor of State_Manager.
    *
    * Parameters     SafetyLogicInterface &SL - reference to SafetyLogic object
    *                F360_Tracker_Interface &TR - reference to F360_Tracker object
    *
    * Returns        None.
    *
    * Externals:     None.
    *
    * Precondition   None.
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   State_Manager::State_Manager(
      SafetyControlLogic& SL,
      F360_Tracker& TR
   ) :
      safety_logic(SL),
      tracker(TR),
      should_reset(false)
   {
      tracker.Initialize();
      SL.initialize();
   }

   /*=========================================================================
    * Method         State_Manager::Initialize
    *
    * Description    Initialize of State Manager.
    *
    * Parameters     None.
    *
    * Returns        None.
    *
    * Externals:     None.
    *
    * Precondition   None.
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   void State_Manager::Initialize() const
   {
      this->tracker.Initialize();
      this->safety_logic.initialize();
   }

   /*=========================================================================
    * Method         State_Manager::initialize(const F360_Variant_T& variant)
    *
    * Description    Initialize of State Manager.
    *
    * Parameters     const F360_Variant_T& variant - reference to variant type
    *
    * Returns        None.
    *
    * Externals:     None.
    *
    * Precondition   None.
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   void State_Manager::Initialize(const F360_Variant_T& variant) const
   {
      this->tracker.Initialize(variant);
      this->safety_logic.initialize();
   }

   /*===========================================================================*\
    * FUNCTION: State_Manager::Initialize_Tracker_State_From_Log()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    *    const F360_Radar_Sensor_T(&r_sensors)[MAX_NUMBER_OF_SENSORS]
    *    const F360_Host_T& r_host
    *    const All_Objects_Log_T& r_all_objects
    *    const Tracker_Info_Log_T& r_tracker_info
    *    const F360_Host_Props_Log_T& r_host_props
    *    const F360_Static_Env_Poly_Log_T& r_static_env_polys
    *    const Tracker_Internal_Log_T& r_tracker_internals
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
    * This function is used to initialize the tracker states from logged data
    *
    * PRECONDITIONS:
    * To be called by an object/reference of F360_Tracker.
    *
    * POSTCONDITIONS:
    * None
    *
   \*===========================================================================*/
   bool State_Manager::Initialize_Tracker_State_From_Log(
      const F360_Host_T& r_host,
      const F360_Object_Log_Output_T& object_log,
      const Tracker_Info_Log_T& r_tracker_info,
      const F360_Host_Props_Log_T& r_host_props,
      const F360_Static_Env_Poly_Log_T& r_static_env_polys,
      const Trailer_Detector_Log_T& r_trailer_detector,
      const Tracker_Internal_T& r_tracker_internals,
      const F360_Internal_Trailer_Detector_T& r_trailer_detector_internal) const
   {
      const bool f_init_ok = this->tracker.Initialize_Tracker_State_From_Log(
         r_host, object_log, r_tracker_info, r_host_props, r_static_env_polys, r_trailer_detector, r_tracker_internals, r_trailer_detector_internal);

      return f_init_ok;
   }

   /*=========================================================================
 * Method         State_Manager::Execute
 *
 * Description    Main function of State_Manager with SG as input.
 *                Should be called inside wrapper, once in initialization
 *                and then continuously once per cycle in wrapper_run().
 *
 * Parameters     const F360_Core_Info_T& core_info - pointer to core info struct
 *                const F360_Host_T &host - reference to host info struct
 *                const sg::SG_Output_T* const p_sg_output - pointer to stationary geometry output
 *                rspp_variant_A::RSPP_Detection_List_T &raw_detect_list - raw detections list
 *                const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensors information struct
 *                const Tracker_Info_Log_T& r_tracker_info - reference to tracker info log struct
 *                All_Objects_Log_T& obj_log - Reference to object tracks information log struct
 *
 * Returns        None.
 *
 * Externals:     None.
 *
 * Precondition   Has to be called instead of F360_tracker::Execute().
 *
 * Postcondition  None.
 *
 * Note           None.
 *========================================================================*/
   void State_Manager::execute(const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const sg::SG_Output_T* const p_sg_output,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Tracker_Info_Log_T& r_tracker_info,
      F360_Object_Log_Output_T& obj_log)
   {

      // Temporally disable tracker reset. To enable it is planned in DFU-408
      if (clear_should_reset_flag())
      {
         should_reset = false;
      }

      if (should_reset)
      {
         tracker.Reset();
         should_reset = false;
      }

      else
      {
         // Pass occupancy grid as NULL pointer
         const ocg::OCG_Outputs_T* const p_occupancy_grid = NULL;

         // Run F360 Tracker
         run_tracker(core_info, host, p_occupancy_grid, p_sg_output, raw_detect_list, sensors, tracker, obj_log);

         // Run SCL
         should_reset = is_critical_fault_detected(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      }
   }

   /*=========================================================================
    * Method         State_Manager::Execute
    *
    * Description    Main function of State_Manager. Should be called inside wrapper
    *                    once in initialization and then continuously once
    *                    per cycle in wrapper_run().
    *
    * Parameters     const F360_Core_Info_T& core_info - pointer to core info struct
    *                const F360_Host_T &host - reference to host info struct
    *                const ocg::OCG_Outputs_T& occupancy_grid - occupancy grid
    *                rspp_variant_A::RSPP_Detection_List_T &raw_detect_list - raw detections list
    *                const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensors information struct
    *                const Tracker_Info_Log_T& r_tracker_info - reference to tracker info log struct
    *                All_Objects_Log_T& obj_log - Reference to object tracks information log struct
    *
    * Returns        None.
    *
    * Externals:     None.
    *
    * Precondition   Has to be called instead of F360_tracker::Execute().
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   void State_Manager::execute(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const ocg::OCG_Outputs_T& occupancy_grid,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Tracker_Info_Log_T& r_tracker_info,
      F360_Object_Log_Output_T& obj_log
   )
   {
      // Temporally disable tracker reset. To enable it is planned in DFU-408
      if (clear_should_reset_flag())
      {
         should_reset = false;
      }

      if (should_reset)
      {
         tracker.Reset();
         should_reset = false;
      }
      else
      {
         // Set p_sg_pointer to NULL, SG is not an input to this state manager execute
         const sg::SG_Output_T* const p_sg_output = NULL;

         // Pass occupancy grid as pointer to avoid memory allocation for projects that do not use occupancy grid
         const ocg::OCG_Outputs_T* const p_occupancy_grid = &occupancy_grid;

         // Run F360 Tracker
         run_tracker(core_info, host, p_occupancy_grid, p_sg_output, raw_detect_list, sensors, tracker, obj_log);

         // Run SCL
         should_reset = is_critical_fault_detected(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      }
   }

   /*=========================================================================
 * Method         State_Manager::Execute
 *
 * Description    Main function of State_Manager. Should be called inside wrapper
 *                    once in initialization and then continuously once
 *                    per cycle in wrapper_run().
 *                Overload. To be called when occupancy grid is not necessary.
 *
 * Parameters     const F360_Core_Info_T& core_info - pointer to core info struct
 *                const F360_Host_T &host - reference to host info struct
 *                const RSPP_Detection_List_T &raw_detect_list - raw detections list
 *                const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensors information struct
 *                All_Objects_Log_T& obj_log - Reference to object tracks information log struct
 *
 * Returns        None.
 *
 * Externals:     None.
 *
 * Precondition   Has to be called instead of F360_tracker::Execute().
 *
 * Postcondition  None.
 *
 * Note           None.
 *========================================================================*/
   void State_Manager::execute(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Tracker_Info_Log_T& r_tracker_info,
      F360_Object_Log_Output_T& obj_log
   )
   {
      // Temporally disable tracker reset. To enable it is planned in DFU-408
      if (clear_should_reset_flag())
      {
         should_reset = false;
      }

      if (should_reset)
      {
         tracker.Reset();
         should_reset = false;
      }
      else
      {
         // Pass occupancy grid as pointer to avoid memory allocation for projects that do not use occupancy grid
         const ocg::OCG_Outputs_T* const p_occupancy_grid = NULL;

         // Set p_sg_pointer to NULL, sincr SG is not an input to this state manager execute
         const sg::SG_Output_T* const p_sg_output = NULL;

         // Run F360 Tracker
         run_tracker(core_info, host, p_occupancy_grid, p_sg_output, raw_detect_list, sensors, tracker, obj_log);

         // Run SCL
         should_reset = is_critical_fault_detected(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      }
   }

   /*=========================================================================
    * Method         State_Manager::run_tracker
    *
    * Description    function to run the tracker
    *
    * Parameters     const F360_Core_Info_T& core_info - pointer to core info struct
    *                const F360_Host_T &host - reference to host info struct
    *                const ocg::OCG_Outputs_T& occupancy_grid - occupancy grid
    *                const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list - raw detections list
    *                const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensors information struct
    *                const F360_Tracker_Interface& tracker_interface - reference to the tracker core
    *                All_Objects_Log_T& obj_log - Reference to object tracks information log struct
    *
    * Returns        None.
    *
    * Externals:     None.
    *
    * Precondition   None.
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   void State_Manager::run_tracker(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const sg::SG_Output_T* const p_sg_output,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker& r_tracker,
      F360_Object_Log_Output_T& obj_log) const
   {
      // Run Tracker
      r_tracker.Execute(core_info, host, p_occupancy_grid, p_sg_output, raw_detect_list, sensors);

      // Fill Tracker Object Output
      r_tracker.Log_Objects(obj_log);
   }

   /*=========================================================================
    * Method         State_Manager::is_critical_fault_detected
    *
    * Description    function determines whether critical fault was detected and tracker has to be reset.
    *
    * Parameters     const F360_Core_Info_T& core_info - pointer to core info struct
    *                const F360_Host_T &host - reference to host info struct
    *                const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensor calibrations struct
    *                const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensors information struct
    *                const All_Objects_Log_T& obj_log - Reference to object tracks information log struct
    *
    * Returns        bool shouldReset.
    *
    * Externals:     None.
    *
    * Precondition   Has to be called instead of F360_tracker::Execute().
    *
    * Postcondition  None.
    *
    * Note           None.
    *========================================================================*/
   bool State_Manager::is_critical_fault_detected(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Object_Log_Output_T& obj_log,
      const Tracker_Info_Log_T& r_tracker_info) const
   {
      SafetyControlLogic::SCL_Output_T const scl_status = safety_logic.evaluate_cycle(
         core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      return scl_status.should_reset;
   }

   void State_Manager::Log_Functional_Safety_Faults(Functional_Safety_Faults_Log_T& log) const
   {
      // input diagnostics faults logging
      const Input_Faults_T& input_faults = safety_logic.get_input_status();
      log.input_faults.core_info.cnt_loops_no_increase = static_cast<uint8_t>(input_faults.core_info.cnt_loops_no_increase);
      log.input_faults.core_info.elapsed_time_above_upper_limit = static_cast<uint8_t>(input_faults.core_info.elapsed_time_above_upper_limit);
      log.input_faults.core_info.elapsed_time_below_lower_limit = static_cast<uint8_t>(input_faults.core_info.elapsed_time_below_lower_limit);
      log.input_faults.core_info.time_us_no_increase = static_cast<uint8_t>(input_faults.core_info.time_us_no_increase);
      log.input_faults.host_info.vehicle_index_no_increase = static_cast<uint8_t>(input_faults.host_info.vehicle_index_no_increase);
      log.input_faults.host_info.host_speed_invalid = static_cast<uint8_t>(input_faults.host_info.host_speed_invalid);
      log.input_faults.host_info.host_yawrate_invalid = static_cast<uint8_t>(input_faults.host_info.host_yawrate_invalid);
      log.input_faults.host_info.host_longitudinal_acceleration_invalid = static_cast<uint8_t>(input_faults.host_info.host_longitudinal_acceleration_invalid);
      log.input_faults.host_info.host_lateral_acceleration_invalid = static_cast<uint8_t>(input_faults.host_info.host_lateral_acceleration_invalid);
      log.input_faults.raw_detection.range_is_invalid = static_cast<uint8_t>(input_faults.raw_detection.range_is_invalid);
      log.input_faults.raw_detection.range_rate_is_invalid = static_cast<uint8_t>(input_faults.raw_detection.range_rate_is_invalid);
      log.input_faults.raw_detection.azimuth_is_invalid = static_cast<uint8_t>(input_faults.raw_detection.azimuth_is_invalid);
      log.input_faults.raw_detection.elevation_is_invalid = static_cast<uint8_t>(input_faults.raw_detection.elevation_is_invalid);
      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         log.input_faults.sensors[i].look_index_no_increase = static_cast<uint8_t>(input_faults.sensors[i].look_index_no_increase);
         log.input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = static_cast<uint8_t>(input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence);
         log.input_faults.sensors_calibs[i].mounting_pos_is_invalid = static_cast<uint8_t>(input_faults.sensors_calibs[i].mounting_pos_is_invalid);
         log.input_faults.sensors_calibs[i].polarity_is_invalid = static_cast<uint8_t>(input_faults.sensors_calibs[i].polarity_is_invalid);
         log.input_faults.sensors_calibs[i].boresight_angle_is_invalid = static_cast<uint8_t>(input_faults.sensors_calibs[i].boresight_angle_is_invalid);
      }

      //output diagnostics faults logging
      const Output_Faults_T& output_faults = safety_logic.get_output_status();
      log.output_faults.f_track_accelerations_faulty = static_cast<uint8_t>(output_faults.f_track_accelerations_faulty);
      log.output_faults.f_track_positions_faulty = static_cast<uint8_t>(output_faults.f_track_positions_faulty);
      log.output_faults.f_track_velocities_faulty = static_cast<uint8_t>(output_faults.f_track_velocities_faulty);
      log.output_faults.f_severe_angle_jump_presence_fault = static_cast<uint8_t>(output_faults.f_severe_angle_jump_presence_fault);

      // Tracker fault status and tracker reset flag logging
      const SafetyControlLogic::SCL_Output_T& scl_faults = safety_logic.get_scl_status();
      log.scl_output_faults.core_info_fault_status = static_cast<uint8_t>(scl_faults.core_info_fault_status);
      log.scl_output_faults.host_info_fault_status = static_cast<uint8_t>(scl_faults.host_info_fault_status);
      log.scl_output_faults.raw_detection_fault_status = static_cast<uint8_t>(scl_faults.raw_detection_fault_status);
      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         log.scl_output_faults.sensors_calibs_fault_status[i] = static_cast<uint8_t>(scl_faults.sensors_calibs_fault_status[i]);
         log.scl_output_faults.sensors_fault_status[i] = static_cast<uint8_t>(scl_faults.sensors_fault_status[i]);
      }
      log.scl_output_faults.object_track_fault_status = static_cast<uint8_t>(scl_faults.object_track_fault_status);
      log.scl_output_faults.angle_jump_fault_status = static_cast<uint8_t>(scl_faults.angle_jump_fault_status);
      log.scl_output_faults.overall_fault_status = static_cast<uint8_t>(scl_faults.overall_fault_status);
      log.scl_output_faults.should_reset = static_cast<uint8_t>(scl_faults.should_reset);
   }
}
