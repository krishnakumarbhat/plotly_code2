/**
 * @file pt_common_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains definitions of functions which are shared across path tracking modules.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_common_functions.h"
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "ml_angle_normalize.h"
#include "ml_interval.h"
#include "ml_line.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pt_constants.h"
#include "pt_output_t.h"
#include "pt_types.h"
#include <assert.h>

/*===========================================================================*\
* Local function forward declarations
\*===========================================================================*/

/**
 * @brief checks if the input values equal the value of PT_PATH_POINTS_DEFAULT_VAL.
 *
 * @return True when intervals contain the default path point value
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7321}
 * @verification{}
 */
static boolean_T Pt_Check_If_Intervals_Contain_Default_Path_Points(float32_T interval_a_lower_limit /**< interval A lower limit */,
                                                                   float32_T interval_a_upper_limit /**< interval A upper limit */,
                                                                   float32_T interval_b_lower_limit /**< interval B lower limit */,
                                                                   float32_T interval_b_upper_limit /**< interval B upper limit */);

/**
 * @brief Returns a pair of range structs which in turn consist of two borders with float32_T type.
 *
 * @return Pair_Of_Intervals_Float_T type with two float32_T ranges
 *
 * @SRS{SF-1537}
 * @SAE{SF-2918}
 * @SDD{SF-7322}
 * @verification{}
 */
static Pt_Pair_Of_Intervals_Float_T Pt_Create_Pair_Of_Intervals_Float(float32_T min_first_interv /**< interval 1 min value */,
                                                                      float32_T max_first_interv /**< interval 1 max value */,
                                                                      float32_T min_sec_interv /**< interval 2 min value */,
                                                                      float32_T max_sec_interv /**< interval 2 max value */);

/**
 * @brief Returns a pair of range structs which in turn consist of two borders with int32_t type.
 *
 * @return Pair_Of_Intervals_Int32_t type with two int32_t ranges
 *
 * @SRS{SF-1577}
 * @SAE{SF-2918}
 * @SDD{SF-7323}
 * @verification{}
 */
static Pt_Pair_Of_Intervals_Int32_T Pt_Create_Pair_Of_Intervals_Int32(int32_t min_first_interv /**< interval 1 min value */,
                                                                      int32_t max_first_interv /**< interval 1 max value */,
                                                                      int32_t min_sec_interv /**< interval 2 min value */,
                                                                      int32_t max_sec_interv /**< interval 2 max value */);

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

void Pt_Extrapolate_Path(Pt_Path_T *p_path, const Pt_Core_Calibration_T *p_cals, const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_cals);
   assert(NULL != grid_array);

   /*extrapolation of values at upper end*/
   if ((p_path->last_p >= PT_EXTRAPOL_INDEX_OFFSET) && (p_path->last_p < PT_HIGHEST_GRID_POINT_INDEX))
   {
      Pt_Extrapolate_Lateral_Points(p_path, p_cals, grid_array, p_path->last_p, (uint8_t) (p_path->last_p - PT_EXTRAPOL_INDEX_OFFSET),
                                    (uint8_t) (p_path->last_p + PT_SINGLE_GRID_POINT_OFFSET), PT_HIGHEST_GRID_POINT_INDEX);
   }

   /*extrapolation of values at lower end*/
   if ((p_path->first_p > PT_LOWEST_GRID_POINT_INDEX) && ((p_path->first_p + PT_EXTRAPOL_INDEX_OFFSET) <= PT_HIGHEST_GRID_POINT_INDEX))
   {
      Pt_Extrapolate_Lateral_Points(p_path, p_cals, grid_array, p_path->first_p,
                                    (uint8_t) (p_path->first_p + PT_EXTRAPOL_INDEX_OFFSET), PT_LOWEST_GRID_POINT_INDEX,
                                    (uint8_t) (p_path->first_p - PT_SINGLE_GRID_POINT_OFFSET));
   }
}

void Pt_Extrapolate_Path_To_Next_Pt_After_Zero_Pt(Pt_Path_T *p_path,
                                                  const Pt_Core_Calibration_T *p_cals,
                                                  const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_cals);
   assert(NULL != grid_array);
   assert(PT_MID_GRID_POINT_INDEX >= PT_SINGLE_GRID_POINT_OFFSET);

   /*extrapolation of values at upper end*/

   if ((p_path->last_p >= PT_EXTRAPOL_INDEX_OFFSET) && (p_path->last_p < (PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET)))
   {
      Pt_Extrapolate_Lateral_Points(p_path, p_cals, grid_array, p_path->last_p, (uint8_t) (p_path->last_p - PT_EXTRAPOL_INDEX_OFFSET),
                                    (uint8_t) (p_path->last_p + PT_SINGLE_GRID_POINT_OFFSET),
                                    PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET);
   }

   /* extrapolation of values at lower end */
   if (p_path->first_p >= PT_MID_GRID_POINT_INDEX)
   {
      Pt_Extrapolate_Lateral_Points(p_path, p_cals, grid_array, p_path->first_p,
                                    (uint8_t) (p_path->first_p + PT_EXTRAPOL_INDEX_OFFSET),
                                    PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET,
                                    (uint8_t) (p_path->first_p - PT_SINGLE_GRID_POINT_OFFSET));
   }
}

void Pt_Extrapolate_Lateral_Points(Pt_Path_T *p_path,
                                   const Pt_Core_Calibration_T *p_cals,
                                   const float32_T grid_array[PT_NUM_GRID_POINTS],
                                   const uint8_t extrapol_ref_idx_a,
                                   const uint8_t extrapol_ref_idx_b,
                                   const uint8_t for_loop_init_index,
                                   const uint8_t for_loop_exit_index)
{
   uint8_t i;
   float32_T extrapolated_value;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != p_cals);
   assert(NULL != grid_array);

   if (for_loop_init_index <= for_loop_exit_index)
   {
      for (i = for_loop_init_index; i <= for_loop_exit_index; i++)
      {
         extrapolated_value = Get_Y_Value_From_Line_By_Coordinates(grid_array[extrapol_ref_idx_b],
                                                                   p_path->path_points[extrapol_ref_idx_b],
                                                                   grid_array[extrapol_ref_idx_a],
                                                                   p_path->path_points[extrapol_ref_idx_a], grid_array[i]);

         if (Fbk_Abs_F(extrapolated_value) > p_cals->k_pt_zone_max_posn)
         {
            p_path->path_points[i] = PT_PATH_POINTS_DEFAULT_VAL;
         }
         else
         {
            p_path->path_points[i] = extrapolated_value;
         }
      }
   }
}

uint8_t Pt_Get_Number_Of_Path_Points(const Pt_Path_T *p_path)
{
   uint8_t num_of_path_points;

   /* Assert */
   assert(NULL != p_path);

   if ((p_path->first_p != PT_DEFAULT_DISCR_BORDER) && (p_path->last_p != PT_DEFAULT_DISCR_BORDER))
   {
      if (p_path->last_p >= p_path->first_p)
      {
         /* first_p and last_p are zero based, therefore an offset has to be applied to get the real number of path points */
         num_of_path_points = (uint8_t) ((uint8_t) (p_path->last_p - p_path->first_p) + PT_SINGLE_GRID_POINT_OFFSET);
      }
      else
      {
         /* first_p and last_p are zero based, therefore an offset has to be applied to get the real number of path points */
         num_of_path_points = (uint8_t) ((uint8_t) (p_path->first_p - p_path->last_p) + PT_SINGLE_GRID_POINT_OFFSET);
      }
   }
   else
   {
      num_of_path_points = FBK_ZERO_UINT;
   }

   return num_of_path_points;
}

boolean_T Pt_Are_Path_Intervals_Nested(float32_T interval_a_lower_limit,
                                       float32_T interval_a_upper_limit,
                                       float32_T interval_b_lower_limit,
                                       float32_T interval_b_upper_limit)
{

   Pt_Pair_Of_Intervals_Float_T pair_of_intervals;
   boolean_T f_path_intervals_contain_default_value;
   boolean_T f_path_intervals_nested;

   /*Create interval*/
   pair_of_intervals = Pt_Create_Pair_Of_Intervals_Float(interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit,
                                                         interval_b_upper_limit);

   /*Check whether default values for path points is contained*/
   f_path_intervals_contain_default_value = Pt_Check_If_Intervals_Contain_Default_Path_Points(
      interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit, interval_b_upper_limit);

   /*Check whether intervals are nested.*/
   f_path_intervals_nested =
      (boolean_T) ((Is_Float_Interval_Subset_Of_Float_Interval(&pair_of_intervals.range_A, &pair_of_intervals.range_B))
                   || (Is_Float_Interval_Subset_Of_Float_Interval(&pair_of_intervals.range_B, &pair_of_intervals.range_A)));


   return (boolean_T) (f_path_intervals_nested && (!f_path_intervals_contain_default_value));
}

boolean_T Pt_Are_Intervals_Overlapping(int32_t interval_a_lower_limit,
                                       int32_t interval_a_upper_limit,
                                       int32_t interval_b_lower_limit,
                                       int32_t interval_b_upper_limit)
{
   Pt_Pair_Of_Intervals_Int32_T pair_of_intervals_for_overlapping_test;
   boolean_T f_intervals_overlapping;

   pair_of_intervals_for_overlapping_test = Pt_Create_Pair_Of_Intervals_Int32(interval_a_lower_limit, interval_a_upper_limit,
                                                                              interval_b_lower_limit, interval_b_upper_limit);

   f_intervals_overlapping = (boolean_T) (Does_Int_Range_Overlap_Int_Range(&pair_of_intervals_for_overlapping_test.range_A,
                                                                           &pair_of_intervals_for_overlapping_test.range_B)
                                          || Does_Int_Range_Overlap_Int_Range(&pair_of_intervals_for_overlapping_test.range_B,
                                                                              &pair_of_intervals_for_overlapping_test.range_A));

   return f_intervals_overlapping;
}

boolean_T Pt_Is_Path_Longitudinal(const Pt_Path_T *p_path)
{
   /* Assert */
   assert(NULL != p_path);

   return (boolean_T) ((PATH_DIRECTION_LONG_BACKWARD == p_path->direction) || (PATH_DIRECTION_LONG_FORWARD == p_path->direction));
}

boolean_T Pt_Has_Path_Longitudinal_Tendencies(const Pt_Path_T *p_path)
{
   boolean_T f_path_has_longitudinal_tendencies = FBK_FALSE;

   /* Assert */
   assert(NULL != p_path);

   if ((PATH_DIRECTION_NONE == p_path->direction)
       && (Fbk_Abs_F(p_path->first.x - p_path->last_mat.x) > Fbk_Abs_F(p_path->first.y - p_path->last_mat.y)))
   {
      f_path_has_longitudinal_tendencies = FBK_TRUE;
   }

   return f_path_has_longitudinal_tendencies;
}

boolean_T Pt_Is_Path_Lateral(const Pt_Path_T *p_path)
{
   /* Assert */
   assert(NULL != p_path);

   return (boolean_T) ((PATH_DIRECTION_LAT_LEFT == p_path->direction) || (PATH_DIRECTION_LAT_RIGHT == p_path->direction));
}

boolean_T Pt_Has_Path_Lateral_Tendencies(const Pt_Path_T *p_path)
{
   boolean_T f_path_has_longitudinal_tendencies = FBK_FALSE;

   /* Assert */
   assert(NULL != p_path);

   if ((PATH_DIRECTION_NONE == p_path->direction)
       && (Fbk_Abs_F(p_path->first.y - p_path->last_mat.y) > Fbk_Abs_F(p_path->first.x - p_path->last_mat.x)))
   {
      f_path_has_longitudinal_tendencies = FBK_TRUE;
   }

   return f_path_has_longitudinal_tendencies;
}

boolean_T Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(const Pt_Path_T *p_path)
{
   /* Assert */
   assert(NULL != p_path);

   return (boolean_T) ((PATH_DIRECTION_LAT_LEFT == p_path->direction) || (PATH_DIRECTION_LONG_BACKWARD == p_path->direction));
}

Pt_Object_Orientation_T Pt_Determine_Object_Orientation(const float32_T heading, const Pt_Core_Calibration_T *p_cals)
{
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_NONE;
   float32_T obj_heading                   = Normalize_Angle(heading, FBK_ZERO_F);

   /* Assert */
   assert(NULL != p_cals);

   if ((Fbk_Abs_F(obj_heading) >= p_cals->k_pt_lower_lim_obj_orient_lat)
       && (Fbk_Abs_F(obj_heading) <= p_cals->k_pt_upper_lim_obj_orient_lat))
   {
      obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;
   }
   else if ((Fbk_Abs_F(obj_heading) < p_cals->k_pt_lower_lim_obj_orient_lat)
            || (Fbk_Abs_F(obj_heading) > p_cals->k_pt_upper_lim_obj_orient_lat))
   {
      obj_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;
   }
   else
   {
      /*Do nothing*/
   }
   return obj_orientation;
}

boolean_T Pt_Do_Path_Direction_And_Object_Orientation_Match(const Pt_Path_T *p_path, const Pt_Object_Orientation_T *p_obj_orientation)
{
   boolean_T f_path_dir_and_obj_orientation_match;

   /* Assert */
   assert(NULL != p_obj_orientation);

   f_path_dir_and_obj_orientation_match =
      (boolean_T) ((Pt_Is_Path_Longitudinal(p_path) && (PT_OBJECT_ORIENTATION_LONGITUDINAL == *(p_obj_orientation)))
                   || (Pt_Is_Path_Lateral(p_path) && (PT_OBJECT_ORIENTATION_LATERAL == *(p_obj_orientation))));

   return f_path_dir_and_obj_orientation_match;
}

uint8_t Pt_Get_Number_Of_Non_Zero_Path_Points(const Pt_Path_T *p_path)
{
   uint8_t i;
   uint8_t num_of_path_points = FBK_ZERO_UINT;

   /* Assert */
   assert(NULL != p_path);

   for (i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      if (Fbk_Abs_F(p_path->path_points[i]) > PT_PATH_POINTS_DEFAULT_VAL)
      {
         num_of_path_points++;
      }
   }

   return num_of_path_points;
}

boolean_T Pt_Is_Object_Tracking_This_Path_Already_Unassigned(const Pt_Path_T *p_path)
{
   boolean_T f_object_is_unassigned;

   /* Assert */
   assert(NULL != p_path);

   f_object_is_unassigned =
      (boolean_T) ((PATH_STATUS_DEFAULT != p_path->path_state) && (FBK_ZERO_UINT == p_path->obj_curr_used_for_path_build.id));

   return f_object_is_unassigned;
}

boolean_T Pt_Do_Paths_Have_The_Same_Direction(const Pt_Path_T *p_path_one, const Pt_Path_T *p_path_two)
{
   boolean_T f_paths_have_same_direction = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_path_one);
   assert(NULL != p_path_two);

   if (((PATH_DIRECTION_LAT_LEFT == p_path_one->direction) && (PATH_DIRECTION_LAT_LEFT == p_path_two->direction))
       || ((PATH_DIRECTION_LAT_RIGHT == p_path_one->direction) && (PATH_DIRECTION_LAT_RIGHT == p_path_two->direction))
       || ((PATH_DIRECTION_LONG_BACKWARD == p_path_one->direction) && (PATH_DIRECTION_LONG_BACKWARD == p_path_two->direction))
       || ((PATH_DIRECTION_LONG_FORWARD == p_path_one->direction) && (PATH_DIRECTION_LONG_FORWARD == p_path_two->direction)))
   {
      f_paths_have_same_direction = FBK_TRUE;
   }

   return f_paths_have_same_direction;
}

boolean_T Pt_Are_Paths_Longitudinally_Orientated(const Pt_Path_T *p_path_a, const Pt_Path_T *p_path_b)
{
   boolean_T f_paths_are_longitudinally_orientated;

   f_paths_are_longitudinally_orientated = (boolean_T) ((Pt_Is_Path_Longitudinal(p_path_a)) && (Pt_Is_Path_Longitudinal(p_path_b)));

   return f_paths_are_longitudinally_orientated;
}

boolean_T Pt_Is_Obj_Moving_Longitudinal(const Pt_Object_Mov_Direction_T *p_obj_move_dir)
{
   /* Assert */
   assert(NULL != p_obj_move_dir);

   return (boolean_T) ((PT_OBJECT_MOV_DIR_LONG_BACKWARD == *p_obj_move_dir) || (PT_OBJECT_MOV_DIR_LONG_FORWARD == *p_obj_move_dir));
}

boolean_T Pt_Is_Obj_Moving_Lateral(const Pt_Object_Mov_Direction_T *p_obj_move_dir)
{
   /* Assert */
   assert(NULL != p_obj_move_dir);

   return (boolean_T) ((PT_OBJECT_MOV_DIR_LAT_LEFT == *p_obj_move_dir) || (PT_OBJECT_MOV_DIR_LAT_RIGHT == *p_obj_move_dir));
}

boolean_T Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(const Pt_Object_Mov_Direction_T *p_obj_move_dir)
{
   /* Assert */
   assert(NULL != p_obj_move_dir);

   return (boolean_T) ((PT_OBJECT_MOV_DIR_LAT_RIGHT == *p_obj_move_dir) || (PT_OBJECT_MOV_DIR_LONG_FORWARD == *p_obj_move_dir));
}

boolean_T Pt_Is_Obj_Mov_Dir_Against_Vcs(const Pt_Object_Mov_Direction_T *p_obj_move_dir)
{
   /* Assert */
   assert(NULL != p_obj_move_dir);

   return (boolean_T) ((PT_OBJECT_MOV_DIR_LAT_LEFT == *p_obj_move_dir) || (PT_OBJECT_MOV_DIR_LONG_BACKWARD == *p_obj_move_dir));
}

float32_T Pt_Calculate_Path_Heading_Near_Object(const uint8_t next_point_idx,
                                                const Pt_Path_T *p_path,
                                                const Pt_Object_Mov_Direction_T obj_move_dir,
                                                const float32_T grid_array[PT_NUM_GRID_POINTS])
{
   Vector_2d_T prev_to_next_point_vector;
   Vector_2d_T long_unit_vector;
   float32_T path_heading_temp;
   uint8_t previous_idx;

   /* Assert */
   assert(NULL != p_path);
   assert(NULL != grid_array);
   assert(PT_NUM_GRID_POINTS > next_point_idx);

   path_heading_temp = FBK_ZERO_F;
   long_unit_vector  = Create_2d_Vector_X_Normal();

   if (Pt_Is_Obj_Mov_Dir_Against_Vcs(&obj_move_dir))
   {
      previous_idx = (uint8_t) (next_point_idx + PT_SINGLE_GRID_POINT_OFFSET);
   }
   else if (Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(&obj_move_dir))
   {
      assert(next_point_idx >= PT_SINGLE_GRID_POINT_OFFSET);
      previous_idx = (uint8_t) (next_point_idx - PT_SINGLE_GRID_POINT_OFFSET);
   }
   else
   {
      previous_idx = PT_DEFAULT_MATCH_INDEX;
   }

   /*Check whether previous idx is initialized. This is only happening when an object has an undefined movement direction.
    * However this is filtered out in superordinate function.*/
   if (PT_DEFAULT_MATCH_INDEX != previous_idx)
   {
      /*Always build up a vector from previous point index to the next depending on the object moving direction*/
      prev_to_next_point_vector.x = p_path->path_points[next_point_idx] - p_path->path_points[previous_idx];
      prev_to_next_point_vector.y = grid_array[next_point_idx] - grid_array[previous_idx];

      if (PT_OBJECT_MOV_DIR_LAT_RIGHT == obj_move_dir)
      {
         path_heading_temp = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &prev_to_next_point_vector));
         path_heading_temp = Normalize_Angle(path_heading_temp, FBK_ZERO_F);
      }
      else if (PT_OBJECT_MOV_DIR_LAT_LEFT == obj_move_dir)
      {
         path_heading_temp = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &prev_to_next_point_vector));
         path_heading_temp = -Normalize_Angle(path_heading_temp, FBK_ZERO_F);
      }
      else if (PT_OBJECT_MOV_DIR_LONG_FORWARD == obj_move_dir)
      {
         Fbk_Swap_Float(&(prev_to_next_point_vector.x), &(prev_to_next_point_vector.y));
         path_heading_temp = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &prev_to_next_point_vector));

         if (prev_to_next_point_vector.y > FBK_ZERO_F)
         {
            path_heading_temp = Normalize_Angle(path_heading_temp, FBK_ZERO_F);
         }
         else
         {
            path_heading_temp = -Normalize_Angle(path_heading_temp, FBK_ZERO_F);
         }
      }
      else if (PT_OBJECT_MOV_DIR_LONG_BACKWARD == obj_move_dir)
      {
         Fbk_Swap_Float(&(prev_to_next_point_vector.x), &(prev_to_next_point_vector.y));
         path_heading_temp = Fast_Acos(Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&long_unit_vector, &prev_to_next_point_vector));
         if (prev_to_next_point_vector.y > FBK_ZERO_F)
         {
            path_heading_temp = Normalize_Angle(path_heading_temp, FBK_ZERO_F);
         }
         else
         {
            path_heading_temp = -Normalize_Angle(path_heading_temp, FBK_ZERO_F);
         }
      }
      else
      {
         /* Do nothing */
      }
   }
   return path_heading_temp;
}

uint8_t Pt_Read_Next_Point_Dep_On_Path_Dir(const Pt_Point_Indices_Dir_Indep_Obj_T *p_next_point_idx, const Pt_Path_T *p_path)
{
   uint8_t next_pt_idx;

   /* Asserts */
   assert(NULL != p_next_point_idx);

   if (Pt_Is_Path_Longitudinal(p_path))
   {
      next_pt_idx = p_next_point_idx->next_point_idx_long_path;
   }
   else
   {
      next_pt_idx = p_next_point_idx->next_point_idx_lat_path;
   }

   return next_pt_idx;
}

boolean_T Pt_Search_Empty_Path_Slot(uint8_t *p_path_idx, const Pt_Path_T paths[PT_NUMBER_OF_PATHS])
{
   boolean_T f_empty_path_slot_found = FBK_FALSE;
   uint8_t path_idx;

   /* Assert */
   assert(NULL != p_path_idx);
   assert(NULL != paths);

   /* Search for an empty path slot */
   for (path_idx = FBK_ZERO_UINT; path_idx < PT_NUMBER_OF_PATHS; path_idx++)
   {
      if (PATH_STATUS_DEFAULT == paths[path_idx].path_state)
      {
         f_empty_path_slot_found = FBK_TRUE;
         *p_path_idx             = path_idx;
         break;
      }
   }
   return f_empty_path_slot_found;
}

/*===========================================================================*\
* Local Functions Definitions
\*===========================================================================*/

static boolean_T Pt_Check_If_Intervals_Contain_Default_Path_Points(float32_T interval_a_lower_limit,
                                                                   float32_T interval_a_upper_limit,
                                                                   float32_T interval_b_lower_limit,
                                                                   float32_T interval_b_upper_limit)
{
   boolean_T f_intervals_contain_default_path_points;

   boolean_T f_a_lower_is_def = (boolean_T) (Fbk_Abs_F(interval_a_lower_limit - PT_PATH_POINTS_DEFAULT_VAL) < EPSILON);
   boolean_T f_a_upper_is_def = (boolean_T) (Fbk_Abs_F(interval_a_upper_limit - PT_PATH_POINTS_DEFAULT_VAL) < EPSILON);
   boolean_T f_b_lower_is_def = (boolean_T) (Fbk_Abs_F(interval_b_lower_limit - PT_PATH_POINTS_DEFAULT_VAL) < EPSILON);
   boolean_T f_b_upper_is_def = (boolean_T) (Fbk_Abs_F(interval_b_upper_limit - PT_PATH_POINTS_DEFAULT_VAL) < EPSILON);

   f_intervals_contain_default_path_points =
      (boolean_T) (f_a_lower_is_def || f_a_upper_is_def || f_b_lower_is_def || f_b_upper_is_def);

   return f_intervals_contain_default_path_points;
}

static Pt_Pair_Of_Intervals_Float_T Pt_Create_Pair_Of_Intervals_Float(float32_T min_first_interv,
                                                                      float32_T max_first_interv,
                                                                      float32_T min_sec_interv,
                                                                      float32_T max_sec_interv)
{
   Pt_Pair_Of_Intervals_Float_T pair_of_intervals;

   pair_of_intervals.range_A = Create_Float_Range(min_first_interv, max_first_interv);
   pair_of_intervals.range_B = Create_Float_Range(min_sec_interv, max_sec_interv);

   return pair_of_intervals;
}

static Pt_Pair_Of_Intervals_Int32_T Pt_Create_Pair_Of_Intervals_Int32(int32_t min_first_interv,
                                                                      int32_t max_first_interv,
                                                                      int32_t min_sec_interv,
                                                                      int32_t max_sec_interv)
{
   Pt_Pair_Of_Intervals_Int32_T pair_of_intervals;

   pair_of_intervals.range_A = Create_Int_Range(min_first_interv, max_first_interv);
   pair_of_intervals.range_B = Create_Int_Range(min_sec_interv, max_sec_interv);

   return pair_of_intervals;
}
