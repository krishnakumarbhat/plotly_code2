/*===================================================================================*\
* FILE: f360_verify_object_size.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Verify_Object_Size function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
* None.
*
\*===================================================================================*/

#include "f360_verify_object_size.h"
#include "f360_shrinking_supporting_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Verify_Object_Size()
   *===========================================================================
   * RETURN VALUE:
   * bool f_merged_obj_size_ok
   *
   * PARAMETERS:
   * const F360_Object_Track_T & obj_to_keep
   * const F360_Object_Track_T & obj_to_kill
   * const F360_Calibrations_T & calibs
   * F360_Dimensions_T & initial_merged_obj_dimensions
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
   * Function verifies merged object dimensions and verifies whether object 
   * dimensions after merge are smaller than maximum allowed values.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Verify_Object_Size(
      const F360_Object_Track_T & obj_to_keep,
      const F360_Object_Track_T & obj_to_kill,
      const F360_Calibrations_T & calibs,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Dimensions_T & initial_merged_obj_dimensions)
   {
       bool f_merged_obj_size_ok = true;

       const bool f_is_in_shrink_prevention_zone = In_Special_Zone_For_No_Shrinking(obj_to_keep.vcs_position, tracker_info.variant.type);

       // If the merged moveable object has big length or width, the merge will be canceled.
       const float32_t abs_obj_speed = std::abs(obj_to_keep.speed);
       if ((abs_obj_speed < calibs.k_object_shrinking_speed_threshold) && (!f_is_in_shrink_prevention_zone))
       {
          // For slow moving objects we limit the length and width because these objects might be shrinking in size
          const float32_t max_length = F360_Linear_Equation_With_Saturation(abs_obj_speed, calibs.k_speed_for_min_length_of_slow_moving_objects, calibs.k_speed_for_max_length_of_slow_moving_objects, calibs.k_min_length_for_slow_moving_objects, calibs.k_max_length_for_slow_moving_objects);

          f_merged_obj_size_ok = (initial_merged_obj_dimensions.length < max_length) &&
                          (initial_merged_obj_dimensions.width <= std::min(calibs.k_movable_max_target_width, max_length));
       }
       else 
       {
          constexpr float32_t max_allowed_merged_object_width = 3.9F;

          // Set maximum allowed merge object length
          constexpr float32_t max_allowed_merged_object_length_fast_moving_buffer = 3.0F;
          const float32_t merged_obj_max_length = calibs.k_fast_movable_max_target_length + max_allowed_merged_object_length_fast_moving_buffer;

          const bool f_obj_not_too_long = (initial_merged_obj_dimensions.length <= merged_obj_max_length);
          const bool f_obj_not_too_wide = (initial_merged_obj_dimensions.width <= max_allowed_merged_object_width);

          // The time_since_split is reset to -1.0 when it's been long enough time since split. So a time above 0 indicates that a split has recently occured.
          const bool f_both_object_have_been_split_recently =  (obj_to_keep.time_since_split >= 0.0F) && (obj_to_kill.time_since_split >= 0.0F);
          if (f_both_object_have_been_split_recently)
          {
             // Only restrict maximum length, i.e. allow merges in TCS y direction if objects have been split recently
             f_merged_obj_size_ok = f_obj_not_too_long;
          }
          else
          {
             f_merged_obj_size_ok = f_obj_not_too_long && f_obj_not_too_wide;
          }
       }
      return f_merged_obj_size_ok;
   }
}
