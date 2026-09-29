/**
 * @file ltb_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ltb.c functions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-46115}
 */

#include "ltb_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ltb.c"
#include "ltb_core_calibration.h"
#include "ltb_core_input_t.h"
#include "ltb_core_output_t.h"
#include "ltb_persistent_t.h"
#include "ltb_types.h"
#include "ml_math.h"
#include "pa_const_macros.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

#ifndef NDEBUG
/**
 * Call core run function with invalid core input pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-46155} \sdd{CSCSA-53897} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Core_Run__core_input_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when core run function is called with null pointer. */
   EXPECT_DEATH(
      { Ltb_Core_Run(&ltb_core_output, NULL, p_ltb_cals, &ltb_instance.persistent, &ltb_instance.ego_traj_predictor_instance); },
      ".*p_ltb_core_input.*");
}

/**
 * Call core run function with invalid core output pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-46156} \sdd{CSCSA-53897} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Core_Run__core_output_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when core run function is called with null pointer. */
   EXPECT_DEATH(
      { Ltb_Core_Run(NULL, &ltb_core_input, p_ltb_cals, &ltb_instance.persistent, &ltb_instance.ego_traj_predictor_instance); },
      ".*p_ltb_core_output.*");
}

/**
 * Call core run function with invalid calibration pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-46157} \sdd{CSCSA-53897} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Core_Run__cals_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when core run function is called with null pointer. */
   EXPECT_DEATH(
      {
         Ltb_Core_Run(&ltb_core_output, &ltb_core_input, NULL, &ltb_instance.persistent, &ltb_instance.ego_traj_predictor_instance);
      },
      ".*p_ltb_cals*");
}
#endif

/**
 * Checks if LTB flag stays enabled, when core run function is called.
 * \uts{CSCSA-46158} \sdd{CSCSA-53897} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Core_Run__stays_enabled)
{
   /** \arrange Set f_ltb_enable to true. */
   ltb_core_input.f_ltb_enable = FBK_TRUE;

   /** \action Call function Ltb_Core_Run with input parameters ltb_core_output, ltb_core_input and p_p_ltb_cals-&gt; */
   Ltb_Core_Run(&ltb_core_output, &ltb_core_input, p_ltb_cals, &ltb_instance.persistent, &ltb_instance.ego_traj_predictor_instance);

   /** \assert Verify that f_ltb_enable is still set to true. */
   EXPECT_TRUE(ltb_core_input.f_ltb_enable);
}

/**
 * Checks if reset function is called internally, when LTB is disabled.
 * \uts{CSCSA-46159} \sdd{CSCSA-53897} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Core_Run__resets_ltb_output_if_not_enabled)
{
   /** \arrange Set f_ltb_enable to false and some core output values to non-default values. */
   ltb_core_input.f_ltb_enable                     = FBK_FALSE;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT] = ALERT_ACTIVE_LEVEL_2;

   /** \action Call function Ltb_Core_Run with input parameters ltb_core_output, ltb_core_input and p_p_ltb_cals-&gt; */
   Ltb_Core_Run(&ltb_core_output, &ltb_core_input, p_ltb_cals, &ltb_instance.persistent, &ltb_instance.ego_traj_predictor_instance);

   /** \assert Verify that f_ltb_enable is still set to false and core output is reseted to default values. */
   EXPECT_FALSE(ltb_core_input.f_ltb_enable);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], NO_ALERT);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT], NO_ALERT);
}

#ifndef NDEBUG
/**
 * Call reset function with invalid core output pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-46160} \sdd{CSCSA-53895} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Reset__ltb_core_output_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when reset function is called with null pointer. */
   EXPECT_DEATH({ Ltb_Reset(NULL, p_ltb_cals, p_ltb_persistent); }, ".*p_ltb_core_output.*");
}

/**
 * Call reset function with invalid calibration pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-46161} \sdd{CSCSA-53895} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Reset__ltb_calibration_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when reset function is called with null pointer. */
   EXPECT_DEATH({ Ltb_Reset(&ltb_core_output, NULL, p_ltb_persistent); }, ".*p_ltb_cals.*");
}
#endif

/**
 * Check if reset function for LTB object resets all object properties.
 * \uts{CSCSA-46162} \sdd{CSCSA-53886} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Reset_Ltb_Object__works_properly)
{
   /** \arrange Set up a LTB object with non-default values. */
   uint8_t obj_index = 4;
   Ltb_Fill_Raw_Tracker_Output(obj_index, -2.9f, 3.9f, 1.0f, 2.0f, 3.0f, 4.0f);
   ltb_object.tracker_data = object_data[obj_index];

   /** \action Call reset object function with the created object as input parameter. */
   Ltb_Reset_Ltb_Object(&ltb_object, p_ltb_cals);

   /** \assert Verify that all LTB object properties are reseted to default values. */
   EXPECT_EQ(ltb_object.tracker_data.id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(ltb_object.tracker_data.index, PA_INVALID_OBJ_INDEX);
   EXPECT_FLOAT_EQ(ltb_object.tracker_data.vcs_pos.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ltb_object.tracker_data.vcs_pos.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ltb_object.tracker_data.vcs_vel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ltb_object.tracker_data.vcs_vel.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ltb_object.tracker_data.vcs_vel_rel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ltb_object.tracker_data.vcs_vel_rel.y, FBK_ZERO_F);
}

#ifndef NDEBUG
/**
 * Call reset ltb object function with null pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-46163} \sdd{CSCSA-53886} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Reset_Ltb_Object__object_pointer_null_throws_expection)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when Ltb_Reset_Ltb_Object is called with null pointer. */
   EXPECT_DEATH({ Ltb_Reset_Ltb_Object(NULL, NULL); }, ".*p_ltb_object.*");
}
#endif

/**
 * Tests that an valid LTB object is classified as valid.
 * \uts{CSCSA-46164} \sdd{CSCSA-53887} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Is_Object_Valid__is_TRUE_if_obj_valid)
{
   /** \arrange Set up a tracker object that fulfills all conditions to be a valid LTB object. */
   boolean_T result;
   uint8_t obj_index = 4;
   Ltb_Fill_Raw_Tracker_Output(obj_index, -2.9f, 3.9f, 0.0f, 0.0f, 0.0f, 0.0f);

   /** \action Call function that checks if the object is valid for LTB. */
   result = Ltb_Is_Object_Valid(&object_data[obj_index]);

   /** \assert Verify that the function returns true for the valid object. */
   EXPECT_TRUE(result);
}

/**
 * Tests that an LTB object is classified as invalid if object status is NEW.
 * \uts{CSCSA-46165} \sdd{CSCSA-53887} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Is_Object_Valid__is_FALSE_if_obj_status_NEW)
{
   /** \arrange Set up a tracker object that fulfills all other conditions to be a valid LTB object, and set the object status to
    * NEW. */
   uint8_t obj_index = 4;
   boolean_T result;

   Ltb_Fill_Raw_Tracker_Output(4u, -2.9f, 3.9f, 0.0f, 0.0f, 0.0f, 0.0f);
   object_data[4u].status = PA_OBJ_STATUS_NEW;

   /** \action Call function that checks if the object is valid for LTB. */
   result = Ltb_Is_Object_Valid(&object_data[obj_index]);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result);
}

/**
 * Create a mature LTB object and ID and verify that it is recognized as valid.
 * \uts{CSCSA-46166} \sdd{CSCSA-53887} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Is_Object_Valid__mature_obj_is_valid)
{
   /** \arrange Set up LTB object with MATURE status and valid ID. */
   object_data[object_index].id     = 1;
   object_data[object_index].status = PA_OBJ_STATUS_MATURE;

   /** \action Validate LTB object */
   boolean_T result = Ltb_Is_Object_Valid(&object_data[object_index]);

   /** \assert Check if LTB object is valid. */
   EXPECT_TRUE(result);
}

/**
 * Create a coasted LTB object and ID and verify that it is recognized as valid.
 * \uts{CSCSA-46167} \sdd{CSCSA-53887} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Is_Object_Valid__coasted_obj_is_valid)
{
   /** \arrange Set up LTB object with COASTED status and valid ID. */
   object_data[object_index].id     = 1;
   object_data[object_index].status = PA_OBJ_STATUS_COASTED;

   /** \action Validate LTB object */
   boolean_T result = Ltb_Is_Object_Valid(&object_data[object_index]);

   /** \assert Check if LTB object is valid. */
   EXPECT_TRUE(result);
}

/**
 * Create a mature LTB object without an ID and verify that it is recognized as invalid.
 * \uts{CSCSA-46168} \sdd{CSCSA-53887} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Is_Object_Valid__obj_is_invalid)
{
   /** \arrange Set up LTB object with MATURE status and invalid ID. */
   object_data[object_index].id     = 0;
   object_data[object_index].status = PA_OBJ_STATUS_MATURE;

   /** \action Validate LTB object */
   boolean_T result = Ltb_Is_Object_Valid(&object_data[object_index]);

   /** \assert Check if LTB object is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Create a LTB object and reset it to validate reset functionality.
 * \uts{CSCSA-46169} \sdd{CSCSA-53886} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Reset_Ltb_Object__reset_given_ltb_object)
{
   /** \arrange Set up LTB object with non-default properties. */
   ltb_object.tracker_data.id               = 1;
   ltb_object.attributes.ttc                = 0.8f;
   ltb_object.attributes.f_obj_ltb_relevant = FBK_TRUE;

   /** \action Reset LTB object */
   Ltb_Reset_Ltb_Object(&ltb_object, p_ltb_cals);

   /** \assert Check if LTB object has default values. */
   EXPECT_EQ(ltb_object.tracker_data.id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(ltb_object.attributes.ttc, LTB_INVALID_TTC);
   EXPECT_FALSE(ltb_object.attributes.f_obj_ltb_relevant);
}

/**
 * Fill persistent LTB data with core output data. Verify that persistent LTB data is filled correctly.
 * \uts{CSCSA-46170} \sdd{CSCSA-53890} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Fill_Persistent_Data__fill_left_side)
{
   /** \arrange Fill core output data and set default persistent data. */
   int8_t side_index = FBK_SIDE_LEFT;

   ltb_core_output.ltb_alert_level[side_index] = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_id[side_index]          = object_index + 1;
   ltb_core_output.ltb_index[side_index]       = object_index;

   p_ltb_persistent->ltb_side_alert_prev_cycle[side_index] = NO_ALERT;

   /** \action Fill side persistent data */
   Ltb_Fill_Persistent_Data(p_ltb_persistent, &ltb_core_output);

   /** \assert Check if side persistent LTB data are correctly filled. */
   EXPECT_EQ(p_ltb_persistent->ltb_side_alert_prev_cycle[side_index], ltb_core_output.ltb_alert_level[side_index]);
}

/*
 * Tests that flag resposible for using constant velocity model is set when alert has level 4.
 * \uts{CSCSA-46171} \sdd{CSCSA-53892} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Fill_Fbk_Predict_Ego_Struct__set_alert_level_4_for_right_side)
{
   /** \arrange Setup LTB scenario with warning relevant target. */
   Fbk_Ego_Predict_Data_T fbk_ego_data;
   p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_3;

   /** \action Debounce alert level */
   Ltb_Fill_Fbk_Predict_Ego_Struct(&fbk_ego_data, p_ltb_persistent, p_ltb_cals);

   /** \assert Verify that the current LTB alert level is set and holding is skipped. */
   EXPECT_EQ(fbk_ego_data.acc_weight_depend_on_alert_lvl, FBK_TRUE);
}


/**
 * Update the ego yaw angle for a yawrate above the specified threshold. Verify that the yaw angle is calculated correctly.
 * \uts{CSCSA-46172} \sdd{CSCSA-53891} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Update_Ego_Yaw_Angle_To_Last_Straight_Section__set_correct_yawangle)
{
   /** \arrange Set up the vehicle and calibration values. */
   p_vehicle_data->yawrate = 0.25f;

   data.time_diff_to_last_cycle                           = 0.05f;
   p_ltb_cals->k_ltb_ego_yawangle_integration_yawrate_min = 0.01f;

   p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section = FBK_ZERO_F;

   /** \action Update the yaw angle. */
   Ltb_Update_Ego_Yaw_Angle_To_Last_Straight_Section(p_ltb_persistent, p_vehicle_data, p_ltb_cals, data.time_diff_to_last_cycle);

   /** \assert Check if the expected yawangle is correctly written to the persistent data. */
   EXPECT_EQ(p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section, p_vehicle_data->yawrate * data.time_diff_to_last_cycle);
}

/**
 * Reset the ego yaw angle for a yawrate below the specified threshold. Verify that the yaw angle is calculated correctly.
 * \uts{CSCSA-46173} \sdd{CSCSA-53891} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Update_Ego_Yaw_Angle_To_Last_Straight_Section__reset_yawangle)
{
   /** \arrange Set up the vehicle and calibration values. */
   p_vehicle_data->yawrate = FBK_ZERO_F;

   data.time_diff_to_last_cycle                           = 0.05f;
   p_ltb_cals->k_ltb_ego_yawangle_integration_yawrate_min = 0.01f;

   p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section = 1.0f;

   /** \action Update the yaw angle. */
   Ltb_Update_Ego_Yaw_Angle_To_Last_Straight_Section(p_ltb_persistent, p_vehicle_data, p_ltb_cals, data.time_diff_to_last_cycle);

   /** \assert Check if the expected yawangle is correctly written to the persistent data. */
   EXPECT_EQ(p_ltb_persistent->ltb_ego_yaw_angle_to_last_straight_section, FBK_ZERO_F);
}


/**
 * Execute LTB algorithm main function for a valid object and check if it returns an alert level.
 * \uts{CSCSA-46174} \sdd{CSCSA-53884} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Algorithm__full_run_valid_object)
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

   p_ltb_persistent->ltb_pred_step_dt                          = 0.1f;
   p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT] = ALERT_ACTIVE_LEVEL_2;

   p_ltb_cals->k_ltb_alert_lvl_2_ttc_threshold = 0.0f;

   /** \action Execute LTB algorithm main function for a collision critical object */
   Ltb_Algorithm(&ltb_core_output, p_ltb_persistent, &ltb_core_input, p_ltb_cals, &ltb_instance.ego_traj_predictor_instance);

   /** \assert Expect alert level 2 on host right side in LTB core output */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT], ALERT_ACTIVE_LEVEL_3);
}

/**
 * Execute LTB algorithm main function and check if it runs through without any errors.
 * \uts{CSCSA-46175} \sdd{CSCSA-53884} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Algorithm__no_fatal_failure)
{
   /** \arrange Not required. */

   /** \action Not required */

   /** \assert Execute LTB algorithm main function, expect not fatal failures during execution. */
   EXPECT_NO_FATAL_FAILURE(
      Ltb_Algorithm(&ltb_core_output, p_ltb_persistent, &ltb_core_input, p_ltb_cals, &ltb_instance.ego_traj_predictor_instance));
}

/**
 * Execute LTB algorithm run function and verify that no active alert level is set
 * \uts{CSCSA-46176} \sdd{CSCSA-53897} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Core_Run__disable_ltb_algorithm)
{
   /** \arrange Set alert levels to "no alert" */
   ltb_core_input.f_ltb_enable                    = FBK_FALSE;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = NO_ALERT;

   /** \action Execute Ltb algorithm run function */
   Ltb_Core_Run(&ltb_core_output, &ltb_core_input, p_ltb_cals, &ltb_instance.persistent, &ltb_instance.ego_traj_predictor_instance);

   /** \assert Check that no alert is raised. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], NO_ALERT);
}

/**
 * Set up a scenario with an active alert. Reset LTB and verify that alerts are no longer set.
 * \uts{CSCSA-46177} \sdd{CSCSA-53895} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Reset__test_resetting_ltb_data)
{
   /** \arrange Set active alert levels */
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;

   /** \action Reset LTB */
   Ltb_Reset(&ltb_core_output, p_ltb_cals, p_ltb_persistent);

   /** \assert Check that no alert is raised. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], NO_ALERT);
}

/**
 * Tests the reset of persistent data of LTB.
 * \uts{CSCSA-46178} \sdd{CSCSA-53889} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Test, Ltb_Reset_Persistent_Data__check_for_defaults)
{
   /** \arrange Set non defaults to persistent */
   p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_2;
   p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT] = ALERT_ACTIVE_LEVEL_2;

   /** \action Execute function to test */
   Ltb_Reset_Persistent_Data(p_ltb_persistent, p_ltb_cals);

   /** \assert Check that persistent data is set to default. */
   EXPECT_EQ(p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_LEFT], NO_ALERT);
   EXPECT_EQ(p_ltb_persistent->ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT], NO_ALERT);
}