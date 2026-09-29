/*===================================================================================*\
* FILE: f360_aeb_confidence.h
*====================================================================================
* Copyright (C) 2026 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_AEB_Confidence_T enum declaration
*
* ABBREVIATIONS:
*  None
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_AEB_CONFIDENCE_H
#define F360_AEB_CONFIDENCE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   enum F360_AEB_Confidence_T : uint8_t
   {
      AEB_CONF_INVALID = 0,
      AEB_CONF_LOW = 1,
      AEB_CONF_MEDIUM = 2,
      AEB_CONF_MEDIUM_HIGH = 3,
      AEB_CONF_HIGH = 4
   };
}
#endif
