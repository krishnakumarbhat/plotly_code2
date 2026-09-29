/**
 * @file ta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Rivian TA post run tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45232}
 */

#include "ta_post_run_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>
#include <math.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
#include "ta_constants.h"
#include "ta_post_run.c"
#include "ta_types.h"
}

/**
 * Check that TA post run is initialized correctly.
 * \uts{CSCSA-45233} \sdd{SF-8650} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run_Init__initialize_post_run)
{
   /** \arrange Set up TA status. */
   /** \action Call Ta_Post_Run_Init to reset post run. */
   /** \assert Check that post run is initialized correctly. */
   ASSERT_NO_FATAL_FAILURE(Ta_Post_Run_Init(););
}

/**
 * Check that TA post run is run correctly.
 * \uts{CSCSA-45234} \sdd{SF-8548} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__run_for_crit_object)
{
   /** \arrange Set up TA alert level. */
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_4;

   /** \action Call Ta_Post_Run. */
   Ta_Post_Run(&ta_instance, &ta_input, &ta_output);

   /** \assert Check that post run is run correctly. */
   EXPECT_EQ(ta_output.ta_alert[FBK_SIDE_RIGHT], FBK_TRUE);
}

/**
 * Check that TA_ALERT_STATE_NONE is map to false in Rivian output
 * \uts{CSCSA-65070} \sdd{SF-8548} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__no_crit_object)
{
   /** \arrange Set up TA alert level none. */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_NONE;

   /** \action Call Ta_Post_Run. */
   Ta_Post_Run(&ta_instance, &ta_input, &ta_output);

   /** \assert Check that post run is run correctly. */
   EXPECT_EQ(ta_output.ta_alert[FBK_SIDE_LEFT], FBK_FALSE);
}

/**
 * Check that TA TTP is mapped correctly to Rivian specific TA TTC.
 * \uts{CSCSA-65071} \sdd{CSCSA-65065} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_TTC_To_Rivian__Core_TTC_is_invalid)
{
   /** \arrange Set up TTC and TTP values. */
   float32_T rivian_ttc              = TA_INVALID_TTC;
   uint8_t side_index                = FBK_SIDE_LEFT;
   ta_core_output.ta_ttc[side_index] = TA_INVALID_TTC;
   ta_core_output.ta_ttp[side_index] = 2.1f;

   /** \action Call Ta_Map_TTC_To_Rivian to map data. */
   rivian_ttc = Ta_Map_TTC_To_Rivian(&ta_core_output, side_index);

   /** \assert Check that rivian ttc is mapped correctly. */
   EXPECT_EQ(rivian_ttc, ta_core_output.ta_ttp[side_index]);
}

/**
 * Check that TA TTC is mapped correctly to Rivian specific TA TTC.
 * \uts{CSCSA-65072} \sdd{CSCSA-65065} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_TTC_To_Rivian__Core_TTC_is_valid)
{
   /** \arrange Set up TTC and TTP values. */
   float32_T rivian_ttc              = TA_INVALID_TTC;
   uint8_t side_index                = FBK_SIDE_LEFT;
   ta_core_output.ta_ttc[side_index] = 1.9f;
   ta_core_output.ta_ttp[side_index] = 2.1f;

   /** \action Call Ta_Map_TTC_To_Rivian to map data. */
   rivian_ttc = Ta_Map_TTC_To_Rivian(&ta_core_output, side_index);

   /** \assert Check that rivian ttc is mapped correctly. */
   EXPECT_EQ(rivian_ttc, ta_core_output.ta_ttc[side_index]);
}

/**
 * Check that Rivian specific TA TTC take default value when core TTC and TTP are invalid.
 * \uts{CSCSA-65073} \sdd{CSCSA-65065} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_TTC_To_Rivian__Core_TTC_and_TTP_inavlid)
{
   /** \arrange Set up TTC and TTP values. */
   float32_T rivian_ttc              = TA_INVALID_TTC;
   uint8_t side_index                = FBK_SIDE_LEFT;
   ta_core_output.ta_ttc[side_index] = TA_INVALID_TTC;
   ta_core_output.ta_ttp[side_index] = TA_INVALID_TTP;

   /** \action Call Ta_Map_TTC_To_Rivian to map data. */
   rivian_ttc = Ta_Map_TTC_To_Rivian(&ta_core_output, side_index);

   /** \assert Check that rivian ttc is set correctly. */
   EXPECT_EQ(rivian_ttc, TA_INVALID_TTC);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45235} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_algorithm_disabled)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_NO_VALID_OBJECTS;
   boolean_T f_ta_enable          = FBK_FALSE;
   boolean_T vehicle_state_valid  = FBK_TRUE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_ALGORITHM_DISABLED);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45236} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_algorithm_disabled_default)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_ALGORITHM_DISABLED;
   boolean_T f_ta_enable          = FBK_TRUE;
   boolean_T vehicle_state_valid  = FBK_TRUE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_ALGORITHM_DISABLED);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45237} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_no_valid_objects_and_invalid_vehicle_state)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_NO_VALID_OBJECTS;
   boolean_T f_ta_enable          = FBK_TRUE;
   boolean_T vehicle_state_valid  = FBK_FALSE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_VEHICLE_STATE_INVALID);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45238} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_no_valid_objects)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_NO_VALID_OBJECTS;
   boolean_T f_ta_enable          = FBK_TRUE;
   boolean_T vehicle_state_valid  = FBK_TRUE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_NO_VALID_OBJECTS);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45239} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_invalid_vehicle_state)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_VEHICLE_STATE_INVALID;
   boolean_T f_ta_enable          = FBK_TRUE;
   boolean_T vehicle_state_valid  = FBK_TRUE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_VEHICLE_STATE_INVALID);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45240} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_no_relevant_objects)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_NO_RELEVANT_OBJECTS;
   boolean_T f_ta_enable          = FBK_TRUE;
   boolean_T vehicle_state_valid  = FBK_TRUE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_NO_RELEVANT_OBJECTS);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45241} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_no_critical_objects)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_NO_CRITICAL_OBJECTS;
   boolean_T f_ta_enable          = FBK_TRUE;
   boolean_T vehicle_state_valid  = FBK_TRUE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_NO_CRITICAL_OBJECTS);
}

/**
 * Check that TA status is mapped correctly to Rivian specific TA status.
 * \uts{CSCSA-45242} \sdd{SF-8635} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Status_To_Rivian__map_critical_object_detected)
{
   /** \arrange Set up TA status. */
   Ta_Algorithm_State_T ta_status = TA_STATE_CRITICAL_OBJECT_DETECTED;
   boolean_T f_ta_enable          = FBK_TRUE;
   boolean_T vehicle_state_valid  = FBK_TRUE;

   /** \action Call Ta_Map_Status_To_Rivian to map status. */
   Ta_Rivian_Status_T ta_rivian_status = Ta_Map_Status_To_Rivian(ta_status, f_ta_enable, vehicle_state_valid);

   /** \assert Check that rivian specific status is set correctly. */
   EXPECT_EQ(ta_rivian_status, RIVIAN_TA_CRITICAL_OBJECT_DETECTED);
}

/**
 * Check that vehicle status is set to valid correctly according to Rivian specific requirements.
 * \uts{CSCSA-45243} \sdd{SF-8634} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Vehicle_State_Valid__host_speed_within_limits_yawrate_within_limits)
{
   /** \arrange Set up vehicle parameters. */
   p_vehicle_data->host_speed = 10.0f; //[m/s]
   p_vehicle_data->yawrate    = 0.5f;  //[rad/s]

   /** \action Call Ta_Vehicle_State_Valid to check status. */
   boolean_T ta_vehicle_state_valid = Ta_Vehicle_State_Valid(&data, &ta_cals);

   /** \assert Check that vehicle status is set correctly. */
   EXPECT_TRUE(ta_vehicle_state_valid);
}

/**
 * Check that vehicle status is set to valid correctly according to Rivian specific requirements.
 * \uts{CSCSA-45244} \sdd{SF-8634} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Vehicle_State_Valid__host_speed_over_limit_yawrate_within_limits)
{
   /** \arrange Set up vehicle parameters. */
   p_vehicle_data->host_speed = 35.0f; //[m/s]
   p_vehicle_data->yawrate    = 0.5f;  //[rad/s]

   /** \action Call Ta_Vehicle_State_Valid to check status. */
   boolean_T ta_vehicle_state_valid = Ta_Vehicle_State_Valid(&data, &ta_cals);

   /** \assert Check that vehicle status is set correctly. */
   EXPECT_FALSE(ta_vehicle_state_valid);
}

/**
 * Check that vehicle status is set to valid correctly according to Rivian specific requirements.
 * \uts{CSCSA-45245} \sdd{SF-8634} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Vehicle_State_Valid__host_speed_below_limit_yawrate_within_limits)
{
   /** \arrange Set up vehicle parameters. */
   p_vehicle_data->host_speed = 0.0f; //[m/s]
   p_vehicle_data->yawrate    = 0.5f; //[rad/s]

   /** \action Call Ta_Vehicle_State_Valid to check status. */
   boolean_T ta_vehicle_state_valid = Ta_Vehicle_State_Valid(&data, &ta_cals);

   /** \assert Check that vehicle status is set correctly. */
   EXPECT_FALSE(ta_vehicle_state_valid);
}

/**
 * Check that vehicle status is set to valid correctly according to Rivian specific requirements.
 * \uts{CSCSA-45246} \sdd{SF-8634} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Vehicle_State_Valid__host_speed_within_limits_yawrate_over_limit)
{
   /** \arrange Set up vehicle parameters. */
   p_vehicle_data->host_speed = 5.0f; //[m/s]
   p_vehicle_data->yawrate    = 1.2f; //[rad/s]

   /** \action Call Ta_Vehicle_State_Valid to check status. */
   boolean_T ta_vehicle_state_valid = Ta_Vehicle_State_Valid(&data, &ta_cals);

   /** \assert Check that vehicle status is set correctly. */
   EXPECT_FALSE(ta_vehicle_state_valid);
}

/**
 * Check that vehicle status is set to valid correctly according to Rivian specific requirements.
 * \uts{CSCSA-45247} \sdd{SF-8634} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Vehicle_State_Valid__host_speed_within_limits_yawrate_negative_over_limit)
{
   /** \arrange Set up vehicle parameters. */
   p_vehicle_data->host_speed = 5.0f;  //[m/s]
   p_vehicle_data->yawrate    = -1.2f; //[rad/s]

   /** \action Call Ta_Vehicle_State_Valid to check status. */
   boolean_T ta_vehicle_state_valid = Ta_Vehicle_State_Valid(&data, &ta_cals);

   /** \assert Check that vehicle status is set correctly. */
   EXPECT_FALSE(ta_vehicle_state_valid);
}