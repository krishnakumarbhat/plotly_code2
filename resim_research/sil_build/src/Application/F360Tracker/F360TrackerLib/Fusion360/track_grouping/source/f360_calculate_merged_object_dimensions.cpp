/*===================================================================================*\
* FILE: f360_calculate_merged_object_dimensions.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Calculate_Merged_Object_Dimensions function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
* None.
*
\*===================================================================================*/

#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_calculate_merged_object_dimensions.h"
#include "f360_bounding_box.h"
#include "f360_convert_bbox_vcs_to_tcs.h"
#include "f360_math_func.h"
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_reference_point_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calculate_Merged_Object_Dimensions()
   *===========================================================================
   * RETURN VALUE:
   * F360_Dimensions_T
   *
   * PARAMETERS:
   * const F360_Object_Track_T & obj_to_keep
   * const F360_Object_Track_T & obj_to_kill
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
   * Function calculates a newly merged object's dimension, based on the keep objects
   * and kill objects data.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   F360_Dimensions_T Calculate_Merged_Object_Dimensions(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T& obj_to_keep,
      const F360_Object_Track_T& obj_to_kill)
   {
      
      float32_t measured_len1 = 0.0F;
      float32_t measured_len2 = 0.0F;

      Det_Based_Merged_Obj_Length(det_props, obj_to_keep, obj_to_kill, measured_len1, measured_len2);
      const F360_Dimensions_T dimensions = Obj_To_Kill_Based_Merged_Obj_Dimensions(obj_to_keep,obj_to_kill);
      const float32_t alpha_trust_measured_dimension = Calc_Weight(obj_to_keep.time_since_initialization, obj_to_kill.time_since_initialization);
      const float32_t alpha_trust_original_dimension = 1.0F - alpha_trust_measured_dimension;
      return Final_Merged_Obj_Dimensions(dimensions, alpha_trust_original_dimension, alpha_trust_measured_dimension, measured_len1, measured_len2);
   }

   /*===========================================================================*\
   * FUNCTION: Det_Based_Merged_Obj_Length
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Object_Track_T& obj_to_keep,
   * const F360_Object_Track_T& obj_to_kill,
   * float32_t& measured_len1,
   * float32_t& measured_len2
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
   * Calculates length of the merged obj based on associated detections
   * longitudinal spread
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Det_Based_Merged_Obj_Length(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T& obj_to_keep,
      const F360_Object_Track_T& obj_to_kill,
      float32_t& measured_len1,
      float32_t& measured_len2
   )
   {
      float32_t det_x_tcs_max_pos = obj_to_keep.bbox.Get_Length() * 0.5F;
      float32_t det_x_tcs_min_pos = -obj_to_keep.bbox.Get_Length() * 0.5F;

      for (uint32_t i = 0U; i < obj_to_keep.ndets; i++)
      {
         const uint32_t det_idx = obj_to_keep.detids[i] - 1U;
         Update_Min_Max_TCS_Pos(det_props[det_idx], obj_to_keep, det_x_tcs_max_pos, det_x_tcs_min_pos);
      }

      for (uint32_t i = 0U; i < obj_to_kill.ndets; i++)
      {
         const uint32_t det_idx = obj_to_kill.detids[i] - 1U;
         Update_Min_Max_TCS_Pos(det_props[det_idx], obj_to_keep, det_x_tcs_max_pos, det_x_tcs_min_pos);
      }
      
      //check if longitudinal middle of the kept obj is behind the killed object
      Point killed_obj_pos_in_kept_obj_tcs = {};
      Convert_VCS_Posn_To_TCS_Posn(obj_to_kill.bbox.Get_Center().x, obj_to_kill.bbox.Get_Center().y, obj_to_keep.bbox.Get_Center().x, obj_to_keep.bbox.Get_Center().y, obj_to_keep.bbox.Get_Orientation(), killed_obj_pos_in_kept_obj_tcs.x, killed_obj_pos_in_kept_obj_tcs.y);
      const bool is_obj_to_keep_behind = 0.0F < killed_obj_pos_in_kept_obj_tcs.x;
      
      measured_len2 = is_obj_to_keep_behind ? det_x_tcs_max_pos : obj_to_keep.bbox.Get_Length() * 0.5F;
      measured_len1 = -det_x_tcs_min_pos;
   }

   /*===========================================================================*\
   * FUNCTION: Update_Min_Max_TCS_Pos
   *===========================================================================
   * RETURN VALUE:
   *
   *
   * PARAMETERS:
   * const F360_Detection_Props_T& det_prop,
   * const F360_Object_Track_T& obj_to_keep,
   * float32_t& det_x_tcs_max_pos,
   * float32_t& det_x_tcs_min_pos
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
   * Updates the most extreme associated detections longitudinal position
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Update_Min_Max_TCS_Pos(
      const F360_Detection_Props_T& det_prop,
      const F360_Object_Track_T& obj_to_keep,
      float32_t& det_x_tcs_max_pos,
      float32_t& det_x_tcs_min_pos
   )
   {
      float32_t det_x_pos_tcs = 0.0F;
      float32_t det_y_pos_tcs = 0.0F;
      const float32_t det_x_pos = det_prop.vcs_position.x;
      const float32_t det_y_pos = det_prop.vcs_position.y;
      Convert_VCS_Posn_To_TCS_Posn(det_x_pos, det_y_pos, obj_to_keep.bbox.Get_Center().x, obj_to_keep.bbox.Get_Center().y, obj_to_keep.bbox.Get_Orientation(), det_x_pos_tcs, det_y_pos_tcs);

      det_x_tcs_min_pos = det_x_pos_tcs < det_x_tcs_min_pos ? det_x_pos_tcs : det_x_tcs_min_pos;
      det_x_tcs_max_pos = det_x_pos_tcs > det_x_tcs_max_pos ? det_x_pos_tcs : det_x_tcs_max_pos;
   }

   /*===========================================================================*\
   * FUNCTION: Obj_To_Kill_Based_Merged_Obj_Dimensions
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Object_Track_T& obj_to_keep,
   *  const F360_Object_Track_T& obj_to_kill
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
   * Calculates merged obj dimensions based on the position of the killed object
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   F360_Dimensions_T Obj_To_Kill_Based_Merged_Obj_Dimensions(
      const F360_Object_Track_T& obj_to_keep,
      const F360_Object_Track_T& obj_to_kill
   )
   {
      const BboxCorners vcs_bbox_kill = obj_to_kill.bbox.Get_Corners();
      const BboxCorners tcs_keep_for_bbox_kill = Convert_Bbox_VCS_To_TCS(vcs_bbox_kill, obj_to_keep); // Convert vcs_bbox_kill to TCS of target that we will keep
      float32_t min_max_vals[2][2] = {};
      Get_Min_And_Max_Of_VCS_Box(tcs_keep_for_bbox_kill, min_max_vals);
      const float32_t minpara_corn = min_max_vals[0][0];
      const float32_t maxpara_corn = min_max_vals[0][1];
      const float32_t minorth_corn = min_max_vals[1][0];
      const float32_t maxorth_corn = min_max_vals[1][1];

      const float32_t obj_to_keep_half_length = 0.5F * obj_to_keep.bbox.Get_Length();
      const float32_t obj_to_keep_half_width = 0.5F * obj_to_keep.bbox.Get_Width();
      const float32_t new_maxpara = (obj_to_keep_half_length > maxpara_corn) ? obj_to_keep_half_length : maxpara_corn;
      const float32_t new_minpara = (-obj_to_keep_half_length < minpara_corn) ? -obj_to_keep_half_length : minpara_corn;
      const float32_t new_maxorth = (obj_to_keep_half_width > maxorth_corn) ? obj_to_keep_half_width : maxorth_corn;
      const float32_t new_minorth = (-obj_to_keep_half_width < minorth_corn) ? -obj_to_keep_half_width : minorth_corn;

      F360_Dimensions_T dimensions;
      dimensions.width = new_maxorth - new_minorth;
      dimensions.length = new_maxpara - new_minpara;

      dimensions.len1 = std::abs(new_minpara);
      dimensions.len2 = new_maxpara;
      dimensions.wid1 = std::abs(new_minorth);
      dimensions.wid2 = new_maxorth;

      return dimensions;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Weight
   *===========================================================================
   * RETURN VALUE:
   * float32_t alpha_trust_measured_dimension
   *
   * PARAMETERS:
   * const float32_t& obj_to_keep_time_since_init
   * const float32_t& obj_to_kill_time_since_init
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
   * Calculates how trustable are dimensions based on detection data
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calc_Weight(
      const float32_t& obj_to_keep_time_since_init,
      const float32_t& obj_to_kill_time_since_init
   )
   {
      constexpr float32_t old_init_treshold = 10.0F;
      const bool f_both_objs_old = (obj_to_kill_time_since_init > old_init_treshold) && (obj_to_keep_time_since_init > old_init_treshold);
      float32_t alpha_trust_measured_dimension;

      if (f_both_objs_old)
      {
         alpha_trust_measured_dimension = 0.0F;
      }

      else
      {
         const float32_t time_ratio = Calc_Time_Ratio(obj_to_keep_time_since_init, obj_to_kill_time_since_init);
         alpha_trust_measured_dimension = 1.0F - time_ratio;
      }
      assert(alpha_trust_measured_dimension <= 1.0F);
      return alpha_trust_measured_dimension;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Time_Ratio
   *===========================================================================
   * RETURN VALUE:
   * float32_t time_youngest_obj / Clamp(time_oldest_obj, F360_EPSILON, time_oldest_obj);
   *
   * PARAMETERS:
   * const float32_t& obj_to_keep_time_since_init
   * const float32_t& obj_to_kill_time_since_init
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
   * Calculates time from init ratio between younger and older objects
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Calc_Time_Ratio(
      const float32_t& obj_to_keep_time_since_init,
      const float32_t& obj_to_kill_time_since_init
   )
   {
      float32_t time_oldest_obj = 0.0F;
      float32_t time_youngest_obj = 0.0F;
      if (obj_to_keep_time_since_init > obj_to_kill_time_since_init)
      {
         time_oldest_obj = obj_to_keep_time_since_init;
         time_youngest_obj = obj_to_kill_time_since_init;
      }
      else
      {
         time_oldest_obj = obj_to_kill_time_since_init;
         time_youngest_obj = obj_to_keep_time_since_init;
      }

      return time_youngest_obj / Clamp(time_oldest_obj, F360_EPSILON, time_oldest_obj);

   }

   /*===========================================================================*\
   * FUNCTION: Final_Merged_Obj_Dimensions
   *===========================================================================
   * RETURN VALUE:
   * F360_Dimensions_T final_dimensions
   *
   * PARAMETERS:
   * const F360_Dimensions_T& dimensions,
   * const float32_t& alpha_trust_original_dimension,
   * const float32_t& alpha_trust_measured_dimension
   * const float32_t& measured_len1,
   * const float32_t& measured_len2
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
   * Calculates final merged obj dimensions
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   F360_Dimensions_T Final_Merged_Obj_Dimensions(
      const F360_Dimensions_T& dimensions,
      const float32_t& alpha_trust_original_dimension,
      const float32_t& alpha_trust_measured_dimension,
      const float32_t& measured_len1,
      const float32_t& measured_len2
   )
   {
      F360_Dimensions_T final_dimensions;
      final_dimensions.width = dimensions.width;
      final_dimensions.length = (measured_len1 + measured_len2) * alpha_trust_measured_dimension + dimensions.length * alpha_trust_original_dimension;
      final_dimensions.len1 = measured_len1 * alpha_trust_measured_dimension + dimensions.len1 * alpha_trust_original_dimension;
      final_dimensions.len2 = measured_len2 * alpha_trust_measured_dimension + dimensions.len2 * alpha_trust_original_dimension;
      final_dimensions.wid1 = dimensions.wid1;
      final_dimensions.wid2 = dimensions.wid2;
      return final_dimensions;
   }
}
