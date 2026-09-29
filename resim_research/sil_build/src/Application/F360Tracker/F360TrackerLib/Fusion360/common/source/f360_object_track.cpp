/*===================================================================================*\
* FILE: f360_object_track.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of F360_Object_Track_T methods.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#include "f360_object_track.h"
#include "f360_reference_point_support_functions.h"

namespace f360_variant_A
{
   /*=========================================================================
   * Method         F360_Object_Track_T::Update_Bbox_Size
   *
   * Description    Update bbox to be aligned with size related signals
   *
   * Parameters     
   * const float32_t& length
   * const float32_t& width
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void F360_Object_Track_T::Update_Bbox_Size(const float32_t& length, const float32_t& width)
   {
      this->bbox.Set_Length(length);
      this->bbox.Set_Width(width);
      this->Update_Bbox_Center();
   }
   /*=========================================================================
   * Method         F360_Object_Track_T::Set_Bbox_Orientation
   *
   * Description    Set new orientation of bbox.
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void F360_Object_Track_T::Set_Bbox_Orientation(const Angle& new_orientation)
   {
      this->bbox.Set_Orientation(new_orientation);
      this->Update_Bbox_Center();
   }

   /*=========================================================================
   * Method         F360_Object_Track_T::Update_Bbox_Center
   *
   * Description    Recalculate bbox center to be aligned with current state of track. 
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void F360_Object_Track_T::Update_Bbox_Center()
   {
      const Point ref_tcs = Get_Reference_Point_Pos_In_TCS(this->reference_point, this->bbox.Get_Length(), this->bbox.Get_Width());
      Point new_center{ -ref_tcs.x , -ref_tcs.y };
      new_center.Rotate_About_Origin(this->bbox.Get_Orientation());
      new_center.Translate(this->vcs_position.x, this->vcs_position.y);
      this->bbox.Set_Center(new_center);
   }
}
