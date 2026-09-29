/*===========================================================================*\
* FILE: f360_get_edge_tcs_start_and_end_point.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains function definition of Get_Edge_TCS_Start_And_End_Point()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "f360_get_edge_tcs_start_and_end_point.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Get_Edge_TCS_Start_And_End_Point()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_valid_edge
   *
   * PARAMETERS:
   * const F360_Object_Track_T & obj   - Object structure
   * const F360_Object_Sides_T edge    - Edge to get points from
   * Point & start_point_tcs           - Start point
   * Point & end_point_tcs             - End point
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
   * Function returns the edge start and end point of the specified edge in TCS
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   bool Get_Edge_TCS_Start_And_End_Point(
      const F360_Object_Track_T & obj,
      const F360_Object_Sides_T edge,
      Point & start_point_tcs,
      Point & end_point_tcs)
   {
      // Set edge end point values depending on passed edge.
      bool f_valid_edge;
      switch (edge)
      {
      case F360_OBJECT_SIDES_FRONT:       
         start_point_tcs.x = obj.bbox.Get_Length() * 0.5F;     // Maximim value in para direction
         start_point_tcs.y = -obj.bbox.Get_Width() * 0.5F;     // Minimum value in orth direction
         end_point_tcs.x = obj.bbox.Get_Length() * 0.5F;
         end_point_tcs.y = obj.bbox.Get_Width() * 0.5F;        // Maximum value in orth direction
         f_valid_edge = true;
         break;
      case F360_OBJECT_SIDES_REAR:       
         start_point_tcs.x = -obj.bbox.Get_Length() * 0.5F;    // Minimum value in para direction
         start_point_tcs.y = -obj.bbox.Get_Width() * 0.5F;
         end_point_tcs.x = -obj.bbox.Get_Length() * 0.5F;
         end_point_tcs.y = obj.bbox.Get_Width() * 0.5F;
         f_valid_edge = true;
         break;
      case F360_OBJECT_SIDES_LEFT:
         start_point_tcs.x = -obj.bbox.Get_Length() * 0.5F;
         start_point_tcs.y = -obj.bbox.Get_Width() * 0.5F;
         end_point_tcs.x = obj.bbox.Get_Length() * 0.5F;
         end_point_tcs.y = -obj.bbox.Get_Width() * 0.5F;
         f_valid_edge = true;
         break;
      case F360_OBJECT_SIDES_RIGHT:
         start_point_tcs.x = -obj.bbox.Get_Length() * 0.5F;
         start_point_tcs.y = obj.bbox.Get_Width() * 0.5F;
         end_point_tcs.x = obj.bbox.Get_Length() * 0.5F;
         end_point_tcs.y = obj.bbox.Get_Width() * 0.5F;
         f_valid_edge = true;
         break;
      default:
         // Only happens if INVALID edge is passed to function, set valid edge flag to invalid
         f_valid_edge = false;
         start_point_tcs.y = INFTY;
         start_point_tcs.x = INFTY;
         end_point_tcs.y = INFTY;
         end_point_tcs.x = INFTY;
         break;
      }
      return f_valid_edge;
   }
}
