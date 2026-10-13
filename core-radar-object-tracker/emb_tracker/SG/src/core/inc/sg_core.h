/*===================================================================================*\
* FILE: sg_core.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Stationary Geometries core declaration class declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CORE_H
#define SG_CORE_H

#include "fused_contour_storage.h"
#include "sg_calibration_initializer_scania.h"
#include "sg_calibration_manager.h"
#include "sg_contour_downselection.h"
#include "sg_contour_storage.h"
#include "sg_core_iface.h"
#include "sg_detection_storage.h"
#include "sg_host_props.h"
#include "sg_input.h"
#include "sg_internals_dump.h"
#include "sg_output.h"
#include "sg_reduced_output.h"
#include "sg_timer_base.h"
#include "sg_timing_info.h"
#include "sg_variant_selector.h"

namespace sg
{
#ifdef _MSC_VER
   #pragma warning(push)
   #pragma warning(disable:4820)
#endif
class SGCore : public SGCoreIface
{
 public:
   SGCore(TimingInfo &timing_info);
   SGCore()          = delete;
   SGCore(SGCore &)  = delete;
   SGCore(SGCore &&) = delete;
   virtual ~SGCore() = default;

   SGCore &operator=(SGCore &) = delete;
   SGCore &operator=(SGCore &&) = delete;

   void initialize(const SG_Internals_Dump_T &sg_internals); // Initializes algorithm from dumped data.

   void step(const SG_Input_T &input); // Executes one iteration of the algorithm

   void get_output(SG_Output_T &sg_output) const;                // Store algorithm output data.
   void get_reduced_output(SG_ReducedOutput_T &sg_output) const; // Store algorithm reduced output data.
   void get_internals(SG_Internals_Dump_T &sg_internals) const;  // Dump algorithm internal data.
   void reset();                                                 // reset algorithm state

 protected:
   DetectionStorage m_detection_storage{}; // container for SG detections (current + historic ones with additional signals)
   ContourStorage m_contours{};            // container for SG contours.
   FusedContourStorage m_fused_contours{}; // container for fused contours.
   ContourDownselection m_contour_selector{m_fused_contours}; // downselection module
   // Keeps and manages current sg::Calibrations structure which initialization depends on variant chosen
#if defined(SG_VARIANT_SCANIA_TRATON)
   CalibrationManager m_dynamic_calibrations{CalibrationInitializerScania {}};
#else
   CalibrationManager m_dynamic_calibrations{CalibrationInitializerPlatform{}};
#endif
   HostProps m_host_properties{};           // host properties data tracked in time
   TimingInfo &m_timing;                    // runtime measurement objects
   std::uint64_t m_timestamp_us{};          // [us] current cycle timestamp
   std::uint64_t m_measurement_timestamp{}; // [us] measurement timestamp
   std::uint32_t m_cycle_index{};           // internal counter value
   bool m_f_state_cleared{true};

   void update_calibrations_step(const float host_speed);
   void time_update_step(const float elapsed_time, const RSPP_Host_T &host);
   void detection_processing_step(const rspp::RSPP_Detection_List_T &rspp_detections,
                                  const rot::F360_Detection_Log_Output_T &rot_detections,
                                  const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                  const RSPP_Host_T &host);
   void detection_clustering_step();
   void measurement_association_step();
   void measurement_update_step();
   void contour_initialization_step(const float curvature_rear);
   void contour_postprocessing_step(const float curvature_rear);
   virtual void drivability_classification_step(const float elapsed_time, const rot::F360_Detection_Log_Output_T &rot_detections,
                                                const rspp::RSPP_Detection_List_T &rspp_detections, const RSPP_Host_T &host);
   virtual void sg_dc_fusion_step();
   void contour_downselection_step(const RSPP_Host_T &host);

   void dump_output(SG_Output_T &output) const;                                    // Dumps output data structure.
   void dump_reduced_output(SG_ReducedOutput_T &output) const;                     // Dumps reduced output data structure.
   void dump_internals(SG_Contour_Storage_Dump_T &contour_storage_dump) const;     // Dumps contours information.
   void dump_internals(SG_Detection_Storage_Dump_T &detection_storage_dump) const; // Dumps detections information.

   float calculate_elapsed_time(const uint64_t timestamp_us); // Calculates elapsed time between two scan indices and
                                                              // update m_timestamp
   void calculate_measurement_timestamp(const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                                        const uint64_t timestamp_step);

   static void clear_internals(SG_Internals_Dump_T &internals); // Clears internal data.
};
#ifdef _MSC_VER
   #pragma warning(pop)
#endif
} // namespace sg

#endif
