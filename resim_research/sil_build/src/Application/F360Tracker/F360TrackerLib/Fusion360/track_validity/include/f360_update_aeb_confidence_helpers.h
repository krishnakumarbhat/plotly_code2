/*===================================================================================*\
* FILE: f360_update_aeb_confidence_helpers.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains helper declaration(s) for Update_AEB_Confidence().
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_UPDATE_AEB_CONFIDENCE_HELPERS_H
#define F360_UPDATE_AEB_CONFIDENCE_HELPERS_H

#include "f360_host.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   float32_t Compute_Iso_Relative_X_Vel(const F360_Object_Track_T& object, const F360_Host_T& host);
}

#endif
