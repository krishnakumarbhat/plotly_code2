/**
 * @file pt_common_functions_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_common_functions.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43479}
 */

#include "pt_common_functions_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_float_range_t.h"
#include "ml_int_range_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pt_common_functions.c"
#include "pt_common_functions.h"
#include "pt_directions.h"
#include "pt_output_t.h"
#include "pt_types.h"
}


/**
 * Tests the extrapolation function with a path segment which is not covering the whole grid array. Thus an extrapolation to the
 * right and also to the left is expected. \uts{CSCSA-43480} \sdd{SF-7338} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Extrapolate_Path__extrapolate_a_given_constant_path_to_both_sides)
{
   /** \arrange setup a path which is not covering the whole grid array. */
   uint8_t lower_idx    = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx    = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_value = 1.0f;

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(p_grid_array[lower_idx] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(p_grid_array[upper_idx] + 3.0f, 1.0f), 0.0f, path_value);

   /** \action call the extrapolation step. */
   Pt_Extrapolate_Path(&path, &cals, p_grid_array);

   /** \assert Expect path value to be set across the whole grid array due to extrapolation. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(path.path_points[idx], path_value);
   }
}


/**
 * Tests the extrapolation function with a path which is completly covering the grid array already. Thus no extrapolation shall be
 * applied. \uts{CSCSA-43481} \sdd{SF-7338} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Extrapolate_Path__path_with_full_coverage_across_grid_array_shall_not_be_extrapolated)
{
   /** \arrange setup a path which is not covering the whole grid array. */
   uint8_t lower_idx    = PT_LOWEST_GRID_POINT_INDEX;
   uint8_t upper_idx    = PT_HIGHEST_GRID_POINT_INDEX;
   float32_T path_value = 1.0f;

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(p_grid_array[lower_idx] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(p_grid_array[upper_idx] + 3.0f, 1.0f), 0.0f, path_value);

   /** \action call the extrapolation step. */
   Pt_Extrapolate_Path(&path, &cals, p_grid_array);

   /** \assert Expect path to be unchanged, since it is already covering the whole area. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(path.path_points[idx], path_value);
   }
}


/**
 * Tests the extrapolation function for the extrapolation needed for unfinished path at the pt output determination. Here a path
 * with sufficient range is already given so that no extrapolation is needed. Thus no extrapolation shall be applied.
 * \uts{CSCSA-43482} \sdd{SF-7339} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Extrapolate_Path_To_Next_Pt_After_Zero_Pt__path_with_sufficient_coverage_is_already_given)
{
   /** \arrange setup a path which is not covering the whole grid array. */
   uint8_t lower_idx    = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx    = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_value = 1.0f;

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(p_grid_array[lower_idx] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(p_grid_array[upper_idx] + 3.0f, 1.0f), 0.0f, path_value);

   /** \action call the extrapolation step. */
   Pt_Extrapolate_Path_To_Next_Pt_After_Zero_Pt(&path, &cals, p_grid_array);

   /** \assert Expect path to be unchanged, since it is already covering the needed area. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      if (idx >= lower_idx && idx <= upper_idx)
      {
         EXPECT_FLOAT_EQ(path.path_points[idx], path_value);
      }
      else
      {
         EXPECT_FLOAT_EQ(path.path_points[idx], 0.0f);
      }
   }
}


/**
 * Tests the extrapolation function for the extrapolation needed for unfinished path at the pt output determination. Here an
 * extrapolation at the lower end shall be executed. \uts{CSCSA-43483} \sdd{SF-7339} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Extrapolate_Path_To_Next_Pt_After_Zero_Pt__extrapolate_at_lower_end)
{
   /** \arrange setup a path which is not covering the whole grid array. */
   uint8_t lower_idx    = PT_MID_GRID_POINT_INDEX + 2u;
   uint8_t upper_idx    = PT_HIGHEST_GRID_POINT_INDEX;
   float32_T path_value = 1.0f;

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(p_grid_array[lower_idx] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(p_grid_array[upper_idx] + 3.0f, 1.0f), 0.0f, path_value);

   /** \action call the extrapolation step. */
   Pt_Extrapolate_Path_To_Next_Pt_After_Zero_Pt(&path, &cals, p_grid_array);

   /** \assert Expect path to be extrapolated. */
   for (uint8_t idx = PT_MID_GRID_POINT_INDEX - 1u; idx < upper_idx; idx++)
   {
      EXPECT_FLOAT_EQ(path.path_points[idx], path_value);
   }
}


/**
 * Tests the extrapolation routine for a path which is already covering the highest possible area. Thus no extrapolation shall be
 * applied on that side. \uts{CSCSA-43484} \sdd{SF-7337} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Extrapolate_Lateral_Points__dont_extrapolate_area_since_it_is_already_recorded_in_the_path)
{
   /** \arrange setup a path which is already covering the desired area. */
   uint8_t lower_idx    = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx    = PT_HIGHEST_GRID_POINT_INDEX;
   float32_T path_value = 1.0f;

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(p_grid_array[lower_idx] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(p_grid_array[upper_idx] + 3.0f, 1.0f), 0.0f, path_value);

   /** \action call the extrapolation function. */
   Pt_Extrapolate_Lateral_Points(&path, &cals, p_grid_array, upper_idx, upper_idx - PT_EXTRAPOL_INDEX_OFFSET,
                                 upper_idx + PT_SINGLE_GRID_POINT_OFFSET, upper_idx);

   /** \assert Expect path to be unchanged, since it is already covering the whole area. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      if (idx >= lower_idx && idx <= upper_idx)
      {
         EXPECT_FLOAT_EQ(path.path_points[idx], path_value);
      }
      else
      {
         EXPECT_FLOAT_EQ(path.path_points[idx], 0.0f);
      }
   }
}


/**
 * Tests the extrapolation routine for a path which contains invalid point values in extrapolation process. Those values are
 * expected to be reset to their defaults. \uts{CSCSA-43485} \sdd{SF-7337} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Extrapolate_Lateral_Points__reset_point_to_its_default_since_it_is_outside_of_the_path_tracking_zone)
{
   /** \arrange setup a path which needs to be extrapolated to the upper direction. */
   uint8_t lower_idx       = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx       = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;
   cals.k_pt_zone_max_posn = 120.0f;

   path.path_points[lower_idx] = 100.0f;
   for (uint8_t idx = lower_idx + PT_SINGLE_GRID_POINT_OFFSET; idx <= upper_idx; idx++)
   {
      path.path_points[idx] = path.path_points[idx - PT_SINGLE_GRID_POINT_OFFSET] + 4.0f;
   }
   path.first_p = lower_idx;
   path.last_p  = upper_idx;

   /** \action call the extrapolation function. */
   Pt_Extrapolate_Lateral_Points(&path, &cals, p_grid_array, upper_idx, upper_idx - PT_EXTRAPOL_INDEX_OFFSET,
                                 upper_idx + PT_SINGLE_GRID_POINT_OFFSET, PT_HIGHEST_GRID_POINT_INDEX);

   /** \assert Expect path to only have zeros added. */
   for (uint8_t idx = path.last_p + PT_SINGLE_GRID_POINT_OFFSET; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      EXPECT_FLOAT_EQ(path.path_points[idx], 0.0f);
   }
}


/**
 * Check whether the path length is returned correctly for a path. Here a path length of 4 is expected to be returned.
 * \uts{CSCSA-43486} \sdd{SF-7341} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Get_Number_Of_Path_Points__valid_path_as_input)
{
   /** \arrange setup a valid path. */
   uint8_t result;
   path.first_p = PT_MID_GRID_POINT_INDEX;
   path.last_p  = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function to test. */
   result = Pt_Get_Number_Of_Path_Points(&path);

   /** \assert Expect path length to be 4. */
   EXPECT_EQ(result, path.last_p - path.first_p + 1);
}

/**
 * Check whether the path length is returned correctly for a path. Here an uninitialized path is given as an input. Thus a default
 * length is expected to be returned. \uts{CSCSA-43487} \sdd{SF-7341} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Get_Number_Of_Path_Points__invalid_path_as_input)
{
   /** \arrange setup an invalid path. */
   uint8_t result;
   path.first_p = PT_DEFAULT_DISCR_BORDER;
   path.last_p  = PT_DEFAULT_DISCR_BORDER;

   /** \action call length calculation of a path. */
   result = Pt_Get_Number_Of_Path_Points(&path);

   /** \assert Expect path length to be 0 due to invalidity. */
   EXPECT_EQ(result, 0);
}


/**
 * Check whether the path length is returned correctly for a path. Here a non ordered path is given as an input. Catch this
 * behavior. \uts{CSCSA-43488} \sdd{SF-7341} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Get_Number_Of_Path_Points__non_ordered_path_as_input)
{
   /** \arrange setup a non ordered path. */
   uint8_t result;
   path.first_p = 5u;
   path.last_p  = 0u;

   /** \action call length calculation of a path. */
   result = Pt_Get_Number_Of_Path_Points(&path);

   /** \assert Expect path length to be 6u independent of the order of the path. */
   EXPECT_EQ(result, 6u);
}


/**
 * Check whether path intervals are nested. Here the second interval is completly contained in the first one. Thus true is
 * expected. \uts{CSCSA-43489} \sdd{SF-7331} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Are_Path_Intervals_Nested__interval_b_is_contained_in_interval_a)
{
   /** \arrange setup nested intervals. */
   boolean_T res;
   float32_T interval_a_lower_limit = 1.0f;
   float32_T interval_a_upper_limit = 1.5f;
   float32_T interval_b_lower_limit = interval_a_lower_limit + EPSILON;
   float32_T interval_b_upper_limit = interval_a_upper_limit - EPSILON;

   /** \action check for nested intervals. */
   res = Pt_Are_Path_Intervals_Nested(interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit, interval_b_upper_limit);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Check whether path intervals are nested. Here the second interval begins and ends above the first one. Thus false is expected.
 * \uts{CSCSA-43490} \sdd{SF-7331} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Are_Path_Intervals_Nested__interval_b_is_above_interval_a)
{
   /** \arrange set up non nested intervals. */
   boolean_T res;
   float32_T interval_a_lower_limit = 1.0f;
   float32_T interval_a_upper_limit = 1.5f;
   float32_T interval_b_lower_limit = interval_a_upper_limit + EPSILON;
   float32_T interval_b_upper_limit = 2.0f * interval_b_lower_limit;

   /** \action check for nested intervals. */
   res = Pt_Are_Path_Intervals_Nested(interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit, interval_b_upper_limit);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Check whether path intervals are nested. Here the first interval is contained in the second. Thus true is expected.
 * \uts{CSCSA-43491} \sdd{SF-7331} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Are_Path_Intervals_Nested__interval_a_is_contained_in_interval_b)
{
   /** \arrange set up nested intervals. */
   boolean_T res;
   float32_T interval_b_lower_limit = 1.0f;
   float32_T interval_b_upper_limit = 2.0f;
   float32_T interval_a_lower_limit = interval_b_lower_limit + EPSILON;
   float32_T interval_a_upper_limit = interval_b_upper_limit - EPSILON;

   /** \action check for nested intervals. */
   res = Pt_Are_Path_Intervals_Nested(interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit, interval_b_upper_limit);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Check whether path intervals are nested. Here the first interval is contained in the second. However the default path point
 * value is contained there. Thus false is expected. \uts{CSCSA-43492} \sdd{SF-7331} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Are_Path_Intervals_Nested__interval_a_is_contained_in_interval_b_but_with_default_path_point_value)
{
   /** \arrange set up nested intervals but one interval with default path point values. */
   boolean_T res;
   float32_T interval_b_lower_limit = -1.0f;
   float32_T interval_b_upper_limit = -interval_b_lower_limit;
   float32_T interval_a_lower_limit = 0.0f;
   float32_T interval_a_upper_limit = 0.0f;

   /** \action check for nested intervals. */
   res = Pt_Are_Path_Intervals_Nested(interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit, interval_b_upper_limit);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Check whether two intervals are overlapping. Here two intervals which are equal are passed, so that true is expected.
 * \uts{CSCSA-43493} \sdd{SF-7330} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Are_Intervals_Overlapping__interval_a_equal_to_interval_b)
{
   /** \arrange set up equal intervals. */
   boolean_T res;
   int32_t interval_a_lower_limit = 1;
   int32_t interval_a_upper_limit = 3;
   int32_t interval_b_lower_limit = interval_a_lower_limit;
   int32_t interval_b_upper_limit = interval_a_upper_limit;

   /** \action check whether intervals are overlapping. */
   res = Pt_Are_Intervals_Overlapping(interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit, interval_b_upper_limit);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Check whether two intervals are overlapping. Here two intervals which are not overlapping are given. Thus frue is expected.
 * \uts{CSCSA-43494} \sdd{SF-7330} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Are_Intervals_Overlapping__intervals_are_not_overlapping)
{
   /** \arrange set up non overlapping intervals. */
   boolean_T res;
   int32_t interval_a_lower_limit = 1;
   int32_t interval_a_upper_limit = 3;
   int32_t interval_b_lower_limit = interval_a_upper_limit + 1;
   int32_t interval_b_upper_limit = interval_b_lower_limit + 1;

   /** \action check whether intervals are overlapping. */
   res = Pt_Are_Intervals_Overlapping(interval_a_lower_limit, interval_a_upper_limit, interval_b_lower_limit, interval_b_upper_limit);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Tests functionality of checking whether path has a tendency to be longitudinal. Here the path has longitudinal tendencies. Thus
 * true is expected. \uts{CSCSA-43495} \sdd{SF-7344} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Longitudinal_Tendencies__path_has_longitudinal_tendencies_fast_abs_negative)
{
   /** \arrange setup a longitudinal tendency. */
   boolean_T result;
   path.direction = PATH_DIRECTION_NONE;
   path.first     = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(5.0f, 1.0f);

   /** \action call function to test. */
   result = Pt_Has_Path_Longitudinal_Tendencies(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}


/**
 * Tests functionality of checking whether path has a tendency to be longitudinal. Here the path has longitudinal tendencies. Thus
 * true is expected. \uts{CSCSA-43496} \sdd{SF-7344} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Longitudinal_Tendencies__path_has_longitudinal_tendencies_fast_abs_positive)
{
   /** \arrange setup a longitudinal tendency. */
   boolean_T result;
   path.direction = PATH_DIRECTION_NONE;
   path.first     = Create_2d_Vector_Coordinates(5.0f, 1.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call function to test. */
   result = Pt_Has_Path_Longitudinal_Tendencies(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}


/**
 * Tests functionality of checking whether path has a tendency to be longitudinal. Here the path has lateral tendencies. Thus false
 * is expected. \uts{CSCSA-43497} \sdd{SF-7344} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Longitudinal_Tendencies__path_has_lateral_tendency)
{
   /** \arrange setup a lateral tendency. */
   boolean_T result;
   path.direction = PATH_DIRECTION_NONE;
   path.first     = Create_2d_Vector_Coordinates(1.0f, 5.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call function to test. */
   result = Pt_Has_Path_Longitudinal_Tendencies(&path);

   /** \assert Expect False. */
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of checking whether path has a tendency to be longitudinal. Here the path has already a set direction. Thus
 * false is expected. \uts{CSCSA-43498} \sdd{SF-7344} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Longitudinal_Tendencies__path_has_a_direction_set_already)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   result = Pt_Has_Path_Longitudinal_Tendencies(&path);

   /** \assert Expect False. */
   EXPECT_FALSE(result);
}

/**
 * Tests functionality of checking whether path has a tendency to be lateral. Here the path has lateral tendencies. Thus true is
 * expected. \uts{CSCSA-43499} \sdd{SF-7343} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Lateral_Tendencies__path_has_lateral_tendencies_fast_abs_negative)
{
   /** \arrange setup a lateral tendency. */
   boolean_T result;
   path.direction = PATH_DIRECTION_NONE;
   path.first     = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(1.0f, 5.0f);
   /** \action call function to test. */
   result = Pt_Has_Path_Lateral_Tendencies(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}


/**
 * Tests functionality of checking whether path has a tendency to be lateral. Here the path has lateral tendencies. Thus true is
 * expected. \uts{CSCSA-43500} \sdd{SF-7343} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Lateral_Tendencies__path_has_longitudinal_tendencies_fast_abs_positive)
{
   /** \arrange setup a lateral tendency. */
   boolean_T result;
   path.direction = PATH_DIRECTION_NONE;
   path.first     = Create_2d_Vector_Coordinates(1.0f, 5.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call function to test. */
   result = Pt_Has_Path_Lateral_Tendencies(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}


/**
 * Tests functionality of checking whether path has a tendency to be lateral. Here the path has lateral tendencies. Thus false is
 * expected. \uts{CSCSA-43501} \sdd{SF-7343} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Lateral_Tendencies__path_has_longitudinal_tendency)
{
   /** \arrange setup a longitudinal tendency. */
   boolean_T result;
   path.direction = PATH_DIRECTION_NONE;
   path.first     = Create_2d_Vector_Coordinates(5.0f, 1.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   /** \action call function to test. */
   result = Pt_Has_Path_Lateral_Tendencies(&path);

   /** \assert Expect False. */
   EXPECT_FALSE(result);
}


/**
 * Tests functionality of checking whether path has a tendency to be lateral. Here the path has already a set direction. Thus false
 * is expected. \uts{CSCSA-43502} \sdd{SF-7343} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Has_Path_Lateral_Tendencies__path_has_a_direction_set_already)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   result = Pt_Has_Path_Lateral_Tendencies(&path);

   /** \assert Expect False. */
   EXPECT_FALSE(result);
}


/**
 * Checks whether path direction is longitudinal. Here the path is longitudinal forward. Thus true is expected.
 * \uts{CSCSA-43503} \sdd{SF-7352} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Longitudinal__path_is_long_forward)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   result = Pt_Is_Path_Longitudinal(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether path direction is longitudinal. Here the path is longitudinal backward. Thus true is expected.
 * \uts{CSCSA-43504} \sdd{SF-7352} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Longitudinal__path_is_long_backward)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action call function to test. */
   result = Pt_Is_Path_Longitudinal(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether path direction is longitudinal. Here the path is lateral left. Thus false is expected.
 * \uts{CSCSA-43505} \sdd{SF-7352} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Longitudinal__path_is_lateral_left)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action call function to test. */
   result = Pt_Is_Path_Longitudinal(&path);

   /** \assert Expect False. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether path direction is lateral. Here the path is lateral left. Thus true is expected.
 * \uts{CSCSA-43506} \sdd{SF-7351} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Lateral__path_is_lateral_left)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action call function to test. */
   result = Pt_Is_Path_Lateral(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether path direction is lateral. Here the path is lateral right. Thus true is expected.
 * \uts{CSCSA-43507} \sdd{SF-7351} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Lateral__path_is_lateral_right)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LAT_RIGHT;

   /** \action call function to test. */
   result = Pt_Is_Path_Lateral(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether path direction is longitudinal. Here the path is lateral left. Thus false is expected.
 * \uts{CSCSA-43508} \sdd{SF-7351} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Lateral__path_is_long_backward)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action call function to test. */
   result = Pt_Is_Path_Lateral(&path);

   /** \assert Expect False. */
   EXPECT_FALSE(result);
}


/**
 * Checks whether path are aligned against vcs. Here the path is lateral left. Thus true is expected.
 * \uts{CSCSA-43509} \sdd{SF-7350} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Dir_Against_Vcs_Axis_Dir__path_is_lateral_left)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action call function to test. */
   result = Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether path is aligned against vcs. Here the path is longitudinal backward. Thus true is expected.
 * \uts{CSCSA-43510} \sdd{SF-7350} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Dir_Against_Vcs_Axis_Dir__path_is_longitudinal_backward)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action call function to test. */
   result = Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(&path);

   /** \assert Expect True. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether path is aligned against vcs. Here the path is longitudinal forward. Thus false is expected.
 * \uts{CSCSA-43511} \sdd{SF-7350} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Path_Dir_Against_Vcs_Axis_Dir__path_is_longitudinal_forward)
{
   /** \arrange setup a path with given direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   result = Pt_Is_Path_Dir_Against_Vcs_Axis_Dir(&path);

   /** \assert Expect False. */
   EXPECT_FALSE(result);
}


/**
 * Checks whether the object orientation check is done correctly. Here the object heading is in the range where orientation is
 * classified as lateral. \uts{CSCSA-43512} \sdd{SF-7334} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Determine_Object_Orientation__object_moves_lateral)
{
   /** \arrange setup a lateral moving object.tracker_data. */
   Pt_Object_Orientation_T orientation;
   float32_T heading = 0.5f * (cals.k_pt_lower_lim_obj_orient_lat + cals.k_pt_upper_lim_obj_orient_lat);

   /** \action call orientation determination. */
   orientation = Pt_Determine_Object_Orientation(heading, &cals);

   /** \assert Expect object to move lateral. */
   EXPECT_EQ(orientation, PT_OBJECT_ORIENTATION_LATERAL);
}


/**
 * Checks whether the object orientation check is done correctly. Here the object heading is in the range where orientation is
 * classified as longitudinal. \uts{CSCSA-43513} \sdd{SF-7334} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Determine_Object_Orientation__object_moves_longitudinal)
{
   /** \arrange setup a longitudinal moving object.tracker_data. */
   Pt_Object_Orientation_T orientation;
   float32_T heading = cals.k_pt_lower_lim_obj_orient_lat - EPSILON;

   /** \action call orientation determination. */
   orientation = Pt_Determine_Object_Orientation(heading, &cals);

   /** \assert Expect object to move longitudinal. */
   EXPECT_EQ(orientation, PT_OBJECT_ORIENTATION_LONGITUDINAL);
}


/**
 * Check whether path direction and orientation of object match. Here a longitudinal path and object with an orientation of
 * longitudinal are given. Thus true is expected. \uts{CSCSA-43514} \sdd{SF-7335} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Path_Direction_And_Object_Orientation_Match__both_are_longitudinal)
{
   /** \arrange set up long path and a longitudinal orientation. */
   Pt_Object_Orientation_T orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;
   boolean_T res;
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action call orientation match check. */
   res = Pt_Do_Path_Direction_And_Object_Orientation_Match(&path, &orientation);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Check whether path direction and orientation of object match. Here a lateral path and object with an orientation of lateral are
 * given. Thus true is expected. \uts{CSCSA-43515} \sdd{SF-7335} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Path_Direction_And_Object_Orientation_Match__both_are_lateral)
{
   /** \arrange set up lateral path and a lateral orientation. */
   Pt_Object_Orientation_T orientation = PT_OBJECT_ORIENTATION_LATERAL;
   boolean_T res;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action call orientation match check. */
   res = Pt_Do_Path_Direction_And_Object_Orientation_Match(&path, &orientation);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Check whether path direction and orientation of object match. Here a lateral path and object with an orientation of longitudinal
 * are given. Thus true is expected. \uts{CSCSA-43516} \sdd{SF-7335} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Path_Direction_And_Object_Orientation_Match__mismatching)
{
   /** \arrange set up lateral path and a longitudinal orientation. */
   Pt_Object_Orientation_T orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;
   boolean_T res;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action call orientation match check. */
   res = Pt_Do_Path_Direction_And_Object_Orientation_Match(&path, &orientation);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}

/**
 * Test the non zero path point calculation. Here the input path has no points. Thus 0 is expected.
 * \uts{CSCSA-43517} \sdd{SF-7340} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Get_Number_Of_Non_Zero_Path_Points__default_path_point_is_given_filled_with_const_zero)
{
   /** \arrange Set up a path filled with zero. */
   uint8_t num_non_zero_points;

   /** \action call calculation of non zero path points. */
   num_non_zero_points = Pt_Get_Number_Of_Non_Zero_Path_Points(&path);

   /** \assert Expect that all points are zero. */
   EXPECT_EQ(num_non_zero_points, 0);
}


/**
 * Test the non zero path point calculation. Here the input path has all points set. Thus PT_NUM_GRID_POINTS is expected.
 * \uts{CSCSA-43518} \sdd{SF-7340} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Get_Number_Of_Non_Zero_Path_Points__path_is_completly_filled_with_vals)
{
   /** \arrange Set up a path filled with values. */
   uint8_t num_non_zero_points;
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      path.path_points[idx] = 1.0f;
   }

   /** \action call calculation of non zero path points. */
   num_non_zero_points = Pt_Get_Number_Of_Non_Zero_Path_Points(&path);

   /** \assert Expect that all points are set. */
   EXPECT_EQ(num_non_zero_points, PT_NUM_GRID_POINTS);
}


/**
 * Checks whether an object is finished with building up its path. Here the object is still assigned.
 * \uts{CSCSA-43519} \sdd{SF-7349} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Object_Tracking_This_Path_Already_Unassigned__object_still_assigned)
{
   /** \arrange Set up a path which is still in creation process. */
   boolean_T res;
   path.path_state                      = PATH_STATUS_CREATION;
   path.obj_curr_used_for_path_build.id = 1u;

   /** \action check whether an object is finished with its path. */
   res = Pt_Is_Object_Tracking_This_Path_Already_Unassigned(&path);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether an object is finished with building up its path. Here the object is unassigned.
 * \uts{CSCSA-43520} \sdd{SF-7349} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Object_Tracking_This_Path_Already_Unassigned__object_is_unassigned)
{
   /** \arrange Set up a path which has finished its creation process. */
   boolean_T res;
   path.path_state                      = PATH_STATUS_MATURE;
   path.obj_curr_used_for_path_build.id = 0u;

   /** \action check whether an object is finished with its path. */
   res = Pt_Is_Object_Tracking_This_Path_Already_Unassigned(&path);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether both paths have the same direction. Here both are lateral left, thus true is expected.
 * \uts{CSCSA-43521} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__both_are_lateral_left)
{
   /** \arrange set up both pt_persistent.paths to lateral left. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_LEFT;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether both paths have the same direction. Here both are lateral right, thus true is expected.
 * \uts{CSCSA-43522} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__both_are_lateral_right)
{
   /** \arrange set up both pt_persistent.paths to lateral right. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_RIGHT;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether both paths have the same direction. Here both are longitudinal forward, thus true is expected.
 * \uts{CSCSA-43523} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__both_are_longitudinal_forward)
{
   /** \arrange set up both pt_persistent.paths to longitudinal forward. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_FORWARD;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether both paths have the same direction. Here both are longitudinal backward, thus true is expected.
 * \uts{CSCSA-43524} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__both_are_longitudinal_backward)
{
   /** \arrange set up both pt_persistent.paths to longitudinal backward. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether both paths have the same direction. Here both are longitudinal backward, thus false is expected.
 * \uts{CSCSA-43525} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__mismatching_directions)
{
   /** \arrange set up both pt_persistent.paths whose directions are not matching. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether both paths are longitudinally oriented. Here the orientations are different. Thus false is expected
 * \uts{CSCSA-43526} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__orientation_mismatching)
{
   /** \arrange set up pt_persistent.paths with differing orientation. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action check whether both input pt_persistent.paths have the same orientation. */
   res = Pt_Are_Paths_Longitudinally_Orientated(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether both paths are longitudinally oriented. Here the orientations are both longitudinal. Thus true is expected
 * \uts{CSCSA-43527} \sdd{SF-7332} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Are_Paths_Longitudinally_Orientated__both_are_longitudinal)
{
   /** \arrange set up pt_persistent.paths with longitudinal orientation. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action check whether both input pt_persistent.paths are longitudinally. */
   res = Pt_Are_Paths_Longitudinally_Orientated(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether the object is longitudinal oriented. Here it is oriented longitudinal backwards. Thus true is expected.
 * \uts{CSCSA-43528} \sdd{SF-7348} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Moving_Longitudinal__object_is_long_backward)
{
   /** \arrange set up a longitudinal backward orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action check whether object is longitudinally oriented. */
   res = Pt_Is_Obj_Moving_Longitudinal(&obj_move_dir);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether the object is longitudinal oriented. Here it is oriented lateral. Thus false is expected.
 * \uts{CSCSA-43529} \sdd{SF-7348} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Moving_Longitudinal__object_is_lat)
{
   /** \arrange set up a lateral orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;

   /** \action check whether object is longitudinally oriented. */
   res = Pt_Is_Obj_Moving_Longitudinal(&obj_move_dir);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether the object is lateral oriented. Here it is oriented lateral left. Thus true is expected.
 * \uts{CSCSA-43530} \sdd{SF-7347} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Moving_Lateral__object_is_lat_left)
{
   /** \arrange set up a lateral left orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;

   /** \action check whether object is lateral oriented. */
   res = Pt_Is_Obj_Moving_Lateral(&obj_move_dir);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether the object is lateral oriented. Here it is oriented longitudinal . Thus false is expected.
 * \uts{CSCSA-43531} \sdd{SF-7347} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Moving_Lateral__object_is_long_backward)
{
   /** \arrange set up a longitudinal orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action check whether object is lateral oriented. */
   res = Pt_Is_Obj_Moving_Lateral(&obj_move_dir);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether the object is aligned with vcs. Here it is oriented lateral right. Thus true is expected.
 * \uts{CSCSA-43532} \sdd{SF-7346} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs__object_is_lat_right)
{
   /** \arrange set up a lateral right orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action check whether object orientation is aligned with vcs. */
   res = Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(&obj_move_dir);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether the object is aligned with vcs. Here it is oriented against it. Thus false is expected.
 * \uts{CSCSA-43533} \sdd{SF-7346} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs__object_is_long_backward)
{
   /** \arrange set up a longitudinal backward orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action check whether object is aligned with vcs */
   res = Pt_Is_Obj_Mov_Dir_Aligned_With_Vcs(&obj_move_dir);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether the object is against vcs. Here it is oriented against it. Thus true is expected.
 * \uts{CSCSA-43534} \sdd{SF-7345} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Mov_Dir_Against_Vcs__object_is_long_backward)
{
   /** \arrange set up a longitudinal backward orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action check whether object is against vcs. */
   res = Pt_Is_Obj_Mov_Dir_Against_Vcs(&obj_move_dir);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether the object is against vcs. Here it is oriented aligned with vcs. Thus false is expected.
 * \uts{CSCSA-43535} \sdd{SF-7345} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Is_Obj_Mov_Dir_Against_Vcs__object_is_lateral_right)
{
   /** \arrange set up a longitudinal forward orientation. */
   boolean_T res;
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;

   /** \action check whether object is against vcs. */
   res = Pt_Is_Obj_Mov_Dir_Against_Vcs(&obj_move_dir);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether the heading of a path segment is returned correctly. Here the object moves lateral left so that a negative
 * heading is expected. \uts{CSCSA-43536} \sdd{SF-7333} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Calculate_Path_Heading_Near_Object__object_moves_lateral_left)
{
   /** \arrange setup a constant path and object to the left. */
   float32_T result_heading;
   Pt_Object_Mov_Direction_T obj_direction;
   Vector_2d_T first_path_point, last_path_point;
   uint8_t next_point_idx;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   first_path_point = Create_2d_Vector_Coordinates(1.0f, p_grid_array[PT_MID_GRID_POINT_INDEX] - EPSILON);
   last_path_point  = Create_2d_Vector_Coordinates(-1.0f, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + EPSILON);

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LAT_RIGHT, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         first_path_point, last_path_point, 0.0f, 1.0f);

   next_point_idx = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;
   obj_direction  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   /** \action call function to test. */
   result_heading = Pt_Calculate_Path_Heading_Near_Object(next_point_idx, &path, obj_direction, p_grid_array);


   /** \assert Expect negative heading result. */
   EXPECT_FLOAT_EQ(result_heading, -0.5f * PI);
}


/**
 * Checks whether the heading of a path segment is returned correctly. Here the object moves lateral right so that a positive
 * heading is expected. \uts{CSCSA-43537} \sdd{SF-7333} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Calculate_Path_Heading_Near_Object__object_moves_lateral_right)
{
   /** \arrange setup a constant path and object to the right. */
   float32_T result_heading;
   Pt_Object_Mov_Direction_T obj_direction;
   Vector_2d_T first_path_point, last_path_point;
   uint8_t next_point_idx;

   first_path_point = Create_2d_Vector_Coordinates(1.0f, p_grid_array[PT_MID_GRID_POINT_INDEX] - EPSILON);
   last_path_point  = Create_2d_Vector_Coordinates(-1.0f, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + EPSILON);

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LAT_RIGHT, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         first_path_point, last_path_point, 0.0f, 1.0f);

   next_point_idx = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;
   obj_direction  = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   /** \action call function to test. */
   result_heading = Pt_Calculate_Path_Heading_Near_Object(next_point_idx, &path, obj_direction, p_grid_array);


   /** \assert Expect positive heading result. */
   EXPECT_FLOAT_EQ(result_heading, 0.5f * PI);
}


/**
 * Checks whether the heading of a path segment is returned correctly. Here the object moves longitudinal forward so that a
 * positive heading is expected. \uts{CSCSA-43538} \sdd{SF-7333} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Calculate_Path_Heading_Near_Object__object_moves_long_forward)
{
   /** \arrange setup a constant path and object moving longitudinal forward. */
   float32_T result_heading;
   Pt_Object_Mov_Direction_T obj_direction;
   Vector_2d_T first_path_point, last_path_point;
   uint8_t next_point_idx;

   first_path_point = Create_2d_Vector_Coordinates(p_grid_array[PT_MID_GRID_POINT_INDEX] - EPSILON, 1.0f);
   last_path_point  = Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + EPSILON, -1.0f);

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_FORWARD, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX,
                         0u, first_path_point, last_path_point, 0.0f, 1.0f);

   next_point_idx = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;
   obj_direction  = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   /** \action call function to test. */
   result_heading = Pt_Calculate_Path_Heading_Near_Object(next_point_idx, &path, obj_direction, p_grid_array);


   /** \assert Expect heading of zero. */
   EXPECT_FLOAT_EQ(result_heading, 0.0f);
}


/**
 * Checks whether the heading of a path segment is returned correctly. Here the object moves longitudinal forward so that a
 * positive heading is expected. \uts{CSCSA-43539} \sdd{SF-7333} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Calculate_Path_Heading_Near_Object__object_moves_long_forward_with_positive_lateral_shift)
{
   /** \arrange setup a constant path and object moving longitudinal forward. */
   float32_T result_heading;
   Pt_Object_Mov_Direction_T obj_direction;
   Vector_2d_T first_path_point, last_path_point;
   uint8_t next_point_idx;
   float32_T lat_shift;

   first_path_point = Create_2d_Vector_Coordinates(p_grid_array[PT_MID_GRID_POINT_INDEX] - EPSILON, 1.0f);
   last_path_point  = Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + EPSILON, -1.0f);
   lat_shift        = ((float32_T) p_grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]
                - (float32_T) p_grid_array[PT_LOWEST_GRID_POINT_INDEX])
               / 50.0f;

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_FORWARD, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX,
                         0u, first_path_point, last_path_point, lat_shift, 1.0f);

   next_point_idx = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;
   obj_direction  = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   /** \action call function to test. */
   result_heading = Pt_Calculate_Path_Heading_Near_Object(next_point_idx, &path, obj_direction, p_grid_array);


   /** \assert Expect heading near zero. */
   EXPECT_NEAR(result_heading, 0.02f, EPSILON);
}

/**
 * Checks whether the heading of a path segment is returned correctly. Here the object moves longitudinal backward so that a
 * positive heading is expected. \uts{CSCSA-43540} \sdd{SF-7333} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Calculate_Path_Heading_Near_Object__object_moves_long_backward)
{
   /** \arrange setup a constant path and object moving longitudinal backward. */
   float32_T result_heading;
   Pt_Object_Mov_Direction_T obj_direction;
   Vector_2d_T first_path_point, last_path_point;
   uint8_t next_point_idx;

   first_path_point = Create_2d_Vector_Coordinates(p_grid_array[PT_MID_GRID_POINT_INDEX] - EPSILON, 1.0f);
   last_path_point  = Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + EPSILON, -1.0f);

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_FORWARD, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX,
                         0u, first_path_point, last_path_point, 0.0f, 1.0f);

   next_point_idx = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;
   obj_direction  = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action call function to test. */
   result_heading = Pt_Calculate_Path_Heading_Near_Object(next_point_idx, &path, obj_direction, p_grid_array);


   /** \assert Expect heading of PI. */
   EXPECT_FLOAT_EQ(result_heading, PI);
}


/**
 * Checks whether the heading of a path segment is returned correctly. Here the object moves longitudinal backward so that a
 * positive heading is expected. \uts{CSCSA-43541} \sdd{SF-7333} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Calculate_Path_Heading_Near_Object__object_moves_long_backward_with_positive_lateral_shift)
{
   /** \arrange setup a constant path and object moving longitudinal backward. */
   float32_T result_heading;
   Pt_Object_Mov_Direction_T obj_direction;
   Vector_2d_T first_path_point, last_path_point;
   uint8_t next_point_idx;
   float32_T lat_shift;

   first_path_point = Create_2d_Vector_Coordinates(p_grid_array[PT_MID_GRID_POINT_INDEX] - EPSILON, 1.0f);
   last_path_point  = Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + EPSILON, -1.0f);
   lat_shift        = ((float32_T) p_grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]
                - (float32_T) p_grid_array[PT_LOWEST_GRID_POINT_INDEX])
               / 50.0f;

   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_FORWARD, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX,
                         0u, first_path_point, last_path_point, -lat_shift, 1.0f);

   next_point_idx = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;
   obj_direction  = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action call function to test. */
   result_heading = Pt_Calculate_Path_Heading_Near_Object(next_point_idx, &path, obj_direction, p_grid_array);


   /** \assert Expect heading of near PI but with a positive sign. */
   EXPECT_NEAR(result_heading, 3.121f, EPSILON);
}


/**
 * Checks whether the heading of a path segment is returned correctly. Here the object does not have a valid movement direction.
 * Thus a heading of 0.0f is expected. \uts{CSCSA-43542} \sdd{SF-7333} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Calculate_Path_Heading_Near_Object__object_does_not_have_specified_direction)
{
   /** \arrange setup a constant path and object moving longitudinal backward. */
   float32_T result_heading;
   Pt_Object_Mov_Direction_T obj_direction;
   uint8_t next_point_idx;

   next_point_idx = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;
   obj_direction  = PT_OBJECT_MOV_DIR_NONE;

   /** \action call function to test. */
   result_heading = Pt_Calculate_Path_Heading_Near_Object(next_point_idx, &path, obj_direction, p_grid_array);


   /** \assert Expect heading of zero. */
   EXPECT_FLOAT_EQ(result_heading, 0.0f);
}


/**
 * Check whether passed floating values contain the path point default value. Here everything is valid, thus false is expected
 * \uts{CSCSA-43543} \sdd{SF-7321} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Check_If_Intervals_Contain_Default_Path_Points__all_valid)
{
   /** \arrange set up valid parameters. */
   boolean_T res;

   /** \action Call routine to check for default path point value containment. */
   res = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, FBK_ONE_F, FBK_ONE_F, FBK_ONE_F);

   /** \assert Expect false */
   EXPECT_FALSE(res);
}


/**
 * Check whether passed floating values contain the path point default value. Here all are invalid, thus true is expected
 * \uts{CSCSA-43544} \sdd{SF-7321} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Check_If_Intervals_Contain_Default_Path_Points__all_invalid)
{
   /** \arrange set up invalid parameters. */
   boolean_T res;

   /** \action Call routine to check for default path point value containment. */
   res = Pt_Check_If_Intervals_Contain_Default_Path_Points(PT_PATH_POINTS_DEFAULT_VAL, PT_PATH_POINTS_DEFAULT_VAL,
                                                           PT_PATH_POINTS_DEFAULT_VAL, PT_PATH_POINTS_DEFAULT_VAL);

   /** \assert Expect true */
   EXPECT_TRUE(res);
}

/**
 * Call constructor for floating point pair of interval. Check that the mapping is done correctly
 * \uts{CSCSA-43545} \sdd{SF-7322} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Create_Pair_Of_Intervals_Float__verify_constructor)
{
   /** \arrange use a constant macro of 1.0f to set up the constructor. */
   Pt_Pair_Of_Intervals_Float_T pair_of_intervals;

   /** \action Call constructor for pair of interval creation. */
   pair_of_intervals = Pt_Create_Pair_Of_Intervals_Float(FBK_ONE_F, FBK_ONE_F, FBK_ONE_F, FBK_ONE_F);

   /** \assert Expect each entry to be equal to one */
   EXPECT_FLOAT_EQ(pair_of_intervals.range_A.max, FBK_ONE_F);
   EXPECT_FLOAT_EQ(pair_of_intervals.range_A.min, FBK_ONE_F);
   EXPECT_FLOAT_EQ(pair_of_intervals.range_B.max, FBK_ONE_F);
   EXPECT_FLOAT_EQ(pair_of_intervals.range_B.min, FBK_ONE_F);
}


/**
 * Call constructor for int32_t interval pair. Check that the mapping is done correctly
 * \uts{CSCSA-43546} \sdd{SF-7323} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Create_Pair_Of_Intervals_Int32__verify_constructor)
{
   /** \arrange use a random numbers to verify the constructor. */
   Pt_Pair_Of_Intervals_Int32_T pair_of_ranges;

   /** \action Call constructor for pair of int ranges creation. */
   pair_of_ranges = Pt_Create_Pair_Of_Intervals_Int32(1, 2, 3, 4);

   /** \assert Expect the mapping to be done correctly */
   EXPECT_EQ(pair_of_ranges.range_A.max, 2);
   EXPECT_EQ(pair_of_ranges.range_A.min, 1);
   EXPECT_EQ(pair_of_ranges.range_B.max, 4);
   EXPECT_EQ(pair_of_ranges.range_B.min, 3);
}


/**
 * Tests the read functionality of the next point index. Here the input path is longitudinal. Thus the next longitudinal index
 * shall be returned. \uts{CSCSA-43547} \sdd{SF-7582} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Read_Next_Point_Dep_On_Path_Dir__long_path)
{
   /** \arrange set up longitudinal path such that longitudinal point index is given. */
   uint8_t next_point_index;
   Pt_Point_Indices_Dir_Indep_Obj_T indices;
   path.direction                   = PATH_DIRECTION_LONG_FORWARD;
   indices.next_point_idx_lat_path  = PT_MID_GRID_POINT_INDEX;
   indices.next_point_idx_long_path = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call read function for next point index. */
   next_point_index = Pt_Read_Next_Point_Dep_On_Path_Dir(&indices, &path);

   /** \assert Expect the mapping to be done correctly */
   EXPECT_EQ(next_point_index, indices.next_point_idx_long_path);
}


/**
 * Tests the read functionality of the next point index. Here the input path is lateral. Thus the next lateral index shall be
 * returned. \uts{CSCSA-43548} \sdd{SF-7582} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Read_Next_Point_Dep_On_Path_Dir__lat_path)
{
   /** \arrange set up longitudinal path such that longitudinal point index is given. */
   uint8_t next_point_index;
   Pt_Point_Indices_Dir_Indep_Obj_T indices;
   path.direction                   = PATH_DIRECTION_LAT_RIGHT;
   indices.next_point_idx_lat_path  = PT_MID_GRID_POINT_INDEX;
   indices.next_point_idx_long_path = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call read function for next point index. */
   next_point_index = Pt_Read_Next_Point_Dep_On_Path_Dir(&indices, &path);

   /** \assert Expect the mapping to be done correctly */
   EXPECT_EQ(next_point_index, indices.next_point_idx_lat_path);
}

/**
 * Check whether passed floating values contain the path point default value. Test multiple options to increase branch coverage
 * \uts{CSCSA-43549} \sdd{SF-7321} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Check_If_Intervals_Contain_Default_Path_Points__test_many)
{
   /** \arrange set up valid parameters. */
   uint8_t index;
   const uint8_t result_size = 10u;
   boolean_T result[result_size];
   float32_T half_epsilon = Fbk_Half(EPSILON);

   /** \action Call routine to check for default path point value containment. */
   result[0] = Pt_Check_If_Intervals_Contain_Default_Path_Points(-FBK_ONE_F, -FBK_ONE_F, -FBK_ONE_F, -FBK_ONE_F);
   result[1] = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, FBK_ONE_F, FBK_ONE_F, FBK_ONE_F);
   result[2] = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, FBK_ONE_F, FBK_ONE_F, -half_epsilon);
   result[3] = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, FBK_ONE_F, FBK_ONE_F, half_epsilon);
   result[4] = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, FBK_ONE_F, -half_epsilon, FBK_ONE_F);
   result[5] = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, FBK_ONE_F, half_epsilon, FBK_ONE_F);
   result[6] = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, -half_epsilon, FBK_ONE_F, FBK_ONE_F);
   result[7] = Pt_Check_If_Intervals_Contain_Default_Path_Points(FBK_ONE_F, half_epsilon, FBK_ONE_F, FBK_ONE_F);
   result[8] = Pt_Check_If_Intervals_Contain_Default_Path_Points(-half_epsilon, FBK_ONE_F, FBK_ONE_F, FBK_ONE_F);
   result[9] = Pt_Check_If_Intervals_Contain_Default_Path_Points(half_epsilon, FBK_ONE_F, FBK_ONE_F, FBK_ONE_F);

   /** \assert Expect false */
   EXPECT_FALSE(result[0]);
   EXPECT_FALSE(result[1]);
   for (index = 2; index < result_size; index++)
   {
      EXPECT_TRUE(result[index]);
   }
}

/**
 * Checks whether both paths are longitudinally oriented. Here both orientations are lateral. Thus false is expected
 * \uts{CSCSA-43550} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__both_lateral)
{
   /** \arrange set up pt_persistent.paths with differing orientation. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_LEFT;

   /** \action check whether both input pt_persistent.paths have the same orientation. */
   res = Pt_Are_Paths_Longitudinally_Orientated(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether both paths are longitudinally oriented. Here first directions is longitudal. Thus false is expected
 * \uts{CSCSA-43551} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__first_long)
{
   /** \arrange set up pt_persistent.paths with differing orientation. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_LEFT;

   /** \action check whether both input pt_persistent.paths have the same orientation. */
   res = Pt_Are_Paths_Longitudinally_Orientated(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether both paths have the same direction. Expect false.
 * \uts{CSCSA-43552} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__first_right_second_not_right)
{
   /** \arrange set up both pt_persistent.paths to lateral left. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_LEFT;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether both paths have the same direction. Expect false.
 * \uts{CSCSA-43553} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__first_backward_second_not_backward)
{
   /** \arrange set up both pt_persistent.paths to lateral left. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_LEFT;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether both paths have the same direction. Expect false.
 * \uts{CSCSA-43554} \sdd{SF-7336} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Do_Paths_Have_The_Same_Direction__first_forward_second_not_forward)
{
   /** \arrange set up both pt_persistent.paths to lateral left. */
   boolean_T res;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_FORWARD;
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_LEFT;

   /** \action check whether both input pt_persistent.paths have the same direction. */
   res = Pt_Do_Paths_Have_The_Same_Direction(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert Expect true. */
   EXPECT_FALSE(res);
}

/**
 * Test the non zero path point calculation. Here the input path has all points set. Thus PT_NUM_GRID_POINTS is expected.
 * \uts{CSCSA-43555} \sdd{SF-7340} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Common_Functions_Test, Pt_Get_Number_Of_Non_Zero_Path_Points__path_is_completly_filled_with_vals_negative)
{
   /** \arrange Set up a path filled with values. */
   uint8_t num_non_zero_points;
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      path.path_points[idx] = -1.0f;
   }

   /** \action call calculation of non zero path points. */
   num_non_zero_points = Pt_Get_Number_Of_Non_Zero_Path_Points(&path);

   /** \assert Expect that all points are set. */
   EXPECT_EQ(num_non_zero_points, PT_NUM_GRID_POINTS);
}