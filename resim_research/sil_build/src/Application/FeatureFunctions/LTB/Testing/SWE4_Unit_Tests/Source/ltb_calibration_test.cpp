/**
 * @file ltb_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for LTB calibration unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-46108}
 */

#include "ltb_calibration_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <stdio.h>
#include <string.h>
#include <tuple>
#include <vector>

extern "C"
{
#include "ltb_core_calibration.c"
#include "ltb_core_calibration_check.h"
#include "ltb_update_calibration.h"
#include "pa_reuse.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-46110} \sdd{CSCSA-216642} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Core_Calibration_Test, Ltb_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Ltb_Core_Cal_Print(p_testfile, &ltb_cals));
}


/**
 * Test for calibration file. Run test to check basic Ltb_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-64251} \sdd{CSCSA-216639} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Core_Calibration_Test, Ltb_Core_Cal_Update_Defaults__is_reseting_k_ltb_alert_lvl_3_decel_threshold)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value = ltb_cals.k_ltb_alert_lvl_3_decel_threshold;
   ltb_cals.k_ltb_alert_lvl_3_decel_threshold += 1;
   Ltb_Core_Cal_Update_Defaults(&ltb_cals);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, ltb_cals.k_ltb_alert_lvl_3_decel_threshold);
}

/**
 * Test for calibration file. Run test to check basic Ltb_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-64252} \sdd{CSCSA-216640} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Core_Calibration_Test, Ltb_Update_Core_Cal_By_Core__is_setting_k_ltb_alert_lvl_3_decel_threshold)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value      = ltb_cals.k_ltb_alert_lvl_3_decel_threshold;
   const float32_T new_value          = orginal_value + 1;
   Ltb_Core_Calibration_T calibration = ltb_cals;

   calibration.k_ltb_alert_lvl_3_decel_threshold = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, ltb_cals.k_ltb_alert_lvl_3_decel_threshold);
   boolean_T result = Ltb_Update_Core_Cal_By_Core(&ltb_cals, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, ltb_cals.k_ltb_alert_lvl_3_decel_threshold);
}

template <typename T> using Single_Case_T = std::tuple<const T Ltb_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Ltb_Test_Ltb_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Ltb_Core_Calibration_T calibration;
   Ltb_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Ltb_Core_Cal_In_Boundary(&calibration));
      const T Ltb_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                           = std::get<1>(single_case);
      const T max_value                           = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Ltb_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Ltb_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Ltb_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Ltb_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Ltb_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-64253} \sdd{CSCSA-216629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Core_Calibration_Test, Ltb_Core_Cal_In_Boundary__Float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Ltb_Core_Calibration_T::k_ltb_zone_length, LTB_MIN_K_LTB_ZONE_LENGTH, LTB_MAX_K_LTB_ZONE_LENGTH},
      {&Ltb_Core_Calibration_T::k_ltb_zone_width, LTB_MIN_K_LTB_ZONE_WIDTH, LTB_MAX_K_LTB_ZONE_WIDTH},
      {&Ltb_Core_Calibration_T::k_ltb_ego_acceleration_weight, LTB_MIN_K_LTB_EGO_ACCELERATION_WEIGHT,
       LTB_MAX_K_LTB_EGO_ACCELERATION_WEIGHT},
      {&Ltb_Core_Calibration_T::k_ltb_ego_shape_gain_fixed, LTB_MIN_K_LTB_EGO_SHAPE_GAIN_FIXED, LTB_MAX_K_LTB_EGO_SHAPE_GAIN_FIXED},
      {&Ltb_Core_Calibration_T::k_ltb_ego_circle_offset, LTB_MIN_K_LTB_EGO_CIRCLE_OFFSET, LTB_MAX_K_LTB_EGO_CIRCLE_OFFSET},
      {&Ltb_Core_Calibration_T::k_ltb_ego_circle_host_length_factor, LTB_MIN_K_LTB_EGO_CIRCLE_HOST_LENGTH_FACTOR,
       LTB_MAX_K_LTB_EGO_CIRCLE_HOST_LENGTH_FACTOR},
      {&Ltb_Core_Calibration_T::k_ltb_ego_deceleration_weight, LTB_MIN_K_LTB_EGO_DECELERATION_WEIGHT,
       LTB_MAX_K_LTB_EGO_DECELERATION_WEIGHT},
      {&Ltb_Core_Calibration_T::k_ltb_ego_max_pred_yaw_angle, LTB_MIN_K_LTB_EGO_MAX_PRED_YAW_ANGLE,
       LTB_MAX_K_LTB_EGO_MAX_PRED_YAW_ANGLE},
      {&Ltb_Core_Calibration_T::k_ltb_ego_yawangle_integration_yawrate_min, LTB_MIN_K_LTB_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN,
       LTB_MAX_K_LTB_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN},
      {&Ltb_Core_Calibration_T::k_ltb_ego_shape_gain_per_pred_step, LTB_MIN_K_LTB_EGO_SHAPE_GAIN_PER_PRED_STEP,
       LTB_MAX_K_LTB_EGO_SHAPE_GAIN_PER_PRED_STEP},
      {&Ltb_Core_Calibration_T::k_ltb_obj_pred_speed_min, LTB_MIN_K_LTB_OBJ_PRED_SPEED_MIN, LTB_MAX_K_LTB_OBJ_PRED_SPEED_MIN},
      {&Ltb_Core_Calibration_T::k_ltb_obj_shape_gain_fixed, LTB_MIN_K_LTB_OBJ_SHAPE_GAIN_FIXED, LTB_MAX_K_LTB_OBJ_SHAPE_GAIN_FIXED},
      {&Ltb_Core_Calibration_T::k_ltb_obj_shape_gain_per_pred_step, LTB_MIN_K_LTB_OBJ_SHAPE_GAIN_PER_PRED_STEP,
       LTB_MAX_K_LTB_OBJ_SHAPE_GAIN_PER_PRED_STEP},
      {&Ltb_Core_Calibration_T::k_ltb_critical_approach_min_safe_distance, LTB_MIN_K_LTB_CRITICAL_APPROACH_MIN_SAFE_DISTANCE,
       LTB_MAX_K_LTB_CRITICAL_APPROACH_MIN_SAFE_DISTANCE},
      {&Ltb_Core_Calibration_T::k_ltb_critical_approach_angle_diff_min, LTB_MIN_K_LTB_CRITICAL_APPROACH_ANGLE_DIFF_MIN,
       LTB_MAX_K_LTB_CRITICAL_APPROACH_ANGLE_DIFF_MIN},
      {&Ltb_Core_Calibration_T::k_ltb_alert_lvl_1_ttc_threshold, LTB_MIN_K_LTB_ALERT_LVL_1_TTC_THRESHOLD,
       LTB_MAX_K_LTB_ALERT_LVL_1_TTC_THRESHOLD},
      {&Ltb_Core_Calibration_T::k_ltb_alert_lvl_2_ttc_threshold, LTB_MIN_K_LTB_ALERT_LVL_2_TTC_THRESHOLD,
       LTB_MAX_K_LTB_ALERT_LVL_2_TTC_THRESHOLD},
      {&Ltb_Core_Calibration_T::k_ltb_alert_lvl_2_ttb_threshold, LTB_MIN_K_LTB_ALERT_LVL_2_TTB_THRESHOLD,
       LTB_MAX_K_LTB_ALERT_LVL_2_TTB_THRESHOLD},
      {&Ltb_Core_Calibration_T::k_ltb_alert_lvl_3_ttc_threshold, LTB_MIN_K_LTB_ALERT_LVL_3_TTC_THRESHOLD,
       LTB_MAX_K_LTB_ALERT_LVL_3_TTC_THRESHOLD},
      {&Ltb_Core_Calibration_T::k_ltb_alert_lvl_3_decel_threshold, LTB_MIN_K_LTB_ALERT_LVL_3_DECEL_THRESHOLD,
       LTB_MAX_K_LTB_ALERT_LVL_3_DECEL_THRESHOLD},
      {&Ltb_Core_Calibration_T::k_ltb_brake_deceleration_max, LTB_MIN_K_LTB_BRAKE_DECELERATION_MAX,
       LTB_MAX_K_LTB_BRAKE_DECELERATION_MAX},
      {&Ltb_Core_Calibration_T::k_ltb_brake_dead_time, LTB_MIN_K_LTB_BRAKE_DEAD_TIME, LTB_MAX_K_LTB_BRAKE_DEAD_TIME},
      {&Ltb_Core_Calibration_T::k_ltb_brake_gradient, LTB_MIN_K_LTB_BRAKE_GRADIENT, LTB_MAX_K_LTB_BRAKE_GRADIENT},
      {&Ltb_Core_Calibration_T::k_ltb_object_long_vel_min, LTB_MIN_K_LTB_OBJECT_LONG_VEL_MIN, LTB_MAX_K_LTB_OBJECT_LONG_VEL_MIN},
   };
   /** \assert Expect correct work of Ltb_Core_Cal_In_Boundary for float32_T fields. */
   Ltb_Test_Ltb_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Ltb_Core_Cal_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-64254} \sdd{CSCSA-216629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Core_Calibration_Test, Ltb_Core_Cal_In_Boundary__Uint8_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      {&Ltb_Core_Calibration_T::k_unused_padding_byte_0, LTB_MIN_K_UNUSED_PADDING_BYTE_0, LTB_MAX_K_UNUSED_PADDING_BYTE_0},
      {&Ltb_Core_Calibration_T::k_ltb_ego_pred_const_velocity_pred_steps_min, LTB_MIN_K_LTB_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN,
       LTB_MAX_K_LTB_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN},
      {&Ltb_Core_Calibration_T::k_ltb_prediction_steps_max, LTB_MIN_K_LTB_PREDICTION_STEPS_MAX, LTB_MAX_K_LTB_PREDICTION_STEPS_MAX},
   };
   /** \assert Expect correct work of Ltb_Core_Cal_In_Boundary for float32_T fields. */
   Ltb_Test_Ltb_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif /* CT_ACTIVATE_CAL_PRINT */