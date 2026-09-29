/*===========================================================================*\
* FILE: f360_side_of_host.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains enum definition of F360_Side_Of_Host()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_SIDE_OF_HOST_H
#define F360_SIDE_OF_HOST_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef enum F360_Side_Of_Host : int8_t
   {
      F360_HOST_LEFT_SIDE = -1,
      F360_HOST_RIGHT_SIDE = 1,
      F360_HOST_INVALID_SIDE = 0
   }F360_Side_Of_Host_T;
}


#endif
