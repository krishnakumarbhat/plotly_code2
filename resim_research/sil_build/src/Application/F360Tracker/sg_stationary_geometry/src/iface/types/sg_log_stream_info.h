/*===================================================================================*\
* FILE: sg_log_stream_info.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition of log stream info structure.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_LOG_STREAM_INFO_H
#define SG_LOG_STREAM_INFO_H

namespace sg
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

   struct SG_LogStreamInfo_T
   {
      uint16_t number{};
      uint16_t version{};

      constexpr SG_LogStreamInfo_T(const uint16_t num, const uint16_t ver) : number(num), version(ver)
      {
      }
   };

#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#endif
