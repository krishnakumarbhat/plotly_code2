/**
 * @file lcda_common_functions_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_common_functions.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42489}
 */

#include "lcda_common_functions_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "lcda_common_functions.c"
#include "lcda_core_output_t.h"
#include "lcda_input_generator.h"
#include "lcda_types.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


static void Lcda_Create_Default_Zone(Fbk_Field_Of_Interest_T *zone)
{
   // Create a default zone (right of ego) which is a rectangle
   // point 0 = (-2.0m, 2.5m)
   // zone length = 10m, width = 2m
   Vector_2d_T zone_p0;

   zone_p0.x = -2.0f;
   zone_p0.y = 2.5f;

   Lcda_Create_Zone(zone_p0, 10.0f, 2.0f, zone);
}

/**
 * Set the left turn signal and a left side alert state. Determine current alert state level and verify that it equals
 * LCDA_ALERT_STATE_LEVEL_2. \uts{CSCSA-42490} \sdd{SF-6559} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Alert_State__is_alert_state_level_2_when_turn_signal_left_and_alert_is_true_on_left_side)
{
   /** \arrange Set up turn signal and alert state. */
   uint8_t side                   = FBK_SIDE_LEFT;
   boolean_T alert_active         = FBK_TRUE;
   Lcda_Turn_Signal_T turn_signal = TURN_SIGNAL_LEFT;
   Lcda_Alert_State_T result_alert_state;

   /** \action Call Lcda_Get_Alert_State to determine the current alert state level. */
   result_alert_state = Lcda_Get_Alert_State(side, alert_active, turn_signal);

   /** \assert Check that the alert state level equals LCDA_ALERT_STATE_LEVEL_2. */
   EXPECT_EQ(result_alert_state, LCDA_ALERT_STATE_LEVEL_2);
}

/**
 * Dont set turn signal but a left side alert state. Determine current alert state level and verify that it equals
 * LCDA_ALERT_STATE_LEVEL_1. \uts{CSCSA-42491} \sdd{SF-6559} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Alert_State__is_alert_state_level_1_when_turn_signal_none_and_alert_is_true_on_left_side)
{
   /** \arrange Set up turn signal and alert state. */
   uint8_t side                   = FBK_SIDE_LEFT;
   boolean_T alert_active         = FBK_TRUE;
   Lcda_Turn_Signal_T turn_signal = TURN_SIGNAL_NONE;
   Lcda_Alert_State_T result_alert_state;

   /** \action Call Lcda_Get_Alert_State to determine the current alert state level. */
   result_alert_state = Lcda_Get_Alert_State(side, alert_active, turn_signal);

   /** \assert Check that the alert state level equals LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(result_alert_state, LCDA_ALERT_STATE_LEVEL_1);
}

/**
 * Set the left turn signal but no alert state. Determine current alert state level and verify that it equals
 * LCDA_ALERT_STATE_NONE. \uts{CSCSA-42492} \sdd{SF-6559} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Alert_State__is_alert_state_level_none_when_turn_signal_left_and_alert_is_false_on_left_side)
{
   /** \arrange Set up turn signal and alert state. */
   uint8_t side                   = FBK_SIDE_LEFT;
   boolean_T alert_active         = FBK_FALSE;
   Lcda_Turn_Signal_T turn_signal = TURN_SIGNAL_LEFT;
   Lcda_Alert_State_T result_alert_state;

   /** \action Call Lcda_Get_Alert_State to determine the current alert state level. */
   result_alert_state = Lcda_Get_Alert_State(side, alert_active, turn_signal);

   /** \assert Check that the alert state level equals LCDA_ALERT_STATE_NONE. */
   EXPECT_EQ(result_alert_state, LCDA_ALERT_STATE_NONE);
}

/**
 * Set neither a turn signal nor an alert state for the right side. Determine current alert state level and verify that it equals
 * LCDA_ALERT_STATE_NONE. \uts{CSCSA-42493} \sdd{SF-6559} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Alert_State__is_alert_state_level_none_when_turn_signal_none_and_alert_is_false_on_right_side)
{
   /** \arrange Set up turn signal and alert state. */
   uint8_t side                   = FBK_SIDE_RIGHT;
   boolean_T alert_active         = FBK_FALSE;
   Lcda_Turn_Signal_T turn_signal = TURN_SIGNAL_NONE;
   Lcda_Alert_State_T result_alert_state;

   /** \action Call Lcda_Get_Alert_State to determine the current alert state level. */
   result_alert_state = Lcda_Get_Alert_State(side, alert_active, turn_signal);

   /** \assert Check that the alert state level equals LCDA_ALERT_STATE_NONE. */
   EXPECT_EQ(result_alert_state, LCDA_ALERT_STATE_NONE);
}

/**
 * Set the right turn signal and a right side alert state. Determine current alert state level and verify that it equals
 * LCDA_ALERT_STATE_LEVEL_2. \uts{CSCSA-42494} \sdd{SF-6559} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Alert_State__is_alert_state_level_2_when_turn_signal_right_and_alert_is_true_on_right_side)
{
   /** \arrange Set up turn signal and alert state. */
   uint8_t side                   = FBK_SIDE_RIGHT;
   boolean_T alert_active         = FBK_TRUE;
   Lcda_Turn_Signal_T turn_signal = TURN_SIGNAL_RIGHT;
   Lcda_Alert_State_T result_alert_state;

   /** \action Call Lcda_Get_Alert_State to determine the current alert state level. */
   result_alert_state = Lcda_Get_Alert_State(side, alert_active, turn_signal);

   /** \assert Check that the alert state level equals LCDA_ALERT_STATE_LEVEL_2. */
   EXPECT_EQ(result_alert_state, LCDA_ALERT_STATE_LEVEL_2);
}


/**
 * Dont set turn signal but a right side alert state. Determine current alert state level and verify that it equals
 * LCDA_ALERT_STATE_LEVEL_1. \uts{CSCSA-42495} \sdd{SF-6559} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Alert_State__is_alert_state_level_1_when_turn_signal_left_and_alert_is_true_on_right_side)
{
   /** \arrange Set up turn signal and alert state. */
   uint8_t side                   = FBK_SIDE_RIGHT;
   boolean_T alert_active         = FBK_TRUE;
   Lcda_Turn_Signal_T turn_signal = TURN_SIGNAL_LEFT;
   Lcda_Alert_State_T result_alert_state;

   /** \action Call Lcda_Get_Alert_State to determine the current alert state level. */
   result_alert_state = Lcda_Get_Alert_State(side, alert_active, turn_signal);

   /** \assert Check that the alert state level equals LCDA_ALERT_STATE_LEVEL_1. */
   EXPECT_EQ(result_alert_state, LCDA_ALERT_STATE_LEVEL_1);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the reference point is within the zone.
 * Call Lcda_Is_Ref_Point_In_Zone and verify that it returns FBK_TRUE. \uts{CSCSA-42496} \sdd{SF-6566}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Ref_Point_In_Zone__returns_true_for_obj_with_center_in_zone)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   boolean_T result;

   lcda_tracker_object.curvi_pos.x   = -6.0f;
   lcda_tracker_object.curvi_pos.y   = 1.0f;
   lcda_tracker_object.curvi_heading = 0.0f;
   lcda_tracker_object.length        = 5.0f;
   lcda_tracker_object.width         = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;
   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Is_Ref_Point_In_Zone to determine if the reference point is within the zone. */
   result = Lcda_Is_Ref_Point_In_Zone(&lcda_tracker_object, &zone, &lcda_cals);

   /** \assert Check that result is FBK_TRUE. */
   EXPECT_TRUE(result);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the reference point is not within the
 * zone. Call Lcda_Is_Ref_Point_In_Zone and verify that it returns FBK_FALSE. \uts{CSCSA-42497} \sdd{SF-6566}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Ref_Point_In_Zone__returns_false_for_obj_outside_zone)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   boolean_T result;

   lcda_tracker_object.curvi_pos.x   = -6.0f;
   lcda_tracker_object.curvi_pos.y   = 4.0f;
   lcda_tracker_object.curvi_heading = 0.0f;
   lcda_tracker_object.length        = 5.0f;
   lcda_tracker_object.width         = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;
   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Is_Ref_Point_In_Zone to determine if the reference point is within the zone. */
   result = Lcda_Is_Ref_Point_In_Zone(&lcda_tracker_object, &zone, &lcda_cals);

   /** \assert Check that result is FBK_FALSE. */
   EXPECT_FALSE(result);
}

/**
 * Create a zone based on object properties and check if the points are set properly.
 * \uts{CSCSA-70061} \sdd{CSCSA-70149} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Create_Object_Field_Of_Interest__create_foi_in_vcs)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T object_foi;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;

   lcda_tracker_object.vcs_pos.x   = -6.0f;
   lcda_tracker_object.vcs_pos.y   = 4.0f;
   lcda_tracker_object.vcs_heading = 0.0f;
   lcda_tracker_object.length      = 5.0f;
   lcda_tracker_object.width       = 2.0f;

   /** \action Call Lcda_Is_Ref_Point_In_Zone to determine if the reference point is within the zone. */
   Lcda_Create_Object_Field_Of_Interest(&object_foi, &lcda_tracker_object, coordinate_system);


   /** \assert Check that result is FBK_FALSE. */
   EXPECT_FLOAT_EQ(object_foi.points[0].x, -3.5f);
   EXPECT_FLOAT_EQ(object_foi.points[1].x, -3.5f);
   EXPECT_FLOAT_EQ(object_foi.points[2].x, -8.5f);
   EXPECT_FLOAT_EQ(object_foi.points[3].x, -8.5f);
   EXPECT_FLOAT_EQ(object_foi.points[0].y, 3.0f);
   EXPECT_FLOAT_EQ(object_foi.points[1].y, 5.0f);
   EXPECT_FLOAT_EQ(object_foi.points[2].y, 5.0f);
   EXPECT_FLOAT_EQ(object_foi.points[3].y, 3.0f);
}


/**
 * Create a default zone and a tracker object. Set the tracker object position such that the reference point is within the zone.
 * Call Lcda_Get_Critical_Point and verify that it returns the tracker objects center. \uts{CSCSA-42498} \sdd{SF-6556}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Critical_Point__returns_center_point_if_it_is_in_zone)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 5.0f;
   float32_T obj_width   = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = -4.0f;
   obj_center.y = 1.0f;

   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical point equals the tracker objects center. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x);
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the tracker object is within the zone but
 * the reference point behind the zone. Call Lcda_Get_Critical_Point and verify that it returns the tracker objects front.
 * \uts{CSCSA-42499} \sdd{SF-6556} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Critical_Point__returns_front_center_point_if_it_is_in_zone_and_center_point_behind_zone)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 5.0f;
   float32_T obj_width   = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = -14.0f;
   obj_center.y = 1.0f;

   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical point equals the tracker objects front. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x + obj_length / 2.0f);
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the tracker object is within the zone but
 * the reference point in front of the zone. Call Lcda_Get_Critical_Point and verify that it returns the tracker objects rear.
 * \uts{CSCSA-42500} \sdd{SF-6556} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Critical_Point__returns_rear_center_point_if_it_is_in_zone_and_center_point_in_front_of_zone)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 5.0f;
   float32_T obj_width   = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = 0.0f;
   obj_center.y = 1.0f;

   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical point equals the tracker objects rear. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x - obj_length / 2.0f);
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the tracker object is within the zone but
 * the tracker objects length such that all reference points are outside the zone. Call Lcda_Get_Critical_Point and verify that it
 * returns the zones center. \uts{CSCSA-42501} \sdd{SF-6556} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Critical_Point__returns_center_point_at_zone_center_if_all_other_candidates_are_outside_of_zone_case1)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 22.0f;
   float32_T obj_width   = 3.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = -12.5f;
   obj_center.y = 2.0f;

   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical point equals the zones center. */
   EXPECT_FLOAT_EQ(result.x, -7.0f);
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the tracker object is within the zone but
 * the tracker objects length such that all reference points are outside the zone. Call Lcda_Get_Critical_Point and verify that it
 * returns the zones center. \uts{CSCSA-42502} \sdd{SF-6556} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Critical_Point__returns_center_point_at_zone_center_if_all_other_candidates_are_outside_of_zone_case2)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 22.0f;
   float32_T obj_width   = 3.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = -1.5f;
   obj_center.y = 2.0f;

   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical point equals the zones center. */
   EXPECT_FLOAT_EQ(result.x, -7.0f);
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the tracker object is within the zone. The
 * tracker object is laterally far off but heading towards the zone. Call Lcda_Get_Critical_Point and verify that the lateral
 * position of the critical point is offset as specified by cal value. \uts{CSCSA-42503} \sdd{SF-6556}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Critical_Point__adjusts_lat_pos_if_obj_beside_zone_not_heading_away_from_zone)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 5.0f;
   float32_T obj_width   = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = -4.0f;
   obj_center.y = 4.0f;

   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical points lateral position is adjusted as specified by cal value. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x);
   EXPECT_FLOAT_EQ(result.y, obj_center.y - lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio * obj_width / 2.0f);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the tracker object is within the zone. The
 * tracker object is laterally far off but heading away from the zone. Call Lcda_Get_Critical_Point and verify that the lateral
 * position of the critical point is not altered. \uts{CSCSA-42504} \sdd{SF-6556} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Critical_Point__does_not_adjust_lat_pos_if_obj_beside_zone_heading_away_from_zone)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.3f;
   float32_T obj_length  = 5.0f;
   float32_T obj_width   = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = -4.0f;
   obj_center.y = 4.0f;

   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical point is the tracker objects center. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x);
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Create a default zone and a tracker object. Set the tracker object position such that the tracker object is within the zone. The
 * tracker object is laterally offset into the ego position. Call Lcda_Get_Critical_Point and verify that the lateral position of
 * the critical point is not altered. \uts{CSCSA-42506} \sdd{SF-6556} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Critical_Point__does_not_adjust_lat_pos_if_obj_beside_zone_on_inner_side)
{
   /** \arrange Create zone and tracker object. */
   Fbk_Field_Of_Interest_T zone{};
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.3f;
   float32_T obj_length  = 5.0f;
   float32_T obj_width   = 2.0f;

   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;

   obj_center.x = -8.0f;
   obj_center.y = 0.2f;

   Lcda_Create_Default_Zone(&zone);
   zone.points[0].x = -3.0f;

   /** \action Call Lcda_Get_Critical_Point to compute the critical point from the tracker object. */
   result = Lcda_Get_Critical_Point(&obj_center, obj_heading, obj_length, obj_width, &zone, &lcda_cals);

   /** \assert Check that critical point is the tracker objects center. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x);
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Set alert state level to LCDA_ALERT_STATE_LEVEL_1 and call Lcda_Is_Alert_On to check if an alert is active. Check that the
 * result is FBK_TRUE. \uts{CSCSA-42507} \sdd{SF-6564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Alert_On__true_when_alert_state_level_1)
{
   /** \arrange Set alert state level. */
   Lcda_Alert_State_T alert_state = LCDA_ALERT_STATE_LEVEL_1;

   /** \action Call Lcda_Is_Alert_On to compute if alert state is on. */
   boolean_T result = Lcda_Is_Alert_On(alert_state);

   /** \assert Check that alert state is FBK_TRUE. */
   EXPECT_TRUE(result);
}

/**
 * Set alert state level to LCDA_ALERT_STATE_LEVEL_2 and call Lcda_Is_Alert_On to check if an alert is active. Check that the
 * result is FBK_TRUE. \uts{CSCSA-42508} \sdd{SF-6564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Alert_On__true_when_alert_state_level_2)
{
   /** \arrange Set alert state level. */
   Lcda_Alert_State_T alert_state = LCDA_ALERT_STATE_LEVEL_2;

   /** \action Call Lcda_Is_Alert_On to compute if alert state is on. */
   boolean_T result = Lcda_Is_Alert_On(alert_state);

   /** \assert Check that alert state is FBK_TRUE. */
   EXPECT_TRUE(result);
}

/**
 * Set alert state level to LCDA_ALERT_STATE_NONE and call Lcda_Is_Alert_On to check if an alert is active. Check that the result
 * is FBK_FALSE. \uts{CSCSA-42509} \sdd{SF-6564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Alert_On__false_when_alert_state_none)
{
   /** \arrange Set alert state level. */
   Lcda_Alert_State_T alert_state = LCDA_ALERT_STATE_NONE;

   /** \action Call Lcda_Is_Alert_On to compute if alert state is on. */
   boolean_T result = Lcda_Is_Alert_On(alert_state);

   /** \assert Check that alert state is FBK_FALSE. */
   EXPECT_FALSE(result);
}

/**
 * Set alert state level to invalid value (out of bounds) and call Lcda_Is_Alert_On to check if an alert is active. Check that the
 * result is FBK_FALSE. \uts{CSCSA-42510} \sdd{SF-6564} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Alert_On__false_when_alert_state_is_invalid)
{
   /** \arrange Set alert state level. */
   Lcda_Alert_State_T alert_state = (Lcda_Alert_State_T) 10;

   /** \action Call Lcda_Is_Alert_On to compute if alert state is on. */
   boolean_T result = Lcda_Is_Alert_On(alert_state);

   /** \assert Check that alert state is FBK_FALSE. */
   EXPECT_FALSE(result);
}

/**
 * Create a non-rectangular zone and adapt its longitudinal length using a factor. Check that the zone has expected size
 * afterwards. \uts{CSCSA-42511} \sdd{SF-6568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor__zone_is_halved_when_factor_is_0p5_zone_not_rectangular_test1)
{
   /** \arrange Set length factor and zone. In this test x-coord of points 0 and 5 are different leading to a non-rectangular zone.
    */
   float32_T factor = 0.5f;
   Fbk_Field_Of_Interest_T zone{};
   zone.size = LCDA_NUMBER_OF_ZONE_POINTS;
   Fbk_Field_Of_Interest_T zone_ref{};

   zone.points[0].x = -2.0f;
   zone.points[0].y = 4.0f;

   zone.points[1].x = -2.0f;
   zone.points[1].y = 4.0f;

   zone.points[2].x = -6.0f;
   zone.points[2].y = 4.0f;

   zone.points[3].x = -6.0f;
   zone.points[3].y = 2.0f;

   zone.points[4].x = -2.0f;
   zone.points[4].y = 2.0f;

   zone.points[5].x = -1.0f;
   zone.points[5].y = 2.0f;

   zone_ref = zone;

   /** \action Call Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor to resize zone. */
   Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(factor, &zone, 2.0f);

   /** \assert Check that only points 2 and 3 should be changed. All other points should remain the same. */
   EXPECT_EQ(zone.points[0].x, zone_ref.points[0].x);
   EXPECT_EQ(zone.points[1].x, zone_ref.points[1].x);
   EXPECT_EQ(zone.points[2].x, -4.0f);
   EXPECT_EQ(zone.points[3].x, -4.0f);
   EXPECT_EQ(zone.points[4].x, zone_ref.points[4].x);
   EXPECT_EQ(zone.points[5].x, zone_ref.points[5].x);
}

/**
 * Create a non-rectangular zone and adapt its longitudinal length using a factor. Check that the zone has expected size
 * afterwards. \uts{CSCSA-42512} \sdd{SF-6568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor__zone_is_halved_when_factor_is_0p5_when_zone_not_rectangular_test2)
{
   /** \arrange Set length factor and zone. In this test x-coord of points 0 and 5 are different leading to a non-rectangular zone.
    */

   float32_T factor = 0.5f;
   Fbk_Field_Of_Interest_T zone{};
   zone.size = LCDA_NUMBER_OF_ZONE_POINTS;
   Fbk_Field_Of_Interest_T zone_ref{};

   zone.points[0].x = -2.0f;
   zone.points[0].y = 4.0f;

   zone.points[1].x = -2.0f;
   zone.points[1].y = 4.0f;

   zone.points[2].x = -6.0f;
   zone.points[2].y = 4.0f;

   zone.points[3].x = -7.0f;
   zone.points[3].y = 2.0f;

   zone.points[4].x = -2.0f;
   zone.points[4].y = 2.0f;

   zone.points[5].x = -2.0f;
   zone.points[5].y = 2.0f;

   zone_ref = zone;

   /** \action Call Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor to resize zone. */
   Lcda_Multiply_Zone_Length_Behind_Ego_By_Factor(factor, &zone, 2.0f);

   /** \assert Check that only points 2 and 3 should be changed. All other points should remain the same. */
   EXPECT_EQ(zone.points[0].x, zone_ref.points[0].x);
   EXPECT_EQ(zone.points[1].x, zone_ref.points[1].x);
   EXPECT_EQ(zone.points[2].x, -4.0f);
   EXPECT_EQ(zone.points[3].x, -4.5);
   EXPECT_EQ(zone.points[4].x, zone_ref.points[4].x);
   EXPECT_EQ(zone.points[5].x, zone_ref.points[5].x);
}

/**
 * Create a tracker object with zero longitudinal relative speed. Compute the longitudinal TTC and check that the TTC has default
 * value LCDA_DEFAULT_LARGE_TTC. \uts{CSCSA-42513} \sdd{SF-6562} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttc__returns_default_large_ttc_when_zero_obj_rel_vel)
{
   /** \arrange Set target objects properties. */
   float32_T ego_length                = 5.0f;
   float32_T result                    = 0.0f;
   lcda_tracker_object.curvi_vel_rel.x = 0.0f;
   lcda_tracker_object.length          = 6.0f;
   lcda_tracker_object.curvi_pos.x     = -15.0f;

   /** \action Call Lcda_Get_Longitudinal_Ttc to calculate the TTC of the target object. */
   result = Lcda_Get_Longitudinal_Ttc(&lcda_tracker_object, ego_length);

   /** \assert Check that the TTC equals LCDA_DEFAULT_LARGE_TTC. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object with negative longitudinal relative speed. Compute the longitudinal TTC and check that the TTC has
 * default value LCDA_DEFAULT_LARGE_TTC. \uts{CSCSA-42514} \sdd{SF-6562} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttc__returns_default_large_ttc_when_obj_is_slower_than_ego)
{
   /** \arrange Set target objects properties. */
   float32_T ego_length                = 5.0f;
   float32_T result                    = 0.0f;
   lcda_tracker_object.curvi_vel_rel.x = -5.0f;
   lcda_tracker_object.length          = 6.0f;
   lcda_tracker_object.curvi_pos.x     = -15.0f;

   /** \action Call Lcda_Get_Longitudinal_Ttc to calculate the TTC of the target object. */
   result = Lcda_Get_Longitudinal_Ttc(&lcda_tracker_object, ego_length);

   /** \assert Check that the TTC equals LCDA_DEFAULT_LARGE_TTC. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object with positive longitudinal relative speed which is overlapping with ego vehicle. Compute the
 * longitudinal TTC and check that the TTC has value zero. \uts{CSCSA-42515} \sdd{SF-6562} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttc__returns_ttc_zero_when_obj_front_coincides_with_ego_rear)
{
   /** \arrange Set target objects properties. */
   float32_T ego_length                = 5.0f;
   float32_T result                    = 0.0f;
   lcda_tracker_object.curvi_vel_rel.x = 5.0f;
   lcda_tracker_object.length          = 6.0f;

   lcda_tracker_object.curvi_pos.x = -ego_length - (lcda_tracker_object.length / 2.0f);

   /** \action Call Lcda_Get_Longitudinal_Ttc to calculate the TTC of the target object. */
   result = Lcda_Get_Longitudinal_Ttc(&lcda_tracker_object, ego_length);

   /** \assert Check that the TTC equals zero. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Create a tracker object with positive longitudinal relative speed approaching the host from behind. Compute the longitudinal TTC
 * and check that the TTC has expected value. \uts{CSCSA-42516} \sdd{SF-6562} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttc__calculates_ttc)
{
   /** \arrange Set target objects properties. */
   float32_T ego_length   = 5.0f;
   float32_T result       = 0.0f;
   float32_T obj_distance = 10.0f; // dist of obj front bumper to ego rear bumper

   lcda_tracker_object.curvi_vel_rel.x = 5.0f;
   lcda_tracker_object.length          = 6.0f;

   // calculate the obj long pos in VCS coord
   lcda_tracker_object.curvi_pos.x = -obj_distance - ego_length - (lcda_tracker_object.length / 2.0f);

   /** \action Call Lcda_Get_Longitudinal_Ttc to calculate the TTC of the target object. */
   result = Lcda_Get_Longitudinal_Ttc(&lcda_tracker_object, ego_length);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, (obj_distance / lcda_tracker_object.curvi_vel_rel.x));
}

/**
 * Create a tracker object with negative lateral relative speed on the left side (moving away). Compute the lateral TTC and check
 * that the TTC has default value LCDA_DEFAULT_LARGE_TTC. \uts{CSCSA-42517} \sdd{SF-6561} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__returns_default_large_ttc_when_obj_on_left_is_moving_away_from_ego)
{
   /** \arrange Set target objects properties. */
   float32_T rel_velocity = -5.0f;
   float32_T obj_width    = 2.0f;
   float32_T obj_lat_pos  = -15.0f;
   float32_T ego_width    = 3.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object with positive lateral relative speed on the right side (moving away). Compute the lateral TTC and check
 * that the TTC has default value LCDA_DEFAULT_LARGE_TTC. \uts{CSCSA-42518} \sdd{SF-6561} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__returns_default_large_ttc_when_obj_on_right_is_moving_away_from_ego)
{
   /** \arrange Set target objects properties. */
   float32_T rel_velocity = 5.0f;
   float32_T obj_width    = 3.0f;
   float32_T obj_lat_pos  = 10.0f;
   float32_T ego_width    = 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object with zero lateral relative speed on the left side. Compute the lateral TTC and check that the TTC has
 * default value LCDA_DEFAULT_LARGE_TTC. \uts{CSCSA-42519} \sdd{SF-6561} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__returns_default_large_ttc_when_zero_obj_rel_vel)
{
   /** \arrange Set target objects properties. */
   float32_T rel_velocity = 0.0f;
   float32_T obj_width    = 3.0f;
   float32_T obj_lat_pos  = -15.0f;
   float32_T ego_width    = 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object with positive lateral speed on the left side (approaching ego vehicle) which is overlapping with ego
 * vehicle. Compute the lateral TTC and check that the TTC has value zero. \uts{CSCSA-42520} \sdd{SF-6561}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__returns_zero_when_obj_on_left_side_coincides_with_ego_side)
{
   /** \arrange Set target objects properties. */
   float32_T rel_velocity = 10.0f;
   float32_T obj_width    = 3.0f;
   float32_T ego_width    = 2.0f;
   float32_T obj_lat_pos  = -(obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Create a tracker object with negative lateral speed on the right side (approaching ego vehicle) which is overlapping with ego
 * vehicle. Compute the lateral TTC and check that the TTC has value zero. \uts{CSCSA-42521} \sdd{SF-6561}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__returns_zero_when_obj_on_right_side_coincides_with_ego_side)
{
   /** \arrange Set target objects properties. */
   float32_T rel_velocity = -10.0f;
   float32_T obj_width    = 3.0f;
   float32_T ego_width    = 2.0f;
   float32_T obj_lat_pos  = (obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, 0.0f);
}

/**
 * Create a tracker object with positive lateral relative speed approaching the host from the left side (approaching ego vehicle).
 * Compute the lateral TTC and check that the TTC has expected value. \uts{CSCSA-42522} \sdd{SF-6561}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__calculates_ttc_obj_on_left_side_moving_towards_ego)
{
   /** \arrange Set target objects properties. */
   float32_T rel_velocity = 10.0f;
   float32_T obj_width    = 3.0f;
   float32_T ego_width    = 2.0f;
   float32_T obj_distance = 5.0f;
   float32_T obj_lat_pos  = -obj_distance - (obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, (obj_distance / rel_velocity));
}

/**
 * Create a tracker object with negative lateral relative speed approaching the host from the right side (approaching ego vehicle).
 * Compute the lateral TTC and check that the TTC has expected value. \uts{CSCSA-42523} \sdd{SF-6561}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__calculates_ttc_obj_on_right_side_moving_towards_ego)
{
   /** \arrange Set target objects properties. */
   float32_T rel_velocity = -10.0f;
   float32_T obj_width    = 3.0f;
   float32_T ego_width    = 2.0f;
   float32_T obj_distance = 5.0f;
   float32_T obj_lat_pos  = obj_distance + (obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, (obj_distance / (Fbk_Abs_F(rel_velocity))));
}

/**
 * Create a tracker object with negative lateral relative speed approaching the host from the left side (approaching ego vehicle).
 * The total relative speed is caused by the host motion (track is laterally constant in world coordinates). Compute the lateral
 * TTC and check that the TTC has expected value. \uts{CSCSA-42524} \sdd{SF-6561} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__calculates_host_induced_ttc_obj_on_left_side_moving_towards_ego)
{
   /** \arrange Set target objects properties. */
   float32_T target_velocity = 0.0f;
   float32_T obj_width       = 3.0f;
   float32_T ego_width       = 2.0f;
   float32_T obj_distance    = 5.0f;
   float32_T rel_velocity    = target_velocity;
   float32_T obj_lat_pos     = -obj_distance - (obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object with negative lateral relative speed approaching the host from the right side (approaching ego vehicle).
 * The total relative speed is caused by the host motion (track is laterally constant in world coordinates). Compute the lateral
 * TTC and check that the TTC has expected value. \uts{CSCSA-42525} \sdd{SF-6561} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__calculates_host_induced_ttc_obj_on_right_side_moving_towards_ego)
{
   /** \arrange Set target objects properties. */
   float32_T target_velocity = 0.0f;
   float32_T obj_width       = 3.0f;
   float32_T ego_width       = 2.0f;
   float32_T obj_distance    = 5.0f;
   float32_T rel_velocity    = target_velocity;
   float32_T obj_lat_pos     = obj_distance + (obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object with negative lateral relative speed approaching the host from the left side (approaching ego vehicle).
 * The host is moving away from the target with the same speed (track and host move in parallel) but should give the same ttc as if
 * the host were stationary. Compute the lateral TTC and check that the TTC has expected value. \uts{CSCSA-42526} \sdd{SF-6561}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Ttc__calculates_ttc_obj_on_left_side_moving_towards_ego_but_host_is_moving_away)
{
   /** \arrange Set target objects properties. */
   float32_T target_velocity = 10.0f;
   float32_T obj_width       = 3.0f;
   float32_T ego_width       = 2.0f;
   float32_T obj_distance    = 5.0f;
   float32_T rel_velocity    = target_velocity;
   float32_T obj_lat_pos     = -obj_distance - (obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, (obj_distance / (target_velocity)));
}

/**
 * Create a tracker object with negative lateral relative speed approaching the host from the right side (approaching ego vehicle).
 * The host is moving away from the target with the same speed (track and host move in parallel) but should give the same ttc as if
 * the host were stationary. Compute the lateral TTC and check that the TTC has expected value. \uts{CSCSA-42527} \sdd{SF-6561}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Lateral_Ttc__calculates_host_induced_ttc_obj_on_right_side_moving_towards_ego_but_host_is_moving_away)
{
   /** \arrange Set target objects properties. */
   float32_T target_velocity = -10.0f;
   float32_T obj_width       = 3.0f;
   float32_T ego_width       = 2.0f;
   float32_T obj_distance    = 5.0f;
   float32_T rel_velocity    = target_velocity;
   float32_T obj_lat_pos     = obj_distance + (obj_width + ego_width) / 2.0f;

   lcda_tracker_object.curvi_pos.y     = obj_lat_pos;
   lcda_tracker_object.curvi_vel_rel.y = rel_velocity;
   lcda_tracker_object.width           = obj_width;

   /** \action Call Lcda_Get_Lateral_Ttc to calculate the TTC of the target object. */
   float32_T result = Lcda_Get_Lateral_Ttc(&lcda_tracker_object, ego_width);

   /** \assert Check that the TTC equals expected value. */
   EXPECT_FLOAT_EQ(result, (obj_distance / (Fbk_Abs_F(target_velocity))));
}

/**
 * Check if default value is returned when object pointer is null.
 * \uts{CSCSA-186401} \sdd{CSCSA-184205} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Ttle__null_object_pointer)
{
   /** \arrange Set zone. */
   Fbk_Field_Of_Interest_T zone = {};

   /** \action Call Lcda_Get_Ttle. */
   float32_T result = Lcda_Get_Ttle(nullptr, &zone, LCDA_USE_VCS);

   /** \assert Check that the TTLE equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTLE);
}

/**
 * Check if default value is returned when zone pointer is null.
 * \uts{CSCSA-186402} \sdd{CSCSA-184205} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Ttle__null_zone_pointer)
{
   /** \arrange N/A */
   /** \action Call Lcda_Get_Ttle. */
   float32_T result = Lcda_Get_Ttle(&lcda_tracker_object, nullptr, LCDA_USE_VCS);

   /** \assert Check that the TTLE equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTLE);
}

/**
 * Checks if default value is returned when the object's VCS relative lateral velocity is 0.
 * \uts{CSCSA-186403} \sdd{CSCSA-184205} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Ttle__zero_vcs_relative_lateral_velocity)
{
   /** \arrange Set target object properties. */
   Fbk_Field_Of_Interest_T zone;
   float32_T result;
   lcda_tracker_object.vcs_vel_rel.y = FBK_ZERO_F;

   /** \action Call Lcda_Get_Ttle. */
   result = Lcda_Get_Ttle(&lcda_tracker_object, &zone, LCDA_USE_VCS);

   /** \assert Check that the TTLE equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTLE);
}


/**
 * Checks if value is returned when object's data meet all criteria.
 * \uts{CSCSA-275212} \sdd{CSCSA-184205} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Ttle__return_calc)
{
   /** \arrange Set target object properties. */
   Fbk_Field_Of_Interest_T zone;
   float32_T result;
   lcda_tracker_object.vcs_vel_rel.y = 1.0f;
   zone.points[0u].y                 = 2.0f;
   /** \action Call Lcda_Get_Ttle. */
   result = Lcda_Get_Ttle(&lcda_tracker_object, &zone, LCDA_USE_VCS);

   /** \assert Check that the TTLE equals expected value. */
   EXPECT_FLOAT_EQ(result, 2.0f);
}


/**
 * Checks if value is returned when object's data meet all criteria.
 * \uts{CSCSA-275213} \sdd{CSCSA-184205} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Ttle__return_calc_above_100)
{
   /** \arrange Set target object properties. */
   Fbk_Field_Of_Interest_T zone;
   float32_T result;
   lcda_tracker_object.vcs_vel_rel.y = 1.0f;
   zone.points[0u].y                 = 200.0f;
   /** \action Call Lcda_Get_Ttle. */
   result = Lcda_Get_Ttle(&lcda_tracker_object, &zone, LCDA_USE_VCS);

   /** \assert Check that the TTLE equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTLE);
}


/**
 * Checks if default value is returned when the object's curvi relative lateral velocity is 0.
 * \uts{CSCSA-186404} \sdd{CSCSA-184205} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Ttle__zero_curvi_relative_lateral_velocity)
{
   /** \arrange Set target object properties. */
   Fbk_Field_Of_Interest_T zone;
   float32_T result;
   lcda_tracker_object.curvi_vel_rel.y = FBK_ZERO_F;


   /** \action Call Lcda_Get_Ttle. */
   result = Lcda_Get_Ttle(&lcda_tracker_object, &zone, LCDA_USE_CURVI);

   /** \assert Check that the TTLE equals expected value. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTLE);
}

/**
 * Create an invalid guard rail on the right side of the ego. Compute the lateral distance to the guard rail and check that it has
 * the default value LCDA_HUGE_LATERAL_DISTANCE. \uts{CSCSA-42528} \sdd{SF-6560} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Distance_Guardrail__is_very_high_when_no_guardrail_available_on_right_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(FBK_SIDE_RIGHT, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals expected LCDA_HUGE_LATERAL_DISTANCE. */
   EXPECT_FLOAT_EQ(lateral_distance, LCDA_HUGE_LATERAL_DISTANCE);
}

/**
 * Create an invalid guard rail on the left side of the ego. Compute the lateral distance to the guard rail and check that it has
 * the default value -LCDA_HUGE_LATERAL_DISTANCE. \uts{CSCSA-42529} \sdd{SF-6560} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Distance_Guardrail__is_very_high_when_no_guardrail_available_on_left_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(FBK_SIDE_LEFT, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals expected -LCDA_HUGE_LATERAL_DISTANCE. */
   EXPECT_FLOAT_EQ(lateral_distance, -LCDA_HUGE_LATERAL_DISTANCE);
}

/**
 * Create a close radar and far camera guard rail on the left side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the radar guard rail. \uts{CSCSA-42530} \sdd{SF-6560} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_radar_guardrail_on_left_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -2.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = -3.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the radar guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].radar.lateral_position);
}

/**
 * Create a far radar and close camera guard rail on the left side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the camera guard rail. \uts{CSCSA-42531} \sdd{SF-6560}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_camera_guardrail_on_left_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -3.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = -2.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the camera guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].camera.lateral_position);
}

/**
 * Create a close radar and far camera guard rail on the right side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the radar guard rail. \uts{CSCSA-42532} \sdd{SF-6560} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_radar_guardrail_on_right_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_RIGHT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 2.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = 3.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the radar guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].radar.lateral_position);
}

/**
 * Create a far radar and close camera guard rail on the right side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the camera guard rail. \uts{CSCSA-42533} \sdd{SF-6560}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_camera_guardrail_on_right_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_RIGHT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 3.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = 2.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the camera guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].camera.lateral_position);
}

/**
 * Create a camera and an invalid radar guard rail on the left side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the camera guard rail. \uts{CSCSA-42534} \sdd{SF-6560}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_camera_guardrail_when_radar_guardrail_invalid_on_left_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -2.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_INVALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = -3.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the camera guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].camera.lateral_position);
}

/**
 * Create a radar and an invalid camera guard rail on the left side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the radar guard rail. \uts{CSCSA-42535} \sdd{SF-6560} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_radar_guardrail_when_camera_guardrail_invalid_on_left_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_LEFT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = -3.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = -2.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_INVALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the radar guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].radar.lateral_position);
}

/**
 * Create a camera and an invalid radar guard rail on the right side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the camera guard rail. \uts{CSCSA-42536} \sdd{SF-6560}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_camera_guardrail_when_radar_guardrail_invalid_on_right_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_RIGHT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 2.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_INVALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = 3.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_VALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the camera guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].camera.lateral_position);
}

/**
 * Create a radar and an invalid camera guard rail on the right side of the ego. Compute the lateral distance to the guard rail and
 * check that it has the lateral distance of the radar guard rail. \uts{CSCSA-42537} \sdd{SF-6560} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Get_Lateral_Distance_Guardrail__is_equal_to_radar_guardrail_when_camera_guardrail_invalid_on_right_side)
{
   /** \arrange Set up guard rail information. */
   float32_T lateral_distance = 0.0f;
   uint8_t side               = FBK_SIDE_RIGHT;

   lcda_core_input.guardrail_data[side].radar.lateral_position = 3.0;
   lcda_core_input.guardrail_data[side].radar.status           = LCDA_GUARDRAIL_VALID;

   lcda_core_input.guardrail_data[side].camera.lateral_position = 2.0;
   lcda_core_input.guardrail_data[side].camera.status           = LCDA_GUARDRAIL_INVALID;

   /** \action Call Lcda_Get_Lateral_Distance_Guardrail to calculate the distance to the guard rail. */
   lateral_distance = Lcda_Get_Lateral_Distance_Guardrail(side, lcda_core_input.guardrail_data);

   /** \assert Check that the lateral distance of the guard rail equals lateral distance to the radar guard rail. */
   EXPECT_FLOAT_EQ(lateral_distance, lcda_core_input.guardrail_data[side].radar.lateral_position);
}

/**
 * Create a situation in which the object was critical before to trigger the hysteresis offset. Compute the existence probability
 * threshold with the hysteresis offset. \uts{CSCSA-42538} \sdd{SF-6807} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Existence_Probability_Threshold__with_previously_critical_object)
{
   /** \arrange Set up previous object information */
   float32_T existence_prob_threshold          = 0.0f;
   float32_T expected_existence_prob_threshold = 0.0f;
   uint8_t prev_obj_index[FBK_NUMBER_OF_SIDES]{};
   uint8_t obj_id = 10u;
   /* prev_obj_index[FBK_SIDE_LEFT] or prev_obj_index[FBK_SIDE_RIGHT] have to be equal to obj_idx for object to be critical
    * before */
   prev_obj_index[FBK_SIDE_LEFT]  = 10u;
   prev_obj_index[FBK_SIDE_RIGHT] = 9u;

   /* If f_use_cvw_lane_change_intention_zone is true
    * k_lcda_exist_prob_lc_intention_hys_offset will be used to set hysteresis offset and
    * k_lcda_min_exist_prob_lc_intention will be used to set existence probability threshold*/
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
   lcda_cals.k_lcda_exist_prob_lc_intention_hys_offset                = 0.05f;
   lcda_cals.k_lcda_min_exist_prob_lc_intention                       = 0.1f;

   expected_existence_prob_threshold =
      lcda_cals.k_lcda_min_exist_prob_lc_intention - lcda_cals.k_lcda_exist_prob_lc_intention_hys_offset;

   /** \action Call Lcda_Get_Existence_Probability_Threshold to calculate the existence probability threshold. */
   existence_prob_threshold = Lcda_Get_Existence_Probability_Threshold(&lcda_core_input, prev_obj_index, obj_id, &lcda_cals);

   /** \assert Check that the expected probability threshold equals the threshold given by calling
    * Lcda_Get_Existence_Probability_Threshold. */
   EXPECT_FLOAT_EQ(existence_prob_threshold, expected_existence_prob_threshold);
}

/**
 * Create a situation in which the object was not critical before. Compute the existence probability threshold without the
 * hysteresis. \uts{CSCSA-42539} \sdd{SF-6807} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Existence_Probability_Threshold__without_previously_critical_object)
{
   /** \arrange Set up previous object information */
   float32_T existence_prob_threshold          = 0.0f;
   float32_T expected_existence_prob_threshold = 0.0f;
   uint8_t prev_obj_index[FBK_NUMBER_OF_SIDES]{};
   uint8_t obj_id = 8u;
   /* prev_obj_index[FBK_SIDE_LEFT] or prev_obj_index[FBK_SIDE_RIGHT] have to be equal to obj_idx for object to be critical
    * before */
   prev_obj_index[FBK_SIDE_LEFT]  = 10u;
   prev_obj_index[FBK_SIDE_RIGHT] = 9u;

   /* If f_use_cvw_lane_change_intention_zone is true
    * k_lcda_min_exist_prob_lc_intention will be used to set existence probability threshold*/
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
   lcda_cals.k_lcda_min_exist_prob_lc_intention                       = 0.1f;

   expected_existence_prob_threshold = lcda_cals.k_lcda_min_exist_prob_lc_intention;

   /** \action Call Lcda_Get_Existence_Probability_Threshold to calculate the existence probability threshold. */
   existence_prob_threshold = Lcda_Get_Existence_Probability_Threshold(&lcda_core_input, prev_obj_index, obj_id, &lcda_cals);

   /** \assert Check that the expected probability threshold equals the threshold given by calling
    * Lcda_Get_Existence_Probability_Threshold. */
   EXPECT_FLOAT_EQ(existence_prob_threshold, expected_existence_prob_threshold);
}

/**
 * For a track object of status mature call function to increase the mature zone counter. Check that mature count is increased.
 * \uts{CSCSA-42540} \sdd{SF-6563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Increment_Mature_Count_In_Zone__increment_if_obj_status_mature)
{
   /** \arrange Set up a counter and a status MATURE. */
   uint8_t starting_count = 3u;
   uint8_t counter        = starting_count;
   Pa_Obj_Status_T status = PA_OBJ_STATUS_MATURE;

   /** \action Call Lcda_Increment_Mature_Count_In_Zone to increase the mature counter. */
   Lcda_Increment_Mature_Count_In_Zone(&counter, status);

   /** \assert Check that the mature counter is increased. */
   EXPECT_EQ(counter, starting_count + 1u);
}

/**
 * For a track object of status coasted call function to increase the mature zone counter. Check that mature count is not
 * increased. \uts{CSCSA-42541} \sdd{SF-6563} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Increment_Mature_Count_In_Zone__no_increment_if_obj_status_coasted)
{
   /** \arrange Set up a counter and a status COASTED. */
   uint8_t starting_count = 3u;
   uint8_t counter        = starting_count;
   Pa_Obj_Status_T status = PA_OBJ_STATUS_COASTED;

   /** \action Call Lcda_Increment_Mature_Count_In_Zone to increase the mature counter. */
   Lcda_Increment_Mature_Count_In_Zone(&counter, status);

   /** \assert Check that the mature counter is not increased. */
   EXPECT_EQ(counter, starting_count);
}

/**
 * Check that suitable object is correctly identified as in ego lane for an object on left ego side.
 * \uts{CSCSA-42542} \sdd{SF-6565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Object_In_Ego_Lane__is_true_for_object_inside_lane_on_left_side)
{
   /** \arrange Set up lane width, lane width factor and object position such that object is left of ego and inside ego lane. */
   float32_T lane_width                                                     = 4.0f;
   Lcda_Coordinate_System_T coordinate_system                               = LCDA_USE_CURVI;
   lcda_tracker_object.curvi_pos.y                                          = -1.9f;
   lcda_cals.k_lcda_ego_lane_check_center_point_only                        = FBK_TRUE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor                    = 1.0f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   /** \action Call function to evaluate if object is in ego lane. */
   boolean_T f_in_ego_lane = Lcda_Is_Object_In_Ego_Lane(lane_width, &lcda_tracker_object, &lcda_cals, coordinate_system);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(f_in_ego_lane);
}

/**
 * Check that suitable object is correctly identified as in ego lane for an object on right ego side.
 * \uts{CSCSA-42543} \sdd{SF-6565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Object_In_Ego_Lane__is_true_for_object_inside_lane_on_right_side)
{
   /** \arrange Set up lane width, lane width factor and object position such that object is right of ego and inside ego lane. */
   float32_T lane_width                                                     = 4.0f;
   Lcda_Coordinate_System_T coordinate_system                               = LCDA_USE_CURVI;
   lcda_tracker_object.curvi_pos.y                                          = 2.9f;
   lcda_cals.k_lcda_ego_lane_check_center_point_only                        = FBK_TRUE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor                    = 1.5f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   /** \action Call function to evaluate if object is in ego lane. */
   boolean_T f_in_ego_lane = Lcda_Is_Object_In_Ego_Lane(lane_width, &lcda_tracker_object, &lcda_cals, coordinate_system);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(f_in_ego_lane);
}

/**
 * Check that suitable object is correctly identified as not in ego lane for an object on left ego side.
 * \uts{CSCSA-42544} \sdd{SF-6565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Object_In_Ego_Lane__is_false_for_object_not_inside_lane_on_left_side)
{
   /** \arrange Set up lane width, lane width factor and object position such that object is left of ego and outside ego lane. */
   float32_T lane_width                                                     = 4.0f;
   Lcda_Coordinate_System_T coordinate_system                               = LCDA_USE_CURVI;
   lcda_tracker_object.curvi_pos.y                                          = -3.1f;
   lcda_cals.k_lcda_ego_lane_check_center_point_only                        = FBK_TRUE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor                    = 1.5f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   /** \action Call function to evaluate if object is in ego lane. */
   boolean_T f_in_ego_lane = Lcda_Is_Object_In_Ego_Lane(lane_width, &lcda_tracker_object, &lcda_cals, coordinate_system);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(f_in_ego_lane);
}

/**
 * Check that suitable object is correctly identified as not in ego lane for an object on right ego side.
 * \uts{CSCSA-42545} \sdd{SF-6565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Object_In_Ego_Lane__is_false_for_object_not_inside_lane_on_right_side)
{
   /** \arrange Set up lane width, lane width factor and object position such that object is right of ego and outside ego lane. */
   float32_T lane_width                                                     = 4.0f;
   Lcda_Coordinate_System_T coordinate_system                               = LCDA_USE_CURVI;
   lcda_tracker_object.curvi_pos.y                                          = 2.1f;
   lcda_cals.k_lcda_ego_lane_check_center_point_only                        = FBK_TRUE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor                    = 1.0f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;

   /** \action Call function to evaluate if object is in ego lane. */
   boolean_T f_in_ego_lane = Lcda_Is_Object_In_Ego_Lane(lane_width, &lcda_tracker_object, &lcda_cals, coordinate_system);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(f_in_ego_lane);
}

/**
 * Check that suitable object is correctly identified as in ego lane when only slightly overlapping in curvi coordinates.
 * \uts{CSCSA-42546} \sdd{SF-6565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Object_In_Ego_Lane__is_true_for_object_slightly_overlapping_ego_lanee_in_curvi)
{
   /** \arrange Set up lane width, lane width factor and object position such that object is right of ego and outside ego lane. */
   float32_T lane_width                                                     = 3.0f;
   Lcda_Coordinate_System_T coordinate_system                               = LCDA_USE_CURVI;
   lcda_cals.k_lcda_ego_lane_check_center_point_only                        = FBK_FALSE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor                    = 1.0f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   Lcda_Create_Bsw_Track(&lcda_tracker_object, 1u, -10.0f, 2.0f);

   /** \action Call function to evaluate if object is in ego lane. */
   boolean_T f_in_ego_lane = Lcda_Is_Object_In_Ego_Lane(lane_width, &lcda_tracker_object, &lcda_cals, coordinate_system);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(f_in_ego_lane);
}

/**
 * Check that suitable object is correctly identified as in ego lane when only slightly overlapping in vcs coordinates.
 * \uts{CSCSA-42547} \sdd{SF-6565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Object_In_Ego_Lane__is_true_for_object_slightly_overlapping_ego_lanee_in_vcs)
{
   /** \arrange Set up lane width, lane width factor and object position such that object is right of ego and outside ego lane. */
   float32_T lane_width                                                     = 3.0f;
   Lcda_Coordinate_System_T coordinate_system                               = LCDA_USE_VCS;
   lcda_cals.k_lcda_ego_lane_check_center_point_only                        = FBK_FALSE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor                    = 1.0f;
   lcda_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention = FBK_FALSE;
   Lcda_Create_Bsw_Track(&lcda_tracker_object, 2u, -10.0f, 2.0f);

   /** \action Call function to evaluate if object is in ego lane. */
   boolean_T f_in_ego_lane = Lcda_Is_Object_In_Ego_Lane(lane_width, &lcda_tracker_object, &lcda_cals, coordinate_system);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(f_in_ego_lane);
}

/**
 * Check that mirror zone functionality flips y coordinates and does not change x coordinates.
 * \uts{CSCSA-42548} \sdd{SF-6567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Mirror_Zone_Across_Long_Axis__is_false_for_object_not_inside_lane_on_right_side)
{
   /** \arrange Set up zone with non-zero coordinates. */
   Fbk_Field_Of_Interest_T zone;
   Fbk_Field_Of_Interest_T control_zone;
   zone.size         = 4u;
   zone.points[0u].x = 1.0f;
   zone.points[1u].x = 2.0f;
   zone.points[2u].x = 3.0f;
   zone.points[3u].x = 4.0f;
   zone.points[0u].y = 5.0f;
   zone.points[1u].y = 6.0f;
   zone.points[2u].y = 7.0f;
   zone.points[3u].y = 8.0f;

   control_zone = zone;

   /** \action Call function to mirror zone across longitudinal axis. */
   Lcda_Mirror_Zone_Across_Long_Axis(&zone);

   /** \assert Verify that all y coordinates are flipped and x coordinates are unchanged. */
   for (uint8_t i = 0; i < zone.size; i++)
   {
      EXPECT_FLOAT_EQ(zone.points[i].x, control_zone.points[i].x);
      EXPECT_FLOAT_EQ(zone.points[i].y, -control_zone.points[i].y);
   }
}

/**
 * Create a situation in which the object was critical before on the right side. Check if Lcda_Was_Object_Critical_Before asserts
 * that object was critical before. \uts{CSCSA-42549} \sdd{SF-6814} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Was_Object_Critical_Before__previously_critical_object_right)
{
   /** \arrange Set up previous object information */
   boolean_T expected_was_obj_critical_before = FBK_TRUE;
   boolean_T was_obj_critical_before          = FBK_FALSE;

   uint8_t prev_obj_index[FBK_NUMBER_OF_SIDES]{};
   uint8_t obj_idx = 10u;
   /* prev_obj_index[FBK_SIDE_LEFT] or prev_obj_index[FBK_SIDE_RIGHT] have to be equal to obj_idx for object to be critical
    * before */
   prev_obj_index[FBK_SIDE_LEFT]  = 9u;
   prev_obj_index[FBK_SIDE_RIGHT] = 10u;

   /** \action Call Lcda_Was_Object_Critical_Before to see if object was critical before. */
   was_obj_critical_before = Lcda_Was_Object_Critical_Before(obj_idx, prev_obj_index);

   /** \assert Check that the object was critical before. */
   EXPECT_TRUE(was_obj_critical_before && expected_was_obj_critical_before);
}

/**
 * Create a situation in which the object was critical before on both sides. Check if Lcda_Was_Object_Critical_Before asserts that
 * object was critical before. \uts{CSCSA-42550} \sdd{SF-6814} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Was_Object_Critical_Before__previously_critical_object_both)
{
   /** \arrange Set up previous object information */
   boolean_T expected_was_obj_critical_before = FBK_TRUE;
   boolean_T was_obj_critical_before          = FBK_FALSE;

   uint8_t prev_obj_index[FBK_NUMBER_OF_SIDES]{};
   uint8_t obj_idx = 10u;
   /* prev_obj_index[FBK_SIDE_LEFT] or prev_obj_index[FBK_SIDE_RIGHT] have to be equal to obj_idx for object to be critical
    * before */
   prev_obj_index[FBK_SIDE_LEFT]  = 10u;
   prev_obj_index[FBK_SIDE_RIGHT] = 10u;

   /** \action Call Lcda_Was_Object_Critical_Before to see if object was critical before. */
   was_obj_critical_before = Lcda_Was_Object_Critical_Before(obj_idx, prev_obj_index);

   /** \assert Check that the object was critical before. */
   EXPECT_TRUE(was_obj_critical_before && expected_was_obj_critical_before);
}
/**
 * Create a situation in which the object was not critical before. Check if Lcda_Was_Object_Critical_Before asserts that object was
 * not critical before. \uts{CSCSA-42551} \sdd{SF-6814} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Was_Object_Critical_Before__no_previously_critical_object)
{
   /** \arrange Set up previous object information */
   boolean_T expected_was_obj_critical_before = FBK_FALSE;
   boolean_T was_obj_critical_before          = FBK_TRUE;

   uint8_t prev_obj_index[FBK_NUMBER_OF_SIDES]{};
   uint8_t obj_idx = 10u;
   /* prev_obj_index[FBK_SIDE_LEFT] or prev_obj_index[FBK_SIDE_RIGHT] have to be equal to obj_idx for object to be critical
    * before */
   prev_obj_index[FBK_SIDE_LEFT]  = 8u;
   prev_obj_index[FBK_SIDE_RIGHT] = 9u;

   /** \action Call Lcda_Was_Object_Critical_Before to see if object was critical before. */
   was_obj_critical_before = Lcda_Was_Object_Critical_Before(obj_idx, prev_obj_index);

   /** \assert Check that the object was critical before. */
   EXPECT_EQ(was_obj_critical_before, expected_was_obj_critical_before);
}

/**
 * Create a situation in which use the cvw lane change intention zone. Compute the existence probability hysteresis.
 * \uts{CSCSA-42552} \sdd{SF-6813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Mode_Dependent_Existence_Prob_Hys__with_cvw_lane_change_intention_zone)
{
   /** \arrange Set up cvw lane change intention zone information */
   float32_T existence_prob_hyst          = 0.0f;
   float32_T expected_existence_prob_hyst = 0.0f;

   /* If f_use_cvw_lane_change_intention_zone is true
    * k_lcda_min_exist_prob_lc_intention will be used to set existence probability threshold*/
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
   lcda_cals.k_lcda_exist_prob_lc_intention_hys_offset                = 0.1f;

   expected_existence_prob_hyst = lcda_cals.k_lcda_exist_prob_lc_intention_hys_offset;

   /** \action Call Lcda_Get_Mode_Dependent_Existence_Prob_Hys to calculate the existence probability threshold. */
   existence_prob_hyst = Lcda_Get_Mode_Dependent_Existence_Prob_Hys(&lcda_core_input, &lcda_cals);

   /** \assert Check that the expected probability hysteresis equals the hysteresis given by calling
    * Lcda_Get_Mode_Dependent_Existence_Prob_Hys. */
   EXPECT_FLOAT_EQ(existence_prob_hyst, expected_existence_prob_hyst);
}

/**
 * Create a situation in which do not use the cvw lane change intention zone. Compute the existence probability hysteresis.
 * \uts{CSCSA-42553} \sdd{SF-6813} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Mode_Dependent_Existence_Prob_Hys__without_cvw_lane_change_intention_zone)
{
   /** \arrange Set up cvw lane change intention zone information */
   float32_T existence_prob_hyst          = 0.0f;
   float32_T expected_existence_prob_hyst = 0.0f;


   /* If f_use_cvw_lane_change_intention_zone is true
    * k_lcda_min_exist_prob_lc_intention will be used to set existence probability threshold*/
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;
   lcda_cals.k_lcda_exist_prob_hys_offset                             = 0.1f;

   expected_existence_prob_hyst = lcda_cals.k_lcda_exist_prob_hys_offset;

   /** \action Call Lcda_Get_Mode_Dependent_Existence_Prob_Hys to calculate the existence probability hysteresis. */
   existence_prob_hyst = Lcda_Get_Mode_Dependent_Existence_Prob_Hys(&lcda_core_input, &lcda_cals);

   /** \assert Check that the expected probability hysteresis equals the hysteresis given by calling
    * Lcda_Get_Mode_Dependent_Existence_Prob_Hys. */
   EXPECT_FLOAT_EQ(existence_prob_hyst, expected_existence_prob_hyst);
}

/**
 * Create a situation in which do use the cvw lane change intention zone. Compute the minimal existence probability threshold.
 * \uts{CSCSA-42554} \sdd{SF-6815} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Mode_Dependent_Min_Threshold__with_cvw_lane_change_intention_zone)
{
   /** \arrange Set up cvw lane change intention zone information */
   float32_T min_existence_prob_threshold          = 0.0f;
   float32_T expected_min_existence_prob_threshold = 0.0f;

   /* If f_use_cvw_lane_change_intention_zone is true
    * k_lcda_min_exist_prob_lc_intention will be used to set existence probability threshold*/
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
   lcda_cals.k_lcda_min_exist_prob_lc_intention                       = 0.1f;

   expected_min_existence_prob_threshold = lcda_cals.k_lcda_min_exist_prob_lc_intention;

   /** \action Call Lcda_Get_Mode_Dependent_Min_Threshold to calculate the existence probability threshold. */
   min_existence_prob_threshold = Lcda_Get_Mode_Dependent_Min_Threshold(&lcda_core_input, &lcda_cals);

   /** \assert Check that the expected minimal probability threshold equals the hysteresis given by calling
    * Lcda_Get_Mode_Dependent_Min_Threshold. */
   EXPECT_FLOAT_EQ(min_existence_prob_threshold, expected_min_existence_prob_threshold);
}

/**
 * Create a situation in which do not use the cvw lane change intention zone. Compute the minimal existence probability threshold.
 * \uts{CSCSA-42555} \sdd{SF-6815} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Mode_Dependent_Min_Threshold__without_cvw_lane_change_intention_zone)
{
   /** \arrange Set up cvw lane change intention zone information */
   float32_T min_existence_prob_threshold          = 0.0f;
   float32_T expected_min_existence_prob_threshold = 0.0f;

   /* If f_use_cvw_lane_change_intention_zone is true
    * k_lcda_min_exist_prob_lc_intention will be used to set existence probability threshold*/
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;
   lcda_cals.k_lcda_min_exist_prop                                    = 0.1f;

   expected_min_existence_prob_threshold = lcda_cals.k_lcda_min_exist_prop;

   /** \action Call Lcda_Get_Mode_Dependent_Min_Threshold to calculate the existence probability threshold. */
   min_existence_prob_threshold = Lcda_Get_Mode_Dependent_Min_Threshold(&lcda_core_input, &lcda_cals);

   /** \assert Check that the expected minimal probability threshold equals the hysteresis given by calling
    * Lcda_Get_Mode_Dependent_Min_Threshold. */
   EXPECT_FLOAT_EQ(min_existence_prob_threshold, expected_min_existence_prob_threshold);
}


/**
 * Check that the opposite side is returned.
 * \uts{CSCSA-42556} \sdd{SF-6929} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Opposite_Side__return_left_for_right)
{
   /** \arrange Set up side variable. */
   uint8_t side = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Get_Opposite_Side. */
   uint8_t returned_side = Lcda_Get_Opposite_Side(side);

   /** \assert Check that the opposite side is returned. */
   EXPECT_EQ(returned_side, FBK_SIDE_LEFT);
}

/**
 * Check that the opposite side is returned.
 * \uts{CSCSA-42557} \sdd{SF-6929} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Opposite_Side__return_right_for_left)
{
   /** \arrange Set up side variable. */
   uint8_t side = FBK_SIDE_LEFT;

   /** \action Call Lcda_Get_Opposite_Side. */
   uint8_t returned_side = Lcda_Get_Opposite_Side(side);

   /** \assert Check that the opposite side is returned. */
   EXPECT_EQ(returned_side, FBK_SIDE_RIGHT);
}

/**
 * Check that minima and maxima of the zones dimensions are returned.
 * \uts{CSCSA-42558} \sdd{SF-6953} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Zone_Maxima__check_maxima)
{
   /** \arrange Set up zone and optima. */
   Fbk_Field_Of_Interest_T zone;
   float32_T long_zone_min = -2.0f;
   float32_T long_zone_max = -2.0f;
   float32_T lat_zone_max  = 2.5f;
   Lcda_Create_Default_Zone(&zone);

   /** \action Call Lcda_Get_Zone_Maxima */
   Lcda_Get_Zone_Maxima(&long_zone_min, &long_zone_max, &lat_zone_max, &zone);

   /** \assert Check that optima are set. */
   EXPECT_FLOAT_EQ(-12.0f, long_zone_min);
   EXPECT_FLOAT_EQ(-2.0f, long_zone_max);
   EXPECT_FLOAT_EQ(2.5f, lat_zone_max);
}

/**
 * Create a tracker object. Set the tracker object position such that the reference point is within the zone longitudinally. Call
 * Lcda_Set_Ref_Position_Longitudinal and verify that it returns the tracker object's center. \uts{CSCSA-42559} \sdd{SF-6952}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Set_Ref_Position_Longitudinal__returns_center_point_if_it_is_in_zone)
{
   /** \arrange Create zone and tracker object. */
   float32_T long_zone_min = -12.0f;
   float32_T long_zone_max = -2.0f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 5.0f;

   obj_center.x = -4.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Longitudinal to compute longitudinal information on the critical point from the tracker
    * object. */
   Lcda_Set_Ref_Position_Longitudinal(&result, &ref_position, &obj_center, long_zone_min, long_zone_max, obj_heading, obj_length);

   /** \assert Check that critical point equals the tracker object's center. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x);
   EXPECT_EQ(LCDA_OBJ_USE_CENTER, ref_position);
}

/**
 * Create a tracker object. Set the tracker object position such that the tracker object is within the zone but the reference point
 * behind the zone. Call Lcda_Set_Ref_Position_Longitudinal and verify that it returns the tracker object's front.
 * \uts{CSCSA-42560} \sdd{SF-6952} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Set_Ref_Position_Longitudinal__returns_front_center_point_if_it_is_in_zone_and_center_point_behind_zone)
{
   /** \arrange Create zone and tracker object. */
   float32_T long_zone_min = -12.0f;
   float32_T long_zone_max = -2.0f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 5.0f;

   obj_center.x = -14.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Longitudinal to compute longitudinal information on the critical point from the tracker
    * object. */
   Lcda_Set_Ref_Position_Longitudinal(&result, &ref_position, &obj_center, long_zone_min, long_zone_max, obj_heading, obj_length);

   /** \assert Check that critical point equals the tracker object's front. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x + obj_length / 2.0f);
   EXPECT_EQ(LCDA_OBJ_USE_FRONT, ref_position);
}

/**
 * Create a tracker object. Set the tracker object position such that the tracker object is within the zone but the reference point
 * in front of the zone. Call Lcda_Set_Ref_Position_Longitudinal and verify that it returns the tracker object's rear.
 * \uts{CSCSA-42561} \sdd{SF-6952} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Set_Ref_Position_Longitudinal__returns_rear_center_point_if_it_is_in_zone_and_center_point_in_front_of_zone)
{
   /** \arrange Create zone and tracker object. */
   float32_T long_zone_min = -12.0f;
   float32_T long_zone_max = -2.0f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 5.0f;

   obj_center.x = 0.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Longitudinal to compute longitudinal information on the critical point from the tracker
    * object. */
   Lcda_Set_Ref_Position_Longitudinal(&result, &ref_position, &obj_center, long_zone_min, long_zone_max, obj_heading, obj_length);

   /** \assert Check that critical point equals the tracker object's rear. */
   EXPECT_FLOAT_EQ(result.x, obj_center.x - obj_length / 2.0f);
   EXPECT_EQ(LCDA_OBJ_USE_REAR, ref_position);
}

/**
 * Create a tracker object. Set the tracker object position such that the tracker object is within the zone but the tracker objects
 * length such that all reference points are outside the zone. Call Lcda_Set_Ref_Position_Longitudinal and verify that it returns
 * the zone's center. \uts{CSCSA-42562} \sdd{SF-6952} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Set_Ref_Position_Longitudinal__returns_center_point_at_zone_center_if_all_other_candidates_are_outside_of_zone_case1)
{
   /** \arrange Create zone and tracker object. */
   float32_T long_zone_min = -12.0f;
   float32_T long_zone_max = -2.0f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 22.0f;

   obj_center.x = -12.5f;
   obj_center.y = 2.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Longitudinal to compute lognitudinal information on the critical point from the tracker
    * object. */
   Lcda_Set_Ref_Position_Longitudinal(&result, &ref_position, &obj_center, long_zone_min, long_zone_max, obj_heading, obj_length);

   /** \assert Check that critical point equals the zone's center. */
   EXPECT_FLOAT_EQ(result.x, long_zone_max - (0.5f * (long_zone_max - long_zone_min)));
   EXPECT_EQ(LCDA_OBJ_USE_ZONECENTER, ref_position);
}

/**
 * Create a tracker object. Set the tracker object position such that the tracker object is within the zone but the tracker objects
 * length such that all reference points are outside the zone. Call Lcda_Set_Ref_Position_Longitudinal and verify that it returns
 * the zone's center. \uts{CSCSA-42563} \sdd{SF-6952} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test,
       Lcda_Set_Ref_Position_Longitudinal__returns_center_point_at_zone_center_if_all_other_candidates_are_outside_of_zone_case2)
{
   /** \arrange Create zone and tracker object. */
   float32_T long_zone_min = -12.0f;
   float32_T long_zone_max = -2.0f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position;
   Vector_2d_T result;
   float32_T obj_heading = 0.0f;
   float32_T obj_length  = 22.0f;

   obj_center.x = -1.5f;
   obj_center.y = 2.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Longitudinal to compute lognitudinal information on the critical point from the tracker
    * object. */
   Lcda_Set_Ref_Position_Longitudinal(&result, &ref_position, &obj_center, long_zone_min, long_zone_max, obj_heading, obj_length);

   /** \assert Check that critical point equals the zone's center. */
   EXPECT_FLOAT_EQ(result.x, long_zone_max - (0.5f * (long_zone_max - long_zone_min)));
   EXPECT_EQ(LCDA_OBJ_USE_ZONECENTER, ref_position);
}

/**
 * Create a tracker object. Set the reference position position to use the object's front. Call Lcda_Set_Ref_Position_Lateral and
 * verify that it returns the object's front. \uts{CSCSA-42564} \sdd{SF-6954} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Set_Ref_Position_Lateral__case_object_front)
{
   /** \arrange Create zone and tracker object. */
   float32_T lat_zone_max = 2.5f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position = LCDA_OBJ_USE_FRONT;
   Vector_2d_T result;
   float32_T obj_heading = 0.1f;
   float32_T obj_length  = 22.0f;
   float32_T obj_width   = 2.0f;

   obj_center.x = -14.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Lateral to compute lateral information on the critical point from the tracker object. */
   Lcda_Set_Ref_Position_Lateral(&result, &lcda_cals, &obj_center, lat_zone_max, obj_heading, obj_width, obj_length, ref_position);

   /** \assert Check that critical point lateral coordinate equals object's front. */
   EXPECT_FLOAT_EQ(result.y, obj_center.y + (0.5f * obj_length * Fast_Sin(obj_heading)));
}

/**
 * Create a tracker object. Set the reference position position to use the object's rear. Call Lcda_Set_Ref_Position_Lateral and
 * verify that it returns the object's rear. \uts{CSCSA-42565} \sdd{SF-6954} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Set_Ref_Position_Lateral__case_object_rear)
{
   /** \arrange Create zone and tracker object. */
   float32_T lat_zone_max = 2.5f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position = LCDA_OBJ_USE_REAR;
   Vector_2d_T result;
   float32_T obj_heading = 0.1f;
   float32_T obj_length  = 22.0f;
   float32_T obj_width   = 2.0f;

   obj_center.x = -14.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Lateral to compute lateral information on the critical point from the tracker object. */
   Lcda_Set_Ref_Position_Lateral(&result, &lcda_cals, &obj_center, lat_zone_max, obj_heading, obj_width, obj_length, ref_position);

   /** \assert Check that critical point lateral coordinate equals object's rear. */
   EXPECT_FLOAT_EQ(result.y, obj_center.y - (0.5f * obj_length * Fast_Sin(obj_heading)));
}

/**
 * Create a tracker object. Set the reference position position to use the object's center. Call Lcda_Set_Ref_Position_Lateral and
 * verify that it returns the object's center. \uts{CSCSA-42566} \sdd{SF-6954} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Set_Ref_Position_Lateral__case_object_center)
{
   /** \arrange Create zone and tracker object. */
   float32_T lat_zone_max = 2.5f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position = LCDA_OBJ_USE_CENTER;
   Vector_2d_T result;
   float32_T obj_heading = 0.1f;
   float32_T obj_length  = 22.0f;
   float32_T obj_width   = 2.0f;

   obj_center.x = -14.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Lateral to compute lateral information on the critical point from the tracker object. */
   Lcda_Set_Ref_Position_Lateral(&result, &lcda_cals, &obj_center, lat_zone_max, obj_heading, obj_width, obj_length, ref_position);

   /** \assert Check that critical point equals the objects's center. */
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/**
 * Create a tracker object. Set the reference position position to use the zone's center. Call Lcda_Set_Ref_Position_Lateral and
 * verify that it returns the zone's center. \uts{CSCSA-42567} \sdd{SF-6954} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Set_Ref_Position_Lateral__case_zone_center)
{
   /** \arrange Create zone and tracker object. */
   float32_T lat_zone_max = 2.5f;
   Vector_2d_T obj_center;
   Lcda_Obj_Ref_Point_T ref_position = LCDA_OBJ_USE_ZONECENTER;
   Vector_2d_T result;
   float32_T obj_heading = 0.1f;
   float32_T obj_length  = 22.0f;
   float32_T obj_width   = 2.0f;

   obj_center.x = -14.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action Call Lcda_Set_Ref_Position_Lateral to compute lateral information on the critical point from the tracker object. */
   Lcda_Set_Ref_Position_Lateral(&result, &lcda_cals, &obj_center, lat_zone_max, obj_heading, obj_width, obj_length, ref_position);

   /** \assert Check that critical point equals the zone's center. */
   EXPECT_FLOAT_EQ(result.y, obj_center.y);
}

/*
 * Check that zone width is limited by lane width.
 * \uts{CSCSA-70062} \sdd{SF-6994} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Limit_Outer_Zone_Points__zone_is_limited_by_lw)
{
   /** \arrange Create zone and lane width. */
   Fbk_Field_Of_Interest_T zone{};
   Lcda_Create_Default_Zone(&zone);
   zone.points[FRONT_OUTER_SIDE].y  = 9.0;
   zone.points[MIDDLE_OUTER_SIDE].y = 9.0;
   zone.points[REAR_OUTER_SIDE].y   = 9.0;
   float32_T lane_width             = 1.5f;

   /** \action Call function Lcda_Limit_Outer_Zone_Points to get zone outer side position. */
   Lcda_Limit_Outer_Zone_Points(&zone, lane_width, &lcda_cals);

   /** \assert Verify that returned zone poistion are as expected. */
   EXPECT_FLOAT_EQ(zone.points[FRONT_OUTER_SIDE].y, Fbk_Half(lane_width) + lcda_cals.k_lcda_max_lane_width);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_OUTER_SIDE].y, Fbk_Half(lane_width) + lcda_cals.k_lcda_max_lane_width);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, Fbk_Half(lane_width) + lcda_cals.k_lcda_max_lane_width);
}

/*
 * Check that zone width is not limited by lane width, becuase of to large lane width.
 * \uts{CSCSA-70063} \sdd{SF-6994} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Limit_Outer_Zone_Points__zone_is_not_limited_by_lw)
{
   /** \arrange Create zone and lane width. */
   Fbk_Field_Of_Interest_T zone{};
   Lcda_Create_Default_Zone(&zone);
   zone.points[FRONT_OUTER_SIDE].y  = 9.0;
   zone.points[MIDDLE_OUTER_SIDE].y = 9.0;
   zone.points[REAR_OUTER_SIDE].y   = 9.0;
   float32_T lane_width             = 3.5f;

   /** \action Call function Lcda_Limit_Outer_Zone_Points to get zone outer side position. */
   Lcda_Limit_Outer_Zone_Points(&zone, lane_width, &lcda_cals);

   /** \assert Verify that returned zone poistion are as expected. */
   EXPECT_FLOAT_EQ(zone.points[FRONT_OUTER_SIDE].y, zone.points[FRONT_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_OUTER_SIDE].y, zone.points[FRONT_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, zone.points[FRONT_OUTER_SIDE].y);
}

/*
 * Check that object front position is calculated correctly by corresponding function.
 * \uts{CSCSA-42568} \sdd{SF-6678} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Obj_Front_Position__returns_correct_position)
{
   /** \arrange Set up CVW object with position and length. */
   float32_T obj_front_position;
   lcda_tracker_object.curvi_pos.x = -7.0f;
   lcda_tracker_object.length      = 4.0f;

   /** \action Call function Lcda_Get_Obj_Front_Position to get object front position. */
   obj_front_position = Lcda_Get_Obj_Front_Position(&lcda_tracker_object);

   /** \assert Verify that returned object front position is correct. */
   EXPECT_FLOAT_EQ(obj_front_position, lcda_tracker_object.curvi_pos.x + 0.5f * lcda_tracker_object.length);
}

/*
 * Check that object front lateral position is calculated correctly by corresponding function.
 * \uts{CSCSA-42569} \sdd{CSCSA-70150} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Obj_Side_Distance_Lateral__returns_correct_position_negative)
{
   /** \arrange Set up CVW object with position and length. */
   float32_T obj_front_position;
   lcda_tracker_object.curvi_pos.y = -7.0f;
   lcda_tracker_object.width       = 4.0f;

   /** \action Call function Lcda_Get_Obj_Side_Distance_Lateral to get object front position. */
   obj_front_position = Lcda_Get_Obj_Side_Distance_Lateral(&lcda_tracker_object);

   /** \assert Verify that returned object front position is correct. */
   EXPECT_FLOAT_EQ(obj_front_position, (Fbk_Abs_F(lcda_tracker_object.curvi_pos.y) - Fbk_Half(lcda_tracker_object.width)));
}

/**
 * Check that object front lateral position is calculated correctly by corresponding function.
 * \uts{CSCSA-42570} \sdd{CSCSA-70150} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Obj_Side_Distance_Lateral__returns_correct_position_positive)
{
   /** \arrange Set up CVW object with position and length. */
   float32_T obj_front_position;
   lcda_tracker_object.curvi_pos.y = 7.0f;
   lcda_tracker_object.width       = 4.0f;

   /** \action Call function Lcda_Get_Obj_Side_Distance_Lateral to get object front position. */
   obj_front_position = Lcda_Get_Obj_Side_Distance_Lateral(&lcda_tracker_object);

   /** \assert Verify that returned object front position is correct. */
   EXPECT_FLOAT_EQ(obj_front_position, (Fbk_Abs_F(lcda_tracker_object.curvi_pos.y) - Fbk_Half(lcda_tracker_object.width)));
}

/**
 * Expect assertion if wrong position is passed
 * \uts{CSCSA-42571} \sdd{SF-6954} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Set_Ref_Position_Lateral__wrong_position)
{
   /** \arrange Create zone and tracker object. */
   float32_T lat_zone_max = 2.5f;
   Vector_2d_T obj_center;
   Vector_2d_T result;
   float32_T obj_heading = 0.1f;
   float32_T obj_length  = 22.0f;
   float32_T obj_width   = 2.0f;

   obj_center.x = -14.0f;
   obj_center.y = 1.0f;
   result       = obj_center;

   /** \action */
   /** \assert */
   EXPECT_DEBUG_DEATH(
      {
         Lcda_Set_Ref_Position_Lateral(&result, &lcda_cals, &obj_center, lat_zone_max, obj_heading, obj_width, obj_length,
                                       (Lcda_Obj_Ref_Point_T) 10u);
      },
      "");
}

/**
 * Check that minima and maxima of the zones dimensions are returned, start with low lat max .
 * \uts{CSCSA-42572} \sdd{SF-6953} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Zone_Maxima__check_maxima_get_y_point)
{
   /** \arrange Set up zone and optima. */
   Fbk_Field_Of_Interest_T zone;
   float32_T long_zone_min = -2.0f;
   float32_T long_zone_max = -2.0f;
   float32_T lat_zone_max  = 0.25f;
   Lcda_Create_Default_Zone(&zone);
   zone.points[2].y = 3.0f;

   /** \action Call Lcda_Get_Zone_Maxima */
   Lcda_Get_Zone_Maxima(&long_zone_min, &long_zone_max, &lat_zone_max, &zone);

   /** \assert Check that optima are set. */
   EXPECT_FLOAT_EQ(-12.0f, long_zone_min);
   EXPECT_FLOAT_EQ(-2.0f, long_zone_max);
   EXPECT_FLOAT_EQ(3.0f, lat_zone_max);
}

/**
 * Create a tracker object negative long speed and positive position value LCDA_DEFAULT_LARGE_TTC.
 * \uts{CSCSA-42573} \sdd{SF-6562} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttc__returns_default_large_ttc_when_negative_vel_positive_pos)
{
   /** \arrange Set target objects properties. */
   float32_T ego_length                = 5.0f;
   float32_T result                    = 0.0f;
   lcda_tracker_object.curvi_vel_rel.x = -10.0f;
   lcda_tracker_object.length          = 6.0f;
   lcda_tracker_object.curvi_pos.x     = 15.0f;

   /** \action Call Lcda_Get_Longitudinal_Ttc to calculate the TTC of the target object. */
   result = Lcda_Get_Longitudinal_Ttc(&lcda_tracker_object, ego_length);

   /** \assert Check that the TTC equals LCDA_DEFAULT_LARGE_TTC. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Create a tracker object positive long speed and positive position value LCDA_DEFAULT_LARGE_TTC.
 * \uts{CSCSA-42574} \sdd{SF-6562} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttc__returns_default_large_ttc_when_positive_vel_positive_pos)
{
   /** \arrange Set target objects properties. */
   float32_T ego_length                = 5.0f;
   float32_T result                    = 0.0f;
   lcda_tracker_object.curvi_vel_rel.x = 0.0f;
   lcda_tracker_object.length          = 6.0f;
   lcda_tracker_object.curvi_pos.x     = 15.0f;

   /** \action Call Lcda_Get_Longitudinal_Ttc to calculate the TTC of the target object. */
   result = Lcda_Get_Longitudinal_Ttc(&lcda_tracker_object, ego_length);

   /** \assert Check that the TTC equals LCDA_DEFAULT_LARGE_TTC. */
   EXPECT_FLOAT_EQ(result, LCDA_DEFAULT_LARGE_TTC);
}

/**
 * Check that suitable object is correctly identified as in ego lane. Different sets of parameters for FBK_Clamped function are
 * tested to increase branch coverage. \uts{CSCSA-42575} \sdd{SF-6565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Is_Object_In_Ego_Lane__clamp_test)
{
   /** \arrange Set up lane width, lane width factor and object position such that object is left of ego and inside ego lane. */
   float32_T lane_width                                  = 4.0f;
   Lcda_Coordinate_System_T coordinate_system            = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.y                         = -1.9f;
   lcda_cals.k_lcda_ego_lane_check_center_point_only     = FBK_TRUE;
   lcda_cals.k_lcda_ego_lane_effective_lane_width_factor = 1.0f;

   /** \action Call function to evaluate if object is in ego lane. */
   boolean_T f_in_ego_lane_1 = Lcda_Is_Object_In_Ego_Lane(lane_width, &lcda_tracker_object, &lcda_cals, coordinate_system);
   boolean_T f_in_ego_lane_2 = Lcda_Is_Object_In_Ego_Lane(1.0f, &lcda_tracker_object, &lcda_cals, coordinate_system);
   boolean_T f_in_ego_lane_3 = Lcda_Is_Object_In_Ego_Lane(8.0f, &lcda_tracker_object, &lcda_cals, coordinate_system);
   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(f_in_ego_lane_1);
   EXPECT_FALSE(f_in_ego_lane_2);
   EXPECT_TRUE(f_in_ego_lane_3);
}

/*
 * Test for Lcda_Get_Object_Location_Data, Lcda_Object_Location_Data_T is set based on reference point
 * \uts{CSCSA-42576} \sdd{CSCSA-27603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Object_Location_Data__ref_point)
{

   /** \arrange Set up inputs for Lcda_Get_Object_Location_Data: bsw_object, p_lcda_cals, coordinate_system, obj_loc_data */
   // Bsw_Object_T bsw_object{};
   Lcda_Object_Location_Data_T obj_loc_data;
   Lcda_Coordinate_System_T coordinate_system;
   Fbk_Field_Of_Interest_T zone_foi;
   coordinate_system                                            = LCDA_USE_CURVI;
   lcda_cals.k_lcda_zone_check_method                           = LCDA_ZONE_CHECK_REF_POINT;
   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.0f;

   lcda_tracker_object.curvi_pos.x = -5.0;
   lcda_tracker_object.curvi_pos.y = -3.0;

   float32_T zone_px[FBK_FOI_SIZE_TETRAGON];
   float32_T zone_py[FBK_FOI_SIZE_TETRAGON];

   zone_px[0] = -2.0f;
   zone_px[1] = -2.0f;
   zone_px[2] = -9.0f;
   zone_px[3] = -9.0f;
   zone_py[0] = -5.0f;
   zone_py[1] = -1.0f;
   zone_py[2] = -1.0f;
   zone_py[3] = -5.0f;
   Fbk_Create_Field_Of_Interest(&zone_foi, zone_px, zone_py, FBK_FOI_SIZE_TETRAGON);

   /** \action Call function Lcda_Get_Object_Location_Data */
   Lcda_Get_Object_Location_Data(&obj_loc_data, &lcda_tracker_object, &zone_foi, &lcda_cals, coordinate_system);

   /** \assert Verify obj_loc_data */
   EXPECT_FLOAT_EQ(obj_loc_data.obj_in_zone, FBK_TRUE);
}

/*
 * Test for Lcda_Get_Object_Location_Data, Lcda_Object_Location_Data_T is set based on overlapped area
 * \uts{CSCSA-42577} \sdd{CSCSA-27603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Object_Location_Data__overlapped)
{
   /** \arrange Set up inputs for Lcda_Get_Object_Location_Data: bsw_object, p_lcda_cals, coordinate_system, obj_loc_data */
   Lcda_Object_Location_Data_T obj_loc_data;
   Lcda_Coordinate_System_T coordinate_system;
   Fbk_Field_Of_Interest_T zone_foi;
   float32_T zone_px[FBK_FOI_SIZE_TETRAGON];
   float32_T zone_py[FBK_FOI_SIZE_TETRAGON];

   coordinate_system                                            = LCDA_USE_CURVI;
   lcda_cals.k_lcda_zone_check_method                           = LCDA_ZONE_CHECK_FOI_OVERLAP;
   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.0f;

   lcda_tracker_object.curvi_pos.x = -6.5;
   lcda_tracker_object.curvi_pos.y = -3.0;
   lcda_tracker_object.length      = 3.0f;
   lcda_tracker_object.width       = 2.0f;

   zone_px[0] = -2.0f;
   zone_px[1] = -2.0f;
   zone_px[2] = -9.0f;
   zone_px[3] = -9.0f;
   zone_py[0] = -5.0f;
   zone_py[1] = -1.0f;
   zone_py[2] = -1.0f;
   zone_py[3] = -5.0f;
   Fbk_Create_Field_Of_Interest(&zone_foi, zone_px, zone_py, FBK_FOI_SIZE_TETRAGON);

   /** \action Call function Lcda_Get_Object_Location_Data */
   Lcda_Get_Object_Location_Data(&obj_loc_data, &lcda_tracker_object, &zone_foi, &lcda_cals, coordinate_system);

   /** \assert Verify obj_loc_data */
   EXPECT_FLOAT_EQ(obj_loc_data.obj_in_zone, FBK_TRUE);
}

/*
 * Test for Lcda_Get_Object_Location_Data, Lcda_Object_Location_Data_T is set based on overlapped area
 * \uts{CSCSA-42578} \sdd{CSCSA-27603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Object_Location_Data__not_overlapped)
{
   /** \arrange Set up inputs for Lcda_Get_Object_Location_Data: bsw_object, p_lcda_cals, coordinate_system, obj_loc_data */
   Lcda_Object_Location_Data_T obj_loc_data;
   Lcda_Coordinate_System_T coordinate_system;
   Fbk_Field_Of_Interest_T zone_foi;
   float32_T zone_px[FBK_FOI_SIZE_TETRAGON];
   float32_T zone_py[FBK_FOI_SIZE_TETRAGON];

   coordinate_system                                            = LCDA_USE_CURVI;
   lcda_cals.k_lcda_zone_check_method                           = LCDA_ZONE_CHECK_FOI_OVERLAP;
   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.0f;

   lcda_tracker_object.curvi_pos.x = -12.0;
   lcda_tracker_object.curvi_pos.y = -1.0;
   lcda_tracker_object.length      = 3.0f;
   lcda_tracker_object.width       = 2.0f;

   zone_px[0] = -2.0f;
   zone_px[1] = -2.0f;
   zone_px[2] = -9.0f;
   zone_px[3] = -9.0f;
   zone_py[0] = -5.0f;
   zone_py[1] = -1.0f;
   zone_py[2] = -1.0f;
   zone_py[3] = -5.0f;
   Fbk_Create_Field_Of_Interest(&zone_foi, zone_px, zone_py, FBK_FOI_SIZE_TETRAGON);

   /** \action Call function Lcda_Get_Object_Location_Data */
   Lcda_Get_Object_Location_Data(&obj_loc_data, &lcda_tracker_object, &zone_foi, &lcda_cals, coordinate_system);

   /** \assert Verify obj_loc_data */
   EXPECT_FLOAT_EQ(obj_loc_data.obj_in_zone, FBK_FALSE);
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, the rear left corner of the object is the farthest one.
 * \uts{CSCSA-136321} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__vcs_rear_left_corner_is_extreme)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, vcs caluclation, object inside zone, heading to left */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.x              = -7.0f;
   lcda_tracker_object.vcs_pos.y              = 3.3f;
   lcda_tracker_object.vcs_heading            = -0.1f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.vcs_vel_rel.x          = 1.9f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.vcs_pos, &lcda_tracker_object.vcs_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify obj_loc_data */
   EXPECT_FLOAT_EQ(res, Fbk_Abs_F(target_corners.points[FBK_REAR_LEFT_CORNER].x / lcda_tracker_object.vcs_vel_rel.x));
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, the rear right corner of the object is the farthest one.
 * \uts{CSCSA-136322} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__vcs_rear_right_corner_is_extreme)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, vcs caluclation, object inside zone, heading to right */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.x              = -7.0f;
   lcda_tracker_object.vcs_pos.y              = 3.3f;
   lcda_tracker_object.vcs_heading            = 0.1f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.vcs_vel_rel.x          = 2.2f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.vcs_pos, &lcda_tracker_object.vcs_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify calculated TTP */
   EXPECT_FLOAT_EQ(res, Fbk_Abs_F(target_corners.points[FBK_REAR_RIGHT_CORNER].x / lcda_tracker_object.vcs_vel_rel.x));
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, the rear right corner of the object in curvi coordinates is the farthest one.
 * \uts{CSCSA-136323} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__curvi_rear_right_corner_is_extreme)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, object inside zone using curvilnear coordinates */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_CURVI;
   lcda_tracker_object.curvi_pos.x            = -23.4f;
   lcda_tracker_object.curvi_pos.y            = -2.1f;
   lcda_tracker_object.curvi_heading          = 0.2f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.curvi_vel_rel.x        = 12.9f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.curvi_pos, &lcda_tracker_object.curvi_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify calculated TTP */
   EXPECT_FLOAT_EQ(res, Fbk_Abs_F(target_corners.points[FBK_REAR_RIGHT_CORNER].x / lcda_tracker_object.curvi_vel_rel.x));
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, relative longitudinal velocity is lower than 0m/s.
 * \uts{CSCSA-136324} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__rel_vel_below_threshold)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, object inside zone, slower than host */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.x              = -7.0f;
   lcda_tracker_object.vcs_pos.y              = 3.3f;
   lcda_tracker_object.vcs_heading            = -0.1f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.vcs_vel_rel.x          = -1.9f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.vcs_pos, &lcda_tracker_object.vcs_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify calculated TTP */
   EXPECT_FLOAT_EQ(res, LCDA_DEFAULT_LARGE_TTC);
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, relative longitudinal velocity is very low, expect default output.
 * \uts{CSCSA-136325} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__very_low_rel_vel)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, set object inside zone with very low relative velocity */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.x              = -10.0f;
   lcda_tracker_object.vcs_pos.y              = 3.3f;
   lcda_tracker_object.vcs_heading            = -0.1f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.vcs_vel_rel.x          = 0.05f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.vcs_pos, &lcda_tracker_object.vcs_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify calculated TTP */
   EXPECT_FLOAT_EQ(res, LCDA_DEFAULT_LARGE_TTC);
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, relative longitudinal position is positive, relative velocity is very low, expect default
 * output. \uts{CSCSA-136326} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__long_positive_value_very_low_rel_vel)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, set object outside zone */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.x              = 10.0f;
   lcda_tracker_object.vcs_pos.y              = 3.3f;
   lcda_tracker_object.vcs_heading            = -0.1f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.vcs_vel_rel.x          = 0.05f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.vcs_pos, &lcda_tracker_object.vcs_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify calculated TTP */
   EXPECT_FLOAT_EQ(res, LCDA_DEFAULT_LARGE_TTC);
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, relative longitudinal position is positive, expects a value indicating the time since
 * crossing the threshold output. \uts{CSCSA-136327} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__long_positive_value)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, object outside zone */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.x              = 10.0f;
   lcda_tracker_object.vcs_pos.y              = 3.3f;
   lcda_tracker_object.vcs_heading            = 0.0f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.vcs_vel_rel.x          = 5.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.vcs_pos, &lcda_tracker_object.vcs_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify calculated TTP */
   EXPECT_FLOAT_EQ(res, target_corners.points[FBK_REAR_RIGHT_CORNER].x / lcda_tracker_object.vcs_vel_rel.x);
}

/*
 * Test for Lcda_Get_Longitudinal_Ttp, longitudinal position in positives.
 * \uts{CSCSA-187410} \sdd{CSCSA-136320} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Longitudinal_Ttp__vcs_longitudinal_in_positives)
{
   /** \arrange Set up inputs for Lcda_Get_Longitudinal_Ttp, vcs caluclation, object inside zone, heading to left */
   float res;
   Fbk_Object_Corners_T target_corners;
   Lcda_Coordinate_System_T coordinate_system = LCDA_USE_VCS;
   lcda_tracker_object.vcs_pos.x              = 7.0f;
   lcda_tracker_object.vcs_pos.y              = 3.3f;
   lcda_tracker_object.vcs_heading            = -0.1f;
   lcda_tracker_object.length                 = 4.8f;
   lcda_tracker_object.width                  = 1.9f;
   lcda_tracker_object.vcs_vel_rel.x          = 1.9f;

   Fbk_Calculate_Target_Corners(&target_corners, &lcda_tracker_object.vcs_pos, &lcda_tracker_object.vcs_heading,
                                &lcda_tracker_object.length, &lcda_tracker_object.width);

   /** \action Call function Lcda_Get_Longitudinal_Ttp */
   res = Lcda_Get_Longitudinal_Ttp(&lcda_tracker_object, coordinate_system);

   /** \assert Verify obj_loc_data */
   EXPECT_FLOAT_EQ(res, Fbk_Abs_F(target_corners.points[FBK_REAR_LEFT_CORNER].x / lcda_tracker_object.vcs_vel_rel.x));
}


/**
 * Checks if 0 is returned when the object's VCS relative lateral velocity is positive.
 * \uts{CSCSA-211361} \sdd{CSCSA-184205} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Ttle__vcs_relative_lateral_velocity_positive)
{
   /** \arrange Set target object properties. */
   Fbk_Field_Of_Interest_T zone      = {};
   float32_T result                  = FBK_ONE_F;
   lcda_tracker_object.vcs_vel_rel.y = FBK_ONE_F;
   zone.points[0u].y                 = -2.0f;

   /** \action Call Lcda_Get_Ttle. */
   result = Lcda_Get_Ttle(&lcda_tracker_object, &zone, LCDA_USE_VCS);

   /** \assert Check that the TTLE equals expected value. */
   EXPECT_FLOAT_EQ(result, FBK_ZERO_F);
}


/*
 * Test for Lcda_Get_Object_Location_Data, due to small object area is set to 0 to prevent from dividing by 0.
 * \uts{CSCSA-211362} \sdd{CSCSA-27603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Common_Functions_Test, Lcda_Get_Object_Location_Data__overlapped_but_area_zero)
{
   /** \arrange Set up inputs for Lcda_Get_Object_Location_Data: bsw_object, p_lcda_cals, coordinate_system, obj_loc_data */
   Lcda_Object_Location_Data_T obj_loc_data;
   Lcda_Coordinate_System_T coordinate_system;
   Fbk_Field_Of_Interest_T zone_foi;
   float32_T zone_px[FBK_FOI_SIZE_TETRAGON];
   float32_T zone_py[FBK_FOI_SIZE_TETRAGON];

   coordinate_system                                            = LCDA_USE_CURVI;
   lcda_cals.k_lcda_zone_check_method                           = LCDA_ZONE_CHECK_FOI_OVERLAP;
   lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.0f;

   lcda_tracker_object.curvi_pos.x = -6.5;
   lcda_tracker_object.curvi_pos.y = -3.0;
   lcda_tracker_object.length      = 0.001f;
   lcda_tracker_object.width       = 0.001f;

   zone_px[0] = -2.0f;
   zone_px[1] = -2.0f;
   zone_px[2] = -9.0f;
   zone_px[3] = -9.0f;
   zone_py[0] = -5.0f;
   zone_py[1] = -1.0f;
   zone_py[2] = -1.0f;
   zone_py[3] = -5.0f;
   Fbk_Create_Field_Of_Interest(&zone_foi, zone_px, zone_py, FBK_FOI_SIZE_TETRAGON);

   /** \action Call function Lcda_Get_Object_Location_Data */
   Lcda_Get_Object_Location_Data(&obj_loc_data, &lcda_tracker_object, &zone_foi, &lcda_cals, coordinate_system);

   /** \assert Verify obj_loc_data */
   EXPECT_FLOAT_EQ(obj_loc_data.obj_in_zone, FBK_TRUE);
   EXPECT_FLOAT_EQ(obj_loc_data.area_overlap_ratio, FBK_ZERO_F);
}