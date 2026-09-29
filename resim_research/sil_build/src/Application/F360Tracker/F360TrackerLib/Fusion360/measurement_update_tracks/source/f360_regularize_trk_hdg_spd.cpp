/*===================================================================================*\
* FILE: f360_regularize_trk_hdg_spd.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains function definition of Regularize_Trk_Hdg_Spd()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_regularize_trk_hdg_spd.h"
#include "f360_norm_heading_angle.h"
#include "f360_trk_fltr_ctca_states.h"

namespace f360_variant_A
{
   static F360_Reference_Point_T Flip_Reference_Point(const F360_Reference_Point_T old_ref_point);

   /*===========================================================================*\
   * FUNCTION: Regularize_Trk_Hdg_Spd
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib
   * F360_Object_Track_T & object_track
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
   * In the case when an objects is reversing (velocity magnitude below zero),  
   * this function will adjust the object state and correspoinding covariances. 
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/


   void Regularize_Trk_Hdg_Spd(
         const F360_Calibrations_T & calib,
         F360_Object_Track_T & object_track
         )
   {

      if (object_track.speed < -calib.k_ctca_msmnt_update_max_reverse_abs_spd)
      {
         object_track.speed = -object_track.speed;
         object_track.predicted_speed = -object_track.predicted_speed;
         object_track.vcs_heading = (object_track.vcs_heading + F360_PI).Normalize();
         object_track.curvature = -object_track.curvature;
         object_track.tang_accel = -object_track.tang_accel;

         // Flip the sign for F360_TRK_FLTR_CTCA_STATE_C, F360_TRK_FLTR_CTCA_STATE_S and F360_TRK_FLTR_CTCA_STATE_A
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y];
         object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H] = -object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H];


         
         const F360_Reference_Point_T flipped_ref_point = Flip_Reference_Point(object_track.reference_point);
         const F360_Reference_Point_T flipped_geometrical_ref_point = Flip_Reference_Point(object_track.min_projection_reference_point);

         object_track.reference_point = flipped_ref_point;
         object_track.min_projection_reference_point = flipped_geometrical_ref_point;

         object_track.Update_Bbox_Center();
      }

   }


   /*===========================================================================*\
   * FUNCTION: Flip_Reference_Point()
   *===========================================================================
   * RETURN VALUE:
   * F360_Reference_Point_T new_ref_point
   *
   * PARAMETERS:
   * const F360_Reference_Point_T ref_point
   *
   * EXTERNAL REFERENCES:
   * None
   *
   * DEVIATIONS FROM STANDARDS:
   * None
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *--------------------------------------------------------------------------
   * Function returns a new reference point, when object is reversing
   * 
   * PRECONDITIONS :
   * None
   *
   * POSTCONDITIONS :
   * None
   *
   \*===========================================================================*/

   static F360_Reference_Point_T Flip_Reference_Point(
       const F360_Reference_Point_T old_ref_point)
   {
       F360_Reference_Point_T new_ref_point;
       switch (old_ref_point)
       {
           case F360_REFERENCE_POINT_FRONT_LEFT:
           {
               new_ref_point = F360_REFERENCE_POINT_REAR_RIGHT;
               break;
           }
           case F360_REFERENCE_POINT_FRONT:
           {
               new_ref_point = F360_REFERENCE_POINT_REAR;
               break;
           }
           case F360_REFERENCE_POINT_FRONT_RIGHT:
           {
               new_ref_point = F360_REFERENCE_POINT_REAR_LEFT;
               break;
           }
           case F360_REFERENCE_POINT_RIGHT:
           {
               new_ref_point = F360_REFERENCE_POINT_LEFT;
               break;
           }
           case F360_REFERENCE_POINT_REAR_RIGHT:
           {
               new_ref_point = F360_REFERENCE_POINT_FRONT_LEFT;
               break;
           }
           case F360_REFERENCE_POINT_REAR:
           {
               new_ref_point = F360_REFERENCE_POINT_FRONT;
               break;
           }
           case F360_REFERENCE_POINT_REAR_LEFT:
           {
               new_ref_point = F360_REFERENCE_POINT_FRONT_RIGHT;
               break;
           }
           case F360_REFERENCE_POINT_LEFT:
           {
               new_ref_point = F360_REFERENCE_POINT_RIGHT;
               break;
           }
           default:
           {
               new_ref_point = F360_REFERENCE_POINT_CENTER;
               break;
           }
       }
       return new_ref_point;
   }
}
