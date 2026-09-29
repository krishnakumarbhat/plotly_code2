/*===================================================================================*\
* FILE: f360_get_reference_point_para_side.cpp
*====================================================================================
* Copyright © 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
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
*   Contains function definition of Get_Reference_Point_Para_Side()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
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

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_get_reference_point_para_side.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Get_Reference_Point_Para_Side()
   *===========================================================================
   * RETURN VALUE:
   * F360_Object_Sides_T para_side
   *
   * PARAMETERS:
   * const F360_Reference_Point_T ref_pnt
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function returns the object FRONT or REAR side if the reference point indicates 
   * that this side is visible from host perspective. If none of these sides are 
   * visible then function returns INVALID.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   F360_Object_Sides_T Get_Reference_Point_Para_Side(const F360_Reference_Point_T ref_pnt)
   {
      F360_Object_Sides_T para_side;

      if ((F360_REFERENCE_POINT_FRONT_LEFT == ref_pnt) ||
         (F360_REFERENCE_POINT_FRONT == ref_pnt) ||
         (F360_REFERENCE_POINT_FRONT_RIGHT == ref_pnt))
      {
         para_side = F360_OBJECT_SIDES_FRONT;
      }
      else if ((F360_REFERENCE_POINT_REAR_LEFT == ref_pnt) ||
         (F360_REFERENCE_POINT_REAR == ref_pnt) ||
         (F360_REFERENCE_POINT_REAR_RIGHT == ref_pnt))
      {
         para_side = F360_OBJECT_SIDES_REAR;
      }
      else
      {
         para_side = F360_OBJECT_SIDES_INVALID;
      }
      return para_side;
   }
}
