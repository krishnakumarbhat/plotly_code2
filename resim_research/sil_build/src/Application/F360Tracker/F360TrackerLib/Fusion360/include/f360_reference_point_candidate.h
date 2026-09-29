#ifndef F360_REFERENCE_POINT_CANDIDATE_H
#define F360_REFERENCE_POINT_CANDIDATE_H
/*===================================================================================*\
* FILE: f360_reference_point_candidate.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains struct definitions for object reference point selection.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_reuse.h"
#include "f360_reference_point.h"

namespace f360_variant_A
{
   struct Reference_Point_Candidate
   {
      float32_t projection_value;
      F360_Reference_Point_T reference_point;
      bool f_visible;
      uint8_t padding[2];
   };

   struct Reference_Point_Data
   {
      Reference_Point_Candidate previous_point;
      Reference_Point_Candidate new_point;
   };

   struct Reference_Point_Data_Ext : Reference_Point_Data
   {
      bool f_all_new_ref_pnt_candidates_visible;
      uint8_t padding[3];
   };
}
#endif
