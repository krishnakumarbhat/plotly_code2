/**
 * @file ta_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45025}
 */

#include "ta_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "ta.c"
#include "ta_constants.h"
}

using ::testing::Eq;
using ::testing::FloatNear;

/**
 * Create a TA object and reset it to validate reset functionality.
 * \uts{CSCSA-45026} \sdd{SF-8649} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Reset_Object__reset_given_ta_object)
{
   /** \arrange Set up TA object with non-default properties. */
   ta_object.tracker_data.id              = 1;
   ta_object.attributes.ttc               = 0.8f;
   ta_object.attributes.f_obj_ta_relevant = FBK_TRUE;

   /** \action Reset TA object */
   Ta_Reset_Object(&ta_object, &ta_cal);

   /** \assert Check if TA object has default values. */
   EXPECT_EQ(ta_object.tracker_data.id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(ta_object.attributes.ttc, TA_INVALID_TTC);
   EXPECT_FALSE(ta_object.attributes.f_obj_ta_relevant);
}

/**
 * Create a mature TA object and ID and verify that it is recognized as valid.
 * \uts{CSCSA-45036} \sdd{SF-8647} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Is_Object_Valid__mature_obj_is_valid)
{
   /** \arrange Set up TA object with MATURE status and valid ID. */
   object_data[object_index].id     = 1;
   object_data[object_index].status = PA_OBJ_STATUS_MATURE;

   /** \action Validate TA object */
   boolean_T result = Ta_Is_Object_Valid(&data, object_index);

   /** \assert Check if TA object is valid. */
   EXPECT_TRUE(result);
}

/**
 * Create a coasted TA object and ID and verify that it is recognized as valid.
 * \uts{CSCSA-45037} \sdd{SF-8647} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Is_Object_Valid__coasted_obj_is_valid)
{
   /** \arrange Set up TA object with COASTED status and valid ID. */
   object_data[object_index].id     = 1;
   object_data[object_index].status = PA_OBJ_STATUS_COASTED;

   /** \action Validate TA object */
   boolean_T result = Ta_Is_Object_Valid(&data, object_index);

   /** \assert Check if TA object is valid. */
   EXPECT_TRUE(result);
}

/**
 * Create a new TA object an ID and verify that it is recognized as invalid.
 * \uts{CSCSA-93449} \sdd{SF-8647} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Is_Object_Valid__obj_with_status_new_is_invalid)
{
   /** \arrange Set up TA object with MATURE status and invalid ID. */
   object_data[object_index].id     = 69;
   object_data[object_index].status = PA_OBJ_STATUS_NEW;

   /** \action Validate TA object */
   boolean_T result = Ta_Is_Object_Valid(&data, object_index);

   /** \assert Check if TA object is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Create a mature TA object without an ID and verify that it is recognized as invalid.
 * \uts{CSCSA-45027} \sdd{SF-8647} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Is_Object_Valid__obj_is_invalid)
{
   /** \arrange Set up TA object with MATURE status and invalid ID. */
   object_data[object_index].id     = 0;
   object_data[object_index].status = PA_OBJ_STATUS_MATURE;

   /** \action Validate TA object */
   boolean_T result = Ta_Is_Object_Valid(&data, object_index);

   /** \assert Check if TA object is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Create a TA object and fill its properties with VCS data from tracker. Verify that data is filled correctly.
 * \uts{CSCSA-45028} \sdd{SF-8646} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Information__fill_obj_info_from_tracker)
{
   /** \arrange Create a TA object. */
   Ta_Reset_Object(&ta_object, &ta_cal);
   object_data[object_index].curvi_coordinates_calc_method = PA_OBJ_CURVI_COORDINATES_UNKNOWN;

   /** \action Fill TA object with VCS tracker data */
   Ta_Fill_Object_Information(&ta_object, &ta_core_input, &ta_persistent, &ta_cal, object_index);

   /** \assert Check if TA object properties are correctly filled. */
   EXPECT_EQ(ta_object.tracker_data.id, object_data[object_index].id);
   EXPECT_EQ(ta_object.tracker_data.vcs_pos.x, object_data[object_index].vcs_pos.x);
   EXPECT_EQ(ta_object.tracker_data.vcs_pos.x, object_data[object_index].vcs_pos.y);
   EXPECT_EQ(ta_object.tracker_data.index, object_index);
   EXPECT_FALSE(ta_object.attributes.f_curvi_available);
}

/**
 * Create a TA object and fill its properties with curvi data from tracker. Verify that data is filled correctly.
 * \uts{CSCSA-45029} \sdd{SF-8646} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Information__fill_obj_info_for_curvi)
{
   /** \arrange Create a TA object. */
   Ta_Reset_Object(&ta_object, &ta_cal);
   object_data[object_index].curvi_coordinates_calc_method = PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;

   /** \action Fill TA object with curvi tracker data */
   Ta_Fill_Object_Information(&ta_object, &ta_core_input, &ta_persistent, &ta_cal, object_index);

   /** \assert Check if TA object properties are correctly filled. */
   EXPECT_EQ(ta_object.tracker_data.id, object_data[object_index].id);
   EXPECT_EQ(ta_object.tracker_data.curvi_pos.x, object_data[object_index].curvi_pos.x);
   EXPECT_EQ(ta_object.tracker_data.curvi_pos.x, object_data[object_index].curvi_pos.y);
   EXPECT_EQ(ta_object.tracker_data.index, object_index);
   EXPECT_TRUE(ta_object.attributes.f_curvi_available);
}

/**
 * Create a TA object and fill its properties with data from tracker modified via the debug mode. Verify that data is filled
 * correctly. \uts{CSCSA-45030} \sdd{SF-8646} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Information__test_debug_mode)
{
   /** \arrange Create a TA object. */
   Ta_Reset_Object(&ta_object, &ta_cal);

   ta_cal.k_f_ta_enable_debug_mode              = 1;
   ta_core_input.f_enable_debug_mode            = FBK_TRUE;
   ta_core_input.debug_mode_obj_pos_long_offset = 5.0f;
   ta_core_input.debug_mode_obj_pos_lat_offset  = 5.0f;

   /** \action Fill TA object with tracker data modified via the debug mode */
   Ta_Fill_Object_Information(&ta_object, &ta_core_input, &ta_persistent, &ta_cal, object_index);

   /** \assert Check if TA object properties are correctly filled. */
   EXPECT_EQ(ta_object.tracker_data.id, object_data[object_index].id);
   EXPECT_EQ(ta_object.tracker_data.vcs_pos.x, object_data[object_index].vcs_pos.x + ta_core_input.debug_mode_obj_pos_long_offset);
   EXPECT_EQ(ta_object.tracker_data.vcs_pos.x, object_data[object_index].vcs_pos.y + ta_core_input.debug_mode_obj_pos_lat_offset);
   EXPECT_EQ(ta_object.tracker_data.index, object_index);
}

/**
 * Update persistent TA data with active object data. Verify that persistent TA data is filled correctly.
 * \uts{CSCSA-45031} \sdd{SF-8644} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Persistent_Data__test_active_obj)
{
   /** \arrange Create a TA object. */
   ta_object.tracker_data.index              = object_index;
   ta_object.attributes.alert_level          = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.f_obj_in_danger_zone = FBK_TRUE;
   ta_object.attributes.f_obj_in_wing_zone   = FBK_TRUE;

   /** \action Fill persistent TA data with object data */
   Ta_Fill_Object_Persistent_Data(&ta_persistent, &ta_object, &ta_cal);

   /** \assert Check if persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_alert_mode[object_index], TA_ALERT_MODE_BOTH);
}

/**
 * Update persistent TA data with inactive object data. Verify that persistent TA data is filled correctly.
 * \uts{CSCSA-45040} \sdd{SF-8644} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Persistent_Data__test_inactive_obj)
{
   /** \arrange Create a TA object. */
   ta_object.tracker_data.index       = object_index;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_NONE;

   /** \action Fill persistent TA data with object data */
   Ta_Fill_Object_Persistent_Data(&ta_persistent, &ta_object, &ta_cal);

   /** \assert Check if persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_alert_mode[object_index], TA_ALERT_MODE_NONE);
}

/**
 * Update persistent TA data with active object data. Verify that persistent TA mode data is filled correctly.
 * \uts{CSCSA-45041} \sdd{SF-8644} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Persistent_Data__test_mode_overwrite)
{
   /** \arrange Create a TA object. */
   ta_object.tracker_data.index                 = object_index;
   ta_object.attributes.alert_level             = TA_ALERT_STATE_LEVEL_4;
   ta_object.attributes.f_obj_in_danger_zone    = FBK_TRUE;
   ta_cal.k_ta_always_overwrite_ta_mode_to_both = FBK_TRUE;

   /** \action Fill persistent TA data with object data */
   Ta_Fill_Object_Persistent_Data(&ta_persistent, &ta_object, &ta_cal);

   /** \assert Check if persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_alert_mode[object_index], TA_ALERT_MODE_BOTH);
}

/**
 * Update persistent TA data with active object data. Verify that persistent TA mode data is filled correctly.
 * \uts{CSCSA-45042} \sdd{SF-8644} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Persistent_Data__test_front_mode)
{
   /** \arrange Create a TA object. */
   ta_object.tracker_data.index                 = object_index;
   ta_object.attributes.alert_level             = TA_ALERT_STATE_LEVEL_4;
   ta_object.attributes.f_obj_in_danger_zone    = FBK_TRUE;
   ta_cal.k_ta_always_overwrite_ta_mode_to_both = FBK_FALSE;

   /** \action Fill persistent TA data with object data */
   Ta_Fill_Object_Persistent_Data(&ta_persistent, &ta_object, &ta_cal);

   /** \assert Check if persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_alert_mode[object_index], TA_ALERT_MODE_FRONT);
}

/**
 * Update persistent TA data with active object data. Verify that persistent TA mode data is filled correctly.
 * \uts{CSCSA-45043} \sdd{SF-8644} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Persistent_Data__test_rear_mode)
{
   /** \arrange Create a TA object. */
   ta_object.tracker_data.index                 = object_index;
   ta_object.attributes.alert_level             = TA_ALERT_STATE_LEVEL_4;
   ta_object.attributes.f_obj_in_wing_zone      = FBK_TRUE;
   ta_cal.k_ta_always_overwrite_ta_mode_to_both = FBK_FALSE;

   /** \action Fill persistent TA data with object data */
   Ta_Fill_Object_Persistent_Data(&ta_persistent, &ta_object, &ta_cal);

   /** \assert Check if persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_alert_mode[object_index], TA_ALERT_MODE_REAR);
}

/**
 * Do not reset persistent object data for object that caused previous alert. Verify that persistent TA mode data is filled
 * correctly. \uts{CSCSA-45046} \sdd{SF-8644} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Object_Persistent_Data__do_not_reset_when_obj_caused_prev_alert)
{
   /** \arrange Create a TA object. */
   ta_object.tracker_data.index                              = object_index;
   ta_object.attributes.alert_level                          = TA_ALERT_STATE_NONE;
   ta_persistent.ta_alert_mode[ta_object.tracker_data.index] = TA_ALERT_MODE_BOTH;
   ta_persistent.ta_side_index_prev_cycle[FBK_SIDE_RIGHT]    = ta_object.tracker_data.index;

   /** \action Fill persistent TA data with object data */
   Ta_Fill_Object_Persistent_Data(&ta_persistent, &ta_object, &ta_cal);

   /** \assert Check if persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_alert_mode[object_index], TA_ALERT_MODE_BOTH);
}

/**
 * Reset persistent TA data. Verify that persistent TA data is filled correctly.
 * \uts{CSCSA-45032} \sdd{SF-8648} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Reset_Object_Persistent_Data__reset_active_obj)
{
   /** \arrange Fill persistent TA data. */
   ta_persistent.ta_alert_mode[object_index] = TA_ALERT_MODE_BOTH;

   /** \action Reset persistent TA data */
   Ta_Reset_Object_Persistent_Data(&ta_persistent, object_index);

   /** \assert Check if persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_alert_mode[object_index], TA_ALERT_MODE_NONE);
}

/**
 * Fill persistent TA data with core output data. Verify that persistent TA data is filled correctly.
 * \uts{CSCSA-45033} \sdd{SF-8645} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Side_Persistent_Data__fill_left_side)
{
   /** \arrange Fill core output data and set default persistent data. */
   int8_t side_index = FBK_SIDE_LEFT;

   ta_core_output.ta_alert_level[side_index] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_id[side_index]          = object_index + 1;
   ta_core_output.ta_index[side_index]       = object_index;

   ta_persistent.ta_side_alert_prev_cycle[side_index] = TA_ALERT_STATE_NONE;
   ta_persistent.ta_side_id_prev_cycle[side_index]    = PA_INVALID_OBJ_ID;
   ta_persistent.ta_side_index_prev_cycle[side_index] = PA_INVALID_OBJ_ID;

   /** \action Fill side persistent data */
   Ta_Fill_Side_Persistent_Data(&ta_persistent, &ta_core_output);

   /** \assert Check if side persistent TA data are correctly filled. */
   EXPECT_EQ(ta_persistent.ta_side_alert_prev_cycle[side_index], ta_core_output.ta_alert_level[side_index]);
   EXPECT_EQ(ta_persistent.ta_side_id_prev_cycle[side_index], ta_core_output.ta_id[side_index]);
   EXPECT_EQ(ta_persistent.ta_side_index_prev_cycle[side_index], ta_core_output.ta_index[side_index]);
}

/*
 * Tests that flag resposible for using constant velocity model is set when alert has level 4. Alert was raised on right side.
 * \uts{CSCSA-45069} \sdd{SF-8630} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Fbk_Predict_Ego_Struct__set_alert_level_4_for_right_side)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   Fbk_Ego_Predict_Data_T fbk_ego_data;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_4;

   /** \action Debounce alert level */
   Ta_Fill_Fbk_Predict_Ego_Struct(&fbk_ego_data, &ta_persistent, &ta_cal);

   /** \assert Verify that the current TA alert level is set and holding is skipped. */
   EXPECT_EQ(fbk_ego_data.acc_weight_depend_on_alert_lvl, FBK_TRUE);
}

/*
 * Tests that flag resposible for using constant velocity model is set when alert has level 4. Alert was raised on left side.
 * \uts{CSCSA-93450} \sdd{SF-8630} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Fill_Fbk_Predict_Ego_Struct__set_alert_level_4_for_left_side)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   Fbk_Ego_Predict_Data_T fbk_ego_data;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_4;

   /** \action Debounce alert level */
   Ta_Fill_Fbk_Predict_Ego_Struct(&fbk_ego_data, &ta_persistent, &ta_cal);

   /** \assert Verify that the current TA alert level is set and holding is skipped. */
   EXPECT_EQ(fbk_ego_data.acc_weight_depend_on_alert_lvl, FBK_TRUE);
}


/**
 * Update the ego yaw angle for a yawrate above the specified threshold. Verify that the yaw angle is calculated correctly.
 * \uts{CSCSA-45044} \sdd{SF-8540} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Update_Ego_Yaw_Angle_To_Last_Straight_Section__set_correct_yawangle)
{
   /** \arrange Set up the vehicle and calibration values. */
   p_vehicle_data->yawrate = 0.25f;

   data.time_diff_to_last_cycle                     = 0.05f;
   ta_cal.k_ta_ego_yawangle_integration_yawrate_min = 0.01f;

   ta_persistent.ta_ego_yaw_angle_to_last_straight_section = FBK_ZERO_F;

   /** \action Update the yaw angle. */
   Ta_Update_Ego_Yaw_Angle_To_Last_Straight_Section(&ta_persistent, &data, p_vehicle_data, &ta_cal);

   /** \assert Check if the expected yawangle is correctly written to the persistent data. */
   EXPECT_EQ(ta_persistent.ta_ego_yaw_angle_to_last_straight_section, p_vehicle_data->yawrate * data.time_diff_to_last_cycle);
}

/**
 * Reset the ego yaw angle for a yawrate below the specified threshold. Verify that the yaw angle is calculated correctly.
 * \uts{CSCSA-45045} \sdd{SF-8540} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Update_Ego_Yaw_Angle_To_Last_Straight_Section__reset_yawangle)
{
   /** \arrange Set up the vehicle and calibration values. */
   p_vehicle_data->yawrate = FBK_ZERO_F;

   data.time_diff_to_last_cycle                     = 0.05f;
   ta_cal.k_ta_ego_yawangle_integration_yawrate_min = 0.01f;

   ta_persistent.ta_ego_yaw_angle_to_last_straight_section = 1.0f;

   /** \action Update the yaw angle. */
   Ta_Update_Ego_Yaw_Angle_To_Last_Straight_Section(&ta_persistent, &data, p_vehicle_data, &ta_cal);

   /** \assert Check if the expected yawangle is correctly written to the persistent data. */
   EXPECT_EQ(ta_persistent.ta_ego_yaw_angle_to_last_straight_section, FBK_ZERO_F);
}


/**
 * Execute TA algorithm main function for a valid object and check if it returns an alert level.
 * \uts{CSCSA-45038} \sdd{SF-8651} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Algorithm__full_run_valid_object)
{
   /** \arrange Valid scenario with an object that is collision critical. */
   uint8_t valid_object_index = 5;

   object_data[valid_object_index].id     = valid_object_index + 1;
   object_data[valid_object_index].status = PA_OBJ_STATUS_MATURE;
   object_data[valid_object_index].age    = 10u;

   object_data[valid_object_index].vcs_pos.x     = -3.83f;
   object_data[valid_object_index].vcs_pos.y     = 4.1f;
   object_data[valid_object_index].vcs_vel.x     = 4.44f;
   object_data[valid_object_index].vcs_vel.y     = -3.32f;
   object_data[valid_object_index].vcs_vel_rel.x = 3.83f;
   object_data[valid_object_index].vcs_vel_rel.y = -3.32f;

   object_data[valid_object_index].vcs_heading   = -0.64f;
   object_data[valid_object_index].heading_rate  = -0.75f;
   object_data[valid_object_index].curvi_heading = -0.60f;

   object_data[valid_object_index].speed                 = 5.56f;
   object_data[valid_object_index].existence_probability = 1.0f;
   object_data[valid_object_index].length                = 1.5f;
   object_data[valid_object_index].width                 = 0.5f;

   object_data[valid_object_index].class_prob_2wheel = 1.0f;

   object_data[valid_object_index].curvi_coordinates_calc_method = PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;
   object_data[valid_object_index].curvi_pos.x                   = -4.11f;
   object_data[valid_object_index].curvi_pos.y                   = 3.9f;
   object_data[valid_object_index].curvi_vel.x                   = 9.75f;
   object_data[valid_object_index].curvi_vel.y                   = -3.06f;
   object_data[valid_object_index].curvi_vel_rel.x               = 6.95f;
   object_data[valid_object_index].curvi_vel_rel.y               = -3.06f;

   p_vehicle_data->host_speed         = 2.8f;
   p_vehicle_data->yawrate            = 0.347f;
   p_vehicle_data->curvature          = 0.1f;
   p_vehicle_data->host_length        = 4.65f;
   p_vehicle_data->host_width         = 1.83f;
   p_vehicle_data->rear_axle_position = -3.5f;

   ta_persistent.ta_pred_step_dt                          = 0.1f;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_2;

   ta_cal.k_ta_alert_lvl_4_ttc_threshold = 0.0f;

   /** \action Execute TA algorithm main function for a collision critical object */
   Ta_Algorithm(&ta_core_output, &ta_persistent, &ta_core_input, &ta_cal, &ego_traj_predictor_instance);

   /** \assert Expect alert level 3 on host right side in TA core output */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_RIGHT], TA_ALERT_STATE_LEVEL_3);
}

/**
 * Execute TA algorithm main function and check if it runs through without any errors.
 * \uts{CSCSA-45039} \sdd{SF-8651} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Algorithm__no_fatal_failure)
{
   /** \arrange Not required. */

   /** \action Not required */

   /** \assert Execute TA algorithm main function, expect not fatal failures during execution. */
   EXPECT_NO_FATAL_FAILURE(Ta_Algorithm(&ta_core_output, &ta_persistent, &ta_core_input, &ta_cal, &ego_traj_predictor_instance));
}

/**
 * Execute TA algorithm run function and verify that no active alert level is set
 * \uts{CSCSA-45034} \sdd{SF-8658} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Core_Run__disable_ta_algorithm)
{
   /** \arrange Set alert levels to "no alert" */
   ta_core_input.f_ta_enable                    = FBK_FALSE;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_NONE;

   /** \action Execute Ta algorithm run function */
   Ta_Core_Run(&ta_core_output, &ta_core_input, &ta_cal, &ego_traj_predictor_instance, &ta_persistent);

   /** \assert Check that no alert is raised. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
}

/**
 * Execute TA algorithm run function and verify that no active alert level is set
 * \uts{CSCSA-93451} \sdd{SF-8658} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Core_Run__enable_ta_algorithm)
{
   /** \arrange Set alert levels to "no alert" and f_ta_enabled to TRUE */
   ta_core_input.f_ta_enable                    = FBK_TRUE;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_NONE;


   /** \action Execute Ta algorithm run function */
   Ta_Core_Run(&ta_core_output, &ta_core_input, &ta_cal, &ego_traj_predictor_instance, &ta_persistent);

   /** \assert Check that no alert is raised. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
}
/**
 * Set up a scenario with an active alert. Reset TA and verify that alerts are no longer set.
 * \uts{CSCSA-45035} \sdd{SF-8656} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Reset__test_resetting_ta_data)
{
   /** \arrange Set active alert levels */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;

   /** \action Reset TA */
   Ta_Reset(&ta_core_output, &ta_cal, &ta_persistent);

   /** \assert Check that no alert is raised. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
}

/**
 * Tests the reset of persistent data of turn assist.
 * \uts{CSCSA-45047} \sdd{SF-8620} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Reset_Persistent__check_for_defaults)
{
   /** \arrange Set non defaults to persistent */
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]          = TA_ALERT_STATE_LEVEL_3;
   ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_LEFT]             = 1u;
   ta_persistent.ta_side_index_prev_cycle[FBK_SIDE_LEFT]          = 1u;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT]  = 1u;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT]     = 1u;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_RIGHT]         = TA_ALERT_STATE_LEVEL_3;
   ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_RIGHT]            = 1u;
   ta_persistent.ta_side_index_prev_cycle[FBK_SIDE_RIGHT]         = 1u;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = 1u;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_RIGHT]    = 1u;

   /** \action Execute function to test */
   Ta_Reset(&ta_core_output, &ta_cal, &ta_persistent);

   /** \assert Check that persistent data is set to default. */
   EXPECT_EQ(ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
   EXPECT_EQ(ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ta_persistent.ta_side_index_prev_cycle[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_RIGHT], TA_ALERT_STATE_NONE);
   EXPECT_EQ(ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ta_persistent.ta_side_index_prev_cycle[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Tests the debounce functionality. Arranged alert level is set to level 1. The alert level shall not be qualified to be output to
 * the Turn Assist output structure \uts{CSCSA-45048} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_if_alert_level_1_is_suppressed)
{
   /** \arrange Setup alert status with insufficient qualifying counters. */
   ta_cal.k_ta_alert_qualifying_cycles                           = 3;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_LEVEL_1;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = 1;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_NONE;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify TA alert is not set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
}

/**
 * Tests the holding logic. The alert level toggles back to none for one cycle. The alert level be held on level 1 which occured in
 * the previous cycle. \uts{CSCSA-45049} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_if_alert_level_1_is_held)
{
   /** \arrange Setup alert status which lost validity this cycle. */
   ta_cal.k_ta_alert_holding_cycles                           = 3;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]               = TA_ALERT_STATE_NONE;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT] = 1;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]      = TA_ALERT_STATE_LEVEL_1;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify TA alert is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_1);
}

/**
 * Tests the holding logic. The alert level toggles back to level 1 on a different object. The alert level be held on level 2 which
 * occured in the previous cycle. \uts{CSCSA-45050} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_if_alert_level_2_is_held_obj_id_changed)
{
   /** \arrange Setup alert status which lost validity this cycle. */
   ta_cal.k_ta_alert_holding_cycles                           = 3;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]               = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                        = 1;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT] = 1;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]      = TA_ALERT_STATE_LEVEL_2;
   ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_LEFT]         = 2;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify TA alert is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_LEFT]);
}

/**
 * Tests the qualifying logic. The alert level is not set and the qualifying counter is saturated. Expect the qualifying counter to
 * be reset. \uts{CSCSA-45051} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_qualifying_counter_reset)
{
   /** \arrange Setup alert level with state NONE and saturated qualifying counter. */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_NONE;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = 3;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify qualifying counter is reset. */
   EXPECT_EQ(ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_INT);
}

/**
 * Tests the holding logic. The alert level is no longer set and the holding counter is saturated. Expect the qualifying counter to
 * be reset. \uts{CSCSA-45052} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_holding_counter_reset)
{
   /** \arrange Setup alert level with state NONE and saturated holding counter. */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]               = TA_ALERT_STATE_NONE;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT] = 3;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]      = TA_ALERT_STATE_NONE;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify holding counter is reset. */
   EXPECT_EQ(ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_INT);
}

/**
 * Tests the qualifying logic. The alert level is set to level 2 for qualification check. Verify that the alert level 2 is set.
 * \uts{CSCSA-45053} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_alert_level_2_qualifying)
{
   /** \arrange Setup alert level two with reset qualifying counter. */
   ta_cal.k_ta_alert_qualifying_cycles                           = 3;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_LEVEL_2;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = 3;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_NONE;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_2);
}

/**
 * Tests the qualifying logic. The alert level is set to level 4 and therefore skipping the qualification cycles for disabled
 * consecutive alert logic. Verify that the alert level 4 is set. \uts{CSCSA-45054} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_alert_level_3_skipping_qualifying_if_consecutive_alerts_disabled)
{
   /** \arrange Setup alert level 3 with reset qualifying counter. */
   ta_cal.k_ta_alert_qualifying_cycles                           = 3;
   ta_cal.k_ta_f_only_allow_consecutive_ttc_based_alert_levels   = 0u;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_LEVEL_4;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = 0;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_NONE;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_4);
}

/**
 * Tests the qualifying logic. The alert level is set to level 4 and is suppressed due to enabled consecutive alert logic. Verify
 * that the alert level is suppressed. \uts{CSCSA-45055} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_alert_level_4_suppressed_qualifying_if_consecutive_alerts_enabled)
{
   /** \arrange Setup alert level 3 with reset qualifying counter. */
   ta_cal.k_ta_alert_qualifying_cycles                           = 3;
   ta_cal.k_ta_f_only_allow_consecutive_ttc_based_alert_levels   = 1u;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_LEVEL_4;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = 0;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_NONE;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is not set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
}

/**
 * Tests the qualifying logic. The alert level is set to level 4 and the previous alert was level 2. Verify that the alert level 4
 * is set as it is a consecutive alert. \uts{CSCSA-45056} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__check_alert_level_4_qualifying_if_consecutive_alerts_enabled)
{
   /** \arrange Setup alert level 3 with reset qualifying counter. */
   ta_cal.k_ta_alert_qualifying_cycles                           = 3;
   ta_cal.k_ta_f_only_allow_consecutive_ttc_based_alert_levels   = 1u;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_LEVEL_4;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = 0;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_LEVEL_3;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_4);
}

/*
 * Tests the boundary for qualification of an object in order to be a valid candidate for a Turn assist warning. Within that test
 * the object shall not be qualified yet. \uts{CSCSA-45057} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__exact_boundary_test_object_not_qualified)
{
   /** \arrange Setup TA scenario with warning relevant target without sufficient qualifying cycles. */
   ta_cal.k_ta_alert_qualifying_cycles                           = 3;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_LEVEL_1;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = ta_cal.k_ta_alert_qualifying_cycles - ((uint8_t) 1);
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_NONE;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is not set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
}

/*
 * Tests a value greater than the boundary needed for qualification of an object in order to be a valid candidate for a Turn assist
 * warning. Within that test the object shall be qualified. \uts{CSCSA-45058} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__gt_boundary_test_object_is_qualified)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   ta_cal.k_ta_alert_qualifying_cycles                           = 3;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]                  = TA_ALERT_STATE_LEVEL_1;
   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = ta_cal.k_ta_alert_qualifying_cycles;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_NONE;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_1);
}

/*
 * Tests a the exact boundary needed for the holding logic of a previously qualified object. Within that test the object shall be
 * hold. \uts{CSCSA-45059} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__exact_boundary_test_object_level_is_hold_drop_back_from_level_3)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   ta_cal.k_ta_alert_qualifying_cycles          = 3;
   ta_cal.k_ta_alert_holding_cycles             = 2;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_2;

   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = ta_cal.k_ta_alert_qualifying_cycles;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_LEVEL_3;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT]    = ta_cal.k_ta_alert_holding_cycles - ((uint8_t) 1);

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_3);
}

/*
 * Tests a value greater than the boundary needed for the holding logic of a previously qualified object. Within that test the
 * object shall be hold. \uts{CSCSA-45060} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__gt_boundary_test_object_level_is_not_hold)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   ta_cal.k_ta_alert_qualifying_cycles          = 3;
   ta_cal.k_ta_alert_holding_cycles             = 2;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_2;

   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = ta_cal.k_ta_alert_qualifying_cycles;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_LEVEL_3;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT]    = ta_cal.k_ta_alert_holding_cycles;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_2);
}

/*
 * Tests the exact boundary needed for the reset of holding counter logic. Within that test holding counter shall be reset.
 * \uts{CSCSA-45061} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__exact_boundary_test_holding_counter_is_reset)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   ta_cal.k_ta_alert_qualifying_cycles          = 3;
   ta_cal.k_ta_alert_holding_cycles             = 2;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_2;

   ta_persistent.ta_side_alert_qualifying_counter[FBK_SIDE_LEFT] = ta_cal.k_ta_alert_qualifying_cycles;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]         = TA_ALERT_STATE_LEVEL_2;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT]    = ta_cal.k_ta_alert_holding_cycles;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that holding counter is reset. */
   EXPECT_EQ(ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_INT);
}

/*
 * Tests that alert holding is skipped for a single alert level drop from level 4 if enabled via cal.
 * \uts{CSCSA-45062} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__skip_holding_for_single_alert_level_drop_if_enabled)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   ta_cal.k_ta_alert_holding_cycles                       = 2;
   ta_cal.k_ta_f_skip_holding_for_single_alert_level_drop = FBK_TRUE;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_3;

   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]      = TA_ALERT_STATE_LEVEL_4;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT] = FBK_ZERO_UINT;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that the current TA alert level is set and holding is skipped. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_3);
}

/*
 * Tests that alert holding is skipping for a single alert level only applies for drops from level 4.
 * \uts{CSCSA-45063} \sdd{SF-8739} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Debounce_Alert_Level__holding_for_single_alert_level_drop_from_level_2)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   ta_cal.k_ta_alert_holding_cycles                       = 2;
   ta_cal.k_ta_f_skip_holding_for_single_alert_level_drop = FBK_TRUE;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;

   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_LEFT]      = TA_ALERT_STATE_LEVEL_2;
   ta_persistent.ta_side_alert_holding_counter[FBK_SIDE_LEFT] = FBK_ZERO_UINT;

   /** \action Debounce alert level */
   Ta_Debounce_Alert_Level(&ta_core_output, &ta_persistent, &ta_cal);

   /** \assert Verify that the TA alert level is held. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_LEVEL_2);
}

/*
 * Tests the updating of the algorithm state based on objects counters and vehicle state relevancy.
 * \uts{CSCSA-45064} \sdd{SF-8627} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Update_Algorithm_State__update_to_no_valid_objects)
{
   /** \arrange Setup counters and relevancy flag */
   ta_core_output.ta_f_vehicle_state_relevant = FBK_FALSE;
   ta_core_output.ta_n_valid_objects          = FBK_ZERO_UINT;
   ta_core_output.ta_n_relevant_objects       = FBK_ZERO_UINT;
   ta_core_output.ta_n_critical_objects       = FBK_ZERO_UINT;

   /** \action Update algorithm state */
   Ta_Update_Algorithm_State(&ta_core_output);

   /** \assert Verify that the correct algorithm state is selected */
   EXPECT_EQ(ta_core_output.ta_algorithm_state, TA_STATE_NO_VALID_OBJECTS);
}

/*
 * Tests the updating of the algorithm state based on objects counters and vehicle state relevancy.
 * \uts{CSCSA-45065} \sdd{SF-8627} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Update_Algorithm_State__update_to_invalid_vehicle_state)
{
   /** \arrange Setup counters and relevancy flag */
   ta_core_output.ta_f_vehicle_state_relevant = FBK_FALSE;
   ta_core_output.ta_n_valid_objects          = 1u;
   ta_core_output.ta_n_relevant_objects       = FBK_ZERO_UINT;
   ta_core_output.ta_n_critical_objects       = FBK_ZERO_UINT;

   /** \action Update algorithm state */
   Ta_Update_Algorithm_State(&ta_core_output);

   /** \assert Verify that the correct algorithm state is selected */
   EXPECT_EQ(ta_core_output.ta_algorithm_state, TA_STATE_VEHICLE_STATE_INVALID);
}

/*
 * Tests the updating of the algorithm state based on objects counters and vehicle state relevancy.
 * \uts{CSCSA-45066} \sdd{SF-8627} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Update_Algorithm_State__update_to_no_relevant_objects)
{
   /** \arrange Setup counters and relevancy flag */
   ta_core_output.ta_f_vehicle_state_relevant = FBK_TRUE;
   ta_core_output.ta_n_valid_objects          = 1u;
   ta_core_output.ta_n_relevant_objects       = FBK_ZERO_UINT;
   ta_core_output.ta_n_critical_objects       = FBK_ZERO_UINT;

   /** \action Update algorithm state */
   Ta_Update_Algorithm_State(&ta_core_output);

   /** \assert Verify that the correct algorithm state is selected */
   EXPECT_EQ(ta_core_output.ta_algorithm_state, TA_STATE_NO_RELEVANT_OBJECTS);
}

/*
 * Tests the updating of the algorithm state based on objects counters and vehicle state relevancy.
 * \uts{CSCSA-45067} \sdd{SF-8627} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Update_Algorithm_State__update_to_no_critical_objects)
{
   /** \arrange Setup counters and relevancy flag */
   ta_core_output.ta_f_vehicle_state_relevant = FBK_TRUE;
   ta_core_output.ta_n_valid_objects          = 1u;
   ta_core_output.ta_n_relevant_objects       = 1u;
   ta_core_output.ta_n_critical_objects       = FBK_ZERO_UINT;

   /** \action Update algorithm state */
   Ta_Update_Algorithm_State(&ta_core_output);

   /** \assert Verify that the correct algorithm state is selected */
   EXPECT_EQ(ta_core_output.ta_algorithm_state, TA_STATE_NO_CRITICAL_OBJECTS);
}

/*
 * Tests the updating of the algorithm state based on objects counters and vehicle state relevancy.
 * \uts{CSCSA-45068} \sdd{SF-8627} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Test, Ta_Update_Algorithm_State__update_to_critical_object_detected)
{
   /** \arrange Setup counters and relevancy flag */
   ta_core_output.ta_f_vehicle_state_relevant = FBK_TRUE;
   ta_core_output.ta_n_valid_objects          = 1u;
   ta_core_output.ta_n_relevant_objects       = 1u;
   ta_core_output.ta_n_critical_objects       = 1u;

   /** \action Update algorithm state */
   Ta_Update_Algorithm_State(&ta_core_output);

   /** \assert Verify that the correct algorithm state is selected */
   EXPECT_EQ(ta_core_output.ta_algorithm_state, TA_STATE_CRITICAL_OBJECT_DETECTED);
}
