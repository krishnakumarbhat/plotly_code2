/*===================================================================================*\
* FILE: f360_update_merged_objects_properties.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Update_Merged_Objects_Properties() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#include "f360_update_merged_objects_properties.h"
#include "f360_update_object_reference_point.h"
#include "f360_math_func.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_static_env_polys_support_functions.h"
#include "f360_norm_heading_angle.h"
#include "f360_get_reference_point_para_side.h"

namespace f360_variant_A
{   
  static void Inherit_Flags_After_Merge(
      F360_Object_Track_T & object_track_to_keep,
      const F360_Object_Track_T & object_track_to_kill);

   /*===========================================================================*\
   * FUNCTION: Update_Merged_Objects_Properties()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track_to_kill
   * const F360_Calibrations_T & calib
   * const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Globals_T& globals
   * const Point& merged_object_center_vcs
   * const F360_Dimensions_T & dimensions
   * F360_Object_Track_T & object_track_to_keep
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
   * Function updates objects properties after merge.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Update_Merged_Objects_Properties(
      const F360_Object_Track_T & object_track_to_kill,
      const F360_Calibrations_T & calibrations,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const Point& merged_object_center_vcs,
      const F360_Dimensions_T& dimensions,
      F360_Object_Track_T& object_track_to_keep)
   {
      Inherit_Flags_After_Merge(object_track_to_keep, object_track_to_kill);

      Adjust_Obj_States_After_Merge(dimensions, calibrations, host, sensors, globals, merged_object_center_vcs, object_track_to_keep);

      Flag_Single_Object_On_And_Behind_SEP(sep, calibrations, globals, object_track_to_keep);
   }

   /*===========================================================================*\
   * FUNCTION: Adjust_Obj_States_After_Merge()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Dimensions_T & merged_object_initial_size
   * const F360_Calibrations_T& calibrations
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Globals_T& globals,
   * const Point& merged_object_center_vcs,
   * F360_Object_Track_T & object_track_to_keep
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
   * Function update objects state after merge
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Adjust_Obj_States_After_Merge(
      const F360_Dimensions_T & merged_object_initial_size,
      const F360_Calibrations_T& calibrations,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const Point& merged_object_center_vcs,
      F360_Object_Track_T& object_track_to_keep)
   {

      // Change position and reference point of object to correspond to the new center
      object_track_to_keep.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track_to_keep.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
      object_track_to_keep.vcs_position.x = merged_object_center_vcs.x;
      object_track_to_keep.vcs_position.y = merged_object_center_vcs.y;

      // for orientation variance
      if (F360_TRACKER_TRKFLTR_CTCA == object_track_to_keep.trk_fltr_type)
      {
         constexpr float32_t k_merged_CTCA_orientation_std = F360_DEG2RAD(20.0F);
         object_track_to_keep.f_prevent_orientation_std_decrease = true;
         object_track_to_keep.orientation_std = k_merged_CTCA_orientation_std;
      }

      // Update predicted position as well to correspond to the new center
      object_track_to_keep.predicted_vcs_position.x = object_track_to_keep.vcs_position.x;
      object_track_to_keep.predicted_vcs_position.y = object_track_to_keep.vcs_position.y;

      // Set new bbox dimensions
      object_track_to_keep.Update_Bbox_Size(merged_object_initial_size.length, merged_object_initial_size.width);

      // Update reference point to choose one which is better than CENTER
      constexpr bool f_update_obj_states = true;
      constexpr bool f_update_obj_pos_only = true;
      constexpr bool f_current_object_behind_another_object = false;
      Update_Object_Reference_Point(
         host.dist_rear_axle_to_vcs_m,
         f_update_obj_states,
         f_update_obj_pos_only,
         f_current_object_behind_another_object,
         calibrations,
         sensors,
         globals,
         object_track_to_keep);
   }

   /*===========================================================================*\
   * FUNCTION: Inherit_Flags_After_Merge()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T & object_track_to_keep,
   * const F360_Object_Track_T & object_track_to_kill
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
   * Inherit flags based on a combination of properties for the track to be kept
   * and the track to be killed.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Inherit_Flags_After_Merge(
      F360_Object_Track_T & object_track_to_keep,
      const F360_Object_Track_T & object_track_to_kill)
   {
      // Inherit flags correctly
      object_track_to_keep.f_vehicular_trk = (object_track_to_keep.f_vehicular_trk || object_track_to_kill.f_vehicular_trk);
      const bool movable_temp = (object_track_to_keep.movable_prob > 0.5F) || (object_track_to_kill.movable_prob > 0.5F);
      object_track_to_keep.movable_prob = static_cast<float32_t>(movable_temp);
      object_track_to_keep.f_moveable = (object_track_to_keep.f_moveable || object_track_to_kill.f_moveable);
   }
}
