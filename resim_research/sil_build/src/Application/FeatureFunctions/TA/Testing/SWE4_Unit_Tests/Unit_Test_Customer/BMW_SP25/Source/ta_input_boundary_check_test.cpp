
/**
 * @file ta_input_boundary_check_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the range checks for ta_input_boundary_check_test.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "ta_input_boundary_check_test.hpp" // IWYU pragma: keep

#include "gtest/gtest-message.h"   // IWYU pragma: keep
#include "gtest/gtest-test-part.h" // IWYU pragma: keep
extern "C"
{
#include "fbk_macros.h" // IWYU pragma: keep
#include "ml_math.h"    // IWYU pragma: keep
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test, Ta_Are_Inputs_In_Boundary__fta_obj_offset_x_positive_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_obj_offset_x_positive to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL)));
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Are_Inputs_In_Boundary(p_input);
   /** \assert Expect true since fta_obj_offset_x_positive is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test, Ta_Are_Inputs_In_Boundary__fta_obj_offset_x_positive_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_obj_offset_x_positive to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive = (float32_T) (TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Are_Inputs_In_Boundary(p_input);
   /** \assert Expect false since fta_obj_offset_x_positive is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_positive_dim_1_lt_lower_boundary)
{
   /** \arrange setup fta_obj_offset_x_positive to value less than its minimum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive = (float32_T) (TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL - EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_x_positive is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_positive_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_obj_offset_x_positive to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive = (float32_T) (TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_x_positive is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_positive_dim_1_eq_lower_boundary)
{
   /** \arrange setup fta_obj_offset_x_positive to value equal to its lower boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive = (float32_T) (TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_x_positive is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_positive_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_obj_offset_x_positive to value equal to its upper boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive = (float32_T) (TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_x_positive is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_positive_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_obj_offset_x_positive to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL)));
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_x_positive is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_negative is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_negative_dim_1_lt_lower_boundary)
{
   /** \arrange setup fta_obj_offset_x_negative to value less than its minimum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_negative = (float32_T) (TA_FTA_OBJ_OFFSET_X_NEGATIVE_MIN_VAL - EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_x_negative is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_x_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_negative is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_negative_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_obj_offset_x_negative to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_negative = (float32_T) (TA_FTA_OBJ_OFFSET_X_NEGATIVE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_x_negative is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_x_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_negative is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_negative_dim_1_eq_lower_boundary)
{
   /** \arrange setup fta_obj_offset_x_negative to value equal to its lower boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_negative = (float32_T) (TA_FTA_OBJ_OFFSET_X_NEGATIVE_MIN_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_x_negative is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_negative is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_negative_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_obj_offset_x_negative to value equal to its upper boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_negative = (float32_T) (TA_FTA_OBJ_OFFSET_X_NEGATIVE_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_x_negative is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_negative is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_x_negative_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_obj_offset_x_negative to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_obj_offset_x_negative =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_X_NEGATIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_X_NEGATIVE_MAX_VAL)));
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_x_negative is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_y_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_positive is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_positive_dim_1_lt_lower_boundary)
{
   /** \arrange setup fta_obj_offset_y_positive to value less than its minimum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_positive = (float32_T) (TA_FTA_OBJ_OFFSET_Y_POSITIVE_MIN_VAL - EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_y_positive is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_y_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_positive is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_positive_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_obj_offset_y_positive to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_positive = (float32_T) (TA_FTA_OBJ_OFFSET_Y_POSITIVE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_y_positive is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_y_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_positive is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_positive_dim_1_eq_lower_boundary)
{
   /** \arrange setup fta_obj_offset_y_positive to value equal to its lower boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_positive = (float32_T) (TA_FTA_OBJ_OFFSET_Y_POSITIVE_MIN_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_y_positive is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_y_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_positive is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_positive_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_obj_offset_y_positive to value equal to its upper boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_positive = (float32_T) (TA_FTA_OBJ_OFFSET_Y_POSITIVE_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_y_positive is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_y_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_positive is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_positive_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_obj_offset_y_positive to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_obj_offset_y_positive =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_Y_POSITIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_Y_POSITIVE_MAX_VAL)));
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_y_positive is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_y_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_negative is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_negative_dim_1_lt_lower_boundary)
{
   /** \arrange setup fta_obj_offset_y_negative to value less than its minimum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_negative = (float32_T) (TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MIN_VAL - EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_y_negative is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_y_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_negative is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_negative_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_obj_offset_y_negative to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_negative = (float32_T) (TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_y_negative is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_obj_offset_y_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_negative is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_negative_dim_1_eq_lower_boundary)
{
   /** \arrange setup fta_obj_offset_y_negative to value equal to its lower boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_negative = (float32_T) (TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MIN_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_y_negative is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_y_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_negative is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_negative_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_obj_offset_y_negative to value equal to its upper boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_y_negative = (float32_T) (TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_y_negative is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_y_negative of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_y_negative is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries__fta_obj_offset_y_negative_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_obj_offset_y_negative to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_obj_offset_y_negative =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_Y_NEGATIVE_MAX_VAL)));
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_y_negative is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable of component Ta_Bmw_Input_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_rta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries__f_rta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_rta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->f_rta_enable = (uint8_t) (TA_F_RTA_ENABLE_MAX_VAL + 1u);
   /** \action Execute Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since f_rta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_rta_enable of component Ta_Bmw_Input_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_rta_enable is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries__f_rta_enable_dim_1_eq_upper_boundary)
{
   /** \arrange setup f_rta_enable to value equal to its upper boundary.*/
   boolean_T result;
   p_input->f_rta_enable = (uint8_t) (TA_F_RTA_ENABLE_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since f_rta_enable is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable of component Ta_Bmw_Input_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_rta_enable is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries__f_rta_enable_dim_1_middle_of_road_test)
{
   /** \arrange setup f_rta_enable to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->f_rta_enable = (uint8_t) ((uint8_t) (0.5f * TA_F_RTA_ENABLE_MAX_VAL));
   /** \action Execute Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since f_rta_enable is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_fta_enable of component Ta_Bmw_Input_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_fta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries__f_fta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_fta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->f_fta_enable = (uint8_t) (TA_F_FTA_ENABLE_MAX_VAL + 1u);
   /** \action Execute Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since f_fta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_fta_enable of component Ta_Bmw_Input_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_fta_enable is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries__f_fta_enable_dim_1_eq_upper_boundary)
{
   /** \arrange setup f_fta_enable to value equal to its upper boundary.*/
   boolean_T result;
   p_input->f_fta_enable = (uint8_t) (TA_F_FTA_ENABLE_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since f_fta_enable is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_fta_enable of component Ta_Bmw_Input_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_fta_enable is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries__f_fta_enable_dim_1_middle_of_road_test)
{
   /** \arrange setup f_fta_enable to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->f_fta_enable = (uint8_t) ((uint8_t) (0.5f * TA_F_FTA_ENABLE_MAX_VAL));
   /** \action Execute Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since f_fta_enable is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_steering_angle_max_left of component Ta_Bmw_Input_Steering_Angle_Thresholds which checks 1-dimensional is
 * within its specified range. Here fta_steering_angle_max_left is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries__fta_steering_angle_max_left_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_steering_angle_max_left to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_steering_angle_max_left = (uint8_t) (TA_FTA_STEERING_ANGLE_MAX_LEFT_MAX_VAL + 1u);
   /** \action Execute Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_steering_angle_max_left is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_steering_angle_max_left of component Ta_Bmw_Input_Steering_Angle_Thresholds which checks 1-dimensional is
 * within its specified range. Here fta_steering_angle_max_left is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries__fta_steering_angle_max_left_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_steering_angle_max_left to value equal to its upper boundary.*/
   boolean_T result;
   p_input->fta_steering_angle_max_left = (uint8_t) (TA_FTA_STEERING_ANGLE_MAX_LEFT_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_steering_angle_max_left is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_steering_angle_max_left of component Ta_Bmw_Input_Steering_Angle_Thresholds which checks 1-dimensional is
 * within its specified range. Here fta_steering_angle_max_left is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries__fta_steering_angle_max_left_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_steering_angle_max_left to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_steering_angle_max_left = (uint8_t) ((uint8_t) (0.5f * TA_FTA_STEERING_ANGLE_MAX_LEFT_MAX_VAL));
   /** \action Execute Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_steering_angle_max_left is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_steering_angle_max_right of component Ta_Bmw_Input_Steering_Angle_Thresholds which checks 1-dimensional is
 * within its specified range. Here fta_steering_angle_max_right is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries__fta_steering_angle_max_right_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_steering_angle_max_right to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_steering_angle_max_right = (uint8_t) (TA_FTA_STEERING_ANGLE_MAX_RIGHT_MAX_VAL + 1u);
   /** \action Execute Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_steering_angle_max_right is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_steering_angle_max_right of component Ta_Bmw_Input_Steering_Angle_Thresholds which checks 1-dimensional is
 * within its specified range. Here fta_steering_angle_max_right is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries__fta_steering_angle_max_right_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_steering_angle_max_right to value equal to its upper boundary.*/
   boolean_T result;
   p_input->fta_steering_angle_max_right = (uint8_t) (TA_FTA_STEERING_ANGLE_MAX_RIGHT_MAX_VAL);
   /** \action Execute Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_steering_angle_max_right is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_steering_angle_max_right of component Ta_Bmw_Input_Steering_Angle_Thresholds which checks 1-dimensional is
 * within its specified range. Here fta_steering_angle_max_right is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries__fta_steering_angle_max_right_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_steering_angle_max_right to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_steering_angle_max_right = (uint8_t) ((uint8_t) (0.5f * TA_FTA_STEERING_ANGLE_MAX_RIGHT_MAX_VAL));
   /** \action Execute Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_steering_angle_max_right is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Module_Are_Input_Within_Boundaries__fta_obj_offset_x_positive_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_obj_offset_x_positive to value equal to mid of its boundaries.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_OBJ_OFFSET_X_POSITIVE_MIN_VAL + TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL)));
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Module_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect true since fta_obj_offset_x_positive is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_obj_offset_x_positive of component Ta_Bmw_Input_Debug_Mode which checks 1-dimensional is within its specified
 * range. Here fta_obj_offset_x_positive is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test, Ta_Bmw_Input_Module_Are_Input_Within_Boundaries__fta_obj_offset_x_positive_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_obj_offset_x_positive to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_obj_offset_x_positive = (float32_T) (TA_FTA_OBJ_OFFSET_X_POSITIVE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Bmw_Input_Debug_Mode_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Module_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_obj_offset_x_positive is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_rta_enable of component Ta_Bmw_Input_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_rta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test, Ta_Bmw_Input_Module_Are_Input_Within_Boundaries__f_rta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_rta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->f_rta_enable = (uint8_t) (TA_F_RTA_ENABLE_MAX_VAL + 1u);
   /** \action Execute Ta_Bmw_Input_Enable_Flags_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Module_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since f_rta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_steering_angle_max_left of component Ta_Bmw_Input_Steering_Angle_Thresholds which checks 1-dimensional is
 * within its specified range. Here fta_steering_angle_max_left is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Input_Boundary_Check_Test,
       Ta_Bmw_Input_Module_Are_Input_Within_Boundaries__fta_steering_angle_max_left_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_steering_angle_max_left to value greater than its maximum boundary.*/
   boolean_T result;
   p_input->fta_steering_angle_max_left = (uint8_t) (TA_FTA_STEERING_ANGLE_MAX_LEFT_MAX_VAL + 1u);
   /** \action Execute Ta_Bmw_Input_Steering_Angle_Thresholds_1_Dimensional_Are_Input_Within_Boundaries.*/
   result = Ta_Bmw_Input_Module_Are_Input_Within_Boundaries(p_input);
   /** \assert Expect false since fta_steering_angle_max_left is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}
