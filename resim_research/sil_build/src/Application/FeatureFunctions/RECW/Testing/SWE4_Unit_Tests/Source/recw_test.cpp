/**
 * @file recw_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is main RECW test source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44334}
 */

#include "recw_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_core_calibration.h"
#include "recw_core_input_t.h"
#include "recw_core_output_t.h"
#include "recw_persistent_t.h"
#include "recw_types.h"
#include "gtest/gtest_pred_impl.h"
}

/**
 * Test that the RECW reset function resets core ouput and persistent data. Verify that core output and persistent data is reset.
 * \uts{CSCSA-44335} \sdd{SF-7851} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Reset__works_properly)
{
   /** \arrange Set core output variables and persistent data to non-standard values. */

   recw_instance.core_output.recw_alert_level          = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_instance.core_output.recw_id                   = 4u;
   recw_instance.core_output.recw_ttc                  = 2.0f;
   recw_instance.persistent.f_is_rear_blocked          = FBK_TRUE;
   recw_instance.persistent.recw_alert_holding_counter = 5u;
   recw_instance.persistent.recw_id_prev_cycle         = 4u;

   /** \action Call RECW reset function. */
   Recw_Reset(&recw_instance.core_output, &recw_instance.persistent);

   /** \assert Verify that core output and persistent data is reset to standard values. */
   EXPECT_EQ(recw_instance.core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_instance.core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(recw_instance.core_output.recw_ttc, RECW_MAX_TTC);
   EXPECT_FALSE(recw_instance.persistent.f_is_rear_blocked);
   EXPECT_EQ(recw_instance.persistent.recw_alert_holding_counter, FBK_ZERO_INT);
   EXPECT_EQ(recw_instance.persistent.recw_id_prev_cycle, FBK_ZERO_INT);
}

/**
 * Call the main RECW function with an active alert and active rear blockage when RECW is disabled. Verify that both, alert and
 * rear blockage is reset. \uts{CSCSA-44336} \sdd{SF-7852} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Core_Run__works_properly_for_Recw_disabled)
{
   /** \arrange Set RECW enabled flag to false and set alert and rear blockage from last cycle. */

   recw_instance.core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_instance.core_output.recw_id          = 4u;
   recw_instance.core_input.f_enable_recw     = FBK_FALSE;
   recw_instance.persistent.f_is_rear_blocked = FBK_TRUE;

   /** \action Call the main RECW function. */
   Recw_Core_Run(&recw_instance.core_output, &recw_instance.core_input, &recw_instance.calibration, &recw_instance.persistent);

   /** \assert Verify that alert and rear blockage is reset. */
   EXPECT_EQ(recw_instance.core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_instance.core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_FALSE(recw_instance.persistent.f_is_rear_blocked);
}

/**
 * Call the main RECW function with an active alert and active rear blockage when RECW is enabled. Verify that alert is reset and
 * rear blockage is unchanged, if no valid objects are present. \uts{CSCSA-44337} \sdd{SF-7852} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Core_Run__works_properly_for_Recw_enabled)
{
   /** \arrange Set RECW enabled flag to true and set alert and rear blockage from last cycle. */

   recw_instance.core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_instance.core_output.recw_id          = 4u;
   recw_instance.core_input.f_enable_recw     = FBK_TRUE;
   recw_instance.persistent.f_is_rear_blocked = FBK_TRUE;

   /** \action Call the main RECW function. */
   Recw_Core_Run(&recw_instance.core_output, &recw_instance.core_input, &recw_instance.calibration, &recw_instance.persistent);

   /** \assert Verify that alert is reset and rear blockage is unchanged. */
   EXPECT_EQ(recw_instance.core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_instance.core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_TRUE(recw_instance.persistent.f_is_rear_blocked);
}


/**
 * Test verifying recw reset when flag indicating that host speed is outside of allowed range is set to false.
 * \uts{CSCSA-204756} \sdd{SF-7852} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Core_Run__Recw_reset_f_host_speed_in_allowed_range_is_false)
{
   /** \arrange Set RECW enabled flag to true, speed in allowed range set to false and set output values to non default. */

   recw_instance.core_output.recw_id      = 4u;
   recw_instance.core_input.f_enable_recw = FBK_TRUE;

   recw_instance.calibration.k_recw_min_host_speed[RECW_NO_ALERT] = 0.0f;
   recw_instance.calibration.k_recw_max_host_speed[RECW_NO_ALERT] = 69.44f;

   data.vehicle_data.host_speed = recw_instance.calibration.k_recw_max_host_speed[RECW_NO_ALERT] + 5.0f;

   recw_instance.core_output.recw_alert_level             = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_instance.core_output.recw_id                      = 5u;
   recw_instance.core_output.recw_ttc                     = 1.2f;
   recw_instance.persistent.f_host_speed_in_allowed_range = FBK_FALSE;

   /** \action Call the main RECW function. */
   Recw_Core_Run(&recw_instance.core_output, &recw_instance.core_input, &recw_instance.calibration, &recw_instance.persistent);

   /** \assert Verify that output does contain default values. */
   EXPECT_EQ(recw_instance.core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_instance.core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(recw_instance.core_output.recw_ttc, RECW_MAX_TTC);
}

/**
 * Test that RECW algorithm function alerts on critical object close behind ego. Verify that alert level is set in core output.
 * \uts{CSCSA-44338} \sdd{SF-7840} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Algorithm__generates_alert_for_critical_obj)
{
   /** \arrange Set up cals, tracker output, persistent data and vehicle data such that critical object is present. */
   uint8_t obj_idx = 2;
   uint8_t obj_id  = obj_idx + 1;

   recw_instance.calibration.k_recw_f_apply_lane_filter                    = 0u;
   recw_instance.calibration.k_recw_f_apply_lane_filter                    = 0u;
   recw_instance.calibration.k_recw_f_enable_traffic_light_ghost_detection = 0u;

   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;
   object_data[obj_idx].id                    = obj_id;
   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].vcs_vel_rel.x = recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1] + 15.0f;
   object_data[obj_idx].width         = recw_instance.calibration.k_recw_max_object_width_warn_on;
   object_data[obj_idx].length        = 4.0f;
   object_data[obj_idx].vcs_accel.x   = 1.0f;
   object_data[obj_idx].vcs_pos.x     = -9.0f;
   object_data[obj_idx].speed         = 1.1f * recw_instance.calibration.k_recw_min_speed_not_stationary;
   object_data[obj_idx].stage_age     = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];

   recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter =
      recw_instance.calibration.k_recw_min_cycles_with_min_crash_prob + 1;
   recw_pers.object_data[obj_id].age               = recw_instance.calibration.k_recw_min_object_age + 1;
   recw_pers.object_data[obj_id].last_object_pos.x = object_data[obj_idx].vcs_pos.x * 1.01f;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->long_acc    = object_data[obj_idx].vcs_accel.x;

   /** \action Call the RECW algorithm function. */
   Recw_Algorithm(&recw_instance.core_output, &recw_pers, &recw_instance.core_input, &recw_instance.calibration, p_vehicle_data,
                  guardrail_data);

   /** \assert Verify that alert level 2 is set in core output. */
   EXPECT_EQ(recw_instance.core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_2);
}

/**
 * Test that RECW algorithm function resets counter for consecutive cycles with minimal crash probability, when an object is
 * relevant but not critical. Verify counter is reset to zero for the object in question. \uts{CSCSA-44339} \sdd{SF-7840}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Algorithm__resets_min_crash_prob_counter_if_object_relevant_but_not_critical)
{
   /** \arrange Set up cals, tracker output, persistent data and vehicle data such that relevant object is present. */
   uint8_t obj_idx = 2;
   uint8_t obj_id  = obj_idx + 1;

   recw_instance.calibration.k_recw_f_apply_lane_filter                    = 0u;
   recw_instance.calibration.k_recw_f_apply_lane_filter                    = 0u;
   recw_instance.calibration.k_recw_f_enable_traffic_light_ghost_detection = 0u;

   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;
   object_data[obj_idx].id                    = obj_id;
   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].vcs_vel_rel.x = recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1] + 15.0f;
   object_data[obj_idx].speed         = recw_instance.calibration.k_recw_min_speed_not_stationary + EPSILON;
   object_data[obj_idx].width         = recw_instance.calibration.k_recw_max_object_width_warn_on;
   object_data[obj_idx].length        = 4.0f;
   object_data[obj_idx].vcs_accel.x   = 1.0f;
   object_data[obj_idx].vcs_pos.x     = 0.0f;

   recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter =
      recw_instance.calibration.k_recw_min_cycles_with_min_crash_prob + 1;
   recw_pers.object_data[obj_id].age                                = recw_instance.calibration.k_recw_min_object_age + 1;
   recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter = 6u;
   recw_pers.object_data[obj_id].last_object_pos.x                  = 0.0f;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->long_acc    = object_data[obj_idx].vcs_accel.x;

   /** \action Call the RECW algorithm function. */
   Recw_Algorithm(&recw_instance.core_output, &recw_pers, &recw_instance.core_input, &recw_instance.calibration, p_vehicle_data,
                  guardrail_data);

   /** \assert Verify that counter for consecutive cycles with minimal crash probability is reset to zero. */
   EXPECT_EQ(recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter, FBK_ZERO_INT);
}

/**
 * Test that RECW algorithm function resets counter for consecutive cycles with minimal crash probability, when an object is valid
 * but not relevant. Verify counter is reset to zero for the object in question. \uts{CSCSA-44340} \sdd{SF-7840}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Algorithm__resets_min_crash_prob_counter_if_object_valid_but_not_relevant)
{
   /** \arrange Set up cals and tracker output such that valid (but not relevant) object is present. */
   uint8_t obj_idx = 2;
   uint8_t obj_id  = obj_idx + 1;

   object_data[obj_idx].status       = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection = FBK_FALSE;
   object_data[obj_idx].id           = obj_idx + 1;

   recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter = 6u;

   /** \action Call the RECW algorithm function. */
   Recw_Algorithm(&recw_instance.core_output, &recw_pers, &recw_instance.core_input, &recw_instance.calibration, p_vehicle_data,
                  guardrail_data);

   /** \assert Verify that counter for consecutive cycles with minimal crash probability is reset to zero. */
   EXPECT_EQ(recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter, FBK_ZERO_INT);
}

/**
 * Test that RECW algorithm function resets rear blockage if ego speed is above the defined threshold. Verify that rear blockage is
 * reset to false. \uts{CSCSA-44341} \sdd{SF-7840} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Algorithm__resets_blockage_if_ego_speed_above_threshold)
{
   /** \arrange set rear blockage and vehicle speed. */
   recw_pers.f_is_rear_blocked = FBK_TRUE;
   p_vehicle_data->host_speed  = recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold + 1.0f;

   /** \action Call the RECW algorithm function. */
   Recw_Algorithm(&recw_instance.core_output, &recw_pers, &recw_instance.core_input, &recw_instance.calibration, p_vehicle_data,
                  guardrail_data);

   /** \assert Verify that rear blockage is reset to false. */
   EXPECT_FALSE(recw_pers.f_is_rear_blocked);
}

/**
 * Test that RECW algorithm function resets object age for objects that have just been merged. Verify that object age is reset to
 * zero. \uts{CSCSA-44342} \sdd{SF-7840} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Algorithm__resets_obj_age_if_obj_is_valid_and_just_merged_flag_is_active)
{
   /** \arrange Set up tracker output and persistent data for an object that was just merged. */
   uint8_t obj_idx = 2;
   uint8_t obj_id  = 7;

   object_data[obj_idx].status          = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection    = FBK_FALSE;
   object_data[obj_idx].id              = obj_id;
   object_data[obj_idx].f_merge_occured = FBK_TRUE;
   recw_pers.object_data[obj_id].age    = 5u;

   /** \action Call the RECW algorithm function. */
   Recw_Algorithm(&recw_instance.core_output, &recw_pers, &recw_instance.core_input, &recw_instance.calibration, p_vehicle_data,
                  guardrail_data);

   /** \assert Verify that object age is reset to zero. */
   EXPECT_EQ(recw_pers.object_data[obj_id].age, FBK_ZERO_INT);
}

/**
 * Test that RECW algorithm function reacts correctly to blockage from a object behing the host.
 * \uts{CSCSA-44343} \sdd{SF-7840} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Algorithm__rear_blocked_by_object)
{
   /** \arrange Set up tracker output, cals and persistent data for an object blocking the rear. */
   uint8_t obj_idx = 2;

   recw_instance.calibration.k_recw_rear_blockage_speed_threshold   = 2.0f;
   recw_instance.calibration.k_recw_rear_blockage_qualifying_cycles = FBK_ZERO_UINT;

   object_data[obj_idx].status          = PA_OBJ_STATUS_COASTED;
   object_data[obj_idx].speed           = recw_instance.calibration.k_recw_rear_blockage_speed_threshold / 2.0f;
   object_data[obj_idx].vcs_heading     = 0.0f;
   object_data[obj_idx].obj_class       = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].vcs_vel.x       = 1.0f;
   object_data[obj_idx].vcs_pos.x       = -(p_vehicle_data->host_length + recw_instance.calibration.k_recw_rear_blockage_length);
   object_data[obj_idx].vcs_pos.y       = 0.3f * recw_instance.calibration.k_recw_rear_blockage_width;
   object_data[obj_idx].width           = 2.0f;
   object_data[obj_idx].length          = 0.5f * recw_instance.calibration.k_recw_rear_blockage_length;
   object_data[obj_idx].f_reflection    = FBK_FALSE;
   object_data[obj_idx].id              = obj_idx + 1;
   object_data[obj_idx].f_merge_occured = FBK_TRUE;
   recw_pers.object_data[object_data[obj_idx].id].age = 5u;

   /** \action Call the RECW algorithm function. */
   Recw_Algorithm(&recw_instance.core_output, &recw_pers, &recw_instance.core_input, &recw_instance.calibration, p_vehicle_data,
                  guardrail_data);

   /** \assert Verify that information related to objects blocking in the rear is set. */
   EXPECT_EQ(recw_pers.f_is_rear_blocked, FBK_TRUE);
   EXPECT_EQ(recw_pers.recw_rear_blockage_qualifying_counter, 1u);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, obj_idx);
}

/**
 * Test when object min probability crash counter is set bellow minimal required value.
 * \uts{CSCSA-204757} \sdd{SF-7840} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Algorithm__min_crash_prob_counter_less_than_k_recw_min_cycles)
{
   /** \arrange Recw potential critical object with low probability counter */
   uint8_t obj_idx = 2;
   uint8_t obj_id  = obj_idx + 1;

   recw_instance.calibration.k_recw_f_apply_lane_filter                    = 0u;
   recw_instance.calibration.k_recw_f_apply_lane_filter                    = 0u;
   recw_instance.calibration.k_recw_f_enable_traffic_light_ghost_detection = 0u;
   recw_instance.calibration.k_recw_min_cycles_with_min_crash_prob         = 5u;

   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;
   object_data[obj_idx].id                    = obj_id;
   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].vcs_vel_rel.x = recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1] + 15.0f;
   object_data[obj_idx].width         = recw_instance.calibration.k_recw_max_object_width_warn_on;
   object_data[obj_idx].length        = 4.0f;
   object_data[obj_idx].vcs_accel.x   = 1.0f;
   object_data[obj_idx].vcs_pos.x     = -9.0f;
   object_data[obj_idx].speed         = 1.1f * recw_instance.calibration.k_recw_min_speed_not_stationary;
   object_data[obj_idx].stage_age     = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];

   recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter =
      recw_instance.calibration.k_recw_min_cycles_with_min_crash_prob + 1;
   recw_pers.object_data[obj_id].age                                = recw_instance.calibration.k_recw_min_object_age + 1;
   recw_pers.object_data[obj_id].last_object_pos.x                  = object_data[obj_idx].vcs_pos.x * 1.01f;
   recw_pers.object_data[obj_id].consecutive_min_crash_prob_counter = 1u;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->long_acc    = object_data[obj_idx].vcs_accel.x;

   /** \action Call main Recw routine */
   Recw_Algorithm(&recw_instance.core_output, &recw_pers, &recw_instance.core_input, &recw_instance.calibration, p_vehicle_data,
                  guardrail_data);

   /** \assert No alert level returned */
   EXPECT_EQ(recw_instance.core_output.recw_alert_level, RECW_NO_ALERT);
}

/**
 * Test that an object that fulfills all conditions results in an active rear blockage. Verify that blockage is set correctly.
 * \uts{CSCSA-44344} \sdd{SF-7839} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Rear_Blocked_By_Object__sets_blockage_to_TRUE_if_all_conditions_are_met_for_car)
{
   /** \arrange Set up tracker output for an object that meets all conditions for blockage. */
   Recw_Object_T recw_obj{};
   boolean_T is_rear_blocked = FBK_FALSE;

   recw_instance.calibration.k_recw_rear_blockage_speed_threshold = 2.0f;

   recw_obj.tracker_data.vcs_heading = 0.0f;
   recw_obj.tracker_data.status      = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.speed       = recw_instance.calibration.k_recw_rear_blockage_speed_threshold / 2.0f;
   recw_obj.tracker_data.obj_class   = PA_OBJ_CLASS_CAR;
   recw_obj.tracker_data.vcs_vel.x   = 1.0f;
   recw_obj.tracker_data.vcs_pos.x   = -(p_vehicle_data->host_length + recw_instance.calibration.k_recw_rear_blockage_length);
   recw_obj.tracker_data.vcs_pos.y   = 0.3f * recw_instance.calibration.k_recw_rear_blockage_width;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.length      = 0.5f * recw_instance.calibration.k_recw_rear_blockage_length;

   /** \action Call function that determines if rear is blocked by object. */
   is_rear_blocked = Recw_Is_Rear_Blocked_By_Object(&recw_obj, &recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that rear is blocked by object. */
   EXPECT_TRUE(is_rear_blocked);
}

/**
 * Test that an object that fulfills all conditions results in an active rear blockage. Verify that blockage is set correctly.
 * \uts{CSCSA-112383} \sdd{SF-7839} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Rear_Blocked_By_Object__sets_blockage_to_TRUE_if_all_conditions_are_met_for_truck)
{
   /** \arrange Set up tracker output for an object that meets all conditions for blockage. */
   Recw_Object_T recw_obj{};
   boolean_T is_rear_blocked = FBK_FALSE;

   recw_instance.calibration.k_recw_rear_blockage_speed_threshold = 2.0f;

   recw_obj.tracker_data.vcs_heading = 0.0f;
   recw_obj.tracker_data.status      = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.speed       = recw_instance.calibration.k_recw_rear_blockage_speed_threshold / 2.0f;
   recw_obj.tracker_data.obj_class   = PA_OBJ_CLASS_TRUCK;
   recw_obj.tracker_data.vcs_vel.x   = 1.0f;
   recw_obj.tracker_data.vcs_pos.x   = -(p_vehicle_data->host_length + recw_instance.calibration.k_recw_rear_blockage_length);
   recw_obj.tracker_data.vcs_pos.y   = 0.3f * recw_instance.calibration.k_recw_rear_blockage_width;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.length      = 0.5f * recw_instance.calibration.k_recw_rear_blockage_length;

   /** \action Call function that determines if rear is blocked by object. */
   is_rear_blocked = Recw_Is_Rear_Blocked_By_Object(&recw_obj, &recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that rear is blocked by object. */
   EXPECT_TRUE(is_rear_blocked);
}

/**
 * Test that an object that fulfills the basic conditions does not set blockage if object class is not valid and persistent block
 * is false. Verify that blockage is set to false. \uts{CSCSA-112384} \sdd{SF-7839} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Rear_Blocked_By_Object__sets_blockage_to_FALSE_if_obj_class_invalid_and_persistent_rear_blocked_false)
{
   /** \arrange Set up tracker output for an object that meets all conditions for blockage except obj class and persistent flag
    * rear blocked. */
   Recw_Object_T recw_obj{};
   boolean_T is_rear_blocked = FBK_FALSE;

   recw_instance.calibration.k_recw_rear_blockage_speed_threshold = 2.0f;

   recw_obj.tracker_data.vcs_heading = 0.0f;
   recw_obj.tracker_data.status      = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.speed       = recw_instance.calibration.k_recw_rear_blockage_speed_threshold / 2.0f;
   recw_obj.tracker_data.obj_class   = PA_OBJ_CLASS_2WHEEL;
   recw_obj.tracker_data.vcs_vel.x   = 1.0f;
   recw_obj.tracker_data.vcs_pos.x   = -(p_vehicle_data->host_length + recw_instance.calibration.k_recw_rear_blockage_length);
   recw_obj.tracker_data.vcs_pos.y   = 0.3f * recw_instance.calibration.k_recw_rear_blockage_width;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.length      = 0.5f * recw_instance.calibration.k_recw_rear_blockage_length;

   recw_pers.f_is_rear_blocked = FBK_FALSE;

   /** \action Call function that determines if rear is blocked by object. */
   is_rear_blocked = Recw_Is_Rear_Blocked_By_Object(&recw_obj, &recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that rear is not blocked by object. */
   EXPECT_FALSE(is_rear_blocked);
}

/**
 * Test that an object that fulfills the basic conditions does not set blockage if object speed is too large. Verify that blockage
 * is set to false. \uts{CSCSA-44345} \sdd{SF-7839} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Rear_Blocked_By_Object__sets_blockage_to_FALSE_if_basic_conditions_are_met_and_obj_speed_is_too_large)
{
   /** \arrange Set up tracker output for an object that meets all conditions for blockage except for object speed */
   Recw_Object_T recw_obj{};
   boolean_T is_rear_blocked = FBK_TRUE;

   recw_instance.calibration.k_recw_rear_blockage_speed_threshold = 2.0f;

   recw_obj.tracker_data.vcs_heading = 0.0f;
   recw_obj.tracker_data.status      = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.speed       = recw_instance.calibration.k_recw_rear_blockage_speed_threshold * 2.0f;
   recw_obj.tracker_data.obj_class   = PA_OBJ_CLASS_2WHEEL;
   recw_obj.tracker_data.vcs_vel.x   = 1.0f;
   recw_obj.tracker_data.vcs_pos.x   = -(p_vehicle_data->host_length + recw_instance.calibration.k_recw_rear_blockage_length);
   recw_obj.tracker_data.vcs_pos.y   = 0.3f * recw_instance.calibration.k_recw_rear_blockage_width;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.length      = 0.5f * recw_instance.calibration.k_recw_rear_blockage_length;

   recw_pers.f_is_rear_blocked = FBK_TRUE;

   /** \action Call function that determines if rear is blocked by object. */
   is_rear_blocked = Recw_Is_Rear_Blocked_By_Object(&recw_obj, &recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that rear is not blocked by object. */
   EXPECT_FALSE(is_rear_blocked);
}

/**
 * Test that blockage is set to false for objects that are not located in the blockage zone.
 * \uts{CSCSA-44346} \sdd{SF-7839} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Rear_Blocked_By_Object__sets_blockage_to_FALSE_if_object_not_in_blockage_zone)
{
   /** \arrange Set up tracker output for an object that meets all conditions for blockage except that it is not located in the ego
    * lane. */
   Recw_Object_T recw_obj{};
   boolean_T is_rear_blocked = FBK_FALSE;

   recw_instance.calibration.k_recw_rear_blockage_speed_threshold = 2.0f;

   recw_obj.tracker_data.vcs_heading = 0.0f;
   recw_obj.tracker_data.status      = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.speed       = recw_instance.calibration.k_recw_rear_blockage_speed_threshold / 2.0f;
   recw_obj.tracker_data.obj_class   = PA_OBJ_CLASS_2WHEEL;
   recw_obj.tracker_data.vcs_vel.x   = 1.0f;
   recw_obj.tracker_data.vcs_pos.x   = -(p_vehicle_data->host_length + recw_instance.calibration.k_recw_rear_blockage_length);
   recw_obj.tracker_data.vcs_pos.y   = 1.1f * recw_instance.calibration.k_recw_rear_blockage_width;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.length      = 0.5f * recw_instance.calibration.k_recw_rear_blockage_length;

   recw_pers.f_is_rear_blocked = FBK_TRUE;

   /** \action Call function that determines if rear is blocked by object. */
   is_rear_blocked = Recw_Is_Rear_Blocked_By_Object(&recw_obj, &recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that rear is not blocked by object. */
   EXPECT_FALSE(is_rear_blocked);
}

/**
 * Test that heading filter functions uses unfiltered values if last position of object was invalid. Verify that unfiltered values
 * are used for the object in question. \uts{CSCSA-44347} \sdd{SF-7836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Filter_Object_Heading__uses_unfiltered_values_if_last_pos_was_invalid)
{
   /** \arrange Set up recw object with invalid last position. */
   Recw_Object_T recw_obj{};
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 1u;

   recw_obj.tracker_data.index                     = obj_idx;
   recw_obj.tracker_data.id                        = obj_id;
   recw_obj.tracker_data.vcs_heading               = 0.3f;
   recw_obj.tracker_data.vcs_pos.x                 = -15.0f;
   recw_pers.object_data[obj_id].last_object_pos.x = RECW_INVALID_LON_POS;

   /** \action Call filter function for heading. */
   Recw_Filter_Object_Heading(&recw_obj, &recw_pers, data.time_diff_to_last_cycle);

   /** \assert Verify that unfiltered values are used. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.filtered_heading, recw_obj.tracker_data.vcs_heading);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos.x, recw_obj.tracker_data.vcs_pos.x);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos.y, recw_obj.tracker_data.vcs_pos.y);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.x, recw_obj.attributes.filtered_diff_pos.x);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.y, recw_obj.attributes.filtered_diff_pos.y);
}

/**
 * Test that heading filter functions uses unfiltered values if cycle time is zero. This ensures the robustness of RECW, if the
 * tracker does not provide this information properly. Verify that unfiltered values are used for the object in question.
 * \uts{CSCSA-44348} \sdd{SF-7836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Filter_Object_Heading__uses_unfiltered_values_if_cycle_time_zero)
{
   /** \arrange Set up valid recw object and set cycle time to zero. */
   Recw_Object_T recw_obj{};
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 19u;

   recw_obj.tracker_data.index                     = obj_idx;
   recw_obj.tracker_data.id                        = obj_id;
   recw_obj.tracker_data.vcs_heading               = 0.3f;
   recw_obj.tracker_data.vcs_pos.x                 = -15.0f;
   recw_obj.tracker_data.vcs_vel_rel.x             = 0.3f;
   recw_obj.tracker_data.vcs_vel_rel.y             = 0.1f;
   recw_pers.object_data[obj_id].last_object_pos.x = -17.0f;
   data.time_diff_to_last_cycle                    = 0.0f;

   /** \action Call filter function for heading. */
   Recw_Filter_Object_Heading(&recw_obj, &recw_pers, data.time_diff_to_last_cycle);

   /** \assert Verify that unfiltered values are used. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.filtered_heading, recw_obj.tracker_data.vcs_heading);
   EXPECT_FLOAT_EQ(recw_obj.tracker_data.vcs_vel_rel.x, recw_obj.attributes.effective_rel_vel.x);
   EXPECT_FLOAT_EQ(recw_obj.tracker_data.vcs_vel_rel.y, recw_obj.attributes.effective_rel_vel.y);
}

/**
 * Test that heading filter functions uses unfiltered values if filter counter is below threshold. Verify that unfiltered values
 * are used for the object in question. \uts{CSCSA-44349} \sdd{SF-7836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Filter_Object_Heading__uses_unfiltered_values_if_filter_counter_is_below_threshold)
{
   /** \arrange Set up recw object and persistent data (especially filter counter below threshold). */
   Recw_Object_T recw_obj{};
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 10u;

   recw_obj.tracker_data.index       = obj_idx;
   recw_obj.tracker_data.id          = obj_id;
   recw_obj.tracker_data.vcs_heading = 0.3f;

   /* Diff between positions is x = 1.0f, y = 0.0f */
   recw_pers.object_data[obj_id].last_object_pos.x = -10.0f;
   recw_pers.object_data[obj_id].last_object_pos.y = 0.0f;
   recw_obj.tracker_data.vcs_pos.x                 = -9.0f;
   recw_obj.tracker_data.vcs_pos.y                 = 0.0f;

   recw_pers.object_data[obj_id].filter_counter = RECW_MIN_POS_DIFF_FILTER_CYCLES - 2;

   /* Use zero for the last object pos to simplify testing of filtering */
   recw_pers.object_data[obj_id].last_object_pos_filtered.x = 0.0f;
   recw_pers.object_data[obj_id].last_object_pos_filtered.y = 0.0f;

   /** \action Call filter function for heading. */
   Recw_Filter_Object_Heading(&recw_obj, &recw_pers, data.time_diff_to_last_cycle);

   /** \assert Verify that unfiltered values are used and that filter counter is incremented. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.filtered_heading, recw_obj.tracker_data.vcs_heading);
   EXPECT_EQ(recw_pers.object_data[obj_id].filter_counter, RECW_MIN_POS_DIFF_FILTER_CYCLES - 1);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.x, 1.0f / recw_pers.object_data[obj_id].filter_counter);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.y, 0.0f);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos.x, recw_obj.tracker_data.vcs_pos.x);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos.y, recw_obj.tracker_data.vcs_pos.y);
}

/**
 * Test that heading filter functions uses filtered values if filter counter is at threshold. Verify that filtered values are used
 * for the object in question. \uts{CSCSA-44350} \sdd{SF-7836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Filter_Object_Heading__uses_filtered_values_if_filter_counter_is_at_threshold)
{
   /** \arrange Set up recw object and persistent data (especially filter counter at threshold). */
   Recw_Object_T recw_obj{};
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 3u;

   recw_obj.tracker_data.index       = obj_idx;
   recw_obj.tracker_data.id          = obj_id;
   recw_obj.tracker_data.vcs_heading = 0.3f;

   /* Diff between positions is x = 1.0f, y = 0.0f */
   recw_pers.object_data[obj_id].last_object_pos.x = -10.0f;
   recw_pers.object_data[obj_id].last_object_pos.y = 0.0f;
   recw_obj.tracker_data.vcs_pos.x                 = -9.0f;
   recw_obj.tracker_data.vcs_pos.y                 = 0.0f;

   recw_pers.object_data[obj_id].filter_counter = RECW_MIN_POS_DIFF_FILTER_CYCLES;

   /* Use zero for the last object pos to simplify testing of filtering */
   recw_pers.object_data[obj_id].last_object_pos_filtered.x = 0.0f;
   recw_pers.object_data[obj_id].last_object_pos_filtered.y = 0.0f;

   /** \action Call filter function for heading. */
   Recw_Filter_Object_Heading(&recw_obj, &recw_pers, data.time_diff_to_last_cycle);

   /** \assert Verify that filtered values are used and that filter counter is constant. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.filtered_heading, 0.0f);
   EXPECT_EQ(recw_pers.object_data[obj_id].filter_counter, RECW_MIN_POS_DIFF_FILTER_CYCLES);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.x, 1.0f / recw_pers.object_data[obj_id].filter_counter);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.y, 0.0f);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos.x, recw_obj.tracker_data.vcs_pos.x);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos.y, recw_obj.tracker_data.vcs_pos.y);
}

/**
 * Test that will not use filtered values if cycle time is equal to zero uder condition that object filter counter is valid.
 * \uts{CSCSA-204758} \sdd{SF-7836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Filter_Object_Heading__cycle_time_forced_to_zero_value)
{
   /** \arrange Setup recw object and persistent data */
   Recw_Object_T recw_obj{};
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 3u;

   recw_obj.tracker_data.index       = obj_idx;
   recw_obj.tracker_data.id          = obj_id;
   recw_obj.tracker_data.vcs_heading = 0.3f;

   recw_pers.object_data[obj_id].last_object_pos.x = -10.0f;
   recw_pers.object_data[obj_id].last_object_pos.y = 0.0f;
   recw_obj.tracker_data.vcs_pos.x                 = -9.0f;
   recw_obj.tracker_data.vcs_pos.y                 = 0.0f;

   recw_pers.object_data[obj_id].filter_counter = RECW_MIN_POS_DIFF_FILTER_CYCLES;

   recw_pers.object_data[obj_id].last_object_pos_filtered.x = 0.0f;
   recw_pers.object_data[obj_id].last_object_pos_filtered.y = 0.0f;

   /** \action Call filter function for heading. With cycle time equal to zero */
   Recw_Filter_Object_Heading(&recw_obj, &recw_pers, 0.0f);

   /** \assert Verify that NOT filtered values are used and that filter counter is constant. */
   EXPECT_EQ(recw_obj.attributes.effective_rel_vel.x, 0.0f);
   EXPECT_EQ(recw_obj.attributes.effective_rel_vel.y, 0.0f);
   EXPECT_EQ(recw_obj.attributes.filtered_heading, recw_obj.tracker_data.vcs_heading);
}

/**
 * Test to ensure that filter will not use filtered heading, if fitered min diff position is not met
 * \uts{CSCSA-204759} \sdd{SF-7836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Filter_Object_Heading__test_usage_of_not_filtered_heading_if_pos_cond_are_not_met)
{
   /** \arrange Setup recw object and persistent data */
   Recw_Object_T recw_obj{};
   uint8_t obj_idx = 4u;
   uint8_t obj_id  = 3u;

   recw_obj.tracker_data.index       = obj_idx;
   recw_obj.tracker_data.id          = obj_id;
   recw_obj.tracker_data.vcs_heading = 0.3f;

   recw_pers.object_data[obj_id].last_object_pos.x = -10.0f;
   recw_pers.object_data[obj_id].last_object_pos.y = 0.0f;
   recw_obj.tracker_data.vcs_pos.x                 = -9.0f;
   recw_obj.tracker_data.vcs_pos.y                 = 0.0f;

   recw_pers.object_data[obj_id].filter_counter = RECW_MIN_POS_DIFF_FILTER_CYCLES;

   recw_pers.object_data[obj_id].last_object_pos_filtered.x = -5.0f;
   recw_pers.object_data[obj_id].last_object_pos_filtered.y = 0.0f;

   /** \action Call filter function for heading. */
   Recw_Filter_Object_Heading(&recw_obj, &recw_pers, data.time_diff_to_last_cycle);

   /** \assert Verify that NOT filtered values are used */
   EXPECT_EQ(recw_obj.attributes.filtered_heading, recw_obj.tracker_data.vcs_heading);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.x, recw_obj.attributes.filtered_diff_pos.x);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_id].last_object_pos_filtered.y, recw_obj.attributes.filtered_diff_pos.y);
}

/**
 * Test that a object which is not flagged as reflection and has MATURE tracker status is valid. Verify that the object is
 * classified as valid. \uts{CSCSA-44351} \sdd{SF-7838} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Object_State_Fulfilled__returns_TRUE_if_obj_is_valid_with_status_MATURE)
{
   /** \arrange Set up object with tracker status MATURE and reflection flag false. */
   uint8_t obj_idx = 4u;
   boolean_T result;

   object_data[obj_idx].status       = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection = FBK_FALSE;

   /** \action Call function that evaluates if object is valid. */
   result = Recw_Is_Object_State_Fulfilled(&object_data[obj_idx]);

   /** \assert Verify that object is valid. */
   EXPECT_TRUE(result);
}

/**
 * Test that a object which is not flagged as reflection and has COASTED tracker status is valid. Verify that the object is
 * classified as valid. \uts{CSCSA-44352} \sdd{SF-7838} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Object_State_Fulfilled__returns_TRUE_if_obj_is_valid_with_status_COASTED)
{
   /** \arrange Set up object with tracker status COASTED and reflection flag false. */
   uint8_t obj_idx = 4u;
   boolean_T result;

   object_data[obj_idx].status       = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection = FBK_FALSE;

   /** \action Call function that evaluates if object is valid. */
   result = Recw_Is_Object_State_Fulfilled(&object_data[obj_idx]);

   /** \assert Verify that object is valid. */
   EXPECT_TRUE(result);
}

/**
 * Test that a object which is not flagged as reflection and has INVALID tracker status is not valid. Verify that the object is
 * classified as not valid. \uts{CSCSA-44353} \sdd{SF-7838} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Object_State_Fulfilled__returns_FALSE_if_obj_is_invalid)
{
   /** \arrange Set up object with tracker status INVALID and reflection flag false. */
   uint8_t obj_idx = 4u;
   boolean_T result;

   object_data[obj_idx].status       = PA_OBJ_STATUS_INVALID;
   object_data[obj_idx].f_reflection = FBK_FALSE;

   /** \action Call function that evaluates if object is valid. */
   result = Recw_Is_Object_State_Fulfilled(&object_data[obj_idx]);

   /** \assert Verify that object is not valid. */
   EXPECT_FALSE(result);
}

/**
 * Test that a object which is flagged as reflection and has MATURE tracker status is not valid. Verify that the object is
 * classified as not valid. \uts{CSCSA-44354} \sdd{SF-7838} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Object_State_Fulfilled__returns_FALSE_if_obj_is_reflection)
{
   /** \arrange Set up object with tracker status MATURE and reflection flag true. */
   uint8_t obj_idx = 4u;
   boolean_T result;

   object_data[obj_idx].status       = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_reflection = FBK_TRUE;

   /** \action Call function that evaluates if object is valid. */
   result = Recw_Is_Object_State_Fulfilled(&object_data[obj_idx]);

   /** \assert Verify that object is not valid. */
   EXPECT_FALSE(result);
}


/**
 * Test calculation of TTC for the case that object and ego have the same acceleration and relative velocity is positive. Verify
 * that calculated TTC is equal distance to ego rear bumper divided by relative velocity of object. \uts{CSCSA-44355} \sdd{SF-7833}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc__sets_ttc_properly_if_accelerations_are_identical_and_vel_rel_is_positive)
{
   /** \arrange Set up object and ego with same acceleration and positive relative velocity. */
   Recw_Object_T recw_obj{};

   /* settings add up to lon distance to rear bumper = -3.0f */
   recw_obj.tracker_data.vcs_pos.x     = -10.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 2.0f;
   recw_obj.tracker_data.vcs_accel.x   = 1.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that calculated TTC is equal distance to ego rear bumper divided by relative velocity of object. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc,
                   3.0f / recw_obj.tracker_data.vcs_vel_rel.x - recw_instance.calibration.k_recw_average_sensor_latency);
}

/**
 * Test calculation of TTC for the case that object and ego have the same acceleration and relative velocity is zero. Verify that
 * calculated TTC is at maximum value, if object is not very close to ego. \uts{CSCSA-44356} \sdd{SF-7833}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc__sets_ttc_properly_if_accelerations_are_identical_and_vel_rel_is_zero_and_obj_not_close_to_ego)
{
   /** \arrange Set up object (not very close to ego) and ego with same acceleration and relative velocity of zero. */
   Recw_Object_T recw_obj{};

   /* settings add up to lon distance to rear bumper = -3.0f */
   recw_obj.tracker_data.vcs_pos.x     = -10.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 0.0f;
   recw_obj.tracker_data.vcs_accel.x   = 1.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that calculated TTC is at maximum value. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc, RECW_MAX_TTC);
}

/**
 * Test calculation of TTC for the case that object and ego have the same acceleration and relative velocity is zero. Verify that
 * calculated TTC is at maximum value, if object is not very close to ego and distance to bumper is positive. \uts{CSCSA-112385}
 * \sdd{SF-7833} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test,
       Recw_Calculate_Ttc__sets_ttc_properly_if_accelerations_are_identical_and_vel_rel_is_zero_and_obj_not_close_to_ego_ahead)
{
   /** \arrange Set up object (not very close to ego) and ego with same acceleration and relative velocity of zero. */
   Recw_Object_T recw_obj{};

   /* settings add up to lon distance to rear bumper = 1.0f */
   recw_obj.tracker_data.vcs_pos.x     = -6.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 0.0f;
   recw_obj.tracker_data.vcs_accel.x   = 1.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that calculated TTC is at maximum value. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc, RECW_MAX_TTC);
}

/**
 * Test calculation of TTC for the case that object and ego have the same acceleration and relative velocity is zero. Verify that
 * calculated TTC is zero, if object is very close to ego. \uts{CSCSA-44357} \sdd{SF-7833} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc__sets_ttc_properly_if_accelerations_are_identical_and_vel_rel_is_zero_and_obj_is_close_to_ego)
{
   /** \arrange Set up object (at ego rear bumper) and ego with same acceleration and relative velocity of zero. */
   Recw_Object_T recw_obj{};

   /* settings add up to lon distance to rear bumper = 0.0f */
   recw_obj.tracker_data.vcs_pos.x     = -7.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 0.0f;
   recw_obj.tracker_data.vcs_accel.x   = 1.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that calculated TTC is zero. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc, FBK_ZERO_F);
}

/**
 * Test calculation of TTC for the case that object acceleration is smaller, but relative velocity is positive. Verify that valid
 * TTC is calculated if radicant negative. \uts{CSCSA-44358} \sdd{SF-7833} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc__sets_ttc_properly_if_acceleration_of_ego_is_larger_and_rel_vel_is_positive_and_radicand_negative)
{
   /** \arrange Set up object with acceleration smaller than acceleration of the ego, but positive relative velocity. */
   Recw_Object_T recw_obj{};

   /* settings add up to lon distance to rear bumper = -3.0f */
   recw_obj.tracker_data.vcs_pos.x     = -10.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 1.0f;
   recw_obj.tracker_data.vcs_accel.x   = 2.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x * 2.0f;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that valid TTC is calculated. */
   EXPECT_GT(recw_obj.attributes.ttc, FBK_ZERO_F);
   EXPECT_LT(recw_obj.attributes.ttc, RECW_MAX_TTC);
}

/**
 * Test calculation of TTC for the case that object acceleration is smaller, but relative velocity is positive. Verify that valid
 * TTC is calculated if radicand positive. \uts{CSCSA-44359} \sdd{SF-7833} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc__sets_ttc_properly_if_acceleration_of_ego_is_larger_and_rel_vel_is_positive_and_radicand_positive)
{
   /** \arrange Set up object with acceleration smaller than acceleration of the ego, but positive relative velocity. */
   Recw_Object_T recw_obj{};

   /* settings add up to lon distance to rear bumper = -3.0f */
   recw_obj.tracker_data.vcs_pos.x     = -10.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 3.0f;
   recw_obj.tracker_data.vcs_accel.x   = 2.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x * 1.1f;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that valid TTC is calculated. */
   EXPECT_GT(recw_obj.attributes.ttc, FBK_ZERO_F);
   EXPECT_LT(recw_obj.attributes.ttc, RECW_MAX_TTC);
}

/**
 * Test calculation of TTC for the case that object acceleration is larger and relative velocity is zero. Verify that valid TTC is
 * calculated. \uts{CSCSA-44360} \sdd{SF-7833} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc__sets_ttc_properly_if_acceleration_of_obj_is_larger_and_rel_vel_is_zero)
{
   /** \arrange Set up object with acceleration larger than acceleration of the ego and relative velocity of zero. */
   Recw_Object_T recw_obj{};

   /* settings add up to lon distance to rear bumper = -3.0f */
   recw_obj.tracker_data.vcs_pos.x     = -10.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 0.0f;
   recw_obj.tracker_data.vcs_accel.x   = 2.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x / 2.0f;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that valid TTC is calculated. */
   EXPECT_GT(recw_obj.attributes.ttc, FBK_ZERO_F);
   EXPECT_LT(recw_obj.attributes.ttc, RECW_MAX_TTC);
}

/**
 * Test calculation of TTC for the case that object lon acceleration is larger than min accel threshold, and relative velocity is
 * close to 0. \uts{CSCSA-204760} \sdd{SF-7833} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc__test_obj_rel_accel_low_rel_velocity_bellow_zero)
{
   /** \arrange Set up object with acceleration smaller than acceleration of the ego, very small relative velocity. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.vcs_pos.x     = -7.0f;
   recw_obj.tracker_data.length        = 4.0f;
   p_vehicle_data->host_length         = 5.0f;
   recw_obj.tracker_data.vcs_vel_rel.x = 0.1f;
   recw_obj.tracker_data.vcs_accel.x   = 3.0f;
   p_vehicle_data->long_acc            = recw_obj.tracker_data.vcs_accel.x * 1.1f;

   /** \action Call function that calculates TTC. */
   Recw_Calculate_Ttc(&recw_obj, p_vehicle_data, &recw_instance.calibration);

   /** \assert Verify that valid TTC is calculated. */
   EXPECT_GT(recw_obj.attributes.ttc, FBK_ZERO_F);
   EXPECT_LT(recw_obj.attributes.ttc, RECW_MAX_TTC);
}

/**
 * Verify that the TTC thresholds is calculated correctly when the objects relative velocity is below the minimal relative
 * velocity. \uts{CSCSA-44361} \sdd{SF-7834} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc_Thresholds__works_properly_if_rel_velocity_below_min_rel_velocity)
{
   /** \arrange Set up calibration values and relative velocity values such that relative velocity is below minimal relative
    * velocity. */
   Recw_Object_T recw_obj{};

   recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1]                       = 2.0f;
   recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_2]                       = 3.0f;
   recw_instance.calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] = 4.0f;
   recw_instance.calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] = 5.0f;
   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1]                      = 2.0f;
   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2]                      = 1.0f;

   recw_obj.tracker_data.vcs_vel_rel.x = 1.0f;

   /** \action Call function that calculates TTC thresholds with prepared object. */
   Recw_Calculate_Ttc_Thresholds(&recw_obj, &recw_pers, &recw_instance.calibration);

   /** \assert Verify that the calculated TTC thresholds are zero. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1], FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2], FBK_ZERO_F);
}

/**
 * Verify that the TTC thresholds is calculated correctly when the objects relative velocity is above the minimal relative velocity
 * for the maximal TTC. \uts{CSCSA-44362} \sdd{SF-7834} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc_Thresholds__works_properly_if_rel_velocity_above_min_rel_velocity_for_max_ttc)
{
   /** \arrange Set up calibration values and relative velocity values such that relative velocity is above the minimal relative
    * velocity for the maximal TTC. */
   Recw_Object_T recw_obj{};

   recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1]                       = 2.0f;
   recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_2]                       = 3.0f;
   recw_instance.calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] = 4.0f;
   recw_instance.calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] = 5.0f;
   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1]                      = 2.0f;
   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2]                      = 1.0f;

   recw_obj.tracker_data.vcs_vel_rel.x = 6.0f;

   /** \action Call function that calculates TTC thresholds with prepared object. */
   Recw_Calculate_Ttc_Thresholds(&recw_obj, &recw_pers, &recw_instance.calibration);

   /** \assert Verify that the calculated TTC thresholds are equal to the maximal TTCs for both alert levels. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1],
                   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1]);
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2],
                   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2]);
}

/**
 * Verify that the TTC thresholds is calculated correctly when the objects relative velocity is above the minimal relative velocity
 * and below the minimal relative velocity for the maximal TTC. \uts{CSCSA-44363} \sdd{SF-7834} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc_Thresholds__works_properly_if_rel_velocity_above_min_rel_vel_and_below_min_rel_vel_for_max_ttc)
{
   /** \arrange Set up calibration values and relative velocity values such that relative velocity is above the minimal relative
    * velocity and below the minimal relative velocity for the maximal TTC. */
   Recw_Object_T recw_obj{};

   recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1]                       = 2.0f;
   recw_instance.calibration.k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_2]                       = 3.0f;
   recw_instance.calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] = 4.0f;
   recw_instance.calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] = 5.0f;
   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1]                      = 2.0f;
   recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2]                      = 1.0f;

   recw_obj.tracker_data.vcs_vel_rel.x = 3.5f;

   /** \action Call function that calculates TTC thresholds with prepared object. */
   Recw_Calculate_Ttc_Thresholds(&recw_obj, &recw_pers, &recw_instance.calibration);

   /** \assert Verify that the calculated TTC thresholds are greater zero and less than the maximal TTCs for both alert levels. */
   EXPECT_GT(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1], FBK_ZERO_F);
   EXPECT_GT(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2], FBK_ZERO_F);
   EXPECT_LT(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1],
             recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_1]);
   EXPECT_LT(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2],
             recw_instance.calibration.k_recw_max_ttc_threshold[RECW_INDEX_ALERT_LEVEL_2]);
}

/**
 * Test for TTC thresholds calculation for recw object, when obj relative velocity is above max ttc treshold and bellow max
 * relative velocity. \uts{CSCSA-204761} \sdd{SF-7834} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Ttc_Thresholds__rel_vel_above_rel_vel_max_ttc_thres)
{
   /** \arrange Setup object information and calibration values */
   Recw_Object_T recw_obj;

   recw_obj.tracker_data.vcs_vel_rel.x = 9.33f;
   recw_obj.tracker_data.id            = 1;

   recw_instance.calibration.k_recw_min_rel_velocity[0] = 2.77f;
   recw_instance.calibration.k_recw_max_rel_velocity[1] = 4.5f;

   recw_instance.calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[0] = 8.33f;

   /** \action Call Recw ttc thresholds */
   Recw_Calculate_Ttc_Thresholds(&recw_obj, &recw_pers, &recw_instance.calibration);

   /** \assert Calculated ttc for level 1 of alert is zero */
   EXPECT_EQ(recw_obj.attributes.ttc_threshold[0], 0.0f);
}

/**
 * Test that no overlap is calculated for object not fully behind the host.
 * \uts{CSCSA-101048} \sdd{CSCSA-100398} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Overlap__target_not_behind_rear_bumper)
{
   /** \arrange Set up tracker data so that the object is not fully behind host */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.vcs_pos.x = -5.5f;
   recw_obj.tracker_data.length    = 4.0f;

   p_vehicle_data->host_length = 4.0f;

   /** \action Call function that calculates overlap with prepared object. */
   Recw_Calculate_Overlap(&recw_obj, p_vehicle_data);

   /** \assert Verify that the calculated overlap is zero. */
   EXPECT_EQ(recw_obj.attributes.overlap, FBK_ZERO_F);
}

/**
 * Test that no overlap is calculated for object which is to the side of host.
 * \uts{CSCSA-101049} \sdd{CSCSA-100398} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Overlap__target_not_overlaping)
{
   /** \arrange Set up tracker data so that the object is laterally distanced to host */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.vcs_pos.x   = -6.5f;
   recw_obj.tracker_data.vcs_pos.y   = 3.0f;
   recw_obj.tracker_data.length      = 4.0f;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.vcs_heading = 0.0f;

   p_vehicle_data->host_length = 4.0f;
   p_vehicle_data->host_width  = 2.0f;

   /** \action Call function that calculates overlap with prepared object. */
   Recw_Calculate_Overlap(&recw_obj, p_vehicle_data);

   /** \assert Verify that the calculated overlap is zero. */
   EXPECT_EQ(recw_obj.attributes.overlap, FBK_ZERO_F);
}

/**
 * Test that correct overlap is calculated for object which is smaller than host.
 * \uts{CSCSA-101050} \sdd{CSCSA-100398} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Overlap__target_smaller_than_host)
{
   /** \arrange Set up tracker data */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.vcs_pos.x   = -10.0f;
   recw_obj.tracker_data.vcs_pos.y   = 0.0f;
   recw_obj.tracker_data.length      = 4.0f;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.vcs_heading = 0.0f;

   p_vehicle_data->host_length = 4.0f;
   p_vehicle_data->host_width  = 2.5f;

   /** \action Call function that calculates overlap with prepared object. */
   Recw_Calculate_Overlap(&recw_obj, p_vehicle_data);

   /** \assert Verify that the calculated overlap is 1. */
   EXPECT_EQ(recw_obj.attributes.overlap, FBK_ONE_F);
}

/**
 * Test that correct overlap is calculated for object which is larger than host.
 * \uts{CSCSA-101051} \sdd{CSCSA-100398} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Overlap__target_larger_than_host)
{
   /** \arrange Set up tracker data */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.vcs_pos.x   = -10.0f;
   recw_obj.tracker_data.vcs_pos.y   = 0.0f;
   recw_obj.tracker_data.length      = 4.0f;
   recw_obj.tracker_data.width       = 2.5f;
   recw_obj.tracker_data.vcs_heading = 0.0f;

   p_vehicle_data->host_length = 4.0f;
   p_vehicle_data->host_width  = 2.0f;

   /** \action Call function that calculates overlap with prepared object. */
   Recw_Calculate_Overlap(&recw_obj, p_vehicle_data);

   /** \assert Verify that the calculated overlap is 1. */
   EXPECT_EQ(recw_obj.attributes.overlap, FBK_ONE_F);
}

/**
 * Test that correct overlap is calculated for object which is half overlapping.
 * \uts{CSCSA-101052} \sdd{CSCSA-100398} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Calculate_Overlap__target_half_overlapping)
{
   /** \arrange Set up tracker data */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.vcs_pos.x   = -10.0f;
   recw_obj.tracker_data.vcs_pos.y   = 1.0f;
   recw_obj.tracker_data.length      = 4.0f;
   recw_obj.tracker_data.width       = 2.0f;
   recw_obj.tracker_data.vcs_heading = 0.0f;

   p_vehicle_data->host_length = 4.0f;
   p_vehicle_data->host_width  = 2.0f;

   /** \action Call function that calculates overlap with prepared object. */
   Recw_Calculate_Overlap(&recw_obj, p_vehicle_data);

   /** \assert Verify that the calculated overlap is 0.5f. */
   EXPECT_EQ(recw_obj.attributes.overlap, 0.5f);
}

/**
 * Test that new object is chosen as most critical object if no critical object was present so far. Verify that most critical
 * object is changed to new object. \uts{CSCSA-44364} \sdd{SF-7844} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Most_Critical_Object__sets_new_obj_most_critical_obj_if_no_critical_obj_was_present_before)
{
   /** \arrange Set up most critical and new object, such that most critical is invalid. */
   Recw_Object_T recw_obj{};

   recw_instance.core_output.recw_id          = PA_INVALID_OBJ_ID;
   recw_instance.core_output.recw_alert_level = RECW_NO_ALERT;
   recw_obj.tracker_data.id                   = 6u;
   recw_obj.attributes.alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_obj.attributes.crash_prob_combined    = 20.0f;

   /** \action Call function that determines most critical object. */
   Recw_Set_Most_Critical_Object(&recw_instance.core_output, &recw_obj);

   /** \assert Verify that most critical object is changed to new object. */
   EXPECT_EQ(recw_instance.core_output.recw_id, recw_obj.tracker_data.id);
}

/**
 * Test that new object is not chosen as most critical object if it doesn't trigger any alert. Verify that most critical object is
 * unchanged. \uts{CSCSA-112386} \sdd{SF-7844} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Most_Critical_Object__sets_no_new_most_critical_obj_if_not_critical_obj_is_given)
{
   /** \arrange Set up most critical and new object, such that it is not critical. */
   Recw_Object_T recw_obj{};
   uint8_t prev_id = 5u;

   recw_instance.core_output.recw_id          = prev_id;
   recw_instance.core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_obj.tracker_data.id                   = 6u;
   recw_obj.attributes.alert_level            = RECW_NO_ALERT;

   /** \action Call function that determines most critical object. */
   Recw_Set_Most_Critical_Object(&recw_instance.core_output, &recw_obj);

   /** \assert Verify that most critical object is not changed. */
   EXPECT_EQ(recw_instance.core_output.recw_id, prev_id);
}

/**
 * Test that new object is chosen as most critical object if it has a higher crash probability. Verify that most critical object is
 * changed to new object. \uts{CSCSA-44365} \sdd{SF-7844} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Most_Critical_Object__sets_new_obj_most_critical_obj_if_crash_prob_is_higher)
{
   /** \arrange Set up most critical and new object, such that new object has higher crash probability. */
   Recw_Object_T recw_obj{};
   uint8_t prev_id = 5u;

   recw_instance.core_output.recw_id          = prev_id;
   recw_instance.core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_obj.tracker_data.id                   = 6u;
   recw_obj.attributes.alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;

   recw_instance.core_output.recw_crash_prob_combined = 40.0f;
   recw_obj.attributes.crash_prob_combined            = 50.0f;

   /** \action Call function that determines most critical object. */
   Recw_Set_Most_Critical_Object(&recw_instance.core_output, &recw_obj);

   /** \assert Verify that most critical object is changed to new object. */
   EXPECT_EQ(recw_instance.core_output.recw_id, recw_obj.tracker_data.id);
   EXPECT_FLOAT_EQ(recw_instance.core_output.recw_crash_prob_combined, recw_obj.attributes.crash_prob_combined);
}

/**
 * Test that most critical object is unchanged if new object has lower crash probability. Verify that most critical object is
 * unchanged. \uts{CSCSA-44366} \sdd{SF-7844} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Most_Critical_Object__sets_no_new_most_critical_obj_if_crash_prob_of_new_obj_is_lower)
{
   /** \arrange Set up most critical and new object, such that new object has lower crash probability. */
   Recw_Object_T recw_obj{};
   uint8_t prev_id           = 5u;
   float32_T prev_crash_prob = 50.0f;

   recw_instance.core_output.recw_id          = prev_id;
   recw_instance.core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_obj.tracker_data.id                   = 6u;
   recw_obj.attributes.alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;

   recw_instance.core_output.recw_crash_prob_combined = prev_crash_prob;
   recw_obj.attributes.crash_prob_combined            = 40.0f;

   /** \action Call function that determines most critical object. */
   Recw_Set_Most_Critical_Object(&recw_instance.core_output, &recw_obj);

   /** \assert Verify that most critical object is unchanged. */
   EXPECT_EQ(recw_instance.core_output.recw_id, prev_id);
   EXPECT_FLOAT_EQ(recw_instance.core_output.recw_crash_prob_combined, prev_crash_prob);
}

/**
 * Test that most critical object is unchanged if new object has lower alert level. Verify that most critical object is unchanged.
 * \uts{CSCSA-112387} \sdd{SF-7844} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Most_Critical_Object__sets_no_new_most_critical_obj_if_alert_level_of_new_obj_is_lower)
{
   /** \arrange Set up most critical and new object, such that new object has lower alert level. */
   Recw_Object_T recw_obj{};
   uint8_t prev_id           = 5u;
   float32_T prev_crash_prob = 50.0f;

   recw_instance.core_output.recw_id          = prev_id;
   recw_instance.core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_obj.tracker_data.id                   = 6u;
   recw_obj.attributes.alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;

   recw_instance.core_output.recw_crash_prob_combined = prev_crash_prob;
   recw_obj.attributes.crash_prob_combined            = 51.0f;

   /** \action Call function that determines most critical object. */
   Recw_Set_Most_Critical_Object(&recw_instance.core_output, &recw_obj);

   /** \assert Verify that most critical object is unchanged. */
   EXPECT_EQ(recw_instance.core_output.recw_id, prev_id);
   EXPECT_FLOAT_EQ(recw_instance.core_output.recw_crash_prob_combined, prev_crash_prob);
}

/**
 * Test that reset of object persistent data works as expected. Verify that all values are reset.
 * \uts{CSCSA-44367} \sdd{SF-7842} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Reset_Object_Persistent__works_properly)
{
   /** \arrange Fill persistent object data with non-standard values. */
   uint8_t obj_index = 4u;

   recw_pers.object_data[obj_index].object_within_lane_counter = 3u;
   recw_pers.object_data[obj_index].filter_counter             = 5u;
   recw_pers.object_data[obj_index].last_object_pos.x          = 5.0f;
   recw_pers.object_data[obj_index].last_object_pos.y          = 4.0f;

   /** \action Call function to reset object persistent data. */
   Recw_Reset_Object_Persistent(&recw_pers, obj_index);

   /** \assert Verify that all values are reset. */
   EXPECT_EQ(recw_pers.object_data[obj_index].object_within_lane_counter, FBK_ZERO_INT);
   EXPECT_EQ(recw_pers.object_data[obj_index].filter_counter, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_index].last_object_pos.x, RECW_INVALID_LON_POS);
   EXPECT_FLOAT_EQ(recw_pers.object_data[obj_index].last_object_pos.y, FBK_ZERO_F);
}

/**
 * Test that reset of RECW object data works as expected. Verify that all values are reset.
 * \uts{CSCSA-44368} \sdd{SF-7843} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Reset_Object__works_properly)
{
   /** \arrange Fill RECW object data with non-standard values. */
   Recw_Object_T recw_obj{};

   recw_obj.attributes.ttc                                     = 2.0f;
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] = 3.0f;
   recw_obj.attributes.crash_prob_braking                      = 80.0f;
   recw_obj.attributes.needed_steering_acceleration            = 9.0;
   recw_obj.tracker_data.age                                   = 24u;
   recw_obj.tracker_data.class_prob_pedestrian                 = 0.87f;

   /** \action Call function to reset RECW object data. */
   Recw_Reset_Object(&recw_obj);

   /** \assert Verify that all values are reset. */
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc, RECW_MAX_TTC);
   EXPECT_FLOAT_EQ(recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2], FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_obj.attributes.crash_prob_braking, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_obj.attributes.needed_steering_acceleration, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_obj.tracker_data.class_prob_pedestrian, FBK_ZERO_F);
   EXPECT_EQ(recw_obj.tracker_data.age, FBK_ZERO_UINT);
}

/**
 * Test that reset of persistent data works as expected. Verify that all values are reset.
 * \uts{CSCSA-44369} \sdd{SF-7841} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Reset_Persistent__works_properly)
{
   /** \arrange Fill persistent data with non-standard values. */
   recw_pers.f_is_rear_blocked          = FBK_TRUE;
   recw_pers.recw_alert_holding_counter = 5u;
   recw_pers.recw_id_prev_cycle         = 4u;
   recw_pers.object_data[4u].age        = 5u;
   recw_pers.recw_ttc_value_hold        = 6.9f;

   /** \action Call function to reset persistent data. */
   Recw_Reset_Persistent(&recw_pers);

   /** \assert Verify that all values are reset. */
   EXPECT_FALSE(recw_pers.f_is_rear_blocked);
   EXPECT_EQ(recw_pers.recw_alert_holding_counter, FBK_ZERO_INT);
   EXPECT_EQ(recw_pers.recw_id_prev_cycle, FBK_ZERO_INT);
   EXPECT_EQ(recw_pers.object_data[4u].age, FBK_ZERO_INT);
   EXPECT_EQ(recw_pers.recw_ttc_value_hold, FBK_ZERO_F);
}

/**
 * Test that reset of core output works as expected. Verify that all values are reset.
 * \uts{CSCSA-44370} \sdd{SF-7850} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Reset_Core_Output__works_properly)
{
   /** \arrange Fill core output with non-standard values. */
   recw_instance.core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_instance.core_output.recw_id          = 4u;
   recw_instance.core_output.recw_ttc         = 2.0f;

   /** \action Call function to reset core output. */
   Recw_Reset_Core_Output(&recw_instance.core_output);

   /** \assert Verify that all values are reset. */
   EXPECT_EQ(recw_instance.core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_instance.core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(recw_instance.core_output.recw_ttc, RECW_MAX_TTC);
}


/**
 * Test that alert level 2 is suppressed due to pedestrian object class.
 * \uts{CSCSA-308808} \sdd{CSCSA-308781} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Alert_Level_Suppressed__suppress_alert_lvl_2_for_pedestrian)
{
   /** \arrange Fill core output with non-standard values. */
   Recw_Object_T recw_obj{};
   recw_obj.tracker_data.obj_class                                        = PA_OBJ_CLASS_PEDESTRIAN;
   uint8_t alert_level                                                    = (uint8_t) RECW_INDEX_ALERT_LEVEL_2;
   recw_instance.calibration.k_recw_f_suppress_alert_lvl_2_for_pedestrian = FBK_TRUE;

   /** \action Call function to suppress alert level. */
   boolean_T res = Recw_Is_Alert_Level_Suppressed(&recw_obj, &recw_instance.calibration, alert_level);

   /** \assert Verify that alert is suppressed. */
   EXPECT_TRUE(res);
}


/**
 * Test that alert level 2 is not suppressed for pedestrian object class due to calibration disabled.
 * \uts{CSCSA-308809} \sdd{CSCSA-308781} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Alert_Level_Suppressed__cal_for_supp_pedestrian_disabled)
{
   /** \arrange Fill core output with non-standard values. */
   Recw_Object_T recw_obj{};
   recw_obj.tracker_data.obj_class                                        = PA_OBJ_CLASS_PEDESTRIAN;
   uint8_t alert_level                                                    = (uint8_t) RECW_INDEX_ALERT_LEVEL_2;
   recw_instance.calibration.k_recw_f_suppress_alert_lvl_2_for_pedestrian = FBK_FALSE;

   /** \action Call function to suppress the alert level. */
   boolean_T res = Recw_Is_Alert_Level_Suppressed(&recw_obj, &recw_instance.calibration, alert_level);

   /** \assert Verify that all values are reset. */
   EXPECT_FALSE(res);
}

/**
 * Test that alert level 1 is not suppressed for pedestrian object class.
 * \uts{CSCSA-308810} \sdd{CSCSA-308781} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Alert_Level_Suppressed__no_suppress_alert_lvl_1_for_pedestrian)
{
   /** \arrange Fill core output with non-standard values. */
   Recw_Object_T recw_obj{};
   recw_obj.tracker_data.obj_class                                        = PA_OBJ_CLASS_PEDESTRIAN;
   uint8_t alert_level                                                    = (uint8_t) RECW_INDEX_ALERT_LEVEL_1;
   recw_instance.calibration.k_recw_f_suppress_alert_lvl_2_for_pedestrian = FBK_TRUE;

   /** \action Call function to suppress the alert level. */
   boolean_T res = Recw_Is_Alert_Level_Suppressed(&recw_obj, &recw_instance.calibration, alert_level);

   /** \assert Verify that all values are reset. */
   EXPECT_FALSE(res);
}

/**
 * Test that alert level 2 is not suppressed due to non pedestrian object class.
 * \uts{CSCSA-308811} \sdd{CSCSA-308781} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Alert_Level_Suppressed__no_suppress_alert_lvl_2_for_non_pedestrian)
{
   /** \arrange Fill core output with non-standard values. */
   Recw_Object_T recw_obj{};
   recw_obj.tracker_data.obj_class                                        = PA_OBJ_CLASS_2WHEEL;
   uint8_t alert_level                                                    = (uint8_t) RECW_INDEX_ALERT_LEVEL_2;
   recw_instance.calibration.k_recw_f_suppress_alert_lvl_2_for_pedestrian = FBK_TRUE;

   /** \action Call function to reset core output. */
   boolean_T res = Recw_Is_Alert_Level_Suppressed(&recw_obj, &recw_instance.calibration, alert_level);

   /** \assert Verify that all values are reset. */
   EXPECT_FALSE(res);
}

/**
 * Test that most critical object does not issue an alert if rear is blocked and rear blockage is used. Verify that no alert is set
 * by Recw_Set_Object_Alert_Level function. \uts{CSCSA-44372} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_no_alert_if_rear_is_blocked_and_blockage_is_used)
{
   /** \arrange Set up most critical object and calibration values such that rear is blocked and rear blockage is used. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] + 2.0f;
   recw_obj.attributes.ttc     = recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_pers.f_is_rear_blocked = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_NO_ALERT);
}

/**
 * Test that most critical object does not issue an alert if rear is blocked and rear blockage is used. Verify that no alert is set
 * by Recw_Set_Object_Alert_Level function. \uts{CSCSA-44373} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_no_alert_if_object_is_coasted_and_coasted_object_alerts_are_disabled)
{
   /** \arrange Set up most critical object and cal. values such that object status is coasted and coasted alerts are disabled. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_COASTED;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] + 2.0f;
   recw_obj.attributes.ttc     = recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_pers.f_is_rear_blocked = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_FALSE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_FALSE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_NO_ALERT);
}

/**
 * Test that most critical object does not issue an alert if stage age is too low. Verify that no alert is set by
 * Recw_Set_Object_Alert_Level function. \uts{CSCSA-44374} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_no_alert_if_stage_age_is_too_low)
{
   /** \arrange Set up most critical object and calibration values such that stage age is too low for alert. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] = 4u;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] - 1u;
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] + 2.0f;
   recw_obj.attributes.ttc     = recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_pers.f_is_rear_blocked = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_NO_ALERT);
}

/**
 * Test that most critical object does not issue an alert if existance probability is too low. Verify that no alert is set by
 * Recw_Set_Object_Alert_Level function. \uts{CSCSA-44371} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_no_alert_if_existance_probability_is_too_low)
{
   /** \arrange Set up most critical object and calibration values such that level 2 conditions are met except existance
    * probability. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability =
      recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2] - EPSILON;
   recw_obj.tracker_data.status    = PA_OBJ_STATUS_MATURE;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_2] + 2.0f;
   recw_obj.attributes.ttc     = recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2];
   recw_pers.f_is_rear_blocked = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_NO_ALERT);
}

/**
 * Test that most critical object does not issue an alert if combined crash probability is too low. Verify that no alert is set by
 * Recw_Set_Object_Alert_Level function. \uts{CSCSA-44375} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_no_alert_if_combined_crash_prob_is_too_low)
{
   /** \arrange Set up most critical object and calibration values such that combined crash probability is too low for alert. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_1] / 2.0f;
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] + 2.0f;
   recw_obj.attributes.ttc     = recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_pers.f_is_rear_blocked = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_NO_ALERT);
}

/**
 * Test that most critical object does not issue an alert if TTC is too large. Verify that no alert is set by
 * Recw_Set_Object_Alert_Level function. \uts{CSCSA-44376} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_no_alert_if_tcc_is_too_large)
{
   /** \arrange Set up most critical object and calibration values such that TTC is too large for alert. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] + 2.0f;
   recw_obj.attributes.ttc     = recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] * 2.0f;
   recw_pers.f_is_rear_blocked = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that no alert is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_NO_ALERT);
}

/**
 * Test that most critical object issues an level 1 alert if all conditions are met. Verify that alert level 1 is set by
 * Recw_Set_Object_Alert_Level function. \uts{CSCSA-44377} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_alert_level_1_if_conditions_are_met)
{
   /** \arrange Set up most critical object and calibration values such that level 1 conditions are met. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_1] + 2.0f;
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] = recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] / 2.0f;
   recw_obj.attributes.ttc                                     = recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1];
   recw_pers.f_is_rear_blocked                                 = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_FALSE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that alert level 1 is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_ALERT_ACTIVE_LEVEL_1);
}

/**
 * Test that most critical object issues an level 2 alert if all conditions are met. Verify that alert level 2 is set by
 * Recw_Set_Object_Alert_Level function. \uts{CSCSA-44378} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__sets_alert_level_2_if_conditions_are_met)
{
   /** \arrange Set up most critical object and calibration values such that level 2 conditions are met. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_2] + 2.0f;
   recw_obj.attributes.ttc     = recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2];
   recw_pers.f_is_rear_blocked = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that alert level 2 is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_ALERT_ACTIVE_LEVEL_2);
}

/**
 * Test that most critical object issues an level 2 alert is suppress if object is a pedestrian. Verify that alert level 1 is set
 * by Recw_Set_Object_Alert_Level function. \uts{CSCSA-308812} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__alert_level_2_suppress_due_to_pedestrian)
{
   /** \arrange Set up most critical object and calibration values such that level 2 conditions are met. */
   Recw_Object_T recw_obj{};

   recw_obj.tracker_data.existence_probability = recw_instance.calibration.k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   recw_obj.tracker_data.obj_class             = PA_OBJ_CLASS_PEDESTRIAN;
   recw_obj.tracker_data.stage_age = recw_instance.calibration.k_recw_min_stage_age_for_alert_level[RECW_INDEX_ALERT_LEVEL_1];
   recw_obj.attributes.crash_prob_combined = recw_instance.calibration.k_recw_min_crash_prob[RECW_INDEX_ALERT_LEVEL_2];
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_1] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_2] + 2.0f;
   recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2] =
      recw_instance.calibration.k_recw_min_ttc_for_alert_level[RECW_INDEX_ALERT_LEVEL_2] + 2.0f;
   recw_obj.attributes.ttc     = recw_obj.attributes.ttc_threshold[RECW_INDEX_ALERT_LEVEL_2];
   recw_pers.f_is_rear_blocked = FBK_FALSE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_1]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_INDEX_ALERT_LEVEL_2]              = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_1] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_INDEX_ALERT_LEVEL_2] = FBK_TRUE;
   recw_instance.calibration.k_recw_f_suppress_alert_lvl_2_for_pedestrian                      = FBK_TRUE;

   /** \action Call function to calculate alert level. */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert Verify that alert level 1 is set. */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_ALERT_ACTIVE_LEVEL_1);
}


/**
 * Test that object will not cause alert in case of too low overlap with host.
 * \uts{CSCSA-204762} \sdd{SF-7845} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Set_Object_Alert_Level__min_obj_overlap_set_to_false)
{
   /** \arrange Set-up critical object with no alert present and low overlap with target */
   Recw_Object_T recw_obj{};
   recw_obj.attributes.alert_level = RECW_NO_ALERT;

   recw_obj.attributes.crash_prob_combined                                    = 0.9f;
   recw_instance.calibration.k_recw_min_crash_prob[RECW_ALERT_ACTIVE_LEVEL_1] = 0.16f;

   recw_instance.calibration.k_recw_f_use_rear_blockage[RECW_ALERT_ACTIVE_LEVEL_1] = FBK_TRUE;
   recw_instance.persistent.f_is_rear_blocked                                      = FBK_FALSE;

   recw_instance.calibration.k_recw_f_allow_alert_on_coasted_objects[RECW_ALERT_ACTIVE_LEVEL_1] = FBK_TRUE;
   recw_obj.tracker_data.status                                                                 = PA_OBJ_STATUS_MATURE;

   recw_obj.attributes.ttc                                     = 0.3f;
   recw_obj.attributes.ttc_threshold[0]                        = recw_obj.attributes.ttc + 0.2f;
   recw_instance.calibration.k_recw_min_ttc_for_alert_level[0] = 0.2f;

   recw_obj.tracker_data.stage_age                                   = 4u;
   recw_instance.calibration.k_recw_min_stage_age_for_alert_level[0] = 4u;

   recw_instance.calibration.k_recw_min_existence_prob[0] = 0.8f;
   recw_obj.tracker_data.existence_probability            = 0.8f;

   recw_obj.attributes.overlap                                     = 0.1f;
   recw_instance.calibration.k_recw_min_overlap_for_alert_level[0] = 0.8f;

   /** \action Call Function to set alert level */
   Recw_Set_Object_Alert_Level(&recw_obj, &recw_instance.calibration, &recw_pers);

   /** \assert No RECW alert on core output */
   EXPECT_EQ(recw_obj.attributes.alert_level, RECW_NO_ALERT);
}

/**
 * Test that the host speed is classified as in allowed range, if the host speed is in the hystersis range of the minimal value and
 * hysteresis is applied. \uts{CSCSA-44379} \sdd{SF-7837} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test,
       Recw_Is_Host_Speed_In_Allowed_Range__returns_true_if_hysteresis_is_applied_and_host_speed_in_minimal_value_hysteresis_range)
{
   /** \arrange Set up calibration values and host speed such that host speed is only in range if hysteresis for minimal value is
    * applied. Set up persistent data such that hysteresis is applied. */
   boolean_T f_host_speed_in_allowed_range = FBK_FALSE;

   recw_instance.calibration.k_recw_min_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 1.0f;
   recw_instance.calibration.k_recw_min_host_speed_hys                        = 0.1f;
   recw_instance.calibration.k_recw_max_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 20.0f;
   recw_instance.calibration.k_recw_max_host_speed_hys                        = 0.1f;

   recw_pers.f_host_speed_in_allowed_range = FBK_TRUE;

   p_vehicle_data->host_speed = recw_instance.calibration.k_recw_min_host_speed[RECW_ALERT_ACTIVE_LEVEL_1]
                                - 0.5f * recw_instance.calibration.k_recw_min_host_speed_hys;

   /** \action Call function to check if host speed is in allowed range. */
   f_host_speed_in_allowed_range = Recw_Is_Host_Speed_In_Allowed_Range(&recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that host speed is classified as in allowed range. */
   EXPECT_TRUE(f_host_speed_in_allowed_range);
}

/**
 * Test that the host speed is classified as in allowed range, if the host speed is in the hystersis range of the maximal value and
 * hysteresis is applied. \uts{CSCSA-44380} \sdd{SF-7837} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test,
       Recw_Is_Host_Speed_In_Allowed_Range__returns_true_if_hysteresis_is_applied_and_host_speed_in_maximal_value_hysteresis_range)
{
   /** \arrange Set up calibration values and host speed such that host speed is only in range if hysteresis for maximal value is
    * applied. Set up persistent data such that hysteresis is applied. */
   boolean_T f_host_speed_in_allowed_range = FBK_FALSE;

   recw_instance.calibration.k_recw_min_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 1.0f;
   recw_instance.calibration.k_recw_min_host_speed_hys                        = 0.1f;
   recw_instance.calibration.k_recw_max_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 20.0f;
   recw_instance.calibration.k_recw_max_host_speed_hys                        = 0.1f;

   recw_pers.f_host_speed_in_allowed_range = FBK_TRUE;

   p_vehicle_data->host_speed = recw_instance.calibration.k_recw_max_host_speed[RECW_ALERT_ACTIVE_LEVEL_1]
                                + 0.5f * recw_instance.calibration.k_recw_max_host_speed_hys;

   /** \action Call function to check if host speed is in allowed range. */
   f_host_speed_in_allowed_range = Recw_Is_Host_Speed_In_Allowed_Range(&recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that host speed is classified as in allowed range. */
   EXPECT_TRUE(f_host_speed_in_allowed_range);
}

/**
 * Test that the host speed is classified as in not allowed range, if the host speed is outside of hystersis range.
 * \uts{CSCSA-44381} \sdd{SF-7837} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Host_Speed_In_Allowed_Range__returns_false_if_host_speed_outside_of_hysteresis_range_too_high)
{
   /** \arrange Set up calibration values and host speed such that host speed is too high and outside of hysteresis range. Set up
    * persistent data such that hysteresis is applied. */
   boolean_T f_host_speed_in_allowed_range = FBK_TRUE;

   recw_instance.calibration.k_recw_min_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 1.0f;
   recw_instance.calibration.k_recw_min_host_speed_hys                        = 0.1f;
   recw_instance.calibration.k_recw_max_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 20.0f;
   recw_instance.calibration.k_recw_max_host_speed_hys                        = 0.1f;

   recw_pers.recw_alert_prev_cycle         = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_pers.f_host_speed_in_allowed_range = FBK_TRUE;

   p_vehicle_data->host_speed = recw_instance.calibration.k_recw_max_host_speed[RECW_ALERT_ACTIVE_LEVEL_1]
                                + 2.0f * recw_instance.calibration.k_recw_max_host_speed_hys;

   /** \action Call function to check if host speed is in allowed range. */
   f_host_speed_in_allowed_range = Recw_Is_Host_Speed_In_Allowed_Range(&recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that host speed is classified as not in allowed range. */
   EXPECT_FALSE(f_host_speed_in_allowed_range);
}

/**
 * Test that the host speed is classified as in not allowed range, if the host speed is too low and outside of hystersis range.
 * \uts{CSCSA-112388} \sdd{SF-7837} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Host_Speed_In_Allowed_Range__returns_false_if_host_speed_outside_of_hysteresis_range_too_low)
{
   /** \arrange Set up calibration values and host speed such that host speed is outside of hysteresis range. Set up persistent
    * data such that hysteresis is applied. */
   boolean_T f_host_speed_in_allowed_range = FBK_TRUE;

   recw_instance.calibration.k_recw_min_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 1.0f;
   recw_instance.calibration.k_recw_min_host_speed_hys                        = 0.1f;
   recw_instance.calibration.k_recw_max_host_speed[RECW_ALERT_ACTIVE_LEVEL_1] = 20.0f;
   recw_instance.calibration.k_recw_max_host_speed_hys                        = 0.1f;

   recw_pers.recw_alert_prev_cycle         = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_pers.f_host_speed_in_allowed_range = FBK_TRUE;

   p_vehicle_data->host_speed = recw_instance.calibration.k_recw_min_host_speed[RECW_ALERT_ACTIVE_LEVEL_1]
                                - 2.0f * recw_instance.calibration.k_recw_min_host_speed_hys;

   /** \action Call function to check if host speed is in allowed range. */
   f_host_speed_in_allowed_range = Recw_Is_Host_Speed_In_Allowed_Range(&recw_instance.calibration, &recw_pers, p_vehicle_data);

   /** \assert Verify that host speed is classified as not in allowed range. */
   EXPECT_FALSE(f_host_speed_in_allowed_range);
}

/**
 * Test that the rear blockage is not set, if qualifying cycles are not reached and there is an active blockage in current cycle.
 * \uts{CSCSA-44382} \sdd{SF-8011} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Update_Rear_Blockage__sets_rear_blockage_FALSE_if_qualifying_cycles_not_reached)
{
   /** \arrange Set up calibrations and persistent data such that qualifying cycles are not reached and set active blockage in
    * current cycle. */
   boolean_T f_is_rear_blocked_current_cycle                        = FBK_TRUE;
   uint8_t rear_blockage_obj_idx_current_cycle                      = 6u;
   recw_instance.calibration.k_recw_rear_blockage_qualifying_cycles = 3u;
   recw_pers.recw_rear_blockage_qualifying_counter = recw_instance.calibration.k_recw_rear_blockage_qualifying_cycles - 1u;
   recw_pers.f_is_rear_blocked                     = FBK_FALSE;
   recw_pers.recw_rear_blockage_object_index       = PA_INVALID_OBJ_INDEX;
   p_vehicle_data->host_speed                      = recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold;

   /** \action Call function to update rear blockage. */
   Recw_Update_Rear_Blockage(&recw_pers, f_is_rear_blocked_current_cycle, rear_blockage_obj_idx_current_cycle,
                             &recw_instance.calibration, &data);

   /** \assert Verify that rear blockage is not active. */
   EXPECT_FALSE(recw_pers.f_is_rear_blocked);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(recw_pers.recw_rear_blockage_qualifying_counter, recw_instance.calibration.k_recw_rear_blockage_qualifying_cycles);
}

/**
 * Test that the rear blockage is set, if qualifying cycles are reached and there is an active blockage in current cycle.
 * \uts{CSCSA-44383} \sdd{SF-8011} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Update_Rear_Blockage__sets_rear_blockage_TRUE_if_qualifying_cycles_reached)
{
   /** \arrange Set up calibrations and persistent data such that qualifying cycles are reached and set active blockage in current
    * cycle. */
   boolean_T f_is_rear_blocked_current_cycle       = FBK_TRUE;
   uint8_t rear_blockage_obj_idx_current_cycle     = 6u;
   recw_pers.recw_rear_blockage_qualifying_counter = recw_instance.calibration.k_recw_rear_blockage_qualifying_cycles;
   recw_pers.f_is_rear_blocked                     = FBK_FALSE;
   recw_pers.recw_rear_blockage_object_index       = PA_INVALID_OBJ_INDEX;
   p_vehicle_data->host_speed                      = recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold;

   /** \action Call function to update rear blockage. */
   Recw_Update_Rear_Blockage(&recw_pers, f_is_rear_blocked_current_cycle, rear_blockage_obj_idx_current_cycle,
                             &recw_instance.calibration, &data);

   /** \assert Verify that rear blockage is active. */
   EXPECT_TRUE(recw_pers.f_is_rear_blocked);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, rear_blockage_obj_idx_current_cycle);
   EXPECT_EQ(recw_pers.recw_rear_blockage_qualifying_counter, recw_instance.calibration.k_recw_rear_blockage_qualifying_cycles + 1u);
}

/**
 * Test that the rear blockage is not set, if ego speed is above threshold.
 * \uts{CSCSA-44384} \sdd{SF-8011} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Update_Rear_Blockage__sets_rear_blockage_FALSE_for_ego_speed_above_threshold)
{
   /** \arrange Set up calibrations and persistent data such that ego speed is above threshold. */
   boolean_T f_is_rear_blocked_current_cycle       = FBK_TRUE;
   uint8_t rear_blockage_obj_idx_current_cycle     = 6u;
   recw_pers.recw_rear_blockage_qualifying_counter = recw_instance.calibration.k_recw_rear_blockage_qualifying_cycles;
   recw_pers.f_is_rear_blocked                     = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index       = 6u;
   p_vehicle_data->host_speed                      = recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold + EPSILON;

   /** \action Call function to update rear blockage. */
   Recw_Update_Rear_Blockage(&recw_pers, f_is_rear_blocked_current_cycle, rear_blockage_obj_idx_current_cycle,
                             &recw_instance.calibration, &data);

   /** \assert Verify that rear blockage is not active. */
   EXPECT_FALSE(recw_pers.f_is_rear_blocked);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(recw_pers.recw_rear_blockage_qualifying_counter, FBK_ZERO_UINT);
}

/**
 * Test that the rear blockage is not reset, if ego speed is below threshold.
 * \uts{CSCSA-44385} \sdd{SF-8011} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Update_Rear_Blockage__keeps_rear_blockage_TRUE_for_ego_speed_below_threshold)
{
   /** \arrange Set up calibrations and persistent data such that ego speed is below threshold and rear blockage was set before. */
   boolean_T f_is_rear_blocked_current_cycle       = FBK_FALSE;
   uint8_t rear_blockage_obj_idx_current_cycle     = PA_INVALID_OBJ_INDEX;
   recw_pers.recw_rear_blockage_qualifying_counter = FBK_ZERO_UINT;
   recw_pers.f_is_rear_blocked                     = FBK_TRUE;
   p_vehicle_data->host_speed                      = recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold - EPSILON;

   /** \action Call function to update rear blockage. */
   Recw_Update_Rear_Blockage(&recw_pers, f_is_rear_blocked_current_cycle, rear_blockage_obj_idx_current_cycle,
                             &recw_instance.calibration, &data);

   /** \assert Verify that rear blockage is active. */
   EXPECT_TRUE(recw_pers.f_is_rear_blocked);
   EXPECT_EQ(recw_pers.recw_rear_blockage_qualifying_counter, FBK_ZERO_UINT);
}

/**
 * Test that the rear blockage is not reset, if ego speed is below threshold and negative in sign.
 * \uts{CSCSA-112389} \sdd{SF-8011} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Update_Rear_Blockage__keeps_rear_blockage_TRUE_for_ego_speed_negative_and_below_threshold)
{
   /** \arrange Set up calibrations and persistent data such that ego speed is below threshold and rear blockage was set before. */
   boolean_T f_is_rear_blocked_current_cycle       = FBK_FALSE;
   uint8_t rear_blockage_obj_idx_current_cycle     = PA_INVALID_OBJ_INDEX;
   recw_pers.recw_rear_blockage_qualifying_counter = FBK_ZERO_UINT;
   recw_pers.f_is_rear_blocked                     = FBK_TRUE;
   p_vehicle_data->host_speed = -(recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold - EPSILON);

   /** \action Call function to update rear blockage. */
   Recw_Update_Rear_Blockage(&recw_pers, f_is_rear_blocked_current_cycle, rear_blockage_obj_idx_current_cycle,
                             &recw_instance.calibration, &data);

   /** \assert Verify that rear blockage is active. */
   EXPECT_TRUE(recw_pers.f_is_rear_blocked);
   EXPECT_EQ(recw_pers.recw_rear_blockage_qualifying_counter, FBK_ZERO_UINT);
}

/**
 * Test that blocking away condition in function will cause reset in previous rear blockage.
 * \uts{CSCSA-204763} \sdd{SF-8011} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Update_Rear_Blockage__blockage_in_previous_cycle_reset)
{
   /** \arrange Fill parameters to cause object to be considered as moving away */
   boolean_T f_is_rear_blocked_current_cycle   = FBK_FALSE;
   uint8_t rear_blockage_obj_idx_current_cycle = PA_INVALID_OBJ_INDEX;

   recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold = 0.6f;
   recw_instance.calibration.k_recw_min_object_age                    = 11u;
   recw_instance.calibration.k_recw_rear_blockage_width               = 2.0;

   p_vehicle_data->host_speed = recw_instance.calibration.k_recw_rear_blockage_ego_speed_threshold - EPSILON;

   recw_instance.persistent.f_is_rear_blocked               = FBK_TRUE;
   recw_instance.persistent.recw_rear_blockage_object_index = 100u;

   data.object_data[recw_instance.persistent.recw_rear_blockage_object_index].age =
      recw_instance.calibration.k_recw_min_object_age + 1u;
   data.object_data[recw_instance.persistent.recw_rear_blockage_object_index].vcs_pos.y =
      recw_instance.calibration.k_recw_rear_blockage_width + 1.0f;
   data.object_data[recw_instance.persistent.recw_rear_blockage_object_index].status = PA_OBJ_STATUS_MATURE;

   /** \action Call function */
   Recw_Update_Rear_Blockage(&recw_pers, f_is_rear_blocked_current_cycle, rear_blockage_obj_idx_current_cycle,
                             &recw_instance.calibration, &data);

   /** \assert Verify that rear blockage is reseted. */
   EXPECT_FALSE(recw_pers.f_is_rear_blocked);
   EXPECT_EQ(recw_pers.recw_rear_blockage_qualifying_counter, FBK_ZERO_UINT);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, PA_INVALID_OBJ_INDEX);
}

/**
 * Test that rear blockage object index is resetted when object exists no longer with invalid status. Also check that for moving
 * away returns false. \uts{CSCSA-44386} \sdd{SF-8012} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Blocking_Object_Moving_Away__returns_false_if_rear_blockage_object_exists_no_longer_invalid)
{
   /** \arrange Set up calibrations and persistent data such that rear blockage object exists no longer. */
   uint8_t obj_index                         = 6u;
   boolean_T result                          = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index = obj_index;
   recw_pers.f_is_rear_blocked               = FBK_TRUE;
   object_data[obj_index].status             = PA_OBJ_STATUS_INVALID;

   /** \action Call function to check if blocking object is moving away. */
   result = Recw_Is_Blocking_Object_Moving_Away(&recw_pers, &recw_instance.calibration, &data);

   /** \assert Verify that result is false. */
   EXPECT_FALSE(result);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, PA_INVALID_OBJ_INDEX);
}

/**
 * Test that rear blockage object index is resetted when object exists no longer with implausible position. Also check that for
 * moving away returns false. \uts{CSCSA-44387} \sdd{SF-8012} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Blocking_Object_Moving_Away__returns_false_if_rear_blockage_object_exists_no_longer_implausible)
{
   /** \arrange Set up calibrations and persistent data such that rear blockage object exists no longer. */
   uint8_t obj_index                               = 6u;
   boolean_T result                                = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index       = obj_index;
   recw_pers.f_is_rear_blocked                     = FBK_TRUE;
   recw_instance.calibration.k_recw_min_object_age = 10u;
   object_data[obj_index].status                   = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].age                      = recw_instance.calibration.k_recw_min_object_age - 1u;
   object_data[obj_index].vcs_pos.y                = 3.0f * recw_instance.calibration.k_recw_rear_blockage_width;

   /** \action Call function to check if blocking object is moving away. */
   result = Recw_Is_Blocking_Object_Moving_Away(&recw_pers, &recw_instance.calibration, &data);

   /** \assert Verify that result is false. */
   EXPECT_FALSE(result);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, PA_INVALID_OBJ_INDEX);
}

/**
 * Test that rear blockage moving away check returns false if no rear blockage object is present.
 * \uts{CSCSA-44388} \sdd{SF-8012} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Blocking_Object_Moving_Away__returns_false_if_no_rear_blockage_object_is_present)
{
   /** \arrange Set up calibrations and persistent data such that no rear blockage object is present. */
   boolean_T result                          = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index = PA_INVALID_OBJ_INDEX;
   recw_pers.f_is_rear_blocked               = FBK_TRUE;

   /** \action Call function to check if blocking object is moving away. */
   result = Recw_Is_Blocking_Object_Moving_Away(&recw_pers, &recw_instance.calibration, &data);

   /** \assert Verify that result is false. */
   EXPECT_FALSE(result);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, PA_INVALID_OBJ_INDEX);
}

/**
 * Test that rear blockage moving away check returns true if blocking object is moving away laterally to the left.
 * \uts{CSCSA-44389} \sdd{SF-8012} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Blocking_Object_Moving_Away__returns_true_if_object_is_moving_away_laterally_to_left)
{
   /** \arrange Set up calibrations and persistent data such that rear blockage object is moving away laterally. */
   uint8_t obj_index                         = 6u;
   boolean_T result                          = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index = obj_index;
   recw_pers.f_is_rear_blocked               = FBK_TRUE;

   object_data[obj_index].index      = obj_index;
   object_data[obj_index].f_moveable = true;

   object_data[obj_index].status    = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.x = -p_vehicle_data->host_length;
   object_data[obj_index].age       = recw_instance.calibration.k_recw_min_object_age + 20u;
   object_data[obj_index].vcs_pos.y = 1.5f * recw_instance.calibration.k_recw_rear_blockage_width;

   /** \action Call function to check if blocking object is moving away. */
   result = Recw_Is_Blocking_Object_Moving_Away(&recw_pers, &recw_instance.calibration, &data);

   /** \assert Verify that result is true. */
   EXPECT_TRUE(result);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, obj_index);
}

/**
 * Test that rear blockage moving away check returns true if blocking object is moving away laterally to the right.
 * \uts{CSCSA-112390} \sdd{SF-8012} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Blocking_Object_Moving_Away__returns_true_if_object_is_moving_away_laterally_to_right)
{
   /** \arrange Set up calibrations and persistent data such that rear blockage object is moving away laterally. */
   uint8_t obj_index                         = 6u;
   boolean_T result                          = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index = obj_index;
   recw_pers.f_is_rear_blocked               = FBK_TRUE;
   object_data[obj_index].status             = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.x          = -p_vehicle_data->host_length;
   object_data[obj_index].age                = recw_instance.calibration.k_recw_min_object_age + 20u;
   object_data[obj_index].vcs_pos.y          = -1.5f * recw_instance.calibration.k_recw_rear_blockage_width;

   /** \action Call function to check if blocking object is moving away. */
   result = Recw_Is_Blocking_Object_Moving_Away(&recw_pers, &recw_instance.calibration, &data);

   /** \assert Verify that result is true. */
   EXPECT_TRUE(result);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, obj_index);
}

/**
 * Test that rear blockage moving away check returns true if blocking object is moving away longitudinally.
 * \uts{CSCSA-44390} \sdd{SF-8012} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Blocking_Object_Moving_Away__returns_true_if_object_is_moving_away_longitudinally)
{
   /** \arrange Set up calibrations and persistent data such that rear blockage object is moving away longitudinally. */
   uint8_t obj_index                         = 6u;
   boolean_T result                          = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index = obj_index;
   recw_pers.f_is_rear_blocked               = FBK_TRUE;
   object_data[obj_index].status             = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.x = -(3.0f * recw_instance.calibration.k_recw_rear_blockage_length + p_vehicle_data->host_length);
   object_data[obj_index].age       = recw_instance.calibration.k_recw_min_object_age + 20u;
   object_data[obj_index].vcs_pos.y = FBK_ZERO_F;

   /** \action Call function to check if blocking object is moving away. */
   result = Recw_Is_Blocking_Object_Moving_Away(&recw_pers, &recw_instance.calibration, &data);

   /** \assert Verify that result is true. */
   EXPECT_TRUE(result);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, obj_index);
}

/**
 * Test that rear blockage moving away check returns false if blocking object is not moving away longitudinally.
 * \uts{CSCSA-112391} \sdd{SF-8012} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Blocking_Object_Moving_Away__returns_false_if_object_is_not_moving_away_longitudinally)
{
   /** \arrange Set up calibrations and persistent data such that rear blockage object is not moving away longitudinally. */
   uint8_t obj_index                         = 6u;
   boolean_T result                          = FBK_TRUE;
   recw_pers.recw_rear_blockage_object_index = obj_index;
   recw_pers.f_is_rear_blocked               = FBK_TRUE;
   object_data[obj_index].status             = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].vcs_pos.x = -(1.5f * recw_instance.calibration.k_recw_rear_blockage_length + p_vehicle_data->host_length);
   object_data[obj_index].age       = recw_instance.calibration.k_recw_min_object_age + 20u;
   object_data[obj_index].vcs_pos.y = FBK_ZERO_F;

   /** \action Call function to check if blocking object is moving away. */
   result = Recw_Is_Blocking_Object_Moving_Away(&recw_pers, &recw_instance.calibration, &data);

   /** \assert Verify that result is false. */
   EXPECT_FALSE(result);
   EXPECT_EQ(recw_pers.recw_rear_blockage_object_index, obj_index);
}

/**
 * Test that object is rear blockage relevant, if it is close to the ego rear.
 * \uts{CSCSA-44391} \sdd{SF-8013} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Object_Rear_Blockage_Relevant__returns_true_for_rear_blockage_relevant_object)
{
   /** \arrange Set up RECW object and calibrations such that object is rear blockage relevant. */
   Recw_Object_T recw_obj{};
   boolean_T result                = FBK_FALSE;
   recw_obj.tracker_data.vcs_pos.x = -recw_instance.calibration.k_recw_rear_blockage_length - p_vehicle_data->host_length;
   recw_obj.tracker_data.vcs_pos.y = FBK_ZERO_F;

   /** \action Call function to check if object is rear blockage relevant. */
   result = Recw_Is_Object_Rear_Blockage_Relevant(&recw_obj, &recw_instance.calibration, p_vehicle_data);

   /** \assert Verify that result is true. */
   EXPECT_TRUE(result);
}

/**
 * Test that object is not rear blockage relevant, if it is on left side of ego.
 * \uts{CSCSA-44392} \sdd{SF-8013} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Object_Rear_Blockage_Relevant__returns_false_for_object_left_of_ego)
{
   /** \arrange Set up RECW object and calibrations such that object is left of ego. */
   Recw_Object_T recw_obj{};
   boolean_T result                = FBK_FALSE;
   recw_obj.tracker_data.vcs_pos.x = -recw_instance.calibration.k_recw_rear_blockage_length - p_vehicle_data->host_length;
   recw_obj.tracker_data.vcs_pos.y = -2.0f * recw_instance.calibration.k_recw_rear_blockage_width;

   /** \action Call function to check if object is rear blockage relevant. */
   result = Recw_Is_Object_Rear_Blockage_Relevant(&recw_obj, &recw_instance.calibration, p_vehicle_data);

   /** \assert Verify that result is false. */
   EXPECT_FALSE(result);
}

/**
 * Test that object is not rear blockage relevant, if it is longitudinally outside of rear blockage area.
 * \uts{CSCSA-112392} \sdd{SF-8013} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Test, Recw_Is_Object_Rear_Blockage_Relevant__returns_false_for_object_longitudinally_not_in_blockage_area)
{
   /** \arrange Set up RECW object and calibrations such that object is longitudinally out of rear blockage area. */
   Recw_Object_T recw_obj{};
   boolean_T result                = FBK_FALSE;
   recw_obj.tracker_data.vcs_pos.x = -(3.0f * recw_instance.calibration.k_recw_rear_blockage_length + p_vehicle_data->host_length);
   recw_obj.tracker_data.vcs_pos.y = FBK_ZERO_F;

   /** \action Call function to check if object is rear blockage relevant. */
   result = Recw_Is_Object_Rear_Blockage_Relevant(&recw_obj, &recw_instance.calibration, p_vehicle_data);

   /** \assert Verify that result is false. */
   EXPECT_FALSE(result);
}
