/**
 * @file ta_warn_logic_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45070}
 */

#include "ta_warn_logic_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_constants.h"
#include "ta_warn_logic.c"
}

/*
 * Set object properties of warning relevant target and verify that its properties are correctly written to the TA core output.
 * \uts{CSCSA-45071} \sdd{SF-8761} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Fill_Core_Output_With_Current_Obj__fill_core_output_correctly)
{
   /** \arrange Create warning relevant target with various properties. */
   ta_object.attributes.alert_level          = TA_ALERT_STATE_LEVEL_2;
   ta_object.tracker_data.id                 = 1;
   ta_object.attributes.ttc                  = 1.8f;
   ta_object.attributes.f_obj_in_danger_zone = FBK_TRUE;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]          = TA_ALERT_STATE_NONE;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                   = PA_INVALID_OBJ_ID;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                  = TA_INVALID_TTC;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT] = FBK_FALSE;

   /** \action Fill TA core output with current object */
   Ta_Fill_Core_Output_With_Current_Obj(&ta_core_output, &ta_object, FBK_SIDE_LEFT);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_object.attributes.alert_level);
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_object.tracker_data.id);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_object.attributes.ttc);
   EXPECT_EQ(ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT], ta_object.attributes.f_obj_in_danger_zone);
}

/*
 * Create a warning relevant target (previously active) with properties which should reach alert level 1. Set object criticallity
 * and verify it reaches alert level 1. \uts{CSCSA-45072} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__check_alert_level_1_obj_was_active_based_on_ttp)
{
   /** \arrange Create warning relevant target with various properties. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_BOTH;
   ta_object.attributes.ttp           = 4.0f + 0.5f * ta_cal.k_ta_active_obj_ttp_offset;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_1);
}

/*
 * Create a warning relevant target (previously inactive) with properties which should reach alert level 1. Set object criticallity
 * and verify it reaches alert level 1. \uts{CSCSA-45073} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__check_alert_level_1_obj_was_inactive)
{
   /** \arrange Create warning relevant target with various properties. */
   float32_T alert_threshold = 4.0f;
   ta_object.attributes.ttp  = 0.5f * 4.0f;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_1);
}

/*
 * Tests the exact boundary value for ttp based alert state level calculation. Object shall have an alert state in this case.
 * \uts{CSCSA-45079} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__exact_boundary_test_ttp_valid_for_alert_level_1_object_was_active)
{
   /** \arrange Create warning relevant target. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_BOTH;
   ta_object.attributes.ttp           = 4.0f + ta_cal.k_ta_active_obj_ttp_offset;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_1);
}

/*
 * Tests a value greater than boundary value for ttp based alert state level calculation. Object shall have no alert state in this
 * case. \uts{CSCSA-45080} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__gt_boundary_test_ttp_invalid_for_alert_level_1_object_was_active)
{
   /** \arrange Create target which is not warning relevant. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_BOTH;
   ta_object.attributes.ttp           = 4.0f + ta_cal.k_ta_active_obj_ttp_offset + EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}

/*
 * Tests ttp set to its invalid default value TA_INVALID_TTP. Object shall not have an alert state here.
 * \uts{CSCSA-45081} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__exact_boundary_test_ttp_equals_invalid_default_ttp_object_was_active)
{
   /** \arrange Create target which is not warning relevant. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_BOTH;
   ta_object.attributes.ttp           = TA_INVALID_TTP;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}

/*
 * Tests ttp set to its invalid default value TA_INVALID_TTP - EPSILON. Object shall still not have an alert state here.
 * \uts{CSCSA-45082} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__lt_boundary_test_ttp_equals_invalid_default_ttp_object_was_active)
{
   /** \arrange Create target which is not warning relevant. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_BOTH;
   ta_object.attributes.ttp           = TA_INVALID_TTP - EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}

/*
 * Tests the exact boundary value for ttp based alert state level calculation. Object shall have an alert state in this case.
 * \uts{CSCSA-45083} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__exact_boundary_test_ttp_valid_for_alert_level_1_object_was_inactive)
{
   /** \arrange Create warning relevant target. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_NONE;
   ta_object.attributes.ttp           = 4.0f;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_1);
}

/*
 * Tests a value greater than boundary value for ttp based alert state level calculation. Object shall have no alert state in this
 * case. \uts{CSCSA-45084} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__gt_boundary_test_ttp_invalid_for_alert_level_1_object_was_inactive)
{
   /** \arrange Create target which is not warning relevant. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_NONE;
   ta_object.attributes.ttp           = 4.0f + EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}

/*
 * Tests ttp set to its invalid default value TA_INVALID_TTP. Object shall not have an alert state here.
 * \uts{CSCSA-45085} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__exact_boundary_test_ttp_equals_invalid_default_ttp_object_was_inactive)
{
   /** \arrange Create target which is not warning relevant. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_NONE;
   ta_object.attributes.ttp           = TA_INVALID_TTP;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}

/*
 * Tests ttp set to its invalid default value TA_INVALID_TTP - EPSILON. Object shall still not have an alert state here.
 * \uts{CSCSA-45086} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__lt_boundary_test_ttp_equals_invalid_default_ttp_object_was_inactive)
{
   /** \arrange Create target which is not warning relevant. */
   float32_T alert_threshold          = 4.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_NONE;
   ta_object.attributes.ttp           = TA_INVALID_TTP - EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}

/*
 * Tests the exact boundary value for ttp based alert state level calculation. Object shall have an alert state in this case.
 * \uts{CSCSA-45125} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__check_ttp_for_alert_trigger_normal)
{
   /** \arrange Create warning relevant target. */
   float32_T alert_threshold          = 3.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_NONE;
   ta_object.attributes.ttp           = 3.0f;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_1);
}

/*
 * Tests the exact boundary value for ttp based alert state level calculation. Object shall have an alert state in this case.
 * \uts{CSCSA-45126} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__check_ttp_for_alert_trigger_late)
{
   /** \arrange Create warning relevant target. */
   float32_T alert_threshold          = 2.0f;
   ta_object.attributes.ta_alert_mode = TA_ALERT_MODE_NONE;
   ta_object.attributes.ttp           = 2.0f;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_1);
}

/*
 * Tests if the ttp applies hysteresis to the ttp threshold check if the global flag is enabled and a warning was active last
 * cycle. \uts{CSCSA-45127} \sdd{SF-8790} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttp_Based_Alert_Level__check_ttp_for_alert_with_hysteresis_globally_active)
{
   /** \arrange Create warning relevant target. */
   float32_T alert_threshold                              = 2.0f;
   ta_object.attributes.ta_alert_mode                     = TA_ALERT_MODE_NONE;
   ta_object.attributes.ttp                               = alert_threshold + EPSILON;
   ta_object.tracker_data.curvi_pos.y                     = 5.0f;
   ta_object.attributes.f_curvi_available                 = FBK_TRUE;
   ta_persistent.ta_side_alert_prev_cycle[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_4;
   ta_cal.k_ta_f_apply_ttp_hysteresis_globally            = FBK_TRUE;

   /** \action Set object criticallity */
   Ta_Set_Ttp_Based_Alert_Level(&ta_object, &ta_persistent, alert_threshold, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_1);
}

/*
 * Create a warning relevant target with properties which should reach alert level 2. Set object criticallity and verify it reaches
 * alert level 2. \uts{CSCSA-45074} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__check_alert_level_2)
{
   /** \arrange Create warning relevant target with various properties. */
   ta_object.attributes.ttc = ta_cal.k_ta_alert_lvl_2_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_2);
}

/*
 * Create a warning relevant target with properties which should reach alert level 3. Set object criticallity and verify it reaches
 * alert level 3. \uts{CSCSA-45075} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__check_alert_level_3)
{
   /** \arrange Create warning relevant target with various properties. */
   ta_object.attributes.ttc = ta_cal.k_ta_alert_lvl_3_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_3);
}

/*
 * Create a warning relevant target with properties which should reach alert level 4. Set object criticallity and verify it reaches
 * alert level 4. \uts{CSCSA-45076} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__check_alert_level_4)
{
   /** \arrange Create warning relevant target with various properties. */
   ta_object.attributes.ttc                 = ta_cal.k_ta_alert_lvl_4_ttc_threshold - 0.1f;
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold + 0.1f;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_4);
}

/*
 * Create a warning relevant target with properties which should normally reach alert level 2. Set object status to COASTED and
 * verify that it does reach alert level 2, when coasted alerts are allowed by cal value. \uts{CSCSA-45121} \sdd{SF-8788}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__check_alert_level_2_for_allowed_coasted_object)
{
   /** \arrange Create warning relevant target with various properties. */
   ta_object.attributes.ttc                                      = ta_cal.k_ta_alert_lvl_2_ttc_threshold - 0.1f;
   ta_object.tracker_data.status                                 = PA_OBJ_STATUS_COASTED;
   ta_cal.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj = FBK_FALSE;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_2);
}

/*
 * Create a warning relevant target with properties which should normally reach alert level 2. Set object status to MATURE and
 * verify that it does reach alert level 2 also when coasted alerts are disabled by cal value. \uts{CSCSA-45122} \sdd{SF-8788}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__check_alert_level_2_for_mature_object)
{
   /** \arrange Create warning relevant target with various properties. */
   ta_object.attributes.ttc                                      = ta_cal.k_ta_alert_lvl_2_ttc_threshold - 0.1f;
   ta_object.tracker_data.status                                 = PA_OBJ_STATUS_MATURE;
   ta_cal.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj = FBK_TRUE;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_2);
}

/*
 * Create a warning relevant target with properties which should normally reach alert level 2. Set object status to COASTED and
 * verify that it does not reach alert level 2, when coasted alerts are deactivated by cal value. \uts{CSCSA-45123} \sdd{SF-8788}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__check_alert_level_for_suppressed_coasted_object)
{
   /** \arrange Create warning relevant target with various properties. */
   ta_object.attributes.ttc                                      = ta_cal.k_ta_alert_lvl_2_ttc_threshold - 0.1f;
   ta_object.tracker_data.status                                 = PA_OBJ_STATUS_COASTED;
   ta_cal.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj = FBK_TRUE;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}


/*
 * Create a warning relevant target on the left side. Verify that warning level is set on correct side.
 * \uts{CSCSA-45077} \sdd{SF-8766} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Object_Criticality__check_alert_side_left)
{
   /** \arrange Create warning relevant target on the left side. */
   ta_object.tracker_data.vcs_pos.y = -1.0f;
   ta_object.attributes.ttc         = ta_cal.k_ta_alert_lvl_2_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ta_Set_Object_Criticality(&ta_object, &ta_core_input, &ta_persistent, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_LEFT);
}

/*
 * Create a warning relevant target on the right side. Verify that warning level is set on correct side.
 * \uts{CSCSA-45078} \sdd{SF-8766} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Object_Criticality__check_alert_side_right)
{
   /** \arrange Create warning relevant target on the right side. */
   ta_object.tracker_data.vcs_pos.y = 1.0f;
   ta_object.attributes.ttc         = ta_cal.k_ta_alert_lvl_2_ttc_threshold - 0.1f;

   /** \action Set object criticallity */
   Ta_Set_Object_Criticality(&ta_object, &ta_core_input, &ta_persistent, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests ttc set to its invalid default value TA_INVALID_TTC. Object shall not have an alert state here.
 * \uts{CSCSA-45087} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_equals_invalid_default_ttc)
{
   /** \arrange Create target which is not warning relevant. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold;
   ta_object.attributes.ttc                 = TA_INVALID_TTC;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}

/*
 * Tests ttc set to its invalid default value TA_INVALID_TTC - EPSILON. Object shall not have an alert state here.
 * \uts{CSCSA-45088} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__lt_boundary_test_ttc_equals_invalid_default_ttc)
{
   /** \arrange Create target which is not warning relevant. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold;
   ta_object.attributes.ttc                 = TA_INVALID_TTC - EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}


/*
 * Test the exact boundary value for ttc based alert state level calculation for state level 4. Object shall have an alert state in
 * this case. \uts{CSCSA-45089} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_obj_valid_for_alert_level_4)
{
   /** \arrange Create warning relevant target. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold;
   ta_object.attributes.ttc                 = ta_cal.k_ta_alert_lvl_4_ttc_threshold;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_4);
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 4. Object shall not have
 * alert state of level 4 in this case. \uts{CSCSA-45090} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__gt_boundary_test_ttc_obj_invalid_for_alert_level_4)
{
   /** \arrange Create warning relevant target. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold;
   ta_object.attributes.ttc                 = ta_cal.k_ta_alert_lvl_4_ttc_threshold + EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_TRUE((ta_object.attributes.alert_level < TA_ALERT_STATE_LEVEL_4));
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 3. Object shall have an
 * alert state in this case. \uts{CSCSA-45091} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_obj_valid_for_alert_level_3)
{
   /** \arrange Create warning relevant target. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold - EPSILON;
   ta_object.attributes.ttc                 = ta_cal.k_ta_alert_lvl_3_ttc_threshold;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_3);
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 3. Object shall not have
 * alert state of level 3 in this case. \uts{CSCSA-45092} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__gt_boundary_test_ttc_obj_invalid_for_alert_level_3)
{
   /** \arrange Create warning relevant target. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold - EPSILON;
   ta_object.attributes.ttc                 = ta_cal.k_ta_alert_lvl_3_ttc_threshold + EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_TRUE((ta_object.attributes.alert_level < TA_ALERT_STATE_LEVEL_3));
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 2. Object shall have an
 * alert state in this case. \uts{CSCSA-45093} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__exact_boundary_test_ttc_obj_valid_for_alert_level_2)
{
   /** \arrange Create warning relevant target. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold - EPSILON;
   ta_object.attributes.ttc                 = ta_cal.k_ta_alert_lvl_2_ttc_threshold;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_LEVEL_2);
}

/*
 * Tests a value greater than boundary value for ttc based alert state level calculation for state level 2. Object shall not have
 * alert state of level 2 in this case. \uts{CSCSA-45094} \sdd{SF-8788} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Ttc_Based_Alert_Level__gt_boundary_test_ttc_obj_invalid_for_alert_level_2)
{
   /** \arrange Create target which is not warn relevant. */
   ta_object.attributes.decel_to_avoid_coll = ta_cal.k_ta_alert_lvl_4_decel_threshold - EPSILON;
   ta_object.attributes.ttc                 = ta_cal.k_ta_alert_lvl_2_ttc_threshold + EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Ttc_Based_Alert_Level(&ta_object, &ta_cal);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_level, TA_ALERT_STATE_NONE);
}


/*
 * Tests the exact boundary of lateral position condition. Object shall have an alert level of 2. Here the vcs coordinates shall be
 * used for determination of alert_side and the alert side shall be the right side. \uts{CSCSA-45095} \sdd{SF-8789}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Obj_Position_Based_Alert_Side__exact_boundary_test_lateral_position_obj_valid_for_alert_level_2_vcs)
{
   /** \arrange Create warn relevant target on right side. */
   ta_object.tracker_data.vcs_pos.y = FBK_ZERO_F;
   ta_object.attributes.alert_level = TA_ALERT_STATE_LEVEL_2;

   /** \action Set object criticallity */
   Ta_Set_Obj_Position_Based_Alert_Side(&ta_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests a value lower than the boundary of lateral position condition. Object shall have an alert level of 2. Here the vcs
 * coordinates shall be used for determination of alert_side and the alert side shall be the left side. \uts{CSCSA-45096}
 * \sdd{SF-8789} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Obj_Position_Based_Alert_Side__lt_boundary_test_lateral_position_obj_valid_for_alert_level_2_vcs)
{
   /** \arrange Create warn relevant target on left side. */
   ta_object.attributes.alert_level = TA_ALERT_STATE_LEVEL_2;
   ta_object.tracker_data.vcs_pos.y = -EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Obj_Position_Based_Alert_Side(&ta_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_LEFT);
}


/*
 * Tests the exact boundary of lateral position condition for determination of alert side. Object shall have an alert level of 2.
 * Here the curvi coordinates shall be used for determination of alert_side and the alert side shall be the right side.
 * \uts{CSCSA-45097} \sdd{SF-8789} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Obj_Position_Based_Alert_Side__exact_boundary_test_lateral_position_obj_valid_for_alert_level_2_curvi)
{
   /** \arrange Create warn relevant target on right side. */
   ta_object.tracker_data.curvi_pos.y     = FBK_ZERO_F;
   ta_object.attributes.alert_level       = TA_ALERT_STATE_LEVEL_2;
   ta_object.attributes.f_curvi_available = FBK_TRUE;

   /** \action Set object criticallity */
   Ta_Set_Obj_Position_Based_Alert_Side(&ta_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests a value less than the boundary of lateral position condition for determination of alert side. Object shall have an alert
 * level of 1. Here the curvi coordinates shall be used for determination of alert_side and the alert side shall be the left side.
 * \uts{CSCSA-45098} \sdd{SF-8789} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Obj_Position_Based_Alert_Side__lt_boundary_test_lateral_position_obj_valid_for_alert_level_2_curvi)
{
   /** \arrange Create warn relevant target on left side. */
   ta_object.attributes.alert_level       = TA_ALERT_STATE_LEVEL_1;
   ta_object.tracker_data.curvi_pos.y     = -EPSILON;
   ta_object.attributes.f_curvi_available = FBK_TRUE;

   /** \action Set object criticallity */
   Ta_Set_Obj_Position_Based_Alert_Side(&ta_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_LEFT);
}


/*
 * Tests a value less than the boundary of lateral position condition for determination of alert side. Object shall have an alert
 * level of 1. Here the curvi coordinates shall be used for determination of alert_side and the alert side shall be the right side
 * \uts{CSCSA-45118} \sdd{SF-8789} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Obj_Position_Based_Alert_Side__gt_boundary_test_lateral_position_obj_valid_for_alert_level_2_curvi)
{
   /** \arrange Create warn relevant target on left side. */
   ta_object.attributes.alert_level       = TA_ALERT_STATE_LEVEL_1;
   ta_object.tracker_data.curvi_pos.y     = EPSILON;
   ta_object.attributes.f_curvi_available = FBK_TRUE;

   /** \action Set object criticallity */
   Ta_Set_Obj_Position_Based_Alert_Side(&ta_object);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_RIGHT);
}


/*
 * Tests the object based alert side function. No alert is given here so that the alert side shall be kept.
 * \uts{CSCSA-45119} \sdd{SF-8789} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Obj_Position_Based_Alert_Side__no_state_alert_keep_the_alert_side)
{
   /** \arrange Create warn relevant target on left side. */
   ta_object.attributes.alert_level = TA_ALERT_STATE_NONE;
   ta_object.attributes.alert_side  = FBK_SIDE_RIGHT;

   /** \action Set object criticallity */
   Ta_Set_Obj_Position_Based_Alert_Side(&ta_object);

   /** \assert Keep right side of TA. */
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_RIGHT);
}

/*
 * Tests the object based alert side function. curvi coordinates are not available thus the vcs coordinates shall be used for alert
 * side calculation. \uts{CSCSA-45120} \sdd{SF-8789} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Obj_Position_Based_Alert_Side__curvi_not_available)
{
   /** \arrange Create warn relevant target on left side. */
   ta_object.attributes.alert_level       = TA_ALERT_STATE_NONE;
   ta_object.attributes.alert_side        = FBK_SIDE_RIGHT;
   ta_object.attributes.f_curvi_available = FBK_FALSE;
   ta_object.tracker_data.vcs_pos.y       = EPSILON;

   /** \action Set object criticallity */
   Ta_Set_Obj_Position_Based_Alert_Side(&ta_object);

   /** \assert Keep right side of TA. */
   EXPECT_EQ(ta_object.attributes.alert_side, FBK_SIDE_RIGHT);
}

/*
 * Create a level 1 warning relevant target and fill the TA core output with default values. After updating the most critical
 * object for each side, the core output should contain the targets properties. \uts{CSCSA-45099} \sdd{SF-8764}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__update_most_crit_obj_lvl_1)
{
   /** \arrange Create warn relevant target with various properties. */
   ta_object.tracker_data.id        = 1;
   ta_object.attributes.alert_level = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side  = FBK_SIDE_LEFT;
   ta_object.attributes.ttp         = 4.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = PA_INVALID_OBJ_ID;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_NONE;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = TA_INVALID_TTP;

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify object properties are filled correctly. */
   EXPECT_EQ(ta_object.tracker_data.id, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_object.attributes.alert_level, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_object.attributes.ttp, ta_core_output.ta_ttp[FBK_SIDE_LEFT]);
}

/*
 * Create a level 2 warning relevant target and fill the TA core output with default values. After updating the most critical
 * object for each side, the core output should contain the targets properties. \uts{CSCSA-45100} \sdd{SF-8764}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__update_most_crit_obj_lvl_2)
{
   /** \arrange Create warn relevant target with various properties. */
   ta_object.tracker_data.id        = 1;
   ta_object.attributes.alert_level = TA_ALERT_STATE_LEVEL_2;
   ta_object.attributes.alert_side  = FBK_SIDE_LEFT;
   ta_object.attributes.ttc         = 2.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = PA_INVALID_OBJ_ID;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_NONE;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = TA_INVALID_TTC;

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify object properties are filled correctly. */
   EXPECT_EQ(ta_object.tracker_data.id, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_object.attributes.alert_level, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_object.attributes.ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/*
 * Create a warning relevant target and fill the TA core output with alert values of a different target. After updating the most
 * critical object for each side, the core output should contain the targets properties. \uts{CSCSA-45101} \sdd{SF-8764}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__update_most_crit_obj_lvl_2_same_ttc)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_2;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = ta_object.attributes.ttc;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = ta_object.attributes.distance_to_ego * 2.0f;

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify object properties are filled correctly. */
   EXPECT_EQ(ta_object.tracker_data.id, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_object.attributes.alert_level, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_object.attributes.ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_object.attributes.distance_to_ego, ta_core_output.ta_distance[FBK_SIDE_LEFT]);
}


/*
 * Test an alert level less than the one provided in the current ta_core_output. The previously set core output shall not be
 * modified by this function. \uts{CSCSA-45102} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__lt_current_alert_level_keep_the_temporary_obj_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output_temp.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_2;
   ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]         = 0.5f;
   ta_core_output_temp.ta_ttp[FBK_SIDE_LEFT]         = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]    = 1.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = ta_core_output_temp.ta_id[FBK_SIDE_LEFT];
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = ta_core_output_temp.ta_ttp[FBK_SIDE_LEFT];
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT];
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT];
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = ta_core_output_temp.ta_distance[FBK_SIDE_LEFT];

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_core_output_temp.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]);
}

/*
 * Test an object with the same alert level and the same ttp. The previously set core output shall not be modified by this
 * function. \uts{CSCSA-45103} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__exact_boundary_ttp_object_shall_not_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.ttp             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = ta_object.attributes.ttp;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = FBK_ZERO_F;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   ta_core_output_temp.ta_id[FBK_SIDE_LEFT]          = ta_core_output.ta_id[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttp[FBK_SIDE_LEFT]         = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]         = ta_core_output.ta_ttc[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]    = ta_core_output.ta_distance[FBK_SIDE_LEFT];

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_core_output_temp.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]);
}

/*
 * Test an object with the same alert level and a ttp less than the one of the most critical object. The previously set core output
 * shall be modified by this function to the new object. \uts{CSCSA-45104} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__exact_boundary_ttp_on_invalid_ttp)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.ttp             = TA_INVALID_TTP;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   ta_core_output_temp.ta_id[FBK_SIDE_LEFT]          = ta_core_output.ta_id[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttp[FBK_SIDE_LEFT]         = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]         = ta_core_output.ta_ttc[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]    = ta_core_output.ta_distance[FBK_SIDE_LEFT];

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_core_output_temp.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]);
}

/*
 * Test an object with the same alert level and a ttp greater than the one upper border. The previously set core output shall not
 * be modified by this function. \uts{CSCSA-45105} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__lt_boundary_ttp_object_shall_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.ttp             = 1.0f - EPSILON;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_object.tracker_data.id);
   EXPECT_EQ(ta_core_output.ta_ttp[FBK_SIDE_LEFT], ta_object.attributes.ttp);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_object.attributes.alert_level);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_object.attributes.ttc);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_object.attributes.distance_to_ego);
}


/*
 * Test an object with the same alert level and a distance to the host equal to the most critical object. The previously set core
 * output shall not be modified by this function to the new object. \uts{CSCSA-45106} \sdd{SF-8764}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test,
       Ta_Set_Most_Critical_Object_Per_Side__exact_boundary_distance_to_ego_object_shall_not_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = ta_core_output.ta_ttc[FBK_SIDE_LEFT];
   ta_object.attributes.ttp             = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_object.attributes.distance_to_ego = ta_core_output.ta_distance[FBK_SIDE_LEFT];


   ta_core_output_temp.ta_id[FBK_SIDE_LEFT]          = ta_core_output.ta_id[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttp[FBK_SIDE_LEFT]         = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]         = ta_core_output.ta_ttc[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]    = ta_core_output.ta_distance[FBK_SIDE_LEFT];


   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_core_output_temp.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_ttp[FBK_SIDE_LEFT], ta_core_output_temp.ta_ttp[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]);
}


/*
 * Test an object with the same alert level and a distance to the host less than the most critical object. The previously set core
 * output shall be modified by this function to the new object. \uts{CSCSA-45107} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__lt_boundary_distance_to_ego_object_shall_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = ta_core_output.ta_ttc[FBK_SIDE_LEFT];
   ta_object.attributes.ttp             = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_object.attributes.distance_to_ego = ta_core_output.ta_distance[FBK_SIDE_LEFT] - EPSILON;

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_object.tracker_data.id);
   EXPECT_EQ(ta_core_output.ta_ttp[FBK_SIDE_LEFT], ta_object.attributes.ttp);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_object.attributes.alert_level);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_object.attributes.ttc);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_object.attributes.distance_to_ego);
}

/*
 * Test an object with the same alert level, but a lower distance to the host than the current most critical object. The previously
 * set core output shall be modified by this function to the new object, if calibrated to evaluate closer objects as more critical.
 * \uts{CSCSA-45124} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__check_distance_based_criticality_for_ttp_based_alert)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttp             = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_object.attributes.distance_to_ego = ta_core_output.ta_distance[FBK_SIDE_LEFT] - EPSILON;

   ta_cal.k_rta_f_higher_obj_crit_based_on_lower_ttp = FBK_FALSE;

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_object.tracker_data.id);
   EXPECT_EQ(ta_core_output.ta_ttp[FBK_SIDE_LEFT], ta_object.attributes.ttp);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_object.attributes.alert_level);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_object.attributes.ttc);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_object.attributes.distance_to_ego);
}


/*
 * Test an object with the same alert level and a ttp less than the one of the most critical object. The previously set core output
 * shall be modified by this function to the new object. \uts{CSCSA-45108} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__lt_boundary_ttc_object_shall_be_taken_as_most_critical_one)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = ta_core_output.ta_ttc[FBK_SIDE_LEFT] - EPSILON;
   ta_object.attributes.ttp             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_object.tracker_data.id);
   EXPECT_EQ(ta_core_output.ta_ttp[FBK_SIDE_LEFT], ta_object.attributes.ttp);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_object.attributes.alert_level);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_object.attributes.ttc);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_object.attributes.distance_to_ego);
}


/*
 * Test an object with the same alert level and a ttc less than the one of the most critical object. The previously set core output
 * shall be modified by this function to the new object. \uts{CSCSA-45109} \sdd{SF-8764} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Object_Per_Side__exact_boundary_ttc_on_invalid_ttc)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_level     = TA_ALERT_STATE_LEVEL_1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = TA_INVALID_TTC;
   ta_object.attributes.ttp             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]          = 5;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 1.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = 5.0f;

   ta_core_output_temp.ta_id[FBK_SIDE_LEFT]          = ta_core_output.ta_id[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttp[FBK_SIDE_LEFT]         = ta_core_output.ta_ttp[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]         = ta_core_output.ta_ttc[FBK_SIDE_LEFT];
   ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]    = ta_core_output.ta_distance[FBK_SIDE_LEFT];

   /** \action Set most critical object per side */
   Ta_Set_Most_Critical_Object_Per_Side(&ta_core_output, &ta_object, &ta_cal);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_id[FBK_SIDE_LEFT], ta_core_output_temp.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], ta_core_output_temp.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_ttc[FBK_SIDE_LEFT], ta_core_output_temp.ta_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_core_output.ta_distance[FBK_SIDE_LEFT], ta_core_output_temp.ta_distance[FBK_SIDE_LEFT]);
}


/*
 * Set up an alert for both sides with the left side being more critical. Set most critical side and verify that the TA core output
 * is set correctly to left side. \uts{CSCSA-45110} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_left)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_NONE;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_LEFT);
}

/*
 * Set up an alert for both sides with the right side being more critical. Set most critical side and verify that the TA core
 * output is set correctly to left side. \uts{CSCSA-45111} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_right)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_NONE;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_1;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_RIGHT);
}

/*
 * Set up an alert for both sides with the left side having lower TTC. Set most critical side and verify that the TA core output is
 * set correctly to left side. \uts{CSCSA-45112} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_left_lower_ttc)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = 1.2f;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_LEFT);
}

/*
 * Set up an alert for both sides with the right side having lower TTC. Set most critical side and verify that the TA core output
 * is set correctly to right side. \uts{CSCSA-45113} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_right_lower_ttc)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = 1.2f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = 1.0f;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_RIGHT);
}

/*
 * Set up an alert for both sides with same TTC but the left side having smaller distance. Set most critical side and verify that
 * the TA core output is set correctly to left side. \uts{CSCSA-45114} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_left_same_ttc_lower_dist)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = ta_core_output.ta_ttc[FBK_SIDE_LEFT];

   ta_core_output.ta_distance[FBK_SIDE_LEFT]  = 5.0f;
   ta_core_output.ta_distance[FBK_SIDE_RIGHT] = 10.0f;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_LEFT);
}

/*
 * Set up an alert for both sides with same TTC but the right side having smaller distance. Set most critical side and verify that
 * the TA core output is set correctly to right side. \uts{CSCSA-45115} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_right_same_ttc_lower_dist)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = ta_core_output.ta_ttc[FBK_SIDE_LEFT];

   ta_core_output.ta_distance[FBK_SIDE_LEFT]  = 10.0f;
   ta_core_output.ta_distance[FBK_SIDE_RIGHT] = 5.0f;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_RIGHT);
}

/*
 * Set up an alert for both sides with invalid TTC but the left side having smaller TTP. Set most critical side and verify that the
 * TA core output is set correctly to left side. \uts{CSCSA-45116} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_left_lower_ttp)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = TA_INVALID_TTC;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = TA_INVALID_TTC;

   ta_core_output.ta_ttp[FBK_SIDE_LEFT]  = 1.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT] = 1.2f;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_LEFT);
}

/*
 * Set up an alert for both sides with invalid TTC but the right side having smaller TTP. Set most critical side and verify that
 * the TA core output is set correctly to right side. \uts{CSCSA-45117} \sdd{SF-8765} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Set_Most_Critical_Side__update_most_crit_side_right_lower_ttp)
{
   /** \arrange Create alert level for both sides */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = ta_core_output.ta_alert_level[FBK_SIDE_LEFT];

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = TA_INVALID_TTC;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = TA_INVALID_TTC;

   ta_core_output.ta_ttp[FBK_SIDE_LEFT]  = 1.2f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT] = 1.0f;

   /** \action Set most critical side */
   Ta_Set_Most_Critical_Side(&ta_core_output);

   /** \assert Verify TA core output is filled correctly. */
   EXPECT_EQ(ta_core_output.ta_most_critical_side, FBK_SIDE_RIGHT);
}

/*
 * Create a relevant target and fill the TA core output with default values. Then check if the target is considered more critical.
 * \uts{CSCSA-45128} \sdd{SF-8629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Is_Ttp_More_Critical__true_ttp_based)
{
   /** \arrange Create warn relevant target with various properties. */
   boolean_T f_more_critical_ttp   = FBK_FALSE;
   ta_object.tracker_data.id       = 1;
   ta_object.attributes.alert_side = FBK_SIDE_LEFT;
   ta_object.attributes.ttp        = 4.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_ID;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT] = TA_INVALID_TTP;

   /** \action Check criticality */
   f_more_critical_ttp = Ta_Is_Ttp_More_Critical(&ta_core_output, &ta_object, &ta_cal, FBK_SIDE_LEFT);

   /** \assert Verify object is more critical. */
   EXPECT_TRUE(f_more_critical_ttp);
}

/*
 * Create a relevant target and fill the TA core output more critical values. Then check if the target is considered less critical.
 * \uts{CSCSA-45129} \sdd{SF-8629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Is_Ttp_More_Critical__false)
{
   /** \arrange Create warn relevant target with various properties. */
   boolean_T f_more_critical_ttp   = FBK_TRUE;
   ta_object.tracker_data.id       = 1;
   ta_object.attributes.alert_side = FBK_SIDE_LEFT;
   ta_object.attributes.ttp        = 4.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = 5;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT] = 2.0;

   /** \action Check criticality */
   f_more_critical_ttp = Ta_Is_Ttp_More_Critical(&ta_core_output, &ta_object, &ta_cal, FBK_SIDE_LEFT);

   /** \assert Verify object is less critical. */
   EXPECT_FALSE(f_more_critical_ttp);
}

/*
 * Create a relevant target and fill the TA core output with default values. Disable TTP as critical criterion in the calibration.
 * Then check if the target is considered more critical by distance instead. \uts{CSCSA-45130} \sdd{SF-8629}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Is_Ttp_More_Critical__true_distance_based)
{
   /** \arrange Create warn relevant target with various properties. */
   boolean_T f_more_critical_ttp   = FBK_FALSE;
   ta_object.tracker_data.id       = 1;
   ta_object.attributes.alert_side = FBK_SIDE_LEFT;
   ta_object.attributes.ttp        = 4.0f;

   ta_cal.k_rta_f_higher_obj_crit_based_on_lower_ttp = FBK_FALSE;

   ta_core_output.ta_id[FBK_SIDE_LEFT]       = 2;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]      = 5.0f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT] = 5.0f;

   /** \action Check criticality */
   f_more_critical_ttp = Ta_Is_Ttp_More_Critical(&ta_core_output, &ta_object, &ta_cal, FBK_SIDE_LEFT);

   /** \assert Verify object is more critical. */
   EXPECT_TRUE(f_more_critical_ttp);
}

/*
 * Create a relevant target and fill the TA core output with default values. The target shoud be deemed more critical.
 * \uts{CSCSA-45131} \sdd{SF-8628} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Is_Ttc_More_Critical__true_ttc_based)
{
   /** \arrange Create warn relevant target with various properties. */
   boolean_T f_more_critical_ttc   = FBK_FALSE;
   ta_object.tracker_data.id       = 1;
   ta_object.attributes.alert_side = FBK_SIDE_LEFT;
   ta_object.attributes.ttc        = 2.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_ID;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT] = TA_INVALID_TTC;

   /** \action Check criticality */
   f_more_critical_ttc = Ta_Is_Ttc_More_Critical(&ta_core_output, &ta_object, FBK_SIDE_LEFT);

   /** \assert Verify object is more critical. */
   EXPECT_TRUE(f_more_critical_ttc);
}

/*
 * Create a relevant target and fill the TA core output with alert values of a different target with same TTC but bigger distance.
 * The target shoud be deemed more critical. \uts{CSCSA-45132} \sdd{SF-8628} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Is_Ttc_More_Critical__true_distance_based)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   boolean_T f_more_critical_ttc        = FBK_FALSE;
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]       = 5;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]      = ta_object.attributes.ttc;
   ta_core_output.ta_distance[FBK_SIDE_LEFT] = ta_object.attributes.distance_to_ego * 2.0f;

   /** \action Set most critical object per side */
   f_more_critical_ttc = Ta_Is_Ttc_More_Critical(&ta_core_output, &ta_object, FBK_SIDE_LEFT);

   /** \assert Verify object is more critical. */
   EXPECT_TRUE(f_more_critical_ttc);
}

/*
 * Create a relevant target and fill the TA core output with more crtitical alert values. The target shoud be deemed less critical.
 * \uts{CSCSA-45133} \sdd{SF-8628} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Is_Ttc_More_Critical__false_ttc_based)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   boolean_T f_more_critical_ttc        = FBK_TRUE;
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]       = 5;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]      = 0.5f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT] = 2.0f;

   /** \action Set most critical object per side */
   f_more_critical_ttc = Ta_Is_Ttc_More_Critical(&ta_core_output, &ta_object, FBK_SIDE_LEFT);

   /** \assert Verify object is less critical. */
   EXPECT_FALSE(f_more_critical_ttc);
}

/*
 * Create a relevant target and fill the TA core output with more crtitical alert values. The target shoud be deemed less critical.
 * \uts{CSCSA-45134} \sdd{SF-8628} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Warn_Logic_Test, Ta_Is_Ttc_More_Critical__false_distance_based)
{
   /** \arrange Create warn relevant target with various properties and fill TA core output with properties of a different target.
    */
   boolean_T f_more_critical_ttc        = FBK_TRUE;
   ta_object.tracker_data.id            = 1;
   ta_object.attributes.alert_side      = FBK_SIDE_LEFT;
   ta_object.attributes.ttc             = 1.0f;
   ta_object.attributes.distance_to_ego = 5.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]       = 5;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]      = ta_object.attributes.ttc;
   ta_core_output.ta_distance[FBK_SIDE_LEFT] = 2.0f;

   /** \action Set most critical object per side */
   f_more_critical_ttc = Ta_Is_Ttc_More_Critical(&ta_core_output, &ta_object, FBK_SIDE_LEFT);

   /** \assert Verify object is more critical. */
   EXPECT_FALSE(f_more_critical_ttc);
}
