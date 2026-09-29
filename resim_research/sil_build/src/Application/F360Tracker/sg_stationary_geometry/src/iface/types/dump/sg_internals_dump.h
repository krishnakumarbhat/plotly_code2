/*===================================================================================*\
* FILE: sg_internals_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes for use with SG component resim.
*   They are: Intenals - so called debug data.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_INTERNALS_DUMP_H
#define SG_INTERNALS_DUMP_H

#include <cstdint>

#include "dc_dump.h"
#include "sg_contour_storage_dump.h"
#include "sg_detection_storage_dump.h"
#include "sg_log_stream_info.h"
#include "sg_sw_version.h"

namespace sg
{
   static const SG_LogStreamInfo_T internals_dump_stream{182U, 5U};

   struct SG_Internals_Dump_T
   {
      uint64_t execution_timestamp_us;
      SG_Contour_Storage_Dump_T contour_storage;
      SG_Detection_Storage_Dump_T detection_storage;
      DC_Dump_T dc_dump;
   };
}

#endif
