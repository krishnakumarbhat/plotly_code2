
/**
 * @file ta_calibration_boundary_check_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Tests the calibration range check of TA.
 *
 * Attention: This code is auto-generated - do not modify manually!
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
#ifndef TA_CALIBRATION_BOUNDARY_CHECK_TEST_HPP
#define TA_CALIBRATION_BOUNDARY_CHECK_TEST_HPP


#include "gtest/gtest.h"           // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h" // IWYU pragma: keep
extern "C"
{
#include "pa_reuse.h"              // IWYU pragma: keep
#include "ta_public_calibration.h" // IWYU pragma: keep
}

extern "C"
{
#include "ta_public_calibration_check.c" // IWYU pragma: keep
}


/**
 * @brief Used for creation of test fixtures for Ta calibrations range checks
 *
 * @return True when containing calibrations are within their boundaries
 *
 * @SRD{n/a}
 * @SAD{n/a}
 * @SDD{n/a}
 * @verification{none}
 **/
class Ta_Public_Calibration_Boundary_Check_Test : public ::testing::Test
{
 public:
   Ta_Public_Calibration_T calibration;

   void SetUp() override
   {
      Ta_Public_Cal_Update_Defaults(&calibration);
   }

   void TearDown() override
   {
   }

   void Ta_Set_Up_Middle_Of_The_Road_Calibration(void);

   template <typename T> void Ta_Set_Array_With_Default_Val(T array_in[], uint8_t array_size, T default_val)
   {
      for (uint8_t i = 0; i < array_size; i++)
      {
         array_in[i] = default_val;
      }
   }
};
void inline Ta_Public_Calibration_Boundary_Check_Test::Ta_Set_Up_Middle_Of_The_Road_Calibration(void)
{
   calibration.k_ta_alert_qualifying_cycles = ((uint8_t) (0.5f * (TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES)));
   calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels =
      ((boolean_T) (0.5f
                    * (TA_MIN_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS
                       + TA_MAX_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS)));
   calibration.k_ta_f_skip_holding_for_single_alert_level_drop =
      ((boolean_T) (0.5f
                    * (TA_MIN_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP
                       + TA_MAX_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP)));
   calibration.k_ta_alert_holding_cycles = ((uint8_t) (0.5f * (TA_MAX_K_TA_ALERT_HOLDING_CYCLES)));
   calibration.k_ta_always_overwrite_ta_mode_to_both =
      ((boolean_T) (0.5f * (TA_MIN_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH + TA_MAX_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH)));
   calibration.k_f_ta_enable_debug_mode = ((boolean_T) (0.5f * (TA_MIN_K_F_TA_ENABLE_DEBUG_MODE + TA_MAX_K_F_TA_ENABLE_DEBUG_MODE)));
   Ta_Set_Array_With_Default_Val(&calibration.k_ta_critical_approach_check_ego_circles[0], 3,
                                 ((uint8_t) (0.5f * (TA_MAX_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES))));
   calibration.k_fta_brake_deceleration_max =
      ((float32_T) (0.5f * (TA_MIN_K_FTA_BRAKE_DECELERATION_MAX + TA_MAX_K_FTA_BRAKE_DECELERATION_MAX)));
   calibration.k_fta_brake_dead_time = ((float32_T) (0.5f * (TA_MIN_K_FTA_BRAKE_DEAD_TIME + TA_MAX_K_FTA_BRAKE_DEAD_TIME)));
   calibration.k_f_fta_enable_brake_gradient_logic =
      ((boolean_T) (0.5f * (TA_MIN_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC + TA_MAX_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC)));
   calibration.k_fta_brake_gradient = ((float32_T) (0.5f * (TA_MIN_K_FTA_BRAKE_GRADIENT + TA_MAX_K_FTA_BRAKE_GRADIENT)));
   calibration.k_f_fta_enable       = ((boolean_T) (0.5f * (TA_MIN_K_F_FTA_ENABLE + TA_MAX_K_F_FTA_ENABLE)));
   calibration.k_f_fta_enable_danger_zones =
      ((boolean_T) (0.5f * (TA_MIN_K_F_FTA_ENABLE_DANGER_ZONES + TA_MAX_K_F_FTA_ENABLE_DANGER_ZONES)));
   calibration.k_fta_obj_vcs_long_pos_straight_min =
      ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN + TA_MAX_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN)));
   calibration.k_fta_obj_age_min = ((uint8_t) (0.5f * (TA_MAX_K_FTA_OBJ_AGE_MIN)));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_exist_prblty[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY + TA_MAX_K_FTA_OBJ_EXIST_PRBLTY))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_heading_straight[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_STRAIGHT + TA_MAX_K_FTA_OBJ_HEADING_STRAIGHT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_speed[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_SPEED + TA_MAX_K_FTA_OBJ_SPEED))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_speed_straight[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_SPEED_STRAIGHT + TA_MAX_K_FTA_OBJ_SPEED_STRAIGHT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_length[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_LENGTH + TA_MAX_K_FTA_OBJ_LENGTH))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_width[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_WIDTH + TA_MAX_K_FTA_OBJ_WIDTH))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_area[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_AREA + TA_MAX_K_FTA_OBJ_AREA))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vru_class_prob[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB + TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_ego_obj_heading_diff[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF + TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_eclipse_value[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE + TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE))));
   calibration.k_fta_obj_velocity_heading_diff_max =
      ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX + TA_MAX_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX)));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_exist_prblty_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_EXIST_PRBLTY_OFST + TA_MAX_K_FTA_OBJ_EXIST_PRBLTY_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_speed_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_SPEED_OFST + TA_MAX_K_FTA_OBJ_SPEED_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_length_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_LENGTH_OFST + TA_MAX_K_FTA_OBJ_LENGTH_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_width_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_WIDTH_OFST + TA_MAX_K_FTA_OBJ_WIDTH_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_area_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_AREA_OFST + TA_MAX_K_FTA_OBJ_AREA_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vru_class_prob_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB_OFST + TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB_OFST))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_fta_ego_obj_heading_diff_ofst[0], 2,
      ((float32_T) (0.5f * (TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF_OFST + TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_eclipse_value_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE_OFST + TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE_OFST))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_fta_obj_vcs_long_vel_rel_ofst[0], 2,
      ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_fta_obj_vcs_lat_vel_rel_ofst[0], 2,
      ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vcs_long_vel_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_OFST + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vcs_lat_vel_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_OFST + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_heading_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_OFST + TA_MAX_K_FTA_OBJ_HEADING_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vcs_long_vel_rel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vcs_lat_vel_rel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vcs_long_vel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LONG_VEL + TA_MAX_K_FTA_OBJ_VCS_LONG_VEL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_vcs_lat_vel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_VCS_LAT_VEL + TA_MAX_K_FTA_OBJ_VCS_LAT_VEL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_heading[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING + TA_MAX_K_FTA_OBJ_HEADING))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_heading_rate[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_RATE + TA_MAX_K_FTA_OBJ_HEADING_RATE))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_fta_obj_heading_rate_straight[0], 2,
      ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_RATE_STRAIGHT + TA_MAX_K_FTA_OBJ_HEADING_RATE_STRAIGHT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_obj_heading_rate_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_OBJ_HEADING_RATE_OFST + TA_MAX_K_FTA_OBJ_HEADING_RATE_OFST))));
   calibration.k_fta_danger_zone_point_size = ((uint8_t) (0.5f * (TA_MAX_K_FTA_DANGER_ZONE_POINT_SIZE)));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_danger_zone_left_long[0], 7,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LONG + TA_MAX_K_FTA_DANGER_ZONE_LEFT_LONG))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_danger_zone_left_lat[0], 7,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_LEFT_LAT + TA_MAX_K_FTA_DANGER_ZONE_LEFT_LAT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_danger_zone_right_long[0], 7,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LONG + TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LONG))));
   Ta_Set_Array_With_Default_Val(&calibration.k_fta_danger_zone_right_lat[0], 7,
                                 ((float32_T) (0.5f * (TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LAT + TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LAT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_ta_ego_speed[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SPEED + TA_MAX_K_TA_EGO_SPEED))));
   Ta_Set_Array_With_Default_Val(&calibration.k_ta_ego_speed_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SPEED_OFST + TA_MAX_K_TA_EGO_SPEED_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_ta_ego_yawrate[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_YAWRATE + TA_MAX_K_TA_EGO_YAWRATE))));
   Ta_Set_Array_With_Default_Val(&calibration.k_ta_ego_yawrate_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_YAWRATE_OFST + TA_MAX_K_TA_EGO_YAWRATE_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_ta_ego_long_acceleration[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_LONG_ACCELERATION + TA_MAX_K_TA_EGO_LONG_ACCELERATION))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_ta_ego_long_acceleration_ofst[0], 2,
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_LONG_ACCELERATION_OFST + TA_MAX_K_TA_EGO_LONG_ACCELERATION_OFST))));
   calibration.k_ta_straight_host_curvature_max =
      ((float32_T) (0.5f * (TA_MIN_K_TA_STRAIGHT_HOST_CURVATURE_MAX + TA_MAX_K_TA_STRAIGHT_HOST_CURVATURE_MAX)));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_ta_lookup_turning_host_speed[0], 4,
      ((float32_T) (0.5f * (TA_MIN_K_TA_LOOKUP_TURNING_HOST_SPEED + TA_MAX_K_TA_LOOKUP_TURNING_HOST_SPEED))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_ta_lookup_turning_host_curvature_min[0], 4,
      ((float32_T) (0.5f * (TA_MIN_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN + TA_MAX_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN))));
   calibration.k_pfgs_symbol_request_sides_enabled =
      ((boolean_T) (0.5f * (TA_MIN_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED + TA_MAX_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED)));
   calibration.k_pfgs_qualification_counter_fast_obj =
      ((uint8_t) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ + TA_MAX_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ)));
   calibration.k_pfgs_qualification_counter_slow_obj =
      ((uint8_t) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ + TA_MAX_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ)));
   calibration.k_pfgs_qualification_ttc_min =
      ((float32_T) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_TTC_MIN + TA_MAX_K_PFGS_QUALIFICATION_TTC_MIN)));
   calibration.k_pfgs_qualification_check_f_stationary =
      ((boolean_T) (0.5f * (TA_MIN_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY + TA_MAX_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY)));
   calibration.k_tap_lvl_2_host_curvature_min =
      ((float32_T) (0.5f * (TA_MIN_K_TAP_LVL_2_HOST_CURVATURE_MIN + TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN)));
   Ta_Set_Array_With_Default_Val(&calibration.k_pfgs_ego_speed[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_PFGS_EGO_SPEED + TA_MAX_K_PFGS_EGO_SPEED))));
   calibration.k_f_rta_enable = ((boolean_T) (0.5f * (TA_MIN_K_F_RTA_ENABLE + TA_MAX_K_F_RTA_ENABLE)));
   calibration.k_f_rta_enable_info_zones =
      ((boolean_T) (0.5f * (TA_MIN_K_F_RTA_ENABLE_INFO_ZONES + TA_MAX_K_F_RTA_ENABLE_INFO_ZONES)));
   calibration.k_f_rta_enable_wing_zones =
      ((boolean_T) (0.5f * (TA_MIN_K_F_RTA_ENABLE_WING_ZONES + TA_MAX_K_F_RTA_ENABLE_WING_ZONES)));
   calibration.k_rta_info_zone_point_size = ((uint8_t) (0.5f * (TA_MAX_K_RTA_INFO_ZONE_POINT_SIZE)));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_info_zone_left_long[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG + TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_info_zone_left_lat[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT + TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_info_zone_right_long[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_info_zone_right_lat[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_info_zone_left_long_hys[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG_HYS + TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG_HYS))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_info_zone_left_lat_hys[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT_HYS + TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT_HYS))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_rta_info_zone_right_long_hys[0], 4,
      ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG_HYS + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG_HYS))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_info_zone_right_lat_hys[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT_HYS + TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT_HYS))));
   calibration.k_rta_obj_age_min = ((uint8_t) (0.5f * (TA_MAX_K_RTA_OBJ_AGE_MIN)));
   calibration.k_rta_ttp_obj_abs_lat_vel_rel_max =
      ((float32_T) (0.5f * (TA_MIN_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX + TA_MAX_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX)));
   calibration.k_rta_ttp_obj_abs_heading_diff_max =
      ((float32_T) (0.5f * (TA_MIN_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX + TA_MAX_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX)));
   calibration.k_rta_ttp_curve_suppression_obj_distance_min =
      ((float32_T) (0.5f
                    * (TA_MIN_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN + TA_MAX_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN)));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_exist_prblty[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY + TA_MAX_K_RTA_OBJ_EXIST_PRBLTY))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_speed[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_SPEED + TA_MAX_K_RTA_OBJ_SPEED))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_length[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_LENGTH + TA_MAX_K_RTA_OBJ_LENGTH))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_width[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_WIDTH + TA_MAX_K_RTA_OBJ_WIDTH))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vru_class_prob[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB + TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_eclipse_value[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE + TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_exist_prblty_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_EXIST_PRBLTY_OFST + TA_MAX_K_RTA_OBJ_EXIST_PRBLTY_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_speed_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_SPEED_OFST + TA_MAX_K_RTA_OBJ_SPEED_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_length_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_LENGTH_OFST + TA_MAX_K_RTA_OBJ_LENGTH_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_width_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_WIDTH_OFST + TA_MAX_K_RTA_OBJ_WIDTH_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vru_class_prob_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB_OFST + TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_eclipse_value_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE_OFST + TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE_OFST))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_rta_obj_vcs_long_vel_rel_ofst[0], 2,
      ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_rta_obj_vcs_lat_vel_rel_ofst[0], 2,
      ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vcs_long_vel_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_OFST + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vcs_lat_vel_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_OFST + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_heading_ofst[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_HEADING_OFST + TA_MAX_K_RTA_OBJ_HEADING_OFST))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vcs_long_vel_rel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vcs_lat_vel_rel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vcs_long_vel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LONG_VEL + TA_MAX_K_RTA_OBJ_VCS_LONG_VEL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_vcs_lat_vel[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_VCS_LAT_VEL + TA_MAX_K_RTA_OBJ_VCS_LAT_VEL))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_obj_heading[0], 2,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_OBJ_HEADING + TA_MAX_K_RTA_OBJ_HEADING))));
   calibration.k_rta_wing_zone_point_size = ((uint8_t) (0.5f * (TA_MAX_K_RTA_WING_ZONE_POINT_SIZE)));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_wing_zone_left_long[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG + TA_MAX_K_RTA_WING_ZONE_LEFT_LONG))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_wing_zone_left_lat[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT + TA_MAX_K_RTA_WING_ZONE_LEFT_LAT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_wing_zone_right_long[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG + TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_wing_zone_right_lat[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT + TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_wing_zone_left_long_hys[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LONG_HYS + TA_MAX_K_RTA_WING_ZONE_LEFT_LONG_HYS))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_wing_zone_left_lat_hys[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_LEFT_LAT_HYS + TA_MAX_K_RTA_WING_ZONE_LEFT_LAT_HYS))));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_rta_wing_zone_right_long_hys[0], 4,
      ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG_HYS + TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG_HYS))));
   Ta_Set_Array_With_Default_Val(&calibration.k_rta_wing_zone_right_lat_hys[0], 4,
                                 ((float32_T) (0.5f * (TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT_HYS + TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT_HYS))));
   calibration.k_ta_prediction_steps_max = ((uint8_t) (0.5f * (TA_MAX_K_TA_PREDICTION_STEPS_MAX)));
   calibration.k_ta_ego_max_pred_yaw_angle =
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_MAX_PRED_YAW_ANGLE + TA_MAX_K_TA_EGO_MAX_PRED_YAW_ANGLE)));
   calibration.k_ta_obj_pred_speed_min = ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_PRED_SPEED_MIN + TA_MAX_K_TA_OBJ_PRED_SPEED_MIN)));
   calibration.k_ta_ego_acceleration_weight =
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_ACCELERATION_WEIGHT + TA_MAX_K_TA_EGO_ACCELERATION_WEIGHT)));
   calibration.k_ta_ego_deceleration_weight =
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_DECELERATION_WEIGHT + TA_MAX_K_TA_EGO_DECELERATION_WEIGHT)));
   calibration.k_ta_ego_pred_const_velocity_pred_steps_min =
      ((uint8_t) (0.5f * (TA_MAX_K_TA_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN)));
   calibration.k_ta_ego_circle_offset = ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_CIRCLE_OFFSET + TA_MAX_K_TA_EGO_CIRCLE_OFFSET)));
   calibration.k_ta_ego_circle_host_length_factor =
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR + TA_MAX_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR)));
   calibration.k_ta_ego_yawangle_integration_yawrate_min =
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN + TA_MAX_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN)));
   calibration.k_ta_ego_shape_gain_per_pred_step =
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP + TA_MAX_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP)));
   calibration.k_ta_obj_shape_gain_per_pred_step =
      ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP + TA_MAX_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP)));
   calibration.k_ta_ego_shape_gain_fixed =
      ((float32_T) (0.5f * (TA_MIN_K_TA_EGO_SHAPE_GAIN_FIXED + TA_MAX_K_TA_EGO_SHAPE_GAIN_FIXED)));
   calibration.k_ta_obj_shape_gain_fixed =
      ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_SHAPE_GAIN_FIXED + TA_MAX_K_TA_OBJ_SHAPE_GAIN_FIXED)));
   calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj =
      ((boolean_T) (0.5f
                    * (TA_MIN_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ
                       + TA_MAX_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ)));
   calibration.k_ta_active_obj_ttp_offset =
      ((float32_T) (0.5f * (TA_MIN_K_TA_ACTIVE_OBJ_TTP_OFFSET + TA_MAX_K_TA_ACTIVE_OBJ_TTP_OFFSET)));
   calibration.k_ta_f_apply_ttp_hysteresis_globally =
      ((boolean_T) (0.5f * (TA_MIN_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY + TA_MAX_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY)));
   calibration.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max =
      ((float32_T) (0.5f
                    * (TA_MIN_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX
                       + TA_MAX_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX)));
   calibration.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max =
      ((float32_T) (0.5f
                    * (TA_MIN_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX
                       + TA_MAX_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX)));
   calibration.k_ta_alert_lvl_2_ttc_threshold =
      ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_2_TTC_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_2_TTC_THRESHOLD)));
   calibration.k_ta_alert_lvl_3_ttc_threshold =
      ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_3_TTC_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_3_TTC_THRESHOLD)));
   calibration.k_ta_alert_lvl_3_ttb_threshold =
      ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_3_TTB_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_3_TTB_THRESHOLD)));
   calibration.k_ta_alert_lvl_4_ttc_threshold =
      ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_4_TTC_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_4_TTC_THRESHOLD)));
   calibration.k_ta_alert_lvl_4_decel_threshold =
      ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_4_DECEL_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_4_DECEL_THRESHOLD)));
   calibration.k_ta_critical_approach_min_safe_distance =
      ((float32_T) (0.5f * (TA_MIN_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE + TA_MAX_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE)));
   calibration.k_ta_critical_approach_angle_diff_min =
      ((float32_T) (0.5f * (TA_MIN_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN + TA_MAX_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN)));
   calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp =
      ((boolean_T) (0.5f * (TA_MIN_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP + TA_MAX_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP)));
   Ta_Set_Array_With_Default_Val(
      &calibration.k_ta_alert_lvl_1_ttp_threshold[0], 3,
      ((float32_T) (0.5f * (TA_MIN_K_TA_ALERT_LVL_1_TTP_THRESHOLD + TA_MAX_K_TA_ALERT_LVL_1_TTP_THRESHOLD))));
   calibration.k_ta_obj_acceleration_long_weight =
      ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_ACCELERATION_LONG_WEIGHT + TA_MAX_K_TA_OBJ_ACCELERATION_LONG_WEIGHT)));
   calibration.k_ta_obj_acceleration_lat_weight =
      ((float32_T) (0.5f * (TA_MIN_K_TA_OBJ_ACCELERATION_LAT_WEIGHT + TA_MAX_K_TA_OBJ_ACCELERATION_LAT_WEIGHT)));
}

#endif /*TA_CALIBRATION_BOUNDARY_CHECK_TEST_HPP*/
