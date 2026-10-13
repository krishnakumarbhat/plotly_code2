/*===================================================================================*\
* FILE: sg_core_with_dc.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Stationary Geometries core class declaration extended by DC module.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CORE_WITH_DC_H
#define SG_CORE_WITH_DC_H

#include "dc_contour_storage.h"
#include "dc_interface_handcode.h"
#include "sg_core.h"

namespace sg
{
#ifdef _MSC_VER
   #pragma warning(push)
   #pragma warning(disable:4820)
#endif
class SGCoreWithDC : public SGCore
{
 public:
   SGCoreWithDC(TimingInfo &timing_info);
   SGCoreWithDC()                = delete;
   SGCoreWithDC(SGCoreWithDC &)  = delete;
   SGCoreWithDC(SGCoreWithDC &&) = delete;
   virtual ~SGCoreWithDC()       = default;

   SGCoreWithDC &operator=(SGCoreWithDC &) = delete;
   SGCoreWithDC &operator=(SGCoreWithDC &&) = delete;

   void initialize(const SG_Internals_Dump_T &sg_internals) override;

 protected:
   dc::DCContourStorage m_dc_contours{};     // container for DC contours.
   dc::CriticalRegion m_critical_region{};   // critical region
   dc::DCInterfaceHandcode m_dc_interface{}; // interface of DC

   void sg_dc_fusion_step() override;
   void drivability_classification_step(const float elapsed_time, const rot::F360_Detection_Log_Output_T &rot_detections,
                                        const rspp::RSPP_Detection_List_T &rspp_detections, const RSPP_Host_T &host) override;

   void get_internals(SG_Internals_Dump_T &sg_internals) const override;

   void dump_internals(DC_Dump_T &dc_dump) const; // Dumps DC data structures.

#ifdef SG_DC_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
   void dump_assigned_detections(DC_Dump_T &dc_dump, const dc::DC_Contour_T::SubsegmentList::iterator subsegment,
                                 uint32_t output_index) const; // Dumps detections assigned to subsegment
#endif
};
#ifdef _MSC_VER
   #pragma warning(pop)
#endif
} // namespace sg

#endif
