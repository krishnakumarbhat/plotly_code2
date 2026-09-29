/*===================================================================================*\
* FILE: sg_sw_version.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file defines structure for storing SG software version.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_SW_VERSION_H
#define SG_SW_VERSION_H

#include <cstdint>

namespace sg
{
   struct SwVersion
   {
      std::uint16_t major{};
      std::uint16_t minor{};
      std::uint16_t patch{};
      char name[20]{};
      uint8_t padding[2]{};
   };
}

#endif
