/*===================================================================================*\
* FILE: f360_host_props_update.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains declarations for host properties updates
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*==========================================================================================*/
#ifndef HOST_PROPS_UPDATE_H
#define HOST_PROPS_UPDATE_H

#include "f360_reuse.h"
#include "f360_host.h"
#include "f360_host_props.h"

namespace f360_variant_A
{
   void Host_Props_Update(
      const float32_t elapsed_time,
      const F360_Host_T& host,
      F360_Host_Props_T& host_props
   );
}
#endif
