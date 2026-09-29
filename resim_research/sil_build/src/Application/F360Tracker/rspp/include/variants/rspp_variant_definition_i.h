#ifndef RSPP_VARIANT_DEFINITION_I_H
#define RSPP_VARIANT_DEFINITION_I_H
/*===========================================================================*\
* FILE: rspp_variant_definition_i.h
*============================================================================
* Copyright (C) 2026 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "rspp_reuse.h"
#include "rspp_variant_type.h"

namespace rspp_variant_I
{
   static constexpr RSPP_Variant_Type_T VARIANT_TYPE = RSPP_VARIANT_TYPE_I;

   static constexpr uint8_t MAX_NUMBER_OF_SRR_SENSORS = 4U;
   static constexpr uint8_t MAX_NUMBER_OF_MRR_SENSORS = 0U;
   static constexpr uint16_t NUMBER_OF_SRR_DETECTIONS = 200U;
   static constexpr uint16_t NUMBER_OF_MRR_DETECTIONS = 0U;
}

#endif
