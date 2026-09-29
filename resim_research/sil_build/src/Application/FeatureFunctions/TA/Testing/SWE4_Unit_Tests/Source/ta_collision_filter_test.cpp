/**
 * @file ta_collision_filter_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44768}
 */

#include "ta_collision_filter_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

using ::testing::Eq;
using ::testing::FloatNear;

extern "C"
{
#include "fbk_macros.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "ta_collision_filter.c"
#include "ta_factory.h"
#include <math.h>
}

/**
 * Set up input values to have a negative acceleration and thus a smaller host vehicle speed after the provided dead time. The
 * expected return value is smaller than current host speed. \uts{CSCSA-44793} \sdd{SF-8792} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Host_Speed_After_Dead_Time__check_correct_host_speed_retrieval)
{
   /** \arrange Set up valid ego trajectory values. */
   ta_cal.k_fta_brake_dead_time = 0.2f;
   uint8_t n_dead_time_cylces   = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Retrieve the host speed from the ego trajectory. */
   float32_T host_speed_after_dead_time = Ta_Get_Host_Speed_After_Dead_Time(&ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Check that the correct value was retrieved from the ego trajectory information. */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed);
}

/**
 * Set up invalid time step value.
 * \uts{CSCSA-44794} \sdd{SF-8792} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Host_Speed_After_Dead_Time__return_default_value_for_invalid_time_step_value)
{
   /** \arrange Set up an invalid time step value. */
   p_ta_persistent->ta_pred_step_dt = FBK_ZERO_F;

   /** \action Try to retrieve the host speed. */
   float32_T host_speed_after_dead_time = Ta_Get_Host_Speed_After_Dead_Time(&ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Check that the default value is returned. */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, FBK_ZERO_F);
}

/**
 * Set up invalid number of prediction steps.
 * \uts{CSCSA-44795} \sdd{SF-8792} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Host_Speed_After_Dead_Time__return_default_value_for_pred_step_out_of_bounds)
{
   /** \arrange Set up an invalid number of prediction steps. */
   ego_trajectory.n_prediction_steps = FBK_ZERO_UINT;

   /** \action Try to retrieve the host speed. */
   float32_T host_speed_after_dead_time = Ta_Get_Host_Speed_After_Dead_Time(&ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Check that the default value is returned. */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, FBK_ZERO_F);
}

/**
 * Set up input values to have quadratic formula with real roots. The expected deceleration is the first root of that formula.
 * \uts{CSCSA-44789} \sdd{SF-8793} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Gradient_Restricted_Deceleration__gradient_corrected_decel)
{
   /** \arrange Set up negative gradient with large absolute value. */
   float32_T t_brake           = 1.2f;
   float32_T host_speed        = 10.0f;
   boolean_T f_enable_gradient = FBK_TRUE;
   float32_T braking_gradient  = -85.0f;

   /** \action Compute the deceleration value. */
   float32_T deceleration = Ta_Get_Gradient_Restricted_Deceleration(t_brake, host_speed, f_enable_gradient, braking_gradient);

   /** \assert Check that the calculated deceleration is greater than the fallback based on just speed and t_brake. */
   EXPECT_GT(deceleration, Abs(host_speed / t_brake));
}

/**
 * Set up input values to have no brake gradient. The expected deceleration is the simple division of host_speed and t_brake.
 * \uts{CSCSA-44787} \sdd{SF-8793} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Gradient_Restricted_Deceleration__for_jerk_value_zero)
{
   /** \arrange Set up a gradient value of zero. */
   float32_T t_brake           = 1.8f;
   float32_T host_speed        = 10.0f;
   boolean_T f_enable_gradient = FBK_FALSE;
   float32_T braking_gradient  = 0.0f;

   /** \action Compute the deceleration value. */
   float32_T deceleration = Ta_Get_Gradient_Restricted_Deceleration(t_brake, host_speed, f_enable_gradient, braking_gradient);

   /** \assert Check that the calculated deceleration is based on just speed and t_brake. */
   EXPECT_FLOAT_EQ(deceleration, Abs(host_speed / t_brake));
}

/**
 * Set up input values to have quadratic formula without real roots. The expected deceleration is a fallback to the simple division
 * of host_speed and t_brake. \uts{CSCSA-44788} \sdd{SF-8793} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Gradient_Restricted_Deceleration__fallback_to_no_gradient_calc)
{
   /** \arrange Set up negative gradient with value close to zero. */
   float32_T t_brake           = 1.0f;
   float32_T host_speed        = 10.0f;
   boolean_T f_enable_gradient = FBK_TRUE;
   float32_T braking_gradient  = -1.0f;

   /** \action Compute the deceleration value. */
   float32_T deceleration = Ta_Get_Gradient_Restricted_Deceleration(t_brake, host_speed, f_enable_gradient, braking_gradient);

   /** \assert Check that the calculated deceleration is the fallback based on just speed and t_brake. */
   EXPECT_FLOAT_EQ(deceleration, Abs(host_speed / t_brake));
}


/**
 * Set up critical scenario at which the ego vehicle and target are at the same position. The expected TTC is zero as well as the
 * collision coordinate. \uts{CSCSA-44769} \sdd{SF-8666} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttc__test_critical_obj)
{
   /** \arrange Set up critical scenario at which the ego vehicle and target are at the same position. */
   uint8_t prediction_step = 0;

   Ta_Reset_Trajectory(&ego_trajectory, &ta_cal);
   Ta_Reset_Trajectory(&ta_object.attributes.trajectory, &ta_cal);

   ta_object.attributes.ttc                     = TA_INVALID_TTC;
   ta_object.attributes.waypoint_at_collision.x = FBK_ZERO_F;
   ta_object.attributes.waypoint_at_collision.y = FBK_ZERO_F;

   ego_trajectory.f_trajectory_valid = FBK_TRUE;
   ego_trajectory.n_prediction_steps = 1;

   ta_object.attributes.trajectory.f_trajectory_valid = FBK_TRUE;
   ta_object.attributes.trajectory.n_prediction_steps = 1;

   ego_trajectory.waypoint[prediction_step].f_waypoint_valid                  = FBK_TRUE;
   ta_object.attributes.trajectory.waypoint[prediction_step].f_waypoint_valid = FBK_TRUE;

   ego_trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle                  = FBK_ZERO_F;
   ta_object.attributes.trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   /** \action Compute the TTC and collision coordinate. */
   Ta_Get_Object_Ttc(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed TTC and coordinates with expected results. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttc, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.y, FBK_ZERO_F);
}

/**
 * Set up critical scenario at which the ego vehicle and target collide but the trajectories are set as invalid. The expected TTC
 * is TA_INVALID_TTC. \uts{CSCSA-44786} \sdd{SF-8666} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttc__test_invalid_trajectories)
{
   /** \arrange Set up critical scenario at which the ego vehicle and target collide but the trajectories are set as invalid. */
   uint8_t prediction_step = 0;

   ta_object.attributes.ttc                     = TA_INVALID_TTC;
   ta_object.attributes.waypoint_at_collision.x = FBK_ZERO_F;
   ta_object.attributes.waypoint_at_collision.y = FBK_ZERO_F;

   Ta_Reset_Trajectory(&ego_trajectory, &ta_cal);
   Ta_Reset_Trajectory(&ta_object.attributes.trajectory, &ta_cal);

   ego_trajectory.waypoint[prediction_step].f_waypoint_valid                  = FBK_FALSE;
   ta_object.attributes.trajectory.waypoint[prediction_step].f_waypoint_valid = FBK_FALSE;

   ego_trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle                  = FBK_ZERO_F;
   ta_object.attributes.trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   /** \action Compute the TTC and collision coordinate. */
   Ta_Get_Object_Ttc(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed TTC and coordinates with expected results. */
   EXPECT_EQ(ta_object.attributes.ttc, TA_INVALID_TTC);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.x, 0.0f);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.y, 0.0f);
}

/**
 * Set up critical scenario at which the ego vehicle and target collide but the target trajectory is set as invalid. The expected
 * TTC is TA_INVALID_TTC. \uts{CSCSA-93442} \sdd{SF-8666} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttc__test_invalid_targert_trajectory)
{
   /** \arrange Set up critical scenario at which the ego vehicle and target collide but the trajectories are set as invalid. */
   uint8_t prediction_step = 0;

   ta_object.attributes.ttc                     = TA_INVALID_TTC;
   ta_object.attributes.waypoint_at_collision.x = FBK_ZERO_F;
   ta_object.attributes.waypoint_at_collision.y = FBK_ZERO_F;

   Ta_Reset_Trajectory(&ego_trajectory, &ta_cal);
   Ta_Reset_Trajectory(&ta_object.attributes.trajectory, &ta_cal);

   ego_trajectory.f_trajectory_valid                  = FBK_TRUE;
   ta_object.attributes.trajectory.f_trajectory_valid = FBK_FALSE;

   ego_trajectory.waypoint[prediction_step].f_waypoint_valid                  = FBK_TRUE;
   ta_object.attributes.trajectory.waypoint[prediction_step].f_waypoint_valid = FBK_FALSE;

   ego_trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle                  = FBK_ZERO_F;
   ta_object.attributes.trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   /** \action Compute the TTC and collision coordinate. */
   Ta_Get_Object_Ttc(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed TTC and coordinates with expected results. */
   EXPECT_EQ(ta_object.attributes.ttc, TA_INVALID_TTC);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.x, 0.0f);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.y, 0.0f);
}


/**
 * Set up critical scenario at which the ego vehicle and target collide but the waypoints are set as invalid. The expected TTC is
 * TA_INVALID_TTC. \uts{CSCSA-44770} \sdd{SF-8666} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttc__skip_steps_for_invalid_waypoints)
{
   /** \arrange Set up critical scenario at which the ego vehicle and target collide but the waypoints are set as invalid. */
   uint8_t prediction_step = 0;

   ta_object.attributes.ttc                     = TA_INVALID_TTC;
   ta_object.attributes.waypoint_at_collision.x = FBK_ZERO_F;
   ta_object.attributes.waypoint_at_collision.y = FBK_ZERO_F;

   Ta_Reset_Trajectory(&ego_trajectory, &ta_cal);
   Ta_Reset_Trajectory(&ta_object.attributes.trajectory, &ta_cal);

   ego_trajectory.f_trajectory_valid = FBK_TRUE;
   ego_trajectory.n_prediction_steps = 1;

   ta_object.attributes.trajectory.f_trajectory_valid = FBK_TRUE;
   ta_object.attributes.trajectory.n_prediction_steps = 1;

   ego_trajectory.waypoint[prediction_step].f_waypoint_valid                  = FBK_FALSE;
   ta_object.attributes.trajectory.waypoint[prediction_step].f_waypoint_valid = FBK_FALSE;

   ego_trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle                  = FBK_ZERO_F;
   ta_object.attributes.trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   /** \action Compute the TTC and collision coordinate. */
   Ta_Get_Object_Ttc(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed TTC and coordinates with expected results. */
   EXPECT_EQ(ta_object.attributes.ttc, TA_INVALID_TTC);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.x, 0.0f);
   EXPECT_FLOAT_EQ(ta_object.attributes.waypoint_at_collision.y, 0.0f);
}

/**
 * Compute the distance from the VCS origin to the target object. The expected distance is the euclidean distance for the target
 * objects VCS coordinates. \uts{CSCSA-44771} \sdd{SF-8664} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Distance_to_Vcs_Origin__check_distance)
{
   /** \arrange Set target at position (10, 10). */
   ta_object.tracker_data.vcs_pos.x = 10.0f;
   ta_object.tracker_data.vcs_pos.y = 10.0f;

   /** \action Compute the distance from VCS to the target. */
   Ta_Get_Object_Distance_to_Vcs_Origin(&ta_object);

   /** \assert Compare computed distance with expected results. */
   float32_T result = sqrtf(ta_object.tracker_data.vcs_pos.x * ta_object.tracker_data.vcs_pos.x
                            + ta_object.tracker_data.vcs_pos.y * ta_object.tracker_data.vcs_pos.y);
   EXPECT_FLOAT_EQ(ta_object.attributes.distance_to_ego, result);
}

/**
 * Compute the deceleration of the ego vehicle required to avoid a collision with the target. Compare the computed result with the
 * manually derived value. \uts{CSCSA-44772} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__calculate_deceleration)
{
   /** \arrange Set ego vehicle parameters and TTC to target. */
   ta_object.attributes.ttc                   = 1.2f;
   ta_cal.k_fta_brake_dead_time               = 0.2f;
   ta_cal.k_fta_brake_deceleration_max        = 10.0f;
   ta_cal.k_f_fta_enable_brake_gradient_logic = FBK_FALSE;
   ta_cal.k_fta_brake_gradient                = 0.0f;
   uint8_t n_dead_time_cylces                 = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 1.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result =
      ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / (ta_object.attributes.ttc - ta_cal.k_fta_brake_dead_time);
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Within that test the condition for the ttc is not fulfilled. This leads to a deceleration signal to be unchanged from its
 * default value. \uts{CSCSA-44773} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__exact_boundary_test_ttc)
{
   /** \arrange Set TTC to invalid value. */
   ta_object.attributes.ttc = TA_INVALID_TTC;

   float32_T default_val_decel              = 0.0f;
   ta_object.attributes.decel_to_avoid_coll = default_val_decel;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, default_val_decel);
}


/*
 * Within that test the condition for the ttc is fulfilled. This leads to a deceleration signal to differ from its default value.
 * \uts{CSCSA-44774} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__lt_boundary_test_ttc)
{
   /** \arrange Set TTC to valid value. */
   ta_object.attributes.ttc = TA_INVALID_TTC - 0.1f;

   ta_cal.k_fta_brake_dead_time               = 0.2f;
   ta_cal.k_fta_brake_deceleration_max        = 10.0f;
   ta_cal.k_f_fta_enable_brake_gradient_logic = FBK_FALSE;
   ta_cal.k_fta_brake_gradient                = 0.0f;
   uint8_t n_dead_time_cylces                 = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result =
      ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / (ta_object.attributes.ttc - ta_cal.k_fta_brake_dead_time);
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Case fulfilled in which the deceleration is calculated by formula. The difference of ttc and braketime shall exceed EPSILON.
 * \uts{CSCSA-44775} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__gt_boundary_test_diff_ttc_and_brake_dead_time)
{
   /** \arrange Set up ego braking parameters. */
   ta_cal.k_fta_brake_dead_time        = 0.8f;
   ta_object.attributes.ttc            = 0.8f + EPSILON;
   ta_cal.k_fta_brake_deceleration_max = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results (saturated deceleration signal). */
   float32_T result = ta_cal.k_fta_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Case fulfilled in which the deceleration is set to the maximum deceleration value. The difference of ttc and braketime shall
 * exceed EPSILON. \uts{CSCSA-44776} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__exact_boundary_test_diff_ttc_and_brake_dead_time)
{
   /** \arrange Set up ego braking parameters. */
   ta_object.attributes.ttc   = 0.8f;
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->long_acc   = 0.0f;

   ta_cal.k_fta_brake_dead_time        = ta_object.attributes.ttc - EPSILON;
   ta_cal.k_fta_brake_deceleration_max = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ta_cal.k_fta_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, result);
}


/*
 * For the saturation the exact boundary for the saturation of deceleration to avoid collision signal shall be tested here. Within
 * this test the deceleration signal shall not be saturated, since it is located on the exact boundary. \uts{CSCSA-44777}
 * \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__exact_boundary_test_saturation_on_max_val)
{
   /** \arrange Set up ego braking parameters. */
   ta_object.attributes.ttc = 1.1f;

   ta_cal.k_fta_brake_dead_time        = 0.1f;
   ta_cal.k_fta_brake_deceleration_max = 10.0f;
   uint8_t n_dead_time_cylces          = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ta_cal.k_fta_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, result);
}


/*
 * A value greater than the saturation check threshold is tested. The Deceleration to avoid collision signal shall be saturated.
 * \uts{CSCSA-44778} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__gt_boundary_test_saturation_on_max_val)
{
   /** \arrange Set up ego braking parameters. */
   ta_object.attributes.ttc = 0.9f;

   ta_cal.k_fta_brake_dead_time        = 0.1f;
   ta_cal.k_fta_brake_deceleration_max = 10.0f;
   uint8_t n_dead_time_cylces          = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ta_cal.k_fta_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, result);
}


/*
 * Modifies the ttc such that a division with zero is possible. A Division with zero shall not occur here.
 * \uts{CSCSA-44779} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__error_guessing_division_with_zero)
{
   /** \arrange Set up ego braking parameters. */
   ta_cal.k_fta_brake_dead_time = 1.0f;
   ta_object.attributes.ttc     = ta_cal.k_fta_brake_dead_time + 0.1f * EPSILON;

   ta_cal.k_fta_brake_deceleration_max = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ta_cal.k_fta_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ta_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Set up a scenario in which the braking signal should saturate and verify that saturation is reached.
 * \uts{CSCSA-44780} \sdd{SF-8663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Ego_Deceleration_To_Avoid_Collision__cap_to_max_deceleration)
{
   /** \arrange Set up ego braking parameters. */
   ta_object.attributes.ttc = 0.2f;

   ta_cal.k_fta_brake_dead_time        = 0.2f;
   ta_cal.k_fta_brake_deceleration_max = 10.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 20.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ta_Get_Ego_Deceleration_To_Avoid_Collision(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed deceleration with expected results. */
   EXPECT_EQ(ta_object.attributes.decel_to_avoid_coll, ta_cal.k_fta_brake_deceleration_max);
}

/*
 * Compute the remaining time to brake for a given TTC and host vehicle condition. Compare the computed TTB with a manually
 * calculated TTB. \uts{CSCSA-44790} \sdd{SF-8665} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttb__calculate_ttb_no_gradient_brake_threshold_not_reached)
{
   /** \arrange Set up scenario with a given TTC and host vehicle condition. */
   ta_object.attributes.ttc = 0.8f;

   ta_cal.k_fta_brake_dead_time            = 0.2f;
   ta_cal.k_fta_brake_gradient             = 0.0f;
   ta_cal.k_ta_alert_lvl_4_ttc_threshold   = 0.8f;
   ta_cal.k_ta_alert_lvl_4_decel_threshold = 6.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 2.0f;

   /** \action Compute the remaining time to brake. */
   Ta_Get_Object_Ttb(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed TTB with expected results. */
   float32_T ttb_no_gradient = ta_object.attributes.ttc
                               - (ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / ta_cal.k_ta_alert_lvl_4_decel_threshold)
                               - ta_cal.k_fta_brake_dead_time;
   EXPECT_FLOAT_EQ(ta_object.attributes.ttb, ttb_no_gradient);
}

/*
 * Compute the remaining time to brake for a given TTC and host vehicle condition. Compare the computed TTB with a manually
 * calculated TTB. \uts{CSCSA-44791} \sdd{SF-8665} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttb__calculate_ttb_with_gradient_brake_threshold_not_reached)
{
   /** \arrange Set up scenario with a given TTC and host vehicle condition. */
   ta_object.attributes.ttc = 0.8f;

   ta_cal.k_fta_brake_dead_time            = 0.2f;
   ta_cal.k_fta_brake_gradient             = -85.0f;
   ta_cal.k_ta_alert_lvl_4_ttc_threshold   = 0.8f;
   ta_cal.k_ta_alert_lvl_4_decel_threshold = 6.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 2.0f;

   /** \action Compute the remaining time to brake. */
   Ta_Get_Object_Ttb(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed TTB with expected results. */
   float32_T ttb_no_gradient = ta_object.attributes.ttc
                               - (ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / ta_cal.k_ta_alert_lvl_4_decel_threshold)
                               - ta_cal.k_fta_brake_dead_time;
   EXPECT_LT(ta_object.attributes.ttb, ttb_no_gradient);
}

/*
 * Compute the remaining time to brake for a given TTC and host vehicle condition. Compare the computed TTB with a manually
 * calculated TTB. \uts{CSCSA-44792} \sdd{SF-8665} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttb__calculate_ttb_for_brake_threshold_reached_but_ttc_not_reached)
{
   /** \arrange Set up scenario with a given TTC and host vehicle condition. */
   ta_object.attributes.ttc = 0.9f;

   ta_cal.k_fta_brake_dead_time            = 0.2f;
   ta_cal.k_ta_alert_lvl_4_ttc_threshold   = 0.8f;
   ta_cal.k_ta_alert_lvl_4_decel_threshold = 6.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ta_cal.k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 5.0f;

   /** \action Compute the remaining time to brake. */
   Ta_Get_Object_Ttb(&ta_object, &ego_trajectory, p_ta_persistent, &ta_cal);

   /** \assert Compare computed TTB with expected results. */
   float32_T result = ta_object.attributes.ttc - ta_cal.k_ta_alert_lvl_4_ttc_threshold;
   EXPECT_FLOAT_EQ(ta_object.attributes.ttb, result);
}

/*
 * Compute the time to pass for a critical target using VCS coordinates. Compare the computed TTP with a manually calculated TTP.
 * \uts{CSCSA-44781} \sdd{SF-8667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttp__calculate_ttp)
{
   /** \arrange Set up scenario with a critical target. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   ta_object.tracker_data.vcs_pos.x       = -10.0f;
   ta_object.tracker_data.vcs_pos.y       = 5.0f;
   ta_object.tracker_data.vcs_vel_rel.x   = 10.0f;
   ta_object.tracker_data.vcs_vel_rel.y   = 0.0f;
   ta_object.tracker_data.vcs_heading     = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max - EPSILON;

   /** \action Compute the time to pass. */
   Ta_Get_Object_Ttp(&ta_object, host_curvature, &ta_cal);

   /** \assert Compare computed TTP with expected results. */
   float32_T result = 1.0f;
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, result);
}

/*
 * Compute the time to pass for a critical target using curvi coordinates. Compare the computed TTP with a manually calculated TTP.
 * \uts{CSCSA-44782} \sdd{SF-8667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttp__calculate_ttp_curvi)
{
   /** \arrange Set up scenario with a critical target. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   ta_object.tracker_data.curvi_pos.x     = -10.0f;
   ta_object.tracker_data.curvi_pos.y     = 5.0f;
   ta_object.tracker_data.curvi_vel_rel.x = 10.0f;
   ta_object.tracker_data.curvi_vel_rel.y = 0.0f;
   ta_object.tracker_data.curvi_heading   = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_TRUE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max - EPSILON;

   /** \action Compute the time to pass. */
   Ta_Get_Object_Ttp(&ta_object, host_curvature, &ta_cal);

   /** \assert Compare computed TTP with expected results. */
   float32_T result = 1.0f;
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, result);
}

/*
 * Compute the time to pass for a critical target with relative longitudinal speed zero. The computed TTP should be set to
 * TA_INVALID_TTP. \uts{CSCSA-44783} \sdd{SF-8667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttp__calculate_ttp_invalid_no_vel_long)
{
   /** \arrange Set up scenario with a critical target but zero relative longitudinal speed. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   ta_object.tracker_data.vcs_pos.x       = -10.0f;
   ta_object.tracker_data.vcs_pos.y       = 5.0f;
   ta_object.tracker_data.vcs_vel_rel.x   = 0.0f;
   ta_object.tracker_data.vcs_vel_rel.y   = 0.0f;
   ta_object.tracker_data.vcs_heading     = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max - EPSILON;

   /** \action Compute the time to pass. */
   Ta_Get_Object_Ttp(&ta_object, host_curvature, &ta_cal);

   /** \assert Compare computed TTP with expected results. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * In case of a missing valid motion in longitudinal direction an invalid value for TTP shall be returned. Here the exact boundary
 * is tested. \uts{CSCSA-44784} \sdd{SF-8667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttp__exact_boundary_test_diff_long_predicted_speed_and_speed)
{
   /** \arrange Set up scenario with a critical target but invalid motion in longitudinal direction. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   ta_object.tracker_data.vcs_pos.x       = 5.0f;
   ta_object.tracker_data.vcs_pos.y       = 5.0f;
   ta_object.tracker_data.vcs_vel_rel.x   = 0.0f;
   ta_object.tracker_data.vcs_vel_rel.y   = 10.0f;
   ta_object.tracker_data.vcs_heading     = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max - EPSILON;

   /** \action Compute the time to pass. */
   Ta_Get_Object_Ttp(&ta_object, host_curvature, &ta_cal);

   /** \assert Compare computed TTP with expected results. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * Here the threshold for the difference of predicted longitudinal motion and motion returned by tracker is exceeded by an absolute
 * minimum. However longitudinal relative velocity needs to be greater than a given threshold. This shall result in a default TTP.
 * \uts{CSCSA-44785} \sdd{SF-8667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttp__lt_boundary_test_diff_long_predicted_speed_and_speed)
{
   /** \arrange Set up scenario with a critical target with contrary longitudinal motion data. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   ta_object.tracker_data.vcs_pos.x       = 5.0f;
   ta_object.tracker_data.vcs_pos.y       = 5.0f;
   ta_object.tracker_data.vcs_vel_rel.x   = 0.1667f * EPSILON + THRESHOLD_IS_ZERO;
   ta_object.tracker_data.vcs_vel_rel.y   = 10.0f;
   ta_object.tracker_data.vcs_heading     = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max - EPSILON;

   /** \action Compute the time to pass. */
   Ta_Get_Object_Ttp(&ta_object, host_curvature, &ta_cal);

   /** \assert Compare computed TTP with expected results. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * Do not compute the time to pass for a target that is moving away from the host lane laterally with curvi info available.
 * \uts{CSCSA-44796} \sdd{SF-8613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Suppress_Ttp_Calculation__suppress_obj_moving_away_from_host_lane_with_curvi)
{
   /** \arrange Set up scenario with a critical target that is moving away from the host lane. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   float32_T obj_distance                 = -10.0f;
   float32_T obj_lat_vel_rel              = ta_cal.k_rta_ttp_obj_abs_lat_vel_rel_max + EPSILON;
   float32_T heading                      = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_TRUE;

   float32_T host_curvature =
      ta_cal.k_ta_lookup_turning_host_curvature_min[TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0 - 1] + EPSILON;

   /** \action Compute the time to pass. */
   Ta_Suppress_Ttp_Calculation(&ta_object, host_curvature, obj_lat_vel_rel, heading, obj_distance, &ta_cal);

   /** \assert Check that no TTP was calculated. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * Do not compute the time to pass for a target that is moving away from the host lane laterally without curvi info available.
 * \uts{CSCSA-44797} \sdd{SF-8613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Suppress_Ttp_Calculation__suppress_obj_moving_away_from_host_lane_without_curvi)
{
   /** \arrange Set up scenario with a critical target that is moving away from the host lane. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   float32_T obj_distance                 = -10.0f;
   float32_T obj_lat_vel_rel              = ta_cal.k_rta_ttp_obj_abs_lat_vel_rel_max + EPSILON;
   float32_T heading                      = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max + EPSILON;

   /** \action Compute the time to pass. */
   Ta_Suppress_Ttp_Calculation(&ta_object, host_curvature, obj_lat_vel_rel, heading, obj_distance, &ta_cal);

   /** \assert Check that no TTP was calculated. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * Do not compute the time to pass for a target that is heading away from the host lane with curvi info available.
 * \uts{CSCSA-44798} \sdd{SF-8613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Suppress_Ttp_Calculation__suppress_obj_heading_away_from_host_lane_with_curvi)
{
   /** \arrange Set up scenario with a critical target that is heading away from the host lane. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   float32_T obj_distance                 = -10.0f;
   float32_T obj_lat_vel_rel              = 0.0f;
   float32_T heading                      = ta_cal.k_rta_ttp_obj_abs_heading_diff_max + EPSILON;
   ta_object.attributes.f_curvi_available = FBK_TRUE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max - EPSILON;

   /** \action Compute the time to pass. */
   Ta_Suppress_Ttp_Calculation(&ta_object, host_curvature, obj_lat_vel_rel, heading, obj_distance, &ta_cal);

   /** \assert Check that no TTP was calculated. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * Do not compute the time to pass for a target that is heading away from the host lane without curvi info available.
 * \uts{CSCSA-44799} \sdd{SF-8613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Suppress_Ttp_Calculation__suppress_obj_heading_away_from_host_lane_without_curvi)
{
   /** \arrange Set up scenario with a critical target that is heading away from the host lane. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   float32_T obj_distance                 = -10.0f;
   float32_T obj_lat_vel_rel              = 0.0f;
   float32_T heading                      = ta_cal.k_rta_ttp_obj_abs_heading_diff_max + EPSILON;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max - EPSILON;

   /** \action Compute the time to pass. */
   Ta_Suppress_Ttp_Calculation(&ta_object, host_curvature, obj_lat_vel_rel, heading, obj_distance, &ta_cal);

   /** \assert Check that no TTP was calculated. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * Do not compute the time to pass for a target that has no curvi info while host vehicle is not driving straight.
 * \uts{CSCSA-44800} \sdd{SF-8667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttp__suppress_obj_no_curvi_host_not_driving_straight)
{
   /** \arrange Set up scenario with a critical target while host vehicle not driving straight. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   ta_object.tracker_data.vcs_pos.x       = -10.0f;
   ta_object.tracker_data.vcs_pos.y       = 5.0f;
   ta_object.tracker_data.vcs_vel_rel.x   = 10.0f;
   ta_object.tracker_data.vcs_vel_rel.y   = 0.0f;
   ta_object.tracker_data.vcs_heading     = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max + EPSILON;

   ta_cal.k_rta_ttp_curve_suppression_obj_distance_min = Fbk_Abs_F(ta_object.tracker_data.vcs_pos.x);

   /** \action Compute the time to pass. */
   Ta_Get_Object_Ttp(&ta_object, host_curvature, &ta_cal);

   /** \assert Check that no TTP was calculated. */
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, TA_INVALID_TTP);
}

/*
 * Do compute the time to pass for a target that has no curvi info while host vehicle is not driving straight but is close enough.
 * \uts{CSCSA-44801} \sdd{SF-8667} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Collision_Filter_Test, Ta_Get_Object_Ttp__dont_suppress_close_obj_no_curvi_host_not_driving_straight)
{
   /** \arrange Set up scenario with a critical target while host vehicle not driving straight. */
   ta_object.attributes.ttp               = TA_INVALID_TTP;
   ta_object.tracker_data.vcs_pos.x       = -10.0f;
   ta_object.tracker_data.vcs_pos.y       = 5.0f;
   ta_object.tracker_data.vcs_vel_rel.x   = 10.0f;
   ta_object.tracker_data.vcs_vel_rel.y   = 0.0f;
   ta_object.tracker_data.vcs_heading     = 0.0f;
   ta_object.attributes.f_curvi_available = FBK_FALSE;

   float32_T host_curvature = ta_cal.k_ta_straight_host_curvature_max + EPSILON;

   ta_cal.k_rta_ttp_curve_suppression_obj_distance_min = Fbk_Abs_F(ta_object.tracker_data.vcs_pos.x) + EPSILON;

   /** \action Compute the time to pass. */
   Ta_Get_Object_Ttp(&ta_object, host_curvature, &ta_cal);

   /** \assert Compare computed TTP with expected results. */
   float32_T result = 1.0f;
   EXPECT_FLOAT_EQ(ta_object.attributes.ttp, result);
}
