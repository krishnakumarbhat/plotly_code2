/**
 * @file lane_model_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lane_model.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42924}
 */

#include "lane_model_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lane_model.c"
#include "pa_reuse.h"
}

/**
 * Check that default output is returned if this is requested by calibration value.
 * \uts{CSCSA-42925} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_default_lane_info_when_cal_is_set)
{

   /** \arrange Set up calibration values such that default lane information is requested. */
   cals.k_lm_use_default_lane_information = FBK_TRUE;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert Verify that default output is returned. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_defaultcountry_default);
}

/**
 * Check that navigation city lane width is used when input data is navigation urban oneway.
 * \uts{CSCSA-42926} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_navigation_city_lane_width_when_input_data_is_NAV_URBAN_ONEWAY)
{
   /** \arrange Set navigation road type to urban oneway and set qualification counters above thresholds. */
   lcda_input.navigation_data_road_type  = NAVI_DATA_URBAN_ONEWAY;
   Lane_Model_Persistent.navi_count_city = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.navi_count_hway = cals.k_lm_min_count_in_state + 2u;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert Verify that navigation city lane width is used. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_NAVIGATION_DATA);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 * Check that navigation highway lane width is used when input data is navigation highway.
 * \uts{CSCSA-42927} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_navigation_highway_lane_width_when_input_data_is_NAV_HIGHWAY)
{

   /** \arrange Set navigation road type to highway and set qualification counters above thresholds. */
   lcda_input.navigation_data_road_type  = NAVI_DATA_HIGHWAY;
   Lane_Model_Persistent.navi_count_city = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.navi_count_hway = cals.k_lm_min_count_in_state + 2u;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert Verify that highway city lane width is used. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_NAVIGATION_DATA);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_highway);
}

/**
 * Check that default lane width is used when input toggles between city and highway.
 * \uts{CSCSA-42928} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_default_lane_width_when_navi_input_toggles_between_city_and_highway)
{

   /** \arrange Set navigation road type such it toggles between city and highway in subsequent cycles. */
   uint8_t i;
   cals.k_lm_enable_use_vehicle_dyn = FBK_FALSE;

   for (i = 0; i < cals.k_lm_min_count_in_state + 3; i++)
   {
      if ((i % 2) == 0)
      {
         lcda_input.navigation_data_road_type = NAVI_DATA_URBAN_ONEWAY;
      }
      else
      {
         lcda_input.navigation_data_road_type = NAVI_DATA_HIGHWAY;
      }

      /** \action Call function Lcda_Process_Lane_Model multiple times to process lane model output. */
      Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);
   }

   /** \assert Verify that the default lane width is used. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_defaultcountry_default);
}

/**
 * Check that default lane width is used when navigation data is not valid.
 * \uts{CSCSA-42929} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_default_lane_width_when_navi_data_is_not_valid)
{

   /** \arrange Set invalid navigation data road type. */
   lcda_input.navigation_data_road_type = 100u;
   cals.k_lm_enable_use_vehicle_dyn     = FBK_FALSE; // disable use of
                                                     // vehicle dynamics

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert Verify that the default lane width is used. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_defaultcountry_default);
}

/**
 * Check that default lane width is used when navigation data and usage of dynamic vehicle data is disabled.
 * \uts{CSCSA-42930} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_default_lane_data_when_use_navi_and_veh_dyn_are_disabled)
{

   /** \arrange Disable usage of navigation data and dynamic vehicle data. */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_FALSE;
   lcda_input.navigation_data_road_type   = NAVI_DATA_HIGHWAY; // set plausible navigation data

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert Verify that the default lane width is used. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_defaultcountry_default);
}

/**
 * Check that city lane width is used when speed is below minimal highway speed.
 * \uts{CSCSA-42931} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_city_lane_width_vehicle_speed_is_below_k_lm_min_speed_hway)
{

   /** \arrange Set up calibration values such that dynamic vehicle data is used and set vehicle speed below minimal highway speed.
    */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   p_vehicle_data->host_speed = 10.0f;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert Verify that city lane width is used. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 * Check that highway lane width is used when speed is above minimal highway speed.
 * \uts{CSCSA-42932} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_highway_lane_width_vehicle_speed_is_above_k_lm_min_speed_hway)
{

   /** \arrange Set up calibration values such that dynamic vehicle data is used and set vehicle speed above minimal highway speed.
    */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   Lane_Model_Persistent.navi_count_city      = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.navi_count_hway      = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.vdyn_count_city2hway = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.vdyn_count_hway2city = cals.k_lm_min_count_in_state + 2u;

   p_vehicle_data->host_speed = 30.0f;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert Verify that highway lane width is used. */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_highway);
}

/**
 * Check that city lane width is used when vehicle speed toggles around threshold with large delta value.
 * \uts{CSCSA-42933} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test,
       Lcda_Process_Lane_Model__outputs_city_lane_width_when_vehicle_speed_toggles_around_threshold_with_large_delta_value)
{

   /** \arrange */
   uint8_t i;
   float32_T delta_speed = cals.k_lm_hys_delta_speed_hway + 0.5f; // delta speed is greater than the hys value

   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   for (i = 0; i < cals.k_lm_min_count_in_state + 3; i++)
   {
      if ((i % 2) == 0)
      {
         p_vehicle_data->host_speed = cals.k_lm_min_speed_hway + 1.0f;
      }
      else
      {
         p_vehicle_data->host_speed = cals.k_lm_min_speed_hway - delta_speed;
      }

      /** \action Call function Lcda_Process_Lane_Model multiple times to process lane model output. */
      Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);
   }

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 * Check that highway lane width is used when vehicle speed toggles around threshold with small delta value.
 * \uts{CSCSA-42934} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test,
       Lcda_Process_Lane_Model__outputs_highway_lane_width_when_vehicle_speed_toggles_around_threshold_with_small_delta_value)
{

   /** \arrange */
   uint8_t i;

   float32_T delta_speed                  = cals.k_lm_hys_delta_speed_hway - 0.5f; // delta speed is smaller than hys value
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   for (i = 0; i < cals.k_lm_min_count_in_state + 3; i++)
   {
      if ((i % 2) == 0)
      {
         p_vehicle_data->host_speed = cals.k_lm_min_speed_hway + 1.0f;
      }
      else
      {
         p_vehicle_data->host_speed = cals.k_lm_min_speed_hway - delta_speed;
      }

      /** \action Call function Lcda_Process_Lane_Model multiple times to process lane model output. */
      Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);
   }

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_highway);
}

/**
 * Check that lane width remains at highway lane width when yaw rate is below threshold for city lane width.
 * \uts{CSCSA-42935} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__output_remains_at_highway_width_when_yaw_rate_is_below_threshold)
{

   /** \arrange */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   p_vehicle_data->host_speed = 30.0f; // speed for highway

   Lane_Model_Persistent.navi_count_city      = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.navi_count_hway      = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.vdyn_count_city2hway = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.vdyn_count_hway2city = cals.k_lm_min_count_in_state + 2u;
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   // Now change only the speed to below highway but yawrate is below city threshold
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->yawrate    = 0.5f;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_highway);
}

/**
 * Check that lane width remains at highway lane width when yaw rate toggles around threshold for city lane width with large delta
 * value. \uts{CSCSA-42936} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test,
       DISABLED_Lcda_Process_Lane_Model__output_remains_at_highway_width_when_yaw_rate_toggles_around_threshold_with_large_delta_value)
{

   /** \arrange */
   uint8_t i;
   float32_T delta_yawrate                = cals.k_lm_hys_delta_yawrate_city_abs + 0.5f; // delta is larger than hys value
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   p_vehicle_data->host_speed = 30.0f; // speed for highway

   Lane_Model_Persistent.navi_count_city      = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.navi_count_hway      = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.vdyn_count_city2hway = cals.k_lm_min_count_in_state + 2u;
   Lane_Model_Persistent.vdyn_count_hway2city = cals.k_lm_min_count_in_state + 2u;
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   // Now change only the speed to below highway
   p_vehicle_data->host_speed = 10.0f;

   // toggle yawrate with a large delta value
   for (i = 0; i < cals.k_lm_min_count_in_state + 3; i++)
   {
      if ((i % 2) == 0)
      {
         p_vehicle_data->yawrate = cals.k_lm_min_yawrate_city_abs + 0.5f;
      }
      else
      {
         p_vehicle_data->yawrate = cals.k_lm_min_yawrate_city_abs - delta_yawrate;
      }

      /** \action Call function Lcda_Process_Lane_Model multiple times to process lane model output. */
      Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);
   }


   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_highway);
}

/**
 * Check that lane width changes from highway lane width to city lane width when yaw rate toggles around threshold for city lane
 * width with small delta value. \uts{CSCSA-42937} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test,
       Lcda_Process_Lane_Model__output_changes_from_highway_to_city_width_when_yaw_rate_toggles_around_threshold_with_small_delta_value)
{

   /** \arrange */
   uint8_t i;
   float32_T delta_yawrate                = cals.k_lm_hys_delta_yawrate_city_abs - 0.1f; // delta is lower than hys threshold
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   p_vehicle_data->host_speed = 30.0f; // speed for highway

   // output is now set to highway
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   // Now change only the speed to below highway
   p_vehicle_data->host_speed = 10.0f;

   // toggle yawrate with a large small value below the yawrate hysteresis threshold
   for (i = 0; i < cals.k_lm_min_count_in_state + 3; i++)
   {
      if ((i % 2) == 0)
      {
         p_vehicle_data->yawrate = cals.k_lm_min_yawrate_city_abs + 0.5f;
      }
      else
      {
         p_vehicle_data->yawrate = cals.k_lm_min_yawrate_city_abs - delta_yawrate;
      }

      /** \action Call function Lcda_Process_Lane_Model multiple times to process lane model output. */
      Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);
   }


   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 * Check that lane width changes from highway lane width to city lane width when speed and yaw rate conditions are met with
 * positive yaw rate. \uts{CSCSA-42938} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test,
       Lcda_Process_Lane_Model__output_changes_from_highway_to_city_width_when_speed_and_yaw_rate_conditions_for_city_are_met_positive_yaw)
{

   /** \arrange */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   p_vehicle_data->host_speed = 30.0f; // speed for highway

   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   // Now change speed and yaw rate to meet city conditions
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->yawrate    = 2.5f;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 * Check that lane width changes from highway lane width to city lane width when speed and yaw rate conditions are met with
 * negative yaw rate. \uts{CSCSA-42939} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test,
       Lcda_Process_Lane_Model__output_changes_from_highway_to_city_width_when_speed_and_yaw_rate_conditions_for_city_are_met_negative_yaw)
{

   /** \arrange */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_FALSE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;

   p_vehicle_data->host_speed = 30.0f; // speed for highway

   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   // Now change speed and yaw rate to meet city conditions
   p_vehicle_data->host_speed = 10.0f;
   p_vehicle_data->yawrate    = -2.5f;

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 *
 * \uts{CSCSA-42940} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, DISABLED_Lcda_Process_Lane_Model__outputs_camera_lane_info_when_enabled)
{

   /** \arrange */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_TRUE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;
   cals.k_lm_enable_use_camera_data       = FBK_TRUE;

   p_vehicle_data->host_speed = 30.0f; // speed for city

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_CAMERA_DATA);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, 0.0f);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, 3.0f);
}

/**
 * Check that dynamic vehicle data is used when camera data is disabled.
 * \uts{CSCSA-42941} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__uses_vehicle_dynamics_when_camera_data_is_disabled)
{

   /** \arrange */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_TRUE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;
   cals.k_lm_enable_use_camera_data       = FBK_FALSE;

   p_vehicle_data->host_speed = 30.0f; // speed for city

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 * Check that dynamic vehicle data is used when camera data is not valid.
 * \uts{CSCSA-42942} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Process_Lane_Model__outputs_vehicle_dyn_lane_when_camera_output_is_not_valid)
{

   /** \arrange */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_TRUE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;
   cals.k_lm_enable_use_camera_data       = FBK_TRUE;

   cam_data.lane_existance_probability_first_left = 40.0f;
   p_vehicle_data->host_speed                     = 30.0f; // speed for city

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DRIVING_DYNAMICS);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
}

/**
 *
 * \uts{CSCSA-42943} \sdd{SF-6887} \testtype{positive}
 */
TEST_F(Lane_Model_Test, DISABLED_Lcda_Process_Lane_Model__outputs_ego_lane_width_from_camera_when_lane_borders_have_low_exist_prob)
{

   /** \arrange */
   cals.k_lm_use_default_lane_information = FBK_FALSE;
   cals.k_lm_enable_use_navigation_data   = FBK_TRUE;
   cals.k_lm_enable_use_vehicle_dyn       = FBK_TRUE;
   cals.k_lm_enable_use_camera_data       = FBK_TRUE;

   cam_data.lane_existance_probability_first_left = 40.0f;
   cam_data.lane_width_ego                        = 3.5f;
   cam_data.quality_lane_width_ego                = 2u; // LCDA_EGO_NORMAL_QUALITY_LANE_WIDTH

   p_vehicle_data->host_speed = 30.0f; // speed for city

   /** \action Call function Lcda_Process_Lane_Model to process lane model output. */
   Lcda_Process_Lane_Model(&lcda_input, p_vehicle_data, &cals, &lane_model_output);

   /** \assert */
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_CAMERA_DATA);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, 0.0f);
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, 3.5f);
}


/**
 * Test the superordinate lane model initialization routine. Default values are expected here.
 * \uts{CSCSA-42944} \sdd{SF-6888} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Initialize_Lane_Model__test_default_values)
{
   /** \arrange Set non default values for sub structures which are initialized in this routine. */
   Lane_Model_Persistent.vdyn_road_type = ROAD_TYPE_UNKNOWN;
   Lane_Model_Persistent.navi_road_type = ROAD_TYPE_CITY;
   lane_model_output.lane_width         = cals.k_lcda_lm_lane_width_germany_default;

   /** \action Call initialization routine for lane model output. */
   Lcda_Initialize_Lane_Model(&cals, &lane_model_output);

   /** \assert Expect default values for substructures */
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_defaultcountry_default);
   EXPECT_EQ(Lane_Model_Persistent.vdyn_road_type, ROAD_TYPE_CITY);
   EXPECT_EQ(Lane_Model_Persistent.navi_road_type, ROAD_TYPE_UNKNOWN);
}


/**
 * Test the default value setter for lane model based on current country. Here a default test case shall be applied, such that a
 * default country setting is used for lane model. \uts{CSCSA-42945} \sdd{SF-6895} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Default_Output__test_default_case)
{
   /** \arrange Set country type to default. */
   Country_Type_T country_type = COUNTRY_TYPE_DEFAULT;

   /** \action Call initialization routine for lane model output. */
   Lcda_Fill_Default_Output(&cals, &lane_model_output, country_type);

   /** \assert Expect default values for lane model for default country */
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_defaultcountry_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
}


/**
 * Test the default value setter for lane model based on current country. Here germany is chosen and thus germany default values
 * are expected. \uts{CSCSA-42946} \sdd{SF-6895} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Default_Output__test_germany_case)
{
   /** \arrange Set country type to germany. */
   Country_Type_T country_type = COUNTRY_TYPE_GERMANY;

   /** \action Call initialization routine for lane model output. */
   Lcda_Fill_Default_Output(&cals, &lane_model_output, country_type);

   /** \assert Expect default values for lane model for germany */
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_germany_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
}


/**
 * Test the default value setter for lane model based on current country. Here US is chosen and thus US default values are
 * expected. \uts{CSCSA-99048} \sdd{SF-6895} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Default_Output__test_us_case)
{
   /** \arrange Set country type to US. */
   Country_Type_T country_type = COUNTRY_TYPE_US;

   /** \action Call initialization routine for lane model output. */
   Lcda_Fill_Default_Output(&cals, &lane_model_output, country_type);

   /** \assert Expect default values for lane model for US */
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_us_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
}


/**
 * Test the default value setter for lane model based on current country. Here japan is chosen and thus japanese default values are
 * expected. \uts{CSCSA-99049} \sdd{SF-6895} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Default_Output__test_japan_case)
{
   /** \arrange Set country type to japan. */
   Country_Type_T country_type = COUNTRY_TYPE_JAPAN;

   /** \action Call initialization routine for lane model output. */
   Lcda_Fill_Default_Output(&cals, &lane_model_output, country_type);

   /** \assert Expect default values for lane model for japan */
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_japan_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
}


/**
 * Test the default value setter for lane model based on current country. Here china is chosen and thus chinese default values are
 * expected. \uts{CSCSA-99050} \sdd{SF-6895} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Default_Output__test_china_case)
{
   /** \arrange Set country type to china. */
   Country_Type_T country_type = COUNTRY_TYPE_CHINA;

   /** \action Call initialization routine for lane model output. */
   Lcda_Fill_Default_Output(&cals, &lane_model_output, country_type);

   /** \assert Expect default values for lane model for china */
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_china_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
}


/**
 * Test the default value setter for lane model based on current country. Here korea is chosen and thus korean default values are
 * expected. \uts{CSCSA-99051} \sdd{SF-6895} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Default_Output__test_korea_case)
{
   /** \arrange Set country type to korea. */
   Country_Type_T country_type = COUNTRY_TYPE_KOREA;

   /** \action Call initialization routine for lane model output. */
   Lcda_Fill_Default_Output(&cals, &lane_model_output, country_type);

   /** \assert Expect default values for lane model for korea */
   EXPECT_FLOAT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_korea_default);
   EXPECT_FLOAT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
}

/**
 * Test vehicle dynamic persistent constructor. Expect default values overwrite previous persistent data.
 * \uts{CSCSA-42947} \sdd{SF-6894} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Init_Veh_Dyn_Pers__expect_default_vals)
{
   /** \arrange Set vehicle persistent to non default values. */
   Lane_Model_Persistent.vdyn_road_type       = ROAD_TYPE_UNKNOWN;
   Lane_Model_Persistent.vdyn_count_city2hway = 5u;
   Lane_Model_Persistent.vdyn_count_hway2city = 10u;

   /** \action Call initialization routine for lane model output. */
   Lcda_Init_Veh_Dyn_Pers();

   /** \assert Expect default values */
   EXPECT_EQ(Lane_Model_Persistent.vdyn_road_type, ROAD_TYPE_CITY);
   EXPECT_EQ(Lane_Model_Persistent.vdyn_count_city2hway, 0u);
   EXPECT_EQ(Lane_Model_Persistent.vdyn_count_hway2city, 0u);
}


/**
 * Test navi persistent constructor. Expect default values overwrite previous persistent data.
 * \uts{CSCSA-42948} \sdd{SF-6893} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Init_Navi_Pers__expect_default_vals)
{
   /** \arrange Set vehicle persistent to non default values. */
   Lane_Model_Persistent.navi_road_type  = ROAD_TYPE_CITY;
   Lane_Model_Persistent.navi_count_city = 5u;
   Lane_Model_Persistent.navi_count_hway = 10u;

   /** \action Call initialization routine for lane model output. */
   Lcda_Init_Navi_Pers();

   /** \assert Expect default values */
   EXPECT_EQ(Lane_Model_Persistent.navi_road_type, ROAD_TYPE_UNKNOWN);
   EXPECT_EQ(Lane_Model_Persistent.navi_count_city, 0u);
   EXPECT_EQ(Lane_Model_Persistent.navi_count_hway, 0u);
}


/**
 * Test lane model constructor. Here invalid data is passed, such that default values are expected.
 * \uts{CSCSA-42949} \sdd{SF-6892} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Lane_Model_Output__expect_default_vals)
{
   /** \arrange Set inputs such that default values are returned. */
   Road_Type_T navi_result     = ROAD_TYPE_UNKNOWN;
   Road_Type_T vdyn_result     = ROAD_TYPE_UNKNOWN;
   Country_Type_T country_type = COUNTRY_TYPE_DEFAULT;
   lane_output_camera.status   = LM_CAMERA_STATUS_INIT;

   /** \action Call lane model population function. */
   Lcda_Fill_Lane_Model_Output(&lane_output_camera, navi_result, vdyn_result, &cals, &lane_model_output, country_type);

   /** \assert Expect default values */
   EXPECT_EQ(lane_model_output.lane_width, cals.k_lcda_lm_lane_width_defaultcountry_default);
   EXPECT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.lane_lateral_speed[FBK_SIDE_LEFT], FBK_ZERO_F);
   EXPECT_EQ(lane_model_output.lane_lateral_speed[FBK_SIDE_RIGHT], FBK_ZERO_F);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_DEFAULT_VALUE);
}


/**
 * Test lane model constructor. Here invalid data is passed, such that default values are expected.
 * \uts{CSCSA-42950} \sdd{SF-6892} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Lane_Model_Output__set_lateral_speed_even_if_camera_is_invalid)
{
   /** \arrange Set inputs such that default values are returned. */
   Road_Type_T navi_result                               = ROAD_TYPE_CITY;
   Road_Type_T vdyn_result                               = ROAD_TYPE_UNKNOWN;
   Country_Type_T country_type                           = COUNTRY_TYPE_DEFAULT;
   lane_output_camera.status                             = LM_CAMERA_STATUS_INIT;
   lane_output_camera.lane_lateral_speed[FBK_SIDE_LEFT]  = FBK_ONE_F;
   lane_output_camera.lane_lateral_speed[FBK_SIDE_RIGHT] = FBK_ONE_F;

   /** \action Call lane model population function. */
   Lcda_Fill_Lane_Model_Output(&lane_output_camera, navi_result, vdyn_result, &cals, &lane_model_output, country_type);

   /** \assert Expect default values */
   EXPECT_EQ(lane_model_output.lane_width, cals.k_lm_lane_width_city);
   EXPECT_EQ(lane_model_output.lane_center_offset, cals.k_lm_lane_center_offset_default);
   EXPECT_EQ(lane_model_output.lane_lateral_speed[FBK_SIDE_LEFT], FBK_ONE_F);
   EXPECT_EQ(lane_model_output.lane_lateral_speed[FBK_SIDE_RIGHT], FBK_ONE_F);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_NAVIGATION_DATA);
}


/**
 * Test lane model constructor. Camera data available thus specific values are expected.
 * \uts{CSCSA-99052} \sdd{SF-6892} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Lane_Model_Output__camera_status_available)
{
   /** \arrange Set inputs such that camera data are returned. */
   Road_Type_T navi_result               = ROAD_TYPE_UNKNOWN;
   Road_Type_T vdyn_result               = ROAD_TYPE_UNKNOWN;
   Country_Type_T country_type           = COUNTRY_TYPE_DEFAULT;
   lane_output_camera.status             = LM_CAMERA_STATUS_AVAILABLE;
   lane_output_camera.lane_width         = 3.0f;
   lane_output_camera.lane_center_offset = 1.5f;
   /** \action Call lane model population function. */
   Lcda_Fill_Lane_Model_Output(&lane_output_camera, navi_result, vdyn_result, &cals, &lane_model_output, country_type);

   /** \assert Expect camera data */
   EXPECT_EQ(lane_model_output.lane_width, 3.0f);
   EXPECT_EQ(lane_model_output.lane_center_offset, 1.5f);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_CAMERA_DATA);
}


/**
 * Test lane model constructor. Camera data available thus specific values are expected.
 * \uts{CSCSA-99053} \sdd{SF-6892} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Fill_Lane_Model_Output__camera_status_degraded)
{
   /** \arrange Set inputs such that camera data are returned. */
   Road_Type_T navi_result               = ROAD_TYPE_UNKNOWN;
   Road_Type_T vdyn_result               = ROAD_TYPE_UNKNOWN;
   Country_Type_T country_type           = COUNTRY_TYPE_DEFAULT;
   lane_output_camera.status             = LM_CAMERA_STATUS_DEGRADED;
   lane_output_camera.lane_width         = 3.0f;
   lane_output_camera.lane_center_offset = 1.5f;
   /** \action Call lane model population function. */
   Lcda_Fill_Lane_Model_Output(&lane_output_camera, navi_result, vdyn_result, &cals, &lane_model_output, country_type);

   /** \assert Expect camera data */
   EXPECT_EQ(lane_model_output.lane_width, 3.0f);
   EXPECT_EQ(lane_model_output.lane_center_offset, 1.5f);
   EXPECT_EQ(lane_model_output.calculation_method, CALC_METHOD_CAMERA_DATA);
}

/**
 * Check whether vehicle data is valid. Here data is chosen such that true is returned.
 * \uts{CSCSA-42951} \sdd{SF-6891} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Is_Veh_Data_Valid__expect_true)
{
   /** \arrange Set inputs such that vehicle data is valid. */
   boolean_T res;
   p_vehicle_data->host_speed = 0.5f * (LCDA_VALID_SPEED_LOWER_THRESH + LCDA_VALID_SPEED_UPPER_THRESH);
   p_vehicle_data->yawrate    = 0.5f * (LCDA_VALID_YAWRATE_LOWER_THRESH + LCDA_VALID_YAWRATE_UPPER_THRESH);

   /** \action Call vehicle data validation. */
   res = Lcda_Is_Veh_Data_Valid(p_vehicle_data);

   /** \assert Expect true */
   EXPECT_TRUE(res);
}


/**
 * Check whether vehicle data is valid. Input data is chosen such that false is expected.
 * \uts{CSCSA-42952} \sdd{SF-6891} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Is_Veh_Data_Valid__expect_false)
{
   /** \arrange Set inputs such that vehicle data is invalid. */
   boolean_T res;
   p_vehicle_data->host_speed = 0.5f * (LCDA_VALID_SPEED_LOWER_THRESH + LCDA_VALID_SPEED_UPPER_THRESH);
   p_vehicle_data->yawrate    = 1.1f * LCDA_VALID_YAWRATE_UPPER_THRESH;

   /** \action Call vehicle data validation. */
   res = Lcda_Is_Veh_Data_Valid(p_vehicle_data);

   /** \assert Expect false */
   EXPECT_FALSE(res);
}


/**
 * Check whether navigation road type is city. Input data is chosen such that true is expected.
 * \uts{CSCSA-99054} \sdd{SF-6890} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Is_Navi_Road_Type_City__expect_true_all_cases)
{
   /** \arrange Set inputs such that vehicle data is valid. */
   boolean_T res;
   Navigation_Road_Type_T navi_road_data[7] = {NAV_TRAFFIC_CALMED_ZONE_BIDIR,
                                               NAV_TRAFFIC_CALMED_ZONE_ONEWAY,
                                               NAV_RESIDENTIAL_AREA_BIDIR,
                                               NAV_RESIDENTIAL_AREA_ONEWAY,
                                               NAV_URBAN_BIDIR,
                                               NAV_URBAN_ONEWAY,
                                               NAV_URBAN_WITH_MEDIAN_STRIP};

   for (uint8_t i = 0; i < 7; i++)
   {

      /** \action Call navigation city road type check. */
      res = Lcda_Is_Navi_Road_Type_City(navi_road_data[i]);

      /** \assert Expect true */
      EXPECT_TRUE(res);
   }
}


/**
 * Check whether navigation road type is not city. Input data is chosen such that false is expected.
 * \uts{CSCSA-99055} \sdd{SF-6890} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Is_Navi_Road_Type_City__expect_false)
{
   /** \arrange Set inputs such that vehicle data is invalid. */
   boolean_T res;
   Navigation_Road_Type_T navi_road_data = NAV_CITY_FREEWAY;

   /** \action Call navigation city road type check. */
   res = Lcda_Is_Navi_Road_Type_City(navi_road_data);

   /** \assert Expect false */
   EXPECT_FALSE(res);
}


/**
 * Check whether navigation road type is highway. Input data is chosen such that true is expected.
 * \uts{CSCSA-99056} \sdd{SF-6889} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Is_Navi_Road_Type_Hway__expect_true_all_cases)
{
   /** \arrange Set inputs such that vehicle data is valid. */
   boolean_T res;
   Navigation_Road_Type_T navi_road_data[12] = {NAV_CITY_FREEWAY,
                                                NAV_CITY_FREEWAY_WITH_MEDIAN_STRIP,
                                                NAV_COUNTRY_ROAD,
                                                NAV_COUNTRY_ROAD_WITH_MEDIAN_STRIP,
                                                NAV_MAIN_ROAD,
                                                NAV_MAIN_ROAD_WITH_MEDIAN_STRIP,
                                                NAV_MAIN_ROAD_ON_RAMP,
                                                NAV_MAIN_ROAD_EXIT,
                                                NAV_MAIN_ROAD_ON_RAMP_AND_EXIT,
                                                NAV_HIGHWAY,
                                                NAV_HIGHWAY_ON_RAMP,
                                                NAV_HIGHWAY_ON_RAMP_AND_EXIT};


   for (uint8_t i = 0; i < 12; i++)
   {

      /** \action Call navigation highway road type check. */
      res = Lcda_Is_Navi_Road_Type_Hway(navi_road_data[i]);

      /** \assert Expect true */
      EXPECT_TRUE(res);
   }
}

/**
 * Check whether navigation road type is not highway. Input data is chosen such that false is expected.
 * \uts{CSCSA-99057} \sdd{SF-6889} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Is_Navi_Road_Type_Hway__expect_false)
{
   /** \arrange Set inputs such that vehicle data is invalid. */
   boolean_T res;
   Navigation_Road_Type_T navi_road_data = NAV_URBAN_ONEWAY;

   /** \action Call navigation highway road type check. */
   res = Lcda_Is_Navi_Road_Type_Hway(navi_road_data);

   /** \assert Expect true */
   EXPECT_FALSE(res);
}


/**
 * Check whether navigation road type is highway. Input data is chosen such that true is expected.
 * \uts{CSCSA-42955} \sdd{SF-6896} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Get_Navi_Data_Lane_Model__expect_default_road_type)
{
   /** \arrange Set inputs such that unknown road data is returned. */
   Road_Type_T res;
   Navigation_Road_Type_T navi_road_data = NAV_ROAD_TYPE_UNKNOWN;

   /** \action Call Return function of navigation data lane model. */
   res = Lcda_Get_Navi_Data_Lane_Model(navi_road_data, &cals);

   /** \assert Expect an unknown road type to be returned */
   EXPECT_EQ(res, ROAD_TYPE_UNKNOWN);
}


/**
 * Test the road type determination based on vehicle dynamics. Here a switch from city to highway shall be observed .
 * \uts{CSCSA-42956} \sdd{SF-6897} \testtype{positive}
 */
TEST_F(Lane_Model_Test, Lcda_Get_Veh_Dyn_Lane_Model__expect_default_road_type)
{
   /** \arrange Set inputs for a switch from city to highway. */
   Lane_Model_Persistent.vdyn_road_type       = ROAD_TYPE_CITY;
   Lane_Model_Persistent.vdyn_count_city2hway = cals.k_lm_min_count_in_state;
   float32_T speed                            = 1.1f * cals.k_lm_min_speed_hway;
   float32_T yawrate                          = 0.0f;
   Road_Type_T res;
   /** \action Call Return function of navigation data lane model. */
   res = Lcda_Get_Veh_Dyn_Lane_Model(speed, yawrate, &cals);

   /** \assert Expect an unknown road type to be returned */
   EXPECT_EQ(res, ROAD_TYPE_HIGHWAY);
}


/**
 * Check whether qualification logic for a switch from city road type to highway gets reset in case that conditions are not
 * fulfilled. \uts{CSCSA-42957} \sdd{SF-6898} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Vd_Process_City__reset_counter_due_to_non_fulfilled_speed_value)
{
   /** \arrange Set inputs for a reset of qualification logic. */
   Lane_Model_Persistent.vdyn_count_city2hway = cals.k_lm_min_count_in_state;
   float32_T speed                            = 0.9f * (cals.k_lm_min_speed_hway - cals.k_lm_hys_delta_speed_hway);

   /** \action Call qualification function for road model determination. */
   Lcda_Vd_Process_City(speed, &cals);

   /** \assert Expect an road type city to be returned and the qualification counter to be reset */
   EXPECT_EQ(Lane_Model_Persistent.vdyn_road_type, ROAD_TYPE_CITY);
   EXPECT_EQ(Lane_Model_Persistent.vdyn_count_city2hway, 0u);
}


/**
 * Check whether qualification logic for a switch from highway road type to city gets reset in case that conditions are not
 * fulfilled. \uts{CSCSA-42958} \sdd{SF-6899} \testtype{negative}
 */
TEST_F(Lane_Model_Test, Lcda_Vd_Process_Highway__reset_counter_due_to_non_fulfilled_yawrate_value)
{
   /** \arrange Set inputs for a reset of qualification logic. */
   Lane_Model_Persistent.vdyn_count_hway2city = cals.k_lm_min_count_in_state;
   float32_T speed                            = cals.k_lm_min_speed_hway - cals.k_lm_hys_delta_speed_hway;
   float32_T yawrate                          = 0.0f;

   /** \action Call qualification function for road model determination. */
   Lcda_Vd_Process_Highway(speed, yawrate, &cals);

   /** \assert Expect an road type city to be returned and the qualification counter to be reset */
   EXPECT_EQ(Lane_Model_Persistent.vdyn_road_type, ROAD_TYPE_HIGHWAY);
   EXPECT_EQ(Lane_Model_Persistent.vdyn_count_hway2city, 0u);
}