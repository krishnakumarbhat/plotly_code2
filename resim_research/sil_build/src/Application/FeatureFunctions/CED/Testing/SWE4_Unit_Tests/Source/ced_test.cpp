/**
 * @file ced_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ced.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41465}
 */

#include "ced_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced.c"
#include "ced_core_calibration.h"
#include "ced_instance.h"
#include "ced_persistent_t.h"
#include "ced_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "pa_const_macros.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
}

#ifndef NDEBUG
/**
 * Call core run function with invalid core input pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41466} \sdd{SF-3584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Core_Run__core_input_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when core run function is called with null pointer. */
   EXPECT_DEATH({ Ced_Core_Run(&ced_instance.core_output, &ced_instance.core_input, NULL, p_ced_cals, &fbk_output); }, ".*p_ced_"
                                                                                                                       "persistanc"
                                                                                                                       "e.*");
}

/**
 * Call core run function with invalid core output pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41467} \sdd{SF-3584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Core_Run__core_output_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when core run function is called with null pointer. */
   EXPECT_DEATH({ Ced_Core_Run(NULL, &ced_instance.core_input, &ced_instance.persistance, p_ced_cals, &fbk_output); }, ".*p_ced_"
                                                                                                                       "core_"
                                                                                                                       "output.*");
}

/**
 * Call core run function with invalid calibration pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41468} \sdd{SF-3584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Core_Run__cals_pointer_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when core run function is called with null pointer. */
   EXPECT_DEATH({ Ced_Core_Run(&ced_instance.core_output, &ced_instance.core_input, &ced_instance.persistance, NULL, &fbk_output); },
                ".*p_ced_cal.*");
}
#endif

/**
 * Checks if CED flag stays enabled, when core run function is called.
 * \uts{CSCSA-41469} \sdd{SF-3584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Core_Run__stays_enabled)
{
   /** \arrange Set f_ced_enable to true. */

   ced_instance.core_input.f_ced_enable = FBK_TRUE;

   /** \action Call function Ced_Core_Run with input parameters core_output, core_input and p_ced_cals */
   Ced_Core_Run(&ced_instance.core_output, &ced_instance.core_input, &ced_instance.persistance, p_ced_cals, &fbk_output);

   /** \assert Verify that f_ced_enable is still set to true. */
   EXPECT_TRUE(ced_instance.core_input.f_ced_enable);
}

/**
 * Checks if reset function is called internally, when CED is disabled.
 * \uts{CSCSA-41470} \sdd{SF-3584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Core_Run__resets_ced_output_if_not_enabled)
{
   /** \arrange Set f_ced_enable to false and some core output values to non-default values. */
   ced_instance.core_input.f_ced_enable               = FBK_FALSE;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_2;

   /** \action Call function Ced_Core_Run with input parameters core_output core_input and p_ced_cals */
   Ced_Core_Run(&ced_instance.core_output, &ced_instance.core_input, &ced_instance.persistance, p_ced_cals, &fbk_output);

   /** \assert Verify that f_ced_enable is still set to false and core output is reseted to default values. */
   EXPECT_FALSE(ced_instance.core_input.f_ced_enable);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/*
 * Checks if reset function is called internally, when speed is above upper limit.
 * \uts{CSCSA-41471} \sdd{SF-3584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Core_Run__resets_ced_output_speed_above_threshold)
{
   /** \arrange Set f_ced_enable to true and host speed above threshold. */
   ced_instance.core_input.f_ced_enable               = FBK_TRUE;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_2;
   p_vehicle_data->host_speed                         = p_ced_cals->k_ced_ego_abs_speed_max + EPSILON;

   /** \action Call function Ced_Core_Run with input parameters ced_instance.core_output, ced_instance.core_input and
    * &amp;amp;p_ced_cals-&amp;gt; */
   Ced_Core_Run(&ced_instance.core_output, &ced_instance.core_input, &ced_instance.persistance, p_ced_cals, &fbk_output);
   ;

   /** \assert Verify that f_ced_enable is set to false and warnings are deactivated. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/*
 * Checks if reset function is called internally, when speed is below lower limit.
 * \uts{CSCSA-41472} \sdd{SF-3584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Core_Run__resets_ced_output_speed_below_threshold)
{
   /** \arrange Set f_ced_enable to true and host speed to negative value above threshold. */
   ced_instance.core_input.f_ced_enable               = FBK_TRUE;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_2;
   p_vehicle_data->host_speed                         = -(p_ced_cals->k_ced_ego_abs_speed_max + EPSILON);

   /** \action Call function Ced_Core_Run with input parameters core_output, core_input and p_ced_cals */
   Ced_Core_Run(&ced_instance.core_output, &ced_instance.core_input, &ced_instance.persistance, p_ced_cals, &fbk_output);


   /** \assert Verify that f_ced_enable is set to false and warnings are deactivated. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

#ifndef NDEBUG
/**
 * Call reset function with invalid core output pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41473} \sdd{SF-3585} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset__p_ced_core_output_null_throws_exception)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when reset function is called with null pointer. */
   EXPECT_DEATH({ Ced_Reset(NULL, &ced_instance.persistance, fbk_output.p_index_id_lookup_table); }, ".*p_ced_core_output.*");
}
#endif

/**
 * Call reset function and verify that core output is reseted.
 * \uts{CSCSA-41474} \sdd{SF-3585} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset__works_properly)
{
   /** \arrange Set core output values to non-default values. */
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                         = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.persistance.ced_object_heading_predicted[5u]                 = 0.2f;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT]    = 7u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT] = 4u;

   /** \action Call reset function with core output as input parameter. */
   Ced_Reset(&ced_instance.core_output, &ced_instance.persistance, fbk_output.p_index_id_lookup_table);

   /** \assert Verify that core output is reseted to default values. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
   EXPECT_FLOAT_EQ(ced_instance.persistance.ced_object_heading_predicted[5u], FBK_ZERO_F);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Check if reset function for CED object resets all object properties.
 * \uts{CSCSA-41475} \sdd{SF-3573} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Ced_Object__works_properly)
{
   /** \arrange Set up a CED object with non-default values. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, -2.9f, 3.9f, 1.0f, 2.0f, 3.0f, 4.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(4u);

   /** \action Call reset object function with the created object as input parameter. */
   Ced_Reset_Ced_Object(&ced_object);

   /** \assert Verify that all CED object properties are reseted to default values. */
   EXPECT_EQ(ced_object.tracker_data.id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_object.tracker_data.index, PA_INVALID_OBJ_INDEX);
   EXPECT_FLOAT_EQ(ced_object.tracker_data.vcs_pos.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.tracker_data.vcs_pos.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.tracker_data.vcs_vel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.tracker_data.vcs_vel.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.tracker_data.vcs_vel_rel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.tracker_data.vcs_vel_rel.y, FBK_ZERO_F);
}

#ifndef NDEBUG
/**
 * Call reset ced object function with null pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41476} \sdd{SF-3573} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Ced_Object__object_pointer_null_throws_expection)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when Ced_Reset_Ced_Object is called with null pointer. */
   EXPECT_DEATH({ Ced_Reset_Ced_Object(NULL); }, ".*p_ced_object.*");
}
#endif

/**
 * Tests that an valid CED object is classified as valid.
 * \uts{CSCSA-41477} \sdd{SF-3571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Valid__is_TRUE_if_obj_valid)
{
   /** \arrange Set up a tracker object that fulfills all conditions to be a valid CED object. */
   boolean_T result;

   Ced_Fill_Raw_Tracker_Output(4u, -2.9f, 3.9f, 0.0f, 0.0f, 0.0f, 0.0f);

   /** \action Call function that checks if the object is valid for CED. */
   result = Ced_Is_Object_Valid(ced_instance.core_input.p_pa_data, 4u, &ced_instance.persistance);

   /** \assert Verify that the function returns true for the valid object. */
   EXPECT_TRUE(result);
}

/**
 * Tests that an CED object is classified as invalid if object status is NEW.
 * \uts{CSCSA-41478} \sdd{SF-3571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Valid__is_FALSE_if_obj_status_NEW)
{
   /** \arrange Set up a tracker object that fulfills all other conditions to be a valid CED object, and set the object status to
    * NEW. */
   boolean_T result;

   Ced_Fill_Raw_Tracker_Output(4u, -2.9f, 3.9f, 0.0f, 0.0f, 0.0f, 0.0f);
   object_data[4u].status = PA_OBJ_STATUS_NEW;

   /** \action Call function that checks if the object is valid for CED. */
   result = Ced_Is_Object_Valid(ced_instance.core_input.p_pa_data, 4u, &ced_instance.persistance);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests that an CED object is classified as invalid if object status is INVALID.
 * \uts{CSCSA-41479} \sdd{SF-3571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Valid__is_FALSE_if_obj_status_INVALID)
{
   /** \arrange Set up a tracker object that fulfills all other conditions to be a valid CED object, and set the object status to
    * INVALID. */

   boolean_T result;

   Ced_Fill_Raw_Tracker_Output(4u, -2.9f, 3.9f, 0.0f, 0.0f, 0.0f, 0.0f);
   object_data[4u].status = PA_OBJ_STATUS_INVALID;

   /** \action Call function that checks if the object is valid for CED. */
   result = Ced_Is_Object_Valid(ced_instance.core_input.p_pa_data, 4u, &ced_instance.persistance);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests that an CED object is classified as invalid if it is marked as a reflection and was not alerted in the previous cycle.
 * \uts{CSCSA-41480} \sdd{SF-3571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Valid__is_FALSE_if_obj_reflection_and_not_alerted_prev_cycle)
{
   /** \arrange Set up a tracker object that fulfills all other conditions to be a valid CED object, and mark object as a
    * reflection without alert in the previous cycle. */

   boolean_T result;

   Ced_Fill_Raw_Tracker_Output(4u, -2.9f, 3.9f, 0.0f, 0.0f, 0.0f, 0.0f);
   object_data[4u].f_reflection = FBK_TRUE;

   /** \action Call function that checks if the object is valid for CED. */
   result = Ced_Is_Object_Valid(ced_instance.core_input.p_pa_data, 4u, &ced_instance.persistance);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests that an CED object is classified as valid if it is marked as a reflection but was alerted in the previous cycle.
 * \uts{CSCSA-41481} \sdd{SF-3571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Valid__is_TRUE_if_obj_reflection_but_was_alerted_prev_cycle)
{
   /** \arrange Set up a tracker object that fulfills all conditions to be a valid CED object and mark object as a reflection with
    * alert in the previous cycle. */

   boolean_T result;
   uint8_t obj_index = 4u;

   Ced_Fill_Raw_Tracker_Output(obj_index, -2.9f, 3.9f, 0.0f, 0.0f, 0.0f, 0.0f);
   object_data[obj_index].f_reflection                               = FBK_TRUE;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT] = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]    = object_data[obj_index].id;

   /** \action Call function that checks if the object is valid for CED. */
   result = Ced_Is_Object_Valid(ced_instance.core_input.p_pa_data, 4u, &ced_instance.persistance);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}

/**
 * Tests if CED object attributes are filled correctly when PT is available
 * \uts{CSCSA-90085} \sdd{CSCSA-90083} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Update_Object_Data__fill_ced_object_attributes_correctly)
{
   /** \arrange Set up ced object and PT info. */
   Ced_Object_T ced_object{};
   p_ced_cals->k_ced_f_path_tracking_enable = FBK_TRUE;

   /** \action Call function that fills CED object attributes with PT. */
   Ced_Update_Object_Data(&ced_object, &ced_instance.core_input, p_ced_cals);

   /** \assert Verify that the function returns expected values for the object. */
   EXPECT_EQ(&ced_object.attributes.p_pt_match_info[0u], &ced_instance.core_input.p_pt_output->path_obj_pair_output[0u]);
   EXPECT_EQ(&ced_object.attributes.p_pt_nearest_path_info[0u], &ced_instance.core_input.p_pt_output->nearest_path_output[0u]);
}

/**
 * Tests if CED object attributes are not filled when PT isn't available
 * \uts{CSCSA-278638} \sdd{CSCSA-90083} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Update_Object_Data__ced_object_attributes_not_filled_due_to_PT_disabled)
{
   /** \arrange Set up ced object and PT info. */
   Ced_Object_T ced_object{};
   p_ced_cals->k_ced_f_path_tracking_enable = FBK_FALSE;

   /** \action Call function that fills CED object attributes with PT. */
   Ced_Update_Object_Data(&ced_object, &ced_instance.core_input, p_ced_cals);

   /** \assert Verify that the function returns expected values for the object. */
   EXPECT_EQ(ced_object.attributes.p_pt_match_info, nullptr);
   EXPECT_EQ(ced_object.attributes.p_pt_nearest_path_info, nullptr);
}

/**
 * Tests if CED object attributes are filled correctly when PT is not available
 * \uts{CSCSA-90086} \sdd{CSCSA-90083} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Update_Object_Data__fill_ced_object_attributes_correctly_no_PT_available)
{
   /** \arrange Set up ced object and PT info. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = NULL;
   Pt_Nearest_Path_T *pt_nearest_path_info     = NULL;
   ced_instance.core_input.p_pt_output         = NULL;
   p_ced_cals->k_ced_f_path_tracking_enable    = FBK_TRUE;
   Ced_Reset_Ced_Object(&ced_object);

   /** \action Call function that fills CED object attributes with PT. */
   Ced_Update_Object_Data(&ced_object, &ced_instance.core_input, p_ced_cals);

   /** \assert Verify that the function returns the expected values for the object. */
   EXPECT_EQ(ced_object.attributes.p_pt_match_info, pt_match_info);
   EXPECT_EQ(ced_object.attributes.p_pt_nearest_path_info, pt_nearest_path_info);
}

/**
 * Tests that an CED object approaching from ego rear is classified as relevant if all conditions are met.
 * \uts{CSCSA-41482} \sdd{SF-3570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Relevant__is_TRUE_for_relevant_target_approaching_from_ego_rear)
{
   /** \arrange Set up a CED object approaching from ego rear that fulfills all conditions to be relevant. */
   boolean_T result;
   Ced_Object_T ced_object{};

   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -59.0f, 19.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(4u);

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function that checks if the object is relevant for CED. */
   result = Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, &ced_instance.core_input, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}

/**
 * Tests that an CED object approaching from ego rear is not classified as relevant if velocity is too low.
 * \uts{CSCSA-41483} \sdd{SF-3570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Relevant__is_FALSE_for_target_approaching_from_ego_rear_if_velocity_too_low)
{
   /** \arrange Set up a CED object approaching from ego rear that fulfills all conditions, except for velocity being too low. */
   boolean_T result;
   Ced_Object_T ced_object{};

   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(5u, -59.0f, 19.0f, long_vel / 2.0f, lat_vel, long_vel_rel, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(5u);

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function that checks if the object is relevant for CED. */
   result = Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, &ced_instance.core_input, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests that an CED object approaching from ego front is not classified as relevant if front CED is disabled.
 * \uts{CSCSA-41484} \sdd{SF-3570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Relevant__is_FALSE_for_deactivated_front_ced)
{
   /** \arrange Set up a CED object approaching from ego front that fulfills all conditions and disable front CED. */
   boolean_T result;
   Ced_Object_T ced_object{};

   float32_T long_vel     = -1.0f * p_ced_cals->k_ced_object_ftm_long_vel_min;
   float32_T long_vel_rel = -1.0f * p_ced_cals->k_ced_object_ftm_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_ftm_lat_vel_max;

   ced_instance.core_input.f_ced_front_mode = FBK_FALSE;

   Ced_Fill_Raw_Tracker_Output(4u, 59.0f, 19.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(4u);

   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min;
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function that checks if the object is relevant for CED. */
   result = Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, &ced_instance.core_input, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests that an CED object approaching from ego front is classified as relevant if all conditions are met.
 * \uts{CSCSA-41485} \sdd{SF-3570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Relevant__is_TRUE_for_relevant_target_approaching_from_ego_front)
{
   /** \arrange Set up a CED object approaching from ego front that fulfills all conditions to be relevant. */
   boolean_T result;
   Ced_Object_T ced_object{};

   float32_T long_vel     = -1.0f * p_ced_cals->k_ced_object_ftm_long_vel_min;
   float32_T long_vel_rel = -1.0f * p_ced_cals->k_ced_object_ftm_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_ftm_lat_vel_max;

   ced_instance.core_input.f_ced_front_mode = FBK_TRUE;

   Ced_Fill_Raw_Tracker_Output(4u, 59.0f, 19.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(4u);

   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min;
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function that checks if the object is relevant for CED. */
   result = Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, &ced_instance.core_input, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}

/**
 * Tests that an CED object approaching from ego front is not classified as relevant if velocity is too low.
 * \uts{CSCSA-41486} \sdd{SF-3570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Relevant__is_FALSE_for_target_approaching_from_ego_front_if_velocity_too_low)
{
   /** \arrange Set up a CED object approaching from ego front that fulfills all conditions, except for velocity being too low. */
   boolean_T result;
   Ced_Object_T ced_object{};

   float32_T long_vel     = -1.0f * p_ced_cals->k_ced_object_ftm_long_vel_min;
   float32_T long_vel_rel = -1.0f * p_ced_cals->k_ced_object_ftm_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_ftm_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(5u, 59.0f, 19.0f, long_vel / 2.0f, lat_vel, long_vel_rel, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(5u);

   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min;
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function that checks if the object is relevant for CED. */
   result = Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, &ced_instance.core_input, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests that object inside funnel zone is correctly classified if crash line is defined at front bumper.
 * \uts{CSCSA-41487} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_TRUE_if_obj_inside_FunnelZone_with_crash_line_defined_at_front_bumper)
{
   /** \arrange Set up CED object inside funnel zone and set crash line to front bumper. */
   boolean_T result_front_corner_right;
   boolean_T result_rear_corner_right;
   Ced_Object_T ced_object_front_corner_right;
   Ced_Object_T ced_object_rear_corner_right;

   Ced_Fill_Raw_Tracker_Output(4u, 59.0f, 19.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   Ced_Fill_Raw_Tracker_Output(5u, 0.5f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object_front_corner_right = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_rear_corner_right  = Ced_Create_Object_From_Tracker_Output(5u);

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function to evaluate if object is in funnel zone. */
   result_front_corner_right =
      Ced_Is_Object_In_Funnel_Zone(&ced_object_front_corner_right, &ced_funnel_zone, p_vehicle_data, p_ced_cals);
   result_rear_corner_right =
      Ced_Is_Object_In_Funnel_Zone(&ced_object_rear_corner_right, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result_front_corner_right);
   EXPECT_TRUE(result_rear_corner_right);
}


/**
 * Tests that object inside funnel zone is correctly classified if refference point i set at the nearest corner of the object.
 * \uts{CSCSA-185584} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_TRUE_if_obj_inside_FunnelZone_with_ref_point_defined_at_nearest_corner_left)
{
   /** \arrange Set up CED object so that middle point is outside of the funnel zone and a front bumper of the object is in a
    * funnel zone. */
   boolean_T result;
   Ced_Object_T ced_object;


   Ced_Fill_Raw_Tracker_Output(5u, -10.0f, -1.5f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object                      = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object.tracker_data.width   = 0.5f;
   ced_object.tracker_data.length  = 1.5f;
   ced_object.attributes.direction = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 2;

   /** \action Call function to evaluate if object is in funnel zone. */
   result = Ced_Is_Object_In_Funnel_Zone(&ced_object, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}


/**
 * Tests that object inside funnel zone is correctly classified if refference point is set at the nearest corner of the object,
 * case when one side crosses ego center vertical line. \uts{CSCSA-250230} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_TRUE_if_obj_inside_FunnelZone_ref_point_at_nearest_corner_left_cross_ego_lane)
{
   /** \arrange Set up CED object so that ref point is inside the funnel zone and target crosses ego center vertical line with its
    * front right corner. */
   boolean_T result;
   Ced_Object_T ced_object;


   Ced_Fill_Raw_Tracker_Output(5u, -10.0f, -1.5f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object                      = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object.tracker_data.width   = 4.0f;
   ced_object.tracker_data.length  = 1.5f;
   ced_object.attributes.direction = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 2;

   /** \action Call function to evaluate if object is in funnel zone. */
   result = Ced_Is_Object_In_Funnel_Zone(&ced_object, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}

/**
 * Tests that object inside funnel zone is correctly classified if refference point i set at the nearest corner of the object.
 * \uts{CSCSA-185585} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_TRUE_if_obj_inside_FunnelZone_with_ref_point_defined_at_nearest_corner_right)
{
   /** \arrange Set up CED object so that middle point is outside of the funnel zone and a front bumper of the object is in a
    * funnel zone. */
   boolean_T result;
   Ced_Object_T ced_object;


   Ced_Fill_Raw_Tracker_Output(5u, -10.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object                      = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object.tracker_data.width   = 0.5f;
   ced_object.tracker_data.length  = 1.5f;
   ced_object.attributes.direction = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 2;

   /** \action Call function to evaluate if object is in funnel zone. */
   result = Ced_Is_Object_In_Funnel_Zone(&ced_object, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}


/**
 * Tests that object inside funnel zone is correctly classified if refference point is set at the nearest corner of the object,
 * case when one side crosses ego center vertical line. \uts{CSCSA-250231} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_TRUE_if_obj_inside_FunnelZone_ref_point_at_nearest_corner_right_cross_ego_lane)
{
   /** \arrange Set up CED object so that ref point is inside the funnel zone and target crosses ego center vertical line with its
    * front left corner. */
   boolean_T result;
   Ced_Object_T ced_object;


   Ced_Fill_Raw_Tracker_Output(5u, -10.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object                      = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object.tracker_data.width   = 4.0f;
   ced_object.tracker_data.length  = 1.5f;
   ced_object.attributes.direction = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 2;

   /** \action Call function to evaluate if object is in funnel zone. */
   result = Ced_Is_Object_In_Funnel_Zone(&ced_object, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}


/**
 * Tests that object approaching from rear left and inside funnel zone is correctly classified if refference point is set at the
 * object front bumper. \uts{CSCSA-278635} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__ref_point_front_bump_approach_rear_left_TRUE)
{
   /** \arrange Set up CED object so that ref point is inside the funnel zone. */
   boolean_T result;
   Ced_Object_T ced_object;


   Ced_Fill_Raw_Tracker_Output(5u, -6.0f, -2.5f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object                      = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object.tracker_data.width   = 2.0f;
   ced_object.tracker_data.length  = 3.0f;
   ced_object.attributes.direction = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 1;

   /** \action Call function to evaluate if object is in funnel zone. */
   result = Ced_Is_Object_In_Funnel_Zone(&ced_object, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}


/**
 * Tests that object approaching from rear right and inside funnel zone is correctly classified if refference point is set at the
 * object front bumper. \uts{CSCSA-278636} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__ref_point_front_bump_approach_rear_right_TRUE)
{
   /** \arrange Set up CED object so that ref point is inside the funnel zone. */
   boolean_T result;
   Ced_Object_T ced_object;


   Ced_Fill_Raw_Tracker_Output(5u, -6.0f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object                      = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object.tracker_data.width   = 2.0f;
   ced_object.tracker_data.length  = 3.0f;
   ced_object.attributes.direction = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 1;

   /** \action Call function to evaluate if object is in funnel zone. */
   result = Ced_Is_Object_In_Funnel_Zone(&ced_object, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result);
}


/**
 * Tests that object outside funnel zone is correctly classified if crash line is defined at front bumper.
 * \uts{CSCSA-41489} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_FALSE_if_obj_outside_FunnelZone_with_crash_line_defined_at_front_bumper)
{
   /** \arrange Set up CED object outside funnel zone and set crash line to front bumper. */
   boolean_T result_in_front_of_zone;
   boolean_T result_beside_zone;
   Ced_Object_T ced_object_in_front_of_zone;
   Ced_Object_T ced_object_beside_zone;

   Ced_Fill_Raw_Tracker_Output(4u, 61.0f, 19.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   Ced_Fill_Raw_Tracker_Output(5u, 1.0f, 4.5f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object_in_front_of_zone = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_beside_zone      = Ced_Create_Object_From_Tracker_Output(5u);

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function to evaluate if object is in funnel zone. */
   result_in_front_of_zone = Ced_Is_Object_In_Funnel_Zone(&ced_object_in_front_of_zone, &ced_funnel_zone, p_vehicle_data, p_ced_cals);
   result_beside_zone      = Ced_Is_Object_In_Funnel_Zone(&ced_object_beside_zone, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result_in_front_of_zone);
   EXPECT_FALSE(result_beside_zone);
}

/**
 * Tests that object inside funnel zone is correctly classified if crash line is defined at rear bumper.
 * \uts{CSCSA-41490} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_TRUE_if_obj_inside_FunnelZone_with_crash_line_defined_at_rear_bumper)
{
   /** \arrange Set up CED object inside funnel zone and set crash line to rear bumper. */
   boolean_T result_rear_corner_right;
   boolean_T result_front_corner_right;
   Ced_Object_T ced_object_rear_corner_right;
   Ced_Object_T ced_object_front_corner_right;

   Ced_Fill_Raw_Tracker_Output(4u, -1.0f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f);
   Ced_Fill_Raw_Tracker_Output(5u, -55.0f, 17.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object_rear_corner_right                       = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_front_corner_right                      = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object_rear_corner_right.attributes.direction  = FBK_SIDE_FRONT;
   ced_object_front_corner_right.attributes.direction = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 1.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 1.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function to evaluate if object is in funnel zone. */
   result_rear_corner_right =
      Ced_Is_Object_In_Funnel_Zone(&ced_object_rear_corner_right, &ced_funnel_zone, p_vehicle_data, p_ced_cals);
   result_front_corner_right =
      Ced_Is_Object_In_Funnel_Zone(&ced_object_front_corner_right, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result_rear_corner_right);
   EXPECT_TRUE(result_front_corner_right);
}

/**
 * Tests that object outside funnel zone is correctly classified if crash line is defined at rear bumper.
 * \uts{CSCSA-41491} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_FALSE_if_obj_outside_FunnelZone_with_crash_line_defined_at_rear_bumper)
{
   /** \arrange Set up CED object outside funnel zone and set crash line to rear bumper. */
   boolean_T result_beside_zone;
   boolean_T result_in_front_of_zone;
   Ced_Object_T ced_object_beside_zone;
   Ced_Object_T ced_object_in_front_of_zone;

   Ced_Fill_Raw_Tracker_Output(4u, -1.0f, 20.2f, 0.0f, 0.0f, 0.0f, 0.0f);
   Ced_Fill_Raw_Tracker_Output(5u, 60.5f, 1.4f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object_beside_zone      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_in_front_of_zone = Ced_Create_Object_From_Tracker_Output(5u);

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 1.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 1.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function to evaluate if object is in funnel zone. */
   result_beside_zone      = Ced_Is_Object_In_Funnel_Zone(&ced_object_beside_zone, &ced_funnel_zone, p_vehicle_data, p_ced_cals);
   result_in_front_of_zone = Ced_Is_Object_In_Funnel_Zone(&ced_object_in_front_of_zone, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result_beside_zone);
   EXPECT_FALSE(result_in_front_of_zone);
}

/**
 * Tests that object outside funnel zone is correctly classified if crash line is defined at front bumper and y position is below
 * halfwidth. \uts{CSCSA-41492} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__is_FALSE_if_obj_outside_FunnelZone_with_y_below_halfwidth)
{
   /** \arrange Set up CED object outside funnel zone and set crash line to front bumper. */
   boolean_T result_in_front_of_zone;
   Ced_Object_T ced_object_in_front_of_zone;

   Ced_Fill_Raw_Tracker_Output(4u, 61.0f, 0.9f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object_in_front_of_zone = Ced_Create_Object_From_Tracker_Output(4u);

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call function to evaluate if object is in funnel zone. */
   result_in_front_of_zone = Ced_Is_Object_In_Funnel_Zone(&ced_object_in_front_of_zone, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result_in_front_of_zone);
}


/**
 * Tests that object inside funnel zone is correctly classified if refference point i set at nearest corner and object in quarter
 * 4th. \uts{CSCSA-246710} \sdd{SF-3568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Funnel_Zone__verify_left_side)
{
   /** \arrange Set up CED object so that middle point is outside of the funnel zone and a front bumper of the object is in a
    * funnel zone. */
   boolean_T result;
   Ced_Object_T ced_object;


   Ced_Fill_Raw_Tracker_Output(5u, 10.0f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object                        = Ced_Create_Object_From_Tracker_Output(5u);
   ced_object.tracker_data.width     = 0.5f;
   ced_object.tracker_data.length    = 1.5f;
   ced_object.tracker_data.obj_class = PA_OBJ_CLASS_CAR;
   ced_object.attributes.direction   = FBK_SIDE_REAR;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 2;
   p_ced_cals->k_ced_lat_pos_shift_enable                              = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_max_shift                                 = -10.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[FBK_ZERO_UINT]  = -2.0;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[FBK_ONE_UINT]   = -1.0;

   /** \action Call function to evaluate if object is in funnel zone. */
   result = Ced_Is_Object_In_Funnel_Zone(&ced_object, &ced_funnel_zone, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_FALSE(result);
}


/**
 * Tests that predicted position is calculated with path tracking information if object is matched to path and path tracking is
 * enabled. \uts{CSCSA-41493} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_predicted_values_when_used_with_path_tracking)
{
   /** \arrange Enable the usage of path tracking. Set up CED object and path tracking output for that object. */
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];

   Ced_Fill_Raw_Tracker_Output(object_index, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(object_index);
   ced_object.tracker_data.vcs_heading = 0.1f;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths                           = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_min_dist_to_crash_line_for_path_match      = 0.0f;

   pt_match_info->track_match                = 5u;
   pt_match_info->track_match_age            = 0u;
   pt_match_info->path_heading               = 0.1f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 2.0f;
   pt_match_info->range_at_zero              = 1.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;
   ced_object.attributes.direction           = FBK_SIDE_REAR;

   p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match    = 1.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted object position is calculated with path tracking output. */
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.x,
                   -p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] * p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(ced_object.attributes.heading_predicted, 0.1f);
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.y, (1.0f + 2.0f) / 2.0f);
   EXPECT_FLOAT_EQ(ced_object.attributes.length_predicted, ced_object.tracker_data.length);
   EXPECT_GT(ced_object.attributes.width_predicted, ced_object.tracker_data.width);
   EXPECT_LT(ced_object.attributes.width_predicted,
             ced_object.tracker_data.width * (FBK_ONE_F + p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match));
}

/**
 * Tests that objects with a close distance to the crash line are not predicted using path tracking information.
 * \uts{CSCSA-41494} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__dont_use_path_matching_info_for_close_objects)
{
   /** \arrange Enable the usage of path tracking. Set up CED object and path tracking output for that object. */
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];

   Ced_Fill_Raw_Tracker_Output(object_index, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(object_index);
   ced_object.tracker_data.vcs_heading = 0.0f;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_TRUE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_min_dist_to_crash_line_for_path_match      = 10.0f;

   pt_match_info->track_match                = 4u;
   pt_match_info->track_match_age            = 0u;
   pt_match_info->path_heading               = 0.1f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 2.0f;
   pt_match_info->range_at_zero              = 1.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted object position is calculated without path information. */
   EXPECT_EQ(ced_object.attributes.p_pt_match_info->track_match, 4u);
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.y, 1.0f);
}

/**
 * Tests that the predicted position takes the range to the path into account if an object is closer to ego than the matched path
 * and path tracking is enabled. \uts{CSCSA-41495} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__takes_range_to_path_into_account_when_used_with_path_tracking)
{
   /** \arrange Enable the usage of path tracking. Set up CED object and path tracking output for that object, such that object is
    * closer to ego than the matched path. */
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];

   Ced_Fill_Raw_Tracker_Output(object_index, -10.0f, 2.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(object_index);
   ced_object.tracker_data.vcs_heading = 0.1f;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths                           = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_min_dist_to_crash_line_for_path_match      = 0.0f;

   pt_match_info->track_match                = 4u;
   pt_match_info->track_match_age            = 0u;
   pt_match_info->path_heading               = -0.2f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.5f;
   pt_match_info->range_at_host_edge         = -5.0f;
   pt_match_info->range_at_zero              = -3.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;
   ced_object.attributes.direction           = FBK_SIDE_REAR;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted object position is calculated with path tracking output including range to path. */
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.x,
                   -p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] * p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(ced_object.attributes.heading_predicted, pt_match_info->path_heading);
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.y,
                   (-3.0f + -5.0f) / 2.0f + (0.5f * p_ced_cals->k_ced_offset_to_path_weight));
   EXPECT_FLOAT_EQ(ced_object.attributes.length_predicted, ced_object.tracker_data.length);
   EXPECT_FLOAT_EQ(ced_object.attributes.width_predicted, ced_object.tracker_data.width);
}

/**
 * Tests that predicted position is calculated without path tracking information if path tracking is disabled.
 * \uts{CSCSA-41496} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_predicted_values_when_used_without_path_tracking)
{
   /** \arrange Disable the usage of path tracking and set up CED object. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading = 0.1f;

   p_ced_cals->k_ced_f_path_tracking_enable                              = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]   = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]    = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match = 1.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match    = 0.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted object position is calculated with filtered heading. */
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.x, FBK_ZERO_F);
   EXPECT_GE(ced_object.attributes.heading_predicted, FBK_ZERO_F);
   EXPECT_GE(ced_object.attributes.position_predicted.y, ced_object.tracker_data.vcs_pos.y);
   EXPECT_GE(ced_object.attributes.length_predicted, ced_object.tracker_data.length);
   EXPECT_GT(ced_object.attributes.width_predicted, ced_object.tracker_data.width);
   EXPECT_LT(ced_object.attributes.width_predicted,
             ced_object.tracker_data.width * (FBK_ONE_F + p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match));
}

/**
 * Tests that predicted position is calculated without path tracking information, relatvie velocity is zero.
 * \uts{CSCSA-186372} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__object_rel_vel_x_equal_zero)
{
   /** \arrange Disable the usage of path tracking and set up CED object. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, -10.0f, 1.0f, 2.0f, 0.0f, 0.0f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading = 0.1f;

   p_ced_cals->k_ced_f_path_tracking_enable                              = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]   = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]    = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match = 1.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match    = 0.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted object position is calculated with filtered heading. */
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.x, FBK_ZERO_F);
   EXPECT_GE(ced_object.attributes.time_to_crash_line, FBK_ZERO_F);
}


/**
 * Tests that predicted position is calculated without path tracking information and without heading filter if both are disabled.
 * \uts{CSCSA-41497} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_predicted_values_when_used_without_path_tracking_and_const_heading)
{
   /** \arrange Disable the usage of path tracking and of the heading filter. Set up CED object. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading = 0.1f;
   ced_object.attributes.direction     = FBK_SIDE_REAR;

   p_ced_cals->k_ced_f_path_tracking_enable                              = FBK_FALSE;
   p_ced_cals->k_ced_f_enable_heading_exp_moving_average                 = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]   = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]    = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match = 1.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match    = 0.0f;
   p_ced_cals->k_ced_f_adapt_heading_ego_lane                            = 0u;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted object position is calculated with current heading. */
   EXPECT_FLOAT_EQ(ced_object.attributes.heading_predicted,
                   ced_object.tracker_data.vcs_heading * p_ced_cals->k_ced_object_heading_predicted_weight);
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.x, FBK_ZERO_F);
   EXPECT_GE(ced_object.attributes.position_predicted.y, ced_object.tracker_data.vcs_pos.y);
   EXPECT_GE(ced_object.attributes.length_predicted, ced_object.tracker_data.length);
   EXPECT_GT(ced_object.attributes.width_predicted, ced_object.tracker_data.width);
   EXPECT_LT(ced_object.attributes.width_predicted,
             ced_object.tracker_data.width * (FBK_ONE_F + p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match));
}

/**
 * Tests that the predicted heading is weighted with the corresponding calibration values when used without path tracking.
 * \uts{CSCSA-41498} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__weights_predicted_heading_when_used_without_path_tracking_and_const_heading)
{
   /** \arrange Disable the usage of path tracking and of the heading filter. Set up CED object. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading = 0.5f;
   ced_object.attributes.direction     = FBK_SIDE_REAR;

   p_ced_cals->k_ced_f_path_tracking_enable              = FBK_FALSE;
   p_ced_cals->k_ced_f_enable_heading_exp_moving_average = FBK_FALSE;
   p_ced_cals->k_ced_f_enable_heading_exp_moving_average = FBK_FALSE;
   p_ced_cals->k_ced_f_adapt_heading_ego_lane            = FBK_FALSE;
   p_ced_cals->k_ced_object_heading_predicted_weight     = 0.1f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted heading is calculated with current heading and weight from calibration values. */
   EXPECT_FLOAT_EQ(ced_object.attributes.heading_predicted,
                   ced_object.tracker_data.vcs_heading * p_ced_cals->k_ced_object_heading_predicted_weight);
}

/**
 * Tests that time to crash line is calculated for an object approaching from the rear.
 * \uts{CSCSA-41499} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_time_to_crash_line_for_obj_from_rear)
{
   /** \arrange Set up CED object that is approaching ego from behind, set crash line to the middle of the host. */
   Ced_Object_T ced_object{};
   float32_T obj_long_pos = -p_vehicle_data->host_length - EPSILON;
   Ced_Fill_Raw_Tracker_Output(4u, obj_long_pos, 1.0f, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                               = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading      = 0.1f;
   ced_object.attributes.time_to_crash_line = CED_INVALID_TIME;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_acceleration_weight                        = 0.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that time to crash line is set to non-default value. */
   EXPECT_NE(ced_object.attributes.time_to_crash_line, CED_INVALID_TIME);
   EXPECT_GT(ced_object.attributes.time_to_crash_line, FBK_ZERO_F);
   EXPECT_NE(ced_object.attributes.time_to_pass_crash_line, CED_INVALID_TIME);
   EXPECT_GT(ced_object.attributes.time_to_pass_crash_line, FBK_ZERO_F);
}

/**
 * Tests that time to crash line is calculated for an object approaching from the rear already past the crash line.
 * \uts{CSCSA-41500} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_time_to_crash_line_for_obj_from_rear_past_crash_line)
{
   /** \arrange Set up CED object that is approaching ego from behind, set crash line to the middle of the vehicle. */
   Ced_Object_T ced_object{};
   float32_T obj_long_pos = FBK_ZERO_F + EPSILON;
   Ced_Fill_Raw_Tracker_Output(4u, obj_long_pos, 1.0f, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                               = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading      = 0.1f;
   ced_object.attributes.time_to_crash_line = CED_INVALID_TIME;
   ced_object.attributes.direction          = FBK_SIDE_REAR;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_acceleration_weight                        = 0.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that time to crash line is set to non-default value. */
   EXPECT_NE(ced_object.attributes.time_to_crash_line, CED_INVALID_TIME);
   EXPECT_LT(ced_object.attributes.time_to_crash_line, FBK_ZERO_F);
   EXPECT_NE(ced_object.attributes.time_to_pass_crash_line, CED_INVALID_TIME);
   EXPECT_LT(ced_object.attributes.time_to_pass_crash_line, FBK_ZERO_F);
}

/**
 * Compare the time to crash line for an object with and without acceleration. The object is approaching (and accelerating) from
 * the rear. \uts{CSCSA-41501} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_time_to_crash_line_for_accelerating_obj_from_rear)
{
   /** \arrange Set up CED object that is approaching ego from behind, set crash line to the front of the ego. */
   Ced_Object_T ced_object{};
   Ced_Object_T ced_object_accel{};

   Ced_Fill_Raw_Tracker_Output(4u, -10.0f, 1.0f, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                               = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_accel.x      = 0.0f;
   ced_object.tracker_data.vcs_heading      = 0.1f;
   ced_object.attributes.time_to_crash_line = CED_INVALID_TIME;

   ced_object_accel                          = ced_object;
   ced_object_accel.tracker_data.vcs_accel.x = 1.0f;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_object_acceleration_weight                        = 1.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object_accel, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that time to crash line is lower when considering acceleration. */
   EXPECT_LT(ced_object_accel.attributes.time_to_crash_line, ced_object.attributes.time_to_crash_line);
   EXPECT_LT(ced_object_accel.attributes.time_to_pass_crash_line, ced_object.attributes.time_to_pass_crash_line);
}

/**
 * Tests that time to crash line is calculated for an object approaching from the front.
 * \uts{CSCSA-41502} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_time_to_crash_line_for_obj_from_front)
{
   /** \arrange Set up CED object that is approaching ego from the front, set crashline to the middle of the ego. */
   Ced_Object_T ced_object{};
   float32_T obj_long_pos = FBK_ZERO_F + EPSILON;
   Ced_Fill_Raw_Tracker_Output(4u, obj_long_pos, 1.0f, -2.0f, 0.0f, -2.0f, 0.0f);
   ced_object                               = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading      = PI - 0.1f;
   ced_object.attributes.time_to_crash_line = CED_INVALID_TIME;
   ced_object.attributes.direction          = FBK_SIDE_FRONT;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_acceleration_weight                        = 0.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that time to crash line is set to non-default value. */
   EXPECT_NE(ced_object.attributes.time_to_crash_line, CED_INVALID_TIME);
   EXPECT_GT(ced_object.attributes.time_to_crash_line, FBK_ZERO_F);
   EXPECT_NE(ced_object.attributes.time_to_pass_crash_line, CED_INVALID_TIME);
   EXPECT_GT(ced_object.attributes.time_to_pass_crash_line, FBK_ZERO_F);
}

/**
 * Tests that time to crash line is calculated for an object approaching from the front already past the crash line.
 * \uts{CSCSA-41503} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_time_to_crash_line_for_obj_from_front_past_crash_line)
{
   /** \arrange Set up CED object that is approaching ego from the front, set crashline to the middle of the ego. */
   Ced_Object_T ced_object{};
   float32_T obj_long_pos = -p_vehicle_data->host_length - EPSILON;
   Ced_Fill_Raw_Tracker_Output(4u, obj_long_pos, 1.0f, -2.0f, 0.0f, -2.0f, 0.0f);
   ced_object                               = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading      = PI - 0.1f;
   ced_object.attributes.time_to_crash_line = CED_INVALID_TIME;
   ced_object.attributes.direction          = FBK_SIDE_FRONT;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_acceleration_weight                        = 0.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that time to crash line is set to non-default value. */
   EXPECT_NE(ced_object.attributes.time_to_crash_line, CED_INVALID_TIME);
   EXPECT_LT(ced_object.attributes.time_to_crash_line, FBK_ZERO_F);
   EXPECT_NE(ced_object.attributes.time_to_pass_crash_line, CED_INVALID_TIME);
   EXPECT_LT(ced_object.attributes.time_to_pass_crash_line, FBK_ZERO_F);
}


/**
 * Compare the time to crash line for an object with and without acceleration. The object is approaching (and accelerating) from
 * the front. \uts{CSCSA-41504} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__fills_time_to_crash_line_for_accelerating_obj_from_front)
{
   /** \arrange Set up CED object that is approaching ego from the front, set crashline to the front of the ego. */
   Ced_Object_T ced_object{};
   Ced_Object_T ced_object_accel{};

   Ced_Fill_Raw_Tracker_Output(4u, 10.0f, 1.0f, -2.0f, 0.0f, -2.0f, 0.0f);
   ced_object                               = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_accel.x      = 0.0f;
   ced_object.tracker_data.vcs_heading      = PI - 0.1f;
   ced_object.attributes.time_to_crash_line = CED_INVALID_TIME;
   ced_object.attributes.direction          = FBK_SIDE_FRONT;

   ced_object_accel                          = ced_object;
   ced_object_accel.tracker_data.vcs_accel.x = -1.0f;

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_object_acceleration_weight                        = 1.0f;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object_accel, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that time to crash line is lower when considering acceleration. */
   EXPECT_LT(ced_object_accel.attributes.time_to_crash_line, ced_object.attributes.time_to_crash_line);
   EXPECT_LT(ced_object_accel.attributes.time_to_pass_crash_line, ced_object.attributes.time_to_pass_crash_line);
}

/**
 * Tests that predicted object width is increased by safety margin for critical objects if object was critical.
 * \uts{CSCSA-41505} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__increases_object_width_for_prev_critical_object)
{
   /** \arrange Set up CED object that is approaching ego from behind and was critical in last cycle. */
   Ced_Object_T ced_object{};
   uint8_t obj_index = 4u;

   Ced_Fill_Raw_Tracker_Output(4u, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(obj_index);

   p_ced_cals->k_ced_f_path_tracking_enable                                      = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]           = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]            = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match            = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match         = 0.0f;
   p_ced_cals->k_ced_object_width_safety_margin_for_active_alert                 = 0.3f;
   p_ced_cals->k_ced_object_predicted_max_width_slope_reduce_factor              = 1.0f;
   p_ced_cals->k_ced_object_predicted_max_width_slope_offset                     = 0.0f;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]                = PA_INVALID_OBJ_ID;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]               = object_data[obj_index].id;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = PT_DEFAULT_MATCH_INDEX;


   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted width is increased by calibration value defined factor. */
   EXPECT_EQ(ced_object.attributes.width_predicted,
             ced_object.tracker_data.width + p_ced_cals->k_ced_object_width_safety_margin_for_active_alert);
}

/**
 * Tests that predicted object width is increased by safety margin for critical path match if object is matched to the same path as
 * an object with active alert. \uts{CSCSA-41506} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__increases_object_width_for_critical_path_match)
{
   /** \arrange Set up CED object that is approaching ego from behind and has same path match as an object that was critical in
    * last cycle. */
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];

   Ced_Fill_Raw_Tracker_Output(object_index, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(object_index);

   p_ced_cals->k_ced_f_path_tracking_enable                                      = FBK_TRUE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]           = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]            = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match            = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match         = 0.0f;
   p_ced_cals->k_ced_object_width_safety_margin_for_critical_path_match          = 0.3f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]               = 25u;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = 6u;
   pt_match_info->track_match                                                    = 6u;
   pt_match_info->track_match_age                                                = 0u;
   pt_match_info->path_heading                                                   = 0.0f;
   pt_match_info->path_direction                                                 = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part                                     = 0.0f;
   pt_match_info->range_at_host_edge                                             = 0.0f;
   pt_match_info->range_at_zero                                                  = 0.0f;
   ced_object.attributes.p_pt_match_info                                         = pt_match_info;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted width is increased by calibration value defined factor. */
   EXPECT_EQ(ced_object.attributes.width_predicted,
             ced_object.tracker_data.width + p_ced_cals->k_ced_object_width_safety_margin_for_critical_path_match);
}

/**
 * Tests that predicted object width is not increased by safety margin for critical objects if object was not critical.
 * \uts{CSCSA-41507} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__does_not_increase_object_width_for_prev_not_critical_object)
{
   /** \arrange Set up CED object that is approaching ego from behind and was not critical in last cycle. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(4u);

   p_ced_cals->k_ced_f_path_tracking_enable                                      = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]           = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]            = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_with_path_match            = 0.0f;
   p_ced_cals->k_ced_object_max_width_increase_factor_without_path_match         = 0.0f;
   p_ced_cals->k_ced_object_width_safety_margin_for_active_alert                 = 0.3f;
   p_ced_cals->k_ced_object_width_safety_margin_for_critical_path_match          = 0.3f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]               = PA_INVALID_OBJ_INDEX;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = PT_DEFAULT_MATCH_INDEX;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted width is equal to actual width. */
   EXPECT_EQ(ced_object.attributes.width_predicted, ced_object.tracker_data.width);
}

/**
 * Tests that the predicted object position is influeced by the weight factor for the path offset at crash line.
 * \uts{CSCSA-41508} \sdd{SF-3572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Predict_Object_Pos_At_Crash_Line__offset_weight_affects_pred_obj_position)
{
   /** \arrange Set up CED object that is approaching ego from behind and was not critical in last cycle. */
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];

   Ced_Fill_Raw_Tracker_Output(object_index, -10.0f, 1.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(object_index);

   p_ced_cals->k_ced_f_path_tracking_enable                            = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths                           = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_object_min_dist_to_crash_line_for_path_match      = 0.0f;
   p_ced_cals->k_ced_offset_to_path_weight                             = 0.5f;

   pt_match_info->track_match                = 4u;
   pt_match_info->track_match_age            = 0u;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.5f;
   pt_match_info->range_at_host_edge         = -4.0f;
   pt_match_info->range_at_zero              = -4.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   /** \action Call function that calculates predicted object position at crash line. */
   Ced_Predict_Object_Pos_At_Crash_Line(&ced_object, &ced_instance.persistance, p_vehicle_data, p_ced_cals);

   /** \assert Verify that predicted objects position is influenced by the offset weight factor. */
   EXPECT_FLOAT_EQ(ced_object.attributes.position_predicted.y, -4.0f + (p_ced_cals->k_ced_offset_to_path_weight * 0.5f));
}

/**
 * Tests that a CED object is classified as matched to a path if it is matched to a longitudinal path in forward direction.
 * \uts{CSCSA-41509} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_TRUE_for_long_forward_path)
{
   /** \arrange Set up CED object and longitudinal object path in forward direction. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable  = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths = FBK_FALSE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_LONG_FORWARD;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that true is returned by function and written to object properties. */
   EXPECT_TRUE(result);
}

/**
 * Tests that a CED object is classified as matched to a path if it is matched to a longitudinal path in forward direction and path
 * is mature. \uts{CSCSA-185595} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_TRUE_for_long_forward_mature_path)
{
   /** \arrange Set up CED object and longitudinal object path in forward direction. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable  = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths = FBK_TRUE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->path_state             = PATH_STATUS_MATURE;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that true is returned by function and written to object properties. */
   EXPECT_TRUE(result);
}

/**
 * Tests that a CED object is classified as not matched to a path if it is matched to a longitudinal path in forward direction and
 * path is not mature. \uts{CSCSA-185596} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_TRUE_for_long_forward_not_mature_path)
{
   /** \arrange Set up CED object and longitudinal object path in forward direction. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable  = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths = FBK_TRUE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->path_state             = PATH_STATUS_CREATION;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that true is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Tests that a CED object is classified as not matched to a path if it is matched to a longitudinal path in forward direction and
 * path is mature but calibration is disabled. \uts{CSCSA-185597} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_TRUE_for_long_forward_mature_path_calibration_disabled)
{
   /** \arrange Set up CED object and longitudinal object path in forward direction. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable  = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths = FBK_FALSE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->path_state             = PATH_STATUS_MATURE;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that true is returned by function and written to object properties. */
   EXPECT_TRUE(result);
}

/**
 * Tests that a CED object has Default track match value.
 * \uts{CSCSA-185598} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_FALSE_for_default_track_match)
{
   /** \arrange Set up CED object and longitudinal object path in forward direction. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable = FBK_TRUE;

   pt_match_info->track_match            = 255u;
   pt_match_info->path_direction         = PATH_DIRECTION_LONG_FORWARD;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that true is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Tests that a CED object is classified as matched to a path if it is matched to a longitudinal path in backward direction.
 * \uts{CSCSA-41510} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_TRUE_for_long_backward_path)
{
   /** \arrange Set up CED object and longitudinal object path in backward direction. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable  = FBK_TRUE;
   p_ced_cals->k_ced_f_use_only_mature_paths = FBK_FALSE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_LONG_BACKWARD;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that true is returned by function and written to object properties. */
   EXPECT_TRUE(result);
}

/**
 * Tests that a CED object is not classified as matched to a path if it is matched to a lateral path with direction right.
 * \uts{CSCSA-41511} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_FALSE_for_lat_right_path)
{
   /** \arrange Set up CED object and lateral object path with direction right. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable = FBK_TRUE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_LAT_RIGHT;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that false is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Tests that a CED object is not classified as matched to a path if it is matched to a lateral path with direction left.
 * \uts{CSCSA-41512} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_FALSE_for_lat_left_path)
{
   /** \arrange Set up CED object and lateral object path with direction left. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable = FBK_TRUE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_LAT_LEFT;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that false is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Tests that a CED object is not classified as matched to a path if it is matched to a path without any direction.
 * \uts{CSCSA-41513} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_FALSE_for_path_direction_none)
{
   /** \arrange Set up CED object and object path with direction none. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   boolean_T result;

   p_ced_cals->k_ced_f_path_tracking_enable = FBK_TRUE;

   pt_match_info->track_match            = 4u;
   pt_match_info->path_direction         = PATH_DIRECTION_NONE;
   ced_object.attributes.p_pt_match_info = pt_match_info;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that false is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Tests that a CED object is not classified as matched to a path if it is not matched to any path.
 * \uts{CSCSA-41514} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__is_FALSE_if_obj_path_is_not_available)
{
   /** \arrange Set up CED object and null pointer for object path. */
   Ced_Object_T ced_object{};
   boolean_T result;
   ced_instance.core_input.p_pt_output      = NULL;
   p_ced_cals->k_ced_f_path_tracking_enable = FBK_TRUE;
   Ced_Update_Object_Data(&ced_object, &ced_instance.core_input, p_ced_cals);

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that false is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Tests that an zone edge point is returned by the point calculation, if the given index corresponds to a zone edge point.
 * \uts{CSCSA-41515} \sdd{SF-3388} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Calculate_Point_To_Check__returns_correct_edge_point_of_zone_for_any_zone_edge_point)
{
   /** \arrange Set up variable to store the calculated point and a zone to use. */
   Vector_2d_T point_to_check;

   for (uint8_t i_point = FBK_ZERO_UINT; i_point < (uint8_t) CED_NUMBER_OF_ZONE_POINTS; i_point++)
   {
      /** \action Call function to calculate point to check from zone for edge point. */
      Ced_Calculate_Point_To_Check(&point_to_check, i_point, &ced_collision_zone);

      /** \assert Verify that the returned point is an edge point of the zone. */
      EXPECT_FLOAT_EQ(point_to_check.x, ced_collision_zone.points[i_point].x);
      EXPECT_FLOAT_EQ(point_to_check.y, ced_collision_zone.points[i_point].y);
   }
}

/**
 * Tests that the correct zone center point is returned by the point calculation, if the given index corresponds to the zone
 * middle. \uts{CSCSA-41516} \sdd{SF-3388} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Calculate_Point_To_Check__returns_correct_point_for_middle_if_zone_centered_at_origin)
{
   /** \arrange Set up variable to store the calculated point and a zone which is centered at the origin. */
   Vector_2d_T point_to_check;
   Fbk_Field_Of_Interest_T zone{};

   zone.size = CED_NUMBER_OF_ZONE_POINTS;

   zone.points[CED_POINT_FRONT_LEFT].x  = 2.0f;
   zone.points[CED_POINT_FRONT_RIGHT].x = 2.0f;
   zone.points[CED_POINT_REAR_LEFT].x   = -2.0f;
   zone.points[CED_POINT_REAR_RIGHT].x  = -2.0f;

   zone.points[CED_POINT_FRONT_LEFT].y  = -1.0f;
   zone.points[CED_POINT_FRONT_RIGHT].y = 1.0f;
   zone.points[CED_POINT_REAR_LEFT].y   = -1.0f;
   zone.points[CED_POINT_REAR_RIGHT].y  = 1.0f;

   /** \action Call function to calculate point to check from zone for middle point. */
   Ced_Calculate_Point_To_Check(&point_to_check, CED_POINT_MIDDLE, &zone);

   /** \assert Verify that the returned point is the origin. */
   EXPECT_FLOAT_EQ(point_to_check.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(point_to_check.y, FBK_ZERO_F);
}

/**
 * Tests that a CED object with predicted position in the right collision zone is correctly classified as inside the collision zone
 * if the predicted object heading is zero. \uts{CSCSA-41517} \sdd{SF-3567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_TRUE_for_collision_critical_objs_with_same_heading_on_right_side)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the predicted position is located in
    * the right collision zone and heading is zero. */
   boolean_T result_obj_front;
   boolean_T result_obj_rear;
   Ced_Object_T ced_object_in_front_of_ego;
   Ced_Object_T ced_object_behind_ego;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object_in_front_of_ego                                 = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_in_front_of_ego.attributes.width_predicted      = p_vehicle_data->host_width;
   ced_object_in_front_of_ego.attributes.length_predicted     = p_vehicle_data->host_length;
   ced_object_in_front_of_ego.attributes.position_predicted.x = ced_object_in_front_of_ego.attributes.length_predicted / 2.0f - 0.1f;
   ced_object_in_front_of_ego.attributes.position_predicted.y = p_vehicle_data->host_width / 2.0f
                                                                + ced_object_in_front_of_ego.attributes.width_predicted / 2.0f
                                                                + p_ced_cals->k_ced_collision_zone_width - 0.1f;
   ced_object_in_front_of_ego.attributes.heading_predicted = 0.0f;

   ced_object_behind_ego                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_behind_ego.attributes.width_predicted  = p_vehicle_data->host_width;
   ced_object_behind_ego.attributes.length_predicted = p_vehicle_data->host_length;
   ced_object_behind_ego.attributes.position_predicted.x =
      -1.0f * (p_vehicle_data->host_length + ced_object_behind_ego.attributes.length_predicted / 2.0f - 0.1f);
   ced_object_behind_ego.attributes.position_predicted.y = p_vehicle_data->host_width / 2.0f
                                                           + ced_object_behind_ego.attributes.width_predicted / 2.0f
                                                           + p_ced_cals->k_ced_collision_zone_width - 0.1f;
   ced_object_behind_ego.attributes.heading_predicted = 0.0f;

   /** \action Call function to evaluate if object is in collision zone. */
   result_obj_front = Ced_Is_Object_Collision_Critical(&ced_object_in_front_of_ego, &ced_collision_zone);
   result_obj_rear  = Ced_Is_Object_Collision_Critical(&ced_object_behind_ego, &ced_collision_zone);

   /** \assert Verify that the function returns true for the object and that intersection is detected on right side. */
   EXPECT_TRUE(result_obj_front);
   EXPECT_TRUE(result_obj_rear);
   EXPECT_EQ(ced_object_in_front_of_ego.attributes.alert_side, INTERSEC_RIGHT_SIDE);
   EXPECT_EQ(ced_object_behind_ego.attributes.alert_side, INTERSEC_RIGHT_SIDE);
}

/**
 * Tests that a CED object with predicted position in the left collision zone is correctly classified as inside the collision zone
 * if the predicted object heading is zero. \uts{CSCSA-41518} \sdd{SF-3567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_TRUE_for_collision_critical_objs_with_same_heading_on_left_side)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the predicted position is located in
    * the left collision zone and heading is zero. */
   boolean_T result_obj_front;
   boolean_T result_obj_rear;
   Ced_Object_T ced_object_in_front_of_ego;
   Ced_Object_T ced_object_behind_ego;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object_in_front_of_ego                                 = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_in_front_of_ego.attributes.width_predicted      = p_vehicle_data->host_width;
   ced_object_in_front_of_ego.attributes.length_predicted     = p_vehicle_data->host_length;
   ced_object_in_front_of_ego.attributes.position_predicted.x = ced_object_in_front_of_ego.attributes.length_predicted / 2.0f - 0.1f;
   ced_object_in_front_of_ego.attributes.position_predicted.y =
      -1.0f
      * (p_vehicle_data->host_width / 2.0f + ced_object_in_front_of_ego.attributes.width_predicted / 2.0f
         + p_ced_cals->k_ced_collision_zone_width - 0.1f);
   ced_object_in_front_of_ego.attributes.heading_predicted = 0.0f;

   ced_object_behind_ego                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_behind_ego.attributes.width_predicted  = p_vehicle_data->host_width;
   ced_object_behind_ego.attributes.length_predicted = p_vehicle_data->host_length;
   ced_object_behind_ego.attributes.position_predicted.x =
      -1.0f * (p_vehicle_data->host_length + ced_object_behind_ego.attributes.length_predicted / 2.0f - 0.1f);
   ced_object_behind_ego.attributes.position_predicted.y =
      -1.0f
      * (p_vehicle_data->host_width / 2.0f + ced_object_behind_ego.attributes.width_predicted / 2.0f
         + p_ced_cals->k_ced_collision_zone_width - 0.1f);
   ced_object_behind_ego.attributes.heading_predicted = 0.0f;

   /** \action Call function to evaluate if object is in collision zone. */
   result_obj_front = Ced_Is_Object_Collision_Critical(&ced_object_in_front_of_ego, &ced_collision_zone);
   result_obj_rear  = Ced_Is_Object_Collision_Critical(&ced_object_behind_ego, &ced_collision_zone);

   /** \assert Verify that the function returns true for the object and that intersection is detected on left side. */
   EXPECT_TRUE(result_obj_front);
   EXPECT_TRUE(result_obj_rear);
   EXPECT_EQ(ced_object_in_front_of_ego.attributes.alert_side, INTERSEC_LEFT_SIDE);
   EXPECT_EQ(ced_object_behind_ego.attributes.alert_side, INTERSEC_LEFT_SIDE);
}

/**
 * Tests that a CED object with predicted position in both collision zones is correctly classified as inside the collision zone if
 * the predicted object heading is zero. \uts{CSCSA-41519} \sdd{SF-3567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_TRUE_for_collision_critical_objs_with_same_heading_on_both_sides)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the predicted position is located in
    * both collision zones and heading is zero. */
   boolean_T result_obj_front;
   boolean_T result_obj_rear;
   Ced_Object_T ced_object_in_front_of_ego;
   Ced_Object_T ced_object_behind_ego;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object_in_front_of_ego                                 = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_in_front_of_ego.attributes.width_predicted      = p_vehicle_data->host_width + EPSILON;
   ced_object_in_front_of_ego.attributes.length_predicted     = p_vehicle_data->host_length;
   ced_object_in_front_of_ego.attributes.position_predicted.x = ced_object_in_front_of_ego.attributes.length_predicted / 2.0f - 0.1f;
   ced_object_in_front_of_ego.attributes.position_predicted.y = 0.0f;
   ced_object_in_front_of_ego.attributes.heading_predicted    = 0.0f;

   ced_object_behind_ego                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_behind_ego.attributes.width_predicted  = p_vehicle_data->host_width + EPSILON;
   ced_object_behind_ego.attributes.length_predicted = p_vehicle_data->host_length;
   ced_object_behind_ego.attributes.position_predicted.x =
      -1.0f * (p_vehicle_data->host_length + ced_object_behind_ego.attributes.length_predicted / 2.0f - 0.1f);
   ced_object_behind_ego.attributes.position_predicted.y = 0.0f;
   ced_object_behind_ego.attributes.heading_predicted    = 0.0f;

   /** \action Call function to evaluate if object is in collision zone. */
   result_obj_front = Ced_Is_Object_Collision_Critical(&ced_object_in_front_of_ego, &ced_collision_zone);
   result_obj_rear  = Ced_Is_Object_Collision_Critical(&ced_object_behind_ego, &ced_collision_zone);

   /** \assert Verify that the function returns true for the object and that intersection is detected on both sides. */
   EXPECT_TRUE(result_obj_front);
   EXPECT_TRUE(result_obj_rear);
   EXPECT_EQ(ced_object_in_front_of_ego.attributes.alert_side, INTERSEC_BOTH_SIDES);
   EXPECT_EQ(ced_object_behind_ego.attributes.alert_side, INTERSEC_BOTH_SIDES);
}

/**
 * Tests that a CED object with predicted position in the collision zone is correctly classified as inside the collision zone if
 * the predicted object heading is non-zero. \uts{CSCSA-41520} \sdd{SF-3567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_TRUE_for_collision_critical_objs_with_different_heading)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the predicted position is located in
    * inside the collision zone because of the heading that is non-zero. */
   boolean_T result_obj_heading_right;
   boolean_T result_obj_heading_left;
   Ced_Object_T ced_object_heading_right;
   Ced_Object_T ced_object_heading_left;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object_heading_right                                 = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_heading_right.attributes.width_predicted      = p_vehicle_data->host_width;
   ced_object_heading_right.attributes.length_predicted     = p_vehicle_data->host_length;
   ced_object_heading_right.attributes.position_predicted.x = ced_object_heading_right.attributes.length_predicted / 4.0f;
   ced_object_heading_right.attributes.position_predicted.y = p_vehicle_data->host_width / 2.0f
                                                              + ced_object_heading_right.attributes.width_predicted / 2.0f
                                                              + p_ced_cals->k_ced_collision_zone_width + 0.1f;
   ced_object_heading_right.attributes.heading_predicted = 0.3f;

   ced_object_heading_left                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_heading_left.attributes.width_predicted  = p_vehicle_data->host_width;
   ced_object_heading_left.attributes.length_predicted = p_vehicle_data->host_length;
   ced_object_heading_left.attributes.position_predicted.x =
      -1.0f * (p_vehicle_data->host_length + ced_object_heading_left.attributes.length_predicted / 4.0f);
   ced_object_heading_left.attributes.position_predicted.y = p_vehicle_data->host_width / 2.0f
                                                             + ced_object_heading_left.attributes.width_predicted / 2.0f
                                                             + p_ced_cals->k_ced_collision_zone_width + 0.1f;
   ced_object_heading_left.attributes.heading_predicted = -0.5f;

   /** \action Call function to evaluate if object is in collision zone. */
   result_obj_heading_right = Ced_Is_Object_Collision_Critical(&ced_object_heading_right, &ced_collision_zone);
   result_obj_heading_left  = Ced_Is_Object_Collision_Critical(&ced_object_heading_left, &ced_collision_zone);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_TRUE(result_obj_heading_right);
   EXPECT_TRUE(result_obj_heading_left);
}

/**
 * Tests that a CED object thats prediction entirely covers both collision zones is correctly classified as inside the collision
 * zone for both sides. \uts{CSCSA-41521} \sdd{SF-3567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_TRUE_for_collision_critical_objs_which_covers_both_collision_zones_entirely)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the object prediction covers both
    * collision zones entirely. */
   boolean_T result;
   Ced_Object_T ced_object;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.width_predicted =
      p_vehicle_data->host_width
      + 2.1f * (Fbk_Abs_F(ced_collision_zone.points[CED_POINT_FRONT_RIGHT].y - ced_collision_zone.points[CED_POINT_FRONT_LEFT].y));
   ced_object.attributes.length_predicted =
      p_vehicle_data->host_length
      + 2.1f * (Fbk_Abs_F(ced_collision_zone.points[CED_POINT_FRONT_RIGHT].x - ced_collision_zone.points[CED_POINT_REAR_RIGHT].x));
   ced_object.attributes.position_predicted.x = p_vehicle_data->host_length / 2.0f;
   ced_object.attributes.position_predicted.y = 0.0f;
   ced_object.attributes.heading_predicted    = 0.0f;

   /** \action Call function to evaluate if object is in collision zone. */
   result = Ced_Is_Object_Collision_Critical(&ced_object, &ced_collision_zone);

   /** \assert Verify that the function returns true for the object and that intersection is detected on both sides. */
   EXPECT_TRUE(result);
   EXPECT_EQ(ced_object.attributes.alert_side, INTERSEC_BOTH_SIDES);
}

/**
 * Tests that a CED object on the right ego side with predicted position outside of the collision zone is correctly classified as
 * outside the collision zone if the predicted object heading is zero. \uts{CSCSA-41522} \sdd{SF-3567}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_FALSE_for_not_collision_critical_objs_with_same_heading_on_right_side)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the predicted position is located on
    * the right ego side outside of the collision zone and heading is zero. */
   boolean_T result_obj_in_front;
   boolean_T result_obj_beside;
   Ced_Object_T ced_object_in_front_of_zone;
   Ced_Object_T ced_object_beside_zone;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object_in_front_of_zone                                 = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_in_front_of_zone.attributes.width_predicted      = p_vehicle_data->host_width;
   ced_object_in_front_of_zone.attributes.length_predicted     = p_vehicle_data->host_length;
   ced_object_in_front_of_zone.attributes.position_predicted.x = ced_object_in_front_of_zone.attributes.length_predicted / 2.0f + 0.1f;
   ced_object_in_front_of_zone.attributes.position_predicted.y = p_vehicle_data->host_width / 2.0f
                                                                 + ced_object_in_front_of_zone.attributes.width_predicted / 2.0f
                                                                 + p_ced_cals->k_ced_collision_zone_width - 0.1f;
   ced_object_in_front_of_zone.attributes.heading_predicted = 0.0f;

   ced_object_beside_zone                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_beside_zone.attributes.width_predicted  = p_vehicle_data->host_width;
   ced_object_beside_zone.attributes.length_predicted = p_vehicle_data->host_length;
   ced_object_beside_zone.attributes.position_predicted.x =
      -1.0f * (p_vehicle_data->host_length + ced_object_beside_zone.attributes.length_predicted / 2.0f - 0.1f);
   ced_object_beside_zone.attributes.position_predicted.y = p_vehicle_data->host_width / 2.0f
                                                            + ced_object_beside_zone.attributes.width_predicted / 2.0f
                                                            + p_ced_cals->k_ced_collision_zone_width + 0.1f;
   ced_object_beside_zone.attributes.heading_predicted = 0.0f;

   /** \action Call function to evaluate if object is in collision zone. */
   result_obj_in_front = Ced_Is_Object_Collision_Critical(&ced_object_in_front_of_zone, &ced_collision_zone);
   result_obj_beside   = Ced_Is_Object_Collision_Critical(&ced_object_beside_zone, &ced_collision_zone);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result_obj_in_front);
   EXPECT_FALSE(result_obj_beside);
}

/**
 * Tests that a CED object on the left ego side with predicted position outside of the collision zone is correctly classified as
 * outside the collision zone if the predicted object heading is zero. \uts{CSCSA-41523} \sdd{SF-3567}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_FALSE_for_not_collision_critical_objs_with_same_heading_on_left_side)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the predicted position is located on
    * the left ego side outside of the collision zone and heading is zero. */
   boolean_T result_obj_in_front;
   boolean_T result_obj_beside;
   Ced_Object_T ced_object_in_front_of_zone;
   Ced_Object_T ced_object_beside_zone;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object_in_front_of_zone                                 = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_in_front_of_zone.attributes.width_predicted      = p_vehicle_data->host_width;
   ced_object_in_front_of_zone.attributes.length_predicted     = p_vehicle_data->host_length;
   ced_object_in_front_of_zone.attributes.position_predicted.x = ced_object_in_front_of_zone.attributes.length_predicted / 2.0f + 0.1f;
   ced_object_in_front_of_zone.attributes.position_predicted.y =
      -1.0f
      * (p_vehicle_data->host_width / 2.0f + ced_object_in_front_of_zone.attributes.width_predicted / 2.0f
         + p_ced_cals->k_ced_collision_zone_width - 0.1f);
   ced_object_in_front_of_zone.attributes.heading_predicted = 0.0f;

   ced_object_beside_zone                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_beside_zone.attributes.width_predicted  = p_vehicle_data->host_width;
   ced_object_beside_zone.attributes.length_predicted = p_vehicle_data->host_length;
   ced_object_beside_zone.attributes.position_predicted.x =
      -1.0f * (p_vehicle_data->host_length + ced_object_beside_zone.attributes.length_predicted / 2.0f - 0.1f);
   ced_object_beside_zone.attributes.position_predicted.y =
      -1.0f
      * (p_vehicle_data->host_width / 2.0f + ced_object_beside_zone.attributes.width_predicted / 2.0f
         + p_ced_cals->k_ced_collision_zone_width + 0.1f);
   ced_object_beside_zone.attributes.heading_predicted = 0.0f;

   /** \action Call function to evaluate if object is in collision zone. */
   result_obj_in_front = Ced_Is_Object_Collision_Critical(&ced_object_in_front_of_zone, &ced_collision_zone);
   result_obj_beside   = Ced_Is_Object_Collision_Critical(&ced_object_beside_zone, &ced_collision_zone);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result_obj_in_front);
   EXPECT_FALSE(result_obj_beside);
}

/**
 * Tests that a CED object with predicted position outside of the collision zone is correctly classified as outside the collision
 * zone if the predicted object heading is non-zero. \uts{CSCSA-41524} \sdd{SF-3567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Collision_Critical__is_FALSE_for_not_collision_critical_objs_with_different_heading)
{
   /** \arrange Set up predicted position, heading, width and length of CED object such that the predicted position is located on
    * outside of the collision zone because of the heading that is non-zero. */
   boolean_T result_heading_right;
   boolean_T result_heading_left;
   Ced_Object_T ced_object_heading_right;
   Ced_Object_T ced_object_heading_left;

   /* Computation uses predicted position and heading
    Therefore actual position and velocity have not to be set here */
   Ced_Fill_Raw_Tracker_Output(4u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

   ced_object_heading_right                                 = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_heading_right.attributes.width_predicted      = p_vehicle_data->host_width;
   ced_object_heading_right.attributes.length_predicted     = p_vehicle_data->host_length;
   ced_object_heading_right.attributes.position_predicted.x = 0.0f;
   ced_object_heading_right.attributes.position_predicted.y = p_vehicle_data->host_width / 2.0f
                                                              + ced_object_heading_right.attributes.width_predicted / 2.0f
                                                              + p_ced_cals->k_ced_collision_zone_width + 2.0f;
   ced_object_heading_right.attributes.heading_predicted = 0.5f;

   ced_object_heading_left                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object_heading_left.attributes.width_predicted  = p_vehicle_data->host_width;
   ced_object_heading_left.attributes.length_predicted = p_vehicle_data->host_length;
   ced_object_heading_left.attributes.position_predicted.x =
      -1.0f * (p_vehicle_data->host_length + ced_object_heading_left.attributes.length_predicted / 2.0f + 2.0f);
   ced_object_heading_left.attributes.position_predicted.y =
      -1.0f
      * (p_vehicle_data->host_width / 2.0f + ced_object_heading_left.attributes.width_predicted / 2.0f
         + p_ced_cals->k_ced_collision_zone_width + 0.1f);
   ced_object_heading_left.attributes.heading_predicted = -0.3f;

   /** \action Call function to evaluate if object is in collision zone. */
   result_heading_right = Ced_Is_Object_Collision_Critical(&ced_object_heading_right, &ced_collision_zone);
   result_heading_left  = Ced_Is_Object_Collision_Critical(&ced_object_heading_left, &ced_collision_zone);

   /** \assert Verify that the function returns false for the object. */
   EXPECT_FALSE(result_heading_right);
   EXPECT_FALSE(result_heading_left);
}

/**
 * Define object zone such that it intersects with the ced zone on the right side.
 * \uts{CSCSA-41525} \sdd{SF-3448} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Zone__intersection_on_right_side)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap_right      = FBK_FALSE;
   boolean_T f_zone_overlap_left       = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Object_In_Zone to evaluate if object is in collision zone. */
   Ced_Is_Object_In_Zone(&f_zone_overlap_right, &f_zone_overlap_left, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on right side was found. */
   EXPECT_TRUE(f_zone_overlap_right);
   EXPECT_FALSE(f_zone_overlap_left);
}

/**
 * Define object zone such that it intersects with the ced zone on the left side.
 * \uts{CSCSA-41526} \sdd{SF-3448} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Zone__intersection_on_left_side)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap_right      = FBK_FALSE;
   boolean_T f_zone_overlap_left       = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Object_In_Zone to evaluate if object is in collision zone. */
   Ced_Is_Object_In_Zone(&f_zone_overlap_right, &f_zone_overlap_left, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on left side was found. */
   EXPECT_FALSE(f_zone_overlap_right);
   EXPECT_TRUE(f_zone_overlap_left);
}

/**
 * Define object zone such that it intersects with the ced zone on both sides.
 * \uts{CSCSA-41527} \sdd{SF-3448} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Zone__intersection_on_both_sides)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap_right      = FBK_FALSE;
   boolean_T f_zone_overlap_left       = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Object_In_Zone to evaluate if object is in collision zone. */
   Ced_Is_Object_In_Zone(&f_zone_overlap_right, &f_zone_overlap_left, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on both sides are found. */
   EXPECT_TRUE(f_zone_overlap_right);
   EXPECT_TRUE(f_zone_overlap_left);
}

/**
 * Define object zone such that it intersects with the ced zone on no side.
 * \uts{CSCSA-41528} \sdd{SF-3448} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_In_Zone__intersection_on_no_side)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap_right      = FBK_FALSE;
   boolean_T f_zone_overlap_left       = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Object_In_Zone to evaluate if object is in collision zone. */
   Ced_Is_Object_In_Zone(&f_zone_overlap_right, &f_zone_overlap_left, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on no side was found. */
   EXPECT_FALSE(f_zone_overlap_right);
   EXPECT_FALSE(f_zone_overlap_left);
}

/**
 * Define object zone such that it intersects with the ced zone on the right side.
 * \uts{CSCSA-41529} \sdd{SF-3449} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Zone_In_Object__intersection_on_right_side)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap            = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4;

   /** \action Call Ced_Is_Zone_In_Object to evaluate if object is in collision zone. */
   Ced_Is_Zone_In_Object(&f_zone_overlap, FBK_SIDE_RIGHT, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on right side was found. */
   EXPECT_TRUE(f_zone_overlap);
}

/**
 * Define object zone such that it does not intersect with the ced zone on the right side.
 * \uts{CSCSA-41530} \sdd{SF-3449} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Zone_In_Object__no_intersection_on_right_side)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap            = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Zone_In_Object to evaluate if object is in collision zone. */
   Ced_Is_Zone_In_Object(&f_zone_overlap, FBK_SIDE_RIGHT, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on right side was not found. */
   EXPECT_FALSE(f_zone_overlap);
}

/**
 * Define object zone such that it intersects with the ced zone on the left side.
 * \uts{CSCSA-41531} \sdd{SF-3449} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Zone_In_Object__intersection_on_left_side)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap            = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Zone_In_Object to evaluate if object is in collision zone. */
   Ced_Is_Zone_In_Object(&f_zone_overlap, FBK_SIDE_LEFT, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on left side was found. */
   EXPECT_TRUE(f_zone_overlap);
}

/**
 * Define object zone such that it does not intersect with the ced zone on the left side.
 * \uts{CSCSA-41532} \sdd{SF-3449} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Zone_In_Object__no_intersection_on_left_side)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap            = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Zone_In_Object to evaluate if object is in collision zone. */
   Ced_Is_Zone_In_Object(&f_zone_overlap, FBK_SIDE_LEFT, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on left side was not found. */
   EXPECT_FALSE(f_zone_overlap);
}

/**
 * Define object zone such that it intersects with the ced zone, and points are on different sides .
 * \uts{CSCSA-186373} \sdd{SF-3449} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Zone_In_Object__overlapp_point_on_diff_sides)
{
   /** \arrange Create object zone based on ced zone. */
   Fbk_Field_Of_Interest_T object_zone = {};
   boolean_T f_zone_overlap            = FBK_FALSE;

   object_zone.points[0].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[0].y = ced_collision_zone.points[2].y - 0.1f;
   object_zone.points[1].x = ced_collision_zone.points[2].x + 0.1f;
   object_zone.points[1].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[2].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[2].y = -ced_collision_zone.points[2].y + 0.1f;
   object_zone.points[3].x = ced_collision_zone.points[2].x - 0.1f;
   object_zone.points[3].y = -ced_collision_zone.points[2].y - 0.1f;
   object_zone.size        = 4u;

   /** \action Call Ced_Is_Zone_In_Object to evaluate if object is in collision zone. */
   Ced_Is_Zone_In_Object(&f_zone_overlap, FBK_SIDE_LEFT, &ced_collision_zone, &object_zone);

   /** \assert Verify that intersection on left side was found. */
   EXPECT_TRUE(f_zone_overlap);
}


/**
 * Define object such that it is validated as approaching from front.
 * \uts{CSCSA-41533} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__object_approaching_from_front)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front was detected. */
   EXPECT_TRUE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front.
 * \uts{CSCSA-41534} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__object_not_approaching_from_front)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min - EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max + EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is validated as approaching from rear.
 * \uts{CSCSA-41535} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__object_approaching_from_rear)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear was detected. */
   EXPECT_TRUE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear.
 * \uts{CSCSA-41536} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__object_not_approaching_from_rear)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min - EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max + EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is suppressed due to being in the ego line.
 * \uts{CSCSA-41537} \sdd{SF-3446} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Ego_Lane__warning_is_suppressed)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                     = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   p_ced_cals->k_ced_f_allow_ego_lane_alerts = FBK_FALSE;
   ced_object.attributes.ego_side            = EGO_LANE;
   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   /** \action Call Ced_Is_Warning_Suppressed_Ego_Lane to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would be suppressed. */
   EXPECT_TRUE(f_suppressed);
}

/**
 * Define object such that it is suppressed due to being in the ego line.
 * \uts{CSCSA-185599} \sdd{SF-3446} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Ego_Lane__warning_is_suppressed_outside_ego_lane)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                     = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   p_ced_cals->k_ced_f_allow_ego_lane_alerts = FBK_FALSE;
   ced_object.attributes.ego_side            = EGO_LEFT_SIDE;
   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   /** \action Call Ced_Is_Warning_Suppressed_Ego_Lane to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would be suppressed. */
   EXPECT_FALSE(f_suppressed);
}

/**
 * Define laterally shifted object with default match PT index, that it is suppressed due to being in the ego line.
 * \uts{CSCSA-112869} \sdd{SF-3446} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Ego_Lane__lat_shifted_object_suppressed_due_to_pt)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                     = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   p_ced_cals->k_ced_f_allow_ego_lane_alerts  = FBK_FALSE;
   ced_object.attributes.ego_side             = EGO_LANE;
   ced_object.attributes.position_predicted.y = p_ced_cals->k_ced_ego_lane_width + EPSILON;
   pt_match_info->track_match                 = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_match_info      = pt_match_info;

   /** \action Call Ced_Is_Warning_Suppressed_Ego_Lane to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would be suppressed. */
   EXPECT_TRUE(f_suppressed);
}

/**
 * Define object with PT parameters that it is suppressed due to being in the ego line.
 * \uts{CSCSA-112870} \sdd{SF-3446} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Ego_Lane__pt_suppressed_warning)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                     = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   p_ced_cals->k_ced_f_allow_ego_lane_alerts = FBK_FALSE;
   ced_object.attributes.ego_side            = EGO_LANE;
   pt_match_info->track_match                = 7u;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   /** \action Call Ced_Is_Warning_Suppressed_Ego_Lane to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would be suppressed. */
   EXPECT_TRUE(f_suppressed);
}

/**
 * Define object such that it is not suppressed due to not being in ego line.
 * \uts{CSCSA-112871} \sdd{SF-3446} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Ego_Lane__warning_not_suppressed)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                     = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   p_ced_cals->k_ced_f_allow_ego_lane_alerts  = FBK_FALSE;
   ced_object.attributes.ego_side             = EGO_LANE;
   ced_object.attributes.position_predicted.y = p_ced_cals->k_ced_ego_lane_width + EPSILON;
   pt_match_info->track_match                 = 1u;
   ced_object.attributes.p_pt_match_info      = pt_match_info;

   /** \action Call Ced_Is_Warning_Suppressed_Ego_Lane to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that object warning wouldn't be suppressed. */
   EXPECT_FALSE(f_suppressed);
}


/**
 * Define object such that it is not suppressed due to being in the ego line but calibration enabled alerts.
 * \uts{CSCSA-41538} \sdd{SF-3446} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Ego_Lane__warning_is_not_suppressed)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                     = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   p_ced_cals->k_ced_f_allow_ego_lane_alerts = FBK_TRUE;
   ced_object.attributes.ego_side            = EGO_LEFT_SIDE;
   pt_match_info->track_match                = 5u;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   /** \action Call Ced_Is_Warning_Suppressed_Ego_Lane to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_FALSE(f_suppressed);
}

/**
 * Define object such that it is suppressed due to being on the right side of the ego but raising an alert on the left side.
 * \uts{CSCSA-41539} \sdd{SF-3447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Opposite_Side__warning_is_suppressed_on_left_side)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                      = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info  = &pt_output.path_obj_pair_output[0u];
   ced_object.attributes.alert_side             = INTERSEC_LEFT_SIDE;
   ced_object.attributes.ego_side               = EGO_RIGHT_SIDE;
   pt_match_info->track_match                   = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_match_info        = pt_match_info;
   p_ced_cals->k_ced_allow_opposite_side_alerts = OPPOSITE_SIDE_ALERT_NOT_ALLOWED;

   /** \action Call Ced_Is_Warning_Suppressed_Ego_Lane to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Opposite_Side(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_TRUE(f_suppressed);
}

/**
 * Define object such that it is suppressed due to being on the left side of the ego but raising an alert on the right side.
 * \uts{CSCSA-41540} \sdd{SF-3447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Opposite_Side__warning_is_suppressed_on_right_side)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                      = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info  = &pt_output.path_obj_pair_output[0u];
   ced_object.attributes.alert_side             = INTERSEC_RIGHT_SIDE;
   ced_object.attributes.ego_side               = EGO_LEFT_SIDE;
   pt_match_info->track_match                   = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_match_info        = pt_match_info;
   p_ced_cals->k_ced_allow_opposite_side_alerts = OPPOSITE_SIDE_ALERT_ONLY_ALLOWED_WITH_PATH_MATCH;

   /** \action Call Ced_Is_Warning_Suppressed_Opposite_Side to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Opposite_Side(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_TRUE(f_suppressed);
}

/**
 * Define object such that it is not suppressed due to being on the opposite side of the ego compared to the warning side.
 * \uts{CSCSA-41541} \sdd{SF-3447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Opposite_Side__warning_is_not_suppressed)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                      = {};
   Pt_Path_Object_Pair_Output_T *pt_match_info  = &pt_output.path_obj_pair_output[0u];
   ced_object.attributes.alert_side             = INTERSEC_LEFT_SIDE;
   ced_object.attributes.ego_side               = EGO_LEFT_SIDE;
   pt_match_info->track_match                   = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_match_info        = pt_match_info;
   p_ced_cals->k_ced_allow_opposite_side_alerts = OPPOSITE_SIDE_ALERT_ALLOWED;

   /** \action Call Ced_Is_Warning_Suppressed_Opposite_Side to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Opposite_Side(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_FALSE(f_suppressed);
}

/**
 * Define object such that it is not suppressed due to being on the opposite side but path tracking path is null.
 * \uts{CSCSA-186374} \sdd{SF-3447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Opposite_Side__Warning_not_suppressed_null_pt_path_on_same_side)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                      = {};
   ced_object.attributes.alert_side             = INTERSEC_LEFT_SIDE;
   ced_object.attributes.ego_side               = EGO_LEFT_SIDE;
   ced_object.attributes.p_pt_match_info        = NULL;
   p_ced_cals->k_ced_allow_opposite_side_alerts = OPPOSITE_SIDE_ALERT_ONLY_ALLOWED_WITH_PATH_MATCH;

   /** \action Call Ced_Is_Warning_Suppressed_Opposite_Side to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Opposite_Side(&ced_object, p_ced_cals);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_FALSE(f_suppressed);
}


/**
 * Define object such that it should be suppressed but checker is disabled.
 * \uts{CSCSA-41542} \sdd{CSCSA-31272} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Cross_Border__cal_disabled_not_suppress)
{
   /** \arrange Create object which should be suppressed but disable calibration flag. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 4u;
   Ced_Fill_Raw_Tracker_Output(obj_index, -10.0f, -0.0f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                                           = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.vcs_heading                  = 0.0f;
   p_ced_cals->k_ced_f_object_lat_on_one_side_of_border = FBK_FALSE;

   /** \action Call Ced_Is_Warning_Suppressed_Cross_Border to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Cross_Border(&ced_object, p_ced_cals, ced_instance.core_input.p_pa_data);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_FALSE(f_suppressed);
}

/**
 * Define object such that it is not suppressed due reference point - front left corner, located on left specific line side.
 * \uts{CSCSA-41543} \sdd{CSCSA-31272} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Cross_Border__left_fov_reference_point_on_left)
{
   /** \arrange Create object which does not cross a specific line laterally. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 4u;
   Ced_Fill_Raw_Tracker_Output(obj_index, -10.0f, -1.01f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                                           = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.vcs_heading                  = 0.0f;
   object_data[obj_index].f_is_in_rl_sensor_fov         = FBK_TRUE;
   p_ced_cals->k_ced_f_object_lat_on_one_side_of_border = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_of_border                  = 1.0f;

   /** \action Call Ced_Is_Warning_Suppressed_Cross_Border to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Cross_Border(&ced_object, p_ced_cals, ced_instance.core_input.p_pa_data);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_FALSE(f_suppressed);
}

/**
 * Define object such that it is not suppressed due reference point - front right corner, located on right specific line side.
 * \uts{CSCSA-41544} \sdd{CSCSA-31272} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Cross_Border__right_fov_reference_point_on_right)
{
   /** \arrange Create object which does not cross a specific line laterally. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 4u;
   Ced_Fill_Raw_Tracker_Output(obj_index, -10.0f, 1.2f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                                           = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.vcs_heading                  = 0.0f;
   object_data[obj_index].f_is_in_rr_sensor_fov         = FBK_TRUE;
   object_data[obj_index].f_is_in_rl_sensor_fov         = FBK_FALSE;
   p_ced_cals->k_ced_f_object_lat_on_one_side_of_border = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_of_border                  = 1.0f;

   /** \action Call Ced_Is_Warning_Suppressed_Cross_Border to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Cross_Border(&ced_object, p_ced_cals, ced_instance.core_input.p_pa_data);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_FALSE(f_suppressed);
}

/**
 * Define object such that it is suppressed due reference point - front right corner, located on opposite side of specific line.
 * \uts{CSCSA-41545} \sdd{CSCSA-31272} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Cross_Border__left_fov_reference_point_on_right)
{
   /** \arrange Create object which does not cross a specific line laterally. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 4u;
   Ced_Fill_Raw_Tracker_Output(obj_index, -10.0f, -0.6f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                                           = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.vcs_heading                  = 0.0f;
   object_data[obj_index].f_is_in_rl_sensor_fov         = FBK_TRUE;
   p_ced_cals->k_ced_f_object_lat_on_one_side_of_border = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_of_border                  = 1.0f;

   /** \action Call Ced_Is_Warning_Suppressed_Cross_Border to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Cross_Border(&ced_object, p_ced_cals, ced_instance.core_input.p_pa_data);

   /** \assert Verify that object warning would not be suppressed. */
   EXPECT_TRUE(f_suppressed);
}


/**
 * Define object such that it is suppressed due reference point - front left corner, located on opposite side of specific line.
 * \uts{CSCSA-41546} \sdd{CSCSA-31272} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Cross_Border__right_fov_reference_point_on_left)
{
   /** \arrange Create object which cross a specific line laterally. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 4u;
   Ced_Fill_Raw_Tracker_Output(obj_index, -10.0f, 0.7f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                                           = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.vcs_heading                  = 0.0f;
   object_data[obj_index].f_is_in_rr_sensor_fov         = FBK_TRUE;
   object_data[obj_index].f_is_in_rl_sensor_fov         = FBK_FALSE;
   p_ced_cals->k_ced_f_object_lat_on_one_side_of_border = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_of_border                  = 1.0f;

   /** \action Call Ced_Is_Warning_Suppressed_Cross_Border to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Cross_Border(&ced_object, p_ced_cals, ced_instance.core_input.p_pa_data);

   /** \assert Verify that object warning should be suppressed. */
   EXPECT_TRUE(f_suppressed);
}

/**
 * Define object such that it is not suppressed due it is not in any FOV.
 * \uts{CSCSA-112872} \sdd{CSCSA-31272} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Warning_Suppressed_Cross_Border__object_in_none_fov)
{
   /** \arrange Create object which cross a specific line laterally. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 2u;
   Ced_Fill_Raw_Tracker_Output(obj_index, -10.0f, 0.7f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                                           = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.vcs_heading                  = 0.0f;
   object_data[obj_index].f_is_in_rr_sensor_fov         = FBK_FALSE;
   object_data[obj_index].f_is_in_rl_sensor_fov         = FBK_FALSE;
   p_ced_cals->k_ced_f_object_lat_on_one_side_of_border = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_of_border                  = 0.0f;

   /** \action Call Ced_Is_Warning_Suppressed_Cross_Border to evaluate if object shall be suppressed. */
   boolean_T f_suppressed = Ced_Is_Warning_Suppressed_Cross_Border(&ced_object, p_ced_cals, &data);

   /** \assert Verify that object warning should be suppressed. */
   EXPECT_FALSE(f_suppressed);
}


/**
 * Tests that the alert level of a critical CED object on the left ego side with a TTC within the calibration values for the first
 * alert level is correctly set. \uts{CSCSA-41547} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_alert_level_1_on_left_side_correctly)
{
   /** \arrange Set up CED object on left ego side with TTC corresponding to the first alert level. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line = p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
                                              - (0.5f
                                                 * (p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
                                                    - p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR]));
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_1 and ego side is left. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_LEFT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the holding is not set for objects with low ttp alert level is correctly set.
 * \uts{CSCSA-138952} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__no_holding_for_obj_below_min_ttp)
{
   /** \arrange Set up CED object and calibration */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                                                       = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction                                  = FBK_SIDE_REAR;
   p_ced_cals->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp = FBK_FALSE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that holding flag is false */
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the alert level of a critical CED object on the right ego side with a TTC within the calibration values for the
 * second alert level is correctly set. \uts{CSCSA-41548} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_alert_level_2_on_right_side_correctly)
{
   /** \arrange Set up CED object on right ego side with TTC corresponding to the second alert level. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;

   p_ced_cals->k_ced_f_third_warning_level_enable = FBK_FALSE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_2 and ego side is right. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_2);
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the alert level of a critical CED object on the right ego side with a TTC within the calibration values for the
 * second alert level is not set because is disabled by calibration. \uts{CSCSA-138953} \sdd{SF-3578}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__no_alert_level_2_due_calibration)
{
   /** \arrange Set up CED object on right ego side with TTC corresponding to the second alert level and disable this alert level
    * by calibration. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line        = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line   = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   p_ced_cals->k_ced_f_second_warning_level_enable = FBK_FALSE;
   p_ced_cals->k_ced_f_third_warning_level_enable  = FBK_FALSE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_1 and ego side is right. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the alert level of a critical CED object on the right ego side with a TTC within the calibration values for the third
 * alert level is correctly set. \uts{CSCSA-138954} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_alert_level_3_on_right_side_correctly)
{
   /** \arrange Set up CED object on right ego side with TTC corresponding to the third alert level. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line       = 0.5f * p_ced_cals->k_ced_third_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line  = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   p_ced_cals->k_ced_f_third_warning_level_enable = FBK_TRUE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_3 and ego side is right. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_3);
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the alert level 3 is not set for a critical CED object on the right ego side with corret TTC, but too large lateral
 * distance. \uts{CSCSA-138955} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_no_alert_level_3_distance_to_large)
{
   /** \arrange Set up CED object on right ego side with TTC corresponding to the third alert level. Lateral distance should be
    * over the limit */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_third_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   ced_object.attributes.closest_lat_dist_predicted =
      p_vehicle_data->host_width + p_ced_cals->k_ced_third_warning_pred_lat_dist_max + EPSILON;
   p_ced_cals->k_ced_f_third_warning_level_enable     = FBK_TRUE;
   p_ced_cals->k_ced_second_warning_pred_lat_dist_max = ced_object.attributes.closest_lat_dist_predicted + EPSILON;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_2 and ego side is right. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_2);
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the alert level of a critical CED object on the right ego side with a TTC within the calibration values for the
 * second alert level is set to CED_NO_ALERT, if TTP is below alert threshold. \uts{CSCSA-41549} \sdd{SF-3578}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_no_alert_if_ttp_below_alert_threshold)
{
   /** \arrange Set up CED object on right ego side with TTC corresponding to the second alert level, but TTP below alert
    * threshold. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] - EPSILON;
   p_ced_cals->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp = FBK_TRUE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_NO_ALERT and ego side is right. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
   EXPECT_TRUE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the alert level and ego side of a critical CED object in the ego lane with a TTC within the calibration values for
 * the second alert level are correctly set. \uts{CSCSA-41550} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_alert_level_2_correctly_for_obj_in_ego_lane)
{
   /** \arrange Set up CED object in the ego lane with TTC corresponding to the second alert level and allow ego lane alerts. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;

   p_ced_cals->k_ced_f_allow_ego_lane_alerts      = 1u;
   p_ced_cals->k_ced_f_third_warning_level_enable = FBK_FALSE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_2 and ego side is ego lane. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_LANE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_2);
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that the alert level of a critical CED object with a TTC higher than the threshold for the first alert level but lower
 * than the sum of the threshold for the first alert level and the needed qualification time is CED_ALERT_QUALIFICATION.
 * \uts{CSCSA-41551} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_alert_qualification_for_obj_with_ttc_lower_sum_of_threshold_and_qualification_time)
{
   /** \arrange Set up CED object on right ego side with a TTC higher than the threshold for the first alert level but lower than
    * the sum of the threshold for the first alert level and the needed qualification time is CED_ALERT_QUALIFICATION. */
   Ced_Object_T ced_object{};
   float32_T qualifying_time;

   p_ced_cals->k_ced_alert_qualifying_cycles = 3u;
   qualifying_time = p_ced_cals->k_ced_alert_qualifying_cycles * ced_instance.core_input.p_pa_data->time_diff_to_last_cycle;

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.alert_side         = INTERSEC_RIGHT_SIDE;
   ced_object.attributes.time_to_crash_line = p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR] + 0.5f * qualifying_time;
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_QUALIFICATION. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_QUALIFICATION);
}

/**
 * Tests that the alert level of a critical CED object with a TTC higher than the sum of the threshold for the first alert level
 * and the needed qualification time is CED_NO_ALERT. \uts{CSCSA-41552} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_no_alert_for_obj_with_ttc_above_sum_of_threshold_and_qualification_time)
{
   /** \arrange Set up CED object on right ego side with TTC higher than the sum of the threshold for the first alert level and the
    * needed qualification time */
   Ced_Object_T ced_object{};
   float32_T qualifying_time = p_ced_cals->k_ced_alert_qualifying_cycles * ced_instance.core_input.p_pa_data->time_diff_to_last_cycle;

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.alert_side         = INTERSEC_RIGHT_SIDE;
   ced_object.attributes.time_to_crash_line = p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR] + qualifying_time + EPSILON;
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_NO_ALERT. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
}

/**
 * Tests that the alert level of a critical CED object with a opposite side alert is set to CED_NO_ALERT, if opposite side alerts
 * are disabled by calibration value. \uts{CSCSA-41553} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_no_alert_for_obj_with_opposite_side_alert_if_this_is_disabled)
{
   /** \arrange Set up CED object on right ego side with alert side left and TTC corresponding to the second alert level. Disable
    * opposite side alerts via calibration value. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.alert_side              = INTERSEC_LEFT_SIDE;
   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   p_ced_cals->k_ced_allow_opposite_side_alerts  = OPPOSITE_SIDE_ALERT_NOT_ALLOWED;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_NO_ALERT. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
}

/**
 * Tests that the alert level of a critical CED object with a opposite side alert is set to CED_NO_ALERT, if opposite side alerts
 * are only allowed with path match and object is not matched with a path. \uts{CSCSA-41554} \sdd{SF-3578}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test,
       Ced_Set_Object_Alert_Level__sets_no_alert_for_unmatched_obj_with_opposite_side_alert_if_this_is_only_allowed_with_path_match)
{
   /** \arrange Set up CED object on right ego side with alert side left and TTC corresponding to the second alert level. Allow
    * opposite side alerts only with path match via calibration value and set matched-to-object flag to false. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.alert_side              = INTERSEC_LEFT_SIDE;
   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   pt_match_info->track_match                    = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_match_info         = pt_match_info;

   p_ced_cals->k_ced_allow_opposite_side_alerts = OPPOSITE_SIDE_ALERT_ONLY_ALLOWED_WITH_PATH_MATCH;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_NO_ALERT. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
}

/**
 * Tests that the alert level of a critical CED object with a opposite side alert is set correctly, if opposite side alerts are
 * allowed with path match and object is matched with a path. \uts{CSCSA-41555} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_alert_for_matched_obj_with_opposite_side_alert_if_this_is_allowed_with_path_match)
{
   /** \arrange Set up CED object on right ego side with alert side left and TTC corresponding to the second alert level. Allow
    * opposite side alerts only with path match via calibration value and set matched-to-object flag to true. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.alert_side              = INTERSEC_LEFT_SIDE;
   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   pt_match_info->track_match                    = 2u;
   ced_object.attributes.p_pt_match_info         = pt_match_info;

   p_ced_cals->k_ced_allow_opposite_side_alerts     = OPPOSITE_SIDE_ALERT_ONLY_ALLOWED_WITH_PATH_MATCH;
   p_ced_cals->k_ced_third_warning_ttc_threshold[0] = 1.5f;
   p_ced_cals->k_ced_third_warning_ttc_threshold[1] = 1.5f;
   p_ced_cals->k_ced_f_third_warning_level_enable   = FBK_TRUE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_3. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_3);
}

/**
 * Tests that the alert level of a critical CED object in the ego lane is set to CED_NO_ALERT, if ego lane alerts are only disabled
 * by calibration value. \uts{CSCSA-41556} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_no_alert_for_critical_obj_in_ego_lane_if_this_is_disabled)
{
   /** \arrange Set up CED object in the ego lane with TTC corresponding to the second alert level and disable ego lane alerts. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;

   p_ced_cals->k_ced_f_allow_ego_lane_alerts = 0u;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_NO_ALERT. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_LANE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
}

/**
 * Tests that the alert side for objects that are critical for both sides is restricted to the object side, if this is enabled by
 * calibration value. \uts{CSCSA-41557} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__restrict_both_side_alert_to_object_side)
{
   /** \arrange Set up CED object on right ego side with alert on both sides and TTC corresponding to the second alert level.
    * Restrict both side alerts to object side via calibration value. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line      = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   ced_object.attributes.alert_side              = INTERSEC_BOTH_SIDES;

   p_ced_cals->k_ced_f_handle_both_side_alerts_as_object_side = 1u;
   p_ced_cals->k_ced_f_third_warning_level_enable             = FBK_FALSE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_2 and alert side is equal to ego side. */
   EXPECT_EQ(ced_object.attributes.ego_side, ced_object.attributes.alert_side);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_2);
}

/**
 * Tests that the alert level of a critical CED object with status COASTED is set to CED_NO_ALERT, if alerts for coasted objects
 * are disabled by calibration value. \uts{CSCSA-41558} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__sets_no_alert_for_coasted_objects_if_this_is_disabled)
{
   /** \arrange Set up critical CED object with status COASTED and disable alerts for coasted objects in calibrations. */
   Ced_Object_T ced_object{};
   uint8_t obj_index = 4u;

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line = p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
                                              - (0.5f
                                                 * (p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
                                                    - p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR]));
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   ced_object.tracker_data.status                = PA_OBJ_STATUS_COASTED;

   p_ced_cals->k_ced_f_allow_coasted_object_alerts = FBK_FALSE;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_NO_ALERT. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_LEFT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
}

/**
 * Tests that the alert level of a new critical object is debounced when its behaving differently from the nearest recorded path
 * information. \uts{CSCSA-41559} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__debounce_object_alert_level_due_to_nearest_path_information)
{
   /** \arrange Set up critical CED object with a TTC below the cut off threshold and previous alert and also nearest path
    * information to which the object does not match. */
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   Ced_Fill_Raw_Tracker_Output(object_index, 1.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                                                        = Ced_Create_Object_From_Tracker_Output(object_index);
   ced_object.attributes.direction                                   = FBK_SIDE_REAR;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]    = object_data[object_index].id;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT] = CED_ALERT_ACTIVE_LEVEL_2;

   ced_object.attributes.time_to_crash_line = p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
                                              - (0.5f
                                                 * (p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
                                                    - p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR]));
   ced_object.attributes.time_to_pass_crash_line = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;

   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   pt_match_info->track_match_age            = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress - 1u;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_NONE;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 0.0f;
   pt_match_info->range_at_zero              = 0.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   pt_nearest_path_info->range_vcs_proj_to_path_segment = p_ced_cals->k_ced_suppress_range_to_nearest_path_max + 0.1f;
   pt_nearest_path_info->segment_heading_diff           = p_ced_cals->k_ced_suppress_pt_heading_diff_ced_alert_max;
   pt_nearest_path_info->track_idx_nearest_path         = 0u;
   ced_object.attributes.p_pt_nearest_path_info         = pt_nearest_path_info;

   ced_object.tracker_data.age = p_ced_cals->k_ced_suppress_alert_object_age_max;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_NO_ALERT. */
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
}

/**
 * Tests that the alert level of a new critical object is downgraded from level 2 to level 1 alert if the lateral distance is too
 * large. information. \uts{CSCSA-41560} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__lvl_2_object_alert_is_downgraded_to_lvl_1_due_to_lateral_dist)
{
   /** \arrange Set up CED object on right ego side with TTC corresponding to the second alert level. */
   Ced_Object_T ced_object{};
   uint8_t object_index = 4u;

   float32_T lat_pos = 6.0f;
   Ced_Fill_Raw_Tracker_Output(object_index, 1.0f, lat_pos, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                      = Ced_Create_Object_From_Tracker_Output(object_index);
   ced_object.attributes.direction = FBK_SIDE_REAR;

   ced_object.attributes.time_to_crash_line         = 0.5f * p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR];
   ced_object.attributes.time_to_pass_crash_line    = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   ced_object.attributes.closest_lat_dist_predicted = lat_pos - Fbk_Half(ced_object.tracker_data.width);
   p_ced_cals->k_ced_second_warning_pred_lat_dist_max =
      ced_object.attributes.closest_lat_dist_predicted - Fbk_Half(p_vehicle_data->host_width) - EPSILON;
   p_ced_cals->k_ced_f_third_warning_level_enable                    = FBK_FALSE;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT] = CED_NO_ALERT;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is downgraded to CED_ALERT_ACTIVE_LEVEL_1 and ego side is right. */
   EXPECT_EQ(ced_object.attributes.ego_side, EGO_RIGHT_SIDE);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_FALSE(ced_object.attributes.f_skip_alert_holding);
}

/**
 * Tests that is no alert level of a critical CED object on the right ego side due to undefined side.
 * \uts{CSCSA-186375} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__alert_level_0_due_undefined_side)
{
   /** \arrange Set up CED object on left ego side with TTC corresponding to the first alert level. */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f);
   ced_object                                                        = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction                                   = FBK_SIDE_UNDEFINED;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT] = CED_NO_ALERT;

   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_1 and ego side is left. */
   EXPECT_EQ(ced_object.attributes.f_skip_alert_holding, p_ced_cals->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp);
   EXPECT_EQ(ced_object.attributes.alert_level, CED_NO_ALERT);
}

/**
 * Tests that alert is triggered becuase hysteresis for lateral distance has been applied.
 * \uts{CSCSA-261205} \sdd{SF-3578} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Object_Alert_Level__hyst_lat_dist)
{
   /** \arrange Set up CED object on left ego side with TTC corresponding to the third alert level. Without hysteresis object is
    * lateraly outside lvl3 threshold */
   Ced_Object_T ced_object{};

   Ced_Fill_Raw_Tracker_Output(4u, 1.0f, 3.1f, 0.0f, 0.0f, 0.0f, 0.0f);
   ced_object                                             = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.direction                        = FBK_SIDE_REAR;
   ced_object.attributes.alert_side                       = INTERSEC_RIGHT_SIDE;
   p_ced_cals->k_ced_f_third_warning_level_enable         = FBK_TRUE;
   p_ced_cals->k_ced_third_warning_pred_lat_dist_max      = 1.0f;
   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis = 0.3f;
   ced_object.attributes.closest_lat_dist_predicted = ced_object.tracker_data.vcs_pos.y - Fbk_Half(ced_object.tracker_data.width);
   ced_object.attributes.time_to_crash_line         = p_ced_cals->k_ced_third_warning_ttc_threshold[FBK_SIDE_REAR] - EPSILON;
   ced_object.attributes.time_to_pass_crash_line    = p_ced_cals->k_ced_alert_ttp_min[FBK_SIDE_REAR] + EPSILON;
   ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3]     = p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_3;


   /** \action Call function that evaluates the alert level for the CED object. */
   Ced_Set_Object_Alert_Level(&ced_object, p_ced_cals, &ced_instance.core_input, p_vehicle_data);

   /** \assert Verify that object alert level is CED_ALERT_ACTIVE_LEVEL_3. */
   EXPECT_EQ(ced_object.attributes.alert_level, CED_ALERT_ACTIVE_LEVEL_3);
}

/**
 * Tests that hysteresis for lateral distance was not applied when the object does not intersect the collision zone.
 * \uts{CSCSA-261206} \sdd{CSCSA-261204} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Distance_Hystereis__no_object_intersection_with_zone)
{
   /** \arrange Set up alert in previous cycle as CED_NO_ALERT */
   Ced_Object_T ced_object{};
   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis            = 0.3f;
   ced_object.attributes.alert_side                                  = INTERSEC_UNDEF_SIDE;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT] = CED_NO_ALERT;

   /** \action Call function that returns the value of hysteresis. */
   Ced_Get_Lat_Distance_Hystereis(&ced_instance.persistance, p_ced_cals, &ced_object);

   /** \assert Verify that hysteresis is not applied. */
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_NO_ALERT], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3], 0.0);
}

/**
 * Tests that right hysteresis for lateral distance was not applied.
 * \uts{CSCSA-261207} \sdd{CSCSA-261204} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Distance_Hystereis__no_right_alert_in_prev_cycle)
{
   /** \arrange Set up alert in previous cycle as CED_NO_ALERT */
   Ced_Object_T ced_object{};
   ced_object.tracker_data.id                                         = 4u;
   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis             = 0.3f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_NO_ALERT;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = 4u;

   /** \action Call function that returns the value of hysteresis. */
   Ced_Get_Lat_Distance_Hystereis(&ced_instance.persistance, p_ced_cals, &ced_object);

   /** \assert Verify that hysteresis is not applied. */
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_NO_ALERT], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3], 0.0);
}


/**
 * Tests that left hysteresis for lateral distance was not applied.
 * \uts{CSCSA-261208} \sdd{CSCSA-261204} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Distance_Hystereis__no_left_alert_in_prev_cycle)
{
   /** \arrange Set up alert in previous cycle as CED_NO_ALERT */
   Ced_Object_T ced_object{};
   ced_object.tracker_data.id                                        = 4u;
   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis            = 0.3f;
   ced_object.attributes.alert_side                                  = INTERSEC_LEFT_SIDE;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT] = CED_NO_ALERT;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]    = 4u;

   /** \action Call function that returns the value of hysteresis. */
   Ced_Get_Lat_Distance_Hystereis(&ced_instance.persistance, p_ced_cals, &ced_object);

   /** \assert Verify that hysteresis is not applied. */
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_NO_ALERT], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3], 0.0);
}

/**
 * Tests that left hysteresis for lateral distance has been applied.
 * \uts{CSCSA-261209} \sdd{CSCSA-261204} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Distance_Hystereis__left_alert_in_prev_cycle)
{
   /** \arrange Set up alert in previous cycle as CED_ALERT_ACTIVE_LEVEL_1 and object left intersection */
   Ced_Object_T ced_object{};
   ced_object.tracker_data.id                                        = 7u;
   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis            = 0.3f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT] = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]    = ced_object.tracker_data.id;

   /** \action Call function that returns the value of hysteresis. */
   Ced_Get_Lat_Distance_Hystereis(&ced_instance.persistance, p_ced_cals, &ced_object);

   /** \assert Verify that hysteresis is applied. */
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_NO_ALERT], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1],
                   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3], 0.0);
}


/**
 * Tests that right hysteresis for lateral distance has been applied.
 * \uts{CSCSA-261210} \sdd{CSCSA-261204} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Distance_Hystereis__right_alert_in_prev_cycle)
{
   /** \arrange Set up alert in previous cycle as CED_ALERT_ACTIVE_LEVEL_1 and object right intersection */
   Ced_Object_T ced_object{};
   ced_object.tracker_data.id                                         = 7u;
   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis             = 0.3f;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = ced_object.tracker_data.id;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_2;

   /** \action Call function that returns the value of hysteresis. */
   Ced_Get_Lat_Distance_Hystereis(&ced_instance.persistance, p_ced_cals, &ced_object);

   /** \assert Verify that hysteresis is applied. */
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_NO_ALERT], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2],
                   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3], 0.0);
}

/**
 * Tests that right hysteresis for lateral distance has been applied for alert level 3.
 * \uts{CSCSA-318427} \sdd{CSCSA-261204} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Distance_Hystereis__right_alert_lvl_3_in_prev_cycle)
{
   /** \arrange Set up alert in previous cycle as CED_ALERT_ACTIVE_LEVEL_1 and object right intersection */
   Ced_Object_T ced_object{};
   ced_object.tracker_data.id                                         = 11u;
   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis             = 0.3f;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = ced_object.tracker_data.id;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_3;

   /** \action Call function that returns the value of hysteresis. */
   Ced_Get_Lat_Distance_Hystereis(&ced_instance.persistance, p_ced_cals, &ced_object);

   /** \assert Verify that hysteresis is applied. */
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_NO_ALERT], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_1], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_2], 0.0);
   EXPECT_FLOAT_EQ(ced_object.attributes.zone_width_hys[CED_ALERT_ACTIVE_LEVEL_3],
                   p_ced_cals->k_ced_warning_pred_lat_dist_max_histeresis);
}


/**
 * Tests that object persistent data are filled correctly for a rear CED object.
 * \uts{CSCSA-41561} \sdd{SF-3565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Fill_Object_Persistent_Data__fill_persistent_data_for_rear_object)
{
   /** \arrange Set up CED object with non-default values. */
   Ced_Object_T ced_object{};
   uint8_t id = 4u;

   ced_object.tracker_data.id              = id;
   ced_object.tracker_data.index           = id - 1u;
   ced_object.attributes.heading_predicted = 0.2f;
   ced_object.attributes.alert_level       = CED_ALERT_ACTIVE_LEVEL_2;
   ced_object.attributes.direction         = FBK_SIDE_REAR;

   /** \action Call function to fill object persistent data. */
   Ced_Fill_Object_Persistent_Data(&ced_instance.persistance, &ced_object);

   /** \assert Verify that object persistent data is filled with CED object data. */
   EXPECT_FLOAT_EQ(ced_instance.persistance.ced_object_heading_predicted[id], ced_object.attributes.heading_predicted);
}

/**
 * Tests that object persistent data are filled correctly for a front CED object.
 * \uts{CSCSA-41562} \sdd{SF-3565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Fill_Object_Persistent_Data__fill_persistent_data_for_front_object)
{
   /** \arrange Set up CED object with non-default values. */
   Ced_Object_T ced_object{};
   uint8_t id = 6u;
   float32_T ced_object_heading_flipped;

   ced_object.tracker_data.id              = id;
   ced_object.tracker_data.index           = id - 4u;
   ced_object.attributes.heading_predicted = 0.2f;
   ced_object_heading_flipped              = Ced_Flip_Heading_Value(ced_object.attributes.heading_predicted);
   ced_object.attributes.alert_level       = CED_ALERT_ACTIVE_LEVEL_2;
   ced_object.attributes.direction         = FBK_SIDE_FRONT;

   /** \action Call function to fill object persistent data. */
   Ced_Fill_Object_Persistent_Data(&ced_instance.persistance, &ced_object);

   /** \assert Verify that object persistent data is filled with CED object data. */
   EXPECT_FLOAT_EQ(ced_instance.persistance.ced_object_heading_predicted[id], ced_object_heading_flipped);
}


/**
 * Tests that side persistent data are filled correctly from core output.
 * \uts{CSCSA-41563} \sdd{SF-3566} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Fill_Side_Persistent_Data__works_properly)
{
   /** \arrange Set up core output with non-default values. */
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                   = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                      = 8u;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT] = 4u;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                   = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                      = 6u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]               = 6u;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT] = 7u;

   /** \action Call function to fill side persistent data. */
   Ced_Fill_Side_Persistent_Data(&ced_instance.persistance, &ced_instance.core_output);

   /** \assert Verify that side persistent data is filled with core output data. */
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT], CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT], 4u);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT], CED_ALERT_ACTIVE_LEVEL_2);
   EXPECT_EQ(ced_instance.persistance.ced_side_unique_id_prev_cycle[FBK_SIDE_RIGHT], 6u);
   EXPECT_EQ(ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT], 7u);
}

/**
 * Tests that a critical CED object with alert side right is set as the most critical object on this side, if no other critical
 * objects are present. \uts{CSCSA-41564} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__sets_most_critical_object_right_side)
{
   /** \arrange Set up a critical CED object with alert side right. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   ced_object.attributes.alert_side         = INTERSEC_RIGHT_SIDE;
   ced_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_object.attributes.direction          = FBK_SIDE_REAR;
   ced_object.attributes.time_to_crash_line = 2.1f;
   ced_object.tracker_data.id               = 5u;
   ced_object.tracker_data.unique_id        = 5u;
   ced_object.tracker_data.index            = 2u;
   pt_match_info->track_match               = 4u;
   ced_object.attributes.p_pt_match_info    = pt_match_info;

   /** \action Call function to set the most critical object with prepared CED object. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &ced_object);

   /** \assert Verify that core output of right side is updated with the object data of the critical CED object. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT], ced_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], ced_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], ced_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT], ced_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_RIGHT], ced_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT], ced_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT], ced_object.attributes.p_pt_match_info->track_match);
}

/**
 * Tests that a critical CED object with alert side left is set as the most critical object on this side, if no other critical
 * objects are present. \uts{CSCSA-41565} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__sets_most_critical_object_left_side)
{
   /** \arrange Set up a critical CED object with alert side left. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   ced_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   ced_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_1;
   ced_object.attributes.direction          = FBK_SIDE_FRONT;
   ced_object.attributes.time_to_crash_line = 2.1f;
   ced_object.tracker_data.id               = 5u;
   ced_object.tracker_data.unique_id        = 5u;
   ced_object.tracker_data.index            = 2u;
   pt_match_info->track_match               = 4u;
   ced_object.attributes.p_pt_match_info    = pt_match_info;

   /** \action Call function to set the most critical object with prepared CED object. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &ced_object);

   /** \assert Verify that core output of left side is updated with the object data of the critical CED object. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], ced_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], ced_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], ced_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], ced_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], ced_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], ced_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT], ced_object.attributes.p_pt_match_info->track_match);
}

/**
 * Tests that a critical CED object with alert on both sides is set as the most critical object on both side, if no other critical
 * objects are present. \uts{CSCSA-41566} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__sets_most_critical_object_both_sides)
{
   /** \arrange Set up a critical CED object with alert on both sides. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   ced_object.attributes.alert_side         = INTERSEC_BOTH_SIDES;
   ced_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_1;
   ced_object.attributes.direction          = FBK_SIDE_FRONT;
   ced_object.attributes.time_to_crash_line = 2.1f;
   ced_object.tracker_data.id               = 5u;
   ced_object.tracker_data.unique_id        = 5u;
   ced_object.tracker_data.index            = 2u;
   pt_match_info->track_match               = 4u;
   ced_object.attributes.p_pt_match_info    = pt_match_info;

   /** \action Call function to set the most critical object with prepared CED object. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &ced_object);

   /** \assert Verify that core output of both sides is updated with the object data of the critical CED object. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], ced_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], ced_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], ced_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], ced_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], ced_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], ced_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT], ced_object.attributes.p_pt_match_info->track_match);

   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT], ced_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], ced_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], ced_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT], ced_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_RIGHT], ced_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT], ced_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT], ced_object.attributes.p_pt_match_info->track_match);
}

/**
 * Tests that a critical CED object replaced the old object as the most critical object on this side, if it is more critical than
 * the previous one. \uts{CSCSA-41567} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__replaces_critical_obj_with_more_critical_obj)
{
   /** \arrange Set up core output with critical object and a more critical CED object. */
   Ced_Object_T critical_object;
   Ced_Object_T more_critical_object;
   Pt_Path_Object_Pair_Output_T *pt_match_info_critical      = &pt_output.path_obj_pair_output[0u];
   Pt_Path_Object_Pair_Output_T *pt_match_info_more_critical = &pt_output.path_obj_pair_output[1u];

   /* prepare core output */
   critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_1;
   critical_object.attributes.direction          = FBK_SIDE_FRONT;
   critical_object.attributes.time_to_crash_line = 2.1f;
   critical_object.tracker_data.id               = 5u;
   critical_object.tracker_data.unique_id        = 5u;
   critical_object.tracker_data.index            = 9u;
   pt_match_info_critical->track_match           = 4u;
   critical_object.attributes.p_pt_match_info    = pt_match_info_critical;
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &critical_object);

   more_critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   more_critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_2;
   more_critical_object.attributes.direction          = FBK_SIDE_REAR;
   more_critical_object.attributes.time_to_crash_line = 1.4f;
   more_critical_object.tracker_data.id               = 7u;
   more_critical_object.tracker_data.unique_id        = 7u;
   more_critical_object.tracker_data.index            = 2u;
   pt_match_info_more_critical->track_match           = 5u;
   more_critical_object.attributes.p_pt_match_info    = pt_match_info_more_critical;

   /** \action Call function to set the most critical object with prepared CED object and prepared core output. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &more_critical_object);

   /** \assert Verify that core output is updated with the object data of the more critical object. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], more_critical_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], more_critical_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], more_critical_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], more_critical_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], more_critical_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], more_critical_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT],
             more_critical_object.attributes.p_pt_match_info->track_match);
}

/**
 * Tests that a critical CED object with an actual alert replaced the old object with status alert qualification as the most
 * critical object on this side. \uts{CSCSA-41568} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__replaces_qualifying_obj_with_more_critical_obj)
{
   /** \arrange Set up core output with critical object with status alert qualification and a more critical CED object with an
    * actual alert. */
   Ced_Object_T critical_object;
   Ced_Object_T more_critical_object;
   Pt_Path_Object_Pair_Output_T *pt_match_info_critical      = &pt_output.path_obj_pair_output[0u];
   Pt_Path_Object_Pair_Output_T *pt_match_info_more_critical = &pt_output.path_obj_pair_output[1u];

   /* prepare core output */
   critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   critical_object.attributes.alert_level        = CED_ALERT_QUALIFICATION;
   critical_object.attributes.direction          = FBK_SIDE_FRONT;
   critical_object.attributes.time_to_crash_line = 3.1f;
   critical_object.tracker_data.id               = 5u;
   critical_object.tracker_data.unique_id        = 5u;
   critical_object.tracker_data.index            = 9u;
   pt_match_info_critical->track_match           = 4u;
   critical_object.attributes.p_pt_match_info    = pt_match_info_critical;
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &critical_object);

   more_critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   more_critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_1;
   more_critical_object.attributes.direction          = FBK_SIDE_REAR;
   more_critical_object.attributes.time_to_crash_line = 1.4f;
   more_critical_object.tracker_data.id               = 15u;
   more_critical_object.tracker_data.unique_id        = 15u;
   more_critical_object.tracker_data.index            = 14u;
   pt_match_info_more_critical->track_match           = 5u;
   more_critical_object.attributes.p_pt_match_info    = pt_match_info_more_critical;

   /** \action Call function to set the most critical object with prepared CED object and prepared core output. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &more_critical_object);

   /** \assert Verify that core output is updated with the object data of the more critical object. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], more_critical_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], more_critical_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], more_critical_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], more_critical_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], more_critical_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], more_critical_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT],
             more_critical_object.attributes.p_pt_match_info->track_match);
}

/**
 * Tests that a critical CED object with an actual alert replaced the old object with status alert qualification as the most
 * critical object on this side, when no Path Tracking is available. \uts{CSCSA-90077} \sdd{SF-3577}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__replaces_qualifying_obj_with_more_critical_obj_no_path_tracking_available)
{
   /** \arrange Set up core output with critical object with status alert qualification and a more critical CED object with an
    * actual alert. */
   Ced_Object_T critical_object;
   Ced_Object_T more_critical_object;

   /* prepare core output */
   critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   critical_object.attributes.alert_level        = CED_ALERT_QUALIFICATION;
   critical_object.attributes.direction          = FBK_SIDE_FRONT;
   critical_object.attributes.time_to_crash_line = 3.1f;
   critical_object.tracker_data.id               = 5u;
   critical_object.tracker_data.unique_id        = 5u;
   critical_object.tracker_data.index            = 9u;
   critical_object.attributes.p_pt_match_info    = NULL;
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &critical_object);

   more_critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   more_critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_1;
   more_critical_object.attributes.direction          = FBK_SIDE_REAR;
   more_critical_object.attributes.time_to_crash_line = 1.4f;
   more_critical_object.tracker_data.id               = 15u;
   more_critical_object.tracker_data.unique_id        = 15u;
   more_critical_object.tracker_data.index            = 14u;
   more_critical_object.attributes.p_pt_match_info    = NULL;

   /** \action Call function to set the most critical object with prepared CED object and prepared core output. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &more_critical_object);

   /** \assert Verify that core output is updated with the object data of the more critical object. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], more_critical_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], more_critical_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], more_critical_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], more_critical_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], more_critical_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], more_critical_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT], PT_DEFAULT_MATCH_INDEX);
}

/**
 * Tests that a critical CED object does not replace the old object as the most critical object on this side, if it is less
 * critical than the previous one. \uts{CSCSA-41569} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__does_not_replace_critical_obj_with_less_critical_obj)
{
   /** \arrange Set up core output with critical object and a less critical CED object. */
   Ced_Object_T critical_object;
   Ced_Object_T less_critical_object;
   Pt_Path_Object_Pair_Output_T *pt_match_info_critical      = &pt_output.path_obj_pair_output[0u];
   Pt_Path_Object_Pair_Output_T *pt_match_info_less_critical = &pt_output.path_obj_pair_output[1u];

   /* prepare core output */
   critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_1;
   critical_object.attributes.direction          = FBK_SIDE_FRONT;
   critical_object.attributes.time_to_crash_line = 2.1f;
   critical_object.tracker_data.id               = 5u;
   critical_object.tracker_data.unique_id        = 5u;
   critical_object.tracker_data.index            = 1u;
   pt_match_info_critical->track_match           = 4u;
   critical_object.attributes.p_pt_match_info    = pt_match_info_critical;
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &critical_object);

   less_critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   less_critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_2;
   less_critical_object.attributes.direction          = FBK_SIDE_REAR;
   less_critical_object.attributes.time_to_crash_line = 3.4f;
   less_critical_object.tracker_data.id               = 7u;
   less_critical_object.tracker_data.unique_id        = 7u;
   less_critical_object.tracker_data.index            = 2u;
   pt_match_info_less_critical->track_match           = 5u;
   less_critical_object.attributes.p_pt_match_info    = pt_match_info_less_critical;

   /** \action Call function to set the most critical object with prepared CED object and prepared core output. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &less_critical_object);

   /** \assert Verify that core output is unchanged. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], critical_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], critical_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], critical_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], critical_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], critical_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], critical_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT],
             critical_object.attributes.p_pt_match_info->track_match);
}

/**
 * Tests that a critical CED object with lower TTC does not replace the old object as the most critical object on this side, if it
 * is less critical than the previous one. \uts{CSCSA-186376} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__does_not_replace_critical_obj_with_less_critical_obj_despite_lower_TTC)
{
   /** \arrange Set up core output with critical object and a less critical CED object. */
   Ced_Object_T critical_object;
   Ced_Object_T less_critical_object;
   Pt_Path_Object_Pair_Output_T *pt_match_info_critical      = &pt_output.path_obj_pair_output[0u];
   Pt_Path_Object_Pair_Output_T *pt_match_info_less_critical = &pt_output.path_obj_pair_output[1u];

   /* prepare core output */
   critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_3;
   critical_object.attributes.direction          = FBK_SIDE_FRONT;
   critical_object.attributes.time_to_crash_line = 2.1f;
   critical_object.tracker_data.id               = 5u;
   critical_object.tracker_data.unique_id        = 5u;
   critical_object.tracker_data.index            = 1u;
   pt_match_info_critical->track_match           = 4u;
   critical_object.attributes.p_pt_match_info    = pt_match_info_critical;
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &critical_object);

   less_critical_object.attributes.alert_side         = INTERSEC_LEFT_SIDE;
   less_critical_object.attributes.alert_level        = CED_ALERT_ACTIVE_LEVEL_2;
   less_critical_object.attributes.direction          = FBK_SIDE_REAR;
   less_critical_object.attributes.time_to_crash_line = 2.0f;
   less_critical_object.tracker_data.id               = 7u;
   less_critical_object.tracker_data.unique_id        = 7u;
   less_critical_object.tracker_data.index            = 2u;
   pt_match_info_less_critical->track_match           = 5u;
   less_critical_object.attributes.p_pt_match_info    = pt_match_info_less_critical;

   /** \action Call function to set the most critical object with prepared CED object and prepared core output. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &less_critical_object);

   /** \assert Verify that core output is unchanged. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], critical_object.attributes.time_to_crash_line);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], critical_object.attributes.alert_level);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], critical_object.tracker_data.id);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], critical_object.tracker_data.unique_id);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], critical_object.tracker_data.index);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], critical_object.attributes.direction);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT],
             critical_object.attributes.p_pt_match_info->track_match);
}

/**
 * Tests that a CED object with alert level CED_NO_ALERT is not set as the most critical object of any side
 * \uts{CSCSA-41570} \sdd{SF-3577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Set_Most_Critical_Object__sets_no_most_critical_object_with_alert_level_NO_ALERT)
{
   /** \arrange Set up a CED object with alert level CED_NO_ALERT. */
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];

   ced_object.attributes.alert_side         = INTERSEC_BOTH_SIDES;
   ced_object.attributes.alert_level        = CED_NO_ALERT;
   ced_object.attributes.direction          = FBK_SIDE_FRONT;
   ced_object.attributes.time_to_crash_line = 2.1f;
   ced_object.tracker_data.id               = 5u;
   ced_object.tracker_data.unique_id        = 5u;
   ced_object.tracker_data.index            = 2u;
   pt_match_info->track_match               = 4u;
   ced_object.attributes.p_pt_match_info    = pt_match_info;

   /** \action Call function to set the most critical object with prepared CED object. */
   Ced_Set_Most_Critical_Object(&ced_instance.core_output, &ced_object);

   /** \assert Verify that core output of left side is still filled with default values. */
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], CED_INVALID_TIME);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT], FBK_SIDE_UNDEFINED);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT], PT_DEFAULT_MATCH_INDEX);
}


/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns true if suppression is disabled by calibration value.
 * \uts{CSCSA-41571} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_TRUE_if_suppression_disabled_by_cal_value)
{
   /** \arrange Set up tracker data such that conditions for alert suppresion are fulfilled, but disable alert suppresion in the
    * calibration values. */
   uint8_t obj_index = 4u;
   boolean_T result  = FBK_FALSE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 0u;
   p_ced_cals->k_ced_alert_holding_obj_abs_heading_max               = 0.5f;
   p_ced_cals->k_ced_alert_holding_obj_long_vel_min                  = 1.0f;

   object_data[obj_index].vcs_pos.x   = -10.0f;
   object_data[obj_index].id          = obj_index + 2u;
   object_data[obj_index].status      = PA_OBJ_STATUS_COASTED;
   object_data[obj_index].vcs_heading = p_ced_cals->k_ced_alert_holding_obj_abs_heading_max + 0.1f;
   object_data[obj_index].vcs_vel.x   = p_ced_cals->k_ced_alert_holding_obj_long_vel_min - 0.1f;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns true if object status is INVALID.
 * \uts{CSCSA-41572} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_TRUE_if_obj_status_invalid)
{
   /** \arrange Set up tracker data such that conditions for alert suppresion are fulfilled, except for the object status being
    * INVALID. */
   uint8_t obj_index = 4u;
   boolean_T result  = FBK_FALSE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 1u;
   p_ced_cals->k_ced_alert_holding_obj_abs_heading_max               = 0.5f;
   p_ced_cals->k_ced_alert_holding_obj_long_vel_min                  = 1.0f;

   object_data[obj_index].id          = obj_index + 1u;
   object_data[obj_index].vcs_pos.x   = -10.0f;
   object_data[obj_index].status      = PA_OBJ_STATUS_INVALID;
   object_data[obj_index].vcs_heading = p_ced_cals->k_ced_alert_holding_obj_abs_heading_max + 0.1f;
   object_data[obj_index].vcs_vel.x   = p_ced_cals->k_ced_alert_holding_obj_long_vel_min - 0.1f;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns true if object index is greater than max object index number.
 * \uts{CSCSA-41573} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_TRUE_if_obj_index_number_is_invalid)
{
   /** \arrange Set up tracker data such that conditions for alert suppresion are fulfilled, except for the object index being
    * INVALID. */
   uint8_t obj_index = PA_OBJ_NUMBER_OF_OBJECTS - 1u;
   boolean_T result  = FBK_FALSE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 1u;
   object_data[obj_index].id                                         = PA_INVALID_OBJ_ID;


   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns true if heading and velocity are in range.
 * \uts{CSCSA-41574} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_TRUE_if_heading_and_velocity_in_range)
{
   /** \arrange Set up tracker data such that heading and velocity are in range and status is not INVALID. */
   uint8_t obj_index = 4u;
   boolean_T result  = FBK_FALSE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 1u;
   p_ced_cals->k_ced_alert_holding_obj_abs_heading_max               = 0.5f;
   p_ced_cals->k_ced_alert_holding_obj_long_vel_min                  = 1.0f;

   object_data[obj_index].vcs_pos.x   = -10.0f;
   object_data[obj_index].id          = obj_index + 1u;
   object_data[obj_index].status      = PA_OBJ_STATUS_COASTED;
   object_data[obj_index].vcs_heading = p_ced_cals->k_ced_alert_holding_obj_abs_heading_max - 0.1f;
   object_data[obj_index].vcs_vel.x   = p_ced_cals->k_ced_alert_holding_obj_long_vel_min + 0.1f;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns false if heading is out of range.
 * \uts{CSCSA-41575} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_FALSE_if_heading_out_of_range)
{
   /** \arrange Set up tracker data such that heading is out of range. */
   uint8_t obj_index = 4u;
   boolean_T result  = FBK_TRUE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 1u;
   p_ced_cals->k_ced_alert_holding_obj_abs_heading_max               = 0.5f;
   p_ced_cals->k_ced_alert_holding_obj_long_vel_min                  = 1.0f;

   object_data[obj_index].id          = 2u;
   object_data[obj_index].vcs_pos.x   = -10.0f;
   object_data[obj_index].status      = PA_OBJ_STATUS_COASTED;
   object_data[obj_index].vcs_heading = p_ced_cals->k_ced_alert_holding_obj_abs_heading_max + 0.1f;
   object_data[obj_index].vcs_vel.x   = p_ced_cals->k_ced_alert_holding_obj_long_vel_min + 0.1f;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns true if heading is equal zero (in range).
 * \uts{CSCSA-186377} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_TRUE_if_negative_heading)
{
   /** \arrange Set up tracker data such that heading is negative value. */
   uint8_t obj_index = 4u;
   boolean_T result  = FBK_TRUE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 1u;
   p_ced_cals->k_ced_alert_holding_obj_abs_heading_max               = 0.5f;
   p_ced_cals->k_ced_alert_holding_obj_long_vel_min                  = 1.0f;

   object_data[obj_index].id          = 2u;
   object_data[obj_index].vcs_pos.x   = -10.0f;
   object_data[obj_index].status      = PA_OBJ_STATUS_COASTED;
   object_data[obj_index].vcs_heading = -p_ced_cals->k_ced_alert_holding_obj_abs_heading_max + 0.1f;
   object_data[obj_index].vcs_vel.x   = p_ced_cals->k_ced_alert_holding_obj_long_vel_min + 0.1f;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that false is returned by function. */
   EXPECT_TRUE(result);
}


/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns false if velocity is out of range.
 * \uts{CSCSA-41576} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_FALSE_if_velocity_out_of_range)
{
   /** \arrange Set up tracker data such that velocity is out of range. */
   uint8_t obj_index = 4u;
   boolean_T result  = FBK_TRUE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 1u;
   p_ced_cals->k_ced_alert_holding_obj_abs_heading_max               = 0.5f;
   p_ced_cals->k_ced_alert_holding_obj_long_vel_min                  = 1.0f;

   object_data[obj_index].id          = 7u;
   object_data[obj_index].vcs_pos.x   = -10.0f;
   object_data[obj_index].status      = PA_OBJ_STATUS_COASTED;
   object_data[obj_index].vcs_heading = p_ced_cals->k_ced_alert_holding_obj_abs_heading_max - 0.1f;
   object_data[obj_index].vcs_vel.x   = p_ced_cals->k_ced_alert_holding_obj_long_vel_min - 0.1f;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Tests that Ced_Should_Object_Alert_Be_Held function returns true if the objects position is already past the host rear bumper.
 * \uts{CSCSA-41577} \sdd{SF-3579} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Should_Object_Alert_Be_Held__returns_TRUE_if_obj_past_rear_bumper)
{
   /** \arrange Set up tracker data such that the objects is already past the host rear bumper. */
   uint8_t obj_index = 4u;
   boolean_T result  = FBK_TRUE;

   p_ced_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects = 1u;

   object_data[obj_index].vcs_pos.x = 0.0f;
   object_data[obj_index].status    = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].id        = obj_index + 2u;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Should_Object_Alert_Be_Held. */
   result = Ced_Should_Object_Alert_Be_Held(p_ced_cals, p_vehicle_data, ced_instance.core_input.p_pa_data, &fbk_output,
                                            object_data[obj_index].id);

   /** \assert Verify that false is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Was_Object_Alerted_In_Prev_Cycle function returns true if object caused alert in previous cycle.
 * \uts{CSCSA-41578} \sdd{SF-3389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Was_Object_Alerted_In_Prev_Cycle__returns_TRUE_for_prev_obj_alert_left)
{
   /** \arrange Set up persistent data */
   uint8_t obj_id = 4u;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_NO_ALERT;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]     = obj_id;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = 0u;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Was_Object_Alerted_In_Prev_Cycle. */
   boolean_T result = Ced_Was_Object_Alerted_In_Prev_Cycle(obj_id, &ced_instance.persistance);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Was_Object_Alerted_In_Prev_Cycle function returns true if object caused alert in previous cycle.
 * \uts{CSCSA-41579} \sdd{SF-3389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Was_Object_Alerted_In_Prev_Cycle__returns_TRUE_for_prev_obj_alert_right)
{
   /** \arrange Set up persistent data */
   uint8_t obj_id = 4u;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]  = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]     = 0u;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = obj_id;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function Ced_Was_Object_Alerted_In_Prev_Cycle. */
   boolean_T result = Ced_Was_Object_Alerted_In_Prev_Cycle(obj_id, &ced_instance.persistance);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Was_Object_Alerted_In_Prev_Cycle function returns false if object did not cause alert in previous cycle.
 * \uts{CSCSA-41580} \sdd{SF-3389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Was_Object_Alerted_In_Prev_Cycle__returns_FALSE_for_no_prev_obj_alert)
{
   /** \arrange Set up persistent data */
   uint8_t obj_id = 4u;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]  = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_NO_ALERT;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = obj_id;

   /** \action Call function Ced_Was_Object_Alerted_In_Prev_Cycle. */
   boolean_T result = Ced_Was_Object_Alerted_In_Prev_Cycle(obj_id, &ced_instance.persistance);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Tests that Ced_Was_Object_Alerted_In_Prev_Cycle function returns false if object did not cause alert in previous cycle.
 * \uts{CSCSA-41581} \sdd{SF-3389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Was_Object_Alerted_In_Prev_Cycle__returns_FALSE_for_not_matching_index)
{
   /** \arrange Set up persistent data */
   uint8_t obj_id = 4u;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]     = obj_id - 1u;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = obj_id - 1u;
   /** \action Call function Ced_Was_Object_Alerted_In_Prev_Cycle. */
   boolean_T result = Ced_Was_Object_Alerted_In_Prev_Cycle(obj_id, &ced_instance.persistance);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Test that level 1 alert is not reseted by debounce alert function if qualifying counter is over threshold for an object that is
 * not classified as slow object. \uts{CSCSA-41582} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__sets_CED_alert_for_internal_alert_level_1_on_normal_obj_if_counter_over_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter over threshold for normal
    * objects). */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]         = {};
   uint8_t obj_index                                                         = 3u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_qualifying_cycles;
   p_ced_cals->k_ced_slow_objects_long_vel_max                               = 0.0f;
   object_data[obj_index].vcs_vel.x = p_ced_cals->k_ced_slow_objects_long_vel_max + EPSILON;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                      = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                         = 4u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]                  = 4u;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                      = obj_index;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                        = 2.0f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]      = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_holding_cycles;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are unchanged and qualifying counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT],
             p_ced_cals->k_ced_alert_qualifying_cycles + 1u);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], 4u);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], 4u);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], 2.0f);
}

/**
 * Test that level 1 alert is reseted due to invalid index.
 * \uts{CSCSA-185600} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__sets_CED_alert_for_internal_alert_level_1_and_invalid_index)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter over threshold for normal
    * objects). */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]         = {};
   uint8_t obj_index                                                         = 3u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_qualifying_cycles;
   p_ced_cals->k_ced_slow_objects_long_vel_max                               = 0.0f;
   object_data[obj_index].vcs_vel.x = p_ced_cals->k_ced_slow_objects_long_vel_max + EPSILON;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                      = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                         = 4u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]                  = 4u;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                      = PA_INVALID_OBJ_INDEX;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                        = 2.0f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]      = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_holding_cycles;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are unchanged and qualifying counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT], 0u);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], 4u);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], 4u);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], 2.0f);
}

/**
 * Test that level 1 alert is reseted by debounce alert function if qualifying counter is below threshold for an object that is not
 * classified as slow object. \uts{CSCSA-41583} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__does_not_set_CED_alert_for_internal_alert_level_1_on_normal_obj_if_counter_below_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter below threshold for normal
    * objects). */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]          = {};
   uint8_t obj_index                                                          = 3u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = p_ced_cals->k_ced_alert_qualifying_cycles - 1u;
   p_ced_cals->k_ced_slow_objects_long_vel_max                                = 0.0f;
   object_data[obj_index].vcs_vel.x = p_ced_cals->k_ced_slow_objects_long_vel_max + EPSILON;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                      = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                         = 4u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]                  = 4u;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                      = obj_index;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                        = 2.0f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]      = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT] = p_ced_cals->k_ced_alert_holding_cycles;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are reseted and qualifying counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT], p_ced_cals->k_ced_alert_qualifying_cycles);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT], CED_INVALID_TIME);
}

/**
 * Test that level 1 alert is not reseted by debounce alert function if qualifying counter is over threshold for an object that is
 * classified as slow object. \uts{CSCSA-41584} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__sets_CED_alert_for_internal_alert_level_1_on_slow_obj_if_counter_over_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter over threshold for slow
    * objects). */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]         = {};
   uint8_t obj_index                                                         = 3u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_qualifying_cycles_slow_objects;
   p_ced_cals->k_ced_slow_objects_long_vel_max                               = 8.0f;
   object_data[obj_index].vcs_vel.x = p_ced_cals->k_ced_slow_objects_long_vel_max - EPSILON;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                      = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                         = 4u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]                  = 4u;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                      = obj_index;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                        = 2.0f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]      = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_holding_cycles;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are unchanged and qualifying counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT],
             p_ced_cals->k_ced_alert_qualifying_cycles_slow_objects + 1u);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], 4u);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], 4u);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], 2.0f);
}

/**
 * Test that level 1 alert is reseted by debounce alert function if qualifying counter is below threshold for an object that is
 * classified as slow object. \uts{CSCSA-41585} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__does_not_set_CED_alert_for_internal_alert_level_1_on_slow_obj_if_counter_below_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter below threshold for slow
    * objects). */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE] = {};
   uint8_t obj_index                                                 = 3u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT] =
      p_ced_cals->k_ced_alert_qualifying_cycles_slow_objects - 1u;
   p_ced_cals->k_ced_slow_objects_long_vel_max = 8.0f;
   object_data[obj_index].vcs_vel.x            = p_ced_cals->k_ced_slow_objects_long_vel_max - EPSILON;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                      = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                         = 4u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]                  = 4u;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                      = obj_index;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                        = 2.0f;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]      = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT] = p_ced_cals->k_ced_alert_holding_cycles;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are reseted and qualifying counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT],
             p_ced_cals->k_ced_alert_qualifying_cycles_slow_objects);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT], CED_INVALID_TIME);
}

/**
 * Tests that a previous alert is held by debounce alert function if holding counter is below threshold.
 * \uts{CSCSA-41586} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__holds_CED_alert_for_no_internal_alert_if_counter_below_threshold)
{
   /** \arrange Set up core output (with no alert) and persistent data (with alert level 1 and holding counter below threshold). */
   p_ced_cals->k_ced_alert_holding_cycles = 16u;
   uint8_t object_index                   = 2u;
   object_data[object_index].id           = object_index + 1u;
   object_data[object_index].unique_id    = object_index + 1u;
   object_data[object_index].index        = object_index;

   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]      = {};
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_holding_cycles - 1u;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                            = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT]    = p_ced_cals->k_ced_alert_qualifying_cycles;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]               = object_index + 1u;
   ced_instance.persistance.ced_side_unique_id_prev_cycle[FBK_SIDE_LEFT]        = object_index + 1u;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT] = 6u;

   p_ced_cals->k_ced_alert_holding_cycles = 16u;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are filled with holded alert and holding counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], 3u);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], 3u);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], 2u);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT],
             ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], p_ced_cals->k_ced_alert_holding_cycles);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Tests that a previous alert is held on one side and qualified on second side for negaitve object velocity.
 * \uts{CSCSA-186378} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__holds_CED_alert_and_qualify_for_negative_velocity_)
{
   /** \arrange Set up core output (with no alert) and persistent data (with alert level 1 and holding counter below threshold). */
   p_ced_cals->k_ced_alert_holding_cycles    = 16u;
   uint8_t object_index_left                 = 7u;
   uint8_t object_index_right                = 11u;
   object_data[object_index_left].id         = object_index_left + 1u;
   object_data[object_index_left].unique_id  = object_index_left + 1u;
   object_data[object_index_left].index      = object_index_left;
   object_data[object_index_right].id        = object_index_right + 1u;
   object_data[object_index_right].unique_id = object_index_right + 1u;
   object_data[object_index_right].index     = object_index_right;
   object_data[object_index_right].vcs_vel.x = -p_ced_cals->k_ced_slow_objects_long_vel_max + EPSILON;

   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]      = {};
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_holding_cycles - 1u;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                            = CED_NO_ALERT;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                           = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]                           = object_index_right;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]                            = object_index_left;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]                        = object_index_left + 1u;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                               = object_index_left + 1u;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                              = object_index_right + 1u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT]    = p_ced_cals->k_ced_alert_qualifying_cycles;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]           = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]               = object_index_left + 1u;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]              = object_index_right + 1u;
   ced_instance.persistance.ced_side_unique_id_prev_cycle[FBK_SIDE_LEFT]        = 99u;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT] = 6u;

   p_ced_cals->k_ced_alert_holding_cycles = 16u;

   /* Run Feature Building Kit Index-ID mapping */
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, ced_instance.core_input.p_pa_data);

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are filled with holded alert and holding counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], 12u);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT], 1u);

   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], 8u);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], 8u);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], 7u);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], p_ced_cals->k_ced_alert_holding_cycles);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT], 0u);
}


/**
 * Tests that a previous alert is not held by debounce alert function if holding counter is below threshold but the skip alert
 * holding flag is active on the previously alerted object. \uts{CSCSA-41587} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__does_not_hold_CED_alert_for_no_internal_alert_if_counter_below_threshold_but_skip_flag_active)
{
   /** \arrange Set up core output (with no alert) and persistent data (with alert level 1 and holding counter below threshold).
    * Additionally set skip alert holding flag to true for the previously alerted object. */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]      = {};
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_holding_cycles - 1u;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                         = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_qualifying_cycles;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]         = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]            = 7u;

   ced_object_f_skip_alert_holding[ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]] = FBK_TRUE;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are filled with standard-values and holding counter is also reseted. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Tests that a previous alert is not held by debounce alert function if holding counter is above threshold.
 * \uts{CSCSA-41588} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__does_not_hold_CED_alert_for_no_internal_alert_if_counter_above_threshold)
{
   /** \arrange Set up core output (with no alert) and persistent data (with alert level 1 and holding counter at threshold). */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]       = {};
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT] = p_ced_cals->k_ced_alert_holding_cycles;

   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                         = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = p_ced_cals->k_ced_alert_qualifying_cycles;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]         = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]            = 3u;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output and alert level are filled with standard-values and holding counter is incremented. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT], p_ced_cals->k_ced_alert_holding_cycles + 1u);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Tests that all counters are reseted by debounce alert function if no current or previous alert is present.
 * \uts{CSCSA-41589} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Debounce_Alert_Level__resets_counter_if_nothing_is_active)
{
   /** \arrange Set up core output (with no alert) and persistent data (with no alert and all counters greater zero). */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]          = {};
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT]  = 5u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = 6u;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT]     = 8u;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT]    = 4u;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                  = CED_NO_ALERT;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                 = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]  = CED_NO_ALERT;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_NO_ALERT;

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that all counter are reseted. */
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Tests that debounce alert function increases qualifying counter and resets core output for most critical object with internal
 * alert level CED_ALERT_QUALIFICATION. \uts{CSCSA-41590} \sdd{SF-3564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test,
       Ced_Debounce_Alert_Level__resets_core_output_and_increases_qualifying_counter_for_critical_object_in_alert_qualification)
{
   /** \arrange Set up core output with most critical object without alert and ttc close to threshold. */
   boolean_T ced_object_f_skip_alert_holding[CED_OBJ_MAX_ARRAY_SIZE]          = {};
   uint8_t counter_value                                                      = 4u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT]  = counter_value;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = counter_value;

   p_ced_cals->k_ced_alert_qualifying_cycles = 3u;

   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]     = CED_ALERT_QUALIFICATION;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]        = 7u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT] = 7u;
   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]     = 6u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT] =
      p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
      + 0.5f * (p_ced_cals->k_ced_alert_qualifying_cycles * ced_instance.core_input.p_pa_data->time_diff_to_last_cycle);
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]     = CED_ALERT_QUALIFICATION;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]        = 9u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT] = 9u;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT]     = 8u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT] =
      p_ced_cals->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]
      + 0.5f * (p_ced_cals->k_ced_alert_qualifying_cycles * ced_instance.core_input.p_pa_data->time_diff_to_last_cycle);

   /** \action Call function to debounce alert level. */
   Ced_Debounce_Alert_Level(&ced_instance.core_output, &ced_instance.persistance, &fbk_output, ced_object_f_skip_alert_holding,
                            p_ced_cals, ced_instance.core_input.p_pa_data, p_vehicle_data);

   /** \assert Verify that core output is reseted and qualifying counter is increased. */
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT], counter_value + 1u);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], CED_INVALID_TIME);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT], counter_value + 1u);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT], CED_INVALID_TIME);
}

/**
 * Tests that CED algorithm alerts on critical object on right ego side.
 * \uts{CSCSA-41591} \sdd{SF-3563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Algorithm__alerts_critical_obj_right)
{
   /** \arrange Set up calibration, persistent data and tracker output such that a critical object on the right ego side is
    * present. */
   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min + 1.0f;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -4.0f, 2.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   object_data[4u].vcs_heading                                                = 0.0f;
   object_data[4u].f_moveable                                                 = FBK_TRUE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]        = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]         = 0.0f;
   p_ced_cals->k_ced_slow_objects_long_vel_max                                = 0.0f;
   p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_LEFT]              = 1.5f;
   p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_RIGHT]             = 1.5f;
   p_ced_cals->k_ced_third_warning_ttc_threshold[FBK_SIDE_LEFT]               = 1.5f;
   p_ced_cals->k_ced_third_warning_ttc_threshold[FBK_SIDE_RIGHT]              = 1.5f;
   p_ced_cals->k_ced_f_second_warning_level_enable                            = FBK_TRUE;
   p_ced_cals->k_ced_f_third_warning_level_enable                             = FBK_TRUE;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                          = 0;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = p_ced_cals->k_ced_alert_qualifying_cycles + 1;

   /** \action Call the CED algorithm function. */
   Ced_Algorithm(&ced_instance.core_output, &ced_instance.persistance, p_vehicle_data, &ced_instance.core_input, p_ced_cals,
                 &fbk_output);

   /** \assert Verify that the core output shows an alert on the right ego side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_ALERT_ACTIVE_LEVEL_3);
}

/**
 * Tests that CED algorithm alerts on critical object on left ego side.
 * \uts{CSCSA-41592} \sdd{SF-3563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Algorithm__alerts_critical_obj_left)
{
   /** \arrange Set up calibration, persistent data and tracker output such that a critical object on the left ego side is present.
    */
   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min + 1.0f;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -4.0f, -2.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   object_data[4u].vcs_heading                                               = 0.0f;
   object_data[4u].f_moveable                                                = FBK_TRUE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]       = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]        = 0.0f;
   p_ced_cals->k_ced_slow_objects_long_vel_max                               = 0.0f;
   p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_LEFT]             = 1.5f;
   p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_RIGHT]            = 1.5f;
   p_ced_cals->k_ced_third_warning_ttc_threshold[FBK_SIDE_LEFT]              = 1.5f;
   p_ced_cals->k_ced_third_warning_ttc_threshold[FBK_SIDE_RIGHT]             = 1.5f;
   p_ced_cals->k_ced_f_second_warning_level_enable                           = FBK_TRUE;
   p_ced_cals->k_ced_f_third_warning_level_enable                            = FBK_TRUE;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                         = 0;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT] = p_ced_cals->k_ced_alert_qualifying_cycles + 1;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]         = CED_NO_ALERT;
   /** \action Call the CED algorithm function. */
   Ced_Algorithm(&ced_instance.core_output, &ced_instance.persistance, p_vehicle_data, &ced_instance.core_input, p_ced_cals,
                 &fbk_output);

   /** \assert Verify that the core output shows an alert on the left ego side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_ALERT_ACTIVE_LEVEL_3);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/**
 * Tests that CED algorithm creates no alert if no relevant object is present.
 * \uts{CSCSA-186379} \sdd{SF-3563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Algorithm__does_not_alert_if_no_relevant_obj_are_present)
{
   /** \arrange Set up calibration, persistent data and tracker output such that no relevant object is present. */
   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -20.0f, -10.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   object_data[4u].vcs_heading                                         = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call the CED algorithm function. */
   Ced_Algorithm(&ced_instance.core_output, &ced_instance.persistance, p_vehicle_data, &ced_instance.core_input, p_ced_cals,
                 &fbk_output);

   /** \assert Verify that the core output shows no alert on both ego side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/**
 * Tests that CED algorithm creates no alert if no critical object is present.
 * \uts{CSCSA-41593} \sdd{SF-3563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Algorithm__does_not_alert_if_no_critical_obj_are_present)
{
   /** \arrange Set up calibration, persistent data and tracker output such that no critical object is present. */
   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -Fbk_Half(p_ced_cals->k_ced_funnel_zone_length), -2.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   object_data[4u].vcs_heading                                         = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.5f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.5f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;

   /** \action Call the CED algorithm function. */
   Ced_Algorithm(&ced_instance.core_output, &ced_instance.persistance, p_vehicle_data, &ced_instance.core_input, p_ced_cals,
                 &fbk_output);

   /** \assert Verify that the core output shows no alert on both ego side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/**
 * Tests that object persistent data are reseted correctly by Ced_Reset_Objects_Persistent.
 * \uts{CSCSA-41594} \sdd{SF-3575} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Objects_Persistent__works_properly)
{
   /** \arrange Set object persistent data to non-default values. */
   uint8_t id                                                = 4u;
   ced_instance.persistance.ced_object_heading_predicted[id] = 1.2f;

   /** \action Call function to reset object persistent data. */
   Ced_Reset_Objects_Persistent(&ced_instance.persistance, fbk_output.p_index_id_lookup_table);

   /** \assert Verify that object persistent data is reseted to default values. */
   EXPECT_FLOAT_EQ(ced_instance.persistance.ced_object_heading_predicted[id], FBK_ZERO_F);
}

/**
 * Tests that persistent data are reseted correctly by Ced_Reset_Persistent_Data.
 * \uts{CSCSA-41595} \sdd{SF-3576} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Persistent_Data__works_properly)
{
   /** \arrange Set persistent data to non-default values. */
   ced_instance.persistance.ced_object_heading_predicted[5u]                     = 0.2f;
   ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT]        = 7u;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT]    = 4u;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]               = 6u;
   ced_instance.persistance.ced_side_unique_id_prev_cycle[FBK_SIDE_RIGHT]        = 6u;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = 6u;

   /** \action Call function to reset persistent data. */
   Ced_Reset_Persistent_Data(&ced_instance.persistance, fbk_output.p_index_id_lookup_table);

   /** \assert Verify that persistent data is reseted to default values. */
   EXPECT_FLOAT_EQ(ced_instance.persistance.ced_object_heading_predicted[5u], FBK_ZERO_F);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_holding_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_unique_id_prev_cycle[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT], PT_DEFAULT_MATCH_INDEX);
}

/**
 * Call reset object persistent data function. Verify that value is not rested due to valid object index.
 * \uts{CSCSA-112873} \sdd{SF-3575} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Objects_Persistent__valid_obj_index)
{
   /** \arrange Set persistent heading prediction value to non-default */
   uint8_t obj_idx                                               = 2;
   uint8_t obj_id                                                = 4;
   float32_T heading_pred_value                                  = 1.1f;
   ced_instance.persistance.ced_object_heading_predicted[obj_id] = heading_pred_value;
   object_data[obj_idx].id                                       = obj_id;
   object_data[obj_idx].index                                    = obj_idx;
   Fbk_Update_Index_Id_Lookup_Table(&empty_lookup_table, &data);

   /** \action Call function to reset persistent data */
   Ced_Reset_Persistent_Data(&ced_instance.persistance, &empty_lookup_table);
   /** \assert Verify that value is not reseted. */
   EXPECT_FLOAT_EQ(ced_instance.persistance.ced_object_heading_predicted[obj_id], heading_pred_value);
}


#ifndef NDEBUG
/**
 * Call reset object persistent data function with null pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41596} \sdd{SF-3575} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Objects_Persistent__persistent_pointer_null_throws_expection)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when Ced_Reset_Objects_Persistent is called with null pointer. */
   EXPECT_DEATH({ Ced_Reset_Objects_Persistent(NULL, NULL); }, ".*p_ced_persistent.*");
}

/**
 * Call reset persistent data function with null pointer. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41597} \sdd{SF-3576} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Persistent_Data__persistent_pointer_null_throws_expection)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when Ced_Reset_Persistent_Data is called with null pointer. */
   EXPECT_DEATH({ Ced_Reset_Persistent_Data(NULL, NULL); }, ".*p_ced_persistent.*");
}

/**
 * Call function for reset of core output side data with invalid side index. Verify that an exception is thrown by assertion.
 * \uts{CSCSA-41598} \sdd{SF-3574} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Core_Output_Side_Data__invalid_side_index_throws_expection)
{
   /** \arrange */
   /** \action */
   /** \assert Verify that an assert is thrown, when Ced_Reset_Core_Output_Side_Data is called with invalid side index. */
   EXPECT_DEATH({ Ced_Reset_Core_Output_Side_Data(&ced_instance.core_output, 23u); }, ".*side_index.*");
}
#endif

/**
 * Test that reset of core output side data works properly for right ego side.
 * \uts{CSCSA-41599} \sdd{SF-3574} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Core_Output_Side_Data__works_properly_for_right_side)
{
   /** \arrange Set up core output data with non-default values. */
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                   = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                      = 4u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT]               = 4u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                     = 2.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT] = 4u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                      = 2.0f;


   /** \action Call Ced_Reset_Core_Output_Side_Data to reset core output for right side. */
   Ced_Reset_Core_Output_Side_Data(&ced_instance.core_output, FBK_SIDE_RIGHT);

   /** \assert Verify that core output is reseted for right side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT], CED_INVALID_TIME);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_RIGHT], PT_DEFAULT_MATCH_INDEX);
   EXPECT_NE(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], CED_INVALID_TIME);
}

/**
 * Test that reset of core output side data works properly for left ego side.
 * \uts{CSCSA-41600} \sdd{SF-3574} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Reset_Core_Output_Side_Data__works_properly_for_left_side)
{
   /** \arrange Set up core output data with non-default values. */
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                   = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                      = 4u;
   ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT]               = 4u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                     = 2.0f;
   ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT] = 4u;
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                    = 2.0f;

   /** \action Call Ced_Reset_Core_Output_Side_Data to reset core output for left side. */
   Ced_Reset_Core_Output_Side_Data(&ced_instance.core_output, FBK_SIDE_LEFT);

   /** \assert Verify that core output is reseted for left side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(ced_instance.core_output.ced_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT], CED_INVALID_TIME);
   EXPECT_EQ(ced_instance.core_output.ced_object_path_match_index[FBK_SIDE_LEFT], PT_DEFAULT_MATCH_INDEX);
   EXPECT_NE(ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT], CED_INVALID_TIME);
}


/**
 * Tests whether an alert shall be suppressed based on nearest path information. Here the object alert shall be suppressed, since
 * the new object behaves different from its nearest recorded path trajectory. \uts{CSCSA-41601} \sdd{SF-3391}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__object_alerted_shall_be_suppressed)
{
   /** \arrange Enable path tracking and create a scenario where object behaves different from the recorded path. */
   boolean_T result;
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   ced_object.tracker_data.index = object_index;
   ced_object.tracker_data.age   = p_ced_cals->k_ced_suppress_alert_object_age_max;

   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   pt_match_info->track_match_age            = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress - 1u;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 0.0f;
   pt_match_info->range_at_zero              = 0.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   pt_nearest_path_info->range_vcs_proj_to_path_segment = p_ced_cals->k_ced_suppress_range_to_nearest_path_max + 0.1f;
   pt_nearest_path_info->segment_heading_diff           = p_ced_cals->k_ced_suppress_pt_heading_diff_ced_alert_max;
   pt_nearest_path_info->track_idx_nearest_path         = 0u;
   ced_object.attributes.p_pt_nearest_path_info         = pt_nearest_path_info;

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is suppressed. */
   EXPECT_TRUE(result);
}


/**
 * Tests whether an alert shall be suppressed based on nearest path information. Here the object alert shall not be suppressed,
 * since the object age exceeds its threshold. \uts{CSCSA-41602} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test,
       Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__object_alerted_shall_not_be_suppressed_since_age_exceeds_threshold)
{
   /** \arrange Enable path tracking and create a scenario where object behaves different from the recorded path. But the age
    * exceeded the threshold. */
   boolean_T result;
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   ced_object.tracker_data.index = object_index;
   ced_object.tracker_data.age   = p_ced_cals->k_ced_suppress_alert_object_age_max + 1u;

   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   pt_match_info->track_match_age            = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress - 1u;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 0.0f;
   pt_match_info->range_at_zero              = 0.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   pt_nearest_path_info->range_vcs_proj_to_path_segment = p_ced_cals->k_ced_suppress_range_to_nearest_path_max + EPSILON;
   pt_nearest_path_info->segment_heading_diff           = p_ced_cals->k_ced_suppress_pt_heading_diff_ced_alert_max;
   pt_nearest_path_info->track_idx_nearest_path         = 0u;
   ced_object.attributes.p_pt_nearest_path_info         = pt_nearest_path_info;

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}


/**
 * Tests whether an alert shall be suppressed based on nearest path information. Here the object alert shall not be suppressed,
 * since the object is too far away from its nearest path. \uts{CSCSA-41603} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__object_alerted_shall_not_be_suppressed_object_is_too_far_away)
{
   /** \arrange Enable path tracking and create a scenario where object behaves different from the recorded path. However it is too
    * far away from its nearest path. */
   boolean_T result;
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   ced_object.tracker_data.index = object_index;
   ced_object.tracker_data.age   = p_ced_cals->k_ced_suppress_alert_object_age_max;

   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   pt_match_info->track_match_age            = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress - 1u;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 0.0f;
   pt_match_info->range_at_zero              = 0.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   pt_nearest_path_info->range_vcs_proj_to_path_segment = p_ced_cals->k_ced_suppress_range_to_nearest_path_max;
   pt_nearest_path_info->segment_heading_diff           = p_ced_cals->k_ced_suppress_pt_heading_diff_ced_alert_max;
   pt_nearest_path_info->track_idx_nearest_path         = 0u;
   ced_object.attributes.p_pt_nearest_path_info         = pt_nearest_path_info;

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}

/**
 * Tests whether an alert shall be suppressed based on nearest path information. Here the object alert shall not be suppressed,
 * since the object heading difference is too small. \uts{CSCSA-41604} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test,
       Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__object_alerted_shall_not_be_suppressed_heading_difference_too_small)
{
   /** \arrange Enable path tracking and create a scenario where object behaves similar to its nearest path. */
   boolean_T result;
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   ced_object.tracker_data.index = object_index;
   ced_object.tracker_data.age   = p_ced_cals->k_ced_suppress_alert_object_age_max;

   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   pt_match_info->track_match_age            = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress - 1u;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 0.0f;
   pt_match_info->range_at_zero              = 0.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   pt_nearest_path_info->range_vcs_proj_to_path_segment = p_ced_cals->k_ced_suppress_range_to_nearest_path_max;
   pt_nearest_path_info->segment_heading_diff           = 0.9f * p_ced_cals->k_ced_suppress_pt_heading_diff_ced_alert_max;
   pt_nearest_path_info->track_idx_nearest_path         = 0u;
   ced_object.attributes.p_pt_nearest_path_info         = pt_nearest_path_info;

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}


/**
 * Tests whether an alert shall be suppressed based on nearest path information. Here the object alert shall not be suppressed,
 * since the object is matched to a valid path. \uts{CSCSA-41605} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test,
       Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__object_alerted_shall_not_be_suppressed_since_obj_is_matched_to_a_valid_path)
{
   /** \arrange Enable path tracking and object is matched to a valid path. */
   boolean_T result;
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   ced_object.tracker_data.index = object_index;
   ced_object.tracker_data.age   = p_ced_cals->k_ced_suppress_alert_object_age_max;

   pt_match_info->track_match                = PT_DEFAULT_MATCH_INDEX;
   pt_match_info->track_match_age            = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 0.0f;
   pt_match_info->range_at_zero              = 0.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   pt_nearest_path_info->range_vcs_proj_to_path_segment = p_ced_cals->k_ced_suppress_range_to_nearest_path_max;
   pt_nearest_path_info->segment_heading_diff           = p_ced_cals->k_ced_suppress_pt_heading_diff_ced_alert_max;
   pt_nearest_path_info->track_idx_nearest_path         = 0u;
   ced_object.attributes.p_pt_nearest_path_info         = pt_nearest_path_info;

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}


/**
 * Tests whether an alert shall be suppressed based on nearest path information. Alert shall not be suppressed, since the object is
 * matched to a valid path. \uts{CSCSA-186380} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__alert_not_suppresed_due_to_valid_path)
{
   /** \arrange Enable path tracking and object is matched to a valid path. */
   boolean_T result;
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   ced_object.tracker_data.index = object_index;
   ced_object.tracker_data.age   = p_ced_cals->k_ced_suppress_alert_object_age_max;

   pt_match_info->track_match                   = 8u;
   pt_match_info->track_match_age               = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress;
   ced_object.attributes.p_pt_match_info        = pt_match_info;
   pt_nearest_path_info->track_idx_nearest_path = 0u;
   ced_object.attributes.p_pt_nearest_path_info = pt_nearest_path_info;

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}

/**
 * Tests invalid nearest track.
 * \uts{CSCSA-185602} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__invalid_nearest_track)
{
   /** \arrange Enable path tracking and object is matched to a valid path. */
   boolean_T result;
   Ced_Object_T ced_object{};
   uint8_t object_index                        = 4u;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[object_index];
   Pt_Nearest_Path_T *pt_nearest_path_info     = &pt_output.nearest_path_output[object_index];

   ced_object.tracker_data.index = object_index;
   ced_object.tracker_data.age   = p_ced_cals->k_ced_suppress_alert_object_age_max;

   pt_match_info->track_match                = 0u;
   pt_match_info->track_match_age            = p_ced_cals->k_ced_min_cycles_for_path_match_for_no_suppress;
   pt_match_info->path_heading               = 0.0f;
   pt_match_info->path_direction             = PATH_DIRECTION_LONG_FORWARD;
   pt_match_info->range_to_current_path_part = 0.0f;
   pt_match_info->range_at_host_edge         = 0.0f;
   pt_match_info->range_at_zero              = 0.0f;
   ced_object.attributes.p_pt_match_info     = pt_match_info;

   pt_nearest_path_info->range_vcs_proj_to_path_segment = p_ced_cals->k_ced_suppress_range_to_nearest_path_max;
   pt_nearest_path_info->segment_heading_diff           = p_ced_cals->k_ced_suppress_pt_heading_diff_ced_alert_max;
   pt_nearest_path_info->track_idx_nearest_path         = PT_DEFAULT_MATCH_INDEX;
   ced_object.attributes.p_pt_nearest_path_info         = pt_nearest_path_info;

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}

/**
 * Tests whether an alert shall be suppressed based on nearest path information. Here the object alert shall not be suppressed,
 * since path tracking output pointer is null. \uts{CSCSA-41606} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test,
       Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__object_alerted_shall_not_be_suppressed_path_tracking_output_pointer_is_null)
{
   /** \arrange Enable path tracking. No pt output info is given. */
   boolean_T result;
   Ced_Object_T ced_object{};

   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}


/**
 * Tests whether an alert shall be suppressed based on nearest path information. Here the object alert shall not be suppressed,
 * since path tracking is deactivated. \uts{CSCSA-41607} \sdd{SF-3391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path__pt_is_deactivated)
{
   /** \arrange disable path tracking. */
   boolean_T result;
   Ced_Object_T ced_object{};

   p_ced_cals->k_ced_f_path_tracking_enable = FBK_FALSE;
   /** \action Call Object suppression based on nearest path check. */
   result = Ced_Shall_Obj_Alert_Be_Suppressed_Based_On_Nearest_Path(&ced_object, p_ced_cals);

   /** \assert Verify that object alert is not suppressed. */
   EXPECT_FALSE(result);
}

/**
 * Tests whether a given object is parking on the ego lane. Here the object is parking in the ego lane and thus true is expected.
 * \uts{CSCSA-41608} \sdd{SF-3393} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Parking_On_Ego_Lane__obj_is_parking_in_ego_lane)
{
   /** \arrange set up an object parking on ego lane. */
   boolean_T result;
   Ced_Object_T ced_object{};
   ced_object.tracker_data.speed              = p_ced_cals->k_ced_ego_lane_parking_maneuver_speed - EPSILON;
   ced_object.tracker_data.vcs_pos.x          = p_ced_cals->k_ced_ego_lane_parking_range - EPSILON;
   ced_object.attributes.position_predicted.y = 0.5f * p_ced_cals->k_ced_ego_lane_width - EPSILON;

   /** \action Call object parking on ego lane check. */
   result = Ced_Is_Object_Parking_On_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert expect true. */
   EXPECT_TRUE(result);
}

/**
 * Tests whether a given object is parking on the ego lane. Here the object is parking in the ego lane shifted to left side,
 * approaching from front, and negative speed value, thus true is expected. \uts{CSCSA-186381} \sdd{SF-3393}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Parking_On_Ego_Lane__obj_is_parking_in_ego_lane_approaching_front)
{
   /** \arrange set up an object parking on ego lane. */
   boolean_T result;
   Ced_Object_T ced_object{};
   ced_object.tracker_data.speed              = -p_ced_cals->k_ced_ego_lane_parking_maneuver_speed + EPSILON;
   ced_object.tracker_data.vcs_pos.x          = -p_ced_cals->k_ced_ego_lane_parking_range + EPSILON;
   ced_object.attributes.position_predicted.y = 0.5f * (-p_ced_cals->k_ced_ego_lane_width) + EPSILON;

   /** \action Call object parking on ego lane check. */
   result = Ced_Is_Object_Parking_On_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert expect true. */
   EXPECT_TRUE(result);
}


/**
 * Tests whether a given object is parking on the ego lane. Here the object is not parking in the ego lane and thus false is
 * expected. \uts{CSCSA-41609} \sdd{SF-3393} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Parking_On_Ego_Lane__obj_is_not_parking_in_ego_lane_since_prediction_position_gt_boundary)
{
   /** \arrange set up an object which is not parking on ego lane. */
   boolean_T result;
   Ced_Object_T ced_object{};
   ced_object.tracker_data.speed              = p_ced_cals->k_ced_ego_lane_parking_maneuver_speed - EPSILON;
   ced_object.tracker_data.vcs_pos.x          = p_ced_cals->k_ced_ego_lane_parking_range - EPSILON;
   ced_object.attributes.position_predicted.y = p_ced_cals->k_ced_ego_lane_width;

   /** \action Call object parking on ego lane check. */
   result = Ced_Is_Object_Parking_On_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert expect false. */
   EXPECT_FALSE(result);
}


/**
 * Tests whether a given object is parking on the ego lane. Here the object is not parking in the ego lane since it is too fast and
 * thus false is expected. \uts{CSCSA-41610} \sdd{SF-3393} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Parking_On_Ego_Lane__obj_is_not_parking_in_ego_lane_since_speed_gt_boundary)
{
   /** \arrange set up an object which is not parking on ego lane. */
   boolean_T result;
   Ced_Object_T ced_object{};
   ced_object.tracker_data.speed              = p_ced_cals->k_ced_ego_lane_parking_maneuver_speed + EPSILON;
   ced_object.tracker_data.vcs_pos.x          = p_ced_cals->k_ced_ego_lane_parking_range;
   ced_object.attributes.position_predicted.y = 0.5f * p_ced_cals->k_ced_ego_lane_width - EPSILON;

   /** \action Call object parking on ego lane check. */
   result = Ced_Is_Object_Parking_On_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert expect false. */
   EXPECT_FALSE(result);
}


/**
 * Tests whether a given object is parking on the ego lane. Here the object is not parking in the ego lane since it is too far away
 * and thus false is expected. \uts{CSCSA-41611} \sdd{SF-3393} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Parking_On_Ego_Lane__obj_is_not_parking_in_ego_lane_since_long_position_gt_boundary)
{
   /** \arrange set up an object which is not parking on ego lane. */
   boolean_T result;
   Ced_Object_T ced_object{};
   ced_object.tracker_data.speed              = p_ced_cals->k_ced_ego_lane_parking_maneuver_speed - EPSILON;
   ced_object.tracker_data.vcs_pos.x          = p_ced_cals->k_ced_ego_lane_parking_range + EPSILON;
   ced_object.attributes.position_predicted.y = 0.5f * p_ced_cals->k_ced_ego_lane_width - EPSILON;

   /** \action Call object parking on ego lane check. */
   result = Ced_Is_Object_Parking_On_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert expect false. */
   EXPECT_FALSE(result);
}

/**
 * Tests that a both sides alert is restricted to the object side, if this is enabled by calibration value.
 * \uts{CSCSA-41612} \sdd{SF-3440} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Object_Alert_Suppression__restricts_both_side_alert_to_object_side_if_enabled_by_cal)
{
   /** \arrange Set up object on left ego side with alert on both sides and enable restriction of alert side in calibrations. */
   Ced_Object_T ced_object{};
   ced_object.attributes.alert_side = INTERSEC_BOTH_SIDES;
   ced_object.attributes.ego_side   = EGO_LEFT_SIDE;

   p_ced_cals->k_ced_f_handle_both_side_alerts_as_object_side = FBK_TRUE;
   p_ced_cals->k_ced_allow_opposite_side_alerts               = 2u;
   p_ced_cals->k_ced_f_allow_ego_lane_alerts                  = FBK_TRUE;
   p_ced_cals->k_ced_f_allow_coasted_object_alerts            = FBK_TRUE;
   p_ced_cals->k_ced_f_path_tracking_enable                   = FBK_FALSE;

   /** \action Call Ced_Get_Object_Alert_Suppression to get alert suppresion reason. */
   Ced_Alert_Suppression_T suppression_reason = Ced_Get_Object_Alert_Suppression(&ced_object, p_ced_cals, &ced_instance.core_input);

   /** \assert Expect no alert suppresion reason, but restriction of alert to left side. */
   EXPECT_EQ(ced_object.attributes.alert_side, INTERSEC_LEFT_SIDE);
   EXPECT_EQ(suppression_reason, CED_SUPPRESS_NO_ALERT);
}


/**
 * Tests that a alert on both side for targert in ego lane is suppresed because object is coasted.
 * \uts{CSCSA-186382} \sdd{SF-3440} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Object_Alert_Suppression__object_in_ego_lane_no_suppress)
{
   /** \arrange Set up object in ego lane with alert on both sides and enable restriction of alert side in calibrations. */
   Ced_Object_T ced_object{};
   ced_object.attributes.alert_side              = INTERSEC_BOTH_SIDES;
   ced_object.attributes.ego_side                = EGO_LANE;
   ced_object.tracker_data.f_is_in_rl_sensor_fov = FBK_TRUE;
   ced_object.tracker_data.status                = PA_OBJ_STATUS_COASTED;

   p_ced_cals->k_ced_f_handle_both_side_alerts_as_object_side = FBK_TRUE;
   p_ced_cals->k_ced_allow_opposite_side_alerts               = 2u;
   p_ced_cals->k_ced_f_allow_ego_lane_alerts                  = FBK_TRUE;
   p_ced_cals->k_ced_f_allow_coasted_object_alerts            = FBK_FALSE;
   p_ced_cals->k_ced_f_path_tracking_enable                   = FBK_FALSE;

   /** \action Call Ced_Get_Object_Alert_Suppression to get alert suppresion reason. */
   Ced_Alert_Suppression_T suppression_reason = Ced_Get_Object_Alert_Suppression(&ced_object, p_ced_cals, &ced_instance.core_input);

   /** \assert Expect alert suppresion reason. */
   EXPECT_EQ(ced_object.attributes.alert_side, INTERSEC_BOTH_SIDES);
   EXPECT_EQ(suppression_reason, CED_SUPPRESS_COASTED_OBJECT_ALERT);
}

/**
 * Tests that for undefined alert side for targert in ego lane is not suppresed.
 * \uts{CSCSA-186383} \sdd{SF-3440} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Object_Alert_Suppression__undefined_alert_side_object_in_ego_lane_no_suppress)
{
   /** \arrange Set up object in ego lanee with undefined alert side and enable restriction of alert side in calibrations. */
   Ced_Object_T ced_object{};
   ced_object.attributes.alert_side = INTERSEC_UNDEF_SIDE;
   ced_object.attributes.ego_side   = EGO_LANE;
   ced_object.tracker_data.status   = PA_OBJ_STATUS_MATURE;

   p_ced_cals->k_ced_f_handle_both_side_alerts_as_object_side = FBK_TRUE;
   p_ced_cals->k_ced_allow_opposite_side_alerts               = 2u;
   p_ced_cals->k_ced_f_allow_ego_lane_alerts                  = FBK_TRUE;
   p_ced_cals->k_ced_f_allow_coasted_object_alerts            = FBK_FALSE;
   p_ced_cals->k_ced_f_path_tracking_enable                   = FBK_FALSE;

   /** \action Call Ced_Get_Object_Alert_Suppression to get alert suppresion reason. */
   Ced_Alert_Suppression_T suppression_reason = Ced_Get_Object_Alert_Suppression(&ced_object, p_ced_cals, &ced_instance.core_input);

   /** \assert Expect no alert suppresion reason, but restriction of alert to left side. */
   EXPECT_EQ(ced_object.attributes.alert_side, INTERSEC_UNDEF_SIDE);
   EXPECT_EQ(suppression_reason, CED_SUPPRESS_NO_ALERT);
}


/**
 * Tests that the default case for the switch statement is covered.
 * \uts{CSCSA-41613} \sdd{SF-3440} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Object_Alert_Suppression__restricts_both_side_alert_to_object_side_default_case)
{
   /** \arrange Set up object on left ego side with alert on both sides and enable restriction of alert side in calibrations. */
   Ced_Object_T ced_object{};
   ced_object.attributes.alert_side = INTERSEC_BOTH_SIDES;
   ced_object.attributes.ego_side   = UNDEF_SIDE;

   p_ced_cals->k_ced_f_handle_both_side_alerts_as_object_side = FBK_TRUE;

   /** \action Call Ced_Get_Object_Alert_Suppression to get alert suppresion reason. */
   Ced_Alert_Suppression_T suppression_reason = Ced_Get_Object_Alert_Suppression(&ced_object, p_ced_cals, &ced_instance.core_input);

   /** \assert Expect no alert suppresion reason, but restriction of alert to left side. */
   EXPECT_EQ(ced_object.attributes.alert_side, INTERSEC_UNDEF_SIDE);
   EXPECT_EQ(suppression_reason, CED_SUPPRESS_NO_ALERT);
}

/**
 * Tests that suppress reason is crossing border line.
 * \uts{CSCSA-41614} \sdd{SF-3440} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Object_Alert_Suppression__object_cross_the_border_line)
{
   /** \arrange Set up object on right ego side. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   Ced_Fill_Raw_Tracker_Output(obj_index, -10.0f, 0.7f, 2.0f, 0.0f, 0.5f, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.vcs_heading = 0.0f;

   p_ced_cals->k_ced_f_object_lat_on_one_side_of_border       = FBK_TRUE;
   p_ced_cals->k_ced_f_handle_both_side_alerts_as_object_side = FBK_FALSE;
   p_ced_cals->k_ced_f_allow_coasted_object_alerts            = FBK_FALSE;
   p_ced_cals->k_ced_lat_pos_of_border                        = 1.0f;

   /** \action Call Ced_Get_Object_Alert_Suppression to get alert suppresion reason. */
   Ced_Alert_Suppression_T suppression_reason = Ced_Get_Object_Alert_Suppression(&ced_object, p_ced_cals, &ced_instance.core_input);

   /** \assert Expect suppresion reason. */
   EXPECT_EQ(suppression_reason, CED_SUPPRESS_CROSS_BORDER);
}


/**
 * Tests that the returned distance is considered zero for objects that lie across the host center line.
 * \uts{CSCSA-41615} \sdd{SF-3451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Closest_Predicted_Lateral_Distance__object_zone_detected_to_be_across_host_center_line)
{
   /** \arrange */
   Fbk_Field_Of_Interest_T object_zone;
   object_zone.size        = 4u;
   object_zone.points[0].y = -1.0f;
   object_zone.points[1].y = 1.0f;
   object_zone.points[2].y = 1.0f;
   object_zone.points[3].y = -1.0f;

   /** \action */
   float32_T closest_pred_lat_dist = Ced_Get_Closest_Predicted_Lateral_Distance(&object_zone);

   /** \assert */
   EXPECT_FLOAT_EQ(closest_pred_lat_dist, FBK_ZERO_F);
}

/**
 * Tests that the returned distance is smallest value of the lateral object points if all points lie on one host vehicle side.
 * \uts{CSCSA-41616} \sdd{SF-3451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Closest_Predicted_Lateral_Distance__object_zone_on_one_host_right_side)
{
   /** \arrange */
   Fbk_Field_Of_Interest_T object_zone;
   object_zone.size        = 4u;
   object_zone.points[0].y = 0.5f;
   object_zone.points[1].y = 1.5f;
   object_zone.points[2].y = 1.6f;
   object_zone.points[3].y = 0.6f;

   /** \action */
   float32_T closest_pred_lat_dist = Ced_Get_Closest_Predicted_Lateral_Distance(&object_zone);

   /** \assert */
   EXPECT_FLOAT_EQ(closest_pred_lat_dist, Fbk_Abs_F(object_zone.points[0].y));
}

/**
 * Tests that the returned distance is smallest value of the lateral object points if all points lie on one host vehicle side.
 * \uts{CSCSA-41617} \sdd{SF-3451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Closest_Predicted_Lateral_Distance__object_zone_on_one_host_left_side)
{
   /** \arrange */
   Fbk_Field_Of_Interest_T object_zone;
   object_zone.size        = 4u;
   object_zone.points[0].y = -1.5f;
   object_zone.points[1].y = -0.5f;
   object_zone.points[2].y = -0.6f;
   object_zone.points[3].y = -1.6f;

   /** \action */
   float32_T closest_pred_lat_dist = Ced_Get_Closest_Predicted_Lateral_Distance(&object_zone);

   /** \assert */
   EXPECT_FLOAT_EQ(closest_pred_lat_dist, Fbk_Abs_F(object_zone.points[1].y));
}

/**
 * Tests that the returned distance is when one point is on opposite side.
 * \uts{CSCSA-186384} \sdd{SF-3451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Closest_Predicted_Lateral_Distance__object_zone_one_point_on_opposite_side)
{
   /** \arrange */
   Fbk_Field_Of_Interest_T object_zone;
   object_zone.size        = 4u;
   object_zone.points[0].y = 0.5;
   object_zone.points[1].y = 1.0f;
   object_zone.points[2].y = 1.0f;
   object_zone.points[3].y = -0.5f;

   /** \action */
   float32_T closest_pred_lat_dist = Ced_Get_Closest_Predicted_Lateral_Distance(&object_zone);

   /** \assert */
   EXPECT_FLOAT_EQ(closest_pred_lat_dist, Fbk_Abs_F(FBK_ZERO_F));
}

/**
 * Tests that the returned distance is when two points is on opposite side.
 * \uts{CSCSA-186385} \sdd{SF-3451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Closest_Predicted_Lateral_Distance__object_zone_two_points_on_opposite_side)
{
   /** \arrange */
   Fbk_Field_Of_Interest_T object_zone;
   object_zone.size        = 4u;
   object_zone.points[0].y = 0.5;
   object_zone.points[1].y = 2.0f;
   object_zone.points[2].y = -0.1f;
   object_zone.points[3].y = -1.0f;

   /** \action */
   float32_T closest_pred_lat_dist = Ced_Get_Closest_Predicted_Lateral_Distance(&object_zone);

   /** \assert */
   EXPECT_FLOAT_EQ(closest_pred_lat_dist, Fbk_Abs_F(FBK_ZERO_F));
}


/**
 * Tests that the returned distance is zero when zone is zero.
 * \uts{CSCSA-112874} \sdd{SF-3451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Closest_Predicted_Lateral_Distance__object_zone_set_to_zero)
{
   /** \arrange */
   Fbk_Field_Of_Interest_T object_zone;
   object_zone.size        = 4u;
   object_zone.points[0].y = 0.0;
   object_zone.points[1].y = 0.0f;
   object_zone.points[2].y = 0.0f;
   object_zone.points[3].y = 0.0f;

   /** \action */
   float32_T closest_pred_lat_dist = Ced_Get_Closest_Predicted_Lateral_Distance(&object_zone);

   /** \assert */
   EXPECT_FLOAT_EQ(closest_pred_lat_dist, Fbk_Abs_F(FBK_ZERO_F));
}


/**
 * Tests that Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle function returns true if object is matched to same path as
 * object that caused an alert on left side. \uts{CSCSA-41618} \sdd{SF-3452} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle__returns_TRUE_for_matching_path_left)
{
   /** \arrange Set up persistent data */
   uint8_t obj_path_match_index = 4u;
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   pt_match_info->track_match                  = obj_path_match_index;
   ced_object.attributes.p_pt_match_info       = pt_match_info;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = obj_path_match_index;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = PT_DEFAULT_MATCH_INDEX;

   /** \action Call function Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle. */
   boolean_T result = Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(&ced_object, &ced_instance.persistance);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle function returns true if object is matched to same path as
 * object that caused an alert on right side. \uts{CSCSA-41619} \sdd{SF-3452} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle__returns_TRUE_for_matching_path_right)
{
   /** \arrange Set up persistent data */
   uint8_t obj_path_match_index = 4u;
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   pt_match_info->track_match                  = obj_path_match_index;
   ced_object.attributes.p_pt_match_info       = pt_match_info;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = obj_path_match_index;

   /** \action Call function Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle. */
   boolean_T result = Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(&ced_object, &ced_instance.persistance);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Tests that Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle function returns false if there was no alert in previous
 * cycle. \uts{CSCSA-41620} \sdd{SF-3452} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle__returns_FALSE_for_no_prev_obj_alert)
{
   /** \arrange Set up persistent data */
   uint8_t obj_path_match_index = 4u;
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   pt_match_info->track_match                  = obj_path_match_index;
   ced_object.attributes.p_pt_match_info       = pt_match_info;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_NO_ALERT;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = PT_DEFAULT_MATCH_INDEX;

   /** \action Call function Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle. */
   boolean_T result = Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(&ced_object, &ced_instance.persistance);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Tests that Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle function returns false if object is not matched to same path
 * as objects that caused an alert on any side. \uts{CSCSA-41621} \sdd{SF-3452} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle__returns_FALSE_for_not_matching_path)
{
   /** \arrange Set up persistent data */
   uint8_t obj_path_match_index = 4u;
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   pt_match_info->track_match                  = obj_path_match_index;
   ced_object.attributes.p_pt_match_info       = pt_match_info;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = 6u;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = 3u;

   /** \action Call function Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle. */
   boolean_T result = Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(&ced_object, &ced_instance.persistance);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Tests that Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle function returns false if object has no path match.
 * \uts{CSCSA-41622} \sdd{SF-3452} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle__returns_FALSE_for_no_path_match)
{
   /** \arrange Set up persistent data */
   uint8_t obj_path_match_index = PT_DEFAULT_MATCH_INDEX;
   Ced_Object_T ced_object{};
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   pt_match_info->track_match                  = obj_path_match_index;
   ced_object.attributes.p_pt_match_info       = pt_match_info;

   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]             = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_LEFT]  = PT_DEFAULT_MATCH_INDEX;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT]            = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_path_match_index_prev_cycle[FBK_SIDE_RIGHT] = PT_DEFAULT_MATCH_INDEX;

   /** \action Call function Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle. */
   boolean_T result = Ced_Is_Object_Matched_To_Path_With_Alert_In_Prev_Cycle(&ced_object, &ced_instance.persistance);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}


/**
 * Tests that Ced_Flip_Heading_Value function returns the correct flipped value for the given positive input.
 * \uts{CSCSA-41623} \sdd{SF-3454} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Flip_Heading_Value__flip_positive_value)
{
   /** \arrange Set up data */
   float32_T input_value = Fbk_Deg_To_Rad(178);

   /** \action Call function Ced_Flip_Heading_Value. */
   float32_T result = Ced_Flip_Heading_Value(input_value);

   /** \assert Verify that the correct flipped value is returned. */
   EXPECT_NEAR(result, Fbk_Deg_To_Rad(-2), EPSILON);
}

/**
 * Tests that Ced_Flip_Heading_Value function returns the correct flipped value for the given negative input.
 * \uts{CSCSA-41624} \sdd{SF-3454} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Flip_Heading_Value__flip_negative_value)
{
   /** \arrange Set up data */
   float32_T input_value = Fbk_Deg_To_Rad(-178);

   /** \action Call function Ced_Flip_Heading_Value. */
   float32_T result = Ced_Flip_Heading_Value(input_value);

   /** \assert Verify that the correct flipped value is returned. */
   EXPECT_NEAR(result, Fbk_Deg_To_Rad(2), EPSILON);
}

/**
 * Define object such that it is validated as not approaching from rear and the relative speed x speed is beyond the limit.
 * \uts{CSCSA-41625} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__relative_speed_x_limit_min)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and the reltive speed x speed is over the max.
 * \uts{CSCSA-41626} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__relative_speed_x_limit_max)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_max + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and the tracker data speed x speed is over the max.
 * \uts{CSCSA-41627} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__data_tracker_speed_x_limit_max)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;
   ced_object.tracker_data.speed                 = p_ced_cals->k_ced_object_vel_max + EPSILON;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and the position x element below lenght.
 * \uts{CSCSA-41628} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__x_position_below_length)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;
   ced_object.tracker_data.length                = FBK_ONE_F;
   ced_object.tracker_data.vcs_pos.x             = ced_object.tracker_data.length + EPSILON;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and data age is too low.
 * \uts{CSCSA-41629} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__data_age_too_low)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and y velocity is over max.
 * \uts{CSCSA-41630} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__y_velocity_over_max)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max + EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and y velocity is over max (negative value, for better
 * branch coverage). \uts{CSCSA-41631} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__y_velocity_over_max_negative)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = -p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and heading angle is to high.
 * \uts{CSCSA-41632} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__heading_angle_over_max)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_heading_abs_angle_max + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}

/**
 * Define object such that it is validated as not approaching from rear and heading angle is to high (negative value, for better
 * branch coverity). \uts{CSCSA-41633} \sdd{SF-3445} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Rear_Object_Relevant__heading_angle_over_max_negative)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = p_ced_cals->k_ced_object_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = p_ced_cals->k_ced_object_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = -p_ced_cals->k_ced_object_heading_abs_angle_max - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Rear_Object_Relevant to evaluate if object is approaching from rear. */
   boolean_T f_approaches_from_rear = Ced_Is_Rear_Object_Relevant(&ced_object, p_ced_cals);

   /** \assert Verify that approach from rear is not detected. */
   EXPECT_FALSE(f_approaches_from_rear);
}
/**
 * Define object such that it is not validated as approaching from front and relative speed is below min
 * \uts{CSCSA-41634} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__relative_speed_below_min)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front and relative speed is over max
 * \uts{CSCSA-41635} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__relative_speed_over_max)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_max - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front and x velocity element beyond the limit
 * \uts{CSCSA-41636} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__x_velocity_beyond_limit)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min + EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}


/**
 * Define object such that it is not validated as approaching from front and tracker data speed is over max
 * \uts{CSCSA-41637} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__tracker_data_speed_over_max)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;
   ced_object.tracker_data.speed                 = p_ced_cals->k_ced_object_vel_max + EPSILON;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front and y velocity is over max
 * \uts{CSCSA-41638} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__y_velocity_over_max)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max + EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front and y velocity is over max (with negative value, for
 * better branch coverage) \uts{CSCSA-41639} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__y_velocity_over_max_negative)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = -p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front and heading angle is beyond limit
 * \uts{CSCSA-41640} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__heading_angle_below_min)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front and heading angle is beyond limit (with negative value,
 * for proper branch coverage) \uts{CSCSA-41641} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__heading_angle_below_min_negative)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = -p_ced_cals->k_ced_object_ftm_lat_vel_max + EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Define object such that it is not validated as approaching from front and x position is below the vehicle lenght
 * \uts{CSCSA-41642} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__x_position_below_length)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;
   ced_object.tracker_data.length                = FBK_ONE_F;
   ced_object.tracker_data.vcs_pos.x             = -EPSILON - p_vehicle_data->host_length - ced_object.tracker_data.length;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}
/**
 * Define object such that it is not validated as approaching from front and data age is too low
 * \uts{CSCSA-41643} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__data_age_too_low)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min - 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front is not detected. */
   EXPECT_FALSE(f_approaches_from_front);
}

/**
 * Tests that a CED object is not classified as matched to a path if the path object pair is not provided (for better branch
 * coverage) \uts{CSCSA-41644} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__set_path_object_pair_output_to_null)
{
   /** \arrange Set up CED object */
   Ced_Object_T ced_object{};
   boolean_T result;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that false is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Tests that a CED object is not classified as matched to a path if the track match is default (for better branch coverage)
 * \uts{CSCSA-41645} \sdd{SF-3569} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Matched_To_Path__track_math_is_default)
{
   /** \arrange Set up CED object */
   Ced_Object_T ced_object{};
   boolean_T result;
   Pt_Path_Object_Pair_Output_T *pt_match_info = &pt_output.path_obj_pair_output[0u];
   pt_match_info->track_match                  = 7u;
   ced_object.attributes.p_pt_match_info       = pt_match_info;
   path_obj_pair_output.track_match            = PT_DEFAULT_MATCH_INDEX;

   /** \action Call function that determines if object is matched to path. */
   result = Ced_Is_Object_Matched_To_Path(&ced_object, p_ced_cals);

   /** \assert Verify that false is returned by function and written to object properties. */
   EXPECT_FALSE(result);
}

/**
 * Define object such that it is validated as approaching from front. Negative heading.
 * \uts{CSCSA-41646} \sdd{SF-3444} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Front_Object_Relevant__object_approaching_from_front_negative_heading)
{
   /** \arrange Create object based on cal values. */
   Ced_Object_T ced_object                       = {};
   ced_object.tracker_data.existence_probability = p_ced_cals->k_ced_object_ftm_existence_probability_min + EPSILON;
   ced_object.tracker_data.vcs_vel_rel.x         = -p_ced_cals->k_ced_object_ftm_long_vel_rel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.x             = -p_ced_cals->k_ced_object_ftm_long_vel_min - EPSILON;
   ced_object.tracker_data.vcs_vel.y             = p_ced_cals->k_ced_object_ftm_lat_vel_max - EPSILON;
   ced_object.tracker_data.vcs_heading           = -p_ced_cals->k_ced_object_ftm_heading_abs_angle_min - EPSILON;
   ced_object.tracker_data.age                   = p_ced_cals->k_ced_object_ftm_age_min + 1u;

   /** \action Call Ced_Is_Front_Object_Relevant to evaluate if object is approaching from front. */
   boolean_T f_approaches_from_front = Ced_Is_Front_Object_Relevant(&ced_object, p_vehicle_data, p_ced_cals);

   /** \assert Verify that approach from front was detected. */
   EXPECT_TRUE(f_approaches_from_front);
}

/**
 * Tests function with rear mode flag disabled.
 * \uts{CSCSA-41647} \sdd{SF-3570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Relevant__rear_mode_flag_disable)
{
   /** \arrange Set up a CED object approaching from ego rear that fulfills all conditions to be relevant. */
   boolean_T result;
   Ced_Object_T ced_object{};

   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -59.0f, 19.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   ced_object = Ced_Create_Object_From_Tracker_Output(4u);

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;
   ced_instance.core_input.f_ced_rear_mode                             = FBK_FALSE;

   /** \action Call function that checks if the object is relevant for CED. */
   result = Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, &ced_instance.core_input, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests function with front mode test with relevant object.
 * \uts{CSCSA-41648} \sdd{SF-3570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Is_Object_Relevant__front_mode_relevant)
{
   /** \arrange Set up a CED object approaching from ego rear that fulfills all conditions to be relevant. */
   boolean_T result;
   Ced_Object_T ced_object{};

   float32_T long_vel     = -p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = -p_ced_cals->k_ced_object_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max - EPSILON;

   Ced_Fill_Raw_Tracker_Output(4u, -9.0f, 19.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   ced_object                          = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.tracker_data.vcs_heading = p_ced_cals->k_ced_object_ftm_heading_abs_angle_min + EPSILON;

   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT] = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]  = 0.0f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                   = 0;
   ced_instance.core_input.f_ced_rear_mode                             = FBK_FALSE;
   ced_instance.core_input.f_ced_front_mode                            = FBK_TRUE;

   /** \action Call function that checks if the object is relevant for CED. */
   result = Ced_Is_Object_Relevant(&ced_object, &ced_funnel_zone, &ced_instance.core_input, p_vehicle_data, p_ced_cals);

   /** \assert Verify that the function returns true for the object. */
   EXPECT_FALSE(result);
}

/**
 * Tests adpation of the heading angle for object slight turns. Here, object is closer than longitudinal limit, thus the change is
 * not applied. \uts{CSCSA-124426} \sdd{CSCSA-122419} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_During_Slight_Turns__object_is_close_long)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos    = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos     = 5.0f;
   float32_T lat_vel     = 1.0f;
   float32_T low_heading = 0.2f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;

   p_ced_cals->k_ced_slight_turn_position_limits[0] = Fbk_Abs_F(long_pos) + FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_position_limits[1] = Fbk_Abs_F(lat_pos) - FBK_ONE_F;

   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_During_Slight_Turns(&ced_object, p_ced_cals);

   /** \assert Verify that the heading has not been changed. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adpation of the heading angle for object slight turns. Here, object is closer than lateral limit, thus the change is not
 * applied. \uts{CSCSA-124429} \sdd{CSCSA-122419} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_During_Slight_Turns__object_is_close_lat)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos    = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos     = -5.0f;
   float32_T lat_vel     = 1.0f;
   float32_T low_heading = 0.2f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;

   p_ced_cals->k_ced_slight_turn_position_limits[0] = Fbk_Abs_F(long_pos) - FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_position_limits[1] = Fbk_Abs_F(lat_pos) + FBK_ONE_F;

   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_During_Slight_Turns(&ced_object, p_ced_cals);

   /** \assert Verify that the heading has not been changed. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for object slight turns. Here object position is correct and speed is below first
 * threshold, thus expect zero heading. \uts{CSCSA-124430} \sdd{CSCSA-122419} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_During_Slight_Turns__position_ok_speed_in_first_range)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos    = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos     = 5.0f;
   float32_T lat_vel     = -4.0f; // set negative to improve bcov
   float32_T low_heading = 0.2f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;

   p_ced_cals->k_ced_slight_turn_position_limits[0] = Fbk_Abs_F(long_pos) - FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_position_limits[1] = Fbk_Abs_F(lat_pos) - FBK_ONE_F;

   p_ced_cals->k_ced_slight_turn_lat_vel_table[0] = -lat_vel + FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_lat_vel_table[1] = -lat_vel + FBK_ONE_F + FBK_ONE_F;

   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_During_Slight_Turns(&ced_object, p_ced_cals);

   /** \assert Verify that the heading has zero value. */
   EXPECT_FLOAT_EQ(FBK_ZERO_F, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for object slight turns. Here object position is correct and speed is half between
 * threshold 1 and 2, thus expect heading is half of the initial value. \uts{CSCSA-124431} \sdd{CSCSA-122419}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_During_Slight_Turns__position_ok_speed_between)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos    = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos     = 5.0f;
   float32_T lat_vel     = 4.0f;
   float32_T low_heading = 0.2f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;

   p_ced_cals->k_ced_slight_turn_position_limits[0] = Fbk_Abs_F(long_pos) - FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_position_limits[1] = Fbk_Abs_F(lat_pos) - FBK_ONE_F;

   p_ced_cals->k_ced_slight_turn_lat_vel_table[0] = lat_vel - FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_lat_vel_table[1] = lat_vel + FBK_ONE_F;

   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_During_Slight_Turns(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is 0.5 of the initial value. */
   EXPECT_FLOAT_EQ(Fbk_Half(low_heading), ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for object slight turns. Here object position is correct and speed over second threshold,
 * thus expect heading hase the initial value. \uts{CSCSA-124432} \sdd{CSCSA-122419} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_During_Slight_Turns__position_ok_speed_over_second_range)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos    = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos     = 5.0f;
   float32_T lat_vel     = 4.0f;
   float32_T low_heading = 0.2f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;

   p_ced_cals->k_ced_slight_turn_position_limits[0] = Fbk_Abs_F(long_pos) - FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_position_limits[1] = Fbk_Abs_F(lat_pos) - FBK_ONE_F;

   p_ced_cals->k_ced_slight_turn_lat_vel_table[0] = lat_vel - FBK_ONE_F - FBK_ONE_F;
   p_ced_cals->k_ced_slight_turn_lat_vel_table[1] = lat_vel - FBK_ONE_F;

   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_During_Slight_Turns(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in 1st quarter thus expect heading hase set to zero.
 * \uts{CSCSA-185603} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_in_1_quarter)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -10.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = -3.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in 2nd quarter thus expect heading hase set to zero.
 * \uts{CSCSA-185604} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_in_2_quarter)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -10.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 3.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in 3rd quarter thus expect heading hase set to zero.
 * \uts{CSCSA-185605} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_in_3_quarter)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = 10.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 3.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in 4th quarter thus expect heading hase set to zero.
 * \uts{CSCSA-185606} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_in_4_quarter)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = 10.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = -3.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in 4th quarter outside of range thus expect heading hase set to zero.
 * \uts{CSCSA-185607} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_in_4_quarter_outside_of_range)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = 50.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = -3.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in ego lane thus expect heading hase set to zero.
 * \uts{CSCSA-185608} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_in_ego_lane)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 0.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(FBK_ZERO_F, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target outside of ego lane, thus expect heading hase the initial value.
 * \uts{CSCSA-185609} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_outside_of_ego_lane)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 4.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, calibration disabled, thus expect heading hase the initial value.
 * \uts{CSCSA-185610} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__adaptation_disabled)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 0.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_FALSE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in too far, thus expect heading hase the initial value.
 * \uts{CSCSA-185611} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_too_far)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -55.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 0.0f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.2f;
   float32_T target_width  = 1.0f;
   float32_T target_length = 4.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in too far object not crossing center line, thus expect heading hase
 * the initial value. \uts{CSCSA-185612} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_too_far_target_left_side_ego_lane)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -55.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 0.2f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.1f;
   float32_T target_width  = 0.4f;
   float32_T target_length = 2.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(low_heading, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in left side of ego lane not crossing middle point, thus expect
 * heading hase the initial value. \uts{CSCSA-185613} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_left_side_ego_lane)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = 0.2f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.1f;
   float32_T target_width  = 0.4f;
   float32_T target_length = 2.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(FBK_ZERO_F, ced_object.attributes.heading_predicted);
}

/**
 * Tests adapation of the heading angle for ego lane, target in right side of ego lane not crossing middle point, thus expect
 * heading hase the initial value. \uts{CSCSA-185614} \sdd{CSCSA-165203} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Adapt_Heading_For_Ego_Lane__target_right_side_ego_lane)
{
   /** \arrange Set up a CED object approaching from ego rear */
   Ced_Object_T ced_object{};
   float32_T long_pos      = -8.0f; // alawys negative, obj behind ego
   float32_T lat_pos       = -0.2f;
   float32_T lat_vel       = 4.0f;
   float32_T low_heading   = 0.1f;
   float32_T target_width  = 0.4f;
   float32_T target_length = 2.0f;

   Ced_Fill_Raw_Tracker_Output(4u, long_pos, lat_pos, 5.0f, lat_vel, 5.0f, lat_vel);
   ced_object                              = Ced_Create_Object_From_Tracker_Output(4u);
   ced_object.attributes.heading_predicted = low_heading;
   ced_object.attributes.width_predicted   = target_width;
   ced_object.attributes.length_predicted  = target_length;

   p_ced_cals->k_ced_f_adapt_heading_ego_lane = FBK_TRUE;


   /** \action Call function adpating the heading. */
   Ced_Adapt_Heading_For_Ego_Lane(&ced_object, p_ced_cals);

   /** \assert Verify that the heading is equal the initial value. */
   EXPECT_FLOAT_EQ(FBK_ZERO_F, ced_object.attributes.heading_predicted);
}

/**
 * Verify that function calculating shifted lateral position is working correctly. Here, option is disabled by calibration
 * \uts{CSCSA-124433} \sdd{CSCSA-124424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Position__disabled)
{
   /** \arrange Set up object and disable option by calibration */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   float32_T lat_pos       = 2.0f;
   float32_T long_pos      = -25.0f;
   float32_T result;
   Ced_Fill_Raw_Tracker_Output(obj_index, long_pos, lat_pos, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                                              = Ced_Create_Object_From_Tracker_Output(obj_index);
   p_ced_cals->k_ced_lat_pos_max_shift                     = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[0] = 10.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[1] = 20.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]  = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]  = 1.5f;
   p_ced_cals->k_ced_lat_pos_shift_enable                  = FBK_FALSE;

   /** \action Call function to calculate shifted lateral position. */
   result = Ced_Get_Lat_Position(&ced_object, p_ced_cals);

   /** \assert Expect no change in lat. position */
   EXPECT_FLOAT_EQ(result, lat_pos);
}

/**
 * Verify that function calculating shifted lateral position is working correctly. Here object is far (longitudianally) from ego,
 * thus expect max shift \uts{CSCSA-124434} \sdd{CSCSA-124424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Position__max_shift)
{
   /** \arrange Set up object far from ego. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   float32_T lat_pos       = 2.0f;
   float32_T long_pos      = -25.0f;
   float32_T result;
   Ced_Fill_Raw_Tracker_Output(obj_index, long_pos, lat_pos, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                                              = Ced_Create_Object_From_Tracker_Output(obj_index);
   p_ced_cals->k_ced_lat_pos_max_shift                     = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[0] = 10.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[1] = 20.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]  = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]  = 1.5f;
   p_ced_cals->k_ced_lat_pos_shift_enable                  = FBK_TRUE;

   /** \action Call function to calculate shifted lateral position. */
   result = Ced_Get_Lat_Position(&ced_object, p_ced_cals);

   /** \assert Expect increased y position */
   EXPECT_FLOAT_EQ(result, lat_pos + p_ced_cals->k_ced_lat_pos_max_shift);
}

/**
 * Verify that function calculating shifted lateral position is working correctly. Here object is far in positives
 * (longitudianally) from ego, thus expect max shift \uts{CSCSA-185615} \sdd{CSCSA-124424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Position__max_shift_long_positive)
{
   /** \arrange Set up object far from ego. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   float32_T lat_pos       = 2.0f;
   float32_T long_pos      = 25.0f;
   float32_T result;
   Ced_Fill_Raw_Tracker_Output(obj_index, long_pos, lat_pos, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                                              = Ced_Create_Object_From_Tracker_Output(obj_index);
   p_ced_cals->k_ced_lat_pos_max_shift                     = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[0] = 10.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[1] = 20.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]  = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]  = 1.5f;
   p_ced_cals->k_ced_lat_pos_shift_enable                  = FBK_TRUE;

   /** \action Call function to calculate shifted lateral position. */
   result = Ced_Get_Lat_Position(&ced_object, p_ced_cals);

   /** \assert Expect increased y position */
   EXPECT_FLOAT_EQ(result, lat_pos + p_ced_cals->k_ced_lat_pos_max_shift);
}

/**
 * Verify that function calculating shifted lateral position is working correctly. Here object is close to ego, thus expect no
 * shift \uts{CSCSA-124435} \sdd{CSCSA-124424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Position__min_shift)
{
   /** \arrange Set up object close to ego */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   float32_T lat_pos       = 2.0f;
   float32_T long_pos      = -5.0f;
   float32_T result;
   Ced_Fill_Raw_Tracker_Output(obj_index, long_pos, lat_pos, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                                              = Ced_Create_Object_From_Tracker_Output(obj_index);
   p_ced_cals->k_ced_lat_pos_shift_enable                  = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_max_shift                     = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[0] = 10.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[1] = 20.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]  = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]  = 1.5f;

   /** \action Call function to calculate shifted lateral position. */
   result = Ced_Get_Lat_Position(&ced_object, p_ced_cals);

   /** \assert Expect no change in lat position. */
   EXPECT_FLOAT_EQ(result, lat_pos);
}

/**
 * Verify that function calculating shifted lateral position is working correctly. Here object is on left side of ego.
 * \uts{CSCSA-124436} \sdd{CSCSA-124424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Position__negative_y)
{
   /** \arrange Set up object on left side. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   float32_T lat_pos       = -2.0f;
   float32_T long_pos      = -25.0f;
   float32_T result;
   Ced_Fill_Raw_Tracker_Output(obj_index, long_pos, lat_pos, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                                              = Ced_Create_Object_From_Tracker_Output(obj_index);
   p_ced_cals->k_ced_lat_pos_shift_enable                  = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_max_shift                     = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[0] = 10.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[1] = 20.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]  = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]  = 1.5f;

   /** \action Call function to calculate shifted lateral position. */
   result = Ced_Get_Lat_Position(&ced_object, p_ced_cals);

   /** \assert Expect reduce y distance. */
   EXPECT_FLOAT_EQ(result, lat_pos - p_ced_cals->k_ced_lat_pos_max_shift);
}


/**
 * Verify that function calculating shifted lateral position is working correctly. Here, shift relates on objects width, which is
 * below threshold thus shift is not applied. \uts{CSCSA-246712} \sdd{CSCSA-124424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Position__obj_width_below_thresh)
{
   /** \arrange Set up object and disable option by calibration */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   float32_T lat_pos       = 2.0f;
   float32_T long_pos      = -25.0f;
   float32_T result;
   Ced_Fill_Raw_Tracker_Output(obj_index, long_pos, lat_pos, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                                              = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.width                           = 1.1f;
   p_ced_cals->k_ced_lat_pos_max_shift                     = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[0] = 10.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[1] = 20.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]  = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]  = 1.5f;
   p_ced_cals->k_ced_lat_pos_shift_enable                  = FBK_TRUE;


   /** \action Call function to calculate shifted lateral position. */
   result = Ced_Get_Lat_Position(&ced_object, p_ced_cals);

   /** \assert Expect no change in lat. position */
   EXPECT_FLOAT_EQ(result, lat_pos);
}

/**
 * Verify that function calculating shifted lateral position is working correctly. Here, shift relates on objects width, which is
 * above threshold thus shift is applied. \uts{CSCSA-246713} \sdd{CSCSA-124424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Get_Lat_Position__obj_width_above_thresh)
{
   /** \arrange Set up object on left side. */
   Ced_Object_T ced_object = {};
   uint8_t obj_index       = 7u;
   float32_T lat_pos       = -2.0f;
   float32_T long_pos      = -25.0f;
   float32_T result;
   Ced_Fill_Raw_Tracker_Output(obj_index, long_pos, lat_pos, 2.0f, 0.0f, 2.0f, 0.0f);
   ced_object                                              = Ced_Create_Object_From_Tracker_Output(obj_index);
   ced_object.tracker_data.width                           = 1.2f;
   p_ced_cals->k_ced_lat_pos_shift_enable                  = FBK_TRUE;
   p_ced_cals->k_ced_lat_pos_max_shift                     = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[0] = 10.0f;
   p_ced_cals->k_ced_lat_pos_shift_long_dist_thresholds[1] = 20.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]  = 1.0f;
   p_ced_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]  = 1.5f;

   /** \action Call function to calculate shifted lateral position. */
   result = Ced_Get_Lat_Position(&ced_object, p_ced_cals);

   /** \assert Expect reduce y distance. */
   EXPECT_FLOAT_EQ(result, lat_pos - p_ced_cals->k_ced_lat_pos_max_shift);
}


/**
 * Tests if filtering stationary objects works properly.
 * \uts{CSCSA-138946} \sdd{SF-3563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Algorithm__filter_stationary)
{
   /** \arrange Set up calibration, persistent data and tracker output such that a critical object on the right ego side is
    * present. */
   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min + 1.0f;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -4.0f, 2.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   object_data[4u].vcs_heading                                                = 0.0f;
   object_data[4u].f_moveable                                                 = FBK_FALSE;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT]        = 0.0f;
   p_ced_cals->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR]         = 0.0f;
   p_ced_cals->k_ced_slow_objects_long_vel_max                                = 0.0f;
   p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_LEFT]              = 1.5f;
   p_ced_cals->k_ced_second_warning_ttc_threshold[FBK_SIDE_RIGHT]             = 1.5f;
   p_ced_cals->k_ced_f_choose_ref_point_funnel_check                          = 0;
   ced_instance.persistance.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = p_ced_cals->k_ced_alert_qualifying_cycles + 1;

   /** \action Call the CED algorithm function. */
   Ced_Algorithm(&ced_instance.core_output, &ced_instance.persistance, p_vehicle_data, &ced_instance.core_input, p_ced_cals,
                 &fbk_output);

   /** \assert Verify that the core output shows an alert on the right ego side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/**
 * Tests that CED algorithm creates no alert if no valid object is present.
 * \uts{CSCSA-186386} \sdd{SF-3563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Algorithm__does_not_alert_if_only_invalid_obj_present)
{
   /** \arrange Set up calibration, persistent data and tracker output such that no valid object is present. */
   float32_T long_vel     = p_ced_cals->k_ced_object_long_vel_min;
   float32_T long_vel_rel = p_ced_cals->k_ced_object_long_vel_rel_min;
   float32_T lat_vel      = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -20.0f, -5.0f, long_vel, lat_vel, long_vel_rel, 0.0f);
   object_data[4u].status = PA_OBJ_STATUS_INVALID;

   /** \action Call the CED algorithm function. */
   Ced_Algorithm(&ced_instance.core_output, &ced_instance.persistance, p_vehicle_data, &ced_instance.core_input, p_ced_cals,
                 &fbk_output);

   /** \assert Verify that the core output shows no alert on both ego side. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/**
 * Tests that CED algorithm creates alert if object is a reflection but was valid in previous cycle.
 * \uts{CSCSA-186387} \sdd{SF-3563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Test, Ced_Algorithm__alert_when_obj_is_reflection)
{
   /** \arrange Set up calibration, persistent data and tracker output such that valid object is present. */
   float32_T lat_vel = p_ced_cals->k_ced_object_lat_vel_max;

   Ced_Fill_Raw_Tracker_Output(4u, -20.0f, -5.0f, 20.f, lat_vel, p_vehicle_data->host_speed + 20.f, 0.0f);
   object_data[4u].status                                             = PA_OBJ_STATUS_COASTED;
   object_data[4u].f_reflection                                       = FBK_TRUE;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.persistance.ced_side_alert_prev_cycle[FBK_SIDE_RIGHT] = CED_NO_ALERT;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_LEFT]     = object_data[4u].id;
   ced_instance.persistance.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]    = 0u;

   /** \action Call the CED algorithm function. */
   Ced_Algorithm(&ced_instance.core_output, &ced_instance.persistance, p_vehicle_data, &ced_instance.core_input, p_ced_cals,
                 &fbk_output);

   /** \assert Verify that the core output shows no alert on both ego side. */
   EXPECT_NE(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}
