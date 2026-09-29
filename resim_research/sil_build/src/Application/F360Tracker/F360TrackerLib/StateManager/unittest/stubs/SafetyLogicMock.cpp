#include "SafetyLogicMock.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
namespace f360_variant_A
{
   SafetyControlLogic::SCL_Output_T SafetyLogicMock::evaluate_cycle(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Object_Log_Output_T& obj_log,
      const Tracker_Info_Log_T &r_tracker_info)
   {
      SCL_Output_T scl_status = {};
      int return_val = mock().actualCall("evaluate_cycle").returnIntValue();
      scl_status.should_reset = static_cast<bool>(return_val);
      return scl_status;
   }

   const SafetyControlLogic::SCL_Output_T& SafetyLogicMock::get_scl_status() const
   {
      mock().actualCall("get_scl_status");
      return scl;
   }

   const Input_Faults_T& SafetyLogicMock::get_input_status() const
   {
      mock().actualCall("get_input_status");
      return in_faults;
   }

   const Output_Faults_T& SafetyLogicMock::get_output_status() const
   {
      mock().actualCall("get_output_status");
      return out_faults;
   }

   void SafetyLogicMock::initialize()
   {
      mock().actualCall("SafetyControlLogic::initialize");
   }
}


