/**
 * @file lane_model_camera_data_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lane_model_camera_data.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42896}
 */

#include "lane_model_camera_data_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lane_model_camera_data.c"
#include "ml_trigonometry.h"
#include "pa_reuse.h"
}


/**
 * Check that lane center offset is negative if ego is driving closer to the right border of the lane.
 * \uts{CSCSA-42897} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__outputs_negative_lane_center_offset_ego_driving_closer_to_right_border)
{
   /** \arrange Set up camera data and persistent data, such that camera state is available and ego is driving closer to the right
    * border of the lane. */
   cam_data.lane_distance_first_left  = 2.0f;
   cam_data.lane_distance_first_right = -1.0f;

   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = cals.k_lm_min_qualification_cycle_count + 2u;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that lane center offset is negative. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, 3.0f);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, -0.5f);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_AVAILABLE);
}

/**
 * Check that lane center offset is positive if ego is driving closer to the left border of the lane.
 * \uts{CSCSA-42898} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__outputs_positive_lane_center_offset_ego_driving_closer_to_left_border)
{
   /** \arrange Set up camera data and persistent data, such that camera state is available and ego is driving closer to the left
    * border of the lane. */
   cam_data.lane_distance_first_left  = 1.0f;
   cam_data.lane_distance_first_right = -2.0f;

   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = cals.k_lm_min_qualification_cycle_count + 2u;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that lane center offset is positive. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, 3.0f);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, 0.5f);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_AVAILABLE);
}

/**
 * Check that camera ego lane width is used when right lane border existence probability is low.
 * \uts{CSCSA-42899} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__uses_camera_ego_lane_width_when_right_lane_border_existence_prob_is_low)
{
   /** \arrange Set up camera data and persistent data, such that camera state is available and the right lane border existence
    * probability is low. */
   cam_data.lane_existance_probability_first_right = 40.0f;

   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = cals.k_lm_min_qualification_cycle_count + 2u;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that camera ego lane width is used. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, cam_data.lane_width_ego);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, 0.0f);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_DEGRADED);
}

/**
 * Check that camera ego lane width is used when left lane border existence probability is low.
 * \uts{CSCSA-42900} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__uses_camera_ego_lane_width_when_left_lane_border_existence_prob_is_low)
{
   /** \arrange Set up camera data and persistent data, such that camera state is available and the left lane border existence
    * probability is low. */
   cam_data.lane_existance_probability_first_left = 40.0f;

   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = cals.k_lm_min_qualification_cycle_count + 2u;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that camera ego lane width is used. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, cam_data.lane_width_ego);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, 0.0f);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_DEGRADED);
}

/**
 * Check that camera output is in initialization state if existence probability and ego lane quality are low.
 * \uts{CSCSA-42901} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__result_is_in_init_state_when_existence_prob_and_ego_lane_quality_are_low)
{
   /** \arrange Set up camera data and persistent data, such that camera state is available and the lane border existence
    * probability and ego lane quality are low. */
   cam_data.lane_existance_probability_first_left = 40.0f;
   cam_data.quality_lane_width_ego                = 0;

   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = cals.k_lm_min_qualification_cycle_count + 2u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = cals.k_lm_min_qualification_cycle_count + 2u;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that status is the initialization state. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, 0.0f);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, 0.0f);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}

/**
 * Check that camera output is in initialization state if qualification cycles are still below threshold.
 * \uts{CSCSA-42902} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__result_is_in_init_state_until_valid_data_is_received_for_at_least_k_lm_min_qualification_cycle_count)
{
   /** \arrange Set up camera data and persistent data, such that qualification cycles are still below threshold. */
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = cals.k_lm_min_qualification_cycle_count - 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = cals.k_lm_min_qualification_cycle_count - 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = cals.k_lm_min_qualification_cycle_count - 1u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = cals.k_lm_min_qualification_cycle_count - 1u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = cals.k_lm_min_qualification_cycle_count - 1u;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that status is the initialization state. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, 0.0f);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, 0.0f);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}

/**
 * Check that camera output holds previous valid output, if camera data quality is low and holding counter is below threshold.
 * \uts{CSCSA-42903} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__holds_prev_valid_output_if_camera_data_quality_is_low_and_holding_counter_below_threshold)
{
   /** \arrange Set up camera data and persistent data, such that camera data quality is low and holding cycles are still below
    * threshold. */
   float32_T lane_width = 4.0f;

   Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_width         = lane_width;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_center_offset = 0.2f;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.status             = LM_CAMERA_STATUS_AVAILABLE;
   cals.k_lm_min_output_hold_cycles                                      = 3u;
   Lane_Model_Camera_Persistent.count_hold_prev_valid_output             = cals.k_lm_min_output_hold_cycles - 1u;

   cam_data.lane_existance_probability_first_left = 40.0f;
   cam_data.quality_lane_width_ego                = 0;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that output uses previous valid camera data. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, lane_width);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_DEGRADED);
}

/**
 * Check that camera output does not hold previous valid output, if camera data quality is low and holding counter is above
 * threshold. \uts{CSCSA-42904} \sdd{SF-6906} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test,
       Lcda_Get_Camera_Data_Lane_Model__does_not_hold_prev_valid_output_if_camera_data_quality_is_low_and_holding_counter_above_threshold)
{
   /** \arrange Set up camera data and persistent data, such that camera data quality is low and holding cycles are above
    * threshold. */
   Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_width         = 4.0f;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_center_offset = 0.2f;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.status             = LM_CAMERA_STATUS_DEGRADED;
   cals.k_lm_min_output_hold_cycles                                      = 3u;
   Lane_Model_Camera_Persistent.count_hold_prev_valid_output             = cals.k_lm_min_output_hold_cycles + 1u;

   cam_data.lane_existance_probability_first_left = 40.0f;
   cam_data.quality_lane_width_ego                = 0;

   /** \action Call function Lcda_Get_Camera_Data_Lane_Model to evaluate camera data lane model. */
   Lcda_Get_Camera_Data_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Verify that output does not use previous valid camera data. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, 0.0f);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, 0.0f);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}


/**
 * Check whether the lane output is initialized correctly.
 * \uts{CSCSA-42905} \sdd{SF-6911} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Init_Lane_Output__initialize_lane_output)
{
   /** \arrange Pass instance of camera output to function. */

   /** \action Call initialization routine for lane output. */
   Lcda_Init_Lane_Output(&lane_model_camera_output);
   /** \assert Verify that mapping is done correctly. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, LCDA_LANE_WIDTH_UNKNOWN);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}


/**
 * Test the processing routine and check whether all validity counters are incremented, when the needed conditions are fulfilled.
 * \uts{CSCSA-42906} \sdd{SF-6910} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Process_Camera_Data__increment_all_counters)
{
   /** \arrange Set up input such that counters for valid lane are incremented. */
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = 0u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = 0u;
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = 0u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = 0u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = 0u;

   cam_data.lane_existance_probability_first_left  = 1.1f * cals.k_lm_min_lane_exist_prob_percent;
   cam_data.lane_existance_probability_first_right = 1.1f * cals.k_lm_min_lane_exist_prob_percent;
   cam_data.quality_lane_width_ego                 = LCDA_EGO_NORMAL_QUALITY_LANE_WIDTH;
   cam_data.quality_lane_width_left                = LCDA_ADJACENT_NORMAL_QUALITY_LANE_WIDTH;
   cam_data.quality_lane_width_right               = LCDA_ADJACENT_NORMAL_QUALITY_LANE_WIDTH;

   /** \action Call process routine for camera data to check whether validity counter shall be incremented. */
   Lcda_Process_Camera_Data(&cam_data, &cals);

   /** \assert Verify that all counters are incremented. */
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_lane_left, 1u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_lane_right, 1u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_ego_lane_width, 1u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width, 1u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width, 1u);
}


/**
 * Test the processing routine and check whether all validity counters are incremented, when the needed conditions are fulfilled.
 * Here the counters shall not be incremented, since the conditions are not fulfilled. \uts{CSCSA-42907} \sdd{SF-6910}
 * \testtype{negative}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Process_Camera_Data__keep_all_counter_values)
{
   /** \arrange Set up input such that counters keep their default value. */
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = 0u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = 0u;
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = 0u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = 0u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = 0u;

   cam_data.lane_existance_probability_first_left  = 0.9f * cals.k_lm_min_lane_exist_prob_percent;
   cam_data.lane_existance_probability_first_right = 0.9f * cals.k_lm_min_lane_exist_prob_percent;
   cam_data.quality_lane_width_ego                 = 5.0f;
   cam_data.quality_lane_width_left                = 5.0f;
   cam_data.quality_lane_width_right               = 5.0f;

   /** \action Call process routine for camera data to check whether validity counter shall be incremented. */
   Lcda_Process_Camera_Data(&cam_data, &cals);

   /** \assert Verify that all counters are remaining their defaults. */
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_lane_left, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_lane_right, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_ego_lane_width, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width, 0u);
}


/**
 * Check whether lane data is plausible. Here inputs are chosen such that true is expected.
 * \uts{CSCSA-42909} \sdd{SF-6917} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Is_Lane_Data_Plausible__lane_data_is_plausible)
{
   /** \arrange Set up such that lane data is plausible. */
   float32_T lane_width = 0.5f * (cals.k_lm_min_plausible_lane_width + cals.k_lm_max_plausible_lane_width);
   float32_T lc_offset  = 0.9f * cals.k_lm_max_plausible_lc_offset_factor * lane_width;
   boolean_T res;
   /** \action Call plausibility check for lane data. */
   res = Lcda_Is_Lane_Data_Plausible(lane_width, lc_offset, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Check whether lane data is plausible. Here inputs are chosen such that false is expected.
 * \uts{CSCSA-42910} \sdd{SF-6917} \testtype{negative}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Is_Lane_Data_Plausible__lane_data_is_implausible)
{
   /** \arrange Set up such that lane data is implausible. */
   float32_T lane_width = 0.5f * (cals.k_lm_min_plausible_lane_width + cals.k_lm_max_plausible_lane_width);
   float32_T lc_offset  = 1.1f * cals.k_lm_max_plausible_lc_offset_factor * lane_width;
   boolean_T res;

   /** \action Call plausibility check for lane data. */
   res = Lcda_Is_Lane_Data_Plausible(lane_width, lc_offset, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Check whether lane data is plausible. Here inputs are chosen such that false is expected.
 * \uts{CSCSA-42911} \sdd{SF-6907} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Init_Lane_Model_Camera__initialize_persistent_counter)
{
   /** \arrange Set up each persistent counter to 1. */
   Lane_Model_Camera_Persistent.count_valid_lane_left                 = 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                = 1u;
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width            = 1u;
   Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width  = 1u;
   Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width = 1u;
   Lane_Model_Camera_Persistent.count_hold_prev_valid_output          = 1u;

   /** \action Initialize lane model camera. */
   Lcda_Init_Lane_Model_Camera();

   /** \assert Expect counters to be reset. */
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_lane_left, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_lane_right, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_ego_lane_width, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_left_adjacent_lane_width, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_valid_right_adjacent_lane_width, 0u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_hold_prev_valid_output, 0u);
}


/**
 * Test output lane border processing. Here the lane borders have not been qualified yet. Thus lane output shall be default.
 * \uts{CSCSA-42912} \sdd{SF-6916} \testtype{negative}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Output_Lane_Borders__lane_borders_are_not_yet_qualified)
{
   /** \arrange Set up persistent data qualification counter less than thresholds. */
   Lane_Model_Camera_Persistent.count_valid_lane_left  = cals.k_lm_min_qualification_cycle_count - 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right = cals.k_lm_min_qualification_cycle_count - 1u;

   /** \action Call output lane border processing. */
   Lcda_Get_Output_Lane_Borders(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, LCDA_LANE_WIDTH_UNKNOWN);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}


/**
 * Test output lane border processing. Here the lane borders are qualified but implausible. Thus default values are expected for
 * the lane model. \uts{CSCSA-42913} \sdd{SF-6916} \testtype{negative}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Output_Lane_Borders__lane_borders_are_qualified_but_implausible)
{
   /** \arrange Set up qualification counters to a qualified state but the lane width in a way, that the lane data is interpreted
    * as implausible. */
   Lane_Model_Camera_Persistent.count_valid_lane_left  = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right = cals.k_lm_min_qualification_cycle_count + 1u;
   lcda_input.camera_data->lane_distance_first_left    = 0.0f;
   lcda_input.camera_data->lane_distance_first_right   = 0.9f * cals.k_lm_min_plausible_lane_width;

   /** \action Call output lane border processing. */
   Lcda_Get_Output_Lane_Borders(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, LCDA_LANE_WIDTH_UNKNOWN);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}

/**
 * Test output lane border processing. Here the lane borders are qualified and plausible. Thus internally calculated lane data is
 * expected to be returned. \uts{CSCSA-42914} \sdd{SF-6916} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Output_Lane_Borders__lane_borders_are_qualified_and_plausible)
{
   /** \arrange Set up qualification counters to a qualified state but the lane width in a way, that the lane data is interpreted
    * as implausible. */
   Lane_Model_Camera_Persistent.count_valid_lane_left  = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right = cals.k_lm_min_qualification_cycle_count + 1u;
   lcda_input.camera_data->lane_distance_first_left    = cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_distance_first_right   = -2.0f * cals.k_lm_min_plausible_lane_width;

   /** \action Call output lane border processing. */
   Lcda_Get_Output_Lane_Borders(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width,
                   lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset,
                   -lcda_input.camera_data->lane_distance_first_right
                      - 0.5f * (lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right));
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_AVAILABLE);
}

/**
 * Test output lane border processing. Here the lane borders are qualified and plausible. Thus internally calculated lane data is
 * expected to be returned. \uts{CSCSA-42915} \sdd{SF-6916} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Output_Lane_Borders__lateral_speed_default_due_to_bad_probability)
{
   /** \arrange Set up qualification counters to a qualified state but the lane width in a way, that the lane data is interpreted
    * as implausible. */
   Lane_Model_Camera_Persistent.count_valid_lane_left             = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right            = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_left  = LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_right = LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER;
   lcda_input.camera_data->lane_distance_first_left               = cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_distance_first_right              = -2.0f * cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_existance_probability_first_right = 130.0f;
   lcda_input.camera_data->lane_existance_probability_first_left  = 130.0f;

   /** \action Call output lane border processing. */
   Lcda_Get_Output_Lane_Borders(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width,
                   lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset,
                   -lcda_input.camera_data->lane_distance_first_right
                      - 0.5f * (lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right));
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_AVAILABLE);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_LEFT], FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_RIGHT], FBK_ZERO_F);
}

/**
 * Test output lane border processing. Here the lane borders are qualified and plausible. Thus internally calculated lane data is
 * expected to be returned. \uts{CSCSA-42916} \sdd{SF-6916} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Output_Lane_Borders__lateral_speed_correct_calulation)
{
   /** \arrange Set up qualification counters to a qualified state but the lane width in a way, that the lane data is interpreted
    * as implausible. */
   Lane_Model_Camera_Persistent.count_valid_lane_left             = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right            = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_left  = LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_right = LCDA_LANE_LATERAL_SPEED_MAX_HOLDING_COUNTER;
   lcda_input.camera_data->lane_distance_first_left               = cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_distance_first_right              = -2.0f * cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_existance_probability_first_right = 80.0f;
   lcda_input.camera_data->lane_angle_first_left                  = FBK_ONE_F;
   lcda_input.camera_data->lane_angle_first_right                 = FBK_ONE_F;
   p_vehicle_data->host_speed                                     = 10.0f;

   /** \action Call output lane border processing. */
   Lcda_Get_Output_Lane_Borders(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);


   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width,
                   lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset,
                   -lcda_input.camera_data->lane_distance_first_right
                      - 0.5f * (lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right));
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_AVAILABLE);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_LEFT],
                   Fast_Sin(lcda_input.camera_data->lane_angle_first_left) * p_vehicle_data->host_speed);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_RIGHT],
                   Fast_Sin(lcda_input.camera_data->lane_angle_first_right) * p_vehicle_data->host_speed);
}

/**
 * Test output lane border processing. Here the lane borders are qualified and plausible. Thus internally calculated lane data is
 * expected to be returned. \uts{CSCSA-42917} \sdd{SF-6916} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Output_Lane_Borders__lateral_speed_holding)
{
   /** \arrange Set up qualification counters to a qualified state but the lane width in a way, that the lane data is interpreted
    * as implausible. */
   Lane_Model_Camera_Persistent.count_valid_lane_left                   = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right                  = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_left        = 1u;
   Lane_Model_Camera_Persistent.count_holding_lateral_speed_right       = 1u;
   Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_LEFT]  = FBK_ONE_F;
   Lane_Model_Camera_Persistent.prev_lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ONE_F;
   lcda_input.camera_data->lane_distance_first_left                     = cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_distance_first_right                    = -2.0f * cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_existance_probability_first_right       = 130.0f;
   lcda_input.camera_data->lane_existance_probability_first_left        = 130.0f;

   /** \action Call output lane border processing. */
   Lcda_Get_Output_Lane_Borders(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width,
                   lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset,
                   -lcda_input.camera_data->lane_distance_first_right
                      - 0.5f * (lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right));
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_AVAILABLE);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_LEFT], FBK_ONE_F);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_LEFT], FBK_ONE_F);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_holding_lateral_speed_left, 2u);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_holding_lateral_speed_right, 2u);
}

/**
 * Test output lane border processing. Here the lane borders are qualified and plausible. Thus internally calculated lane data is
 * expected to be returned. \uts{CSCSA-42918} \sdd{SF-6916} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Output_Lane_Borders__lateral_speed_default_due_to_maxed_out_values)
{
   /** \arrange Set up qualification counters to a qualified state but the lane width in a way, that the lane data is interpreted
    * as implausible. */
   Lane_Model_Camera_Persistent.count_valid_lane_left  = cals.k_lm_min_qualification_cycle_count + 1u;
   Lane_Model_Camera_Persistent.count_valid_lane_right = cals.k_lm_min_qualification_cycle_count + 1u;
   lcda_input.camera_data->lane_distance_first_left    = cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_distance_first_right   = -2.0f * cals.k_lm_min_plausible_lane_width;
   lcda_input.camera_data->lane_angle_first_left       = 1.0f;
   lcda_input.camera_data->lane_angle_first_right      = 1.0f;
   p_vehicle_data->host_speed                          = 30.0f;

   /** \action Call output lane border processing. */
   Lcda_Get_Output_Lane_Borders(&lcda_input, p_vehicle_data, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width,
                   lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset,
                   -lcda_input.camera_data->lane_distance_first_right
                      - 0.5f * (lcda_input.camera_data->lane_distance_first_left - lcda_input.camera_data->lane_distance_first_right));
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_AVAILABLE);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_LEFT], FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_lateral_speed[FBK_SIDE_RIGHT], FBK_ZERO_F);
}

/**
 * Test returning routine of host lane information. Here the host lane has not qualified yet, thus default information for the host
 * lane output is expected. \uts{CSCSA-42919} \sdd{SF-6915} \testtype{negative}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Camera_Ego_Lane_Info__ego_lane_has_not_qualified_yet)
{
   /** \arrange Set up persistent host lane width qualification counter to an unqualified counter value. */
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width = cals.k_lm_min_qualification_cycle_count - 1u;

   /** \action Call ego lane information function. */
   Lcda_Get_Camera_Ego_Lane_Info(&lcda_input, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, LCDA_LANE_WIDTH_UNKNOWN);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}

/**
 * Test returning routine of host lane information. Here the host lane has qualified, but is implausible. Thus default information
 * for the host lane output is expected. \uts{CSCSA-42920} \sdd{SF-6915} \testtype{negative}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Camera_Ego_Lane_Info__ego_lane_is_qualified_but_implausible)
{
   /** \arrange Set up qualified host lane which is implausible. */
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width = cals.k_lm_min_qualification_cycle_count + 1u;
   lcda_input.camera_data->lane_width_ego                  = 0.0f;

   /** \action Call ego lane information function. */
   Lcda_Get_Camera_Ego_Lane_Info(&lcda_input, &cals, &lane_model_camera_output);

   /** \assert Expect default output for lane model output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, LCDA_LANE_WIDTH_UNKNOWN);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
}


/**
 * Test returning routine of host lane information. Here the host lane has qualified and plausible. Thus non default host lane is
 * returned. \uts{CSCSA-42921} \sdd{SF-6915} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Get_Camera_Ego_Lane_Info__ego_lane_is_qualified_and_plausible)
{
   /** \arrange Set up qualified host lane which is plausible. */
   Lane_Model_Camera_Persistent.count_valid_ego_lane_width = cals.k_lm_min_qualification_cycle_count + 1u;
   lcda_input.camera_data->lane_width_ego                  = 1.1f * cals.k_lm_min_plausible_lane_width;

   /** \action Call ego lane information function. */
   Lcda_Get_Camera_Ego_Lane_Info(&lcda_input, &cals, &lane_model_camera_output);

   /** \assert Expect internally set camera output. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, lcda_input.camera_data->lane_width_ego);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_DEGRADED);
}

/**
 * Test holding logic for lane model. Here the lane model was held too long, thus a reset of the lane model is expected.
 * \uts{CSCSA-42922} \sdd{SF-6914} \testtype{negative}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Hold_Previous_Output__holding_logic_exceeds_holding_cycles)
{
   /** \arrange Set up persistent holding counter to an unqualified counter value. */
   Lane_Model_Camera_Persistent.count_hold_prev_valid_output = cals.k_lm_min_output_hold_cycles;

   /** \action Call holding logic. */
   Lcda_Hold_Previous_Output(&lane_model_camera_output, &cals);

   /** \assert Expect default output for lane model output and reset holding counter. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, LCDA_LANE_WIDTH_UNKNOWN);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_INIT);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_hold_prev_valid_output, 0u);
}


/**
 * Test holding logic for lane model. Here the lane model was not held yet, thus the previous output is expected to be held.
 * \uts{CSCSA-42923} \sdd{SF-6914} \testtype{positive}
 */
TEST_F(Lane_Model_Camera_Data_Test, Lcda_Hold_Previous_Output__hold_previous_output)
{
   /** \arrange Set inputs for holding logic such that previous output is held. */
   Lane_Model_Camera_Persistent.count_hold_prev_valid_output     = 0u;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_width = 2.5f;
   Lane_Model_Camera_Persistent.prev_lm_output_camera.status     = LM_CAMERA_STATUS_DEGRADED;

   /** \action Call holding logic. */
   Lcda_Hold_Previous_Output(&lane_model_camera_output, &cals);

   /** \assert Expect output to be held. */
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_width, Lane_Model_Camera_Persistent.prev_lm_output_camera.lane_width);
   EXPECT_FLOAT_EQ(lane_model_camera_output.lane_center_offset, LCDA_LANE_CENTER_OFFSET_UNKNOWN);
   EXPECT_EQ(lane_model_camera_output.status, LM_CAMERA_STATUS_DEGRADED);
   EXPECT_EQ(Lane_Model_Camera_Persistent.count_hold_prev_valid_output, 1u);
}
