/**
 * @file ltb_time_calculation_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for LTB unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-46117}
 */

#include "ltb_time_calculation_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_macros.h"
#include "ltb_time_calculation.c"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <math.h>
}

/**
 * Each combination of host circle and object circle are critical in this test and are checked. Here it is expected that the
 * approach is critical. \uts{CSCSA-46179} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__result_true_all_distances)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_radius                                  = 5.0f;
   obj.circle_radius                                  = 5.0f;
   ego.waypoint_yaw_angle.angle                       = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle                       = ltb_cals.k_ltb_critical_approach_angle_diff_min;
   ltb_cals.k_ltb_critical_approach_min_safe_distance = 0.5f;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}

/**
 * Each combination of host circle and object circle is checked within this test. The approach shall not be critical.
 * \uts{CSCSA-46180} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__result_false_all_distances)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.waypoint_coordinates.x = 10.0f;
   ego.waypoint_coordinates.y = 10.0f;
   ego.circle_center_front.x  = 10.0f;
   ego.circle_center_front.y  = 10.0f;
   ego.circle_center_middle.x = 10.0f;
   ego.circle_center_middle.y = 10.0f;
   ego.circle_center_rear.x   = 10.0f;
   ego.circle_center_rear.y   = 10.0f;

   ego.circle_radius = 1.0f;
   obj.circle_radius = 1.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}

/**
 * Approach is only deemed critical if the approach angle is larger than a specified threshold.
 * \uts{CSCSA-46181} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__approach_angle_too_shallow)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_radius                                  = 5.0f;
   obj.circle_radius                                  = 5.0f;
   ego.waypoint_yaw_angle.angle                       = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle                       = FBK_ZERO_F;
   ltb_cals.k_ltb_critical_approach_min_safe_distance = 0.5f;


   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}

/**
 * Test whether a critical approach is occuring with respect to the front circles of object and host. The approach shall be
 * critical. \uts{CSCSA-46182} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__result_true_ego_front)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_center_front.x = 10.0f;
   ego.circle_center_front.y = 10.0f;
   ego.circle_radius         = 1.0f;

   obj.circle_center_front.x = 10.0f;
   obj.circle_center_front.y = 10.0f;
   obj.circle_radius         = 1.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects front center and hosts front. Result shall be that no
 * critical approach is given. \uts{CSCSA-46183} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__exact_boundary_test_host_front_obj_front_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = 7.5f;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ltb_cals.k_ltb_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = ego.circle_center_front.y + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   ltb_cals.k_ltb_critical_approach_min_safe_distance = 0.5f;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}

/**
 * Tests the exact boundary of minimum needed safety distance between objects front center and hosts front. Result shall be that a
 * critical approach is given. \uts{CSCSA-46184} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__lt_boundary_test_host_front_obj_front_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   float32_T pos_without_eps = 7.5f;
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ltb_cals.k_ltb_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = pos_without_eps + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects center circles and hosts front. Result shall be that
 * no critical approach is given. \uts{CSCSA-46185} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__exact_boundary_test_host_front_obj_center_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = 7.5f;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ltb_cals.k_ltb_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 2.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = 0.0f;
   obj.circle_center_middle.y = ego.circle_center_front.y + min_safe_dist;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   ltb_cals.k_ltb_critical_approach_min_safe_distance = 0.5f;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects center circles and hosts front. Result shall be that
 * a critical approach is given. \uts{CSCSA-46186} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__lt_boundary_test_host_front_obj_center_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps = 7.5f;

   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ltb_cals.k_ltb_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = ego.circle_center_front.y + min_safe_dist;
   obj.circle_center_middle.x = -1.0f;
   obj.circle_center_middle.y = 9.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   ltb_cals.k_ltb_critical_approach_min_safe_distance = 0.5f;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts front. Result shall be that no
 * critical approach is given. \uts{CSCSA-46187} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__exact_boundary_test_host_front_obj_rear_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = 7.5f;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ltb_cals.k_ltb_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = -4.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = 0.0f;
   obj.circle_center_rear.y   = ego.circle_center_front.y + min_safe_dist;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   ltb_cals.k_ltb_critical_approach_min_safe_distance = 0.5f;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts front. Result shall be that a
 * critical approach is given. \uts{CSCSA-46188} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__lt_boundary_test_host_front_obj_rear_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps = 7.5f;
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   obj.circle_center_front.x  = -4.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = 0.0f;
   obj.circle_center_rear.y   = 10.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}

/**
 * Test whether a critical approach is occuring with respect to the front circles of host and middle circles of object. The
 * approach shall be critical. \uts{CSCSA-46189} \sdd{CSCSA-53939} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Is_Critical_Approach__result_true_ego_middle)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_center_front.x = 10.0f;
   ego.circle_center_front.y = 10.0f;
   ego.circle_radius         = 1.0f;

   obj.circle_center_middle.x = 10.0f;
   obj.circle_center_middle.y = 10.0f;
   obj.circle_radius          = 1.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Get critical approach estimation */
   boolean_T result = Ltb_Is_Critical_Approach(&ltb_cals, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}

/**
 * Set up input values to have a negative acceleration and thus a smaller host vehicle speed after the provided dead time. The
 * expected return value is smaller than current host speed. \uts{CSCSA-46190} \sdd{CSCSA-53936} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Host_Speed_After_Dead_Time__check_correct_host_speed_retrieval)
{
   /** \arrange Set up valid ego trajectory values. */
   ltb_cals.k_ltb_brake_dead_time = 0.2f;
   uint8_t n_dead_time_cylces     = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Retrieve the host speed from the ego trajectory. */
   float32_T host_speed_after_dead_time = Ltb_Get_Host_Speed_After_Dead_Time(&ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Check that the correct value was retrieved from the ego trajectory information. */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed);
}

/**
 * Set up invalid time step value.
 * \uts{CSCSA-46191} \sdd{CSCSA-53936} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Host_Speed_After_Dead_Time__return_default_value_for_invalid_time_step_value)
{
   /** \arrange Set up an invalid time step value. */
   p_ltb_persistent->ltb_pred_step_dt = FBK_ZERO_F;

   /** \action Try to retrieve the host speed. */
   float32_T host_speed_after_dead_time = Ltb_Get_Host_Speed_After_Dead_Time(&ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Check that the default value is returned. */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, FBK_ZERO_F);
}

/**
 * Set up invalid number of prediction steps.
 * \uts{CSCSA-46192} \sdd{CSCSA-53936} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Host_Speed_After_Dead_Time__return_default_value_for_pred_step_out_of_bounds)
{
   /** \arrange Set up an invalid number of prediction steps. */
   ego_trajectory.n_prediction_steps = FBK_ZERO_UINT;

   /** \action Try to retrieve the host speed. */
   float32_T host_speed_after_dead_time = Ltb_Get_Host_Speed_After_Dead_Time(&ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Check that the default value is returned. */
   EXPECT_FLOAT_EQ(host_speed_after_dead_time, FBK_ZERO_F);
}

/**
 * Set up input values to have quadratic formula with real roots. The expected deceleration is the first root of that formula.
 * \uts{CSCSA-46193} \sdd{CSCSA-53940} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Gradient_Restricted_Deceleration__gradient_corrected_decel)
{
   /** \arrange Set up negative gradient with large absolute value. */
   float32_T t_brake           = 1.2f;
   float32_T host_speed        = 10.0f;
   boolean_T f_enable_gradient = FBK_TRUE;
   float32_T braking_gradient  = -85.0f;

   /** \action Compute the deceleration value. */
   float32_T deceleration = Ltb_Get_Gradient_Restricted_Deceleration(t_brake, host_speed, f_enable_gradient, braking_gradient);

   /** \assert Check that the calculated deceleration is greater than the fallback based on just speed and t_brake. */
   EXPECT_GT(deceleration, Abs(host_speed / t_brake));
}

/**
 * Set up input values to have no brake gradient. The expected deceleration is the simple division of host_speed and t_brake.
 * \uts{CSCSA-46194} \sdd{CSCSA-53940} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Gradient_Restricted_Deceleration__for_jerk_value_zero)
{
   /** \arrange Set up a gradient value of zero. */
   float32_T t_brake           = 1.8f;
   float32_T host_speed        = 10.0f;
   boolean_T f_enable_gradient = FBK_FALSE;
   float32_T braking_gradient  = 0.0f;

   /** \action Compute the deceleration value. */
   float32_T deceleration = Ltb_Get_Gradient_Restricted_Deceleration(t_brake, host_speed, f_enable_gradient, braking_gradient);

   /** \assert Check that the calculated deceleration is based on just speed and t_brake. */
   EXPECT_FLOAT_EQ(deceleration, Abs(host_speed / t_brake));
}

/**
 * Set up input values to have quadratic formula without real roots. The expected deceleration is a fallback to the simple division
 * of host_speed and t_brake. \uts{CSCSA-46195} \sdd{CSCSA-53940} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Gradient_Restricted_Deceleration__fallback_to_no_gradient_calc)
{
   /** \arrange Set up negative gradient with value close to zero. */
   float32_T t_brake           = 1.0f;
   float32_T host_speed        = 10.0f;
   boolean_T f_enable_gradient = FBK_TRUE;
   float32_T braking_gradient  = -1.0f;

   /** \action Compute the deceleration value. */
   float32_T deceleration = Ltb_Get_Gradient_Restricted_Deceleration(t_brake, host_speed, f_enable_gradient, braking_gradient);

   /** \assert Check that the calculated deceleration is the fallback based on just speed and t_brake. */
   EXPECT_FLOAT_EQ(deceleration, Abs(host_speed / t_brake));
}


/**
 * Set up critical scenario at which the ego vehicle and target are at the same position. The expected TTC is zero as well as the
 * collision coordinate. \uts{CSCSA-46196} \sdd{CSCSA-53932} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Object_Ttc__test_critical_obj)
{
   /** \arrange Set up critical scenario at which the ego vehicle and target are at the same position. */
   uint8_t prediction_step = 0;

   Ltb_Reset_Trajectory(&ego_trajectory, &ltb_cals);
   Ltb_Reset_Trajectory(&ltb_object.attributes.trajectory, &ltb_cals);

   ltb_object.attributes.ttc                     = LTB_INVALID_TTC;
   ltb_object.attributes.waypoint_at_collision.x = FBK_ZERO_F;
   ltb_object.attributes.waypoint_at_collision.y = FBK_ZERO_F;

   ego_trajectory.f_trajectory_valid = FBK_TRUE;
   ego_trajectory.n_prediction_steps = 1;

   ltb_object.attributes.trajectory.f_trajectory_valid = FBK_TRUE;
   ltb_object.attributes.trajectory.n_prediction_steps = 1;

   ego_trajectory.waypoint[prediction_step].f_waypoint_valid                   = FBK_TRUE;
   ltb_object.attributes.trajectory.waypoint[prediction_step].f_waypoint_valid = FBK_TRUE;

   ego_trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle = FBK_ZERO_F;
   ltb_object.attributes.trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle =
      ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Compute the TTC and collision coordinate. */
   Ltb_Get_Object_Ttc(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed TTC and coordinates with expected results. */
   EXPECT_FLOAT_EQ(ltb_object.attributes.ttc, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ltb_object.attributes.waypoint_at_collision.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ltb_object.attributes.waypoint_at_collision.y, FBK_ZERO_F);
}

/**
 * Set up critical scenario at which the ego vehicle and target collide but the trajectories are set as invalid. The expected TTC
 * is LTB_INVALID_TTC. \uts{CSCSA-46197} \sdd{CSCSA-53932} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Object_Ttc__test_invalid_trajectories)
{
   /** \arrange Set up critical scenario at which the ego vehicle and target collide but the trajectories are set as invalid. */
   uint8_t prediction_step = 0;

   ltb_object.attributes.ttc                     = LTB_INVALID_TTC;
   ltb_object.attributes.waypoint_at_collision.x = FBK_ZERO_F;
   ltb_object.attributes.waypoint_at_collision.y = FBK_ZERO_F;

   Ltb_Reset_Trajectory(&ego_trajectory, &ltb_cals);
   Ltb_Reset_Trajectory(&ltb_object.attributes.trajectory, &ltb_cals);

   ego_trajectory.waypoint[prediction_step].f_waypoint_valid                   = FBK_FALSE;
   ltb_object.attributes.trajectory.waypoint[prediction_step].f_waypoint_valid = FBK_FALSE;

   ego_trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle = FBK_ZERO_F;
   ltb_object.attributes.trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle =
      ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Compute the TTC and collision coordinate. */
   Ltb_Get_Object_Ttc(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed TTC and coordinates with expected results. */
   EXPECT_EQ(ltb_object.attributes.ttc, LTB_INVALID_TTC);
   EXPECT_FLOAT_EQ(ltb_object.attributes.waypoint_at_collision.x, 0.0f);
   EXPECT_FLOAT_EQ(ltb_object.attributes.waypoint_at_collision.y, 0.0f);
}

/**
 * Set up critical scenario at which the ego vehicle and target collide but the waypoints are set as invalid. The expected TTC is
 * LTB_INVALID_TTC. \uts{CSCSA-46198} \sdd{CSCSA-53932} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Object_Ttc__skip_steps_for_invalid_waypoints)
{
   /** \arrange Set up critical scenario at which the ego vehicle and target collide but the waypoints are set as invalid. */
   uint8_t prediction_step = 0;

   ltb_object.attributes.ttc                     = LTB_INVALID_TTC;
   ltb_object.attributes.waypoint_at_collision.x = FBK_ZERO_F;
   ltb_object.attributes.waypoint_at_collision.y = FBK_ZERO_F;

   Ltb_Reset_Trajectory(&ego_trajectory, &ltb_cals);
   Ltb_Reset_Trajectory(&ltb_object.attributes.trajectory, &ltb_cals);

   ego_trajectory.f_trajectory_valid = FBK_TRUE;
   ego_trajectory.n_prediction_steps = 1;

   ltb_object.attributes.trajectory.f_trajectory_valid = FBK_TRUE;
   ltb_object.attributes.trajectory.n_prediction_steps = 1;

   ego_trajectory.waypoint[prediction_step].f_waypoint_valid                   = FBK_FALSE;
   ltb_object.attributes.trajectory.waypoint[prediction_step].f_waypoint_valid = FBK_FALSE;

   ego_trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle = FBK_ZERO_F;
   ltb_object.attributes.trajectory.waypoint[prediction_step].waypoint_yaw_angle.angle =
      ltb_cals.k_ltb_critical_approach_angle_diff_min;

   /** \action Compute the TTC and collision coordinate. */
   Ltb_Get_Object_Ttc(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed TTC and coordinates with expected results. */
   EXPECT_EQ(ltb_object.attributes.ttc, LTB_INVALID_TTC);
   EXPECT_FLOAT_EQ(ltb_object.attributes.waypoint_at_collision.x, 0.0f);
   EXPECT_FLOAT_EQ(ltb_object.attributes.waypoint_at_collision.y, 0.0f);
}


/**
 * Compute the deceleration of the ego vehicle required to avoid a collision with the target. Compare the computed result with the
 * manually derived value. \uts{CSCSA-46199} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__calculate_deceleration)
{
   /** \arrange Set ego vehicle parameters and TTC to target. */
   ltb_object.attributes.ttc                    = 1.2f;
   ltb_cals.k_ltb_brake_dead_time               = 0.2f;
   ltb_cals.k_ltb_brake_deceleration_max        = 10.0f;
   ltb_cals.k_f_ltb_enable_brake_gradient_logic = FBK_FALSE;
   ltb_cals.k_ltb_brake_gradient                = 0.0f;
   uint8_t n_dead_time_cylces                   = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 1.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result =
      ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / (ltb_object.attributes.ttc - ltb_cals.k_ltb_brake_dead_time);
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Within that test the condition for the ttc is not fulfilled. This leads to a deceleration signal to be unchanged from its
 * default value. \uts{CSCSA-46200} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__exact_boundary_test_ttc)
{
   /** \arrange Set TTC to invalid value. */
   ltb_object.attributes.ttc = LTB_INVALID_TTC;

   float32_T default_val_decel               = 0.0f;
   ltb_object.attributes.decel_to_avoid_coll = default_val_decel;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, default_val_decel);
}


/*
 * Within that test the condition for the ttc is fulfilled. This leads to a deceleration signal to differ from its default value.
 * \uts{CSCSA-46201} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__lt_boundary_test_ttc)
{
   /** \arrange Set TTC to valid value. */
   ltb_object.attributes.ttc = LTB_INVALID_TTC - 0.1f;

   ltb_cals.k_ltb_brake_dead_time               = 0.2f;
   ltb_cals.k_ltb_brake_deceleration_max        = 10.0f;
   ltb_cals.k_f_ltb_enable_brake_gradient_logic = FBK_FALSE;
   ltb_cals.k_ltb_brake_gradient                = 0.0f;
   uint8_t n_dead_time_cylces                   = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result =
      ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / (ltb_object.attributes.ttc - ltb_cals.k_ltb_brake_dead_time);
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Case fulfilled in which the deceleration is calculated by formula. The difference of ttc and braketime shall exceed EPSILON.
 * \uts{CSCSA-46202} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__gt_boundary_test_diff_ttc_and_brake_dead_time)
{
   /** \arrange Set up ego braking parameters. */
   ltb_cals.k_ltb_brake_dead_time        = 0.8f;
   ltb_object.attributes.ttc             = 0.8f + EPSILON;
   ltb_cals.k_ltb_brake_deceleration_max = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results (saturated deceleration signal). */
   float32_T result = ltb_cals.k_ltb_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Case fulfilled in which the deceleration is set to the maximum deceleration value. The difference of ttc and braketime shall
 * exceed EPSILON. \uts{CSCSA-46203} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__exact_boundary_test_diff_ttc_and_brake_dead_time)
{
   /** \arrange Set up ego braking parameters. */
   ltb_object.attributes.ttc  = 0.8f;
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->long_acc   = 0.0f;

   ltb_cals.k_ltb_brake_dead_time        = ltb_object.attributes.ttc - EPSILON;
   ltb_cals.k_ltb_brake_deceleration_max = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ltb_cals.k_ltb_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, result);
}


/*
 * For the saturation the exact boundary for the saturation of deceleration to avoid collision signal shall be tested here. Within
 * this test the deceleration signal shall not be saturated, since it is located on the exact boundary. \uts{CSCSA-46204}
 * \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__exact_boundary_test_saturation_on_max_val)
{
   /** \arrange Set up ego braking parameters. */
   ltb_object.attributes.ttc = 1.1f;

   ltb_cals.k_ltb_brake_dead_time        = 0.1f;
   ltb_cals.k_ltb_brake_deceleration_max = 10.0f;
   uint8_t n_dead_time_cylces            = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ltb_cals.k_ltb_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, result);
}


/*
 * A value greater than the saturation check threshold is tested. The Deceleration to avoid collision signal shall be saturated.
 * \uts{CSCSA-46205} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__gt_boundary_test_saturation_on_max_val)
{
   /** \arrange Set up ego braking parameters. */
   ltb_object.attributes.ttc = 0.9f;

   ltb_cals.k_ltb_brake_dead_time        = 0.1f;
   ltb_cals.k_ltb_brake_deceleration_max = 10.0f;
   uint8_t n_dead_time_cylces            = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ltb_cals.k_ltb_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, result);
}


/*
 * Modifies the ttc such that a division with zero is possible. A Division with zero shall not occur here.
 * \uts{CSCSA-46206} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__error_guessing_division_with_zero)
{
   /** \arrange Set up ego braking parameters. */
   ltb_cals.k_ltb_brake_dead_time = 1.0f;
   ltb_object.attributes.ttc      = ltb_cals.k_ltb_brake_dead_time + 0.1f * EPSILON;

   ltb_cals.k_ltb_brake_deceleration_max = 10.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   float32_T result = ltb_cals.k_ltb_brake_deceleration_max;
   EXPECT_FLOAT_EQ(ltb_object.attributes.decel_to_avoid_coll, result);
}

/*
 * Set up a scenario in which the braking signal should saturate and verify that saturation is reached.
 * \uts{CSCSA-46207} \sdd{CSCSA-53935} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Ego_Deceleration_To_Avoid_Collision__cap_to_max_deceleration)
{
   /** \arrange Set up ego braking parameters. */
   ltb_object.attributes.ttc = 0.2f;

   ltb_cals.k_ltb_brake_dead_time        = 0.2f;
   ltb_cals.k_ltb_brake_deceleration_max = 10.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 20.0f;

   /** \action Compute the deceleration required to avoid a collision. */
   Ltb_Get_Ego_Deceleration_To_Avoid_Collision(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed deceleration with expected results. */
   EXPECT_EQ(ltb_object.attributes.decel_to_avoid_coll, ltb_cals.k_ltb_brake_deceleration_max);
}

/*
 * Compute the remaining time to brake for a given TTC and host vehicle condition. Compare the computed TTB with a manually
 * calculated TTB. \uts{CSCSA-46208} \sdd{CSCSA-53933} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Object_Ttb__calculate_ttb_no_gradient_brake_threshold_not_reached)
{
   /** \arrange Set up scenario with a given TTC and host vehicle condition. */
   ltb_object.attributes.ttc = 0.8f;

   ltb_cals.k_ltb_brake_dead_time             = 0.2f;
   ltb_cals.k_ltb_brake_gradient              = 0.0f;
   ltb_cals.k_ltb_alert_lvl_3_ttc_threshold   = 0.8f;
   ltb_cals.k_ltb_alert_lvl_3_decel_threshold = 6.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 2.0f;

   /** \action Compute the remaining time to brake. */
   Ltb_Get_Object_Ttb(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed TTB with expected results. */
   float32_T ttb_no_gradient =
      ltb_object.attributes.ttc
      - (ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / ltb_cals.k_ltb_alert_lvl_3_decel_threshold)
      - ltb_cals.k_ltb_brake_dead_time;
   EXPECT_FLOAT_EQ(ltb_object.attributes.ttb, ttb_no_gradient);
}

/*
 * Compute the remaining time to brake for a given TTC and host vehicle condition. Compare the computed TTB with a manually
 * calculated TTB. \uts{CSCSA-46209} \sdd{CSCSA-53933} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Object_Ttb__calculate_ttb_with_gradient_brake_threshold_not_reached)
{
   /** \arrange Set up scenario with a given TTC and host vehicle condition. */
   ltb_object.attributes.ttc = 0.8f;

   ltb_cals.k_f_ltb_enable_brake_gradient_logic = FBK_TRUE;
   ltb_cals.k_ltb_brake_dead_time               = 0.2f;
   ltb_cals.k_ltb_brake_gradient                = -85.0f;
   ltb_cals.k_ltb_alert_lvl_3_ttc_threshold     = 0.8f;
   ltb_cals.k_ltb_alert_lvl_3_decel_threshold   = 6.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 2.0f;

   /** \action Compute the remaining time to brake. */
   Ltb_Get_Object_Ttb(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed TTB with expected results. */
   float32_T ttb_no_gradient =
      ltb_object.attributes.ttc
      - (ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed / ltb_cals.k_ltb_alert_lvl_3_decel_threshold)
      - ltb_cals.k_ltb_brake_dead_time;
   EXPECT_LT(ltb_object.attributes.ttb, ttb_no_gradient);
}

/*
 * Compute the remaining time to brake for a given TTC and host vehicle condition. Compare the computed TTB with a manually
 * calculated TTB. \uts{CSCSA-46210} \sdd{CSCSA-53933} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Time_Calculation_Test, Ltb_Get_Object_Ttb__calculate_ttb_for_brake_threshold_reached_but_ttc_not_reached)
{
   /** \arrange Set up scenario with a given TTC and host vehicle condition. */
   ltb_object.attributes.ttc = 0.9f;

   ltb_cals.k_ltb_brake_dead_time             = 0.2f;
   ltb_cals.k_ltb_alert_lvl_3_ttc_threshold   = 0.8f;
   ltb_cals.k_ltb_alert_lvl_3_decel_threshold = 6.0f;

   uint8_t n_dead_time_cylces = (uint8_t) (ltb_cals.k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt);
   ego_trajectory.waypoint[n_dead_time_cylces].waypoint_speed = 5.0f;

   /** \action Compute the remaining time to brake. */
   Ltb_Get_Object_Ttb(&ltb_object, &ego_trajectory, p_ltb_persistent, &ltb_cals);

   /** \assert Compare computed TTB with expected results. */
   float32_T result = ltb_object.attributes.ttc - ltb_cals.k_ltb_alert_lvl_3_ttc_threshold;
   EXPECT_FLOAT_EQ(ltb_object.attributes.ttb, result);
}
