#ifndef F360_CONF_H
#define F360_CONF_H
/*===================================================================================*\
* FILE: f360_conf.h
*====================================================================================
*Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains enum definitions for confidence.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

namespace f360_variant_A
{
   typedef enum CONF9_Tag
   {
      CONF9_NONE = 0,
      CONF9_LOW1 = 1,
      CONF9_LOW2 = 2,
      CONF9_LOW3 = 3,
      CONF9_LOW4 = 4,
      CONF9_MED1 = 5,
      CONF9_MED2 = 6,
      CONF9_MED3 = 7,
      CONF9_MED4 = 8,
      CONF9_HIGH = 9
   } CONF9_T;

   typedef enum CONF3_Tag
   {
      CONF3_NONE = 0,
      CONF3_LOW = 1,
      CONF3_MED = 2,
      CONF3_HIGH = 3
   } CONF3_T;
}
#endif
