#ifndef F360_SPLIT_TYPE
#define F360_SPLIT_TYPE
/*===================================================================================*\
* FILE: f360_split_type.h
*====================================================================================
*Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains enum definitions for object reference point.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

namespace f360_variant_A
{
   typedef enum SPLIT_TYPE
   {
      NONE = 0,
      ORTH_GAP_BASED = 1,
      RR_ERROR_BASED = 2,
   }SPLIT_TYPE;
}
#endif
