/*===================================================================================*\
* FILE: sg_output.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG output datatypes.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_OUTPUT_H
#define SG_OUTPUT_H

#include "sg_constants.h"
#include "sg_contour_out.h"
#include "sg_log_stream_info.h"
#include "sg_sw_version.h"
#include "sg_vertex_out.h"

namespace sg
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

   static const SG_LogStreamInfo_T output_stream{static_cast<uint16_t>(180U), static_cast<uint16_t>(5U)};

   struct SG_Output_T final
   {
      SG_Output_T()                               = default;
      ~SG_Output_T()                              = default;
      SG_Output_T(const SG_Output_T &)            = default;
      SG_Output_T &operator=(const SG_Output_T &) = default;
      SG_Output_T &operator=(SG_Output_T &&)      = default;

      uint64_t execution_timestamp_us{0U};                     // [us] timestamp
      uint64_t measurement_timestamp_us{0U};                   // [us] timestamp
      SG_Vertex_Out_T vertices[SG_MAX_NUM_OUTPUT_VERTICES]{};  // [-] array of vertices
      SG_Contour_Out_T contours[SG_MAX_NUM_OUTPUT_CONTOURS]{}; // [-] array of contours
      SwVersion software_version{};                            // [-] SG software version
      uint32_t cycle_index{0U};                                // [-] internal cycle index
      uint16_t num_contours{0U};                               // [-] number of contours
      bool f_valid{false}; // [-] Flag telling that SG output is usable in current tracking cycle
      uint8_t padding[5]{};
   };

#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#endif
