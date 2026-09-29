/*===================================================================================*\
* FILE: dc_algorithm_step.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains enum for Drivability Classification (DC) module steps.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_ALGORITHM_STEP_H
#define DC_ALGORITHM_STEP_H

namespace sg
{
   enum class DC_AlgorithmStep_T : uint8_t
   {
      // DC steps
      TIME_UPDATE_SUBSEGMENTS = 0,
      CREATE_CRITICAL_REGION,
      UPDATE_SUBSEGMENTS,
      ASSIGN_DETECTIONS_AND_UPDATE_FEATURES,
      UPDATE_SUBSEGMENTS_DRIVABILITY,

      // Other
      NUM_OF_ALGO_STEPS,
   };
}

#endif
