/*===================================================================================*\
* FILE: f360_merge_bbox_overlap_test.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declarations of Merge_Bbox_Overlap_Test function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_MERGE_BBOX_OVERLAP_TEST_H
#define F360_MERGE_BBOX_OVERLAP_TEST_H

#include "f360_object_track.h"

namespace f360_variant_A
{
   bool Merge_Bbox_Overlap_Test(
      const F360_Object_Track_T & first_object,
      const F360_Object_Track_T & second_object);
}
#endif
