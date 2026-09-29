/**
 * @file pt_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for PT calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43476}
 */

#include "pt_calibration_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>


extern "C"
{
#include "pa_reuse.h"
#include "pt_core_calibration.c"
#include "pt_core_calibration_check.h"
#include "pt_update_calibration.h"
#include <stdio.h>
#include <string.h>
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-43478} \sdd{CSCSA-216704} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Core_Calibration_Test, Pt_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Pt_Core_Cal_Print(p_testfile, &pt_cals));
}


/**
 * Test for calibration file. Run test to check basic Pt_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-73372} \sdd{CSCSA-216702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Core_Calibration_Test, Pt_Core_Cal_Update_Defaults__is_reseting_k_pt_kill_lane_change_min_diff_path_point)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value = pt_cals.k_pt_kill_lane_change_min_diff_path_point;
   pt_cals.k_pt_kill_lane_change_min_diff_path_point += 1;
   Pt_Core_Cal_Update_Defaults(&pt_cals);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, pt_cals.k_pt_kill_lane_change_min_diff_path_point);
}

/**
 * Test for calibration file. Run test to check basic Pt_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-73373} \sdd{CSCSA-216706} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Core_Calibration_Test, Pt_Update_Core_Cal_By_Core__is_setting_k_pt_kill_lane_change_min_diff_path_point)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value       = pt_cals.k_pt_kill_lane_change_min_diff_path_point;
   const uint8_t new_value           = orginal_value + 1;
   Pt_Core_Calibration_T calibration = pt_cals;

   calibration.k_pt_kill_lane_change_min_diff_path_point = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, pt_cals.k_pt_kill_lane_change_min_diff_path_point);
   boolean_T result = Pt_Update_Core_Cal_By_Core(&pt_cals, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, pt_cals.k_pt_kill_lane_change_min_diff_path_point);
}

template <typename T> using Single_Case_T = std::tuple<const T Pt_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Pt_Test_Pt_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Pt_Core_Calibration_T calibration;
   Pt_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Pt_Core_Cal_In_Boundary(&calibration));
      const T Pt_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                          = std::get<1>(single_case);
      const T max_value                          = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Pt_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Pt_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Pt_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Pt_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Pt_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-73374} \sdd{CSCSA-216698} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Core_Calibration_Test, Pt_Core_Cal_In_Boundary__Float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Pt_Core_Calibration_T::k_pt_zone_max_posn, PT_MIN_K_PT_ZONE_MAX_POSN, PT_MAX_K_PT_ZONE_MAX_POSN},
      {&Pt_Core_Calibration_T::k_pt_min_obj_speed, PT_MIN_K_PT_MIN_OBJ_SPEED, PT_MAX_K_PT_MIN_OBJ_SPEED},
      {&Pt_Core_Calibration_T::k_pt_overlap_max_match_value, PT_MIN_K_PT_OVERLAP_MAX_MATCH_VALUE, PT_MAX_K_PT_OVERLAP_MAX_MATCH_VALUE},
      {&Pt_Core_Calibration_T::k_pt_move_max_value, PT_MIN_K_PT_MOVE_MAX_VALUE, PT_MAX_K_PT_MOVE_MAX_VALUE},
      {&Pt_Core_Calibration_T::k_pt_group_max_match_value, PT_MIN_K_PT_GROUP_MAX_MATCH_VALUE, PT_MAX_K_PT_GROUP_MAX_MATCH_VALUE},
      {&Pt_Core_Calibration_T::k_pt_group_max_match_value_ad, PT_MIN_K_PT_GROUP_MAX_MATCH_VALUE_AD,
       PT_MAX_K_PT_GROUP_MAX_MATCH_VALUE_AD},
      {&Pt_Core_Calibration_T::k_pt_group_dir_max_diff_value, PT_MIN_K_PT_GROUP_DIR_MAX_DIFF_VALUE,
       PT_MAX_K_PT_GROUP_DIR_MAX_DIFF_VALUE},
      {&Pt_Core_Calibration_T::k_pt_group_dir_max_avg_diff_value, PT_MIN_K_PT_GROUP_DIR_MAX_AVG_DIFF_VALUE,
       PT_MAX_K_PT_GROUP_DIR_MAX_AVG_DIFF_VALUE},
      {&Pt_Core_Calibration_T::k_pt_find_max_lat_posn, PT_MIN_K_PT_FIND_MAX_LAT_POSN, PT_MAX_K_PT_FIND_MAX_LAT_POSN},
      {&Pt_Core_Calibration_T::k_pt_find_min_speed, PT_MIN_K_PT_FIND_MIN_SPEED, PT_MAX_K_PT_FIND_MIN_SPEED},
      {&Pt_Core_Calibration_T::k_pt_find_max_long_posn, PT_MIN_K_PT_FIND_MAX_LONG_POSN, PT_MAX_K_PT_FIND_MAX_LONG_POSN},
      {&Pt_Core_Calibration_T::k_pt_path_change_match_hyst_default, PT_MIN_K_PT_PATH_CHANGE_MATCH_HYST_DEFAULT,
       PT_MAX_K_PT_PATH_CHANGE_MATCH_HYST_DEFAULT},
      {&Pt_Core_Calibration_T::k_pt_path_change_differing_states_hyst_default,
       PT_MIN_K_PT_PATH_CHANGE_DIFFERING_STATES_HYST_DEFAULT, PT_MAX_K_PT_PATH_CHANGE_DIFFERING_STATES_HYST_DEFAULT},
      {&Pt_Core_Calibration_T::k_pt_path_change_match_hyst_more_established, PT_MIN_K_PT_PATH_CHANGE_MATCH_HYST_MORE_ESTABLISHED,
       PT_MAX_K_PT_PATH_CHANGE_MATCH_HYST_MORE_ESTABLISHED},
      {&Pt_Core_Calibration_T::k_pt_path_change_one_grouped_one_mature, PT_MIN_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_MATURE,
       PT_MAX_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_MATURE},
      {&Pt_Core_Calibration_T::k_pt_path_change_one_grouped_one_creation, PT_MIN_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_CREATION,
       PT_MAX_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_CREATION},
      {&Pt_Core_Calibration_T::k_pt_kill_path_exceed_dist_thres, PT_MIN_K_PT_KILL_PATH_EXCEED_DIST_THRES,
       PT_MAX_K_PT_KILL_PATH_EXCEED_DIST_THRES},
      {&Pt_Core_Calibration_T::k_pt_kill_path_max_diff_posn, PT_MIN_K_PT_KILL_PATH_MAX_DIFF_POSN, PT_MAX_K_PT_KILL_PATH_MAX_DIFF_POSN},
      {&Pt_Core_Calibration_T::k_pt_lower_lim_obj_orient_lat, PT_MIN_K_PT_LOWER_LIM_OBJ_ORIENT_LAT,
       PT_MAX_K_PT_LOWER_LIM_OBJ_ORIENT_LAT},
      {&Pt_Core_Calibration_T::k_pt_upper_lim_obj_orient_lat, PT_MIN_K_PT_UPPER_LIM_OBJ_ORIENT_LAT,
       PT_MAX_K_PT_UPPER_LIM_OBJ_ORIENT_LAT},
      {&Pt_Core_Calibration_T::k_pt_apply_move_point_min_speed, PT_MIN_K_PT_APPLY_MOVE_POINT_MIN_SPEED,
       PT_MAX_K_PT_APPLY_MOVE_POINT_MIN_SPEED},
      {&Pt_Core_Calibration_T::k_pt_apply_move_point_min_yaw_rate, PT_MIN_K_PT_APPLY_MOVE_POINT_MIN_YAW_RATE,
       PT_MAX_K_PT_APPLY_MOVE_POINT_MIN_YAW_RATE},
      {&Pt_Core_Calibration_T::k_pt_move_point_yaw_rate_thres_calc_ego_shift, PT_MIN_K_PT_MOVE_POINT_YAW_RATE_THRES_CALC_EGO_SHIFT,
       PT_MAX_K_PT_MOVE_POINT_YAW_RATE_THRES_CALC_EGO_SHIFT},
      {&Pt_Core_Calibration_T::k_pt_group_paths_min_interval_dist, PT_MIN_K_PT_GROUP_PATHS_MIN_INTERVAL_DIST,
       PT_MAX_K_PT_GROUP_PATHS_MIN_INTERVAL_DIST},
      {&Pt_Core_Calibration_T::k_pt_path_track_long_range_limit, PT_MIN_K_PT_PATH_TRACK_LONG_RANGE_LIMIT,
       PT_MAX_K_PT_PATH_TRACK_LONG_RANGE_LIMIT},
      {&Pt_Core_Calibration_T::k_pt_path_track_lat_range_limit, PT_MIN_K_PT_PATH_TRACK_LAT_RANGE_LIMIT,
       PT_MAX_K_PT_PATH_TRACK_LAT_RANGE_LIMIT},
      {&Pt_Core_Calibration_T::k_pt_group_overlap_paths_min_diff, PT_MIN_K_PT_GROUP_OVERLAP_PATHS_MIN_DIFF,
       PT_MAX_K_PT_GROUP_OVERLAP_PATHS_MIN_DIFF},
      {&Pt_Core_Calibration_T::k_pt_en_algo_min_vel_inactive, PT_MIN_K_PT_EN_ALGO_MIN_VEL_INACTIVE,
       PT_MAX_K_PT_EN_ALGO_MIN_VEL_INACTIVE},
      {&Pt_Core_Calibration_T::k_pt_en_algo_max_val_active, PT_MIN_K_PT_EN_ALGO_MAX_VAL_ACTIVE, PT_MAX_K_PT_EN_ALGO_MAX_VAL_ACTIVE},
      {&Pt_Core_Calibration_T::k_pt_default_range_of_tracking_zone, PT_MIN_K_PT_DEFAULT_RANGE_OF_TRACKING_ZONE,
       PT_MAX_K_PT_DEFAULT_RANGE_OF_TRACKING_ZONE},
      {&Pt_Core_Calibration_T::k_pt_min_exist_prob_to_be_valid, PT_MIN_K_PT_MIN_EXIST_PROB_TO_BE_VALID,
       PT_MAX_K_PT_MIN_EXIST_PROB_TO_BE_VALID},
      {&Pt_Core_Calibration_T::k_pt_weight_of_last_trail_point, PT_MIN_K_PT_WEIGHT_OF_LAST_TRAIL_POINT,
       PT_MAX_K_PT_WEIGHT_OF_LAST_TRAIL_POINT},
      {&Pt_Core_Calibration_T::k_pt_weight_of_sec_last_trail_point, PT_MIN_K_PT_WEIGHT_OF_SEC_LAST_TRAIL_POINT,
       PT_MAX_K_PT_WEIGHT_OF_SEC_LAST_TRAIL_POINT},
      {&Pt_Core_Calibration_T::k_pt_min_confidence_valid_match, PT_MIN_K_PT_MIN_CONFIDENCE_VALID_MATCH,
       PT_MAX_K_PT_MIN_CONFIDENCE_VALID_MATCH},
      {&Pt_Core_Calibration_T::k_pt_dist_betw_paths_similarity_matching, PT_MIN_K_PT_DIST_BETW_PATHS_SIMILARITY_MATCHING,
       PT_MAX_K_PT_DIST_BETW_PATHS_SIMILARITY_MATCHING},
      {&Pt_Core_Calibration_T::k_pt_host_implausibilty_range, PT_MIN_K_PT_HOST_IMPLAUSIBILTY_RANGE,
       PT_MAX_K_PT_HOST_IMPLAUSIBILTY_RANGE},
      {&Pt_Core_Calibration_T::k_pt_trail_max_speed_trail_to_path_conv, PT_MIN_K_PT_TRAIL_MAX_SPEED_TRAIL_TO_PATH_CONV,
       PT_MAX_K_PT_TRAIL_MAX_SPEED_TRAIL_TO_PATH_CONV},
      {&Pt_Core_Calibration_T::k_pt_minimum_host_trail_length, PT_MIN_K_PT_MINIMUM_HOST_TRAIL_LENGTH,
       PT_MAX_K_PT_MINIMUM_HOST_TRAIL_LENGTH},
      {&Pt_Core_Calibration_T::k_pt_max_heading_diff_valid_interval, PT_MIN_K_PT_MAX_HEADING_DIFF_VALID_INTERVAL,
       PT_MAX_K_PT_MAX_HEADING_DIFF_VALID_INTERVAL},
   };
   /** \assert Expect correct work of Pt_Core_Cal_In_Boundary for float32_T fields. */
   Pt_Test_Pt_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Pt_Core_Cal_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-73375} \sdd{CSCSA-216698} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Core_Calibration_Test, Pt_Core_Cal_In_Boundary__Uint8_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {

      {&Pt_Core_Calibration_T::k_unused_padding_byte_0, PT_MIN_K_UNUSED_PADDING_BYTE_0, PT_MAX_K_UNUSED_PADDING_BYTE_0},
      {&Pt_Core_Calibration_T::k_pt_max_diff_num_path_point, PT_MIN_K_PT_MAX_DIFF_NUM_PATH_POINT, PT_MAX_K_PT_MAX_DIFF_NUM_PATH_POINT},
      {&Pt_Core_Calibration_T::k_pt_group_path_min_overlap_count, PT_MIN_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT,
       PT_MAX_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT},
      {&Pt_Core_Calibration_T::k_pt_group_path_min_overlap_count_ad, PT_MIN_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT_AD,
       PT_MAX_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT_AD},
      {&Pt_Core_Calibration_T::k_pt_find_min_diff_path_points, PT_MIN_K_PT_FIND_MIN_DIFF_PATH_POINTS,
       PT_MAX_K_PT_FIND_MIN_DIFF_PATH_POINTS},
      {&Pt_Core_Calibration_T::k_pt_find_max_diff_path_points, PT_MIN_K_PT_FIND_MAX_DIFF_PATH_POINTS,
       PT_MAX_K_PT_FIND_MAX_DIFF_PATH_POINTS},
      {&Pt_Core_Calibration_T::k_pt_kill_lane_change_min_diff_path_point, PT_MIN_K_PT_KILL_LANE_CHANGE_MIN_DIFF_PATH_POINT,
       PT_MAX_K_PT_KILL_LANE_CHANGE_MIN_DIFF_PATH_POINT},
      {&Pt_Core_Calibration_T::k_pt_min_diff_num_path_points, PT_MIN_K_PT_MIN_DIFF_NUM_PATH_POINTS,
       PT_MAX_K_PT_MIN_DIFF_NUM_PATH_POINTS},
      {&Pt_Core_Calibration_T::k_pt_min_path_length_proc_lane_change, PT_MIN_K_PT_MIN_PATH_LENGTH_PROC_LANE_CHANGE,
       PT_MAX_K_PT_MIN_PATH_LENGTH_PROC_LANE_CHANGE},
      {&Pt_Core_Calibration_T::k_pt_cond_kill_implaus_path, PT_MIN_K_PT_COND_KILL_IMPLAUS_PATH, PT_MAX_K_PT_COND_KILL_IMPLAUS_PATH},
      {&Pt_Core_Calibration_T::k_pt_min_path_length_after_rot, PT_MIN_K_PT_MIN_PATH_LENGTH_AFTER_ROT,
       PT_MAX_K_PT_MIN_PATH_LENGTH_AFTER_ROT},
      {&Pt_Core_Calibration_T::k_pt_min_path_length_obj_trail, PT_MIN_K_PT_MIN_PATH_LENGTH_OBJ_TRAIL,
       PT_MAX_K_PT_MIN_PATH_LENGTH_OBJ_TRAIL},
      {&Pt_Core_Calibration_T::k_pt_range_nearest_border_impl_path, PT_MIN_K_PT_RANGE_NEAREST_BORDER_IMPL_PATH,
       PT_MAX_K_PT_RANGE_NEAREST_BORDER_IMPL_PATH},
      {&Pt_Core_Calibration_T::k_pt_start_of_lane_change_processing, PT_MIN_K_PT_START_OF_LANE_CHANGE_PROCESSING,
       PT_MAX_K_PT_START_OF_LANE_CHANGE_PROCESSING},
      {&Pt_Core_Calibration_T::k_pt_end_of_lane_change_processing, PT_MIN_K_PT_END_OF_LANE_CHANGE_PROCESSING,
       PT_MAX_K_PT_END_OF_LANE_CHANGE_PROCESSING},
      {&Pt_Core_Calibration_T::k_pt_minimum_amount_of_trail_points, PT_MIN_K_PT_MINIMUM_AMOUNT_OF_TRAIL_POINTS,
       PT_MAX_K_PT_MINIMUM_AMOUNT_OF_TRAIL_POINTS},
   };
   /** \assert Expect correct work of Pt_Core_Cal_In_Boundary for float32_T fields. */
   Pt_Test_Pt_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif /* CT_ACTIVATE_CAL_PRINT */
