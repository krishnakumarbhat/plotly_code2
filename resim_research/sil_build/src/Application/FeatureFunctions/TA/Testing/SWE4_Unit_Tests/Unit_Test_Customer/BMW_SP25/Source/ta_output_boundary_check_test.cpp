
/**
 * @file ta_output_boundary_check_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the range checks for ta_output_boundary_check_test.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "ta_output_boundary_check_test.hpp" // IWYU pragma: keep

#include "gtest/gtest-message.h"   // IWYU pragma: keep
#include "gtest/gtest-test-part.h" // IWYU pragma: keep
extern "C"
{
#include "fbk_macros.h" // IWYU pragma: keep
#include "ml_math.h"    // IWYU pragma: keep
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Are_Outputs_In_Boundary__fta_brake_deceleration_request_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_brake_deceleration_request to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL + TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL)));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Are_Outputs_In_Boundary(p_output);
   /** \assert Expect true since fta_brake_deceleration_request is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Are_Outputs_In_Boundary__fta_brake_deceleration_request_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_brake_deceleration_request to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request = (float32_T) (TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL + EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Are_Outputs_In_Boundary(p_output);
   /** \assert Expect false since fta_brake_deceleration_request is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_deceleration_request_dim_1_lt_lower_boundary)
{
   /** \arrange setup fta_brake_deceleration_request to value less than its minimum boundary.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request = (float32_T) (TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL - EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_brake_deceleration_request is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_deceleration_request_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_brake_deceleration_request to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request = (float32_T) (TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL + EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_brake_deceleration_request is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_deceleration_request_dim_1_eq_lower_boundary)
{
   /** \arrange setup fta_brake_deceleration_request to value equal to its lower boundary.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request = (float32_T) (TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_deceleration_request is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_deceleration_request_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_brake_deceleration_request to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request = (float32_T) (TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_deceleration_request is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_deceleration_request_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_brake_deceleration_request to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL + TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL)));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_deceleration_request is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_target_gap of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_target_gap is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_target_gap_dim_1_lt_lower_boundary)
{
   /** \arrange setup fta_target_gap to value less than its minimum boundary.*/
   boolean_T result;
   p_output->fta_target_gap = (float32_T) (TA_FTA_TARGET_GAP_MIN_VAL - EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_target_gap is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_target_gap of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_target_gap is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_target_gap_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_target_gap to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_target_gap = (float32_T) (TA_FTA_TARGET_GAP_MAX_VAL + EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_target_gap is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_target_gap of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_target_gap is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_target_gap_dim_1_eq_lower_boundary)
{
   /** \arrange setup fta_target_gap to value equal to its lower boundary.*/
   boolean_T result;
   p_output->fta_target_gap = (float32_T) (TA_FTA_TARGET_GAP_MIN_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_target_gap is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_target_gap of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_target_gap is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_target_gap_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_target_gap to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_target_gap = (float32_T) (TA_FTA_TARGET_GAP_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_target_gap is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_target_gap of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_target_gap is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_target_gap_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_target_gap to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_target_gap = (float32_T) ((float32_T) (0.5f * (TA_FTA_TARGET_GAP_MIN_VAL + TA_FTA_TARGET_GAP_MAX_VAL)));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_target_gap is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_ttc of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_ttc is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_ttc_dim_1_lt_lower_boundary)
{
   /** \arrange setup fta_ttc to value less than its minimum boundary.*/
   boolean_T result;
   p_output->fta_ttc = (float32_T) (TA_FTA_TTC_MIN_VAL - EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_ttc is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_ttc of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_ttc is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_ttc_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_ttc to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_ttc = (float32_T) (TA_FTA_TTC_MAX_VAL + EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_ttc is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_ttc of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_ttc is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_ttc_dim_1_eq_lower_boundary)
{
   /** \arrange setup fta_ttc to value equal to its lower boundary.*/
   boolean_T result;
   p_output->fta_ttc = (float32_T) (TA_FTA_TTC_MIN_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_ttc is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_ttc of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_ttc is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_ttc_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_ttc to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_ttc = (float32_T) (TA_FTA_TTC_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_ttc is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_ttc of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_ttc is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_ttc_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_ttc to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_ttc = (float32_T) ((float32_T) (0.5f * (TA_FTA_TTC_MIN_VAL + TA_FTA_TTC_MAX_VAL)));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_ttc is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_alert_level of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_alert_level is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_alert_level_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_alert_level to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_alert_level = (uint8_t) (TA_FTA_ALERT_LEVEL_MAX_VAL + 1u);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_alert_level is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_alert_level of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_alert_level is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_alert_level_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_alert_level to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_alert_level = (uint8_t) (TA_FTA_ALERT_LEVEL_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_alert_level is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_alert_level of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_alert_level is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_alert_level_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_alert_level to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_alert_level = (uint8_t) ((uint8_t) (0.5f * TA_FTA_ALERT_LEVEL_MAX_VAL));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_alert_level is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_symbol_request of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_symbol_request is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_symbol_request_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_symbol_request to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_symbol_request = (uint8_t) (TA_FTA_SYMBOL_REQUEST_MAX_VAL + 1u);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_symbol_request is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_symbol_request of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_symbol_request is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_symbol_request_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_symbol_request to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_symbol_request = (uint8_t) (TA_FTA_SYMBOL_REQUEST_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_symbol_request is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_symbol_request of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_symbol_request is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_symbol_request_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_symbol_request to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_symbol_request = (uint8_t) ((uint8_t) (0.5f * TA_FTA_SYMBOL_REQUEST_MAX_VAL));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_symbol_request is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_threshold_reduction of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_brake_threshold_reduction is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_threshold_reduction_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_brake_threshold_reduction to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_brake_threshold_reduction = (uint8_t) (TA_FTA_BRAKE_THRESHOLD_REDUCTION_MAX_VAL + 1u);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_brake_threshold_reduction is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_brake_threshold_reduction of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_brake_threshold_reduction is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_threshold_reduction_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_brake_threshold_reduction to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_brake_threshold_reduction = (uint8_t) (TA_FTA_BRAKE_THRESHOLD_REDUCTION_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_threshold_reduction is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_threshold_reduction of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_brake_threshold_reduction is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_threshold_reduction_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_brake_threshold_reduction to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_brake_threshold_reduction = (uint8_t) ((uint8_t) (0.5f * TA_FTA_BRAKE_THRESHOLD_REDUCTION_MAX_VAL));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_threshold_reduction is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_conditioning of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_brake_conditioning is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_conditioning_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_brake_conditioning to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_brake_conditioning = (uint8_t) (TA_FTA_BRAKE_CONDITIONING_MAX_VAL + 1u);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_brake_conditioning is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_brake_conditioning of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_brake_conditioning is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_conditioning_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_brake_conditioning to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_brake_conditioning = (uint8_t) (TA_FTA_BRAKE_CONDITIONING_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_conditioning is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_conditioning of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_brake_conditioning is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_brake_conditioning_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_brake_conditioning to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_brake_conditioning = (uint8_t) ((uint8_t) (0.5f * TA_FTA_BRAKE_CONDITIONING_MAX_VAL));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_conditioning is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_maneuver_direction of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_maneuver_direction is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_maneuver_direction_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_maneuver_direction to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_maneuver_direction = (uint8_t) (TA_FTA_MANEUVER_DIRECTION_MAX_VAL + 1u);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_maneuver_direction is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether fta_maneuver_direction of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_maneuver_direction is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_maneuver_direction_dim_1_eq_upper_boundary)
{
   /** \arrange setup fta_maneuver_direction to value equal to its upper boundary.*/
   boolean_T result;
   p_output->fta_maneuver_direction = (uint8_t) (TA_FTA_MANEUVER_DIRECTION_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_maneuver_direction is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_maneuver_direction of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here fta_maneuver_direction is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__fta_maneuver_direction_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_maneuver_direction to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_maneuver_direction = (uint8_t) ((uint8_t) (0.5f * TA_FTA_MANEUVER_DIRECTION_MAX_VAL));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_maneuver_direction is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_diagnostic_mode of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here f_diagnostic_mode is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__f_diagnostic_mode_dim_1_lt_lower_boundary)
{
   /** \arrange setup f_diagnostic_mode to value less than its minimum boundary.*/
   boolean_T result;
   p_output->f_diagnostic_mode = (boolean_T) (TA_F_DIAGNOSTIC_MODE_MIN_VAL - 1u);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since f_diagnostic_mode is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_diagnostic_mode of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here f_diagnostic_mode is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__f_diagnostic_mode_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_diagnostic_mode to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->f_diagnostic_mode = (boolean_T) (TA_F_DIAGNOSTIC_MODE_MAX_VAL + 1u);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since f_diagnostic_mode is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_diagnostic_mode of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here f_diagnostic_mode is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__f_diagnostic_mode_dim_1_eq_lower_boundary)
{
   /** \arrange setup f_diagnostic_mode to value equal to its lower boundary.*/
   boolean_T result;
   p_output->f_diagnostic_mode = (boolean_T) (TA_F_DIAGNOSTIC_MODE_MIN_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_diagnostic_mode is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_diagnostic_mode of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here f_diagnostic_mode is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__f_diagnostic_mode_dim_1_eq_upper_boundary)
{
   /** \arrange setup f_diagnostic_mode to value equal to its upper boundary.*/
   boolean_T result;
   p_output->f_diagnostic_mode = (boolean_T) (TA_F_DIAGNOSTIC_MODE_MAX_VAL);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_diagnostic_mode is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_diagnostic_mode of component Ta_Fta_Output which checks 1-dimensional is within its specified range.
 * Here f_diagnostic_mode is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries__f_diagnostic_mode_dim_1_middle_of_road_test)
{
   /** \arrange setup f_diagnostic_mode to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->f_diagnostic_mode = (boolean_T) ((boolean_T) (0.5f * (TA_F_DIAGNOSTIC_MODE_MIN_VAL + TA_F_DIAGNOSTIC_MODE_MAX_VAL)));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_diagnostic_mode is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether ta_current_deceleration_estimate of component Ta_Output which checks 1-dimensional is within its specified range.
 * Here ta_current_deceleration_estimate is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_1_Dimensional_Are_Output_Within_Boundaries__ta_current_deceleration_estimate_dim_1_lt_lower_boundary)
{
   /** \arrange setup ta_current_deceleration_estimate to value less than its minimum boundary.*/
   boolean_T result;
   p_output->ta_current_deceleration_estimate = (float32_T) (TA_CURRENT_DECELERATION_ESTIMATE_MIN_VAL - EPSILON);
   /** \action Execute Ta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since ta_current_deceleration_estimate is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether ta_current_deceleration_estimate of component Ta_Output which checks 1-dimensional is within its specified range.
 * Here ta_current_deceleration_estimate is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_1_Dimensional_Are_Output_Within_Boundaries__ta_current_deceleration_estimate_dim_1_gt_upper_boundary)
{
   /** \arrange setup ta_current_deceleration_estimate to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->ta_current_deceleration_estimate = (float32_T) (TA_CURRENT_DECELERATION_ESTIMATE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since ta_current_deceleration_estimate is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether ta_current_deceleration_estimate of component Ta_Output which checks 1-dimensional is within its specified range.
 * Here ta_current_deceleration_estimate is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_1_Dimensional_Are_Output_Within_Boundaries__ta_current_deceleration_estimate_dim_1_eq_lower_boundary)
{
   /** \arrange setup ta_current_deceleration_estimate to value equal to its lower boundary.*/
   boolean_T result;
   p_output->ta_current_deceleration_estimate = (float32_T) (TA_CURRENT_DECELERATION_ESTIMATE_MIN_VAL);
   /** \action Execute Ta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since ta_current_deceleration_estimate is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether ta_current_deceleration_estimate of component Ta_Output which checks 1-dimensional is within its specified range.
 * Here ta_current_deceleration_estimate is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_1_Dimensional_Are_Output_Within_Boundaries__ta_current_deceleration_estimate_dim_1_eq_upper_boundary)
{
   /** \arrange setup ta_current_deceleration_estimate to value equal to its upper boundary.*/
   boolean_T result;
   p_output->ta_current_deceleration_estimate = (float32_T) (TA_CURRENT_DECELERATION_ESTIMATE_MAX_VAL);
   /** \action Execute Ta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since ta_current_deceleration_estimate is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether ta_current_deceleration_estimate of component Ta_Output which checks 1-dimensional is within its specified range.
 * Here ta_current_deceleration_estimate is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_1_Dimensional_Are_Output_Within_Boundaries__ta_current_deceleration_estimate_dim_1_middle_of_road_test)
{
   /** \arrange setup ta_current_deceleration_estimate to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->ta_current_deceleration_estimate =
      (float32_T) ((float32_T) (0.5f * (TA_CURRENT_DECELERATION_ESTIMATE_MIN_VAL + TA_CURRENT_DECELERATION_ESTIMATE_MAX_VAL)));
   /** \action Execute Ta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since ta_current_deceleration_estimate is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_fta_enable of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_fta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_fta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_fta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->f_fta_enable = (uint8_t) (TA_F_FTA_ENABLE_MAX_VAL + 1u);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since f_fta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_fta_enable of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_fta_enable is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_fta_enable_dim_1_eq_upper_boundary)
{
   /** \arrange setup f_fta_enable to value equal to its upper boundary.*/
   boolean_T result;
   p_output->f_fta_enable = (uint8_t) (TA_F_FTA_ENABLE_MAX_VAL);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_fta_enable is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_fta_enable of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_fta_enable is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_fta_enable_dim_1_middle_of_road_test)
{
   /** \arrange setup f_fta_enable to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->f_fta_enable = (uint8_t) ((uint8_t) (0.5f * TA_F_FTA_ENABLE_MAX_VAL));
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_fta_enable is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_rta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_rta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->f_rta_enable = (uint8_t) (TA_F_RTA_ENABLE_MAX_VAL + 1u);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since f_rta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_rta_enable of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_rta_enable is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_dim_1_eq_upper_boundary)
{
   /** \arrange setup f_rta_enable to value equal to its upper boundary.*/
   boolean_T result;
   p_output->f_rta_enable = (uint8_t) (TA_F_RTA_ENABLE_MAX_VAL);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_rta_enable is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_rta_enable is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_dim_1_middle_of_road_test)
{
   /** \arrange setup f_rta_enable to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->f_rta_enable = (uint8_t) ((uint8_t) (0.5f * TA_F_RTA_ENABLE_MAX_VAL));
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_rta_enable is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable_turning_area of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here f_rta_enable_turning_area is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_turning_area_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_rta_enable_turning_area to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->f_rta_enable_turning_area = (uint8_t) (TA_F_RTA_ENABLE_TURNING_AREA_MAX_VAL + 1u);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since f_rta_enable_turning_area is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_rta_enable_turning_area of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here f_rta_enable_turning_area is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_turning_area_dim_1_eq_upper_boundary)
{
   /** \arrange setup f_rta_enable_turning_area to value equal to its upper boundary.*/
   boolean_T result;
   p_output->f_rta_enable_turning_area = (uint8_t) (TA_F_RTA_ENABLE_TURNING_AREA_MAX_VAL);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_rta_enable_turning_area is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable_turning_area of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here f_rta_enable_turning_area is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_turning_area_dim_1_middle_of_road_test)
{
   /** \arrange setup f_rta_enable_turning_area to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->f_rta_enable_turning_area = (uint8_t) ((uint8_t) (0.5f * TA_F_RTA_ENABLE_TURNING_AREA_MAX_VAL));
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_rta_enable_turning_area is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable_dynamic_area of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here f_rta_enable_dynamic_area is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_dynamic_area_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_rta_enable_dynamic_area to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->f_rta_enable_dynamic_area = (uint8_t) (TA_F_RTA_ENABLE_DYNAMIC_AREA_MAX_VAL + 1u);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since f_rta_enable_dynamic_area is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_rta_enable_dynamic_area of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here f_rta_enable_dynamic_area is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_dynamic_area_dim_1_eq_upper_boundary)
{
   /** \arrange setup f_rta_enable_dynamic_area to value equal to its upper boundary.*/
   boolean_T result;
   p_output->f_rta_enable_dynamic_area = (uint8_t) (TA_F_RTA_ENABLE_DYNAMIC_AREA_MAX_VAL);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_rta_enable_dynamic_area is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether f_rta_enable_dynamic_area of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here f_rta_enable_dynamic_area is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries__f_rta_enable_dynamic_area_dim_1_middle_of_road_test)
{
   /** \arrange setup f_rta_enable_dynamic_area to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->f_rta_enable_dynamic_area = (uint8_t) ((uint8_t) (0.5f * TA_F_RTA_ENABLE_DYNAMIC_AREA_MAX_VAL));
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since f_rta_enable_dynamic_area is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_dynamic_area_status of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_dynamic_area_status is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_dynamic_area_status_dim_1_gt_upper_boundary)
{
   /** \arrange setup rta_dynamic_area_status to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->rta_dynamic_area_status = (uint8_t) (TA_RTA_DYNAMIC_AREA_STATUS_MAX_VAL + 1u);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since rta_dynamic_area_status is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether rta_dynamic_area_status of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_dynamic_area_status is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_dynamic_area_status_dim_1_eq_upper_boundary)
{
   /** \arrange setup rta_dynamic_area_status to value equal to its upper boundary.*/
   boolean_T result;
   p_output->rta_dynamic_area_status = (uint8_t) (TA_RTA_DYNAMIC_AREA_STATUS_MAX_VAL);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_dynamic_area_status is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_dynamic_area_status of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_dynamic_area_status is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_dynamic_area_status_dim_1_middle_of_road_test)
{
   /** \arrange setup rta_dynamic_area_status to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->rta_dynamic_area_status = (uint8_t) ((uint8_t) (0.5f * TA_RTA_DYNAMIC_AREA_STATUS_MAX_VAL));
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_dynamic_area_status is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_turning_area_status of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_turning_area_status is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_turning_area_status_dim_1_gt_upper_boundary)
{
   /** \arrange setup rta_turning_area_status to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->rta_turning_area_status = (uint8_t) (TA_RTA_TURNING_AREA_STATUS_MAX_VAL + 1u);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since rta_turning_area_status is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether rta_turning_area_status of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_turning_area_status is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_turning_area_status_dim_1_eq_upper_boundary)
{
   /** \arrange setup rta_turning_area_status to value equal to its upper boundary.*/
   boolean_T result;
   p_output->rta_turning_area_status = (uint8_t) (TA_RTA_TURNING_AREA_STATUS_MAX_VAL);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_turning_area_status is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_turning_area_status of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_turning_area_status is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_turning_area_status_dim_1_middle_of_road_test)
{
   /** \arrange setup rta_turning_area_status to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->rta_turning_area_status = (uint8_t) ((uint8_t) (0.5f * TA_RTA_TURNING_AREA_STATUS_MAX_VAL));
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_turning_area_status is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_alert_left of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_alert_left is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_alert_left_dim_1_gt_upper_boundary)
{
   /** \arrange setup rta_alert_left to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->rta_alert_left = (uint8_t) (TA_RTA_ALERT_LEFT_MAX_VAL + 1u);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since rta_alert_left is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether rta_alert_left of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_alert_left is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_alert_left_dim_1_eq_upper_boundary)
{
   /** \arrange setup rta_alert_left to value equal to its upper boundary.*/
   boolean_T result;
   p_output->rta_alert_left = (uint8_t) (TA_RTA_ALERT_LEFT_MAX_VAL);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_alert_left is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_alert_left of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_alert_left is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_alert_left_dim_1_middle_of_road_test)
{
   /** \arrange setup rta_alert_left to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->rta_alert_left = (uint8_t) ((uint8_t) (0.5f * TA_RTA_ALERT_LEFT_MAX_VAL));
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_alert_left is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_alert_right of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_alert_right is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_alert_right_dim_1_gt_upper_boundary)
{
   /** \arrange setup rta_alert_right to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->rta_alert_right = (uint8_t) (TA_RTA_ALERT_RIGHT_MAX_VAL + 1u);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since rta_alert_right is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether rta_alert_right of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_alert_right is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_alert_right_dim_1_eq_upper_boundary)
{
   /** \arrange setup rta_alert_right to value equal to its upper boundary.*/
   boolean_T result;
   p_output->rta_alert_right = (uint8_t) (TA_RTA_ALERT_RIGHT_MAX_VAL);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_alert_right is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether rta_alert_right of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_alert_right is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries__rta_alert_right_dim_1_middle_of_road_test)
{
   /** \arrange setup rta_alert_right to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->rta_alert_right = (uint8_t) ((uint8_t) (0.5f * TA_RTA_ALERT_RIGHT_MAX_VAL));
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since rta_alert_right is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Bmw_Module_Are_Output_Within_Boundaries__fta_brake_deceleration_request_dim_1_middle_of_road_test)
{
   /** \arrange setup fta_brake_deceleration_request to value equal to mid of its boundaries.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request =
      (float32_T) ((float32_T) (0.5f * (TA_FTA_BRAKE_DECELERATION_REQUEST_MIN_VAL + TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL)));
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect true since fta_brake_deceleration_request is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether fta_brake_deceleration_request of component Ta_Fta_Output which checks 1-dimensional is within its specified
 * range. Here fta_brake_deceleration_request is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Bmw_Module_Are_Output_Within_Boundaries__fta_brake_deceleration_request_dim_1_gt_upper_boundary)
{
   /** \arrange setup fta_brake_deceleration_request to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->fta_brake_deceleration_request = (float32_T) (TA_FTA_BRAKE_DECELERATION_REQUEST_MAX_VAL + EPSILON);
   /** \action Execute Ta_Fta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since fta_brake_deceleration_request is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether ta_current_deceleration_estimate of component Ta_Output which checks 1-dimensional is within its specified range.
 * Here ta_current_deceleration_estimate is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test,
       Ta_Output_Bmw_Module_Are_Output_Within_Boundaries__ta_current_deceleration_estimate_dim_1_gt_upper_boundary)
{
   /** \arrange setup ta_current_deceleration_estimate to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->ta_current_deceleration_estimate = (float32_T) (TA_CURRENT_DECELERATION_ESTIMATE_MAX_VAL + EPSILON);
   /** \action Execute Ta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since ta_current_deceleration_estimate is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether f_fta_enable of component Ta_Output_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here f_fta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Output_Bmw_Module_Are_Output_Within_Boundaries__f_fta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup f_fta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->f_fta_enable = (uint8_t) (TA_F_FTA_ENABLE_MAX_VAL + 1u);
   /** \action Execute Ta_Output_Enable_Flags_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since f_fta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether rta_dynamic_area_status of component Ta_Rta_Output which checks 1-dimensional is within its specified range.
 * Here rta_dynamic_area_status is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Output_Boundary_Check_Test, Ta_Output_Bmw_Module_Are_Output_Within_Boundaries__rta_dynamic_area_status_dim_1_gt_upper_boundary)
{
   /** \arrange setup rta_dynamic_area_status to value greater than its maximum boundary.*/
   boolean_T result;
   p_output->rta_dynamic_area_status = (uint8_t) (TA_RTA_DYNAMIC_AREA_STATUS_MAX_VAL + 1u);
   /** \action Execute Ta_Rta_Output_1_Dimensional_Are_Output_Within_Boundaries.*/
   result = Ta_Output_Bmw_Module_Are_Output_Within_Boundaries(p_output);
   /** \assert Expect false since rta_dynamic_area_status is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}
