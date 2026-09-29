/**
 * @file fbk_obj_traj_predictor_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42241}
 */

#include "fbk_obj_traj_predictor_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_obj_traj_predictor.c"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
}


/**
 * Tests the object trajectory prediction. The predicted trajectory shall be valid.
 * \uts{CSCSA-42456} \sdd{SF-4241} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, Fbk_Predict_Obj_Trajectory__obj_traj_valid)
{
   /** \arrange Create a target and reset its trajectory. */
   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = -2.0f;
   tracker_data.vcs_vel.y   = -2.0f;
   tracker_data.vcs_accel.x = 1.0f;
   tracker_data.vcs_accel.y = 1.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 20;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;

   /** \action Predict target trajectory */
   Fbk_Predict_Obj_Trajectory(&trajectory, &tracker_data, &fbk_obj_data);

   /** \assert Check if trajectory and waypoints are valid. */
   EXPECT_TRUE(trajectory.f_trajectory_valid);
   EXPECT_TRUE(trajectory.waypoint[1].f_waypoint_valid);
}

/**
 * Tests the object trajectory prediction with an object which comes to standstill. The waypoints shall be equal when the object
 * would be predicted backwards. This shall happen starting from the 10-th point. \uts{CSCSA-42457} \sdd{}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, FbkPredict_Obj_Trajectory__use_last_valid_waypoint)
{
   /** \arrange Create a target and reset its trajectory. */
   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = -2.0f;
   tracker_data.vcs_vel.y   = -2.0f;
   tracker_data.vcs_accel.x = 2.0f;
   tracker_data.vcs_accel.y = 2.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 20;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;

   /** \action Predict target trajectory */
   Fbk_Predict_Obj_Trajectory(&trajectory, &tracker_data, &fbk_obj_data);

   /** \assert Check if trajectory/waypoints are valid and that the waypoint coordinates equal after the target comes to a hold */
   EXPECT_TRUE(trajectory.f_trajectory_valid);
   EXPECT_EQ(trajectory.waypoint[10].waypoint_coordinates.x, trajectory.waypoint[11].waypoint_coordinates.x);
   EXPECT_EQ(trajectory.waypoint[10].waypoint_coordinates.y, trajectory.waypoint[11].waypoint_coordinates.y);
}

/**
 * Tests the object trajectory prediction without any prediction iteration the object trajectory shall be invalid.
 * \uts{CSCSA-42458} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, FbkPredict_Obj_Trajectory__obj_traj_invalid)
{
   /** \arrange Create a target and reset its trajectory. */
   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = -2.0f;
   tracker_data.vcs_vel.y   = -2.0f;
   tracker_data.vcs_accel.x = 1.0f;
   tracker_data.vcs_accel.y = 1.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 0;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;

   /** \action Predict target trajectory */
   Fbk_Predict_Obj_Trajectory(&trajectory, &tracker_data, &fbk_obj_data);

   /** \assert Check if trajectory is invalid */
   EXPECT_FALSE(trajectory.f_trajectory_valid);
}

/**
 * Predict object waypoint and compare it with a manually calculated waypoint position.
 * \uts{CSCSA-42459} \sdd{SF-4238} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, Fbk_Get_Predicted_Object_Waypoint__test_calculation_of_waypoint_structure)
{
   /** \arrange Create a target. */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure;
   uint8_t prediction_step = 0;

   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = -2.0f;
   tracker_data.vcs_vel.y   = -2.0f;
   tracker_data.vcs_accel.x = 1.0f;
   tracker_data.vcs_accel.y = 1.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 1;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;

   /** \action Get predicted waypoint */
   waypoint_with_circle_centers_structure = Fbk_Get_Predicted_Object_Waypoint(&tracker_data, &fbk_obj_data, prediction_step);

   /** \assert Check if computed waypoint equals the manually calculated waypoint. */
   EXPECT_TRUE(waypoint_with_circle_centers_structure.f_waypoint_valid);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.circle_radius, 1.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.waypoint_coordinates.x, 2.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.waypoint_coordinates.y, 2.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.circle_center_front.x, 2.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.circle_center_front.y, 2.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.circle_center_middle.x, 2.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.circle_center_middle.y, 2.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.circle_center_rear.x, 2.0f);
   EXPECT_FLOAT_EQ(waypoint_with_circle_centers_structure.circle_center_rear.y, 2.0f);
}


/**
 * Tests the object object waypoint prediction with an object which is reversing right from the start of the prediction iterations.
 * The predicted object trajectory shall be invalid. \uts{CSCSA-42460} \sdd{SF-4238} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, Fbk_Get_Predicted_Object_Waypoint__waypoint_invalid_obj_reverses_direction)
{
   /** \arrange Create a reversing target. */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure;
   uint8_t prediction_step = 1;

   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = 1.0f;
   tracker_data.vcs_vel.y   = -1.0f;
   tracker_data.vcs_accel.x = -11.0f;
   tracker_data.vcs_accel.y = 11.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 2;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;
   fbk_obj_data.obj_pred_speed_min           = 0.7f;

   /** \action Get predicted waypoint */
   waypoint_with_circle_centers_structure = Fbk_Get_Predicted_Object_Waypoint(&tracker_data, &fbk_obj_data, prediction_step);

   /** \assert Check if computed waypoint is invalid. */
   EXPECT_FALSE(waypoint_with_circle_centers_structure.f_waypoint_valid);
}


/**
 * Tests the object object waypoint prediction with an object which is reversing right from the start but is fast enough. in that
 * prediction step The predicted object trajectory shall be valid. \uts{CSCSA-42461} \sdd{SF-4238} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, Fbk_Get_Predicted_Object_Waypoint__waypoint_invalid_obj_reverses_one_direction_but_still_fast_enough)
{
   /** \arrange Create a target. */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure;
   uint8_t prediction_step = 1;

   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = 1.0f;
   tracker_data.vcs_vel.y   = -1.0f;
   tracker_data.vcs_accel.x = 0.0f;
   tracker_data.vcs_accel.y = 11.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 2;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;
   fbk_obj_data.obj_pred_speed_min           = 0.7f;

   /** \action Get predicted waypoint */
   waypoint_with_circle_centers_structure = Fbk_Get_Predicted_Object_Waypoint(&tracker_data, &fbk_obj_data, prediction_step);

   /** \assert Check if computed waypoint is valid. */
   EXPECT_TRUE(waypoint_with_circle_centers_structure.f_waypoint_valid);
}


/*
 * Tests the exact boundary for missing movement across longitudinal direction. No acceleration is occuring in this testcase
 * causing the predicted velocity to be equal to the measured velocity. The waypoint shall be valid. \uts{CSCSA-42462}
 * \sdd{SF-4238} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, Fbk_Get_Predicted_Object_Waypoint__exact_boundary_test_object_stand_still_in_long_direction)
{
   /** \arrange Create a target. */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure;
   uint8_t prediction_step = 1;

   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = 0.0f;
   tracker_data.vcs_vel.y   = 1.0f;
   tracker_data.vcs_accel.x = 0.0f;
   tracker_data.vcs_accel.y = 0.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 2;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;
   fbk_obj_data.obj_pred_speed_min           = 0.7f;

   /** \action Get predicted waypoint */
   waypoint_with_circle_centers_structure = Fbk_Get_Predicted_Object_Waypoint(&tracker_data, &fbk_obj_data, prediction_step);

   /** \assert Check if computed waypoint is valid. */
   EXPECT_TRUE(waypoint_with_circle_centers_structure.f_waypoint_valid);
}

/*
 * Tests the exact boundary for missing movement across lateral direction. No acceleration is occuring in this testcase causing the
 * predicted velocity to be equal to the measured velocity. The waypoint shall be valid. \uts{CSCSA-42463} \sdd{SF-4238}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test, Fbk_Get_Predicted_Object_Waypoint__exact_boundary_test_object_stand_still_in_lat_direction)
{
   /** \arrange Create a target. */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure;
   uint8_t prediction_step = 1;

   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = 1.0f;
   tracker_data.vcs_vel.y   = 0.0f;
   tracker_data.vcs_accel.x = 0.0f;
   tracker_data.vcs_accel.y = 0.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 2;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;
   fbk_obj_data.obj_pred_speed_min           = 0.7f;

   /** \action Get predicted waypoint */
   waypoint_with_circle_centers_structure = Fbk_Get_Predicted_Object_Waypoint(&tracker_data, &fbk_obj_data, prediction_step);

   /** \assert Check if computed waypoint is valid. */
   EXPECT_TRUE(waypoint_with_circle_centers_structure.f_waypoint_valid);
}


/*
 * Tests the exact boundary for missing movement across lateral direction. The waypoint shall still be invalid, since the sign of
 * the waypoint is changing. \uts{CSCSA-42464} \sdd{SF-4238} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test,
       Fbk_Get_Predicted_Object_Waypoint__lt_boundary_test_object_stand_still_in_lat_directions_sign_changes)
{
   /** \arrange Create a target. */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure;
   uint8_t prediction_step = 1;

   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = -EPSILON;
   tracker_data.vcs_vel.y   = 0.0f;
   tracker_data.vcs_accel.x = 1.0f;
   tracker_data.vcs_accel.y = 0.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 2;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;
   fbk_obj_data.obj_pred_speed_min           = 0.7f;

   /** \action Get predicted waypoint */
   waypoint_with_circle_centers_structure = Fbk_Get_Predicted_Object_Waypoint(&tracker_data, &fbk_obj_data, prediction_step);

   /** \assert Check if computed waypoint is invalid. */
   EXPECT_FALSE(waypoint_with_circle_centers_structure.f_waypoint_valid);
}

/*
 * Tests the exact boundary for missing movement across longitudinal direction. The waypoint shall still be invalid, since it is
 * changing its sign. \uts{CSCSA-42465} \sdd{SF-4238} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Traj_Predictor_Test,
       Fbk_Get_Predicted_Object_Waypoint__exact_boundary_test_object_stand_still_in_long_directions_sign_changes)
{
   /** \arrange Create a target. */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure;
   uint8_t prediction_step = 1;

   tracker_data.vcs_pos.x   = 2.0f;
   tracker_data.vcs_pos.y   = 2.0f;
   tracker_data.vcs_vel.x   = -EPSILON;
   tracker_data.vcs_vel.y   = 0.0f;
   tracker_data.vcs_accel.x = 1.0f;
   tracker_data.vcs_accel.y = 0.0f;
   tracker_data.vcs_heading = PI / 2.0f;
   tracker_data.length      = 2.0f;
   tracker_data.width       = 2.0f;

   fbk_obj_data.pred_step_dt = 0.1f;

   trajectory.n_prediction_steps             = 2;
   fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
   fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;
   fbk_obj_data.obj_pred_speed_min           = 0.7f;


   /** \action Get predicted waypoint */
   waypoint_with_circle_centers_structure = Fbk_Get_Predicted_Object_Waypoint(&tracker_data, &fbk_obj_data, prediction_step);

   /** \assert Check if computed waypoint is invalid. */
   EXPECT_FALSE(waypoint_with_circle_centers_structure.f_waypoint_valid);
}
