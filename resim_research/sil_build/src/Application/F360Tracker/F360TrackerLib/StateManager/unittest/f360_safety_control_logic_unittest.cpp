/** \file
Unit test to evaluates the fault status from Input Diagnostics and Output Diagnostics
*/

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

#include "f360_input_diagnostics_mock.h"
#include "f360_output_diagnostics_mock.h"
#include "f360_safety_control_logic.h"



using namespace f360_variant_A;

inline bool operator==(const SafetyControlLogic::SCL_Output_T& first, const SafetyControlLogic::SCL_Output_T& second)
{
   bool f_is_equal = (first.should_reset == second.should_reset) &&
      (first.core_info_fault_status == second.core_info_fault_status) &&
      (first.host_info_fault_status == second.host_info_fault_status) &&
      (first.raw_detection_fault_status == second.raw_detection_fault_status) &&
      (first.overall_fault_status  == second.overall_fault_status);
   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      f_is_equal = f_is_equal &&
         (first.sensors_calibs_fault_status[i] == second.sensors_calibs_fault_status[i]) &&
         (first.sensors_fault_status[i] == second.sensors_fault_status[i]);
   }
   return (f_is_equal);
}

inline bool operator!=(const SafetyControlLogic::SCL_Output_T& first, const SafetyControlLogic::SCL_Output_T& second)
{
   return !(first == second);
}

SimpleString StringFrom(const SafetyControlLogic::CYCLE_FAULT_STATUS status)
{
   switch (status)
   {
      case SafetyControlLogic::FAULT_PRESENT_STATUS:
         return{ "FAULT_PRESENT_STATUS" };
      case SafetyControlLogic::FAULT_PARTIAL_PRESENT_STATUS:
         return{ "FAULT_PARTIAL_PRESENT_STATUS" };
      case SafetyControlLogic::FAULT_NOT_PRESENT_STATUS:
         return{ "FAULT_NOT_PRESENT_STATUS" };
      default:
         return{ "fault_undefined_in_StringFrom" };
   }
}

SimpleString StringFrom(const SafetyControlLogic::SCL_Output_T& scl_out)
{
   return SimpleString("[fault_status: ") + StringFrom(scl_out.overall_fault_status) +
      SimpleString(", should_reset: ") + StringFrom(scl_out.should_reset) +
      SimpleString("]");
}

SafetyControlLogic::SCL_Output_T get_empty_SCL_Output()
{
   SafetyControlLogic::SCL_Output_T scl_output = {};
   scl_output.core_info_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   scl_output.host_info_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   scl_output.raw_detection_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   scl_output.object_track_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   scl_output.angle_jump_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      scl_output.sensors_calibs_fault_status[i] = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      scl_output.sensors_fault_status[i] = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   }
   scl_output.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   scl_output.should_reset = false;

   return (scl_output);
}

TEST_GROUP(f360_safety_control_logic)
{
   const F360_Core_Info_T core_info = {};
   const F360_Host_T host = {};
   const rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   const F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   const F360_Object_Log_Output_T obj_log = {};
   const Tracker_Info_Log_T r_tracker_info = {};

   TEST_SETUP()
   {}

   TEST_TEARDOWN()
   {
      mock().checkExpectations();
      mock().clear();
   }

   Input_Diagnostics_Mock input_diagnostics_mock;
   Output_Diagnostics_Mock output_diagnostics_mock;
   SafetyControlLogic safetyControlLogic { input_diagnostics_mock, output_diagnostics_mock };
};

/**
*\purpose  when there is different outputs of SCL_Output_T the Operator should be not equal.
*\req      NA
*/
TEST(f360_safety_control_logic, operatorBoolNotEqualCheck)
{
   /** \precond
   * there are different SCL_Output_T
   */
   SafetyControlLogic::SCL_Output_T scl_out1 = get_empty_SCL_Output();
   SafetyControlLogic::SCL_Output_T scl_out2 = get_empty_SCL_Output();
   scl_out1.object_track_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
   scl_out1.should_reset = true;

   /** \action
   * call operator bool
   */
   bool result = operator==(scl_out1, scl_out2);

   /** \result
   * Check operator returns false
   */
   bool expected = false;
   CHECK_EQUAL(expected, result);
}

/**
*\purpose  when there is different outputs of SCL_Output_T the Operator should be not equal.
*\req      NA
*/
TEST(f360_safety_control_logic, operatorBoolOneObjectNotEqualCheck)
{
   /** \precond
   * there are different should_reset
   */
   SafetyControlLogic::SCL_Output_T scl_out1 = get_empty_SCL_Output();
   SafetyControlLogic::SCL_Output_T scl_out2 = get_empty_SCL_Output();
   scl_out1.should_reset = true;

   /** \action
   * call operator bool
   */
   bool result = operator==(scl_out1, scl_out2);

   /** \result
   * Check operator returns false
   */
   bool expected = false;
   CHECK_EQUAL(expected, result);
}

/**
*\purpose  Check input is returned correctly after execution of Input Diagnostics
*\req      NA
*/
TEST(f360_safety_control_logic, checkInputFaultReturned)
{
   /** \precond
   * there are input faults
   */
   Input_Faults_T input_faults = {};
   Output_Faults_T output_faults = {};

   bool expected_input_fault = true;

   input_faults.core_info.time_us_no_increase = true;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   //call evaluate_cycle once
   safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   //call evaluate_cycle twice
   safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   //call evaluate_cycle thrice
   safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   /** \action
   * call get_input_status
   */
   const Input_Faults_T& result = safetyControlLogic.get_input_status();

   /** \result
   * Check get_input_status returned value
   */
   CHECK_EQUAL(expected_input_fault, result.core_info.time_us_no_increase);
}

/**
*\purpose  Check Output is returned correctly after execution of Output Diagnostics
*\req      NA
*/
TEST(f360_safety_control_logic, checkOutputFaultReturned)
{
   /** \precond
   * there are output faults
   */
   Input_Faults_T input_faults = {};
   Output_Faults_T output_faults = {};

   bool expected_output_fault = false;

   output_faults.f_track_positions_faulty = false;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   //call evaluate_cycle
   safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   /** \action
   * call get_input_status
   */
   const Output_Faults_T& result = safetyControlLogic.get_output_status();

   /** \result
   * Check get_output_status returned value
   */
   CHECK_EQUAL(expected_output_fault, result.f_track_positions_faulty);
}

/**
*\purpose  Check output is returned correctly after execution of Safety Control Logic
*\req      NA
*/
TEST(f360_safety_control_logic, checkSclOutputReturned)
{
   /** \precond
   * there are output faults
   */
   Input_Faults_T input_faults = {};
   Output_Faults_T output_faults = {};
   SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();

   expected.object_track_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
   expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
   expected.should_reset = true;

   output_faults.f_enough_faulty_unique_objs = true;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   //call evaluate_cycle once
   safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   /** \action
   * call get_scl_status
   */
   const SafetyControlLogic::SCL_Output_T& result = safetyControlLogic.get_scl_status();

   /** \result
   * Check get_scl_status returned value
   */
   CHECK_EQUAL(expected, result);
}

/**
*\purpose  Check output is returned correctly after execution of Safety Control Logic
*\req      NA
*/
TEST(f360_safety_control_logic, NoFaultsDetected) {

   Input_Faults_T input_faults = {};
   Output_Faults_T output_faults = {};

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   auto result = safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   CHECK_EQUAL(result.overall_fault_status, 195);
}

/**
*\purpose  Check Safety Control Logic produce no fault after initialize
*\req      NA
*/
TEST(f360_safety_control_logic, CheckIfSCLIsInitialized) {

   SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();

   Input_Faults_T input_faults = {};
   Output_Faults_T output_faults = {};
   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
   mock().expectOneCall("Input_Diagnostics::Initialize");

   safetyControlLogic.initialize();

   auto result = safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   CHECK_EQUAL(expected, result);
}


/** @}*/

