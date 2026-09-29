/**
 * @file fbk_ego_traj_predictor_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42223}
 */

#include "fbk_ego_traj_predictor_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
}

using ::testing::FloatNear;

Fbk_Ego_Traj_Predictor_Instance_T Fbk_Ego_Traj_Predictor_Test::Ego_Traj_Instance;

/**
 * Compute ego vehicle curve radius from speed and yawrate.
 * \uts{CSCSA-42278} \sdd{SF-4224} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Host_Curve_Radius__check_curve_radius_calculation)
{
   /** \arrange Set up speed and yawrate. */
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->yawrate    = 0.5f;

   /** \action Compute ego curve radius */
   float32_T curve_radius = Fbk_Get_Host_Curve_Radius(p_vehicle_data);

   /** \assert Check if computed curve radius is correct. */
   EXPECT_FLOAT_EQ(curve_radius, 20.0f);
}

/**
 * Compute ego vehicle curve radius from negative speed and yawrate.
 * \uts{CSCSA-186066} \sdd{SF-4224} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Host_Curve_Radius__check_curve_radius_calculation_negative_vel)
{
   /** \arrange Set up speed and yawrate. */
   p_vehicle_data->host_speed = -10.0f;
   p_vehicle_data->yawrate    = 0.5f;

   /** \action Compute ego curve radius */
   float32_T curve_radius = Fbk_Get_Host_Curve_Radius(p_vehicle_data);

   /** \assert Check if computed curve radius is correct. */
   EXPECT_FLOAT_EQ(curve_radius, 20.0f);
}

/**
 * Give default value for driving straight.
 * \uts{CSCSA-42279} \sdd{SF-4224} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Host_Curve_Radius__check_default_value)
{
   /** \arrange Set up speed and yawrate. */
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->yawrate    = 0.0001f;

   /** \action Compute ego curve radius */
   float32_T curve_radius = Fbk_Get_Host_Curve_Radius(p_vehicle_data);

   /** \assert Check if computed curve radius is the default value. */
   EXPECT_FLOAT_EQ(curve_radius, 0.0f);
}

/**
 * Give default value for driving straight with negative yawrate.
 * \uts{CSCSA-186067} \sdd{SF-4224} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Host_Curve_Radius__check_default_value_negative_yawrate)
{
   /** \arrange Set up speed and yawrate. */
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->yawrate    = -0.0001f;

   /** \action Compute ego curve radius */
   float32_T curve_radius = Fbk_Get_Host_Curve_Radius(p_vehicle_data);

   /** \assert Check if computed curve radius is the default value. */
   EXPECT_FLOAT_EQ(curve_radius, 0.0f);
}


/**
 * Initialize ego circle properties from vehicle dimensions.
 * \uts{CSCSA-42280} \sdd{SF-4226} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Init_Ego_Circle_Props__test_ego_circle_probs)
{
   /** \arrange Set up vehicle properties. */
   Fbk_Host_Circles_Props_T host_circles_props;
   p_vehicle_data->host_length       = 4.0f;
   p_vehicle_data->host_width        = 2.0f;
   fbk_ego_data.ego_shape_gain_fixed = 3.0f;

   /** \action Initialize ego circle properties */
   Fbk_Init_Ego_Circle_Props(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, &host_circles_props);

   /** \assert Check if computed ego circle properties equal expected results. */
   EXPECT_FLOAT_EQ(host_circles_props.radius, 3.0f);
   EXPECT_FLOAT_EQ(host_circles_props.offsets.offset_front_x, -1.0f);
   EXPECT_FLOAT_EQ(host_circles_props.offsets.offset_middle_x, -2.0f);
   EXPECT_FLOAT_EQ(host_circles_props.offsets.offset_rear_x, -3.0f);
}

/**
 * Create simple ego trajectory and verify that trajectory is valid.
 * \uts{CSCSA-42281} \sdd{SF-4229} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Predict_Ego_Trajectory__traj_is_valid)
{
   /** \arrange Set up vehicle properties. */
   p_vehicle_data->host_speed         = 1.2f;
   p_vehicle_data->long_acc           = 0.0f;
   p_vehicle_data->yawrate            = 0.0f;
   p_vehicle_data->host_length        = 4.5f;
   p_vehicle_data->host_width         = 2.7f;
   p_vehicle_data->rear_axle_position = -2.25f;

   /** \action Predict ego trajectory */
   Fbk_Predict_Ego_Trajectory(&Ego_Traj_Instance, &ego_trajectory, p_vehicle_data, &fbk_ego_data);

   /** \assert Check if computed ego trajectory is valid. */
   EXPECT_TRUE(ego_trajectory.f_trajectory_valid);
   EXPECT_TRUE(ego_trajectory.waypoint[1].f_waypoint_valid);
}

/**
 * Create invalid ego vehicle properties and verify that trajectory is invalid.
 * \uts{CSCSA-42282} \sdd{SF-4229} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Predict_Ego_Trajectory__break_the_loop)
{
   /** \arrange Set up vehicle properties. */
   p_vehicle_data->host_speed         = 1.0f;
   p_vehicle_data->long_acc           = -10.0f;
   p_vehicle_data->yawrate            = 0.0f;
   p_vehicle_data->host_length        = 4.5f;
   p_vehicle_data->host_width         = 2.7f;
   p_vehicle_data->rear_axle_position = -2.25f;

   /** \action Predict ego trajectory */
   Fbk_Predict_Ego_Trajectory(&Ego_Traj_Instance, &ego_trajectory, p_vehicle_data, &fbk_ego_data);

   /** \assert Check if computed ego trajectory is invalid. */
   EXPECT_FALSE(ego_trajectory.waypoint[1].f_waypoint_valid);
}

/**
 * Create invalid ego vehicle properties and verify that trajectory is invalid.
 * \uts{CSCSA-42283} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__initial_call_sine_test)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 0u;

   p_vehicle_data->yawrate            = 0.0f;
   p_vehicle_data->host_length        = 4.5f;
   p_vehicle_data->host_width         = 2.7f;
   p_vehicle_data->host_speed         = 1.2f;
   p_vehicle_data->rear_axle_position = -2.25f;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.x, p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.y, 0.0f);
}

/**
 * Create vehicle trajectory once with and without braking flag set. Since the vehicle is decelerating (only in the second
 * computation), the computed trajectories should be equal. \uts{CSCSA-42284} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__f_customer_adapter_feature_brake_active_true)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;
   Fbk_Waypoint_with_Circle_Centers_T waypoint_brake_flag_on{};
   Fbk_Waypoint_with_Circle_Centers_T waypoint_brake_flag_off{};

   p_vehicle_data->yawrate            = 0.0f;
   p_vehicle_data->host_length        = 4.0f;
   p_vehicle_data->host_width         = 2.0f;
   p_vehicle_data->host_speed         = 1.0f;
   p_vehicle_data->rear_axle_position = -2.0f;

   fbk_ego_data.ego_pred_const_velocity_pred_steps_min = prediction_step;

   /** \action Predict ego waypoint, first without any acceleration and no brake alert. Afterwards with decelaration and brake
    * alert. If the brake alert is active, acceleration should not be taken into account. As a consequence, both calls should
    * compute the same values. Since Integral_Predicted_Arc_Length is persistent, we need to reset it. */
   Ego_Traj_Instance.Integral_Predicted_Arc_Length = 0.0f;
   fbk_ego_data.acc_weight_depend_on_alert_lvl     = FBK_FALSE;
   p_vehicle_data->long_acc                        = 0.0f;

   waypoint_brake_flag_off = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   Ego_Traj_Instance.Integral_Predicted_Arc_Length = 0.0f;
   fbk_ego_data.acc_weight_depend_on_alert_lvl     = FBK_TRUE;
   p_vehicle_data->long_acc                        = -5.0f;

   waypoint_brake_flag_on = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_brake_flag_on.f_waypoint_valid);
   EXPECT_TRUE(waypoint_brake_flag_off.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_brake_flag_on.waypoint_coordinates.x, waypoint_brake_flag_off.waypoint_coordinates.x);
   EXPECT_FLOAT_EQ(waypoint_brake_flag_on.waypoint_coordinates.y, waypoint_brake_flag_off.waypoint_coordinates.y);
}

/**
 * Create vehicle trajectory with positive host vehicle acceleration. Predicted position change should influenced by acceleration
 * weight. \uts{CSCSA-42285} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__acceleration_weighted_for_positive_accel)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->host_speed                    = 5.0f;
   p_vehicle_data->long_acc                      = 10.0f;
   Ego_Traj_Instance.Integral_Predicted_Velocity = p_vehicle_data->host_speed;

   fbk_ego_data.ego_acceleration_weight = 0.5f;
   fbk_ego_data.ego_deceleration_weight = 1.0f;

   float32_T expected_result =
      p_vehicle_data->host_speed + (p_vehicle_data->long_acc * fbk_ego_data.ego_acceleration_weight * fbk_ego_data.pred_step_dt);

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(Ego_Traj_Instance.Integral_Predicted_Velocity, expected_result);
}

/**
 * Create vehicle trajectory with negative host vehicle acceleration. Predicted position change should influenced by acceleration
 * weight. \uts{CSCSA-42286} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__acceleration_weighted_for_negative_accel)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->host_speed                    = 5.0f;
   p_vehicle_data->long_acc                      = -10.0f;
   Ego_Traj_Instance.Integral_Predicted_Velocity = p_vehicle_data->host_speed;

   fbk_ego_data.ego_acceleration_weight = 0.5f;
   fbk_ego_data.ego_deceleration_weight = 1.0f;

   float32_T expected_result =
      p_vehicle_data->host_speed + (p_vehicle_data->long_acc * fbk_ego_data.ego_deceleration_weight * fbk_ego_data.pred_step_dt);

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(Ego_Traj_Instance.Integral_Predicted_Velocity, expected_result);
}

/**
 * Create vehicle trajectory with negative host vehicle acceleration. Predicted position change should not be influenced by
 * acceleration after specified prediction step when level 4 alert is active. \uts{CSCSA-42287} \sdd{SF-4225}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__acceleration_zero_for_negative_accel_due_to_alert_lvl)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = fbk_ego_data.ego_pred_const_velocity_pred_steps_min;

   p_vehicle_data->host_speed                    = 5.0f;
   p_vehicle_data->long_acc                      = -10.0f;
   Ego_Traj_Instance.Integral_Predicted_Velocity = p_vehicle_data->host_speed;
   fbk_ego_data.acc_weight_depend_on_alert_lvl   = FBK_TRUE;

   float32_T expected_result = p_vehicle_data->host_speed;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(Ego_Traj_Instance.Integral_Predicted_Velocity, expected_result);
}

/*
 * Tests the exact boundary for integral predicted velocity signal. The change of the predicted signals shall not occur and the
 * waypoint shall be valid. \uts{CSCSA-42288} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__exact_boundary_test_integral_predicted_velocity)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 0u;

   p_vehicle_data->yawrate            = 0.0f;
   p_vehicle_data->host_length        = 4.5f;
   p_vehicle_data->host_width         = 2.7f;
   p_vehicle_data->host_speed         = EPSILON;
   p_vehicle_data->rear_axle_position = -2.25f;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.x, p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.y, 0.0f);
}

/*
 * Tests a value greater than the boundary for integral predicted velocity signal. The predicted position shall be modified and the
 * waypoint shall still be valid. \uts{CSCSA-42289} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__gt_boundary_test_integral_predicted_velocity)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->yawrate            = 0.0f;
   p_vehicle_data->host_length        = 4.5f;
   p_vehicle_data->host_width         = 2.7f;
   p_vehicle_data->host_speed         = EPSILON + 0.1f * EPSILON;
   p_vehicle_data->rear_axle_position = -2.25f;
   p_vehicle_data->long_acc           = 1.0f;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.x, -2.2399f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.y, 0.0f);
}

/*
 * Tests the function response for invalid input data.
 * \uts{CSCSA-42290} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__check_invalid_input_data)
{
   /** \arrange Set up invalid vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->yawrate            = FBK_ZERO_F;
   p_vehicle_data->host_length        = FBK_ZERO_F;
   p_vehicle_data->host_width         = FBK_ZERO_F;
   p_vehicle_data->host_speed         = FBK_ZERO_F;
   p_vehicle_data->rear_axle_position = FBK_ZERO_F;
   p_vehicle_data->long_acc           = FBK_ZERO_F;
   Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, FBK_ZERO_UINT);

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if waypoint is considered invalid. */
   EXPECT_FALSE(waypoint_with_circle_centers.f_waypoint_valid);
}


/**
 * Create simple ego trajectory and verify that trajectory is not valid due to low prediction steps.
 * \uts{CSCSA-42292} \sdd{SF-4229} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Predict_Ego_Trajectory__traj_is_not_valid_low_steps)
{
   /** \arrange Set up vehicle properties. */
   p_vehicle_data->host_speed         = 1.2f;
   p_vehicle_data->long_acc           = 0.0f;
   p_vehicle_data->yawrate            = 0.0f;
   p_vehicle_data->host_length        = 4.5f;
   p_vehicle_data->host_width         = 2.7f;
   p_vehicle_data->rear_axle_position = -2.25f;
   ego_trajectory.n_prediction_steps  = 0u;

   /** \action Predict ego trajectory */
   Fbk_Predict_Ego_Trajectory(&Ego_Traj_Instance, &ego_trajectory, p_vehicle_data, &fbk_ego_data);

   /** \assert Check if computed ego trajectory is not valid. */
   EXPECT_FALSE(ego_trajectory.f_trajectory_valid);
}

/*
 * Tests a value greater than the boundary for integral predicted velocity signal. The predicted position shall be modified using
 * curce radius and the waypoint shall still be valid. \uts{CSCSA-42293} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__use_radius_for_calculation)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->yawrate             = 0.0f;
   p_vehicle_data->host_length         = 4.5f;
   p_vehicle_data->host_width          = 2.7f;
   p_vehicle_data->host_speed          = EPSILON + 0.1f * EPSILON;
   p_vehicle_data->rear_axle_position  = -2.25f;
   p_vehicle_data->long_acc            = 1.0f;
   Ego_Traj_Instance.Host_Curve_Radius = 1.0f;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.x, -2.2400293f);
}

/*
 * Tests a value greater than the boundary for integral predicted velocity signal. The predicted position shall be modified and the
 * waypoint shall still be valid. \uts{CSCSA-42294} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__radius_over_max)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->yawrate             = 0.0f;
   p_vehicle_data->host_length         = 4.5f;
   p_vehicle_data->host_width          = 2.7f;
   p_vehicle_data->host_speed          = EPSILON + 0.1f * EPSILON;
   p_vehicle_data->rear_axle_position  = -2.25f;
   p_vehicle_data->long_acc            = 1.0f;
   Ego_Traj_Instance.Host_Curve_Radius = 301.0f;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.x, -2.22f);
}

/*
 * Tests a value greater than the boundary for integral predicted velocity signal. The predicted position shall be modified using
 * curce radius and the waypoint shall still be valid. \uts{CSCSA-42295} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__use_radius_for_calculation_negative_value)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->yawrate             = 0.0f;
   p_vehicle_data->host_length         = 4.5f;
   p_vehicle_data->host_width          = 2.7f;
   p_vehicle_data->host_speed          = EPSILON + 0.1f * EPSILON;
   p_vehicle_data->rear_axle_position  = -2.25f;
   p_vehicle_data->long_acc            = 1.0f;
   Ego_Traj_Instance.Host_Curve_Radius = -1.0f;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.x, -2.1902103f);
}

/*
 * Tests a value greater than the boundary for integral predicted velocity signal. The predicted position shall be modified and the
 * waypoint shall still be valid. \uts{CSCSA-42296} \sdd{SF-4225} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ego_Traj_Predictor_Test, Fbk_Get_Predicted_Ego_Waypoint__radius_over_max_negative_value)
{
   /** \arrange Set up vehicle properties. */
   uint8_t prediction_step = 1u;

   p_vehicle_data->yawrate             = 0.0f;
   p_vehicle_data->host_length         = 4.5f;
   p_vehicle_data->host_width          = 2.7f;
   p_vehicle_data->host_speed          = EPSILON + 0.1f * EPSILON;
   p_vehicle_data->rear_axle_position  = -2.25f;
   p_vehicle_data->long_acc            = 1.0f;
   Ego_Traj_Instance.Host_Curve_Radius = -301.0f;

   /** \action Predict ego waypoint */
   waypoint_with_circle_centers = Fbk_Get_Predicted_Ego_Waypoint(&Ego_Traj_Instance, p_vehicle_data, &fbk_ego_data, prediction_step);

   /** \assert Check if computed ego trajectory is valid and compare waypoint coordinates with expected results. */
   EXPECT_TRUE(waypoint_with_circle_centers.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers.waypoint_coordinates.x, -2.1500001f);
}
