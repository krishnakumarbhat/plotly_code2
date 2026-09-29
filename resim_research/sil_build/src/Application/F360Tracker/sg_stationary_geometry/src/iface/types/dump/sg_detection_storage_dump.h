/*===================================================================================*\
* FILE: sg_detection_storage_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes i.e. detections list dump type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_DETECTION_STORAGE_DUMP_H
#define SG_DETECTION_STORAGE_DUMP_H

#include "sg_constants.h"
#include "sg_detection_dump.h"

namespace sg
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif
   struct SG_Detection_Storage_Dump_T
   {
      SG_Detection_Dump_T detections[SG_MAX_NUM_INTERNAL_DETS];
   };
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#endif
