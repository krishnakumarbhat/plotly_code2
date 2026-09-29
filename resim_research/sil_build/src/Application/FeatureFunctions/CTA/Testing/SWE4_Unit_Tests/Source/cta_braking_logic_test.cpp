/**
 * @file cta_brake_logic_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_brake_logic_test.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41810}
 */

#include "cta_braking_logic_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <math.h>

extern "C"
{
#include "cta_braking_logic.c"
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
}


/**
 * Check whether speed after dead time is equal to speed signal when no acceleration is occuring
 * \uts{CSCSA-41811} \sdd{SF-3726} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Host_Speed_After_Dead_Time__equals_to_speed_signal_since_no_acceleration_is_occuring)
{
   /** \arrange longitudinal acceleration and speed signal */
   float32_T host_speed_after_dead_time;
   p_vehicle_data->long_acc   = 0.0f;
   p_vehicle_data->host_speed = 1.0f;

   /** \action executes function to test */
   host_speed_after_dead_time = Cta_Get_Host_Speed_After_Dead_Time(p_vehicle_data, &cals);

   /** \assert Expect speed after dead time to be current speed */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, p_vehicle_data->host_speed);
}


/**
 * Check whether speed after dead time is increasing when an acceleration is occuring in the current timestamp
 * \uts{CSCSA-41812} \sdd{SF-3726} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Host_Speed_After_Dead_Time__increases_when_positive_acc_is_occuring)
{
   /** \arrange longitudinal acceleration and speed signal */
   float32_T host_speed_after_dead_time;
   p_vehicle_data->long_acc   = 0.5f;
   p_vehicle_data->host_speed = 1.0f;

   cals.k_ctb_responsetime_brake_actuation = 0.3f;
   cals.k_ctb_lower_safety_distance_thres  = 0.3f;

   float32_T acc_caused_speed_after_dt =
      p_vehicle_data->long_acc * cals.k_ctb_host_acc_weight * cals.k_ctb_responsetime_brake_actuation;

   /** \action executes function to test */
   host_speed_after_dead_time = Cta_Get_Host_Speed_After_Dead_Time(p_vehicle_data, &cals);

   /** \assert Expect speed after dead time to also increase with acceleration signal */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, p_vehicle_data->host_speed + acc_caused_speed_after_dt);
   EXPECT_GT(host_speed_after_dead_time, p_vehicle_data->host_speed);
}


/**
 * Check whether speed after dead time is decreasing when a deceleration is occuring in the current timestamp
 * \uts{CSCSA-41813} \sdd{SF-3726} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Host_Speed_After_Dead_Time__decreases_when_deceleration_is_occuring)
{
   /** \arrange longitudinal acceleration and speed signal */
   float32_T host_speed_after_dead_time;
   p_vehicle_data->long_acc   = -0.5f;
   p_vehicle_data->host_speed = 1.0f;

   cals.k_ctb_responsetime_brake_actuation = 0.3f;
   cals.k_ctb_lower_safety_distance_thres  = 0.3f;

   float32_T acc_caused_speed_after_dt =
      p_vehicle_data->long_acc * cals.k_ctb_host_acc_weight * cals.k_ctb_responsetime_brake_actuation;

   /** \action executes function to test */
   host_speed_after_dead_time = Cta_Get_Host_Speed_After_Dead_Time(p_vehicle_data, &cals);

   /** \assert Expect speed after dead time to also decrease with deceleration signal */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, p_vehicle_data->host_speed + acc_caused_speed_after_dt);
   EXPECT_LT(host_speed_after_dead_time, p_vehicle_data->host_speed);
}

/**
 * Check whether braking time is filled correctly when within ramp in time host can come to standstill
 * \uts{CSCSA-41814} \sdd{SF-3725} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Braking_Time__host_can_come_to_standstill_in_ramp_in_time)
{
   /** \arrange host velocity and maximum velocity of host which can be braked at ramp in */
   float32_T host_velocity_start_braking = 2.0f;
   float32_T v_max_ramp_in               = 3.5f;
   float32_T braking_time;

   float32_T expected_res = Fast_Sqrt(2.0f * host_velocity_start_braking / cals.k_ctb_braking_jerk);
   /** \action executes function to test */
   braking_time = Cta_Get_Braking_Time(&cals, host_velocity_start_braking, v_max_ramp_in, cals.k_ctb_ramp_in_time);

   /** \assert braking time to be a constant dependent on velocity at the start of braking and the jerk */
   EXPECT_FLOAT_EQ(braking_time, expected_res);
}

/**
 * Check whether braking time is filled correctly when within ramp in time host cant come to standstill
 * \uts{CSCSA-41815} \sdd{SF-3725} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Braking_Time__host_can_not_come_to_standstill_in_ramp_in_time)
{
   /** \arrange host velocity and maximum velocity of host which cant be braked at ramp in */
   float32_T host_velocity_start_braking = -2.0f;
   float32_T v_max_ramp_in               = -3.5f;
   float32_T velocity_diff               = host_velocity_start_braking - v_max_ramp_in;
   float32_T braking_time;

   float32_T expected_res = cals.k_ctb_ramp_in_time + velocity_diff / Fbk_Abs_F(cals.k_ctb_const_decel_after_ramp_in);
   /** \action executes function to test */
   braking_time = Cta_Get_Braking_Time(&cals, host_velocity_start_braking, v_max_ramp_in, cals.k_ctb_ramp_in_time);

   /** \assert braking time to be dependent on the maximum deceleration signal after ramp in time */
   EXPECT_FLOAT_EQ(braking_time, expected_res);
}


/**
 * Check the model in use when braking can fully occure in ramp in phase so that the constant deceleration phase is also not needed
 * \uts{CSCSA-41816} \sdd{SF-3724} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Braking_Distance__standstill_in_ramp_in_phase)
{
   /** \arrange host velocity and maximum velocity of host which cant be braked at ramp in */
   float32_T host_velocity_start_braking = 2.0f;
   float32_T v_max_ramp_in               = 2.78f;
   float32_T braking_time                = 0.7f;
   float32_T braking_distance, expected_distance;

   expected_distance = host_velocity_start_braking * cals.k_ctb_responsetime_brake_actuation + host_velocity_start_braking * braking_time
                       - 1.0f / 6.0f * Fbk_Abs_F(cals.k_ctb_braking_jerk) * braking_time * braking_time * braking_time;

   /** \action executes function to test */
   braking_distance =
      Cta_Get_Braking_Distance(&cals, host_velocity_start_braking, v_max_ramp_in, braking_time, cals.k_ctb_ramp_in_time);

   /** \assert braking time to be dependent on the maximum deceleration signal after ramp in time */
   EXPECT_FLOAT_EQ(braking_distance, expected_distance);
}


/**
 * Check the model in use when braking can not fully occure in ramp in phase so that the constant deceleration phase is also needed
 * \uts{CSCSA-41817} \sdd{SF-3724} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Braking_Distance__cant_come_to_standstill_in_ramp_in_phase)
{
   /** \arrange host velocity and maximum velocity of host which cant be braked at ramp in */
   float32_T host_velocity_start_braking = 10.0f;
   float32_T v_max_ramp_in               = 2.78f;
   float32_T braking_time                = 0.7f;
   float32_T braking_distance, expected_distance;


   v_max_ramp_in = -0.5f * cals.k_ctb_braking_jerk * cals.k_ctb_ramp_in_time * cals.k_ctb_ramp_in_time;

   float32_T braking_time_diff = braking_time - cals.k_ctb_ramp_in_time;

   float32_T dist_till_start          = host_velocity_start_braking * cals.k_ctb_responsetime_brake_actuation;
   float32_T dist_with_jerk_influence = host_velocity_start_braking * cals.k_ctb_ramp_in_time
                                        - 1.0f / 6.0f * Fbk_Abs_F(cals.k_ctb_braking_jerk) * powf(cals.k_ctb_ramp_in_time, 3.0f);

   float32_T dist_with_const_deceleration = 0.5f * Fbk_Abs_F(cals.k_ctb_const_decel_after_ramp_in) * powf(braking_time_diff, 2.0f);

   expected_distance = dist_till_start + dist_with_jerk_influence + dist_with_const_deceleration;

   /** \action executes function to test */
   braking_distance =
      Cta_Get_Braking_Distance(&cals, host_velocity_start_braking, v_max_ramp_in, braking_time, cals.k_ctb_ramp_in_time);

   /** \assert braking time to be dependent on the maximum deceleration signal after ramp in time */
   EXPECT_FLOAT_EQ(braking_distance, expected_distance);
}


/**
 * Check that a braking qualifier is held when it is turning off from one to the next cycle.
 * \uts{CSCSA-41818} \sdd{SF-3727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Hold_Brake_Decision__qualifier_shall_be_hold)
{
   /** \arrange Previously a brake qualifier was active */
   boolean_T result;
   cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT] = FBK_TRUE;
   cta_persistent.brake_holding_counter[mode][FBK_SIDE_LEFT]    = cals.k_ctb_min_brake_hold_ctr_thres - 2u;
   /** \action Execute function to test */
   result = Cta_Hold_Brake_Decision(&cta_persistent.brake_holding_counter[mode][FBK_SIDE_LEFT],
                                    &cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT], &cals, FBK_FALSE);
   /** \assert Expect true */
   EXPECT_TRUE(result);
}

/**
 * Check that a braking qualifier is turned to false by holding logic when it is turning off and was already held by the given
 * amount of cycles. \uts{CSCSA-41819} \sdd{SF-3727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Hold_Brake_Decision__qualifier_shall_not_be_hold)
{
   /** \arrange Previously a brake qualifier was active */
   boolean_T result;
   cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT] = FBK_TRUE;
   cta_persistent.brake_holding_counter[mode][FBK_SIDE_LEFT]    = cals.k_ctb_min_brake_hold_ctr_thres;

   /** \action Execute function to test */
   result = Cta_Hold_Brake_Decision(&cta_persistent.brake_holding_counter[mode][FBK_SIDE_LEFT],
                                    &cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT], &cals, FBK_FALSE);
   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Check that a braking qualifier is not returning falsely information when qualifier is set to false and previously not braking
 * qualifier has been set. \uts{CSCSA-41820} \sdd{SF-3727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Hold_Brake_Decision__no_braking_input_and_no_previous_braking_occured)
{
   /** \arrange Previously no brake qualifier was active */
   boolean_T result;
   cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT] = FBK_FALSE;

   /** \action Execute function to test */
   result = Cta_Hold_Brake_Decision(&cta_persistent.brake_holding_counter[mode][FBK_SIDE_LEFT],
                                    &cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT], &cals, FBK_FALSE);
   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Check that the counter remains 0 when a brake indicator is true as input.
 * \uts{CSCSA-41821} \sdd{SF-3727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Hold_Brake_Decision__brake_indicator_is_true_and_counter_shall_not_be_changed)
{
   /** \arrange Previously no brake qualifier was active */
   boolean_T result;
   cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT] = FBK_TRUE;

   /** \action Execute function to test */
   result = Cta_Hold_Brake_Decision(&cta_persistent.brake_holding_counter[mode][FBK_SIDE_LEFT],
                                    &cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_LEFT], &cals, FBK_TRUE);
   /** \assert Expect true */
   EXPECT_TRUE(result);
   EXPECT_EQ(cta_persistent.brake_holding_counter[mode][FBK_SIDE_LEFT], 0u);
}


/**
 * Check that a braking qualifier is supressed in case that it has not reached the qualyfying cycles yet.
 * \uts{CSCSA-41822} \sdd{SF-3728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Qualify_Brake_Decision__brake_flag_is_not_qualified)
{
   /** \arrange Setup result declaration */
   boolean_T result;

   /** \action Execute function to test */
   result = Cta_Qualify_Brake_Decision(&cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT], &cals, FBK_TRUE);

   /** \assert Expect false and increment of qualifier counter */
   EXPECT_FALSE(result);
   EXPECT_EQ(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT], 1u);
}


/**
 * Check that an braking qualifier is supressed in case that it has not reached the qualyfying cycles yet.
 * \uts{CSCSA-41823} \sdd{SF-3728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Qualify_Brake_Decision__brake_flag_gets_qualified_in_this_cycle)
{
   /** \arrange Setup result declaration */
   boolean_T result;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /** \action Execute function to test */
   result = Cta_Qualify_Brake_Decision(&cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT], &cals, FBK_TRUE);

   /** \assert Expect true and increment of qualifier counter */
   EXPECT_TRUE(result);
   EXPECT_EQ(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT], cals.k_ctb_min_brake_qual_ctr_thres + 1u);
}


/**
 * Check that a braking qualifier is returned as false since the input qualifier is also set to false.
 * \uts{CSCSA-41824} \sdd{SF-3728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Qualify_Brake_Decision__brake_flag_is_set_to_false_and_counter_shall_be_reset)
{
   /** \arrange Setup qualification counter of previously active braking flag */
   boolean_T result;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT] = cals.k_ctb_min_brake_qual_ctr_thres - 1u;

   /** \action Execute function to test */
   result = Cta_Qualify_Brake_Decision(&cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT], &cals, FBK_FALSE);

   /** \assert Expect false and increment of qualifier counter */
   EXPECT_FALSE(result);
   EXPECT_EQ(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_LEFT], 0u);
}


/**
 * Check that the main function is not returning any brake, when the host is in standstill.
 * \uts{CSCSA-41825} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__Host_is_not_moving_and_false_is_expected)
{
   /** \arrange Host is in standstill */
   boolean_T result;
   p_vehicle_data->host_speed = 0.0f;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect false since standstill */
   EXPECT_FALSE(result);
}


/**
 * Check that the main function is not returning any brake, when time to brake is less than the min threshold.
 * \uts{CSCSA-41826} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__time_to_brake_lt_min_threshold)
{
   /** \arrange Time to brake less than threshold */
   boolean_T result;
   p_vehicle_data->host_speed = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      cals.k_ctb_min_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect false less than minimum threshold */
   EXPECT_FALSE(result);
}

/**
 * Check that the main function returns true, when time to brake is in range due to ttp mode.
 * \uts{CSCSA-319551} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__time_to_brake_in_range_due_to_ttp_mode)
{
   /** \arrange Time to brake less than threshold */
   boolean_T result;
   cals.k_ctb_responsetime_brake_actuation                       = 0.15f;
   cals.k_ctb_event_time_buffer                                  = 0.15f;
   cals.k_ctb_ramp_in_time                                       = 0.3f;
   cals.k_ctb_time_to_ask_for_final_brake_decel                  = 0.0f;
   cals.k_ctb_host_acc_weight                                    = 1.0f;
   cals.k_ctb_braking_jerk                                       = 5.0f;
   cals.k_ctb_const_decel_after_ramp_in                          = 6.0f;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;
   cta_instance.core_input.cta_stop_mode                         = CTA_STOP_MODE_TTP;
   p_vehicle_data->host_speed                                    = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      -cals.k_ctb_min_braking_time - cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp =
      cals.k_ctb_min_braking_time + cals.k_ctb_responsetime_brake_actuation + EPSILON;


   p_vehicle_data->host_length                                                                                          = 6.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode]  = -6.8f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode] = -6.8f;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect true */
   EXPECT_TRUE(result);
}


/**
 * Check that the main function is not returning any brake, when time to brake is greater than the max threshold.
 * \uts{CSCSA-41827} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__time_to_brake_gt_max_threshold)
{
   /** \arrange Time to brake greater than threshold */
   boolean_T result;
   p_vehicle_data->host_speed = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation + EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect false since greater than maximum */
   EXPECT_FALSE(result);
}


/**
 * Check that the main returns true when the ttc and safety distance are fulfilled. The flag shall also be already qualified.
 * \uts{CSCSA-41828} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__brake_qualifier_shall_be_set_since_qualified_and_cond_fulfilled)
{
   /** \arrange Host drives backwards */
   boolean_T result;

   cals.k_ctb_responsetime_brake_actuation                       = 0.15f;
   cals.k_ctb_event_time_buffer                                  = 0.15f;
   cals.k_ctb_ramp_in_time                                       = 0.3f;
   cals.k_ctb_time_to_ask_for_final_brake_decel                  = 0.0f;
   cals.k_ctb_host_acc_weight                                    = 1.0f;
   cals.k_ctb_braking_jerk                                       = 5.0f;
   cals.k_ctb_const_decel_after_ramp_in                          = 6.0f;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;
   cals.k_cta_f_use_brake_gradient                               = FBK_FALSE;

   p_vehicle_data->host_speed = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      0.5f * cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;

   p_vehicle_data->host_length                                                                                          = 6.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode]  = -6.8f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode] = -6.8f;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect true since ttc and safety distance condition are fulfilled. */
   EXPECT_TRUE(result);
}


/**
 * Check that the main returns true when the ttc and safety distance are fulfilled. The flag shall also be already qualified.
 * \uts{CSCSA-41829} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__brake_qualifier_test_calculate_braking_gradient)
{
   /** \arrange Host drives backwards */
   boolean_T result;

   cals.k_ctb_responsetime_brake_actuation                       = 0.15f;
   cals.k_ctb_event_time_buffer                                  = 0.15f;
   cals.k_ctb_ramp_in_time                                       = 0.3f;
   cals.k_ctb_time_to_ask_for_final_brake_decel                  = 0.0f;
   cals.k_ctb_host_acc_weight                                    = 1.0f;
   cals.k_ctb_braking_jerk                                       = 5.0f;
   cals.k_ctb_const_decel_after_ramp_in                          = 6.0f;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;

   p_vehicle_data->host_speed = -1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      0.5f * cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;

   p_vehicle_data->host_length                                                                                          = 6.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode]  = -7.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode] = -7.0f;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect true since ttc and safety distance condition are fulfilled. */
   EXPECT_TRUE(result);
}

/**
 * Check that the main returns false when ttp is not fulfilled.
 * \uts{CSCSA-185715} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__ttp_not_fulfilled)
{
   /** \arrange Host drives backwards */
   boolean_T result;
   cta_instance.core_input.cta_stop_mode = CTA_STOP_MODE_TTP;
   p_vehicle_data->host_speed            = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      0.5f * cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time - EPSILON;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect false since ttp is not fulfilled. */
   EXPECT_FALSE(result);
}


/**
 * Check that the main returns true when the ttc and safety distance are fulfilled. The flag shall also be already qualified. Here
 * the ramp in time is set by the time when the maximum value of braking logic is requested. \uts{CSCSA-41830} \sdd{SF-3732}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test,
       Cta_Shall_Brake_Qualifier_Be_Set__brake_qualifier_test_calculate_braking_gradient_with_maximum_time_specification)
{
   /** \arrange Host drives backwards */
   boolean_T result;

   cals.k_ctb_responsetime_brake_actuation                       = 0.15f;
   cals.k_ctb_event_time_buffer                                  = 0.15f;
   cals.k_ctb_ramp_in_time                                       = 0.3f;
   cals.k_ctb_host_acc_weight                                    = 1.0f;
   cals.k_ctb_braking_jerk                                       = 5.0f;
   cals.k_ctb_const_decel_after_ramp_in                          = 6.0f;
   cals.k_ctb_time_to_ask_for_final_brake_decel                  = cals.k_ctb_ramp_in_time + 0.05f;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;

   p_vehicle_data->host_speed = -1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      0.5f * cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;

   p_vehicle_data->host_length                                                                                          = 6.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode]  = -7.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode] = -9.0f;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect true since ttc and safety distance condition are fulfilled. */
   EXPECT_TRUE(result);
}


/**
 * Check that the main returns false when the ttc condition is not fulfilled.
 * \uts{CSCSA-41831} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__brake_qualifier_ttc_condition_not_fulfilled)
{
   /** \arrange Host drives backwards */
   boolean_T result;
   cals.k_ctb_max_braking_time = 0.9f;
   p_vehicle_data->host_speed  = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;

   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;

   p_vehicle_data->host_length                                                                                          = 6.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode]  = -7.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode] = -9.0f;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect false since ttc is not fulfilled. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT], 0u);
}


/**
 * Check that the main function returs false since distance to driving tube is less than the minimum required safety distance.
 * \uts{CSCSA-41832} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__dist_driving_tube_lt_min_safety_distance)
{
   /** \arrange Host drives backwards */
   boolean_T result;
   p_vehicle_data->host_speed = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      0.5f * cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;

   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;
   cals.k_ctb_responsetime_brake_actuation                       = 0.3f;
   cals.k_ctb_lower_safety_distance_thres                        = 0.3f;

   p_vehicle_data->host_length                                                                                          = 6.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode]  = -6.6f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode] = -6.6f;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /*Modify minimum safety distance*/
   cals.k_ctb_lower_safety_distance_thres = 0.3f;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect false since distance to driving tube is less than minimum safety distance. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT], 0u);
}


/**
 * Check that the main function returs false since distance to driving tube is greater than the maximum permissible safety
 * distance. \uts{CSCSA-41833} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__dist_driving_tube_gt_max_safety_distance)
{
   /** \arrange Host drives backwards */
   boolean_T result;
   p_vehicle_data->host_speed = 1.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttc =
      0.5f * cals.k_ctb_max_braking_time + cals.k_ctb_responsetime_brake_actuation - EPSILON;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->ttp = cals.k_ctb_min_braking_time + EPSILON;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point           = 1u;
   p_vehicle_data->host_length                                             = 6.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode]  = -8.0f;
   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode] = -10.0f;
   cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT] = cals.k_ctb_min_brake_qual_ctr_thres;

   /*Modify maximum safety distance*/
   cals.k_ctb_upper_safety_distance_thres_lut[0] = 1.0f;
   cals.k_ctb_upper_safety_distance_thres_lut[1] = 1.25f;
   cals.k_ctb_upper_safety_distance_thres_lut[2] = 1.375f;
   cals.k_ctb_upper_safety_distance_thres_lut[3] = 1.5f;

   /** \action Execute function to test */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect false since distance to driving tube is less than minimum safety distance. */
   EXPECT_FALSE(result);
   EXPECT_EQ(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT], 0u);
}


/**
 * Pass in a Null pointer of object attributes to ctb. This occures when an object is not critical anymore. NULL however is still a
 * valid input here, check that the holding cycles are incremented to the holding threshold such that no brake is given afterwards.
 * \uts{CSCSA-41834} \sdd{SF-3732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Shall_Brake_Qualifier_Be_Set__check_that_persistent_data_is_reset_correctly)
{
   /** \arrange Set up persistent brake holding logic close to shut down */
   boolean_T result;

   cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes = NULL;
   cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]         = cals.k_ctb_min_brake_hold_ctr_thres;
   cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]      = FBK_TRUE;

   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;
   /** \action Execute ctb */
   result = Cta_Shall_Brake_Qualifier_Be_Set(&(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.brake_suppression_counter[mode][FBK_SIDE_RIGHT]),
                                             &(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]),
                                             cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes, &cta_instance,
                                             FBK_SIDE_RIGHT, mode, p_vehicle_data);

   /** \assert Expect no brake qualifier and persistent data to be reset. */
   EXPECT_FALSE(result);
   EXPECT_FALSE(cta_persistent.previous_brake_qualifier[mode][FBK_SIDE_RIGHT]);
   EXPECT_EQ(cta_persistent.brake_holding_counter[mode][FBK_SIDE_RIGHT], 0u);
}

/**
 * Check that the upper safety distance boundary is returned correctly. Here the host is moving with a speed of
 * k_ctb_safety_dist_host_vel_lut[0] thus k_ctb_upper_safety_distance_thres_lut[0] is expected. \uts{CSCSA-41835} \sdd{SF-3926}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Maximum_Safety_Dist_Thres__saturate_on_the_lower_border)
{
   /** \arrange Host drives with positive speed */
   float32_T result;
   p_vehicle_data->host_speed = cals.k_ctb_safety_dist_host_vel_lut[0];

   /** \action Execute function to test */
   result = Cta_Get_Maximum_Safety_Dist_Thres(p_vehicle_data, &cals);

   /** \assert Expect the upper safety distance to be the lowest possible value. */
   EXPECT_FLOAT_EQ(result, cals.k_ctb_upper_safety_distance_thres_lut[0]);
}


/**
 * Check that the upper safety distance boundary is returned correctly. Here the host is moving with a speed of -(1.0f)
 * k_ctb_safety_dist_host_vel_lut[0] thus k_ctb_upper_safety_distance_thres_lut[0] is expected. \uts{CSCSA-41836} \sdd{SF-3926}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Maximum_Safety_Dist_Thres__saturate_on_the_lower_border_with_negative_speed)
{
   /** \arrange Host drives backwards */
   float32_T result;
   p_vehicle_data->host_speed = -cals.k_ctb_safety_dist_host_vel_lut[0];

   /** \action Execute function to test */
   result = Cta_Get_Maximum_Safety_Dist_Thres(p_vehicle_data, &cals);

   /** \assert Expect the upper safety distance to be the lowest possible value. */
   EXPECT_FLOAT_EQ(result, cals.k_ctb_upper_safety_distance_thres_lut[0]);
}


/**
 * Check that the upper safety distance boundary is returned correctly. Here the host is moving with a speed of
 * k_ctb_safety_dist_host_vel_lut[K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0-1] thus
 * k_ctb_upper_safety_distance_thres_lut[K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0-1] is expected. \uts{CSCSA-41837}
 * \sdd{SF-3926} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Get_Maximum_Safety_Dist_Thres__saturate_on_upper_border)
{
   /** \arrange Host drives with positive speed */
   float32_T result;
   p_vehicle_data->host_speed = cals.k_ctb_safety_dist_host_vel_lut[CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0 - 1u];

   /** \action Execute function to test */
   result = Cta_Get_Maximum_Safety_Dist_Thres(p_vehicle_data, &cals);

   /** \assert Expect the upper safety distance to be the highest possible value. */
   EXPECT_FLOAT_EQ(result, cals.k_ctb_upper_safety_distance_thres_lut[CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0 - 1u]);
}

/**
 * Check that the distance to the driving tube is correctly calculated.
 * \uts{CSCSA-41838} \sdd{SF-3729} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Return_Distance_To_Driving_Tube__general_test)
{
   /** \arrange Set data to pass the conditions */
   float32_T result;
   float32_T expected                                            = 5.0f;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   mode                                                          = CTA_MODE_FRONT;
   attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode]    = expected * 2.0f;
   attributes.long_isect_point_candidate[FBK_SIDE_RIGHT][mode]   = expected * 3.0f;
   attributes.CTA_heading                                        = 0.0f;

   /** \action Execute function to test */
   result = Cta_Return_Distance_To_Driving_Tube(&attributes, expected, &cals, p_vehicle_data, mode);

   /** \assert Check the result */
   EXPECT_FLOAT_EQ(result, expected);
}

/**
 * Check that the distance to the driving tube is correctly calculated when absolute distance for both corner are same.
 * \uts{CSCSA-293117} \sdd{SF-3729} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Return_Distance_To_Driving_Tube__both_corner_same_abs_distance)
{
   /** \arrange Set data to pass the conditions */
   float32_T result;
   float32_T expected                                            = 2.5f;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   mode                                                          = CTA_MODE_FRONT;
   attributes.long_isect_point_candidate[FBK_SIDE_LEFT][mode]    = 5.0f;
   attributes.long_isect_point_candidate[FBK_SIDE_RIGHT][mode]   = 5.0f;
   attributes.CTA_heading                                        = 0.0f;

   /** \action Execute function to test */
   result = Cta_Return_Distance_To_Driving_Tube(&attributes, expected, &cals, p_vehicle_data, mode);

   /** \assert Check the result */
   EXPECT_FLOAT_EQ(result, expected);
}

/**
 * Check that the brake_deceleration is correctly calculated and f_standstill_qualifier is not triggered.
 * \uts{CSCSA-290885} \sdd{CSCSA-290874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Calculate_Object_Braking_Attributes__f_standstill_qualifier_is_false)
{
   /** \arrange host velocity and maximum velocity of host which can be braked at ramp in */
   float32_T host_velocity_start_braking = 2.0f;
   float32_T v_max_ramp_in               = 8.0f;
   float32_T ramp_in_time                = 3.0f;
   float32_T t_brake                     = 2.0f;
   float32_T host_speed                  = 5;

   attributes.f_standstill_qualifier = FBK_FALSE;

   /** \action executes function to test */
   Cta_Calculate_Object_Braking_Attributes(&attributes, &cals, host_velocity_start_braking, v_max_ramp_in, ramp_in_time, t_brake,
                                           host_speed);

   /** \assert brake_deceleration is correctly calculated and f_standstill_qualifier is not triggered */
   EXPECT_FLOAT_EQ(attributes.brake_deceleration, 10.0);
   EXPECT_FALSE(attributes.f_standstill_qualifier);
}

/**
 * Check that the brake_deceleration is correctly calculated and f_standstill_qualifier is triggered.
 * \uts{CSCSA-290886} \sdd{CSCSA-290874} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Braking_Logic_Test, Cta_Calculate_Object_Braking_Attributes__f_standstill_qualifier_is_true)
{
   /** \arrange host velocity and maximum velocity of host which can be braked at ramp in */
   float32_T host_velocity_start_braking = -10.0f;
   float32_T v_max_ramp_in               = -12.0f;
   float32_T ramp_in_time                = 3.0f;
   float32_T t_brake                     = 2.0f;
   attributes.f_standstill_qualifier     = FBK_FALSE;
   float32_T host_speed                  = 5;


   /** \action executes function to test */
   Cta_Calculate_Object_Braking_Attributes(&attributes, &cals, host_velocity_start_braking, v_max_ramp_in, ramp_in_time, t_brake,
                                           host_speed);

   /** \assert brake_deceleration is correctly calculated and f_standstill_qualifier is triggered */
   EXPECT_FLOAT_EQ(attributes.brake_deceleration, 5.0);
   EXPECT_TRUE(attributes.f_standstill_qualifier);
}
