/*===========================================================================*\
* FILE: f360_cond_deassoc_low_rr_dets.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential _ Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Cond_Deassoc_Low_RR_Dets()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_COND_DEASSOC_LOW_RR_DETS_H
#define F360_COND_DEASSOC_LOW_RR_DETS_H

#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   void Cond_Deassoc_Low_RR_Dets(
      const F360_Calibrations_T& calibs,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj);
}

#endif
