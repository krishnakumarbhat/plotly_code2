/**
 * @file lcda_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for LCDA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42486}
 */

#include "lcda_calibration_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "lcda_core_calibration.c"
#include "lcda_core_calibration_check.h"
#include "lcda_core_calibration_print_functions.c"
#include "lcda_update_calibration.h"
#include "pa_reuse.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-42488} \sdd{CSCSA-186582} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Core_Calibration_Test, Lcda_Core_Cal_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Core_Cal_Print(p_testfile, &lcda_cals));
}

/**
 * Test for calibration file. Run test to check basic Lcda_Core_Cal_Update_Defaults work.
 * \uts{CSCSA-68104} \sdd{CSCSA-186583} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Core_Calibration_Test, Lcda_Core_Cal_Update_Defaults__is_reseting_k_lcda_host_activation_speed_min)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value = lcda_cals.k_lcda_host_activation_speed_min;
   lcda_cals.k_lcda_host_activation_speed_min += 1;
   Lcda_Core_Cal_Update_Defaults(&lcda_cals);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, lcda_cals.k_lcda_host_activation_speed_min);
}

/**
 * Test for calibration file. Run test to check basic Lcda_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-68105} \sdd{CSCSA-186581} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Core_Calibration_Test, Lcda_Update_Core_Cal_By_Core__is_setting_k_lcda_host_activation_speed_min)
{
   /** \arrange n/a */

   /** \action n/a */
   const float32_T orginal_value       = lcda_cals.k_lcda_host_activation_speed_min;
   const float32_T new_value           = orginal_value + 1;
   Lcda_Core_Calibration_T calibration = lcda_cals;

   calibration.k_lcda_host_activation_speed_min = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, lcda_cals.k_lcda_host_activation_speed_min);
   boolean_T result = Lcda_Update_Core_Cal_By_Core(&lcda_cals, &calibration);
   ASSERT_TRUE(result);
   EXPECT_EQ(new_value, lcda_cals.k_lcda_host_activation_speed_min);
}

template <typename T> using Single_Case_T = std::tuple<const T Lcda_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Lcda_Test_Lcda_Core_Cal_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Lcda_Core_Calibration_T calibration;
   Lcda_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Lcda_Core_Cal_In_Boundary(&calibration));
      const T Lcda_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                            = std::get<1>(single_case);
      const T max_value                            = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Lcda_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Lcda_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Lcda_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Lcda_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Lcda_Core_Cal_In_Boundary basic work for float32_T.
 * \uts{CSCSA-68106} \sdd{CSCSA-186587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Core_Calibration_Test, Lcda_Core_Cal_In_Boundary__Float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Lcda_Core_Calibration_T::k_lcda_host_activation_speed_min, LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MIN,
       LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MIN},
      {&Lcda_Core_Calibration_T::k_lcda_host_activation_speed_max, LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MAX,
       LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MAX},
      {&Lcda_Core_Calibration_T::k_lcda_host_activation_speed_max_hys, LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MAX_HYS,
       LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MAX_HYS},
      {&Lcda_Core_Calibration_T::k_lcda_distance_traveled_scale_factor, LCDA_MIN_K_LCDA_DISTANCE_TRAVELED_SCALE_FACTOR,
       LCDA_MAX_K_LCDA_DISTANCE_TRAVELED_SCALE_FACTOR},
      {&Lcda_Core_Calibration_T::k_lcda_zone_intersect_critical_point_lateral_ratio,
       LCDA_MIN_K_LCDA_ZONE_INTERSECT_CRITICAL_POINT_LATERAL_RATIO, LCDA_MAX_K_LCDA_ZONE_INTERSECT_CRITICAL_POINT_LATERAL_RATIO},
      {&Lcda_Core_Calibration_T::k_lcda_ego_lane_effective_lane_width_factor, LCDA_MIN_K_LCDA_EGO_LANE_EFFECTIVE_LANE_WIDTH_FACTOR,
       LCDA_MAX_K_LCDA_EGO_LANE_EFFECTIVE_LANE_WIDTH_FACTOR},
      {&Lcda_Core_Calibration_T::k_bsw_zone_y_hys_min, LCDA_MIN_K_BSW_ZONE_Y_HYS_MIN, LCDA_MAX_K_BSW_ZONE_Y_HYS_MIN},
      {&Lcda_Core_Calibration_T::k_bsw_zone_y_hys_max, LCDA_MIN_K_BSW_ZONE_Y_HYS_MAX, LCDA_MAX_K_BSW_ZONE_Y_HYS_MAX},
      {&Lcda_Core_Calibration_T::k_bsw_overlap_area_threshold, LCDA_MIN_K_BSW_OVERLAP_AREA_THRESHOLD,
       LCDA_MAX_K_BSW_OVERLAP_AREA_THRESHOLD},
      {&Lcda_Core_Calibration_T::k_bsw_fallback_rel_vel_thres, LCDA_MIN_K_BSW_FALLBACK_REL_VEL_THRES,
       LCDA_MAX_K_BSW_FALLBACK_REL_VEL_THRES},
      {&Lcda_Core_Calibration_T::k_bsw_fallback_rel_vel_thres_hys, LCDA_MIN_K_BSW_FALLBACK_REL_VEL_THRES_HYS,
       LCDA_MAX_K_BSW_FALLBACK_REL_VEL_THRES_HYS},
      {&Lcda_Core_Calibration_T::k_bsw_max_heading_abs, LCDA_MIN_K_BSW_MAX_HEADING_ABS, LCDA_MAX_K_BSW_MAX_HEADING_ABS},
      {&Lcda_Core_Calibration_T::k_bsw_min_obj_long_vel, LCDA_MIN_K_BSW_MIN_OBJ_LONG_VEL, LCDA_MAX_K_BSW_MIN_OBJ_LONG_VEL},
      {&Lcda_Core_Calibration_T::k_cvw_zone_y_hys_max, LCDA_MIN_K_CVW_ZONE_Y_HYS_MAX, LCDA_MAX_K_CVW_ZONE_Y_HYS_MAX},
      {&Lcda_Core_Calibration_T::k_cvw_zone_y_hys_min, LCDA_MIN_K_CVW_ZONE_Y_HYS_MIN, LCDA_MAX_K_CVW_ZONE_Y_HYS_MIN},
      {&Lcda_Core_Calibration_T::k_cvw_ttc, LCDA_MIN_K_CVW_TTC, LCDA_MAX_K_CVW_TTC},
      {&Lcda_Core_Calibration_T::k_cvw_ttc_hys, LCDA_MIN_K_CVW_TTC_HYS, LCDA_MAX_K_CVW_TTC_HYS},
      {&Lcda_Core_Calibration_T::k_cvw_max_curvi_heading_abs, LCDA_MIN_K_CVW_MAX_CURVI_HEADING_ABS,
       LCDA_MAX_K_CVW_MAX_CURVI_HEADING_ABS},
      {&Lcda_Core_Calibration_T::k_cvw_min_obj_curvi_long_vel, LCDA_MIN_K_CVW_MIN_OBJ_CURVI_LONG_VEL,
       LCDA_MAX_K_CVW_MIN_OBJ_CURVI_LONG_VEL},
      {&Lcda_Core_Calibration_T::k_cvw_object_curvi_relative_speed_hys, LCDA_MIN_K_CVW_OBJECT_CURVI_RELATIVE_SPEED_HYS,
       LCDA_MAX_K_CVW_OBJECT_CURVI_RELATIVE_SPEED_HYS},
      {&Lcda_Core_Calibration_T::k_cvw_max_object_curvi_relative_speed, LCDA_MIN_K_CVW_MAX_OBJECT_CURVI_RELATIVE_SPEED,
       LCDA_MAX_K_CVW_MAX_OBJECT_CURVI_RELATIVE_SPEED},
      {&Lcda_Core_Calibration_T::k_lcda_min_exist_prop, LCDA_MIN_K_LCDA_MIN_EXIST_PROP, LCDA_MAX_K_LCDA_MIN_EXIST_PROP},
      {&Lcda_Core_Calibration_T::k_lcda_host_activation_speed_min_hys, LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MIN_HYS,
       LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MIN_HYS},
      {&Lcda_Core_Calibration_T::k_lcda_min_curve_radius, LCDA_MIN_K_LCDA_MIN_CURVE_RADIUS, LCDA_MAX_K_LCDA_MIN_CURVE_RADIUS},
      {&Lcda_Core_Calibration_T::k_lcda_min_curve_radius_hys, LCDA_MIN_K_LCDA_MIN_CURVE_RADIUS_HYS,
       LCDA_MAX_K_LCDA_MIN_CURVE_RADIUS_HYS},
      {&Lcda_Core_Calibration_T::k_lcda_curve_radius_threshold_for_zone_adaptation,
       LCDA_MIN_K_LCDA_CURVE_RADIUS_THRESHOLD_FOR_ZONE_ADAPTATION, LCDA_MAX_K_LCDA_CURVE_RADIUS_THRESHOLD_FOR_ZONE_ADAPTATION},
      {&Lcda_Core_Calibration_T::k_lcda_min_lane_width, LCDA_MIN_K_LCDA_MIN_LANE_WIDTH, LCDA_MAX_K_LCDA_MIN_LANE_WIDTH},
      {&Lcda_Core_Calibration_T::k_lcda_max_lane_width, LCDA_MIN_K_LCDA_MAX_LANE_WIDTH, LCDA_MAX_K_LCDA_MAX_LANE_WIDTH},
      {&Lcda_Core_Calibration_T::k_lcda_min_ego_vehicle_width, LCDA_MIN_K_LCDA_MIN_EGO_VEHICLE_WIDTH,
       LCDA_MAX_K_LCDA_MIN_EGO_VEHICLE_WIDTH},
      {&Lcda_Core_Calibration_T::k_lcda_max_ego_vehicle_width, LCDA_MIN_K_LCDA_MAX_EGO_VEHICLE_WIDTH,
       LCDA_MAX_K_LCDA_MAX_EGO_VEHICLE_WIDTH},
      {&Lcda_Core_Calibration_T::k_lcda_min_ego_vehicle_length, LCDA_MIN_K_LCDA_MIN_EGO_VEHICLE_LENGTH,
       LCDA_MAX_K_LCDA_MIN_EGO_VEHICLE_LENGTH},
      {&Lcda_Core_Calibration_T::k_lcda_max_ego_vehicle_length, LCDA_MIN_K_LCDA_MAX_EGO_VEHICLE_LENGTH,
       LCDA_MAX_K_LCDA_MAX_EGO_VEHICLE_LENGTH},
      {&Lcda_Core_Calibration_T::k_lcda_exist_prob_lc_intention_hys_offset, LCDA_MIN_K_LCDA_EXIST_PROB_LC_INTENTION_HYS_OFFSET,
       LCDA_MAX_K_LCDA_EXIST_PROB_LC_INTENTION_HYS_OFFSET},
      {&Lcda_Core_Calibration_T::k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone,
       LCDA_MIN_K_LCDA_LC_INTENTION_GUARDRAIL_MIN_LAT_POS_TO_USE_SMALL_ZONE,
       LCDA_MAX_K_LCDA_LC_INTENTION_GUARDRAIL_MIN_LAT_POS_TO_USE_SMALL_ZONE},
      {&Lcda_Core_Calibration_T::k_lcda_exist_prob_hys_offset, LCDA_MIN_K_LCDA_EXIST_PROB_HYS_OFFSET,
       LCDA_MAX_K_LCDA_EXIST_PROB_HYS_OFFSET},
      {&Lcda_Core_Calibration_T::k_lcda_lane_change_intention_vel_lat_thresh, LCDA_MIN_K_LCDA_LANE_CHANGE_INTENTION_VEL_LAT_THRESH,
       LCDA_MAX_K_LCDA_LANE_CHANGE_INTENTION_VEL_LAT_THRESH},
      {&Lcda_Core_Calibration_T::k_lcda_min_exist_prob_lc_intention, LCDA_MIN_K_LCDA_MIN_EXIST_PROB_LC_INTENTION,
       LCDA_MAX_K_LCDA_MIN_EXIST_PROB_LC_INTENTION},
      {&Lcda_Core_Calibration_T::k_bsw_lateral_distance_zone, LCDA_MIN_K_BSW_LATERAL_DISTANCE_ZONE,
       LCDA_MAX_K_BSW_LATERAL_DISTANCE_ZONE},
      {&Lcda_Core_Calibration_T::k_bsw_warntrigger_early, LCDA_MIN_K_BSW_WARNTRIGGER_EARLY, LCDA_MAX_K_BSW_WARNTRIGGER_EARLY},
      {&Lcda_Core_Calibration_T::k_bsw_warntrigger_late, LCDA_MIN_K_BSW_WARNTRIGGER_LATE, LCDA_MAX_K_BSW_WARNTRIGGER_LATE},
      {&Lcda_Core_Calibration_T::k_min_exist_prob_radar_guardrail, LCDA_MIN_K_MIN_EXIST_PROB_RADAR_GUARDRAIL,
       LCDA_MAX_K_MIN_EXIST_PROB_RADAR_GUARDRAIL},
      {&Lcda_Core_Calibration_T::k_min_exist_prob_camera_guardrail, LCDA_MIN_K_MIN_EXIST_PROB_CAMERA_GUARDRAIL,
       LCDA_MAX_K_MIN_EXIST_PROB_CAMERA_GUARDRAIL},
      {&Lcda_Core_Calibration_T::k_bsw_dynzone_speed_dropback_max, LCDA_MIN_K_BSW_DYNZONE_SPEED_DROPBACK_MAX,
       LCDA_MAX_K_BSW_DYNZONE_SPEED_DROPBACK_MAX},
      {&Lcda_Core_Calibration_T::k_bsw_min_length_long_object, LCDA_MIN_K_BSW_MIN_LENGTH_LONG_OBJECT,
       LCDA_MAX_K_BSW_MIN_LENGTH_LONG_OBJECT},
      {&Lcda_Core_Calibration_T::k_bsw_min_length_long_object_hys, LCDA_MIN_K_BSW_MIN_LENGTH_LONG_OBJECT_HYS,
       LCDA_MAX_K_BSW_MIN_LENGTH_LONG_OBJECT_HYS},
      {&Lcda_Core_Calibration_T::k_bsw_suppress_late_warning_max_time_till_leave,
       LCDA_MIN_K_BSW_SUPPRESS_LATE_WARNING_MAX_TIME_TILL_LEAVE, LCDA_MAX_K_BSW_SUPPRESS_LATE_WARNING_MAX_TIME_TILL_LEAVE},
      {&Lcda_Core_Calibration_T::k_bsw_suppress_late_warning_min_pos_behind_host,
       LCDA_MIN_K_BSW_SUPPRESS_LATE_WARNING_MIN_POS_BEHIND_HOST, LCDA_MAX_K_BSW_SUPPRESS_LATE_WARNING_MIN_POS_BEHIND_HOST},
      {&Lcda_Core_Calibration_T::k_bsw_suppress_late_warning_max_rel_vel, LCDA_MIN_K_BSW_SUPPRESS_LATE_WARNING_MAX_REL_VEL,
       LCDA_MAX_K_BSW_SUPPRESS_LATE_WARNING_MAX_REL_VEL},
      {&Lcda_Core_Calibration_T::k_bsw_trailer_zone_min_width, LCDA_MIN_K_BSW_TRAILER_ZONE_MIN_WIDTH,
       LCDA_MAX_K_BSW_TRAILER_ZONE_MIN_WIDTH},
      {&Lcda_Core_Calibration_T::k_bsw_trailer_zone_ext_safety_margin, LCDA_MIN_K_BSW_TRAILER_ZONE_EXT_SAFETY_MARGIN,
       LCDA_MAX_K_BSW_TRAILER_ZONE_EXT_SAFETY_MARGIN},
      {&Lcda_Core_Calibration_T::k_bsw_guardrail_distance_safety_margin, LCDA_MIN_K_BSW_GUARDRAIL_DISTANCE_SAFETY_MARGIN,
       LCDA_MAX_K_BSW_GUARDRAIL_DISTANCE_SAFETY_MARGIN},
      {&Lcda_Core_Calibration_T::k_cvw_warntrigger_early, LCDA_MIN_K_CVW_WARNTRIGGER_EARLY, LCDA_MAX_K_CVW_WARNTRIGGER_EARLY},
      {&Lcda_Core_Calibration_T::k_cvw_warntrigger_late, LCDA_MIN_K_CVW_WARNTRIGGER_LATE, LCDA_MAX_K_CVW_WARNTRIGGER_LATE},
      {&Lcda_Core_Calibration_T::k_lcda_max_range, LCDA_MIN_K_LCDA_MAX_RANGE, LCDA_MAX_K_LCDA_MAX_RANGE},
      {&Lcda_Core_Calibration_T::k_cvw_gap_bridge, LCDA_MIN_K_CVW_GAP_BRIDGE, LCDA_MAX_K_CVW_GAP_BRIDGE},
      {&Lcda_Core_Calibration_T::k_cvw_candidate_ttc, LCDA_MIN_K_CVW_CANDIDATE_TTC, LCDA_MAX_K_CVW_CANDIDATE_TTC},
      {&Lcda_Core_Calibration_T::k_zone_hys_obj_width_correction, LCDA_MIN_K_ZONE_HYS_OBJ_WIDTH_CORRECTION,
       LCDA_MAX_K_ZONE_HYS_OBJ_WIDTH_CORRECTION},
      {&Lcda_Core_Calibration_T::k_cvw_curve_zone_factor_outer, LCDA_MIN_K_CVW_CURVE_ZONE_FACTOR_OUTER,
       LCDA_MAX_K_CVW_CURVE_ZONE_FACTOR_OUTER},
      {&Lcda_Core_Calibration_T::k_cvw_curve_zone_factor_inner, LCDA_MIN_K_CVW_CURVE_ZONE_FACTOR_INNER,
       LCDA_MAX_K_CVW_CURVE_ZONE_FACTOR_INNER},
      {&Lcda_Core_Calibration_T::k_cvw_time_obj_start_decel_after_lane_change,
       LCDA_MIN_K_CVW_TIME_OBJ_START_DECEL_AFTER_LANE_CHANGE, LCDA_MAX_K_CVW_TIME_OBJ_START_DECEL_AFTER_LANE_CHANGE},
      {&Lcda_Core_Calibration_T::k_cvw_time_diff_after_obj_decel, LCDA_MIN_K_CVW_TIME_DIFF_AFTER_OBJ_DECEL,
       LCDA_MAX_K_CVW_TIME_DIFF_AFTER_OBJ_DECEL},
      {&Lcda_Core_Calibration_T::k_cvw_crit_dist_hys_factor, LCDA_MIN_K_CVW_CRIT_DIST_HYS_FACTOR, LCDA_MAX_K_CVW_CRIT_DIST_HYS_FACTOR},
      {&Lcda_Core_Calibration_T::k_cvw_crit_dist_additive_hys, LCDA_MIN_K_CVW_CRIT_DIST_ADDITIVE_HYS,
       LCDA_MAX_K_CVW_CRIT_DIST_ADDITIVE_HYS},
      {&Lcda_Core_Calibration_T::k_cvw_critical_obj_decel_after_lane_change, LCDA_MIN_K_CVW_CRITICAL_OBJ_DECEL_AFTER_LANE_CHANGE,
       LCDA_MAX_K_CVW_CRITICAL_OBJ_DECEL_AFTER_LANE_CHANGE},
      {&Lcda_Core_Calibration_T::k_cvw_y_width1, LCDA_MIN_K_CVW_Y_WIDTH1, LCDA_MAX_K_CVW_Y_WIDTH1},
      {&Lcda_Core_Calibration_T::k_cvw_x_length1, LCDA_MIN_K_CVW_X_LENGTH1, LCDA_MAX_K_CVW_X_LENGTH1},
      {&Lcda_Core_Calibration_T::k_cvw_y1, LCDA_MIN_K_CVW_Y1, LCDA_MAX_K_CVW_Y1},
      {&Lcda_Core_Calibration_T::k_cvw_x_length0, LCDA_MIN_K_CVW_X_LENGTH0, LCDA_MAX_K_CVW_X_LENGTH0},
      {&Lcda_Core_Calibration_T::k_cvw_y_width0, LCDA_MIN_K_CVW_Y_WIDTH0, LCDA_MAX_K_CVW_Y_WIDTH0},
      {&Lcda_Core_Calibration_T::k_cvw_y0, LCDA_MIN_K_CVW_Y0, LCDA_MAX_K_CVW_Y0},
      {&Lcda_Core_Calibration_T::k_cvw_x0, LCDA_MIN_K_CVW_X0, LCDA_MAX_K_CVW_X0},
      {&Lcda_Core_Calibration_T::k_bsw_y1_hys, LCDA_MIN_K_BSW_Y1_HYS, LCDA_MAX_K_BSW_Y1_HYS},
      {&Lcda_Core_Calibration_T::k_bsw_y0_hys, LCDA_MIN_K_BSW_Y0_HYS, LCDA_MAX_K_BSW_Y0_HYS},
      {&Lcda_Core_Calibration_T::k_bsw_x1_hys, LCDA_MIN_K_BSW_X1_HYS, LCDA_MAX_K_BSW_X1_HYS},
      {&Lcda_Core_Calibration_T::k_bsw_x0_hys, LCDA_MIN_K_BSW_X0_HYS, LCDA_MAX_K_BSW_X0_HYS},
      {&Lcda_Core_Calibration_T::k_bsw_y_width, LCDA_MIN_K_BSW_Y_WIDTH, LCDA_MAX_K_BSW_Y_WIDTH},
      {&Lcda_Core_Calibration_T::k_bsw_y0, LCDA_MIN_K_BSW_Y0, LCDA_MAX_K_BSW_Y0},
      {&Lcda_Core_Calibration_T::k_bsw_x_length, LCDA_MIN_K_BSW_X_LENGTH, LCDA_MAX_K_BSW_X_LENGTH},
      {&Lcda_Core_Calibration_T::k_bsw_x0, LCDA_MIN_K_BSW_X0, LCDA_MAX_K_BSW_X0},
      {&Lcda_Core_Calibration_T::k_lka_ov_zone_width, LCDA_MIN_K_LKA_OV_ZONE_WIDTH, LCDA_MAX_K_LKA_OV_ZONE_WIDTH},
      {&Lcda_Core_Calibration_T::k_slc_max_curvi_heading_abs, LCDA_MIN_K_SLC_MAX_CURVI_HEADING_ABS,
       LCDA_MAX_K_SLC_MAX_CURVI_HEADING_ABS},
      {&Lcda_Core_Calibration_T::k_slc_min_obj_curvi_long_vel_abs, LCDA_MIN_K_SLC_MIN_OBJ_CURVI_LONG_VEL_ABS,
       LCDA_MAX_K_SLC_MIN_OBJ_CURVI_LONG_VEL_ABS},
      {&Lcda_Core_Calibration_T::k_slc_max_obj_eclipse, LCDA_MIN_K_SLC_MAX_OBJ_ECLIPSE, LCDA_MAX_K_SLC_MAX_OBJ_ECLIPSE},
      {&Lcda_Core_Calibration_T::k_slc_critical_lat_ttc, LCDA_MIN_K_SLC_CRITICAL_LAT_TTC, LCDA_MAX_K_SLC_CRITICAL_LAT_TTC},
      {&Lcda_Core_Calibration_T::k_slc_critical_lat_ttc_hys, LCDA_MIN_K_SLC_CRITICAL_LAT_TTC_HYS, LCDA_MAX_K_SLC_CRITICAL_LAT_TTC_HYS},
      {&Lcda_Core_Calibration_T::k_slc_critical_lon_ttc, LCDA_MIN_K_SLC_CRITICAL_LON_TTC, LCDA_MAX_K_SLC_CRITICAL_LON_TTC},
      {&Lcda_Core_Calibration_T::k_slc_critical_lon_ttc_hys, LCDA_MIN_K_SLC_CRITICAL_LON_TTC_HYS, LCDA_MAX_K_SLC_CRITICAL_LON_TTC_HYS},
      {&Lcda_Core_Calibration_T::k_slc_warntrigger_TTC_lat_late, LCDA_MIN_K_SLC_WARNTRIGGER_TTC_LAT_LATE,
       LCDA_MAX_K_SLC_WARNTRIGGER_TTC_LAT_LATE},
      {&Lcda_Core_Calibration_T::k_slc_warntrigger_TTC_lat_early, LCDA_MIN_K_SLC_WARNTRIGGER_TTC_LAT_EARLY,
       LCDA_MAX_K_SLC_WARNTRIGGER_TTC_LAT_EARLY},
      {&Lcda_Core_Calibration_T::k_slc_warntrigger_TTC_lon_late, LCDA_MIN_K_SLC_WARNTRIGGER_TTC_LON_LATE,
       LCDA_MAX_K_SLC_WARNTRIGGER_TTC_LON_LATE},
      {&Lcda_Core_Calibration_T::k_slc_warntrigger_TTC_lon_early, LCDA_MIN_K_SLC_WARNTRIGGER_TTC_LON_EARLY,
       LCDA_MAX_K_SLC_WARNTRIGGER_TTC_LON_EARLY},
      {&Lcda_Core_Calibration_T::k_slc_obj_lc_effective_speed_min, LCDA_MIN_K_SLC_OBJ_LC_EFFECTIVE_SPEED_MIN,
       LCDA_MAX_K_SLC_OBJ_LC_EFFECTIVE_SPEED_MIN},
      {&Lcda_Core_Calibration_T::k_elc_max_curvi_heading_abs, LCDA_MIN_K_ELC_MAX_CURVI_HEADING_ABS,
       LCDA_MAX_K_ELC_MAX_CURVI_HEADING_ABS},
      {&Lcda_Core_Calibration_T::k_elc_min_obj_curvi_long_vel_abs, LCDA_MIN_K_ELC_MIN_OBJ_CURVI_LONG_VEL_ABS,
       LCDA_MAX_K_ELC_MIN_OBJ_CURVI_LONG_VEL_ABS},
      {&Lcda_Core_Calibration_T::k_elc_critical_longitudinal_ttc, LCDA_MIN_K_ELC_CRITICAL_LONGITUDINAL_TTC,
       LCDA_MAX_K_ELC_CRITICAL_LONGITUDINAL_TTC},
      {&Lcda_Core_Calibration_T::k_elc_critical_longitudinal_ttc_hys, LCDA_MIN_K_ELC_CRITICAL_LONGITUDINAL_TTC_HYS,
       LCDA_MAX_K_ELC_CRITICAL_LONGITUDINAL_TTC_HYS},
      {&Lcda_Core_Calibration_T::k_elc_obj_safe_deceleration_threshold, LCDA_MIN_K_ELC_OBJ_SAFE_DECELERATION_THRESHOLD,
       LCDA_MAX_K_ELC_OBJ_SAFE_DECELERATION_THRESHOLD},
      {&Lcda_Core_Calibration_T::k_elc_obj_safe_deceleration_threshold_hys, LCDA_MIN_K_ELC_OBJ_SAFE_DECELERATION_THRESHOLD_HYS,
       LCDA_MAX_K_ELC_OBJ_SAFE_DECELERATION_THRESHOLD_HYS},
      {&Lcda_Core_Calibration_T::k_lm_lane_width_city, LCDA_MIN_K_LM_LANE_WIDTH_CITY, LCDA_MAX_K_LM_LANE_WIDTH_CITY},
      {&Lcda_Core_Calibration_T::k_lm_lane_width_highway, LCDA_MIN_K_LM_LANE_WIDTH_HIGHWAY, LCDA_MAX_K_LM_LANE_WIDTH_HIGHWAY},
      {&Lcda_Core_Calibration_T::k_lcda_lm_lane_width_us_default, LCDA_MIN_K_LCDA_LM_LANE_WIDTH_US_DEFAULT,
       LCDA_MAX_K_LCDA_LM_LANE_WIDTH_US_DEFAULT},
      {&Lcda_Core_Calibration_T::k_lcda_lm_lane_width_japan_default, LCDA_MIN_K_LCDA_LM_LANE_WIDTH_JAPAN_DEFAULT,
       LCDA_MAX_K_LCDA_LM_LANE_WIDTH_JAPAN_DEFAULT},
      {&Lcda_Core_Calibration_T::k_lcda_lm_lane_width_china_default, LCDA_MIN_K_LCDA_LM_LANE_WIDTH_CHINA_DEFAULT,
       LCDA_MAX_K_LCDA_LM_LANE_WIDTH_CHINA_DEFAULT},
      {&Lcda_Core_Calibration_T::k_lcda_lm_lane_width_korea_default, LCDA_MIN_K_LCDA_LM_LANE_WIDTH_KOREA_DEFAULT,
       LCDA_MAX_K_LCDA_LM_LANE_WIDTH_KOREA_DEFAULT},
      {&Lcda_Core_Calibration_T::k_lcda_lm_lane_width_germany_default, LCDA_MIN_K_LCDA_LM_LANE_WIDTH_GERMANY_DEFAULT,
       LCDA_MAX_K_LCDA_LM_LANE_WIDTH_GERMANY_DEFAULT},
      {&Lcda_Core_Calibration_T::k_lcda_lm_lane_width_defaultcountry_default, LCDA_MIN_K_LCDA_LM_LANE_WIDTH_DEFAULTCOUNTRY_DEFAULT,
       LCDA_MAX_K_LCDA_LM_LANE_WIDTH_DEFAULTCOUNTRY_DEFAULT},
      {&Lcda_Core_Calibration_T::k_lm_lane_center_offset_default, LCDA_MIN_K_LM_LANE_CENTER_OFFSET_DEFAULT,
       LCDA_MAX_K_LM_LANE_CENTER_OFFSET_DEFAULT},
      {&Lcda_Core_Calibration_T::k_lm_min_speed_hway, LCDA_MIN_K_LM_MIN_SPEED_HWAY, LCDA_MAX_K_LM_MIN_SPEED_HWAY},
      {&Lcda_Core_Calibration_T::k_lm_hys_delta_speed_hway, LCDA_MIN_K_LM_HYS_DELTA_SPEED_HWAY, LCDA_MAX_K_LM_HYS_DELTA_SPEED_HWAY},
      {&Lcda_Core_Calibration_T::k_lm_min_yawrate_city_abs, LCDA_MIN_K_LM_MIN_YAWRATE_CITY_ABS, LCDA_MAX_K_LM_MIN_YAWRATE_CITY_ABS},
      {&Lcda_Core_Calibration_T::k_lm_hys_delta_yawrate_city_abs, LCDA_MIN_K_LM_HYS_DELTA_YAWRATE_CITY_ABS,
       LCDA_MAX_K_LM_HYS_DELTA_YAWRATE_CITY_ABS},
      {&Lcda_Core_Calibration_T::k_lm_min_lane_exist_prob_percent, LCDA_MIN_K_LM_MIN_LANE_EXIST_PROB_PERCENT,
       LCDA_MAX_K_LM_MIN_LANE_EXIST_PROB_PERCENT},
      {&Lcda_Core_Calibration_T::k_lm_min_plausible_lane_width, LCDA_MIN_K_LM_MIN_PLAUSIBLE_LANE_WIDTH,
       LCDA_MAX_K_LM_MIN_PLAUSIBLE_LANE_WIDTH},
      {&Lcda_Core_Calibration_T::k_lm_max_plausible_lane_width, LCDA_MIN_K_LM_MAX_PLAUSIBLE_LANE_WIDTH,
       LCDA_MAX_K_LM_MAX_PLAUSIBLE_LANE_WIDTH},
      {&Lcda_Core_Calibration_T::k_lm_max_plausible_lc_offset_factor, LCDA_MIN_K_LM_MAX_PLAUSIBLE_LC_OFFSET_FACTOR,
       LCDA_MAX_K_LM_MAX_PLAUSIBLE_LC_OFFSET_FACTOR},
   };
   /** \assert Expect correct work of Lcda_Core_Cal_In_Boundary for float32_T fields. */
   Lcda_Test_Lcda_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Lcda_Core_Cal_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-68107} \sdd{CSCSA-186587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Core_Calibration_Test, Lcda_Core_Cal_In_Boundary__Uint8_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      {&Lcda_Core_Calibration_T::k_bsw_alert_holding_cycles, LCDA_MIN_K_BSW_ALERT_HOLDING_CYCLES, LCDA_MAX_K_BSW_ALERT_HOLDING_CYCLES},
      {&Lcda_Core_Calibration_T::k_cvw_alert_holding_cycles, LCDA_MIN_K_CVW_ALERT_HOLDING_CYCLES, LCDA_MAX_K_CVW_ALERT_HOLDING_CYCLES},
      {&Lcda_Core_Calibration_T::k_lcda_zone_check_method, LCDA_MIN_K_LCDA_ZONE_CHECK_METHOD, LCDA_MAX_K_LCDA_ZONE_CHECK_METHOD},
      {&Lcda_Core_Calibration_T::k_bsw_min_mature_cycles, LCDA_MIN_K_BSW_MIN_MATURE_CYCLES, LCDA_MAX_K_BSW_MIN_MATURE_CYCLES},
      {&Lcda_Core_Calibration_T::k_lcda_min_track_age, LCDA_MIN_K_LCDA_MIN_TRACK_AGE, LCDA_MAX_K_LCDA_MIN_TRACK_AGE},
      {&Lcda_Core_Calibration_T::k_cvw_min_mature_cycles, LCDA_MIN_K_CVW_MIN_MATURE_CYCLES, LCDA_MAX_K_CVW_MIN_MATURE_CYCLES},
      {&Lcda_Core_Calibration_T::k_elc_alert_holding_cycles, LCDA_MIN_K_ELC_ALERT_HOLDING_CYCLES, LCDA_MAX_K_ELC_ALERT_HOLDING_CYCLES},
      {&Lcda_Core_Calibration_T::k_slc_alert_holding_cycles, LCDA_MIN_K_SLC_ALERT_HOLDING_CYCLES, LCDA_MAX_K_SLC_ALERT_HOLDING_CYCLES},
      {&Lcda_Core_Calibration_T::k_lcda_turn_signal_coast_cycles, LCDA_MIN_K_LCDA_TURN_SIGNAL_COAST_CYCLES,
       LCDA_MAX_K_LCDA_TURN_SIGNAL_COAST_CYCLES},
      {&Lcda_Core_Calibration_T::k_lcda_lc_intention_cycles_for_zone_change_threshold,
       LCDA_MIN_K_LCDA_LC_INTENTION_CYCLES_FOR_ZONE_CHANGE_THRESHOLD, LCDA_MAX_K_LCDA_LC_INTENTION_CYCLES_FOR_ZONE_CHANGE_THRESHOLD},
      {&Lcda_Core_Calibration_T::k_bsw_fallback_fast_to_slow_qual_thres, LCDA_MIN_K_BSW_FALLBACK_FAST_TO_SLOW_QUAL_THRES,
       LCDA_MAX_K_BSW_FALLBACK_FAST_TO_SLOW_QUAL_THRES},
      {&Lcda_Core_Calibration_T::k_bsw_alert_track_age, LCDA_MIN_K_BSW_ALERT_TRACK_AGE, LCDA_MAX_K_BSW_ALERT_TRACK_AGE},
      {&Lcda_Core_Calibration_T::k_bsw_stop_alert_reaching_front_custom_limit_mode,
       LCDA_MIN_K_BSW_STOP_ALERT_REACHING_FRONT_CUSTOM_LIMIT_MODE, LCDA_MAX_K_BSW_STOP_ALERT_REACHING_FRONT_CUSTOM_LIMIT_MODE},
      {&Lcda_Core_Calibration_T::k_cvw_min_mature_cycles_lc_intention, LCDA_MIN_K_CVW_MIN_MATURE_CYCLES_LC_INTENTION,
       LCDA_MAX_K_CVW_MIN_MATURE_CYCLES_LC_INTENTION},
      {&Lcda_Core_Calibration_T::k_cvw_zone_calculation_mode, LCDA_MIN_K_CVW_ZONE_CALCULATION_MODE,
       LCDA_MAX_K_CVW_ZONE_CALCULATION_MODE},
      {&Lcda_Core_Calibration_T::k_slc_min_mature_cycles, LCDA_MIN_K_SLC_MIN_MATURE_CYCLES, LCDA_MAX_K_SLC_MIN_MATURE_CYCLES},
      {&Lcda_Core_Calibration_T::k_slc_alert_qualifying_counter, LCDA_MIN_K_SLC_ALERT_QUALIFYING_COUNTER,
       LCDA_MAX_K_SLC_ALERT_QUALIFYING_COUNTER},
      {&Lcda_Core_Calibration_T::k_elc_min_mature_cycles, LCDA_MIN_K_ELC_MIN_MATURE_CYCLES, LCDA_MAX_K_ELC_MIN_MATURE_CYCLES},
      {&Lcda_Core_Calibration_T::k_lcda_f_enable_camera_based_guardrail, LCDA_MIN_K_LCDA_F_ENABLE_CAMERA_BASED_GUARDRAIL,
       LCDA_MAX_K_LCDA_F_ENABLE_CAMERA_BASED_GUARDRAIL},
      {&Lcda_Core_Calibration_T::k_lcda_default_warntrigger_hmi, LCDA_MIN_K_LCDA_DEFAULT_WARNTRIGGER_HMI,
       LCDA_MAX_K_LCDA_DEFAULT_WARNTRIGGER_HMI},
      {&Lcda_Core_Calibration_T::k_lm_min_qualification_cycle_count, LCDA_MIN_K_LM_MIN_QUALIFICATION_CYCLE_COUNT,
       LCDA_MAX_K_LM_MIN_QUALIFICATION_CYCLE_COUNT},
      {&Lcda_Core_Calibration_T::k_lm_min_output_hold_cycles, LCDA_MIN_K_LM_MIN_OUTPUT_HOLD_CYCLES,
       LCDA_MAX_K_LM_MIN_OUTPUT_HOLD_CYCLES},
   };
   /** \assert Expect correct work of Lcda_Core_Cal_In_Boundary for float32_T fields. */
   Lcda_Test_Lcda_Core_Cal_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif /* CT_ACTIVATE_CAL_PRINT */
