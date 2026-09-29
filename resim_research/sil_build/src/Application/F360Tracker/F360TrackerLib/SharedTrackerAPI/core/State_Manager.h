/*=========================================================================
*  FILE: State_Manager.h
*=========================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------
*
*  DESCRIPTION:
*    This file contains State Manager class declaration.
*
*
*=========================================================================
*------------------------------------------------------------------------------
*
* class:        State_Manager
*
* Description:  Manages tracker states, executes tracker, calls
*        SafetyControlLogic. Basing on information recieved from SafetyControlLogic
*        changes/keeps tracker state. Execute function should be
*        called inside wrapper.
*
* Deviations from standards: None
*
*========================================================================*/
#ifndef State_Manager_VARIANT_A_H
#define State_Manager_VARIANT_A_H

/******************************
* Includes
*******************************/
#include "f360_safety_control_logic.h"
#include "f360_tracker.h"
#include "f360_functional_safety_faults_log.h"
#include "f360_sg_interface.h"
#include "rspp_detection_list.h"

/******************************
* Class definition
*******************************/
namespace f360_variant_A
{
   class State_Manager
   {
   public:
      State_Manager(SafetyControlLogic&, F360_Tracker&);
      virtual ~State_Manager() = default;

      void Initialize(void) const;
      void Initialize(const F360_Variant_T& variant) const;

      virtual bool Initialize_Tracker_State_From_Log(
         const F360_Host_T& r_host,
         const F360_Object_Log_Output_T& object_log,
         const Tracker_Info_Log_T& r_tracker_info,
         const F360_Host_Props_Log_T& r_host_props,
         const F360_Static_Env_Poly_Log_T& r_static_env_polys,
         const Trailer_Detector_Log_T& r_trailer_detector,
         const Tracker_Internal_T& r_tracker_internals,
         const F360_Internal_Trailer_Detector_T& r_trailer_detector_internal) const;

      void execute( // Main function of TSM to be called - overload. For only SG as input
         const F360_Core_Info_T& core_info,
         const F360_Host_T& host,
         const sg::SG_Output_T* const p_sg_output,
         const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
         const Tracker_Info_Log_T& r_tracker_info,
         F360_Object_Log_Output_T& obj_log);

      void execute( // Main function of TSM to be called - overload. For only OCG as input
         const F360_Core_Info_T& core_info,
         const F360_Host_T& host,
         const ocg::OCG_Outputs_T& occupancy_grid,
         const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
         const Tracker_Info_Log_T& r_tracker_info,
         F360_Object_Log_Output_T& obj_log);

      void execute( // Main function of TSM to be called - overload. Neither OCG or SG as input
         const F360_Core_Info_T& core_info,
         const F360_Host_T& host,
         const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
         const Tracker_Info_Log_T& r_tracker_info,
         F360_Object_Log_Output_T& obj_log);

      void Log_Functional_Safety_Faults(Functional_Safety_Faults_Log_T& log) const;

   protected:
      virtual bool clear_should_reset_flag(void) const
      {
         return true;
      };
      virtual void run_tracker(
         const F360_Core_Info_T& core_info,
         const F360_Host_T& host,
         const ocg::OCG_Outputs_T* const p_occupancy_grid,
         const sg::SG_Output_T* const p_sg_output,
         const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
         const F360_Tracker& r_tracker,
         F360_Object_Log_Output_T& obj_log) const;

   private:
      bool is_critical_fault_detected(
         const F360_Core_Info_T& core_info,
         const F360_Host_T& host,
         const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
         const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
         const F360_Object_Log_Output_T& obj_log,
         const Tracker_Info_Log_T& r_tracker_info) const;

      SafetyControlLogic& safety_logic; // reference to SafetyLogic object
      F360_Tracker& tracker; // reference to F360_Tracker object
      bool should_reset;
   };
}
#endif
