/**
 * @file ltb_warn_logic_test.cpp
 * @author SFL (Side Feature Logic) scrum teamltb
 * @brief Test implementation for LTB unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-46119}
 */

#include "ltb_warn_logic_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ltb_types.h"
#include "ltb_warn_logic.c"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/*
 * Set object properties of warning relevant target and verify that its properties are correctly written to the LTB core output.
 * \uts{CSCSA-46211} \sdd{CSCSA-53945} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Fill_Core_Output_With_Current_Obj__fill_core_output_correctly)
{
   /** \arrange Create warning relevant target with various properties. */
   ltb_object.attributes.alert_level   = ALERT_ACTIVE_LEVEL_1;
   ltb_object.tracker_data.id          = 1;
   ltb_object.attributes.ttc           = 1.8f;
   ltb_object.attributes.f_obj_in_zone = FBK_TRUE;

   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]   = NO_ALERT;
   ltb_core_output.ltb_id[FBK_SIDE_LEFT]            = PA_INVALID_OBJ_ID;
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]           = LTB_INVALID_TTC;
   ltb_core_output.ltb_f_obj_in_zone[FBK_SIDE_LEFT] = FBK_FALSE;

   /** \action Fill LTB core output with current object */
   Ltb_Fill_Core_Output_With_Current_Obj(&ltb_core_output, &ltb_object, FBK_SIDE_LEFT);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ltb_object.attributes.alert_level);
   EXPECT_EQ(ltb_core_output.ltb_id[FBK_SIDE_LEFT], ltb_object.tracker_data.id);
   EXPECT_EQ(ltb_core_output.ltb_ttc[FBK_SIDE_LEFT], ltb_object.attributes.ttc);
   EXPECT_EQ(ltb_core_output.ltb_f_obj_in_zone[FBK_SIDE_LEFT], ltb_object.attributes.f_obj_in_zone);
}


/*
 * Create a warning relevant target with properties which should reach alert level 1. Set object criticallity and verify it reaches
 * alert level 1. \uts{CSCSA-46212} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__check_alert_level_1)
{
   /** \arrange Create warning relevant target with various properties. */
   ltb_object.attributes.ttc = ltb_cals.k_ltb_alert_lvl_1_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_1);
}

/*
 * Create a warning relevant target with properties which should reach alert level 2. Set object criticallity and verify it reaches
 * alert level 2. \uts{CSCSA-46213} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__check_alert_level_3)
{
   /** \arrange Create warning relevant target with various properties. */
   ltb_object.attributes.ttc = ltb_cals.k_ltb_alert_lvl_2_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_2);
}

/*
 * Create a warning relevant target with properties which should reach alert level 3. Set object criticallity and verify it reaches
 * alert level 3. \uts{CSCSA-46214} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__check_alert_level_4)
{
   /** \arrange Create warning relevant target with various properties. */
   ltb_object.attributes.ttc                 = ltb_cals.k_ltb_alert_lvl_3_ttc_threshold - 0.1f;
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold + 0.1f;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_3);
}

/*
 * Create a warning relevant target with properties which should normally reach alert level 1. Set object status to COASTED and
 * verify that it does reach alert. \uts{CSCSA-46215} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__check_alert_level_1_for_coasted_object)
{
   /** \arrange Create warning relevant target with various properties. */
   ltb_object.attributes.ttc      = ltb_cals.k_ltb_alert_lvl_1_ttc_threshold - 0.1f;
   ltb_object.tracker_data.status = PA_OBJ_STATUS_COASTED;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_1);
}

/*
 * Create a warning relevant target with properties which should normally reach alert level 2. Set object status to MATURE and
 * verify that it does reach alert. \uts{CSCSA-46216} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__check_alert_level_1_for_mature_object)
{
   /** \arrange Create warning relevant target with various properties. */
   ltb_object.attributes.ttc      = ltb_cals.k_ltb_alert_lvl_1_ttc_threshold - 0.1f;
   ltb_object.tracker_data.status = PA_OBJ_STATUS_MATURE;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_1);
}


/*
 * Create a warning relevant target on the left side. Verify that warning level is set on correct side.
 * \uts{CSCSA-46217} \sdd{CSCSA-53942} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Object_Criticality__check_alert_side_left)
{
   /** \arrange Create warning relevant target on the left side. */
   ltb_object.tracker_data.vcs_pos.y = -1.0f;
   ltb_object.attributes.ttc         = ltb_cals.k_ltb_alert_lvl_1_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ltb_Set_Object_Criticality(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_LEFT);
}

/*
 * Create a warning relevant target on the right side. Verify that warning level is set on correct side.
 * \uts{CSCSA-46218} \sdd{CSCSA-53942} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Object_Criticality__check_alert_side_right)
{
   /** \arrange Create warning relevant target on the right side. */
   ltb_object.tracker_data.vcs_pos.y = 1.0f;
   ltb_object.attributes.ttc         = ltb_cals.k_ltb_alert_lvl_1_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ltb_Set_Object_Criticality(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests ttc set to its invalid default value LTB_INVALID_TTC. Object shall not have an alert state here.
 * \uts{CSCSA-46219} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_equals_invalid_default_ttc)
{
   /** \arrange Create target which is not warning relevant. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold;
   ltb_object.attributes.ttc                 = LTB_INVALID_TTC;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, NO_ALERT);
}

/*
 * Tests ttc set to its invalid default value LTB_INVALID_TTC - EPSILON. Object shall not have an alert state here.
 * \uts{CSCSA-46222} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__lt_boundary_test_ttc_equals_invalid_default_ttc)
{
   /** \arrange Create target which is not warning relevant. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold;
   ltb_object.attributes.ttc                 = LTB_INVALID_TTC - EPSILON;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, NO_ALERT);
}


/*
 * Test the exact boundary value for ttc based alert state level calculation for state level 3. Object shall have an alert state in
 * this case. \uts{CSCSA-46228} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_obj_valid_for_alert_level_3)
{
   /** \arrange Create warning relevant target. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold;
   ltb_object.attributes.ttc                 = ltb_cals.k_ltb_alert_lvl_3_ttc_threshold;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_3);
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 3. Object shall not have
 * alert state of level 3 in this case. \uts{CSCSA-46233} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__gt_boundary_test_ttc_obj_invalid_for_alert_level_3)
{
   /** \arrange Create warning relevant target. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold;
   ltb_object.attributes.ttc                 = ltb_cals.k_ltb_alert_lvl_3_ttc_threshold + EPSILON;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_TRUE((ltb_object.attributes.alert_level < ALERT_ACTIVE_LEVEL_3));
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 2. Object shall have an
 * alert state level 2 in this case. \uts{CSCSA-46238} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_obj_valid_for_alert_level_2)
{
   /** \arrange Create warning relevant target. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold - EPSILON;
   ltb_object.attributes.ttc                 = ltb_cals.k_ltb_alert_lvl_2_ttc_threshold;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_2);
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 2. Object shall not have
 * alert state of level 2 in this case. \uts{CSCSA-46243} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__gt_boundary_test_ttc_obj_invalid_for_alert_level_2)
{
   /** \arrange Create warning relevant target. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold - EPSILON;
   ltb_object.attributes.ttc                 = ltb_cals.k_ltb_alert_lvl_2_ttc_threshold + EPSILON;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_TRUE((ltb_object.attributes.alert_level < ALERT_ACTIVE_LEVEL_2));
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 1. Object shall have an
 * alert state in this case. \uts{CSCSA-46247} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_obj_valid_for_alert_level_1)
{
   /** \arrange Create warning relevant target. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold - EPSILON;
   ltb_object.attributes.ttc                 = ltb_cals.k_ltb_alert_lvl_1_ttc_threshold;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, ALERT_ACTIVE_LEVEL_1);
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 1. Object shall not have
 * alert state of level 1 in this case. \uts{CSCSA-46250} \sdd{CSCSA-53946} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Ttc_Based_Alert_Level__gt_boundary_test_ttc_obj_invalid_for_alert_level_1)
{
   /** \arrange Create target which is not warn relevant. */
   ltb_object.attributes.decel_to_avoid_coll = ltb_cals.k_ltb_alert_lvl_3_decel_threshold - EPSILON;
   ltb_object.attributes.ttc                 = ltb_cals.k_ltb_alert_lvl_1_ttc_threshold + EPSILON;

   /** \action Set object criticallity */
   Ltb_Set_Ttc_Based_Alert_Level(&ltb_object, &ltb_cals);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_level, NO_ALERT);
}


/*
 * Tests the exact boundary of lateral position condition. Object shall have an alert level of 2. Here the vcs coordinates shall be
 * used for determination of alert_side and the alert side shall be the right side. \uts{CSCSA-46252} \sdd{CSCSA-53947}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Obj_Position_Based_Alert_Side__exact_boundary_test_lateral_position_obj_valid_for_alert_level_2_vcs)
{
   /** \arrange Create warn relevant target on right side. */
   ltb_object.tracker_data.vcs_pos.y = FBK_ZERO_F;
   ltb_object.attributes.alert_level = ALERT_ACTIVE_LEVEL_2;

   /** \action Set object criticallity */
   Ltb_Set_Obj_Position_Based_Alert_Side(&ltb_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests a value lower than the boundary of lateral position condition. Object shall have an alert level of 2. Here the vcs
 * coordinates shall be used for determination of alert_side and the alert side shall be the left side. \uts{CSCSA-46255}
 * \sdd{CSCSA-53947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Obj_Position_Based_Alert_Side__lt_boundary_test_lateral_position_obj_valid_for_alert_level_2_vcs)
{
   /** \arrange Create warn relevant target on left side. */
   ltb_object.attributes.alert_level = ALERT_ACTIVE_LEVEL_2;
   ltb_object.tracker_data.vcs_pos.y = -EPSILON;

   /** \action Set object criticallity */
   Ltb_Set_Obj_Position_Based_Alert_Side(&ltb_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_LEFT);
}


/*
 * Tests the exact boundary of lateral position condition for determination of alert side. Object shall have an alert level of 1.
 * Here the curvi coordinates shall be used for determination of alert_side and the alert side shall be the right side.
 * \uts{CSCSA-46257} \sdd{CSCSA-53947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test,
       Ltb_Set_Obj_Position_Based_Alert_Side__exact_boundary_test_lateral_position_obj_valid_for_alert_level_1_curvi)
{
   /** \arrange Create warn relevant target on right side. */
   ltb_object.tracker_data.curvi_pos.y     = FBK_ZERO_F;
   ltb_object.attributes.alert_level       = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.f_curvi_available = FBK_TRUE;

   /** \action Set object criticallity */
   Ltb_Set_Obj_Position_Based_Alert_Side(&ltb_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests a value less than the boundary of lateral position condition for determination of alert side. Object shall have an alert
 * level of 1. Here the curvi coordinates shall be used for determination of alert_side and the alert side shall be the left side.
 * \uts{CSCSA-46260} \sdd{CSCSA-53947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Obj_Position_Based_Alert_Side__lt_boundary_test_lateral_position_obj_valid_for_alert_level_1_curvi)
{
   /** \arrange Create warn relevant target on left side. */
   ltb_object.attributes.alert_level       = ALERT_ACTIVE_LEVEL_1;
   ltb_object.tracker_data.curvi_pos.y     = -EPSILON;
   ltb_object.attributes.f_curvi_available = FBK_TRUE;

   /** \action Set object criticallity */
   Ltb_Set_Obj_Position_Based_Alert_Side(&ltb_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_LEFT);
}


/*
 * Tests a value less than the boundary of lateral position condition for determination of alert side. Object shall have an alert
 * level of 1. Here the curvi coordinates shall be used for determination of alert_side and the alert side shall be the right side
 * \uts{CSCSA-46263} \sdd{CSCSA-53947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Obj_Position_Based_Alert_Side__gt_boundary_test_lateral_position_obj_valid_for_alert_level_1_curvi)
{
   /** \arrange Create warn relevant target on left side. */
   ltb_object.attributes.alert_level       = ALERT_ACTIVE_LEVEL_1;
   ltb_object.tracker_data.curvi_pos.y     = EPSILON;
   ltb_object.attributes.f_curvi_available = FBK_TRUE;

   /** \action Set object criticallity */
   Ltb_Set_Obj_Position_Based_Alert_Side(&ltb_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests the object based alert side function. No alert is given here so that the alert side shall be kept.
 * \uts{CSCSA-46266} \sdd{CSCSA-53947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Obj_Position_Based_Alert_Side__no_state_alert_keep_the_alert_side)
{
   /** \arrange Create warn relevant target on left side. */
   ltb_object.attributes.alert_level = NO_ALERT;
   ltb_object.attributes.alert_side  = FBK_SIDE_RIGHT;

   /** \action Set object criticallity */
   Ltb_Set_Obj_Position_Based_Alert_Side(&ltb_object);

   /** \assert Keep right side of LTB. */
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_RIGHT);
}

/*
 * Tests the object based alert side function. curvi coordinates are not available thus the vcs coordinates shall be used for alert
 * side calculation. \uts{CSCSA-46269} \sdd{CSCSA-53947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Obj_Position_Based_Alert_Side__curvi_not_available)
{
   /** \arrange Create warn relevant target on left side. */
   ltb_object.attributes.alert_level       = NO_ALERT;
   ltb_object.attributes.alert_side        = FBK_SIDE_RIGHT;
   ltb_object.attributes.f_curvi_available = FBK_FALSE;
   ltb_object.tracker_data.vcs_pos.y       = EPSILON;

   /** \action Set object criticallity */
   Ltb_Set_Obj_Position_Based_Alert_Side(&ltb_object);

   /** \assert Keep right side of LTB. */
   EXPECT_EQ(ltb_object.attributes.alert_side, FBK_SIDE_RIGHT);
}

/*
 * Create a level 1 warning relevant target and fill the LTB core output with default values. After updating the most critical
 * object for each side, the core output should contain the targets properties. \uts{CSCSA-46272} \sdd{CSCSA-53949}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Object_Per_Side__update_most_crit_obj_lvl_2)
{
   /** \arrange Create warn relevant target with various properties. */
   ltb_object.tracker_data.id        = 1;
   ltb_object.attributes.alert_level = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.alert_side  = FBK_SIDE_LEFT;
   ltb_object.attributes.ttc         = 2.0f;

   ltb_core_output.ltb_id[FBK_SIDE_LEFT]          = PA_INVALID_OBJ_ID;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = NO_ALERT;
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]         = LTB_INVALID_TTC;

   /** \action Set most critical object per side */
   Ltb_Set_Most_Critical_Object_Per_Side(&ltb_core_output, &ltb_object);

   /** \assert Verify object properties are filled correctly. */
   EXPECT_EQ(ltb_object.tracker_data.id, ltb_core_output.ltb_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_object.attributes.alert_level, ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_object.attributes.ttc, ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]);
}

/*
 * Create a warning relevant target and fill the LTB core output with alert values of a different target. After updating the most
 * critical object for each side, the core output should contain the targets properties. \uts{CSCSA-46275} \sdd{CSCSA-53949}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Object_Per_Side__update_most_crit_obj_lvl_2_same_ttc)
{
   /** \arrange Create warn relevant target with various properties and fill LTB core output with properties of a different target.
    */
   ltb_object.tracker_data.id            = 1;
   ltb_object.attributes.alert_level     = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ltb_object.attributes.ttc             = 1.0f;
   ltb_object.attributes.distance_to_ego = 5.0f;

   ltb_core_output.ltb_id[FBK_SIDE_LEFT]          = 5;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]         = ltb_object.attributes.ttc;
   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]    = ltb_object.attributes.distance_to_ego * 2.0f;

   /** \action Set most critical object per side */
   Ltb_Set_Most_Critical_Object_Per_Side(&ltb_core_output, &ltb_object);

   /** \assert Verify object properties are filled correctly. */
   EXPECT_EQ(ltb_object.tracker_data.id, ltb_core_output.ltb_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_object.attributes.alert_level, ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_object.attributes.ttc, ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_object.attributes.distance_to_ego, ltb_core_output.ltb_distance[FBK_SIDE_LEFT]);
}


/*
 * Test an alert level less than the one provided in the current ltb_core_output. The previously set core output shall not be
 * modified by this function. \uts{CSCSA-46278} \sdd{CSCSA-53949} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Object_Per_Side__lt_current_alert_level_keep_the_temporary_obj_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill LTB core output with properties of a different target.
    */
   ltb_object.tracker_data.id            = 1;
   ltb_object.attributes.alert_level     = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ltb_object.attributes.ttc             = 1.0f;
   ltb_object.attributes.distance_to_ego = 5.0f;

   ltb_core_output_temp.ltb_id[FBK_SIDE_LEFT]          = 5;
   ltb_core_output_temp.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output_temp.ltb_ttc[FBK_SIDE_LEFT]         = 0.5f;
   ltb_core_output_temp.ltb_distance[FBK_SIDE_LEFT]    = 1.0f;

   ltb_core_output.ltb_id[FBK_SIDE_LEFT]          = ltb_core_output_temp.ltb_id[FBK_SIDE_LEFT];
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ltb_core_output_temp.ltb_alert_level[FBK_SIDE_LEFT];
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]         = ltb_core_output_temp.ltb_ttc[FBK_SIDE_LEFT];
   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]    = ltb_core_output_temp.ltb_distance[FBK_SIDE_LEFT];

   /** \action Set most critical object per side */
   Ltb_Set_Most_Critical_Object_Per_Side(&ltb_core_output, &ltb_object);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_id[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_ttc[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_distance[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_distance[FBK_SIDE_LEFT]);
}


/*
 * Test an object with the same alert level and a distance to the host equal to the most critical object. The previously set core
 * output shall not be modified by this function to the new object. \uts{CSCSA-46283} \sdd{CSCSA-53949}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test,
       Ltb_Set_Most_Critical_Object_Per_Side__exact_boundary_distance_to_ego_object_shall_not_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill LTB core output with properties of a different target.
    */
   ltb_core_output.ltb_id[FBK_SIDE_LEFT]          = 5;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]    = 5.0f;

   ltb_object.tracker_data.id            = 1;
   ltb_object.attributes.alert_level     = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ltb_object.attributes.ttc             = ltb_core_output.ltb_ttc[FBK_SIDE_LEFT];
   ltb_object.attributes.distance_to_ego = ltb_core_output.ltb_distance[FBK_SIDE_LEFT];


   ltb_core_output_temp.ltb_id[FBK_SIDE_LEFT]          = ltb_core_output.ltb_id[FBK_SIDE_LEFT];
   ltb_core_output_temp.ltb_alert_level[FBK_SIDE_LEFT] = ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT];
   ltb_core_output_temp.ltb_ttc[FBK_SIDE_LEFT]         = ltb_core_output.ltb_ttc[FBK_SIDE_LEFT];
   ltb_core_output_temp.ltb_distance[FBK_SIDE_LEFT]    = ltb_core_output.ltb_distance[FBK_SIDE_LEFT];


   /** \action Set most critical object per side */
   Ltb_Set_Most_Critical_Object_Per_Side(&ltb_core_output, &ltb_object);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_id[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_ttc[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_distance[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_distance[FBK_SIDE_LEFT]);
}


/*
 * Test an object with the same alert level and a distance to the host less than the most critical object. The previously set core
 * output shall be modified by this function to the new object. \uts{CSCSA-46287} \sdd{CSCSA-53949}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test,
       Ltb_Set_Most_Critical_Object_Per_Side__lt_boundary_distance_to_ego_object_shall_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill LTB core output with properties of a different target.
    */
   ltb_core_output.ltb_id[FBK_SIDE_LEFT]          = 5;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]    = 5.0f;

   ltb_object.tracker_data.id            = 1;
   ltb_object.attributes.alert_level     = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ltb_object.attributes.ttc             = ltb_core_output.ltb_ttc[FBK_SIDE_LEFT];
   ltb_object.attributes.distance_to_ego = ltb_core_output.ltb_distance[FBK_SIDE_LEFT] - EPSILON;

   /** \action Set most critical object per side */
   Ltb_Set_Most_Critical_Object_Per_Side(&ltb_core_output, &ltb_object);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_id[FBK_SIDE_LEFT], ltb_object.tracker_data.id);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ltb_object.attributes.alert_level);
   EXPECT_EQ(ltb_core_output.ltb_ttc[FBK_SIDE_LEFT], ltb_object.attributes.ttc);
   EXPECT_EQ(ltb_core_output.ltb_distance[FBK_SIDE_LEFT], ltb_object.attributes.distance_to_ego);
}

/*
 * Test an object with the same alert level of the most critical object. The previously set core output shall be modified by this
 * function to the new object. \uts{CSCSA-46289} \sdd{CSCSA-53949} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Object_Per_Side__lt_boundary_ttc_object_shall_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill LTB core output with properties of a different target.
    */
   ltb_core_output.ltb_id[FBK_SIDE_LEFT]          = 5;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]    = 5.0f;

   ltb_object.tracker_data.id            = 1;
   ltb_object.attributes.alert_level     = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ltb_object.attributes.ttc             = ltb_core_output.ltb_ttc[FBK_SIDE_LEFT] - EPSILON;
   ltb_object.attributes.distance_to_ego = 5.0f;

   /** \action Set most critical object per side */
   Ltb_Set_Most_Critical_Object_Per_Side(&ltb_core_output, &ltb_object);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_id[FBK_SIDE_LEFT], ltb_object.tracker_data.id);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ltb_object.attributes.alert_level);
   EXPECT_EQ(ltb_core_output.ltb_ttc[FBK_SIDE_LEFT], ltb_object.attributes.ttc);
   EXPECT_EQ(ltb_core_output.ltb_distance[FBK_SIDE_LEFT], ltb_object.attributes.distance_to_ego);
}


/*
 * Test an object with the same alert level and a ttc less than the one of the most critical object. The previously set core output
 * shall be modified by this function to the new object. \uts{CSCSA-46292} \sdd{CSCSA-53949} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Object_Per_Side__exact_boundary_ttc_on_invalid_ttc)
{
   /** \arrange Create warn relevant target with various properties and fill LTB core output with properties of a different target.
    */
   ltb_object.tracker_data.id            = 1;
   ltb_object.attributes.alert_level     = ALERT_ACTIVE_LEVEL_1;
   ltb_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ltb_object.attributes.ttc             = LTB_INVALID_TTC;
   ltb_object.attributes.distance_to_ego = 5.0f;

   ltb_core_output.ltb_id[FBK_SIDE_LEFT]          = 5;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]    = 5.0f;

   ltb_core_output_temp.ltb_id[FBK_SIDE_LEFT]          = ltb_core_output.ltb_id[FBK_SIDE_LEFT];
   ltb_core_output_temp.ltb_alert_level[FBK_SIDE_LEFT] = ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT];
   ltb_core_output_temp.ltb_ttc[FBK_SIDE_LEFT]         = ltb_core_output.ltb_ttc[FBK_SIDE_LEFT];
   ltb_core_output_temp.ltb_distance[FBK_SIDE_LEFT]    = ltb_core_output.ltb_distance[FBK_SIDE_LEFT];

   /** \action Set most critical object per side */
   Ltb_Set_Most_Critical_Object_Per_Side(&ltb_core_output, &ltb_object);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_id[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_ttc[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_core_output.ltb_distance[FBK_SIDE_LEFT], ltb_core_output_temp.ltb_distance[FBK_SIDE_LEFT]);
}


/*
 * Set up an alert for both sides with the left side being more critical. Set most critical side and verify that the LTB core
 * output is set correctly to left side. \uts{CSCSA-46297} \sdd{CSCSA-53950} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Side__update_most_crit_side_left)
{
   /** \arrange Create alert level for both sides */
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT] = NO_ALERT;

   /** \action Set most critical side */
   Ltb_Set_Most_Critical_Side(&ltb_core_output);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_most_critical_side, FBK_SIDE_LEFT);
}

/*
 * Set up an alert for both sides with the right side being more critical. Set most critical side and verify that the LTB core
 * output is set correctly to left side. \uts{CSCSA-46309} \sdd{CSCSA-53950} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Side__update_most_crit_side_right)
{
   /** \arrange Create alert level for both sides */
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]  = NO_ALERT;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT] = ALERT_ACTIVE_LEVEL_1;

   /** \action Set most critical side */
   Ltb_Set_Most_Critical_Side(&ltb_core_output);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_most_critical_side, FBK_SIDE_RIGHT);
}

/*
 * Set up an alert for both sides with the left side having lower TTC. Set most critical side and verify that the LTB core output
 * is set correctly to left side. \uts{CSCSA-46312} \sdd{CSCSA-53950} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Side__update_most_crit_side_left_lower_ttc)
{
   /** \arrange Create alert level for both sides */
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT] = ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT];

   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ltb_core_output.ltb_ttc[FBK_SIDE_RIGHT] = 1.2f;

   /** \action Set most critical side */
   Ltb_Set_Most_Critical_Side(&ltb_core_output);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_most_critical_side, FBK_SIDE_LEFT);
}

/*
 * Set up an alert for both sides with the right side having lower TTC. Set most critical side and verify that the LTB core output
 * is set correctly to right side. \uts{CSCSA-46315} \sdd{CSCSA-53950} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Side__update_most_crit_side_right_lower_ttc)
{
   /** \arrange Create alert level for both sides */
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT] = ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT];

   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]  = 1.2f;
   ltb_core_output.ltb_ttc[FBK_SIDE_RIGHT] = 1.0f;

   /** \action Set most critical side */
   Ltb_Set_Most_Critical_Side(&ltb_core_output);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_most_critical_side, FBK_SIDE_RIGHT);
}

/*
 * Set up an alert for both sides with same TTC but the left side having smaller distance. Set most critical side and verify that
 * the LTB core output is set correctly to left side. \uts{CSCSA-46318} \sdd{CSCSA-53950} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Side__update_most_crit_side_left_same_ttc_lower_dist)
{
   /** \arrange Create alert level for both sides */
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT] = ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT];

   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ltb_core_output.ltb_ttc[FBK_SIDE_RIGHT] = ltb_core_output.ltb_ttc[FBK_SIDE_LEFT];

   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]  = 5.0f;
   ltb_core_output.ltb_distance[FBK_SIDE_RIGHT] = 10.0f;

   /** \action Set most critical side */
   Ltb_Set_Most_Critical_Side(&ltb_core_output);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_most_critical_side, FBK_SIDE_LEFT);
}

/*
 * Set up an alert for both sides with same TTC but the right side having smaller distance. Set most critical side and verify that
 * the LTB core output is set correctly to right side. \uts{CSCSA-46321} \sdd{CSCSA-53950} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Set_Most_Critical_Side__update_most_crit_side_right_same_ttc_lower_dist)
{
   /** \arrange Create alert level for both sides */
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT] = ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT];

   ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ltb_core_output.ltb_ttc[FBK_SIDE_RIGHT] = ltb_core_output.ltb_ttc[FBK_SIDE_LEFT];

   ltb_core_output.ltb_distance[FBK_SIDE_LEFT]  = 10.0f;
   ltb_core_output.ltb_distance[FBK_SIDE_RIGHT] = 5.0f;

   /** \action Set most critical side */
   Ltb_Set_Most_Critical_Side(&ltb_core_output);

   /** \assert Verify LTB core output is filled correctly. */
   EXPECT_EQ(ltb_core_output.ltb_most_critical_side, FBK_SIDE_RIGHT);
}

/**
 * Tests the debouncer logic. On right side arranged alert level is set to level 1. The alert level shall not be qualified to be
 * output to the LTB output structure. On left side the alert level toggles back to none for one cycle. The alert level be held on
 * level 1 which occured in the previous cycle \uts{CSCSA-65996} \sdd{CSCSA-63525} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Debounce_Alert_Level__qualifying_and_holding_works)
{
   /** \arrange Setup configuration for checking qualifying and holding. */
   ltb_cals.k_ltb_alert_holding_cycles                          = 2;
   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]               = NO_ALERT;
   ltb_persistent.ltb_side_alert_holding_counter[FBK_SIDE_LEFT] = 1;
   ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_LEFT]      = ALERT_ACTIVE_LEVEL_1;

   ltb_cals.k_ltb_alert_qualifying_cycles                           = 2;
   ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT]                  = ALERT_ACTIVE_LEVEL_1;
   ltb_persistent.ltb_side_alert_qualifying_counter[FBK_SIDE_RIGHT] = 1;
   ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT]         = NO_ALERT;

   /** \action Debounce alert level */
   Ltb_Debounce_Alert_Level(&ltb_core_output, &ltb_persistent, &ltb_cals);

   /** \assert Verify TA alert is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT], NO_ALERT);
}

/**
 * Tests the debounce functionality. Arranged alert level is set to level 1. The alert level shall not be qualified to be output to
 * the LTB output structure \uts{CSCSA-64255} \sdd{CSCSA-65659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__check_if_alert_level_1_is_suppressed)
{
   /** \arrange Setup alert status with insufficient qualifying counters. */
   uint8_t side_index                                           = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles                       = 2;
   ltb_core_output.ltb_alert_level[side_index]                  = ALERT_ACTIVE_LEVEL_1;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = 1;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = NO_ALERT;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify TA alert is not set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], NO_ALERT);
}

/**
 * Tests the holding logic. The alert level toggles back to none for one cycle. The alert level be held on level 1 which occured in
 * the previous cycle. \uts{CSCSA-64256} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__check_if_alert_level_1_is_held)
{
   /** \arrange Setup alert status which lost validity this cycle. */
   uint8_t side_index = FBK_SIDE_LEFT;

   ltb_cals.k_ltb_alert_holding_cycles                       = 2;
   ltb_core_output.ltb_alert_level[side_index]               = NO_ALERT;
   ltb_persistent.ltb_side_alert_holding_counter[side_index] = 1;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]      = ALERT_ACTIVE_LEVEL_1;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify TA alert is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_1);
}

/**
 * Tests the holding logic. The alert level toggles back to level 1 on a different object. The alert level be held on level 2 which
 * occured in the previous cycle. \uts{CSCSA-64257} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__check_if_alert_level_2_is_held_obj_id_changed)
{
   /** \arrange Setup alert status which lost validity this cycle. */
   uint8_t side_index                                        = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_holding_cycles                       = 2;
   ltb_core_output.ltb_alert_level[side_index]               = ALERT_ACTIVE_LEVEL_1;
   ltb_core_output.ltb_id[side_index]                        = 1;
   ltb_persistent.ltb_side_alert_holding_counter[side_index] = 1;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]      = ALERT_ACTIVE_LEVEL_2;
   ltb_persistent.ltb_side_id_prev_cycle[side_index]         = 2;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify TA alert is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_2);
   EXPECT_EQ(ltb_core_output.ltb_id[side_index], ltb_persistent.ltb_side_id_prev_cycle[side_index]);
}

/**
 * Tests the qualifying logic. The alert level is not set and the qualifying counter is saturated. Expect the qualifying counter to
 * be reset. \uts{CSCSA-64258} \sdd{CSCSA-65659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__check_qualifying_counter_reset)
{
   /** \arrange Setup alert level with state NONE and saturated qualifying counter. */
   uint8_t side_index                                           = FBK_SIDE_LEFT;
   ltb_core_output.ltb_alert_level[side_index]                  = NO_ALERT;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = 2;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));


   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify qualifying counter is reset. */
   EXPECT_EQ(ltb_persistent.ltb_side_alert_qualifying_counter[side_index], FBK_ZERO_INT);
}

/**
 * Tests the holding logic. The alert level is no longer set and the holding counter is saturated. Expect the qualifying counter to
 * be reset. \uts{CSCSA-64259} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__check_holding_counter_reset)
{
   /** \arrange Setup alert level with state NONE and saturated holding counter. */
   uint8_t side_index                                        = FBK_SIDE_LEFT;
   ltb_core_output.ltb_alert_level[side_index]               = NO_ALERT;
   ltb_persistent.ltb_side_alert_holding_counter[side_index] = 2;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]      = NO_ALERT;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify holding counter is reset. */
   EXPECT_EQ(ltb_persistent.ltb_side_alert_holding_counter[side_index], FBK_ZERO_INT);
}

/**
 * Tests the qualifying logic. The alert level is set to level 2 for qualification check. Verify that the alert level 2 is set.
 * \uts{CSCSA-64260} \sdd{CSCSA-65659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__check_alert_level_2_qualifying)
{
   /** \arrange Setup alert level two with reset qualifying counter. */
   uint8_t side_index                                           = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles                       = 2;
   ltb_core_output.ltb_alert_level[side_index]                  = ALERT_ACTIVE_LEVEL_2;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = 3;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = NO_ALERT;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_2);
}

/**
 * Tests the qualifying logic. The alert level is set to level 3 and therefore skipping the qualification cycles for disabled
 * consecutive alert logic. Verify that the alert level 3 is set. \uts{CSCSA-64261} \sdd{CSCSA-65659}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__check_alert_level_3_skipping_qualifying_if_consecutive_alerts_disabled)
{
   /** \arrange Setup alert level 3 with reset qualifying counter. */
   uint8_t side_index                                             = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles                         = 2;
   ltb_cals.k_ltb_f_only_allow_consecutive_ttc_based_alert_levels = 0u;
   ltb_core_output.ltb_alert_level[side_index]                    = ALERT_ACTIVE_LEVEL_3;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index]   = 0;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]           = NO_ALERT;

   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_3);
}

/**
 * Tests the qualifying logic. The alert level is set to level 3 and is suppressed due to enabled consecutive alert logic. Verify
 * that the alert level is suppressed. \uts{CSCSA-64262} \sdd{CSCSA-65659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__check_alert_level_4_suppressed_qualifying_if_consecutive_alerts_enabled)
{
   /** \arrange Setup alert level 3 with reset qualifying counter. */
   uint8_t side_index                                             = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles                         = 2;
   ltb_cals.k_ltb_f_only_allow_consecutive_ttc_based_alert_levels = 1u;
   ltb_core_output.ltb_alert_level[side_index]                    = ALERT_ACTIVE_LEVEL_3;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index]   = 0;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]           = NO_ALERT;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is not set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], NO_ALERT);
}

/**
 * Tests the qualifying logic. The alert level is set to level 3 and the previous alert was level 1. Verify that the alert level 3
 * is set as it is a consecutive alert. \uts{CSCSA-64263} \sdd{CSCSA-65659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__check_alert_level_3_qualifying_if_consecutive_alerts_enabled)
{
   /** \arrange Setup alert level 3 with reset qualifying counter. */
   uint8_t side_index                                             = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles                         = 2;
   ltb_cals.k_ltb_f_only_allow_consecutive_ttc_based_alert_levels = 1u;
   ltb_core_output.ltb_alert_level[side_index]                    = ALERT_ACTIVE_LEVEL_3;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index]   = 0;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]           = ALERT_ACTIVE_LEVEL_2;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_3);
}

/*
 * Tests the boundary for qualification of an object in order to be a valid candidate for a Turn assist warning. Within that test
 * the object shall not be qualified yet. \uts{CSCSA-66591} \sdd{CSCSA-65659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__exact_boundary_test_object_not_qualified)
{
   /** \arrange Setup TA scenario with warning relevant target without sufficient qualifying cycles. */
   uint8_t side_index                                           = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles                       = 2;
   ltb_core_output.ltb_alert_level[side_index]                  = ALERT_ACTIVE_LEVEL_1;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = ltb_cals.k_ltb_alert_qualifying_cycles - ((uint8_t) 1);
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = NO_ALERT;

   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is not set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], NO_ALERT);
}

/*
 * Tests a value greater than the boundary needed for qualification of an object in order to be a valid candidate for a Turn assist
 * warning. Within that test the object shall be qualified. \uts{CSCSA-64265} \sdd{CSCSA-65659} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Qualifying_Alert__gt_boundary_test_object_is_qualified)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   uint8_t side_index                                           = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles                       = 2;
   ltb_core_output.ltb_alert_level[side_index]                  = ALERT_ACTIVE_LEVEL_1;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = ltb_cals.k_ltb_alert_qualifying_cycles;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = NO_ALERT;

   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Qualifying_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_1);
}

/*
 * Tests a the exact boundary needed for the holding logic of a previously qualified object. Within that test the object shall be
 * hold. \uts{CSCSA-64266} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__exact_boundary_test_object_level_is_hold_drop_back_from_level_3)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   uint8_t side_index                          = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles      = 2;
   ltb_cals.k_ltb_alert_holding_cycles         = 2;
   ltb_core_output.ltb_alert_level[side_index] = ALERT_ACTIVE_LEVEL_2;

   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = ltb_cals.k_ltb_alert_qualifying_cycles;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = ALERT_ACTIVE_LEVEL_3;
   ltb_persistent.ltb_side_alert_holding_counter[side_index]    = ltb_cals.k_ltb_alert_holding_cycles - ((uint8_t) 1);
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_3);
}

/*
 * Tests a value greater than the boundary needed for the holding logic of a previously qualified object. Within that test the
 * object shall be hold. \uts{CSCSA-64267} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__gt_boundary_test_object_level_is_not_hold)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   uint8_t side_index                          = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles      = 2;
   ltb_cals.k_ltb_alert_holding_cycles         = 2;
   ltb_core_output.ltb_alert_level[side_index] = ALERT_ACTIVE_LEVEL_2;

   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = ltb_cals.k_ltb_alert_qualifying_cycles;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = ALERT_ACTIVE_LEVEL_3;
   ltb_persistent.ltb_side_alert_holding_counter[side_index]    = ltb_cals.k_ltb_alert_holding_cycles;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that TA alert level is set. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_2);
}

/*
 * Tests the exact boundary needed for the reset of holding counter logic. Within that test holding counter shall be reset.
 * \uts{CSCSA-64268} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__exact_boundary_test_holding_counter_is_reset)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   uint8_t side_index                          = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_qualifying_cycles      = 2;
   ltb_cals.k_ltb_alert_holding_cycles         = 2;
   ltb_core_output.ltb_alert_level[side_index] = ALERT_ACTIVE_LEVEL_2;

   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = ltb_cals.k_ltb_alert_qualifying_cycles;
   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = ALERT_ACTIVE_LEVEL_2;
   ltb_persistent.ltb_side_alert_holding_counter[side_index]    = ltb_cals.k_ltb_alert_holding_cycles;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that holding counter is reset. */
   EXPECT_EQ(ltb_persistent.ltb_side_alert_holding_counter[side_index], FBK_ZERO_INT);
}

/*
 * Tests that alert holding is skipped for a single alert level drop from level 3 if enabled via cal.
 * \uts{CSCSA-64269} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__skip_holding_for_single_alert_level_drop_if_enabled)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   uint8_t side_index                                        = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_holding_cycles                       = 2;
   ltb_cals.k_ltb_f_skip_holding_for_single_alert_level_drop = FBK_TRUE;

   ltb_core_output.ltb_alert_level[side_index] = ALERT_ACTIVE_LEVEL_2;

   ltb_persistent.ltb_side_alert_prev_cycle[side_index]         = ALERT_ACTIVE_LEVEL_3;
   ltb_persistent.ltb_side_alert_holding_counter[side_index]    = FBK_ZERO_UINT;
   ltb_persistent.ltb_side_alert_qualifying_counter[side_index] = 2;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that the current TA alert level is set and holding is skipped. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[side_index], ALERT_ACTIVE_LEVEL_2);
}

/*
 * Tests that alert holding is skipping for a single alert level only applies for drops from level 3.
 * \uts{CSCSA-64270} \sdd{CSCSA-65660} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Warn_Logic_Test, Ltb_Holding_Alert__holding_for_single_alert_level_drop_from_level_2)
{
   /** \arrange Setup TA scenario with warning relevant target. */
   uint8_t side_index                                        = FBK_SIDE_LEFT;
   ltb_cals.k_ltb_alert_holding_cycles                       = 2;
   ltb_cals.k_ltb_f_skip_holding_for_single_alert_level_drop = FBK_TRUE;

   ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT] = ALERT_ACTIVE_LEVEL_1;

   ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_LEFT]      = ALERT_ACTIVE_LEVEL_2;
   ltb_persistent.ltb_side_alert_holding_counter[FBK_SIDE_LEFT] = FBK_ZERO_UINT;
   int32_t alert_level_delta =
      ((int32_t) (ltb_core_output.ltb_alert_level[side_index]) - ((int32_t) ltb_persistent.ltb_side_alert_prev_cycle[side_index]));

   /** \action Debounce alert level */
   Ltb_Holding_Alert(&ltb_core_output, &ltb_persistent, alert_level_delta, side_index, &ltb_cals);

   /** \assert Verify that the TA alert level is held. */
   EXPECT_EQ(ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT], ALERT_ACTIVE_LEVEL_2);
}
