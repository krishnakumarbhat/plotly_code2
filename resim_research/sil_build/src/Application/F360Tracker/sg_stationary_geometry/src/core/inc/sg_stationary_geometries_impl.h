/*===================================================================================*\
* FILE: sg_stationary_geometries_impl.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Stationary Geometries implementation clas declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef STATIONARY_GEOMETRIES_IMPL_H
#define STATIONARY_GEOMETRIES_IMPL_H

#include "sg_core.h"
#include "sg_output.h"
#include "sg_reduced_output.h"
#include "sg_safety.h"
#include "sg_timing_dump.h"
#include "sg_timing_info.h"

namespace sg
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif
   class Stationary_Geometries_Impl
   {
     public:
      Stationary_Geometries_Impl(TimerBase *const external_clock = nullptr);
      Stationary_Geometries_Impl(Stationary_Geometries_Impl &)  = delete;
      Stationary_Geometries_Impl(Stationary_Geometries_Impl &&) = delete;
      virtual ~Stationary_Geometries_Impl()                     = default;

      Stationary_Geometries_Impl &operator=(Stationary_Geometries_Impl &)  = delete;
      Stationary_Geometries_Impl &operator=(Stationary_Geometries_Impl &&) = delete;

      void initialize(const SG_Internals_Dump_T &sg_internals); // Initializes algorithm from dumped data.

      /**
       * @brief    Executes one iteration of the algorithm.
       *
       * @param[in]    timestamp_us [us]
       * @param[in]    rspp_detections
       * @param[in]    sensors
       * @param[in]    host
       *
       **/
      void step(const uint64_t timestamp_us,
                const rot::F360_Detection_Log_Output_T &rot_detections,
                const rspp::RSPP_Detection_List_T &rspp_detections,
                const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                const RSPP_Host_T &host);

      void get_output(SG_Output_T &sg_output) const;                // Store algorithm output data.
      void get_reduced_output(SG_ReducedOutput_T &sg_output) const; // Store algorithm reduced output data.
      void get_internals(SG_Internals_Dump_T &sg_internals) const;  // Dump algorithm internal data.
      void get_timing(SG_Timing_Dump_T &sg_timing) const;           // Dump runtime measurement data.

     private:
      TimingInfo m_timing;
      SgCore m_core{m_timing};
      Safety m_safety_analyzer{};
      SG_Output_T m_output{};
      SG_ReducedOutput_T m_reduced_output{};

      void dump_timing(SG_Timing_Dump_T &timing_dump) const;
      void reset();
   };
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#endif
