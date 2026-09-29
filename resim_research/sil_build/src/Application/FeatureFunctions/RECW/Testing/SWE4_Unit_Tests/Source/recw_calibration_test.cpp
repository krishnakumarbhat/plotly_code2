/**
 * @file recw_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for RECW calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44249}
 */

#include "recw_calibration_test.hpp"
#include "pa_obj_in.h"
#include "gmock/gmock.h"

extern "C"
{
#include "recw_core_calibration.c"
#include "recw_core_calibration_check.h"
#include "recw_update_calibration.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-44251} \sdd{CSCSA-186530} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Core_Calibration_Test, Recw_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Recw_Core_Cal_Print(p_testfile, &recw_cals));
}

/**
 * Test for calibration file. Run test to check basic Recw_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-101044} \sdd{CSCSA-186531} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Core_Calibration_Test, Recw_Core_Cal_Update_Defaults__is_reseting_k_recw_max_allowed_rel_vel_long_diff)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value = recw_cals.k_recw_max_allowed_rel_vel_long_diff;
   recw_cals.k_recw_max_allowed_rel_vel_long_diff += 1;
   Recw_Core_Cal_Update_Defaults(&recw_cals);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, recw_cals.k_recw_max_allowed_rel_vel_long_diff);
}

/**
 * Test for calibration file. Run test to check basic Recw_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-101045} \sdd{CSCSA-186529} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Core_Calibration_Test, Recw_Update_Core_Cal_By_Core__is_setting_k_recw_max_allowed_rel_vel_long_diff)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value       = recw_cals.k_recw_max_allowed_rel_vel_long_diff;
   const float32_T new_value           = orginal_value + 1;
   Recw_Core_Calibration_T calibration = recw_cals;

   calibration.k_recw_max_allowed_rel_vel_long_diff = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, recw_cals.k_recw_max_allowed_rel_vel_long_diff);
   boolean_T result = Recw_Update_Core_Cal_By_Core(&recw_cals, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, recw_cals.k_recw_max_allowed_rel_vel_long_diff);
}

/**
 * Test for calibration. Validate function return zero in case of incorrect inputs for calibration update.
 * \uts{CSCSA-204750} \sdd{CSCSA-186529} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Core_Calibration_Test, Recw_Update_Core_Cal_By_Core__calibration_is_out_of_boundary)
{
   /** \arrange Change one of the calibration to invalid value */
   boolean_T result;

   Recw_Core_Calibration_T calibration = recw_cals;
   const float32_T new_value           = recw_cals.k_recw_max_allowed_rel_vel_long_diff + 50.0f;

   /** \action Call function and fetch status */
   calibration.k_recw_max_allowed_rel_vel_long_diff = new_value;
   result                                           = Recw_Update_Core_Cal_By_Core(&recw_cals, &calibration);

   /** \assert Zero status on function output. */
   EXPECT_FALSE(result);
   EXPECT_FALSE(Recw_Update_Core_Cal_By_Core(&recw_cals, nullptr));
}

template <typename T> using Single_Case_T = std::tuple<const T Recw_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Recw_Test_Recw_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Recw_Core_Calibration_T calibration;
   Recw_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Recw_Core_Cal_In_Boundary(&calibration));
      const T Recw_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                            = std::get<1>(single_case);
      const T max_value                            = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Recw_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Recw_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Recw_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Recw_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Recw_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-101046} \sdd{CSCSA-186520} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Core_Calibration_Test, Recw_Core_Cal_In_Boundary__Float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Recw_Core_Calibration_T::k_recw_max_heading_hys, RECW_MIN_K_RECW_MAX_HEADING_HYS, RECW_MAX_K_RECW_MAX_HEADING_HYS},
      {&Recw_Core_Calibration_T::k_recw_factor_ego_width, RECW_MIN_K_RECW_FACTOR_EGO_WIDTH, RECW_MAX_K_RECW_FACTOR_EGO_WIDTH},
      {&Recw_Core_Calibration_T::k_recw_average_sensor_latency, RECW_MIN_K_RECW_AVERAGE_SENSOR_LATENCY,
       RECW_MAX_K_RECW_AVERAGE_SENSOR_LATENCY},
      {&Recw_Core_Calibration_T::k_recw_min_existence_prob_hys, RECW_MIN_K_RECW_MIN_EXISTENCE_PROB_HYS,
       RECW_MAX_K_RECW_MIN_EXISTENCE_PROB_HYS},
      {&Recw_Core_Calibration_T::k_recw_min_host_speed_hys, RECW_MIN_K_RECW_MIN_HOST_SPEED_HYS, RECW_MAX_K_RECW_MIN_HOST_SPEED_HYS},
      {&Recw_Core_Calibration_T::k_recw_max_host_speed_hys, RECW_MIN_K_RECW_MAX_HOST_SPEED_HYS, RECW_MAX_K_RECW_MAX_HOST_SPEED_HYS},
      {&Recw_Core_Calibration_T::k_recw_min_dist_for_young_slow_targets, RECW_MIN_K_RECW_MIN_DIST_FOR_YOUNG_SLOW_TARGETS,
       RECW_MAX_K_RECW_MIN_DIST_FOR_YOUNG_SLOW_TARGETS},
      {&Recw_Core_Calibration_T::k_recw_min_abs_speed_for_young_close_targets,
       RECW_MIN_K_RECW_MIN_ABS_SPEED_FOR_YOUNG_CLOSE_TARGETS, RECW_MAX_K_RECW_MIN_ABS_SPEED_FOR_YOUNG_CLOSE_TARGETS},
      {&Recw_Core_Calibration_T::k_recw_max_rel_velocity_hys, RECW_MIN_K_RECW_MAX_REL_VELOCITY_HYS,
       RECW_MAX_K_RECW_MAX_REL_VELOCITY_HYS},
      {&Recw_Core_Calibration_T::k_recw_max_rel_lon_vel_release_car_wash, RECW_MIN_K_RECW_MAX_REL_LON_VEL_RELEASE_CAR_WASH,
       RECW_MAX_K_RECW_MAX_REL_LON_VEL_RELEASE_CAR_WASH},
      {&Recw_Core_Calibration_T::k_recw_max_lon_distance_car_wash, RECW_MIN_K_RECW_MAX_LON_DISTANCE_CAR_WASH,
       RECW_MAX_K_RECW_MAX_LON_DISTANCE_CAR_WASH},
      {&Recw_Core_Calibration_T::k_recw_max_lat_distance_car_wash, RECW_MIN_K_RECW_MAX_LAT_DISTANCE_CAR_WASH,
       RECW_MAX_K_RECW_MAX_LAT_DISTANCE_CAR_WASH},
      {&Recw_Core_Calibration_T::k_recw_min_rel_lon_vel_car_wash, RECW_MIN_K_RECW_MIN_REL_LON_VEL_CAR_WASH,
       RECW_MAX_K_RECW_MIN_REL_LON_VEL_CAR_WASH},
      {&Recw_Core_Calibration_T::k_recw_max_speed_ego_car_wash, RECW_MIN_K_RECW_MAX_SPEED_EGO_CAR_WASH,
       RECW_MAX_K_RECW_MAX_SPEED_EGO_CAR_WASH},
      {&Recw_Core_Calibration_T::k_recw_lane_filter_width, RECW_MIN_K_RECW_LANE_FILTER_WIDTH, RECW_MAX_K_RECW_LANE_FILTER_WIDTH},
      {&Recw_Core_Calibration_T::k_recw_lane_filter_width_hys, RECW_MIN_K_RECW_LANE_FILTER_WIDTH_HYS,
       RECW_MAX_K_RECW_LANE_FILTER_WIDTH_HYS},
      {&Recw_Core_Calibration_T::k_recw_lane_filter_max_abs_ego_speed_vcs_coord,
       RECW_MIN_K_RECW_LANE_FILTER_MAX_ABS_EGO_SPEED_VCS_COORD, RECW_MAX_K_RECW_LANE_FILTER_MAX_ABS_EGO_SPEED_VCS_COORD},
      {&Recw_Core_Calibration_T::k_recw_lane_width_slope, RECW_MIN_K_RECW_LANE_WIDTH_SLOPE, RECW_MAX_K_RECW_LANE_WIDTH_SLOPE},
      {&Recw_Core_Calibration_T::k_recw_max_eclipse_value_for_valid_object, RECW_MIN_K_RECW_MAX_ECLIPSE_VALUE_FOR_VALID_OBJECT,
       RECW_MAX_K_RECW_MAX_ECLIPSE_VALUE_FOR_VALID_OBJECT},
      {&Recw_Core_Calibration_T::k_recw_max_object_width_warn_on, RECW_MIN_K_RECW_MAX_OBJECT_WIDTH_WARN_ON,
       RECW_MAX_K_RECW_MAX_OBJECT_WIDTH_WARN_ON},
      {&Recw_Core_Calibration_T::k_recw_max_allowed_rel_vel_long_diff, RECW_MIN_K_RECW_MAX_ALLOWED_REL_VEL_LONG_DIFF,
       RECW_MAX_K_RECW_MAX_ALLOWED_REL_VEL_LONG_DIFF},
      {&Recw_Core_Calibration_T::k_recw_max_allowed_rel_vel_lat_diff, RECW_MIN_K_RECW_MAX_ALLOWED_REL_VEL_LAT_DIFF,
       RECW_MAX_K_RECW_MAX_ALLOWED_REL_VEL_LAT_DIFF},
      {&Recw_Core_Calibration_T::k_recw_max_allowed_heading_diff, RECW_MIN_K_RECW_MAX_ALLOWED_HEADING_DIFF,
       RECW_MAX_K_RECW_MAX_ALLOWED_HEADING_DIFF},
      {&Recw_Core_Calibration_T::k_recw_rear_blockage_speed_threshold, RECW_MIN_K_RECW_REAR_BLOCKAGE_SPEED_THRESHOLD,
       RECW_MAX_K_RECW_REAR_BLOCKAGE_SPEED_THRESHOLD},
      {&Recw_Core_Calibration_T::k_recw_rear_blockage_width, RECW_MIN_K_RECW_REAR_BLOCKAGE_WIDTH, RECW_MAX_K_RECW_REAR_BLOCKAGE_WIDTH},
      {&Recw_Core_Calibration_T::k_recw_rear_blockage_length, RECW_MIN_K_RECW_REAR_BLOCKAGE_LENGTH,
       RECW_MAX_K_RECW_REAR_BLOCKAGE_LENGTH},
      {&Recw_Core_Calibration_T::k_recw_rear_blockage_ego_speed_threshold, RECW_MIN_K_RECW_REAR_BLOCKAGE_EGO_SPEED_THRESHOLD,
       RECW_MAX_K_RECW_REAR_BLOCKAGE_EGO_SPEED_THRESHOLD},
      {&Recw_Core_Calibration_T::k_recw_heading_accuracy_threshold, RECW_MIN_K_RECW_HEADING_ACCURACY_THRESHOLD,
       RECW_MAX_K_RECW_HEADING_ACCURACY_THRESHOLD},
      {&Recw_Core_Calibration_T::k_recw_min_speed_not_stationary, RECW_MIN_K_RECW_MIN_SPEED_NOT_STATIONARY,
       RECW_MAX_K_RECW_MIN_SPEED_NOT_STATIONARY},
      {&Recw_Core_Calibration_T::k_recb_max_host_speed, RECW_MIN_K_RECB_MAX_HOST_SPEED, RECW_MAX_K_RECB_MAX_HOST_SPEED},
      {&Recw_Core_Calibration_T::k_recb_max_host_speed_hys, RECW_MIN_K_RECB_MAX_HOST_SPEED_HYS, RECW_MAX_K_RECB_MAX_HOST_SPEED_HYS},
      {&Recw_Core_Calibration_T::k_recb_max_velocity_host_standstill, RECW_MIN_K_RECB_MAX_VELOCITY_HOST_STANDSTILL,
       RECW_MAX_K_RECB_MAX_VELOCITY_HOST_STANDSTILL},
      {&Recw_Core_Calibration_T::k_recb_min_rel_velocity, RECW_MIN_K_RECB_MIN_REL_VELOCITY, RECW_MAX_K_RECB_MIN_REL_VELOCITY},
      {&Recw_Core_Calibration_T::k_recb_min_rel_velocity_hys, RECW_MIN_K_RECB_MIN_REL_VELOCITY_HYS,
       RECW_MAX_K_RECB_MIN_REL_VELOCITY_HYS},
      {&Recw_Core_Calibration_T::k_recb_max_ttc, RECW_MIN_K_RECB_MAX_TTC, RECW_MAX_K_RECB_MAX_TTC},
      {&Recw_Core_Calibration_T::k_recb_nominal_acceleration_applied, RECW_MIN_K_RECB_NOMINAL_ACCELERATION_APPLIED,
       RECW_MAX_K_RECB_NOMINAL_ACCELERATION_APPLIED},
      {&Recw_Core_Calibration_T::k_recb_accelerator_pedal_gradient_threshold, RECW_MIN_K_RECB_ACCELERATOR_PEDAL_GRADIENT_THRESHOLD,
       RECW_MAX_K_RECB_ACCELERATOR_PEDAL_GRADIENT_THRESHOLD},
   };
   /** \assert Expect correct work of Recw_Core_Cal_In_Boundary for float32_T fields. */
   Recw_Test_Recw_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Recw_Core_Cal_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-101047} \sdd{CSCSA-186520} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Core_Calibration_Test, Recw_Core_Cal_In_Boundary__Uint8_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      {&Recw_Core_Calibration_T::k_unused_padding_byte_0, RECW_MIN_K_UNUSED_PADDING_BYTE_0, RECW_MAX_K_UNUSED_PADDING_BYTE_0},
      {&Recw_Core_Calibration_T::k_recw_alert_qualifying_cycles, RECW_MIN_K_RECW_ALERT_QUALIFYING_CYCLES,
       RECW_MAX_K_RECW_ALERT_QUALIFYING_CYCLES},
      {&Recw_Core_Calibration_T::k_recw_min_age_for_close_slow_targets, RECW_MIN_K_RECW_MIN_AGE_FOR_CLOSE_SLOW_TARGETS,
       RECW_MAX_K_RECW_MIN_AGE_FOR_CLOSE_SLOW_TARGETS},
      {&Recw_Core_Calibration_T::k_recw_min_object_age, RECW_MIN_K_RECW_MIN_OBJECT_AGE, RECW_MAX_K_RECW_MIN_OBJECT_AGE},
      {&Recw_Core_Calibration_T::k_recw_min_cycles_with_min_crash_prob, RECW_MIN_K_RECW_MIN_CYCLES_WITH_MIN_CRASH_PROB,
       RECW_MAX_K_RECW_MIN_CYCLES_WITH_MIN_CRASH_PROB},
      {&Recw_Core_Calibration_T::k_recw_en_active_car_wash_logic, RECW_MIN_K_RECW_EN_ACTIVE_CAR_WASH_LOGIC,
       RECW_MAX_K_RECW_EN_ACTIVE_CAR_WASH_LOGIC},
      {&Recw_Core_Calibration_T::k_recw_lane_filter_num_consecutive_cycles, RECW_MIN_K_RECW_LANE_FILTER_NUM_CONSECUTIVE_CYCLES,
       RECW_MAX_K_RECW_LANE_FILTER_NUM_CONSECUTIVE_CYCLES},
      {&Recw_Core_Calibration_T::k_recw_max_allowed_consecutive_coasted_cycles,
       RECW_MIN_K_RECW_MAX_ALLOWED_CONSECUTIVE_COASTED_CYCLES, RECW_MAX_K_RECW_MAX_ALLOWED_CONSECUTIVE_COASTED_CYCLES},
      {&Recw_Core_Calibration_T::k_recw_rear_blockage_qualifying_cycles, RECW_MIN_K_RECW_REAR_BLOCKAGE_QUALIFYING_CYCLES,
       RECW_MAX_K_RECW_REAR_BLOCKAGE_QUALIFYING_CYCLES},
      {&Recw_Core_Calibration_T::k_recb_min_cycles_host_standstill, RECW_MIN_K_RECB_MIN_CYCLES_HOST_STANDSTILL,
       RECW_MAX_K_RECB_MIN_CYCLES_HOST_STANDSTILL},
      {&Recw_Core_Calibration_T::k_recb_ssm_braking, RECW_MIN_K_RECB_SSM_BRAKING, RECW_MAX_K_RECB_SSM_BRAKING},
      {&Recw_Core_Calibration_T::k_recb_ssm_request_cancelled, RECW_MIN_K_RECB_SSM_REQUEST_CANCELLED,
       RECW_MAX_K_RECB_SSM_REQUEST_CANCELLED},
      {&Recw_Core_Calibration_T::k_recb_integrity, RECW_MIN_K_RECB_INTEGRITY, RECW_MAX_K_RECB_INTEGRITY},
      {&Recw_Core_Calibration_T::k_recb_qualifier_nominal_acceleration, RECW_MIN_K_RECB_QUALIFIER_NOMINAL_ACCELERATION,
       RECW_MAX_K_RECB_QUALIFIER_NOMINAL_ACCELERATION},
      {&Recw_Core_Calibration_T::k_recb_max_cycles_braking_request_duration, RECW_MIN_K_RECB_MAX_CYCLES_BRAKING_REQUEST_DURATION,
       RECW_MAX_K_RECB_MAX_CYCLES_BRAKING_REQUEST_DURATION},
   };
   /** \assert Expect correct work of Recw_Core_Cal_In_Boundary for float32_T fields. */
   Recw_Test_Recw_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}


#endif /* CT_ACTIVATE_CAL_PRINT */
