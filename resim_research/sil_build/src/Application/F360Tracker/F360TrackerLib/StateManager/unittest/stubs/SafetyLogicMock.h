#ifndef SAFETY_LOGIC_MOCK_H
#define SAFETY_LOGIC_MOCK_H

#include "f360_safety_control_logic.h"
#include "f360_input_diagnostics_mock.h"
#include "f360_output_diagnostics_mock.h"

namespace f360_variant_A
{
   class SafetyLogicMock : public SafetyControlLogic
   {
   public:
      SafetyLogicMock(Input_Diagnostics_Mock &input_diagnostics, Output_Diagnostics_Mock &output_diagnostics)
         : SafetyControlLogic(input_diagnostics, output_diagnostics),
           scl{},
           in_faults{},
           out_faults{}
      {};

      virtual SCL_Output_T evaluate_cycle(
         const F360_Core_Info_T& core_info,
         const F360_Host_T& host,
         const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
         const F360_Object_Log_Output_T& obj_log,
         const Tracker_Info_Log_T &r_tracker_info); // function returns flag stating whether critical fault was detected

      virtual const SCL_Output_T& get_scl_status() const;
      virtual const Input_Faults_T& get_input_status() const;
      virtual const Output_Faults_T& get_output_status() const;

      virtual void initialize();

   private:
       SCL_Output_T scl;
       Input_Faults_T in_faults;
       Output_Faults_T out_faults;
   };
}
#endif
