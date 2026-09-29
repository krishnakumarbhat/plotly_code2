
/**
 * @file ta_calibration_boundary_check_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the range checks for ta_calibration_boundary_check_test.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "ta_calibration_boundary_check_test.hpp" // IWYU pragma: keep

#include "gtest/gtest-message.h"   // IWYU pragma: keep
#include "gtest/gtest-test-part.h" // IWYU pragma: keep
extern "C"
{
#include "fbk_macros.h" // IWYU pragma: keep
#include "ml_math.h"    // IWYU pragma: keep
}

/**
 * Check whether k_ta_alert_qualifying_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_qualifying_cycles is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Core_Cal_In_Boundary__k_ta_alert_qualifying_cycles_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_qualifying_cycles to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_qualifying_cycles = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES));
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_qualifying_cycles is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_qualifying_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_qualifying_cycles is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Core_Cal_In_Boundary__k_ta_alert_qualifying_cycles_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_qualifying_cycles to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_qualifying_cycles = (uint8_t) (TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES + 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_qualifying_cycles is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Core_Cal_In_Boundary__k_fta_brake_deceleration_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_brake_deceleration_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max = (float32_T) (TA_MAX_K_FTA_BRAKE_DECELERATION_MAX + EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_deceleration_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Core_Cal_In_Boundary__k_tap_lvl_2_host_curvature_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min = (float32_T) (TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN + EPSILON);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_tap_lvl_2_host_curvature_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_qualifying_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_qualifying_cycles is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_qualifying_cycles_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_qualifying_cycles to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_qualifying_cycles = (uint8_t) (TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES + 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_qualifying_cycles is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_qualifying_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_qualifying_cycles is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_qualifying_cycles_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_qualifying_cycles to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_qualifying_cycles = (uint8_t) (TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_qualifying_cycles is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_qualifying_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_qualifying_cycles is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_qualifying_cycles_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_qualifying_cycles to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_qualifying_cycles = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES));
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_qualifying_cycles is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_only_allow_consecutive_ttc_based_alert_levels of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to a value less than its minimum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_consecutive_ttc_based_alert_levels_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_f_only_allow_consecutive_ttc_based_alert_levels to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels =
      (boolean_T) (TA_MIN_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS - 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to value less than its minimum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_only_allow_consecutive_ttc_based_alert_levels of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_consecutive_ttc_based_alert_levels_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_f_only_allow_consecutive_ttc_based_alert_levels to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels =
      (boolean_T) (TA_MAX_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS + 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to value greater than its maximum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_only_allow_consecutive_ttc_based_alert_levels of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to a value equal to its lower
 * boundary. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_consecutive_ttc_based_alert_levels_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_f_only_allow_consecutive_ttc_based_alert_levels to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels =
      (boolean_T) (TA_MIN_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_only_allow_consecutive_ttc_based_alert_levels of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to a value equal to its upper
 * boundary. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_consecutive_ttc_based_alert_levels_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_f_only_allow_consecutive_ttc_based_alert_levels to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels =
      (boolean_T) (TA_MAX_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_only_allow_consecutive_ttc_based_alert_levels of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to a value equal to mid of its
 * boundaries. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_consecutive_ttc_based_alert_levels_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_f_only_allow_consecutive_ttc_based_alert_levels to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels =
      (boolean_T) ((boolean_T) (0.5f
                                * (TA_MIN_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS
                                   + TA_MAX_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS)));
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_only_allow_consecutive_ttc_based_alert_levels is set to value equal to mid of its
    * boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_skip_holding_for_single_alert_level_drop of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_skip_holding_for_single_alert_level_drop is set to a value less than its minimum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_skip_holding_for_single_alert_level_drop_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_f_skip_holding_for_single_alert_level_drop to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_f_skip_holding_for_single_alert_level_drop =
      (boolean_T) (TA_MIN_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP - 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_skip_holding_for_single_alert_level_drop is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_skip_holding_for_single_alert_level_drop of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_skip_holding_for_single_alert_level_drop is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_skip_holding_for_single_alert_level_drop_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_f_skip_holding_for_single_alert_level_drop to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_f_skip_holding_for_single_alert_level_drop =
      (boolean_T) (TA_MAX_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP + 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_skip_holding_for_single_alert_level_drop is set to value greater than its maximum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_skip_holding_for_single_alert_level_drop of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_skip_holding_for_single_alert_level_drop is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_skip_holding_for_single_alert_level_drop_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_f_skip_holding_for_single_alert_level_drop to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_f_skip_holding_for_single_alert_level_drop = (boolean_T) (TA_MIN_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_skip_holding_for_single_alert_level_drop is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_skip_holding_for_single_alert_level_drop of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_skip_holding_for_single_alert_level_drop is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_skip_holding_for_single_alert_level_drop_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_f_skip_holding_for_single_alert_level_drop to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_f_skip_holding_for_single_alert_level_drop = (boolean_T) (TA_MAX_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_skip_holding_for_single_alert_level_drop is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_skip_holding_for_single_alert_level_drop of component Ta_Alert_Debouncer which checks 1-dimensional is
 * within its specified range. Here k_ta_f_skip_holding_for_single_alert_level_drop is set to a value equal to mid of its
 * boundaries. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_skip_holding_for_single_alert_level_drop_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_f_skip_holding_for_single_alert_level_drop to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_f_skip_holding_for_single_alert_level_drop =
      (boolean_T) ((boolean_T) (0.5f
                                * (TA_MIN_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP
                                   + TA_MAX_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP)));
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_skip_holding_for_single_alert_level_drop is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_holding_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_holding_cycles is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_holding_cycles_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_holding_cycles to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_holding_cycles = (uint8_t) (TA_MAX_K_TA_ALERT_HOLDING_CYCLES + 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_holding_cycles is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_holding_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_holding_cycles is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_holding_cycles_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_holding_cycles to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_holding_cycles = (uint8_t) (TA_MAX_K_TA_ALERT_HOLDING_CYCLES);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_holding_cycles is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_holding_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_holding_cycles is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_holding_cycles_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_holding_cycles to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_holding_cycles = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_TA_ALERT_HOLDING_CYCLES));
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_holding_cycles is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_always_overwrite_ta_mode_to_both of component Ta_Enable_Flags which checks 1-dimensional is within its
 * specified range. Here k_ta_always_overwrite_ta_mode_to_both is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_always_overwrite_ta_mode_to_both_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_always_overwrite_ta_mode_to_both to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_always_overwrite_ta_mode_to_both = (boolean_T) (TA_MIN_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH - 1u);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_always_overwrite_ta_mode_to_both is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_always_overwrite_ta_mode_to_both of component Ta_Enable_Flags which checks 1-dimensional is within its
 * specified range. Here k_ta_always_overwrite_ta_mode_to_both is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_always_overwrite_ta_mode_to_both_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_always_overwrite_ta_mode_to_both to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_always_overwrite_ta_mode_to_both = (boolean_T) (TA_MAX_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH + 1u);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_always_overwrite_ta_mode_to_both is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_always_overwrite_ta_mode_to_both of component Ta_Enable_Flags which checks 1-dimensional is within its
 * specified range. Here k_ta_always_overwrite_ta_mode_to_both is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_always_overwrite_ta_mode_to_both_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_always_overwrite_ta_mode_to_both to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_always_overwrite_ta_mode_to_both = (boolean_T) (TA_MIN_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_always_overwrite_ta_mode_to_both is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_always_overwrite_ta_mode_to_both of component Ta_Enable_Flags which checks 1-dimensional is within its
 * specified range. Here k_ta_always_overwrite_ta_mode_to_both is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_always_overwrite_ta_mode_to_both_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_always_overwrite_ta_mode_to_both to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_always_overwrite_ta_mode_to_both = (boolean_T) (TA_MAX_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_always_overwrite_ta_mode_to_both is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_always_overwrite_ta_mode_to_both of component Ta_Enable_Flags which checks 1-dimensional is within its
 * specified range. Here k_ta_always_overwrite_ta_mode_to_both is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_always_overwrite_ta_mode_to_both_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_always_overwrite_ta_mode_to_both to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_always_overwrite_ta_mode_to_both = (boolean_T) ((
      boolean_T) (0.5f * (TA_MIN_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH + TA_MAX_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH)));
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_always_overwrite_ta_mode_to_both is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_ta_enable_debug_mode of component Ta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_ta_enable_debug_mode is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_ta_enable_debug_mode_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_f_ta_enable_debug_mode to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_f_ta_enable_debug_mode = (boolean_T) (TA_MIN_K_F_TA_ENABLE_DEBUG_MODE - 1u);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_ta_enable_debug_mode is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_ta_enable_debug_mode of component Ta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_ta_enable_debug_mode is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_ta_enable_debug_mode_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_ta_enable_debug_mode to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_ta_enable_debug_mode = (boolean_T) (TA_MAX_K_F_TA_ENABLE_DEBUG_MODE + 1u);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_ta_enable_debug_mode is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_ta_enable_debug_mode of component Ta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_ta_enable_debug_mode is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_ta_enable_debug_mode_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_f_ta_enable_debug_mode to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_f_ta_enable_debug_mode = (boolean_T) (TA_MIN_K_F_TA_ENABLE_DEBUG_MODE);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_ta_enable_debug_mode is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_ta_enable_debug_mode of component Ta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_ta_enable_debug_mode is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_ta_enable_debug_mode_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_f_ta_enable_debug_mode to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_f_ta_enable_debug_mode = (boolean_T) (TA_MAX_K_F_TA_ENABLE_DEBUG_MODE);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_ta_enable_debug_mode is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_ta_enable_debug_mode of component Ta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_ta_enable_debug_mode is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_ta_enable_debug_mode_dim_1_middle_of_road_test)
{
   /** \arrange setup k_f_ta_enable_debug_mode to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_f_ta_enable_debug_mode =
      (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_F_TA_ENABLE_DEBUG_MODE + TA_MAX_K_F_TA_ENABLE_DEBUG_MODE)));
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_ta_enable_debug_mode is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_check_ego_circles of component Ta_Enable_Flags which checks 3-dimensional is within its
 * specified range. Here k_ta_critical_approach_check_ego_circles is set to a value greater than its maximum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_check_ego_circles_dim_3_gt_upper_boundary)
{
   /** \arrange setup k_ta_critical_approach_check_ego_circles to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_check_ego_circles[0] = (uint8_t) (TA_MAX_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES + 1u);
   /** \action Execute Ta_Enable_Flags_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_critical_approach_check_ego_circles is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_critical_approach_check_ego_circles of component Ta_Enable_Flags which checks 3-dimensional is within its
 * specified range. Here k_ta_critical_approach_check_ego_circles is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_check_ego_circles_dim_3_eq_upper_boundary)
{
   /** \arrange setup k_ta_critical_approach_check_ego_circles to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_check_ego_circles[0] = (uint8_t) (TA_MAX_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES);
   /** \action Execute Ta_Enable_Flags_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_check_ego_circles is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_check_ego_circles of component Ta_Enable_Flags which checks 3-dimensional is within its
 * specified range. Here k_ta_critical_approach_check_ego_circles is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Enable_Flags_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_check_ego_circles_dim_3_middle_of_road_test)
{
   /** \arrange setup k_ta_critical_approach_check_ego_circles to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_critical_approach_check_ego_circles[0] =
      (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES));
   /** \action Execute Ta_Enable_Flags_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_check_ego_circles is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_deceleration_max_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_fta_brake_deceleration_max to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max = (float32_T) (TA_MIN_K_FTA_BRAKE_DECELERATION_MAX - EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_deceleration_max is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_deceleration_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_brake_deceleration_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max = (float32_T) (TA_MAX_K_FTA_BRAKE_DECELERATION_MAX + EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_deceleration_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_deceleration_max_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_fta_brake_deceleration_max to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max = (float32_T) (TA_MIN_K_FTA_BRAKE_DECELERATION_MAX);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_deceleration_max is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_deceleration_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_fta_brake_deceleration_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max = (float32_T) (TA_MAX_K_FTA_BRAKE_DECELERATION_MAX);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_deceleration_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_deceleration_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_brake_deceleration_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_BRAKE_DECELERATION_MAX + TA_MAX_K_FTA_BRAKE_DECELERATION_MAX)));
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_deceleration_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_dead_time of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_dead_time is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_dead_time_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_fta_brake_dead_time to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_dead_time = (float32_T) (TA_MIN_K_FTA_BRAKE_DEAD_TIME - EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_dead_time is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_dead_time of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_dead_time is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_dead_time_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_brake_dead_time to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_dead_time = (float32_T) (TA_MAX_K_FTA_BRAKE_DEAD_TIME + EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_dead_time is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_dead_time of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_dead_time is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_dead_time_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_fta_brake_dead_time to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_brake_dead_time = (float32_T) (TA_MIN_K_FTA_BRAKE_DEAD_TIME);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_dead_time is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_dead_time of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_dead_time is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_dead_time_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_fta_brake_dead_time to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_brake_dead_time = (float32_T) (TA_MAX_K_FTA_BRAKE_DEAD_TIME);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_dead_time is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_dead_time of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_dead_time is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_dead_time_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_brake_dead_time to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_brake_dead_time =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_BRAKE_DEAD_TIME + TA_MAX_K_FTA_BRAKE_DEAD_TIME)));
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_dead_time is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable_brake_gradient_logic of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its
 * specified range. Here k_f_fta_enable_brake_gradient_logic is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_brake_gradient_logic_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_f_fta_enable_brake_gradient_logic to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_brake_gradient_logic = (boolean_T) (TA_MIN_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC - 1u);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_fta_enable_brake_gradient_logic is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_fta_enable_brake_gradient_logic of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its
 * specified range. Here k_f_fta_enable_brake_gradient_logic is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_brake_gradient_logic_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_fta_enable_brake_gradient_logic to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_brake_gradient_logic = (boolean_T) (TA_MAX_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC + 1u);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_fta_enable_brake_gradient_logic is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_fta_enable_brake_gradient_logic of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its
 * specified range. Here k_f_fta_enable_brake_gradient_logic is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_brake_gradient_logic_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_f_fta_enable_brake_gradient_logic to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_brake_gradient_logic = (boolean_T) (TA_MIN_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable_brake_gradient_logic is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable_brake_gradient_logic of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its
 * specified range. Here k_f_fta_enable_brake_gradient_logic is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_brake_gradient_logic_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_f_fta_enable_brake_gradient_logic to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_brake_gradient_logic = (boolean_T) (TA_MAX_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable_brake_gradient_logic is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable_brake_gradient_logic of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its
 * specified range. Here k_f_fta_enable_brake_gradient_logic is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_brake_gradient_logic_dim_1_middle_of_road_test)
{
   /** \arrange setup k_f_fta_enable_brake_gradient_logic to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_f_fta_enable_brake_gradient_logic =
      (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC + TA_MAX_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC)));
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable_brake_gradient_logic is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_gradient of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_gradient is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_gradient_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_fta_brake_gradient to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_gradient = (float32_T) (TA_MIN_K_FTA_BRAKE_GRADIENT - EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_gradient is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_gradient of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_gradient is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_gradient_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_brake_gradient to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_gradient = (float32_T) (TA_MAX_K_FTA_BRAKE_GRADIENT + EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_gradient is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_gradient of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_gradient is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_gradient_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_fta_brake_gradient to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_brake_gradient = (float32_T) (TA_MIN_K_FTA_BRAKE_GRADIENT);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_gradient is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_gradient of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_gradient is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_gradient_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_fta_brake_gradient to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_brake_gradient = (float32_T) (TA_MAX_K_FTA_BRAKE_GRADIENT);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_gradient is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_gradient of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified range.
 * Here k_fta_brake_gradient is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_brake_gradient_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_brake_gradient to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_brake_gradient = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_BRAKE_GRADIENT + TA_MAX_K_FTA_BRAKE_GRADIENT)));
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_gradient is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_fta_enable is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_f_fta_enable to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable = (boolean_T) (TA_MIN_K_F_FTA_ENABLE - 1u);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_fta_enable is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_fta_enable of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_fta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_fta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable = (boolean_T) (TA_MAX_K_F_FTA_ENABLE + 1u);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_fta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_fta_enable of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_fta_enable is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_f_fta_enable to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable = (boolean_T) (TA_MIN_K_F_FTA_ENABLE);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_fta_enable is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_f_fta_enable to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable = (boolean_T) (TA_MAX_K_F_FTA_ENABLE);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_fta_enable is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_dim_1_middle_of_road_test)
{
   /** \arrange setup k_f_fta_enable to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_f_fta_enable = (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_F_FTA_ENABLE + TA_MAX_K_F_FTA_ENABLE)));
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable_danger_zones of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_fta_enable_danger_zones is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_danger_zones_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_f_fta_enable_danger_zones to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_danger_zones = (boolean_T) (TA_MIN_K_F_FTA_ENABLE_DANGER_ZONES - 1u);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_fta_enable_danger_zones is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_fta_enable_danger_zones of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_fta_enable_danger_zones is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_danger_zones_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_fta_enable_danger_zones to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_danger_zones = (boolean_T) (TA_MAX_K_F_FTA_ENABLE_DANGER_ZONES + 1u);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_fta_enable_danger_zones is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_fta_enable_danger_zones of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_fta_enable_danger_zones is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_danger_zones_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_f_fta_enable_danger_zones to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_danger_zones = (boolean_T) (TA_MIN_K_F_FTA_ENABLE_DANGER_ZONES);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable_danger_zones is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable_danger_zones of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_fta_enable_danger_zones is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_danger_zones_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_f_fta_enable_danger_zones to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable_danger_zones = (boolean_T) (TA_MAX_K_F_FTA_ENABLE_DANGER_ZONES);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable_danger_zones is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_fta_enable_danger_zones of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_fta_enable_danger_zones is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_fta_enable_danger_zones_dim_1_middle_of_road_test)
{
   /** \arrange setup k_f_fta_enable_danger_zones to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_f_fta_enable_danger_zones =
      (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_F_FTA_ENABLE_DANGER_ZONES + TA_MAX_K_F_FTA_ENABLE_DANGER_ZONES)));
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_fta_enable_danger_zones is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_pos_straight_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_pos_straight_min is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_pos_straight_min_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_pos_straight_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_pos_straight_min = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_pos_straight_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_pos_straight_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_pos_straight_min is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_pos_straight_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_pos_straight_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_pos_straight_min = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_pos_straight_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_pos_straight_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_pos_straight_min is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_pos_straight_min_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_pos_straight_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_pos_straight_min = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN);
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_pos_straight_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_pos_straight_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_pos_straight_min is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_pos_straight_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_pos_straight_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_pos_straight_min = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN);
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_pos_straight_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_pos_straight_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_pos_straight_min is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_pos_straight_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_long_pos_straight_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_pos_straight_min =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN + TA_MAX_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN)));
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_pos_straight_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_age_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its specified range.
 * Here k_fta_obj_age_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_age_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_age_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_age_min = (uint8_t) (TA_MAX_K_FTA_OBJ_AGE_MIN + 1u);
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_age_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_age_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its specified range.
 * Here k_fta_obj_age_min is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_age_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_age_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_age_min = (uint8_t) (TA_MAX_K_FTA_OBJ_AGE_MIN);
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_age_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_age_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its specified range.
 * Here k_fta_obj_age_min is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_age_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_age_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_age_min = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_FTA_OBJ_AGE_MIN));
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_age_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_exist_prblty is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty[0] = (float32_T) (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_exist_prblty is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_exist_prblty is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty[0] = (float32_T) (TA_MAX_K_FTA_OBJ_EXIST_PRBLTY + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_exist_prblty is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_exist_prblty is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty[0] = (float32_T) (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_exist_prblty is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_exist_prblty is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty[0] = (float32_T) (TA_MAX_K_FTA_OBJ_EXIST_PRBLTY);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_exist_prblty is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_exist_prblty is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_exist_prblty to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY + TA_MAX_K_FTA_OBJ_EXIST_PRBLTY)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_exist_prblty is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_heading_straight is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_straight_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_straight to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_straight[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_STRAIGHT - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_straight is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_heading_straight is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_straight_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_straight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_straight[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_STRAIGHT + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_straight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_heading_straight is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_straight_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_straight to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_straight[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_STRAIGHT);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_straight is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_heading_straight is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_straight_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_straight to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_straight[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_STRAIGHT);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_straight is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_heading_straight is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_straight_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_heading_straight to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_heading_straight[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_STRAIGHT + TA_MAX_K_FTA_OBJ_HEADING_STRAIGHT)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_straight is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_speed is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_speed to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed[0] = (float32_T) (TA_MIN_K_FTA_OBJ_SPEED - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_speed is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_speed of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_speed is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_speed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed[0] = (float32_T) (TA_MAX_K_FTA_OBJ_SPEED + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_speed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_speed of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_speed is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_speed to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed[0] = (float32_T) (TA_MIN_K_FTA_OBJ_SPEED);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_speed is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_speed to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed[0] = (float32_T) (TA_MAX_K_FTA_OBJ_SPEED);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_speed is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_speed to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_speed[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_SPEED + TA_MAX_K_FTA_OBJ_SPEED)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_straight is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_straight_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_speed_straight to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_straight[0] = (float32_T) (TA_MIN_K_FTA_OBJ_SPEED_STRAIGHT - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_speed_straight is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_speed_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_straight is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_straight_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_speed_straight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_straight[0] = (float32_T) (TA_MAX_K_FTA_OBJ_SPEED_STRAIGHT + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_speed_straight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_speed_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_straight is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_straight_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_speed_straight to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_straight[0] = (float32_T) (TA_MIN_K_FTA_OBJ_SPEED_STRAIGHT);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed_straight is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_straight is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_straight_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_speed_straight to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_straight[0] = (float32_T) (TA_MAX_K_FTA_OBJ_SPEED_STRAIGHT);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed_straight is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed_straight of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_straight is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_straight_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_speed_straight to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_speed_straight[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_SPEED_STRAIGHT + TA_MAX_K_FTA_OBJ_SPEED_STRAIGHT)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed_straight is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_length of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_length is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_length to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length[0] = (float32_T) (TA_MIN_K_FTA_OBJ_LENGTH - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_length is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_length of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_length is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_length to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length[0] = (float32_T) (TA_MAX_K_FTA_OBJ_LENGTH + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_length is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_length of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_length is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_length to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length[0] = (float32_T) (TA_MIN_K_FTA_OBJ_LENGTH);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_length is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_length of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_length is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_length to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length[0] = (float32_T) (TA_MAX_K_FTA_OBJ_LENGTH);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_length is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_length of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_length is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_length to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_length[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_LENGTH + TA_MAX_K_FTA_OBJ_LENGTH)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_length is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_width of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_width is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_width to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width[0] = (float32_T) (TA_MIN_K_FTA_OBJ_WIDTH - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_width is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_width of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_width is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_width to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width[0] = (float32_T) (TA_MAX_K_FTA_OBJ_WIDTH + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_width is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_width of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_width is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_width to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width[0] = (float32_T) (TA_MIN_K_FTA_OBJ_WIDTH);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_width is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_width of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_width is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_width to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width[0] = (float32_T) (TA_MAX_K_FTA_OBJ_WIDTH);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_width is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_width of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_width is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_width to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_width[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_WIDTH + TA_MAX_K_FTA_OBJ_WIDTH)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_width is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_area of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_area is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_area to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area[0] = (float32_T) (TA_MIN_K_FTA_OBJ_AREA - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_area is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_area of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_area is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_area to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area[0] = (float32_T) (TA_MAX_K_FTA_OBJ_AREA + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_area is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_area of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_area is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_area to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area[0] = (float32_T) (TA_MIN_K_FTA_OBJ_AREA);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_area is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_area of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_area is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_area to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area[0] = (float32_T) (TA_MAX_K_FTA_OBJ_AREA);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_area is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_area of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_area is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_area to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_area[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_AREA + TA_MAX_K_FTA_OBJ_AREA)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_area is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_vru_class_prob is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vru_class_prob is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_vru_class_prob is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vru_class_prob is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_vru_class_prob is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vru_class_prob is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_vru_class_prob is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vru_class_prob is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_vru_class_prob is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vru_class_prob to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB + TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vru_class_prob is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_ego_obj_heading_diff is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff[0] = (float32_T) (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_ego_obj_heading_diff is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_ego_obj_heading_diff is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff[0] = (float32_T) (TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_ego_obj_heading_diff is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_ego_obj_heading_diff is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff[0] = (float32_T) (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_ego_obj_heading_diff is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_ego_obj_heading_diff is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff[0] = (float32_T) (TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_ego_obj_heading_diff is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_ego_obj_heading_diff is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_ego_obj_heading_diff to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF + TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_ego_obj_heading_diff is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_eclipse_value is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value[0] = (float32_T) (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_eclipse_value is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_eclipse_value is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value[0] = (float32_T) (TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_eclipse_value is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_eclipse_value is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value[0] = (float32_T) (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_eclipse_value is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_eclipse_value is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value[0] = (float32_T) (TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_eclipse_value is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_eclipse_value is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_eclipse_value to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE + TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE)));
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_eclipse_value is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_velocity_heading_diff_max of component Ta_Fta_Object_Filter_Offsets which checks 1-dimensional is within
 * its specified range. Here k_fta_obj_velocity_heading_diff_max is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_velocity_heading_diff_max_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_velocity_heading_diff_max to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_velocity_heading_diff_max = (float32_T) (TA_MIN_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_velocity_heading_diff_max is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_velocity_heading_diff_max of component Ta_Fta_Object_Filter_Offsets which checks 1-dimensional is within
 * its specified range. Here k_fta_obj_velocity_heading_diff_max is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_velocity_heading_diff_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_velocity_heading_diff_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_velocity_heading_diff_max = (float32_T) (TA_MAX_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_velocity_heading_diff_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_velocity_heading_diff_max of component Ta_Fta_Object_Filter_Offsets which checks 1-dimensional is within
 * its specified range. Here k_fta_obj_velocity_heading_diff_max is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_velocity_heading_diff_max_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_velocity_heading_diff_max to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_velocity_heading_diff_max = (float32_T) (TA_MIN_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_velocity_heading_diff_max is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_velocity_heading_diff_max of component Ta_Fta_Object_Filter_Offsets which checks 1-dimensional is within
 * its specified range. Here k_fta_obj_velocity_heading_diff_max is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_velocity_heading_diff_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_velocity_heading_diff_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_velocity_heading_diff_max = (float32_T) (TA_MAX_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_velocity_heading_diff_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_velocity_heading_diff_max of component Ta_Fta_Object_Filter_Offsets which checks 1-dimensional is within
 * its specified range. Here k_fta_obj_velocity_heading_diff_max is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_velocity_heading_diff_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_velocity_heading_diff_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_velocity_heading_diff_max =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX + TA_MAX_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_velocity_heading_diff_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_exist_prblty_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_exist_prblty_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_exist_prblty_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_EXIST_PRBLTY_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_exist_prblty_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_exist_prblty_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_exist_prblty_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_exist_prblty_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_EXIST_PRBLTY_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_exist_prblty_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_exist_prblty_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_exist_prblty_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY_OFST + TA_MAX_K_FTA_OBJ_EXIST_PRBLTY_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_exist_prblty_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_speed_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_SPEED_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_speed_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_speed_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_speed_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_SPEED_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_speed_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_speed_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_speed_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_SPEED_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_speed_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_speed_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_SPEED_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_speed_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_speed_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_speed_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_speed_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_speed_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_SPEED_OFST + TA_MAX_K_FTA_OBJ_SPEED_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_speed_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_length_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_length_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_length_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_LENGTH_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_length_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_length_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_length_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_length_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_LENGTH_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_length_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_length_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_length_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_length_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_LENGTH_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_length_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_length_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_length_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_length_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_length_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_LENGTH_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_length_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_length_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_length_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_length_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_length_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_length_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_LENGTH_OFST + TA_MAX_K_FTA_OBJ_LENGTH_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_length_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_width_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_width_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_width_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_WIDTH_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_width_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_width_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_width_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_width_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_WIDTH_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_width_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_width_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_width_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_width_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_WIDTH_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_width_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_width_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_width_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_width_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_width_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_WIDTH_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_width_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_width_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_width_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_width_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_width_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_width_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_WIDTH_OFST + TA_MAX_K_FTA_OBJ_WIDTH_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_width_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_area_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_area_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_area_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_AREA_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_area_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_area_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_area_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_area_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_AREA_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_area_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_area_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_area_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_area_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_AREA_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_area_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_area_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_area_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_area_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_area_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_AREA_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_area_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_area_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_fta_obj_area_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_area_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_area_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_area_ofst[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_AREA_OFST + TA_MAX_K_FTA_OBJ_AREA_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_area_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vru_class_prob_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vru_class_prob_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vru_class_prob_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vru_class_prob_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vru_class_prob_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vru_class_prob_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vru_class_prob_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vru_class_prob_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vru_class_prob_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vru_class_prob_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vru_class_prob_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vru_class_prob_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vru_class_prob_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vru_class_prob_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB_OFST + TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vru_class_prob_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_ego_obj_heading_diff_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff_ofst[0] = (float32_T) (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_ego_obj_heading_diff_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_ego_obj_heading_diff_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff_ofst[0] = (float32_T) (TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_ego_obj_heading_diff_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_ego_obj_heading_diff_ofst is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff_ofst[0] = (float32_T) (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_ego_obj_heading_diff_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_ego_obj_heading_diff_ofst is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_ego_obj_heading_diff_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff_ofst[0] = (float32_T) (TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_ego_obj_heading_diff_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_ego_obj_heading_diff_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_ego_obj_heading_diff_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_ego_obj_heading_diff_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_ego_obj_heading_diff_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_ego_obj_heading_diff_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF_OFST + TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_ego_obj_heading_diff_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_eclipse_value_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_eclipse_value_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_eclipse_value_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_eclipse_value_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_eclipse_value_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_eclipse_value_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_eclipse_value_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_eclipse_value_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_eclipse_value_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_eclipse_value_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_eclipse_value_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_eclipse_value_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_eclipse_value_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_eclipse_value_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE_OFST + TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_eclipse_value_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_fta_obj_vcs_long_vel_rel_ofst is set to a value less than its minimum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_rel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_fta_obj_vcs_long_vel_rel_ofst is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_rel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_fta_obj_vcs_long_vel_rel_ofst is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_rel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_fta_obj_vcs_long_vel_rel_ofst is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_rel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_fta_obj_vcs_long_vel_rel_ofst is set to a value equal to mid of its
 * boundaries. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_rel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_fta_obj_vcs_lat_vel_rel_ofst is set to a value less than its minimum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel_rel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_fta_obj_vcs_lat_vel_rel_ofst is set to a value greater than its maximum boundary. Thus
 * false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel_rel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_fta_obj_vcs_lat_vel_rel_ofst is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_rel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_fta_obj_vcs_lat_vel_rel_ofst is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_rel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_fta_obj_vcs_lat_vel_rel_ofst is set to a value equal to mid of its boundaries. Thus true
 * is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_rel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_long_vel_ofst is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_long_vel_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_long_vel_ofst is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_long_vel_ofst is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_long_vel_ofst is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_OFST + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_lat_vel_ofst is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_lat_vel_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_lat_vel_ofst is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_lat_vel_ofst is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_vcs_lat_vel_ofst is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_OFST + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_ofst is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_ofst is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_ofst is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_heading_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_heading_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_OFST + TA_MAX_K_FTA_OBJ_HEADING_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_long_vel_rel is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_rel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_long_vel_rel is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_rel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_long_vel_rel is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_rel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_long_vel_rel is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_rel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_long_vel_rel is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel_rel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_lat_vel_rel is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel_rel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_lat_vel_rel is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel_rel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_lat_vel_rel is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_rel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_lat_vel_rel is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_rel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_lat_vel_rel is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_rel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel_rel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel_rel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel_rel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_vel is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_vel is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_vel is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_vel is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_vel is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_long_vel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_long_vel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_lat_vel is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_lat_vel is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_lat_vel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_lat_vel is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel[0] = (float32_T) (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_lat_vel is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LAT_VEL);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_vcs_lat_vel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_vcs_lat_vel is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_lat_vel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_vcs_lat_vel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_lat_vel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_vcs_lat_vel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_heading to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_heading[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING + TA_MAX_K_FTA_OBJ_HEADING)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading_rate is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_RATE - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_rate is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_rate of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading_rate is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_RATE + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_rate is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_rate of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading_rate is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_RATE);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading_rate is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_RATE);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_heading_rate is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_heading_rate to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_RATE + TA_MAX_K_FTA_OBJ_HEADING_RATE)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_straight of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_rate_straight is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_straight_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_straight to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_straight[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_RATE_STRAIGHT - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_rate_straight is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_straight of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_rate_straight is set to a value greater than its maximum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_straight_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_straight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_straight[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_RATE_STRAIGHT + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_rate_straight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_straight of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_rate_straight is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_straight_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_straight to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_straight[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_RATE_STRAIGHT);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate_straight is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_straight of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_rate_straight is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_straight_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_straight to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_straight[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_RATE_STRAIGHT);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate_straight is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_straight of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_fta_obj_heading_rate_straight is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_straight_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_heading_rate_straight to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_straight[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_RATE_STRAIGHT + TA_MAX_K_FTA_OBJ_HEADING_RATE_STRAIGHT)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate_straight is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_ofst of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_heading_rate_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_RATE_OFST - EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_rate_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_ofst of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_heading_rate_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_RATE_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_heading_rate_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_ofst of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_heading_rate_ofst is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_ofst[0] = (float32_T) (TA_MIN_K_FTA_OBJ_HEADING_RATE_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_ofst of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_heading_rate_ofst is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_fta_obj_heading_rate_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_HEADING_RATE_OFST);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_obj_heading_rate_ofst of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_heading_rate_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_fta_obj_heading_rate_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_fta_obj_heading_rate_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_obj_heading_rate_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_RATE_OFST + TA_MAX_K_FTA_OBJ_HEADING_RATE_OFST)));
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_obj_heading_rate_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_point_size of component Ta_Fta_Zones which checks 1-dimensional is within its specified range.
 * Here k_fta_danger_zone_point_size is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_point_size_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_point_size to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_point_size = (uint8_t) (TA_MAX_K_FTA_DANGER_ZONE_POINT_SIZE + 1u);
   /** \action Execute Ta_Fta_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_point_size is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_point_size of component Ta_Fta_Zones which checks 1-dimensional is within its specified range.
 * Here k_fta_danger_zone_point_size is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_point_size_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_point_size to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_point_size = (uint8_t) (TA_MAX_K_FTA_DANGER_ZONE_POINT_SIZE);
   /** \action Execute Ta_Fta_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_point_size is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_point_size of component Ta_Fta_Zones which checks 1-dimensional is within its specified range.
 * Here k_fta_danger_zone_point_size is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_1_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_point_size_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_danger_zone_point_size to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_danger_zone_point_size = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_FTA_DANGER_ZONE_POINT_SIZE));
   /** \action Execute Ta_Fta_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_point_size is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_left_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_long is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_long_dim_7_lt_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_long to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_long[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LONG - EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_left_long is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_left_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_long_dim_7_gt_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_long[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_LEFT_LONG + EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_left_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_left_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_long is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_long_dim_7_eq_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_long to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_long[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LONG);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_left_long is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_left_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_long is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_long_dim_7_eq_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_long to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_long[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_LEFT_LONG);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_left_long is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_left_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_long is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_long_dim_7_middle_of_road_test)
{
   /** \arrange setup k_fta_danger_zone_left_long to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_long[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LONG + TA_MAX_K_FTA_DANGER_ZONE_LEFT_LONG)));
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_left_long is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_left_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_lat is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_lat_dim_7_lt_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_lat to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_lat[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LAT - EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_left_lat is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_left_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_lat is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_lat_dim_7_gt_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_lat to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_lat[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_LEFT_LAT + EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_left_lat is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_left_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_lat is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_lat_dim_7_eq_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_lat to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_lat[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LAT);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_left_lat is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_left_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_lat is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_lat_dim_7_eq_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_lat to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_lat[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_LEFT_LAT);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_left_lat is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_left_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_lat is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_lat_dim_7_middle_of_road_test)
{
   /** \arrange setup k_fta_danger_zone_left_lat to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_lat[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LAT + TA_MAX_K_FTA_DANGER_ZONE_LEFT_LAT)));
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_left_lat is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_right_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_long is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_long_dim_7_lt_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_long to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_long[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LONG - EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_right_long is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_right_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_long_dim_7_gt_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_long[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LONG + EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_right_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_right_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_long is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_long_dim_7_eq_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_long to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_long[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LONG);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_right_long is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_right_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_long is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_long_dim_7_eq_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_long to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_long[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LONG);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_right_long is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_right_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_long is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_long_dim_7_middle_of_road_test)
{
   /** \arrange setup k_fta_danger_zone_right_long to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_long[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LONG + TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LONG)));
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_right_long is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_right_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_lat is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_lat_dim_7_lt_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_lat to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_lat[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LAT - EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_right_lat is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_right_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_lat is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_lat_dim_7_gt_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_lat to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_lat[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LAT + EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_right_lat is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_right_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_lat is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_lat_dim_7_eq_lower_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_lat to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_lat[0] = (float32_T) (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LAT);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_right_lat is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_right_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_lat is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_lat_dim_7_eq_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_right_lat to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_lat[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LAT);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_right_lat is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_danger_zone_right_lat of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_right_lat is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries__k_fta_danger_zone_right_lat_dim_7_middle_of_road_test)
{
   /** \arrange setup k_fta_danger_zone_right_lat to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_danger_zone_right_lat[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LAT + TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LAT)));
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_danger_zone_right_lat is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_speed of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_speed to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed[0] = (float32_T) (TA_MIN_K_TA_EGO_SPEED - EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_speed is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_speed of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_speed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed[0] = (float32_T) (TA_MAX_K_TA_EGO_SPEED + EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_speed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_speed of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_speed to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed[0] = (float32_T) (TA_MIN_K_TA_EGO_SPEED);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_speed is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_speed of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_speed to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed[0] = (float32_T) (TA_MAX_K_TA_EGO_SPEED);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_speed is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_speed of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_dim_2_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_speed to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_speed[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SPEED + TA_MAX_K_TA_EGO_SPEED)));
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_speed is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_speed_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed_ofst is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_speed_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed_ofst[0] = (float32_T) (TA_MIN_K_TA_EGO_SPEED_OFST - EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_speed_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_speed_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed_ofst is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_speed_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed_ofst[0] = (float32_T) (TA_MAX_K_TA_EGO_SPEED_OFST + EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_speed_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_speed_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed_ofst is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_speed_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed_ofst[0] = (float32_T) (TA_MIN_K_TA_EGO_SPEED_OFST);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_speed_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_speed_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed_ofst is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_speed_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed_ofst[0] = (float32_T) (TA_MAX_K_TA_EGO_SPEED_OFST);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_speed_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_speed_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed_ofst is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_speed_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_speed_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_speed_ofst[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SPEED_OFST + TA_MAX_K_TA_EGO_SPEED_OFST)));
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_speed_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawrate of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_yawrate is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_yawrate to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate[0] = (float32_T) (TA_MIN_K_TA_EGO_YAWRATE - EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_yawrate is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_yawrate of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_yawrate is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_yawrate to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate[0] = (float32_T) (TA_MAX_K_TA_EGO_YAWRATE + EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_yawrate is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_yawrate of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_yawrate is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_yawrate to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate[0] = (float32_T) (TA_MIN_K_TA_EGO_YAWRATE);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawrate is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawrate of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_yawrate is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_yawrate to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate[0] = (float32_T) (TA_MAX_K_TA_EGO_YAWRATE);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawrate is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawrate of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_yawrate is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_dim_2_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_yawrate to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_YAWRATE + TA_MAX_K_TA_EGO_YAWRATE)));
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawrate is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawrate_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_yawrate_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_yawrate_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate_ofst[0] = (float32_T) (TA_MIN_K_TA_EGO_YAWRATE_OFST - EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_yawrate_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_yawrate_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_yawrate_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_yawrate_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate_ofst[0] = (float32_T) (TA_MAX_K_TA_EGO_YAWRATE_OFST + EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_yawrate_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_yawrate_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_yawrate_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_yawrate_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate_ofst[0] = (float32_T) (TA_MIN_K_TA_EGO_YAWRATE_OFST);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawrate_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawrate_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_yawrate_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_yawrate_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate_ofst[0] = (float32_T) (TA_MAX_K_TA_EGO_YAWRATE_OFST);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawrate_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawrate_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_yawrate_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawrate_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_yawrate_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_yawrate_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_YAWRATE_OFST + TA_MAX_K_TA_EGO_YAWRATE_OFST)));
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawrate_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_long_acceleration is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration[0] = (float32_T) (TA_MIN_K_TA_EGO_LONG_ACCELERATION - EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_long_acceleration is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_long_acceleration is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration[0] = (float32_T) (TA_MAX_K_TA_EGO_LONG_ACCELERATION + EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_long_acceleration is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_long_acceleration is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration[0] = (float32_T) (TA_MIN_K_TA_EGO_LONG_ACCELERATION);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_long_acceleration is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_long_acceleration is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration[0] = (float32_T) (TA_MAX_K_TA_EGO_LONG_ACCELERATION);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_long_acceleration is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration of component Ta_Host_State_Validator which checks 2-dimensional is within its specified
 * range. Here k_ta_ego_long_acceleration is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_dim_2_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_long_acceleration to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_LONG_ACCELERATION + TA_MAX_K_TA_EGO_LONG_ACCELERATION)));
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_long_acceleration is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its
 * specified range. Here k_ta_ego_long_acceleration_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration_ofst[0] = (float32_T) (TA_MIN_K_TA_EGO_LONG_ACCELERATION_OFST - EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_long_acceleration_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its
 * specified range. Here k_ta_ego_long_acceleration_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration_ofst[0] = (float32_T) (TA_MAX_K_TA_EGO_LONG_ACCELERATION_OFST + EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_long_acceleration_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its
 * specified range. Here k_ta_ego_long_acceleration_ofst is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration_ofst[0] = (float32_T) (TA_MIN_K_TA_EGO_LONG_ACCELERATION_OFST);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_long_acceleration_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its
 * specified range. Here k_ta_ego_long_acceleration_ofst is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_long_acceleration_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration_ofst[0] = (float32_T) (TA_MAX_K_TA_EGO_LONG_ACCELERATION_OFST);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_long_acceleration_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_long_acceleration_ofst of component Ta_Host_State_Validator which checks 2-dimensional is within its
 * specified range. Here k_ta_ego_long_acceleration_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_long_acceleration_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_long_acceleration_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_long_acceleration_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_LONG_ACCELERATION_OFST + TA_MAX_K_TA_EGO_LONG_ACCELERATION_OFST)));
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_long_acceleration_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_straight_host_curvature_max of component Ta_Object_Filter which checks 1-dimensional is within its specified
 * range. Here k_ta_straight_host_curvature_max is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_straight_host_curvature_max_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_straight_host_curvature_max to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_straight_host_curvature_max = (float32_T) (TA_MIN_K_TA_STRAIGHT_HOST_CURVATURE_MAX - EPSILON);
   /** \action Execute Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_straight_host_curvature_max is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_straight_host_curvature_max of component Ta_Object_Filter which checks 1-dimensional is within its specified
 * range. Here k_ta_straight_host_curvature_max is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_straight_host_curvature_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_straight_host_curvature_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_straight_host_curvature_max = (float32_T) (TA_MAX_K_TA_STRAIGHT_HOST_CURVATURE_MAX + EPSILON);
   /** \action Execute Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_straight_host_curvature_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_straight_host_curvature_max of component Ta_Object_Filter which checks 1-dimensional is within its specified
 * range. Here k_ta_straight_host_curvature_max is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_straight_host_curvature_max_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_straight_host_curvature_max to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_straight_host_curvature_max = (float32_T) (TA_MIN_K_TA_STRAIGHT_HOST_CURVATURE_MAX);
   /** \action Execute Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_straight_host_curvature_max is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_straight_host_curvature_max of component Ta_Object_Filter which checks 1-dimensional is within its specified
 * range. Here k_ta_straight_host_curvature_max is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_straight_host_curvature_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_straight_host_curvature_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_straight_host_curvature_max = (float32_T) (TA_MAX_K_TA_STRAIGHT_HOST_CURVATURE_MAX);
   /** \action Execute Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_straight_host_curvature_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_straight_host_curvature_max of component Ta_Object_Filter which checks 1-dimensional is within its specified
 * range. Here k_ta_straight_host_curvature_max is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_straight_host_curvature_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_straight_host_curvature_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_straight_host_curvature_max =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_STRAIGHT_HOST_CURVATURE_MAX + TA_MAX_K_TA_STRAIGHT_HOST_CURVATURE_MAX)));
   /** \action Execute Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_straight_host_curvature_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_speed of component Ta_Object_Filter_Lookup which checks 4-dimensional is within its
 * specified range. Here k_ta_lookup_turning_host_speed is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_speed_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_speed to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_speed[0] = (float32_T) (TA_MIN_K_TA_LOOKUP_TURNING_HOST_SPEED - EPSILON);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_lookup_turning_host_speed is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_speed of component Ta_Object_Filter_Lookup which checks 4-dimensional is within its
 * specified range. Here k_ta_lookup_turning_host_speed is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_speed_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_speed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_speed[0] = (float32_T) (TA_MAX_K_TA_LOOKUP_TURNING_HOST_SPEED + EPSILON);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_lookup_turning_host_speed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_speed of component Ta_Object_Filter_Lookup which checks 4-dimensional is within its
 * specified range. Here k_ta_lookup_turning_host_speed is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_speed_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_speed to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_speed[0] = (float32_T) (TA_MIN_K_TA_LOOKUP_TURNING_HOST_SPEED);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_lookup_turning_host_speed is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_speed of component Ta_Object_Filter_Lookup which checks 4-dimensional is within its
 * specified range. Here k_ta_lookup_turning_host_speed is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_speed_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_speed to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_speed[0] = (float32_T) (TA_MAX_K_TA_LOOKUP_TURNING_HOST_SPEED);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_lookup_turning_host_speed is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_speed of component Ta_Object_Filter_Lookup which checks 4-dimensional is within its
 * specified range. Here k_ta_lookup_turning_host_speed is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_speed_dim_4_middle_of_road_test)
{
   /** \arrange setup k_ta_lookup_turning_host_speed to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_speed[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_LOOKUP_TURNING_HOST_SPEED + TA_MAX_K_TA_LOOKUP_TURNING_HOST_SPEED)));
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_lookup_turning_host_speed is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_curvature_min of component Ta_Object_Filter_Lookup which checks 4-dimensional is within
 * its specified range. Here k_ta_lookup_turning_host_curvature_min is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_curvature_min_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_curvature_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_curvature_min[0] = (float32_T) (TA_MIN_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN - EPSILON);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_lookup_turning_host_curvature_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_curvature_min of component Ta_Object_Filter_Lookup which checks 4-dimensional is within
 * its specified range. Here k_ta_lookup_turning_host_curvature_min is set to a value greater than its maximum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_curvature_min_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_curvature_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_curvature_min[0] = (float32_T) (TA_MAX_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN + EPSILON);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_lookup_turning_host_curvature_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_curvature_min of component Ta_Object_Filter_Lookup which checks 4-dimensional is within
 * its specified range. Here k_ta_lookup_turning_host_curvature_min is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_curvature_min_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_curvature_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_curvature_min[0] = (float32_T) (TA_MIN_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_lookup_turning_host_curvature_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_curvature_min of component Ta_Object_Filter_Lookup which checks 4-dimensional is within
 * its specified range. Here k_ta_lookup_turning_host_curvature_min is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_curvature_min_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_curvature_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_curvature_min[0] = (float32_T) (TA_MAX_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_lookup_turning_host_curvature_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_curvature_min of component Ta_Object_Filter_Lookup which checks 4-dimensional is within
 * its specified range. Here k_ta_lookup_turning_host_curvature_min is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_curvature_min_dim_4_middle_of_road_test)
{
   /** \arrange setup k_ta_lookup_turning_host_curvature_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_curvature_min[0] = (float32_T) ((
      float32_T) (0.5f * (TA_MIN_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN + TA_MAX_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN)));
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_lookup_turning_host_curvature_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_symbol_request_sides_enabled of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_symbol_request_sides_enabled is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_symbol_request_sides_enabled_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_pfgs_symbol_request_sides_enabled to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_pfgs_symbol_request_sides_enabled = (boolean_T) (TA_MIN_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED - 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_symbol_request_sides_enabled is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_symbol_request_sides_enabled of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_symbol_request_sides_enabled is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_symbol_request_sides_enabled_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_pfgs_symbol_request_sides_enabled to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_pfgs_symbol_request_sides_enabled = (boolean_T) (TA_MAX_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED + 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_symbol_request_sides_enabled is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_symbol_request_sides_enabled of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_symbol_request_sides_enabled is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_symbol_request_sides_enabled_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_pfgs_symbol_request_sides_enabled to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_pfgs_symbol_request_sides_enabled = (boolean_T) (TA_MIN_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_symbol_request_sides_enabled is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_symbol_request_sides_enabled of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_symbol_request_sides_enabled is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_symbol_request_sides_enabled_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_pfgs_symbol_request_sides_enabled to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_pfgs_symbol_request_sides_enabled = (boolean_T) (TA_MAX_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_symbol_request_sides_enabled is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_symbol_request_sides_enabled of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_symbol_request_sides_enabled is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_symbol_request_sides_enabled_dim_1_middle_of_road_test)
{
   /** \arrange setup k_pfgs_symbol_request_sides_enabled to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_pfgs_symbol_request_sides_enabled =
      (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED + TA_MAX_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED)));
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_symbol_request_sides_enabled is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_fast_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_fast_obj is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_fast_obj_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_fast_obj to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_fast_obj = (uint8_t) (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ - 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_counter_fast_obj is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_fast_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_fast_obj is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_fast_obj_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_fast_obj to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_fast_obj = (uint8_t) (TA_MAX_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ + 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_counter_fast_obj is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_fast_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_fast_obj is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_fast_obj_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_fast_obj to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_fast_obj = (uint8_t) (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_counter_fast_obj is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_fast_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_fast_obj is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_fast_obj_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_fast_obj to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_fast_obj = (uint8_t) (TA_MAX_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_counter_fast_obj is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_fast_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_fast_obj is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_fast_obj_dim_1_middle_of_road_test)
{
   /** \arrange setup k_pfgs_qualification_counter_fast_obj to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_fast_obj =
      (uint8_t) ((uint8_t) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ + TA_MAX_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ)));
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_counter_fast_obj is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_slow_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_slow_obj is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_slow_obj_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_slow_obj to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_slow_obj = (uint8_t) (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ - 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_counter_slow_obj is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_slow_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_slow_obj is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_slow_obj_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_slow_obj to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_slow_obj = (uint8_t) (TA_MAX_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ + 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_counter_slow_obj is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_slow_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_slow_obj is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_slow_obj_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_slow_obj to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_slow_obj = (uint8_t) (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_counter_slow_obj is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_slow_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_slow_obj is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_slow_obj_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_counter_slow_obj to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_slow_obj = (uint8_t) (TA_MAX_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_counter_slow_obj is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_counter_slow_obj of component Ta_Post_Run which checks 1-dimensional is within its specified
 * range. Here k_pfgs_qualification_counter_slow_obj is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_counter_slow_obj_dim_1_middle_of_road_test)
{
   /** \arrange setup k_pfgs_qualification_counter_slow_obj to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_pfgs_qualification_counter_slow_obj =
      (uint8_t) ((uint8_t) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ + TA_MAX_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ)));
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_counter_slow_obj is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_ttc_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_pfgs_qualification_ttc_min is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_ttc_min_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_ttc_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_ttc_min = (float32_T) (TA_MIN_K_PFGS_QUALIFICATION_TTC_MIN - EPSILON);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_ttc_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_ttc_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_pfgs_qualification_ttc_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_ttc_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_ttc_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_ttc_min = (float32_T) (TA_MAX_K_PFGS_QUALIFICATION_TTC_MIN + EPSILON);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_ttc_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_ttc_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_pfgs_qualification_ttc_min is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_ttc_min_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_ttc_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_ttc_min = (float32_T) (TA_MIN_K_PFGS_QUALIFICATION_TTC_MIN);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_ttc_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_ttc_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_pfgs_qualification_ttc_min is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_ttc_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_ttc_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_ttc_min = (float32_T) (TA_MAX_K_PFGS_QUALIFICATION_TTC_MIN);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_ttc_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_ttc_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_pfgs_qualification_ttc_min is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_ttc_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_pfgs_qualification_ttc_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_pfgs_qualification_ttc_min =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_TTC_MIN + TA_MAX_K_PFGS_QUALIFICATION_TTC_MIN)));
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_ttc_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_check_f_stationary of component Ta_Post_Run which checks 1-dimensional is within its
 * specified range. Here k_pfgs_qualification_check_f_stationary is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_check_f_stationary_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_check_f_stationary to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_check_f_stationary = (boolean_T) (TA_MIN_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY - 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_check_f_stationary is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_check_f_stationary of component Ta_Post_Run which checks 1-dimensional is within its
 * specified range. Here k_pfgs_qualification_check_f_stationary is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_check_f_stationary_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_check_f_stationary to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_check_f_stationary = (boolean_T) (TA_MAX_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY + 1u);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_qualification_check_f_stationary is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_qualification_check_f_stationary of component Ta_Post_Run which checks 1-dimensional is within its
 * specified range. Here k_pfgs_qualification_check_f_stationary is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_check_f_stationary_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_pfgs_qualification_check_f_stationary to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_check_f_stationary = (boolean_T) (TA_MIN_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_check_f_stationary is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_check_f_stationary of component Ta_Post_Run which checks 1-dimensional is within its
 * specified range. Here k_pfgs_qualification_check_f_stationary is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_check_f_stationary_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_pfgs_qualification_check_f_stationary to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_pfgs_qualification_check_f_stationary = (boolean_T) (TA_MAX_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_check_f_stationary is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_qualification_check_f_stationary of component Ta_Post_Run which checks 1-dimensional is within its
 * specified range. Here k_pfgs_qualification_check_f_stationary is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_qualification_check_f_stationary_dim_1_middle_of_road_test)
{
   /** \arrange setup k_pfgs_qualification_check_f_stationary to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_pfgs_qualification_check_f_stationary = (boolean_T) ((
      boolean_T) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY + TA_MAX_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY)));
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_qualification_check_f_stationary is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_tap_lvl_2_host_curvature_min_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min = (float32_T) (TA_MIN_K_TAP_LVL_2_HOST_CURVATURE_MIN - EPSILON);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_tap_lvl_2_host_curvature_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_tap_lvl_2_host_curvature_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min = (float32_T) (TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN + EPSILON);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_tap_lvl_2_host_curvature_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_tap_lvl_2_host_curvature_min_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min = (float32_T) (TA_MIN_K_TAP_LVL_2_HOST_CURVATURE_MIN);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_tap_lvl_2_host_curvature_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_tap_lvl_2_host_curvature_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min = (float32_T) (TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_tap_lvl_2_host_curvature_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries__k_tap_lvl_2_host_curvature_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TAP_LVL_2_HOST_CURVATURE_MIN + TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN)));
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_tap_lvl_2_host_curvature_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_ego_speed of component Ta_Post_Run which checks 2-dimensional is within its specified range.
 * Here k_pfgs_ego_speed is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_ego_speed_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_pfgs_ego_speed to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_pfgs_ego_speed[0] = (float32_T) (TA_MIN_K_PFGS_EGO_SPEED - EPSILON);
   /** \action Execute Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_ego_speed is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_ego_speed of component Ta_Post_Run which checks 2-dimensional is within its specified range.
 * Here k_pfgs_ego_speed is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_ego_speed_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_pfgs_ego_speed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_pfgs_ego_speed[0] = (float32_T) (TA_MAX_K_PFGS_EGO_SPEED + EPSILON);
   /** \action Execute Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_pfgs_ego_speed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_pfgs_ego_speed of component Ta_Post_Run which checks 2-dimensional is within its specified range.
 * Here k_pfgs_ego_speed is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_ego_speed_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_pfgs_ego_speed to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_pfgs_ego_speed[0] = (float32_T) (TA_MIN_K_PFGS_EGO_SPEED);
   /** \action Execute Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_ego_speed is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_ego_speed of component Ta_Post_Run which checks 2-dimensional is within its specified range.
 * Here k_pfgs_ego_speed is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_ego_speed_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_pfgs_ego_speed to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_pfgs_ego_speed[0] = (float32_T) (TA_MAX_K_PFGS_EGO_SPEED);
   /** \action Execute Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_ego_speed is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_pfgs_ego_speed of component Ta_Post_Run which checks 2-dimensional is within its specified range.
 * Here k_pfgs_ego_speed is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries__k_pfgs_ego_speed_dim_2_middle_of_road_test)
{
   /** \arrange setup k_pfgs_ego_speed to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_pfgs_ego_speed[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_PFGS_EGO_SPEED + TA_MAX_K_PFGS_EGO_SPEED)));
   /** \action Execute Ta_Post_Run_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_pfgs_ego_speed is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_rta_enable is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_f_rta_enable to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable = (boolean_T) (TA_MIN_K_F_RTA_ENABLE - 1u);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_rta_enable is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_rta_enable of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_rta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_rta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable = (boolean_T) (TA_MAX_K_F_RTA_ENABLE + 1u);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_rta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_rta_enable of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_rta_enable is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_f_rta_enable to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable = (boolean_T) (TA_MIN_K_F_RTA_ENABLE);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_rta_enable is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_f_rta_enable to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable = (boolean_T) (TA_MAX_K_F_RTA_ENABLE);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_rta_enable is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_dim_1_middle_of_road_test)
{
   /** \arrange setup k_f_rta_enable to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_f_rta_enable = (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_F_RTA_ENABLE + TA_MAX_K_F_RTA_ENABLE)));
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable_info_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_info_zones is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_info_zones_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_f_rta_enable_info_zones to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_info_zones = (boolean_T) (TA_MIN_K_F_RTA_ENABLE_INFO_ZONES - 1u);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_rta_enable_info_zones is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_rta_enable_info_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_info_zones is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_info_zones_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_rta_enable_info_zones to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_info_zones = (boolean_T) (TA_MAX_K_F_RTA_ENABLE_INFO_ZONES + 1u);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_rta_enable_info_zones is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_rta_enable_info_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_info_zones is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_info_zones_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_f_rta_enable_info_zones to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_info_zones = (boolean_T) (TA_MIN_K_F_RTA_ENABLE_INFO_ZONES);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable_info_zones is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable_info_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_info_zones is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_info_zones_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_f_rta_enable_info_zones to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_info_zones = (boolean_T) (TA_MAX_K_F_RTA_ENABLE_INFO_ZONES);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable_info_zones is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable_info_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_info_zones is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_info_zones_dim_1_middle_of_road_test)
{
   /** \arrange setup k_f_rta_enable_info_zones to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_f_rta_enable_info_zones =
      (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_F_RTA_ENABLE_INFO_ZONES + TA_MAX_K_F_RTA_ENABLE_INFO_ZONES)));
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable_info_zones is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable_wing_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_wing_zones is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_wing_zones_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_f_rta_enable_wing_zones to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_wing_zones = (boolean_T) (TA_MIN_K_F_RTA_ENABLE_WING_ZONES - 1u);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_rta_enable_wing_zones is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_rta_enable_wing_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_wing_zones is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_wing_zones_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_rta_enable_wing_zones to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_wing_zones = (boolean_T) (TA_MAX_K_F_RTA_ENABLE_WING_ZONES + 1u);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_rta_enable_wing_zones is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_rta_enable_wing_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_wing_zones is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_wing_zones_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_f_rta_enable_wing_zones to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_wing_zones = (boolean_T) (TA_MIN_K_F_RTA_ENABLE_WING_ZONES);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable_wing_zones is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable_wing_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_wing_zones is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_wing_zones_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_f_rta_enable_wing_zones to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable_wing_zones = (boolean_T) (TA_MAX_K_F_RTA_ENABLE_WING_ZONES);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable_wing_zones is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_f_rta_enable_wing_zones of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified
 * range. Here k_f_rta_enable_wing_zones is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries__k_f_rta_enable_wing_zones_dim_1_middle_of_road_test)
{
   /** \arrange setup k_f_rta_enable_wing_zones to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_f_rta_enable_wing_zones =
      (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_F_RTA_ENABLE_WING_ZONES + TA_MAX_K_F_RTA_ENABLE_WING_ZONES)));
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_f_rta_enable_wing_zones is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_point_size of component Ta_Rta_Info_Zone which checks 1-dimensional is within its specified range.
 * Here k_rta_info_zone_point_size is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_point_size_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_point_size to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_point_size = (uint8_t) (TA_MAX_K_RTA_INFO_ZONE_POINT_SIZE + 1u);
   /** \action Execute Ta_Rta_Info_Zone_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_point_size is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_point_size of component Ta_Rta_Info_Zone which checks 1-dimensional is within its specified range.
 * Here k_rta_info_zone_point_size is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_point_size_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_point_size to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_point_size = (uint8_t) (TA_MAX_K_RTA_INFO_ZONE_POINT_SIZE);
   /** \action Execute Ta_Rta_Info_Zone_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_point_size is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_point_size of component Ta_Rta_Info_Zone which checks 1-dimensional is within its specified range.
 * Here k_rta_info_zone_point_size is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_point_size_dim_1_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_point_size to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_point_size = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_RTA_INFO_ZONE_POINT_SIZE));
   /** \action Execute Ta_Rta_Info_Zone_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_point_size is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_long is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_long is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_long is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_long is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_long is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_long is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_long is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_left_long to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG + TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_long is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_lat is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_lat is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_lat is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_lat is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_lat is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_lat is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_lat is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_lat is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_lat is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_left_lat to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT + TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_lat is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_long is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_long is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_long is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_long is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_long is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_long is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_long is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_right_long to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_long is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_lat is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_lat is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_lat is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_lat is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_lat is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_lat is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_lat is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_lat is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_right_lat is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_right_lat to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_lat is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_long_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG_HYS - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_long_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_long_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG_HYS + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_long_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_long_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_long_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_long_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_long_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_long_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_left_long_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG_HYS + TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG_HYS)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_long_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_lat_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT_HYS - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_lat_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_lat_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT_HYS + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_lat_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_lat_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_lat_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_lat_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_lat_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_lat_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_left_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_left_lat_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_lat_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_left_lat_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_lat_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT_HYS + TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT_HYS)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_left_lat_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_long_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG_HYS - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_long_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_long_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG_HYS + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_long_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_long_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_long_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_long_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_long_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_long_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_long_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_long_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_long_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_right_long_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_long_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG_HYS + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG_HYS)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_long_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_lat_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT_HYS - EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_lat_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_lat_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT_HYS + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_right_lat_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_lat_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_lat_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_lat_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_right_lat_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT_HYS);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_lat_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_info_zone_right_lat_hys of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified
 * range. Here k_rta_info_zone_right_lat_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_info_zone_right_lat_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_info_zone_right_lat_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_info_zone_right_lat_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT_HYS + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT_HYS)));
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_info_zone_right_lat_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_age_min of component Ta_Rta_Object_Filter which checks 1-dimensional is within its specified range.
 * Here k_rta_obj_age_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_age_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_age_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_age_min = (uint8_t) (TA_MAX_K_RTA_OBJ_AGE_MIN + 1u);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_age_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_age_min of component Ta_Rta_Object_Filter which checks 1-dimensional is within its specified range.
 * Here k_rta_obj_age_min is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_age_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_age_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_age_min = (uint8_t) (TA_MAX_K_RTA_OBJ_AGE_MIN);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_age_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_age_min of component Ta_Rta_Object_Filter which checks 1-dimensional is within its specified range.
 * Here k_rta_obj_age_min is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_age_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_age_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_age_min = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_RTA_OBJ_AGE_MIN));
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_age_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_lat_vel_rel_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_lat_vel_rel_max is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_lat_vel_rel_max_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_lat_vel_rel_max to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_lat_vel_rel_max = (float32_T) (TA_MIN_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_ttp_obj_abs_lat_vel_rel_max is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_lat_vel_rel_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_lat_vel_rel_max is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_lat_vel_rel_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_lat_vel_rel_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_lat_vel_rel_max = (float32_T) (TA_MAX_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_ttp_obj_abs_lat_vel_rel_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_lat_vel_rel_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_lat_vel_rel_max is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_lat_vel_rel_max_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_lat_vel_rel_max to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_lat_vel_rel_max = (float32_T) (TA_MIN_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_obj_abs_lat_vel_rel_max is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_lat_vel_rel_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_lat_vel_rel_max is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_lat_vel_rel_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_lat_vel_rel_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_lat_vel_rel_max = (float32_T) (TA_MAX_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_obj_abs_lat_vel_rel_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_lat_vel_rel_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_lat_vel_rel_max is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_lat_vel_rel_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_rta_ttp_obj_abs_lat_vel_rel_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_lat_vel_rel_max =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX + TA_MAX_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX)));
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_obj_abs_lat_vel_rel_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_heading_diff_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_heading_diff_max is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_heading_diff_max_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_heading_diff_max to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_heading_diff_max = (float32_T) (TA_MIN_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_ttp_obj_abs_heading_diff_max is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_heading_diff_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_heading_diff_max is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_heading_diff_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_heading_diff_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_heading_diff_max = (float32_T) (TA_MAX_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_ttp_obj_abs_heading_diff_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_heading_diff_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_heading_diff_max is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_heading_diff_max_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_heading_diff_max to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_heading_diff_max = (float32_T) (TA_MIN_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_obj_abs_heading_diff_max is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_heading_diff_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_heading_diff_max is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_heading_diff_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_rta_ttp_obj_abs_heading_diff_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_heading_diff_max = (float32_T) (TA_MAX_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_obj_abs_heading_diff_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_obj_abs_heading_diff_max of component Ta_Rta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_rta_ttp_obj_abs_heading_diff_max is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_obj_abs_heading_diff_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_rta_ttp_obj_abs_heading_diff_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_ttp_obj_abs_heading_diff_max =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX + TA_MAX_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX)));
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_obj_abs_heading_diff_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_curve_suppression_obj_distance_min of component Ta_Rta_Object_Filter which checks 1-dimensional is
 * within its specified range. Here k_rta_ttp_curve_suppression_obj_distance_min is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_curve_suppression_obj_distance_min_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_rta_ttp_curve_suppression_obj_distance_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_curve_suppression_obj_distance_min =
      (float32_T) (TA_MIN_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_ttp_curve_suppression_obj_distance_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_ttp_curve_suppression_obj_distance_min of component Ta_Rta_Object_Filter which checks 1-dimensional is
 * within its specified range. Here k_rta_ttp_curve_suppression_obj_distance_min is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_curve_suppression_obj_distance_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_ttp_curve_suppression_obj_distance_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_curve_suppression_obj_distance_min =
      (float32_T) (TA_MAX_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_ttp_curve_suppression_obj_distance_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_ttp_curve_suppression_obj_distance_min of component Ta_Rta_Object_Filter which checks 1-dimensional is
 * within its specified range. Here k_rta_ttp_curve_suppression_obj_distance_min is set to a value equal to its lower boundary.
 * Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_curve_suppression_obj_distance_min_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_rta_ttp_curve_suppression_obj_distance_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_curve_suppression_obj_distance_min = (float32_T) (TA_MIN_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_curve_suppression_obj_distance_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_curve_suppression_obj_distance_min of component Ta_Rta_Object_Filter which checks 1-dimensional is
 * within its specified range. Here k_rta_ttp_curve_suppression_obj_distance_min is set to a value equal to its upper boundary.
 * Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_curve_suppression_obj_distance_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_rta_ttp_curve_suppression_obj_distance_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_ttp_curve_suppression_obj_distance_min = (float32_T) (TA_MAX_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_curve_suppression_obj_distance_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_ttp_curve_suppression_obj_distance_min of component Ta_Rta_Object_Filter which checks 1-dimensional is
 * within its specified range. Here k_rta_ttp_curve_suppression_obj_distance_min is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_ttp_curve_suppression_obj_distance_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_rta_ttp_curve_suppression_obj_distance_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_ttp_curve_suppression_obj_distance_min = (float32_T) ((
      float32_T) (0.5f * (TA_MIN_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN + TA_MAX_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN)));
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_ttp_curve_suppression_obj_distance_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_exist_prblty is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty[0] = (float32_T) (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_exist_prblty is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_exist_prblty is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty[0] = (float32_T) (TA_MAX_K_RTA_OBJ_EXIST_PRBLTY + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_exist_prblty is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_exist_prblty is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty[0] = (float32_T) (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_exist_prblty is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_exist_prblty is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty[0] = (float32_T) (TA_MAX_K_RTA_OBJ_EXIST_PRBLTY);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_exist_prblty is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_exist_prblty is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_exist_prblty to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY + TA_MAX_K_RTA_OBJ_EXIST_PRBLTY)));
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_exist_prblty is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_speed of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_speed is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_speed to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed[0] = (float32_T) (TA_MIN_K_RTA_OBJ_SPEED - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_speed is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_speed of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_speed is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_speed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed[0] = (float32_T) (TA_MAX_K_RTA_OBJ_SPEED + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_speed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_speed of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_speed is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_speed to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed[0] = (float32_T) (TA_MIN_K_RTA_OBJ_SPEED);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_speed is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_speed of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_speed is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_speed to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed[0] = (float32_T) (TA_MAX_K_RTA_OBJ_SPEED);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_speed is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_speed of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_speed is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_speed to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_speed[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_SPEED + TA_MAX_K_RTA_OBJ_SPEED)));
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_speed is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_length of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_length is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_length to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length[0] = (float32_T) (TA_MIN_K_RTA_OBJ_LENGTH - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_length is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_length of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_length is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_length to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length[0] = (float32_T) (TA_MAX_K_RTA_OBJ_LENGTH + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_length is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_length of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_length is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_length to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length[0] = (float32_T) (TA_MIN_K_RTA_OBJ_LENGTH);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_length is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_length of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_length is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_length to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length[0] = (float32_T) (TA_MAX_K_RTA_OBJ_LENGTH);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_length is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_length of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_length is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_length to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_length[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_LENGTH + TA_MAX_K_RTA_OBJ_LENGTH)));
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_length is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_width of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_width is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_width to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width[0] = (float32_T) (TA_MIN_K_RTA_OBJ_WIDTH - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_width is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_width of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_width is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_width to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width[0] = (float32_T) (TA_MAX_K_RTA_OBJ_WIDTH + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_width is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_width of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_width is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_width to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width[0] = (float32_T) (TA_MIN_K_RTA_OBJ_WIDTH);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_width is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_width of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_width is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_width to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width[0] = (float32_T) (TA_MAX_K_RTA_OBJ_WIDTH);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_width is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_width of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_width is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_width to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_width[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_WIDTH + TA_MAX_K_RTA_OBJ_WIDTH)));
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_width is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_vru_class_prob is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vru_class_prob is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_vru_class_prob is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vru_class_prob is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_vru_class_prob is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vru_class_prob is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_vru_class_prob is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vru_class_prob is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_vru_class_prob is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vru_class_prob to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB + TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB)));
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vru_class_prob is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_eclipse_value is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value[0] = (float32_T) (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_eclipse_value is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_eclipse_value is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value[0] = (float32_T) (TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_eclipse_value is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_eclipse_value is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value[0] = (float32_T) (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_eclipse_value is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_eclipse_value is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value[0] = (float32_T) (TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_eclipse_value is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_eclipse_value is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_eclipse_value to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE + TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE)));
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_eclipse_value is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_exist_prblty_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_exist_prblty_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_exist_prblty_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_EXIST_PRBLTY_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_exist_prblty_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_exist_prblty_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_exist_prblty_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_exist_prblty_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_EXIST_PRBLTY_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_exist_prblty_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_exist_prblty_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_exist_prblty_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY_OFST + TA_MAX_K_RTA_OBJ_EXIST_PRBLTY_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_exist_prblty_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_speed_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_speed_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_speed_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_SPEED_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_speed_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_speed_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_speed_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_speed_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_SPEED_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_speed_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_speed_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_speed_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_speed_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_SPEED_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_speed_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_speed_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_speed_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_speed_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_speed_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_SPEED_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_speed_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_speed_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_speed_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_speed_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_speed_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_speed_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_SPEED_OFST + TA_MAX_K_RTA_OBJ_SPEED_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_speed_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_length_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_length_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_length_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_LENGTH_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_length_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_length_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_length_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_length_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_LENGTH_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_length_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_length_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_length_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_length_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_LENGTH_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_length_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_length_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_length_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_length_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_length_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_LENGTH_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_length_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_length_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_length_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_length_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_length_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_length_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_LENGTH_OFST + TA_MAX_K_RTA_OBJ_LENGTH_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_length_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_width_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_width_ofst is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_width_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_WIDTH_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_width_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_width_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_width_ofst is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_width_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_WIDTH_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_width_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_width_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_width_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_width_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_WIDTH_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_width_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_width_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_width_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_width_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_width_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_WIDTH_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_width_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_width_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its specified
 * range. Here k_rta_obj_width_ofst is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_width_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_width_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_width_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_WIDTH_OFST + TA_MAX_K_RTA_OBJ_WIDTH_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_width_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vru_class_prob_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vru_class_prob_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vru_class_prob_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vru_class_prob_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vru_class_prob_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vru_class_prob_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vru_class_prob_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vru_class_prob_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vru_class_prob_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vru_class_prob_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vru_class_prob_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vru_class_prob_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vru_class_prob_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vru_class_prob_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB_OFST + TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vru_class_prob_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_eclipse_value_ofst is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_eclipse_value_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_eclipse_value_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_eclipse_value_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_eclipse_value_ofst is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_eclipse_value_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_eclipse_value_ofst is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_eclipse_value_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_eclipse_value_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_eclipse_value_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_eclipse_value_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_eclipse_value_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_eclipse_value_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_eclipse_value_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE_OFST + TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_eclipse_value_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_rta_obj_vcs_long_vel_rel_ofst is set to a value less than its minimum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_rel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_rta_obj_vcs_long_vel_rel_ofst is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_rel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_rta_obj_vcs_long_vel_rel_ofst is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_rel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_rta_obj_vcs_long_vel_rel_ofst is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_rel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_rta_obj_vcs_long_vel_rel_ofst is set to a value equal to mid of its
 * boundaries. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_rel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_rta_obj_vcs_lat_vel_rel_ofst is set to a value less than its minimum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel_rel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_rta_obj_vcs_lat_vel_rel_ofst is set to a value greater than its maximum boundary. Thus
 * false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel_rel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_rta_obj_vcs_lat_vel_rel_ofst is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_rel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_rta_obj_vcs_lat_vel_rel_ofst is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_rel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional
 * is within its specified range. Here k_rta_obj_vcs_lat_vel_rel_ofst is set to a value equal to mid of its boundaries. Thus true
 * is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_rel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_long_vel_ofst is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_long_vel_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_long_vel_ofst is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_long_vel_ofst is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_long_vel_ofst is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_OFST + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_lat_vel_ofst is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_lat_vel_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_lat_vel_ofst is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_lat_vel_ofst is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_vcs_lat_vel_ofst is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_OFST + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_heading_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_heading_ofst is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_ofst_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_heading_ofst to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_HEADING_OFST - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_heading_ofst is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_heading_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_heading_ofst is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_heading_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_HEADING_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_heading_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_heading_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_heading_ofst is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_ofst_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_heading_ofst to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading_ofst[0] = (float32_T) (TA_MIN_K_RTA_OBJ_HEADING_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_heading_ofst is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_heading_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_heading_ofst is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_ofst_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_heading_ofst to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_HEADING_OFST);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_heading_ofst is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_heading_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks 2-dimensional is
 * within its specified range. Here k_rta_obj_heading_ofst is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_ofst_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_heading_ofst to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_heading_ofst[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_HEADING_OFST + TA_MAX_K_RTA_OBJ_HEADING_OFST)));
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_heading_ofst is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_long_vel_rel is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_rel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_long_vel_rel is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_rel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_long_vel_rel is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_rel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_long_vel_rel is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_rel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_long_vel_rel is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL)));
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel_rel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_lat_vel_rel is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel_rel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_lat_vel_rel is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel_rel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_lat_vel_rel is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_rel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_lat_vel_rel is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_rel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_lat_vel_rel is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_rel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel_rel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel_rel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL)));
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel_rel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_long_vel is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_long_vel is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_long_vel is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_long_vel is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_long_vel is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_long_vel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL)));
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_long_vel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_lat_vel is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_lat_vel is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_lat_vel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_lat_vel is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel[0] = (float32_T) (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_lat_vel is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LAT_VEL);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_vcs_lat_vel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_vcs_lat_vel is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_lat_vel_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_vcs_lat_vel to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_lat_vel[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL)));
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_vcs_lat_vel is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_heading of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_heading is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_dim_2_lt_lower_boundary)
{
   /** \arrange setup k_rta_obj_heading to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading[0] = (float32_T) (TA_MIN_K_RTA_OBJ_HEADING - EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_heading is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_heading of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_heading is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_heading to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading[0] = (float32_T) (TA_MAX_K_RTA_OBJ_HEADING + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_heading is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_heading of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_heading is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_dim_2_eq_lower_boundary)
{
   /** \arrange setup k_rta_obj_heading to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading[0] = (float32_T) (TA_MIN_K_RTA_OBJ_HEADING);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_heading is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_heading of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_heading is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_dim_2_eq_upper_boundary)
{
   /** \arrange setup k_rta_obj_heading to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_obj_heading[0] = (float32_T) (TA_MAX_K_RTA_OBJ_HEADING);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_heading is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_obj_heading of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_heading is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries__k_rta_obj_heading_dim_2_middle_of_road_test)
{
   /** \arrange setup k_rta_obj_heading to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_obj_heading[0] = (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_HEADING + TA_MAX_K_RTA_OBJ_HEADING)));
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_obj_heading is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_point_size of component Ta_Rta_Wing_Zones which checks 1-dimensional is within its specified
 * range. Here k_rta_wing_zone_point_size is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_point_size_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_point_size to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_point_size = (uint8_t) (TA_MAX_K_RTA_WING_ZONE_POINT_SIZE + 1u);
   /** \action Execute Ta_Rta_Wing_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_point_size is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_point_size of component Ta_Rta_Wing_Zones which checks 1-dimensional is within its specified
 * range. Here k_rta_wing_zone_point_size is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_point_size_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_point_size to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_point_size = (uint8_t) (TA_MAX_K_RTA_WING_ZONE_POINT_SIZE);
   /** \action Execute Ta_Rta_Wing_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_point_size is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_point_size of component Ta_Rta_Wing_Zones which checks 1-dimensional is within its specified
 * range. Here k_rta_wing_zone_point_size is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_point_size_dim_1_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_point_size to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_point_size = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_RTA_WING_ZONE_POINT_SIZE));
   /** \action Execute Ta_Rta_Wing_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_point_size is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_long is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_long is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LONG + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_long is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_long is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_long is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LONG);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_long is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_long is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_left_long to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG + TA_MAX_K_RTA_WING_ZONE_LEFT_LONG)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_long is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_lat is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_lat is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_lat is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LAT + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_lat is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_lat is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_lat is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_lat is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LAT);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_lat is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_lat is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_left_lat to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT + TA_MAX_K_RTA_WING_ZONE_LEFT_LAT)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_lat is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_long is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_long is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_long is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_right_long to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG + TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_long is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_right_lat is set to a value less than its minimum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_lat is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_right_lat is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_lat is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_right_lat is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_lat is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_right_lat is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_lat is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_right_lat is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_right_lat to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT + TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_lat is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_long_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG_HYS - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_long_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_long_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LONG_HYS + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_long_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_long_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_long_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_long_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LONG_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_long_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_long_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_left_long_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG_HYS + TA_MAX_K_RTA_WING_ZONE_LEFT_LONG_HYS)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_long_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_lat_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT_HYS - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_lat_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_lat_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LAT_HYS + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_lat_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_lat_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_lat_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_lat_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_lat_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LAT_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_lat_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_left_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_left_lat_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_lat_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_left_lat_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_lat_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT_HYS + TA_MAX_K_RTA_WING_ZONE_LEFT_LAT_HYS)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_left_lat_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG_HYS - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_long_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG_HYS + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_long_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_long_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_long_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_long_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_long_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_long_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_long_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_right_long_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_long_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG_HYS + TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG_HYS)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_long_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_lat_hys is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_hys_dim_4_lt_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat_hys to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT_HYS - EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_lat_hys is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_lat_hys is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_hys_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat_hys to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT_HYS + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_right_lat_hys is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_lat_hys is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_hys_dim_4_eq_lower_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat_hys to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat_hys[0] = (float32_T) (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_lat_hys is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_lat_hys is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_hys_dim_4_eq_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_right_lat_hys to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat_hys[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT_HYS);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_lat_hys is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_wing_zone_right_lat_hys of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified
 * range. Here k_rta_wing_zone_right_lat_hys is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries__k_rta_wing_zone_right_lat_hys_dim_4_middle_of_road_test)
{
   /** \arrange setup k_rta_wing_zone_right_lat_hys to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_wing_zone_right_lat_hys[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT_HYS + TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT_HYS)));
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_wing_zone_right_lat_hys is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_prediction_steps_max of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_prediction_steps_max is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_prediction_steps_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_prediction_steps_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_prediction_steps_max = (uint8_t) (TA_MAX_K_TA_PREDICTION_STEPS_MAX + 1u);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_prediction_steps_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_prediction_steps_max of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_prediction_steps_max is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_prediction_steps_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_prediction_steps_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_prediction_steps_max = (uint8_t) (TA_MAX_K_TA_PREDICTION_STEPS_MAX);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_prediction_steps_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_prediction_steps_max of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_prediction_steps_max is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_prediction_steps_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_prediction_steps_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_prediction_steps_max = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_TA_PREDICTION_STEPS_MAX));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_prediction_steps_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_max_pred_yaw_angle of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_max_pred_yaw_angle is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_max_pred_yaw_angle_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_max_pred_yaw_angle to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_max_pred_yaw_angle = (float32_T) (TA_MIN_K_TA_EGO_MAX_PRED_YAW_ANGLE - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_max_pred_yaw_angle is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_max_pred_yaw_angle of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_max_pred_yaw_angle is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_max_pred_yaw_angle_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_max_pred_yaw_angle to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_max_pred_yaw_angle = (float32_T) (TA_MAX_K_TA_EGO_MAX_PRED_YAW_ANGLE + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_max_pred_yaw_angle is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_max_pred_yaw_angle of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_max_pred_yaw_angle is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_max_pred_yaw_angle_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_max_pred_yaw_angle to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_max_pred_yaw_angle = (float32_T) (TA_MIN_K_TA_EGO_MAX_PRED_YAW_ANGLE);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_max_pred_yaw_angle is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_max_pred_yaw_angle of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_max_pred_yaw_angle is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_max_pred_yaw_angle_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_max_pred_yaw_angle to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_max_pred_yaw_angle = (float32_T) (TA_MAX_K_TA_EGO_MAX_PRED_YAW_ANGLE);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_max_pred_yaw_angle is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_max_pred_yaw_angle of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_max_pred_yaw_angle is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_max_pred_yaw_angle_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_max_pred_yaw_angle to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_max_pred_yaw_angle =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_MAX_PRED_YAW_ANGLE + TA_MAX_K_TA_EGO_MAX_PRED_YAW_ANGLE)));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_max_pred_yaw_angle is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_pred_speed_min of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_obj_pred_speed_min is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_pred_speed_min_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_obj_pred_speed_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_pred_speed_min = (float32_T) (TA_MIN_K_TA_OBJ_PRED_SPEED_MIN - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_pred_speed_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_pred_speed_min of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_obj_pred_speed_min is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_pred_speed_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_obj_pred_speed_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_pred_speed_min = (float32_T) (TA_MAX_K_TA_OBJ_PRED_SPEED_MIN + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_pred_speed_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_pred_speed_min of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_obj_pred_speed_min is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_pred_speed_min_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_obj_pred_speed_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_obj_pred_speed_min = (float32_T) (TA_MIN_K_TA_OBJ_PRED_SPEED_MIN);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_pred_speed_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_pred_speed_min of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_obj_pred_speed_min is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_pred_speed_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_obj_pred_speed_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_obj_pred_speed_min = (float32_T) (TA_MAX_K_TA_OBJ_PRED_SPEED_MIN);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_pred_speed_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_pred_speed_min of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_obj_pred_speed_min is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_pred_speed_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_obj_pred_speed_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_obj_pred_speed_min =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_PRED_SPEED_MIN + TA_MAX_K_TA_OBJ_PRED_SPEED_MIN)));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_pred_speed_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_acceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_acceleration_weight is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_acceleration_weight_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_acceleration_weight to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_acceleration_weight = (float32_T) (TA_MIN_K_TA_EGO_ACCELERATION_WEIGHT - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_acceleration_weight is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_acceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_acceleration_weight is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_acceleration_weight_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_acceleration_weight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_acceleration_weight = (float32_T) (TA_MAX_K_TA_EGO_ACCELERATION_WEIGHT + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_acceleration_weight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_acceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_acceleration_weight is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_acceleration_weight_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_acceleration_weight to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_acceleration_weight = (float32_T) (TA_MIN_K_TA_EGO_ACCELERATION_WEIGHT);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_acceleration_weight is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_acceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_acceleration_weight is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_acceleration_weight_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_acceleration_weight to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_acceleration_weight = (float32_T) (TA_MAX_K_TA_EGO_ACCELERATION_WEIGHT);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_acceleration_weight is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_acceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_acceleration_weight is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_acceleration_weight_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_acceleration_weight to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_acceleration_weight =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_ACCELERATION_WEIGHT + TA_MAX_K_TA_EGO_ACCELERATION_WEIGHT)));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_acceleration_weight is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_deceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_deceleration_weight is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_deceleration_weight_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_deceleration_weight to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_deceleration_weight = (float32_T) (TA_MIN_K_TA_EGO_DECELERATION_WEIGHT - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_deceleration_weight is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_deceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_deceleration_weight is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_deceleration_weight_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_deceleration_weight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_deceleration_weight = (float32_T) (TA_MAX_K_TA_EGO_DECELERATION_WEIGHT + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_deceleration_weight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_deceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_deceleration_weight is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_deceleration_weight_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_deceleration_weight to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_deceleration_weight = (float32_T) (TA_MIN_K_TA_EGO_DECELERATION_WEIGHT);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_deceleration_weight is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_deceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_deceleration_weight is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_deceleration_weight_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_deceleration_weight to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_deceleration_weight = (float32_T) (TA_MAX_K_TA_EGO_DECELERATION_WEIGHT);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_deceleration_weight is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_deceleration_weight of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_deceleration_weight is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_deceleration_weight_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_deceleration_weight to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_deceleration_weight =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_DECELERATION_WEIGHT + TA_MAX_K_TA_EGO_DECELERATION_WEIGHT)));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_deceleration_weight is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_pred_const_velocity_pred_steps_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_pred_const_velocity_pred_steps_min is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_pred_const_velocity_pred_steps_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_pred_const_velocity_pred_steps_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_pred_const_velocity_pred_steps_min = (uint8_t) (TA_MAX_K_TA_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN + 1u);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_pred_const_velocity_pred_steps_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_pred_const_velocity_pred_steps_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_pred_const_velocity_pred_steps_min is set to a value equal to its upper boundary. Thus
 * true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_pred_const_velocity_pred_steps_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_pred_const_velocity_pred_steps_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_pred_const_velocity_pred_steps_min = (uint8_t) (TA_MAX_K_TA_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_pred_const_velocity_pred_steps_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_pred_const_velocity_pred_steps_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_pred_const_velocity_pred_steps_min is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_pred_const_velocity_pred_steps_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_pred_const_velocity_pred_steps_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_pred_const_velocity_pred_steps_min =
      (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_TA_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_pred_const_velocity_pred_steps_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_circle_offset of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_ego_circle_offset is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_offset_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_circle_offset to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_offset = (float32_T) (TA_MIN_K_TA_EGO_CIRCLE_OFFSET - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_circle_offset is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_circle_offset of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_ego_circle_offset is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_offset_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_circle_offset to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_offset = (float32_T) (TA_MAX_K_TA_EGO_CIRCLE_OFFSET + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_circle_offset is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_circle_offset of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_ego_circle_offset is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_offset_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_circle_offset to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_offset = (float32_T) (TA_MIN_K_TA_EGO_CIRCLE_OFFSET);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_circle_offset is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_circle_offset of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_ego_circle_offset is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_offset_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_circle_offset to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_offset = (float32_T) (TA_MAX_K_TA_EGO_CIRCLE_OFFSET);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_circle_offset is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_circle_offset of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_ego_circle_offset is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_offset_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_circle_offset to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_circle_offset =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_CIRCLE_OFFSET + TA_MAX_K_TA_EGO_CIRCLE_OFFSET)));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_circle_offset is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_circle_host_length_factor of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_circle_host_length_factor is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_host_length_factor_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_circle_host_length_factor to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_host_length_factor = (float32_T) (TA_MIN_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_circle_host_length_factor is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_circle_host_length_factor of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_circle_host_length_factor is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_host_length_factor_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_circle_host_length_factor to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_host_length_factor = (float32_T) (TA_MAX_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_circle_host_length_factor is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_circle_host_length_factor of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_circle_host_length_factor is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_host_length_factor_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_circle_host_length_factor to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_host_length_factor = (float32_T) (TA_MIN_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_circle_host_length_factor is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_circle_host_length_factor of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_circle_host_length_factor is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_host_length_factor_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_circle_host_length_factor to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_circle_host_length_factor = (float32_T) (TA_MAX_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_circle_host_length_factor is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_circle_host_length_factor of component Ta_Trajectory_Prediction which checks 1-dimensional is within its
 * specified range. Here k_ta_ego_circle_host_length_factor is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_circle_host_length_factor_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_circle_host_length_factor to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_circle_host_length_factor =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR + TA_MAX_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR)));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_circle_host_length_factor is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawangle_integration_yawrate_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_yawangle_integration_yawrate_min is set to a value less than its minimum boundary.
 * Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawangle_integration_yawrate_min_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_yawangle_integration_yawrate_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawangle_integration_yawrate_min = (float32_T) (TA_MIN_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_yawangle_integration_yawrate_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_yawangle_integration_yawrate_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_yawangle_integration_yawrate_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawangle_integration_yawrate_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_yawangle_integration_yawrate_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawangle_integration_yawrate_min = (float32_T) (TA_MAX_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_yawangle_integration_yawrate_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_yawangle_integration_yawrate_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_yawangle_integration_yawrate_min is set to a value equal to its lower boundary. Thus
 * true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawangle_integration_yawrate_min_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_yawangle_integration_yawrate_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawangle_integration_yawrate_min = (float32_T) (TA_MIN_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawangle_integration_yawrate_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawangle_integration_yawrate_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_yawangle_integration_yawrate_min is set to a value equal to its upper boundary. Thus
 * true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawangle_integration_yawrate_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_yawangle_integration_yawrate_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_yawangle_integration_yawrate_min = (float32_T) (TA_MAX_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawangle_integration_yawrate_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_yawangle_integration_yawrate_min of component Ta_Trajectory_Prediction which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_yawangle_integration_yawrate_min is set to a value equal to mid of its boundaries.
 * Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_yawangle_integration_yawrate_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_yawangle_integration_yawrate_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_yawangle_integration_yawrate_min = (float32_T) ((
      float32_T) (0.5f * (TA_MIN_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN + TA_MAX_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN)));
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_yawangle_integration_yawrate_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_shape_gain_per_pred_step is set to a value less than its minimum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_per_pred_step_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_per_pred_step to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_per_pred_step = (float32_T) (TA_MIN_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_shape_gain_per_pred_step is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_shape_gain_per_pred_step is set to a value greater than its maximum boundary. Thus
 * false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_per_pred_step_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_per_pred_step to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_per_pred_step = (float32_T) (TA_MAX_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_shape_gain_per_pred_step is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_shape_gain_per_pred_step is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_per_pred_step_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_per_pred_step to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_per_pred_step = (float32_T) (TA_MIN_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_shape_gain_per_pred_step is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_shape_gain_per_pred_step is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_per_pred_step_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_per_pred_step to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_per_pred_step = (float32_T) (TA_MAX_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_shape_gain_per_pred_step is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_shape_gain_per_pred_step is set to a value equal to mid of its boundaries. Thus true
 * is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_per_pred_step_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_shape_gain_per_pred_step to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_per_pred_step =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP + TA_MAX_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP)));
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_shape_gain_per_pred_step is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_obj_shape_gain_per_pred_step is set to a value less than its minimum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_per_pred_step_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_per_pred_step to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_per_pred_step = (float32_T) (TA_MIN_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_shape_gain_per_pred_step is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_obj_shape_gain_per_pred_step is set to a value greater than its maximum boundary. Thus
 * false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_per_pred_step_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_per_pred_step to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_per_pred_step = (float32_T) (TA_MAX_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_shape_gain_per_pred_step is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_obj_shape_gain_per_pred_step is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_per_pred_step_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_per_pred_step to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_per_pred_step = (float32_T) (TA_MIN_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_shape_gain_per_pred_step is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_obj_shape_gain_per_pred_step is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_per_pred_step_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_per_pred_step to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_per_pred_step = (float32_T) (TA_MAX_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_shape_gain_per_pred_step is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_obj_shape_gain_per_pred_step is set to a value equal to mid of its boundaries. Thus true
 * is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_per_pred_step_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_obj_shape_gain_per_pred_step to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_per_pred_step =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP + TA_MAX_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP)));
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_shape_gain_per_pred_step is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_ego_shape_gain_fixed is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_fixed_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_fixed to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_fixed = (float32_T) (TA_MIN_K_TA_EGO_SHAPE_GAIN_FIXED - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_shape_gain_fixed is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_ego_shape_gain_fixed is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_fixed_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_fixed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_fixed = (float32_T) (TA_MAX_K_TA_EGO_SHAPE_GAIN_FIXED + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_shape_gain_fixed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_ego_shape_gain_fixed is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_fixed_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_fixed to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_fixed = (float32_T) (TA_MIN_K_TA_EGO_SHAPE_GAIN_FIXED);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_shape_gain_fixed is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_ego_shape_gain_fixed is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_fixed_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_fixed to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_fixed = (float32_T) (TA_MAX_K_TA_EGO_SHAPE_GAIN_FIXED);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_shape_gain_fixed is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_ego_shape_gain_fixed is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_fixed_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_ego_shape_gain_fixed to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_fixed =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SHAPE_GAIN_FIXED + TA_MAX_K_TA_EGO_SHAPE_GAIN_FIXED)));
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_ego_shape_gain_fixed is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_obj_shape_gain_fixed is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_fixed_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_fixed to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_fixed = (float32_T) (TA_MIN_K_TA_OBJ_SHAPE_GAIN_FIXED - EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_shape_gain_fixed is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_obj_shape_gain_fixed is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_fixed_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_fixed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_fixed = (float32_T) (TA_MAX_K_TA_OBJ_SHAPE_GAIN_FIXED + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_shape_gain_fixed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_obj_shape_gain_fixed is set to a value equal to its lower boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_fixed_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_fixed to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_fixed = (float32_T) (TA_MIN_K_TA_OBJ_SHAPE_GAIN_FIXED);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_shape_gain_fixed is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_obj_shape_gain_fixed is set to a value equal to its upper boundary. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_fixed_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_obj_shape_gain_fixed to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_fixed = (float32_T) (TA_MAX_K_TA_OBJ_SHAPE_GAIN_FIXED);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_shape_gain_fixed is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_shape_gain_fixed of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is within
 * its specified range. Here k_ta_obj_shape_gain_fixed is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_shape_gain_fixed_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_obj_shape_gain_fixed to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_obj_shape_gain_fixed =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_SHAPE_GAIN_FIXED + TA_MAX_K_TA_OBJ_SHAPE_GAIN_FIXED)));
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_shape_gain_fixed is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj of component Ta_Warn_Level_Logic which checks 1-dimensional
 * is within its specified range. Here k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to a value less than its
 * minimum boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj =
      (boolean_T) (TA_MIN_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ - 1u);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to value less than its minimum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj of component Ta_Warn_Level_Logic which checks 1-dimensional
 * is within its specified range. Here k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to a value greater than its
 * maximum boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj =
      (boolean_T) (TA_MAX_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ + 1u);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to value greater than its maximum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj of component Ta_Warn_Level_Logic which checks 1-dimensional
 * is within its specified range. Here k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to a value equal to its lower
 * boundary. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj =
      (boolean_T) (TA_MIN_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to value equal to its lower
    * boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj of component Ta_Warn_Level_Logic which checks 1-dimensional
 * is within its specified range. Here k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to a value equal to its upper
 * boundary. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj =
      (boolean_T) (TA_MAX_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to value equal to its upper
    * boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj of component Ta_Warn_Level_Logic which checks 1-dimensional
 * is within its specified range. Here k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to a value equal to mid of its
 * boundaries. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj =
      (boolean_T) ((boolean_T) (0.5f
                                * (TA_MIN_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ
                                   + TA_MAX_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to value equal to mid of its
    * boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_active_obj_ttp_offset of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_active_obj_ttp_offset is set to a value less than its minimum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_active_obj_ttp_offset_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_active_obj_ttp_offset to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_active_obj_ttp_offset = (float32_T) (TA_MIN_K_TA_ACTIVE_OBJ_TTP_OFFSET - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_active_obj_ttp_offset is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_active_obj_ttp_offset of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_active_obj_ttp_offset is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_active_obj_ttp_offset_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_active_obj_ttp_offset to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_active_obj_ttp_offset = (float32_T) (TA_MAX_K_TA_ACTIVE_OBJ_TTP_OFFSET + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_active_obj_ttp_offset is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_active_obj_ttp_offset of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_active_obj_ttp_offset is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_active_obj_ttp_offset_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_active_obj_ttp_offset to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_active_obj_ttp_offset = (float32_T) (TA_MIN_K_TA_ACTIVE_OBJ_TTP_OFFSET);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_active_obj_ttp_offset is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_active_obj_ttp_offset of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_active_obj_ttp_offset is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_active_obj_ttp_offset_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_active_obj_ttp_offset to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_active_obj_ttp_offset = (float32_T) (TA_MAX_K_TA_ACTIVE_OBJ_TTP_OFFSET);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_active_obj_ttp_offset is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_active_obj_ttp_offset of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_active_obj_ttp_offset is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_active_obj_ttp_offset_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_active_obj_ttp_offset to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_active_obj_ttp_offset =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_ACTIVE_OBJ_TTP_OFFSET + TA_MAX_K_TA_ACTIVE_OBJ_TTP_OFFSET)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_active_obj_ttp_offset is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_apply_ttp_hysteresis_globally of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_f_apply_ttp_hysteresis_globally is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_apply_ttp_hysteresis_globally_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_f_apply_ttp_hysteresis_globally to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_f_apply_ttp_hysteresis_globally = (boolean_T) (TA_MIN_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY - 1u);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_apply_ttp_hysteresis_globally is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_apply_ttp_hysteresis_globally of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_f_apply_ttp_hysteresis_globally is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_apply_ttp_hysteresis_globally_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_f_apply_ttp_hysteresis_globally to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_f_apply_ttp_hysteresis_globally = (boolean_T) (TA_MAX_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY + 1u);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_apply_ttp_hysteresis_globally is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_apply_ttp_hysteresis_globally of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_f_apply_ttp_hysteresis_globally is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_apply_ttp_hysteresis_globally_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_f_apply_ttp_hysteresis_globally to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_f_apply_ttp_hysteresis_globally = (boolean_T) (TA_MIN_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_apply_ttp_hysteresis_globally is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_apply_ttp_hysteresis_globally of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_f_apply_ttp_hysteresis_globally is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_apply_ttp_hysteresis_globally_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_f_apply_ttp_hysteresis_globally to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_f_apply_ttp_hysteresis_globally = (boolean_T) (TA_MAX_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_apply_ttp_hysteresis_globally is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_f_apply_ttp_hysteresis_globally of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_f_apply_ttp_hysteresis_globally is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_f_apply_ttp_hysteresis_globally_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_f_apply_ttp_hysteresis_globally to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_f_apply_ttp_hysteresis_globally =
      (boolean_T) ((boolean_T) (0.5f * (TA_MIN_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY + TA_MAX_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_f_apply_ttp_hysteresis_globally is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to a value less than its minimum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max =
      (float32_T) (TA_MIN_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max =
      (float32_T) (TA_MAX_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to value greater than its maximum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to a value equal to its lower boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max =
      (float32_T) (TA_MIN_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to a value equal to its upper boundary.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max =
      (float32_T) (TA_MAX_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to a value equal to mid of its
 * boundaries. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max =
      (float32_T) ((float32_T) (0.5f
                                * (TA_MIN_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX
                                   + TA_MAX_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to a value less than its minimum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max =
      (float32_T) (TA_MIN_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to value less than its minimum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max =
      (float32_T) (TA_MAX_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to value greater than its maximum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to a value equal to its lower
 * boundary. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max =
      (float32_T) (TA_MIN_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to a value equal to its upper
 * boundary. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max =
      (float32_T) (TA_MAX_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max of component Ta_Warn_Level_Logic which checks 1-dimensional is
 * within its specified range. Here k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to a value equal to mid of its
 * boundaries. Thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max =
      (float32_T) ((float32_T) (0.5f
                                * (TA_MIN_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX
                                   + TA_MAX_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max is set to value equal to mid of its
    * boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_2_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_2_ttc_threshold is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_2_ttc_threshold_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_2_ttc_threshold to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_2_ttc_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_2_TTC_THRESHOLD - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_2_ttc_threshold is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_2_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_2_ttc_threshold is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_2_ttc_threshold_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_2_ttc_threshold to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_2_ttc_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_2_TTC_THRESHOLD + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_2_ttc_threshold is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_2_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_2_ttc_threshold is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_2_ttc_threshold_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_2_ttc_threshold to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_2_ttc_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_2_TTC_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_2_ttc_threshold is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_2_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_2_ttc_threshold is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_2_ttc_threshold_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_2_ttc_threshold to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_2_ttc_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_2_TTC_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_2_ttc_threshold is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_2_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_2_ttc_threshold is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_2_ttc_threshold_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_2_ttc_threshold to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_2_ttc_threshold =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_2_TTC_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_2_TTC_THRESHOLD)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_2_ttc_threshold is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttc_threshold is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttc_threshold_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttc_threshold to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttc_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_3_TTC_THRESHOLD - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_3_ttc_threshold is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttc_threshold is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttc_threshold_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttc_threshold to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttc_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_3_TTC_THRESHOLD + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_3_ttc_threshold is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttc_threshold is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttc_threshold_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttc_threshold to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttc_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_3_TTC_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_3_ttc_threshold is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttc_threshold is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttc_threshold_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttc_threshold to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttc_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_3_TTC_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_3_ttc_threshold is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttc_threshold is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttc_threshold_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_3_ttc_threshold to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttc_threshold =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_3_TTC_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_3_TTC_THRESHOLD)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_3_ttc_threshold is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttb_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttb_threshold is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttb_threshold_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttb_threshold to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttb_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_3_TTB_THRESHOLD - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_3_ttb_threshold is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttb_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttb_threshold is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttb_threshold_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttb_threshold to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttb_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_3_TTB_THRESHOLD + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_3_ttb_threshold is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttb_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttb_threshold is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttb_threshold_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttb_threshold to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttb_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_3_TTB_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_3_ttb_threshold is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttb_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttb_threshold is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttb_threshold_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_3_ttb_threshold to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttb_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_3_TTB_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_3_ttb_threshold is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_3_ttb_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_3_ttb_threshold is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_3_ttb_threshold_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_3_ttb_threshold to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_3_ttb_threshold =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_3_TTB_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_3_TTB_THRESHOLD)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_3_ttb_threshold is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_4_ttc_threshold is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_ttc_threshold_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_ttc_threshold to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_ttc_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_4_TTC_THRESHOLD - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_4_ttc_threshold is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_4_ttc_threshold is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_ttc_threshold_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_ttc_threshold to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_ttc_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_4_TTC_THRESHOLD + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_4_ttc_threshold is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_4_ttc_threshold is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_ttc_threshold_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_ttc_threshold to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_ttc_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_4_TTC_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_4_ttc_threshold is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_4_ttc_threshold is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_ttc_threshold_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_ttc_threshold to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_ttc_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_4_TTC_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_4_ttc_threshold is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_ttc_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_lvl_4_ttc_threshold is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_ttc_threshold_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_4_ttc_threshold to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_ttc_threshold =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_4_TTC_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_4_TTC_THRESHOLD)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_4_ttc_threshold is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_decel_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_alert_lvl_4_decel_threshold is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_decel_threshold_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_decel_threshold to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_decel_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_4_DECEL_THRESHOLD - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_4_decel_threshold is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_decel_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_alert_lvl_4_decel_threshold is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_decel_threshold_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_decel_threshold to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_decel_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_4_DECEL_THRESHOLD + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_4_decel_threshold is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_decel_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_alert_lvl_4_decel_threshold is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_decel_threshold_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_decel_threshold to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_decel_threshold = (float32_T) (TA_MIN_K_TA_ALERT_LVL_4_DECEL_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_4_decel_threshold is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_decel_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_alert_lvl_4_decel_threshold is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_decel_threshold_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_4_decel_threshold to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_decel_threshold = (float32_T) (TA_MAX_K_TA_ALERT_LVL_4_DECEL_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_4_decel_threshold is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_4_decel_threshold of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_alert_lvl_4_decel_threshold is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_4_decel_threshold_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_4_decel_threshold to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_4_decel_threshold =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_4_DECEL_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_4_DECEL_THRESHOLD)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_4_decel_threshold is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_min_safe_distance of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_min_safe_distance is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_min_safe_distance_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_critical_approach_min_safe_distance to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_min_safe_distance = (float32_T) (TA_MIN_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_critical_approach_min_safe_distance is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_critical_approach_min_safe_distance of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_min_safe_distance is set to a value greater than its maximum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_min_safe_distance_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_critical_approach_min_safe_distance to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_min_safe_distance = (float32_T) (TA_MAX_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_critical_approach_min_safe_distance is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_critical_approach_min_safe_distance of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_min_safe_distance is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_min_safe_distance_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_critical_approach_min_safe_distance to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_min_safe_distance = (float32_T) (TA_MIN_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_min_safe_distance is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_min_safe_distance of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_min_safe_distance is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_min_safe_distance_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_critical_approach_min_safe_distance to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_min_safe_distance = (float32_T) (TA_MAX_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_min_safe_distance is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_min_safe_distance of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_min_safe_distance is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_min_safe_distance_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_critical_approach_min_safe_distance to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_critical_approach_min_safe_distance = (float32_T) ((
      float32_T) (0.5f * (TA_MIN_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE + TA_MAX_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_min_safe_distance is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_angle_diff_min of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_angle_diff_min is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_angle_diff_min_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_critical_approach_angle_diff_min to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_angle_diff_min = (float32_T) (TA_MIN_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_critical_approach_angle_diff_min is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_critical_approach_angle_diff_min of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_angle_diff_min is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_angle_diff_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_critical_approach_angle_diff_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_angle_diff_min = (float32_T) (TA_MAX_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_critical_approach_angle_diff_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_critical_approach_angle_diff_min of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_angle_diff_min is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_angle_diff_min_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_critical_approach_angle_diff_min to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_angle_diff_min = (float32_T) (TA_MIN_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_angle_diff_min is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_angle_diff_min of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_angle_diff_min is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_angle_diff_min_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_critical_approach_angle_diff_min to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_angle_diff_min = (float32_T) (TA_MAX_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_angle_diff_min is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_critical_approach_angle_diff_min of component Ta_Warn_Level_Logic which checks 1-dimensional is within its
 * specified range. Here k_ta_critical_approach_angle_diff_min is set to a value equal to mid of its boundaries. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_critical_approach_angle_diff_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_critical_approach_angle_diff_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_critical_approach_angle_diff_min = (float32_T) ((
      float32_T) (0.5f * (TA_MIN_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN + TA_MAX_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_critical_approach_angle_diff_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_f_higher_obj_crit_based_on_lower_ttp of component Ta_Warn_Level_Logic which checks 1-dimensional is within
 * its specified range. Here k_rta_f_higher_obj_crit_based_on_lower_ttp is set to a value less than its minimum boundary. Thus
 * false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_f_higher_obj_crit_based_on_lower_ttp_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_rta_f_higher_obj_crit_based_on_lower_ttp to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp = (boolean_T) (TA_MIN_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP - 1u);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_f_higher_obj_crit_based_on_lower_ttp is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_f_higher_obj_crit_based_on_lower_ttp of component Ta_Warn_Level_Logic which checks 1-dimensional is within
 * its specified range. Here k_rta_f_higher_obj_crit_based_on_lower_ttp is set to a value greater than its maximum boundary. Thus
 * false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_f_higher_obj_crit_based_on_lower_ttp_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_f_higher_obj_crit_based_on_lower_ttp to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp = (boolean_T) (TA_MAX_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP + 1u);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_f_higher_obj_crit_based_on_lower_ttp is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_f_higher_obj_crit_based_on_lower_ttp of component Ta_Warn_Level_Logic which checks 1-dimensional is within
 * its specified range. Here k_rta_f_higher_obj_crit_based_on_lower_ttp is set to a value equal to its lower boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_f_higher_obj_crit_based_on_lower_ttp_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_rta_f_higher_obj_crit_based_on_lower_ttp to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp = (boolean_T) (TA_MIN_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_f_higher_obj_crit_based_on_lower_ttp is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_f_higher_obj_crit_based_on_lower_ttp of component Ta_Warn_Level_Logic which checks 1-dimensional is within
 * its specified range. Here k_rta_f_higher_obj_crit_based_on_lower_ttp is set to a value equal to its upper boundary. Thus true is
 * expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_f_higher_obj_crit_based_on_lower_ttp_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_rta_f_higher_obj_crit_based_on_lower_ttp to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp = (boolean_T) (TA_MAX_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_f_higher_obj_crit_based_on_lower_ttp is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_rta_f_higher_obj_crit_based_on_lower_ttp of component Ta_Warn_Level_Logic which checks 1-dimensional is within
 * its specified range. Here k_rta_f_higher_obj_crit_based_on_lower_ttp is set to a value equal to mid of its boundaries. Thus true
 * is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries__k_rta_f_higher_obj_crit_based_on_lower_ttp_dim_1_middle_of_road_test)
{
   /** \arrange setup k_rta_f_higher_obj_crit_based_on_lower_ttp to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp = (boolean_T) ((
      boolean_T) (0.5f * (TA_MIN_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP + TA_MAX_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP)));
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_rta_f_higher_obj_crit_based_on_lower_ttp is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_threshold of component Ta_Warn_Level_Logic which checks 3-dimensional is within its specified
 * range. Here k_ta_alert_lvl_1_ttp_threshold is set to a value less than its minimum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_threshold_dim_3_lt_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_threshold to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_threshold[0] = (float32_T) (TA_MIN_K_TA_ALERT_LVL_1_TTP_THRESHOLD - EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_1_ttp_threshold is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_threshold of component Ta_Warn_Level_Logic which checks 3-dimensional is within its specified
 * range. Here k_ta_alert_lvl_1_ttp_threshold is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_threshold_dim_3_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_threshold to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_threshold[0] = (float32_T) (TA_MAX_K_TA_ALERT_LVL_1_TTP_THRESHOLD + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_1_ttp_threshold is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_threshold of component Ta_Warn_Level_Logic which checks 3-dimensional is within its specified
 * range. Here k_ta_alert_lvl_1_ttp_threshold is set to a value equal to its lower boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_threshold_dim_3_eq_lower_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_threshold to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_threshold[0] = (float32_T) (TA_MIN_K_TA_ALERT_LVL_1_TTP_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_threshold is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_threshold of component Ta_Warn_Level_Logic which checks 3-dimensional is within its specified
 * range. Here k_ta_alert_lvl_1_ttp_threshold is set to a value equal to its upper boundary. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_threshold_dim_3_eq_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_threshold to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_threshold[0] = (float32_T) (TA_MAX_K_TA_ALERT_LVL_1_TTP_THRESHOLD);
   /** \action Execute Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_threshold is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_threshold of component Ta_Warn_Level_Logic which checks 3-dimensional is within its specified
 * range. Here k_ta_alert_lvl_1_ttp_threshold is set to a value equal to mid of its boundaries. Thus true is expected. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_threshold_dim_3_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_threshold to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_threshold[0] =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_1_TTP_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_1_TTP_THRESHOLD)));
   /** \action Execute Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_lvl_1_ttp_threshold is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_acceleration_long_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_long_weight is set to a value less than its minimum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_long_weight_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_long_weight to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_long_weight = (float32_T) (TA_MIN_K_TA_OBJ_ACCELERATION_LONG_WEIGHT - EPSILON);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_acceleration_long_weight is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_acceleration_long_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_long_weight is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_long_weight_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_long_weight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_long_weight = (float32_T) (TA_MAX_K_TA_OBJ_ACCELERATION_LONG_WEIGHT + EPSILON);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_acceleration_long_weight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_acceleration_long_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_long_weight is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_long_weight_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_long_weight to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_long_weight = (float32_T) (TA_MIN_K_TA_OBJ_ACCELERATION_LONG_WEIGHT);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_acceleration_long_weight is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_acceleration_long_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_long_weight is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_long_weight_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_long_weight to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_long_weight = (float32_T) (TA_MAX_K_TA_OBJ_ACCELERATION_LONG_WEIGHT);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_acceleration_long_weight is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_acceleration_long_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_long_weight is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_long_weight_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_obj_acceleration_long_weight to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_long_weight =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_ACCELERATION_LONG_WEIGHT + TA_MAX_K_TA_OBJ_ACCELERATION_LONG_WEIGHT)));
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_acceleration_long_weight is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_acceleration_lat_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_lat_weight is set to a value less than its minimum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_lat_weight_dim_1_lt_lower_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_lat_weight to value less than its minimum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_lat_weight = (float32_T) (TA_MIN_K_TA_OBJ_ACCELERATION_LAT_WEIGHT - EPSILON);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_acceleration_lat_weight is set to value less than its minimum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_acceleration_lat_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_lat_weight is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_lat_weight_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_lat_weight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_lat_weight = (float32_T) (TA_MAX_K_TA_OBJ_ACCELERATION_LAT_WEIGHT + EPSILON);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_acceleration_lat_weight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_acceleration_lat_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_lat_weight is set to a value equal to its lower boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_lat_weight_dim_1_eq_lower_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_lat_weight to value equal to its lower boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_lat_weight = (float32_T) (TA_MIN_K_TA_OBJ_ACCELERATION_LAT_WEIGHT);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_acceleration_lat_weight is set to value equal to its lower boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_acceleration_lat_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_lat_weight is set to a value equal to its upper boundary. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_lat_weight_dim_1_eq_upper_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_lat_weight to value equal to its upper boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_lat_weight = (float32_T) (TA_MAX_K_TA_OBJ_ACCELERATION_LAT_WEIGHT);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_acceleration_lat_weight is set to value equal to its upper boundary.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_obj_acceleration_lat_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_lat_weight is set to a value equal to mid of its boundaries. Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_lat_weight_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_obj_acceleration_lat_weight to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_lat_weight =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_ACCELERATION_LAT_WEIGHT + TA_MAX_K_TA_OBJ_ACCELERATION_LAT_WEIGHT)));
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_obj_acceleration_lat_weight is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_qualifying_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_qualifying_cycles is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_alert_qualifying_cycles_dim_1_middle_of_road_test)
{
   /** \arrange setup k_ta_alert_qualifying_cycles to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_ta_alert_qualifying_cycles = (uint8_t) ((uint8_t) (0.5f * TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES));
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_ta_alert_qualifying_cycles is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_ta_alert_qualifying_cycles of component Ta_Alert_Debouncer which checks 1-dimensional is within its specified
 * range. Here k_ta_alert_qualifying_cycles is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_alert_qualifying_cycles_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_qualifying_cycles to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_qualifying_cycles = (uint8_t) (TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES + 1u);
   /** \action Execute Ta_Alert_Debouncer_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_qualifying_cycles is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_always_overwrite_ta_mode_to_both of component Ta_Enable_Flags which checks 1-dimensional is within its
 * specified range. Here k_ta_always_overwrite_ta_mode_to_both is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_always_overwrite_ta_mode_to_both_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_always_overwrite_ta_mode_to_both to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_always_overwrite_ta_mode_to_both = (boolean_T) (TA_MAX_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH + 1u);
   /** \action Execute Ta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_always_overwrite_ta_mode_to_both is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_critical_approach_check_ego_circles of component Ta_Enable_Flags which checks 3-dimensional is within its
 * specified range. Here k_ta_critical_approach_check_ego_circles is set to a value greater than its maximum boundary. Thus false
 * is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_critical_approach_check_ego_circles_dim_3_gt_upper_boundary)
{
   /** \arrange setup k_ta_critical_approach_check_ego_circles to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_critical_approach_check_ego_circles[0] = (uint8_t) (TA_MAX_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES + 1u);
   /** \action Execute Ta_Enable_Flags_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_critical_approach_check_ego_circles is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_speed of component Ta_Host_State_Validator which checks 2-dimensional is within its specified range.
 * Here k_ta_ego_speed is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Module_Are_Calibration_Within_Boundaries__k_ta_ego_speed_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_speed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_speed[0] = (float32_T) (TA_MAX_K_TA_EGO_SPEED + EPSILON);
   /** \action Execute Ta_Host_State_Validator_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_speed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_straight_host_curvature_max of component Ta_Object_Filter which checks 1-dimensional is within its specified
 * range. Here k_ta_straight_host_curvature_max is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_straight_host_curvature_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_straight_host_curvature_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_straight_host_curvature_max = (float32_T) (TA_MAX_K_TA_STRAIGHT_HOST_CURVATURE_MAX + EPSILON);
   /** \action Execute Ta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_straight_host_curvature_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_lookup_turning_host_speed of component Ta_Object_Filter_Lookup which checks 4-dimensional is within its
 * specified range. Here k_ta_lookup_turning_host_speed is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_lookup_turning_host_speed_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_ta_lookup_turning_host_speed to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_lookup_turning_host_speed[0] = (float32_T) (TA_MAX_K_TA_LOOKUP_TURNING_HOST_SPEED + EPSILON);
   /** \action Execute Ta_Object_Filter_Lookup_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_lookup_turning_host_speed is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_prediction_steps_max of component Ta_Trajectory_Prediction which checks 1-dimensional is within its specified
 * range. Here k_ta_prediction_steps_max is set to a value greater than its maximum boundary. Thus false is expected. \uts{} \sdd{}
 * \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_prediction_steps_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_prediction_steps_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_prediction_steps_max = (uint8_t) (TA_MAX_K_TA_PREDICTION_STEPS_MAX + 1u);
   /** \action Execute Ta_Trajectory_Prediction_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_prediction_steps_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_ego_shape_gain_per_pred_step of component Ta_Trajectory_Prediction_Gain_Factors which checks 1-dimensional is
 * within its specified range. Here k_ta_ego_shape_gain_per_pred_step is set to a value greater than its maximum boundary. Thus
 * false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_ego_shape_gain_per_pred_step_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_ego_shape_gain_per_pred_step to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_ego_shape_gain_per_pred_step = (float32_T) (TA_MAX_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP + EPSILON);
   /** \action Execute Ta_Trajectory_Prediction_Gain_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_ego_shape_gain_per_pred_step is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj of component Ta_Warn_Level_Logic which checks 1-dimensional
 * is within its specified range. Here k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to a value greater than its
 * maximum boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj =
      (boolean_T) (TA_MAX_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ + 1u);
   /** \action Execute Ta_Warn_Level_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj is set to value greater than its maximum
    * boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_alert_lvl_1_ttp_threshold of component Ta_Warn_Level_Logic which checks 3-dimensional is within its specified
 * range. Here k_ta_alert_lvl_1_ttp_threshold is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_alert_lvl_1_ttp_threshold_dim_3_gt_upper_boundary)
{
   /** \arrange setup k_ta_alert_lvl_1_ttp_threshold to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_alert_lvl_1_ttp_threshold[0] = (float32_T) (TA_MAX_K_TA_ALERT_LVL_1_TTP_THRESHOLD + EPSILON);
   /** \action Execute Ta_Warn_Level_Logic_3_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_alert_lvl_1_ttp_threshold is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_ta_obj_acceleration_long_weight of component Ta_Weighting_Factors which checks 1-dimensional is within its
 * specified range. Here k_ta_obj_acceleration_long_weight is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Module_Are_Calibration_Within_Boundaries__k_ta_obj_acceleration_long_weight_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_ta_obj_acceleration_long_weight to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_ta_obj_acceleration_long_weight = (float32_T) (TA_MAX_K_TA_OBJ_ACCELERATION_LONG_WEIGHT + EPSILON);
   /** \action Execute Ta_Weighting_Factors_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_ta_obj_acceleration_long_weight is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value equal to mid of its boundaries. Thus true is expected. \uts{} \sdd{}
 * \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_brake_deceleration_max_dim_1_middle_of_road_test)
{
   /** \arrange setup k_fta_brake_deceleration_max to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_FTA_BRAKE_DECELERATION_MAX + TA_MAX_K_FTA_BRAKE_DECELERATION_MAX)));
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_fta_brake_deceleration_max is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_fta_brake_deceleration_max of component Ta_Fta_Braking_Logic which checks 1-dimensional is within its specified
 * range. Here k_fta_brake_deceleration_max is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_brake_deceleration_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_brake_deceleration_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_brake_deceleration_max = (float32_T) (TA_MAX_K_FTA_BRAKE_DECELERATION_MAX + EPSILON);
   /** \action Execute Ta_Fta_Braking_Logic_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_brake_deceleration_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_fta_enable of component Ta_Fta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_fta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_f_fta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_fta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_fta_enable = (boolean_T) (TA_MAX_K_F_FTA_ENABLE + 1u);
   /** \action Execute Ta_Fta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_fta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_pos_straight_min of component Ta_Fta_Object_Filter which checks 1-dimensional is within its
 * specified range. Here k_fta_obj_vcs_long_pos_straight_min is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_pos_straight_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_pos_straight_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_pos_straight_min = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_pos_straight_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty of component Ta_Fta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_fta_obj_exist_prblty is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty[0] = (float32_T) (TA_MAX_K_FTA_OBJ_EXIST_PRBLTY + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_exist_prblty is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_velocity_heading_diff_max of component Ta_Fta_Object_Filter_Offsets which checks 1-dimensional is within
 * its specified range. Here k_fta_obj_velocity_heading_diff_max is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_obj_velocity_heading_diff_max_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_velocity_heading_diff_max to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_velocity_heading_diff_max = (float32_T) (TA_MAX_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_velocity_heading_diff_max is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_exist_prblty_ofst of component Ta_Fta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_fta_obj_exist_prblty_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_obj_exist_prblty_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_exist_prblty_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_EXIST_PRBLTY_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_exist_prblty_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel_ofst of component Ta_Fta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_fta_obj_vcs_long_vel_rel_ofst is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_rel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_obj_vcs_long_vel_rel of component Ta_Fta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_fta_obj_vcs_long_vel_rel is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_obj_vcs_long_vel_rel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_fta_obj_vcs_long_vel_rel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL + EPSILON);
   /** \action Execute Ta_Fta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_obj_vcs_long_vel_rel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_point_size of component Ta_Fta_Zones which checks 1-dimensional is within its specified range.
 * Here k_fta_danger_zone_point_size is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_danger_zone_point_size_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_point_size to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_point_size = (uint8_t) (TA_MAX_K_FTA_DANGER_ZONE_POINT_SIZE + 1u);
   /** \action Execute Ta_Fta_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_point_size is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_fta_danger_zone_left_long of component Ta_Fta_Zones which checks 7-dimensional is within its specified range.
 * Here k_fta_danger_zone_left_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Fta_Module_Are_Calibration_Within_Boundaries__k_fta_danger_zone_left_long_dim_7_gt_upper_boundary)
{
   /** \arrange setup k_fta_danger_zone_left_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_fta_danger_zone_left_long[0] = (float32_T) (TA_MAX_K_FTA_DANGER_ZONE_LEFT_LONG + EPSILON);
   /** \action Execute Ta_Fta_Zones_7_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_fta_danger_zone_left_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value equal to mid of its boundaries.
 * Thus true is expected.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_tap_lvl_2_host_curvature_min_dim_1_middle_of_road_test)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value equal to mid of its boundaries.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min =
      (float32_T) ((float32_T) (0.5f * (TA_MIN_K_TAP_LVL_2_HOST_CURVATURE_MIN + TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN)));
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect true since k_tap_lvl_2_host_curvature_min is set to value equal to mid of its boundaries.*/
   EXPECT_TRUE(result);
}

/**
 * Check whether k_tap_lvl_2_host_curvature_min of component Ta_Post_Run which checks 1-dimensional is within its specified range.
 * Here k_tap_lvl_2_host_curvature_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_tap_lvl_2_host_curvature_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_tap_lvl_2_host_curvature_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_tap_lvl_2_host_curvature_min = (float32_T) (TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN + EPSILON);
   /** \action Execute Ta_Post_Run_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_tap_lvl_2_host_curvature_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_f_rta_enable of component Ta_Rta_Enable_Flags which checks 1-dimensional is within its specified range.
 * Here k_f_rta_enable is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_f_rta_enable_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_f_rta_enable to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_f_rta_enable = (boolean_T) (TA_MAX_K_F_RTA_ENABLE + 1u);
   /** \action Execute Ta_Rta_Enable_Flags_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_f_rta_enable is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_point_size of component Ta_Rta_Info_Zone which checks 1-dimensional is within its specified range.
 * Here k_rta_info_zone_point_size is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_info_zone_point_size_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_point_size to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_point_size = (uint8_t) (TA_MAX_K_RTA_INFO_ZONE_POINT_SIZE + 1u);
   /** \action Execute Ta_Rta_Info_Zone_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_point_size is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_info_zone_left_long of component Ta_Rta_Info_Zone which checks 4-dimensional is within its specified range.
 * Here k_rta_info_zone_left_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_info_zone_left_long_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_info_zone_left_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_info_zone_left_long[0] = (float32_T) (TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG + EPSILON);
   /** \action Execute Ta_Rta_Info_Zone_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_info_zone_left_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_age_min of component Ta_Rta_Object_Filter which checks 1-dimensional is within its specified range.
 * Here k_rta_obj_age_min is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test, Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_obj_age_min_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_age_min to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_age_min = (uint8_t) (TA_MAX_K_RTA_OBJ_AGE_MIN + 1u);
   /** \action Execute Ta_Rta_Object_Filter_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_age_min is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty of component Ta_Rta_Object_Filter which checks 2-dimensional is within its specified range.
 * Here k_rta_obj_exist_prblty is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty[0] = (float32_T) (TA_MAX_K_RTA_OBJ_EXIST_PRBLTY + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_exist_prblty is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_exist_prblty_ofst of component Ta_Rta_Object_Filter_Offsets which checks 2-dimensional is within its
 * specified range. Here k_rta_obj_exist_prblty_ofst is set to a value greater than its maximum boundary. Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_obj_exist_prblty_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_exist_prblty_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_exist_prblty_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_EXIST_PRBLTY_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_exist_prblty_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel_ofst of component Ta_Rta_Object_Filter_Offsets_Vcs_Properties which checks
 * 2-dimensional is within its specified range. Here k_rta_obj_vcs_long_vel_rel_ofst is set to a value greater than its maximum
 * boundary. Thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_ofst_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel_ofst to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel_ofst[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Offsets_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_rel_ofst is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_obj_vcs_long_vel_rel of component Ta_Rta_Object_Filter_Vcs_Properties which checks 2-dimensional is within
 * its specified range. Here k_rta_obj_vcs_long_vel_rel is set to a value greater than its maximum boundary. Thus false is
 * expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_obj_vcs_long_vel_rel_dim_2_gt_upper_boundary)
{
   /** \arrange setup k_rta_obj_vcs_long_vel_rel to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_obj_vcs_long_vel_rel[0] = (float32_T) (TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL + EPSILON);
   /** \action Execute Ta_Rta_Object_Filter_Vcs_Properties_2_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_obj_vcs_long_vel_rel is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_point_size of component Ta_Rta_Wing_Zones which checks 1-dimensional is within its specified
 * range. Here k_rta_wing_zone_point_size is set to a value greater than its maximum boundary. Thus false is expected. \uts{}
 * \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_wing_zone_point_size_dim_1_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_point_size to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_point_size = (uint8_t) (TA_MAX_K_RTA_WING_ZONE_POINT_SIZE + 1u);
   /** \action Execute Ta_Rta_Wing_Zones_1_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_point_size is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}

/**
 * Check whether k_rta_wing_zone_left_long of component Ta_Rta_Wing_Zones which checks 4-dimensional is within its specified range.
 * Here k_rta_wing_zone_left_long is set to a value greater than its maximum boundary.
 * Thus false is expected.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Calibration_Boundary_Check_Test,
       Ta_Rta_Module_Are_Calibration_Within_Boundaries__k_rta_wing_zone_left_long_dim_4_gt_upper_boundary)
{
   /** \arrange setup k_rta_wing_zone_left_long to value greater than its maximum boundary.*/
   boolean_T result;
   calibration.k_rta_wing_zone_left_long[0] = (float32_T) (TA_MAX_K_RTA_WING_ZONE_LEFT_LONG + EPSILON);
   /** \action Execute Ta_Rta_Wing_Zones_4_Dimensional_Are_Calibration_Within_Boundaries.*/
   result = Ta_Core_Cal_In_Boundary(&calibration);
   /** \assert Expect false since k_rta_wing_zone_left_long is set to value greater than its maximum boundary.*/
   EXPECT_FALSE(result);
}
