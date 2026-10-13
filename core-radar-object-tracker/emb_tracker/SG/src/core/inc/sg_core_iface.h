/*===================================================================================*\
* FILE: sg_core_iface.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Stationary Geometries Core interface class declaration.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CORE_IFACE_H
#define SG_CORE_IFACE_H

#include "fused_contour_storage.h"
#include "sg_calibration_manager.h"
#include "sg_contour_downselection.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"
#include "sg_host_props.h"
#include "sg_input.h"
#include "sg_internals_dump.h"
#include "sg_output.h"
#include "sg_reduced_output.h"
#include "sg_timer_base.h"
#include "sg_timing_info.h"

namespace sg
{
#ifdef _MSC_VER
   #pragma warning(push)
   #pragma warning(disable:4820)
#endif
class SGCoreIface
{
 public:
   virtual void initialize(const SG_Internals_Dump_T &sg_internals) = 0; // Initializes algorithm from dumped data.

   virtual void step(const SG_Input_T &input) = 0; // Executes one iteration of the algorithm

   virtual void get_output(SG_Output_T &sg_output) const                = 0; // Store algorithm output data.
   virtual void get_reduced_output(SG_ReducedOutput_T &sg_output) const = 0; // Store algorithm reduced output data.
   virtual void get_internals(SG_Internals_Dump_T &sg_internals) const  = 0; // Dump algorithm internal data.
   virtual void reset()                                                 = 0; // reset algorithm state
};
#ifdef _MSC_VER
   #pragma warning(pop)
#endif
} // namespace sg

#endif
