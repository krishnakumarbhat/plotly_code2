#ifndef TRACKER_MOCK_H
#define TRACKER_MOCK_H

#include "f360_tracker.h"

namespace f360_variant_A
{
   class TrackerMock : public F360_Tracker
   {
   public:
      TrackerMock();
      ~TrackerMock();

      void Execute(
         const F360_Core_Info_T& core_info,
         const F360_Host_T& host,
         const ocg::OCG_Outputs_T* const occupancy_grid,
         const sg::SG_Output_T* const p_sg_output,
         const rspp_variant_A::RSPP_Detection_List_T& det_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]) const;

      void Initialize() const;
      void Initialize(const F360_Variant_T& input_variant) const;

      bool Initialize_Tracker_State_From_Log(
         const F360_Radar_Sensor_T(&r_sensors)[MAX_NUMBER_OF_SENSORS],
         const F360_Host_T& r_host,
         const F360_Object_Log_Output_T& object_log,
         const Tracker_Info_Log_T& r_tracker_info,
         const F360_Host_Props_Log_T& r_host_props,
         const F360_Static_Env_Poly_Log_T& r_static_env_polys,
         const Tracker_Internal_T& r_tracker_internals) const;
         
      void Reset() const;

      void Log_Objects(F360_Object_Log_Output_T& obj_log) const;
   };
}
#endif
