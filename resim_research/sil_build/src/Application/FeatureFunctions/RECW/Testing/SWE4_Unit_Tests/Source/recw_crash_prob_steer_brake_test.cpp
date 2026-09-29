/**
 * @file recw_crash_prob_steer_brake_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is crash probability calculation test source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44267}
 */

#include "recw_crash_prob_steer_brake_test.hpp"

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_crash_prob_steer_brake.c"
}

/**
 * Set up a collision critical object and calculate its crash probablities when heading filter is disabled. Verify that plausible
 * crash probabilities are calculated. \uts{CSCSA-44268} \sdd{SF-7886} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test,
       Recw_Calculate_Crash_Probabilities__returns_plausible_crash_probs_for_critical_obj_without_heading_filter)
{
   /** \arrange Set up object that is collision critical and set cal value to disable heading filter. */
   recw_object.tracker_data.vcs_pos.x       = -8.0f;
   recw_object.tracker_data.vcs_pos.y       = -0.0f;
   recw_object.tracker_data.length          = 4.0f;
   recw_object.tracker_data.width           = 2.0f;
   p_vehicle_data->host_length              = 5.0f;
   recw_object.tracker_data.vcs_vel_rel.x   = 3.0f;
   recw_object.tracker_data.vcs_vel_rel.y   = 0.5f;
   recw_cals.k_recw_f_enable_heading_filter = 0u;
   recw_object.tracker_data.vcs_heading     = -0.1f;

   /** \action Call calculation function to get crash probablity for object. */
   Recw_Calculate_Crash_Probabilities(&recw_object, p_vehicle_data, guardrail_data, &recw_cals);

   /** \assert Verify that both crash probabilities for object are greater zero and combined probability is calculated correctly. */
   EXPECT_GT(recw_object.attributes.crash_prob_braking, 0.0f);
   EXPECT_GT(recw_object.attributes.crash_prob_steering, 0.0f);
   EXPECT_FLOAT_EQ(recw_object.attributes.crash_prob_combined,
                   Fbk_Min(recw_object.attributes.crash_prob_braking, recw_object.attributes.crash_prob_steering));
}

/**
 * Set up a collision critical object and calculate its crash probablities when heading filter is enabled. Verify that plausible
 * crash probabilities are calculated. \uts{CSCSA-44269} \sdd{SF-7886} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test,
       Recw_Calculate_Crash_Probabilities__returns_plausible_crash_probs_for_critical_obj_with_heading_filter)
{
   /** \arrange Set up object that is collision critical and set cal value to enable heading filter. */
   recw_object.tracker_data.vcs_pos.x         = -8.0f;
   recw_object.tracker_data.vcs_pos.y         = -0.0f;
   recw_object.tracker_data.length            = 4.0f;
   recw_object.tracker_data.width             = 2.0f;
   p_vehicle_data->host_length                = 5.0f;
   recw_object.tracker_data.vcs_vel_rel.x     = 3.0f;
   recw_cals.k_recw_f_enable_heading_filter   = 1u;
   recw_object.attributes.effective_rel_vel.x = 3.0f;
   recw_object.attributes.effective_rel_vel.y = 0.5f;
   recw_object.attributes.ttc                 = RECW_MAX_TTC;
   recw_object.tracker_data.vcs_heading       = -0.1f;

   /** \action Call calculation function to get crash probablity for object. */
   Recw_Calculate_Crash_Probabilities(&recw_object, p_vehicle_data, guardrail_data, &recw_cals);

   /** \assert Verify that both crash probabilities for object are greater zero and combined probability is calculated correctly. */
   EXPECT_GT(recw_object.attributes.crash_prob_braking, 0.0f);
   EXPECT_GT(recw_object.attributes.crash_prob_steering, 0.0f);
   EXPECT_FLOAT_EQ(recw_object.attributes.crash_prob_combined,
                   Fbk_Min(recw_object.attributes.crash_prob_braking, recw_object.attributes.crash_prob_steering));
}

/**
 * Calculate the needed brake acceleration for an object with negative relative velocity. Verify that the needed brake acceleration
 * is zero. \uts{CSCSA-44270} \sdd{SF-7882} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision__returns_zero_if_obj_rel_vel_smaller_zero)
{
   /** \arrange Set up function inputs (with relative velocity smaller zero) and result variable. */
   float32_T vcs_long_acc       = 0.0f;
   float32_T lon_rel_vel_obj    = -1.0f;
   float32_T lon_pos_bumper_obj = -1.0f;
   float32_T result;

   /** \action Call function to get needed brake acceleration. */
   result = Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision(vcs_long_acc, lon_rel_vel_obj, lon_pos_bumper_obj);

   /** \assert Verify that needed brake acceleration is zero. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Calculate the needed brake acceleration for an object in front of or beside of the ego. Verify that the needed brake
 * acceleration is zero. \uts{CSCSA-44271} \sdd{SF-7882} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision__returns_zero_if_obj_lon_pos_greater_zero)
{
   /** \arrange Set up function inputs (with lon pos greater zero) and result variable. */
   float32_T vcs_long_acc       = 0.0f;
   float32_T lon_rel_vel_obj    = 1.0f;
   float32_T lon_pos_bumper_obj = 1.0f;
   float32_T result;

   /** \action Call function to get needed brake acceleration. */
   result = Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision(vcs_long_acc, lon_rel_vel_obj, lon_pos_bumper_obj);

   /** \assert Verify that needed brake acceleration is zero. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Calculate the needed brake acceleration for an object very close behind the ego. Verify that the needed brake acceleration is at
 * maximum value. \uts{CSCSA-44272} \sdd{SF-7882} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision__returns_max_for_very_small_negative_lon_pos)
{
   /** \arrange Set up function inputs (with very small negative lon pos) and result variable. */
   float32_T vcs_long_acc       = 0.0f;
   float32_T lon_rel_vel_obj    = 1.0f;
   float32_T lon_pos_bumper_obj = -0.00001f;
   float32_T result;

   /** \action Call function to get needed brake acceleration. */
   result = Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision(vcs_long_acc, lon_rel_vel_obj, lon_pos_bumper_obj);

   /** \assert Verify that needed brake acceleration is at maximum value. */
   EXPECT_FLOAT_EQ(result, RECW_ACC_MAX);
}

/**
 * Calculate the needed brake acceleration for an object behind the ego when correction is not used. Verify that the needed brake
 * acceleration matches value calculated with the expected calculation formula. \uts{CSCSA-44273} \sdd{SF-7882}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision__calculates_acc_correctly_without_correction)
{
   /** \arrange Set up function inputs (with plausible critical values) and result variable. */
   float32_T vcs_long_acc       = 0.0f;
   float32_T lon_rel_vel_obj    = 2.0f;
   float32_T lon_pos_bumper_obj = -2.0f;
   float32_T result;

   /** \action Call function to get needed brake acceleration. */
   result = Recw_Get_Brake_Acc_To_Avoid_Rear_End_Collision(vcs_long_acc, lon_rel_vel_obj, lon_pos_bumper_obj);

   /** \assert Verify that needed brake acceleration matches value calculated with expected calculation formula. */
   EXPECT_FLOAT_EQ(result, -(0.5f * ((lon_rel_vel_obj * lon_rel_vel_obj) / lon_pos_bumper_obj)));
}

/**
 * Calculate the needed steering acceleration for an object with negative relative velocity. Verify that the needed steering
 * acceleration is zero. \uts{CSCSA-44274} \sdd{SF-7883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision__returns_zero_if_obj_rel_vel_smaller_zero)
{
   /** \arrange Set up function inputs (with relative velocity smaller zero) and result variable. */
   float32_T lon_rel_vel_obj       = -1.0f;
   float32_T lat_rel_vel_obj       = 0.0f;
   float32_T lon_pos_bumper_obj    = -1.0f;
   float32_T corner_lat_pos_fl_obj = 0.0f;
   float32_T corner_lat_pos_fr_obj = 0.0f;
   float32_T width_obj             = 0.0f;
   float32_T result;

   /** \action Call function to get needed steering acceleration. */
   result = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(p_vehicle_data, guardrail_data, lon_rel_vel_obj, lat_rel_vel_obj,
                                                           lon_pos_bumper_obj, corner_lat_pos_fl_obj, corner_lat_pos_fr_obj,
                                                           width_obj, &recw_cals);

   /** \assert Verify that needed steering acceleration is zero. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Calculate the needed steering acceleration for an object in front of or beside of the ego. Verify that the needed steering
 * acceleration is zero. \uts{CSCSA-44275} \sdd{SF-7883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision__returns_zero_if_obj_lon_pos_greater_zero)
{
   /** \arrange Set up function inputs (with lon pos greater zero) and result variable. */
   float32_T lon_rel_vel_obj       = 1.0f;
   float32_T lat_rel_vel_obj       = 0.5f;
   float32_T lon_pos_bumper_obj    = 1.0f;
   float32_T corner_lat_pos_fl_obj = 0.0f;
   float32_T corner_lat_pos_fr_obj = 0.0f;
   float32_T width_obj             = 2.0f;
   float32_T result;

   /** \action Call function to get needed steering acceleration. */
   result = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(p_vehicle_data, guardrail_data, lon_rel_vel_obj, lat_rel_vel_obj,
                                                           lon_pos_bumper_obj, corner_lat_pos_fl_obj, corner_lat_pos_fr_obj,
                                                           width_obj, &recw_cals);

   /** \assert Verify that needed steering acceleration is zero. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Calculate the needed steering acceleration for an object beside of the ego. Verify that the needed steering acceleration is
 * zero. \uts{CSCSA-44276} \sdd{SF-7883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision__returns_zero_if_obj_lat_pos_fl_greater_host)
{
   /** \arrange Set up function inputs (with lat pos greater host) and result variable. */
   float32_T lon_rel_vel_obj       = 1.0f;
   float32_T lat_rel_vel_obj       = 0.5f;
   float32_T lon_pos_bumper_obj    = 0.0f;
   float32_T corner_lat_pos_fl_obj = 10.0f;
   float32_T corner_lat_pos_fr_obj = 0.0f;
   float32_T width_obj             = 2.0f;
   float32_T result;

   /** \action Call function to get needed steering acceleration. */
   result = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(p_vehicle_data, guardrail_data, lon_rel_vel_obj, lat_rel_vel_obj,
                                                           lon_pos_bumper_obj, corner_lat_pos_fl_obj, corner_lat_pos_fr_obj,
                                                           width_obj, &recw_cals);

   /** \assert Verify that needed steering acceleration is zero. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Calculate the needed steering acceleration for an object beside of the ego. Verify that the needed steering acceleration is
 * zero. \uts{CSCSA-112364} \sdd{SF-7883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision__returns_zero_if_obj_lat_pos_fr_greater_host)
{
   /** \arrange Set up function inputs (with lat pos greater host) and result variable. */
   float32_T lon_rel_vel_obj       = 1.0f;
   float32_T lat_rel_vel_obj       = 0.5f;
   float32_T lon_pos_bumper_obj    = 0.0f;
   float32_T corner_lat_pos_fl_obj = 0.0f;
   float32_T corner_lat_pos_fr_obj = -10.0f;
   float32_T width_obj             = 2.0f;
   float32_T result;

   /** \action Call function to get needed steering acceleration. */
   result = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(p_vehicle_data, guardrail_data, lon_rel_vel_obj, lat_rel_vel_obj,
                                                           lon_pos_bumper_obj, corner_lat_pos_fl_obj, corner_lat_pos_fr_obj,
                                                           width_obj, &recw_cals);

   /** \assert Verify that needed steering acceleration is zero. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Calculate the needed steering acceleration for an object very close behind the ego. Verify that the needed steering acceleration
 * is at maximum value. \uts{CSCSA-44277} \sdd{SF-7883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision__returns_max_for_very_small_negative_lon_pos)
{
   /** \arrange Set up function inputs (with very small negative lon pos) and result variable. */
   float32_T lon_rel_vel_obj       = 1.0f;
   float32_T lat_rel_vel_obj       = 0.5f;
   float32_T lon_pos_bumper_obj    = -0.00001f;
   float32_T corner_lat_pos_fl_obj = 0.0f;
   float32_T corner_lat_pos_fr_obj = 0.0f;
   float32_T width_obj             = 2.0f;
   float32_T result;

   /** \action Call function to get needed steering acceleration. */
   result = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(p_vehicle_data, guardrail_data, lon_rel_vel_obj, lat_rel_vel_obj,
                                                           lon_pos_bumper_obj, corner_lat_pos_fl_obj, corner_lat_pos_fr_obj,
                                                           width_obj, &recw_cals);

   /** \assert Verify that needed steering acceleration is at maximum value. */
   EXPECT_FLOAT_EQ(result, RECW_ACC_MAX);
}

/**
 * Calculate the needed steering acceleration for an object behind the ego. Verify that the needed steering acceleration is in the
 * expected interval. \uts{CSCSA-44278} \sdd{SF-7883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision__calculates_steer_acc)
{
   /** \arrange Set up function inputs (with plausible critical values) and result variable. */
   float32_T lon_rel_vel_obj       = 1.0f;
   float32_T lat_rel_vel_obj       = 0.5f;
   float32_T lon_pos_bumper_obj    = -1.0f;
   float32_T corner_lat_pos_fl_obj = -1.0f;
   float32_T corner_lat_pos_fr_obj = 1.0f;
   float32_T width_obj             = 2.0f;
   float32_T result;

   recw_cals.k_recw_f_make_use_of_guardrail = 0u;

   /** \action Call function to get needed steering acceleration. */
   result = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(p_vehicle_data, guardrail_data, lon_rel_vel_obj, lat_rel_vel_obj,
                                                           lon_pos_bumper_obj, corner_lat_pos_fl_obj, corner_lat_pos_fr_obj,
                                                           width_obj, &recw_cals);

   /** \assert Verify that needed steering acceleration is in expected interval. */
   EXPECT_GE(result, 0.0f);
   EXPECT_LE(result, RECW_ACC_MAX);
}

/**
 * Calculate the needed steering acceleration for an object behind the ego. Verify that the needed steering acceleration is maximal
 * for existing guardrails. \uts{CSCSA-44279} \sdd{SF-7883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision__calculates_steer_RECW_ACC_MAX_guardrail)
{
   /** \arrange Set up function inputs (with plausible critical values) and result variable. */
   float32_T lon_rel_vel_obj       = 1.0f;
   float32_T lat_rel_vel_obj       = 0.5f;
   float32_T lon_pos_bumper_obj    = -1.0f;
   float32_T corner_lat_pos_fl_obj = -10.0f;
   float32_T corner_lat_pos_fr_obj = 1.0f;
   float32_T width_obj             = 2.0f;
   float32_T result;
   uint8_t side                   = PA_ENV_GUARDRAIL_SIDE_LEFT;
   guardrail_data[side].f_present = FBK_TRUE;
   guardrail_data[side].f_active  = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_MATURE;
   guardrail_data[side].lat_pos   = width_obj + (0.5f * p_vehicle_data->host_width) - EPSILON;
   side                           = PA_ENV_GUARDRAIL_SIDE_RIGHT;
   guardrail_data[side].f_present = FBK_TRUE;
   guardrail_data[side].f_active  = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_MATURE;
   guardrail_data[side].lat_pos   = width_obj + (0.5f * p_vehicle_data->host_width) - EPSILON;

   p_vehicle_data->host_width = 2.0f;

   recw_cals.k_recw_f_make_use_of_guardrail = 1u;

   /** \action Call function to get needed steering acceleration. */
   result = Recw_Get_Steer_Acc_To_Avoid_Rear_End_Collision(p_vehicle_data, guardrail_data, lon_rel_vel_obj, lat_rel_vel_obj,
                                                           lon_pos_bumper_obj, corner_lat_pos_fl_obj, corner_lat_pos_fr_obj,
                                                           width_obj, &recw_cals);

   /** \assert Verify that maximal steering acceleration is used. */
   EXPECT_FLOAT_EQ(result, RECW_ACC_MAX);
}

/**
 * Verify that a guardrail that fulfills all necessary conditions is classified as valid guardrail on left side.
 * \uts{CSCSA-44280} \sdd{SF-8009} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Is_Valid_Guardrail_Present_On_Side__returns_true_for_valid_guardrail_on_left_side)
{
   /** \arrange Set up valid guardrail for left side that fulfills all conditions. */
   boolean_T result;
   uint8_t side                   = PA_ENV_GUARDRAIL_SIDE_LEFT;
   float32_T width_obj            = 2.0f;
   guardrail_data[side].f_present = FBK_TRUE;
   guardrail_data[side].f_active  = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_MATURE;
   p_vehicle_data->host_width     = 2.0f;
   guardrail_data[side].lat_pos   = width_obj + (0.5f * p_vehicle_data->host_width) - EPSILON;

   recw_cals.k_recw_f_make_use_of_guardrail = 1u;

   /** \action Call function to evalute if valid guardrail is present. */
   result = Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[side], width_obj, &recw_cals);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Verify that no guardrail information will be used if this is disabled by calibration value.
 * \uts{CSCSA-44281} \sdd{SF-8009} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Is_Valid_Guardrail_Present_On_Side__returns_false_if_check_disabled_by_calibration)
{
   /** \arrange Set up valid guardrail for left side that fulfills all conditions, but disable check by calibration value */
   boolean_T result;
   uint8_t side                   = PA_ENV_GUARDRAIL_SIDE_LEFT;
   float32_T width_obj            = 2.0f;
   guardrail_data[side].f_present = FBK_TRUE;
   guardrail_data[side].f_active  = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_MATURE;
   p_vehicle_data->host_width     = 2.0f;
   guardrail_data[side].lat_pos   = width_obj + (0.5f * p_vehicle_data->host_width) - EPSILON;

   recw_cals.k_recw_f_make_use_of_guardrail = 0u;

   /** \action Call function to evalute if valid guardrail is present. */
   result = Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[side], width_obj, &recw_cals);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Verify that no guardrail information will be used if no guardrail is detected.
 * \uts{CSCSA-112365} \sdd{SF-8009} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Is_Valid_Guardrail_Present_On_Side__returns_false_if_no_guardrail_present)
{
   /** \arrange Set up all necessary data with no present guardrail */
   boolean_T result;
   uint8_t side                   = PA_ENV_GUARDRAIL_SIDE_LEFT;
   float32_T width_obj            = 2.0f;
   guardrail_data[side].f_present = FBK_FALSE;
   guardrail_data[side].f_active  = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_INVALID;
   p_vehicle_data->host_width     = 2.0f;
   guardrail_data[side].lat_pos   = FBK_ZERO_F;

   recw_cals.k_recw_f_make_use_of_guardrail = 1u;

   /** \action Call function to evalute if valid guardrail is present. */
   result = Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[side], width_obj, &recw_cals);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Verify that no guardrail information will be used if guardrail status is not mature.
 * \uts{CSCSA-112366} \sdd{SF-8009} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Is_Valid_Guardrail_Present_On_Side__returns_false_if_guardrail_status_not_mature)
{
   /** \arrange Set up all necessary data with guardrail status coasted */
   boolean_T result;
   uint8_t side                   = PA_ENV_GUARDRAIL_SIDE_LEFT;
   float32_T width_obj            = 2.0f;
   guardrail_data[side].f_present = FBK_TRUE;
   guardrail_data[side].f_active  = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_COASTED;
   p_vehicle_data->host_width     = 2.0f;
   guardrail_data[side].lat_pos   = FBK_ZERO_F;

   recw_cals.k_recw_f_make_use_of_guardrail = 1u;

   /** \action Call function to evalute if valid guardrail is present. */
   result = Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[side], width_obj, &recw_cals);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Verify that no guardrail information will be used if guardrail is laterally far away.
 * \uts{CSCSA-112367} \sdd{SF-8009} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Is_Valid_Guardrail_Present_On_Side__returns_false_if_guardrail_is_laterally_far)
{
   /** \arrange Set up all necessary data with guardrail laterally distanced */
   boolean_T result;
   uint8_t side                   = PA_ENV_GUARDRAIL_SIDE_LEFT;
   float32_T width_obj            = 2.0f;
   guardrail_data[side].f_present = FBK_TRUE;
   guardrail_data[side].f_active  = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_MATURE;
   p_vehicle_data->host_width     = 2.0f;
   guardrail_data[side].lat_pos   = -(width_obj + (0.5f * p_vehicle_data->host_width) + EPSILON);

   recw_cals.k_recw_f_make_use_of_guardrail = 1u;

   /** \action Call function to evalute if valid guardrail is present. */
   result = Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[side], width_obj, &recw_cals);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Verify that no guardrail information will be used if is disabled by flag.
 * \uts{CSCSA-204751} \sdd{SF-8009} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Crash_Prob_Steer_Brake_Test, Recw_Is_Valid_Guardrail_Present_On_Side__returns_false_enable_flag_false_calibration_true)
{
   /** \arrange Set up all necessary data with flag and calibration set to false */
   boolean_T result;
   uint8_t side                   = PA_ENV_GUARDRAIL_SIDE_LEFT;
   float32_T width_obj            = 2.0f;
   guardrail_data[side].f_present = FBK_TRUE;
   guardrail_data[side].status    = PA_OBJ_STATUS_MATURE;
   guardrail_data[side].lat_pos   = -(width_obj + (0.5f * p_vehicle_data->host_width) + EPSILON);

   guardrail_data[side].f_active            = FBK_FALSE;
   recw_cals.k_recw_f_make_use_of_guardrail = 1u;

   p_vehicle_data->host_width = 2.0f;

   /** \action Call function to evalute if valid guardrail is present. */
   result = Recw_Is_Valid_Guardrail_Present_On_Side(p_vehicle_data, &guardrail_data[side], width_obj, &recw_cals);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(Fbk_Is_True(guardrail_data[side].f_active));
   EXPECT_TRUE(Fbk_Is_True(recw_cals.k_recw_f_make_use_of_guardrail));
   EXPECT_FALSE(result);
}