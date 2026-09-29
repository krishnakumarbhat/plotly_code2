/*===================================================================================*\
* FILE: sg_stationary_geometries.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Stationary Geometries class declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef STATIONARY_GEOMETRIES_H
#define STATIONARY_GEOMETRIES_H

#include "sg_input.h"
#include "sg_internals_dump.h"
#include "sg_output.h"
#include "sg_reduced_output.h"
#include "sg_sw_version.h"
#include "sg_timer_base.h"
#include "sg_timing_dump.h"

namespace sg
{
   class Stationary_Geometries
   {
     public:
      Stationary_Geometries(TimerBase *m_clock = nullptr); // Constructor providing external timer.

      void initialize(const SG_Internals_Dump_T &sg_internal); // Initializes the algorithm.

      /**
       * @brief         Executes one iteration of the algorithm.
       *
       * @param[in]     timestamp_us [us]
       * @param[in]     detections_list
       * @param[in]     sensors
       * @param[in]     host
       *
       **/
      void step(const uint64_t timestamp_us,
                const rot::F360_Detection_Log_Output_T &rot_detections,
                const rspp::RSPP_Detection_List_T &detections_list,
                const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                const RSPP_Host_T &host);

      void get_output(SG_Output_T &sg_output) const;                // Store algorithm output data.
      void get_reduced_output(SG_ReducedOutput_T &sg_output) const; // Store reduced algorithm output data.
      void get_internals(SG_Internals_Dump_T &sg_internal);         // Dump algorithm internal data.
      void get_timing(SG_Timing_Dump_T &sg_timing);                 // Dump runtime measurement data.

     private:
      class Stationary_Geometries_Impl *m_implementation;
      // UPDATE m_version ONLY ON MASTER BRANCH with correct values (major, minor, patch, name)
      const SwVersion m_version{1, 5, 0, {"platform"}}; // on DEV branch we indicate NO VERSION by putting {0, 0, 0, "dev"}
   };
}

#endif
