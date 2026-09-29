/**
 * @file ta_intersection_analyzer_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44846}
 */

#include "ta_intersection_analyzer_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "ta_intersection_analyzer.c"
}

/**
 * Each combination of host circle and object circle are critical in this test and are checked. Here it is expected that the
 * approach is critical. \uts{CSCSA-44847} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__result_true_all_distances)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_radius                               = 5.0f;
   obj.circle_radius                               = 5.0f;
   ego.waypoint_yaw_angle.angle                    = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle                    = ta_cal.k_ta_critical_approach_angle_diff_min;
   ta_cal.k_ta_critical_approach_min_safe_distance = 0.5f;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}

/**
 * Each combination of host circle and object circle is checked within this test. The approach shall not be critical.
 * \uts{CSCSA-44848} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__result_false_all_distances)
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
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}

/**
 * Approach is only deemed critical if the approach angle is larger than a specified threshold.
 * \uts{CSCSA-44871} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__approach_angle_too_shallow)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_radius                               = 5.0f;
   obj.circle_radius                               = 5.0f;
   ego.waypoint_yaw_angle.angle                    = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle                    = FBK_ZERO_F;
   ta_cal.k_ta_critical_approach_min_safe_distance = 0.5f;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}

/**
 * Test whether a critical approach is occuring with respect to the front circles of object and host. The approach shall be
 * critical. \uts{CSCSA-44849} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__result_true_ego_front)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_center_front.x = 10.0f;
   ego.circle_center_front.y = 10.0f;
   ego.circle_radius         = 1.0f;

   obj.circle_center_front.x = 10.0f;
   obj.circle_center_front.y = 10.0f;
   obj.circle_radius         = 1.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Test whether a critical approach is occuring with respect to the middle circles of host and front circles of object. The
 * approach shall be critical. \uts{CSCSA-44850} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__result_true_ego_middle)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_center_middle.x = 10.0f;
   ego.circle_center_middle.y = 10.0f;
   ego.circle_radius          = 1.0f;

   obj.circle_center_front.x = 10.0f;
   obj.circle_center_front.y = 10.0f;
   obj.circle_radius         = 1.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}

/**
 * Test whether a critical approach is occuring with respect to the rear circles of host and front circles of object. The approach
 * shall be critical. \uts{CSCSA-44851} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__result_true_ego_rear)
{
   /** \arrange Set up critical approach/circles. */
   ego.circle_center_rear.x = 10.0f;
   ego.circle_center_rear.y = 10.0f;
   ego.circle_radius        = 1.0f;

   obj.circle_center_front.x = 10.0f;
   obj.circle_center_front.y = 10.0f;
   obj.circle_radius         = 1.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}

/**
 * Test whether a critical approach is occuring with respect to the front circles of host and front circles of object. The approach
 * shall not be critical since all checks are deactivated. \uts{CSCSA-44852} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__result_false_ego_front)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_front.x = 10.0f;
   ego.circle_center_front.y = 10.0f;
   ego.circle_radius         = 1.0f;

   obj.circle_center_front.x = 10.0f;
   obj.circle_center_front.y = 10.0f;
   obj.circle_radius         = 1.0f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects front center and hosts front. Result shall be that no
 * critical approach is given. \uts{CSCSA-44853} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_front_obj_front_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = 7.5f;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = ego.circle_center_front.y + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle   */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle   */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}

/**
 * Tests the exact boundary of minimum needed safety distance between objects front center and hosts front. Result shall be that a
 * critical approach is given. \uts{CSCSA-44854} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_front_obj_front_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   float32_T pos_without_eps = 7.5f;
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = pos_without_eps + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects center circles and hosts front. Result shall be that
 * no critical approach is given. \uts{CSCSA-44855} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_front_obj_center_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = 7.5f;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 2.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = 0.0f;
   obj.circle_center_middle.y = ego.circle_center_front.y + min_safe_dist;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects center circles and hosts front. Result shall be that
 * a critical approach is given. \uts{CSCSA-44856} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_front_obj_center_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps = 7.5f;

   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = ego.circle_center_front.y + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts front. Result shall be that no
 * critical approach is given. \uts{CSCSA-44857} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_front_obj_rear_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_front.x = 0.0f;
   ego.circle_center_front.y = 7.5f;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = -4.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = 0.0f;
   obj.circle_center_rear.y   = ego.circle_center_front.y + min_safe_dist;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts front. Result shall be that a
 * critical approach is given. \uts{CSCSA-44858} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_front_obj_rear_circles_on_min_dist)
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
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 1; /* enables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects front circles and hosts middle. Result shall be that
 * no critical approach is given. \uts{CSCSA-44859} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_middle_obj_front_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_middle.x = 0.0f;
   ego.circle_center_middle.y = 7.5f;
   ego.circle_radius          = 1.0f;
   obj.circle_radius          = 1.0f;
   float32_T min_safe_dist    = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = ego.circle_center_middle.y + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}

/**
 * Tests the exact boundary of minimum needed safety distance between objects front circles and hosts middle. Result shall be that
 * a critical approach is given. \uts{CSCSA-44860} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_middle_obj_front_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps  = 7.5f;
   ego.circle_center_middle.x = 0.0f;
   ego.circle_center_middle.y = pos_without_eps + EPSILON;
   ego.circle_radius          = 1.0f;
   obj.circle_radius          = 1.0f;
   float32_T min_safe_dist    = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = pos_without_eps + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle*/

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects center circles and hosts middle. Result shall be that
 * no critical approach is given. \uts{CSCSA-44861} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_middle_obj_center_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_middle.x = 0.0f;
   ego.circle_center_middle.y = 7.5f;
   ego.circle_radius          = 1.0f;
   obj.circle_radius          = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 2.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = 0.0f;
   obj.circle_center_middle.y = ego.circle_center_middle.y + min_safe_dist;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects center circles and hosts middle. Result shall be that
 * a critical approach is given. \uts{CSCSA-44862} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_middle_obj_center_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps  = 7.5f;
   ego.circle_center_middle.x = 0.0f;
   ego.circle_center_middle.y = pos_without_eps + EPSILON;
   ego.circle_radius          = 1.0f;
   obj.circle_radius          = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 2.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = 0.0f;
   obj.circle_center_middle.y = pos_without_eps + min_safe_dist;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts middle. Result shall be that
 * no critical approach is given. \uts{CSCSA-44863} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_middle_obj_rear_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_middle.x = 0.0f;
   ego.circle_center_middle.y = 7.5f;
   ego.circle_radius          = 1.0f;
   obj.circle_radius          = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = -4.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = 0.0f;
   obj.circle_center_rear.y   = ego.circle_center_middle.y + min_safe_dist;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle*/

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts middle. Result shall be that a
 * critical approach is given. \uts{CSCSA-44864} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_middle_obj_rear_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps  = 7.5f;
   ego.circle_center_middle.x = 0.0f;
   ego.circle_center_middle.y = pos_without_eps + EPSILON;
   ego.circle_radius          = 1.0f;
   obj.circle_radius          = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = -4.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = 0.0f;
   obj.circle_center_rear.y   = pos_without_eps + min_safe_dist;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;


   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 1; /* enables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 0; /* disables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects front circles and hosts rear. Result shall be that no
 * critical approach is given. \uts{CSCSA-44865} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_rear_obj_front_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_rear.x = 0.0f;
   ego.circle_center_rear.y = 7.5f;
   ego.circle_radius        = 1.0f;
   obj.circle_radius        = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = ego.circle_center_rear.y + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle*/

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects front circles and hosts rear. Result shall be that a
 * critical approach is given. \uts{CSCSA-44866} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_rear_obj_front_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps = 7.5f;
   ego.circle_center_rear.x  = 0.0f;
   ego.circle_center_rear.y  = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = pos_without_eps + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;


   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}

/**
 * Tests the exact boundary of minimum needed safety distance between objects middle circles and hosts rear. Result shall be that
 * no critical approach is given. \uts{CSCSA-44867} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_rear_obj_middle_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_rear.x = 0.0f;
   ego.circle_center_rear.y = 7.5f;
   ego.circle_radius        = 1.0f;
   obj.circle_radius        = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = ego.circle_center_rear.y + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle*/

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects middle circles and hosts rear. Result shall be that a
 * critical approach is given. \uts{CSCSA-44868} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_rear_obj_middle_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps = 7.5f;
   ego.circle_center_rear.x  = 0.0f;
   ego.circle_center_rear.y  = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = pos_without_eps + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;


   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts rear. Result shall be that no
 * critical approach is given. \uts{CSCSA-44869} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__exact_boundary_test_host_rear_obj_rear_circles_on_min_dist)
{
   /** \arrange Set up non-critical approach/circles. */
   ego.circle_center_rear.x = 0.0f;
   ego.circle_center_rear.y = 7.5f;
   ego.circle_radius        = 1.0f;
   obj.circle_radius        = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = -4.0f;
   obj.circle_center_front.y  = 10.0f;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = 0.0f;
   obj.circle_center_rear.y   = ego.circle_center_middle.y + min_safe_dist;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;

   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle*/
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle*/

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is non-critical. */
   EXPECT_FALSE(result);
}


/**
 * Tests the exact boundary of minimum needed safety distance between objects rear circles and hosts rear. Result shall be that a
 * critical approach is given. \uts{CSCSA-44870} \sdd{SF-8723} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Is_Critical_Approach__lt_boundary_test_host_rear_obj_rear_circles_on_min_dist)
{
   /** \arrange Set up critical approach/circles. */
   float32_T pos_without_eps = 7.5f;
   ego.circle_center_rear.x  = 0.0f;
   ego.circle_center_rear.y  = pos_without_eps + EPSILON;
   ego.circle_radius         = 1.0f;
   obj.circle_radius         = 1.0f;

   float32_T min_safe_dist = ta_cal.k_ta_critical_approach_min_safe_distance + ego.circle_radius + obj.circle_radius;

   obj.circle_center_front.x  = 0.0f;
   obj.circle_center_front.y  = pos_without_eps + min_safe_dist;
   obj.circle_center_middle.x = -2.0f;
   obj.circle_center_middle.y = 10.5f;
   obj.circle_center_rear.x   = -2.5f;
   obj.circle_center_rear.y   = 11.5f;

   ego.waypoint_yaw_angle.angle = FBK_ZERO_F;
   obj.waypoint_yaw_angle.angle = ta_cal.k_ta_critical_approach_angle_diff_min;


   ta_cal.k_ta_critical_approach_check_ego_circles[0] = 0; /* disables front ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[1] = 0; /* disables middle ego circle */
   ta_cal.k_ta_critical_approach_check_ego_circles[2] = 1; /* enables rear ego circle */

   /** \action Get critical approach estimation */
   boolean_T result = Ta_Is_Critical_Approach(&ta_cal, &ego, &obj);

   /** \assert Verify that approach is critical. */
   EXPECT_TRUE(result);
}


/**
 * Check that no overlap is detected for invalid circle type.
 * \uts{CSCSA-44872} \sdd{SF-8615} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Does_Ego_Circle_Overlap_Any_Object_Circle__return_false_invalid_circle_type)
{
   /** \arrange Set up critical approach/circles. */
   float32_T min_safe_dist = 1.0f;

   /** \action Run function */
   boolean_T result = Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(&ego, &obj, (Ta_Circle_Type_T) 99, min_safe_dist);

   /** \assert Verify that no overlap is detected. */
   EXPECT_FALSE(result);
}

/**
 * Check that overlap is detected for ego front with object center middle.
 * \uts{CSCSA-93444} \sdd{SF-8615} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Does_Ego_Circle_Overlap_Any_Object_Circle__ego_front_obj_center_middle)
{
   /** \arrange Set up critical approach/circles. */
   float32_T min_safe_dist  = 1.0f;
   float32_T circle_pos     = 1.0f;
   ego.circle_center_front  = {circle_pos, circle_pos};
   obj.circle_center_front  = {circle_pos + min_safe_dist + EPSILON, circle_pos + min_safe_dist + EPSILON};
   obj.circle_center_middle = {circle_pos + 0.5f, circle_pos + 0.5f};


   /** \action Run function */
   boolean_T result = Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(&ego, &obj, (Ta_Circle_Type_T) 0, min_safe_dist);

   /** \assert Verify that no overlap is detected. */
   EXPECT_TRUE(result);
}

/**
 * Check that overlap is detected for ego rear with object center middle.
 * \uts{CSCSA-93445} \sdd{SF-8615} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Does_Ego_Circle_Overlap_Any_Object_Circle__ego_rear_obj_center_middle)
{
   /** \arrange Set up critical approach/circles. */
   float32_T min_safe_dist  = 1.0f;
   float32_T circle_pos     = -2.3f;
   ego.circle_center_rear   = {circle_pos, circle_pos};
   obj.circle_center_rear   = {circle_pos + min_safe_dist + EPSILON, circle_pos + min_safe_dist + EPSILON};
   obj.circle_center_middle = {circle_pos + 0.37f, circle_pos + 0.48f};


   /** \action Run function */
   boolean_T result = Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(&ego, &obj, (Ta_Circle_Type_T) 2, min_safe_dist);

   /** \assert Verify that no overlap is detected. */
   EXPECT_TRUE(result);
}


/**
 * Check that overlap is detected for ego rear with object center rear.
 * \uts{CSCSA-93446} \sdd{SF-8615} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Intersection_Analyzer_Test, Ta_Does_Ego_Circle_Overlap_Any_Object_Circle__ego_rear_obj_center_rear)
{
   /** \arrange Set up critical approach/circles. */
   float32_T min_safe_dist = 1.0f;
   float32_T circle_pos    = -2.3f;
   ego.circle_center_rear  = {circle_pos, circle_pos};
   obj.circle_center_rear  = {circle_pos + min_safe_dist + EPSILON, circle_pos + min_safe_dist + EPSILON};
   obj.circle_center_rear  = {circle_pos + 0.49f, circle_pos + 0.49f};


   /** \action Run function */
   boolean_T result = Ta_Does_Ego_Circle_Overlap_Any_Object_Circle(&ego, &obj, (Ta_Circle_Type_T) 2, min_safe_dist);

   /** \assert Verify that no overlap is detected. */
   EXPECT_TRUE(result);
}
