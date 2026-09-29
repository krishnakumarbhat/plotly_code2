/**
 * @file lcda_process_cvw_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_process_cvw.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42692}
 */

#include "lcda_process_cvw_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <math.h>
#include <string.h>

extern "C"
{
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "lcda_common_functions.h"
#include "lcda_process_cvw.c"
#include "lcda_test_helpers.h"
#include "lcda_types.h"
#include "ml_math.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pa_vehicle_in.h"
}


/**
 * Set up CVW persistent data such that it does not equal its default values. Reset the CVW core output and check that the CVW
 * persistent data and output is reset for CVW. \uts{CSCSA-42693} \sdd{SF-6698} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Reset_Cvw_Core__core_Cvw_output_is_reset)
{
   /** \arrange Set up CVW persistent data. */
   memset(&cvw_core_output, 0xaa, sizeof(cvw_core_output));
   lcda_persistent.f_cvw_prev_reset = FBK_FALSE;

   /** \action Call Lcda_Reset_Cvw_Core to reset CVW core. */
   Lcda_Reset_Cvw_Core(&cvw_core_output, &lcda_persistent, &cvw_persistent);

   /** \assert Check if CVW output and persistent data was reset. */
   EXPECT_TRUE(Lcda_Is_Cvw_Output_Default(&cvw_core_output));
   EXPECT_TRUE(lcda_persistent.f_cvw_prev_reset);
}

/**
 * Check that the function that shall reset the CVW persistent data actually resets all persistent variables to default values.
 * \uts{CSCSA-42694} \sdd{SF-6684} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Reset_Cvw_Persistent_Data__works_properly)
{
   /** \arrange Set up CVW persistent data with non-default values. */
   uint8_t obj_id = 8u;
   memset(&cvw_persistent, 0xaa, sizeof(cvw_persistent));

   /** \action Call Lcda_Reset_Cvw_Persistent_Data to reset CVW persistent data. */
   Lcda_Reset_Cvw_Persistent_Data(&cvw_persistent);

   /** \assert Check if CVW persistent data is reseted to default values. */
   EXPECT_EQ(cvw_persistent.f_prev_cvw_active[FBK_SIDE_LEFT], FBK_FALSE);
   EXPECT_EQ(cvw_persistent.cvw_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_obj_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(cvw_persistent.f_prev_cvw_active[FBK_SIDE_RIGHT], FBK_FALSE);
   EXPECT_EQ(cvw_persistent.cvw_hold_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_obj_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(cvw_persistent.mature_count_in_cvw_zone[obj_id], FBK_ZERO_UINT);
}

/**
 * Check that the function that shall reset the CVW object persistent data actually resets all object persistent variables to
 * default values. \uts{CSCSA-42695} \sdd{SF-6685} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Reset_Cvw_Persistent_Object_Data__works_properly)
{
   /** \arrange Set up CVW object persistent data with non-default values. */
   uint8_t obj_id                                  = 5u;
   cvw_persistent.mature_count_in_cvw_zone[obj_id] = 4u;

   /** \action Call Lcda_Reset_Cvw_Persistent_Data to reset CVW object persistent data. */
   Lcda_Reset_Cvw_Persistent_Object_Data(&cvw_persistent, obj_id);

   /** \assert Check if CVW object persistent data is reseted to default values. */
   EXPECT_EQ(cvw_persistent.mature_count_in_cvw_zone[obj_id], FBK_ZERO_UINT);
}

/**
 * Check that the function that fills the persistent data from the core output and verify that the core output information is
 * stored correctly in the persistent data. \uts{CSCSA-42696} \sdd{SF-6677} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Fill_Side_Persistent_Cvw_Data__works_properly)
{
   /** \arrange Set up core output with alerts for both sides. */
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   cvw_core_output.cvw_alert[FBK_SIDE_LEFT]     = LCDA_ALERT_STATE_LEVEL_1;
   cvw_core_output.cvw_index[FBK_SIDE_LEFT]     = 7u;
   cvw_core_output.cvw_id[FBK_SIDE_LEFT]        = 33u;
   cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT] = 34u;

   cvw_core_output.cvw_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_2;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT]     = 4u;
   cvw_core_output.cvw_id[FBK_SIDE_RIGHT]        = 11u;
   cvw_core_output.cvw_unique_id[FBK_SIDE_RIGHT] = 12u;

   /** \action Call Lcda_Fill_Side_Persistent_Cvw_Data to fill CVW persistent data from core output. */
   Lcda_Fill_Side_Persistent_Cvw_Data(&cvw_persistent, f_use_small_lc_intention_zone, &cvw_core_output);

   /** \assert Check if CVW persistent data is filled correctly with respect to given core output. */
   EXPECT_EQ(cvw_persistent.f_prev_cvw_active[FBK_SIDE_LEFT], FBK_TRUE);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT], cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_unique_obj_id[FBK_SIDE_LEFT], cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_obj_index[FBK_SIDE_LEFT], cvw_core_output.cvw_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(cvw_persistent.f_prev_cvw_active[FBK_SIDE_RIGHT], FBK_TRUE);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT], cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_unique_obj_id[FBK_SIDE_RIGHT], cvw_core_output.cvw_unique_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_obj_index[FBK_SIDE_RIGHT], cvw_core_output.cvw_index[FBK_SIDE_RIGHT]);
}

/**
 * Check that core output for left side is reseted properly by corresponding function.
 * \uts{CSCSA-42697} \sdd{SF-6675} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Clear_Cvw_Core_Output_On_Side__works_properly_for_FBK_SIDE_LEFT)
{
   /** \arrange Set up non-default core output on left side. */
   uint8_t side                           = FBK_SIDE_LEFT;
   cvw_core_output.cvw_alert[side]        = LCDA_ALERT_STATE_LEVEL_1;
   cvw_core_output.cvw_index[side]        = 7u;
   cvw_core_output.cvw_id[side]           = 33u;
   cvw_core_output.cvw_unique_id[side]    = 33u;
   cvw_core_output.cvw_ttc[side]          = 1.2f;
   cvw_core_output.cvw_distance_lat[side] = 3.5f;

   /** \action Call Lcda_Clear_Cvw_Core_Output_On_Side to reset core output for left side. */
   Lcda_Clear_Cvw_Core_Output_On_Side(&cvw_core_output, side);

   /** \assert Verify that core output for left side is reseted properly. */
   EXPECT_EQ(cvw_core_output.cvw_alert[side], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(cvw_core_output.cvw_unique_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(cvw_core_output.cvw_index[side], PA_INVALID_OBJ_INDEX);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_ttc[side], LCDA_CVW_DEFAULT_NO_ALERT_TTC);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance_lat[side], -LCDA_DEFAULT_OBJ_DIST);
}

/**
 * Check that core output for right side is reseted properly by corresponding function.
 * \uts{CSCSA-42698} \sdd{SF-6675} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Clear_Cvw_Core_Output_On_Side__works_properly_for_FBK_SIDE_RIGHT)
{
   /** \arrange Set up non-default core output on right side. */
   uint8_t side                           = FBK_SIDE_RIGHT;
   cvw_core_output.cvw_alert[side]        = LCDA_ALERT_STATE_LEVEL_2;
   cvw_core_output.cvw_index[side]        = 4u;
   cvw_core_output.cvw_id[side]           = 11u;
   cvw_core_output.cvw_unique_id[side]    = 11u;
   cvw_core_output.cvw_ttc[side]          = 1.2f;
   cvw_core_output.cvw_distance_lat[side] = 3.5f;

   /** \action Call Lcda_Clear_Cvw_Core_Output_On_Side to reset core output for right side. */
   Lcda_Clear_Cvw_Core_Output_On_Side(&cvw_core_output, side);

   /** \assert Verify that core output for right side is reseted properly. */
   EXPECT_EQ(cvw_core_output.cvw_alert[side], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(cvw_core_output.cvw_unique_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(cvw_core_output.cvw_index[side], PA_INVALID_OBJ_INDEX);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_ttc[side], LCDA_CVW_DEFAULT_NO_ALERT_TTC);

   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance_lat[side], -LCDA_DEFAULT_OBJ_DIST);
}

/**
 * Check that initialization of CVW object works properly and sets tracker object pointer.
 * \uts{CSCSA-42699} \sdd{SF-6680} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Init_Cvw_Object_Data__works_properly)
{
   /** \arrange Set up CVW object with non-default values and create tracker object. */
   cvw_object.ego_side               = FBK_SIDE_LEFT;
   cvw_object.f_obj_in_zone          = FBK_TRUE;
   cvw_object.zone.size              = 3u;
   cvw_object.p_tracker_data         = NULL;
   cvw_object.obj_front_position_lat = 3.0f;

   /** \action Call Lcda_Init_Cvw_Object_Data to initialize CVW object. */
   Lcda_Init_Cvw_Object_Data(&cvw_object, &tracker_object);

   /** \assert Verify that pointer to tracker object is set and attributes are on default values. */
   EXPECT_EQ(cvw_object.ego_side, FBK_SIDE_UNDEFINED);
   EXPECT_FALSE(cvw_object.f_obj_in_zone);
   EXPECT_EQ(cvw_object.zone.size, FBK_ZERO_UINT);
   EXPECT_EQ(cvw_object.p_tracker_data, &tracker_object);
   EXPECT_FLOAT_EQ(cvw_object.obj_front_position_lat, LCDA_DEFAULT_OBJ_DIST);
}

/**
 * Create a CVW alert state which was raised for a target object the last cycle. Obtain the last warned object id for this alert.
 * Verify that target object id are equal computed object id. \uts{CSCSA-42700} \sdd{SF-6693} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side__non_default_id_when_alert_is_active_previously_on_side)
{
   /** \arrange Set up CVW alert state and corresponding persistent data. */
   uint8_t result;
   uint8_t obj_id = 3u;

   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT]  = obj_id;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   /** \action Call Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side to obtain the last CVW alert relevant track object id. */
   result = Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(FBK_SIDE_LEFT, &cvw_persistent);

   /** \assert Check that the computed target object id equals the target objects id. */
   EXPECT_EQ(result, obj_id);
}

/**
 * Create a CVW alert state which was inactive (no alert raised) in the last cycle. Obtain the last warned object id for this
 * alert. Verify that computed id equals the default object id. \uts{CSCSA-42701} \sdd{SF-6693} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side__default_id_when_alert_is_not_active_previously_on_side)
{
   /** \arrange Set up CVW alert state and corresponding persistent data. */
   uint8_t result;
   uint8_t obj_id = 3u;

   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT]  = obj_id;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   /** \action Call Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side to obtain the last CVW alert relevant track object index. */
   result = Lcda_Get_Prev_Cvw_Alert_Object_Id_On_Side(FBK_SIDE_RIGHT, &cvw_persistent);

   /** \assert Check that the computed target object id equals the default object id. */
   EXPECT_EQ(result, PA_INVALID_OBJ_ID);
}

/**
 * Create a CVW alert state which was raised for a target object the last cycle. Obtain the last warned alert state. Verify that
 * the last warn state is set to active on left side. \uts{CSCSA-42702} \sdd{SF-6694} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Prev_Cvw_Alert_Active_On_Obj__left_is_TRUE_when_cvw_alert_is_active_on_obj_in_previous_cyclet)
{
   /** \arrange Set up CVW alert state and corresponding persistent data. */
   uint8_t result = FBK_FALSE;
   uint8_t obj_id = 3u;

   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT] = obj_id;
   cvw_persistent.f_prev_cvw_active[FBK_SIDE_LEFT]     = FBK_TRUE;

   /** \action Call Lcda_Is_Prev_Cvw_Alert_Active_On_Obj to obtain the alert state from the last cycle. */
   result = Lcda_Is_Prev_Cvw_Alert_Active_On_Obj(&cvw_persistent, obj_id, FBK_SIDE_LEFT);

   /** \assert Check that CVW alert was active last cycle. */
   EXPECT_TRUE(result);
}

/**
 * Create a CVW alert state which was raised for a target object the last cycle. Obtain the last warned alert state. Verify that
 * the last warn state is set to active on right side. \uts{CSCSA-42703} \sdd{SF-6694} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Is_Prev_Cvw_Alert_Active_On_Obj__right_is_TRUE_when_cvw_alert_is_active_on_obj_in_previous_cycle_right)
{
   /** \arrange Set up CVW alert state and corresponding persistent data. */
   uint8_t result = FBK_FALSE;
   uint8_t obj_id = 3u;

   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = obj_id;
   cvw_persistent.f_prev_cvw_active[FBK_SIDE_RIGHT]     = FBK_TRUE;

   /** \action Call Lcda_Is_Prev_Cvw_Alert_Active_On_Obj to obtain the alert state from the last cycle. */
   result = Lcda_Is_Prev_Cvw_Alert_Active_On_Obj(&cvw_persistent, obj_id, FBK_SIDE_RIGHT);

   /** \assert Check that CVW alert was active last cycle. */
   EXPECT_TRUE(result);
}

/**
 * Create a CVW alert state which was not raised for a target object the last cycle. Obtain the last warned alert state. Verify
 * that the last warn state is set to inactive. \uts{CSCSA-42704} \sdd{SF-6694} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Prev_Cvw_Alert_Active_On_Obj__is_FALSE_when_cvw_alert_is_not_active_on_obj_in_previous_cycle)
{
   /** \arrange Set up CVW alert state and corresponding persistent data. */
   uint8_t result = FBK_FALSE;
   uint8_t obj_id = 3u;

   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT] = obj_id;
   cvw_persistent.f_prev_cvw_active[FBK_SIDE_LEFT]     = FBK_TRUE;

   /** \action Call Lcda_Is_Prev_Cvw_Alert_Active_On_Obj to obtain the alert state from the last cycle. */
   result = Lcda_Is_Prev_Cvw_Alert_Active_On_Obj(&cvw_persistent, obj_id + 1, FBK_SIDE_LEFT);

   /** \assert Check that CVW alert was inactive last cycle. */
   EXPECT_FALSE(result);
}

/**
 * Check that an previous alert is held if the holding counter is below the threshold.
 * \uts{CSCSA-42705} \sdd{SF-6683} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Output__holds_alert_if_holding_counter_below_threshold)
{
   /** \arrange Set up CVW alert state and corresponding persistent data such that holding counter is below threshold. */
   uint8_t obj_id = 3u;

   cvw_core_output.cvw_index[FBK_SIDE_LEFT]     = PA_INVALID_OBJ_INDEX;
   cvw_core_output.cvw_id[FBK_SIDE_LEFT]        = PA_INVALID_OBJ_ID;
   cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT] = PA_INVALID_OBJ_ID;
   cvw_core_output.cvw_alert[FBK_SIDE_LEFT]     = LCDA_ALERT_STATE_NONE;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT]    = PA_INVALID_OBJ_INDEX;
   cvw_core_output.cvw_id[FBK_SIDE_RIGHT]       = PA_INVALID_OBJ_ID;
   cvw_core_output.cvw_alert[FBK_SIDE_RIGHT]    = LCDA_ALERT_STATE_NONE;

   lcda_cals.k_cvw_alert_holding_cycles = 3u;

   cvw_persistent.f_prev_cvw_active[FBK_SIDE_LEFT]            = FBK_TRUE;
   cvw_persistent.prev_cvw_alert_obj_index[FBK_SIDE_LEFT]     = obj_id - 1u;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT]        = obj_id;
   cvw_persistent.prev_cvw_alert_unique_obj_id[FBK_SIDE_LEFT] = obj_id;
   cvw_persistent.cvw_hold_counter[FBK_SIDE_LEFT]             = lcda_cals.k_cvw_alert_holding_cycles - 1u;

   /** \action Call function Lcda_Process_Cvw_Output that is responsible for alert holding with no turn signal. */
   Lcda_Process_Cvw_Output(&cvw_core_output, &cvw_persistent, &lcda_cals, TURN_SIGNAL_NONE);

   /** \assert Check that the CVW alert level is LCDA_ALERT_STATE_LEVEL_1 and holding counter is increased. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_LEFT], obj_id - 1u);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_LEFT], obj_id);
   EXPECT_EQ(cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT], obj_id);
   EXPECT_EQ(cvw_persistent.cvw_hold_counter[FBK_SIDE_LEFT], lcda_cals.k_cvw_alert_holding_cycles);
}

/**
 * Check that an previous alert is no longer held if the holding counter exceeds the threshold.
 * \uts{CSCSA-42706} \sdd{SF-6683} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Output__alert_is_switched_off_if_holding_counter_exceeds_threshold)
{
   /** \arrange Set up CVW alert state and corresponding persistent data such that holding counter exceeds threshold. */
   uint8_t obj_id = 3u;

   cvw_core_output.cvw_index[FBK_SIDE_LEFT]     = PA_INVALID_OBJ_INDEX;
   cvw_core_output.cvw_id[FBK_SIDE_LEFT]        = PA_INVALID_OBJ_ID;
   cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT] = PA_INVALID_OBJ_ID;
   cvw_core_output.cvw_alert[FBK_SIDE_LEFT]     = LCDA_ALERT_STATE_NONE;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT]    = PA_INVALID_OBJ_INDEX;
   cvw_core_output.cvw_id[FBK_SIDE_RIGHT]       = PA_INVALID_OBJ_ID;
   cvw_core_output.cvw_alert[FBK_SIDE_RIGHT]    = LCDA_ALERT_STATE_NONE;

   lcda_cals.k_cvw_alert_holding_cycles = 3u;

   cvw_persistent.f_prev_cvw_active[FBK_SIDE_LEFT]     = FBK_TRUE;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT] = obj_id;
   cvw_persistent.cvw_hold_counter[FBK_SIDE_LEFT]      = lcda_cals.k_cvw_alert_holding_cycles;

   /** \action Call function Lcda_Process_Cvw_Output that is responsible for alert holding with no turn signal. */
   Lcda_Process_Cvw_Output(&cvw_core_output, &cvw_persistent, &lcda_cals, TURN_SIGNAL_NONE);

   /** \assert Check that the alert is no longer held. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
}

/**
 * Create a target object and a guard rail on the left side of the ego such that the target object is behind the guard rail. Check
 * if target object is flagged as behind guardrail. \uts{CSCSA-42707} \sdd{SF-6681} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_Behind_Guardrail__is_true_when_object_is_behind_left_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t obj_id = 0;

   tracker_object.curvi_pos.y = -5.0f;
   tracker_object.width       = 1.9f;
   cvw_object.ego_side        = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);
   tracker_object.id          = obj_id;

   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.lateral_position = -4.0;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Cvw_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Cvw_Object_Behind_Guardrail(&cvw_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as behind guardrail. */
   EXPECT_TRUE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the left side of the ego such that the target object is overlapping the guard rail.
 * Check if target object is flagged as not behind guardrail. \uts{CSCSA-42708} \sdd{SF-6681} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_Behind_Guardrail__is_false_when_object_is_on_left_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t obj_id = 0;

   tracker_object.curvi_pos.y = -4.0f;
   tracker_object.width       = 2.0f;
   cvw_object.ego_side        = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);
   tracker_object.id          = obj_id;

   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.lateral_position = -4.0;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Cvw_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Cvw_Object_Behind_Guardrail(&cvw_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the left side of the ego such that the target object is between host and the guard
 * rail (not overlapping with any). Check if target object is flagged as not behind guardrail. \uts{CSCSA-42709} \sdd{SF-6681}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_Behind_Guardrail__is_false_when_object_is_ahead_of_left_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t obj_id = 0;

   tracker_object.curvi_pos.y = -3.0f;
   tracker_object.width       = 1.9f;
   cvw_object.ego_side        = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);
   tracker_object.id          = obj_id;

   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.lateral_position = -4.0;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Cvw_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Cvw_Object_Behind_Guardrail(&cvw_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the right side of the ego such that the target object is behind the guard rail. Check
 * if target object is flagged as behind guardrail. \uts{CSCSA-42710} \sdd{SF-6681} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_Behind_Guardrail__is_true_when_object_is_behind_right_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t obj_id = 0;

   tracker_object.curvi_pos.y = 5.0f;
   tracker_object.width       = 1.9f;
   cvw_object.ego_side        = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);
   tracker_object.id          = obj_id;

   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.lateral_position = 4.0;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Cvw_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Cvw_Object_Behind_Guardrail(&cvw_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as behind guardrail. */
   EXPECT_TRUE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the right side of the ego such that the target object is overlapping the guard rail.
 * Check if target object is flagged as not behind guardrail. \uts{CSCSA-42711} \sdd{SF-6681} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_Behind_Guardrail__is_false_when_object_is_on_right_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t obj_id = 0;

   tracker_object.curvi_pos.y = 4.0f;
   tracker_object.width       = 2.0f;
   cvw_object.ego_side        = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);
   tracker_object.id          = obj_id;

   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.lateral_position = 4.0;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Cvw_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Cvw_Object_Behind_Guardrail(&cvw_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}

/**
 * Create a target object and a guard rail on the right side of the ego such that the target object is between host and the guard
 * rail (not overlapping with any). Check if target object is flagged as not behind guardrail. \uts{CSCSA-42712} \sdd{SF-6681}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_Behind_Guardrail__is_false_when_object_is_ahead_of_right_guardrail)
{
   /** \arrange Create target object and guard rail. */
   boolean_T behind_guardrail;
   uint8_t obj_id = 0;

   tracker_object.curvi_pos.y = 3.0f;
   tracker_object.width       = 1.9f;
   cvw_object.ego_side        = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);
   tracker_object.id          = obj_id;

   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.lateral_position = 4.0;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[cvw_object.ego_side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Cvw_Object_Behind_Guardrail to check if target object is behind guard rail. */
   behind_guardrail = Lcda_Is_Cvw_Object_Behind_Guardrail(&cvw_object, lcda_core_input.guardrail_data);

   /** \assert Check if target object is flagged as not behind guardrail. */
   EXPECT_FALSE(behind_guardrail);
}


/**
 * Calculate critical distance for lane change intention in this case no bsw alert is given thus the critical distance shall be
 * shall be equal to the internal calculated one. \uts{CSCSA-42713} \sdd{SF-6811} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Calculate_Critical_Distance__no_bsw_alert_given_host_130_kmh_obj_150_kmh)
{
   /** \arrange Calibrations and persistent data for critical distance calculation. */
   float32_T expected_crit_dist;
   float32_T dist_reaction_time;
   float32_T dist_decel_after_lane_change;
   float32_T dist_dist_after_lc;


   lcda_cals.k_cvw_critical_obj_decel_after_lane_change   = 3.0f;
   lcda_cals.k_cvw_time_obj_start_decel_after_lane_change = 0.4f;
   lcda_cals.k_cvw_time_diff_after_obj_decel              = 1.0f;
   tracker_object.curvi_vel_rel.x                         = 20.0f / 3.6f;
   tracker_object.index                                   = 1;
   p_vehicle_data->host_speed                             = 130.0f / 3.6f;

   dist_reaction_time = cvw_object.p_tracker_data->curvi_vel_rel.x * lcda_cals.k_cvw_time_obj_start_decel_after_lane_change;
   dist_decel_after_lane_change =
      0.5f * (pow(cvw_object.p_tracker_data->curvi_vel_rel.x, 2.0f) / lcda_cals.k_cvw_critical_obj_decel_after_lane_change);
   dist_dist_after_lc = p_vehicle_data->host_speed * lcda_cals.k_cvw_time_diff_after_obj_decel;

   expected_crit_dist = dist_reaction_time + dist_decel_after_lane_change + dist_dist_after_lc;

   /** \action Calculate critical distance. */
   Lcda_Calculate_Critical_Distance(&cvw_object, p_vehicle_data, &lcda_cals, &cvw_persistent);

   /** \assert Compare returned critical distance with expected. */
   EXPECT_FLOAT_EQ(expected_crit_dist, cvw_object.critical_distance);
}


/**
 * Calculate critical distance for lane change intention in this case no bsw alert is given thus a critical distance shall be equal
 * to the internal calculated one with hysteresis applied. \uts{CSCSA-42714} \sdd{SF-6811} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Calculate_Critical_Distance__no_bsw_alert_given_host_130_kmh_obj_150_kmh_hysteresis_applied)
{
   /** \arrange Calibrations and persistent data for critical distance calculation. */
   float32_T expected_crit_dist;
   float32_T dist_reaction_time;
   float32_T dist_decel_after_lane_change;
   float32_T dist_dist_after_lc;


   lcda_cals.k_cvw_critical_obj_decel_after_lane_change   = 3.0f;
   lcda_cals.k_cvw_time_obj_start_decel_after_lane_change = 0.4f;
   lcda_cals.k_cvw_time_diff_after_obj_decel              = 1.0f;
   lcda_cals.k_cvw_crit_dist_hys_factor                   = 1.1f;
   lcda_cals.k_cvw_crit_dist_additive_hys                 = 5.0f;

   tracker_object.curvi_vel_rel.x = 20.0f / 3.6f;
   tracker_object.index           = 1;
   p_vehicle_data->host_speed     = 130.0f / 3.6f;

   cvw_persistent.f_prev_cvw_active[cvw_object.ego_side] = FBK_TRUE;
   dist_reaction_time = cvw_object.p_tracker_data->curvi_vel_rel.x * lcda_cals.k_cvw_time_obj_start_decel_after_lane_change;
   dist_decel_after_lane_change =
      0.5f * (pow(cvw_object.p_tracker_data->curvi_vel_rel.x, 2.0f) / lcda_cals.k_cvw_critical_obj_decel_after_lane_change);
   dist_dist_after_lc = p_vehicle_data->host_speed * lcda_cals.k_cvw_time_diff_after_obj_decel;

   expected_crit_dist = dist_reaction_time + dist_decel_after_lane_change + dist_dist_after_lc;
   expected_crit_dist = expected_crit_dist * lcda_cals.k_cvw_crit_dist_hys_factor + lcda_cals.k_cvw_crit_dist_additive_hys;

   /** \action Calculate critical distance. */
   Lcda_Calculate_Critical_Distance(&cvw_object, p_vehicle_data, &lcda_cals, &cvw_persistent);

   /** \assert Compare returned critical distance with expected. */
   EXPECT_FLOAT_EQ(expected_crit_dist, cvw_object.critical_distance);
}

/**
 * Checks whether Obj shall be considered as most critical one. Here no environment conflict is expected and the ttc condition is
 * fulfilled. Thus true is expected returned. \uts{CSCSA-42715} \sdd{SF-6810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Shall_Obj_Be_Considered_As_Most_Critical__environment_and_ttc_fulfilled)
{
   /** \arrange arrange flags. */
   boolean_T res;
   float32_T cvw_ttc_threshold;
   cvw_object.ego_side                                                = FBK_SIDE_LEFT;
   cvw_ttc_threshold                                                  = 1.5f;
   cvw_object.ttc                                                     = cvw_ttc_threshold - EPSILON;
   cvw_object.f_obj_in_ego_lane                                       = FBK_FALSE;
   cvw_object.f_obj_behind_guardrail                                  = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;

   /** \action Check whether object shall be examined as most critical on its side. */
   res = Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&cvw_object, &lcda_core_input, p_vehicle_data, cvw_ttc_threshold);

   /** \assert Check that true is returned. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether Obj shall be considered as most critical one. Here object is behind guardrail. Thus false is expected returned.
 * \uts{CSCSA-42716} \sdd{SF-6810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Shall_Obj_Be_Considered_As_Most_Critical__environment_conflict_due_to_object_behind_guardrail)
{
   /** \arrange arrange flags. */
   boolean_T res;
   float32_T cvw_ttc_threshold;
   cvw_object.ego_side                                                = FBK_SIDE_LEFT;
   cvw_ttc_threshold                                                  = 1.5f;
   cvw_object.ttc                                                     = cvw_ttc_threshold - EPSILON;
   cvw_object.f_obj_in_ego_lane                                       = FBK_FALSE;
   cvw_object.f_obj_behind_guardrail                                  = FBK_TRUE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;

   /** \action Check whether object shall be examined as most critical on its side. */
   res = Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&cvw_object, &lcda_core_input, p_vehicle_data, cvw_ttc_threshold);

   /** \assert Check that false is returned. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether Obj shall be considered as most critical one. Here object is in ego lane. Thus false is expected returned.
 * \uts{CSCSA-42717} \sdd{SF-6810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Shall_Obj_Be_Considered_As_Most_Critical__environment_conflict_due_to_object_being_in_host_lane)
{
   /** \arrange arrange flags. */
   boolean_T res;
   float32_T cvw_ttc_threshold;
   cvw_object.ego_side                                                = FBK_SIDE_LEFT;
   cvw_ttc_threshold                                                  = 1.5f;
   cvw_object.ttc                                                     = cvw_ttc_threshold - EPSILON;
   cvw_object.f_obj_in_ego_lane                                       = FBK_TRUE;
   cvw_object.f_obj_behind_guardrail                                  = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;

   /** \action Check whether object shall be examined as most critical on its side. */
   res = Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&cvw_object, &lcda_core_input, p_vehicle_data, cvw_ttc_threshold);

   /** \assert Check that false is returned. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether Obj shall be considered as most critical one. Here objects ttc is not critical enough. Thus false is expected
 * returned. \uts{CSCSA-42718} \sdd{SF-6810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Shall_Obj_Be_Considered_As_Most_Critical__ttc_gt_threshold)
{
   /** \arrange arrange flags. */
   boolean_T res;
   float32_T cvw_ttc_threshold;
   cvw_object.ego_side                                                = FBK_SIDE_LEFT;
   cvw_ttc_threshold                                                  = 1.5f;
   cvw_object.ttc                                                     = cvw_ttc_threshold + EPSILON;
   cvw_object.f_obj_in_ego_lane                                       = FBK_FALSE;
   cvw_object.f_obj_behind_guardrail                                  = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;

   /** \action Check whether object shall be examined as most critical on its side. */
   res = Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&cvw_object, &lcda_core_input, p_vehicle_data, cvw_ttc_threshold);

   /** \assert Check that false is returned. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether Obj shall be considered as most critical one. Here no environment conflict is expected and the critical distance
 * condition is fulfilled. Thus true is expected. returned. \uts{CSCSA-42719} \sdd{SF-6810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Shall_Obj_Be_Considered_As_Most_Critical__no_environment_conflict_and_critical_dist_cond_fulfilled)
{
   /** \arrange arrange flags. */
   boolean_T res;
   float32_T cvw_ttc_threshold;

   cvw_ttc_threshold                                                  = 0.5f;
   cvw_object.ego_side                                                = FBK_SIDE_LEFT;
   cvw_object.f_obj_in_ego_lane                                       = FBK_FALSE;
   cvw_object.f_obj_behind_guardrail                                  = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   tracker_object.curvi_pos.x     = -10.0f;
   tracker_object.length          = 4.0f;
   tracker_object.curvi_vel_rel.x = 1.0f;
   cvw_object.critical_distance = -(tracker_object.curvi_pos.x + 0.5f * tracker_object.length + p_vehicle_data->host_length) + EPSILON;

   /** \action Check whether object shall be examined as most critical on its side. */
   res = Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&cvw_object, &lcda_core_input, p_vehicle_data, cvw_ttc_threshold);

   /** \assert Check that true is returned. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether Obj shall be considered as most critical one. Here no environment conflict is expected but the critical distance
 * condition is not fulfilled, since distance to host is greater than critical distance. Thus false is expected. \uts{CSCSA-42720}
 * \sdd{SF-6810} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Shall_Obj_Be_Considered_As_Most_Critical__no_environment_conflict_but_distance_not_critical)
{
   /** \arrange arrange flags and distance to host greater than critical distance. */
   boolean_T res;
   float32_T cvw_ttc_threshold;

   cvw_ttc_threshold                                                  = 0.5f;
   cvw_object.ego_side                                                = FBK_SIDE_LEFT;
   cvw_object.f_obj_in_ego_lane                                       = FBK_FALSE;
   cvw_object.f_obj_behind_guardrail                                  = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   tracker_object.curvi_pos.x     = -10.0f;
   tracker_object.length          = 4.0f;
   tracker_object.curvi_vel_rel.x = 1.0f;
   cvw_object.critical_distance = -(tracker_object.curvi_pos.x + 0.5f * tracker_object.length + p_vehicle_data->host_length) - EPSILON;

   /** \action Check whether object shall be examined as most critical on its side. */
   res = Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&cvw_object, &lcda_core_input, p_vehicle_data, cvw_ttc_threshold);

   /** \assert Check that false is returned. */
   EXPECT_FALSE(res);
}

/**
 * Checks whether Obj shall be considered as most critical one. Here no environment conflict is expected but the critical distance
 * condition is not fulfilled, object is a fallback candidate. Thus false is expected. \uts{CSCSA-42721} \sdd{SF-6810}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Shall_Obj_Be_Considered_As_Most_Critical__no_environment_conflict_but_object_is_fallback_candidate)
{
   /** \arrange arrange flags and distance to host greater than critical distance. */
   boolean_T res;
   float32_T cvw_ttc_threshold;

   cvw_ttc_threshold                                                  = 0.5f;
   cvw_object.ego_side                                                = FBK_SIDE_LEFT;
   cvw_object.f_obj_in_ego_lane                                       = FBK_FALSE;
   cvw_object.f_obj_behind_guardrail                                  = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   tracker_object.curvi_pos.x     = -10.0f;
   tracker_object.length          = 4.0f;
   tracker_object.curvi_vel_rel.x = -1.0f;
   cvw_object.critical_distance = -(tracker_object.curvi_pos.x + 0.5f * tracker_object.length + p_vehicle_data->host_length) + EPSILON;

   /** \action Check whether object shall be examined as most critical on its side. */
   res = Lcda_Shall_Obj_Be_Considered_As_Most_Critical(&cvw_object, &lcda_core_input, p_vehicle_data, cvw_ttc_threshold);

   /** \assert Check that false is returned. */
   EXPECT_FALSE(res);
}

/**
 * Checks that lane change intention qualification cycles are returned in case that warn setting flag is set to true.
 * \uts{CSCSA-42722} \sdd{SF-6809} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Return_Mode_Dep_Min_Mature_Cycles__lane_change_intention_cycles_shall_be_returned)
{
   /** \arrange set up lane change intention mode to true */
   uint8_t res;
   lcda_cals.k_cvw_min_mature_cycles_lc_intention                     = 1;
   lcda_cals.k_cvw_min_mature_cycles                                  = 2;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   /** \action Call function to test */
   res = Lcda_Return_Mode_Dep_Min_Mature_Cycles(&lcda_core_input, &lcda_cals);

   /** \assert Check that lane change intention cycles are returned. */
   EXPECT_EQ(lcda_cals.k_cvw_min_mature_cycles_lc_intention, res);
}

/**
 * Check that usual mature cycle amount is returned in case that flag is deactivated.
 * \uts{CSCSA-42723} \sdd{SF-6809} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Return_Mode_Dep_Min_Mature_Cycles__min_mature_cycles_shall_be_returned)
{
   /** \arrange set up lane change intention mode to false */
   uint8_t res;
   lcda_cals.k_cvw_min_mature_cycles_lc_intention                     = 1;
   lcda_cals.k_cvw_min_mature_cycles                                  = 2;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;

   /** \action Call function to test */
   res = Lcda_Return_Mode_Dep_Min_Mature_Cycles(&lcda_core_input, &lcda_cals);

   /** \assert Check that standard cycle amount is returned. */
   EXPECT_EQ(lcda_cals.k_cvw_min_mature_cycles, res);
}

/*
 * Check that for an active alert in the current cycle the CVW post-processing resets the holding counter and fills the persistent
 * data according to this alert. \uts{CSCSA-42724} \sdd{SF-6695} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Postprocess_Cvw__resets_holding_counter_and_fills_persistent_data_for_active_alert)
{
   /** \arrange Set up core output with an alert and persistent data with holding counter unequal zero. */
   cvw_core_output.cvw_alert[FBK_SIDE_LEFT]                     = LCDA_ALERT_STATE_LEVEL_1;
   cvw_core_output.cvw_index[FBK_SIDE_LEFT]                     = 3u;
   cvw_persistent.cvw_hold_counter[FBK_SIDE_LEFT]               = 4u;
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   /** \action Call function Lcda_Postprocess_Cvw for post-processing of current CVW cycle. */
   Lcda_Postprocess_Cvw(&cvw_core_output, f_use_small_lc_intention_zone, &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Verify that holding counter is reseted and previous alert is set in persistent data. */
   EXPECT_TRUE(cvw_persistent.f_prev_cvw_active[FBK_SIDE_LEFT]);
   EXPECT_EQ(cvw_persistent.prev_cvw_alert_obj_index[FBK_SIDE_LEFT], cvw_core_output.cvw_index[FBK_SIDE_LEFT]);
   EXPECT_EQ(cvw_persistent.cvw_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/*
 * Check that after an object merge the mature count in zone of the merged object is taken over by the new object.
 * \uts{CSCSA-42725} \sdd{SF-6687} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Update_Cvw_Data_For_Merged_Objects__takes_over_mature_count_in_zone_from_merged_object)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_index    = 12u;
   uint8_t new_obj_id       = 9u;
   uint8_t merged_obj_index = 11u;
   uint8_t merged_id        = 6u;
   uint8_t mature_count     = 4u;

   cvw_persistent.mature_count_in_cvw_zone[merged_id]  = mature_count;
   cvw_persistent.mature_count_in_cvw_zone[new_obj_id] = 0u;
   object_data[merged_obj_index].id                    = merged_id;
   object_data[new_obj_index].id                       = new_obj_id;
   object_data[new_obj_index].f_merge_occured          = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj            = merged_id;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Cvw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Cvw_Data_For_Merged_Objects(&cvw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_EQ(cvw_persistent.mature_count_in_cvw_zone[new_obj_id], mature_count);
}

/*
 * Check that the pre-processing of CVW processes object merges and resets the CVW core output and closest object position.
 * \uts{CSCSA-42726} \sdd{SF-6696} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Preprocess_Cvw__processes_object_merges_and_resets_cvw_core_output_and_closest_obj_pos)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information and core output with
    * non-default values. */
   uint8_t new_obj_id                                           = 44u;
   uint8_t new_obj_index                                        = 3u;
   uint8_t merged_obj_index                                     = 12u;
   uint8_t mature_count                                         = 5u;
   uint8_t merged_id                                            = 7u;
   float32_T ttc_threshold[FBK_NUMBER_OF_SIDES]                 = {1.0f, 1.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   cvw_persistent.mature_count_in_cvw_zone[merged_id]  = mature_count;
   cvw_persistent.mature_count_in_cvw_zone[new_obj_id] = 0u;
   object_data[merged_obj_index].id                    = merged_id;
   object_data[new_obj_index].id                       = new_obj_id;
   object_data[new_obj_index].f_merge_occured          = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj            = merged_id;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_2;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT] = 14u;

   /** \action Call function Lcda_Preprocess_Cvw for pre-processing of current CVW cycle. */
   Lcda_Preprocess_Cvw(&cvw_core_output, ttc_threshold, f_use_small_lc_intention_zone, &lcda_core_input, &lcda_cals, &fbk_output,
                       &cvw_persistent);

   /** \assert Verify that object merges are processed and CVW core output and closest object position is reseted. */
   EXPECT_EQ(cvw_persistent.mature_count_in_cvw_zone[new_obj_id], mature_count);
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
}

/*
 * Check that a new critical object is considered most critical if no critical object was present before.
 * \uts{CSCSA-42727} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__sets_new_object_most_critical_if_no_object_was_critical_before)
{
   /** \arrange Set up cvw object and core output such that no other object was critical before. */
   uint8_t side = FBK_SIDE_RIGHT;

   tracker_object.index           = 5u;
   tracker_object.id              = 8u;
   tracker_object.unique_id       = 9u;
   tracker_object.curvi_pos.x     = -14.6f;
   tracker_object.curvi_pos.y     = 2.0f;
   tracker_object.curvi_heading   = 0.0f;
   tracker_object.length          = 4.8f;
   tracker_object.width           = 2.0f;
   tracker_object.curvi_vel_rel.x = 12.0f;
   cvw_object.ego_side            = side;
   cvw_object.ttc                 = 1.4f;
   cvw_object.obj_front_position  = -12.2f;

   /** \action Call function Lcda_Set_Most_Critical_Cvw_Object to update the most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Verify that the new object is set as most critical object. */
   EXPECT_EQ(cvw_core_output.cvw_index[side], tracker_object.index);
   EXPECT_EQ(cvw_core_output.cvw_id[side], tracker_object.id);
   EXPECT_EQ(cvw_core_output.cvw_unique_id[side], tracker_object.unique_id);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_ttc[side], cvw_object.ttc);
   EXPECT_EQ(cvw_core_output.cvw_alert[side], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance[side], cvw_object.obj_front_position);
   EXPECT_LT(cvw_core_output.cvw_ttp[side], LCDA_DEFAULT_LARGE_TTC);
}

/*
 * Check that a new critical object is not considered most critical if it is not closer to the ego than current most critical
 * object. \uts{CSCSA-42728} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__does_not_set_new_object_most_critical_if_it_is_not_closer_to_ego)
{
   /** \arrange Set up cvw object, core output and persistent data such that object is not closer to ego than current most critical
    * object. */
   uint8_t side       = FBK_SIDE_LEFT;
   float32_T prev_pos = -10.0f;
   float32_T prev_ttp = 2.5f;

   cvw_core_output.cvw_index[side]    = 24u;
   cvw_core_output.cvw_distance[side] = prev_pos;
   cvw_core_output.cvw_ttp[side]      = prev_ttp;
   tracker_object.index               = 5u;
   tracker_object.id                  = 8u;
   tracker_object.unique_id           = 9u;
   cvw_object.ego_side                = side;
   cvw_object.ttc                     = 1.4f;
   cvw_object.obj_front_position      = prev_pos - 5.0f;


   /** \action Call function Lcda_Set_Most_Critical_Cvw_Object to update the most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Verify that the new object is not set as most critical object. */
   EXPECT_NE(cvw_core_output.cvw_index[side], tracker_object.index);
   EXPECT_NE(cvw_core_output.cvw_id[side], tracker_object.id);
   EXPECT_NE(cvw_core_output.cvw_unique_id[side], tracker_object.unique_id);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance[side], prev_pos);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_ttp[side], prev_ttp);
}

/*
 * Check that persistent storage of CVW curve zone factor works properly.
 * \uts{CSCSA-42729} \sdd{SF-6699} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Store_Pers_Cvw_Curve_Zone_Factor__works_properly)
{
   /** \arrange Set up curve factor to store and set persistent value on different value. */
   uint8_t side                                = FBK_SIDE_LEFT;
   float32_T curve_factor                      = 1.5f;
   cvw_persistent.prev_curve_zone_factor[side] = 2.5f;

   /** \action Call function Lcda_Store_Pers_Cvw_Curve_Zone_Factor to set new persistent curve factor. */
   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, curve_factor, side);

   /** \assert Verify that new curve factor is stored in persistent data. */
   EXPECT_FLOAT_EQ(cvw_persistent.prev_curve_zone_factor[side], curve_factor);
}

/*
 * Check that stored persistent value of CVW curve zone factor is correctly returned by get-function.
 * \uts{CSCSA-42730} \sdd{SF-6692} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor__works_properly)
{
   /** \arrange Set up curve factor in persistent data. */
   float32_T curve_factor;
   uint8_t side                                = FBK_SIDE_LEFT;
   cvw_persistent.prev_curve_zone_factor[side] = 4.5f;

   /** \action Call function Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor to get stored persistent curve factor. */
   curve_factor = Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor(side, &cvw_persistent);

   /** \assert Verify that curve factor from persistent data is returned. */
   EXPECT_FLOAT_EQ(cvw_persistent.prev_curve_zone_factor[side], curve_factor);
}

/*
 * Check that correct ttc threshold is computed if no BSW or CVW alert have been present in the last cycle.
 * \uts{CSCSA-42731} \sdd{SF-6679} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Get_Time_To_Conflict_Threshold__returns_default_ttc_if_no_prev_alerts_are_present)
{
   /** \arrange Set up default ttc threshold and flags indicating that no alert was active previously. */
   boolean_T f_prev_bsw_alert = FBK_FALSE;
   boolean_T f_prev_cvw_alert = FBK_FALSE;
   Lcda_Warn_Settings_T warn_settings;
   float32_T result_ttc_threshold;

   warn_settings.cvw_ttc_threshold = 2.0f;

   /** \action Call function Lcda_Get_Time_To_Conflict_Threshold to compute ttc threshold. */
   result_ttc_threshold = Lcda_Get_Time_To_Conflict_Threshold(&lcda_cals, f_prev_cvw_alert, f_prev_bsw_alert, &warn_settings);

   /** \assert Verify that computed threshold is equal to default threshold without previous alerts. */
   EXPECT_FLOAT_EQ(result_ttc_threshold, warn_settings.cvw_ttc_threshold);
}

/*
 * Check that correct ttc threshold is computed if BSW alert has been present in the last cycle.
 * \uts{CSCSA-42732} \sdd{SF-6679} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Get_Time_To_Conflict_Threshold__returns_altered_ttc_if_prev_bsw_alert_is_present)
{
   /** \arrange Set up default ttc threshold and flags indicating that a BSW alert was active previously. */
   boolean_T f_prev_bsw_alert = FBK_TRUE;
   boolean_T f_prev_cvw_alert = FBK_FALSE;
   Lcda_Warn_Settings_T warn_settings;
   float32_T result_ttc_threshold;

   warn_settings.cvw_ttc_threshold = 2.0f;
   lcda_cals.k_cvw_gap_bridge      = 3.0f;

   /** \action Call function Lcda_Get_Time_To_Conflict_Threshold to compute ttc threshold. */
   result_ttc_threshold = Lcda_Get_Time_To_Conflict_Threshold(&lcda_cals, f_prev_cvw_alert, f_prev_bsw_alert, &warn_settings);

   /** \assert Verify that computed threshold is altered from default threshold since a BSW alert was present last cycle. */
   EXPECT_FLOAT_EQ(result_ttc_threshold, warn_settings.cvw_ttc_threshold + lcda_cals.k_cvw_gap_bridge);
}

/*
 * Check that correct ttc threshold is computed if CVW alert has been present in the last cycle.
 * \uts{CSCSA-42733} \sdd{SF-6679} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Get_Time_To_Conflict_Threshold__returns_altered_ttc_if_prev_cvw_alert_is_present)
{
   /** \arrange Set up default ttc threshold and flags indicating that a CVW alert was active previously. */
   boolean_T f_prev_bsw_alert = FBK_FALSE;
   boolean_T f_prev_cvw_alert = FBK_TRUE;
   Lcda_Warn_Settings_T warn_settings;
   float32_T result_ttc_threshold;

   warn_settings.cvw_ttc_threshold = 2.0f;
   lcda_cals.k_cvw_ttc_hys         = 3.0f;

   /** \action Call function Lcda_Get_Time_To_Conflict_Threshold to compute ttc threshold. */
   result_ttc_threshold = Lcda_Get_Time_To_Conflict_Threshold(&lcda_cals, f_prev_cvw_alert, f_prev_bsw_alert, &warn_settings);

   /** \assert Verify that computed threshold is altered from default threshold since a CVW alert was present last cycle. */
   EXPECT_FLOAT_EQ(result_ttc_threshold, warn_settings.cvw_ttc_threshold + lcda_cals.k_cvw_ttc_hys);
}

/*
 * Check that object is classified as relevant if all criteria are fulfilled with object status mature.
 * \uts{CSCSA-42734} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__returns_true_if_all_criteria_are_fulfilled_with_status_mature)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status mature. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                               = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading                        = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.curvi_vel.x                          = lcda_cals.k_cvw_min_obj_curvi_long_vel + EPSILON;
   tracker_object.curvi_vel_rel.x                      = lcda_cals.k_cvw_min_object_curvi_relative_speed[0] + EPSILON;
   tracker_object.existence_probability                = 1.0f;
   lcda_core_input.warn_settings.cvw_rel_vel_range.min = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
   lcda_core_input.warn_settings.cvw_rel_vel_range.max = lcda_cals.k_cvw_max_object_curvi_relative_speed;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as relevant. */
   EXPECT_TRUE(result);
}

/*
 * Check that object is not classified as relevant if heading is out of range.
 * \uts{CSCSA-42735} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__returns_false_if_heading_is_out_of_range)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed, except for heading being out of range. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading         = lcda_cals.k_cvw_max_curvi_heading_abs + EPSILON;
   tracker_object.curvi_vel.x           = lcda_cals.k_cvw_min_obj_curvi_long_vel + EPSILON;
   tracker_object.curvi_vel_rel.x       = lcda_cals.k_cvw_min_object_curvi_relative_speed[0] + EPSILON;
   tracker_object.existence_probability = 1.0f;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_FALSE(result);
}

/*
 * Check that object is not classified as relevant if velocity is out of range.
 * \uts{CSCSA-42736} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__returns_false_if_velocity_is_out_of_range)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed, except for velocity being out of range. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading         = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.curvi_vel.x           = lcda_cals.k_cvw_min_obj_curvi_long_vel - EPSILON;
   tracker_object.curvi_vel_rel.x       = lcda_cals.k_cvw_min_object_curvi_relative_speed[0] + EPSILON;
   tracker_object.existence_probability = 1.0f;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_FALSE(result);
}

/*
 * Check that object is classified as relevant if all criteria are fulfilled with object status new and
 * k_lcda_allow_track_status_new activated. \uts{CSCSA-42737} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__returns_true_if_all_criteria_are_fulfilled_with_status_new)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status new and enable
    * k_lcda_allow_track_status_new. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                               = PA_OBJ_STATUS_NEW;
   tracker_object.curvi_heading                        = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.curvi_vel.x                          = lcda_cals.k_cvw_min_obj_curvi_long_vel + EPSILON;
   tracker_object.curvi_vel.x                          = lcda_cals.k_cvw_min_obj_curvi_long_vel + EPSILON;
   tracker_object.curvi_vel_rel.x                      = lcda_cals.k_cvw_min_object_curvi_relative_speed[0] + EPSILON;
   tracker_object.existence_probability                = 1.0f;
   lcda_core_input.warn_settings.cvw_rel_vel_range.min = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
   lcda_core_input.warn_settings.cvw_rel_vel_range.max = lcda_cals.k_cvw_max_object_curvi_relative_speed;

   lcda_cals.k_lcda_allow_track_status_new              = FBK_TRUE;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as relevant. */
   EXPECT_TRUE(result);
}

/**
 * Create CVW warn relevant target object which is fully qualified. Set the hosts turn signal to none. Process the CVW warning
 * state and check that the alert level is LCDA_ALERT_STATE_LEVEL_1. \uts{CSCSA-42738} \sdd{SF-6697}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__cvw_alert_level_1_when_obj_is_in_cvw_zone_and_ego_base_checks)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t obj_id                                         = 20;
   const float32_T obj_lon_pos                                  = -16.0f;
   const float32_T obj_lat_pos                                  = 2.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_cals.k_cvw_zone_calculation_mode = CVW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_cvw_min_mature_cycles     = 1;                    // so that CVW alert is
                                                                 // given when the object
                                                                 // is in the zone even for
                                                                 // 1 cycle
   lcda_cals.k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.vcs_pos.y                            = obj_lat_pos;
   tracker_object.vcs_pos.x                            = obj_lon_pos;
   tracker_object.curvi_pos.y                          = obj_lat_pos;
   tracker_object.curvi_pos.x                          = obj_lon_pos;
   tracker_object.vcs_vel_rel.x                        = 5.0f;
   lcda_core_input.warn_settings.cvw_rel_vel_range.min = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
   lcda_core_input.warn_settings.cvw_rel_vel_range.max = lcda_cals.k_cvw_max_object_curvi_relative_speed;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_RIGHT], obj_id);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_RIGHT], tracker_object.id);
}

/**
 * Create CVW warn relevant target object which is not mature. Set the hosts turn signal to none. Process the CVW warning state and
 * check that there is no alert. \uts{CSCSA-186406} \sdd{SF-6697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__no_cvw_alert_not_mature_obj_in_cvw_zone)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t obj_id                                         = 20;
   const float32_T obj_lon_pos                                  = -16.0f;
   const float32_T obj_lat_pos                                  = 2.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_cals.k_cvw_zone_calculation_mode             = CVW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_cvw_min_mature_cycles                 = 10;
   lcda_cals.k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.vcs_pos.y                        = obj_lat_pos;
   tracker_object.vcs_pos.x                        = obj_lon_pos;
   tracker_object.curvi_pos.y                      = obj_lat_pos;
   tracker_object.curvi_pos.x                      = obj_lon_pos;
   cvw_persistent.mature_count_in_cvw_zone[obj_id] = 0;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that there is no alert. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_RIGHT], 0u);
}

/**
 * Create CVW warn relevant target object which is fully qualified. Set the hosts turn signal to none. Process the CVW warning.
 * Object will be classified as less critical due to position wrt ego lane (f_obj_in_ego_lane) \uts{CSCSA-42739} \sdd{SF-6697}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__obj_less_critical)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t obj_idx                                        = 20;
   const float32_T obj_lon_pos                                  = -16.0f;
   const float32_T obj_lat_pos                                  = 2.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_cals.k_cvw_zone_calculation_mode = CVW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_cvw_min_mature_cycles     = 1;                        // so that CVW alert is
                                                                     // given when the object
                                                                     // is in the zone even for
                                                                     // 1 cycle
   lcda_cals.k_lcda_ego_lane_check_center_point_only     = FBK_TRUE; // Use the simple center point check for ego lane occupation
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check       = FBK_TRUE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor = 2.0f; // Change lane width factor to classify object as less critical

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_idx);
   tracker_object.vcs_pos.y   = obj_lat_pos;
   tracker_object.vcs_pos.x   = obj_lon_pos;
   tracker_object.curvi_pos.y = obj_lat_pos;
   tracker_object.curvi_pos.x = obj_lon_pos;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the correct target object is does not raise the alert. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/**
 * Create CVW warn relevant target object which is fully qualified. Set the hosts turn signal to none. Process the CVW warning.
 * Object will be classified as less critical due to position wrt ego lane (f_obj_in_ego_lane) \uts{CSCSA-186407} \sdd{SF-6697}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__obj_less_critical_calib_true)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t obj_idx       = 20;
   const float32_T obj_lon_pos = -16.0f;
   const float32_T obj_lat_pos = 2.0f;
   float32_T front_position;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_cals.k_cvw_zone_calculation_mode = CVW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_cvw_min_mature_cycles     = 1;                        // so that CVW alert is
                                                                     // given when the object
                                                                     // is in the zone even for
                                                                     // 1 cycle
   lcda_cals.k_lcda_ego_lane_check_center_point_only     = FBK_TRUE; // Use the simple center point check for ego lane occupation
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check       = FBK_TRUE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor = 2.0f; // Change lane width factor to classify object as less critical
   lcda_cals.k_cvw_f_most_crit_obj_must_be_closest_relevant_obj = FBK_TRUE;
   lcda_core_input.warn_settings.cvw_rel_vel_range.min          = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
   lcda_core_input.warn_settings.cvw_rel_vel_range.max          = lcda_cals.k_cvw_max_object_curvi_relative_speed;

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_idx);
   tracker_object.vcs_pos.y   = obj_lat_pos;
   tracker_object.vcs_pos.x   = obj_lon_pos;
   tracker_object.curvi_pos.y = obj_lat_pos;
   tracker_object.curvi_pos.x = obj_lon_pos;
   front_position             = obj_lon_pos + Fbk_Half(tracker_object.length);

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the correct target object is does not raise the alert. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance[FBK_SIDE_RIGHT], front_position);
}

/**
 * Create object which ttc is lower then zero so that a cvw alert is not triggered and check that the alert level is
 * LCDA_ALERT_STATE_NONE and ttc is set to default no alert value. \uts{CSCSA-42740} \sdd{SF-6697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__no_cvw_alert_long_pos)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t obj_id                                         = 20;
   const float32_T obj_lon_pos                                  = -0.1f;
   const float32_T obj_lat_pos                                  = 2.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.vcs_pos.y   = obj_lat_pos;
   tracker_object.vcs_pos.x   = obj_lon_pos;
   tracker_object.curvi_pos.y = obj_lat_pos;
   tracker_object.curvi_pos.x = obj_lon_pos;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT], LCDA_CVW_DEFAULT_NO_ALERT_TTC);
}

/**
 * Create CVW warn relevant target object which is fully qualified. Set the hosts turn signal to none. Process the CVW warning
 * state and check that the alert level is LCDA_ALERT_STATE_LEVEL_1. Additionally activate the ego lane check and the CVW lane
 * change intention zone. Both should not affect the warning. \uts{CSCSA-42741} \sdd{SF-6697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__cvw_alert_level_1_when_obj_is_in_cvw_zone_and_ego_additional_checks)
{
   /** \arrange Set up target object and host vehicle state. Activate ego lane check and CVW lane change intention zone. */
   const uint8_t obj_id                                         = 20;
   const float32_T obj_lon_pos                                  = -16.0f;
   const float32_T obj_lat_pos                                  = 2.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_cals.k_cvw_zone_calculation_mode = CVW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_cvw_min_mature_cycles     = 10; // so that CVW alert is
                                               // given when the object
                                               // is in the zone even
                                               // for 1 cycle

   cvw_persistent.mature_count_in_cvw_zone[obj_id]           = 0u;
   cvw_persistent.f_prev_cvw_active[FBK_SIDE_RIGHT]          = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check           = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check = FBK_FALSE;
   (&lcda_cals)->k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
   lcda_core_input.warn_settings.cvw_rel_vel_range.min                = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
   lcda_core_input.warn_settings.cvw_rel_vel_range.max                = lcda_cals.k_cvw_max_object_curvi_relative_speed;

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.vcs_pos.y   = obj_lat_pos;
   tracker_object.vcs_pos.x   = obj_lon_pos;
   tracker_object.curvi_pos.y = obj_lat_pos;
   tracker_object.curvi_pos.x = obj_lon_pos;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_RIGHT], obj_id);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_RIGHT], tracker_object.id);
}

/**
 * If a warning was qualifying/active last cycle, the persistent data is non default. Process the CVW warning level for a non
 * relevant object and check that neither a warning is created nor the persistent data remains non-default. \uts{CSCSA-42742}
 * \sdd{SF-6697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__reset_persistent_object_data)
{
   /** \arrange Set up target object (not warning relevant) and host vehicle state. */
   const uint8_t obj_id                                         = 20;
   const float32_T obj_lon_pos                                  = -120.0f;
   const float32_T obj_lat_pos                                  = 2.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   cvw_persistent.mature_count_in_cvw_zone[obj_id] = 5u;

   // Track on right of the ego, outside the default CVW zone
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.vcs_pos.y   = obj_lat_pos;
   tracker_object.vcs_pos.x   = obj_lon_pos;
   tracker_object.curvi_pos.y = obj_lat_pos;
   tracker_object.curvi_pos.x = obj_lon_pos;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the persistent data is reset and that the alert state level is LCDA_ALERT_STATE_NONE. */
   EXPECT_EQ(cvw_persistent.mature_count_in_cvw_zone[obj_id], 0u);
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/**
 * Object not qualified due to high relative speed
 * \uts{CSCSA-187419} \sdd{SF-6697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__non_relevant_object)
{
   /** \arrange Set up target object (not warning relevant) and host vehicle state. */
   const uint8_t obj_id                                         = 20;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   cvw_persistent.mature_count_in_cvw_zone[obj_id] = 5u;

   // Non relevant Track on right of the ego
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.existence_probability = 0.0f;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the alert state level is LCDA_ALERT_STATE_NONE. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/**
 * Check that CVW zone with hysteresis is used if mature count in CVW zone count is above threshold for object in question.
 * \uts{CSCSA-42743} \sdd{SF-6676} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Create_Cvw_Object_Zone__zone_is_cvw_zone_hys_when_mature_count_in_cvw_zone_is_above_threshold)
{
   /** \arrange Set up target object and mature counter for CVW zone above threshold. Set zone calculation method to fixed input
    * from core input. */
   uint8_t side                                                 = FBK_SIDE_RIGHT;
   uint8_t obj_id                                               = 4;
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   tracker_object.id   = obj_id;
   cvw_object.ego_side = side;

   cvw_persistent.mature_count_in_cvw_zone[obj_id] =
      Max(lcda_cals.k_cvw_min_mature_cycles_lc_intention, lcda_cals.k_cvw_min_mature_cycles) + 1;
   lcda_cals.k_cvw_zone_calculation_mode = CVW_ZONE_CALC_FIXED_INPUT;

   /** \action Call Lcda_Create_Cvw_Object_Zone to create CVW zone. */
   Lcda_Create_Cvw_Object_Zone(&cvw_object, &cvw_persistent, f_use_small_lc_intention_zone, &lcda_core_input, p_vehicle_data,
                               &lcda_cals);

   /** \assert Check that CVW zone is equal to hysteresis zone defined in core input. */
   for (uint8_t point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(cvw_object.zone.points[point].x, default_cvw_zone_hys.points[point].x);
      ASSERT_EQ(cvw_object.zone.points[point].y, default_cvw_zone_hys.points[point].y);
   }
}

/**
 * Check that CVW zone without hysteresis is used if mature count in CVW zone count is below threshold for object in question.
 * \uts{CSCSA-42744} \sdd{SF-6676} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Create_Cvw_Object_Zone__zone_is_cvw_zone_when_mature_count_in_cvw_zone_is_below_threshold)
{
   /** \arrange Set up target object and mature counter for CVW zone below threshold. Set zone calculation method to fixed input
    * from core input. */
   uint8_t side                                                 = FBK_SIDE_RIGHT;
   uint8_t obj_id                                               = 4;
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   tracker_object.id   = obj_id;
   cvw_object.ego_side = side;

   lcda_cals.k_cvw_min_mature_cycles_lc_intention  = 3u;
   lcda_cals.k_cvw_min_mature_cycles               = 3u;
   cvw_persistent.mature_count_in_cvw_zone[obj_id] = 0u;
   lcda_cals.k_cvw_zone_calculation_mode           = CVW_ZONE_CALC_FIXED_INPUT;

   /** \action Call Lcda_Create_Cvw_Object_Zone to create CVW zone. */
   Lcda_Create_Cvw_Object_Zone(&cvw_object, &cvw_persistent, f_use_small_lc_intention_zone, &lcda_core_input, p_vehicle_data,
                               &lcda_cals);

   /** \assert Check that CVW zone is equal to normal zone defined in core input. */
   for (uint8_t point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(cvw_object.zone.points[point].x, default_cvw_zone.points[point].x);
      ASSERT_EQ(cvw_object.zone.points[point].y, default_cvw_zone.points[point].y);
   }
}

/**
 * Check that small lane chane intention zone is used if no guardrail is present and calibration values are set accordingly.
 * \uts{CSCSA-42745} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_true_if_no_guardrail_present_and_cal_is_set_accordingly)
{
   /** \arrange Set up core input, persistent data and calibration values, such that no guardrail is present and small zone should
    * be used in this case. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_INVALID;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_TRUE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_TRUE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that small zone is used. */
   EXPECT_TRUE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}

/**
 * Check that normal lane chane intention zone is used if no guardrail is present and calibration values are set accordingly.
 * \uts{CSCSA-42746} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_false_if_no_guardrail_present_and_cal_is_set_accordingly)
{
   /** \arrange Set up core input, persistent data and calibration values, such that no guardrail is present and normal zone should
    * be used in this case. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_INVALID;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_FALSE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_FALSE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that normal zone is used. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}

/**
 * Check that used lane chane intention zone is not changed compared to previous cycle if zone change counter is below threshold.
 * \uts{CSCSA-42747} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_same_result_as_last_cycle_if_zone_change_counter_below_threshold)
{
   /** \arrange Set up core input, persistent data and calibration values, such that small zone was used last cycle and normal zone
    * would be used this cycle, but zone change counter is below threshold. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_INVALID;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_TRUE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_FALSE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that small zone is used. */
   EXPECT_TRUE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], 3u);
}

/**
 * Check that used lane chane intention zone is changed compared to previous cycle if zone change counter is at threshold.
 * \uts{CSCSA-42748} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_different_result_as_last_cycle_if_zone_change_counter_at_threshold)
{
   /** \arrange Set up core input, persistent data and calibration values, such that small zone was used last cycle and normal zone
    * would be used this cycle and zone change counter is at threshold. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_INVALID;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 3u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_TRUE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_FALSE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that normal zone is used. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}

/**
 * Check that small lane chane intention zone is used if a guardrail is present and guardrail lateral position is above minimum.
 * \uts{CSCSA-42749} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_true_if_guardrail_is_present_and_but_lat_pos_above_minimum)
{
   /** \arrange Set up core input, persistent data and calibration values, such that a guardrail is present and lateral position is
    * above minimum. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone = 9.0f;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_VALID;
   lcda_core_input.guardrail_data[side].radar.lateral_position =
      -lcda_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone - EPSILON;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_TRUE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_FALSE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that small zone is used. */
   EXPECT_TRUE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}

/**
 * Check that normal lane chane intention zone is used if a guardrail is present and guardrail lateral position is below minimum.
 * \uts{CSCSA-42750} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_false_if_guardrail_is_present_and_lat_pos_below_minimum)
{
   /** \arrange Set up core input, persistent data and calibration values, such that a guardrail is present and lateral position is
    * below minimum. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone = 9.0f;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_VALID;
   lcda_core_input.guardrail_data[side].radar.lateral_position =
      -lcda_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone + EPSILON;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_FALSE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_TRUE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that normal zone is used. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}

/**
 * Test if criticallity is correctly set by the lateral distance
 * \uts{CSCSA-42751} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_lat_distance_left)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance_lat[FBK_SIDE_LEFT] = 5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_LEFT]    = CVW_CRIT_LAT_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_LEFT]        = 5u;
   cvw_object.obj_front_position_lat               = 4.0f;
   cvw_object.ego_side                             = FBK_SIDE_LEFT;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_LEFT], cvw_object.p_tracker_data->index);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance_lat[FBK_SIDE_LEFT], cvw_object.obj_front_position_lat);
}

/**
 * Test if criticallity is correctly set by the lateral distance, object is too far
 * \uts{CSCSA-42752} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_lat_distance_left_too_far)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance_lat[FBK_SIDE_LEFT] = 5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_LEFT]    = CVW_CRIT_LAT_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_LEFT]        = 5u;
   cvw_object.obj_front_position_lat               = 6.0f;
   cvw_object.ego_side                             = FBK_SIDE_LEFT;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_LEFT], 5u);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance_lat[FBK_SIDE_LEFT], 5.0f);
}

/**
 * Test if criticallity is correctly set by the lateral distance
 * \uts{CSCSA-42753} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_lat_distance_right)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance_lat[FBK_SIDE_RIGHT] = 5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_RIGHT]    = CVW_CRIT_LAT_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT]        = 5u;
   cvw_object.obj_front_position_lat                = 4.0f;
   cvw_object.ego_side                              = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_RIGHT], cvw_object.p_tracker_data->index);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance_lat[FBK_SIDE_RIGHT], cvw_object.obj_front_position_lat);
}

/**
 * Test if criticallity is correctly set by the lateral distance, object is too far
 * \uts{CSCSA-42754} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_lat_distance_right_too_far)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance_lat[FBK_SIDE_RIGHT] = 5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_RIGHT]    = CVW_CRIT_LAT_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT]        = 5u;
   cvw_object.obj_front_position_lat                = 6.0f;
   cvw_object.ego_side                              = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_RIGHT], 5u);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance_lat[FBK_SIDE_RIGHT], 5.0f);
}

/**
 * Test if criticallity is correctly set by the long distance,
 * \uts{CSCSA-42755} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_long_distance_right)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance[FBK_SIDE_RIGHT]  = -5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_RIGHT] = CVW_CRIT_LONG_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT]     = 5u;
   cvw_object.obj_front_position                 = -4.0f;
   cvw_object.ego_side                           = FBK_SIDE_RIGHT;
   tracker_object.curvi_pos.x                    = -6.5f;
   tracker_object.curvi_pos.y                    = 2.0f;
   tracker_object.curvi_heading                  = 0.0f;
   tracker_object.length                         = 5.0f;
   tracker_object.width                          = 2.0f;
   tracker_object.curvi_vel_rel.x                = 12.0f;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_RIGHT], cvw_object.p_tracker_data->index);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance[FBK_SIDE_RIGHT], -4.0f);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_ttp[FBK_SIDE_RIGHT], 0.75f);
}

/**
 * Test if criticallity is correctly set by the long distance, obj too far
 * \uts{CSCSA-42756} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_long_distance_right_too_far)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance[FBK_SIDE_RIGHT]  = -5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_RIGHT] = CVW_CRIT_LONG_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_RIGHT]     = 5u;
   cvw_object.obj_front_position                 = -6.0f;
   cvw_object.ego_side                           = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_RIGHT], 5u);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance[FBK_SIDE_RIGHT], -5.0f);
}

/**
 * Test if criticallity is correctly set by the long distance,
 * \uts{CSCSA-42757} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_long_distance_left)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance[FBK_SIDE_LEFT]  = -5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_LEFT] = CVW_CRIT_LONG_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_LEFT]     = 5u;
   cvw_object.obj_front_position                = -4.0f;
   cvw_object.ego_side                          = FBK_SIDE_LEFT;
   tracker_object.curvi_pos.x                   = -6.5f;
   tracker_object.curvi_pos.y                   = -2.0f;
   tracker_object.curvi_heading                 = 0.0f;
   tracker_object.length                        = 5.0f;
   tracker_object.width                         = 2.0f;
   tracker_object.curvi_vel_rel.x               = 12.0f;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_LEFT], cvw_object.p_tracker_data->index);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance[FBK_SIDE_LEFT], -4.0f);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_ttp[FBK_SIDE_LEFT], 0.75f);
}

/**
 * Test if criticallity is correctly set by the long distance, obj too far
 * \uts{CSCSA-42758} \sdd{SF-6686} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Set_Most_Critical_Cvw_Object__criticallity_by_long_distance_left_too_far)
{
   /** \arrange Set up target object and current state. */
   cvw_core_output.cvw_distance[FBK_SIDE_LEFT]  = -5.0f;
   lcda_core_input.cvw_crit_mode[FBK_SIDE_LEFT] = CVW_CRIT_LONG_DIST;
   cvw_core_output.cvw_index[FBK_SIDE_LEFT]     = 5u;
   cvw_object.obj_front_position                = -6.0f;
   cvw_object.ego_side                          = FBK_SIDE_LEFT;

   /** \action Call Lcda_Set_Most_Critical_Cvw_Object to set most critical object. */
   Lcda_Set_Most_Critical_Cvw_Object(&cvw_core_output, &lcda_core_input, &cvw_object, TURN_SIGNAL_NONE);

   /** \assert Check that the output data is correct */
   EXPECT_EQ(cvw_core_output.cvw_index[FBK_SIDE_LEFT], 5u);
   EXPECT_FLOAT_EQ(cvw_core_output.cvw_distance[FBK_SIDE_LEFT], -5.0f);
}

/**
 * Check that normal lane chane intention zone is used if a guardrail is present and guardrail lateral position is below minimum.
 * \uts{CSCSA-42759} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_false_if_guardrail_is_present_and_lat_pos_below_minimum_positive)
{
   /** \arrange Set up core input, persistent data and calibration values, such that a guardrail is present and lateral position is
    * below minimum. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone = 9.0f;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_VALID;
   lcda_core_input.guardrail_data[side].radar.lateral_position =
      -(-lcda_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone + EPSILON);

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_FALSE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_TRUE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that normal zone is used. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}

/**
 * Calculate critical distance for object with valid index.
 * \uts{CSCSA-42760} \sdd{SF-6811} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Calculate_Critical_Distance__not_invalid_index)
{
   /** \arrange Calibrations and persistent data for critical distance calculation. */
   float32_T distance                                   = 3.0f;
   cvw_object.critical_distance                         = distance;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_LEFT]  = 10u;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = 10u;

   /** \action Calculate critical distance. */
   Lcda_Calculate_Critical_Distance(&cvw_object, p_vehicle_data, &lcda_cals, &cvw_persistent);

   /** \assert Returned distance should not change. */
   EXPECT_FLOAT_EQ(distance, cvw_object.critical_distance);
}

/*
 * Check that cvw data is updated correctly with high mature count (for branch coverage)
 * \uts{CSCSA-42761} \sdd{SF-6687} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Update_Cvw_Data_For_Merged_Objects__high_mature_count)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_index    = 3u;
   uint8_t merged_obj_index = 12u;
   uint8_t mature_count     = 5u;
   uint8_t merged_id        = 7u;

   cvw_persistent.mature_count_in_cvw_zone[merged_obj_index] = mature_count;
   cvw_persistent.mature_count_in_cvw_zone[new_obj_index]    = 10u;
   object_data[merged_obj_index].id                          = merged_id;
   object_data[new_obj_index].f_merge_occured                = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj                  = merged_id;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Cvw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Cvw_Data_For_Merged_Objects(&cvw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_EQ(cvw_persistent.mature_count_in_cvw_zone[new_obj_index], 10u);
}

/*
 * Check that cvw data is updated correctly with wrong id of merged object
 * \uts{CSCSA-42762} \sdd{SF-6687} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Update_Cvw_Data_For_Merged_Objects__wrong_id)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_index    = 3u;
   uint8_t merged_obj_index = 12u;
   uint8_t mature_count     = 5u;
   uint8_t merged_id        = 7u;

   cvw_persistent.mature_count_in_cvw_zone[merged_obj_index] = mature_count;
   cvw_persistent.mature_count_in_cvw_zone[new_obj_index]    = 10u;
   object_data[merged_obj_index].id                          = merged_id;
   object_data[new_obj_index].f_merge_occured                = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj                  = 255u;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Cvw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Cvw_Data_For_Merged_Objects(&cvw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_EQ(cvw_persistent.mature_count_in_cvw_zone[new_obj_index], 10u);
}

/*
 * Check that lane change is detected for different combinatiosn of parameters (to increase branch coverage)
 * \uts{CSCSA-42763} \sdd{SF-6965} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Lane_Change_Detected__test_all)
{
   /** \arrange */
   boolean_T result1;
   boolean_T result2;
   boolean_T result3;
   boolean_T result4;
   boolean_T result5;
   boolean_T result6;
   boolean_T result7;

   cvw_object.ego_side                           = FBK_SIDE_LEFT;
   lcda_core_input.f_lane_change[FBK_SIDE_LEFT]  = FBK_TRUE;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_TRUE;
   lcda_core_input.lane_width                    = 3.0f;
   tracker_object.curvi_pos.y                    = 2.0f;
   lcda_cals.k_lcda_min_lane_width               = 2.0f;

   /** \action */
   result1 = Lcda_Is_Lane_Change_Detected(&lcda_core_input, &cvw_object, (&lcda_cals));

   cvw_object.ego_side = FBK_SIDE_RIGHT;
   result2             = Lcda_Is_Lane_Change_Detected(&lcda_core_input, &cvw_object, (&lcda_cals));

   cvw_object.ego_side                           = FBK_SIDE_LEFT;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_FALSE;
   result3                                       = Lcda_Is_Lane_Change_Detected(&lcda_core_input, &cvw_object, (&lcda_cals));

   cvw_object.ego_side                          = FBK_SIDE_RIGHT;
   lcda_core_input.f_lane_change[FBK_SIDE_LEFT] = false;
   result4                                      = Lcda_Is_Lane_Change_Detected(&lcda_core_input, &cvw_object, (&lcda_cals));

   cvw_object.ego_side                           = FBK_SIDE_LEFT;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_TRUE;
   lcda_core_input.lane_width                    = 3.0f;
   tracker_object.curvi_pos.y                    = 1.0f;
   result5                                       = Lcda_Is_Lane_Change_Detected(&lcda_core_input, &cvw_object, (&lcda_cals));

   cvw_object.ego_side                           = FBK_SIDE_LEFT;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_TRUE;
   lcda_core_input.lane_width                    = 1.0f;
   tracker_object.curvi_pos.y                    = 0.75f;
   result6                                       = Lcda_Is_Lane_Change_Detected(&lcda_core_input, &cvw_object, (&lcda_cals));

   tracker_object.curvi_pos.y = -0.75f;
   result7                    = Lcda_Is_Lane_Change_Detected(&lcda_core_input, &cvw_object, (&lcda_cals));

   /** \assert . */
   EXPECT_TRUE(result1);
   EXPECT_TRUE(result2);
   EXPECT_FALSE(result3);
   EXPECT_FALSE(result4);
   EXPECT_FALSE(result5);
   EXPECT_FALSE(result6);
   EXPECT_FALSE(result7);
}

/**
 * Create CVW object that is within ego lane and such lateral relative velocity that object will leave ego lane and enter cvw zone.
 * Expect ego lane check to not delay the alert - obj_in_ego_lane = FALSE \uts{CSCSA-109115} \sdd{CSCSA-109113}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_In_Ego_Lane__object_in_ego_lane_rel_vel_above_thresh)
{
   /** \arrange Set up target object. Activate ego lane check with rel_vel logic. */
   boolean_T result;
   Cvw_Object_T cvw_obj;
   const uint8_t obj_id = 20;

   lcda_cals.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check = FBK_TRUE;
   (&lcda_cals)->k_lcda_ego_lane_check_center_point_only     = FBK_TRUE;

   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.curvi_pos.y     = 1.1f;
   tracker_object.curvi_pos.x     = -16.0f;
   tracker_object.curvi_vel_rel.y = 2.0;

   cvw_obj.p_tracker_data = &tracker_object;
   cvw_obj.ego_side       = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);

   /** \action Call Lcda_Is_Cvw_Object_In_Ego_Lane to compute the ego lane occupance. */
   result = Lcda_Is_Cvw_Object_In_Ego_Lane(&lcda_core_input, (&lcda_cals), &cvw_obj, LCDA_USE_CURVI);

   /** \assert Check that the ego lane check occupance return false. */
   EXPECT_EQ(result, FBK_FALSE);
}

/**
 * Create CVW object that is within ego lane and such lateral relative velocity that object moves towards ego center. Expect ego
 * lane check to suppress alert - obj_in_ego_lane = TRUE \uts{CSCSA-109116} \sdd{CSCSA-109113} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Cvw_Object_In_Ego_Lane__object_in_ego_lane_rel_vel_below_thresh)
{
   /** \arrange Set up target object. Activate ego lane check with rel_vel logic. */
   boolean_T result;
   Cvw_Object_T cvw_obj;
   const uint8_t obj_id = 20;

   lcda_cals.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check = FBK_TRUE;
   (&lcda_cals)->k_lcda_ego_lane_check_center_point_only     = FBK_TRUE;

   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.curvi_pos.y     = 1.1f;
   tracker_object.curvi_pos.x     = -16.0f;
   tracker_object.curvi_vel_rel.y = -2.0;

   cvw_obj.p_tracker_data = &tracker_object;
   cvw_obj.ego_side       = Fbk_Get_Obj_Side(tracker_object.curvi_pos.y);

   /** \action Call Lcda_Is_Cvw_Object_In_Ego_Lane to compute the ego lane occupance. */
   result = Lcda_Is_Cvw_Object_In_Ego_Lane(&lcda_core_input, (&lcda_cals), &cvw_obj, LCDA_USE_CURVI);

   /** \assert Check that the ego lane check occupance return true. */
   EXPECT_EQ(result, FBK_TRUE);
}

/**
 * Check if calculation of dynamic CVW TTC threshold is correct)
 * \uts{CSCSA-200508} \sdd{CSCSA-196335} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Calculate_Dyn_Cvw_Ttc_Treshold__calculating_dyn_ttc_is_enabled)
{
   /** \arrange Set up target object. Activate ego lane check with rel_vel logic. */
   float32_T result;
   float32_T obj_long_vel_rel;
   float32_T cvw_ttc_threshold;
   float32_T dyn_ttc_speed_factor;

   obj_long_vel_rel                                         = FBK_ONE_F;
   cvw_ttc_threshold                                        = lcda_cals.k_lcda_cvw_ttc_const[FBK_ZERO_UINT];
   dyn_ttc_speed_factor                                     = lcda_cals.k_lcda_cvw_ttc_accel[FBK_ZERO_UINT];
   lcda_cals.k_lcda_f_enable_dyn_cvw_ttc_threshold          = FBK_TRUE;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ZERO_UINT] = 0.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ONE_UINT]  = 0.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_speed_parameter             = 4.0f;

   /** \action Call Lcda_Is_Cvw_Object_In_Ego_Lane to compute the ego lane occupance. */
   result = Lcda_Calculate_Dyn_Cvw_Ttc_Treshold(obj_long_vel_rel, cvw_ttc_threshold, dyn_ttc_speed_factor, &lcda_cals);

   /** \assert Check that the ego lane check occupance return true. */
   EXPECT_FLOAT_EQ(result, 10.062f);
}

/**
 * Check if calculation of dynamic CVW TTC threshold is correct)
 * \uts{CSCSA-200509} \sdd{CSCSA-196335} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Calculate_Dyn_Cvw_Ttc_Treshold__calculating_dyn_ttc_is_disabled)
{
   /** \arrange Set up target object. Activate ego lane check with rel_vel logic. */
   float32_T result;
   float32_T obj_long_vel_rel;
   float32_T cvw_ttc_threshold;
   float32_T dyn_ttc_speed_factor;

   obj_long_vel_rel                                         = FBK_ONE_F;
   cvw_ttc_threshold                                        = lcda_cals.k_lcda_cvw_ttc_const[FBK_ZERO_UINT];
   dyn_ttc_speed_factor                                     = lcda_cals.k_lcda_cvw_ttc_accel[FBK_ZERO_UINT];
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ZERO_UINT] = 0.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ONE_UINT]  = 0.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_speed_parameter             = 4.0f;
   lcda_cals.k_lcda_f_enable_dyn_cvw_ttc_threshold          = FBK_FALSE;


   /** \action Call Lcda_Is_Cvw_Object_In_Ego_Lane to compute the ego lane occupance. */
   result = Lcda_Calculate_Dyn_Cvw_Ttc_Treshold(obj_long_vel_rel, cvw_ttc_threshold, dyn_ttc_speed_factor, &lcda_cals);

   /** \assert Check that the ego lane check occupance return true. */
   EXPECT_FLOAT_EQ(result, cvw_ttc_threshold);
}


/**
 * Check if calculation of TTC threshold with comepnsation is correct)
 * \uts{CSCSA-274777} \sdd{CSCSA-196335} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Calculate_Dyn_Cvw_Ttc_Treshold__ttc_compensation_below_thresh)
{
   /** \arrange Set up target object. Activate ego lane check with rel_vel logic. */
   float32_T result;
   float32_T obj_long_vel_rel;
   float32_T cvw_ttc_threshold;
   float32_T dyn_ttc_speed_factor;

   obj_long_vel_rel                                         = FBK_ONE_F;
   cvw_ttc_threshold                                        = lcda_cals.k_lcda_cvw_ttc_const[FBK_ZERO_UINT];
   dyn_ttc_speed_factor                                     = lcda_cals.k_lcda_cvw_ttc_accel[FBK_ZERO_UINT];
   lcda_cals.k_lcda_f_enable_dyn_cvw_ttc_threshold          = FBK_FALSE;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh      = 2.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ZERO_UINT] = 1.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ONE_UINT]  = 2.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_speed_parameter             = 4.0f;

   /** \action Call Lcda_Is_Cvw_Object_In_Ego_Lane to compute the ego lane occupance. */
   result = Lcda_Calculate_Dyn_Cvw_Ttc_Treshold(obj_long_vel_rel, cvw_ttc_threshold, dyn_ttc_speed_factor, &lcda_cals);

   /** \assert Check that the ego lane check occupance return true. */
   EXPECT_FLOAT_EQ(result, cvw_ttc_threshold + lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ZERO_UINT]);
}


/**
 * Check if calculation of TTC threshold with comepnsation is correct)
 * \uts{CSCSA-274778} \sdd{CSCSA-196335} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Calculate_Dyn_Cvw_Ttc_Treshold__ttc_compensation_above_thresh)
{
   /** \arrange Set up target object. Activate ego lane check with rel_vel logic. */
   float32_T result;
   float32_T obj_long_vel_rel;
   float32_T cvw_ttc_threshold;
   float32_T dyn_ttc_speed_factor;

   obj_long_vel_rel                                         = FBK_ONE_F;
   cvw_ttc_threshold                                        = lcda_cals.k_lcda_cvw_ttc_const[FBK_ZERO_UINT];
   dyn_ttc_speed_factor                                     = lcda_cals.k_lcda_cvw_ttc_accel[FBK_ZERO_UINT];
   lcda_cals.k_lcda_f_enable_dyn_cvw_ttc_threshold          = FBK_FALSE;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh      = 0.1f;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ZERO_UINT] = 1.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ONE_UINT]  = 2.0f;
   lcda_cals.k_lcda_dyn_cvw_ttc_speed_parameter             = 4.0f;

   /** \action Call Lcda_Is_Cvw_Object_In_Ego_Lane to compute the ego lane occupance. */
   result = Lcda_Calculate_Dyn_Cvw_Ttc_Treshold(obj_long_vel_rel, cvw_ttc_threshold, dyn_ttc_speed_factor, &lcda_cals);

   /** \assert Check that the ego lane check occupance return true. */
   EXPECT_FLOAT_EQ(result, cvw_ttc_threshold + lcda_cals.k_lcda_dyn_cvw_ttc_compens_time[FBK_ONE_UINT]);
}


/**
 * Create CVW warn relevant target object which is fully qualified. Set the hosts turn signal to none. Process the CVW warning
 * state and check that the alert level is LCDA_ALERT_STATE_LEVEL_1. Additionally activate the ego lane check and the CVW lane
 * change intention zone. Both should not affect the warning. \uts{CSCSA-211402} \sdd{SF-6697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__cvw_alert_level_1_when_obj_is_in_cvw_zone_and_ego_additional_checks_case2)
{
   /** \arrange Set up target object and host vehicle state. Activate ego lane check and CVW lane change intention zone. */
   const uint8_t obj_id                                         = 20;
   const float32_T obj_lon_pos                                  = -16.0f;
   const float32_T obj_lat_pos                                  = 2.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_core_input.warn_settings.cvw_rel_vel_range.min = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
   lcda_core_input.warn_settings.cvw_rel_vel_range.max = lcda_cals.k_cvw_max_object_curvi_relative_speed;
   lcda_cals.k_cvw_zone_calculation_mode               = CVW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_cvw_min_mature_cycles                   = 10; // so that CVW alert is
                                                             // given when the object
                                                             // is in the zone even
                                                             // for 1 cycle
   lcda_cals.k_cvw_candidate_ttc                             = 1.0f;
   cvw_persistent.mature_count_in_cvw_zone[obj_id]           = 0u;
   cvw_persistent.f_prev_cvw_active[FBK_SIDE_RIGHT]          = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check           = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check = FBK_FALSE;
   (&lcda_cals)->k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.vcs_pos.y   = obj_lat_pos;
   tracker_object.vcs_pos.x   = obj_lon_pos;
   tracker_object.curvi_pos.y = obj_lat_pos;
   tracker_object.curvi_pos.x = obj_lon_pos;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_RIGHT], obj_id);
   EXPECT_EQ(cvw_core_output.cvw_id[FBK_SIDE_RIGHT], tracker_object.id);
}


/**
 * Create object which relative velocity is above threshold so that a object is irrelevant.
 * \uts{CSCSA-211403} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__no_cvw_alert_curvi_rel_vel)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status mature. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                           = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading                    = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.existence_probability            = 1.0f;
   lcda_cals.k_cvw_max_object_curvi_relative_speed = 1.0f;
   tracker_object.curvi_vel_rel.x                  = lcda_cals.k_cvw_max_object_curvi_relative_speed + EPSILON;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as irrelevant. */
   EXPECT_FALSE(result);
}

/**
 * Create object which relative velocity is below threshold so that a object is irrelevant.
 * \uts{CSCSA-247842} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__no_cvw_alert_curvi_rel_vel_below)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status mature. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                           = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading                    = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.existence_probability            = 1.0f;
   lcda_cals.k_cvw_max_object_curvi_relative_speed = 1000.0f;
   tracker_object.curvi_vel.x                      = 12.0f;
   tracker_object.curvi_vel_rel.x                  = lcda_cals.k_cvw_min_object_curvi_relative_speed[0] - EPSILON;


   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as irrelevant. */
   EXPECT_FALSE(result);
}


/**
 * Create object which relative velocity is in range so that a object is relevant.
 * \uts{CSCSA-247843} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__cvw_alert_curvi_rel_vel_in_range)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status mature. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                               = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading                        = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.existence_probability                = 1.0f;
   lcda_cals.k_cvw_max_object_curvi_relative_speed     = 10.0f;
   tracker_object.curvi_vel.x                          = 12.0f;
   tracker_object.curvi_vel_rel.x                      = lcda_cals.k_cvw_min_object_curvi_relative_speed[0] + EPSILON;
   lcda_core_input.warn_settings.cvw_rel_vel_range.min = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
   lcda_core_input.warn_settings.cvw_rel_vel_range.max = lcda_cals.k_cvw_max_object_curvi_relative_speed;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as irrelevant. */
   EXPECT_TRUE(result);
}


/**
 * Create object which is not in zone so that a cvw alert is not triggered and check that the alert level is LCDA_ALERT_STATE_NONE
 * and ttc is set to default no alert value. \uts{CSCSA-211404} \sdd{SF-6697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Process_Cvw_Object__no_cvw_alert_object_outside_zone)
{
   /** \arrange Set up target object and host vehicle state. Activate ego lane check and CVW lane change intention zone. */
   const uint8_t obj_id                                         = 20;
   const float32_T obj_lon_pos                                  = -16.0f;
   const float32_T obj_lat_pos                                  = 20.0f;
   float32_T cvw_ttc_threshold[FBK_NUMBER_OF_SIDES]             = {5.0f, 5.0f};
   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_cals.k_cvw_zone_calculation_mode = CVW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_cvw_min_mature_cycles     = 10; // so that CVW alert is
                                               // given when the object
                                               // is in the zone even
                                               // for 1 cycle

   cvw_persistent.mature_count_in_cvw_zone[obj_id]           = 0u;
   cvw_persistent.f_prev_cvw_active[FBK_SIDE_RIGHT]          = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check           = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check = FBK_FALSE;
   lcda_cals.k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   // Track on right of the ego, inside the default CVW zone - long dist = -16m and lat dist = 2m
   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, obj_id);
   tracker_object.vcs_pos.y   = obj_lat_pos;
   tracker_object.vcs_pos.x   = obj_lon_pos;
   tracker_object.curvi_pos.y = obj_lat_pos;
   tracker_object.curvi_pos.x = obj_lon_pos;

   /** \action Call Lcda_Process_Cvw_Object to compute the CVW alert state. */
   Lcda_Process_Cvw_Object(&cvw_core_output, cvw_ttc_threshold, f_use_small_lc_intention_zone, &tracker_object, &lcda_core_input,
                           &lcda_cals, &lcda_persistent, &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_NONE. */
   EXPECT_EQ(cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT], LCDA_CVW_DEFAULT_NO_ALERT_TTC);
}


/**
 * Check that object is classified as not relevant if previous alert not active.
 * \uts{CSCSA-211405} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__returns_false_previous_alert_not_active)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed except f_previous_alert_active */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_NEW;
   tracker_object.curvi_heading         = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.curvi_vel.x           = lcda_cals.k_cvw_min_obj_curvi_long_vel + EPSILON;
   tracker_object.existence_probability = 1.0f;

   lcda_cals.k_lcda_allow_track_status_new              = FBK_TRUE;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = FBK_FALSE;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_FALSE(result);
}


/**
 * Check that object is classified as not relevant if object status invalid.
 * \uts{CSCSA-211406} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__returns_false_status_invalid)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed except object status */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_INVALID;
   tracker_object.curvi_heading         = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.curvi_vel.x           = lcda_cals.k_cvw_min_obj_curvi_long_vel + EPSILON;
   tracker_object.existence_probability = 1.0f;

   lcda_cals.k_lcda_allow_track_status_new              = FBK_TRUE;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_FALSE(result);
}

/**
 * Check that object is classified as not relevant if new objects not allowed.
 * \uts{CSCSA-211407} \sdd{SF-6682} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Is_Object_Relevant_For_Cvw__returns_false_new_objects_not_allowed)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed except k_lcda_allow_track_status_new */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_NEW;
   tracker_object.curvi_heading         = lcda_cals.k_cvw_max_curvi_heading_abs - EPSILON;
   tracker_object.curvi_vel.x           = lcda_cals.k_cvw_min_obj_curvi_long_vel + EPSILON;
   tracker_object.existence_probability = 1.0f;

   lcda_cals.k_lcda_allow_track_status_new              = FBK_FALSE;
   cvw_persistent.prev_cvw_alert_obj_id[FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Call function Lcda_Is_Object_Relevant_For_Cvw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Cvw(&lcda_core_input, &tracker_object, &lcda_cals, &cvw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_FALSE(result);
}


/**
 * Check that small lane chane intention zone is used if guardrail is present and calibration values are set accordingly.
 * \uts{CSCSA-211408} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test,
       Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_true_if_no_guardrail_present_and_use_small_zone_disabled)
{
   /** \arrange Set up core input, persistent data and calibration values, such that no guardrail is present and small zone should
    * be used in this case. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.status = LCDA_GUARDRAIL_VALID;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_TRUE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_FALSE;
   lcda_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone  = -1.0f;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that small zone is used. */
   EXPECT_TRUE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}


/**
 * Check that normal lane chane intention zone is used if guardrail status undefined.
 * \uts{CSCSA-211409} \sdd{SF-6822} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Cvw_Test, Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used__returns_false_guardrail_status_undefined)
{
   /** \arrange Set up core input, persistent data and calibration values, such that no guardrail is present and normal zone should
    * be used in this case. */
   boolean_T result;
   uint8_t side = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.status = (Lcda_Guardrail_Status_T) 0x02;

   cvw_persistent.lc_intention_zone_change_counter[side]    = 2u;
   cvw_persistent.f_prev_used_small_lc_intention_zone[side] = FBK_FALSE;

   lcda_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold         = 4u;
   lcda_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present = FBK_FALSE;

   /** \action Call Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used to determine which lane change intention zone should be
    * used. */
   result = Lcda_Should_Small_Lane_Change_Intention_Zone_Be_Used(&cvw_persistent, side, &lcda_core_input, &lcda_cals);

   /** \assert Verify that normal zone is used. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cvw_persistent.lc_intention_zone_change_counter[side], FBK_ZERO_UINT);
}