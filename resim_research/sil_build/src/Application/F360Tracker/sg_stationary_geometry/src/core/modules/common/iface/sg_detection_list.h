/*===================================================================================*\
* FILE: sg_detection_list.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Detection List - the container for Detection_T objects.
*   Detection_T objects are stored in DetectionList which has type of EmbeddedList.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_DETECTION_LIST_H
#define SG_DETECTION_LIST_H

#include "embedded_list.h"
#include "sg_constants.h"
#include "sg_detection.h"

namespace sg
{
   using DetectionList = EmbeddedList<Detection_T, SG_MAX_NUM_INTERNAL_DETS>;
}

#endif