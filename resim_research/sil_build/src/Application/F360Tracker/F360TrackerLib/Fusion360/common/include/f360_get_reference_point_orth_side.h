#ifndef F360_GET_REFERENCE_POINT_ORTH_SIDE_H
#define F360_GET_REFERENCE_POINT_ORTH_SIDE_H
/*===================================================================================*\
* FILE: f360_get_reference_point_orth_side.h
*====================================================================================
* Copyright - 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* full_filespec: 
* version: 
* derived_by: 
* date_created: 
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Get_Reference_Point_Orth_Side().
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#include "f360_object_sides.h"
#include "f360_object_track.h"
namespace f360_variant_A
{
   F360_Object_Sides_T Get_Reference_Point_Orth_Side(const F360_Reference_Point_T ref_pnt);
}
#endif

