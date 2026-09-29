/*===================================================================================*\
FILE: f360_dimensions.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential  Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains declaration of the debug structure for dimensions
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_DIMENSIONS_H
#define F360_DIMENSIONS_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   struct F360_Dimensions_T
   {
      float32_t length;
      float32_t width;
      float32_t len1;
      float32_t len2;
      float32_t wid1;
      float32_t wid2;
   };

   static_assert(24 == sizeof(F360_Dimensions_T), "sizeof(F360_Dimensions_T) not as expected. Remember to align padding if needed");


}
#endif
