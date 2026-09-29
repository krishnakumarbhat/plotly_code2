/**
 * @file lcda_process_bsw_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_process_bsw.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42632}
 */

#include "lcda_process_bsw_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <string.h>


extern "C"
{
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "lcda_process_bsw.c"
#include "lcda_test_helpers.h"
#include "lcda_types.h"
#include "ml_math.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Set up BSW persistent data such that it does not equal its default values. Reset the BSW core output and check that the BSW
 * persistent data and output is reset for BSW. \uts{CSCSA-45582} \sdd{SF-6668} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Reset_Bsw_Core__core_Bsw_output_is_reset)
{
   /** \arrange Set up BSW persistent data. */
   memset(&bsw_core_output, 0xaa, sizeof(bsw_core_output));
   lcda_persistent.f_bsw_prev_reset = FBK_FALSE;

   /** \action Call Lcda_Reset_Bsw_Core to reset BSW core. */
   Lcda_Reset_Bsw_Core(&bsw_core_output, &bsw_persistent, &lcda_persistent, &lcda_core_input);

   /** \assert Check if BSW output and persistent data was reset. */
   EXPECT_TRUE(Lcda_Is_Bsw_Output_Default(&bsw_core_output));
   EXPECT_TRUE(lcda_persistent.f_bsw_prev_reset);
}

/**
 * Check that the function that shall reset the BSW persistent data actually resets all persistent variables to default values.
 * \uts{CSCSA-45583} \sdd{SF-6655} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Reset_Bsw_Persistent_Data__works_properly)
{
   /** \arrange Set up BSW persistent data with non-default values. */
   uint8_t obj_id = 8u;
   memset(&bsw_persistent, 0xaa, sizeof(bsw_persistent));

   /** \action Call Lcda_Reset_Bsw_Persistent_Data to reset BSW persistent data. */
   Lcda_Reset_Bsw_Persistent_Data(&bsw_persistent);

   /** \assert Check if BSW persistent data is reseted to default values. */
   EXPECT_EQ(bsw_persistent.f_prev_bsw_active[FBK_SIDE_LEFT], FBK_FALSE);
   EXPECT_EQ(bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_persistent.f_prev_bsw_active[FBK_SIDE_RIGHT], FBK_FALSE);
   EXPECT_EQ(bsw_persistent.bsw_hold_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_persistent.fallback_state[obj_id], FALLBACK_FAST);
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[obj_id], FBK_ZERO_UINT);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[obj_id], FBK_ZERO_UINT);
}

/**
 * Check that the function that shall reset the BSW object persistent data actually resets all object persistent variables to
 * default values. \uts{CSCSA-45584} \sdd{SF-6656} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Reset_Bsw_Persistent_Object_Data__works_properly)
{
   /** \arrange Set up BSW object persistent data with non-default values. */
   uint8_t obj_id                                        = 5u;
   bsw_persistent.fallback_state[obj_id]                 = FALLBACK_SLOW;
   bsw_persistent.mature_count_in_bsw_zone[obj_id]       = 4u;
   bsw_persistent.fallback_fast_to_slow_qual_ctr[obj_id] = 3u;

   /** \action Call Lcda_Reset_Bsw_Persistent_Data to reset BSW object persistent data. */
   Lcda_Reset_Bsw_Persistent_Object_Data(&bsw_persistent, obj_id);

   /** \assert Check if BSW object persistent data is reseted to default values. */
   EXPECT_EQ(bsw_persistent.fallback_state[obj_id], FALLBACK_FAST);
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[obj_id], FBK_ZERO_UINT);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[obj_id], FBK_ZERO_UINT);
   EXPECT_EQ(bsw_persistent.f_prev_long_truck_status[obj_id], FBK_FALSE);
}

/**
 * Check that the function that fills the persistent data from the core output and verify that the core output information is
 * stored correctly in the persistent data. \uts{CSCSA-45585} \sdd{SF-6648} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Fill_Side_Persistent_Bsw_Data__works_properly)
{
   /** \arrange Set up core output with alerts for both sides. */
   bsw_core_output.bsw_alert[FBK_SIDE_LEFT]     = LCDA_ALERT_STATE_LEVEL_1;
   bsw_core_output.bsw_index[FBK_SIDE_LEFT]     = 7u;
   bsw_core_output.bsw_id[FBK_SIDE_LEFT]        = 33u;
   bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT] = 34u;

   bsw_core_output.bsw_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_2;
   bsw_core_output.bsw_index[FBK_SIDE_RIGHT]     = 4u;
   bsw_core_output.bsw_id[FBK_SIDE_RIGHT]        = 11u;
   bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT] = 12u;

   /** \action Call Lcda_Fill_Side_Persistent_Bsw_Data to fill BSW persistent data from core output. */
   Lcda_Fill_Side_Persistent_Bsw_Data(&bsw_persistent, &bsw_core_output);

   /** \assert Check if BSW persistent data is filled correctly with respect to given core output. */
   EXPECT_EQ(bsw_persistent.f_prev_bsw_active[FBK_SIDE_LEFT], FBK_TRUE);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT], bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_unique_id[FBK_SIDE_LEFT], bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(bsw_persistent.f_prev_bsw_active[FBK_SIDE_RIGHT], FBK_TRUE);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_RIGHT], bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_unique_id[FBK_SIDE_RIGHT], bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT]);
}

/**
 * Check that core output for left side is reseted properly by corresponding function.
 * \uts{CSCSA-45586} \sdd{SF-6646} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Clear_Bsw_Core_Output_On_Side__works_properly_for_left_side)
{
   /** \arrange Set up non-default core output on left side. */
   uint8_t side                        = FBK_SIDE_LEFT;
   bsw_core_output.bsw_alert[side]     = LCDA_ALERT_STATE_LEVEL_1;
   bsw_core_output.bsw_index[side]     = 7u;
   bsw_core_output.bsw_id[side]        = 33u;
   bsw_core_output.bsw_unique_id[side] = 33u;

   /** \action Call Lcda_Clear_Bsw_Core_Output_On_Side to reset core output for left side. */
   Lcda_Clear_Bsw_Core_Output_On_Side(&bsw_core_output, &lcda_core_input, side);

   /** \assert Verify that core output for left side is reseted properly. */
   EXPECT_EQ(bsw_core_output.bsw_alert[side], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(bsw_core_output.bsw_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_core_output.bsw_index[side], PA_INVALID_OBJ_INDEX);
}

/**
 * Check that core output for right side is reseted properly by corresponding function.
 * \uts{CSCSA-45587} \sdd{SF-6646} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Clear_Bsw_Core_Output_On_Side__works_properly_for_right_side)
{
   /** \arrange Set up non-default core output on right side. */
   uint8_t side                        = FBK_SIDE_RIGHT;
   bsw_core_output.bsw_alert[side]     = LCDA_ALERT_STATE_LEVEL_2;
   bsw_core_output.bsw_index[side]     = 4u;
   bsw_core_output.bsw_id[side]        = 11u;
   bsw_core_output.bsw_unique_id[side] = 11u;

   /** \action Call Lcda_Clear_Bsw_Core_Output_On_Side to reset core output for right side. */
   Lcda_Clear_Bsw_Core_Output_On_Side(&bsw_core_output, &lcda_core_input, side);

   /** \assert Verify that core output for right side is reseted properly. */
   EXPECT_EQ(bsw_core_output.bsw_alert[side], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(bsw_core_output.bsw_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[side], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_core_output.bsw_index[side], PA_INVALID_OBJ_INDEX);
}

/**
 * Check that initialization of BSW object works properly and sets tracker object pointer.
 * \uts{CSCSA-45588} \sdd{SF-6649} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Init_Bsw_Object_Data__works_properly)
{
   /** \arrange Set up BSW object with non-default values and create tracker object. */
   bsw_object.ego_side       = FBK_SIDE_LEFT;
   bsw_object.f_obj_in_zone  = FBK_TRUE;
   bsw_object.zone.size      = 3u;
   bsw_object.p_tracker_data = NULL;

   /** \action Call Lcda_Init_Bsw_Object_Data to initialize BSW object. */
   Lcda_Init_Bsw_Object_Data(&bsw_object, &tracker_object);

   /** \assert Verify that pointer to tracker object is set and attributes are on default values. */
   EXPECT_EQ(bsw_object.ego_side, FBK_SIDE_UNDEFINED);
   EXPECT_FALSE(bsw_object.f_obj_in_zone);
   EXPECT_EQ(bsw_object.zone.size, FBK_ZERO_UINT);
   EXPECT_EQ(bsw_object.p_tracker_data, &tracker_object);
}

/**
 * Set active BSW alert for target object on right side and mature count greater than zero. Create BSW zone and check that it
 * equals the default BSW hysteresis zone size. \uts{CSCSA-45589} \sdd{SF-6647} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test,
       Lcda_Create_Bsw_Object_Zone__zone_is_bsw_zone_hys_when_prev_bsw_active_is_true_and_mature_count_in_zone_greater_than_zero)
{
   /** \arrange Set up target object, BSW alert, and mature counter for BSW zone. */
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 4;
   uint8_t point;

   tracker_object.id   = obj_id;
   bsw_object.ego_side = side;

   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 1u;
   bsw_persistent.f_prev_bsw_active[side]          = FBK_TRUE;

   lcda_core_input.warn_settings.bsw_len_factor     = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone             = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone  = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode        = BSW_ZONE_CALC_FIXED_INPUT;

   /** \action Call Lcda_Create_Bsw_Object_Zone to create bsw zone. */
   Lcda_Create_Bsw_Object_Zone(&bsw_object, &bsw_persistent, &lcda_core_input, &lcda_cals, &cvw_persistent);

   /** \assert Check that bsw zone and the default bsw hysteresis zone have same size. */
   for (point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(bsw_object.zone.points[point].x, default_bsw_zone_hys.points[point].x);
      ASSERT_EQ(bsw_object.zone.points[point].y, default_bsw_zone_hys.points[point].y);
   }
}

/**
 * Set inactive BSW alert but active CVW alert for target object on right side. Create BSW zone and check that it equals the
 * default BSW hysteresis zone size. \uts{CSCSA-45590} \sdd{SF-6647} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Create_Bsw_Object_Zone__zone_is_bsw_zone_hys_when_prev_cvw_active_on_obj_is_true)
{
   /** \arrange Set up target object, BSW and CVW alert state. */
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 4;
   uint8_t point;

   tracker_object.id   = obj_id;
   bsw_object.ego_side = side;

   lcda_cals.k_bsw_uses_cvw_alert_state_enabled    = 1;
   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 0u;
   bsw_persistent.f_prev_bsw_active[side]          = FBK_FALSE;

   lcda_core_input.warn_settings.bsw_len_factor     = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone             = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone  = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode        = BSW_ZONE_CALC_FIXED_INPUT;

   Lcda_Create_Cvw_Alert(side, obj_id, &cvw_persistent);
   /** \action Call Lcda_Create_Bsw_Object_Zone to create bsw zone. */
   Lcda_Create_Bsw_Object_Zone(&bsw_object, &bsw_persistent, &lcda_core_input, &lcda_cals, &cvw_persistent);

   /** \assert Check that bsw zone and the default bsw hysteresis zone have same size. */
   for (point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(bsw_object.zone.points[point].x, default_bsw_zone_hys.points[point].x);
      ASSERT_EQ(bsw_object.zone.points[point].y, default_bsw_zone_hys.points[point].y);
   }
}

/**
 * Set inactive BSW alert but active CVW alert for target object on right side. Deactivate cal value
 * k_bsw_uses_cvw_alert_state_enabled to avoid adjusting the bsw zone hysteresis. Create BSW zone and check that it equals the
 * default BSW zone size. \uts{CSCSA-45591} \sdd{SF-6647} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test,
       Lcda_Create_Bsw_Object_Zone__previous_cvw_alert_does_not_affect_bsw_zone_when_k_bsw_uses_cvw_alert_state_enabled_is_false)
{
   /** \arrange Set up target object, BSW and CVW alert state. */
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 4;
   uint8_t point;

   tracker_object.id   = obj_id;
   bsw_object.ego_side = side;

   lcda_cals.k_bsw_uses_cvw_alert_state_enabled    = 0;
   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 0u;
   bsw_persistent.f_prev_bsw_active[side]          = FBK_FALSE;

   lcda_core_input.warn_settings.bsw_len_factor     = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone             = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone  = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode        = BSW_ZONE_CALC_FIXED_INPUT;

   // Create a CVW alert on the same object
   Lcda_Create_Cvw_Alert(side, obj_id, &cvw_persistent);

   /** \action Call Lcda_Create_Bsw_Object_Zone to create bsw zone. */
   Lcda_Create_Bsw_Object_Zone(&bsw_object, &bsw_persistent, &lcda_core_input, &lcda_cals, &cvw_persistent);

   /** \assert Check that bsw zone and the default bsw zone have same size. */
   for (point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(bsw_object.zone.points[point].x, default_bsw_zone.points[point].x);
      ASSERT_EQ(bsw_object.zone.points[point].y, default_bsw_zone.points[point].y);
   }
}

/**
 * Set BSW alert for target object on right side with mature BSW zone counter greater than zero. Set that no previous BSW warning
 * was active. Create BSW zone and check that it equals the default BSW zone size. \uts{CSCSA-45592} \sdd{SF-6647}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Create_Bsw_Object_Zone__zone_is_bsw_zone_when_prev_bsw_active_is_false)
{
   /** \arrange Set up target object and BSW alert state. */
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 4;
   uint8_t point;

   tracker_object.id   = obj_id;
   bsw_object.ego_side = side;

   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 1u;
   bsw_persistent.f_prev_bsw_active[side]          = FBK_FALSE;

   lcda_core_input.warn_settings.bsw_len_factor     = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone             = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone  = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode        = BSW_ZONE_CALC_FIXED_INPUT;

   /** \action Call Lcda_Create_Bsw_Object_Zone to create bsw zone. */
   Lcda_Create_Bsw_Object_Zone(&bsw_object, &bsw_persistent, &lcda_core_input, &lcda_cals, &cvw_persistent);

   /** \assert Check that bsw zone and the default bsw zone have same size. */
   for (point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(bsw_object.zone.points[point].x, default_bsw_zone.points[point].x);
      ASSERT_EQ(bsw_object.zone.points[point].y, default_bsw_zone.points[point].y);
   }
}

/**
 * Set BSW alert for target object on right side with mature BSW zone equal zero. Set that no previous BSW warning was active.
 * Create BSW zone and check that it equals the default BSW zone size. \uts{CSCSA-45593} \sdd{SF-6647}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Create_Bsw_Object_Zone__zone_is_bsw_zone_when_mature_count_in_zone_is_zero)
{
   /** \arrange Set up target object and BSW alert state. */
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 4;
   uint8_t point;

   tracker_object.id   = obj_id;
   bsw_object.ego_side = side;

   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 0u;
   bsw_persistent.f_prev_bsw_active[side]          = FBK_TRUE;

   lcda_core_input.warn_settings.bsw_len_factor     = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone             = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone  = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode        = BSW_ZONE_CALC_FIXED_INPUT;

   /** \action Call Lcda_Create_Bsw_Object_Zone to create bsw zone. */
   Lcda_Create_Bsw_Object_Zone(&bsw_object, &bsw_persistent, &lcda_core_input, &lcda_cals, &cvw_persistent);

   /** \assert Check that bsw zone and the default bsw zone have same size. */
   for (point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(bsw_object.zone.points[point].x, default_bsw_zone.points[point].x);
      ASSERT_EQ(bsw_object.zone.points[point].y, default_bsw_zone.points[point].y);
   }
}

/**
 * Create BSW warn relevant target object which is fully qualified. Set the hosts turn signal to none. Process the BSW warning
 * state and check that the alert level is LEVEL1. \uts{CSCSA-45594} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__bsw_alert_level1_when_obj_is_in_bsw_zone_and_ego_turn_signal_none)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t id_track_inside_bsw_zone = 20;
   const float32_T track_long_pos         = -8.0f;
   const float32_T track_lat_pos          = 2.0f;

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles         = 1;                // so that BSW alert is
                                                                 // given when the object
                                                                 // is in the zone even for
                                                                 // 1 cycle
   lcda_cals.k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation

   lcda_cals.k_bsw_overlap_area_check_enable                      = FBK_TRUE;
   lcda_cals.k_bsw_overlap_area_threshold                         = 0.0f;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;


   // Track on right of the ego, inside the default BSW zone - long dist = -8m and lat dist = 2m
   Lcda_Create_Bsw_Track(&tracker_object, id_track_inside_bsw_zone, track_long_pos, track_lat_pos);

   // Set qualification counter
   bsw_persistent.fallback_fast_to_slow_qual_ctr[id_track_inside_bsw_zone] = lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres + 1u;

   /** \action Call Lcda_Process_Bsw_Object to compute the BSW alert state. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(bsw_core_output.bsw_index[FBK_SIDE_RIGHT], tracker_object.index);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
}

/**
 * Create BSW warn relevant target object which is fully qualified. Set the hosts turn signal to none. Process the BSW warning
 * state and check that the alert level is LEVEL1. \uts{CSCSA-45595} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__bsw_alert_level1_when_obj_is_in_bsw_zone_and_ego_turn_signal_none_curvi_coord)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t id_track_inside_bsw_zone = 20;
   const float32_T track_long_pos         = -8.0f;
   const float32_T track_lat_pos          = 2.0f;

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles         = 1;                // so that BSW alert is
                                                                 // given when the object
                                                                 // is in the zone even for
                                                                 // 1 cycle
   lcda_cals.k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation
   lcda_cals.k_bsw_use_curvi_coordinates             = FBK_TRUE;

   lcda_cals.k_bsw_overlap_area_check_enable                      = FBK_TRUE;
   lcda_cals.k_bsw_overlap_area_threshold                         = 0.0f;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;


   // Track on right of the ego, inside the default BSW zone - long dist = -8m and lat dist = 2m
   Lcda_Create_Bsw_Track(&tracker_object, id_track_inside_bsw_zone, track_long_pos, track_lat_pos);

   // Set qualification counter
   bsw_persistent.fallback_fast_to_slow_qual_ctr[id_track_inside_bsw_zone] = lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres + 1u;

   /** \action Call Lcda_Process_Bsw_Object to compute the BSW alert state. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_index[FBK_SIDE_RIGHT], tracker_object.index);
}

/**
 * Create BSW warn relevant target object which is fully qualified. Set the hosts turn signal to right. Process the BSW warning
 * state and check that the alert level is LEVEL2. \uts{CSCSA-45596} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__bsw_alert_level2_when_obj_is_in_bsw_zone_and_ego_turn_signal_right)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t id_track_inside_bsw_zone = 22;
   const float32_T track_long_pos         = -8.0f;
   const float32_T track_lat_pos          = 2.0f;

   lcda_persistent.turn_signal_held = TURN_SIGNAL_RIGHT;

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles         = 1;                // so that BSW alert is
                                                                 // given when the object
                                                                 // is in the zone even for
                                                                 // 1 cycle
   lcda_cals.k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation

   lcda_cals.k_bsw_overlap_area_check_enable                      = FBK_TRUE;
   lcda_cals.k_bsw_overlap_area_threshold                         = 0.0f;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;

   // Track on right of the ego, inside the default BSW zone - long dist = -8m and lat dist = 2m
   Lcda_Create_Bsw_Track(&tracker_object, id_track_inside_bsw_zone, track_long_pos, track_lat_pos);

   // Set qualification counter
   bsw_persistent.fallback_fast_to_slow_qual_ctr[id_track_inside_bsw_zone] = lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres + 1u;

   /** \action Call Lcda_Process_Bsw_Object to compute the bsw alert state. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_2. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_index[FBK_SIDE_RIGHT], tracker_object.index);
}

/**
 * Create target object which is outside of the BSW zone. Process the BSW warning state and check that the alert level is
 * LCDA_ALERT_STATE_NONE. \uts{CSCSA-45597} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__bsw_alert_state_none_when_obj_is_outside_bsw_zone)
{
   /** \arrange Set up target object. */
   const uint8_t id_track_outside_bsw_zone = 30;
   const float32_T track_long_pos          = -8.0f;
   const float32_T track_lat_pos           = 4.0f;

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles         = 1; // so that BSW alert is
                                                  // given when the object
                                                  // is in the zone even for
                                                  // 1 cycle

   // Track on right of the ego, outside the default BSW zone - long dist = -8m and lat dist = 4m
   Lcda_Create_Bsw_Track(&tracker_object, id_track_outside_bsw_zone, track_long_pos, track_lat_pos);

   /** \action Call Lcda_Process_Bsw_Object to compute the bsw alert state. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Check that the alert state level is LCDA_ALERT_STATE_NONE. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/**
 * Create a BSW warn relevant target object and set the turn signal to none. Create a concave BSW zone and check that a BSW alert
 * is raised. \uts{CSCSA-45598} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__bsw_alert_level1_when_obj_is_in_bsw_zone_and_ego_turn_signal_none_concave_zone)
{
   /** \arrange Set up target object and host state. Create concave BSW zone. */
   uint8_t id_track_inside_bsw_zone = 1;
   float32_T track_long_pos         = -9.3f;
   float32_T track_lat_pos          = 2.97f;
   float32_T curvi_heading          = -0.004f;

   // Zone is a concave polygon
   lcda_core_input.initial_bsw_zone.points[0].x = -2.7f;
   lcda_core_input.initial_bsw_zone.points[0].y = 6.2f;

   lcda_core_input.initial_bsw_zone.points[1].x = -4.7f;
   lcda_core_input.initial_bsw_zone.points[1].y = 6.2f;

   lcda_core_input.initial_bsw_zone.points[2].x = -15.0f;
   lcda_core_input.initial_bsw_zone.points[2].y = 6.2f;

   lcda_core_input.initial_bsw_zone.points[3].x = -15.0f;
   lcda_core_input.initial_bsw_zone.points[3].y = 1.6f;

   lcda_core_input.initial_bsw_zone.points[4].x = -4.7f;
   lcda_core_input.initial_bsw_zone.points[4].y = 1.6f;

   lcda_core_input.initial_bsw_zone.points[5].x = -2.8f;
   lcda_core_input.initial_bsw_zone.points[5].y = 1.0f;

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles         = 1; // so that BSW alert is
                                                  // given when the object
                                                  // is in the zone even for
                                                  // 1 cycle

   lcda_cals.k_bsw_overlap_area_check_enable                      = FBK_TRUE;
   lcda_cals.k_bsw_overlap_area_threshold                         = 0.0f;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;

   // Track on right of the ego, inside the default BSW zone at the given long and lat positions
   Lcda_Create_Bsw_Track(&tracker_object, id_track_inside_bsw_zone, track_long_pos, track_lat_pos);
   tracker_object.curvi_heading = curvi_heading;

   // Set qualification counter
   bsw_persistent.fallback_fast_to_slow_qual_ctr[id_track_inside_bsw_zone] = lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres + 1u;

   /** \action Call Lcda_Process_Bsw_Object to compute the bsw alert state. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the BSW alert state level equals LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_index[FBK_SIDE_RIGHT], tracker_object.index);
}

/**
 * Create a BSW alert state which was raised for a target object the last cycle. Obtain the last warned object id for this alert.
 * Verify that target object id and computed id are equal. \uts{CSCSA-45599} \sdd{SF-6664} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side__non_default_id_when_alert_is_active_previously_on_side)
{
   /** \arrange Set up BSW alert state and corresponding persistent data. */
   uint8_t result;
   uint8_t obj_id = 3u;

   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT]  = obj_id;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   /** \action Call Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side to obtain the last BSW alert relevant track object id. */
   result = Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side(FBK_SIDE_LEFT, &bsw_persistent);

   /** \assert Check that the computed target object id equals the target objects id. */
   EXPECT_EQ(result, obj_id);
}

/**
 * Create a BSW alert state which was inactive (no alert raised) in the last cycle. Obtain the last warned object id for this
 * alert. Verify that computed id equals the default object id. \uts{CSCSA-45600} \sdd{SF-6664} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side__default_id_when_alert_is_not_active_previously_on_side)
{
   /** \arrange Set up BSW alert state and corresponding persistent data. */
   uint8_t result;
   uint8_t obj_id = 3u;

   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT]  = obj_id;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   /** \action Call Lcda_Get_Prev_Bsw_Alert_Object_ID_On_Side to obtain the last BSW alert relevant track object id. */
   result = Lcda_Get_Prev_Bsw_Alert_Object_Id_On_Side(FBK_SIDE_RIGHT, &bsw_persistent);

   /** \assert Check that the computed target object id equals the default object id. */
   EXPECT_EQ(result, PA_INVALID_OBJ_ID);
}

/**
 * Check that an previous alert is held if the holding counter is below the threshold.
 * \uts{CSCSA-45601} \sdd{SF-6654} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Output__holds_alert_if_holding_counter_below_threshold)
{
   /** \arrange Set up BSW alert state and corresponding persistent data such that holding counter is below threshold. */
   uint8_t obj_id                 = 3u;
   uint8_t obj_index              = 8u;
   data.object_data[obj_index].id = obj_id;

   bsw_core_output.bsw_index[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_INDEX;
   bsw_core_output.bsw_id[FBK_SIDE_LEFT]     = PA_INVALID_OBJ_ID;
   bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_INDEX;
   bsw_core_output.bsw_id[FBK_SIDE_RIGHT]    = PA_INVALID_OBJ_ID;
   bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;

   lcda_cals.k_bsw_alert_holding_cycles = 3u;

   bsw_persistent.f_prev_bsw_active[FBK_SIDE_LEFT]            = FBK_TRUE;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT]        = obj_id;
   bsw_persistent.prev_bsw_alert_obj_unique_id[FBK_SIDE_LEFT] = obj_id;
   bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT]             = lcda_cals.k_bsw_alert_holding_cycles - 1;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Process_Bsw_Output that is responsible for alert holding with no turn signal. */
   Lcda_Process_Bsw_Output(&bsw_core_output, &bsw_persistent, &lcda_core_input, &lcda_cals, TURN_SIGNAL_NONE, &fbk_output);

   /** \assert Check that the BSW alert level is LCDA_ALERT_STATE_LEVEL_1 and holding counter is increased. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(bsw_core_output.bsw_index[FBK_SIDE_LEFT], obj_index);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_LEFT], obj_id);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT], obj_id);
   EXPECT_EQ(bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT], lcda_cals.k_bsw_alert_holding_cycles);
}

/**
 * Check that BSW Alert hold counter is set to zero when object index is different than invalid.
 * \uts{CSCSA-45602} \sdd{SF-6654} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Output__bsw_hold_counter_set_to_0_when_bsw_index_is_diff_than_invalid)
{
   /** \arrange Set up BSW index and holding counter. */
   bsw_core_output.bsw_index[FBK_SIDE_LEFT]        = 2u;
   bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT]  = 1u;
   bsw_core_output.bsw_index[FBK_SIDE_RIGHT]       = 4u;
   bsw_persistent.bsw_hold_counter[FBK_SIDE_RIGHT] = 3u;

   /** \action Call function Lcda_Process_Bsw_Output that is responsible for alert holding. */
   Lcda_Process_Bsw_Output(&bsw_core_output, &bsw_persistent, &lcda_core_input, &lcda_cals, TURN_SIGNAL_NONE, &fbk_output);

   /** \assert Check that the hold counter is set to zero. */
   EXPECT_EQ(bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(bsw_persistent.bsw_hold_counter[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Check that an previous alert is no longer held if the holding counter exceeds the threshold.
 * \uts{CSCSA-45603} \sdd{SF-6654} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Output__alert_is_switched_off_if_holding_counter_exceeds_threshold)
{
   /** \arrange Set up BSW alert state and corresponding persistent data such that holding counter exceeds threshold. */
   uint8_t obj_id = 3u;

   bsw_core_output.bsw_index[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_INDEX;
   bsw_core_output.bsw_id[FBK_SIDE_LEFT]     = PA_INVALID_OBJ_ID;
   bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_INDEX;
   bsw_core_output.bsw_id[FBK_SIDE_RIGHT]    = PA_INVALID_OBJ_ID;
   bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;

   lcda_cals.k_bsw_alert_holding_cycles = 3u;

   bsw_persistent.f_prev_bsw_active[FBK_SIDE_LEFT]            = FBK_TRUE;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT]        = obj_id;
   bsw_persistent.prev_bsw_alert_obj_unique_id[FBK_SIDE_LEFT] = obj_id;
   bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT]             = lcda_cals.k_bsw_alert_holding_cycles;

   /** \action Call function Lcda_Process_Bsw_Output that is responsible for alert holding with no turn signal. */
   Lcda_Process_Bsw_Output(&bsw_core_output, &bsw_persistent, &lcda_core_input, &lcda_cals, TURN_SIGNAL_NONE, &fbk_output);


   /** \assert Check that the alert is no longer held. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(bsw_core_output.bsw_index[FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
}

/**
 * Create a target object and a guardrail on the left side of the ego such that the target object is overlapping with the the
 * guardrail. Check for environment conflicts of the target and check that a conflict is detected. \uts{CSCSA-45604} \sdd{SF-6652}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_In_Environment_Conflict__is_true_when_object_is_on_left_guardrail)
{
   /** \arrange Create target object and guardrail. */
   boolean_T is_in_conflict;
   uint8_t side   = FBK_SIDE_LEFT;
   uint8_t obj_id = 1;

   tracker_object.vcs_pos.y = -3.0f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0f + lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   lcda_cals.k_bsw_guardrail_distance_safety_margin = 0.0f;

   /** \action Call Lcda_Is_Object_In_Environment_Conflict to check if target object is on guardrail. */
   is_in_conflict = Lcda_Is_Object_In_Environment_Conflict(&bsw_object, lcda_core_input.guardrail_data, &lcda_cals);

   /** \assert Check that evironment conflict is detected. */
   EXPECT_TRUE(is_in_conflict);
}

/**
 * Create a target object and a guardrail on the left side of the ego such that the target object is between the host and the
 * guardrail and no overlapping with the guardrail occurs. Check for environment conflicts of the target and check that no conflict
 * is detected. \uts{CSCSA-45605} \sdd{SF-6652} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_In_Environment_Conflict__is_false_when_object_is_ahead_of_left_guardrail)
{
   /** \arrange Create target object and guardrail. */
   boolean_T is_in_conflict;
   uint8_t side   = FBK_SIDE_LEFT;
   uint8_t obj_id = 1;

   tracker_object.vcs_pos.y = -3.0f;
   tracker_object.width     = 1.9f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0f + lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Object_In_Environment_Conflict to check if object is on guardrail. */
   is_in_conflict = Lcda_Is_Object_In_Environment_Conflict(&bsw_object, lcda_core_input.guardrail_data, &lcda_cals);

   /** \assert Check that no evironment conflict is detected. */
   EXPECT_FALSE(is_in_conflict);
}

/**
 * Create a target object and a guardrail on the left side of the ego such that the target object is between the host and the
 * guardrail and the overlapping with the guardrail is within the calibrated safety margin. Check for environment conflicts of the
 * target and check that no conflict is detected. \uts{CSCSA-45606} \sdd{SF-6652} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test,
       Lcda_Is_Object_In_Environment_Conflict__is_false_when_object_is_ahead_of_left_guardrail_due_to_safety_margin)
{
   /** \arrange Create target object and guardrail. */
   boolean_T is_in_conflict;
   uint8_t side   = FBK_SIDE_LEFT;
   uint8_t obj_id = 2;

   tracker_object.vcs_pos.y = -3.0f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_cals.k_bsw_guardrail_distance_safety_margin = 0.1f;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Object_In_Environment_Conflict to check if object is on guardrail. */
   is_in_conflict = Lcda_Is_Object_In_Environment_Conflict(&bsw_object, lcda_core_input.guardrail_data, &lcda_cals);

   /** \assert Check that no evironment conflict is detected. */
   EXPECT_FALSE(is_in_conflict);
}

/**
 * Create a target object and a guardrail on the right side of the ego such that the target object overlapping with the the guard
 * rail. Check for environment conflicts of the target and check that a conflict is detected. \uts{CSCSA-45607} \sdd{SF-6652}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_In_Environment_Conflict__is_true_when_object_is_on_right_guardrail)
{
   /** \arrange Create target object and guardrail. */
   boolean_T is_in_conflict;
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 6;

   tracker_object.vcs_pos.y = 3.0f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 4.0f - lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Object_In_Environment_Conflict to check if object is on guardrail. */
   is_in_conflict = Lcda_Is_Object_In_Environment_Conflict(&bsw_object, lcda_core_input.guardrail_data, &lcda_cals);

   /** \assert Check that evironment conflict is detected. */
   EXPECT_TRUE(is_in_conflict);
}

/**
 * Create a target object and a guardrail on the right side of the ego such that the target object is between the host and the
 * guardrail and no overlapping with the guardrail occurs. Check for environment conflicts of the target and check that no conflict
 * is detected. \uts{CSCSA-45608} \sdd{SF-6652} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_In_Environment_Conflict__is_false_when_object_is_ahead_of_right_guardrail)
{
   /** \arrange Create target object and guardrail. */
   boolean_T is_in_conflict;
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 3;

   tracker_object.vcs_pos.y = 3.0f;
   tracker_object.width     = 1.9f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 4.0f - lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Object_In_Environment_Conflict to check if object is on guardrail. */
   is_in_conflict = Lcda_Is_Object_In_Environment_Conflict(&bsw_object, lcda_core_input.guardrail_data, &lcda_cals);

   /** \assert Check that no evironment conflict is detected. */
   EXPECT_FALSE(is_in_conflict);
}

/**
 * Create a target object and a guardrail on the right side of the ego such that the target object is between the host and the
 * guardrail and the overlapping with the guardrail is within the calibrated safety margin. Check for environment conflicts of the
 * target and check that no conflict is detected. \uts{CSCSA-45609} \sdd{SF-6652} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test,
       Lcda_Is_Object_In_Environment_Conflict__is_false_when_object_is_ahead_of_right_guardrail_due_to_safety_margin)
{
   /** \arrange Create target object and guardrail. */
   boolean_T is_in_conflict;
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 1;

   tracker_object.vcs_pos.y = 3.0f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_cals.k_bsw_guardrail_distance_safety_margin = 0.1f;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 4.0;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Is_Object_In_Environment_Conflict to check if object is on guardrail. */
   is_in_conflict = Lcda_Is_Object_In_Environment_Conflict(&bsw_object, lcda_core_input.guardrail_data, &lcda_cals);

   /** \assert Check that no evironment conflict is detected. */
   EXPECT_FALSE(is_in_conflict);
}

/*
 * Create a BSW warning relevant track which is falling back fast and activate the fallback handler. Verify that the track is not
 * warned. \uts{CSCSA-45610} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__bsw_alert_level1_for_obj_fallback_fast_handler_active)
{
   /** \arrange Enable fallback handler and prepare track valid for BSW warning. */
   lcda_core_input.enabled_flags.f_fallback_enabled = FBK_TRUE;
   const uint8_t id_track_inside_bsw_zone           = 20;
   const float32_T track_long_pos                   = -8.0f;
   const float32_T track_lat_pos                    = 2.0f;

   /* Set the cal to take the zone from the lcda_core_input which is set to the default zone. Set min cycles required to qualify
    * for warning to one. */
   lcda_core_input.bsw_zone_calculation_mode                      = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles                              = 1;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;

   /* Create track on right side of the ego, inside the default BSW zone. */
   Lcda_Create_Bsw_Track(&tracker_object, id_track_inside_bsw_zone, track_long_pos, track_lat_pos);
   tracker_object.curvi_vel_rel.x = -15.0f;

   /** \action Call function Lcda_Process_Bsw_Object to process BSW warning. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Verify that BSW LCDA_ALERT_STATE_NONE is reached on right side. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/*
 * Create a BSW warning relevant track which is falling back fast and deactivate the fallback handler. Verify that the track is
 * warned. \uts{CSCSA-45611} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__bsw_alert_level1_for_obj_fallback_fast_handler_inactive)
{
   /** \arrange Disable fallback handler and prepare track valid for BSW warning. */
   lcda_core_input.enabled_flags.f_fallback_enabled = FBK_FALSE;
   lcda_cals.k_bsw_dynzone_speed_dropback[0u]       = -2.75f;
   lcda_cals.k_bsw_dynzone_speed_dropback[1u]       = -2.22f;
   lcda_cals.k_bsw_dynzone_speed_dropback[2u]       = -1.95f;
   lcda_cals.k_bsw_dynzone_speed_dropback[3u]       = -1.4f;
   lcda_cals.k_bsw_dynzone_speed_dropback[4u]       = -1.11f;
   lcda_cals.k_bsw_dynzone_speed_dropback[5u]       = -0.83f;
   lcda_cals.k_bsw_dynzone_speed_dropback[6u]       = -0.0f;
   lcda_cals.k_bsw_dynzone_speed_dropback[7u]       = -0.0f;

   lcda_cals.k_bsw_dynzone_range_dropback[0u] = 0.55f;
   lcda_cals.k_bsw_dynzone_range_dropback[1u] = 0.65f;
   lcda_cals.k_bsw_dynzone_range_dropback[2u] = 0.7f;
   lcda_cals.k_bsw_dynzone_range_dropback[3u] = 0.8f;
   lcda_cals.k_bsw_dynzone_range_dropback[4u] = 0.9f;
   lcda_cals.k_bsw_dynzone_range_dropback[5u] = 0.95f;
   lcda_cals.k_bsw_dynzone_range_dropback[6u] = 1.0f;
   lcda_cals.k_bsw_dynzone_range_dropback[7u] = 1.0f;

   /* Set the cal to take the zone from the lcda_core_input which is set to the default zone. Set min cycles required to qualify
    * for warning to one. */
   lcda_core_input.bsw_zone_calculation_mode       = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles               = 1;
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable                      = FBK_TRUE;
   lcda_cals.k_bsw_overlap_area_threshold                         = 0.0f;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;


   /* Create track on right side of the ego, inside the default BSW zone. */
   Lcda_Create_Bsw_Track(&tracker_object, 20u, -7.0f, 2.0f);
   tracker_object.curvi_vel_rel.x = -15.0f;

   /** \action Call function Lcda_Process_Bsw_Object to process BSW warning. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Verify that BSW LCDA_ALERT_STATE_LEVEL_1 is reached on right side. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
}

/*
 * Create a BSW non relevant object. Verify that persistent data is rested.
 * \uts{CSCSA-185793} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__non_relevant_object)
{
   /** \arrange Disable fallback handler and prepare track valid for BSW warning. */

   Lcda_Create_Bsw_Track(&tracker_object, 20u, -7.0f, 2.0f);
   tracker_object.vcs_vel.x                                   = lcda_cals.k_bsw_max_obj_long_vel + EPSILON;
   bsw_persistent.mature_count_in_bsw_zone[tracker_object.id] = 5u;


   /** \action Call function Lcda_Process_Bsw_Object to process BSW warning. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Verify that persistent data for specifc object has been reseted. */
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[tracker_object.id], FBK_ZERO_UINT);
}


/*
 * Check whether transition from fast fallback to slow fallback is suppressed, since qualification counter is not reached. The
 * initialized fallback state shall remain FALLBACK_FAST \uts{CSCSA-45612} \sdd{SF-6659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test,
       Lcda_Update_Fallback_State__fallback_state_switch_from_fast_to_slow_not_qualified_yet_due_to_low_qualification_ctr)
{
   /** \arrange Check fallback handlers transistion from fast to slow. */
   tracker_object.id              = 1;
   tracker_object.curvi_vel_rel.x = lcda_cals.k_bsw_fallback_rel_vel_thres + EPSILON;

   lcda_core_input.enabled_flags.f_fallback_enabled             = FBK_TRUE;
   bsw_persistent.fallback_state[bsw_object.p_tracker_data->id] = FALLBACK_FAST;

   lcda_cals.k_bsw_f_fallback_default_status_slow   = FBK_FALSE;
   lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres = 2u;
   bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id] =
      lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres - 1u;

   /** \action Call function Lcda_Update_Fallback_State to fallback state. */
   Lcda_Update_Fallback_State(&bsw_persistent, &bsw_object, &lcda_cals, &lcda_core_input);

   /** \assert fallback state stays the same and qualification counter increased. */
   EXPECT_EQ(bsw_persistent.fallback_state[bsw_object.p_tracker_data->id], FALLBACK_FAST);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id],
             lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres);
}

/*
 * Check whether transition from fast fallback to slow fallback is supressed, since qualification counter is not reached. The
 * initialized fallback state shall switch to FALLBACK_SLOW \uts{CSCSA-45613} \sdd{SF-6659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Fallback_State__fallback_state_switch_from_fast_to_slow_is_qualified)
{
   /** \arrange 0 - Arrange */
   tracker_object.id              = 1;
   tracker_object.curvi_vel_rel.x = lcda_cals.k_bsw_fallback_rel_vel_thres + EPSILON;

   lcda_core_input.enabled_flags.f_fallback_enabled             = FBK_TRUE;
   bsw_persistent.fallback_state[bsw_object.p_tracker_data->id] = FALLBACK_FAST;

   lcda_cals.k_bsw_f_fallback_default_status_slow                               = FBK_FALSE;
   lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres                             = 2u;
   bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id] = lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres;

   /** \action Call function Lcda_Update_Fallback_State to update the fallback state. */
   Lcda_Update_Fallback_State(&bsw_persistent, &bsw_object, &lcda_cals, &lcda_core_input);

   /** \assert Check that fallback state is updated correctly. */
   EXPECT_EQ(bsw_persistent.fallback_state[bsw_object.p_tracker_data->id], FALLBACK_SLOW);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id],
             lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres + 1u);
}

/*
 * Check whether transition from slow fallback to fast fallback is working, if the relative velocity is below the specified
 * threshold (as specified by the calibratio parameters). \uts{CSCSA-45614} \sdd{SF-6659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Fallback_State__fallback_state_switch_from_slow_to_fast_is_qualified)
{
   /** \arrange 0 - Arrange */
   tracker_object.id              = 1;
   tracker_object.curvi_vel_rel.x = (lcda_cals.k_bsw_fallback_rel_vel_thres - lcda_cals.k_bsw_fallback_rel_vel_thres_hys) - EPSILON;

   lcda_core_input.enabled_flags.f_fallback_enabled             = FBK_TRUE;
   bsw_persistent.fallback_state[bsw_object.p_tracker_data->id] = FALLBACK_SLOW;

   lcda_cals.k_bsw_f_fallback_default_status_slow = FBK_FALSE;

   /** \action Call function Lcda_Update_Fallback_State to update the fallback state. */
   Lcda_Update_Fallback_State(&bsw_persistent, &bsw_object, &lcda_cals, &lcda_core_input);

   /** \assert Check that fallback state is updated correctly. */
   EXPECT_EQ(bsw_persistent.fallback_state[bsw_object.p_tracker_data->id], FALLBACK_FAST);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id], FBK_ZERO_UINT);
}

/*
 * Check that for an active alert in the current cycle the BSW post-processing resets the holding counter and fills the persistent
 * data according to this alert. \uts{CSCSA-45615} \sdd{SF-6665} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Postprocess_Bsw__resets_holding_counter_and_fills_persistent_data_for_active_alert)
{
   /** \arrange Set up core output with an alert and persistent data with holding counter unequal zero. */
   bsw_core_output.bsw_alert[FBK_SIDE_LEFT]       = LCDA_ALERT_STATE_LEVEL_1;
   bsw_core_output.bsw_id[FBK_SIDE_LEFT]          = 3u;
   bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT]   = 5u;
   bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT] = 4u;

   /** \action Call function Lcda_Postprocess_Bsw for post-processing of current BSW cycle. */
   Lcda_Postprocess_Bsw(&bsw_core_output, &bsw_persistent, &lcda_core_input, &lcda_cals, &lcda_persistent, &fbk_output);

   /** \assert Verify that holding counter is reseted and previous alert is set in persistent data. */
   EXPECT_TRUE(bsw_persistent.f_prev_bsw_active[FBK_SIDE_LEFT]);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT], bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(bsw_persistent.prev_bsw_alert_obj_unique_id[FBK_SIDE_LEFT], bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(bsw_persistent.bsw_hold_counter[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/*
 * Check that after an object merge the mature count in zone of the merged object is taken over by the new object.
 * \uts{CSCSA-45616} \sdd{SF-6658} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Bsw_Data_For_Merged_Objects__takes_over_mature_count_in_zone_from_merged_object)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_index    = 3u;
   uint8_t new_obj_id       = 4u;
   uint8_t merged_obj_index = 12u;
   uint8_t merged_id        = 7u;
   uint8_t mature_count     = 5u;

   bsw_persistent.mature_count_in_bsw_zone[merged_id]  = mature_count;
   bsw_persistent.mature_count_in_bsw_zone[new_obj_id] = 0u;
   object_data[merged_obj_index].id                    = merged_id;
   object_data[new_obj_index].id                       = new_obj_id;
   object_data[new_obj_index].f_merge_occured          = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj            = merged_id;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Bsw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Bsw_Data_For_Merged_Objects(&bsw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[new_obj_id], mature_count);
}

/*
 * Check that the pre-processing of BSW processes object merges and resets the BSW core output.
 * \uts{CSCSA-45617} \sdd{SF-6666} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Preprocess_Bsw__processes_object_merges_and_resets_bsw_core_output)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information and core output with
    * non-default values. */
   uint8_t new_obj_index    = 6u;
   uint8_t new_obj_id       = 3u;
   uint8_t merged_obj_index = 12u;
   uint8_t merged_id        = 7u;
   uint8_t mature_count     = 5u;

   bsw_persistent.mature_count_in_bsw_zone[merged_id]  = mature_count;
   bsw_persistent.mature_count_in_bsw_zone[new_obj_id] = 0u;
   object_data[merged_obj_index].id                    = merged_id;
   object_data[new_obj_index].id                       = new_obj_id;
   object_data[new_obj_index].f_merge_occured          = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj            = merged_id;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_2;
   bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = 14u;

   /** \action Call function Lcda_Preprocess_Bsw for pre-processing of current BSW cycle. */
   Lcda_Preprocess_Bsw(&bsw_core_output, &lcda_core_input, &lcda_cals, &fbk_output, &bsw_persistent);

   /** \assert Verify that object merges are processed and BSW core output is reseted. */
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[new_obj_id], mature_count);
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(bsw_core_output.bsw_index[FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
}

/*
 * Check that alert criteria are classified as passed, if a BSW warning was present in the last cycle and the fallback test is
 * passed. \uts{CSCSA-45618} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__returns_true_if_warning_was_active_before_and_fallback_test_passed)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id - 2u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   lcda_cals.k_bsw_overlap_area_check_enable                                = FBK_FALSE;

   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, criterias for overlapped area are passed.
 * \uts{CSCSA-45619} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__ovlp_methd_area_checked_within_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id + 3u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_TRUE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_FOI_OVERLAP;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.1f;
   loc_data.area_overlap_ratio               = 0.5f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, criterias for overlapped area are NOT passed.
 * \uts{CSCSA-45620} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__ovlp_methd_area_checked_below_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_idx  = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.index = obj_idx;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_idx] = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_TRUE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_FOI_OVERLAP;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.2f;
   loc_data.area_overlap_ratio               = 0.1f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_FALSE(result);
}

/*
 * Check that alert criteria are classified as passed, criteria for overlapped area not verified.
 * \uts{CSCSA-45621} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__ovlp_methd_area_not_checked_within_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id + 2u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_FALSE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_FOI_OVERLAP;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.2f;
   loc_data.area_overlap_ratio               = 0.5f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, criteria for overlapped area not verified (2).
 * \uts{CSCSA-45622} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__ovlp_methd_area_not_checked_below_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id + 2u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_FALSE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_FOI_OVERLAP;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.2f;
   loc_data.area_overlap_ratio               = 0.1f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, criteria for overlapped area not verified (3).
 * \uts{CSCSA-45623} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__refp_methd_area_checked_within_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id - 2u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_TRUE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_REF_POINT;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.2f;
   loc_data.area_overlap_ratio               = 0.5f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, criteria for overlapped area not verified (4).
 * \uts{CSCSA-45624} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__refp_methd_area_checked_below_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id + 2u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_TRUE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_REF_POINT;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.2f;
   loc_data.area_overlap_ratio               = 0.1f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, criteria for overlapped area not verified (5).
 * \uts{CSCSA-45625} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__refp_methd_area_not_checked_within_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id + 1u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_FALSE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_REF_POINT;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.2f;
   loc_data.area_overlap_ratio               = 0.5f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, criteria for overlapped area not verified (6).
 * \uts{CSCSA-45626} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__refp_methd_area_not_checked_below_threshold)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with ego side and index. */
   uint8_t side     = FBK_SIDE_RIGHT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side  = side;
   tracker_object.id    = obj_id;
   tracker_object.index = obj_id + 5u;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable = FBK_FALSE;
   lcda_cals.k_lcda_zone_check_method        = LCDA_ZONE_CHECK_REF_POINT;
   lcda_cals.k_bsw_overlap_area_threshold    = 0.2f;
   loc_data.area_overlap_ratio               = 0.1f;


   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, if there was an active warning in the last cycle and the BSW object
 * interferes with a guardrail. \uts{CSCSA-45627} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__returns_true_if_warning_active_and_guardrail_conflict)
{
   /** \arrange Set up persistent data with warning in last cycle and fallback state slow and BSW object with guardrail conflict. */
   uint8_t side     = FBK_SIDE_LEFT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_TRUE;
   Lcda_Object_Location_Data_T loc_data;

   tracker_object.vcs_pos.y = -3.0f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;
   tracker_object.index     = obj_id + 11u;

   lcda_cals.k_min_exist_prob_radar_guardrail                               = 0.99f;
   lcda_cals.k_min_exist_prob_camera_guardrail                              = 0.99f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   lcda_cals.k_bsw_overlap_area_check_enable                                = FBK_FALSE;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0f + lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;

   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as failed, if there was no active warning in the last cycle and the BSW object
 * interferes with a guardrail. \uts{CSCSA-45628} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__returns_false_if_warning_not_active_and_guardrail_conflict)
{
   /** \arrange Set up persistent data without warning in last cycle and fallback state slow and BSW object with guardrail
    * conflict. */
   uint8_t side     = FBK_SIDE_LEFT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_TRUE;
   Lcda_Object_Location_Data_T loc_data;

   tracker_object.vcs_pos.y = -3.0f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_cals.k_min_exist_prob_radar_guardrail                               = 0.99f;
   lcda_cals.k_min_exist_prob_camera_guardrail                              = 0.99f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   lcda_cals.k_bsw_overlap_area_check_enable                                = FBK_FALSE;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0f + lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   bsw_persistent.f_prev_bsw_active[side] = FBK_FALSE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as failed. */
   EXPECT_FALSE(result);
}

/*
 * Check that alert criteria are classified as passed, if there was active CVW warning in the last cycle but BSW not triggered
 * alert, Set mature_count_in_bsw_zone lower than threshold but object age is sufficient to trigger alert. \uts{CSCSA-45629}
 * \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__return_true_if_bsw_not_active_but_cvw_alert_was_active)
{
   /** \arrange Set up persistent data without warning in last cycle and fallback state slow and BSW object with guardrail
    * conflict. */
   uint8_t side     = FBK_SIDE_LEFT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_FALSE;
   Lcda_Object_Location_Data_T loc_data;

   tracker_object.vcs_pos.y = -4.2f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;
   tracker_object.index     = obj_id - 2u;
   tracker_object.age       = lcda_cals.k_bsw_alert_track_age;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge      = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check                          = FBK_FALSE;
   lcda_cals.k_bsw_uses_cvw_alert_state_enabled                             = FBK_TRUE;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_bsw_overlap_area_check_enable                                = FBK_FALSE;
   lcda_cals.k_min_exist_prob_radar_guardrail                               = 0.99f;
   lcda_cals.k_min_exist_prob_camera_guardrail                              = 0.99f;


   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0f + lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   bsw_persistent.f_prev_bsw_active[side] = FBK_TRUE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   // Create a CVW alert on the same object
   Lcda_Create_Cvw_Alert(side, obj_id, &cvw_persistent);

   // Adjust persistent parameters
   bsw_persistent.f_prev_bsw_active[side]          = FBK_FALSE;
   bsw_persistent.fallback_state[obj_id]           = FALLBACK_SLOW;
   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 1u;

   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as failed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as passed, if there was no active warning in the last cycle, the BSW object does not
 * interfere with a guardrail, and the BSW object is not in the ego lane. \uts{CSCSA-45630} \sdd{SF-6650}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__returns_true_if_warning_not_active_and_no_guardrail_conflict)
{
   /** \arrange Set up persistent data with no warning in last cycle and fallback state slow and BSW object with no guardrail
    * conflict. */
   uint8_t side     = FBK_SIDE_LEFT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_TRUE;
   Lcda_Object_Location_Data_T loc_data;

   tracker_object.vcs_pos.y   = -3.0f;
   tracker_object.width       = 2.1f;
   bsw_object.ego_side        = side;
   tracker_object.id          = obj_id;
   tracker_object.age         = lcda_cals.k_bsw_alert_track_age;
   tracker_object.curvi_pos.y = -20.0f;

   lcda_cals.k_min_exist_prob_radar_guardrail                               = 0.99f;
   lcda_cals.k_min_exist_prob_camera_guardrail                              = 0.99f;
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check                          = FBK_TRUE;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   lcda_cals.k_bsw_overlap_area_check_enable                                = FBK_FALSE;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -30.0f;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   bsw_persistent.fallback_state[obj_id]           = FALLBACK_SLOW;
   bsw_persistent.mature_count_in_bsw_zone[obj_id] = lcda_cals.k_bsw_min_mature_cycles;

   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as passed. */
   EXPECT_TRUE(result);
}

/*
 * Check that alert criteria are classified as invalid, if object has reached the host front and the corresponding calibration
 * value has been activated. \uts{CSCSA-45631} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__returns_false_if_object_reached_host_front)
{
   /** \arrange Set up persistent data with no warning in last cycle and fallback state slow and BSW object with no guardrail
    * conflict. */
   uint8_t side   = FBK_SIDE_LEFT;
   uint8_t obj_id = 6u;
   Lcda_Object_Location_Data_T loc_data;

   bsw_object.ego_side           = side;
   bsw_object.obj_front_position = 1.0f;
   tracker_object.vcs_pos.y      = -3.0f;
   tracker_object.width          = 2.1f;
   tracker_object.id             = obj_id;
   tracker_object.age            = lcda_cals.k_bsw_alert_track_age;
   tracker_object.curvi_pos.y    = -20.0f;
   tracker_object.vcs_vel_rel.x  = 1.0f;

   lcda_cals.k_min_exist_prob_radar_guardrail                               = 0.99f;
   lcda_cals.k_min_exist_prob_camera_guardrail                              = 0.99f;
   lcda_cals.k_lcda_f_enable_obj_in_ego_lane_check                          = FBK_FALSE;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions           = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   lcda_cals.k_bsw_overlap_area_check_enable                                = FBK_FALSE;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -30.0f;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   bsw_persistent.fallback_state[obj_id]           = FALLBACK_SLOW;
   bsw_persistent.mature_count_in_bsw_zone[obj_id] = lcda_cals.k_bsw_min_mature_cycles;

   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are invalid. */
   boolean_T result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                                        LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as invalid. */
   EXPECT_FALSE(result);
}


/*
 * Check that object is classified as relevant if all criteria are fulfilled with object status mature.
 * \uts{CSCSA-45632} \sdd{SF-6653} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Relevant_For_Bsw__returns_true_if_all_criteria_are_fulfilled_with_status_mature)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status mature. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.vcs_heading           = lcda_cals.k_bsw_max_heading_abs - EPSILON;
   tracker_object.vcs_vel.x             = lcda_cals.k_bsw_min_obj_long_vel + EPSILON;
   tracker_object.existence_probability = 1.0f;

   /** \action Call function Lcda_Is_Object_Relevant_For_Bsw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   /** \assert Verify that object is classified as relevant. */
   EXPECT_TRUE(result);
}

/*
 * Check that object is not classified as relevant if heading is out of range.
 * \uts{CSCSA-45633} \sdd{SF-6653} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Relevant_For_Bsw__returns_false_if_heading_is_out_of_range)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed, except for heading being out of range. */
   boolean_T result = FBK_FALSE;

   tracker_object.status      = PA_OBJ_STATUS_MATURE;
   tracker_object.vcs_heading = lcda_cals.k_bsw_max_heading_abs + lcda_cals.k_bsw_max_heading_abs_hysteresis + EPSILON;
   tracker_object.vcs_vel.x   = lcda_cals.k_bsw_min_obj_long_vel + EPSILON;

   tracker_object.existence_probability = 1.0f;

   /** \action Call function Lcda_Is_Object_Relevant_For_Bsw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_FALSE(result);
}

/*
 * Check that object is classified as relevant if heading is out of range but in range with hysteresis.
 * \uts{CSCSA-116580} \sdd{SF-6653} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Relevant_For_Bsw__returns_false_if_heading_is_in_range_hysteris)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed, except for heading being out of range. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.vcs_heading           = lcda_cals.k_bsw_max_heading_abs + lcda_cals.k_bsw_max_heading_abs_hysteresis - EPSILON;
   tracker_object.vcs_vel.x             = lcda_cals.k_bsw_min_obj_long_vel + EPSILON;
   tracker_object.id                    = 2u;
   tracker_object.curvi_pos.y           = -FBK_ONE_F;
   tracker_object.existence_probability = 1.0f;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT] = tracker_object.id;

   /** \action Call function Lcda_Is_Object_Relevant_For_Bsw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, (&lcda_cals), &bsw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_TRUE(result);
}

/*
 * Check that object is not classified as relevant if velocity is out of range.
 * \uts{CSCSA-45634} \sdd{SF-6653} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Relevant_For_Bsw__returns_false_if_velocity_is_out_of_range)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed, except for velocity being out of range. */
   boolean_T result = FBK_FALSE;

   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.vcs_heading           = -lcda_cals.k_bsw_max_heading_abs + EPSILON;
   tracker_object.vcs_vel.x             = lcda_cals.k_bsw_min_obj_long_vel + lcda_cals.k_bsw_min_obj_long_vel_hysteresis - EPSILON;
   tracker_object.existence_probability = 1.0f;

   /** \action Call function Lcda_Is_Object_Relevant_For_Bsw to evaluate if object is relevant. */
   result = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   /** \assert Verify that object is classified as not relevant. */
   EXPECT_FALSE(result);
}

/*
 * Check that a new critical object is considered most critical if no critical object was present before.
 * \uts{CSCSA-45635} \sdd{SF-6657} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Most_Critical_Bsw_Object__sets_new_object_most_critical_if_no_object_was_critical_before)
{
   /** \arrange Set up bsw object and core output such that no other object was critical before. */
   uint8_t side = FBK_SIDE_RIGHT;

   bsw_core_output.bsw_index[side] = PA_INVALID_OBJ_INDEX;
   tracker_object.index            = 5u;
   tracker_object.id               = 8u;
   tracker_object.unique_id        = 9u;
   tracker_object.vcs_pos.x        = 8.0f;
   tracker_object.vcs_pos.y        = 3.0f;
   tracker_object.vcs_heading      = 0.0f;
   bsw_object.ego_side             = side;

   /** \action Call function Lcda_Set_Most_Critical_Bsw_Object to update the most critical object. */
   Lcda_Set_Most_Critical_Bsw_Object(&bsw_core_output, &bsw_object, TURN_SIGNAL_NONE);

   /** \assert Verify that the new object is set as most critical object. */
   EXPECT_EQ(bsw_core_output.bsw_index[side], tracker_object.index);
   EXPECT_EQ(bsw_core_output.bsw_id[side], tracker_object.id);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[side], tracker_object.unique_id);
   EXPECT_EQ(bsw_core_output.bsw_alert[side], LCDA_ALERT_STATE_LEVEL_1);
}

/*
 * Check that a new critical object is considered most critical if it is closer to the ego vehicle than the currently most critical
 * object. \uts{CSCSA-45636} \sdd{SF-6657} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Most_Critical_Bsw_Object__sets_new_object_most_critical_if_it_is_closer_to_ego)
{
   /** \arrange Set up bsw object and core output such that new object is closer to the ego. */
   uint8_t side = FBK_SIDE_LEFT;

   bsw_core_output.bsw_index[side]    = 24u;
   tracker_object.index               = 5u;
   tracker_object.id                  = 8u;
   tracker_object.unique_id           = 9u;
   tracker_object.vcs_pos.x           = -5.5f;
   tracker_object.vcs_pos.y           = 3.3f;
   tracker_object.vcs_heading         = 0.0f;
   tracker_object.length              = 5.0f;
   tracker_object.width               = 2.0f;
   tracker_object.vcs_vel_rel.x       = 4.0f;
   bsw_object.obj_front_position      = -3.0f;
   bsw_object.ego_side                = side;
   bsw_core_output.bsw_distance[side] = -4.0f;

   /** \action Call function Lcda_Set_Most_Critical_Bsw_Object to update the most critical object. */
   Lcda_Set_Most_Critical_Bsw_Object(&bsw_core_output, &bsw_object, TURN_SIGNAL_NONE);

   /** \assert Verify that the new object is set as most critical object. */
   EXPECT_EQ(bsw_core_output.bsw_index[side], tracker_object.index);
   EXPECT_EQ(bsw_core_output.bsw_id[side], tracker_object.id);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[side], tracker_object.unique_id);
   EXPECT_EQ(bsw_core_output.bsw_alert[side], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_FLOAT_EQ(bsw_core_output.bsw_distance[side], -3.0f);
   EXPECT_FLOAT_EQ(bsw_core_output.bsw_ttp[side], 2.0f);
}

/*
 * Check that a new critical object is not considered most critical if it is not closer to the ego vehicle than the currently most
 * critical object. \uts{CSCSA-45637} \sdd{SF-6657} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Most_Critical_Bsw_Object__sets_no_new_most_critical_object_if_new_obj_is_not_closer_to_ego)
{
   /** \arrange Set up bsw object and core output such that new object is not closer to the ego. */
   uint8_t side = FBK_SIDE_LEFT;

   bsw_core_output.bsw_index[side]     = 24u;
   bsw_core_output.bsw_id[side]        = 23u;
   bsw_core_output.bsw_unique_id[side] = 23u;
   bsw_core_output.bsw_alert[side]     = LCDA_ALERT_STATE_LEVEL_1;
   tracker_object.index                = 5u;
   tracker_object.id                   = 8u;
   bsw_object.obj_front_position       = -4.0f;
   bsw_object.ego_side                 = side;
   bsw_core_output.bsw_distance[side]  = -3.0f;
   bsw_core_output.bsw_ttp[side]       = 1.8f;

   /** \action Call function Lcda_Set_Most_Critical_Bsw_Object to update the most critical object. */
   Lcda_Set_Most_Critical_Bsw_Object(&bsw_core_output, &bsw_object, TURN_SIGNAL_NONE);

   /** \assert Verify that most critical object is unchanged. */
   EXPECT_EQ(bsw_core_output.bsw_index[side], 24u);
   EXPECT_EQ(bsw_core_output.bsw_id[side], 23u);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[side], 23u);
   EXPECT_EQ(bsw_core_output.bsw_alert[side], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_FLOAT_EQ(bsw_core_output.bsw_distance[side], -3.0f);
   EXPECT_FLOAT_EQ(bsw_core_output.bsw_ttp[side], 1.8f);
}

/*
 * Check that a fallback warning is considered as in time if longitudinal relative velocity is above threshold.
 * \uts{CSCSA-45638} \sdd{SF-6651} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Fallback_Warning_In_Time__is_true_if_longitudinal_rel_velocity_is_above_threshold)
{
   /** \arrange Set up bsw object such that longitudinal relative velocity is above threshold. */
   float32_T host_length = 5.0f;
   boolean_T result      = FBK_FALSE;

   tracker_object.vcs_vel_rel.x = lcda_cals.k_bsw_suppress_late_warning_max_rel_vel + EPSILON;

   /** \action Call function Lcda_Is_Fallback_Warning_In_Time to evalute if fallback warning is in time. */
   result = Lcda_Is_Fallback_Warning_In_Time(&bsw_object, host_length, &lcda_cals);

   /** \assert Verify that warning shall still be relevant. */
   EXPECT_TRUE(result);
}

/*
 * Create a back falling track which could be warning relevant. Check if the fallback warning for LCDA is in time. Since the
 * objects distance is too large, a possible warning should be suppressed. \uts{CSCSA-45639} \sdd{SF-6651}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Fallback_Warning_In_Time__distant_object_is_not_relevant_since_it_is_falling_back)
{
   /** \arrange Set up distant bsw object and a short BSW warning zone (both longitudinal). */
   Vector_2d_T zone_point;
   float32_T host_length = 5.0f;
   boolean_T result      = FBK_FALSE;
   zone_point.x          = -10;

   bsw_object.p_tracker_data = &tracker_object;

   bsw_object.zone.size      = 3u;
   bsw_object.zone.points[1] = zone_point;

   tracker_object.vcs_vel_rel.x = lcda_cals.k_bsw_suppress_late_warning_max_rel_vel - EPSILON;
   tracker_object.vcs_pos.x     = -40;

   /** \action Call function Lcda_Is_Fallback_Warning_In_Time to evalute if fallback warning is in time. */
   result = Lcda_Is_Fallback_Warning_In_Time(&bsw_object, host_length, &lcda_cals);

   /** \assert Verify that warning shall no longer be relevant. */
   EXPECT_FALSE(result);
}

/*
 * Checks that target is overtaking host and crossed front custom limit before bsw alert is taken off. Test assumes relative
 * velocity to be negative, then false expected from output. \uts{CSCSA-45640} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__returns_false_if_negative_velocity_sot_enabled)
{
   /** \arrange Set up object data so that object crossed front limit but relative velocity is negative. */
   boolean_T result = FBK_TRUE;

   tracker_object.vcs_vel_rel.x  = -3.0f;
   bsw_object.obj_front_position = 1.0f;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions           = FBK_TRUE;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions to evaluate if criteria, to take bsw alert off earlier,
    * are passed. */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that warning shall still be relevant. */
   EXPECT_FALSE(result);
}


/*
 * Checks that target is overtaking host and crossed front custom limit before bsw alert is taken off. Test assumes relative
 * velocity to be negative, then true expected from output. \uts{CSCSA-99045} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__returns_true_if_negative_velocity_sot_disabled)
{
   /** \arrange Set up object data so that object crossed front limit but relative velocity is negative. */
   boolean_T result = FBK_TRUE;

   tracker_object.vcs_vel_rel.x  = -3.0f;
   bsw_object.obj_front_position = 1.0f;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions           = FBK_FALSE;


   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions to evaluate if criteria, to take bsw alert off earlier,
    * are passed. */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that warning shall still be relevant. */
   EXPECT_TRUE(result);
}


/*
 * Checks that target is overtaking host and crossed front custom limit before bsw alert is taken off. Test assumes target not
 * reached front limit yet, then false expected from output. \uts{CSCSA-45641} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__returns_true_if_negative_front_position)
{
   /** \arrange Set up object data so that object has positive relative velocity, however it hasn't crossed front limit yet. */
   boolean_T result = FBK_FALSE;

   tracker_object.vcs_vel_rel.x  = 3.0f;
   bsw_object.obj_front_position = -4.0f;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode    = BSW_FRONT;

   bsw_object.zone.size        = 3u;
   bsw_object.zone.points[0].x = -3.0f;
   bsw_object.zone.points[1].x = -3.0f;
   bsw_object.zone.points[2].x = -10.0f;
   bsw_object.zone.points[3].x = -10.0f;
   bsw_object.zone.points[4].x = -10.0f;
   bsw_object.zone.points[5].x = -10.0f;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions to evaluate if criteria, to take bsw alert off earlier,
    * are passed. */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert shall still be relevant. */
   EXPECT_TRUE(result);
}

/*
 * Checks that target is overtaking host and crossed front custom limit before bsw alert is taken off. Test assumes target reached
 * front limit and continues overtaking, then false expected from output. \uts{CSCSA-45642} \sdd{CSCSA-70156}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__returns_false_if_front_position_and_relvel_positive)
{
   /** \arrange Set up object data so that object crossed front limit and relative velocity is positive. */
   boolean_T result = FBK_TRUE;

   tracker_object.vcs_vel_rel.x  = 3.0f;
   bsw_object.obj_front_position = 0.1f;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode    = HOST_FRONT;


   bsw_object.zone.size        = 3u;
   bsw_object.zone.points[0].x = -3.0f;
   bsw_object.zone.points[1].x = -3.0f;
   bsw_object.zone.points[2].x = -10.0f;
   bsw_object.zone.points[3].x = -10.0f;
   bsw_object.zone.points[4].x = -10.0f;
   bsw_object.zone.points[5].x = -10.0f;
   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions to evaluate if criteria, to take bsw alert off earlier,
    * are passed. */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert shall be not relevant. */
   EXPECT_FALSE(result);
}


/*
 * Checks specific front zone boundary condition, when subject is overtaking target and front of the object is within zone. Alert
 * is expected. \uts{CSCSA-90036} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__sot_whole_obj_in_zone)
{
   /** \arrange Set up object data so that whole object in zone and relative velocity is negative. */
   boolean_T result = FBK_TRUE;

   tracker_object.vcs_vel_rel.x  = -3.0f;
   bsw_object.obj_front_position = -3.1f;


   bsw_object.zone.size        = 3u;
   bsw_object.zone.points[0].x = -3.0f;
   bsw_object.zone.points[1].x = -3.0f;
   bsw_object.zone.points[2].x = -10.0f;
   bsw_object.zone.points[3].x = -10.0f;
   bsw_object.zone.points[4].x = -10.0f;
   bsw_object.zone.points[5].x = -10.0f;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions           = FBK_TRUE;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions to evaluate if criteria, to take bsw alert off earlier,
    * are passed. */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert shall still be relevant. */
   EXPECT_TRUE(result);
}

/*
 * Checks that target was causing alert in previous cycle and has length greater than minimum length required to be considered as a
 * long object. Expect object status as a long object (True). \uts{CSCSA-45643} \sdd{CSCSA-70157} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Considered_Long__returns_true_if_object_above_threshold)
{
   /** \arrange Set up object data so that object triggered alert before and is longer than threshold. */
   boolean_T result = FBK_FALSE;

   uint8_t obj_id = 2u;

   tracker_object.id     = obj_id;
   tracker_object.length = 7.11f;

   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT] = obj_id;
   bsw_persistent.f_prev_long_truck_status[obj_id]     = FBK_FALSE;

   lcda_cals.k_bsw_min_length_long_object     = 7.1f;
   lcda_cals.k_bsw_min_length_long_object_hys = 0.2f;

   /** \action Call function Lcda_Is_Object_Considered_Long to evaluate what type of object is considered. */
   result = Lcda_Is_Object_Considered_Long(&bsw_persistent, &bsw_object, &lcda_cals);

   /** \assert Verify that object should be considered as a long object. */
   EXPECT_TRUE(result);
   EXPECT_EQ(result, bsw_persistent.f_prev_long_truck_status[obj_id]);
}


/*
 * Checks that target was causing alert in previous cycle and has length below minimum length required to be considered as a long
 * object. Expect object status as a short object (False). \uts{CSCSA-45644} \sdd{CSCSA-70157} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Considered_Long__returns_false_if_object_below_hysteresis)
{
   /** \arrange Set up object data so that object triggered alert before and is longer than threshold. */
   boolean_T result = FBK_TRUE;

   uint8_t obj_id = 2u;

   tracker_object.id     = obj_id;
   tracker_object.length = 6.89f;

   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT] = obj_id;
   bsw_persistent.f_prev_long_truck_status[obj_id]     = FBK_FALSE;

   lcda_cals.k_bsw_min_length_long_object     = 7.1f;
   lcda_cals.k_bsw_min_length_long_object_hys = 0.2f;

   /** \action Call function Lcda_Is_Object_Considered_Long to evaluate what type of object is considered. */
   result = Lcda_Is_Object_Considered_Long(&bsw_persistent, &bsw_object, &lcda_cals);

   /** \assert Verify that object should be considered as a short object. */
   EXPECT_FALSE(result);
   EXPECT_EQ(result, bsw_persistent.f_prev_long_truck_status[obj_id]);
}

/*
 * Checks that target was causing alert in previous cycle and has length in a range of long object hysteresis. Expect object status
 * as a long object (True). \uts{CSCSA-45645} \sdd{CSCSA-70157} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Considered_Long__returns_true_if_object_in_hysteresis_and_previously_long)
{
   /** \arrange Set up object data so that object triggered alert before and is below minimum length but in a range of a
    * hysteresis. */
   boolean_T result = FBK_FALSE;

   uint8_t obj_id = 2u;

   tracker_object.id     = obj_id;
   tracker_object.length = 6.99f;

   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT] = obj_id;
   bsw_persistent.f_prev_long_truck_status[obj_id]     = FBK_TRUE;

   lcda_cals.k_bsw_min_length_long_object     = 7.1f;
   lcda_cals.k_bsw_min_length_long_object_hys = 0.2f;

   /** \action Call function Lcda_Is_Object_Considered_Long to evaluate what type of object is considered. */
   result = Lcda_Is_Object_Considered_Long(&bsw_persistent, &bsw_object, &lcda_cals);

   /** \assert Verify that object should be considered as a long object. */
   EXPECT_TRUE(result);
   EXPECT_EQ(result, bsw_persistent.f_prev_long_truck_status[obj_id]);
}

/*
 * Check if alert still valid when object change lane intention check enabled, other criteria fullfilled and VCS coordinates used
 * \uts{CSCSA-45646} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__alert_valid_vcs)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;

   tracker_object.vcs_pos.y     = -1.0f;
   tracker_object.vcs_vel_rel.y = -1.0f;

   tracker_object.curvi_pos.y     = -1.0f;
   tracker_object.curvi_vel_rel.y = -1.0f;

   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(
      &lcda_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_TRUE(f_alert_valid);
}

/*
 * Check if alert still valid when object change lane intention check enabled, other criteria fullfilled and CURVI coordinates used
 * \uts{CSCSA-45647} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__alert_valid_curvi)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use CURVI coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_TRUE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;

   tracker_object.vcs_pos.y     = -1.0f;
   tracker_object.vcs_vel_rel.y = -1.0f;

   tracker_object.curvi_pos.y     = -1.0f;
   tracker_object.curvi_vel_rel.y = -1.0f;

   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(&lcda_core_input, f_alert_valid, f_warning_active,
                                                                    f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data,
                                                                    LCDA_USE_CURVI);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_TRUE(f_alert_valid);
}

/*
 * Check if f_alert_valid status not changed when object is outside of the ego lane.
 * \uts{CSCSA-45649} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__obj_outside_ego_lane)
{
   /** \arrange Set up data so all functionality enabled but the object is outside of the ego lane. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_TRUE;
   boolean_T f_obj_in_ego_lane = FBK_FALSE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;

   tracker_object.vcs_pos.y     = -1.0f;
   tracker_object.vcs_vel_rel.y = -1.0f;

   tracker_object.curvi_pos.y     = -1.0f;
   tracker_object.curvi_vel_rel.y = -1.0f;

   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(
      &lcda_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data, LCDA_USE_VCS);

   /** \assert Verify that alert is not updated. */
   EXPECT_TRUE(f_alert_valid);
}

/*
 * Check if f_alert_valid status not changed when object lane change intention check disabled.
 * \uts{CSCSA-45650} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__obj_lane_change_check_not_enabled)
{
   /** \arrange Set up data so that object lane change intention check disabled. */

   boolean_T f_alert_valid     = FBK_FALSE;
   boolean_T f_warning_active  = FBK_TRUE;
   boolean_T f_obj_in_ego_lane = FBK_FALSE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;

   tracker_object.vcs_pos.y     = -1.0f;
   tracker_object.vcs_vel_rel.y = -1.0f;

   tracker_object.curvi_pos.y     = -1.0f;
   tracker_object.curvi_vel_rel.y = -1.0f;

   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(
      &lcda_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data, LCDA_USE_VCS);

   /** \assert Verify that alert is not updated. */
   EXPECT_FALSE(f_alert_valid);
}


/*
 * Check if alert still valid when object change lane intention check enabled with extended position logic, VCS coordinates used
 * \uts{CSCSA-68112} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_no_suppress)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;

   tracker_object.vcs_pos.y     = -3.5f;
   tracker_object.vcs_vel_rel.y = -1.0f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position = -5.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(
      &lcda_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data, LCDA_USE_VCS);

   /** \assert Verify that alert is not updated. */
   EXPECT_TRUE(f_alert_valid);
}


/*
 * Check if alert still valid when object change lane intention check enabled with extended position logic, VCS coordinates used
 * \uts{CSCSA-187415} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_f_warning_active_true)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_TRUE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;

   tracker_object.vcs_pos.y     = -3.5f;
   tracker_object.vcs_vel_rel.y = -1.0f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position = -5.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(
      &lcda_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data, LCDA_USE_VCS);

   /** \assert Verify that alert is not updated. */
   EXPECT_TRUE(f_alert_valid);
}


/*
 * Check if alert suppressed when object change lane intention check enabled with extended position logic, VCS coordinates used
 * \uts{CSCSA-68113} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_suppress)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;

   tracker_object.vcs_pos.y     = -0.5f;
   tracker_object.vcs_vel_rel.y = -0.3f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position           = -10.0f;
   bsw_object.zone.points[REAR_EGO_SIDE].y = -2.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(
      &lcda_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as invalid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is suppressed when object change lane intention advanced logic is enabled and zone without hysteresis is picked
 * \uts{CSCSA-120726} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_suppress_no_hysteresis)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;
   lcda_cals.k_bsw_f_use_zone_without_hysteresis_lane_change_intention      = FBK_TRUE;

   tracker_object.vcs_pos.y     = 0.5f;
   tracker_object.vcs_vel_rel.y = 0.3f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position           = -10.0f;
   bsw_object.zone.points[REAR_EGO_SIDE].y = -2.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(&lcda_core_input, f_alert_valid, f_warning_active,
                                                                    f_obj_in_ego_lane, (&lcda_cals), &bsw_object, p_vehicle_data,
                                                                    LCDA_USE_VCS);

   /** \assert Verify that alert is classified as invalid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is suppressed when object change lane intention advanced logic is enabled and zone with hysteresis is picked
 * \uts{CSCSA-120727} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_suppress_with_hysteresis)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;
   lcda_cals.k_bsw_f_use_zone_without_hysteresis_lane_change_intention      = FBK_FALSE;

   tracker_object.vcs_pos.y     = 0.5f;
   tracker_object.vcs_vel_rel.y = 0.3f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position           = -10.0f;
   bsw_object.zone.points[REAR_EGO_SIDE].y = -2.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(&lcda_core_input, f_alert_valid, f_warning_active,
                                                                    f_obj_in_ego_lane, (&lcda_cals), &bsw_object, p_vehicle_data,
                                                                    LCDA_USE_VCS);

   /** \assert Verify that alert is classified as invalid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is valid when cal parameters is off.
 * \uts{CSCSA-45651} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__cal_disabled)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid. */

   boolean_T f_alert_valid                                             = FBK_TRUE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_FALSE;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_TRUE(f_alert_valid);
}

/*
 * Check if alert is not valid when it was already false.
 * \uts{CSCSA-45652} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__cal_enabled_f_alert_disabled)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert invalid since f_alert_valid is from the beginning invalid.
    */

   boolean_T f_alert_valid                                             = FBK_FALSE;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_TRUE;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is invalid when object crosses the zone boundary from ego side, left side in VCS
 * \uts{CSCSA-45653} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__left_side_vcs_outside_zone)
{
   /** \arrange Set up data so all requirements fullfilled, set a fragment of an object outside the zone, to keep alert invalid. */

   boolean_T f_alert_valid = FBK_TRUE;
   Fbk_Object_Corners_T target_corners;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_TRUE;
   bsw_object.ego_side                                                 = FBK_SIDE_LEFT;

   tracker_object.vcs_pos.y   = -1.0f;
   tracker_object.vcs_heading = 0.0f;
   tracker_object.width       = 1.83f;
   tracker_object.length      = 4.65f;

   bsw_object.zone.points[FRONT_EGO_SIDE].x = -2.0f;
   bsw_object.zone.points[FRONT_EGO_SIDE].y = -2.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &bsw_object.p_tracker_data->vcs_pos, &bsw_object.p_tracker_data->vcs_heading,
                                &bsw_object.p_tracker_data->length, &bsw_object.p_tracker_data->width);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is invalid when object crosses the zone boundary from ego side, left side in curvi
 * \uts{CSCSA-45654} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__left_side_curvi_outside_zone)
{
   /** \arrange Set up data so all requirements fullfilled, set a fragment of an object outside the zone, to keep alert invalid. */

   boolean_T f_alert_valid = FBK_TRUE;
   Fbk_Object_Corners_T target_corners;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_TRUE;
   bsw_object.ego_side                                                 = FBK_SIDE_LEFT;

   tracker_object.vcs_pos.y   = -1.0f;
   tracker_object.vcs_heading = 0.0f;
   tracker_object.width       = 1.83f;
   tracker_object.length      = 4.65f;

   bsw_object.zone.points[FRONT_EGO_SIDE].x = -2.0f;
   bsw_object.zone.points[FRONT_EGO_SIDE].y = -2.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &bsw_object.p_tracker_data->vcs_pos, &bsw_object.p_tracker_data->vcs_heading,
                                &bsw_object.p_tracker_data->length, &bsw_object.p_tracker_data->width);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_CURVI);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is invalid when object crosses the zone boundary from ego side, right side in VCS
 * \uts{CSCSA-45655} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__right_side_vcs_outside_zone)
{
   /** \arrange Set up data so all requirements fullfilled, set a fragment of an object outside the zone, to keep alert invalid. */

   boolean_T f_alert_valid = FBK_TRUE;
   Fbk_Object_Corners_T target_corners;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_TRUE;
   bsw_object.ego_side                                                 = FBK_SIDE_RIGHT;

   tracker_object.vcs_pos.y   = 1.0f;
   tracker_object.vcs_heading = 0.0f;
   tracker_object.width       = 1.83f;
   tracker_object.length      = 4.65f;

   bsw_object.zone.points[FRONT_EGO_SIDE].x = -2.0f;
   bsw_object.zone.points[FRONT_EGO_SIDE].y = -2.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &bsw_object.p_tracker_data->vcs_pos, &bsw_object.p_tracker_data->vcs_heading,
                                &bsw_object.p_tracker_data->length, &bsw_object.p_tracker_data->width);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is invalid when object crosses the zone boundary from ego side, right side in curvi
 * \uts{CSCSA-45656} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__right_side_curvi_outside_zone)
{
   /** \arrange Set up data so all requirements fullfilled, set a fragment of an object outside the zone, to keep alert invalid. */

   boolean_T f_alert_valid = FBK_TRUE;
   Fbk_Object_Corners_T target_corners;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_TRUE;
   bsw_object.ego_side                                                 = FBK_SIDE_RIGHT;

   tracker_object.vcs_pos.y   = 1.0f;
   tracker_object.vcs_heading = 0.0f;
   tracker_object.width       = 1.83f;
   tracker_object.length      = 4.65f;

   bsw_object.zone.points[FRONT_EGO_SIDE].x = -2.0f;
   bsw_object.zone.points[FRONT_EGO_SIDE].y = -2.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &bsw_object.p_tracker_data->vcs_pos, &bsw_object.p_tracker_data->vcs_heading,
                                &bsw_object.p_tracker_data->length, &bsw_object.p_tracker_data->width);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_CURVI);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert is invalid when rear of the object crosses the zone boundary from ego side, left side in vcs
 * \uts{CSCSA-45657} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__rear_side_outside_zone)
{
   /** \arrange Set up data so all requirements fullfilled, set a fragment of an object outside the zone, to keep alert invalid. */

   boolean_T f_alert_valid = FBK_TRUE;
   Fbk_Object_Corners_T target_corners;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_TRUE;
   bsw_object.ego_side                                                 = FBK_SIDE_LEFT;

   tracker_object.vcs_pos.y   = -2.83f;
   tracker_object.vcs_heading = -0.2f;
   tracker_object.width       = 1.83f;
   tracker_object.length      = 4.65f;

   bsw_object.zone.points[FRONT_EGO_SIDE].x = -2.0f;
   bsw_object.zone.points[FRONT_EGO_SIDE].y = -2.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &bsw_object.p_tracker_data->vcs_pos, &bsw_object.p_tracker_data->vcs_heading,
                                &bsw_object.p_tracker_data->length, &bsw_object.p_tracker_data->width);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as invalid. */
   EXPECT_FALSE(f_alert_valid);
}

/*
 * Check if alert still valid when object is fully in zone.
 * \uts{CSCSA-45658} \sdd{CSCSA-39775} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge__object_full_in_zone)
{
   /** \arrange Set up data so all requirements fullfilled, set object inside the zone, to keep alert valid. */

   boolean_T f_alert_valid = FBK_TRUE;
   Fbk_Object_Corners_T target_corners;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge = FBK_TRUE;
   bsw_object.ego_side                                                 = FBK_SIDE_LEFT;

   tracker_object.vcs_pos.y   = -3.0f;
   tracker_object.vcs_heading = 0.0f;
   tracker_object.width       = 1.83f;
   tracker_object.length      = 4.65f;

   bsw_object.zone.points[FRONT_EGO_SIDE].x = -2.0f;
   bsw_object.zone.points[FRONT_EGO_SIDE].y = 2.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &bsw_object.p_tracker_data->vcs_pos, &bsw_object.p_tracker_data->vcs_heading,
                                &bsw_object.p_tracker_data->length, &bsw_object.p_tracker_data->width);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Overhangs_Zone_Edge(f_alert_valid, &lcda_cals, &bsw_object, LCDA_USE_VCS);

   /** \assert Verify that alert is classified as valid. */
   EXPECT_TRUE(f_alert_valid);
}


/*
 * Check that mature_count_in_bsw_zone is set correctly while merged obj id is invalid
 * \uts{CSCSA-45659} \sdd{SF-6658} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Bsw_Data_For_Merged_Objects__false_flag_merge_occured)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_id    = 3u;
   uint8_t merged_obj_id = 12u;
   uint8_t merged_id     = 7u;
   uint8_t mature_count  = 5u;

   bsw_persistent.mature_count_in_bsw_zone[merged_obj_id] = mature_count;
   bsw_persistent.mature_count_in_bsw_zone[new_obj_id]    = 0u;
   object_data[merged_obj_id].id                          = merged_id;
   object_data[new_obj_id].f_merge_occured                = FBK_TRUE;
   object_data[new_obj_id].id_merged_obj                  = 255u;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Bsw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Bsw_Data_For_Merged_Objects(&bsw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[new_obj_id], FBK_ZERO_F);
}

/*
 * Check that mature_count_in_bsw_zone is set correctly while mature count is too high
 * \uts{CSCSA-45660} \sdd{SF-6658} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Bsw_Data_For_Merged_Objects__high_mature_count)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_id    = 3u;
   uint8_t merged_obj_id = 12u;
   uint8_t merged_id     = 7u;
   uint8_t mature_count  = 5u;

   bsw_persistent.mature_count_in_bsw_zone[merged_obj_id] = mature_count;
   bsw_persistent.mature_count_in_bsw_zone[new_obj_id]    = 10u;
   object_data[merged_obj_id].id                          = merged_id;
   object_data[new_obj_id].f_merge_occured                = FBK_TRUE;
   object_data[new_obj_id].id_merged_obj                  = merged_id;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Bsw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Bsw_Data_For_Merged_Objects(&bsw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_FALSE(Fbk_Equal_F(bsw_persistent.mature_count_in_bsw_zone[new_obj_id], FBK_ZERO_F));
}

/**
 * Check that ego side is correctly taken from vcs coordinates
 * \uts{CSCSA-45661} \sdd{SF-6667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Process_Bsw_Object__vcs_system)
{
   /** \arrange Set up target object and host vehicle state. */
   const uint8_t id_track_inside_bsw_zone = 20;
   const float32_T track_long_pos         = -8.0f;
   const float32_T track_lat_pos          = 2.0f;

   // Set the cal to take the zone from the lcda_core_input which is set to the default zone
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_min_mature_cycles         = 1;                // so that BSW alert is
                                                                 // given when the object
                                                                 // is in the zone even for
                                                                 // 1 cycle
   lcda_cals.k_lcda_ego_lane_check_center_point_only = FBK_TRUE; // Use the simple center point check for ego lane occupation
   lcda_cals.k_bsw_use_curvi_coordinates             = FBK_FALSE;

   lcda_cals.k_bsw_overlap_area_check_enable                      = FBK_TRUE;
   lcda_cals.k_bsw_overlap_area_threshold                         = 0.0f;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_FALSE;

   // Track on right of the ego, inside the default BSW zone - long dist = -8m and lat dist = 2m
   Lcda_Create_Bsw_Track(&tracker_object, id_track_inside_bsw_zone, track_long_pos, track_lat_pos);

   // Set qualification counter
   bsw_persistent.fallback_fast_to_slow_qual_ctr[id_track_inside_bsw_zone] = lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres + 1u;

   /** \action Call Lcda_Process_Bsw_Object to compute the BSW alert state. */
   Lcda_Process_Bsw_Object(&bsw_core_output, &tracker_object, &lcda_core_input, &lcda_cals, &lcda_persistent, &bsw_persistent,
                           &cvw_persistent);

   /** \assert Check that the correct target object is warned and that the alert state level is LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_id[FBK_SIDE_RIGHT], tracker_object.id);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT], id_track_inside_bsw_zone);
   EXPECT_EQ(bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT], tracker_object.unique_id);
}

/*
 * Check that object is classified correctly for different track object status values.
 * \uts{CSCSA-45662} \sdd{SF-6653} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Relevant_For_Bsw__track_status_test)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status mature. */
   boolean_T result1 = FBK_FALSE;
   boolean_T result2 = FBK_FALSE;
   boolean_T result3 = FBK_FALSE;
   boolean_T result4 = FBK_TRUE;

   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.vcs_heading           = lcda_cals.k_bsw_max_heading_abs - EPSILON;
   tracker_object.vcs_vel.x             = lcda_cals.k_bsw_min_obj_long_vel + EPSILON;
   tracker_object.existence_probability = 1.0f;

   /** \action Call function Lcda_Is_Object_Relevant_For_Bsw to evaluate if object is relevant. */
   result1 = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   tracker_object.status = PA_OBJ_STATUS_COASTED;
   result2               = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   tracker_object.status = PA_OBJ_STATUS_INVALID;
   result3               = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   tracker_object.existence_probability = 0.5f;
   tracker_object.status                = PA_OBJ_STATUS_INVALID;
   result4 = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);


   /** \assert Verify that object is classified as relevant. */
   EXPECT_TRUE(result1);
   EXPECT_TRUE(result2);
   EXPECT_FALSE(result3);
   EXPECT_FALSE(result4);
}

/*
 * Check that object is classified correctly for different previus alert combinations.
 * \uts{CSCSA-45663} \sdd{SF-6653} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Relevant_For_Bsw__previus_alert_on_new_track)
{
   /** \arrange Set up tracker object such that all criteria for relevance are passed with object status mature. */
   boolean_T result1                                    = FBK_FALSE;
   boolean_T result2                                    = FBK_FALSE;
   boolean_T result3                                    = FBK_FALSE;
   boolean_T result4                                    = FBK_FALSE;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_RIGHT] = 1u;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT]  = 1u;
   tracker_object.vcs_heading                           = lcda_cals.k_bsw_max_heading_abs - EPSILON;
   tracker_object.vcs_vel.x                             = lcda_cals.k_bsw_min_obj_long_vel + EPSILON;
   tracker_object.existence_probability                 = 1.0f;
   tracker_object.curvi_pos.y                           = 2.0f;


   /** \action Call function Lcda_Is_Object_Relevant_For_Bsw to evaluate if object is relevant. */
   tracker_object.status                   = PA_OBJ_STATUS_MATURE;
   lcda_cals.k_lcda_allow_track_status_new = FBK_TRUE;
   result1 = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   tracker_object.status                   = PA_OBJ_STATUS_COASTED;
   lcda_cals.k_lcda_allow_track_status_new = FBK_FALSE;
   result2 = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   lcda_cals.k_lcda_allow_track_status_new = FBK_TRUE;
   tracker_object.status                   = PA_OBJ_STATUS_NEW;
   result3 = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_RIGHT] = 0u;
   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT]  = 0u;
   lcda_cals.k_lcda_allow_track_status_new              = FBK_TRUE;
   tracker_object.status                                = PA_OBJ_STATUS_NEW;
   result4 = Lcda_Is_Object_Relevant_For_Bsw(&lcda_core_input, &tracker_object, &lcda_cals, &bsw_persistent);

   /** \assert Verify that object is classified as relevant. */
   EXPECT_TRUE(result1);
   EXPECT_TRUE(result2);
   EXPECT_TRUE(result3);
   EXPECT_FALSE(result4);
}

/*
 * Check that error is thrown while fallback state is wrong
 * \uts{CSCSA-45664} \sdd{SF-6659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Fallback_State__wrong_fallback_state)
{
   /** \arrange Check fallback handlers transistion from fast to slow. */
   tracker_object.id              = 1;
   tracker_object.curvi_vel_rel.x = lcda_cals.k_bsw_fallback_rel_vel_thres + EPSILON;

   lcda_core_input.enabled_flags.f_fallback_enabled             = FBK_TRUE;
   bsw_persistent.fallback_state[bsw_object.p_tracker_data->id] = (Lcda_Fallback_State_T) 10u;

   lcda_cals.k_bsw_f_fallback_default_status_slow   = FBK_FALSE;
   lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres = 2u;
   bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id] =
      lcda_cals.k_bsw_fallback_fast_to_slow_qual_thres - 1u;

   /** \action Call function Lcda_Update_Fallback_State to fallback state. */

   /** \assert Calling function should cause error */
   EXPECT_DEBUG_DEATH({ Lcda_Update_Fallback_State(&bsw_persistent, &bsw_object, &lcda_cals, &lcda_core_input); }, "");
}

/*
 * Check whether transition to slow fallback is held if velocity is over threshold
 * \uts{CSCSA-45665} \sdd{SF-6659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Fallback_State__fallback_fast_velocity_hugh)
{
   /** \arrange Check fallback handlers transistion from fast to slow. */
   tracker_object.id              = 1;
   tracker_object.curvi_vel_rel.x = lcda_cals.k_bsw_fallback_rel_vel_thres + lcda_cals.k_bsw_fallback_rel_vel_thres_hys + EPSILON;

   lcda_core_input.enabled_flags.f_fallback_enabled             = FBK_TRUE;
   bsw_persistent.fallback_state[bsw_object.p_tracker_data->id] = FALLBACK_SLOW;

   lcda_cals.k_bsw_f_fallback_default_status_slow = FBK_FALSE;


   /** \action Call function Lcda_Update_Fallback_State to fallback state. */
   Lcda_Update_Fallback_State(&bsw_persistent, &bsw_object, &lcda_cals, &lcda_core_input);

   /** \assert fallback state stays the same and qualification counter increased. */
   EXPECT_EQ(bsw_persistent.fallback_state[bsw_object.p_tracker_data->id], FALLBACK_SLOW);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id], FBK_ZERO_UINT);
}

/*
 * Check whether default fallback status of new object is changed from FAST to SLOW when such functionality is activated.
 * \uts{CSCSA-140104} \sdd{SF-6659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Fallback_State__default_fallback_status_set_as_slow)
{
   /** \arrange 0 - Arrange */
   tracker_object.id              = 2;
   tracker_object.curvi_vel_rel.x = lcda_cals.k_bsw_fallback_rel_vel_thres;

   lcda_core_input.enabled_flags.f_fallback_enabled                             = FBK_TRUE;
   bsw_persistent.fallback_state[bsw_object.p_tracker_data->id]                 = FALLBACK_FAST;
   bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id] = 3u;

   lcda_cals.k_bsw_f_fallback_default_status_slow = FBK_TRUE;

   /** \action Call function Lcda_Update_Fallback_State to update the fallback state. */
   Lcda_Update_Fallback_State(&bsw_persistent, &bsw_object, (&lcda_cals), &lcda_core_input);

   /** \assert Check that fallback state is updated correctly. */
   EXPECT_EQ(bsw_persistent.fallback_state[bsw_object.p_tracker_data->id], FALLBACK_SLOW);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id], 3u);
}

/*
 * Check if error is thrown while providing wrong calibration
 * \uts{CSCSA-45666} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__wrong_calibration)
{
   /** \arrange Set up object data so that object crossed front limit but relative velocity is negative. */
   tracker_object.vcs_vel_rel.x  = -3.0f;
   bsw_object.obj_front_position = 1.0f;

   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode    = 10u;

   /** \action */

   /** \assert Verify */
   EXPECT_DEBUG_DEATH({ Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals); }, "");
}

/*
 * Test if reaching front detection works while holding alert is on and object is long
 * \uts{CSCSA-45667} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__hold_for_long_obj)
{
   /** \arrange Set up object data so that only a part of a long object crossed the front limit. */
   boolean_T result;
   tracker_object.vcs_vel_rel.x                                   = 3.0f;
   bsw_object.obj_front_position                                  = 1.0f;
   bsw_object.f_obj_long                                          = FBK_TRUE;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode    = 1u;
   lcda_cals.k_bsw_hold_alert_long_object                         = FBK_TRUE;


   /** \action */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that warning shall still be relevant. */
   EXPECT_TRUE(result);
}

/*
 * Test if position compensation trigger alert suppresion
 * \uts{CSCSA-185794} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__compensation_suppress_alert)
{
   /** \arrange Set up object data so that only a part of a long object crossed the front limit. */
   boolean_T result;
   tracker_object.vcs_vel_rel.x                                   = 15.0f;
   bsw_object.obj_front_position                                  = -0.00f;
   lcda_cals.k_bsw_enable_zone_front_boundary_specific_conditions = FBK_TRUE;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode    = COMPENSATED;


   /** \action */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that warning shall be not relevant. */
   EXPECT_FALSE(result);
}

/*
 * Test if reaching front detection works while holding alert is on and object is short
 * \uts{CSCSA-45668} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__crossed_front_negative_velocity)
{
   /** \arrange Set up object data so that object crossed front limit but relative velocity is negative. */
   boolean_T result;
   tracker_object.vcs_vel_rel.x                                = 3.0f;
   bsw_object.obj_front_position                               = 1.0f;
   bsw_object.f_obj_long                                       = FBK_FALSE;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode = 1u;
   lcda_cals.k_bsw_hold_alert_long_object                      = FBK_TRUE;

   /** \action */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that warning shall be not relevant. */
   EXPECT_FALSE(result);
}

/*
 * Checks that target was causing alert in previous cycle and has length greater than minimum length required to be considered as a
 * long object. Expect false beacuse wrong calibration. \uts{CSCSA-45669} \sdd{CSCSA-70157} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Object_Considered_Long__returns_false_if_object_above_threshold_and_length_below_calibration)
{
   /** \arrange Set up object data so that object triggered alert before and is longer than threshold. */
   boolean_T result = FBK_FALSE;

   uint8_t obj_id = 2u;

   tracker_object.id     = obj_id;
   tracker_object.length = 6.5f;

   bsw_persistent.prev_bsw_alert_obj_id[FBK_SIDE_LEFT] = obj_id;
   bsw_persistent.f_prev_long_truck_status[obj_id]     = FBK_TRUE;

   lcda_cals.k_bsw_min_length_long_object     = 7.0f;
   lcda_cals.k_bsw_min_length_long_object_hys = 0.2f;

   /** \action Call function Lcda_Is_Object_Considered_Long to evaluate what type of object is considered. */
   result = Lcda_Is_Object_Considered_Long(&bsw_persistent, &bsw_object, &lcda_cals);

   /** \assert Verify that object should be considered as a long object. */
   EXPECT_FALSE(result);
}

/*
 * Test for Lcda_Set_Bsw_Alert_Flag, Alert flag is set for defined flag variation, bsw object and coordinate system .
 * \uts{CSCSA-45670} \sdd{CSCSA-27684} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Bsw_Alert_Flag__flat_variation)
{

   /** \arrange Set up inputs for Lcda_Set_Bsw_Alert_Flag: (condition_flags), bsw_object, p_lcda_cals, coordinate_system, */
   boolean_T f_alert_valid;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_CURVI;
   boolean_T f_warning_active                 = FBK_TRUE;
   boolean_T f_count_check_passed             = FBK_TRUE;
   boolean_T f_fallback_warning_in_time       = FBK_TRUE;
   boolean_T f_obj_in_ego_lane                = FBK_TRUE;
   boolean_T f_obj_in_environment_conflict    = FBK_TRUE;
   boolean_T f_fallback_check_passed          = FBK_TRUE;
   boolean_T f_obj_reached_front_custom_limit = FBK_TRUE;
   boolean_T f_obj_overlap_below_threshold    = FBK_TRUE;
   boolean_T f_obj_below_max_rel_vel          = FBK_TRUE;

   /** \action Call function Lcda_Set_Bsw_Alert_Flag */
   f_alert_valid = Lcda_Set_Bsw_Alert_Flag(&lcda_core_input, f_warning_active, f_count_check_passed, f_fallback_warning_in_time,
                                           f_obj_in_ego_lane, f_obj_in_environment_conflict, f_fallback_check_passed,
                                           f_obj_reached_front_custom_limit, f_obj_overlap_below_threshold,
                                           f_obj_below_max_rel_vel, &bsw_object, p_vehicle_data, &lcda_cals, coordinate_system);

   /** \assert Verify f_alert_valid */
   EXPECT_FLOAT_EQ(f_alert_valid, FBK_FALSE);
}


/**
 * Check that object longitudinal relative velocity is verified correctly. Object has long_rel_vel above threshold and didn't
 * trigger alert in previous cycle \uts{CSCSA-116581} \sdd{CSCSA-116577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Flyby_Criteria_Passed__is_false_when_obj_above_thresh_and_no_alert_prev_cycle)
{
   /** \arrange Provide basic object data. */
   boolean_T vel_below_thresh;
   boolean_T f_prev_cycle_warning = FBK_FALSE;
   float32_T obj_long_rel_vel     = 16.0f;

   lcda_cals.k_bsw_obj_max_rel_vel_thresh = 15.0f;

   /** \action Call Lcda_Is_Flyby_Criteria_Passed to check if rel_vel below thresh. */
   vel_below_thresh = Lcda_Is_Flyby_Criteria_Passed(f_prev_cycle_warning, obj_long_rel_vel, (&lcda_cals));

   /** \assert Check that object is above velocity upper limit. */
   EXPECT_FALSE(vel_below_thresh);
}


/**
 * Check that object longitudinal relative velocity is verified correctly. Object has long_rel_vel below threshold and didn't
 * trigger alert in previous cycle \uts{CSCSA-116582} \sdd{CSCSA-116577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Flyby_Criteria_Passed__is_true_when_obj_below_thresh_and_no_alert_prev_cycle)
{
   /** \arrange Provide basic object data. */
   boolean_T vel_below_thresh;
   boolean_T f_prev_cycle_warning = FBK_FALSE;
   float32_T obj_long_rel_vel     = 14.0f;

   lcda_cals.k_bsw_obj_max_rel_vel_thresh = 15.0f;

   /** \action Call Lcda_Is_Flyby_Criteria_Passed to check if rel_vel below thresh. */
   vel_below_thresh = Lcda_Is_Flyby_Criteria_Passed(f_prev_cycle_warning, obj_long_rel_vel, (&lcda_cals));

   /** \assert Check that object is below velocity upper limit. */
   EXPECT_TRUE(vel_below_thresh);
}

/**
 * Check that object longitudinal relative velocity is verified correctly. Object has long_rel_vel above threshold, below
 * hysteresis value and triggered alert in previous cycle \uts{CSCSA-116583} \sdd{CSCSA-116577} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Flyby_Criteria_Passed__is_true_when_obj_below_thresh_and_alert_prev_cycle)
{
   /** \arrange Provide basic object data. */
   boolean_T vel_below_thresh;
   boolean_T f_prev_cycle_warning = FBK_TRUE;
   float32_T obj_long_rel_vel     = 16.0f;

   lcda_cals.k_bsw_obj_max_rel_vel_thresh = 15.0f;

   /** \action Call Lcda_Is_Flyby_Criteria_Passed to check if rel_vel below thresh. */
   vel_below_thresh = Lcda_Is_Flyby_Criteria_Passed(f_prev_cycle_warning, obj_long_rel_vel, (&lcda_cals));

   /** \assert Check that object is below velocity upper limit. */
   EXPECT_TRUE(vel_below_thresh);
}

/**
 * Test if zone check for long objects works correctly. Here object's rear bumper is behind n line (ego rear bumper), thus alert
 * shall not be triggered. \uts{CSCSA-120213} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__sot_long_obj_behind_rear_bumper)
{
   /** \arrange Set up long object with rear bumper behind ego n line */
   boolean_T result;

   bsw_object.f_obj_long         = FBK_TRUE;
   bsw_object.obj_front_position = FBK_ZERO_F;
   tracker_object.vcs_vel_rel.x  = -FBK_ONE_F;

   lcda_cals.k_bsw_n_line_position_for_long_object_sot_scenario = bsw_object.obj_front_position - tracker_object.length - EPSILON;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions         = FBK_TRUE;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert will not be triggered */
   EXPECT_FALSE(result);
}

/**
 * Test if zone check for long objects works correctly. Here object's rear bumper is after n line (ego rear bumper), thus expect
 * alert to be triggered \uts{CSCSA-120214} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__sot_long_obj_after_rear_bumper)
{
   /** \arrange Set up long object with rear bumper behind ego n line */
   boolean_T result;

   bsw_object.f_obj_long         = FBK_TRUE;
   bsw_object.obj_front_position = FBK_ZERO_F;
   tracker_object.vcs_vel_rel.x  = -FBK_ONE_F;

   lcda_cals.k_bsw_f_use_front_zone_as_n_line                   = FBK_FALSE;
   lcda_cals.k_bsw_n_line_position_for_long_object_sot_scenario = bsw_object.obj_front_position - tracker_object.length + EPSILON;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions         = FBK_TRUE;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert will be triggered */
   EXPECT_TRUE(result);
}


/**
 * Test if zone check for long objects works correctly. Here n line is set to the same value as bsw zone front line. Object's rear
 * bumper is behind n line, thus expect alert to be triggered \uts{CSCSA-247739} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__sot_long_obj_n_line_equal_bsw_zone_front_alert)
{
   /** \arrange Set up long object with rear bumper behind ego n line */
   boolean_T result;

   bsw_object.f_obj_long         = FBK_TRUE;
   bsw_object.obj_front_position = FBK_ONE_F;
   tracker_object.vcs_vel_rel.x  = -FBK_ONE_F;
   tracker_object.length         = 5.0f;

   lcda_cals.k_bsw_f_use_front_zone_as_n_line           = FBK_TRUE;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions = FBK_TRUE;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert will be triggered */
   EXPECT_TRUE(result);
}

/**
 * Test if zone check for long objects works correctly. Here n line is set to the same value as bsw zone front line. Object's rear
 * bumper is in front of n line, thus expect alert to be not triggered \uts{CSCSA-247740} \sdd{CSCSA-70156}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__sot_long_obj_n_line_equal_bsw_zone_front_no_alert)
{
   /** \arrange Set up long object with rear bumper in front of ego n line */
   boolean_T result;

   bsw_object.f_obj_long         = FBK_TRUE;
   bsw_object.obj_front_position = 6.0f;
   tracker_object.vcs_vel_rel.x  = -FBK_ONE_F;
   tracker_object.length         = 5.0f;

   lcda_cals.k_bsw_f_use_front_zone_as_n_line           = FBK_TRUE;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions = FBK_TRUE;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert will not be triggered */
   EXPECT_FALSE(result);
}


/**
 * Test if zone check for the custom line objects works correctly. Alert shall not be triggered
 * \uts{CSCSA-127631} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__tos_reach_custom_line_no_alert)
{
   /** \arrange Set up long object with rear bumper behind ego n line */
   boolean_T result;

   bsw_object.obj_front_position                               = lcda_core_input.initial_bsw_zone.points[0].x - 1.0f;
   tracker_object.vcs_vel_rel.x                                = FBK_ONE_F;
   lcda_cals.k_bsw_line_to_stop_TOS_alert                      = lcda_core_input.initial_bsw_zone.points[0].x - 2.0f;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode = CUSTOM;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert is not triggered */
   EXPECT_FALSE(result);
}

/**
 * Test if zone check for the custom line objects works correctly. Alert shall triggered
 * \uts{CSCSA-127632} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__tos_not_reach_custom_line_alert_triggered)
{
   /** \arrange Set up posiotion of object behaind custom line */
   boolean_T result;

   bsw_object.obj_front_position                               = -1.0f;
   tracker_object.vcs_vel_rel.x                                = FBK_ONE_F;
   lcda_cals.k_bsw_line_to_stop_TOS_alert                      = -0.5f;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode = CUSTOM;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert will be triggered */
   EXPECT_TRUE(result);
}

/**
 * Test if zone check for the custom line objects works correctly. Here previus alert was ON. Alert shall be triggered
 * \uts{CSCSA-187416} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__tos_compensated_prev_alert_on_alert_triggered)
{
   /** \arrange Set up posiotion of object behaind custom line */
   boolean_T result;
   bsw_persistent.f_prev_bsw_active[FBK_SIDE_RIGHT]            = FBK_TRUE;
   bsw_object.obj_front_position                               = -FBK_ONE_F;
   bsw_object.ego_side                                         = FBK_SIDE_RIGHT;
   tracker_object.vcs_vel_rel.x                                = FBK_ONE_F;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode = COMPENSATED;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert will be triggered */
   EXPECT_TRUE(result);
}

/**
 * Test if sot scenario is not enabled for positive relative speed.
 * \uts{CSCSA-187417} \sdd{CSCSA-70156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Zone_Front_Boundary_Specific_Conditions__sot_positive_speed)
{
   /** \arrange Set up posiotion of object behaind custom line */
   boolean_T result;
   bsw_object.obj_front_position                               = -FBK_ONE_F;
   tracker_object.vcs_vel_rel.x                                = FBK_ONE_F;
   lcda_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode = HOST_FRONT;
   lcda_cals.k_bsw_enable_specific_front_sot_conditions        = 1u;

   /** \action Call function Lcda_Zone_Front_Boundary_Specific_Conditions */
   result = Lcda_Zone_Front_Boundary_Specific_Conditions(&bsw_object, &lcda_cals);

   /** \assert Verify that alert will be triggered */
   EXPECT_TRUE(result);
}

/*
 * Test for Lcda_Set_Bsw_Alert_Flag, enable alert in lane by calibration.
 * \uts{CSCSA-187418} \sdd{CSCSA-27684} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Bsw_Alert_Flag__enable_alert_in_lane)
{

   /** \arrange Set up inputs for Lcda_Set_Bsw_Alert_Flag: (condition_flags), bsw_object, p_lcda_cals, coordinate_system, */
   boolean_T f_alert_valid;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_CURVI;
   boolean_T f_warning_active                 = FBK_TRUE;
   boolean_T f_count_check_passed             = FBK_TRUE;
   boolean_T f_fallback_warning_in_time       = FBK_TRUE;
   boolean_T f_obj_in_ego_lane                = FBK_TRUE;
   boolean_T f_obj_in_environment_conflict    = FBK_TRUE;
   boolean_T f_fallback_check_passed          = FBK_TRUE;
   boolean_T f_obj_reached_front_custom_limit = FBK_TRUE;
   boolean_T f_obj_overlap_below_threshold    = FBK_TRUE;
   boolean_T f_obj_below_max_rel_vel          = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_alert_obj_in_ego_lane = FBK_TRUE;

   /** \action Call function Lcda_Set_Bsw_Alert_Flag */
   f_alert_valid = Lcda_Set_Bsw_Alert_Flag(&lcda_core_input, f_warning_active, f_count_check_passed, f_fallback_warning_in_time,
                                           f_obj_in_ego_lane, f_obj_in_environment_conflict, f_fallback_check_passed,
                                           f_obj_reached_front_custom_limit, f_obj_overlap_below_threshold,
                                           f_obj_below_max_rel_vel, &bsw_object, p_vehicle_data, &lcda_cals, coordinate_system);

   /** \assert Verify f_alert_valid */
   EXPECT_FLOAT_EQ(f_alert_valid, FBK_FALSE);
}


/**
 * Set BSW alert for target object on right side with mature BSW zone counter greater than zero. Set that no previous BSW warning
 * was active. Create BSW zone and check that it equals the default BSW zone size. \uts{CSCSA-211387} \sdd{SF-6647}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Create_Bsw_Object_Zone__prev_bsw_and_lane_change_return_false)
{
   /** \arrange Set up target object and BSW alert state. */
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 4;
   uint8_t point;

   tracker_object.id   = obj_id;
   bsw_object.ego_side = side;

   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 1u;
   bsw_persistent.f_prev_bsw_active[side]          = FBK_FALSE;

   cvw_persistent.f_prev_cvw_active[side]     = FBK_TRUE;
   cvw_persistent.prev_cvw_alert_obj_id[side] = obj_id;


   lcda_core_input.warn_settings.bsw_len_factor                = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled            = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone                        = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone             = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode                   = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_core_input.f_lane_change[Lcda_Get_Opposite_Side(side)] = FBK_TRUE;

   /** \action Call Lcda_Create_Bsw_Object_Zone to create bsw zone. */
   Lcda_Create_Bsw_Object_Zone(&bsw_object, &bsw_persistent, &lcda_core_input, &lcda_cals, &cvw_persistent);

   /** \assert Check that bsw zone and the default bsw zone have same size. */
   for (point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(bsw_object.zone.points[point].x, default_bsw_zone.points[point].x);
      ASSERT_EQ(bsw_object.zone.points[point].y, default_bsw_zone.points[point].y);
   }
}


/**
 * Set BSW alert for target object on right side with mature BSW zone counter greater than zero. Set that no previous BSW warning
 * was active. Create BSW zone and check that it equals the default BSW zone size. \uts{CSCSA-211388} \sdd{SF-6647}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Create_Bsw_Object_Zone__lane_change_return_false)
{
   /** \arrange Set up target object and BSW alert state. */
   uint8_t side   = FBK_SIDE_RIGHT;
   uint8_t obj_id = 4;
   uint8_t point;

   tracker_object.id   = obj_id;
   bsw_object.ego_side = side;

   bsw_persistent.mature_count_in_bsw_zone[obj_id] = 1u;
   bsw_persistent.f_prev_bsw_active[side]          = FBK_TRUE;

   cvw_persistent.f_prev_cvw_active[side]     = FBK_TRUE;
   cvw_persistent.prev_cvw_alert_obj_id[side] = obj_id;


   lcda_core_input.warn_settings.bsw_len_factor                = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled            = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone                        = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone             = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode                   = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_core_input.f_lane_change[Lcda_Get_Opposite_Side(side)] = FBK_TRUE;

   /** \action Call Lcda_Create_Bsw_Object_Zone to create bsw zone. */
   Lcda_Create_Bsw_Object_Zone(&bsw_object, &bsw_persistent, &lcda_core_input, &lcda_cals, &cvw_persistent);

   /** \assert Check that bsw zone and the default bsw zone have same size. */
   for (point = 0; point < LCDA_NUMBER_OF_ZONE_POINTS; point++)
   {
      ASSERT_EQ(bsw_object.zone.points[point].x, default_bsw_zone.points[point].x);
      ASSERT_EQ(bsw_object.zone.points[point].y, default_bsw_zone.points[point].y);
   }
}


/*
 * Check whether default fallback status of new object is changed from FAST to SLOW when such functionality is activated.
 * \uts{CSCSA-211389} \sdd{SF-6659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Fallback_State__default_fallback_status_object_age_above_thresh)
{
   /** \arrange 0 - Arrange */
   tracker_object.id              = 2;
   tracker_object.curvi_vel_rel.x = lcda_cals.k_bsw_fallback_rel_vel_thres;
   tracker_object.age             = 3;

   lcda_core_input.enabled_flags.f_fallback_enabled                             = FBK_TRUE;
   bsw_persistent.fallback_state[bsw_object.p_tracker_data->id]                 = FALLBACK_FAST;
   bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id] = 3u;

   lcda_cals.k_bsw_f_fallback_default_status_slow = FBK_TRUE;

   /** \action Call function Lcda_Update_Fallback_State to update the fallback state. */
   Lcda_Update_Fallback_State(&bsw_persistent, &bsw_object, (&lcda_cals), &lcda_core_input);

   /** \assert Check that fallback state is updated correctly. */
   EXPECT_EQ(bsw_persistent.fallback_state[bsw_object.p_tracker_data->id], FALLBACK_FAST);
   EXPECT_EQ(bsw_persistent.fallback_fast_to_slow_qual_ctr[bsw_object.p_tracker_data->id], 3u);
}

/*
 * Create a back falling track which could be warning relevant. Check if the fallback warning for LCDA is in time. Due to very high
 * time to leave bsw zone alert won;t be suppressed be suppressed. \uts{CSCSA-211390} \sdd{SF-6651}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Fallback_Warning_In_Time__distant_object_is_not_suppressed_time_to_leave_too_high)
{
   /** \arrange Set up distant bsw object and a short BSW warning zone (both longitudinal). */
   Vector_2d_T zone_point;
   float32_T host_length = 5.0f;
   boolean_T result      = FBK_FALSE;
   zone_point.x          = -10;

   bsw_object.p_tracker_data = &tracker_object;

   bsw_object.zone.size      = 3u;
   bsw_object.zone.points[1] = zone_point;

   tracker_object.vcs_vel_rel.x = lcda_cals.k_bsw_suppress_late_warning_max_rel_vel - EPSILON;
   tracker_object.vcs_pos.x     = -40;

   lcda_cals.k_bsw_suppress_late_warning_max_time_till_leave = -20.0f;
   /** \action Call function Lcda_Is_Fallback_Warning_In_Time to evalute if fallback warning is in time. */
   result = Lcda_Is_Fallback_Warning_In_Time(&bsw_object, host_length, &lcda_cals);

   /** \assert Verify that warning shall no longer be relevant. */
   EXPECT_TRUE(result);
}


/*
 * Create a back falling track which could be warning relevant. Check if the fallback warning for LCDA is in time. Due to very low
 * rel vel alert won't be suppressed. \uts{CSCSA-211391} \sdd{SF-6651} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Fallback_Warning_In_Time__distant_object_is_not_suppressed_rel_vel_almost_zero)
{
   /** \arrange Set up distant bsw object and a short BSW warning zone (both longitudinal). */
   Vector_2d_T zone_point;
   float32_T host_length = 5.0f;
   boolean_T result      = FBK_FALSE;
   zone_point.x          = -10;

   bsw_object.p_tracker_data = &tracker_object;

   bsw_object.zone.size      = 3u;
   bsw_object.zone.points[1] = zone_point;

   tracker_object.vcs_vel_rel.x = (float32_T) 1E-12;
   tracker_object.vcs_pos.x     = -40;

   lcda_cals.k_bsw_suppress_late_warning_max_rel_vel = (float32_T) 1E-10;
   /** \action Call function Lcda_Is_Fallback_Warning_In_Time to evalute if fallback warning is in time. */
   result = Lcda_Is_Fallback_Warning_In_Time(&bsw_object, host_length, &lcda_cals);

   /** \assert Verify that warning shall no longer be relevant. */
   EXPECT_TRUE(result);
}

/*
 * Check that after an object merge the mature count in zone of the merged object is not taken over by the new object.
 * \uts{CSCSA-211392} \sdd{SF-6658} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Bsw_Data_For_Merged_Objects__invalid_merged_object_id)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_index    = 3u;
   uint8_t new_obj_id       = 4u;
   uint8_t merged_obj_index = 12u;
   uint8_t merged_id        = 7u;
   uint8_t mature_count     = 5u;

   bsw_persistent.mature_count_in_bsw_zone[merged_id]  = mature_count;
   bsw_persistent.mature_count_in_bsw_zone[new_obj_id] = 0u;
   object_data[merged_obj_index].id                    = merged_id;
   object_data[new_obj_index].id                       = new_obj_id;
   object_data[new_obj_index].f_merge_occured          = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj            = PA_INVALID_OBJ_ID;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Bsw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Bsw_Data_For_Merged_Objects(&bsw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[new_obj_id], 0u);
}

/*
 * Check that after an object merge the mature count in zone of the merged object is not taken over by the new object.
 * \uts{CSCSA-211393} \sdd{SF-6658} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Update_Bsw_Data_For_Merged_Objects__merge_object_mature_count_less_than_new_object)
{
   /** \arrange Set up persistent data with mature in zone counts and tracker data with merge information. */
   uint8_t new_obj_index    = 3u;
   uint8_t new_obj_id       = 4u;
   uint8_t merged_obj_index = 12u;
   uint8_t merged_id        = 7u;
   uint8_t mature_count     = 5u;

   bsw_persistent.mature_count_in_bsw_zone[merged_id]  = mature_count;
   bsw_persistent.mature_count_in_bsw_zone[new_obj_id] = 10u;
   object_data[merged_obj_index].id                    = merged_id;
   object_data[new_obj_index].id                       = new_obj_id;
   object_data[new_obj_index].f_merge_occured          = FBK_TRUE;
   object_data[new_obj_index].id_merged_obj            = 1u;

   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   /** \action Call function Lcda_Update_Bsw_Data_For_Merged_Objects to update mature in zone counter for merged objects. */
   Lcda_Update_Bsw_Data_For_Merged_Objects(&bsw_persistent, &fbk_output);

   /** \assert Verify that mature in zone count is taken over to new object. */
   EXPECT_EQ(bsw_persistent.mature_count_in_bsw_zone[new_obj_id], 10u);
}


/*
 * Check if f_alert_valid status will change when all conditions true except f_alert_valid.
 * \uts{CSCSA-211394} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__alert_valid_is_false)
{
   /** \arrange Set up data so that object lane change intention check enabled. */

   boolean_T f_alert_valid     = FBK_FALSE;
   boolean_T f_warning_active  = FBK_TRUE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;

   tracker_object.vcs_pos.y     = -1.0f;
   tracker_object.vcs_vel_rel.y = -1.0f;

   tracker_object.curvi_pos.y     = -1.0f;
   tracker_object.curvi_vel_rel.y = -1.0f;

   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(
      &lcda_core_input, f_alert_valid, f_warning_active, f_obj_in_ego_lane, &lcda_cals, &bsw_object, p_vehicle_data, LCDA_USE_VCS);

   /** \assert Verify that alert is not updated. */
   EXPECT_FALSE(f_alert_valid);
}


/*
 * Check if alert is suppressed when object change lane intention advanced logic is enabled and target front position is positive
 * \uts{CSCSA-211395} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_suppress_front_position_positive)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;
   lcda_cals.k_bsw_f_use_zone_without_hysteresis_lane_change_intention      = FBK_FALSE;

   tracker_object.vcs_pos.y     = 0.5f;
   tracker_object.vcs_vel_rel.y = 0.3f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position           = 2.0f;
   bsw_object.zone.points[REAR_EGO_SIDE].y = -2.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(&lcda_core_input, f_alert_valid, f_warning_active,
                                                                    f_obj_in_ego_lane, (&lcda_cals), &bsw_object, p_vehicle_data,
                                                                    LCDA_USE_VCS);

   /** \assert Verify that alert is classified as invalid. */
   EXPECT_FALSE(f_alert_valid);
}


/*
 * Check if alert is suppressed when object change lane intention advanced logic is enabled and zone without hysteresis is picked
 * \uts{CSCSA-211396} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_vel_lat_above_thresh_false)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;
   lcda_cals.k_bsw_f_use_zone_without_hysteresis_lane_change_intention      = FBK_TRUE;
   lcda_cals.k_lcda_lane_change_intention_vel_lat_thresh                    = 0.1f;

   tracker_object.vcs_pos.y     = 0.5f;
   tracker_object.vcs_vel_rel.y = 0.3f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position           = -10.0f;
   bsw_object.zone.points[REAR_EGO_SIDE].y = -2.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(&lcda_core_input, f_alert_valid, f_warning_active,
                                                                    f_obj_in_ego_lane, (&lcda_cals), &bsw_object, p_vehicle_data,
                                                                    LCDA_USE_VCS);

   /** \assert Verify that alert is classified as invalid. */
   EXPECT_TRUE(f_alert_valid);
}

/*
 * Check if alert is suppressed when object change lane intention advanced logic is enabled and zone without hysteresis is picked
 * \uts{CSCSA-211397} \sdd{CSCSA-39777} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Suppress_Alert_Object_Lane_Change_Intention__extended_check_lat_overlap_above_thresh_true)
{
   /** \arrange Set up data so all requirements fullfilled to keep alert valid and use VCS coordinate system. */

   boolean_T f_alert_valid     = FBK_TRUE;
   boolean_T f_warning_active  = FBK_FALSE;
   boolean_T f_obj_in_ego_lane = FBK_TRUE;

   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention              = FBK_TRUE;
   lcda_cals.k_bsw_f_use_zone_without_hysteresis_lane_change_intention      = FBK_TRUE;
   lcda_cals.k_bsw_lane_change_intention_pos_lat_thres                      = -1.0f;

   tracker_object.vcs_pos.y     = 0.5f;
   tracker_object.vcs_vel_rel.y = 0.3f;


   Lcda_Create_Valid_Bsw_Cvw_Track(&tracker_object, FBK_ZERO_INT);

   bsw_object.obj_front_position           = -10.0f;
   bsw_object.zone.points[REAR_EGO_SIDE].y = -2.0f;

   /** \action Call function to update f_alert_valid status. */

   f_alert_valid = Lcda_Suppress_Alert_Object_Lane_Change_Intention(&lcda_core_input, f_alert_valid, f_warning_active,
                                                                    f_obj_in_ego_lane, (&lcda_cals), &bsw_object, p_vehicle_data,
                                                                    LCDA_USE_VCS);

   /** \assert Verify that alert is classified as invalid. */
   EXPECT_FALSE(f_alert_valid);
}


/*
 * Check that alert criteria are classified as failed, if there was no active warning in the last cycle and the BSW object
 * interferes with a guardrail. \uts{CSCSA-211398} \sdd{SF-6650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Is_Bsw_Alert_Criteria_Passed__returns_false_if_warning_not_active_and_cvw_not_used)
{
   /** \arrange Set up persistent data without warning in last cycle and fallback state slow and BSW object with guardrail
    * conflict. */
   uint8_t side     = FBK_SIDE_LEFT;
   uint8_t obj_id   = 6u;
   boolean_T result = FBK_TRUE;
   Lcda_Object_Location_Data_T loc_data;

   tracker_object.vcs_pos.y = -3.0f;
   tracker_object.width     = 2.1f;
   bsw_object.ego_side      = side;
   tracker_object.id        = obj_id;

   lcda_cals.k_min_exist_prob_radar_guardrail                               = 0.99f;
   lcda_cals.k_min_exist_prob_camera_guardrail                              = 0.99f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   lcda_cals.k_bsw_overlap_area_check_enable                                = FBK_FALSE;
   lcda_cals.k_bsw_uses_cvw_alert_state_enabled                             = FBK_FALSE;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -4.0f + lcda_cals.k_bsw_guardrail_distance_safety_margin;
   lcda_core_input.guardrail_data[side].radar.confidence       = 1.0f;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   bsw_persistent.f_prev_bsw_active[side] = FBK_FALSE;
   bsw_persistent.fallback_state[obj_id]  = FALLBACK_SLOW;

   /** \action Call function Lcda_Is_Bsw_Alert_Criteria_Passed to evaluate if alert criteria are passed. */
   result = Lcda_Is_Bsw_Alert_Criteria_Passed(&bsw_object, &lcda_core_input, &lcda_cals, &bsw_persistent, &loc_data,
                                              LCDA_USE_CURVI, &cvw_persistent);

   /** \assert Verify that alert criteria are classified as failed. */
   EXPECT_FALSE(result);
}


/*
 * Test for Lcda_Set_Bsw_Alert_Flag, Alert flag is set for defined flag variation, bsw object and coordinate system .
 * \uts{CSCSA-211399} \sdd{CSCSA-27684} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Bsw_Alert_Flag__returns_false_no_warning_and_env_conflict)
{

   /** \arrange Set up inputs for Lcda_Set_Bsw_Alert_Flag: (condition_flags), bsw_object, p_lcda_cals, coordinate_system, */
   boolean_T f_alert_valid;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_CURVI;
   boolean_T f_warning_active                 = FBK_FALSE;
   boolean_T f_count_check_passed             = FBK_TRUE;
   boolean_T f_fallback_warning_in_time       = FBK_TRUE;
   boolean_T f_obj_in_ego_lane                = FBK_TRUE;
   boolean_T f_obj_in_environment_conflict    = FBK_TRUE;
   boolean_T f_fallback_check_passed          = FBK_FALSE;
   boolean_T f_obj_reached_front_custom_limit = FBK_FALSE;
   boolean_T f_obj_overlap_below_threshold    = FBK_TRUE;
   boolean_T f_obj_below_max_rel_vel          = FBK_FALSE;

   lcda_cals.k_lcda_f_enable_alert_obj_in_ego_lane = FBK_TRUE;

   /** \action Call function Lcda_Set_Bsw_Alert_Flag */
   f_alert_valid = Lcda_Set_Bsw_Alert_Flag(&lcda_core_input, f_warning_active, f_count_check_passed, f_fallback_warning_in_time,
                                           f_obj_in_ego_lane, f_obj_in_environment_conflict, f_fallback_check_passed,
                                           f_obj_reached_front_custom_limit, f_obj_overlap_below_threshold,
                                           f_obj_below_max_rel_vel, &bsw_object, p_vehicle_data, &lcda_cals, coordinate_system);

   /** \assert Verify f_alert_valid */
   EXPECT_FLOAT_EQ(f_alert_valid, FBK_FALSE);
}


/*
 * Test for Lcda_Set_Bsw_Alert_Flag, Alert flag is set for defined flag variation, bsw object and coordinate system .
 * \uts{CSCSA-211400} \sdd{CSCSA-27684} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Bsw_Alert_Flag__returns_false_no_warning_and_obj_not_in_lane)
{

   /** \arrange Set up inputs for Lcda_Set_Bsw_Alert_Flag: (condition_flags), bsw_object, p_lcda_cals, coordinate_system, */
   boolean_T f_alert_valid;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_CURVI;
   boolean_T f_warning_active                 = FBK_FALSE;
   boolean_T f_count_check_passed             = FBK_TRUE;
   boolean_T f_fallback_warning_in_time       = FBK_TRUE;
   boolean_T f_obj_in_ego_lane                = FBK_TRUE;
   boolean_T f_obj_in_environment_conflict    = FBK_TRUE;
   boolean_T f_fallback_check_passed          = FBK_FALSE;
   boolean_T f_obj_reached_front_custom_limit = FBK_FALSE;
   boolean_T f_obj_overlap_below_threshold    = FBK_TRUE;
   boolean_T f_obj_below_max_rel_vel          = FBK_FALSE;

   lcda_cals.k_lcda_f_enable_alert_obj_in_ego_lane = FBK_FALSE;

   /** \action Call function Lcda_Set_Bsw_Alert_Flag */
   f_alert_valid = Lcda_Set_Bsw_Alert_Flag(&lcda_core_input, f_warning_active, f_count_check_passed, f_fallback_warning_in_time,
                                           f_obj_in_ego_lane, f_obj_in_environment_conflict, f_fallback_check_passed,
                                           f_obj_reached_front_custom_limit, f_obj_overlap_below_threshold,
                                           f_obj_below_max_rel_vel, &bsw_object, p_vehicle_data, &lcda_cals, coordinate_system);

   /** \assert Verify f_alert_valid */
   EXPECT_FLOAT_EQ(f_alert_valid, FBK_FALSE);
}


/*
 * Test for Lcda_Set_Bsw_Alert_Flag, Alert flag is set for defined flag variation, bsw object and coordinate system .
 * \uts{CSCSA-211401} \sdd{CSCSA-27684} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Process_Bsw_Test, Lcda_Set_Bsw_Alert_Flag__returns_false_no_warning_and_fallback_not_in_time)
{

   /** \arrange Set up inputs for Lcda_Set_Bsw_Alert_Flag: (condition_flags), bsw_object, p_lcda_cals, coordinate_system, */
   boolean_T f_alert_valid;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_CURVI;
   boolean_T f_warning_active                 = FBK_FALSE;
   boolean_T f_count_check_passed             = FBK_TRUE;
   boolean_T f_fallback_warning_in_time       = FBK_FALSE;
   boolean_T f_obj_in_ego_lane                = FBK_TRUE;
   boolean_T f_obj_in_environment_conflict    = FBK_TRUE;
   boolean_T f_fallback_check_passed          = FBK_FALSE;
   boolean_T f_obj_reached_front_custom_limit = FBK_FALSE;
   boolean_T f_obj_overlap_below_threshold    = FBK_TRUE;
   boolean_T f_obj_below_max_rel_vel          = FBK_FALSE;

   lcda_cals.k_lcda_f_enable_alert_obj_in_ego_lane = FBK_FALSE;

   /** \action Call function Lcda_Set_Bsw_Alert_Flag */
   f_alert_valid = Lcda_Set_Bsw_Alert_Flag(&lcda_core_input, f_warning_active, f_count_check_passed, f_fallback_warning_in_time,
                                           f_obj_in_ego_lane, f_obj_in_environment_conflict, f_fallback_check_passed,
                                           f_obj_reached_front_custom_limit, f_obj_overlap_below_threshold,
                                           f_obj_below_max_rel_vel, &bsw_object, p_vehicle_data, &lcda_cals, coordinate_system);

   /** \assert Verify f_alert_valid */
   EXPECT_FLOAT_EQ(f_alert_valid, FBK_FALSE);
}