/** \file
This file contains qualification tests for evaluation of the fault status from Input Diagnostics and Output Diagnostics
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
      (first.object_track_fault_status  == second.object_track_fault_status) &&
      (first.angle_jump_fault_status == second.angle_jump_fault_status) &&
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
*\purpose  when there is no Input Faults from Input Diagnostics and no Output Faults from Output
*\         Diagnostics Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and
*\         should_reset as False.
*\req      CPR-3907
*/
TEST(f360_safety_control_logic, whenThereIsNoFaults)
{
   /** \precond
   * there are no faults from Input_Diagnostics and Output_Diagnostics
   */
   Input_Faults_T input_faults = {};
   Output_Faults_T output_faults = {};
   SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   /** \action
   * call evaluate_cycle
   */
   SafetyControlLogic::SCL_Output_T result = safetyControlLogic.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   /** \result
   * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
   */
   CHECK_EQUAL(expected, result);
}

/**
*\purpose  when there is Input Fault from Core_Info_Faults_T, after 1 cycle Evaluate Cycle
*\         should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False.
*\req      CPR-3915, CPR-3880, CPR-3881, CPR-3882, CPR-3883
*/
TEST(f360_safety_control_logic, whenThereIsCoreInputFaultforOneCycle)
{
   /** \precond
   * there are input faults
   * i = 0: time_us_no_increase fault
   * i = 1: cnt_loops_no_increase fault
   * i = 2: elapsed_time_below_lower_limit fault
   * i = 3: elapsed_time_above_upper_limit fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle once
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);


      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault from Core_Info_Faults_T, after 2 cycle Evaluate Cycle
*\         should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False.
*\req      CPR-3915, CPR-3880, CPR-3881, CPR-3882, CPR-3883
*/
TEST(f360_safety_control_logic, whenThereIsCoreInputFaultforTwoCycle)
{
   /** \precond
   * Input fault exists and evaluate cycle should be called once
   * i = 0: time_us_no_increase fault
   * i = 1: cnt_loops_no_increase fault
   * i = 2: elapsed_time_below_lower_limit fault
   * i = 3: elapsed_time_above_upper_limit fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //Call evaluate_cycle first time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle second time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when Input Fault from Core_Info_Faults_T is disappears after 2 cycle,
*\         Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False.
*\req      CPR-3915, CPR-3880, CPR-3881, CPR-3882, CPR-3883
*/
TEST(f360_safety_control_logic, whenThereIsNoCoreInputFaultAfterTwoCycles)
{
   /** \precond
   * there are input faults and evaluate cycle called twice and fault disappears
   * i = 0: time_us_no_increase fault
   * i = 1: cnt_loops_no_increase fault
   * i = 2: elapsed_time_below_lower_limit fault
   * i = 3: elapsed_time_above_upper_limit fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle first time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle second time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //Check No Faults of Core_Info_Faults_T after 2 cycles
      input_faults.core_info.time_us_no_increase            = false;
      input_faults.core_info.cnt_loops_no_increase          = false;
      input_faults.core_info.elapsed_time_below_lower_limit = false;
      input_faults.core_info.elapsed_time_above_upper_limit = false;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle third time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Core Info Input Fault, after 3 cycle Evaluate Cycle should return
*\         core_info_fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\req      CPR-3915, CPR-3880, CPR-3881, CPR-3882, CPR-3883, CPR-3935
*/
TEST(f360_safety_control_logic, whenThreIsCoreInputFaultEvalueateCycleCalledThrice)
{
   /** \precond
   * there are input faults
   * i = 0: time_us_no_increase fault
   * i = 1: cnt_loops_no_increase fault
   * i = 2: elapsed_time_below_lower_limit fault
   * i = 3: elapsed_time_above_upper_limit fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle thrice
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS after 3 indices and should_reset as false
      expected.core_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is core info Input Fault, after 3 cycle Evaluate Cycle should return
*\         core_info_fault_status as FAULT_PRESENT_STATUS and should_reset as false. To set back fault_status
*\         as FAULT_NOT_PRESENT_STATUS, it should check for 10 indices for no faults
*\req      CPR-3915, CPR-3880, CPR-3881, CPR-3882, CPR-3883, CPR-3931, CPR-3935
*/
TEST(f360_safety_control_logic, whenThreIsElapsedTimeFaultEvalueateCycleCalledTenTimesToCheckNoFaults)
{
   /** \precond
   * there are input faults
   * i = 0: time_us_no_increase fault
   * i = 1: cnt_loops_no_increase fault
   * i = 2: elapsed_time_below_lower_limit fault
   * i = 3: elapsed_time_above_upper_limit fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle thrice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //No faults
      input_faults.core_info.time_us_no_increase            = false;
      input_faults.core_info.cnt_loops_no_increase          = false;
      input_faults.core_info.elapsed_time_below_lower_limit = false;
      input_faults.core_info.elapsed_time_above_upper_limit = false;

      //Call evaluate_cycle 9 times
      for (int j = 0; j<9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         expected.core_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as false
      */
      expected.core_info_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault, after 3 cycle Evaluate Cycle should return
*\         core_info_fault_status as FAULT_PRESENT_STATUS and should_reset as false. To set back fault_status
*\         as FAULT_NOT_PRESENT_STATUS, it should check for 10 indices for no faults. While
*\         Setting back to FAULT_NOT_PRESENT_STATUS, if fault occurs the no fault should be checked
*\         again for 10 more indices
*\req      CPR-3931, CPR-3935
*/
TEST(f360_safety_control_logic, whenThreIsCoreFaultEvalueateCycleCalledTenTimesToCheckNoFaults)
{
   /** \precond
   * there are input faults
   * i = 0: time_us_no_increase fault
   * i = 1: cnt_loops_no_increase fault
   * i = 2: elapsed_time_below_lower_limit fault
   * i = 3: elapsed_time_above_upper_limit fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle thrice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //No faults
      input_faults.core_info.time_us_no_increase            = false;
      input_faults.core_info.cnt_loops_no_increase          = false;
      input_faults.core_info.elapsed_time_below_lower_limit = false;
      input_faults.core_info.elapsed_time_above_upper_limit = false;

      //Call evaluate_cycle 6 times
      for (int j = 0; j<6; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         expected.core_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      //Faults present
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);


      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //Call Evaluate_cycle
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //Evaluate Cycle should return core_info_fault_status as FAULT_PRESENT_STATUS and should_reset as false
      expected.core_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);

      //No faults
      input_faults.core_info.time_us_no_increase            = false;
      input_faults.core_info.cnt_loops_no_increase          = false;
      input_faults.core_info.elapsed_time_below_lower_limit = false;
      input_faults.core_info.elapsed_time_above_upper_limit = false;

      //Call evaluate_cycle 9 times
      for (int j = 0; j<9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         expected.core_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return core_info_fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as false
      */
      expected.core_info_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  When there is Input Fault from Host_Info_Faults_T for 1 consecutive cycles.
*\         Evaluate Cycle should return host_info_fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False.
*\         Test for all the different host info faults
*\req      CPR-3918, CPR-3921, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsHostInputFaultforOneCycle)
{
   /** \precond
   * Set host info fault to true
   * i = 0: vehicle_index_no_increase fault
   * i = 1: host_yawrate_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 2; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Set faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = (0 == i);
      input_faults.host_info.host_lateral_acceleration_invalid      = (1 == i);

      /** \action
      * call evaluate_cycle once
      */
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  When there is Input Fault from Host_Info_Faults_T for 2 consecutive cycles.
*\         Evaluate Cycle should return host_info_fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False.
*\         Test for all the different host info faults
*\req      CPR-3918, CPR-3921, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsHostInputFaultforTwoCycle)
{
   /** \precond
   * Set host info fault to true
   * i = 0: vehicle_index_no_increase fault
   * i = 1: host_yawrate_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 2; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Set faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = (0 == i);
      input_faults.host_info.host_yawrate_invalid                   = (1 == i);

      /** \action
      * call evaluate_cycle two times time
      */
      for (int j = 0; j < 2; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      }

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  When there is Input Fault from Host_Info_Faults_T for 2 consecutive cycles then the fault dissapears.
*\         Evaluate Cycle should return host_info_fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False.
*\         Test for all the different host info faults
*\req      CPR-3917, CPR-3921, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsNoHostInputFaultAfterTwoCycles)
{
   /** \precond
   * Set host info fault to true
   * i = 0: vehicle_index_no_increase fault
   * i = 1: host_yawrate_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 2; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Set faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = (0 == i);
      input_faults.host_info.host_yawrate_invalid                   = (1 == i);

      /** \action
      * 1. call evaluate_cycle 2 times
      * 2. Reset the fault
      * 3. call evaluate cycle a third time
      */
      // 1. call evaluate_cycle 2 times
      for (int j = 0; j < 2; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      }

      // 2. Reset faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = false;
      input_faults.host_info.host_yawrate_invalid                   = false;

      // 3. call evaluate cycle a third time 
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  When there is Input Fault from Host_Info_Faults_T for 3 consecutive cycles.
*\         Evaluate Cycle should return host_info_fault_status as FAULT_PRESENT_STATUS and should_reset as False.
*\         Test for all the different host info faults
*\req      CPR-3918, CPR-3921, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsHostInputFaultforThreeCycles)
{
   /** \precond
   * Set host info fault to true
   * i = 0: vehicle_index_no_increase fault
   * i = 1: host_yawrate_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 2; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Set faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = (0 == i);
      input_faults.host_info.host_yawrate_invalid                   = (1 == i);

      /** \action
      * 1. call evaluate_cycle 2 times (Fault should no be present)
      * 2. call evaluate cycle a third time
      */
      // 1. call evaluate_cycle 2 times
      for (int j = 0; j < 2; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         CHECK_EQUAL(expected, result);
      }

      // 3. call evaluate cycle a third time
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS and should_reset as False
      */
      expected.host_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault, after 3 cycle Evaluate Cycle should return
*\         host_info_fault_status as FAULT_PRESENT_STATUS and should_reset as false. To set back fault_status
*\         as FAULT_NOT_PRESENT_STATUS, it should check for 10 indices for no faults
*\req      CPR-3918, CPR-3921,  CPR-3932, CPR-3935
*/
TEST(f360_safety_control_logic, whenThreIsHostInfoFaultEvaluateCycleCalledTenTimesToCheckNoFaults)
{
   /** \precond
   * Set host info fault to true
   * i = 0: vehicle_index_no_increase fault
   * i = 1: host_yawrate_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 2; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Set faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = (0 == i);
      input_faults.host_info.host_yawrate_invalid                   = (1 == i);

      /** \action
      * 1. call evaluate_cycle 3 times
      * 2. Check that fault is preset
      * 3. Reset Fault
      * 4. call evaluate_cycle 9 times (Fault should be present)
      * 5. call evaluate_cycle time 10 after reset
      */
      // 1. call evaluate_cycle 3 times
      for (int j = 0; j < 3; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      }

      // 2. Check that fault is preset
      expected.host_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);

      // 3. Reset faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = false;
      input_faults.host_info.host_yawrate_invalid                   = false;

      // 4. call evaluate_cycle 9 times
      for (int j = 0; j < 9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         expected.host_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      // 5. call evaluate_cycle time 10 after reset
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as false
      */
      expected.host_info_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault, after 3 cycle Evaluate Cycle should return
*\         host_info_fault_status as FAULT_PRESENT_STATUS and should_reset as false. To set back fault_status
*\         as FAULT_NOT_PRESENT_STATUS, it should check for 10 indices for no faults. While
*\         Setting back to FAULT_NOT_PRESENT_STATUS, if fault occurs the no fault should be checked
*\         again for 10 more indices
*\req      CPR-3918,  CPR-3932, CPR-3935
*/
TEST(f360_safety_control_logic, whenThreIsHostInfoFaultEvaluateCycleCalledTenTimesToCheckNoFaults_2)
{
   /** \precond
   * Set host info fault to true
   * i = 0: vehicle_index_no_increase fault
   * i = 2: host_yawrate_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 2; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Set faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = (0 == i);
      input_faults.host_info.host_yawrate_invalid                   = (1 == i);

      /** \action
      * 1. call evaluate_cycle 3 times
      * 2. Check that fault is preset
      * 3. Reset Fault
      * 4. call evaluate_cycle 6 times (Fault should be present)
      * 5. Set fault
      * 6. call evaluate_cycle (Fault should be present)
      * 7. Reset Fault
      * 8. call evaluate_cycle 9 times (Fault should be present)
      * 9. call evaluate_cycle time 10 after reset
      */
      // 1. call evaluate_cycle 3 times
      for (int j = 0; j < 3; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      }

      // 2. Check that fault is preset
      expected.host_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);

      // 3. Reset faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = false;
      input_faults.host_info.host_yawrate_invalid                   = false;

      // 4. call evaluate_cycle 6 times
      for (int j = 0; j < 6; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         expected.host_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      // 5. Set faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = (0 == i);
      input_faults.host_info.host_yawrate_invalid                   = (1 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      // 6. call evaluate_cycle 
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      expected.host_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);

      // 7. Reset faults of Core_Info_Faults_T
      input_faults.host_info.vehicle_index_no_increase              = false;
      input_faults.host_info.host_yawrate_invalid                   = false;

      // 8. Call evaluate_cycle 9 times
      for (int j = 0; j < 9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         expected.host_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.should_reset = false;
         CHECK_EQUAL(expected, result);
      }

      // 9. call evaluate_cycle time 10 after reset
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as false
      */
      expected.host_info_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.should_reset = false;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault from Raw_Detection_Faults_T, after 1 cycle Evaluate Cycle
*\         should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False.
*\req      CPR-5089, CPR-5087, CPR-5086, CPR-5088, CPR-5091, CPR-5092, CPR-5093, CPR-5094
*/
TEST(f360_safety_control_logic, whenThereIsRawDetectionFaultforOneCycle)
{
   /** \precond
   * there are input faults
   * i = 0: range_is_invalid fault
   * i = 1: range_rate_is_invalid fault
   * i = 2: azimuth_is_invalid fault
   * i = 3: elevation_is_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.raw_detection.range_is_invalid               = (0 == i);
      input_faults.raw_detection.range_rate_is_invalid          = (1 == i);
      input_faults.raw_detection.azimuth_is_invalid             = (2 == i);
      input_faults.raw_detection.elevation_is_invalid           = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle once
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);


      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault from Raw_Detection_Faults_T, after 3 cycle Evaluate Cycle should return
*\         raw_detection_fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\req      CPR-5089, CPR-5087, CPR-5086, CPR-5088, CPR-5091, CPR-5092, CPR-5093, CPR-5094
*/
TEST(f360_safety_control_logic, whenThreIsRawDetectionEvalueateCycleCalledThrice)
{
   /** \precond
   * there are input faults
   * i = 0: range_is_invalid fault
   * i = 1: range_rate_is_invalid fault
   * i = 2: azimuth_is_invalid fault
   * i = 3: elevation_is_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.raw_detection.range_is_invalid               = (0 == i);
      input_faults.raw_detection.range_rate_is_invalid          = (1 == i);
      input_faults.raw_detection.azimuth_is_invalid             = (2 == i);
      input_faults.raw_detection.elevation_is_invalid           = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle thrice
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS after 3 indices and should_reset as false
      expected.raw_detection_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault from Raw_Detection_Faults_T after 3 indices, Evaluate Cycle should check for 
*\         10 indices for no faults to set back fault_status as FAULT_NOT_PRESENT_STATUS.
*\req      CPR-5089, CPR-5087, CPR-5086, CPR-5088, CPR-5091, CPR-5092, CPR-5093, CPR-5094
*/
TEST(f360_safety_control_logic, whenThreIsRawDetectionFaultEvalueateCycleCalledTenTimesToCheckNoFaults)
{
   /** \precond
   * there are input faults
   * i = 0: range_is_invalid fault
   * i = 1: range_rate_is_invalid fault
   * i = 2: azimuth_is_invalid fault
   * i = 3: elevation_is_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.raw_detection.range_is_invalid               = (0 == i);
      input_faults.raw_detection.range_rate_is_invalid          = (1 == i);
      input_faults.raw_detection.azimuth_is_invalid             = (2 == i);
      input_faults.raw_detection.elevation_is_invalid           = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle thrice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //No faults
      input_faults.raw_detection.range_is_invalid           = false;
      input_faults.raw_detection.range_rate_is_invalid      = false;
      input_faults.raw_detection.azimuth_is_invalid         = false;
      input_faults.raw_detection.elevation_is_invalid       = false;

      //Call evaluate_cycle 9 times
      for (int j = 0; j<9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         expected.raw_detection_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as false
      */
      expected.raw_detection_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is Input Fault from Raw_Detection_Faults_T after 3 indices, Evaluate Cycle should check for 
*\         10 indices for no faults to set back fault_status as FAULT_NOT_PRESENT_STATUS. While  Setting back to 
*\         FAULT_NOT_PRESENT_STATUS, if fault occurs the no fault should be checked again for 10 more indices.
*\req      CPR-5114, CPR-3935
*/
TEST(f360_safety_control_logic, callRawDetectionEvaluateCycleTenTimesToCheckAgainNoFaults)
{
   /** \precond
   * there are input faults
   * i = 0: range_is_invalid fault
   * i = 1: range_rate_is_invalid fault
   * i = 2: azimuth_is_invalid fault
   * i = 3: elevation_is_invalid fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.raw_detection.range_is_invalid               = (0 == i);
      input_faults.raw_detection.range_rate_is_invalid          = (1 == i);
      input_faults.raw_detection.azimuth_is_invalid             = (2 == i);
      input_faults.raw_detection.elevation_is_invalid           = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle thrice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //No faults
      input_faults.raw_detection.range_is_invalid           = false;
      input_faults.raw_detection.range_rate_is_invalid      = false;
      input_faults.raw_detection.azimuth_is_invalid         = false;
      input_faults.raw_detection.elevation_is_invalid       = false;

      //Call evaluate_cycle 6 times
      for (int j = 0; j<6; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         expected.raw_detection_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      //Faults present
      input_faults.raw_detection.range_is_invalid               = (0 == i);
      input_faults.raw_detection.range_rate_is_invalid          = (1 == i);
      input_faults.raw_detection.azimuth_is_invalid             = (2 == i);
      input_faults.raw_detection.elevation_is_invalid           = (3 == i);


      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //Call Evaluate_cycle
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //Evaluate Cycle should return raw_detection_fault_status as FAULT_PRESENT_STATUS and should_reset as false
      expected.raw_detection_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);

      //No faults
      input_faults.raw_detection.range_is_invalid           = false;
      input_faults.raw_detection.range_rate_is_invalid      = false;
      input_faults.raw_detection.azimuth_is_invalid         = false;
      input_faults.raw_detection.elevation_is_invalid       = false;

      //Call evaluate_cycle 9 times
      for (int j = 0; j<9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         expected.raw_detection_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return raw_detection_fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as false
      */
      expected.raw_detection_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is look_index_no_increase Fault from Input Diagnostics,
*\         after 2 cycle Evaluate Cycle should return fault_status as
*\         FAULT_NOT_PRESENT_STATUS and should_reset as false.
*\req      CPR-3926
*/
TEST(f360_safety_control_logic, whenThereIsLookIndexFaultforTwoCycles)
{
   /** \precond
   * there are input faults
   */

   //Faults of look_index_no_increase one sensor per loop
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      input_faults.sensors[i].look_index_no_increase = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //Call evaluate_cycle
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle second time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS and should_reset as False
      */
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is look_index_no_increase Fault from Input Diagnostics,
*\         after 3 cycle Evaluate Cycle
*\         should return fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\req      CPR-3926, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsLookIndexFault)
{
   /** \precond
   * there are input faults
   */

   //Faults of look_index_no_increase one sensor per loop
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      input_faults.sensors[i].look_index_no_increase = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle first time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle second time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle third time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS and should_reset as False
      */
      expected.sensors_fault_status[i] = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is sensor_vs_tracker_timestamp_divergence Fault from Input Diagnostics,
*\         after 3 cycle Evaluate Cycle
*\         should return fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\req      CPR-3925, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsRadarSensorTimeStrampFault)
{
   /** \precond
   * there are input faults
   */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once 
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle third time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS and should_reset as False
      */
      expected.sensors_fault_status[i] = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is one sensor Fault from Input Diagnostics, after 3 cycle Evaluate Cycle
*\         should return fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\         To set back fault_status to FAULT_NOT_PRESENT_STATUS, it should check for 10 indices with no faults
*\req      CPR-3925, CPR-3931, CPR-3933, CPR-3935
*/
TEST(f360_safety_control_logic, whenThreIsSensorTimestampFaultEvalueateCycleCalledTenTimesToCheckNoFaults)
{
   /** \precond
   * there are input faults
   */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      // Check fault present
      expected.sensors_fault_status[i] = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);

      //No Fault
      input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = false;

      for (int j = 0; j< 9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         expected.sensors_fault_status[i] = SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);
      }

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as False
      */
      expected.sensors_fault_status[i] = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is sensor Fault from Input Diagnostics,after 3 cycle Evaluate Cycle
*\         should return fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\         To set back fault_status to Ok, it should check for 10 indices with no faults.
*\         While setting back to FAULT_NOT_PRESENT_STATUS, if there fault occurs, again
*\         no faults checked for 10 more indices.
*\req      CPR-3922, CPR-3889, CPR-3933, CPR-3923, CPR-3890, CPR-3924, CPR-3891
*\         CPR-3925, CPR-3892, CPR-3934, CPR-3926, CPR-3893, CPR-3935
*/
TEST(f360_safety_control_logic, whenThreIsSensorFaultEvaluateCycleCalledTenTimesToCheckNoFaults)
{
   /** \precond
   * there are input faults
   */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++) // Loop all sensors
   {
      for (int j = 0; j < 5; j++)
      {
         Input_Faults_T input_faults = {};
         Output_Faults_T output_faults = {};
         SafetyControlLogic::SCL_Output_T result = {};
         SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
         SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

         //Sensor Faults Present
         input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = (0 == j);
         input_faults.sensors[i].look_index_no_increase                 = (1 == j);
         input_faults.sensors_calibs[i].polarity_is_invalid             = (2 == j);
         input_faults.sensors_calibs[i].mounting_pos_is_invalid         = (3 == j);
         input_faults.sensors_calibs[i].boresight_angle_is_invalid      = (4 == j);

         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         //call evaluate_cycle
         safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         //call evaluate_cycle
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         CHECK_EQUAL(expected, result);

         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         //call evaluate_cycle
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         // Check fault present
         expected.sensors_fault_status[i]        = (2 > j) ? SafetyControlLogic::FAULT_PRESENT_STATUS : SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
         expected.sensors_calibs_fault_status[i] = (2 > j) ? SafetyControlLogic::FAULT_NOT_PRESENT_STATUS : SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);

         //No Faults
         input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = false;
         input_faults.sensors[i].look_index_no_increase                 = false;
         input_faults.sensors_calibs[i].polarity_is_invalid             = false;
         input_faults.sensors_calibs[i].mounting_pos_is_invalid         = false;
         input_faults.sensors_calibs[i].boresight_angle_is_invalid      = false;

         for (int k = 0; k< 5; k++)
         {
            mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
            mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

            result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
            expected.sensors_fault_status[i]        = (2 > j) ? SafetyControlLogic::FAULT_PRESENT_STATUS : SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
            expected.sensors_calibs_fault_status[i] = (2 > j) ? SafetyControlLogic::FAULT_NOT_PRESENT_STATUS : SafetyControlLogic::FAULT_PRESENT_STATUS;
            expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
            CHECK_EQUAL(expected, result);
         }

         //Faults Present
         input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = (0 == j);
         input_faults.sensors[i].look_index_no_increase                 = (1 == j);
         input_faults.sensors_calibs[i].polarity_is_invalid             = (2 == j);
         input_faults.sensors_calibs[i].mounting_pos_is_invalid         = (3 == j);
         input_faults.sensors_calibs[i].boresight_angle_is_invalid      = (4 == j);

         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         // check evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS and should_reset as False
         expected.sensors_fault_status[i]        = (2 > j) ? SafetyControlLogic::FAULT_PRESENT_STATUS : SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
         expected.sensors_calibs_fault_status[i] = (2 > j) ? SafetyControlLogic::FAULT_NOT_PRESENT_STATUS : SafetyControlLogic::FAULT_PRESENT_STATUS;
         expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
         CHECK_EQUAL(expected, result);

         //No Faults
         input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence = false;
         input_faults.sensors[i].look_index_no_increase                 = false;
         input_faults.sensors_calibs[i].polarity_is_invalid             = false;
         input_faults.sensors_calibs[i].mounting_pos_is_invalid         = false;
         input_faults.sensors_calibs[i].boresight_angle_is_invalid      = false;
         for (int k = 0; k<9; k++)
         {
            mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
            mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

            result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
            expected.sensors_fault_status[i]        = (2 > j) ? SafetyControlLogic::FAULT_PRESENT_STATUS : SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
            expected.sensors_calibs_fault_status[i] = (2 > j) ? SafetyControlLogic::FAULT_NOT_PRESENT_STATUS : SafetyControlLogic::FAULT_PRESENT_STATUS;
            expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
            CHECK_EQUAL(expected, result);
         }

         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         /** \action
         * call evaluate_cycle
         */
         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

         /** \result
         * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS after 10 indices and should_reset as False
         */
         expected = get_empty_SCL_Output();
         CHECK_EQUAL(expected, result);
      }
   }
}

/**
*\purpose  when there is mounting_pos_is_invalid Fault from Input Diagnostics,
*\         after 3 cycle Evaluate Cycle
*\         should return fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\req      CPR-3922, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsMountPosInvalidFault)
{
   /** \precond
   * there are input faults
   */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      input_faults.sensors_calibs[i].mounting_pos_is_invalid = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle first time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle second time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle third time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS and should_reset as False
      */
      expected.sensors_calibs_fault_status[i] = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when there is polarity_is_invalid Fault from Input Diagnostics,
*\         after 3 cycle Evaluate Cycle
*\         should return fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\req      CPR-3923, CPR-3935
*/
TEST(f360_safety_control_logic, whenThereIsMountPolarityInvalidFault)
{
   /** \precond
   * there are input faults
   */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      input_faults.sensors_calibs[i].polarity_is_invalid = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle first time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle second time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle third time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS and should_reset as False
      */
      expected.sensors_calibs_fault_status[i] = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}

/**
*\purpose  when boresight_angle_is_invalid is set to true from Input Diagnostics,
*\         after 3 cycle Evaluate Cycle
*\         should return fault_status as FAULT_PRESENT_STATUS and should_reset as false.
*\req      CPR-3891, CPR-3935
*/
TEST(f360_safety_control_logic, When_Mount_Boresight_Angle_Is_Invalid)
{
   /** \precond
   * there are input faults
   */
   for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      input_faults.sensors_calibs[i].boresight_angle_is_invalid = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle first time
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle second time
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      CHECK_EQUAL(expected, result); // After two cycles of fault present, the fault status should not set yet.
      /** \action
      * call evaluate_cycle third time
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS after three cycles and should_reset as False
      */
      expected.sensors_calibs_fault_status[i] = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);
   }
}
/**\purpose
 * When there is fault signals from Output Diagnostics, Evaluate Cycle
 * should always return fault_status as FAULT_PRESENT_STATUS.
 *
 *\req   CPR-3929, CPR-3935
 */
TEST(f360_safety_control_logic, whenThereIsObjectFaultFault)
{
   /** \precond
   * there are output faults
   * f_enough_faulty_unique_objs = true;
   */

   Input_Faults_T input_faults   = {};
   Output_Faults_T output_faults = {};
   SafetyControlLogic::SCL_Output_T result = {};
   SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
   SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

   //Enough faults of F360_Object_Track_T
   output_faults.f_enough_faulty_unique_objs = true;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   //call evaluate_cycle
   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   //Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS
   expected.object_track_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
   expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;

   CHECK_EQUAL(expected.object_track_fault_status, result.object_track_fault_status);
   CHECK_EQUAL(expected.overall_fault_status, result.overall_fault_status);

   //Not enough faults of F360_Object_Track_T
   output_faults.f_enough_faulty_unique_objs = false;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   /** \action
   * call evaluate_cycle
   */
   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   /** \result
   * Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS
   */
   expected.object_track_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
   expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
   CHECK_EQUAL(expected.object_track_fault_status, result.object_track_fault_status);
   CHECK_EQUAL(expected.overall_fault_status, result.overall_fault_status);
}

/** \purpose
 * When there is a property-fault flag set (f_track_positions_faulty,
 * f_track_velocities_faulty, or f_track_accelerations_faulty) while
 * object_track_fault_status is already FAULT_PRESENT_STATUS, the 10-cycle
 * countdown counter is reset and the status remains FAULT_PRESENT_STATUS.
 * After 10 consecutive no-fault cycles the status clears.
 *
 *\req      CPR-3935, CPR-8564
*/
TEST(f360_safety_control_logic, whenThereIsObjectPropertyFaultAndThenNoFaults)
{

  /** \precond
   * there are output faults
   */

   // i = 0: f_track_positions_faulty
   // i = 1: f_track_velocities_faulty
   // i = 2: f_track_accelerations_faulty
   for (int i = 0; i < 3; i++)
   {
      Input_Faults_T input_faults   = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Pre-activate: trigger fault via f_enough_faulty_unique_objs (CPR-3929)
      output_faults.f_enough_faulty_unique_objs = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      output_faults.f_enough_faulty_unique_objs = false;

      // Property fault resets the countdown counter (CPR-8564 inhibition)
      output_faults.f_track_positions_faulty     = (0 == i);
      output_faults.f_track_velocities_faulty    = (1 == i);
      output_faults.f_track_accelerations_faulty = (2 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);

      // No faults — countdown runs for 9 cycles (counter 10 -> 1, all still FAULT_PRESENT)
      output_faults.f_track_positions_faulty     = false;
      output_faults.f_track_velocities_faulty    = false;
      output_faults.f_track_accelerations_faulty = false;

      for (int j = 0; j < 9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
         CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);
      }

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle (10th consecutive no-fault cycle)
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS
      */
      CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.overall_fault_status);
   }
}

 /** \purpose
 * When f_enough_faulty_unique_objs triggers object_track_fault_status to
 * FAULT_PRESENT_STATUS, it must remain FAULT_PRESENT_STATUS for 10 consecutive
 * no-fault cycles before clearing to FAULT_NOT_PRESENT_STATUS.
 * \req  CPR-3929, CPR-3935, CPR-8564
 */
TEST(f360_safety_control_logic, whenThereIsEnoughFaultyObjsForOneCycleAndThenNoFaults)
{
   /** \precond
   * there are output faults
   */

   Input_Faults_T input_faults   = {};
   Output_Faults_T output_faults = {};
   SafetyControlLogic::SCL_Output_T result = {};
   SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

   output_faults.f_enough_faulty_unique_objs = true;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
   CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
   CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);

   // No faults — countdown runs for 9 cycles (counter 10 -> 1, all still FAULT_PRESENT)
   output_faults.f_enough_faulty_unique_objs = false;

   for (int j = 0; j < 9; j++)
   {
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);
   }

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   /** \action
   * call evaluate_cycle (10th consecutive no-fault cycle)
   */
   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   /** \result
   * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS
   */
   CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.object_track_fault_status);
   CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.overall_fault_status);
}

/** \purpose
 * When a property-fault flag (f_track_positions_faulty, f_track_velocities_faulty,
 * or f_track_accelerations_faulty) appears twice while object_track_fault_status is
 * FAULT_PRESENT_STATUS, each occurrence resets the 10-cycle countdown counter.
 * After 10 consecutive no-fault cycles following the last occurrence the status clears.
 *
 * \req  CPR-8564
 */
TEST(f360_safety_control_logic, whenThereIsObjectPropertyFaultAndThenAgainAfterFiveCycles)
{
   /** \precond
    * f_enough_faulty_unique_objs = true
   */

   // i = 0: f_track_positions_faulty
   // i = 1: f_track_velocities_faulty
   // i = 2: f_track_accelerations_faulty
   for (int i = 0; i < 3; i++)
   {
      Input_Faults_T input_faults   = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      // Pre-activate: trigger fault via f_enough_faulty_unique_objs (CPR-3929)
      output_faults.f_enough_faulty_unique_objs = true;

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      output_faults.f_enough_faulty_unique_objs = false;

      // Property fault resets the countdown counter (CPR-8564 inhibition)
      output_faults.f_track_positions_faulty     = (0 == i);
      output_faults.f_track_velocities_faulty    = (1 == i);
      output_faults.f_track_accelerations_faulty = (2 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);

      // No faults for 5 cycles — countdown progresses (counter 10 -> 5)
      output_faults.f_track_positions_faulty     = false;
      output_faults.f_track_velocities_faulty    = false;
      output_faults.f_track_accelerations_faulty = false;

      for (int j = 0; j < 5; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
         CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);
      }

      // Property fault appears again — counter resets to 10 (CPR-8564 inhibition)
      output_faults.f_track_positions_faulty     = (0 == i);
      output_faults.f_track_velocities_faulty    = (1 == i);
      output_faults.f_track_accelerations_faulty = (2 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);

      // No faults — countdown runs for 9 cycles (counter 10 -> 1, all still FAULT_PRESENT)
      output_faults.f_track_positions_faulty     = false;
      output_faults.f_track_velocities_faulty    = false;
      output_faults.f_track_accelerations_faulty = false;

      for (int j = 0; j < 9; j++)
      {
         mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
         mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

         result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
         CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
         CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);
      }

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      /** \action
      * call evaluate_cycle (10th consecutive no-fault cycle)
      */
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      /** \result
      * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS
      */
      CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.overall_fault_status);
   }
}

/** purpose
 * When f_enough_faulty_unique_objs triggers object_track_fault_status to
 * FAULT_PRESENT_STATUS, and the fault re-occurs mid-countdown, the 10-cycle
 * countdown counter resets. Only after 10 consecutive no-fault cycles following
 * the last occurrence does the status clear to FAULT_NOT_PRESENT_STATUS.
 *
 *\req CPR-3929
*/
TEST(f360_safety_control_logic, whenThereIsEnoughFaultyObjsAndThenAgainAfterFiveCycles)
{
   /** \precond
   * there are output faults
   * f_enough_faulty_unique_objs = true;
   */
   Input_Faults_T input_faults   = {};
   Output_Faults_T output_faults = {};
   SafetyControlLogic::SCL_Output_T result = {};
   SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

   output_faults.f_enough_faulty_unique_objs = true;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
   CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
   CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);

   // No faults for 5 cycles — countdown progresses (counter 10 -> 5)
   output_faults.f_enough_faulty_unique_objs = false;

   for (int j = 0; j < 5; j++)
   {
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);
   }

   // Fault occurs again — counter resets to 10 (CPR-3929)
   output_faults.f_enough_faulty_unique_objs = true;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
   CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
   CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);

   // No faults — countdown runs for 9 cycles (counter 10 -> 1, all still FAULT_PRESENT)
   output_faults.f_enough_faulty_unique_objs = false;

   for (int j = 0; j < 9; j++)
   {
      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.object_track_fault_status);
      CHECK_EQUAL(SafetyControlLogic::FAULT_PRESENT_STATUS, result.overall_fault_status);
   }

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   /** \action
   * call evaluate_cycle (10th consecutive no-fault cycle)
   */
   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

   /** \result
   * Evaluate Cycle should return fault_status as FAULT_NOT_PRESENT_STATUS
   */
   CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.object_track_fault_status);
   CHECK_EQUAL(SafetyControlLogic::FAULT_NOT_PRESENT_STATUS, result.overall_fault_status);
}

/** purpose 
 * When there is only an angle jump presence fault for one cycle, angle_jump_fault_status should be FAULT_PRESENT_STATUS,
 * but overall fault status should be FAULT_NOT_PRESENT_STATUS
 * \req
 * TBD
*/
TEST(f360_safety_control_logic, WhenThereIsAngleJumpFault)
{
   /**precond
    * there is an angle jump fault present
    */
   Input_Faults_T input_faults   = {};
   Output_Faults_T output_faults = {};
   SafetyControlLogic::SCL_Output_T result = {};
   SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
   SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

   output_faults.f_severe_angle_jump_presence_fault = true;

   mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
   mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

   //call evaluate_cycle
   result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);
   expected.angle_jump_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
   expected.overall_fault_status = SafetyControlLogic::FAULT_NOT_PRESENT_STATUS;
   expected.should_reset = false;
   CHECK_EQUAL(expected, result);
}

/**
*\purpose  Existing faults should be cleared when Initialize is called. Evaluate Cycle should return
*\         FAULT_NOT_PRESENT_STATUS and should_reset as false.
*\req      
*/
TEST(f360_safety_control_logic, faultIsResetAfterInitialize)
{
   /** \precond
   * there are input faults
   * i = 0: time_us_no_increase fault
   * i = 1: cnt_loops_no_increase fault
   * i = 2: elapsed_time_below_lower_limit fault
   * i = 3: elapsed_time_above_upper_limit fault
   */

   // Loop over the different faults
   for (int i = 0; i < 4; i++)
   {
      Input_Faults_T input_faults = {};
      Output_Faults_T output_faults = {};
      SafetyControlLogic::SCL_Output_T result = {};
      SafetyControlLogic::SCL_Output_T expected = get_empty_SCL_Output();
      SafetyControlLogic safetyControlLogic_loop { input_diagnostics_mock, output_diagnostics_mock };

      //Input faults exist
      input_faults.core_info.time_us_no_increase            = (0 == i);
      input_faults.core_info.cnt_loops_no_increase          = (1 == i);
      input_faults.core_info.elapsed_time_below_lower_limit = (2 == i);
      input_faults.core_info.elapsed_time_above_upper_limit = (3 == i);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle once
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle twice
      safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle thrice
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //Evaluate Cycle should return fault_status as FAULT_PRESENT_STATUS after 3 indices and should_reset as false
      expected.core_info_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      expected.overall_fault_status = SafetyControlLogic::FAULT_PRESENT_STATUS;
      CHECK_EQUAL(expected, result);

      //Initialize SCL
      mock().expectOneCall("Input_Diagnostics::Initialize");
      safetyControlLogic_loop.initialize();

      mock().expectOneCall("Input_Diagnostics::Execute").andReturnValue(&input_faults);
      mock().expectOneCall("Output_Diagnostics::Execute").andReturnValue(&output_faults);

      //call evaluate_cycle after initalize
      result = safetyControlLogic_loop.evaluate_cycle(core_info, host, raw_detect_list, sensors, obj_log, r_tracker_info);

      //Expect no faults
      expected = get_empty_SCL_Output();
      CHECK_EQUAL(expected, result);

   }
}

