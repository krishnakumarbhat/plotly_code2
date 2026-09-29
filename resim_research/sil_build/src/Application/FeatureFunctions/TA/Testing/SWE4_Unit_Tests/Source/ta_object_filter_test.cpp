/**
 * @file ta_object_filter_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44884}
 */

#include "ta_object_filter_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "ml_lookup_table_2d.h"
#include "ta_object_filter.c"
#include <float.h>
#include <math.h>
}

/**
 * Tests if heading is within range in a straight scenario. Middle-of-the-road-test for straight scenario parameters.
 * \uts{CSCSA-44997} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__motr_heading_inside_range_straight_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = -1.4f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   boolean_T f_extended_range = FBK_FALSE;
   p_vehicle_data->curvature  = 0.0f;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is inside range. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests if heading is outside range in a straight scenario. Heading shall be set in way to be inside range for curved but outside
 * range for straight scenario. \uts{CSCSA-44998} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__heading_outside_range_straight_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = 0.9f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   boolean_T f_extended_range = FBK_FALSE;
   p_vehicle_data->curvature  = 0.0f;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests if the heading rate is outside the range for a straight scenario. Middle-of-the-road-test for straight scenario
 * parameters. \uts{CSCSA-45023} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__heading_rate_outside_range_straight_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = -1.4f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;
   ta_object.tracker_data.heading_rate   = ta_cal.k_fta_obj_heading_rate[TA_MAX] + EPSILON;

   boolean_T f_extended_range = FBK_FALSE;
   p_vehicle_data->curvature  = 0.0f;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   /** \action Check if value is outside thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside of the range. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests if heading is outside range in case curvature value is NaN. Heading shall be set to middle-of-the-road value, while
 * curvature shall be NAN. \uts{CSCSA-44999} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__motr_heading_nan_curvature)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = 1.4f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   boolean_T f_extended_range = FBK_FALSE;
   p_vehicle_data->curvature  = NAN;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests if heading is outside range in case curvature value is equal to threshold. Heading shall be outside of range for straight
 * but inside range for curved scenario, while curvature shall be border case. \uts{CSCSA-45000} \sdd{SF-8539}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__boundary_value_curvature)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = -0.8f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   float32_T host_speed = 3.0f;
   float32_T host_curvature_min =
      Get_Value_From_2d_Lookup_Table(ta_cal.k_ta_lookup_turning_host_speed, ta_cal.k_ta_lookup_turning_host_curvature_min,
                                     TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, host_speed);
   boolean_T f_extended_range = FBK_FALSE;

   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->host_speed = host_speed;
   p_vehicle_data->curvature  = host_curvature_min;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests if heading is outside range in case curvature value is epsilon below threshold. Heading shall be outside of range for
 * straight but inside range for curved scenario, while curvature shall be border case. \uts{CSCSA-45001} \sdd{SF-8539}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__epsilon_boundary_value_curvature)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = -0.8f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   boolean_T f_extended_range = FBK_FALSE;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->curvature = ta_cal.k_ta_straight_host_curvature_max - FLT_EPSILON;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests if heading is outside range in case curvature value is equal to negative threshold. Heading shall be outside of range for
 * straight but inside range for curved scenario, while curvature shall be border case. \uts{CSCSA-45002} \sdd{SF-8539}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__negative_boundary_value_curvature)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = 0.8f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   float32_T host_speed = 3.0f;
   float32_T host_curvature_min =
      Get_Value_From_2d_Lookup_Table(ta_cal.k_ta_lookup_turning_host_speed, ta_cal.k_ta_lookup_turning_host_curvature_min,
                                     TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, host_speed);
   boolean_T f_extended_range = FBK_FALSE;

   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->host_speed = host_speed;
   p_vehicle_data->curvature  = -host_curvature_min;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests if heading is inside range in a left turn scenario. Middle-of-the-road-test for left turn scenario.
 * \uts{CSCSA-45003} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__motr_heading_inside_range_left_turn_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = 0.9f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   float32_T host_speed = 3.0f;
   float32_T host_curvature_min =
      Get_Value_From_2d_Lookup_Table(ta_cal.k_ta_lookup_turning_host_speed, ta_cal.k_ta_lookup_turning_host_curvature_min,
                                     TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, host_speed);
   boolean_T f_extended_range = FBK_FALSE;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->host_speed = host_speed;
   p_vehicle_data->curvature  = -(host_curvature_min + EPSILON);

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests if heading is outside range in left turn scenario. Heading shall be set in way to be inside range for right turn but
 * outside range for left turn scenario. \uts{CSCSA-45004} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__heading_outside_range_left_turn_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = -0.9f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   boolean_T f_extended_range = FBK_FALSE;
   p_vehicle_data->curvature  = -0.1f;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests if heading is inside the extended range in a left turn scenario.
 * \uts{CSCSA-45016} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__heading_inside_extended_range_left_turn_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = 1.9f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   float32_T host_speed = 3.0f;
   float32_T host_curvature_min =
      Get_Value_From_2d_Lookup_Table(ta_cal.k_ta_lookup_turning_host_speed, ta_cal.k_ta_lookup_turning_host_curvature_min,
                                     TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, host_speed);
   boolean_T f_extended_range = FBK_TRUE;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.2f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->host_speed = host_speed;
   p_vehicle_data->curvature  = -(host_curvature_min + EPSILON);

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that the checked value is inside the extended range. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests if heading is inside range in a right turn scenario. Middle-of-the-road-test for right turn scenario.
 * \uts{CSCSA-45005} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__motr_heading_inside_range_right_turn_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = -0.9f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   float32_T host_speed = 3.0f;
   float32_T host_curvature_min =
      Get_Value_From_2d_Lookup_Table(ta_cal.k_ta_lookup_turning_host_speed, ta_cal.k_ta_lookup_turning_host_curvature_min,
                                     TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, host_speed);
   boolean_T f_extended_range = FBK_FALSE;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->host_speed = host_speed;
   p_vehicle_data->curvature  = host_curvature_min + EPSILON;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that the checked value is inside the range. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests if heading is inside the extended range in a right turn scenario.
 * \uts{CSCSA-45017} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__heading_inside_extended_range_right_turn_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = -1.9f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   float32_T host_speed = 3.0f;
   float32_T host_curvature_min =
      Get_Value_From_2d_Lookup_Table(ta_cal.k_ta_lookup_turning_host_speed, ta_cal.k_ta_lookup_turning_host_curvature_min,
                                     TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, host_speed);
   boolean_T f_extended_range = FBK_TRUE;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.2f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->host_speed = host_speed;
   p_vehicle_data->curvature  = host_curvature_min + EPSILON;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that the checked value is inside the extended range. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests if heading is outside range in right turn scenario. Heading shall be set in way to be inside range for left turn but
 * outside range for right turn scenario. \uts{CSCSA-45006} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__heading_outside_range_right_turn_scenario)
{
   /** \arrange Setup scenario. */
   ta_object.tracker_data.vcs_heading    = 0.9f;
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   boolean_T f_extended_range = FBK_FALSE;
   p_vehicle_data->curvature  = 0.1f;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_FALSE(result_value);
}

/**
 * Test that object is reported as not relevant due to difference between heading and velocity vector.
 * \uts{CSCSA-45019} \sdd{SF-8539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Relevant_Regarding_Host_Curvature__heading_differs_from_velocity_vector)
{
   /** \arrange Setup scenario with differing heading and velocity_heading. */
   ta_object.tracker_data.vcs_heading    = 0.9f;
   ta_object.attributes.velocity_heading = -ta_object.tracker_data.vcs_heading;
   ta_object.tracker_data.speed          = ta_cal.k_fta_obj_speed_straight[TA_MIN];
   ta_object.attributes.ego_heading_diff = 0.0f;
   ta_object.tracker_data.vcs_pos.x      = ta_cal.k_fta_obj_vcs_long_pos_straight_min;

   float32_T host_speed = 3.0f;
   float32_T host_curvature_min =
      Get_Value_From_2d_Lookup_Table(ta_cal.k_ta_lookup_turning_host_speed, ta_cal.k_ta_lookup_turning_host_curvature_min,
                                     TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, host_speed);
   boolean_T f_extended_range = FBK_FALSE;

   ta_cal.k_ta_straight_host_curvature_max        = 0.033f;
   ta_cal.k_fta_obj_heading_straight[TA_MIN]      = 1.0f;
   ta_cal.k_fta_obj_heading_straight[TA_MAX]      = 1.7f;
   ta_cal.k_fta_obj_heading[TA_MIN]               = 0.2f;
   ta_cal.k_fta_obj_heading[TA_MAX]               = 1.7f;
   ta_cal.k_fta_obj_heading_ofst[TA_MIN]          = -0.1f;
   ta_cal.k_fta_obj_heading_ofst[TA_MAX]          = 0.1f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MIN]      = 0.00f;
   ta_cal.k_fta_ego_obj_heading_diff[TA_MAX]      = 0.25f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MIN] = 0.0f;
   ta_cal.k_fta_ego_obj_heading_diff_ofst[TA_MAX] = 0.0f;

   p_vehicle_data->host_speed = host_speed;
   p_vehicle_data->curvature  = -(host_curvature_min + EPSILON);

   /** \action Check if value is within thresholds */
   boolean_T result_value = Ta_Is_Obj_Relevant_Regarding_Host_Curvature(&ta_object, f_extended_range, p_vehicle_data, &ta_cal);

   /** \assert Verify that checked value is outside range. */
   EXPECT_FALSE(result_value);
}


/**
 * Tests the state relevance of the ego. Here all conditions shall be fulfilled by the arranged input parameters. The object shall
 * be a relevant Turn Assist candidate. \uts{CSCSA-44984} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__check_ego_general_relevance)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is TA relevant. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests the exact boundary of minimum needed speed for an object to be relevant for Turn Assist. The considered object here shall
 * be valid. \uts{CSCSA-44985} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__exact_lower_boundary_speed_signal_obj_is_valid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->host_speed = ta_cal.k_ta_ego_speed[TA_MIN];

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is TA relevant. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests a value less than the minimum required boundary of speed for an object to be relevant for Turn Assist. The considered
 * object here shall be invalid. \uts{CSCSA-44986} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__lt_lower_boundary_speed_signal_obj_is_invalid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->host_speed = ta_cal.k_ta_ego_speed[TA_MIN] - EPSILON;

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is not TA relevant. */
   EXPECT_FALSE(result_value);
}


/**
 * Tests the exact boundary of minimum needed speed for an object to be relevant for Turn Assist. The considered object here shall
 * be valid. \uts{CSCSA-44987} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__exact_upper_boundary_speed_signal_obj_is_valid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->host_speed = ta_cal.k_ta_ego_speed[TA_MAX];

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is TA relevant. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests a value greater than the maximum allowed boundary of speed for an object to be relevant for Turn Assist. The considered
 * object here shall be invalid. \uts{CSCSA-44988} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__gt_upper_boundary_speed_signal_obj_is_invalid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->host_speed = ta_cal.k_ta_ego_speed[TA_MAX] + EPSILON;

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is not TA relevant. */
   EXPECT_FALSE(result_value);
}


/**
 * Tests the exact boundary of minimum needed yawrate for an object to be relevant for Turn Assist. The considered object here
 * shall be valid. \uts{CSCSA-44989} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__exact_lower_boundary_yawrate_signal_obj_is_valid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->yawrate = ta_cal.k_ta_ego_yawrate[TA_MIN];

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is TA relevant. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests a value less than the minimum required boundary of yawrate for an object to be relevant for Turn Assist. The considered
 * object here shall be invalid. \uts{CSCSA-44990} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__lt_lower_boundary_yawrate_signal_obj_is_invalid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.1f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->yawrate = ta_cal.k_ta_ego_yawrate[TA_MIN] - EPSILON;

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is not TA relevant. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests the exact boundary of minimum needed yawrate for an object to be relevant for Turn Assist. The considered object here
 * shall be valid. \uts{CSCSA-44991} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__exact_upper_boundary_yawrate_signal_obj_is_valid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->yawrate = ta_cal.k_ta_ego_yawrate[TA_MAX];

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is TA relevant. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests a value greater than the maximum allowed boundary of yawrate for an object to be relevant for Turn Assist. The considered
 * object here shall be invalid. \uts{CSCSA-44992} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__gt_upper_boundary_yawrate_signal_obj_is_invalid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->long_acc    = 0.0f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->yawrate = ta_cal.k_ta_ego_yawrate[TA_MAX] + EPSILON;

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is not TA relevant. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests the exact boundary of minimum allowed longitudinal acceleration for an object to be relevant for Turn Assist. The
 * considered object here shall be valid. \uts{CSCSA-45020} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__exact_lower_boundary_long_acc_signal_obj_is_valid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->long_acc = ta_cal.k_ta_ego_long_acceleration[TA_MIN];

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is TA relevant. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests a value less than the minimum required boundary of longitudinal acceleration for an object to be relevant for Turn Assist.
 * The considered object here shall be invalid. \uts{CSCSA-45021} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__lt_lower_boundary_long_acc_signal_obj_is_invalid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.1f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->long_acc = ta_cal.k_ta_ego_long_acceleration[TA_MIN] - EPSILON;

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is not TA relevant. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests the exact boundary of maximum allowed longitudinal acceleration for an object to be relevant for Turn Assist. The
 * considered object here shall be valid. \uts{CSCSA-45022} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__exact_upper_boundary_long_acc_signal_obj_is_valid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->long_acc = ta_cal.k_ta_ego_long_acceleration[TA_MAX];

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is TA relevant. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests a value greater than the maximum allowed boundary of longitudinal acceleration for an object to be relevant for Turn
 * Assist. The considered object here shall be invalid. \uts{CSCSA-45024} \sdd{SF-8730} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Vehicle_State_Relevant__gt_upper_boundary_long_acc_signal_obj_is_invalid)
{
   /** \arrange Setup host vehicle and target properties, set cal values. */
   ta_object.tracker_data.length = 2.0f;
   ta_object.tracker_data.width  = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_ta_ego_speed[TA_MIN]             = 1.0f;
   ta_cal.k_ta_ego_speed[TA_MAX]             = 10.0f;
   ta_cal.k_ta_ego_yawrate[TA_MIN]           = 0.0f;
   ta_cal.k_ta_ego_yawrate[TA_MAX]           = 1.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MIN] = -5.0f;
   ta_cal.k_ta_ego_long_acceleration[TA_MAX] = 5.0f;

   p_vehicle_data->long_acc = ta_cal.k_ta_ego_long_acceleration[TA_MAX] + EPSILON;

   /** \action Check if vehicle state is TA relevant */
   boolean_T result_value = Ta_Is_Vehicle_State_Relevant(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that vehicle state is not TA relevant. */
   EXPECT_FALSE(result_value);
}

/**
 * Create FTA relevant target in danger zone. Update TA target relevance and verify it is recognized being within the danger zone.
 * \uts{CSCSA-44889} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__check_obj_fta_relevance)
{
   /** \arrange Setup FTA relevant target in danger zone. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the lower boundary of minimum needed object existence prob for an object to be relevant for the danger zone. The
 * considered object here shall be in danger zone \uts{CSCSA-44890} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_existence_prob_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_fta_obj_exist_prblty[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object existence prob for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44891} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_existence_prob_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_fta_obj_exist_prblty[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the upper boundary of maximum allowed object existence prob for an object to be relevant for the danger zone. The
 * considered object here shall be in danger zone \uts{CSCSA-44892} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_existence_prob_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_fta_obj_exist_prblty[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects existence prob for an object to be relevant for the danger
 * \uts{CSCSA-44893} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_existence_prob_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_fta_obj_exist_prblty[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the lower boundary of minimum needed object long vel rel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44894} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_long_vel_rel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_fta_obj_vcs_long_vel_rel[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object long vel rel for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44895} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_long_vel_rel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_fta_obj_vcs_long_vel_rel[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the upper boundary of maximum allowed object long vel rel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44896} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_long_vel_rel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_fta_obj_vcs_long_vel_rel[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects long vel rel for an object to be relevant for the danger
 * \uts{CSCSA-44897} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_long_vel_rel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_fta_obj_vcs_long_vel_rel[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object lat vel rel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44898} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_lat_vel_rel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_fta_obj_vcs_lat_vel_rel[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object lat vel rel for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44899} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_lat_vel_rel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_fta_obj_vcs_lat_vel_rel[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the upper boundary of maximum allowed object lat vel rel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44900} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_lat_vel_rel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_fta_obj_vcs_lat_vel_rel[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects lat vel rel for an object to be relevant for the danger
 * \uts{CSCSA-44901} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_lat_vel_rel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_fta_obj_vcs_lat_vel_rel[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the lower boundary of minimum needed object long vel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44902} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_long_vel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_fta_obj_vcs_long_vel[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object long vel for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44903} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_long_vel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_fta_obj_vcs_long_vel[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the upper boundary of maximum allowed object long vel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44904} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_long_vel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_fta_obj_vcs_long_vel[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects long vel for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44905} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_long_vel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_fta_obj_vcs_long_vel[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object lat vel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44906} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_lat_vel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_fta_obj_vcs_lat_vel[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object lat vel for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44907} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_lat_vel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_fta_obj_vcs_lat_vel[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the upper boundary of maximum allowed object lat vel for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44908} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_lat_vel_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_fta_obj_vcs_lat_vel[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects lat vel for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44909} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_lat_vel_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_fta_obj_vcs_lat_vel[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object heading for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44910} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_heading_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_heading    = ta_cal.k_fta_obj_heading[TA_MIN];
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object heading for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44911} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_heading_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_heading = ta_cal.k_fta_obj_heading[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the upper boundary of maximum allowed object heading for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44912} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_heading_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_heading    = ta_cal.k_fta_obj_heading[TA_MAX];
   ta_object.attributes.velocity_heading = ta_object.tracker_data.vcs_heading;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects heading for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44913} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_heading_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_heading = ta_cal.k_fta_obj_vcs_lat_vel[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object speed for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44914} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_speed_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_fta_obj_speed[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object speed for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44915} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_speed_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_fta_obj_speed[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the upper boundary of maximum allowed object speed for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44916} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_speed_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_fta_obj_speed[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects speed for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44917} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_speed_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_fta_obj_speed[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object length for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44964} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_length_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_fta_obj_length[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object length for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44965} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_length_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_fta_obj_length[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the upper boundary of maximum allowed object length for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44966} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_length_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_fta_obj_length[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects length for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44967} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_length_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_fta_obj_length[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object width for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44968} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_width_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_fta_obj_width[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object width for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44969} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_width_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_fta_obj_width[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the upper boundary of maximum allowed object width for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44970} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_width_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_fta_obj_width[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects width for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44971} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_width_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_fta_obj_width[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object class for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44918} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_object_class_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_fta_obj_vru_class_prob[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object class for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44919} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_object_class_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_fta_obj_vru_class_prob[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the upper boundary of maximum allowed object class for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44920} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_object_class_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_fta_obj_vru_class_prob[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed class for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44921} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_object_class_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_fta_obj_vru_class_prob[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object eclipse_value for an object to be relevant for the danger zone. The considered
 * object here shall be in danger zone \uts{CSCSA-45012} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_object_eclipse_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.eclipse_value = ta_cal.k_fta_obj_eclipse_value[TA_MIN];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object eclipse_value for an object to be relevant for the danger zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-45013} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_object_eclipse_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.eclipse_value = ta_cal.k_fta_obj_eclipse_value[TA_MIN] - EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests the upper boundary of maximum allowed object eclipse_value for an object to be relevant for the danger zone. The
 * considered object here shall be in danger zone \uts{CSCSA-45014} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_upper_boundary_object_eclipse_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.eclipse_value = ta_cal.k_fta_obj_eclipse_value[TA_MAX];

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects eclipse_value for an object to be relevant for the danger
 * zone. The considered object here shall not be in danger zone \uts{CSCSA-45015} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__gt_upper_boundary_object_eclipse_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.eclipse_value = ta_cal.k_fta_obj_eclipse_value[TA_MAX] + EPSILON;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Tests the lower boundary of minimum needed object age for an object to be relevant for the danger zone. The considered object
 * here shall be in danger zone \uts{CSCSA-44972} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__exact_lower_boundary_object_age_signal_obj_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.age = ta_cal.k_fta_obj_age_min;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is relevant to danger zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Tests a value less than lower boundary of minimum needed object age for an object to be relevant for the danger zone. The
 * considered object here shall not be in danger zone \uts{CSCSA-44973} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__lt_lower_boundary_object_age_signal_obj_not_in_danger_zone)
{
   /** \arrange Setup FTA relevant target in danger zone and set its properties. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.age = ta_cal.k_fta_obj_age_min - ((uint8_t) 1);

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}

/**
 * Create FTA relevant target in danger zone. Disable fta switch. Update TA target relevance and verify it is recognized as not in
 * zone. \uts{CSCSA-93447} \sdd{SF-8731} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Fta_Relevance__fta_disabled)
{
   /** \arrange Setup FTA relevant target in danger zone. */
   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);
   ta_cal.k_f_fta_enable = FBK_FALSE;

   /** \action Evaluate target FTA relevance */
   Ta_Update_Obj_Fta_Relevance(&ta_object, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not relevant to danger zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_danger_zone);
}


/**
 * Create RTA relevant target. Update TA target relevance and verify it is recognized as relevant.
 * \uts{CSCSA-44922} \sdd{SF-8732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Rta_Relevance__check_obj_rta_relevance)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);

   /** \action Evaluate target RTA relevance */
   Ta_Update_Obj_Rta_Relevance(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(ta_object.attributes.f_obj_in_info_zone);
   EXPECT_TRUE(ta_object.attributes.f_obj_in_wing_zone);
}

/**
 * Create an object that is not RTA relevant. Update TA target relevance and verify it is recognized as not being relevant.
 * \uts{CSCSA-45007} \sdd{SF-8732} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Obj_Rta_Relevance__check_obj_not_rta_relevant)
{
   /** \arrange Setup RTA relevant target, but disable RTA. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_cal.k_f_rta_enable = FBK_FALSE;

   /** \action Evaluate target RTA relevance */
   Ta_Update_Obj_Rta_Relevance(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(ta_object.attributes.f_obj_in_info_zone);
   EXPECT_FALSE(ta_object.attributes.f_obj_in_wing_zone);
}

/**
 * Tests the lower boundary of minimum needed object existence prob for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44923} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_existence_prob_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_rta_obj_exist_prblty[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object existence prob for an object to be relevant for the info and
 * \uts{CSCSA-44924} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_existence_prob_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_rta_obj_exist_prblty[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the upper boundary of maximum allowed object existence prob for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44925} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_existence_prob_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_rta_obj_exist_prblty[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects existence prob for an object to be relevant info and wing
 * \uts{CSCSA-44926} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_existence_prob_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.existence_probability = ta_cal.k_rta_obj_exist_prblty[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant. */
   EXPECT_FALSE(f_relevant);
}

/**
 * Tests the lower boundary of minimum needed object long vel rel for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44927} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_long_vel_rel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_rta_obj_vcs_long_vel_rel[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object long vel rel for an object to be relevant for the info and wing
 * \uts{CSCSA-44928} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_long_vel_rel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_rta_obj_vcs_long_vel_rel[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the upper boundary of maximum allowed object long vel rel for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44929} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_long_vel_rel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_rta_obj_vcs_long_vel_rel[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects long vel rel for an object to be relevant for the info and
 * \uts{CSCSA-44930} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_long_vel_rel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.x = ta_cal.k_rta_obj_vcs_long_vel_rel[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object lat vel rel for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44931} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_lat_vel_rel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_rta_obj_vcs_lat_vel_rel[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object lat vel rel for an object to be relevant for the info and wing
 * \uts{CSCSA-44932} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_lat_vel_rel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_rta_obj_vcs_lat_vel_rel[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the upper boundary of maximum allowed object lat vel rel for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44933} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_lat_vel_rel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_rta_obj_vcs_lat_vel_rel[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects lat vel rel for an object to be relevant for the info and
 * \uts{CSCSA-44934} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_lat_vel_rel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel_rel.y = ta_cal.k_rta_obj_vcs_lat_vel_rel[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}

/**
 * Tests the lower boundary of minimum needed object long vel for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44935} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_long_vel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_rta_obj_vcs_long_vel[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object long vel for an object to be relevant for the info and wing
 * \uts{CSCSA-44936} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_long_vel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_rta_obj_vcs_long_vel[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the upper boundary of maximum allowed object long vel for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44937} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_long_vel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_rta_obj_vcs_long_vel[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects long vel for an object to be relevant for the info and wing
 * \uts{CSCSA-44938} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_long_vel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.x = ta_cal.k_rta_obj_vcs_long_vel[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object lat vel for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44939} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_lat_vel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_rta_obj_vcs_lat_vel[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object lat vel for an object to be relevant for the info and wing zone.
 * The considered object here shall not be relevant \uts{CSCSA-44940} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_lat_vel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_rta_obj_vcs_lat_vel[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the upper boundary of maximum allowed object lat vel for an object to be relevant for the info and wing zone. The
 * considered object here shall be in danger zone \uts{CSCSA-44941} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_lat_vel_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_rta_obj_vcs_lat_vel[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects lat vel for an object to be relevant for the info and wing
 * \uts{CSCSA-44942} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_lat_vel_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_vel.y = ta_cal.k_rta_obj_vcs_lat_vel[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object heading for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44943} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_heading_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_heading = ta_cal.k_rta_obj_heading[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object heading for an object to be relevant for the info and wing zone.
 * The considered object here shall not be relevant \uts{CSCSA-44944} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_heading_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);

   ta_cal.k_rta_obj_heading[TA_MIN]   = ta_cal.k_rta_obj_heading[TA_MIN] + EPSILON;
   ta_object.tracker_data.vcs_heading = ta_cal.k_rta_obj_heading[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the upper boundary of maximum allowed object heading for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44945} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_heading_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_heading = ta_cal.k_rta_obj_heading[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects heading for an object to be relevant for the info and wing
 * \uts{CSCSA-44946} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_heading_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.vcs_heading = ta_cal.k_rta_obj_vcs_lat_vel[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object speed for an object to be relevant for the info and wing zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44947} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_speed_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_rta_obj_speed[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object speed for an object to be relevant for the info and wing zone.
 * The considered object here shall not be in danger zone \uts{CSCSA-44948} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_speed_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_rta_obj_speed[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the upper boundary of maximum allowed object speed for an object to be relevant for the info and wing zone. The considered
 * object here shall be relevant \uts{CSCSA-44949} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_speed_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_rta_obj_speed[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects speed for an object to be relevant for the info and wing
 * \uts{CSCSA-44950} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_speed_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.speed = ta_cal.k_rta_obj_speed[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object length for an object to be relevant for the info and wing zone. The considered
 * object here shall be relevant \uts{CSCSA-44974} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_length_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_rta_obj_length[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object length for an object to be relevant for the info and wing zone.
 * The considered object here shall not be relevant \uts{CSCSA-44975} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_length_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_rta_obj_length[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}

/**
 * Tests the upper boundary of maximum allowed object length for an object to be relevant for the info and wing zone. The
 * considered object here shall be relevant \uts{CSCSA-44976} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_length_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_rta_obj_length[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects length for an object to be relevant for the info and wing
 * zone. The considered object here shall not be in danger zone \uts{CSCSA-44977} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_length_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.length = ta_cal.k_rta_obj_length[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object width for an object to be relevant for the info and wing zone. The considered
 * object here shall be relevant \uts{CSCSA-44978} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_width_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_rta_obj_width[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object width for an object to be relevant for the info and wing zone.
 * The considered object here shall not be relevant \uts{CSCSA-44979} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_width_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_rta_obj_width[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}

/**
 * Tests the upper boundary of maximum allowed object width for an object to be relevant for the info and wing zone. The considered
 * object here shall be relevant \uts{CSCSA-44980} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_width_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_rta_obj_width[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects width for an object to be relevant for the info and wing
 * zone. The considered object here shall not be relevant \uts{CSCSA-44981} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_width_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.width = ta_cal.k_rta_obj_width[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object class for an object to be relevant for the info and wing zone. The considered
 * object here shall be relevant \uts{CSCSA-44951} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_object_class_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_rta_obj_vru_class_prob[TA_MIN];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object class for an object to be relevant for the info and wing
 * \uts{CSCSA-44952} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_object_class_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_rta_obj_vru_class_prob[TA_MIN] - EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}

/**
 * Tests the upper boundary of maximum allowed object speed for an object to be relevant for the info and wing zone. The considered
 * object here shall be in danger zone \uts{CSCSA-44953} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_upper_boundary_object_class_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_rta_obj_vru_class_prob[TA_MAX];

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value greater than upper boundary of maximum allowed objects speed for an object to be relevant for the info and wing
 * \uts{CSCSA-44954} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__gt_upper_boundary_speed_object_class_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.attributes.object_class_probability_vru = ta_cal.k_rta_obj_vru_class_prob[TA_MAX] + EPSILON;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests the lower boundary of minimum needed object age for an object to be relevant for the info and wing zone. The considered
 * object here shall be relevant \uts{CSCSA-44982} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__exact_lower_boundary_object_age_signal_obj_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.age = ta_cal.k_rta_obj_age_min;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is relevant to info and wing zone. */
   EXPECT_TRUE(f_relevant);
}

/**
 * Tests a value less than lower boundary of minimum needed object age for an object to be relevant for the info and wing zone. The
 * considered object here shall not be relevant \uts{CSCSA-44983} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__lt_lower_boundary_object_age_signal_obj_not_in_zones)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.age = ta_cal.k_rta_obj_age_min - ((uint8_t) 1);

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}

/**
 * Tests an object to be relevant for the info and wing zone that is flagged as a reflection. The considered object here shall not
 * be relevant \uts{CSCSA-45018} \sdd{SF-8787} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Rta_Relevant__object_is_reflection)
{
   /** \arrange Setup RTA relevant target. */
   Ta_Set_Up_Rta_Relevant_Object(&ta_object, &ta_cal);
   ta_object.tracker_data.f_reflection = FBK_TRUE;

   /** \action Evaluate target RTA relevance */
   boolean_T f_relevant = Ta_Is_Obj_Rta_Relevant(&ta_object, &ta_cal);

   /** \assert Verify that target is not relevant to info and wing zone. */
   EXPECT_FALSE(f_relevant);
}


/**
 * Tests whether an object is within the danger zone. The object shall be within the left part of the danger zone.
 * \uts{CSCSA-44955} \sdd{SF-8727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Danger_Zone__test_object_in_danger_zone_left)
{
   /** \arrange Setup a target within the left part of the danger zone. */
   ta_object.tracker_data.vcs_pos.x   = 2.0f;
   ta_object.tracker_data.vcs_pos.y   = -5.0f;
   ta_object.tracker_data.vcs_heading = 0.3f;

   ta_cal.k_f_fta_enable_danger_zones  = 1;
   ta_cal.k_fta_danger_zone_point_size = 4;

   ta_cal.k_fta_danger_zone_left_long[0] = 8.0f;
   ta_cal.k_fta_danger_zone_left_long[1] = 8.0f;
   ta_cal.k_fta_danger_zone_left_long[2] = -4.0f;
   ta_cal.k_fta_danger_zone_left_long[3] = -4.0f;

   ta_cal.k_fta_danger_zone_left_lat[0] = -10.0f;
   ta_cal.k_fta_danger_zone_left_lat[1] = 0.0f;
   ta_cal.k_fta_danger_zone_left_lat[2] = 0.0f;
   ta_cal.k_fta_danger_zone_left_lat[3] = -10.0f;

   /** \action Evaluate if target is in danger zone */
   boolean_T result_value = Ta_Is_Obj_In_Danger_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in danger zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the danger zone. The object shall be within the right part of the danger zone.
 * \uts{CSCSA-44956} \sdd{SF-8727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Danger_Zone__test_object_in_danger_zone_right)
{
   /** \arrange Setup a target within the right part of the danger zone. */
   ta_object.tracker_data.vcs_pos.x   = 2.0f;
   ta_object.tracker_data.vcs_pos.y   = 5.0f;
   ta_object.tracker_data.vcs_heading = -0.3f;

   ta_cal.k_f_fta_enable_danger_zones  = 1;
   ta_cal.k_fta_danger_zone_point_size = 4;

   ta_cal.k_fta_danger_zone_right_long[0] = 8.0f;
   ta_cal.k_fta_danger_zone_right_long[1] = 8.0f;
   ta_cal.k_fta_danger_zone_right_long[2] = -4.0f;
   ta_cal.k_fta_danger_zone_right_long[3] = -4.0f;

   ta_cal.k_fta_danger_zone_right_lat[0] = 0.0f;
   ta_cal.k_fta_danger_zone_right_lat[1] = 10.0f;
   ta_cal.k_fta_danger_zone_right_lat[2] = 10.0f;
   ta_cal.k_fta_danger_zone_right_lat[3] = 0.0f;

   /** \action Evaluate if target is in danger zone */
   boolean_T result_value = Ta_Is_Obj_In_Danger_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in danger zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the danger zone. The object shall not be within the danger zone.
 * \uts{CSCSA-45008} \sdd{SF-8727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Danger_Zone__test_object_not_in_danger_zone_right)
{
   /** \arrange Setup a target outside of the danger zone. */
   ta_object.tracker_data.vcs_pos.x   = 2.0f;
   ta_object.tracker_data.vcs_pos.y   = 15.0f;
   ta_object.tracker_data.vcs_heading = -0.3f;

   ta_cal.k_f_fta_enable_danger_zones  = 1;
   ta_cal.k_fta_danger_zone_point_size = 4;

   ta_cal.k_fta_danger_zone_right_long[0] = 8.0f;
   ta_cal.k_fta_danger_zone_right_long[1] = 8.0f;
   ta_cal.k_fta_danger_zone_right_long[2] = -4.0f;
   ta_cal.k_fta_danger_zone_right_long[3] = -4.0f;

   ta_cal.k_fta_danger_zone_right_lat[0] = 0.0f;
   ta_cal.k_fta_danger_zone_right_lat[1] = 10.0f;
   ta_cal.k_fta_danger_zone_right_lat[2] = 10.0f;
   ta_cal.k_fta_danger_zone_right_lat[3] = 0.0f;

   /** \action Evaluate if target is in danger zone */
   boolean_T result_value = Ta_Is_Obj_In_Danger_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is not in danger zone. */
   EXPECT_FALSE(result_value);
}

/**
 * Checks that the function returns false when the danger zone switch is off.
 * \uts{CSCSA-93448} \sdd{SF-8727} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Danger_Zone__fta_danger_zone_disabled)
{
   /** \arrange Setup a target outside of the danger zone. */
   ta_cal.k_f_fta_enable_danger_zones = FBK_FALSE;

   /** \action Evaluate if target is in danger zone */
   boolean_T result_value = Ta_Is_Obj_In_Danger_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is not in danger zone. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests whether an object is within the info zone. The object shall be within the left part of the info zone.
 * \uts{CSCSA-44957} \sdd{SF-8728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Info_Zone__test_object_in_info_zone_left)
{
   /** \arrange Setup a target within the left part of the info zone. */
   ta_object.attributes.f_curvi_available = FBK_FALSE;
   ta_object.tracker_data.vcs_pos.x       = -10.0f;
   ta_object.tracker_data.vcs_pos.y       = -5.0f;

   ta_cal.k_f_rta_enable_info_zones  = 1;
   ta_cal.k_rta_info_zone_point_size = 4;

   ta_cal.k_rta_info_zone_left_long[0] = -1.0f;
   ta_cal.k_rta_info_zone_left_long[1] = -1.0f;
   ta_cal.k_rta_info_zone_left_long[2] = -26.0f;
   ta_cal.k_rta_info_zone_left_long[3] = -26.0f;

   ta_cal.k_rta_info_zone_left_lat[0] = -7.0f;
   ta_cal.k_rta_info_zone_left_lat[1] = -1.0f;
   ta_cal.k_rta_info_zone_left_lat[2] = -1.0f;
   ta_cal.k_rta_info_zone_left_lat[3] = -7.0f;

   /** \action Evaluate if target is in info zone */
   boolean_T result_value = Ta_Is_Obj_In_Info_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in info zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the info zone. The object shall be within the right part of the info zone.
 * \uts{CSCSA-44958} \sdd{SF-8728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Info_Zone__test_object_in_info_zone_right)
{
   /** \arrange Setup a target within the right part of the info zone. */
   ta_object.attributes.f_curvi_available = FBK_FALSE;
   ta_object.tracker_data.vcs_pos.x       = -10.0f;
   ta_object.tracker_data.vcs_pos.y       = 5.0f;

   ta_cal.k_f_rta_enable_info_zones  = 1;
   ta_cal.k_rta_info_zone_point_size = 4;

   ta_cal.k_rta_info_zone_right_long[0] = -1.0f;
   ta_cal.k_rta_info_zone_right_long[1] = -1.0f;
   ta_cal.k_rta_info_zone_right_long[2] = -26.0f;
   ta_cal.k_rta_info_zone_right_long[3] = -26.0f;

   ta_cal.k_rta_info_zone_right_lat[0] = 1.0f;
   ta_cal.k_rta_info_zone_right_lat[1] = 7.0f;
   ta_cal.k_rta_info_zone_right_lat[2] = 7.0f;
   ta_cal.k_rta_info_zone_right_lat[3] = 1.0f;

   /** \action Evaluate if target is in info zone */
   boolean_T result_value = Ta_Is_Obj_In_Info_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in info zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the info zone. For this check the curvi object position shall be used The object shall be
 * within the left part of the info zone. \uts{CSCSA-44959} \sdd{SF-8728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Info_Zone__test_object_in_info_zone_left_curvi)
{
   /** \arrange Setup a target within the left part of the info zone. */
   ta_object.attributes.f_curvi_available = FBK_TRUE;
   ta_object.tracker_data.curvi_pos.x     = -10.0f;
   ta_object.tracker_data.curvi_pos.y     = -5.0f;

   ta_cal.k_f_rta_enable_info_zones  = 1;
   ta_cal.k_rta_info_zone_point_size = 4;

   ta_cal.k_rta_info_zone_left_long[0] = -1.0f;
   ta_cal.k_rta_info_zone_left_long[1] = -1.0f;
   ta_cal.k_rta_info_zone_left_long[2] = -26.0f;
   ta_cal.k_rta_info_zone_left_long[3] = -26.0f;

   ta_cal.k_rta_info_zone_left_lat[0] = -7.0f;
   ta_cal.k_rta_info_zone_left_lat[1] = -1.0f;
   ta_cal.k_rta_info_zone_left_lat[2] = -1.0f;
   ta_cal.k_rta_info_zone_left_lat[3] = -7.0f;

   /** \action Evaluate if target is in info zone */
   boolean_T result_value = Ta_Is_Obj_In_Info_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in info zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the info zone. For this check the curvi object position shall be used The object shall be
 * within the right part of the info zone. \uts{CSCSA-44960} \sdd{SF-8728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Info_Zone__test_object_in_info_zone_right_curvi)
{
   /** \arrange Setup a target within the right part of the info zone. */
   ta_object.attributes.f_curvi_available = FBK_TRUE;
   ta_object.tracker_data.curvi_pos.x     = -10.0f;
   ta_object.tracker_data.curvi_pos.y     = 5.0f;

   ta_cal.k_f_rta_enable_info_zones  = 1;
   ta_cal.k_rta_info_zone_point_size = 4;

   ta_cal.k_rta_info_zone_right_long[0] = -1.0f;
   ta_cal.k_rta_info_zone_right_long[1] = -1.0f;
   ta_cal.k_rta_info_zone_right_long[2] = -26.0f;
   ta_cal.k_rta_info_zone_right_long[3] = -26.0f;

   ta_cal.k_rta_info_zone_right_lat[0] = 1.0f;
   ta_cal.k_rta_info_zone_right_lat[1] = 7.0f;
   ta_cal.k_rta_info_zone_right_lat[2] = 7.0f;
   ta_cal.k_rta_info_zone_right_lat[3] = 1.0f;

   /** \action Evaluate if target is in info zone */
   boolean_T result_value = Ta_Is_Obj_In_Info_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in info zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the info zone. For this check the curvi object position shall be used Here the info zone will
 * be deactivated by calibration value. \uts{CSCSA-45009} \sdd{SF-8728} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Info_Zone__info_zone_deactivated)
{
   /** \arrange Disable info zone. */
   ta_cal.k_f_rta_enable_info_zones = 0;

   /** \action Evaluate if target is in info zone */
   boolean_T result_value = Ta_Is_Obj_In_Info_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in info zone. */
   EXPECT_FALSE(result_value);
}

/**
 * Tests whether an object is within the wing zone. The object shall be within the left part of the wing zone.
 * \uts{CSCSA-44961} \sdd{SF-8729} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Wing_Zone__test_object_in_wing_zone_left)
{
   /** \arrange Setup a target within the left part of the wing zone. */
   ta_object.tracker_data.vcs_pos.x = -5.0f;
   ta_object.tracker_data.vcs_pos.y = -5.0f;

   ta_cal.k_f_rta_enable_wing_zones  = 1;
   ta_cal.k_rta_wing_zone_point_size = 4;

   ta_cal.k_rta_wing_zone_left_long[0] = -1.0f;
   ta_cal.k_rta_wing_zone_left_long[1] = -1.0f;
   ta_cal.k_rta_wing_zone_left_long[2] = -10.0f;
   ta_cal.k_rta_wing_zone_left_long[3] = -10.0f;

   ta_cal.k_rta_wing_zone_left_lat[0] = -10.0f;
   ta_cal.k_rta_wing_zone_left_lat[1] = -1.0f;
   ta_cal.k_rta_wing_zone_left_lat[2] = -1.0f;
   ta_cal.k_rta_wing_zone_left_lat[3] = -10.0f;

   /** \action Evaluate if target is in wing zone */
   boolean_T result_value = Ta_Is_Obj_In_Wing_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in wing zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the wing zone. The object shall be within the right part of the wing zone.
 * \uts{CSCSA-44962} \sdd{SF-8729} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Wing_Zone__test_object_in_wing_zone_right)
{
   /** \arrange Setup a target within the right part of the wing zone. */
   ta_object.tracker_data.vcs_pos.x = -5.0f;
   ta_object.tracker_data.vcs_pos.y = 5.0f;

   ta_cal.k_f_rta_enable_wing_zones  = 1;
   ta_cal.k_rta_wing_zone_point_size = 4;

   ta_cal.k_rta_wing_zone_right_long[0] = -1.0f;
   ta_cal.k_rta_wing_zone_right_long[1] = -1.0f;
   ta_cal.k_rta_wing_zone_right_long[2] = -10.0f;
   ta_cal.k_rta_wing_zone_right_long[3] = -10.0f;

   ta_cal.k_rta_wing_zone_right_lat[0] = 1.0f;
   ta_cal.k_rta_wing_zone_right_lat[1] = 10.0f;
   ta_cal.k_rta_wing_zone_right_lat[2] = 10.0f;
   ta_cal.k_rta_wing_zone_right_lat[3] = 1.0f;

   /** \action Evaluate if target is in wing zone */
   boolean_T result_value = Ta_Is_Obj_In_Wing_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in wing zone. */
   EXPECT_TRUE(result_value);
}

/**
 * Tests whether an object is within the wing zone. Here the wing zone will be deactivated by calibration value.
 * \uts{CSCSA-45010} \sdd{SF-8729} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_In_Wing_Zone__wing_zone_deactivated)
{
   /** \arrange Disable info zone. */
   ta_cal.k_f_rta_enable_wing_zones = 0;

   /** \action Evaluate if target is in info zone */
   boolean_T result_value = Ta_Is_Obj_In_Wing_Zone(&ta_object, &ta_cal);

   /** \assert Verify that target is in info zone. */
   EXPECT_FALSE(result_value);
}

/**
 * Checks whether an object is relevant for the Turn Assist feature. Here the object shall be relevant for the Front Turn Assist.
 * \uts{CSCSA-44963} \sdd{SF-8735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Object_Relevance__check_obj_ta_relevance)
{
   /** \arrange Setup a FTA relevant target. */
   ta_object.attributes.f_obj_ta_relevant = FBK_FALSE;
   ta_object.tracker_data.length          = 2.0f;
   ta_object.tracker_data.width           = 1.0f;

   p_vehicle_data->host_speed  = 5.0f;
   p_vehicle_data->yawrate     = 0.5f;
   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   ta_cal.k_f_fta_enable = FBK_TRUE;

   Ta_Set_Up_Fta_Relevant_Object(&ta_object, &ta_cal);

   /** \action Update TA object relevance */
   Ta_Update_Object_Relevance(&ta_object, &ta_core_input, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is TA relevant. */
   EXPECT_TRUE(ta_object.attributes.f_obj_ta_relevant);
}

/**
 * Checks whether an object is relevant for the Turn Assist feature. Here the object shall not be relevant for the Turn Assist.
 * \uts{CSCSA-45011} \sdd{SF-8735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Update_Object_Relevance__check_obj_not_ta_relevant)
{
   /** \arrange Setup a FTA relevant target, but ego is too fast. */
   ta_object.attributes.f_obj_ta_relevant = FBK_FALSE;
   p_vehicle_data->host_speed             = 50.0f;
   p_vehicle_data->yawrate                = 0.5f;
   ta_cal.k_f_fta_enable                  = FBK_TRUE;
   ta_cal.k_ta_ego_speed[TA_MAX]          = 10.0f;

   /** \action Update TA object relevance */
   Ta_Update_Object_Relevance(&ta_object, &ta_core_input, p_vehicle_data, &ta_cal);

   /** \assert Verify that target is not TA relevant. */
   EXPECT_FALSE(ta_object.attributes.f_obj_ta_relevant);
}

/**
 * Checks whether an object was alerted in last cycle. Here the object shall be relevant for both the Front Turn Assist and Rear
 * Turn Assist. The cal value k_ta_always_overwrite_ta_mode_to_both is set to false. \uts{CSCSA-44993} \sdd{SF-8791}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Active__alert_both_check_for_both_overwrite_off)
{
   /** \arrange Setup a relevant target. */
   ta_cal.k_ta_always_overwrite_ta_mode_to_both = FBK_FALSE;

   ta_object.attributes.f_obj_ta_relevant = FBK_TRUE;
   ta_object.attributes.ta_alert_mode     = TA_ALERT_MODE_BOTH;

   /** \action Request if object was active last cycle */
   boolean_T f_ta_is_obj_active = Ta_Is_Obj_Active(&ta_object, TA_ALERT_MODE_BOTH);

   /** \assert Verify that target is TA relevant. */
   EXPECT_TRUE(f_ta_is_obj_active);
}

/**
 * Checks whether an object was alerted in last cycle. Here the object shall be relevant for the Front Turn Assist. The cal value
 * k_ta_always_overwrite_ta_mode_to_both is set to false. \uts{CSCSA-44994} \sdd{SF-8791} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Active__alert_front_check_for_both_overwrite_off)
{
   /** \arrange Setup a relevant target. */
   ta_cal.k_ta_always_overwrite_ta_mode_to_both = FBK_FALSE;

   ta_object.attributes.f_obj_ta_relevant = FBK_TRUE;
   ta_object.attributes.ta_alert_mode     = TA_ALERT_MODE_FRONT;

   /** \action Request if object was active last cycle */
   boolean_T f_ta_is_obj_active = Ta_Is_Obj_Active(&ta_object, TA_ALERT_MODE_BOTH);

   /** \assert Verify that target is TA relevant. */
   EXPECT_TRUE(f_ta_is_obj_active);
}

/**
 * Checks whether an object was alerted in last cycle. Here the object shall be relevant for the Front Turn Assist. The cal value
 * k_ta_always_overwrite_ta_mode_to_both is set to false. \uts{CSCSA-44995} \sdd{SF-8791} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Active__alert_front_check_for_rear_overwrite_off)
{
   /** \arrange Setup a relevant target. */
   ta_cal.k_ta_always_overwrite_ta_mode_to_both = FBK_FALSE;

   ta_object.attributes.f_obj_ta_relevant = FBK_TRUE;
   ta_object.attributes.ta_alert_mode     = TA_ALERT_MODE_FRONT;

   /** \action Request if object was active last cycle */
   boolean_T f_ta_is_obj_active = Ta_Is_Obj_Active(&ta_object, TA_ALERT_MODE_REAR);

   /** \assert Verify that target is not TA relevant. */
   EXPECT_FALSE(f_ta_is_obj_active);
}

/**
 * Checks whether an object was alerted in last cycle. Here the object shall be relevant for the Front Turn Assist. The cal value
 * k_ta_always_overwrite_ta_mode_to_both is set to false. \uts{CSCSA-44996} \sdd{SF-8791} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Object_Filter_Test, Ta_Is_Obj_Active__alert_front_check_for_rear_overwrite_on)
{
   /** \arrange Setup a relevant target. */
   ta_cal.k_ta_always_overwrite_ta_mode_to_both = FBK_TRUE;

   ta_object.attributes.f_obj_ta_relevant = FBK_TRUE;
   ta_object.attributes.ta_alert_mode     = TA_ALERT_MODE_FRONT;

   /** \action Request if object was active last cycle */
   boolean_T f_ta_is_obj_active = Ta_Is_Obj_Active(&ta_object, TA_ALERT_MODE_REAR);

   /** \assert Verify that target is not TA relevant. */
   EXPECT_FALSE(f_ta_is_obj_active);
}