/*===========================================================================*\
* FILE: inputs_preprocessing.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Inputs_Preprocessing function declaration.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef INPUTS_PREPROCESSING_H
#define INPUTS_PREPROCESSING_H

#include "f360_core_info.h"
#include "f360_host.h"
#include "f360_host_props.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
    void Inputs_Preprocessing(
       const F360_Core_Info_T &core_info,
       const F360_Host_T &host,
       F360_Host_Props_T &host_props,
       F360_TRKR_TIMING_INFO_T & timing_info);
}
#endif
