/*===================================================================================*\
* FILE: sg_algorithm_step.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains enum for runtime measurement purposes.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_ALGORITHM_STEP_H
#define SG_ALGORITHM_STEP_H

namespace sg
{
   enum class SG_AlgorithmStep_T : uint8_t
   {
      // main steps
      UPDATE_CALIBRATIONS = 0,
      TIME_UPDATE,
      DETECTION_PROCESSING,
      DETECTION_CLUSTERING,
      MEASUREMENT_ASSOCIATION,
      MEASUREMENT_UPDATE,
      CONTOURS_INITIALIZATION,
      CONTOURS_POSTPROCESSING,
      CONTOURS_DOWNSELECTION,
      DRIVABILITY_CLASSIFICATION,
      SG_DC_FUSION,
      NUM_OF_ALGO_STEPS
   };
}

#endif
