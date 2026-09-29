/**
 * @file lcda_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_post_run.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42959}
 */

#include "lcda_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_post_run.c"
#include "ml_checked_rounding.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Tests whether the default routine is setting defaults correctly to the output.
 * \uts{CSCSA-42960} \sdd{SF-6986} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Reset_Output__set_defaults_accordingly)
{

   /** \arrange Set up non defaults for output */

   lcda_output.f_lcda_enabled                              = 1u;
   lcda_output.f_bsw_enabled                               = 1u;
   lcda_output.f_cvw_enabled                               = 1u;
   lcda_output.f_slc_enabled                               = 1u;
   lcda_output.f_awa_enabled                               = 1u;
   lcda_output.bsw_alert[FBK_SIDE_LEFT]                    = 1u;
   lcda_output.bsw_id[FBK_SIDE_LEFT]                       = 1u;
   lcda_output.bsw_unique_id[FBK_SIDE_LEFT]                = 1u;
   lcda_output.cvw_alert[FBK_SIDE_LEFT]                    = 1u;
   lcda_output.cvw_id[FBK_SIDE_LEFT]                       = 1u;
   lcda_output.cvw_unique_id[FBK_SIDE_LEFT]                = 1u;
   lcda_output.cvw_ttc[FBK_SIDE_LEFT]                      = 1.0f;
   lcda_output.slc_alert[FBK_SIDE_LEFT]                    = 1u;
   lcda_output.slc_id[FBK_SIDE_LEFT]                       = 1u;
   lcda_output.slc_unique_id[FBK_SIDE_LEFT]                = 1u;
   lcda_output.slc_ttc[FBK_SIDE_LEFT]                      = 1.0f;
   lcda_output.slc_lane_change_probability[FBK_SIDE_LEFT]  = 1.0f;
   lcda_output.awa_alert[FBK_SIDE_LEFT]                    = 1u;
   lcda_output.awa_id[FBK_SIDE_LEFT]                       = 1u;
   lcda_output.awa_ttc[FBK_SIDE_LEFT]                      = 1.0f;
   lcda_output.awa_dec[FBK_SIDE_LEFT]                      = 1.9f;
   lcda_output.bsw_alert[FBK_SIDE_RIGHT]                   = 1u;
   lcda_output.bsw_id[FBK_SIDE_RIGHT]                      = 1u;
   lcda_output.bsw_unique_id[FBK_SIDE_RIGHT]               = 1u;
   lcda_output.cvw_alert[FBK_SIDE_RIGHT]                   = 1u;
   lcda_output.cvw_id[FBK_SIDE_RIGHT]                      = 1u;
   lcda_output.cvw_unique_id[FBK_SIDE_RIGHT]               = 1u;
   lcda_output.cvw_ttc[FBK_SIDE_RIGHT]                     = 1.0f;
   lcda_output.slc_alert[FBK_SIDE_RIGHT]                   = 1u;
   lcda_output.slc_id[FBK_SIDE_RIGHT]                      = 1u;
   lcda_output.slc_unique_id[FBK_SIDE_RIGHT]               = 1u;
   lcda_output.slc_ttc[FBK_SIDE_RIGHT]                     = 1.0f;
   lcda_output.slc_lane_change_probability[FBK_SIDE_RIGHT] = 1.0f;
   lcda_output.awa_alert[FBK_SIDE_RIGHT]                   = 1u;
   lcda_output.awa_id[FBK_SIDE_RIGHT]                      = 1u;
   lcda_output.awa_ttc[FBK_SIDE_RIGHT]                     = 1.0f;
   lcda_output.awa_dec[FBK_SIDE_RIGHT]                     = 1.0f;
   lcda_output.lane_width                                  = 1.0f;
   lcda_output.lane_center_offset                          = 1.0f;
   lcda_output.lane_lateral_speed[FBK_SIDE_LEFT]           = 1.0f;
   lcda_output.lane_lateral_speed[FBK_SIDE_RIGHT]          = 1.0f;
   lcda_output.lcda_object_type_left                       = (uint8_t) LCDA_OBJ_TYPE_CVW;
   lcda_output.lcda_object_id_left                         = 1u;
   lcda_output.lcda_object_px_left                         = 1.0f;
   lcda_output.lcda_object_py_left                         = 1.0f;
   lcda_output.lcda_object_ttc_left                        = 1.0f;
   lcda_output.lcda_object_vx_left                         = 1.0f;
   lcda_output.lcda_object_vy_left                         = 1.0f;
   lcda_output.lcda_object_existance_probability_left      = 1u;
   lcda_output.lcda_object_lane_change_probability_left    = 1u;
   lcda_output.lcda_object_type_right                      = (uint8_t) LCDA_OBJ_TYPE_CVW;
   lcda_output.lcda_object_id_right                        = 1u;
   lcda_output.lcda_object_px_right                        = 1.0f;
   lcda_output.lcda_object_py_right                        = 1.0f;
   lcda_output.lcda_object_ttc_right                       = 1.0f;
   lcda_output.lcda_object_vx_right                        = 1.0f;
   lcda_output.lcda_object_vy_right                        = 1.0f;
   lcda_output.lcda_object_existance_probability_right     = 1u;
   lcda_output.lcda_object_existance_probability_right     = 1u;
   lcda_output.lcda_object_lane_change_probability_right   = 1u;

   /** \action Call function to test. */

   Lcda_Reset_Output(&lcda_output);

   /** \assert Check whether output is default */

   EXPECT_EQ(lcda_output.f_lcda_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_bsw_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_cvw_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_slc_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_awa_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bsw_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bsw_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.bsw_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc[FBK_SIDE_LEFT], LCDA_CVW_DEFAULT_NO_ALERT_TTC);
   EXPECT_EQ(lcda_output.slc_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.slc_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.slc_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.slc_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(lcda_output.slc_lane_change_probability[FBK_SIDE_LEFT], LCDA_SLC_PROBABILITY_NONE);
   EXPECT_EQ(lcda_output.awa_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.awa_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.awa_ttc[FBK_SIDE_LEFT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(lcda_output.awa_dec[FBK_SIDE_LEFT], FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bsw_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bsw_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.bsw_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc[FBK_SIDE_RIGHT], LCDA_CVW_DEFAULT_NO_ALERT_TTC);
   EXPECT_EQ(lcda_output.slc_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.slc_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.slc_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.slc_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(lcda_output.slc_lane_change_probability[FBK_SIDE_RIGHT], LCDA_SLC_PROBABILITY_NONE);
   EXPECT_EQ(lcda_output.awa_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.awa_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.awa_ttc[FBK_SIDE_RIGHT], LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(lcda_output.awa_dec[FBK_SIDE_RIGHT], FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lane_width, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lane_center_offset, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lane_lateral_speed[FBK_SIDE_LEFT], FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lane_lateral_speed[FBK_SIDE_RIGHT], FBK_ZERO_F);
   EXPECT_EQ(lcda_output.lcda_object_type_left, (uint8_t) LCDA_OBJ_TYPE_NONE);
   EXPECT_EQ(lcda_output.lcda_object_id_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_px_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_py_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_ttc_left, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_vx_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_vy_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_existance_probability_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_lane_change_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.lcda_object_type_right, (uint8_t) LCDA_OBJ_TYPE_NONE);
   EXPECT_EQ(lcda_output.lcda_object_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_px_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_py_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_ttc_right, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_vx_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_vy_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.lcda_object_existance_probability_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.lcda_object_existance_probability_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.lcda_object_lane_change_probability_right, FBK_ZERO_UINT);
}

/**
 * Setup data for the existence probability wrapper. Check that a default value is returned when objects existence probability is
 * lower than the default threshold and lane change intention is given. \uts{CSCSA-42962} \sdd{SF-6857} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test,
       Lcda_Set_Existence_Probability__f_use_cvw_lane_change_intention_zone_is_given_and_objects_ex_prob_is_lt_threshold)
{
   /** \arrange Set up tracker data. */
   uint8_t index                                                      = 1u;
   object_data[index].existence_probability                           = 0.9f * customer_cals.k_bmw_sp25_exist_prob_lc_intention;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
   /** \action Call the wrapper for setting of existence probability. */
   Lcda_Set_Existence_Probability(&(lcda_output.lcda_object_existance_probability_left), &lcda_core_input, &customer_cals, index);
   /** \assert Check whether default value is returned. */
   EXPECT_EQ(lcda_output.lcda_object_existance_probability_left,
             (uint16_t) Ml_Roundf(100.0f * customer_cals.k_bmw_sp25_exist_prob_lc_intention));
}
/**
 * Setup data for the existence probability wrapper. Check that the existence probability of tracker is returned when its greater
 * than default and lane change intention is given. \uts{CSCSA-42963} \sdd{SF-6857} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test,
       Lcda_Set_Existence_Probability__f_use_cvw_lane_change_intention_zone_is_given_and_objects_ex_prob_is_gt_threshold)
{
   /** \arrange Set up tracker data. */
   uint8_t index                                                      = 1u;
   object_data[index].existence_probability                           = customer_cals.k_bmw_sp25_exist_prob_lc_intention + EPSILON;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;
   /** \action Call the wrapper for setting of existence probability. */
   Lcda_Set_Existence_Probability(&(lcda_output.lcda_object_existance_probability_left), &lcda_core_input, &customer_cals, index);
   /** \assert Check that trackers existence probability is returned. */
   EXPECT_EQ(lcda_output.lcda_object_existance_probability_left,
             (uint16_t) Ml_Roundf(100.0f * object_data[index].existence_probability));
}
/**
 * Setup data for the existence probability wrapper. No lane change intention is given and thus the trackers existence prob shall
 * be returned. \uts{CSCSA-42964} \sdd{SF-6857} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Existence_Probability__no_lane_change_intention_is_given_thus_trackers_prob_is_expected)
{
   /** \arrange Set up tracker data. */
   uint8_t index                                                      = 1u;
   object_data[index].existence_probability                           = 0.9f * customer_cals.k_bmw_sp25_exist_prob_lc_intention;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;
   /** \action Call the wrapper for setting of existence probability. */
   Lcda_Set_Existence_Probability(&(lcda_output.lcda_object_existance_probability_left), &lcda_core_input, &customer_cals, index);
   /** \assert Check that trackers existence probability is returned. */
   EXPECT_EQ(lcda_output.lcda_object_existance_probability_left,
             (uint16_t) Ml_Roundf(100.0f * object_data[index].existence_probability));
}
/**
 * Check that the transformation to the BMW coordinate system works properly for the right side.
 * \uts{CSCSA-42965} \sdd{SF-6858} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Transformation_To_Bmw_Coord_System_Right__works_properly)
{
   /** \arrange Set up LCDA object in LCDA output with non-zero values. */
   float32_T prev_x_value             = -7.0f;
   float32_T prev_y_value             = 2.5f;
   p_vehicle_data->rear_axle_position = -3.0f;
   lcda_output.lcda_object_px_right   = prev_x_value;
   lcda_output.lcda_object_py_right   = prev_y_value;
   lcda_output.lcda_object_vy_right   = prev_y_value;
   /** \action Call Lcda_Transformation_To_Bmw_Coord_System_Right to transform to BMW coordinate system for right side. */
   Lcda_Transformation_To_Bmw_Coord_System_Right(p_vehicle_data, &lcda_output);
   /** \assert Check that transformation is done correctly. */
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_px_right, prev_x_value - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_py_right, -prev_y_value);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_vy_right, -prev_y_value);
}
/**
 * Check that the transformation to the BMW coordinate system works properly for the left side.
 * \uts{CSCSA-42966} \sdd{SF-6855} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Transformation_To_Bmw_Coord_System_Left__works_properly)
{
   /** \arrange Set up LCDA object in LCDA output with non-zero values. */
   float32_T prev_x_value             = -7.0f;
   float32_T prev_y_value             = 2.5f;
   p_vehicle_data->rear_axle_position = -3.0f;
   lcda_output.lcda_object_px_left    = prev_x_value;
   lcda_output.lcda_object_py_left    = prev_y_value;
   lcda_output.lcda_object_vy_left    = prev_y_value;
   /** \action Call Lcda_Transformation_To_Bmw_Coord_System_Right to transform to BMW coordinate system for left side. */
   Lcda_Transformation_To_Bmw_Coord_System_Left(p_vehicle_data, &lcda_output);
   /** \assert Check that transformation is done correctly. */
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_px_left, prev_x_value - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_py_left, -prev_y_value);
   EXPECT_FLOAT_EQ(lcda_output.lcda_object_vy_left, -prev_y_value);
}
/**
 * Check that in case of an active alert for any submodule the corresponding object ID is filled to the LCDA output.
 * \uts{CSCSA-42967} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__fills_alert_id_to_lcda_output_for_active_alerts)
{
   /** \arrange Set up LCDA core output such that alerts are present for all submodules on both sides. */
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]         = 1u;
   lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]         = 2u;
   lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.elc_core_output.elc_id[FBK_SIDE_LEFT]         = 3u;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]         = 4u;
   lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_LEFT]  = 4u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]        = 5u;
   lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT] = 5u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]        = 6u;
   lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_RIGHT] = 6u;
   lcda_core_output.elc_core_output.elc_id[FBK_SIDE_RIGHT]        = 7u;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]        = 8u;
   lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_RIGHT] = 8u;
   lcda_core_output.lcda_status                                   = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   /** \assert Check that object IDs are set correctly for all active alerts. */
   EXPECT_EQ(lcda_output.bsw_id[FBK_SIDE_LEFT], lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bsw_unique_id[FBK_SIDE_LEFT], lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_LEFT], lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_LEFT], lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.awa_id[FBK_SIDE_LEFT], lcda_core_output.elc_core_output.elc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.slc_id[FBK_SIDE_LEFT], lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.slc_unique_id[FBK_SIDE_LEFT], lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bsw_id[FBK_SIDE_RIGHT], lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bsw_unique_id[FBK_SIDE_RIGHT], lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_RIGHT], lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_RIGHT], lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.awa_id[FBK_SIDE_RIGHT], lcda_core_output.elc_core_output.elc_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.slc_id[FBK_SIDE_RIGHT], lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.slc_unique_id[FBK_SIDE_RIGHT], lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_RIGHT]);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * BSW alert is most critical. \uts{CSCSA-42968} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__sets_lcda_object_type_to_most_critical_alert_case_bsw_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for BSW and all less critical submodules are present on both sides. */
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.lcda_status                               = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   /** \assert Check that object type is the set to BSW. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_BSW);
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_BSW);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * CVW alert is most critical. \uts{CSCSA-42969} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__sets_lcda_object_type_to_most_critical_alert_case_cvw_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for CVW and all less critical submodules are present on both sides. */
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.lcda_status                               = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   /** \assert Check that object type is the set to CVW. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_CVW);
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_CVW);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * SLC alert is most critical. \uts{CSCSA-42970} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__sets_lcda_object_type_to_most_critical_alert_case_slc_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for SLC and all less critical submodules are present on both sides. */
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.lcda_status                               = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   /** \assert Check that object type is the set to SLC. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_SLC);
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_SLC);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * ELC alert is most critical. \uts{CSCSA-42971} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__sets_lcda_object_type_to_most_critical_alert_case_elc_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for ELC and all less critical submodules are present on both sides. */
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.lcda_status                               = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   /** \assert Check that object type is the set to ELC. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_ELC);
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_ELC);
}
/**
 * Check activation condition for china specific logic.
 * \uts{CSCSA-42972} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__run_china_logic)
{
   /** \arrange Set country input to China. */
   lcda_input.f_lcda_enable_cvw_limit_zone = FBK_ONE_UINT;
   lcda_input.f_lcda_enable_bsw_GBT        = FBK_ONE_UINT;
   /** \action Call Lcda_Post_Run. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   /** \assert Check that LCDA is enabled. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, FBK_ONE_UINT);
}

/**
 * Check that in case of an active alert for any submodule the corresponding object ID is filled to the LCDA output.
 * \uts{CSCSA-70064} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__fills_alert_id_to_lcda_output_for_active_alerts_Error_state)
{
   /** \arrange Set up LCDA core output such that alerts are present for all submodules on both sides. */
   *current_state                                                 = LCDA_STATE_ERROR;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT]      = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT]     = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]         = 1u;
   lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]         = 2u;
   lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.elc_core_output.elc_id[FBK_SIDE_LEFT]         = 3u;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]         = 4u;
   lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_LEFT]  = 4u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]        = 5u;
   lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT] = 5u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]        = 6u;
   lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_RIGHT] = 6u;
   lcda_core_output.elc_core_output.elc_id[FBK_SIDE_RIGHT]        = 7u;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]        = 8u;
   lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_RIGHT] = 8u;
   lcda_core_output.lcda_status                                   = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   /** \assert Check that object IDs are set correctly for all active alerts. */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, BSW_FUNCTION_REPORTS_ERROR);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, CVW_FUNCTION_REPORTS_ERROR);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, SLC_FUNCTION_REPORTS_ERROR);
   EXPECT_EQ(lcda_output.bsw_id[FBK_SIDE_LEFT], lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bsw_unique_id[FBK_SIDE_LEFT], lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_LEFT], lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_LEFT], lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.awa_id[FBK_SIDE_LEFT], lcda_core_output.elc_core_output.elc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.slc_id[FBK_SIDE_LEFT], lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.slc_unique_id[FBK_SIDE_LEFT], lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bsw_id[FBK_SIDE_RIGHT], lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bsw_unique_id[FBK_SIDE_RIGHT], lcda_core_output.bsw_core_output.bsw_unique_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_RIGHT], lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_RIGHT], lcda_core_output.cvw_core_output.cvw_unique_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.awa_id[FBK_SIDE_RIGHT], lcda_core_output.elc_core_output.elc_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.slc_id[FBK_SIDE_RIGHT], lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.slc_unique_id[FBK_SIDE_RIGHT], lcda_core_output.slc_core_output.slc_unique_id[FBK_SIDE_RIGHT]);
}

/**
 * Check that in case of an active alert for any submodule the corresponding object ID is filled to the LCDA output.
 * \uts{CSCSA-70065} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__fills_alert_id_to_lcda_output_for_active_alerts_Active_state)
{
   /** \arrange Set up LCDA core output such that alerts are present for all submodules on both sides. */
   *current_state                                             = LCDA_STATE_ACTIVE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]     = 1u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]     = 2u;
   lcda_core_output.elc_core_output.elc_id[FBK_SIDE_LEFT]     = 3u;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]     = 4u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]    = 5u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]    = 6u;
   lcda_core_output.elc_core_output.elc_id[FBK_SIDE_RIGHT]    = 7u;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]    = 8u;
   lcda_core_output.bsw_core_output.bsw_ttp[FBK_SIDE_RIGHT]   = LCDA_DEFAULT_LARGE_TTP;
   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that object IDs are set correctly for all active alerts. */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, VEHICLE_IN_THE_LEFT_BSW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_CVW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, NO_VEHICLE_IN_THE_RIGHT_AND_LEFT_SLC_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right, LCDA_DEFAULT_LARGE_TTP);
}

/**
 * Check that the convertion of boolean flags to unsigned integer works for FBK_TRUE.
 * \uts{CSCSA-42973} \sdd{SF-6918} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Uint8_Flag_From_Boolean__works_properly_for_TRUE)
{
   /** \arrange Set up boolean input flag with value FBK_TRUE. */
   boolean_T flag = FBK_TRUE;
   uint8_t result;
   /** \action Call Lcda_Get_Uint8_Flag_From_Boolean to convert to unsigned integer. */
   result = Lcda_Get_Uint8_Flag_From_Boolean(flag);
   /** \assert Check that FBK_ONE_UINT is returned. */
   EXPECT_EQ(result, FBK_ONE_UINT);
}
/**
 * Check that the convertion of boolean flags to unsigned integer works for FBK_FALSE.
 * \uts{CSCSA-42974} \sdd{SF-6918} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Uint8_Flag_From_Boolean__works_properly_for_FALSE)
{
   /** \arrange Set up boolean input flag with value FBK_FALSE. */
   boolean_T flag = FBK_FALSE;
   uint8_t result;
   /** \action Call Lcda_Get_Uint8_Flag_From_Boolean to convert to unsigned integer. */
   result = Lcda_Get_Uint8_Flag_From_Boolean(flag);
   /** \assert Check that FBK_ZERO_UINT is returned. */
   EXPECT_EQ(result, FBK_ZERO_UINT);
}
/**
 * Check that the function returns false if no trailer is connected.
 * \uts{CSCSA-42975} \sdd{SF-6925} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_false_if_no_trailer_connected)
{
   /** \arrange Set up lcda input, such that flags for trailer mode and trailer connected are set to false. Set up tracker data for
    * checked object with default values. */
   lcda_input.f_lcda_trailer_mode      = FBK_ZERO_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ZERO_UINT;
   uint8_t object_index                = 1u;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns false. */
   EXPECT_FALSE(result);
}


/**
 * Check that the function returns false if trailer mode enabled but no trailer is connected.
 * \uts{CSCSA-99058} \sdd{SF-6925} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_false_if_mode_enabled_trailer_disconnected)
{
   /** \arrange Set up lcda input, such that flags are trailer mode true and trailer connected false. Set up tracker data for
    * checked object with default values. */
   lcda_input.f_lcda_trailer_mode      = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ZERO_UINT;
   uint8_t object_index                = 1u;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns false. */
   EXPECT_FALSE(result);
}

/**
 * Check that the function returns false if trailer mode disabled while trailer is connected.
 * \uts{CSCSA-99059} \sdd{SF-6925} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_false_if_mode_disabled_trailer_connected)
{
   /** \arrange Set up lcda input, such that flags are trailer mode false and trailer connected true. Set up tracker data for
    * checked object with default values. */
   lcda_input.f_lcda_trailer_mode      = FBK_ZERO_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ONE_UINT;
   uint8_t object_index                = 1u;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns false. */
   EXPECT_FALSE(result);
}


/**
 * Check that the function returns false if default object index is given.
 * \uts{CSCSA-42976} \sdd{SF-6925} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_false_for_default_object_index)
{
   /** \arrange Set up lcda input, such that flags for trailer mode and trailer connected are set to true. Set object index for
    * object that will be checked to the default object index. */
   lcda_input.f_lcda_trailer_mode      = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ONE_UINT;
   uint8_t object_index                = PA_INVALID_OBJ_INDEX;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns false. */
   EXPECT_FALSE(result);
}
/**
 * Check that the function returns true if given object is likely a trailer given the fact that a trailer is attached and an object
 * is positioned in the usual trailer area. \uts{CSCSA-42977} \sdd{SF-6925} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_true_for_trailer_object)
{
   /** \arrange Set up lcda input, such that flags for trailer mode and trailer connected are set to true. Set up tracker data for
    * checked object such that object is positioned in the usual trailer area. */
   lcda_input.f_lcda_trailer_mode      = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ONE_UINT;
   uint8_t object_index                = 1u;
   p_vehicle_data->host_length         = 5.0f;
   p_vehicle_data->host_width          = 2.0f;
   object_data[object_index].vcs_pos.x =
      -(p_vehicle_data->host_length + 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_trailer_length);
   object_data[object_index].vcs_pos.y = -0.1f;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns true. */
   EXPECT_TRUE(result);
}
/**
 * Check that the function returns true if given object is likely a bike carrier given the fact that a trailer is attached and an
 * object is positioned in the usual bike carrier area. \uts{CSCSA-42978} \sdd{SF-6925} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_true_for_bike_carrier_object)
{
   /** \arrange Set up lcda input, such that flags for trailer mode and trailer connected are set to true. Set up tracker data for
    * checked object such that object is positioned in the usual bike carrier area. */
   lcda_input.f_lcda_trailer_mode      = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ONE_UINT;
   uint8_t object_index                = 1u;
   p_vehicle_data->host_length         = 5.0f;
   p_vehicle_data->host_width          = 2.0f;
   object_data[object_index].width     = 1.0f;
   object_data[object_index].length    = 2.0f;
   object_data[object_index].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[object_index].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[object_index].width;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns true. */
   EXPECT_TRUE(result);
}
/**
 * Check that the function returns false if given object is outside of trailer area and bike carrier area longitudinally.
 * \uts{CSCSA-42979} \sdd{SF-6925} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_false_for_object_outside_of_area_longitudinally)
{
   /** \arrange Set up lcda input, such that flags for trailer mode and trailer connected are set to true. Set up tracker data for
    * checked object such that object is positioned outside of the area because of the longitudinal coordinate. */
   lcda_input.f_lcda_trailer_mode      = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ONE_UINT;
   uint8_t object_index                = 1u;
   p_vehicle_data->host_length         = 5.0f;
   p_vehicle_data->host_width          = 2.0f;
   object_data[object_index].vcs_pos.x =
      -(p_vehicle_data->host_length + customer_cals.k_bmw_sp25_trailer_mode_max_trailer_length + EPSILON);
   object_data[object_index].vcs_pos.y = -0.1f;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns false. */
   EXPECT_FALSE(result);
}
/**
 * Check that the function returns false if given object is outside of trailer area laterally and outside of bike carrier area
 * longitudinally. \uts{CSCSA-42980} \sdd{SF-6925} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_false_for_object_outside_of_trailer_area_laterally)
{
   /** \arrange Set up lcda input, such that flags for trailer mode and trailer connected are set to true. Set up tracker data for
    * checked object such that object is positioned outside of the trailer area because of the lateral coordinate. */
   lcda_input.f_lcda_trailer_mode      = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ONE_UINT;
   uint8_t object_index                = 1u;
   p_vehicle_data->host_length         = 5.0f;
   p_vehicle_data->host_width          = 2.0f;
   object_data[object_index].vcs_pos.x = -8.0f;
   object_data[object_index].vcs_pos.y = p_vehicle_data->host_width + EPSILON;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns false. */
   EXPECT_FALSE(result);
}
/**
 * Check that the function returns false if given object is outside of bike carrier area laterally.
 * \uts{CSCSA-42981} \sdd{SF-6925} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Object_Trailer__return_false_for_object_outside_of_bike_carrier_area_laterally)
{
   /** \arrange Set up lcda input, such that flags for trailer mode and trailer connected are set to true. Set up tracker data for
    * checked object such that object is positioned outside of the bike carrier area because of the lateral coordinate. */
   lcda_input.f_lcda_trailer_mode      = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected = FBK_ONE_UINT;
   uint8_t object_index                = 1u;
   p_vehicle_data->host_length         = 5.0f;
   p_vehicle_data->host_width          = 2.0f;
   object_data[object_index].width     = customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_buffer * 2.0f;
   object_data[object_index].length    = 2.0f;
   object_data[object_index].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[object_index].vcs_pos.y =
      0.5f * p_vehicle_data->host_width + customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_buffer + EPSILON;
   /** \action Call function Lcda_Is_Object_Trailer to check if the given object is the trailer or bike carrier. */
   boolean_T result = Lcda_Is_Object_Trailer(&lcda_input, &data, object_index, &customer_cals);
   /** \assert Verify that the function returns false. */
   EXPECT_FALSE(result);
}
/**
 * Check that the CVW alert is unchanged when no CVW alert is active.
 * \uts{CSCSA-42982} \sdd{SF-6926} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Limit_Cvw_Alert_By_Zone_Length__cvw_output_unchanged_for_no_alert)
{
   /** \arrange Set up function inputs. */
   uint8_t cvw_index[FBK_NUMBER_OF_SIDES]              = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};
   boolean_T f_previous_cvw_alert[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};
   p_vehicle_data->host_length                         = 5.0f;

   lcda_input.f_lcda_enable_cvw_limit_zone           = FBK_ONE_UINT;
   lcda_input.lcda_cvw_limit_zone_range              = 29u;
   customer_cals.k_bmw_sp25_cvw_limit_zone_range_hys = 1.0f;

   lcda_output.cvw_alert[FBK_SIDE_LEFT]      = FBK_ZERO_UINT;
   lcda_output.cvw_id[FBK_SIDE_LEFT]         = PA_INVALID_OBJ_ID;
   lcda_output.cvw_unique_id[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_ID;
   lcda_output.cvw_alert[FBK_SIDE_RIGHT]     = FBK_ZERO_UINT;
   lcda_output.cvw_id[FBK_SIDE_RIGHT]        = PA_INVALID_OBJ_ID;
   lcda_output.cvw_unique_id[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_ID;

   /** \action Call Lcda_Limit_Cvw_Alert_By_Zone_Length. */
   Lcda_Limit_Cvw_Alert_By_Zone_Length(&lcda_output, &lcda_input, &data, cvw_index, f_previous_cvw_alert, &customer_cals);
   /** \assert Check that all CVW outputs are set correctly. */
   EXPECT_EQ(lcda_output.cvw_alert[FBK_SIDE_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_LEFT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
}
/**
 * Check that the CVW alert is suppressed for objects further away than the threshold.
 * \uts{CSCSA-42983} \sdd{SF-6926} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Limit_Cvw_Alert_By_Zone_Length__suppress_cvw_alert_for_far_away_object)
{
   /** \arrange Set up function inputs. */
   uint8_t object_index                                = 1u;
   uint8_t cvw_index[FBK_NUMBER_OF_SIDES]              = {PA_INVALID_OBJ_INDEX, object_index};
   boolean_T f_previous_cvw_alert[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_FALSE};
   p_vehicle_data->host_length                         = 5.0f;

   lcda_input.f_lcda_enable_cvw_limit_zone           = FBK_ONE_UINT;
   lcda_input.lcda_cvw_limit_zone_range              = 29u;
   customer_cals.k_bmw_sp25_cvw_limit_zone_range_hys = 1.0f;

   object_data[object_index].status      = PA_OBJ_STATUS_MATURE;
   object_data[object_index].id          = object_index + 1u;
   object_data[object_index].length      = 5.0f;
   object_data[object_index].curvi_pos.x = (float32_T) lcda_input.lcda_cvw_limit_zone_range
                                           + Fbk_Half(object_data[object_index].length) + p_vehicle_data->host_length + EPSILON;
   lcda_output.cvw_alert[FBK_SIDE_RIGHT]     = FBK_ONE_UINT;
   lcda_output.cvw_id[FBK_SIDE_RIGHT]        = object_data[object_index].id;
   lcda_output.cvw_unique_id[FBK_SIDE_RIGHT] = object_data[object_index].unique_id;
   /** \action Call Lcda_Limit_Cvw_Alert_By_Zone_Length. */
   Lcda_Limit_Cvw_Alert_By_Zone_Length(&lcda_output, &lcda_input, &data, cvw_index, f_previous_cvw_alert, &customer_cals);
   /** \assert Check that all CVW outputs are set correctly. */
   EXPECT_EQ(lcda_output.cvw_alert[FBK_SIDE_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
}
/**
 * Check that the CVW alert is not suppressed for objects closer than the threshold.
 * \uts{CSCSA-42984} \sdd{SF-6926} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Limit_Cvw_Alert_By_Zone_Length__cvw_alert_unchanged_for_near_object)
{
   /** \arrange Set up function inputs. */
   uint8_t object_index                                = 1u;
   uint8_t cvw_index[FBK_NUMBER_OF_SIDES]              = {PA_INVALID_OBJ_INDEX, object_index};
   boolean_T f_previous_cvw_alert[FBK_NUMBER_OF_SIDES] = {FBK_FALSE, FBK_TRUE};
   p_vehicle_data->host_length                         = 5.0f;

   lcda_input.f_lcda_enable_cvw_limit_zone           = FBK_ONE_UINT;
   lcda_input.lcda_cvw_limit_zone_range              = 29u;
   customer_cals.k_bmw_sp25_cvw_limit_zone_range_hys = 1.0f;

   object_data[object_index].status      = PA_OBJ_STATUS_MATURE;
   object_data[object_index].id          = object_index + 1u;
   object_data[object_index].length      = 5.0f;
   object_data[object_index].curvi_pos.x = (float32_T) lcda_input.lcda_cvw_limit_zone_range
                                           - customer_cals.k_bmw_sp25_cvw_limit_zone_range_hys
                                           + Fbk_Half(object_data[object_index].length) + p_vehicle_data->host_length - EPSILON;
   lcda_output.cvw_alert[FBK_SIDE_RIGHT]     = FBK_ONE_UINT;
   lcda_output.cvw_id[FBK_SIDE_RIGHT]        = object_data[object_index].id;
   lcda_output.cvw_unique_id[FBK_SIDE_RIGHT] = object_data[object_index].unique_id;
   /** \action Call Lcda_Limit_Cvw_Alert_By_Zone_Length. */
   Lcda_Limit_Cvw_Alert_By_Zone_Length(&lcda_output, &lcda_input, &data, cvw_index, f_previous_cvw_alert, &customer_cals);
   /** \assert Check that all CVW outputs are set correctly. */
   EXPECT_EQ(lcda_output.cvw_alert[FBK_SIDE_RIGHT], FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.cvw_id[FBK_SIDE_RIGHT], object_data[object_index].id);
   EXPECT_EQ(lcda_output.cvw_unique_id[FBK_SIDE_RIGHT], object_data[object_index].unique_id);
}
/**
 * Check that the BSW alert related customer outputs are set.
 * \uts{CSCSA-42985} \sdd{SF-6957} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bsw_Alert__alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ZERO_UINT;
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES]           = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};
   lcda_core_output.bsw_core_output.bsw_alert[side] = LCDA_ALERT_STATE_LEVEL_2;
   /** \action Call Lcda_Set_Bsw_Alert. */
   Lcda_Set_Bsw_Alert(&lcda_input, &data, &lcda_core_output, &customer_cals, side, &lcda_output, bsw_index);
   /** \assert Check that all BSW alert related outputs are set correctly. */
   EXPECT_EQ((uint8_t) lcda_core_output.bsw_core_output.bsw_alert[side], lcda_output.bsw_alert[side]);
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_id[side], lcda_output.bsw_id[side]);
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_unique_id[side], lcda_output.bsw_unique_id[side]);
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_index[side], bsw_index[side]);
}
/**
 * Check that the BSW alert related customer outputs are not set.
 * \uts{CSCSA-42986} \sdd{SF-6957} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bsw_Alert__no_alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ONE_UINT;
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES]           = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};
   lcda_core_output.bsw_core_output.bsw_alert[side] = LCDA_ALERT_STATE_NONE;
   /** \action Call Lcda_Set_Bsw_Alert. */
   Lcda_Set_Bsw_Alert(&lcda_input, &data, &lcda_core_output, &customer_cals, side, &lcda_output, bsw_index);
   /** \assert Check that all BSW alert related outputs are set correctly. */
   EXPECT_EQ(FBK_ZERO_UINT, lcda_output.bsw_alert[side]);
   EXPECT_EQ(PA_INVALID_OBJ_ID, lcda_output.bsw_id[side]);
   EXPECT_EQ(PA_INVALID_OBJ_ID, lcda_output.bsw_unique_id[side]);
   EXPECT_EQ(PA_INVALID_OBJ_INDEX, bsw_index[side]);
}

/**
 * Check that the BSW alert related customer outputs are set.
 * \uts{CSCSA-70066} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bsw_Alert__alert_tailer_object)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ZERO_UINT;
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES]           = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};
   lcda_core_output.bsw_core_output.bsw_alert[side] = LCDA_ALERT_STATE_LEVEL_2;
   lcda_input.f_lcda_trailer_mode                   = FBK_ONE_UINT;
   lcda_input.f_lcda_trailer_connected              = FBK_ONE_UINT;

   /** \action Call Lcda_Set_Bsw_Alert. */
   Lcda_Set_Bsw_Alert(&lcda_input, &data, &lcda_core_output, &customer_cals, side, &lcda_output, bsw_index);

   /** \assert Check that all BSW alert related outputs are set correctly. */
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_id[side], lcda_output.bsw_id[side]);
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_unique_id[side], lcda_output.bsw_unique_id[side]);
}

/**
 * Check that the CVW alert related customer outputs are set.
 * \uts{CSCSA-42987} \sdd{SF-6958} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Cvw_Alert__alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ZERO_UINT;
   uint8_t cvw_index[FBK_NUMBER_OF_SIDES]           = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};
   lcda_core_output.cvw_core_output.cvw_alert[side] = LCDA_ALERT_STATE_LEVEL_2;
   /** \action Call Lcda_Set_Cvw_Alert. */
   Lcda_Set_Cvw_Alert(&lcda_core_output, side, &lcda_output, cvw_index);
   /** \assert Check that all CVW alert related outputs are set correctly. */
   EXPECT_EQ((uint8_t) lcda_core_output.cvw_core_output.cvw_alert[side], lcda_output.cvw_alert[side]);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_id[side], lcda_output.cvw_id[side]);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_unique_id[side], lcda_output.cvw_unique_id[side]);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_index[side], cvw_index[side]);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc[side], lcda_core_output.cvw_core_output.cvw_ttc[side]);
}
/**
 * Check that the CVW alert related customer outputs are not set.
 * \uts{CSCSA-42988} \sdd{SF-6958} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Cvw_Alert__no_alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ONE_UINT;
   uint8_t cvw_index[FBK_NUMBER_OF_SIDES]           = {PA_INVALID_OBJ_INDEX, PA_INVALID_OBJ_INDEX};
   lcda_core_output.cvw_core_output.cvw_alert[side] = LCDA_ALERT_STATE_NONE;
   /** \action Call Lcda_Set_Cvw_Alert. */
   Lcda_Set_Cvw_Alert(&lcda_core_output, side, &lcda_output, cvw_index);
   /** \assert Check that all CVW alert related outputs are set correctly. */
   EXPECT_EQ(FBK_ZERO_UINT, lcda_output.cvw_alert[side]);
   EXPECT_EQ(PA_INVALID_OBJ_ID, lcda_output.cvw_id[side]);
   EXPECT_EQ(PA_INVALID_OBJ_ID, lcda_output.cvw_unique_id[side]);
   EXPECT_EQ(PA_INVALID_OBJ_INDEX, cvw_index[side]);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc[side], lcda_core_output.cvw_core_output.cvw_ttc[side]);
}
/**
 * Check that the SLC alert related customer outputs are set.
 * \uts{CSCSA-42989} \sdd{SF-6959} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Slc_Alert__alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ZERO_UINT;
   lcda_core_output.slc_core_output.slc_alert[side] = FBK_TRUE;
   /** \action Call Lcda_Set_Slc_Alert. */
   Lcda_Set_Slc_Alert(&lcda_core_output, side, &lcda_output);
   /** \assert Check that all SLC alert related outputs are set correctly. */
   EXPECT_EQ(FBK_ONE_UINT, lcda_output.slc_alert[side]);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_id[side], lcda_output.slc_id[side]);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_unique_id[side], lcda_output.slc_unique_id[side]);
   EXPECT_FLOAT_EQ(lcda_output.slc_ttc[side], lcda_core_output.slc_core_output.slc_lat_ttc[side]);
   EXPECT_FLOAT_EQ(lcda_output.slc_lane_change_probability[side], lcda_core_output.slc_core_output.slc_lane_change_prob[side]);
}
/**
 * Check that the SLC alert related customer outputs are not set.
 * \uts{CSCSA-42990} \sdd{SF-6959} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Slc_Alert__no_alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ONE_UINT;
   lcda_core_output.slc_core_output.slc_alert[side] = LCDA_ALERT_STATE_NONE;
   /** \action Call Lcda_Set_Slc_Alert. */
   Lcda_Set_Slc_Alert(&lcda_core_output, side, &lcda_output);
   /** \assert Check that all SLC alert related outputs are set correctly. */
   EXPECT_EQ(FBK_ZERO_UINT, lcda_output.slc_alert[side]);
   EXPECT_EQ(PA_INVALID_OBJ_ID, lcda_output.slc_id[side]);
   EXPECT_EQ(PA_INVALID_OBJ_ID, lcda_output.slc_unique_id[side]);
   EXPECT_FLOAT_EQ(lcda_output.slc_ttc[side], lcda_core_output.slc_core_output.slc_lat_ttc[side]);
   EXPECT_FLOAT_EQ(lcda_output.slc_lane_change_probability[side], lcda_core_output.slc_core_output.slc_lane_change_prob[side]);
}
/**
 * Check that the AWA alert related customer outputs are set.
 * \uts{CSCSA-42991} \sdd{SF-6960} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Awa_Alert__alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ZERO_UINT;
   lcda_core_output.elc_core_output.elc_alert[side] = FBK_TRUE;
   /** \action Call Lcda_Set_Awa_Alert. */
   Lcda_Set_Awa_Alert(&lcda_core_output, side, &lcda_output);
   /** \assert Check that all AWA alert related outputs are set correctly. */
   EXPECT_EQ(FBK_ONE_UINT, lcda_output.awa_alert[side]);
   EXPECT_EQ(lcda_core_output.elc_core_output.elc_id[side], lcda_output.awa_id[side]);
   EXPECT_FLOAT_EQ(lcda_output.awa_ttc[side], lcda_core_output.elc_core_output.elc_ttc[side]);
   EXPECT_FLOAT_EQ(lcda_output.awa_dec[side], lcda_core_output.elc_core_output.elc_decel_to_reach_host_speed[side]);
}
/**
 * Check that the AWA alert related customer outputs are not set.
 * \uts{CSCSA-42992} \sdd{SF-6960} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Awa_Alert__no_alert)
{
   /** \arrange Set up function inputs. */
   uint8_t side                                     = FBK_ONE_UINT;
   lcda_core_output.elc_core_output.elc_alert[side] = LCDA_ALERT_STATE_NONE;
   /** \action Call Lcda_Set_Awa_Alert. */
   Lcda_Set_Awa_Alert(&lcda_core_output, side, &lcda_output);
   /** \assert Check that all AWA alert related outputs are set correctly. */
   EXPECT_EQ(FBK_ZERO_UINT, lcda_output.awa_alert[side]);
   EXPECT_EQ(PA_INVALID_OBJ_ID, lcda_output.awa_id[side]);
   EXPECT_FLOAT_EQ(lcda_output.awa_ttc[side], lcda_core_output.elc_core_output.elc_ttc[side]);
   EXPECT_FLOAT_EQ(lcda_output.awa_dec[side], lcda_core_output.elc_core_output.elc_decel_to_reach_host_speed[side]);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * BSW alert is most critical. \uts{CSCSA-42993} \sdd{SF-6961} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Left__sets_lcda_object_type_to_most_critical_alert_case_bsw_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for BSW and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.bsw_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.cvw_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_NONE;
   lcda_output.awa_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_NONE;
   lcda_output.slc_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_NONE;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Left to fill LCDA output from core output. */
   Lcda_Fill_Object_Left(&data, &lcda_core_input, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to BSW. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_BSW);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * CVW alert is most critical. \uts{CSCSA-42994} \sdd{SF-6961} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Left__sets_lcda_object_type_to_most_critical_alert_case_cvw_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for CVW and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.cvw_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.awa_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_NONE;
   lcda_output.slc_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_NONE;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Left to fill LCDA output from core output. */
   Lcda_Fill_Object_Left(&data, &lcda_core_input, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to CVW. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_CVW);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * SLC alert is most critical. \uts{CSCSA-42995} \sdd{SF-6961} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Left__sets_lcda_object_type_to_most_critical_alert_case_slc_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for SLC and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.awa_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_NONE;
   lcda_output.slc_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Left to fill LCDA output from core output. */
   Lcda_Fill_Object_Left(&data, &lcda_core_input, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to SLC. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_SLC);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * ELC alert is most critical. \uts{CSCSA-42996} \sdd{SF-6961} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Left__sets_lcda_object_type_to_most_critical_alert_case_elc_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for ELC and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.awa_alert[FBK_SIDE_LEFT]   = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Left to fill LCDA output from core output. */
   Lcda_Fill_Object_Left(&data, &lcda_core_input, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to ELC. */
   EXPECT_EQ(lcda_output.lcda_object_type_left, LCDA_OBJ_TYPE_ELC);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * BSW alert is most critical. \uts{CSCSA-42997} \sdd{SF-6962} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Right__sets_lcda_object_type_to_most_critical_alert_case_bsw_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for BSW and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.bsw_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.cvw_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_NONE;
   lcda_output.awa_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_NONE;
   lcda_output.slc_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Right to fill LCDA output from core output. */
   Lcda_Fill_Object_Right(&lcda_core_input, p_vehicle_data, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to BSW. */
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_BSW);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * CVW alert is most critical. \uts{CSCSA-42998} \sdd{SF-6962} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Right__sets_lcda_object_type_to_most_critical_alert_case_cvw_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for CVW and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.cvw_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.awa_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_NONE;
   lcda_output.slc_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Right to fill LCDA output from core output. */
   Lcda_Fill_Object_Right(&lcda_core_input, p_vehicle_data, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to CVW. */
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_CVW);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * SLC alert is most critical. \uts{CSCSA-42999} \sdd{SF-6962} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Right__sets_lcda_object_type_to_most_critical_alert_case_slc_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for SLC and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.awa_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_NONE;
   lcda_output.slc_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Right to fill LCDA output from core output. */
   Lcda_Fill_Object_Right(&lcda_core_input, p_vehicle_data, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to SLC. */
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_SLC);
}
/**
 * Check that the object type of the LCDA object from LCDA output is set to the most critical alert type that is present in case
 * ELC alert is most critical. \uts{CSCSA-43000} \sdd{SF-6962} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Fill_Object_Right__sets_lcda_object_type_to_most_critical_alert_case_elc_most_critical)
{
   /** \arrange Set up LCDA core output such that alerts for ELC and all less critical submodules are present on both sides. */
   uint8_t bsw_index[FBK_NUMBER_OF_SIDES] = {FBK_ZERO_UINT, FBK_ZERO_UINT};
   lcda_output.awa_alert[FBK_SIDE_RIGHT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.lcda_status           = LCDA_STATUS_ACTIVE;
   /** \action Call Lcda_Fill_Object_Right to fill LCDA output from core output. */
   Lcda_Fill_Object_Right(&lcda_core_input, p_vehicle_data, &lcda_core_output, &customer_cals, bsw_index, &lcda_output);
   /** \assert Check that object type is the set to ELC. */
   EXPECT_EQ(lcda_output.lcda_object_type_right, LCDA_OBJ_TYPE_ELC);
}
/**
 * Check that the object type of the LCDA object and set the LCDA output
 * \uts{CSCSA-70067} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Reset_Bmw_LCDA_Output_Bus_Signals__Lcda_reset_output_signal)
{

   /** \arrange data to test Lcda_Reset_Bmw_LCDA_Output_Bus_Signals function */
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour           = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute         = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second         = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left                       = FBK_ONE_UINT;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left               = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left               = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left                    = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left                   = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left                      = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left                      = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left                     = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left               = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left               = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left    = FBK_ONE_UINT;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left  = FBK_ONE_UINT;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour          = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute        = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second        = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right                      = FBK_ONE_UINT;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right              = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right              = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right                   = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right                  = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right                     = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right                     = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right                    = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right              = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right              = FBK_ONE_F;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right   = FBK_ONE_UINT;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right = FBK_ONE_UINT;
   /** \action Call Lcda_Reset_Bmw_LCDA_Output_Bus_Signals */
   Lcda_Reset_Bmw_LCDA_Output_Bus_Signals(&lcda_output);
   /** \assert check all mappings */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left, LCDA_DEFAULT_LARGE_TTP);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left, LCDA_DEFAULT_LARGE_TTLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right, LCDA_DEFAULT_LARGE_TTP);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right, LCDA_DEFAULT_LARGE_TTLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right, FBK_ZERO_UINT);
}
/**
 * Check that the object type of the LCDA object and set the LCDA output
 * \uts{CSCSA-70068} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Reset_Bmw_LCDA_Output_Bus_Signals_Qualifiers_And_FF_Status__Lcda_reset_output_signal_Qualifiers)
{
   /** \arrange data to test Lcda_Reset_Bmw_LCDA_Output_Bus_Signals function */
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier = EVENT_DATA_AVAILABLE_REDUCED;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier   = STAND_BY;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw    = BSW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw    = CVW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES;
   lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc    = SLC_ZONE_OUTSIDE_SYSTEM_BOUNDARIES;
   /** \action Call Lcda_Reset_Bmw_LCDA_Output_Bus_Signals_Qualifiers_And_FF_Status */
   Lcda_Reset_Bmw_LCDA_Output_Bus_Signals_Qualifiers_And_FF_Status(&lcda_output);
   /** \assert check all mappings */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier,
             EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT); // need to be confirmed
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, BSW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, CVW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, SLC_FUNCTION_INTERFACE_IS_NOT_AVAILABLE);
}
/**
 * Change the Output object according to the Functinal States if Not_Available
 * \uts{CSCSA-70069} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State__When_Not_Available)
{
   /** \arrange Data To Check signal values when LCW FF is in INACTIVE state */
   lcda_output.bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state = LCDA_STATE_NOT_AVAILABLE;

   /**\action Call Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State */
   Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State(&lcda_output);

   /** \assert Check all signal Mappings */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier,
             EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT); // need to be confirmed
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, BSW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, CVW_FUNCTION_INTERFACE_IS_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, SLC_FUNCTION_INTERFACE_IS_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right, FBK_ZERO_UINT);
}
/**
 * Change the Output object according to the Functinal States if Inactive
 * \uts{CSCSA-70070} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State__When_Inactive)
{
   /** \arrange set lcw feature function state to active */
   lcda_output.bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state = LCDA_STATE_INACTIVE;

   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State(&lcda_output);

   /** \assert Check all signal Mappings */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier,
             EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT); // need to be confirmed
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, BSW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, CVW_ZONE_OUTSIDE_SYSTEM_BOUNDARIES);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, SLC_ZONE_OUTSIDE_SYSTEM_BOUNDARIES);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left, LCDA_DEFAULT_LARGE_TTP);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left, LCDA_DEFAULT_LARGE_TTLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right, LCDA_DEFAULT_LARGE_TTC);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right, LCDA_DEFAULT_LARGE_TTP);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right, LCDA_DEFAULT_LARGE_TTLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right, FBK_ZERO_UINT);
}
/**
 * Change the Output object according to the Functinal States if Error
 * \uts{CSCSA-70071} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State__When_Error)
{
   /** \arrange setting function state to Error State */
   lcda_output.bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state = LCDA_STATE_ERROR;

   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State(&lcda_output);

   /** \assert Check all signal Mappings */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_NOT_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, EXTENDED_QUALIFIER_EVENT_DATA_INVALID_OR_TIMEOUT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, BSW_FUNCTION_REPORTS_ERROR);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, CVW_FUNCTION_REPORTS_ERROR);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, SLC_FUNCTION_REPORTS_ERROR);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right, FBK_ZERO_UINT);
}

/**
 * Change the Output object according to the Functinal States if Active
 * \uts{CSCSA-70072} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State__When_Avtive)
{
   /** \arrange setting function state to Error State */
   lcda_output.bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state = LCDA_STATE_ACTIVE;

   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_Not_In_Active_State(&lcda_output);

   /** \assert Check all signal Mappings */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state, LCDA_STATE_ACTIVE);
}

/**
 * Change the Output object according to the Functinal States is in Active when BSW Enabled.
 * \uts{CSCSA-70073} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Active_State__BSW)
{
   /** \arrange data to check signal output when in Active State */
   p_vehicle_data->rear_axle_position                         = 0;
   lcda_output.bsw_alert[FBK_SIDE_RIGHT]                      = FBK_ONE_UINT;
   lcda_output.bsw_alert[FBK_SIDE_LEFT]                       = FBK_ONE_UINT;
   uint8_t bsw_object_index_right                             = 1u;
   uint8_t bsw_object_index_left                              = 2u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]    = bsw_object_index_right;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]     = bsw_object_index_left;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = bsw_object_index_right;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT]  = bsw_object_index_left;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   object_data[bsw_object_index_right].width  = 1.0f;
   object_data[bsw_object_index_right].length = 2.0f;
   object_data[bsw_object_index_right].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[bsw_object_index_right].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[bsw_object_index_right].width;
   object_data[bsw_object_index_right].vcs_vel.x = 10.0f;
   object_data[bsw_object_index_right].vcs_vel.y = 10.0f;

   object_data[bsw_object_index_left].width  = 1.0f;
   object_data[bsw_object_index_left].length = 2.0f;
   object_data[bsw_object_index_left].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[bsw_object_index_left].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[bsw_object_index_left].width;
   object_data[bsw_object_index_left].vcs_vel.x = 20.0f;
   object_data[bsw_object_index_left].vcs_vel.y = 20.0f;

   lcda_output.lcda_object_existance_probability_right = 0;
   lcda_output.lcda_object_existance_probability_left  = 0;

   lcda_output.lcda_object_lane_change_probability_right = FBK_ZERO_UINT;
   lcda_output.lcda_object_lane_change_probability_left  = FBK_ZERO_UINT;


   data.object_data[bsw_object_index_left]  = object_data[bsw_object_index_left];
   data.object_data[bsw_object_index_right] = object_data[bsw_object_index_right];

   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(&data, &lcda_output, &lcda_core_output);
   /** \assert Check all signal Mappings */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, VEHICLE_IN_THE_LEFT_BSW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right,
             lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right, object_data[bsw_object_index_right].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right,
             (-1 * object_data[bsw_object_index_right].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right, object_data[bsw_object_index_right].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right,
             (-1 * object_data[bsw_object_index_right].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right, object_data[bsw_object_index_right].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right, object_data[bsw_object_index_right].length);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right,
             lcda_output.lcda_object_existance_probability_right);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right,
             lcda_output.lcda_object_lane_change_probability_right);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left, lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left, object_data[bsw_object_index_left].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left,
             (-1 * object_data[bsw_object_index_left].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left, object_data[bsw_object_index_left].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left,
             (-1 * object_data[bsw_object_index_left].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left, object_data[bsw_object_index_left].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left, object_data[bsw_object_index_left].length);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left,
             lcda_output.lcda_object_existance_probability_left);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left,
             lcda_output.lcda_object_lane_change_probability_left);
}
/**
 * Change the Output object according to the Functinal States is in Active when CVW Enabled.
 * \uts{CSCSA-70074} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Active_State__CVW)
{
   /** \arrange data to check signal mapping when in Active State */
   p_vehicle_data->rear_axle_position    = 0;
   lcda_output.cvw_alert[FBK_SIDE_RIGHT] = FBK_ONE_UINT;
   lcda_output.cvw_alert[FBK_SIDE_LEFT]  = FBK_ONE_UINT;

   uint8_t cvw_object_index_right                             = 1u;
   uint8_t cvw_object_index_left                              = 2u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]    = cvw_object_index_right;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]     = cvw_object_index_left;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_RIGHT] = cvw_object_index_right;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_LEFT]  = cvw_object_index_left;
   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT]   = 1.2f;
   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_LEFT]    = 2.0f;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   object_data[cvw_object_index_right].width  = 1.0f;
   object_data[cvw_object_index_right].length = 2.0f;
   object_data[cvw_object_index_right].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[cvw_object_index_right].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[cvw_object_index_right].width;
   object_data[cvw_object_index_right].vcs_vel.x = 10.0f;
   object_data[cvw_object_index_right].vcs_vel.y = 10.0f;

   object_data[cvw_object_index_left].width  = 1.0f;
   object_data[cvw_object_index_left].length = 2.0f;
   object_data[cvw_object_index_left].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[cvw_object_index_left].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[cvw_object_index_left].width;
   object_data[cvw_object_index_left].vcs_vel.x = 20.0f;
   object_data[cvw_object_index_left].vcs_vel.y = 20.0f;

   data.object_data[cvw_object_index_left]  = object_data[cvw_object_index_left];
   data.object_data[cvw_object_index_right] = object_data[cvw_object_index_right];

   lcda_output.lcda_object_existance_probability_right   = FBK_ZERO_UINT;
   lcda_output.lcda_object_lane_change_probability_right = FBK_ZERO_UINT;

   lcda_output.lcda_object_existance_probability_left   = FBK_ZERO_UINT;
   lcda_output.lcda_object_lane_change_probability_left = FBK_ZERO_UINT;
   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(&data, &lcda_output, &lcda_core_output);
   /** \assert Check all signal mapping */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, VEHICLE_IN_THE_LEFT_CVW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_cvw,
             lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_cvw,
             object_data[cvw_object_index_right].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_cvw,
             (-1 * object_data[cvw_object_index_right].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_cvw,
             lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_cvw,
             object_data[cvw_object_index_right].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_cvw,
             (-1 * object_data[cvw_object_index_right].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_cvw, object_data[cvw_object_index_right].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_cvw, object_data[cvw_object_index_right].length);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_cvw,
             lcda_output.lcda_object_existance_probability_right);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_cvw,
             lcda_output.lcda_object_lane_change_probability_right);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_cvw,
             lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_cvw,
             object_data[cvw_object_index_left].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_cvw,
             (-1 * object_data[cvw_object_index_left].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_cvw,
             lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_cvw,
             object_data[cvw_object_index_left].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_cvw,
             (-1 * object_data[cvw_object_index_left].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_cvw, object_data[cvw_object_index_left].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_cvw, object_data[cvw_object_index_left].length);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_cvw,
             lcda_output.lcda_object_existance_probability_left);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_cvw,
             lcda_output.lcda_object_lane_change_probability_left);
}
/**
 * Change the Output object according to the Functinal States is in Active when SLC Enabled.
 * \uts{CSCSA-70075} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Active_State__SLC)
{
   /** \arrange data to check signal mapping when in Active State */
   p_vehicle_data->rear_axle_position    = 0;
   lcda_output.slc_alert[FBK_SIDE_RIGHT] = FBK_ONE_UINT;
   lcda_output.slc_alert[FBK_SIDE_LEFT]  = FBK_ONE_UINT;

   uint8_t slc_object_index_right                               = 1u;
   uint8_t slc_object_index_left                                = 2u;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]      = slc_object_index_right;
   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]       = slc_object_index_left;
   lcda_core_output.slc_core_output.slc_index[FBK_SIDE_RIGHT]   = slc_object_index_right;
   lcda_core_output.slc_core_output.slc_index[FBK_SIDE_LEFT]    = slc_object_index_left;
   lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT] = 1.2f;
   lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT]  = 2.0f;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   object_data[slc_object_index_right].width  = 1.0f;
   object_data[slc_object_index_right].length = 2.0f;
   object_data[slc_object_index_right].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[slc_object_index_right].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[slc_object_index_right].width;
   object_data[slc_object_index_right].vcs_vel.x = 10.0f;
   object_data[slc_object_index_right].vcs_vel.y = 10.0f;

   object_data[slc_object_index_left].width  = 1.0f;
   object_data[slc_object_index_left].length = 2.0f;
   object_data[slc_object_index_left].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[slc_object_index_left].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[slc_object_index_left].width;
   object_data[slc_object_index_left].vcs_vel.x = 20.0f;
   object_data[slc_object_index_left].vcs_vel.y = 20.0f;

   data.object_data[slc_object_index_right] = object_data[slc_object_index_right];
   data.object_data[slc_object_index_left]  = object_data[slc_object_index_left];

   lcda_output.lcda_object_existance_probability_right   = FBK_ZERO_UINT;
   lcda_output.lcda_object_lane_change_probability_right = FBK_ZERO_UINT;

   lcda_output.lcda_object_existance_probability_left   = FBK_ZERO_UINT;
   lcda_output.lcda_object_lane_change_probability_left = FBK_ZERO_UINT;
   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(&data, &lcda_output, &lcda_core_output);
   /** \assert Check All signal Mapping */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, VEHICLE_IN_THE_LEFT_SLC_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_slc,
             lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_slc,
             object_data[slc_object_index_right].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_slc,
             (-1 * object_data[slc_object_index_right].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_slc,
             lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_slc,
             object_data[slc_object_index_right].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_slc,
             (-1 * object_data[slc_object_index_right].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_slc, object_data[slc_object_index_right].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_slc, object_data[slc_object_index_right].length);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_slc,
             lcda_output.lcda_object_existance_probability_right);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_slc,
             lcda_output.lcda_object_lane_change_probability_right);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_slc,
             lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_slc,
             object_data[slc_object_index_left].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_slc,
             (-1 * object_data[slc_object_index_left].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_slc,
             lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_slc,
             object_data[slc_object_index_left].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_slc,
             (-1 * object_data[slc_object_index_left].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_slc, object_data[slc_object_index_left].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_slc, object_data[slc_object_index_left].length);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_slc,
             lcda_output.lcda_object_existance_probability_left);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_slc,
             lcda_output.lcda_object_lane_change_probability_left);
}

/**
 * Change the Output object according to the Functinal States is in Active when right no alert and left slc alert.
 * \uts{CSCSA-70076} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Active_State_right_no_alert_left_slc_alert)
{
   /** \arrange data to check signal mapping when in Active State */
   p_vehicle_data->rear_axle_position    = 0;
   lcda_output.bsw_alert[FBK_SIDE_RIGHT] = FBK_ONE_UINT;
   lcda_output.cvw_alert[FBK_SIDE_RIGHT] = FBK_ONE_UINT;
   lcda_output.slc_alert[FBK_SIDE_RIGHT] = FBK_ONE_UINT;
   p_vehicle_data->host_length           = 5.0f;
   p_vehicle_data->host_width            = 2.0f;

   uint8_t bsw_object_index_right                             = 1u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]    = bsw_object_index_right;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = bsw_object_index_right;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   object_data[bsw_object_index_right].width  = 1.0f;
   object_data[bsw_object_index_right].length = 2.0f;
   object_data[bsw_object_index_right].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[bsw_object_index_right].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[bsw_object_index_right].width;
   object_data[bsw_object_index_right].vcs_vel.x = 10.0f;
   object_data[bsw_object_index_right].vcs_vel.y = 10.0f;
   data.object_data[bsw_object_index_right]      = object_data[bsw_object_index_right];

   uint8_t cvw_object_index_right = 2u;

   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT] = cvw_object_index_right;

   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_RIGHT] = cvw_object_index_right;

   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT] = 1.2f;


   object_data[cvw_object_index_right].width  = 1.0f;
   object_data[cvw_object_index_right].length = 2.0f;
   object_data[cvw_object_index_right].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[cvw_object_index_right].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[cvw_object_index_right].width;
   object_data[cvw_object_index_right].vcs_vel.x = 10.0f;
   object_data[cvw_object_index_right].vcs_vel.y = 10.0f;

   data.object_data[cvw_object_index_right] = object_data[cvw_object_index_right];

   uint8_t slc_object_index_right = 3u;

   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]      = slc_object_index_right;
   lcda_core_output.slc_core_output.slc_index[FBK_SIDE_RIGHT]   = slc_object_index_right;
   lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT] = 1.2f;


   object_data[slc_object_index_right].width  = 1.0f;
   object_data[slc_object_index_right].length = 2.0f;
   object_data[slc_object_index_right].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[slc_object_index_right].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[slc_object_index_right].width;
   object_data[slc_object_index_right].vcs_vel.x = 10.0f;
   object_data[slc_object_index_right].vcs_vel.y = 10.0f;

   data.object_data[slc_object_index_right] = object_data[slc_object_index_right];

   lcda_output.lcda_object_existance_probability_right   = FBK_ZERO_UINT;
   lcda_output.lcda_object_lane_change_probability_right = FBK_ZERO_UINT;

   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(&data, &lcda_output, &lcda_core_output);

   /** \assert Check All signal Mapping */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, VEHICLE_IN_THE_RIGHT_BSW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right,
             lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right, object_data[bsw_object_index_right].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right,
             (-1 * object_data[bsw_object_index_right].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right, object_data[bsw_object_index_right].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right,
             (-1 * object_data[bsw_object_index_right].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right, object_data[bsw_object_index_right].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right, object_data[bsw_object_index_right].length);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right,
             lcda_output.lcda_object_existance_probability_right);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right,
             lcda_output.lcda_object_lane_change_probability_right);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, VEHICLE_IN_THE_RIGHT_CVW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_cvw,
             lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_cvw,
             object_data[cvw_object_index_right].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_cvw,
             (-1 * object_data[cvw_object_index_right].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_cvw,
             lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_cvw,
             object_data[cvw_object_index_right].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_cvw,
             (-1 * object_data[cvw_object_index_right].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_cvw, object_data[cvw_object_index_right].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_cvw, object_data[cvw_object_index_right].length);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_cvw,
             lcda_output.lcda_object_existance_probability_right);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_cvw,
             lcda_output.lcda_object_lane_change_probability_right);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc, VEHICLE_IN_THE_RIGHT_SLC_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right_slc,
             lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right_slc,
             object_data[slc_object_index_right].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right_slc,
             (-1 * object_data[slc_object_index_right].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right_slc,
             lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right_slc,
             object_data[slc_object_index_right].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right_slc,
             (-1 * object_data[slc_object_index_right].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right_slc, object_data[slc_object_index_right].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right_slc, object_data[slc_object_index_right].length);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right_slc,
             lcda_output.lcda_object_existance_probability_right);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right_slc,
             lcda_output.lcda_object_lane_change_probability_right);
}

/**
 * Change the Output object according to the Functinal States is in Active when left no alert and right slc alert.
 * \uts{CSCSA-70077} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Bmw_Output_Bus_Signals_When_Active_State_left_no_alert_right_slc_alert)
{
   /** \arrange data to check signal mapping when in Active State */
   p_vehicle_data->rear_axle_position   = 0;
   lcda_output.bsw_alert[FBK_SIDE_LEFT] = FBK_ONE_UINT;
   lcda_output.cvw_alert[FBK_SIDE_LEFT] = FBK_ONE_UINT;
   lcda_output.slc_alert[FBK_SIDE_LEFT] = FBK_ONE_UINT;
   p_vehicle_data->host_length          = 5.0f;
   p_vehicle_data->host_width           = 2.0f;

   uint8_t bsw_object_index_left                             = 1u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]    = bsw_object_index_left;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT] = bsw_object_index_left;

   p_vehicle_data->host_length = 5.0f;
   p_vehicle_data->host_width  = 2.0f;

   object_data[bsw_object_index_left].width  = 1.0f;
   object_data[bsw_object_index_left].length = 2.0f;
   object_data[bsw_object_index_left].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[bsw_object_index_left].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[bsw_object_index_left].width;
   object_data[bsw_object_index_left].vcs_vel.x = 10.0f;
   object_data[bsw_object_index_left].vcs_vel.y = 10.0f;
   data.object_data[bsw_object_index_left]      = object_data[bsw_object_index_left];

   uint8_t cvw_object_index_left = 2u;

   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT] = cvw_object_index_left;

   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_LEFT] = cvw_object_index_left;

   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_LEFT] = 1.2f;


   object_data[cvw_object_index_left].width  = 1.0f;
   object_data[cvw_object_index_left].length = 2.0f;
   object_data[cvw_object_index_left].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[cvw_object_index_left].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[cvw_object_index_left].width;
   object_data[cvw_object_index_left].vcs_vel.x = 10.0f;
   object_data[cvw_object_index_left].vcs_vel.y = 10.0f;

   data.object_data[cvw_object_index_left] = object_data[cvw_object_index_left];

   uint8_t slc_object_index_left = 3u;

   lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]      = slc_object_index_left;
   lcda_core_output.slc_core_output.slc_index[FBK_SIDE_LEFT]   = slc_object_index_left;
   lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT] = 1.2f;


   object_data[slc_object_index_left].width  = 1.0f;
   object_data[slc_object_index_left].length = 2.0f;
   object_data[slc_object_index_left].vcs_pos.x =
      -p_vehicle_data->host_length - 0.5f * customer_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance;
   object_data[slc_object_index_left].vcs_pos.y = 0.5f * p_vehicle_data->host_width + 0.4f * object_data[slc_object_index_left].width;
   object_data[slc_object_index_left].vcs_vel.x = 10.0f;
   object_data[slc_object_index_left].vcs_vel.y = 10.0f;

   data.object_data[slc_object_index_left] = object_data[slc_object_index_left];

   lcda_output.lcda_object_existance_probability_right   = FBK_ZERO_UINT;
   lcda_output.lcda_object_lane_change_probability_right = FBK_ZERO_UINT;

   /** \action Call Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State function */
   Lcda_Set_Bmw_Output_Bus_Signals_When_In_Active_State(&data, &lcda_output, &lcda_core_output);

   /** \assert Check All signal Mapping */
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, VEHICLE_IN_THE_LEFT_BSW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left, lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left, object_data[bsw_object_index_left].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left,
             (-1 * object_data[bsw_object_index_left].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left, object_data[bsw_object_index_left].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left,
             (-1 * object_data[bsw_object_index_left].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left, object_data[bsw_object_index_left].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left, object_data[bsw_object_index_left].length);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left,
             lcda_output.lcda_object_existance_probability_left);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left,
             lcda_output.lcda_object_lane_change_probability_left);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right,
             lcda_output.lcda_object_existance_probability_right);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right,
             lcda_output.lcda_object_lane_change_probability_right);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw, VEHICLE_IN_THE_LEFT_CVW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_cvw,
             lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_cvw,
             object_data[cvw_object_index_left].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_cvw,
             (-1 * object_data[cvw_object_index_left].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_cvw,
             lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left_cvw, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_cvw,
             object_data[cvw_object_index_left].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_cvw,
             (-1 * object_data[cvw_object_index_left].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_cvw, object_data[cvw_object_index_left].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_cvw, object_data[cvw_object_index_left].length);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_cvw,
             lcda_output.lcda_object_existance_probability_left);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_cvw,
             lcda_output.lcda_object_lane_change_probability_left);

   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier, EVENT_DATA_AVAILABLE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw, VEHICLE_IN_THE_LEFT_BSW_ZONE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier, NORMAL_OPERATION_MODE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second, LCDA_MOCKED_TIMESTAMP_VALUE);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left_slc,
             lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left_slc,
             object_data[slc_object_index_left].vcs_pos.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left_slc,
             (-1 * object_data[slc_object_index_left].vcs_pos.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left_slc,
             lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left_slc, FBK_ZERO_F);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left_slc,
             object_data[slc_object_index_left].vcs_vel.x);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left_slc,
             (-1 * object_data[slc_object_index_left].vcs_vel.y));
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left_slc, object_data[slc_object_index_left].width);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left_slc, object_data[slc_object_index_left].length);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left_slc,
             lcda_output.lcda_object_existance_probability_left);
   EXPECT_EQ(lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left_slc,
             lcda_output.lcda_object_lane_change_probability_left);
}

/**
 * Tests that calibration is updated correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Init_Output_test)
{
   /** \arrange declare variable for result and simple imput */
   Lcda_Output_T output;
   output.f_lcda_enabled = 1;
   /** \action call calibration update */
   Lcda_Init_Output(&output);
   /** \assert expect succes */
   EXPECT_EQ(output.f_lcda_enabled, 0);
}