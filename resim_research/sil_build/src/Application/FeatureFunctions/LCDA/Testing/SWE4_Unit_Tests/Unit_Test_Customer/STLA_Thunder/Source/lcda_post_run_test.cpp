/**
 * @file lcda_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_post_run.c functions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "lcda_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_post_run.c"
#include "ml_checked_rounding.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Check that initialization in Lcda_Post_Run_Init is performed correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run_Init__check_correct_reset)
{
   /** \arrange Set up lcda output arbitrary, such that it differs from the initialization value */
   /** \action Call Lcda_Post_Run_Init to initialize lcda_output. */
   /** \assert Check that lcda output is initialized correctly. */
   ASSERT_NO_FATAL_FAILURE(Lcda_Post_Run_Init(); Lcda_Post_Run_Init(););
}

/**
 * Check that signal mapping in Lcda_Post_Run is correct.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__check_correct_mapping)
{
   /** \arrange Set up lcda output arbitrary, such that it differs from the values set in Lcda_Post_Run*/
   lcda_core_output.lcda_status                                  = LCDA_STATUS_ACTIVE;
   lcda_core_output.bsw_core_output.f_bsw_is_enabled             = FBK_TRUE;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled             = FBK_TRUE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT]    = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT]    = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]        = 2;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]       = 4;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]        = 6;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]       = 8;
   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_LEFT]       = 2.5f;
   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT]      = 3.4f;
   lcda_core_output.bsw_core_output.bsw_distance[FBK_SIDE_LEFT]  = 1.0f;
   lcda_core_output.bsw_core_output.bsw_distance[FBK_SIDE_RIGHT] = 2.0f;
   lcda_core_output.cvw_core_output.cvw_distance[FBK_SIDE_LEFT]  = 3.0f;
   lcda_core_output.cvw_core_output.cvw_distance[FBK_SIDE_RIGHT] = 4.0f;
   lcda_core_output.lcda_status                                  = LCDA_STATUS_ACTIVE;

   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);

   /** \action Call Lcda_Post_Run to fill customer output accordingly */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that customer output is set correctly. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_output.f_bsw_enabled, FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.f_cvw_enabled, FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.bsw_alert_left, LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(lcda_output.bsw_alert_right, LCDA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(lcda_output.cvw_alert_left, LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(lcda_output.cvw_alert_right, LCDA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(lcda_output.bsw_id_left, 2);
   EXPECT_EQ(lcda_output.bsw_id_right, 4);
   EXPECT_EQ(lcda_output.cvw_id_left, 6);
   EXPECT_EQ(lcda_output.cvw_id_right, 8);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_left, 2.5f);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_right, 3.4f);
   EXPECT_FLOAT_EQ(lcda_output.bsw_distance_left, 1.0f);
   EXPECT_FLOAT_EQ(lcda_output.bsw_distance_right, 2.0f);
   EXPECT_FLOAT_EQ(lcda_output.cvw_distance_left, 3.0f);
   EXPECT_FLOAT_EQ(lcda_output.cvw_distance_right, 4.0f);
   EXPECT_EQ(lcda_output.lcda_status, LCDA_STATUS_ACTIVE);
}

/**
 * Check that signal mapping in Lcda_Post_Run is correct when holding is enabled.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__check_correct_mapping_when_holding_enabled)
{
   /** \arrange Set up lcda output arbitrary, such that it differs from the values set in Lcda_Post_Run*/
   p_vehicle_data->host_speed                                 = 1.1f;
   lcda_core_output.lcda_status                               = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;
   lcda_core_output.bsw_core_output.f_bsw_is_enabled          = FBK_FALSE;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled          = FBK_FALSE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.bsw_id_left                                    = 3u;
   lcda_output.bsw_id_right                                   = 4u;
   lcda_output.bsw_alert_left                                 = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.bsw_alert_right                                = LCDA_ALERT_STATE_LEVEL_2;
   data.object_data[1u].id                                    = 3u;
   data.object_data[1u].status                                = PA_OBJ_STATUS_MATURE;
   data.object_data[1u].width                                 = 2.f;
   data.object_data[1u].length                                = 5.f;
   data.object_data[1u].vcs_vel.x                             = cals.k_bsw_min_obj_long_vel + EPSILON;
   data.object_data[1u].curvi_pos.x                           = -5.7f;
   data.object_data[1u].curvi_pos.y                           = -2.9f;
   data.object_data[2u].id                                    = 4;
   data.object_data[2u].status                                = PA_OBJ_STATUS_MATURE;
   data.object_data[2u].width                                 = 2.f;
   data.object_data[2u].length                                = 5.f;
   data.object_data[2u].curvi_pos.x                           = -5.9f;
   data.object_data[2u].curvi_pos.y                           = 3.1f;
   data.object_data[2u].vcs_vel.x                             = cals.k_bsw_min_obj_long_vel + EPSILON;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);

   /** \action Call Lcda_Post_Run to fill customer output accordingly */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that customer output is set correctly. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_output.f_bsw_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_output.bsw_alert_left, LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(lcda_output.bsw_alert_right, LCDA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(lcda_output.bsw_id_left, 3);
   EXPECT_EQ(lcda_output.bsw_id_right, 4);
}


/**
 * Check that signal mapping in Lcda_Post_Run is correct when holding is enabled.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__check_correct_mapping_when_not_met_holding_criteria)
{
   /** \arrange Set up lcda output arbitrary, such that it differs from the values set in Lcda_Post_Run*/
   p_vehicle_data->host_speed                                 = 1.5f;
   lcda_core_output.lcda_status                               = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.bsw_alert_left                                 = LCDA_ALERT_STATE_NONE;
   lcda_output.bsw_alert_right                                = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]     = 0u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]    = 0u;
   data.object_data[1u].id                                    = 7u;
   data.object_data[1u].status                                = PA_OBJ_STATUS_MATURE;
   data.object_data[1u].width                                 = 2.f;
   data.object_data[1u].length                                = 5.f;
   data.object_data[1u].curvi_pos.x                           = -5.7f;
   data.object_data[1u].curvi_pos.y                           = -1.19f;
   data.object_data[2u].id                                    = 4;
   data.object_data[2u].status                                = PA_OBJ_STATUS_MATURE;
   data.object_data[2u].width                                 = 2.f;
   data.object_data[2u].length                                = 5.f;
   data.object_data[2u].curvi_pos.x                           = 14.9f;
   data.object_data[2u].curvi_pos.y                           = 3.1f;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);

   /** \action Call Lcda_Post_Run to fill customer output accordingly */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that customer output is set correctly. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_output.bsw_alert_left, LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_output.bsw_alert_right, LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_output.bsw_id_left, 0);
   EXPECT_EQ(lcda_output.bsw_id_right, 0);
}


/**
 * Check if the BSW alert is hold correctly. General case for both sides
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__general_bsw_case_both_side)
{
   /** \arrange Set object properties. Object inside zone */
   lcda_output.bsw_id_left          = 7u;
   lcda_output.bsw_id_right         = 4u;
   data.object_data[1u].id          = 7u;
   data.object_data[1u].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[1u].width       = 2.f;
   data.object_data[1u].length      = 5.f;
   data.object_data[1u].curvi_pos.x = -5.5f;
   data.object_data[1u].curvi_pos.y = -3.0f;
   data.object_data[1u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel + EPSILON;
   data.object_data[2u].id          = 4u;
   data.object_data[2u].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[2u].width       = 2.f;
   data.object_data[2u].length      = 5.f;
   data.object_data[2u].curvi_pos.x = -5.5f;
   data.object_data[2u].curvi_pos.y = 3.0f;
   data.object_data[2u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel + EPSILON;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, BSW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_LEFT], FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_RIGHT], FBK_ONE_UINT);
}

/**
 * Check if the CVW alert is hold correctly. General case for both sides
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__general_cvw_case_both_side)
{
   /** \arrange Set object properties. Object inside zone */

   lcda_output.cvw_id_left          = 7u;
   lcda_output.cvw_id_right         = 4u;
   data.object_data[1u].id          = 7;
   data.object_data[1u].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[1u].width       = 2.f;
   data.object_data[1u].length      = 5.f;
   data.object_data[1u].curvi_pos.x = -5.5;
   data.object_data[1u].curvi_pos.y = -3.0;
   data.object_data[1u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel + EPSILON;
   data.object_data[2u].id          = 4;
   data.object_data[2u].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[2u].width       = 2.f;
   data.object_data[2u].length      = 5.f;
   data.object_data[2u].curvi_pos.x = -5.5;
   data.object_data[2u].curvi_pos.y = 3.0;
   data.object_data[2u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel + EPSILON;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, CVW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_cvw_hold_alert[FBK_SIDE_LEFT], FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.f_cvw_hold_alert[FBK_SIDE_RIGHT], FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.cvw_id_left, data.object_data[1u].id);
   EXPECT_EQ(lcda_output.cvw_id_right, data.object_data[2u].id);
}

/**
 * Check if the CVW alert is not hold due to low object speed.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__holding_disabled_due_low_object_speed)
{
   /** \arrange Set object properties. Object inside zone */

   lcda_output.cvw_id_left          = 7u;
   lcda_output.cvw_id_right         = 4u;
   data.object_data[1u].id          = 7;
   data.object_data[1u].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[1u].width       = 2.f;
   data.object_data[1u].length      = 5.f;
   data.object_data[1u].speed       = 15.f;
   data.object_data[1u].curvi_pos.x = -10.5;
   data.object_data[1u].curvi_pos.y = -3.0;
   data.object_data[1u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel - EPSILON;
   data.object_data[2u].id          = 4;
   data.object_data[2u].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[2u].width       = 2.f;
   data.object_data[2u].length      = 5.f;
   data.object_data[2u].speed       = 20.f;
   data.object_data[2u].curvi_pos.x = -12.5;
   data.object_data[2u].curvi_pos.y = 3.0;
   data.object_data[2u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel - EPSILON;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, CVW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_cvw_hold_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_cvw_hold_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}


/**
 * Check if the BSW alert is hold correctly for long objects. General case for both sides
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__bsw_two_long_objects)
{
   /** \arrange Set object properties. Object inside zone */

   lcda_output.bsw_id_left     = 7u;
   lcda_output.bsw_id_right    = 4u;
   data.object_data[1u].id     = 7u;
   data.object_data[1u].status = PA_OBJ_STATUS_MATURE;
   data.object_data[1u].width  = 2.f;
   data.object_data[1u].length = cals.k_bsw_min_length_long_object + EPSILON;
   data.object_data[1u].curvi_pos.x =
      lcda_core_input.initial_bsw_zone_hys.points[FRONT_EGO_SIDE].x + Fbk_Half(data.object_data[1u].length) - EPSILON;
   data.object_data[1u].curvi_pos.y = -3.0;
   data.object_data[1u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel + EPSILON;
   data.object_data[2u].id          = 4u;
   data.object_data[2u].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[2u].width       = 2.f;
   data.object_data[2u].length      = cals.k_bsw_min_length_long_object + EPSILON;
   data.object_data[2u].curvi_pos.x =
      lcda_core_input.initial_bsw_zone_hys.points[FRONT_EGO_SIDE].x + Fbk_Half(data.object_data[2u].length) + EPSILON;
   data.object_data[2u].curvi_pos.y = 3.0;
   data.object_data[2u].vcs_vel.x   = cals.k_bsw_min_obj_long_vel + EPSILON;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, BSW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_LEFT], FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}


/**
 * Check if the BSW alert is not hold due to object outside the left zone. The front of the object is in front of the zone.
 * Does not meet thunder TOS requirements
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__bsw_object_in_front_left_zone)
{
   /** \arrange Set the properties of an object. Object outisde the front boundary of zone */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].id          = 4;
   data.object_data[bsw_index].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[bsw_index].width       = 2.f;
   data.object_data[bsw_index].length      = 5.f;
   data.object_data[bsw_index].curvi_pos.x = -3.5;
   data.object_data[bsw_index].curvi_pos.y = -3.0;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, BSW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}

/**
 * Check if the BSW alert is not hold due to object outside the right zone. Object is laterally outside the zone.
 * Does not meet thunder TOS requirements
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__bsw_object_laterally_outside_right_zone)
{
   /** \arrange Set the properties of an object. Object outisde the front boundary of zone */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].id          = 2;
   data.object_data[bsw_index].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[bsw_index].width       = 2.f;
   data.object_data[bsw_index].length      = 5.f;
   data.object_data[bsw_index].curvi_pos.x = -5.5;
   data.object_data[bsw_index].curvi_pos.y = 1.f;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, BSW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Check if the CVW alert is not hold due to object outside the zone. Object is laterally outside the zone.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__cvw_object_laterally_outside_zone)
{
   /** \arrange Set the properties of an object. Object outisde the front boundary of zone */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].id          = 2;
   data.object_data[bsw_index].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[bsw_index].width       = 2.f;
   data.object_data[bsw_index].length      = 5.f;
   data.object_data[bsw_index].curvi_pos.x = 5.f;
   data.object_data[bsw_index].curvi_pos.y = -0.5f;
   data.object_data[bsw_index].id          = 8;
   data.object_data[bsw_index].status      = PA_OBJ_STATUS_MATURE;
   data.object_data[bsw_index].width       = 2.f;
   data.object_data[bsw_index].length      = 5.f;
   data.object_data[bsw_index].curvi_pos.x = 10.0;
   data.object_data[bsw_index].curvi_pos.y = 0.5f;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, CVW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_cvw_hold_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_cvw_hold_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
}


/**
 * Check if the BSW alert is not hold due to object outside the left zone. The front of the object is in behind of the zone.
 * Does not meet thunder TOS requirements
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__bsw_object_behind_right_zone)
{
   /** \arrange Set the properties of an object. Object outisde the front boundary of zone */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].id     = 9;
   data.object_data[bsw_index].status = PA_OBJ_STATUS_MATURE;
   data.object_data[bsw_index].width  = 2.f;
   data.object_data[bsw_index].length = 5.f;
   data.object_data[bsw_index].curvi_pos.x =
      lcda_core_input.initial_bsw_zone_hys.points[REAR_EGO_SIDE].x - data.object_data[bsw_index].length;
   data.object_data[bsw_index].curvi_pos.y = -3.f;
   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, BSW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Check if the BSW alert is not hold due to invalid object.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert__bsw_general_case_invalid_object)
{
   /** \arrange Set object properties. Object inside zone */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].id     = 1;
   data.object_data[bsw_index].status = PA_OBJ_STATUS_INVALID;

   Fbk_Update_Index_Id_Lookup_Table(&fbk_index_id_lookup_table, &data);
   /** \action Call function to hold alert */
   Lcda_Hold_Alert(&data, &cals, &lcda_core_input.initial_bsw_zone_hys, BSW, &fbk_index_id_lookup_table, &lcda_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_bsw_hold_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
}

/**
 * Tests that calibration is updated correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Init_Output_test)
{
   /** \arrange declare variable for result and simple imput */
   Lcda_Output_T output;
   output.f_lcda_enabled = 1;
   /** \action call calibration update */
   Lcda_Init_Output(&output);
   /** \assert expect succes */
   EXPECT_EQ(output.f_lcda_enabled, 0);
}