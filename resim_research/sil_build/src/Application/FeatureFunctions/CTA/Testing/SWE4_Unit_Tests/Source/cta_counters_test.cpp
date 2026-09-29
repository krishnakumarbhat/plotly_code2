/**
 * @file cta_counters_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_counters.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41864}
 */

#include "cta_counters_test.hpp"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_counters.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

using ::testing::FloatNear;

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Call Cta_Process_Current_Alert_Level with
 * a higher criticallity level. Verify criticallity is increased and object id updated \uts{CSCSA-41865} \sdd{SF-3761}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Criticallity_Level_Increased)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                                    = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]                  = CTA_CRIT_LEVEL_1;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction]        = 4;
   object.tracker_data.id                                                        = 5;
   cta_persistent.previous_most_critical_unique_obj_id[mode][approach_direction] = 6;
   object.tracker_data.unique_id                                                 = 7;

   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_2, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_2);
   EXPECT_EQ(cta_persistent.previous_crit_level[mode][approach_direction], CTA_CRIT_LEVEL_2);
   EXPECT_EQ(cta_persistent.previous_most_critical_obj_id[mode][approach_direction], object.tracker_data.id);
   EXPECT_EQ(cta_persistent.previous_most_critical_unique_obj_id[mode][approach_direction], object.tracker_data.unique_id);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Call Cta_Process_Current_Alert_Level with
 * a higher criticallity level. Verify criticallity is decreased and object id updated \uts{CSCSA-188371} \sdd{SF-3761}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Criticallity_Level_Decreased)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                                    = FBK_SIDE_LEFT;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                                 = FBK_TRUE;
   cals.k_cta_cycle_count_hold_true_warning                                      = 0u;
   cta_persistent.previous_crit_level[mode][approach_direction]                  = CTA_CRIT_LEVEL_2;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction]        = 4;
   object.tracker_data.id                                                        = 5;
   cta_persistent.previous_most_critical_unique_obj_id[mode][approach_direction] = 6;
   object.tracker_data.unique_id                                                 = 7;

   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_NONE, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_NONE);
   EXPECT_EQ(cta_persistent.previous_crit_level[mode][approach_direction], CTA_CRIT_LEVEL_NONE);
   EXPECT_EQ(cta_persistent.previous_most_critical_obj_id[mode][approach_direction], FBK_ZERO_UINT);
   EXPECT_EQ(cta_persistent.previous_most_critical_unique_obj_id[mode][approach_direction], FBK_ZERO_UINT);
}


/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Last and current object IDs are the same.
 * Call Cta_Process_Current_Alert_Level with a higher criticallity level. Verify Suppression counter is set to max value.
 * \uts{CSCSA-41866} \sdd{SF-3761} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Counter_Increased_Below_Threshold_Object_Ids_Equal)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                                    = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]                  = CTA_CRIT_LEVEL_2;
   cta_persistent.warning_holding_counter[mode][approach_direction]              = 3;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction]        = 5;
   object.tracker_data.id                                                        = 5;
   object.persistent->crit_level_suppression_counter[mode][CTA_CRIT_LEVEL_2 - 1] = 7;

   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_1, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][CTA_CRIT_LEVEL_1], CTA_COUNTER_MAX);
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_2);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_3 and approach side FBK_SIDE_LEFT. Last and current object IDs differ. Call
 * Cta_Process_Current_Alert_Level with a higher criticallity level. Verify Suppression counter is not set to max value.
 * \uts{CSCSA-41867} \sdd{SF-3761} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Counter_Increased_Below_Threshold_Object_Ids_Differ)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                                    = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]                  = CTA_CRIT_LEVEL_2;
   cta_persistent.warning_holding_counter[mode][approach_direction]              = 3;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction]        = 5;
   object.tracker_data.id                                                        = 6;
   object.persistent->crit_level_suppression_counter[mode][CTA_CRIT_LEVEL_2 - 1] = 7;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                                 = 0u;

   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_1, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_NE(object.persistent->crit_level_suppression_counter[mode][CTA_CRIT_LEVEL_1], CTA_COUNTER_MAX);
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_2);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Call Cta_Process_Current_Alert_Level with
 * lower criticallity level. Verify criticallity level is prevented from falling back to level 1. \uts{CSCSA-41868} \sdd{SF-3761}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Counter_Increased_Above_Threshold_Prevent_Fall_Back)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                       = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]     = CTA_CRIT_LEVEL_2;
   cta_persistent.warning_holding_counter[mode][approach_direction] = 15;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                    = 1u;

   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_1, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_2);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Call Cta_Process_Current_Alert_Level with
 * CTA_CRIT_LEVEL_1. Verify counter for approaching side is reset and filled with id of approaching object. \uts{CSCSA-41869}
 * \sdd{SF-3761} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Counter_Increased_Above_Threshold_Warning_Level_Active)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                       = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]     = CTA_CRIT_LEVEL_2;
   cta_persistent.warning_holding_counter[mode][approach_direction] = 15;
   object.tracker_data.id                                           = 7;
   object.tracker_data.unique_id                                    = 7;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                    = 0u;
   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_1, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(cta_persistent.warning_holding_counter[mode][approach_direction], 0);
   EXPECT_EQ(cta_persistent.previous_most_critical_obj_id[mode][approach_direction], object.tracker_data.id);
   EXPECT_EQ(cta_persistent.previous_most_critical_unique_obj_id[mode][approach_direction], object.tracker_data.unique_id);
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_1);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Call Cta_Process_Current_Alert_Level with
 * CTA_CRIT_LEVEL_NONE. Verify that id of approaching object is reset. \uts{CSCSA-41870} \sdd{SF-3761}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Counter_Increased_Above_Threshold_Warning_Level_Inactive)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                                    = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]                  = CTA_CRIT_LEVEL_2;
   cta_persistent.warning_holding_counter[mode][approach_direction]              = 15;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction]        = 4;
   object.tracker_data.id                                                        = 7;
   cta_persistent.previous_most_critical_unique_obj_id[mode][approach_direction] = 4;
   object.tracker_data.unique_id                                                 = 7;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                                 = 0u;
   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_NONE, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(cta_persistent.previous_most_critical_obj_id[mode][approach_direction], 0);
   EXPECT_EQ(cta_persistent.previous_most_critical_unique_obj_id[mode][approach_direction], 0u);
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_NONE);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_1 and approach side FBK_SIDE_LEFT. Call Cta_Process_Current_Alert_Level with
 * a higher criticallity level but tracker_output pointer is NULL. Verify criticallity is not increased and object id is not
 * updated. \uts{CSCSA-41871} \sdd{SF-3761} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Criticallity_Level_Increased_Tracker_Output_Is_Null)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                             = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]           = CTA_CRIT_LEVEL_1;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction] = 4;
   object.tracker_data.id                                                 = PA_INVALID_OBJ_ID;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                          = 0u;

   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_2, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_NE(crit_level, CTA_CRIT_LEVEL_2);
   EXPECT_NE(cta_persistent.previous_crit_level[mode][approach_direction], CTA_CRIT_LEVEL_2);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Call Cta_Process_Current_Alert_Level with
 * CTA_CRIT_LEVEL_NONE but tracker_output is NULL. Verify that id of approaching object is not reset. \uts{CSCSA-41872}
 * \sdd{SF-3761} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Counter_Increased_Above_Threshold_Tracker_Output_Is_Null)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                             = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]           = CTA_CRIT_LEVEL_2;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction] = 4;
   cta_persistent.warning_holding_counter[mode][approach_direction]       = 15;
   object.tracker_data.id                                                 = PA_INVALID_OBJ_ID;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                          = 0u;

   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_NONE, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_NONE);
}

/**
 * Create a counter with warning level CTA_CRIT_LEVEL_2 and approach side FBK_SIDE_LEFT. Last and current object IDs are the same,
 * but persistent is NULL. Call Cta_Process_Current_Alert_Level with a higher criticallity level. Verify criticallity level did not
 * change. \uts{CSCSA-41873} \sdd{SF-3761} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Process_Current_Alert_Level__Counter_Increased_Below_Threshold_Persistent_Is_Null)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                             = FBK_SIDE_LEFT;
   cta_persistent.previous_crit_level[mode][approach_direction]           = CTA_CRIT_LEVEL_2;
   cta_persistent.warning_holding_counter[mode][approach_direction]       = 3;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction] = 6;
   object.tracker_data.id                                                 = 6;
   object.persistent                                                      = NULL;
   cals.k_cta_f_prevent_fall_back_to_critlevel_1                          = 0u;
   /** \action execute function Cta_Process_Current_Alert_Level */
   Cta_Crit_Level_T crit_level =
      Cta_Process_Current_Alert_Level(&cta_persistent, CTA_CRIT_LEVEL_1, mode, approach_direction, &object, &cals);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(crit_level, CTA_CRIT_LEVEL_2);
}


/**
 * Create a counter with approach side FBK_SIDE_LEFT. Last and current object IDs are the same and object is not within field of
 * interest. Call Cta_Check_Stop_Level_Holding. Verify objects counter for approach side FBK_SIDE_LEFT is set to max counter value.
 * \uts{CSCSA-41874} \sdd{SF-3760} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Check_Stop_Level_Holding__object_Ids_Same_Not_Within_Field_Of_Interest)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                             = FBK_SIDE_LEFT;
   boolean_T f_within_field_of_interest                                   = FBK_FALSE;
   cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT]            = 20;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction] = 6;
   object.tracker_data.id                                                 = 6;
   object.attributes->ttc                                                 = -1;

   /** \action execute function Cta_Check_Stop_Level_Holding */
   Cta_Check_Stop_Level_Holding(&cta_persistent, &object, f_within_field_of_interest);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT], CTA_COUNTER_MAX);
}

/**
 * Create a counter with approach side FBK_SIDE_LEFT. Last and current object IDs are the same and object is within field of
 * interest. Call Cta_Check_Stop_Level_Holding. Verify objects counter for approach side FBK_SIDE_LEFT is not set to max counter
 * value. \uts{CSCSA-41875} \sdd{SF-3760} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Check_Stop_Level_Holding__object_Ids_Same_Within_Field_Of_Interest)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                             = FBK_SIDE_LEFT;
   boolean_T f_within_field_of_interest                                   = FBK_TRUE;
   cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT]            = 20;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction] = 6;
   object.tracker_data.id                                                 = 6;

   /** \action execute function Cta_Check_Stop_Level_Holding */
   Cta_Check_Stop_Level_Holding(&cta_persistent, &object, f_within_field_of_interest);

   /** \assert Verify expected outcome. */
   EXPECT_NE(cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT], CTA_COUNTER_MAX);
}

/**
 * Create a counter with approach side FBK_SIDE_LEFT. Last and current object IDs are the same and object is within field of
 * interest. Call Cta_Check_Stop_Level_Holding. Verify objects counter for approach side FBK_SIDE_LEFT is not set to max counter
 * value. \uts{CSCSA-41876} \sdd{SF-3760} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Check_Stop_Level_Holding__object_Ids_Differ)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                             = FBK_SIDE_LEFT;
   boolean_T f_within_field_of_interest                                   = FBK_TRUE;
   cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT]            = 20;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction] = 5;
   object.tracker_data.id                                                 = 6;

   /** \action execute function Cta_Check_Stop_Level_Holding */
   Cta_Check_Stop_Level_Holding(&cta_persistent, &object, f_within_field_of_interest);

   /** \assert Verify expected outcome. */
   EXPECT_NE(cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT], CTA_COUNTER_MAX);
}

/**
 * Create a counter with approach side FBK_SIDE_LEFT. Last and current object IDs are the same and object is not within field of
 * interest. Verify objects counter for approach side FBK_SIDE_LEFT is not set to max counter value due to invalid TTC
 * \uts{CSCSA-188372} \sdd{SF-3760} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Counters_Test, Cta_Check_Stop_Level_Holding__Within_Field_Of_Interest_invalid_ttc)
{

   /** \arrange Create a counter with properties as described in unit test description. */
   uint8_t approach_direction                                             = FBK_SIDE_LEFT;
   boolean_T f_within_field_of_interest                                   = FBK_FALSE;
   cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT]            = 20;
   cta_persistent.previous_most_critical_obj_id[mode][approach_direction] = 6;
   object.tracker_data.id                                                 = 6;
   object.attributes->ttc                                                 = FBK_ZERO_F + EPSILON;

   /** \action execute function Cta_Check_Stop_Level_Holding */
   Cta_Check_Stop_Level_Holding(&cta_persistent, &object, f_within_field_of_interest);

   /** \assert Verify expected outcome. */
   EXPECT_EQ(cta_persistent.warning_holding_counter[mode][FBK_SIDE_LEFT], 20);
}
