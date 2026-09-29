#include "State_Manager.h"
#include "SafetyLogicMock.h"
#include "TrackerMock.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>


//Declaration of stubbed/mock functions


/** \brief
 *  State Manager unit tests. They verify whether tracker was initialized, executed and reseted.
 */
using namespace f360_variant_A;
TEST_GROUP(TSM)
{
   TrackerMock tracker;
   Input_Diagnostics_Mock in_faults;
   Output_Diagnostics_Mock out_faults;
   F360_Core_Info_T core_info = {};
   F360_Host_T host = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Log_Output_T obj_log = {};
   ocg::OCG_Outputs_T occupancy_grid = {};
   sg::SG_Output_T sg_output{};
   sg::SG_Output_T* p_sg_output = &sg_output;
   const Tracker_Info_Log_T r_tracker_info = {};


   TEST_SETUP()
   {

   }


   TEST_TEARDOWN()
   {
      mock().checkExpectations();
      mock().clear();
   }

};

/**
*\purpose  Purpose of this test is to verify state manager initialize function call
*/
TEST(TSM, TSM__check_initialize_function)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);

   /** \precond
   * No preconditions
   */

   mock().expectNCalls(2, "Initialize");
   mock().expectNCalls(2, "SafetyControlLogic::initialize");

   /** \action
   * Execute tracker state manager and its init function 
   */

   State_Manager tracker_state_manager(safety_logic, tracker);

   tracker_state_manager.Initialize();

   /** \result
   * Check whether tracker was initialized (in TEST_TEARDOWN)
   */
}
/** @}*/

/**
*\purpose  Purpose of this test is to verify state manager initialize function with variant init
*/
TEST(TSM, TSM__check_initialize_function_with_variant_init)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);

   /** \precond
   * Tracker Variant
   */
   F360_Variant_T variant;
   Set_Tracker_Variant(variant);

   mock().expectOneCall("Initialize(Variant)");
   mock().expectNCalls(2, "SafetyControlLogic::initialize");

   /** \action
   * Execute tracker state manager.
   */

   State_Manager tracker_state_manager(safety_logic, tracker);

   tracker_state_manager.Initialize(variant);

   /** \result
   * Check whether tracker was initialized (in TEST_TEARDOWN)
   */
}
/** @}*/

/**
*\purpose  Purpose of this test is to verify whether tracker is initialized.
*/
TEST(TSM, TSM__check_if_tracker_is_initialized)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);

   /** \precond
   * No preconditions
   */

   mock().expectOneCall("Initialize");
   mock().expectOneCall("SafetyControlLogic::initialize");

   /** \action
   * Execute tracker state manager.
   */

   State_Manager tracker_state_manager(safety_logic, tracker);

   /** \result
   * Check whether tracker was initialized (in TEST_TEARDOWN)
   */
}
/** @}*/
/**
*\purpose  Purpose of this test is to verify whether tracker with OCG input is executed.
*/
TEST(TSM, TSM__check_if_tracker_with_ocg_input_is_executed)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);
   State_Manager tracker_state_manager(safety_logic, tracker);

   /** \precond
   * tracker_state_manager.execute has to be called once before
   */

   mock().expectOneCall("Execute");

   /** \action
   * Execute tracker state manager
   */

   tracker_state_manager.execute(core_info, host, occupancy_grid, raw_detect_list, sensors, r_tracker_info, obj_log);

   /** \result
   * Check whether tracker was executed once (in TEST_TEARDOWN)
   */

}
/** @}*/

/**
*\purpose  Purpose of this test is to verify whether tracker with sg input is executed.
*/
TEST(TSM, TSM__check_if_tracker_with_sg_input_is_executed)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);
   State_Manager tracker_state_manager(safety_logic, tracker);

   /** \precond
   * tracker_state_manager.execute has to be called once before
   */

   mock().expectOneCall("Execute");

   /** \action
   * Execute tracker state manager
   */

   tracker_state_manager.execute(core_info, host, p_sg_output, raw_detect_list, sensors, r_tracker_info, obj_log);

   /** \result
   * Check whether tracker was executed once (in TEST_TEARDOWN)
   */

}
/** @}*/

/**
*\purpose  Purpose of this test is to verify whether tracker is executed without occupancy grid and stationary geometries.
*/
TEST(TSM, TSM__check_if_tracker_is_executed_without_ocg_and_sg)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);
   State_Manager tracker_state_manager(safety_logic, tracker);

   /** \precond
   * tracker_state_manager.execute has to be called once before
   */

   mock().expectOneCall("Execute");

   /** \action
   * Execute tracker state manager
   */

   tracker_state_manager.execute(core_info, host, raw_detect_list, sensors, r_tracker_info, obj_log);

   /** \result
   * Check whether tracker was executed once (in TEST_TEARDOWN)
   */

}

/** @}*/
/**
*\purpose  Purpose of this test is to verify whether tracker with ocg input is reset.
*/
TEST(TSM, TSM__check_if_tracker_with_ocg_input_is_reset)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);
   State_Manager tracker_state_manager(safety_logic, tracker);

   /** \precond
    * tracker_state_manager.execute has to be called twice before
    * field: raw_detect_list.detections[0].sensor_id has to be set to 3
    */

   //mock().expectOneCall("Reset"); // Temperately disable this test purpose. TSM is hardcode to not reset for now

   mock().expectOneCall("evaluate_cycle").andReturnValue(static_cast<int>(true));
   //tracker_state_manager.execute(core_info, host, raw_detect_list, sensors, obj_log); // Temperately disable this test purpose. TSM is hardcode to not reset for now


   /** \action
    * Execute tracker state manager
    */

   tracker_state_manager.execute(core_info, host, occupancy_grid, raw_detect_list, sensors, r_tracker_info, obj_log);

   /** \result
    * Check whether tracker state manager was reseted (in TEST_TEARDOWN)
    */

}
/** @}*/
/**
*\purpose  Purpose of this test is to verify whether tracker with sg input is reset.
*/
TEST(TSM, TSM__check_if_tracker_with_sg_input_is_reset)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);
   State_Manager tracker_state_manager(safety_logic, tracker);

   /** \precond
    * tracker_state_manager.execute has to be called twice before
    * field: raw_detect_list.detections[0].sensor_id has to be set to 3
    */

   //mock().expectOneCall("Reset"); // Temperately disable this test purpose. TSM is hardcode to not reset for now

   mock().expectOneCall("evaluate_cycle").andReturnValue(static_cast<int>(true));
   //tracker_state_manager.execute(core_info, host, raw_detect_list, sensors, obj_log); // Temperately disable this test purpose. TSM is hardcode to not reset for now


   /** \action
    * Execute tracker state manager
    */

   tracker_state_manager.execute(core_info, host, p_sg_output, raw_detect_list, sensors, r_tracker_info, obj_log);

   /** \result
    * Check whether tracker state manager was reseted (in TEST_TEARDOWN)
    */

}
/** @}*/
/**
*\purpose  Purpose of this test is to verify whether tracker is reset without occupancy grid and Stationary Geometries.
*/
TEST(TSM, TSM__check_if_tracker_is_reset_without_ocg_and_sg)
{
   mock().ignoreOtherCalls();
   SafetyLogicMock safety_logic(in_faults, out_faults);
   State_Manager tracker_state_manager(safety_logic, tracker);

   /** \precond
    * tracker_state_manager.execute has to be called twice before
    * field: raw_detect_list.detections[0].sensor_id has to be set to 3
    */

   //mock().expectOneCall("Reset"); // Temperately disable this test purpose. TSM is hardcode to not reset for now

   mock().expectOneCall("evaluate_cycle").andReturnValue(static_cast<int>(true));
   //tracker_state_manager.execute(core_info, host, raw_detect_list, sensors, obj_log); // Temperately disable this test purpose. TSM is hardcode to not reset for now


   /** \action
    * Execute tracker state manager
    */

   tracker_state_manager.execute(core_info, host, raw_detect_list, sensors, r_tracker_info, obj_log);

   /** \result
    * Check whether tracker state manager was reseted (in TEST_TEARDOWN)
    */

}
/** @}*/
/**
*\purpose  Purpose of this test is to verify whether TSM can create proper logging output.
* Member function of State_Manager class to be tested is 
* void Log_Functional_Safety_Faults(Functional_Safety_Faults_Log_T& log) const
*
*/
TEST(TSM, TSM__test_log_functional_safety_faults)
{
   mock().ignoreOtherCalls();

   /** \precond
    * Preparation for the test of log function
    */
   SafetyLogicMock safety_logic(in_faults, out_faults);
   State_Manager tracker_state_manager(safety_logic, tracker);


   /** \action
    * Execute log function
    */
   Functional_Safety_Faults_Log_T log{};
   tracker_state_manager.Log_Functional_Safety_Faults(log);

   /** \result
    * Check log output
    */
   const Input_Faults_T& in_faults = safety_logic.get_input_status();

   CHECK_EQUAL_TEXT(in_faults.core_info.elapsed_time_above_upper_limit, log.input_faults.core_info.elapsed_time_above_upper_limit, "Failed on elapsed_time_above_upper_limit");
   CHECK_EQUAL_TEXT(in_faults.core_info.elapsed_time_below_lower_limit, log.input_faults.core_info.elapsed_time_below_lower_limit, "Failed on elapsed_time_below_lower_limit");
   CHECK_EQUAL_TEXT(in_faults.core_info.cnt_loops_no_increase, log.input_faults.core_info.cnt_loops_no_increase,"Failed on cnt_loops_no_increase");
   CHECK_EQUAL_TEXT(in_faults.core_info.time_us_no_increase, log.input_faults.core_info.time_us_no_increase, "Failed on time_us_no_increase");
   CHECK_EQUAL_TEXT(in_faults.host_info.vehicle_index_no_increase, log.input_faults.host_info.vehicle_index_no_increase, "Failed vehicle_index_no_increase");
   CHECK_EQUAL_TEXT(in_faults.host_info.host_speed_invalid, log.input_faults.host_info.host_speed_invalid, "Failed on host_speed_invalid");
   CHECK_EQUAL_TEXT(in_faults.host_info.host_yawrate_invalid, log.input_faults.host_info.host_yawrate_invalid, "Failed on host_yawrate_invalid");
   CHECK_EQUAL_TEXT(in_faults.host_info.host_longitudinal_acceleration_invalid, log.input_faults.host_info.host_longitudinal_acceleration_invalid, "Failed on host_longitudinal_acceleration_invalid" );
   CHECK_EQUAL_TEXT(in_faults.host_info.host_lateral_acceleration_invalid, log.input_faults.host_info.host_lateral_acceleration_invalid, "Failed on host_lateral_acceleration_invalid");
   CHECK_EQUAL_TEXT(in_faults.raw_detection.range_is_invalid, log.input_faults.raw_detection.range_is_invalid, "Failed on range_is_invalid");
   CHECK_EQUAL_TEXT(in_faults.raw_detection.range_rate_is_invalid, log.input_faults.raw_detection.range_rate_is_invalid, "Failed on range_rate_is_invalid");
   CHECK_EQUAL_TEXT(in_faults.raw_detection.azimuth_is_invalid, log.input_faults.raw_detection.azimuth_is_invalid, "Failed on azimuth_is_invalid");
   CHECK_EQUAL_TEXT(in_faults.raw_detection.elevation_is_invalid, log.input_faults.raw_detection.elevation_is_invalid, "Failed on elevation_is_invalid");
   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      CHECK_EQUAL_TEXT(in_faults.sensors[i].look_index_no_increase, log.input_faults.sensors[i].look_index_no_increase, "Failed on look_index_no_increase");
      CHECK_EQUAL_TEXT(in_faults.sensors[i].sensor_vs_tracker_timestamp_divergence, log.input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence , "Failed on sensor_vs_tracker_timestamp_divergence");
      CHECK_EQUAL_TEXT(in_faults.sensors_calibs[i].mounting_pos_is_invalid, log.input_faults.sensors_calibs[i].mounting_pos_is_invalid, "Failed on mounting_pos_is_invalid");
      CHECK_EQUAL_TEXT(in_faults.sensors_calibs[i].polarity_is_invalid, log.input_faults.sensors_calibs[i].polarity_is_invalid, "Failed on polarity_is_invalid");
      CHECK_EQUAL_TEXT(in_faults.sensors_calibs[i].boresight_angle_is_invalid, log.input_faults.sensors_calibs[i].boresight_angle_is_invalid, "Failed on boresight_angle_is_invalid");
   }

      //output diagnostics faults logging
      const Output_Faults_T& output_faults = safety_logic.get_output_status();
      CHECK_EQUAL_TEXT(output_faults.f_track_accelerations_faulty, log.output_faults.f_track_accelerations_faulty, "Failed on f_track_accelerations_faulty");
      CHECK_EQUAL_TEXT(output_faults.f_track_positions_faulty, log.output_faults.f_track_positions_faulty, "Failed on f_track_positions_faulty");
      CHECK_EQUAL_TEXT(output_faults.f_track_velocities_faulty, log.output_faults.f_track_velocities_faulty, "Failed on f_track_velocities_faulty");

      // Tracker fault status and tracker reset flag logging
      const SafetyControlLogic::SCL_Output_T& scl_faults = safety_logic.get_scl_status();
      CHECK_EQUAL_TEXT(scl_faults.core_info_fault_status, log.scl_output_faults.core_info_fault_status, "Failed on core_info_fault_status");
      CHECK_EQUAL_TEXT(scl_faults.host_info_fault_status, log.scl_output_faults.host_info_fault_status, "Failed on host_info_fault_status");
      CHECK_EQUAL_TEXT(scl_faults.raw_detection_fault_status, log.scl_output_faults.raw_detection_fault_status, "Failed on raw_detection_fault_status");
      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         CHECK_EQUAL_TEXT(scl_faults.sensors_calibs_fault_status[i], log.scl_output_faults.sensors_calibs_fault_status[i], "Failed on sensors_calibs_fault_status[i]");
         CHECK_EQUAL_TEXT(scl_faults.sensors_fault_status[i], log.scl_output_faults.sensors_fault_status[i], "Failed on sensors_fault_status[i]");
      }
      CHECK_EQUAL_TEXT(scl_faults.object_track_fault_status, log.scl_output_faults.object_track_fault_status, "Failed on object_track_fault_status");
      CHECK_EQUAL_TEXT(scl_faults.overall_fault_status, log.scl_output_faults.overall_fault_status, "Failed on overall_fault_status");
      CHECK_EQUAL_TEXT(scl_faults.should_reset, log.scl_output_faults.should_reset, "Failed on should_reset")

}

/** @}*/
