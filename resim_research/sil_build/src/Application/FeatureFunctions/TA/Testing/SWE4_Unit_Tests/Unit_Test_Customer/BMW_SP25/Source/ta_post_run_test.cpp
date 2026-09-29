/**
 * @file ta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 TA post run tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45153}
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
#include "ta_bmw_enums.h"
#include "ta_constants.h"
#include "ta_post_run.c"
#include "ta_types.h"
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Get_Maneuver_Direction throws exception, when ta input pointer is NULL.
 * \uts{CSCSA-45157} \sdd{SF-8560} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Maneuver_Direction__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Maneuver_Direction throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Get_Maneuver_Direction(NULL, NULL); }, ".*p_ta_input.*");
}

/**
 * Checks, if Ta_Get_Maneuver_Direction throws exception, when vehicle data pointer is NULL.
 * \uts{CSCSA-45212} \sdd{SF-8560} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Maneuver_Direction__vehicle_data_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Maneuver_Direction throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Get_Maneuver_Direction(&ta_input, NULL); }, ".*p_vehicle_data.*");
}

#endif // !NDEBUG

/**
 * Checks, if Ta_Get_Maneuver_Direction returns straight maneuver, when steering angle is between left and right max values.
 * \uts{CSCSA-45159} \sdd{SF-8560} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Maneuver_Direction__returns_straight_if_steering_angle_within_limits)
{
   /** \arrange Declare variable of type uint8_t as target for return value of tested function. Assign steering angle a value
    * between left and right thresholds. */
   Bmw_Maneuver_Direction_State_T maneuver_direction;
   ta_input.fta_steering_angle_max_left  = 30;
   ta_input.fta_steering_angle_max_right = 30;
   p_vehicle_data->steering_angle        = 0.0f;

   /** \action Call function Ta_Get_Maneuver_Direction with parameters ta_input. */
   maneuver_direction = Ta_Get_Maneuver_Direction(&ta_input, p_vehicle_data);

   /** \assert Check, if determined maneuver is straight. */
   EXPECT_EQ(maneuver_direction, BMW_MANEUVER_DIRECTION_STRAIGHT);
}

/**
 * Checks, if Ta_Get_Maneuver_Direction returns turn maneuver, when steering angle is beyond right max value.
 * \uts{CSCSA-45160} \sdd{SF-8560} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Maneuver_Direction__returns_turn_if_steering_angle_beyond_right_limit)
{
   /** \arrange Declare variable of type uint8_t as target for return value of tested function. Assign steering angle a value
    * beyond right threshold. */
   Bmw_Maneuver_Direction_State_T maneuver_direction;
   ta_input.fta_steering_angle_max_left  = 40;
   ta_input.fta_steering_angle_max_right = 30;
   p_vehicle_data->steering_angle        = 31.0f * (PI / 180.0f);

   /** \action Call function Ta_Get_Maneuver_Direction with parameters ta_input. */
   maneuver_direction = Ta_Get_Maneuver_Direction(&ta_input, p_vehicle_data);

   /** \assert Check, if determined maneuver is turn. */
   EXPECT_EQ(maneuver_direction, BMW_MANEUVER_DIRECTION_TURN);
}

/**
 * Checks, if Ta_Get_Maneuver_Direction returns turn maneuver, when steering angle is beyond left max value.
 * \uts{CSCSA-45161} \sdd{SF-8560} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Maneuver_Direction__returns_turn_if_steering_angle_beyond_left_limit)
{
   /** \arrange Declare variable of type uint8_t as target for return value of tested function. Assign steering angle a value
    * beyond left threshold. */
   Bmw_Maneuver_Direction_State_T maneuver_direction;
   ta_input.fta_steering_angle_max_left  = 30;
   ta_input.fta_steering_angle_max_right = 40;
   p_vehicle_data->steering_angle        = -31.0f * (PI / 180.0f);

   /** \action Call function Ta_Get_Maneuver_Direction with parameters ta_input. */
   maneuver_direction = Ta_Get_Maneuver_Direction(&ta_input, p_vehicle_data);

   /** \assert Check, if determined maneuver is turn. */
   EXPECT_EQ(maneuver_direction, BMW_MANEUVER_DIRECTION_TURN);
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Get_Symbol_Request throws exception, when ta cal pointer is NULL.
 * \uts{CSCSA-45162} \sdd{SF-8561} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Symbol_Request__ta_cal_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Maneuver_Direction throws exception, when p_ta_cal is NULL pointer. */
   EXPECT_DEATH({ Ta_Get_Symbol_Request(NULL, FBK_FALSE, FBK_FALSE); }, ".*p_ta_cal.*");
}

#endif // !NDEBUG

/**
 * Checks, if Ta_Get_Symbol_Request returns 'CENTRAL_CLOSE', when cal parameter k_pfgs_symbol_request_sides_enabled is 0
 * \uts{CSCSA-45163} \sdd{SF-8561} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Symbol_Request__returns_person_central_as_default)
{
   /** \arrange Declare variable of type boolean_T as target for return value of tested function. Set all input parameters to 0 */
   Bmw_Symbol_Request_T symbol_request;
   boolean_T obj_in_zone_left                  = FBK_FALSE;
   boolean_T obj_in_zone_right                 = FBK_FALSE;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 0;

   /** \action Call function Ta_Get_Symbol_Request with parameters ta_cals, obj_in_zone_left and obj_in_zone_right. */
   symbol_request = Ta_Get_Symbol_Request(&ta_cals, obj_in_zone_left, obj_in_zone_right);

   /** \assert Check, if returned symbol is 'CENTRAL_CLOSE'. */
   EXPECT_EQ(symbol_request, BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE);
}

/**
 * Checks, if Ta_Get_Symbol_Request returns 'LEFT', when cal parameter k_pfgs_symbol_request_sides_enabled is 1 and obj in left
 * zone. \uts{CSCSA-45164} \sdd{SF-8561} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Symbol_Request__returns_left_when_object_in_left_zone)
{
   /** \arrange Declare variable of type boolean_T as target for return value of tested function. Set all input parameters to 1
    * except obj_in_zone_right */
   Bmw_Symbol_Request_T symbol_request;
   boolean_T obj_in_zone_left                  = FBK_TRUE;
   boolean_T obj_in_zone_right                 = FBK_FALSE;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 1;

   /** \action Call function Ta_Get_Symbol_Request with parameters ta_cals, obj_in_zone_left and obj_in_zone_right. */
   symbol_request = Ta_Get_Symbol_Request(&ta_cals, obj_in_zone_left, obj_in_zone_right);

   /** \assert Check, if returned symbol is 'LEFT'. */
   EXPECT_EQ(symbol_request, BMW_SYMBOL_REQUEST_PERSON_LEFT);
}

/**
 * Checks, if Ta_Get_Symbol_Request returns 'RIGHT', when cal parameter k_pfgs_symbol_request_sides_enabled is 1 and obj in left
 * zone. \uts{CSCSA-45165} \sdd{SF-8561} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Get_Symbol_Request__returns_left_when_object_in_right_zone)
{
   /** \arrange Declare variable of type boolean_T as target for return value of tested function. Set all input parameters to 1
    * except obj_in_zone_left */
   Bmw_Symbol_Request_T symbol_request;
   boolean_T obj_in_zone_left                  = FBK_FALSE;
   boolean_T obj_in_zone_right                 = FBK_TRUE;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 1;

   /** \action Call function Ta_Get_Symbol_Request with parameters ta_cals, obj_in_zone_left and obj_in_zone_right. */
   symbol_request = Ta_Get_Symbol_Request(&ta_cals, obj_in_zone_left, obj_in_zone_right);

   /** \assert Check, if returned symbol is 'RIGHT'. */
   EXPECT_EQ(symbol_request, BMW_SYMBOL_REQUEST_PERSON_RIGHT);
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Map_Rta_Output_Signals throws exception, when ta output pointer is NULL.
 * \uts{CSCSA-45166} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__ta_output_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Map_Rta_Output_Signals throws exception, when p_ta_output is NULL pointer. */
   EXPECT_DEATH({ Ta_Map_Rta_Output_Signals(NULL, &ta_input, &ta_core_output, &data); }, ".*p_ta_output.*");
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals throws exception, when ta input pointer is NULL.
 * \uts{CSCSA-45167} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Map_Rta_Output_Signals throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Map_Rta_Output_Signals(&ta_output, NULL, &ta_core_output, &data); }, ".*p_ta_input.*");
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals throws exception, when ta core output pointer is NULL.
 * \uts{CSCSA-45168} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__ta_core_output_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Map_Rta_Output_Signals throws exception, when p_ta_core_output is NULL pointer. */
   EXPECT_DEATH({ Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, NULL, &data); }, ".*p_ta_core_output.*");
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals throws exception, when vehicle data pointer is NULL.
 * \uts{CSCSA-45213} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__vehicle_data_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Map_Rta_Output_Signals throws exception, when p_ta_core_output is NULL pointer. */
   EXPECT_DEATH({ Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, NULL); }, ".*p_pa_data.*");
}

#endif // !NDEBUG

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output to default zone.
 * \uts{CSCSA-45169} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__no_valid_object_default_rta_output)
{
   /** \arrange Set required input values in a way to provide invalid object ids */
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_ID;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_NONE;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_NONE;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all rta related parameters are set to default values. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, 10.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, 10.0f);

   EXPECT_EQ(ta_output.rta_dynamic_area_status, 1);
   EXPECT_EQ(ta_output.rta_turning_area_status, 1);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.rta_id_right, TA_BMW_TARGET_ID_INVALID);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output to default zone.
 * \uts{CSCSA-45170} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__valid_object_default_rta_output)
{
   /** \arrange Set required input values in a way to provide valid object ids */
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = 3;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = 4;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_NONE;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_NONE;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all rta related parameters are set to default values. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, 10.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, 10.0f);

   EXPECT_EQ(ta_output.rta_dynamic_area_status, 1);
   EXPECT_EQ(ta_output.rta_turning_area_status, 1);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_id_right, ta_core_output.ta_id[FBK_SIDE_RIGHT]);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output to default on right side, while parameters on left
 * side are set according to input object data. \uts{CSCSA-45171} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__valid_object_left_wing_zone_correct_output)
{
   /** \arrange Set required input values in a way to provide invalid object ids */
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   p_vehicle_data->rear_axle_position = -4.0f;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = 2.0f;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = 1;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_1;

   ta_core_output.ta_index[FBK_SIDE_LEFT] = ta_core_output.ta_id[FBK_SIDE_LEFT] - 1;

   object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_pos.x             = -10.0f;
   object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_pos.y             = 3.0f;
   object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_vel.x             = 2.0f;
   object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_vel.y             = -1.0f;
   object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].existence_probability = 0.9f;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all rta related parameters are set to default values on right side, while on left side are set according
    * to the input object data. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left,
                   object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, -object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, -object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left,
                   floor(100.0f * object_data[ta_core_output.ta_index[FBK_SIDE_LEFT]].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, 10.0f);

   EXPECT_EQ(ta_output.rta_dynamic_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK);
   EXPECT_EQ(ta_output.rta_turning_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_id_right, TA_BMW_TARGET_ID_INVALID);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output to default on left side, while parameters on right
 * side are set according to input object data. \uts{CSCSA-45172} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__valid_object_right_wing_zone_correct_output)
{
   /** \arrange Set required input values in a way to provide invalid object ids */
   uint8_t obj_idx_right = 1;

   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_TRUE;

   p_vehicle_data->rear_axle_position = -4.0f;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = 1.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = 2.0f;

   ta_core_output.ta_index[FBK_SIDE_RIGHT] = obj_idx_right;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_ID;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = obj_idx_right + 1;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_1;

   object_data[obj_idx_right].vcs_pos.x             = -10.0f;
   object_data[obj_idx_right].vcs_pos.y             = 3.0f;
   object_data[obj_idx_right].vcs_vel.x             = 2.0f;
   object_data[obj_idx_right].vcs_vel.y             = -1.0f;
   object_data[obj_idx_right].existence_probability = 0.9f;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all rta related parameters are set to default values on left side, while on right side are set according
    * to the input object data. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, object_data[obj_idx_right].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, -object_data[obj_idx_right].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, object_data[obj_idx_right].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, -object_data[obj_idx_right].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, floor(100.0f * object_data[obj_idx_right].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, 10.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, ta_core_output.ta_ttc[FBK_SIDE_RIGHT]);

   EXPECT_EQ(ta_output.rta_dynamic_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK);
   EXPECT_EQ(ta_output.rta_turning_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.rta_id_right, ta_core_output.ta_id[FBK_SIDE_RIGHT]);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output to default on right side, while parameters on left
 * side are set according to input object data. \uts{CSCSA-45173} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__valid_object_left_info_zone_correct_output)
{
   /** \arrange Set required input values in a way to provide invalid object ids */
   uint8_t obj_idx_left = 0;

   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   p_vehicle_data->rear_axle_position = -4.0f;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = TA_INVALID_TTC;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = TA_INVALID_TTC;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]  = 3.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT] = 4.0f;

   ta_core_output.ta_index[FBK_SIDE_LEFT] = obj_idx_left;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = obj_idx_left + 1;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_1;

   object_data[obj_idx_left].vcs_pos.x             = -10.0f;
   object_data[obj_idx_left].vcs_pos.y             = 3.0f;
   object_data[obj_idx_left].vcs_vel.x             = 2.0f;
   object_data[obj_idx_left].vcs_vel.y             = -1.0f;
   object_data[obj_idx_left].existence_probability = 0.9f;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all rta related parameters are set to default values on right side, while on left side are set according
    * to the input object data. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, object_data[obj_idx_left].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, -object_data[obj_idx_left].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, object_data[obj_idx_left].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, -object_data[obj_idx_left].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, floor(100.0f * object_data[obj_idx_left].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, ta_core_output.ta_ttp[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, 10.0f);

   EXPECT_EQ(ta_output.rta_dynamic_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK);
   EXPECT_EQ(ta_output.rta_turning_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_id_right, TA_BMW_TARGET_ID_INVALID);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output to default on left side, while parameters on right
 * side are set according to input object data. \uts{CSCSA-45174} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__valid_object_right_info_zone_correct_output)
{
   /** \arrange Set required input values in a way to provide invalid object ids */
   uint8_t obj_idx_right = 1;

   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_TRUE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   p_vehicle_data->rear_axle_position = -4.0f;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = TA_INVALID_TTC;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = TA_INVALID_TTC;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]  = 3.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT] = 4.0f;

   ta_core_output.ta_index[FBK_SIDE_RIGHT] = obj_idx_right;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_ID;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = obj_idx_right + 1;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_1;

   object_data[obj_idx_right].vcs_pos.x             = -10.0f;
   object_data[obj_idx_right].vcs_pos.y             = 3.0f;
   object_data[obj_idx_right].vcs_vel.x             = 2.0f;
   object_data[obj_idx_right].vcs_vel.y             = -1.0f;
   object_data[obj_idx_right].existence_probability = 0.9f;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all rta related parameters are set to default values on left side, while on right side are set according
    * to the input object data. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, object_data[obj_idx_right].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, -object_data[obj_idx_right].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, object_data[obj_idx_right].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, -object_data[obj_idx_right].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, floor(100.0f * object_data[obj_idx_right].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, 10.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, ta_core_output.ta_ttp[FBK_SIDE_RIGHT]);

   EXPECT_EQ(ta_output.rta_dynamic_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK);
   EXPECT_EQ(ta_output.rta_turning_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.rta_id_right, ta_core_output.ta_id[FBK_SIDE_RIGHT]);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output according to valid objects on both sides in input
 * object data. \uts{CSCSA-45175} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__valid_object_left_right_info_wing_zone_level1)
{
   /** \arrange Set required input values in a way to provide invalid object ids */
   uint8_t obj_idx_left  = 4;
   uint8_t obj_idx_right = 5;

   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_TRUE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_TRUE;

   p_vehicle_data->rear_axle_position = -4.0f;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = TA_INVALID_TTC;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = TA_INVALID_TTC;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]  = 3.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT] = 4.0f;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_1;

   ta_core_output.ta_index[FBK_SIDE_LEFT]  = obj_idx_left;
   ta_core_output.ta_index[FBK_SIDE_RIGHT] = obj_idx_right;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = obj_idx_left + 1;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = obj_idx_right + 1;

   object_data[obj_idx_left].vcs_pos.x             = -10.0f;
   object_data[obj_idx_left].vcs_pos.y             = 3.0f;
   object_data[obj_idx_left].vcs_vel.x             = 2.0f;
   object_data[obj_idx_left].vcs_vel.y             = -1.0f;
   object_data[obj_idx_left].existence_probability = 0.9f;

   object_data[obj_idx_right].vcs_pos.x             = -9.0f;
   object_data[obj_idx_right].vcs_pos.y             = -4.0f;
   object_data[obj_idx_right].vcs_vel.x             = 3.0f;
   object_data[obj_idx_right].vcs_vel.y             = 2.0f;
   object_data[obj_idx_right].existence_probability = 0.95f;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, object_data[obj_idx_left].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, object_data[obj_idx_right].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, -object_data[obj_idx_left].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, -object_data[obj_idx_right].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, object_data[obj_idx_left].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, object_data[obj_idx_right].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, -object_data[obj_idx_left].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, -object_data[obj_idx_right].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, floor(100.0f * object_data[obj_idx_left].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, floor(100.0f * object_data[obj_idx_right].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, ta_core_output.ta_ttp[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, ta_core_output.ta_ttp[FBK_SIDE_RIGHT]);

   EXPECT_EQ(ta_output.rta_dynamic_area_status,
             TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK | TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK);
   EXPECT_EQ(ta_output.rta_turning_area_status,
             TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK | TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_id_right, ta_core_output.ta_id[FBK_SIDE_RIGHT]);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets rta related parameters in TA output according to valid objects on both sides in input
 * object data. \uts{CSCSA-45176} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__valid_object_left_right_info_wing_zone_level2)
{
   /** \arrange Set required input values in a way to provide invalid object ids */
   uint8_t obj_idx_left  = 9;
   uint8_t obj_idx_right = 15;

   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_TRUE;

   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_TRUE;

   p_vehicle_data->rear_axle_position = -4.0f;
   p_vehicle_data->curvature          = ta_cals.k_tap_lvl_2_host_curvature_min + EPSILON;

   ta_input.f_rta_enable              = FBK_TRUE;
   ta_input.f_rta_enable_turning_area = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area = FBK_TRUE;

   ta_core_output.ta_ttc[FBK_SIDE_LEFT]  = TA_INVALID_TTC;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT] = TA_INVALID_TTC;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]  = 3.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT] = 4.0f;

   ta_core_output.ta_index[FBK_SIDE_LEFT]  = obj_idx_left;
   ta_core_output.ta_index[FBK_SIDE_RIGHT] = obj_idx_right;

   ta_core_output.ta_id[FBK_SIDE_LEFT]  = obj_idx_left + 1;
   ta_core_output.ta_id[FBK_SIDE_RIGHT] = obj_idx_right + 1;

   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_2;

   object_data[obj_idx_left].vcs_pos.x             = -10.0f;
   object_data[obj_idx_left].vcs_pos.y             = 3.0f;
   object_data[obj_idx_left].vcs_vel.x             = 2.0f;
   object_data[obj_idx_left].vcs_vel.y             = -1.0f;
   object_data[obj_idx_left].existence_probability = 0.9f;

   object_data[obj_idx_right].vcs_pos.x             = -9.0f;
   object_data[obj_idx_right].vcs_pos.y             = -4.0f;
   object_data[obj_idx_right].vcs_vel.x             = 3.0f;
   object_data[obj_idx_right].vcs_vel.y             = 2.0f;
   object_data[obj_idx_right].existence_probability = 0.95f;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, object_data[obj_idx_left].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, object_data[obj_idx_right].vcs_pos.x - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, -object_data[obj_idx_left].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, -object_data[obj_idx_right].vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, object_data[obj_idx_left].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, object_data[obj_idx_right].vcs_vel.x);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, -object_data[obj_idx_left].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, -object_data[obj_idx_right].vcs_vel.y);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, floor(100.0f * object_data[obj_idx_left].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, floor(100.0f * object_data[obj_idx_right].existence_probability + 0.5f));
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, ta_core_output.ta_ttp[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, ta_core_output.ta_ttp[FBK_SIDE_RIGHT]);

   EXPECT_EQ(ta_output.rta_dynamic_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK
                                                   | TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK | TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK
                                                   | TA_RTA_AREA_RIGHT_ALERT_LEVEL_MASK);
   EXPECT_EQ(ta_output.rta_turning_area_status, TA_RTA_AREA_SYSTEM_LIMITS_MASK | TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK
                                                   | TA_RTA_AREA_RIGHT_CAR_IN_AREA_MASK | TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK
                                                   | TA_RTA_AREA_RIGHT_ALERT_LEVEL_MASK);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_id_right, ta_core_output.ta_id[FBK_SIDE_RIGHT]);
}

/**
 * Checks, if Ta_Map_Rta_Output_Signals sets correct default TTC value in case of alert holding
 * \uts{CSCSA-45202} \sdd{SF-8563} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Rta_Output_Signals__fill_default_ttc_value_for_alert_holding)
{
   /** \arrange Set required ta alert values to holding a alert level 1 */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_id[FBK_SIDE_LEFT]           = 1u;
   ta_core_output.ta_id[FBK_SIDE_RIGHT]          = 2u;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]          = TA_INVALID_TTC;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT]         = TA_INVALID_TTC;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]          = TA_INVALID_TTP;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT]         = TA_INVALID_TTP;

   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input and ta_core_output. */
   Ta_Map_Rta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &data);

   /** \assert Check, if correct default TTC value is set. */
   EXPECT_EQ(ta_output.rta_ttc_left, TA_TAP_DEFAULT_TTC);
   EXPECT_EQ(ta_output.rta_ttc_right, TA_TAP_DEFAULT_TTC);
}


/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in TA output to default values
 * \uts{CSCSA-45177} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__no_object_in_both_danger_zones)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set flags for danger zones on both sides to false. */
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_UNDEFINED;

   ta_input.f_fta_enable = FBK_TRUE;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, 0u);

   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_target_id, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_INVALID);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_NO_WARNING);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_NO_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, TA_INVALID_TTC);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in TA output correctly, when object is in left zone
 * \uts{CSCSA-45178} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__object_in_left_danger_zone)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set flag for left danger zone to true. */
   uint8_t obj_idx = 0;

   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = 1u;

   ta_core_output.ta_most_critical_side         = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]       = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]          = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_3;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.1f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]         = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = TA_INVALID_DISTANCE;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold = 1.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_NO_WARNING);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_NO_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in TA output correctly, when object is in left zone
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__object_in_right_danger_zone)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set flag for left danger zone to true. */
   uint8_t obj_idx = 0;

   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = 1u;
   Ta_Bmw_Persistent.fta_prev_alert_level                 = (uint8_t) BMW_ALERT_LEVEL_ACUTE_WARNING;

   ta_core_output.ta_most_critical_side         = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]       = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]          = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_3;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.1f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]         = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = TA_INVALID_DISTANCE;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold = 1.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_NO_WARNING);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_NO_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in TA output correctly, when object is in left zone
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__object_in_danger_zone_alert)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set flag for left danger zone to true. */
   uint8_t obj_idx = 0;

   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = 1u;
   Ta_Bmw_Persistent.fta_prev_alert_level                 = (uint8_t) BMW_ALERT_LEVEL_NO_WARNING;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_LEVEL_3;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = 1.1f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = TA_INVALID_DISTANCE;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold = 1.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_NO_WARNING);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_NO_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, TA_INVALID_TTC);
}

/**
 * Checks, if target id in FTA output is saturated to TA_BMW_TARGET_ID_INVALID
 * \uts{CSCSA-45179} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__target_id_out_of_limits)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set object id in core output to TA_BMW_TARGET_ID_INVALID + 1. */
   uint8_t obj_idx = 0;

   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;

   ta_core_output.ta_most_critical_side         = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]       = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]          = PA_INVALID_OBJ_ID;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_3;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]         = 1.1f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]         = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]    = TA_INVALID_DISTANCE;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold = 1.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if fta_target_id in TA output is TA_BMW_TARGET_ID_INVALID. */
   EXPECT_EQ(ta_output.fta_target_id, TA_BMW_TARGET_ID_INVALID);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get a 600 ms brake conditioning request
 * \uts{CSCSA-45180} \sdd{SF-8567} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Fta_Brake_Conditioning__brake_request_600ms)
{
   /** \arrange Set fta_brake_deceleration_request to 0.0f. */
   ta_output.fta_brake_deceleration_request = 0.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output and a ttb of 600ms. */
   Ta_Set_Fta_Brake_Conditioning(&ta_output, TA_BMW_BRAKE_CONDITIONING_THRESHOLD_600_MS);

   /** \assert Check whether brake coniditioning is set to 600ms. */
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_REQUEST_IN_600_MS);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get a 800 ms brake conditioning request
 * \uts{CSCSA-45181} \sdd{SF-8567} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Fta_Brake_Conditioning__brake_request_800ms)
{
   /** \arrange Set fta_brake_deceleration_request to 0.0f. */
   ta_output.fta_brake_deceleration_request = 0.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output and a ttb of 800ms. */
   Ta_Set_Fta_Brake_Conditioning(&ta_output, TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_REQUEST_IN_800_MS);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get a 400 ms brake conditioning request
 * \uts{CSCSA-45182} \sdd{SF-8567} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Fta_Brake_Conditioning__brake_request_400ms)
{
   /** \arrange Set fta_brake_deceleration_request to 0.0f. */
   ta_output.fta_brake_deceleration_request = 0.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output and a ttb of 400ms. */
   Ta_Set_Fta_Brake_Conditioning(&ta_output, TA_BMW_BRAKE_CONDITIONING_THRESHOLD_400_MS);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_REQUEST_IN_400_MS);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get a 200 ms brake conditioning request
 * \uts{CSCSA-45183} \sdd{SF-8567} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Fta_Brake_Conditioning__brake_request_200ms)
{
   /** \arrange Set fta_brake_deceleration_request to 0.0f. */
   ta_output.fta_brake_deceleration_request = 0.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output and a ttb of 200ms. */
   Ta_Set_Fta_Brake_Conditioning(&ta_output, TA_BMW_BRAKE_CONDITIONING_THRESHOLD_200_MS);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_REQUEST_IN_200_MS);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets no brake conditioning request for ttb higher 800 ms
 * \uts{CSCSA-45184} \sdd{SF-8567} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Fta_Brake_Conditioning__no_brake_request_with_high_ttb)
{
   /** \arrange Set fta_brake_deceleration_request to 0.0f. */
   ta_output.fta_brake_deceleration_request = 0.0f;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output and a ttc of 800ms + Epsilon. */
   Ta_Set_Fta_Brake_Conditioning(&ta_output, TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS + EPSILON);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get a no request
 * \uts{CSCSA-45185} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__no_brake_request)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set ttb below -k_fta_brake_conditioning_time_offset. */
   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_LEVEL_3;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = 1.3f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = 0.41f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = 60.1f;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = 3.0f;
   ta_core_output.ta_n_valid_objects                      = 1;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);
   object_data[obj_idx].speed = ta_cals.k_fta_obj_speed[TA_MIN];

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold      = 1.5f;
   ta_cals.k_ta_alert_lvl_4_ttc_threshold      = 1.0f;
   ta_cals.k_ta_alert_lvl_4_decel_threshold    = 4.0f;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 0;

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_slow_obj - 1u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_target_id, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_LIMIT_MAX);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_REQUEST_IN_600_MS);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get a request
 * \uts{CSCSA-45186} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__brake_request_high_decel_estimate)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set ttb above TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS and ta_decel_estimate above
    * k_fta_brake_deceleration_request_threshold. */
   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_TRUE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_LEVEL_4;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = 1.0f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS + 0.1f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = 60.1f;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = 3.0f;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);
   object_data[obj_idx].speed = ta_cals.k_fta_obj_speed[TA_MIN];

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold      = 1.5f;
   ta_cals.k_ta_alert_lvl_4_ttc_threshold      = 1.1f;
   ta_cals.k_ta_alert_lvl_4_decel_threshold    = 2.0f;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 0;
   ta_cals.k_f_fta_enable_brake_gradient_logic = FBK_FALSE;
   ta_cals.k_fta_brake_gradient                = 0.0f;

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_slow_obj - 1u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_target_id, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_LIMIT_MAX);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get a request
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__brake_request_high_decel_estimate_brake_start)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set ttb above TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS and ta_decel_estimate above
    * k_fta_brake_deceleration_request_threshold. */
   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_TRUE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_LEVEL_4;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = 1.0f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS + 0.1f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = 60.1f;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = 3.0f;
   Ta_Bmw_Persistent.ta_host_speed_at_brake_start         = 42.0f;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);
   object_data[obj_idx].speed = ta_cals.k_fta_obj_speed[TA_MIN];

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold      = 1.5f;
   ta_cals.k_ta_alert_lvl_4_ttc_threshold      = 1.1f;
   ta_cals.k_ta_alert_lvl_4_decel_threshold    = 2.0f;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 0;
   ta_cals.k_f_fta_enable_brake_gradient_logic = FBK_FALSE;
   ta_cals.k_fta_brake_gradient                = 0.0f;

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_slow_obj - 1u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_target_id, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_LIMIT_MAX);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}


/**
 * Checks, if warning qualification is not achieved, if host velocity is above threshold
 * \uts{CSCSA-45187} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__default_output_if_high_host_velocity)
{
   /** \arrange Considered level 4 alert scenario. Set host velocity equal above host speed threshold. */
   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_LEVEL_4;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = 1.0f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS + 0.1f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = 60.1f;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = 3.0f;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);
   object_data[obj_idx].speed = ta_cals.k_fta_obj_speed[TA_MIN];

   ta_input.f_fta_enable = FBK_TRUE;

   p_vehicle_data->host_speed = ta_cals.k_pfgs_ego_speed[TA_MAX] + EPSILON;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold      = 1.5f;
   ta_cals.k_ta_alert_lvl_4_ttc_threshold      = 1.1f;
   ta_cals.k_ta_alert_lvl_4_decel_threshold    = 2.0f;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 0;
   ta_cals.k_f_fta_enable_brake_gradient_logic = FBK_FALSE;
   ta_cals.k_fta_brake_gradient                = 0.0f;

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = 0u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_NO_WARNING);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_NO_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get fade in brake request
 * \uts{CSCSA-45188} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__fade_in_brake_request_high_decel_estimate)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Set ttb above BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS and ta_decel_estimate above
    * k_fta_brake_deceleration_request_threshold. */

   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_LEVEL_4;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = 1.0f;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = TA_BMW_BRAKE_CONDITIONING_THRESHOLD_800_MS + 0.1f;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = 60.1f;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = 3.0f;

   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, obj_idx);
   object_data[obj_idx].speed   = ta_cals.k_fta_obj_speed[TA_MIN];
   data.time_diff_to_last_cycle = 0.05f;

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_ta_alert_lvl_3_ttc_threshold      = 1.5f;
   ta_cals.k_ta_alert_lvl_4_ttc_threshold      = 1.1f;
   ta_cals.k_ta_alert_lvl_4_decel_threshold    = 2.0f;
   ta_cals.k_pfgs_symbol_request_sides_enabled = 0;
   ta_cals.k_f_fta_enable_brake_gradient_logic = 1u;

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_slow_obj - 1u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;
   float32_T decel_request_max_delta = Fbk_Abs_F(ta_cals.k_fta_brake_gradient * data.time_diff_to_last_cycle);

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_target_id, ta_core_output.ta_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_LIMIT_MAX);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, decel_request_max_delta);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get fade out brake request
 * \uts{CSCSA-45189} \sdd{SF-8562} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__fade_out_brake_request_no_decel_estimate)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. */

   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_UNDEFINED;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_NONE;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = TA_INVALID_TTC;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = TA_INVALID_TTB;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = TA_INVALID_DISTANCE;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = FBK_ZERO_F;

   object_data[obj_idx].speed   = ta_cals.k_fta_obj_speed[TA_MIN];
   data.time_diff_to_last_cycle = 0.05f;

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_f_fta_enable_brake_gradient_logic = 1u;

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_slow_obj - 1u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = ta_cals.k_fta_brake_deceleration_max;
   float32_T decel_request_max_delta = Fbk_Abs_F(ta_cals.k_fta_brake_gradient * data.time_diff_to_last_cycle) * 0.5f;
   float32_T expected_brake          = ta_cals.k_fta_brake_deceleration_max - decel_request_max_delta;
   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_target_id, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_LIMIT_MAX);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, expected_brake);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals sets fta related parameters in a way to get fade out brake request
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__fade_out_brake_request_brake_gradient)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. */

   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_UNDEFINED;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_NONE;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = TA_INVALID_TTC;
   ta_core_output.ta_ttb[FBK_SIDE_LEFT]                   = TA_INVALID_TTB;
   ta_core_output.ta_distance[FBK_SIDE_LEFT]              = TA_INVALID_DISTANCE;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = FBK_ZERO_F;

   object_data[obj_idx].speed   = ta_cals.k_fta_obj_speed[TA_MIN];
   data.time_diff_to_last_cycle = 0.05f;

   ta_input.f_fta_enable = FBK_TRUE;

   ta_cals.k_f_fta_enable_brake_gradient_logic = 0u;

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_slow_obj - 1u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = ta_cals.k_fta_brake_deceleration_max;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.fta_target_id, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_LIMIT_MAX);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, ta_core_output.ta_ttc[FBK_SIDE_LEFT]);
}

/**
 * Checks, if Ta_Map_Fta_Output_Signals does not send a brake request for coasted objects
 * \uts{CSCSA-45196} \sdd{SF-8562} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Map_Fta_Output_Signals__dont_start_braking_for_coasted_obj)
{
   /** \arrange Set critical ta core output that would indicate a brake request. */

   uint8_t obj_idx                                        = 10;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_TRUE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_LEFT;
   ta_core_output.ta_index[FBK_SIDE_LEFT]                 = obj_idx;
   ta_core_output.ta_id[FBK_SIDE_LEFT]                    = obj_idx + 1;
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]           = TA_ALERT_STATE_LEVEL_4;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]                   = 0.8f;
   ta_core_output.ta_decel_estimate[FBK_SIDE_LEFT]        = 10.0f;

   object_data[obj_idx].status = PA_OBJ_STATUS_COASTED;
   object_data[obj_idx].speed  = ta_cals.k_fta_obj_speed[TA_MIN];

   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_slow_obj;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call function Ta_Map_Fta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Map_Fta_Output_Signals(&ta_output, &ta_input, &ta_core_output, &ta_cals, &data);

   /** \assert Check, if all output parameters are set as expected. */
   EXPECT_EQ(ta_output.fta_target_id, obj_idx + 1);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Post_Run throws exception, when ta output is NULL.
 * \uts{CSCSA-45190} \sdd{SF-8548} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__ta_output_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Maneuver_Direction throws exception, when p_ta_output is NULL pointer. */
   EXPECT_DEATH({ Ta_Post_Run(&ta_instance, &ta_input, NULL); }, ".*p_ta_output.*");
}

/**
 * Checks, if Ta_Post_Run throws exception, when ta core output is NULL.
 * \uts{CSCSA-45191} \sdd{SF-8548} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__p_ta_instance_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Maneuver_Direction throws exception, when p_ta_core_output is NULL pointer. */
   EXPECT_DEATH({ Ta_Post_Run(NULL, &ta_input, &ta_output); }, ".*p_ta_instance.*");
}

/**
 * Checks, if Ta_Post_Run throws exception, when ta input is NULL.
 * \uts{CSCSA-45192} \sdd{SF-8548} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Get_Maneuver_Direction throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Post_Run(&ta_instance, NULL, &ta_output); }, ".*p_ta_input.*");
}
#endif // !NDEBUG

/**
 * Checks, checks if all interfaces are filled correctly by Ta_Post_Run and its subroutines
 * \uts{CSCSA-45194} \sdd{SF-8548} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__iface_check_debug_mode_disabled)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized
    * variables. Disable debug mode and set ta object ids to invalid. */

   // Input parameters from core output
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_id[FBK_SIDE_LEFT]           = PA_INVALID_OBJ_ID;
   ta_core_output.ta_id[FBK_SIDE_RIGHT]          = PA_INVALID_OBJ_ID;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]          = 5.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT]         = 6.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]          = 7.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT]         = 8.0f;

   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cals.k_f_ta_enable_debug_mode   = 0u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   // Input parameters for subfunction Ta_Get_Maneuver_Direction
   ta_input.fta_steering_angle_max_left  = 30;
   ta_input.fta_steering_angle_max_right = 30;
   p_vehicle_data->steering_angle        = 0.0f;
   p_vehicle_data->curvature             = ta_cals.k_ta_lookup_turning_host_curvature_min[FBK_ZERO_UINT];

   // Input parameters for subfunction Ta_Map_Fta_Output_Signals
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_UNDEFINED;
   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, 0u);

   // Input parameters for subfunction Ta_Map_Rta_Output_Signals
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_input.f_rta_enable                                = FBK_TRUE;
   ta_input.f_rta_enable_turning_area                   = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area                   = FBK_TRUE;

   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Post_Run(&ta_instance, &ta_input, &ta_output);

   /** \assert Check, if all output parameters are set correctly. */
   // value checks related to subfunction Ta_Is_Diagnostic_Mode_Enabled
   EXPECT_EQ(ta_output.f_diagnostic_mode, FBK_FALSE);
   // value checks related to subfunction Ta_Get_Maneuver_Direction
   EXPECT_EQ(ta_output.fta_maneuver_direction, BMW_MANEUVER_DIRECTION_STRAIGHT);
   // value checks related to subfunction Ta_Map_Fta_Output_Signals
   EXPECT_EQ(ta_output.fta_target_id, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_INVALID);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_NO_WARNING);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_NO_WARNING);
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, TA_INVALID_TTC);
   // value checks related to subfunction Ta_Map_Rta_Output_Signals
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, 0.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, 10.0f);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, 10.0f);
   EXPECT_EQ(ta_output.rta_dynamic_area_status, 1);
   EXPECT_EQ(ta_output.rta_turning_area_status, 1);
   EXPECT_EQ(ta_output.rta_alert_left, ta_core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ta_output.rta_alert_right, ta_core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ta_output.rta_id_left, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.rta_id_right, TA_BMW_TARGET_ID_INVALID);
}

/**
 * Checks, if Ta_Qualify_Pfgs_Alert resets persistent qualification counter in case that no ta alert level and no deceleration
 * requrests are given. \uts{CSCSA-45197} \sdd{SF-8564} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Qualify_Pfgs_Alert__no_alert_no_deceleration_request)
{
   /** \arrange set up no alert without any active deceleration */
   Ta_Alert_State_T fta_alert_level                             = TA_ALERT_STATE_NONE;
   Pa_Obj_Status_T object_status                                = PA_OBJ_STATUS_MATURE;
   float32_T fta_ttc                                            = TA_INVALID_TTC;
   uint8_t fta_index                                            = FBK_ZERO_UINT;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;

   /** \action Call qualification logic of pfgs alert. */
   Ta_Qualify_Pfgs_Alert(&data, p_vehicle_data, &ta_cals, fta_index, fta_alert_level, object_status, fta_ttc);

   /** \assert Verify that counter is reset. */
   EXPECT_EQ(Ta_Bmw_Persistent.pfgs_qualification_counter, FBK_ZERO_UINT);
}

/**
 * Checks, if Ta_Qualify_Pfgs_Alert resets persistent qualification counter in case the maximum PFGS cycle count is reached.
 * \uts{CSCSA-45204} \sdd{SF-8564} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Qualify_Pfgs_Alert__reset_pfgs_qualification_when_max_cycles_count_is_reached)
{
   /** \arrange set up no alert without any active deceleration */
   Ta_Alert_State_T fta_alert_level                             = TA_ALERT_STATE_NONE;
   Pa_Obj_Status_T object_status                                = PA_OBJ_STATUS_MATURE;
   float32_T fta_ttc                                            = TA_INVALID_TTC;
   uint8_t fta_index                                            = FBK_ZERO_UINT;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = 10.0f;
   Ta_Bmw_Persistent.pfgs_qualification_counter                 = TA_PFGS_QUALIFICATION_MAX;

   /** \action Call qualification logic of pfgs alert. */
   Ta_Qualify_Pfgs_Alert(&data, p_vehicle_data, &ta_cals, fta_index, fta_alert_level, object_status, fta_ttc);

   /** \assert Verify that counter is reset. */
   EXPECT_EQ(Ta_Bmw_Persistent.pfgs_qualification_counter, FBK_ZERO_UINT);
}

/**
 * Checks, if Ta_Qualify_Pfgs_Alert resets persistent qualification counter in case the ego comes to standstill.
 * \uts{CSCSA-45205} \sdd{SF-8564} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Qualify_Pfgs_Alert__reset_pfgs_qualification_when_ego_speed_zero)
{
   /** \arrange set up no alert without any active deceleration */
   Ta_Alert_State_T fta_alert_level                             = TA_ALERT_STATE_NONE;
   Pa_Obj_Status_T object_status                                = PA_OBJ_STATUS_MATURE;
   float32_T fta_ttc                                            = TA_INVALID_TTC;
   uint8_t fta_index                                            = FBK_ZERO_UINT;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = 10.0f;
   Ta_Bmw_Persistent.pfgs_qualification_counter                 = ta_cals.k_pfgs_qualification_counter_fast_obj;
   p_vehicle_data->host_speed                                   = FBK_ZERO_F;

   /** \action Call qualification logic of pfgs alert. */
   Ta_Qualify_Pfgs_Alert(&data, p_vehicle_data, &ta_cals, fta_index, fta_alert_level, object_status, fta_ttc);

   /** \assert Verify that counter is reset. */
   EXPECT_EQ(Ta_Bmw_Persistent.pfgs_qualification_counter, FBK_ZERO_UINT);
}

/**
 * Checks that the PFGS qualification counter is not increased for objects below the minimum TTC threshold.
 * \uts{CSCSA-45208} \sdd{SF-8564} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Qualify_Pfgs_Alert__no_pfgs_qualification_increase_for_low_ttc)
{
   /** \arrange set up no alert without any active deceleration */
   Ta_Alert_State_T fta_alert_level             = TA_ALERT_STATE_LEVEL_4;
   Pa_Obj_Status_T object_status                = PA_OBJ_STATUS_MATURE;
   float32_T fta_ttc                            = ta_cals.k_pfgs_qualification_ttc_min - 0.1f;
   uint8_t fta_index                            = FBK_ZERO_UINT;
   Ta_Bmw_Persistent.pfgs_qualification_counter = FBK_ZERO_UINT;
   p_vehicle_data->host_speed                   = ta_cals.k_pfgs_ego_speed[TA_MIN];

   /** \action Call qualification logic of pfgs alert. */
   Ta_Qualify_Pfgs_Alert(&data, p_vehicle_data, &ta_cals, fta_index, fta_alert_level, object_status, fta_ttc);

   /** \assert Verify that the counter is not increased. */
   EXPECT_EQ(Ta_Bmw_Persistent.pfgs_qualification_counter, FBK_ZERO_UINT);
}


/**
 * Checks that an object is qualified to cause PFGS alert.
 * \uts{CSCSA-45206} \sdd{SF-8614} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Is_Pfgs_Alert_Qualified__most_critical_object_is_qualified)
{
   /** \arrange Set up qualified object alert. */
   Ta_Alert_State_T fta_alert_level             = TA_ALERT_STATE_LEVEL_3;
   float32_T obj_speed                          = ta_cals.k_fta_obj_speed[TA_MIN];
   Ta_Bmw_Persistent.pfgs_qualification_counter = ta_cals.k_pfgs_qualification_counter_slow_obj;

   /** \action Call Ta_Is_Pfgs_Alert_Qualified. */
   boolean_T result = Ta_Is_Pfgs_Alert_Qualified(fta_alert_level, obj_speed, &ta_cals);

   /** \assert Verify that the result is correct. */
   EXPECT_TRUE(result);
}

/**
 * Checks that an object is not qualified to cause PFGS alert.
 * \uts{CSCSA-45207} \sdd{SF-8614} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Is_Pfgs_Alert_Qualified__most_critical_object_is_not_qualified)
{
   /** \arrange Set up object that is not (yet) qualified. */
   Ta_Alert_State_T fta_alert_level             = TA_ALERT_STATE_LEVEL_3;
   float32_T obj_speed                          = ta_cals.k_fta_obj_speed[TA_MIN];
   Ta_Bmw_Persistent.pfgs_qualification_counter = FBK_ZERO_UINT;

   /** \action Call Ta_Is_Pfgs_Alert_Qualified. */
   boolean_T result = Ta_Is_Pfgs_Alert_Qualified(fta_alert_level, obj_speed, &ta_cals);

   /** \assert Verify that the result is correct. */
   EXPECT_FALSE(result);
}

/**
 * Checks, if Ta_Set_Pfgs_Warning_Signals sets alert level and threshold reduction respectively.
 * \uts{CSCSA-45198} \sdd{SF-8565} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Pfgs_Warning_Signals__symbol_request_disabled)
{
   /** \arrange set up non default values for output */
   ta_cals.k_pfgs_symbol_request_sides_enabled = FBK_FALSE;
   ta_output.fta_alert_level                   = BMW_ALERT_LEVEL_NO_WARNING;
   ta_output.fta_brake_threshold_reduction     = BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC;

   /** \action Call warning signals setter of pfgs. */
   Ta_Set_Pfgs_Warning_Signals(&ta_output, &ta_core_output, &ta_cals);

   /** \assert Verify that mapping os done correct. */
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
}

/**
 * Checks, if Ta_Set_Pfgs_Warning_Signals sets alert level and threshold reduction respectively.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Pfgs_Warning_Signals__symbol_request_enabled)
{
   /** \arrange set up non default values for output */
   ta_cals.k_pfgs_symbol_request_sides_enabled = FBK_FALSE;
   ta_output.fta_alert_level                   = BMW_ALERT_LEVEL_NO_WARNING;
   ta_output.fta_brake_threshold_reduction     = BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC;
   ta_output.fta_symbol_request                = (uint8_t) BMW_SYMBOL_REQUEST_PERSON_CENTRAL_CLOSE;

   /** \action Call warning signals setter of pfgs. */
   Ta_Set_Pfgs_Warning_Signals(&ta_output, &ta_core_output, &ta_cals);

   /** \assert Verify that mapping os done correct. */
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_ACUTE_WARNING);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY);
}

/**
 * Checks, if Ta_Set_Pfgs_Braking_Signals sets brake deceleration request signal to the current estimate, since it is not capping
 * the limits. \uts{CSCSA-45199} \sdd{SF-8566} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Pfgs_Braking_Signals__symbol_request_disabled)
{
   /** \arrange set current deceleration estimate such that it is not capped internally. */
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = -1.0f;
   data.time_diff_to_last_cycle                                 = 0.05f;
   ta_output.ta_current_deceleration_estimate = 0.9f * ta_cals.k_fta_brake_gradient * data.time_diff_to_last_cycle;

   /** \action Call braking signals setter of pfgs. */
   Ta_Set_Pfgs_Braking_Signals(&ta_output, &data, &ta_cals);

   /** \assert Verify that current deceleration estimate is . */
   EXPECT_FLOAT_EQ(ta_output.fta_brake_deceleration_request, ta_output.ta_current_deceleration_estimate);
}


/**
 * Checks, if Ta_Fill_Fta_Relevant_Object_List fills first object on the output list of FTA for valid fta index.
 * \uts{CSCSA-45200} \sdd{SF-8568} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Fill_Fta_Relevant_Object_List__fills_first_object_for_valid_fta_index)
{
   /** \arrange Set fta index to valid id and set some tracker data to non-default values. */
   uint8_t fta_index                            = 5u;
   object_data[fta_index].vcs_heading           = 0.5f;
   object_data[fta_index].existence_probability = 0.89f;
   object_data[fta_index].vcs_vel.y             = 2.3f;

   /** \action Call Fta relevant object list. */
   Ta_Fill_Fta_Relevant_Object_List(&ta_output, &data, fta_index);

   /** \assert Verify that first object of output list is filled with tracker data. */
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_exist_prob, object_data[fta_index].existence_probability);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yaw_angle, object_data[fta_index].vcs_heading);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_vel, object_data[fta_index].vcs_vel.y);
}


/**
 * Checks, if Ta_Set_Rta_Area_Status lets the area status remain on 0.
 * \uts{CSCSA-45201} \sdd{SF-8569} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Rta_Area_Status__check_decoding_when_obj_not_in_dynamic_and_not_in_turning_area)
{
   /** \arrange set rta state to none. */
   uint8_t rta_alert_state = 0u;

   /** \action Call rta are status estimation. */
   Ta_Set_Rta_Area_Status(&ta_output, rta_alert_state, FBK_FALSE, FBK_FALSE, TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK,
                          TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK);

   /** \assert Verify that area states are remaining on zero. */
   EXPECT_EQ(ta_output.rta_dynamic_area_status, 0u);
   EXPECT_EQ(ta_output.rta_turning_area_status, 0u);
}

/**
 * Checks that area status does not change in case of alert holding
 * \uts{CSCSA-45203} \sdd{SF-8569} \testtype{negative}
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Rta_Area_Status__skip_update_for_alert_holding)
{
   /** \arrange Set up alert state that indicates alert holding. Alert state is set, but no object in zone detected in this cycle.
    */
   uint8_t rta_alert_state         = 1u;
   boolean_T f_obj_in_dynamic_area = FBK_FALSE;
   boolean_T f_obj_in_turning_area = FBK_FALSE;

   uint8_t test_value                = 123u;
   ta_output.rta_dynamic_area_status = test_value;
   ta_output.rta_turning_area_status = test_value;

   /** \action Call rta are status estimation. */
   Ta_Set_Rta_Area_Status(&ta_output, rta_alert_state, f_obj_in_dynamic_area, f_obj_in_turning_area,
                          TA_RTA_AREA_LEFT_CAR_IN_AREA_MASK, TA_RTA_AREA_LEFT_ALERT_LEVEL_MASK);

   /** \assert Verify that area states are remaining on their previous values. */
   EXPECT_EQ(ta_output.rta_dynamic_area_status, test_value);
   EXPECT_EQ(ta_output.rta_turning_area_status, test_value);
}

/**
 * Set up a scenario with an active alert. Reset TA output and verify that alerts are no longer set.
 * \uts{CSCSA-45209} \sdd{SF-8618} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Reset_Output__test_resetting_ta_output_data)
{
   /** \arrange Set active alert levels in TA output. */
   ta_output.f_diagnostic_mode                = FBK_TRUE;
   ta_output.ta_current_deceleration_estimate = FBK_ONE_F;
   ta_output.rta_alert_left                   = (uint8_t) TA_ALERT_STATE_LEVEL_3;
   ta_output.rta_alert_right                  = (uint8_t) TA_ALERT_STATE_LEVEL_3;
   ta_output.rta_id_left                      = 10u;
   ta_output.rta_id_right                     = 10u;
   ta_output.f_rta_enable                     = FBK_ONE_UINT;
   ta_output.f_rta_enable_turning_area        = FBK_ONE_UINT;
   ta_output.f_rta_enable_dynamic_area        = FBK_ONE_UINT;
   ta_output.rta_long_posn_left               = FBK_ONE_F;
   ta_output.rta_lat_posn_left                = FBK_ONE_F;
   ta_output.rta_long_vel_left                = FBK_ONE_F;
   ta_output.rta_lat_vel_left                 = FBK_ONE_F;
   ta_output.rta_existence_probability_left   = FBK_ONE_F;
   ta_output.rta_ttc_left                     = 3.0f;
   ta_output.rta_long_posn_right              = FBK_ONE_F;
   ta_output.rta_lat_posn_right               = FBK_ONE_F;
   ta_output.rta_long_vel_right               = FBK_ONE_F;
   ta_output.rta_lat_vel_right                = FBK_ONE_F;
   ta_output.rta_existence_probability_right  = FBK_ONE_F;
   ta_output.rta_ttc_right                    = 3.0f;
   ta_output.f_fta_enable                     = FBK_ONE_UINT;
   ta_output.fta_target_gap                   = 5.0f;
   ta_output.fta_ttc                          = 2.0f;
   ta_output.fta_target_age                   = FBK_ONE_F;
   ta_output.fta_target_vel_long              = FBK_ONE_F;
   ta_output.fta_target_vel_lat               = FBK_ONE_F;
   ta_output.fta_target_exist_prob            = FBK_ONE_F;
   ta_output.fta_target_id                    = 10u;

   ta_output.fta_maneuver_direction         = (uint8_t) BMW_MANEUVER_DIRECTION_TURN;
   ta_output.fta_symbol_request             = (uint8_t) BMW_SYMBOL_REQUEST_NO_WARNING;
   ta_output.fta_alert_level                = (uint8_t) BMW_ALERT_LEVEL_PRE_WARNING;
   ta_output.fta_brake_deceleration_request = FBK_ONE_F;
   ta_output.fta_brake_conditioning         = (uint8_t) BMW_BRAKE_CONDITIONING_REQUEST_IN_600_MS;
   ta_output.fta_brake_threshold_reduction  = (uint8_t) BMW_BRAKE_THRESHOLD_REDUCTION_HIGHEST_SENSITIVITY;

   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_rcs                       = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_id                        = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_age                       = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_meas_status               = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_move_status               = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_exist_prob                = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_point                 = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_long_posn         = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_long_posn_std_dev = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_lat_posn          = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_lat_posn_std_dev  = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_covariance_posn           = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yaw_angle                 = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yaw_angle_std_dev         = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_vel                  = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_vel_std_dev          = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_vel                   = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_vel_std_dev           = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_covariance_vel            = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_accel                = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_accel_std_dev        = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_accel                 = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_accel_std_dev         = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_covariance_accel          = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yawrate                   = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yawrate_std_dev           = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_length                    = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_length_std_dev            = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_width                     = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_width_std_dev             = FBK_ONE_F;
   ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_object_class              = FBK_ONE_F;

   /** \action Reset TA output. */
   Ta_Reset_Output(&ta_output);

   /** \assert Check that no alert is raised in TA output. */
   EXPECT_FALSE(ta_output.f_diagnostic_mode);
   EXPECT_FLOAT_EQ(ta_output.ta_current_deceleration_estimate, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_alert_left, (uint8_t) TA_ALERT_STATE_NONE);
   EXPECT_EQ(ta_output.rta_alert_right, (uint8_t) TA_ALERT_STATE_NONE);
   EXPECT_EQ(ta_output.rta_id_left, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.rta_id_right, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.f_rta_enable, FBK_ZERO_UINT);
   EXPECT_EQ(ta_output.f_rta_enable_turning_area, FBK_ZERO_UINT);
   EXPECT_EQ(ta_output.f_rta_enable_dynamic_area, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_left, TA_TAP_DEFAULT_TTC);
   EXPECT_FLOAT_EQ(ta_output.rta_long_posn_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_posn_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_long_vel_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_lat_vel_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_existence_probability_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.rta_ttc_right, TA_TAP_DEFAULT_TTC);
   EXPECT_FLOAT_EQ(ta_output.f_fta_enable, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ta_output.fta_target_gap, TA_BMW_TARGET_GAP_INVALID);
   EXPECT_FLOAT_EQ(ta_output.fta_ttc, TA_INVALID_TTC);
   EXPECT_FLOAT_EQ(ta_output.fta_target_age, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_target_vel_long, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_target_vel_lat, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_target_exist_prob, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_target_id, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.fta_maneuver_direction, BMW_MANEUVER_DIRECTION_STRAIGHT);
   EXPECT_EQ(ta_output.fta_symbol_request, BMW_SYMBOL_REQUEST_NO_WARNING);
   EXPECT_EQ(ta_output.fta_alert_level, BMW_ALERT_LEVEL_NO_WARNING);
   EXPECT_EQ(ta_output.fta_brake_deceleration_request, FBK_ZERO_F);
   EXPECT_EQ(ta_output.fta_brake_conditioning, BMW_BRAKE_CONDITIONING_NO_REQUEST);
   EXPECT_EQ(ta_output.fta_brake_threshold_reduction, BMW_BRAKE_THRESHOLD_REDUCTION_DEFAULT_PARAM_DBC);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_rcs, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_id, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_age, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_meas_status, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_move_status, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_exist_prob, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_point, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_long_posn, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_long_posn_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_lat_posn, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_ref_pnt_lat_posn_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_covariance_posn, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yaw_angle, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yaw_angle_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_vel, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_vel_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_vel, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_vel_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_covariance_vel, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_accel, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_long_accel_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_accel, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_lat_accel_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_covariance_accel, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yawrate, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_yawrate_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_length, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_length_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_width, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_width_std_dev, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_output.fta_relevant_object[FBK_ZERO_UINT].fta_obj_list_object_class, FBK_ZERO_F);
}


/**
 * Check that the initialization routine of post run is resetting persistent data and outputs to their corresponding defaults.
 * \uts{CSCSA-45210} \sdd{SF-8650} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run_Init__check_for_defaults)
{
   /** \arrange Set non default outputs */
   Ta_Bmw_Persistent.fta_prev_alert_level = 5u;

   /** \action Reset TA output. */
   Ta_Post_Run_Init();

   /** \assert Check defaults are present. */
   EXPECT_EQ(Ta_Bmw_Persistent.fta_prev_alert_level, 0u);
}


/**
 * Check that the reset routine of post run for the persistent data is resetting instances to default.
 * \uts{CSCSA-45211} \sdd{SF-8619} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Reset_Post_Run_Persistent__check_for_defaults)
{
   /** \arrange Set non default outputs */
   Ta_Bmw_Persistent.fta_prev_alert_level                       = 42u;
   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = 42.0f;
   Ta_Bmw_Persistent.pfgs_qualification_counter                 = 42u;
   Ta_Bmw_Persistent.pfgs_qualification_counter_min             = 42u;
   Ta_Bmw_Persistent.ta_host_speed_at_brake_start               = 42.0f;
   Ta_Bmw_Persistent.ta_host_speed_reduction_achieved           = 42.0f;
   Ta_Bmw_Persistent.ta_host_speed_reduction_requested          = 42.0f;

   /** \action Reset TA output. */
   Ta_Reset_Post_Run_Persistent(&Ta_Bmw_Persistent);

   /** \assert Check defaults are present. */
   EXPECT_EQ(Ta_Bmw_Persistent.fta_prev_alert_level, 0u);
   EXPECT_EQ(Ta_Bmw_Persistent.pfgs_qualification_counter, 0u);
   EXPECT_EQ(Ta_Bmw_Persistent.pfgs_qualification_counter_min, 0u);
   EXPECT_FLOAT_EQ(Ta_Bmw_Persistent.ta_host_speed_at_brake_start, 0.0f);
   EXPECT_FLOAT_EQ(Ta_Bmw_Persistent.ta_host_speed_reduction_achieved, 0.0f);
   EXPECT_FLOAT_EQ(Ta_Bmw_Persistent.ta_host_speed_reduction_requested, 0.0f);
   EXPECT_FLOAT_EQ(Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request, 0.0f);
}

/**
 * Checks, checks if all interfaces are filled correctly by Ta_Post_Run and its subroutines in Active State.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__if_Active)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized */

   // Input parameters from core output
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_id[FBK_SIDE_LEFT]           = PA_INVALID_OBJ_ID;
   ta_core_output.ta_id[FBK_SIDE_RIGHT]          = PA_INVALID_OBJ_ID;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]          = 5.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT]         = 6.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]          = 7.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT]         = 8.0f;

   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cals.k_f_ta_enable_debug_mode   = 0u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   // Input parameters for subfunction Ta_Get_Maneuver_Direction
   ta_input.fta_steering_angle_max_left  = 30;
   ta_input.fta_steering_angle_max_right = 30;
   p_vehicle_data->steering_angle        = 0.0f;
   p_vehicle_data->curvature             = ta_cals.k_ta_lookup_turning_host_curvature_min[FBK_ZERO_UINT];

   // Input parameters for subfunction Ta_Map_Fta_Output_Signals
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_UNDEFINED;
   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, 0u);

   // Input parameters for subfunction Ta_Map_Rta_Output_Signals
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_input.f_rta_enable                                = FBK_TRUE;
   ta_input.f_rta_enable_turning_area                   = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area                   = FBK_TRUE;

   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;
   ta_output.fta_alert_level                                    = 2;

   // Input parameters for subfunction Ta_Set_Current_Ta_Functional_State
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.5f;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   *p_ta_current_state                                    = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** Action when whether Transiformation is happening from Inactive state to Active*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, p_ta_current_state, p_vehicle_data);

   // Input parameters for subfunction Ta_Set_Output_Bus_Signals_Of_Right_Object_In_Active_State
   ta_output.rta_id_right                    = 10u;
   ta_output.rta_alert_right                 = TA_ALERT_STATE_LEVEL_2;
   ta_output.rta_long_posn_right             = 1.0f;
   ta_output.rta_lat_posn_right              = 1.0f;
   ta_output.rta_right_object_width          = 1.0f;
   ta_output.rta_right_object_length         = 1.0f;
   ta_output.rta_long_vel_right              = 1.0f;
   ta_output.rta_lat_vel_right               = 2.0f;
   ta_output.rta_ttc_right                   = 1.5f;
   ta_output.rta_existence_probability_right = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_RIGHT]      = 2u;

   // Input parameters for subfunction Ta_Set_Output_Bus_Signals_Of_Left_Object_In_Active_State
   ta_output.rta_id_left                    = 10u;
   ta_output.rta_alert_left                 = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_long_posn_left             = 1.0f;
   ta_output.rta_lat_posn_left              = 1.0f;
   ta_output.rta_left_object_width          = 1.0f;
   ta_output.rta_left_object_length         = 1.0f;
   ta_output.rta_long_vel_left              = 1.0f;
   ta_output.rta_lat_vel_left               = 2.0f;
   ta_output.rta_ttc_left                   = 1.5f;
   ta_output.rta_existence_probability_left = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_LEFT]      = 2u;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Post_Run(&ta_instance, &ta_input, &ta_output);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.f_diagnostic_mode, FBK_FALSE);
   EXPECT_EQ(ta_output.fta_maneuver_direction, BMW_MANEUVER_DIRECTION_STRAIGHT);
   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_right_output, NOT_CRITICAL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_right_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_id, ta_output.rta_id_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_x, ta_output.rta_long_posn_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_y, ta_output.rta_lat_posn_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_width, ta_output.rta_right_object_width);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_length, ta_output.rta_right_object_length);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_x, ta_output.rta_long_vel_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_y, ta_output.rta_lat_vel_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttc, ta_output.rta_ttc_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttp, ta_output.rta_ttc_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_existence_probability, ta_output.rta_existence_probability_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_probability_of_collision, FBK_ZERO_F);

   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_left_output, INFO_LEVEL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_left_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_id, ta_output.rta_id_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_x, ta_output.rta_long_posn_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_y, ta_output.rta_lat_posn_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_width, ta_output.rta_left_object_width);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_length, ta_output.rta_left_object_length);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_x, ta_output.rta_long_vel_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_y, ta_output.rta_lat_vel_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttc, ta_output.rta_ttc_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttp, ta_output.rta_ttc_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_existence_probability, ta_output.rta_existence_probability_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_probability_of_collision, FBK_ZERO_F);
}


/**
 * Checks, checks if all interfaces are filled correctly by Ta_Post_Run and its subroutines
 * \uts{} \sdd{n/a} \testtype{positive}
 */

TEST_F(Ta_Post_Run_Test, Ta_Post_Run__if_Not_Active)
{
   /** \arrange Assign more or less random values to all required input parameters in order to avoid access on uninitialized */

   /** \ Input parameters from core output */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_id[FBK_SIDE_LEFT]           = PA_INVALID_OBJ_ID;
   ta_core_output.ta_id[FBK_SIDE_RIGHT]          = PA_INVALID_OBJ_ID;
   ta_core_output.ta_ttc[FBK_SIDE_LEFT]          = 5.0f;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT]         = 6.0f;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]          = 7.0f;
   ta_core_output.ta_ttp[FBK_SIDE_RIGHT]         = 8.0f;

   /** \ Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled */
   ta_cals.k_f_ta_enable_debug_mode   = 0u;
   ta_input.f_fta_enable              = 1u;
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = 0.0f;
   ta_input.fta_obj_offset_y_positive = 0.0f;
   ta_input.fta_obj_offset_y_negative = 0.0f;

   /** \Input parameters for subfunction Ta_Get_Maneuver_Direction*/
   ta_input.fta_steering_angle_max_left  = 30;
   ta_input.fta_steering_angle_max_right = 30;
   p_vehicle_data->steering_angle        = 0.0f;
   p_vehicle_data->curvature             = ta_cals.k_ta_lookup_turning_host_curvature_min[FBK_ZERO_UINT];

   /** \ Input parameters for subfunction Ta_Map_Fta_Output_Signals */
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_most_critical_side                   = FBK_SIDE_UNDEFINED;
   Ta_Ut_Helper_Set_Tracker_Object_Data(object_data, 0u);

   /** \ Input parameters for subfunction Ta_Map_Rta_Output_Signals */
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
   ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   ta_input.f_rta_enable                                = FBK_TRUE;
   ta_input.f_rta_enable_turning_area                   = FBK_TRUE;
   ta_input.f_rta_enable_dynamic_area                   = FBK_TRUE;

   Ta_Bmw_Persistent.pfgs_prev_cycle_brake_deceleration_request = FBK_ZERO_F;
   ta_output.fta_alert_level                                    = 2;

   *p_ta_current_state = TA_STATE_INACTIVE;

   /** \ Input parameters for subfunction Ta_Set_Output_Bus_Signals_Of_Right_Object_In_Active_State */
   ta_output.rta_id_right                    = 10u;
   ta_output.rta_alert_right                 = TA_ALERT_STATE_LEVEL_2;
   ta_output.rta_long_posn_right             = 1.0f;
   ta_output.rta_lat_posn_right              = 1.0f;
   ta_output.rta_right_object_width          = 1.0f;
   ta_output.rta_right_object_length         = 1.0f;
   ta_output.rta_long_vel_right              = 1.0f;
   ta_output.rta_lat_vel_right               = 2.0f;
   ta_output.rta_ttc_right                   = 1.5f;
   ta_output.rta_existence_probability_right = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_RIGHT]      = 2u;

   /** \  Input parameters for subfunction Ta_Set_Output_Bus_Signals_Of_Left_Object_In_Active_State */
   ta_output.rta_id_left                    = 10u;
   ta_output.rta_alert_left                 = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_long_posn_left             = 1.0f;
   ta_output.rta_lat_posn_left              = 1.0f;
   ta_output.rta_left_object_width          = 1.0f;
   ta_output.rta_left_object_length         = 1.0f;
   ta_output.rta_long_vel_left              = 1.0f;
   ta_output.rta_lat_vel_left               = 2.0f;
   ta_output.rta_ttc_left                   = 1.5f;
   ta_output.rta_existence_probability_left = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_LEFT]      = 2u;

   /** \action Call function Ta_Map_Rta_Output_Signals with parameters ta_output, ta_input, ta_core_output and ta_cals. */
   Ta_Post_Run(&ta_instance, &ta_input, &ta_output);

   /** \assert Check, if all output parameters are set correctly. */
   EXPECT_EQ(ta_output.ta_output_bus_signal.f_ta_status, TA_BMW_BIT_POSITION_11);
   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_right_output, NOT_CRITICAL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_right_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_id, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_x, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_y, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_width, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_length, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_x, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_y, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttc, TA_TAP_DEFAULT_TTC);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttp, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_existence_probability, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_probability_of_collision, FBK_ZERO_F);

   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_left_output, NOT_CRITICAL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_left_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_id, TA_BMW_TARGET_ID_INVALID);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_x, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_y, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_width, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_length, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_x, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_y, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttc, TA_TAP_DEFAULT_TTC);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttp, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_existence_probability, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_probability_of_collision, FBK_ZERO_F);
}

/**
 * Checks, checks if Ta_Status Encoding is set correctly based on feature function state
 * \uts{} \sdd{n/a} \testtype{positive}
 */

TEST_F(Ta_Post_Run_Test, Ta_Post_Run__Test_Ta_Status_Encoding_Based_On_Feature_Function_Error_State)
{
   /** \arrange FF State*/
   ta_output.ta_output_bus_signal.bmw_qualifier_ta_function_state = TA_STATE_ERROR;
   /** \action Call function Ta_Set_Status_Based_On_Encoding */
   Ta_Set_Status_Based_On_Encoding(&ta_output);
   /** \assert Verify Ta_Status Encoding*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.f_ta_status, TA_BMW_BIT_POSITION_10);
}

/**
 * Checks, checks if Ta_Status Encoding is set correctly based on alert level based on current implementation
 * \uts{} \sdd{n/a} \testtype{positive}
 */

TEST_F(Ta_Post_Run_Test, Ta_Post_Run__Test_Ta_Status_Encoding_Based_On_Left_Alert_State_None)
{
   /** \arrange alert level*/
   ta_output.rta_alert_left = TA_ALERT_STATE_NONE;
   /** \action Call function Ta_Set_Status_Based_On_Encoding */
   Ta_Set_Status_Based_On_Encoding(&ta_output);
   /** \assert Verify Ta_Status Encoding*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.f_ta_status, TA_BMW_BIT_POSITION_3);
}

/**
 * Checks, checks if Ta_Status Encoding is set correctly based on alert level based on current implementation
 * \uts{} \sdd{n/a} \testtype{positive}
 */

TEST_F(Ta_Post_Run_Test, Ta_Post_Run__Test_Ta_Status_Encoding_Based_On_Left_Alert_State_Level_1)
{
   /** \arrange alert level*/
   ta_output.rta_alert_left = TA_ALERT_STATE_LEVEL_1;
   /** \action Call function Ta_Set_Status_Based_On_Encoding */
   Ta_Set_Status_Based_On_Encoding(&ta_output);
   /** \assert Verify Ta_Status Encoding*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.f_ta_status, TA_BMW_BIT_POSITION_1);
}

/**
 * Checks, checks if Ta_Status Encoding is set correctly based on alert level based on current implementation
 * \uts{} \sdd{n/a} \testtype{positive}
 */

TEST_F(Ta_Post_Run_Test, Ta_Post_Run__Test_Ta_Status_Encoding_Based_On_Left_Alert_State_Level_3)
{
   /** \arrange alert level*/
   ta_output.rta_alert_left = TA_ALERT_STATE_LEVEL_3;
   /** \action Call function Ta_Set_Status_Based_On_Encoding */
   Ta_Set_Status_Based_On_Encoding(&ta_output);
   /** \assert Verify Ta_Status Encoding*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.f_ta_status, TA_BMW_BIT_POSITION_2);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Output_Bus_Signals_In_Active_State__Set_output_signal_left_id)
{
   /** \arrange left object class*/
   ta_output.rta_id_right                   = TA_BMW_TARGET_ID_INVALID;
   ta_output.rta_alert_right                = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_id_left                    = 3u;
   ta_output.rta_alert_left                 = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_long_posn_left             = 1.0f;
   ta_output.rta_lat_posn_left              = 1.0f;
   ta_output.rta_left_object_width          = 1.0f;
   ta_output.rta_left_object_length         = 1.0f;
   ta_output.rta_long_vel_left              = 1.0f;
   ta_output.rta_lat_vel_left               = 2.0f;
   ta_output.rta_ttc_left                   = 1.5f;
   ta_output.rta_existence_probability_left = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_LEFT]      = 2u;

   /** \action Call function Ta_Set_Output_Bus_Signals_In_Active_State */
   Ta_Set_Output_Bus_Signals_In_Active_State(&ta_output);

   /** \assert Verify Mapped object type*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_left_output, INFO_LEVEL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_left_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_id, ta_output.rta_id_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_x, ta_output.rta_long_posn_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_y, ta_output.rta_lat_posn_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_width, ta_output.rta_left_object_width);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_length, ta_output.rta_left_object_length);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_x, ta_output.rta_long_vel_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_y, ta_output.rta_lat_vel_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttc, ta_output.rta_ttc_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttp, ta_output.rta_ttc_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_existence_probability, ta_output.rta_existence_probability_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_probability_of_collision, FBK_ZERO_F);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Output_Bus_Signals_In_Active_State__Set_output_signal_left_alert)
{
   /** \arrange left object class*/
   ta_output.rta_id_right                   = 2u;
   ta_output.rta_alert_right                = TA_ALERT_STATE_NONE;
   ta_output.rta_id_left                    = 3u;
   ta_output.rta_alert_left                 = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_long_posn_left             = 1.0f;
   ta_output.rta_lat_posn_left              = 1.0f;
   ta_output.rta_left_object_width          = 1.0f;
   ta_output.rta_left_object_length         = 1.0f;
   ta_output.rta_long_vel_left              = 1.0f;
   ta_output.rta_lat_vel_left               = 2.0f;
   ta_output.rta_ttc_left                   = 1.5f;
   ta_output.rta_existence_probability_left = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_LEFT]      = 2u;

   /** \action Call function Ta_Set_Output_Bus_Signals_In_Active_State */
   Ta_Set_Output_Bus_Signals_In_Active_State(&ta_output);

   /** \assert Verify Mapped object type*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_left_output, INFO_LEVEL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_left_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_id, ta_output.rta_id_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_x, ta_output.rta_long_posn_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_position_y, ta_output.rta_lat_posn_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_width, ta_output.rta_left_object_width);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_length, ta_output.rta_left_object_length);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_x, ta_output.rta_long_vel_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_velocity_y, ta_output.rta_lat_vel_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttc, ta_output.rta_ttc_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_ttp, ta_output.rta_ttc_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_existence_probability, ta_output.rta_existence_probability_left);
   EXPECT_EQ(ta_output.ta_output_bus_signal.left_object_probability_of_collision, FBK_ZERO_F);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Output_Bus_Signals_In_Active_State__Set_output_signal_right_id)
{
   /** \arrange right object class*/
   ta_output.rta_id_left                     = TA_BMW_TARGET_ID_INVALID;
   ta_output.rta_alert_left                  = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_id_right                    = 3u;
   ta_output.rta_alert_right                 = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_long_posn_right             = 1.0f;
   ta_output.rta_lat_posn_right              = 1.0f;
   ta_output.rta_right_object_width          = 1.0f;
   ta_output.rta_right_object_length         = 1.0f;
   ta_output.rta_long_vel_right              = 1.0f;
   ta_output.rta_lat_vel_right               = 2.0f;
   ta_output.rta_ttc_right                   = 1.5f;
   ta_output.rta_existence_probability_right = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_LEFT]       = 2u;

   /** \action Call function Ta_Set_Output_Bus_Signals_In_Active_State */
   Ta_Set_Output_Bus_Signals_In_Active_State(&ta_output);

   /** \assert Verify Mapped object type*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_right_output, INFO_LEVEL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_right_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_id, ta_output.rta_id_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_x, ta_output.rta_long_posn_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_y, ta_output.rta_lat_posn_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_width, ta_output.rta_right_object_width);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_length, ta_output.rta_right_object_length);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_x, ta_output.rta_long_vel_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_y, ta_output.rta_lat_vel_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttc, ta_output.rta_ttc_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttp, ta_output.rta_ttc_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_existence_probability, ta_output.rta_existence_probability_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_probability_of_collision, FBK_ZERO_F);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_F(Ta_Post_Run_Test, Ta_Set_Output_Bus_Signals_In_Active_State__Set_output_signal_right_alert)
{
   /** \arrange right object class*/
   ta_output.rta_id_left                     = 2u;
   ta_output.rta_alert_left                  = TA_ALERT_STATE_NONE;
   ta_output.rta_id_right                    = 3u;
   ta_output.rta_alert_right                 = TA_ALERT_STATE_LEVEL_1;
   ta_output.rta_long_posn_right             = 1.0f;
   ta_output.rta_lat_posn_right              = 1.0f;
   ta_output.rta_right_object_width          = 1.0f;
   ta_output.rta_right_object_length         = 1.0f;
   ta_output.rta_long_vel_right              = 1.0f;
   ta_output.rta_lat_vel_right               = 2.0f;
   ta_output.rta_ttc_right                   = 1.5f;
   ta_output.rta_existence_probability_right = 1.0f;
   ta_core_output.ta_id[FBK_SIDE_LEFT]       = 2u;

   /** \action Call function Ta_Set_Output_Bus_Signals_In_Active_State */
   Ta_Set_Output_Bus_Signals_In_Active_State(&ta_output);

   /** \assert Verify Mapped object type*/
   EXPECT_EQ(ta_output.ta_output_bus_signal.ta_object_criticality_right_output, INFO_LEVEL);
   EXPECT_EQ(ta_output.ta_output_bus_signal.timestamp_right_object, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_id, ta_output.rta_id_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_x, ta_output.rta_long_posn_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_position_y, ta_output.rta_lat_posn_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_width, ta_output.rta_right_object_width);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_length, ta_output.rta_right_object_length);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_x, ta_output.rta_long_vel_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_velocity_y, ta_output.rta_lat_vel_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttc, ta_output.rta_ttc_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttb, FBK_ZERO_F);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_ttp, ta_output.rta_ttc_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_existence_probability, ta_output.rta_existence_probability_right);
   EXPECT_EQ(ta_output.ta_output_bus_signal.right_object_probability_of_collision, FBK_ZERO_F);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_F(Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Object_Type_Based_On_Object_Class,
       Ta_Post_Run__Test_Map_Object_Type_Of_Left_Object_Based_On_Undefined_Object_Class)
{
   /** \arrange left object class*/
   ta_output.rta_left_object_type   = 5u;
   TA_Object_Type_T expected_output = OBJECT_TYPE_UNKNOWN;
   /** \action Call function Ta_Map_Left_Object_Type_As_Per_Object_Class */
   TA_Object_Type_T output = Ta_Map_Left_Object_Type_As_Per_Object_Class(&ta_output);
   /** \assert Verify Mapped object type*/
   EXPECT_EQ(output, expected_output);
}

/**
 * check whether the Mapping of Critically Based on Critical Level is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_F(Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Criticality_Based_On_Alert_Level,
       Ta_Post_Run__Test_Map_Criticality_Of_Left_Object_Based_On_Undefined_Alert_Level)
{
   /** \arrange alert level*/
   ta_output.rta_alert_left                      = TA_ALERT_STATE_LEVEL_2;
   TA_Object_Criticality_Level_T expected_output = NOT_CRITICAL;
   /** \action Call function Ta_Map_Criticality_Of_Left_Object_As_Per_Alert_State */
   TA_Object_Criticality_Level_T output = Ta_Map_Criticality_Of_Left_Object_As_Per_Alert_State(&ta_output);
   /** \assert Verify Mapped Criticality*/
   EXPECT_EQ(output, expected_output);
}

/**
 * check whether the Mapping of Critically Based on Critical Level is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_P(Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Criticality_Based_On_Alert_Level,
       Ta_Post_Run__Test_Map_Criticality_Of_Right_Object_Based_On_Alert_Level)
{
   /** \arrange alert level*/
   ta_output.rta_alert_right                     = (uint8_t) std::get<0>(GetParam());
   TA_Object_Criticality_Level_T expected_output = std::get<1>(GetParam());
   /** \action Call function Ta_Map_Criticality_Of_Right_Object_As_Per_Alert_State */
   TA_Object_Criticality_Level_T output = Ta_Map_Criticality_Of_Right_Object_As_Per_Alert_State(&ta_output);
   /** \assert Verify Mapped Criticality*/
   EXPECT_EQ(output, expected_output);
}

/**
 * check whether the Mapping of Critically Based on Critical Level is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_P(Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Criticality_Based_On_Alert_Level,
       Ta_Post_Run__Test_Map_Criticality_Of_Left_Object_Based_On_Alert_Level)
{
   /** \arrange alert level*/
   ta_output.rta_alert_left                      = (uint8_t) std::get<0>(GetParam());
   TA_Object_Criticality_Level_T expected_output = std::get<1>(GetParam());
   /** \action Call function Ta_Map_Criticality_Of_Left_Object_As_Per_Alert_State */
   TA_Object_Criticality_Level_T output = Ta_Map_Criticality_Of_Left_Object_As_Per_Alert_State(&ta_output);
   /** \assert Verify Mapped Criticality*/
   EXPECT_EQ(output, expected_output);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_P(Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Object_Type_Based_On_Object_Class,
       Ta_Post_Run__Test_Map_Object_Type_Of_Right_Object_Based_On_Object_Class)
{
   /** \arrange right object class*/
   ta_output.rta_right_object_type  = (uint8_t) std::get<0>(GetParam());
   TA_Object_Type_T expected_output = std::get<1>(GetParam());
   /** \action Call function Ta_Map_Right_Object_Type_As_Per_Object_Class */
   TA_Object_Type_T output = Ta_Map_Right_Object_Type_As_Per_Object_Class(&ta_output);
   /** \assert Verify Mapped object type*/
   EXPECT_EQ(output, expected_output);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_F(Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Object_Type_Based_On_Object_Class,
       Ta_Post_Run__Test_Map_Object_Type_Of_Right_Object_Based_On_Undefined_Object_Class)
{
   /** \arrange right object class*/
   ta_output.rta_right_object_type  = 5u;
   TA_Object_Type_T expected_output = OBJECT_TYPE_UNKNOWN;
   /** \action Call function Ta_Map_Right_Object_Type_As_Per_Object_Class */
   TA_Object_Type_T output = Ta_Map_Right_Object_Type_As_Per_Object_Class(&ta_output);
   /** \assert Verify Mapped object type*/
   EXPECT_EQ(output, expected_output);
}

/**
 * check whether the Mapping of Object Type Based on Object Class is Set Right
 * \uts{} \sdd{n/a} \testtype{positive}
 *
 */
TEST_P(Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Object_Type_Based_On_Object_Class,
       Ta_Post_Run__Test_Map_Object_Type_Of_Left_Object_Based_On_Object_Class)
{
   /** \arrange left object class*/
   ta_output.rta_left_object_type   = (uint8_t) std::get<0>(GetParam());
   TA_Object_Type_T expected_output = std::get<1>(GetParam());
   /** \action Call function Ta_Map_Left_Object_Type_As_Per_Object_Class */
   TA_Object_Type_T output = Ta_Map_Left_Object_Type_As_Per_Object_Class(&ta_output);
   /** \assert Verify Mapped object type*/
   EXPECT_EQ(output, expected_output);
}

INSTANTIATE_TEST_SUITE_P(Test_Mapping_of_Criticality_Based_On_Alert_Level,
                         Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Criticality_Based_On_Alert_Level,
                         testing::Values(std::make_tuple(TA_ALERT_STATE_NONE, NOT_CRITICAL),
                                         std::make_tuple(TA_ALERT_STATE_LEVEL_1, INFO_LEVEL),
                                         std::make_tuple(TA_ALERT_STATE_LEVEL_3, ACUTE_LEVEL)));

INSTANTIATE_TEST_SUITE_P(Test_For_Mapping_Of_Object_Type_Based_On_Object_Class,
                         Ta_Post_Run_Parameterized_Test_For_Mapping_Of_Object_Type_Based_On_Object_Class,
                         testing::Values(std::make_tuple(PA_OBJ_CLASS_UNKNOWN, OBJECT_TYPE_UNKNOWN),
                                         std::make_tuple(PA_OBJ_CLASS_PEDESTRIAN, OBJECT_TYPE_PEDESTRIAN),
                                         std::make_tuple(PA_OBJ_CLASS_2WHEEL, OBJECT_TYPE_2_WHEEL),
                                         std::make_tuple(PA_OBJ_CLASS_CAR, OBJECT_TYPE_CAR),
                                         std::make_tuple(PA_OBJ_CLASS_TRUCK, OBJECT_TYPE_TRUCK)));
