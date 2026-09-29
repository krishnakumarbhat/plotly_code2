/**
 * @file ccta_calibration_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for CTA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41839}
 */

#include "cta_core_calibration_test.hpp"

extern "C"
{
#include "cta_core_calibration.c"
#include "cta_core_calibration_check.h"
#include "cta_public_calibration.h"
#include "cta_update_calibration.h"
#include "pa_obj_in.h"
}


#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * Test for calibration file. Run test for print function.
 * \uts{CSCSA-41841} \sdd{CSCSA-186433} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Core_Calibration_Print__check_that_no_fatal_failure_occures)
{
   /** \arrange Set up variable */
   FILE *p_testfile = stdout;

   /** \action n/a */

   /** \assert Expect no failures. */
   EXPECT_NO_FATAL_FAILURE(Cta_Core_Cal_Print(p_testfile, &cta_cal));
}

/**
 * Test for calibration file. Run test to check basic Cta_Update_Cal_Defaults work.
 * \uts{CSCSA-70344} \sdd{CSCSA-186433} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Core_Calibration_Update_Defaults__is_reseting_k_cta_cycle_count_hold_true_warning)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value = cta_cal.k_cta_cycle_count_hold_true_warning;
   cta_cal.k_cta_cycle_count_hold_true_warning += 1;
   Cta_Core_Cal_Update_Defaults(&cta_cal);

   /** \assert Expect equal adress. */
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
}

/**
 * Test for calibration file. Run test to check basic Cta_Update_Core_Cal_By_Core work.
 * \uts{CSCSA-70345} \sdd{CSCSA-186434} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Update_Core_Cal_By_Core__is_setting_k_cta_cycle_count_hold_true_warning)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value        = cta_cal.k_cta_cycle_count_hold_true_warning;
   const uint8_t new_value            = orginal_value + 1;
   Cta_Core_Calibration_T calibration = cta_cal;

   calibration.k_cta_cycle_count_hold_true_warning = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
   boolean_T result = Cta_Update_Core_Cal_By_Core(&cta_cal, &calibration);
   EXPECT_TRUE(result);
   EXPECT_EQ(new_value, cta_cal.k_cta_cycle_count_hold_true_warning);
}

/**
 * Test for calibration file. Run test to check Cta_Update_Core_Cal_By_Core error handing.
 * \uts{CSCSA-70346} \sdd{CSCSA-186434} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Update_Core_Cal_By_Core__error_handing)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value = cta_cal.k_cta_cycle_count_hold_true_warning;
   const uint8_t new_value     = CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING + 1;

   Cta_Core_Calibration_T calibration = cta_cal;

   calibration.k_cta_cycle_count_hold_true_warning = new_value;

   /** \assert Expect correct values. */
   EXPECT_FALSE(Cta_Update_Core_Cal_By_Core(&cta_cal, nullptr));
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
   EXPECT_FALSE(Cta_Update_Core_Cal_By_Core(&cta_cal, &calibration));
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
}

/**
 * Test for calibration file. Run test to check basic Cta_Update_Core_Cal_By_Public work.
 * \uts{CSCSA-277511} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Update_Core_Cal_By_Public__is_setting_k_cta_cycle_count_hold_true_warning)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value          = cta_cal.k_cta_cycle_count_hold_true_warning;
   const uint8_t new_value              = orginal_value + 1;
   Cta_Public_Calibration_T calibration = {};
   Cta_Public_Cal_Update_Defaults(&calibration);

   calibration.k_cta_cycle_count_hold_true_warning = new_value;

   /** \assert Expect correct values. */
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
   boolean_T result = Cta_Update_Core_Cal_By_Public(&cta_cal, &calibration);
   EXPECT_TRUE(result);
   EXPECT_EQ(new_value, cta_cal.k_cta_cycle_count_hold_true_warning);
}

/**
 * Test for calibration file. Run test to check Cta_Update_Core_Cal_By_Core error handing.
 * \uts{CSCSA-277512} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Update_Core_Cal_By_Public__error_handing)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t orginal_value = cta_cal.k_cta_cycle_count_hold_true_warning;
   const uint8_t new_value     = CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING + 1;

   Cta_Public_Calibration_T calibration = {};
   Cta_Public_Cal_Update_Defaults(&calibration);

   calibration.k_cta_cycle_count_hold_true_warning = new_value;

   /** \assert Expect correct values. */
   EXPECT_FALSE(Cta_Update_Core_Cal_By_Public(&cta_cal, nullptr));
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
   EXPECT_FALSE(Cta_Update_Core_Cal_By_Public(&cta_cal, &calibration));
   EXPECT_EQ(orginal_value, cta_cal.k_cta_cycle_count_hold_true_warning);
}


/**
 * Test for calibration file. Run test to check Cta_Update_Customer_Cal_By_Public error handing.
 * \uts{CSCSA-277513} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Update_Customer_Cal_By_Public__error_handing)
{
   /** \arrange n/a */

   /** \action n/a */
   const uint8_t new_value = CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING + 1;

   Cta_Customer_Calibration_T customer_calibration = {};

   Cta_Public_Calibration_T calibration = {};
   Cta_Public_Cal_Update_Defaults(&calibration);

   calibration.k_cta_cycle_count_hold_true_warning = new_value;

   /** \assert Expect correct values. */
   EXPECT_FALSE(Cta_Update_Customer_Cal_By_Public(&customer_calibration, nullptr));
   EXPECT_FALSE(Cta_Update_Customer_Cal_By_Public(&customer_calibration, &calibration));
}


template <typename T> using Single_Case_T = std::tuple<const T Cta_Core_Calibration_T::*, T, T>;

template <typename T> using All_Cases_T = std::vector<Single_Case_T<T>>;

template <typename T> void Cta_Test_Cta_Are_Calibrations_In_Boundary_For_Single_Member_Type(const All_Cases_T<T> &all_cases)
{
   Cta_Core_Calibration_T calibration;
   Cta_Core_Cal_Update_Defaults(&calibration);
   for (const Single_Case_T<T> &single_case : all_cases)
   {
      EXPECT_TRUE(Cta_Core_Cal_In_Boundary(&calibration));
      const T Cta_Core_Calibration_T::*member_ptr = std::get<0>(single_case);
      const T min_value                           = std::get<1>(single_case);
      const T max_value                           = std::get<2>(single_case);

      if (std::numeric_limits<T>::min() < min_value)
      {
         const_cast<T &>(calibration.*member_ptr) = min_value - 1;
         EXPECT_FALSE(Cta_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = min_value;
      EXPECT_TRUE(Cta_Core_Cal_In_Boundary(&calibration));

      if (std::numeric_limits<T>::max() > max_value)
      {
         const_cast<T &>(calibration.*member_ptr) = max_value + 1;
         EXPECT_FALSE(Cta_Core_Cal_In_Boundary(&calibration));
      }

      const_cast<T &>(calibration.*member_ptr) = max_value;
      EXPECT_TRUE(Cta_Core_Cal_In_Boundary(&calibration));
   }
}


/**
 * Test for calibration file. Run test to check Cta_Core_Calibration_In_Boundary basic work for float32_T.
 * \uts{CSCSA-70347} \sdd{CSCSA-186506} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Are_Calibrations_In_Boundary__float32_T)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<float32_T> all_cases = {
      {&Cta_Core_Calibration_T::k_cta_ego_abs_speed_max, CTA_MIN_K_CTA_EGO_ABS_SPEED_MAX, CTA_MAX_K_CTA_EGO_ABS_SPEED_MAX},
      {&Cta_Core_Calibration_T::k_cta_min_speed, CTA_MIN_K_CTA_MIN_SPEED, CTA_MAX_K_CTA_MIN_SPEED},
      {&Cta_Core_Calibration_T::k_cta_max_speed, CTA_MIN_K_CTA_MAX_SPEED, CTA_MAX_K_CTA_MAX_SPEED},
      {&Cta_Core_Calibration_T::k_cta_stop_alert_ttc, CTA_MIN_K_CTA_STOP_ALERT_TTC, CTA_MAX_K_CTA_STOP_ALERT_TTC},
      {&Cta_Core_Calibration_T::k_cta_min_park_angle, CTA_MIN_K_CTA_MIN_PARK_ANGLE, CTA_MAX_K_CTA_MIN_PARK_ANGLE},
      {&Cta_Core_Calibration_T::k_cta_min_lateral_approach_speed, CTA_MIN_K_CTA_MIN_LATERAL_APPROACH_SPEED,
       CTA_MAX_K_CTA_MIN_LATERAL_APPROACH_SPEED},
      {&Cta_Core_Calibration_T::k_cta_min_rel_existence_probability, CTA_MIN_K_CTA_MIN_REL_EXISTENCE_PROBABILITY,
       CTA_MAX_K_CTA_MIN_REL_EXISTENCE_PROBABILITY},
      {&Cta_Core_Calibration_T::k_cta_rel_warning_hysteresis, CTA_MIN_K_CTA_REL_WARNING_HYSTERESIS,
       CTA_MAX_K_CTA_REL_WARNING_HYSTERESIS},
      {&Cta_Core_Calibration_T::k_cta_max_length_fov, CTA_MIN_K_CTA_MAX_LENGTH_FOV, CTA_MAX_K_CTA_MAX_LENGTH_FOV},
      {&Cta_Core_Calibration_T::k_cta_min_host_speed_to_discard_pt_info, CTA_MIN_K_CTA_MIN_HOST_SPEED_TO_DISCARD_PT_INFO,
       CTA_MAX_K_CTA_MIN_HOST_SPEED_TO_DISCARD_PT_INFO},
      {&Cta_Core_Calibration_T::k_cta_obj_dist_to_discard_pt_info, CTA_MIN_K_CTA_OBJ_DIST_TO_DISCARD_PT_INFO,
       CTA_MAX_K_CTA_OBJ_DIST_TO_DISCARD_PT_INFO},
      {&Cta_Core_Calibration_T::k_cta_ghost_condition_max_heading_diff_path_tracker,
       CTA_MIN_K_CTA_GHOST_CONDITION_MAX_HEADING_DIFF_PATH_TRACKER, CTA_MAX_K_CTA_GHOST_CONDITION_MAX_HEADING_DIFF_PATH_TRACKER},
      {&Cta_Core_Calibration_T::k_cta_max_obstruction_probability, CTA_MIN_K_CTA_MAX_OBSTRUCTION_PROBABILITY,
       CTA_MAX_K_CTA_MAX_OBSTRUCTION_PROBABILITY},
      {&Cta_Core_Calibration_T::k_cta_min_ttc_additional_mature_qualification,
       CTA_MIN_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION, CTA_MAX_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION},
      {&Cta_Core_Calibration_T::k_cta_max_object_eclipse_for_level_qualification,
       CTA_MIN_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION, CTA_MAX_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION},
      {&Cta_Core_Calibration_T::k_ctb_lower_safety_distance_thres, CTA_MIN_K_CTB_LOWER_SAFETY_DISTANCE_THRES,
       CTA_MAX_K_CTB_LOWER_SAFETY_DISTANCE_THRES},
      {&Cta_Core_Calibration_T::k_ctb_event_time_buffer, CTA_MIN_K_CTB_EVENT_TIME_BUFFER, CTA_MAX_K_CTB_EVENT_TIME_BUFFER},
      {&Cta_Core_Calibration_T::k_ctb_min_braking_time, CTA_MIN_K_CTB_MIN_BRAKING_TIME, CTA_MAX_K_CTB_MIN_BRAKING_TIME},
      {&Cta_Core_Calibration_T::k_ctb_max_braking_time, CTA_MIN_K_CTB_MAX_BRAKING_TIME, CTA_MAX_K_CTB_MAX_BRAKING_TIME},
      {&Cta_Core_Calibration_T::k_ctb_responsetime_brake_actuation, CTA_MIN_K_CTB_RESPONSETIME_BRAKE_ACTUATION,
       CTA_MAX_K_CTB_RESPONSETIME_BRAKE_ACTUATION},
      {&Cta_Core_Calibration_T::k_ctb_ramp_in_time, CTA_MIN_K_CTB_RAMP_IN_TIME, CTA_MAX_K_CTB_RAMP_IN_TIME},
      {&Cta_Core_Calibration_T::k_ctb_const_decel_after_ramp_in, CTA_MIN_K_CTB_CONST_DECEL_AFTER_RAMP_IN,
       CTA_MAX_K_CTB_CONST_DECEL_AFTER_RAMP_IN},
      {&Cta_Core_Calibration_T::k_ctb_braking_jerk, CTA_MIN_K_CTB_BRAKING_JERK, CTA_MAX_K_CTB_BRAKING_JERK},
      {&Cta_Core_Calibration_T::k_ctb_host_acc_weight, CTA_MIN_K_CTB_HOST_ACC_WEIGHT, CTA_MAX_K_CTB_HOST_ACC_WEIGHT},
      {&Cta_Core_Calibration_T::k_ctb_time_to_ask_for_final_brake_decel, CTA_MIN_K_CTB_TIME_TO_ASK_FOR_FINAL_BRAKE_DECEL,
       CTA_MAX_K_CTB_TIME_TO_ASK_FOR_FINAL_BRAKE_DECEL},
      {&Cta_Core_Calibration_T::k_cta_dist_thres_crit_level_reset, CTA_MIN_K_CTA_DIST_THRES_CRIT_LEVEL_RESET,
       CTA_MAX_K_CTA_DIST_THRES_CRIT_LEVEL_RESET},
      {&Cta_Core_Calibration_T::k_cta_accelerationpedal_gradient_threshold_ctb,
       CTA_MIN_K_CTA_ACCELERATIONPEDAL_GRADIENT_THRESHOLD_CTB, CTA_MAX_K_CTA_ACCELERATIONPEDAL_GRADIENT_THRESHOLD_CTB},
      {&Cta_Core_Calibration_T::k_cta_host_width_sensor_fov_suppr_factor, CTA_MIN_K_CTA_HOST_WIDTH_SENSOR_FOV_SUPPR_FACTOR,
       CTA_MAX_K_CTA_HOST_WIDTH_SENSOR_FOV_SUPPR_FACTOR},
      {&Cta_Core_Calibration_T::k_cta_max_heading_variance, CTA_MIN_K_CTA_MAX_HEADING_VARIANCE, CTA_MAX_K_CTA_MAX_HEADING_VARIANCE},
      {&Cta_Core_Calibration_T::k_cta_max_seg_heading_diff_no_ghost, CTA_MIN_K_CTA_MAX_SEG_HEADING_DIFF_NO_GHOST,
       CTA_MAX_K_CTA_MAX_SEG_HEADING_DIFF_NO_GHOST},
      {&Cta_Core_Calibration_T::k_cta_range_to_path_segment_ghost_qualif, CTA_MIN_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF,
       CTA_MAX_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF},
      {&Cta_Core_Calibration_T::k_cta_ttc_warntrigger_early, CTA_MIN_K_CTA_TTC_WARNTRIGGER_EARLY, CTA_MAX_K_CTA_TTC_WARNTRIGGER_EARLY},
      {&Cta_Core_Calibration_T::k_cta_ttc_warntrigger_late, CTA_MIN_K_CTA_TTC_WARNTRIGGER_LATE, CTA_MAX_K_CTA_TTC_WARNTRIGGER_LATE},
      {&Cta_Core_Calibration_T::k_cta_object_heading_exp_moving_average_alpha,
       CTA_MIN_K_CTA_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA, CTA_MAX_K_CTA_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA},
   };
   /** \assert Expect correct work of Cta_Core_Calibration_In_Boundary for float32_T fields. */
   Cta_Test_Cta_Are_Calibrations_In_Boundary_For_Single_Member_Type(all_cases);
}

/**
 * Test for calibration file. Run test to check Cta_Core_Calibration_In_Boundary basic work for uint8_t.
 * \uts{CSCSA-70348} \sdd{CSCSA-186506} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Core_Calibration_Test, Cta_Are_Calibrations_In_Boundary__uint8_t)
{
   /** \arrange n/a */

   /** \action n/a */

   All_Cases_T<uint8_t> all_cases = {
      {&Cta_Core_Calibration_T::k_cta_cycle_count_hold_true_warning, CTA_MIN_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING,
       CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING},
      {&Cta_Core_Calibration_T::k_cta_min_object_age_check_valid, CTA_MIN_K_CTA_MIN_OBJECT_AGE_CHECK_VALID,
       CTA_MAX_K_CTA_MIN_OBJECT_AGE_CHECK_VALID},
      {&Cta_Core_Calibration_T::k_cta_amount_butterfly_points_in_use, CTA_MIN_K_CTA_AMOUNT_BUTTERFLY_POINTS_IN_USE,
       CTA_MAX_K_CTA_AMOUNT_BUTTERFLY_POINTS_IN_USE},
      {&Cta_Core_Calibration_T::k_cta_cycle_count_suppress_true_warning, CTA_MIN_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING,
       CTA_MAX_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING},
      {&Cta_Core_Calibration_T::k_cta_min_object_age_thres, CTA_MIN_K_CTA_MIN_OBJECT_AGE_THRES, CTA_MAX_K_CTA_MIN_OBJECT_AGE_THRES},
      {&Cta_Core_Calibration_T::k_cta_cycles_coasted_to_ignore, CTA_MIN_K_CTA_CYCLES_COASTED_TO_IGNORE,
       CTA_MAX_K_CTA_CYCLES_COASTED_TO_IGNORE},
      {&Cta_Core_Calibration_T::k_cta_object_supress_counter, CTA_MIN_K_CTA_OBJECT_SUPRESS_COUNTER,
       CTA_MAX_K_CTA_OBJECT_SUPRESS_COUNTER},
      {&Cta_Core_Calibration_T::k_cta_ghost_validation_min_age, CTA_MIN_K_CTA_GHOST_VALIDATION_MIN_AGE,
       CTA_MAX_K_CTA_GHOST_VALIDATION_MIN_AGE},
      {&Cta_Core_Calibration_T::k_cta_ghost_validation_min_mature, CTA_MIN_K_CTA_GHOST_VALIDATION_MIN_MATURE,
       CTA_MAX_K_CTA_GHOST_VALIDATION_MIN_MATURE},
      {&Cta_Core_Calibration_T::k_cta_min_mature_cycles_level_qualifiction, CTA_MIN_K_CTA_MIN_MATURE_CYCLES_LEVEL_QUALIFICTION,
       CTA_MAX_K_CTA_MIN_MATURE_CYCLES_LEVEL_QUALIFICTION},
      {&Cta_Core_Calibration_T::k_cta_additional_qualification_mature_cycles, CTA_MIN_K_CTA_ADDITIONAL_QUALIFICATION_MATURE_CYCLES,
       CTA_MAX_K_CTA_ADDITIONAL_QUALIFICATION_MATURE_CYCLES},
      {&Cta_Core_Calibration_T::k_ctb_min_brake_qual_ctr_thres, CTA_MIN_K_CTB_MIN_BRAKE_QUAL_CTR_THRES,
       CTA_MAX_K_CTB_MIN_BRAKE_QUAL_CTR_THRES},
      {&Cta_Core_Calibration_T::k_ctb_min_brake_hold_ctr_thres, CTA_MIN_K_CTB_MIN_BRAKE_HOLD_CTR_THRES,
       CTA_MAX_K_CTB_MIN_BRAKE_HOLD_CTR_THRES},
      {&Cta_Core_Calibration_T::k_cta_addit_mature_cycles_outside_sensor_fov, CTA_MIN_K_CTA_ADDIT_MATURE_CYCLES_OUTSIDE_SENSOR_FOV,
       CTA_MAX_K_CTA_ADDIT_MATURE_CYCLES_OUTSIDE_SENSOR_FOV},
      {&Cta_Core_Calibration_T::k_cta_min_age_obj_outside_sensor_fov, CTA_MIN_K_CTA_MIN_AGE_OBJ_OUTSIDE_SENSOR_FOV,
       CTA_MAX_K_CTA_MIN_AGE_OBJ_OUTSIDE_SENSOR_FOV},
      {&Cta_Core_Calibration_T::k_cta_min_qual_age_obj_crossing_paths, CTA_MIN_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS,
       CTA_MAX_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS},
      {&Cta_Core_Calibration_T::k_cta_cycles_valid_match_of_pot_ghost, CTA_MIN_K_CTA_CYCLES_VALID_MATCH_OF_POT_GHOST,
       CTA_MAX_K_CTA_CYCLES_VALID_MATCH_OF_POT_GHOST},
      {&Cta_Core_Calibration_T::k_cta_age_for_new_creation_below_long_intersection,
       CTA_MIN_K_CTA_AGE_FOR_NEW_CREATION_BELOW_LONG_INTERSECTION, CTA_MAX_K_CTA_AGE_FOR_NEW_CREATION_BELOW_LONG_INTERSECTION},
   };
   /** \assert Expect correct work of Cta_Core_Calibration_In_Boundary for uint8_t fields. */
   Cta_Test_Cta_Are_Calibrations_In_Boundary_For_Single_Member_Type(all_cases);
}

#endif
