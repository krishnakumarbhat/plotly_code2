/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_determine_cv_driving_scenario.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Determine_CV_Driving_Scenario()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/
#ifndef F360_DETERMINE_CV_DRIVING_SCENARIO_H
#define F360_DETERMINE_CV_DRIVING_SCENARIO_H

#include "f360_host.h"

namespace f360_variant_A
{
    bool Determine_CV_Driving_Scenario(const F360_Host_T& host);
    void Reset_Determine_CV_Driving_Scenario_Variables();
}

#endif
