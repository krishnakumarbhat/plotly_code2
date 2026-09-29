#include "TrackerMock.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
namespace f360_variant_A
{
    TrackerMock::TrackerMock()
   {

   }

   TrackerMock::~TrackerMock()
   {

   }

   void TrackerMock::Execute(
      const F360_Core_Info_T& core_info,
      const F360_Host_T &host,
      const ocg::OCG_Outputs_T* const occupancy_grid,
      const sg::SG_Output_T* const p_sg_output,
      const rspp_variant_A::RSPP_Detection_List_T &det_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]) const
   {
      mock().actualCall("Execute");
   }

   void TrackerMock::Initialize() const
   {
      mock().actualCall("Initialize");
   }

   void TrackerMock::Initialize(const F360_Variant_T& input_variant) const
   {
      mock().actualCall("Initialize(Variant)");
   }

   bool TrackerMock::Initialize_Tracker_State_From_Log(
         const F360_Radar_Sensor_T(&r_sensors)[MAX_NUMBER_OF_SENSORS],
         const F360_Host_T& r_host,
         const F360_Object_Log_Output_T& object_log,
         const Tracker_Info_Log_T& r_tracker_info,
         const F360_Host_Props_Log_T& r_host_props,
         const F360_Static_Env_Poly_Log_T& r_static_env_polys,
         const Tracker_Internal_T& r_tracker_internals) const
   {
      mock().actualCall("Initialize_Tracker_State_From_Log");
      return bool{};
   }
   
   void TrackerMock::Reset() const
   {
      mock().actualCall("Reset");
   }

   void TrackerMock::Log_Objects(F360_Object_Log_Output_T& obj_log) const
   {
      mock().actualCall("Log_Objects");
   }
}


