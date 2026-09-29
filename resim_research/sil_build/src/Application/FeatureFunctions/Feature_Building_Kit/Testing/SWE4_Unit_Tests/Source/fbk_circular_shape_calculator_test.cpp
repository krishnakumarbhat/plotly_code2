/**
 * @file fbk_circular_shape_calculator_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42221}
 */

#include "fbk_circular_shape_calculator_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <float.h>
#include <math.h>
}

/**
 * Calculate waypoint coordinates for a zero yaw angle and compare results with manually computed values.
 * \uts{CSCSA-42262} \sdd{SF-4248} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Fill_Circle_Center_Coordinates__circle_center_filling_yaw_angle_zero)
{
   /** \arrange Create waypoint coordinates and set up offset relative to pivot. */
   float32_T offset_x_front  = 2.0f;
   float32_T offset_x_middle = 3.0f;
   float32_T offset_x_rear   = 1.0f;
   float32_T psi             = 0.0f;

   waypoint_res.waypoint_coordinates.x = 1.5f;
   waypoint_res.waypoint_coordinates.y = 2.5f;
   waypoint_res.waypoint_yaw_angle     = Create_Angle(psi);

   /** \action Calculate circle coordinates. */
   Fbk_Fill_Circle_Center_Coordinates(&waypoint_res, offset_x_front, offset_x_middle, offset_x_rear);

   /** \assert Compare computed coordinates with expected results. */
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.x, 3.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.y, 2.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.x, 4.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.y, 2.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.x, 2.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.y, 2.5f);
}

/**
 * Calculate waypoint coordinates for yaw angle within 0 to 2PI and compare results with manually computed values.
 * \uts{CSCSA-42263} \sdd{SF-4248} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Fill_Circle_Center_Coordinates__test_correct_coordinate_setting)
{
   /** \arrange Create waypoint coordinates and set up offset relative to pivot. */
   float32_T offset_x_front  = 2.0f;
   float32_T offset_x_middle = 3.0f;
   float32_T offset_x_rear   = 1.0f;
   float32_T psi             = 1.5f * PI;

   waypoint_res.waypoint_coordinates.x = 1.5f;
   waypoint_res.waypoint_coordinates.y = 2.5f;
   waypoint_res.waypoint_yaw_angle     = Create_Angle(psi);

   /** \action Calculate circle coordinates. */
   Fbk_Fill_Circle_Center_Coordinates(&waypoint_res, offset_x_front, offset_x_middle, offset_x_rear);

   /** \assert Compare computed coordinates with expected results. */
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.x, 1.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.y, 0.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.x, 1.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.y, -0.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.x, 1.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.y, 1.5f);
}

/**
 * Calculate waypoint coordinates for yaw angle outside 0 to 2PI and compare results with manually computed values.
 * \uts{CSCSA-42264} \sdd{SF-4248} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Fill_Circle_Center_Coordinates__circle_center_filling_yaw_angle_nonzero)
{
   /** \arrange Create waypoint coordinates and set up offset relative to pivot. */
   float32_T offset_x_front  = 1.0f;
   float32_T offset_x_middle = 2.0f;
   float32_T offset_x_rear   = 0.0f;
   float32_T psi             = 3.5f * PI;

   waypoint_res.waypoint_coordinates.x = 1.5f;
   waypoint_res.waypoint_coordinates.y = 2.5f;
   waypoint_res.waypoint_yaw_angle     = Create_Angle(psi);

   /** \action Calculate circle coordinates. */
   Fbk_Fill_Circle_Center_Coordinates(&waypoint_res, offset_x_front, offset_x_middle, offset_x_rear);

   /** \assert Compare computed coordinates with expected results. */
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.x, 1.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.y, 1.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.x, 1.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.y, .5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.x, 1.5f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.y, 2.5f);
}

/*
 * Pass invalid yaw angle (max float value) and check that no fatal failure occurs.
 * \uts{CSCSA-42265} \sdd{SF-4248} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Fill_Circle_Center_Coordinates__max_boundary_test_angle)
{
   /** \arrange Create waypoint coordinates and set up offset relative to pivot. */
   float32_T offset_x_front  = 1.0f;
   float32_T offset_x_middle = 2.0f;
   float32_T offset_x_rear   = 0.0f;

   waypoint_res.waypoint_coordinates.x = 1.5f;
   waypoint_res.waypoint_coordinates.y = 2.5f;
   waypoint_res.waypoint_yaw_angle     = Create_Angle(FLT_MAX);

   /** \action See assert. */

   /** \assert Execute Fbk_Fill_Circle_Center_Coordinates function and check that no fatal failure occurs. */
   EXPECT_NO_FATAL_FAILURE(Fbk_Fill_Circle_Center_Coordinates(&waypoint_res, offset_x_front, offset_x_middle, offset_x_rear));
}

/*
 * Test the lower boundary where a grow gain is applied to the radius. Here a grow gain shall be applied to the radius, since the
 * exact boundary is inclusive in the condition. \uts{CSCSA-42266} \sdd{SF-4249} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Get_Circle_Radius__exact_min_valid_boundary_test_grow_gain)
{
   /** \arrange Setup grow rate for a circle with a given radius. */
   float32_T base_radius       = 1.5f;
   float32_T base_gain         = 1.0f;
   float32_T grow_gain         = 1.0f - EPSILON;
   float32_T calculated_radius = 0.0f;
   uint8_t prediction_step     = 2;

   float32_T expected_res = base_radius * base_gain * powf(grow_gain, prediction_step);

   /** \action Calculate circle radius. */
   calculated_radius = Fbk_Get_Circle_Radius(prediction_step, base_radius, base_gain, grow_gain);

   /** \assert Compare computed radius with manually calculated result. */
   EXPECT_FLOAT_EQ(calculated_radius, expected_res);
}

/*
 * Tests the grow condition with a value outside of the appliance range for a grow gain. Here a grow gain shall not be applied to
 * the radius, since the boundary condition is not fulfilled. \uts{CSCSA-42267} \sdd{SF-4249} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Get_Circle_Radius__min_invalid_boundary_test_grow_gain)
{
   /** \arrange Setup grow rate for a circle with a given radius. */
   float32_T base_radius       = 1.5f;
   float32_T base_gain         = 1.0f;
   float32_T grow_gain         = 1.0f;
   float32_T calculated_radius = 0.0f;
   uint8_t prediction_step     = 2;

   float32_T expected_res = base_radius * base_gain;

   /** \action Calculate circle radius. */
   calculated_radius = Fbk_Get_Circle_Radius(prediction_step, base_radius, base_gain, grow_gain);

   /** \assert Compare computed radius with manually calculated result. */
   EXPECT_FLOAT_EQ(calculated_radius, expected_res);
}

/*
 * Tests the grow condition with a value exactly on the upper boundary of the grow gain condition. Here a grow gain shall be
 * applied to the radius, since the exact boundary is inclusive in the condition. \uts{CSCSA-42268} \sdd{SF-4249}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Get_Circle_Radius__max_valid_boundary_test_grow_gain)
{
   /** \arrange Setup grow rate for a circle with a given radius. */
   float32_T base_radius       = 1.5f;
   float32_T base_gain         = 1.0f;
   float32_T grow_gain         = 1.0f + EPSILON;
   float32_T calculated_radius = 0.0f;
   uint8_t prediction_step     = 2;

   float32_T expected_res = base_radius * base_gain * powf(grow_gain, prediction_step);

   /** \action Calculate circle radius. */
   calculated_radius = Fbk_Get_Circle_Radius(prediction_step, base_radius, base_gain, grow_gain);

   /** \assert Compare computed radius with manually calculated result. */
   EXPECT_FLOAT_EQ(calculated_radius, expected_res);
}

/*
 * Tests the grow condition with a value outside of the appliance range for a grow gain. Here a grow gain shall not be applied to
 * the radius, since the boundary condition is not fulfilled. \uts{CSCSA-42269} \sdd{SF-4249} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Get_Circle_Radius__max_invalid_boundary_test_gain_angle)
{
   /** \arrange Setup grow rate for a circle with a given radius. */
   float32_T base_radius       = 1.5f;
   float32_T base_gain         = 1.0f;
   float32_T grow_gain         = 1.0f;
   float32_T calculated_radius = 0.0f;
   uint8_t prediction_step     = 2;

   float32_T expected_res = base_radius * base_gain;

   /** \action Calculate circle radius. */
   calculated_radius = Fbk_Get_Circle_Radius(prediction_step, base_radius, base_gain, grow_gain);

   /** \assert Compare computed radius with manually calculated result. */
   EXPECT_FLOAT_EQ(calculated_radius, expected_res);
}

/*
 * Tests the grow condition with a value slightly above 1.0f. Apply the grow gain and verify that the result matches the manually
 * calculated result. \uts{CSCSA-42270} \sdd{SF-4249} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Get_Circle_Radius__test_correct_circle_radius)
{
   /** \arrange Setup grow rate for a circle with a given radius. */
   float32_T base_radius       = 1.5f;
   float32_T base_gain         = 1.0f;
   float32_T grow_gain         = 1.001f;
   uint8_t prediction_step     = 2;
   float32_T calculated_radius = 0.0f;

   /** \action Calculate circle radius. */
   calculated_radius = Fbk_Get_Circle_Radius(prediction_step, base_radius, base_gain, grow_gain);

   /** \assert Compare computed radius with manually calculated result. */
   EXPECT_FLOAT_EQ(calculated_radius, (1.5f * pow(1.001f, 2)));
}

/*
 * Tests the circle radius calculation function. Input invalid data and check function behavior.
 * \uts{CSCSA-42271} \sdd{SF-4249} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Get_Circle_Radius__check_behavior_for_invalid_input)
{
   /** \arrange Setup invalid input. */
   float32_T base_radius   = 0.0f;
   float32_T base_gain     = 0.0f;
   float32_T grow_gain     = 0.0f;
   uint8_t prediction_step = 0;

   /** \action Calculate circle radius. */
   float32_T calculated_radius = Fbk_Get_Circle_Radius(prediction_step, base_radius, base_gain, grow_gain);

   /** \assert Compare computed radius with expected result. */
   EXPECT_FLOAT_EQ(calculated_radius, 0.0f);
}

/*
 * Compute the ego circle center offsets for a vehicle with a given width and length. The computed result is compared with a
 * manually calculated expected result. \uts{CSCSA-42272} \sdd{SF-4244} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Calculate_Ego_Circle_Center_Offsets__test_calculated_ego_circle_center_offsets)
{
   /** \arrange Setup vehicle with a given width and length. */
   float32_T obj_length = 4.5f;
   float32_T obj_width  = 1.8f;
   Fbk_Circle_Center_Offset_T circle_center_offset;

   /** \action Calculate the circle center offset for given width and length. */
   circle_center_offset = Fbk_Calculate_Ego_Circle_Center_Offsets(obj_length, obj_width, &fbk_ego_data);

   /** \assert Compare computed offset with manually calculated result. */
   EXPECT_FLOAT_EQ(circle_center_offset.offset_front_x, -0.9f);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_middle_x, -2.25f);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_rear_x, -3.6f);
}

/*
 * Compute the ego circle center offsets for a vehicle with a given width and length. Here the additional factors are also applied.
 * \uts{CSCSA-42273} \sdd{SF-4244} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test,
       Fbk_Calculate_Ego_Circle_Center_Offsets__test_calculated_ego_circle_center_offsets_with_factors_setup)
{
   /** \arrange Setup vehicle with a given width and length. */
   float32_T obj_length = 4.5f;
   float32_T obj_width  = 1.8f;
   Fbk_Circle_Center_Offset_T circle_center_offset;
   fbk_ego_data.ego_circle_offset             = 0.8f;
   fbk_ego_data.ego_circle_host_length_factor = 1.0f;

   /** \action Calculate the circle center offset for given width and length. */
   circle_center_offset = Fbk_Calculate_Ego_Circle_Center_Offsets(obj_length, obj_width, &fbk_ego_data);

   /** \assert Compare computed offset with manually calculated result. */
   EXPECT_FLOAT_EQ(circle_center_offset.offset_front_x, -0.9f + fbk_ego_data.ego_circle_offset);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_middle_x, -2.25f + fbk_ego_data.ego_circle_offset);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_rear_x, -3.6f + fbk_ego_data.ego_circle_offset);
}

/*
 * Compute the ego circle center offsets for a vehicle with a given width and length. Here invalid data is provided to this
 * function. \uts{CSCSA-42274} \sdd{SF-4244} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Calculate_Ego_Circle_Center_Offsets__test_invalid_input_data)
{
   /** \arrange Setup invalid data as function input. */
   float32_T obj_length = 0.0f;
   float32_T obj_width  = 0.0f;
   Fbk_Circle_Center_Offset_T circle_center_offset;
   fbk_ego_data.ego_circle_offset             = 0.0f;
   fbk_ego_data.ego_circle_host_length_factor = 0.0f;

   /** \action Calculate the circle center offset for given data. */
   circle_center_offset = Fbk_Calculate_Ego_Circle_Center_Offsets(obj_length, obj_width, &fbk_ego_data);

   /** \assert Compare computed offset with manually calculated result. */
   EXPECT_FLOAT_EQ(circle_center_offset.offset_front_x, 0.0f);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_middle_x, 0.0f);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_rear_x, 0.0f);
}


/*
 * Compute the object circle center offsets for a vehicle with a given width and length. The computed result is compared with a
 * manually calculated expected result. \uts{CSCSA-42275} \sdd{SF-4247} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Calculate_Obj_Circle_Center_Offsets__test_calculated_object_circle_center_offsets)
{
   /** \arrange Setup vehicle with a given widht and length. */
   float32_T obj_length = 4.5f;
   float32_T obj_width  = 1.8f;
   Fbk_Circle_Center_Offset_T circle_center_offset;

   /** \action Calculate the circle center offset for given width and length. */
   circle_center_offset = Fbk_Calculate_Obj_Circle_Center_Offsets(obj_length, obj_width);

   /** \assert Compare computed offset with manually calculated result. */
   EXPECT_FLOAT_EQ(circle_center_offset.offset_front_x, 1.35f);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_middle_x, -0.0f);
   EXPECT_FLOAT_EQ(circle_center_offset.offset_rear_x, -1.35f);
}

/*
 * Tests the constructor for the type Ta_Circle_Center_Offset_T. The attributes of circle center offset variable shall be filled
 * with the given input values of 0.0f. \uts{CSCSA-42276} \sdd{SF-4251} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Init_Circle_Center_Offset_Structure__Initialize)
{
   /** \arrange See action. */

   /** \action Initialize persistent circle center offset struct */
   Fbk_Init_Circle_Center_Offset_Structure(&circle_center_offset_structure);

   /** \assert Check that offset values in circle center offset struct are default values. */
   EXPECT_FLOAT_EQ(circle_center_offset_structure.offset_front_x, 0.0f);
   EXPECT_FLOAT_EQ(circle_center_offset_structure.offset_middle_x, 0.0f);
   EXPECT_FLOAT_EQ(circle_center_offset_structure.offset_rear_x, 0.0f);
}

/*
 * Tests the constructor for the type Fbk_Waypoint_with_Circle_Centers_T. The attributes of waypoint with circle center variable
 * shall be filled with the given input values of 0.0f. \uts{CSCSA-42277} \sdd{SF-4250} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Circular_Shape_Calculator_Test, Fbk_Init_Waypoint_with_Circle_Centers_Structure__Initialize)
{
   /** \arrange See action. */

   /** \action Initialize persistent waypoint circle center struct */
   Fbk_Init_Waypoint_with_Circle_Centers_Structure(&waypoint_res);

   /** \assert Check that offset values in waypoint circle center offset are default values. */
   EXPECT_FLOAT_EQ(waypoint_res.circle_radius, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.waypoint_coordinates.x, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.waypoint_coordinates.y, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.x, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_front.y, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.x, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_middle.y, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.x, 0.0f);
   EXPECT_FLOAT_EQ(waypoint_res.circle_center_rear.y, 0.0f);
   EXPECT_FALSE(waypoint_res.f_waypoint_valid);
}
