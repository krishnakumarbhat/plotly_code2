/*===========================================================================*\
* FILE: f360_data_generator.h
*============================================================================
* Copyright ? 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential ? Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains detections data generation for unit testing
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [30-Mar-2018]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/
#ifndef F360_DATA_GENERATORS_H
#define F360_DATA_GENERATORS_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   float32_t Get_Azimuth_From_Cart_Pos(
      const float32_t posn_lat,
      const float32_t posn_long
   );
}
#endif
