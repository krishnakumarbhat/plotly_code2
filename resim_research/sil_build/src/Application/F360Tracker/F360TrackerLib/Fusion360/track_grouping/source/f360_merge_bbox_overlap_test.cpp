/*===================================================================================*\
* FILE: f360_merge_bbox_overlap_test.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Merge_Bbox_Overlap_Test() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
* None.
*
\*===================================================================================*/

#include "f360_merge_bbox_overlap_test.h"
#include "f360_bounding_box.h"
#include "f360_norm_heading_angle.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Merge_Bbox_Overlap_Test()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Object_Track_T & first_object,
   * const F360_Object_Track_T & second_object
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
   * Function checks if extended bounding boxes for objects overlap or not.
   * Extension is computed based on object lengths, speeds, and headings.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Merge_Bbox_Overlap_Test(
      const F360_Object_Track_T & first_object,
      const F360_Object_Track_T & second_object)
   {

      /* Calculate bbox extension.
       *  Allow for bigger extension for longer objects, larger speeds and smaller heading diff between the objects.
       *  A plot visualising the function for computing the extension can be seen in ticket ICV-45
       */

      const float32_t k_slow_speed_threshold_for_extn_factor = 5.0F; /* Threshold for considering an object as slow - moving for the purpose of extension factor calculation */ 

      // Heading dependency - the maximum possible multiplication factor (for large speeds) is smaller if heading difference between bboxes are large
      float32_t slow_speed_mult_factor = 0.06F; 

      // Transform second object's position to first object's TCS
      float32_t longitudinal_offset = 0.0F;
      float32_t lateral_offset = 0.0F;
	  
      Convert_VCS_Posn_To_TCS_Posn(
         second_object.bbox.Get_Center().x,
         second_object.bbox.Get_Center().y,
         first_object.bbox.Get_Center().x,
         first_object.bbox.Get_Center().y,
         first_object.bbox.Get_Orientation(),
         longitudinal_offset,
         lateral_offset
      );

      // Objects are side-by-side if lateral separation dominates longitudinal separation
      const bool f_objects_side_by_side = (std::abs(lateral_offset) > std::abs(longitudinal_offset));
      const float32_t abs_mean_obj_speed = std::abs(0.5F * (first_object.speed + second_object.speed));
      if (f_objects_side_by_side || (abs_mean_obj_speed > k_slow_speed_threshold_for_extn_factor))
      {
         /* For targets moving at a higher speed that 5mps,don't extend objects as much when side by side to avoid overlap due to small heading differences. */
         slow_speed_mult_factor = 0.0417F;
      }
      const float32_t k_max_large_speed_scale_factor = 0.125F; // 3/24
      const float32_t k_min_hdg_for_large_factor = F360_DEG2RAD(3.0F);
      const float32_t k_max_hdg_for_small_factor = F360_DEG2RAD(10.0F);
      const float32_t hdg_diff = Normalize_Heading_Angle(first_object.vcs_heading.Value() - second_object.vcs_heading.Value(), 0.0F);
      const float32_t large_speed_scale_factor = F360_Linear_Equation_With_Saturation(std::abs(hdg_diff), k_min_hdg_for_large_factor, k_max_hdg_for_small_factor, k_max_large_speed_scale_factor, slow_speed_mult_factor); // Factor can be increased up to 3/24 if object heading is very similar and object speeds are large
      
      // Speed dependecy - the multiplication factor is larger for larger speeds
      const float32_t k_min_speed_for_small_factor = F360_KPH2MPS(30.0F);
      const float32_t k_max_speed_for_large_factor = F360_KPH2MPS(90.0F);
      const float32_t k_extension_factor = F360_Linear_Equation_With_Saturation(abs_mean_obj_speed, k_min_speed_for_small_factor, k_max_speed_for_large_factor, slow_speed_mult_factor, large_speed_scale_factor);

      // Compute total length of the two bboxes and saturate.
      // Step function: Use baseline transition (1-5 m/s) until 4 m/s, then jump to high-speed values
      const float32_t k_step_speed_threshold = 4.0F; // Speed at which to jump to high-speed saturation values
      const float32_t k_max_length_high_speed = 30.0F;
      const float32_t k_min_length_high_speed = 18.0F;

      float32_t k_max_length;
      float32_t k_min_length;
      
      if (abs_mean_obj_speed < k_step_speed_threshold)
      {
         // Below 4 m/s: Use baseline transition (1-5 m/s range)
         const float32_t k_min_length_low_speed = 5.0F;
         const float32_t k_max_length_low_speed = 5.0F;
         const float32_t k_max_speed_threshold_for_length_saturation = 5.0F;
         const float32_t k_min_speed_threshold_for_length_saturation = 1.0F;
         k_max_length = F360_Linear_Equation_With_Saturation(abs_mean_obj_speed, k_min_speed_threshold_for_length_saturation, k_max_speed_threshold_for_length_saturation, k_max_length_low_speed, k_max_length_high_speed);
         k_min_length = F360_Linear_Equation_With_Saturation(abs_mean_obj_speed, k_min_speed_threshold_for_length_saturation, k_max_speed_threshold_for_length_saturation, k_min_length_low_speed, k_min_length_high_speed);
      }
      else
      {
         // At or above 4 m/s: Jump to high-speed saturation values
         k_max_length = k_max_length_high_speed;
         k_min_length = k_min_length_high_speed;
      }
      
      const float32_t saturated_obj_tot_length = F360_Saturate(first_object.bbox.Get_Length() + second_object.bbox.Get_Length(), k_min_length, k_max_length);

      // Multiply total length of objects with speed and heading dependant factor to obtain the final bbox extension
      const float32_t length_ext = k_extension_factor * saturated_obj_tot_length;
      const float32_t width_ext = 0.0F;


      /* Extend bounding boxes and check if the extended bounding boxes overlap/collide */
      BoundingBox first_bbox{ first_object.bbox };
      BoundingBox second_bbox{ second_object.bbox };

      first_bbox.Extend_Boundaries(width_ext, width_ext, length_ext, length_ext);
      second_bbox.Extend_Boundaries(width_ext, width_ext, length_ext, length_ext);

      return first_bbox.Collides(second_bbox);
   }
}
