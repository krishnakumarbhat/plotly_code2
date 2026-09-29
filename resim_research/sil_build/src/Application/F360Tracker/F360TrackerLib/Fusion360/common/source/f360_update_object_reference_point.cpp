/*===================================================================================*\
* FILE: f360_update_object_reference_point.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Update_Object_Reference_Point()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_update_object_reference_point.h"
#include "f360_update_object_reference_point_support_functions.h"
#include "f360_reference_point_candidate.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Update_Object_Reference_Point()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t host_dist_rear_axle_to_vcs_m               - Distance from rear axel to VCS origin [m]
   * const bool f_update_obj_states                             - Update object state flag
   * const bool f_update_obj_pos_only                           - Update object position only flag
   * const bool f_current_object_behind_another_object          - Only allow corner reference points
   * const F360_Calibrations_T& calibrations                    - Calibration structure
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - Sensors
   * const F360_Globals_T& globals                              - Globals structure
   * F360_Object_Track_T& obj                                   - Object structure
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function sets a new reference point for an object and makes necessary
   * updates to object Kalman filter related and size related properties.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Object_Reference_Point(
      const float32_t host_dist_rear_axle_to_vcs_m,
      const bool f_update_obj_states,
      const bool f_update_obj_pos_only,
      const bool f_current_object_behind_another_object,
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& obj)
   {
      if (obj.movable_prob > 0.5F)
      {
         Reference_Point_Data ref_pnt_ignoring_visibility;
         Reference_Point_Data_Ext ref_pnt_considering_visibility;

         // Initialize the expected new reference point to be equal to prev ref point
         ref_pnt_considering_visibility.new_point.reference_point = obj.reference_point;
         ref_pnt_ignoring_visibility.new_point.reference_point = obj.min_projection_reference_point;

         Derive_Object_Reference_Point(
            f_current_object_behind_another_object,
            calibrations.k_far_away_object_dist_sq_thr,
            host_dist_rear_axle_to_vcs_m,
            sensors,
            globals,
            obj,
            ref_pnt_ignoring_visibility,
            ref_pnt_considering_visibility);

         Apply_Reference_Point_Hysteresis(calibrations, obj, ref_pnt_ignoring_visibility, ref_pnt_considering_visibility);

         if (f_update_obj_states)
         {
            Reference_Point_Blind_Spot_Countermeasure(obj, ref_pnt_considering_visibility);
         }

         // This flag is only set to true when called from initialization,
         // allows moveable objects to always select a reference point other than default (center)
         if (f_update_obj_pos_only && globals.f_single_front_center_radar_only)
         {
            ref_pnt_considering_visibility.new_point.reference_point = ref_pnt_ignoring_visibility.new_point.reference_point;
         }

         // Update object according to the new reference point
         const F360_Reference_Point_T old_ref_pnt = obj.reference_point;
         constexpr float32_t small_object_area_thresh_for_refpoint_switching =0.3F;
         if (obj.bbox.Get_Area() < small_object_area_thresh_for_refpoint_switching)
         {
            obj.reference_point = F360_REFERENCE_POINT_CENTER;
            obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
         }
         else
         {
            obj.reference_point = ref_pnt_considering_visibility.new_point.reference_point;
            obj.min_projection_reference_point = ref_pnt_ignoring_visibility.new_point.reference_point;
         }

         if (ref_pnt_considering_visibility.new_point.reference_point != ref_pnt_considering_visibility.previous_point.reference_point)
         {
            // The reference point is changed
            if (f_update_obj_states)
            {
               // Update KF states to correspond to the new reference position.
               // This means position of bounding box remains the same.
               Update_Object_KF_States_After_Reference_Point_Change(obj);

               // Increase position uncertainty after reference point swicth
               Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(old_ref_pnt, calibrations, obj);
            }
            else
            {
               // Don't update KF states, only update object size related object variables.
               // This means that the position of bounding box is shifted and hence we need to compute the bbox center.
               obj.Update_Bbox_Center();
            }
         }
      }
      else
      {
         obj.reference_point = F360_REFERENCE_POINT_CENTER;
         obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
      }

   }
}
