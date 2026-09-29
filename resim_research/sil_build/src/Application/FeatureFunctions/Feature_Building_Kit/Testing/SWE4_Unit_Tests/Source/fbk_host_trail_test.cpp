/**
 * @file fbk_host_trail_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK host trail interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42229}
 */

#include "fbk_host_trail_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_core_calibration_t.h"
#include "fbk_host_trail.c"
#include "fbk_iface_types.h"
#include "fbk_macros.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
}


/**
 * Test the initialization of host trail via Main routine of path tracking.
 * \uts{CSCSA-42368} \sdd{SF-4165} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Init_Host_Trail__initialize_host_trail)
{
   /* \arrange Set some non default values to host trail. */
   host_trail.f_trail_full_buffer = FBK_TRUE;
   p_vehicle_data->host_speed     = 16.0f;

   /* \action Run Path_Algorithm. */
   Fbk_Init_Host_Trail(&host_trail);

   /* \assert Check if host trail is reset correctly. */
   EXPECT_FALSE(host_trail.f_trail_full_buffer);
}


/**
 * Tests whether the constructor for host trail sets the trails members to their defaults.
 * \uts{CSCSA-42369} \sdd{SF-4165} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Init_Host_Trail__set_host_trail_to_its_default)
{
   /** \arrange Set up an host trail with non default values. */
   host_trail.f_trail_full_buffer = FBK_TRUE;
   host_trail.trail_index         = 255u;
   host_trail.oldest_trail_index  = 255u;
   host_trail.trail_diff_dist     = 100.0f;
   host_trail.trail_diff_heading  = PI;
   host_trail.trail_host_position = Create_2d_Vector_X_Normal();
   host_trail.trail_host_dist     = 100.0f;
   host_trail.trail_host_heading  = Create_Angle(PI);

   for (uint8_t i = FBK_ZERO_UINT; i < FBK_NUM_HOST_TRAIL_POINTS; i++)
   {
      host_trail.segments[i].point               = Create_2d_Vector_X_Normal();
      host_trail.segments[i].distance_traveled   = 100.0f;
      host_trail.segments[i].dist_between_points = 100.0f;
      host_trail.segments[i].heading             = PI;
   }

   /** \action call host trail . */
   Fbk_Init_Host_Trail(&host_trail);

   /** \assert Check that correct defaults are set. */
   EXPECT_FALSE(host_trail.f_trail_full_buffer);
   EXPECT_EQ(host_trail.trail_index, FBK_ZERO_UINT);
   EXPECT_EQ(host_trail.oldest_trail_index, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_dist, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_heading, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(host_trail.trail_host_position.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(host_trail.trail_host_position.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(host_trail.trail_host_dist, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(host_trail.trail_host_heading.angle, FBK_ZERO_F);

   for (uint8_t i = FBK_ZERO_UINT; i < FBK_NUM_HOST_TRAIL_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(host_trail.segments[i].point.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(host_trail.segments[i].point.y, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(host_trail.segments[i].distance_traveled, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(host_trail.segments[i].dist_between_points, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(host_trail.segments[i].heading, FBK_ZERO_F);
   }
}


/**
 * Test main routine for host lane updates. Here the speed signal is exceeding a given speed threshold and thus the trail is reset.
 * \uts{CSCSA-42370} \sdd{SF-4166} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Update_Host_Trail__initialize_host_lane_structure)
{
   /** \arrange Set up non default values to host lane. */
   host_trail.f_trail_full_buffer                = FBK_TRUE;
   host_trail.f_was_trail_point_added_this_cycle = FBK_TRUE;
   p_vehicle_data->host_speed                    = 1.1f * fbk_cals.k_fbk_host_trail_max_recording_speed;

   /** \action call update functionality. */
   Fbk_Update_Host_Trail(&host_trail, &data, &fbk_cals);

   /** \assert Expect member to be reset to its default. */
   EXPECT_FALSE(host_trail.f_was_trail_point_added_this_cycle);
}


/**
 * Test main routine for host lane updates. Here the the trail is empty and thus a point shall be added.
 * \uts{CSCSA-42371} \sdd{SF-4166} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Update_Host_Trail__add_point_to_empty_trail)
{
   /** \arrange Set up default values to host lane. */
   host_trail.f_trail_full_buffer = FBK_FALSE;
   host_trail.trail_index         = 0u;
   p_vehicle_data->host_speed     = 0.9f * fbk_cals.k_fbk_host_trail_max_recording_speed;

   /** \action call update functionality. */
   Fbk_Update_Host_Trail(&host_trail, &data, &fbk_cals);

   /** \assert Expect trail index to be increase. */
   EXPECT_EQ(host_trail.trail_index, 1u);
}


/**
 * Test main routine for host lane updates. Here the the trail is empty and thus a point shall be added.
 * \uts{CSCSA-42372} \sdd{SF-4166} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Update_Host_Trail__host_lane_shall_not_be_extended_by_additional_points)
{
   /** \arrange Set up some values to the host lane. */
   host_trail.f_trail_full_buffer = FBK_FALSE;
   host_trail.trail_index         = 2u;
   p_vehicle_data->host_speed     = 0.9f * fbk_cals.k_fbk_host_trail_max_recording_speed;
   data.time_diff_to_last_cycle   = 0.05f;

   /** \action call update functionality. */
   Fbk_Update_Host_Trail(&host_trail, &data, &fbk_cals);

   /** \assert Expect trail index to remain. */
   EXPECT_EQ(host_trail.trail_index, 2u);
}


/**
 * Tests whether the correct point indices for looping over the trail points are returned. Here no overflow of the host trail is
 * expected. \uts{CSCSA-42373} \sdd{SF-4169} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Get_Point_Indices__return_point_interval_without_overflow)
{
   /** \arrange Set up a specific counter and interval. */
   Fbk_Point_Pair_T index_pair;
   Fbk_Host_Lane_Interval_T interval;
   uint8_t trail_counter = 2u;
   interval.interval_end = 15u;

   /** \action call point indice. */
   Fbk_Get_Point_Indices(&index_pair, &interval, trail_counter);

   /** \assert Check that a correct index pair is set. */
   EXPECT_EQ(index_pair.passed, 16u);
   EXPECT_EQ(index_pair.next, 17u);
}

/**
 * Tests whether the correct point indices for looping over the trail points are returned. Here an overflow of the host trail is
 * expected and thus the returned index shall be less than the input index. \uts{CSCSA-42374} \sdd{SF-4169}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Get_Point_Indices__return_point_interval_with_overflow)
{
   /** \arrange Set up a specific counter and interval. */
   Fbk_Point_Pair_T index_pair;
   Fbk_Host_Lane_Interval_T interval;
   uint8_t trail_counter = 10u;
   interval.interval_end = 15u;

   /** \action call point indice. */
   Fbk_Get_Point_Indices(&index_pair, &interval, trail_counter);

   /** \assert Check that a correct index pair is set. */
   EXPECT_EQ(index_pair.passed, 4u);
   EXPECT_EQ(index_pair.next, 5u);
}


/**
 * Tests whether the input vcs trail is transformed correctly. Here a buffer which is not filled completly shall be transformed.
 * Thus parts of the vcs trail remain empty. \uts{CSCSA-42375} \sdd{SF-4168} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Init_Trail_Vcs__check_whether_non_full_buffer_is_transformed_correctly)
{
   /** \arrange Set up a specific counter and interval. */
   Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS]{};

   host_trail.f_trail_full_buffer     = FBK_FALSE;
   host_trail.trail_host_position     = Create_2d_Vector_Origin();
   host_trail.trail_host_heading      = Create_Angle(PI);
   p_vehicle_data->rear_axle_position = FBK_ZERO_F;

   for (uint8_t i = 0; i < FBK_NUM_HOST_TRAIL_POINTS; i++)
   {
      host_trail.segments[i].point.x = FBK_ONE_F;
      host_trail.segments[i].point.y = FBK_ZERO_F;
   }
   host_trail.oldest_trail_index = FBK_ZERO_UINT;
   host_trail.trail_index        = 10u;

   /** \action call trail transformation. */
   Fbk_Init_Trail_Vcs(trail_vcs, &host_trail, p_vehicle_data->rear_axle_position);

   /** \assert Check that a correct index pair is set. */
   for (uint8_t i = 0; i < FBK_NUM_HOST_TRAIL_POINTS; i++)
   {
      if (i >= host_trail.oldest_trail_index && i < host_trail.trail_index)
      {
         EXPECT_FLOAT_EQ(trail_vcs[i].x, -FBK_ONE_F);
         EXPECT_NEAR(trail_vcs[i].y, FBK_ZERO_F, 0.00001f);
      }
      else
      {
         EXPECT_FLOAT_EQ(trail_vcs[i].x, FBK_ZERO_F);
         EXPECT_FLOAT_EQ(trail_vcs[i].y, FBK_ZERO_F);
      }
   }
}


/**
 * Tests whether the input vcs trail is transformed correctly. Here a buffer which is full shall be transformed.
 * \uts{CSCSA-42376} \sdd{SF-4168} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Init_Trail_Vcs__check_whether_full_buffer_is_transformed_correctly)
{
   /** \arrange Set up a specific counter and interval. */
   Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS]{};

   host_trail.f_trail_full_buffer     = FBK_TRUE;
   host_trail.trail_host_position     = Create_2d_Vector_Origin();
   host_trail.trail_host_heading      = Create_Angle(PI);
   p_vehicle_data->rear_axle_position = FBK_ZERO_F;

   for (uint8_t i = 0; i < FBK_NUM_HOST_TRAIL_POINTS; i++)
   {
      host_trail.segments[i].point.x = FBK_ONE_F;
      host_trail.segments[i].point.y = FBK_ZERO_F;
   }

   /** \action call trail transformation. */
   Fbk_Init_Trail_Vcs(trail_vcs, &host_trail, p_vehicle_data->rear_axle_position);

   /** \assert Check that a correct index pair is set. */
   for (uint8_t i = 0; i < FBK_NUM_HOST_TRAIL_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(trail_vcs[i].x, -FBK_ONE_F);
      EXPECT_NEAR(trail_vcs[i].y, FBK_ZERO_F, 0.00001f);
   }
}


/**
 * Tests whether points shall be added to a host trail. Here a point shall be added due to the heading condition thus true is
 * expected. \uts{CSCSA-42377} \sdd{SF-4163} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Shall_Point_Be_Added_To_Trail__point_shall_be_added_to_trail_due_to_heading_condition)
{
   /** \arrange Set up conditions such that a point shall be added. */
   boolean_T res;

   host_trail.trail_diff_dist     = fbk_cals.k_fbk_host_trail_dist_separation - 0.1f;
   host_trail.trail_diff_heading  = 1.1f * fbk_cals.k_fbk_host_trail_heading_separation;
   host_trail.trail_index         = 1u;
   host_trail.f_trail_full_buffer = FBK_TRUE;

   /** \action call routine to check whether a point shall be added to the host trail. */
   res = Fbk_Shall_Point_Be_Added_To_Trail(&host_trail, &fbk_cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests whether points shall be added to a host trail. Here a point shall be added since the buffer is empty.
 * \uts{CSCSA-42378} \sdd{SF-4163} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Shall_Point_Be_Added_To_Trail__point_shall_be_added_to_trail_due_to_empty_buffer)
{
   /** \arrange Set up conditions such that a point shall be added. */
   boolean_T res;

   host_trail.trail_diff_dist     = fbk_cals.k_fbk_host_trail_dist_separation - 0.1f;
   host_trail.trail_diff_heading  = 0.9f * fbk_cals.k_fbk_host_trail_heading_separation;
   host_trail.trail_index         = 0u;
   host_trail.f_trail_full_buffer = FBK_FALSE;
   /** \action call routine to check whether a point shall be added to the host trail. */
   res = Fbk_Shall_Point_Be_Added_To_Trail(&host_trail, &fbk_cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests whether points shall be added to a host trail. Here a point shall not be added.
 * \uts{CSCSA-42379} \sdd{SF-4163} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Shall_Point_Be_Added_To_Trail__point_shall_not_be_added)
{
   /** \arrange Set up conditions such that a point shall not be added. */
   boolean_T res;

   host_trail.trail_diff_dist     = fbk_cals.k_fbk_host_trail_dist_separation - 0.1f;
   host_trail.trail_diff_heading  = 0.9f * fbk_cals.k_fbk_host_trail_heading_separation;
   host_trail.trail_index         = 1u;
   host_trail.f_trail_full_buffer = FBK_TRUE;
   /** \action call routine to check whether a point shall be added to the host trail. */
   res = Fbk_Shall_Point_Be_Added_To_Trail(&host_trail, &fbk_cals);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}

/**
 * Tests whether points shall be added to a host trail. Here a point shall be added thus true is expected.
 * \uts{CSCSA-42380} \sdd{SF-4163} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Shall_Point_Be_Added_To_Trail__point_shall_be_added_to_trail_due_to_dist_condition)
{
   /** \arrange Set up conditions such that a point shall be added. */
   boolean_T res;

   const float32_T host_trail_dist_separation    = 5.0f;
   const float32_T host_trail_heading_separation = 0.2617f;

   host_trail.trail_diff_dist     = 1.1f * host_trail_dist_separation;
   host_trail.trail_diff_heading  = 0.9f * host_trail_heading_separation;
   host_trail.trail_index         = 1u;
   host_trail.f_trail_full_buffer = FBK_TRUE;
   /** \action call routine to check whether a point shall be added to the host trail. */
   res = Fbk_Shall_Point_Be_Added_To_Trail(&host_trail, &fbk_cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Check world coordinate system calculation. Here the yawrate is big enough, thus division with zero will not be catched.
 * \uts{CSCSA-42381} \sdd{SF-4164} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Get_Wcs_Shift__yawrate_is_big_enough_but_speed_is_zero)
{
   /** \arrange Set up speed to zero. */
   Angle_T yaw_shift = Create_Angle(1.1f * EPSILON);
   Vector_2d_T vec;
   p_vehicle_data->yawrate    = EPSILON;
   p_vehicle_data->host_speed = FBK_ZERO_F;

   /** \action call routine to get a wcs shift. */
   vec = Fbk_Get_Wcs_Shift(&data, &yaw_shift);

   /** \assert Expect shift of zero, because we are in standstill. */
   EXPECT_FLOAT_EQ(vec.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(vec.y, FBK_ZERO_F);
}


/**
 * Check world coordinate system calculation. Here the yaw shift is zero.
 * \uts{CSCSA-42382} \sdd{SF-4164} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Get_Wcs_Shift__yaw_shift_is_zero)
{
   /** \arrange Set yaw shift to zero. */
   Angle_T yaw_shift = Create_Angle(0.0f);
   Vector_2d_T vec;
   data.time_diff_to_last_cycle = 0.05f;
   p_vehicle_data->host_speed   = 100.0f;

   /** \action call routine to get a wcs shift. */
   vec = Fbk_Get_Wcs_Shift(&data, &yaw_shift);

   /** \assert Expect shift in one direction. */
   EXPECT_FLOAT_EQ(vec.x, 5.0f);
   EXPECT_NEAR(vec.y, FBK_ZERO_F, EPSILON);
}

/**
 * Check whether host trailer point is correctly added.
 * \uts{CSCSA-42383} \sdd{SF-4170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Add_Host_Trailer_Point__add_point_to_non_full_buffer)
{
   /** \arrange Set up some properties of the trail. */
   host_trail.trail_index                                 = 5u;
   host_trail.trail_host_position                         = Create_2d_Vector_Coordinates(10.0f, 0.0f);
   host_trail.segments[host_trail.trail_index - 1u].point = Create_2d_Vector_Coordinates(5.0f, 0.0f);
   host_trail.trail_host_dist                             = 10.0f;
   host_trail.trail_host_heading                          = Create_Angle(0.0f);

   /** \action call point adding function. */
   Fbk_Add_Host_Trailer_Point(&host_trail);

   /** \assert Expect properties of the trail set correctly. */
   EXPECT_FLOAT_EQ(host_trail.segments[5u].point.x, 10.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[5u].point.y, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[5u].distance_traveled, 10.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[5u].dist_between_points, 5.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[5u].heading, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_dist, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_heading, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_index, 6u);
   EXPECT_FLOAT_EQ(host_trail.oldest_trail_index, 0u);
}

/**
 * Check whether host trailer point is correctly added.
 * \uts{CSCSA-42384} \sdd{SF-4170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Add_Host_Trailer_Point__add_last_point_to_buffer)
{
   /** \arrange Set up some properties of the trail. */
   host_trail.trail_index                                 = 19u;
   host_trail.trail_host_position                         = Create_2d_Vector_Coordinates(100.0f, 0.0f);
   host_trail.segments[host_trail.trail_index - 1u].point = Create_2d_Vector_Coordinates(95.0f, 0.0f);
   host_trail.trail_host_dist                             = 100.0f;
   host_trail.trail_host_heading                          = Create_Angle(0.0f);

   /** \action call point adding function. */
   Fbk_Add_Host_Trailer_Point(&host_trail);

   /** \assert Expect properties of the trail set correctly. */
   EXPECT_FLOAT_EQ(host_trail.segments[19u].point.x, 100.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[19u].point.y, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[19u].distance_traveled, 100.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[19u].dist_between_points, 5.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[19u].heading, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_dist, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_heading, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_index, 0u);
   EXPECT_FLOAT_EQ(host_trail.oldest_trail_index, 0u);
   EXPECT_TRUE(host_trail.f_trail_full_buffer);
}


/**
 * Check whether host trailer point is correctly added.
 * \uts{CSCSA-42385} \sdd{SF-4170} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Add_Host_Trailer_Point__trail_buffer_is_full_and_first_point_is_overridden)
{
   /** \arrange Set up some properties of the trail. */
   host_trail.trail_index         = 0u;
   host_trail.trail_host_position = Create_2d_Vector_Coordinates(100.0f, 0.0f);
   host_trail.segments[19u].point = Create_2d_Vector_Coordinates(95.0f, 0.0f);
   host_trail.trail_host_dist     = 100.0f;
   host_trail.trail_host_heading  = Create_Angle(0.0f);
   host_trail.f_trail_full_buffer = FBK_TRUE;

   /*Set none default values to the first point.*/
   host_trail.segments[0u].point.x             = 100.0f;
   host_trail.segments[0u].point.y             = 0.0f;
   host_trail.segments[0u].distance_traveled   = 100.0f;
   host_trail.segments[0u].dist_between_points = 5.0f;
   host_trail.segments[0u].heading             = 0.0f;

   /** \action call point adding function. */
   Fbk_Add_Host_Trailer_Point(&host_trail);

   /** \assert Expect properties of the trail set correctly. */
   EXPECT_FLOAT_EQ(host_trail.segments[0u].point.x, 100.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[0u].point.y, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[0u].distance_traveled, 100.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[0u].dist_between_points, 5.0f);
   EXPECT_FLOAT_EQ(host_trail.segments[0u].heading, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_dist, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_diff_heading, 0.0f);
   EXPECT_FLOAT_EQ(host_trail.trail_index, 1u);
   EXPECT_FLOAT_EQ(host_trail.oldest_trail_index, 1u);
   EXPECT_TRUE(host_trail.f_trail_full_buffer);
}

/**
 * Tests whether points shall be added to a host trail. Here a point shall be added due to the heading condition thus true is.
 * Negative values for branch coverage. expected. \uts{CSCSA-42386} \sdd{SF-4163} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Shall_Point_Be_Added_To_Trail__point_shall_be_added_to_trail_due_to_heading_condition_negative_values)
{
   /** \arrange Set up conditions such that a point shall be added. */
   boolean_T res;

   host_trail.trail_diff_dist     = fbk_cals.k_fbk_host_trail_dist_separation - 0.1f;
   host_trail.trail_diff_heading  = -1.1f * fbk_cals.k_fbk_host_trail_heading_separation;
   host_trail.trail_index         = 1u;
   host_trail.f_trail_full_buffer = FBK_TRUE;

   /** \action call routine to check whether a point shall be added to the host trail. */
   res = Fbk_Shall_Point_Be_Added_To_Trail(&host_trail, &fbk_cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Tests whether points shall be added to a host trail. Here a point shall be added due to the large trail diff distance.
 * \uts{CSCSA-42387} \sdd{SF-4163} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Shall_Point_Be_Added_To_Trail__point_shall_be_added_trail_dist)
{
   /** \arrange Set up conditions such that a point shall be added. */
   boolean_T res;

   host_trail.trail_diff_dist     = fbk_cals.k_fbk_host_trail_dist_separation + 0.1f;
   host_trail.trail_diff_heading  = 1.1f * fbk_cals.k_fbk_host_trail_heading_separation;
   host_trail.trail_index         = 1u;
   host_trail.f_trail_full_buffer = FBK_TRUE;

   /** \action call routine to check whether a point shall be added to the host trail. */
   res = Fbk_Shall_Point_Be_Added_To_Trail(&host_trail, &fbk_cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}

/**
 * Check world coordinate system calculation. Here the yawrate is big enough and negative.
 * \uts{CSCSA-42388} \sdd{SF-4164} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Get_Wcs_Shift__yawrate_is_big_enough_negative_value_speed_is_zero)
{
   /** \arrange Set up speed to zero. */
   Angle_T yaw_shift = Create_Angle(-1.1f * EPSILON);
   Vector_2d_T vec;
   p_vehicle_data->yawrate    = EPSILON;
   p_vehicle_data->host_speed = FBK_ZERO_F;

   /** \action call routine to get a wcs shift. */
   vec = Fbk_Get_Wcs_Shift(&data, &yaw_shift);

   /** \assert Expect shift of zero, because we are in standstill. */
   EXPECT_FLOAT_EQ(vec.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(vec.y, FBK_ZERO_F);
}

/**
 * Check if the empty host trail is detected. Here trail index is zero and f_trail_full_buffer is false, thus true is returned
 * \uts{CSCSA-42389} \sdd{SF-4167} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Host_Trail_Test, Fbk_Is_Host_Trail_Empty__both_fullfilled)
{
   /** \arrange Set up host trail with. */
   boolean_T result;
   host_trail.trail_index         = FBK_ZERO_UINT;
   host_trail.f_trail_full_buffer = FBK_FALSE;

   /** \action call function. */
   result = Fbk_Is_Host_Trail_Empty(&host_trail);

   /** \assert Expect true */
   EXPECT_TRUE(result);
}