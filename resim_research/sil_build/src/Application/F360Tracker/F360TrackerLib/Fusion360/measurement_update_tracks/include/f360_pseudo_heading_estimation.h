/*===========================================================================*\
* FILE: f360_pseudo_heading_estimation.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Pseudo_Heading_Estimation().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_PSEUDO_HEADING_ESTIMATION_H
#define F360_PSEUDO_HEADING_ESTIMATION_H

#include "f360_object_track.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Pseudo_Heading_Estimation(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj);
}
#endif
