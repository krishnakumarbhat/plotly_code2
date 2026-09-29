/**
 * @file ced_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for CED calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41438}
 */

#include "ced_calibration_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "ced_core_calibration.c"
#include "ced_core_calibration_check.h"
#include "ced_update_calibration.h"
#include "pa_reuse.h"
}

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-41440} \sdd{CSCSA-186574} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Core_Calibration_Test, Ced_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Ced_Core_Cal_Print(p_testfile, &ced_cal));
}

/**
 * Test for calibration file. Run test to check basic Ced_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-70474} \sdd{CSCSA-186573} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Core_Calibration_Test, Ced_Core_Cal_Update_Defaults__is_reseting_k_ced_object_heading_predicted_weight)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value = ced_cal.k_ced_object_heading_predicted_weight;
   ced_cal.k_ced_object_heading_predicted_weight += 1;
   Ced_Core_Cal_Update_Defaults(&ced_cal);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, ced_cal.k_ced_object_heading_predicted_weight);
}

/**
 * Test for calibration file. Run test to check basic Ced_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-70475} \sdd{CSCSA-89982} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Core_Calibration_Test, Ced_Update_Core_Cal_By_Core__is_setting_k_ced_object_heading_abs_angle_max)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value      = ced_cal.k_ced_object_heading_abs_angle_max;
   const float32_T new_value          = orginal_value + 0.1f;
   Ced_Core_Calibration_T calibration = ced_cal;

   calibration.k_ced_object_heading_abs_angle_max = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, ced_cal.k_ced_object_heading_abs_angle_max);
   boolean_T result = Ced_Update_Core_Cal_By_Core(&ced_cal, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, ced_cal.k_ced_object_heading_abs_angle_max);
}

/**
 * Test for calibration file. Run test to check if function returns false, when no calibration is passed.
 * \uts{CSCSA-90056} \sdd{CSCSA-89982} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Core_Calibration_Test, Ced_Update_Core_Cal_By_Core__null_pointer_for_cals)
{
   /** \arrange n/a */

   /** \action n/a */
   boolean_T result = Ced_Update_Core_Cal_By_Core(&ced_cal, NULL);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

template <typename T> using Single_Case_T = std::tuple<const T Ced_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Ced_Test_Ced_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Ced_Core_Calibration_T calibration;
   Ced_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Ced_Core_Cal_In_Boundary(&calibration));
      const T Ced_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                           = std::get<1>(single_case);
      const T max_value                           = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Ced_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Ced_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Ced_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Ced_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Ced_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-70476} \sdd{CSCSA-186577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Core_Calibration_Test, Ced_Core_Cal_In_Boundary__Float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Ced_Core_Calibration_T::k_ced_object_acceleration_weight, CED_MIN_K_CED_OBJECT_ACCELERATION_WEIGHT,
       CED_MAX_K_CED_OBJECT_ACCELERATION_WEIGHT},
      {&Ced_Core_Calibration_T::k_ced_object_heading_predicted_weight, CED_MIN_K_CED_OBJECT_HEADING_PREDICTED_WEIGHT,
       CED_MAX_K_CED_OBJECT_HEADING_PREDICTED_WEIGHT},
      {&Ced_Core_Calibration_T::k_ced_object_existence_probability_min, CED_MIN_K_CED_OBJECT_EXISTENCE_PROBABILITY_MIN,
       CED_MAX_K_CED_OBJECT_EXISTENCE_PROBABILITY_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_heading_abs_angle_max, CED_MIN_K_CED_OBJECT_HEADING_ABS_ANGLE_MAX,
       CED_MAX_K_CED_OBJECT_HEADING_ABS_ANGLE_MAX},
      {&Ced_Core_Calibration_T::k_ced_object_long_vel_rel_min, CED_MIN_K_CED_OBJECT_LONG_VEL_REL_MIN,
       CED_MAX_K_CED_OBJECT_LONG_VEL_REL_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_long_vel_min, CED_MIN_K_CED_OBJECT_LONG_VEL_MIN, CED_MAX_K_CED_OBJECT_LONG_VEL_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_lat_vel_max, CED_MIN_K_CED_OBJECT_LAT_VEL_MAX, CED_MAX_K_CED_OBJECT_LAT_VEL_MAX},
      {&Ced_Core_Calibration_T::k_ced_object_max_width_increase_factor_with_path_match,
       CED_MIN_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITH_PATH_MATCH, CED_MAX_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITH_PATH_MATCH},
      {&Ced_Core_Calibration_T::k_ced_object_max_width_increase_factor_without_path_match,
       CED_MIN_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITHOUT_PATH_MATCH,
       CED_MAX_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITHOUT_PATH_MATCH},
      {&Ced_Core_Calibration_T::k_ced_object_heading_exp_moving_average_alpha,
       CED_MIN_K_CED_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA, CED_MAX_K_CED_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA},
      {&Ced_Core_Calibration_T::k_ced_object_ftm_existence_probability_min, CED_MIN_K_CED_OBJECT_FTM_EXISTENCE_PROBABILITY_MIN,
       CED_MAX_K_CED_OBJECT_FTM_EXISTENCE_PROBABILITY_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_ftm_heading_abs_angle_min, CED_MIN_K_CED_OBJECT_FTM_HEADING_ABS_ANGLE_MIN,
       CED_MAX_K_CED_OBJECT_FTM_HEADING_ABS_ANGLE_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_ftm_long_vel_rel_min, CED_MIN_K_CED_OBJECT_FTM_LONG_VEL_REL_MIN,
       CED_MAX_K_CED_OBJECT_FTM_LONG_VEL_REL_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_ftm_long_vel_min, CED_MIN_K_CED_OBJECT_FTM_LONG_VEL_MIN,
       CED_MAX_K_CED_OBJECT_FTM_LONG_VEL_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_ftm_lat_vel_max, CED_MIN_K_CED_OBJECT_FTM_LAT_VEL_MAX,
       CED_MAX_K_CED_OBJECT_FTM_LAT_VEL_MAX},
      {&Ced_Core_Calibration_T::k_ced_second_warning_pred_lat_dist_max, CED_MIN_K_CED_SECOND_WARNING_PRED_LAT_DIST_MAX,
       CED_MAX_K_CED_SECOND_WARNING_PRED_LAT_DIST_MAX},
      {&Ced_Core_Calibration_T::k_ced_ego_abs_speed_max, CED_MIN_K_CED_EGO_ABS_SPEED_MAX, CED_MAX_K_CED_EGO_ABS_SPEED_MAX},
      {&Ced_Core_Calibration_T::k_ced_object_width_safety_margin_for_active_alert,
       CED_MIN_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_ACTIVE_ALERT, CED_MAX_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_ACTIVE_ALERT},
      {&Ced_Core_Calibration_T::k_ced_object_width_safety_margin_for_critical_path_match,
       CED_MIN_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_CRITICAL_PATH_MATCH,
       CED_MAX_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_CRITICAL_PATH_MATCH},
      {&Ced_Core_Calibration_T::k_ced_object_min_dist_to_crash_line_for_path_match,
       CED_MIN_K_CED_OBJECT_MIN_DIST_TO_CRASH_LINE_FOR_PATH_MATCH, CED_MAX_K_CED_OBJECT_MIN_DIST_TO_CRASH_LINE_FOR_PATH_MATCH},
      {&Ced_Core_Calibration_T::k_ced_offset_to_path_weight, CED_MIN_K_CED_OFFSET_TO_PATH_WEIGHT, CED_MAX_K_CED_OFFSET_TO_PATH_WEIGHT},
      {&Ced_Core_Calibration_T::k_ced_collision_zone_width, CED_MIN_K_CED_COLLISION_ZONE_WIDTH, CED_MAX_K_CED_COLLISION_ZONE_WIDTH},
      {&Ced_Core_Calibration_T::k_ced_funnel_zone_length, CED_MIN_K_CED_FUNNEL_ZONE_LENGTH, CED_MAX_K_CED_FUNNEL_ZONE_LENGTH},
      {&Ced_Core_Calibration_T::k_ced_funnel_zone_width, CED_MIN_K_CED_FUNNEL_ZONE_WIDTH, CED_MAX_K_CED_FUNNEL_ZONE_WIDTH},
      {&Ced_Core_Calibration_T::k_ced_slow_objects_long_vel_max, CED_MIN_K_CED_SLOW_OBJECTS_LONG_VEL_MAX,
       CED_MAX_K_CED_SLOW_OBJECTS_LONG_VEL_MAX},
      {&Ced_Core_Calibration_T::k_ced_ego_lane_width, CED_MIN_K_CED_EGO_LANE_WIDTH, CED_MAX_K_CED_EGO_LANE_WIDTH},
      {&Ced_Core_Calibration_T::k_ced_ego_lane_parking_range, CED_MIN_K_CED_EGO_LANE_PARKING_RANGE,
       CED_MAX_K_CED_EGO_LANE_PARKING_RANGE},
      {&Ced_Core_Calibration_T::k_ced_ego_lane_parking_maneuver_speed, CED_MIN_K_CED_EGO_LANE_PARKING_MANEUVER_SPEED,
       CED_MAX_K_CED_EGO_LANE_PARKING_MANEUVER_SPEED},
      {&Ced_Core_Calibration_T::k_ced_alert_holding_obj_abs_heading_max, CED_MIN_K_CED_ALERT_HOLDING_OBJ_ABS_HEADING_MAX,
       CED_MAX_K_CED_ALERT_HOLDING_OBJ_ABS_HEADING_MAX},
      {&Ced_Core_Calibration_T::k_ced_alert_holding_obj_long_vel_min, CED_MIN_K_CED_ALERT_HOLDING_OBJ_LONG_VEL_MIN,
       CED_MAX_K_CED_ALERT_HOLDING_OBJ_LONG_VEL_MIN},
      {&Ced_Core_Calibration_T::k_ced_suppress_pt_heading_diff_ced_alert_max, CED_MIN_K_CED_SUPPRESS_PT_HEADING_DIFF_CED_ALERT_MAX,
       CED_MAX_K_CED_SUPPRESS_PT_HEADING_DIFF_CED_ALERT_MAX},
      {&Ced_Core_Calibration_T::k_ced_suppress_range_to_nearest_path_max, CED_MIN_K_CED_SUPPRESS_RANGE_TO_NEAREST_PATH_MAX,
       CED_MAX_K_CED_SUPPRESS_RANGE_TO_NEAREST_PATH_MAX},
      {&Ced_Core_Calibration_T::k_ced_object_long_vel_rel_max, CED_MIN_K_CED_OBJECT_LONG_VEL_REL_MAX,
       CED_MAX_K_CED_OBJECT_LONG_VEL_REL_MAX},
      {&Ced_Core_Calibration_T::k_ced_object_ftm_long_vel_rel_max, CED_MIN_K_CED_OBJECT_FTM_LONG_VEL_REL_MAX,
       CED_MAX_K_CED_OBJECT_FTM_LONG_VEL_REL_MAX},
      {&Ced_Core_Calibration_T::k_ced_object_vel_max, CED_MIN_K_CED_OBJECT_VEL_MAX, CED_MAX_K_CED_OBJECT_VEL_MAX},
   };
   /** \assert Expect correct work of Ced_Core_Cal_In_Boundary for float32_T fields. */
   Ced_Test_Ced_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Ced_Core_Cal_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-70477} \sdd{CSCSA-186577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Core_Calibration_Test, Ced_Core_Cal_In_Boundary__Uint8_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      /*{&Ced_Core_Calibration_T::k_unused_padding_byte_0, CED_MIN_K_UNUSED_PADDING_BYTE_0, CED_MAX_K_UNUSED_PADDING_BYTE_0},*/
      {&Ced_Core_Calibration_T::k_ced_alert_qualifying_cycles, CED_MIN_K_CED_ALERT_QUALIFYING_CYCLES,
       CED_MAX_K_CED_ALERT_QUALIFYING_CYCLES},
      {&Ced_Core_Calibration_T::k_ced_alert_qualifying_cycles_slow_objects, CED_MIN_K_CED_ALERT_QUALIFYING_CYCLES_SLOW_OBJECTS,
       CED_MAX_K_CED_ALERT_QUALIFYING_CYCLES_SLOW_OBJECTS},
      {&Ced_Core_Calibration_T::k_ced_alert_holding_cycles, CED_MIN_K_CED_ALERT_HOLDING_CYCLES, CED_MAX_K_CED_ALERT_HOLDING_CYCLES},
      {&Ced_Core_Calibration_T::k_ced_allow_opposite_side_alerts, CED_MIN_K_CED_ALLOW_OPPOSITE_SIDE_ALERTS,
       CED_MAX_K_CED_ALLOW_OPPOSITE_SIDE_ALERTS},
      {&Ced_Core_Calibration_T::k_ced_suppress_alert_object_age_max, CED_MIN_K_CED_SUPPRESS_ALERT_OBJECT_AGE_MAX,
       CED_MAX_K_CED_SUPPRESS_ALERT_OBJECT_AGE_MAX},
      {&Ced_Core_Calibration_T::k_ced_min_cycles_for_path_match_for_no_suppress,
       CED_MIN_K_CED_MIN_CYCLES_FOR_PATH_MATCH_FOR_NO_SUPPRESS, CED_MAX_K_CED_MIN_CYCLES_FOR_PATH_MATCH_FOR_NO_SUPPRESS},
      {&Ced_Core_Calibration_T::k_ced_object_age_min, CED_MIN_K_CED_OBJECT_AGE_MIN, CED_MAX_K_CED_OBJECT_AGE_MIN},
      {&Ced_Core_Calibration_T::k_ced_object_ftm_age_min, CED_MIN_K_CED_OBJECT_FTM_AGE_MIN, CED_MAX_K_CED_OBJECT_FTM_AGE_MIN},
   };
   /** \assert Expect correct work of Ced_Core_Cal_In_Boundary for float32_T fields. */
   Ced_Test_Ced_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif /* CT_ACTIVATE_CAL_PRINT */
