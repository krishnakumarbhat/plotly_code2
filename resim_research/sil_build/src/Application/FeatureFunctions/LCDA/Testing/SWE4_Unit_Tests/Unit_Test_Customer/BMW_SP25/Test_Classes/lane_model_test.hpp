#ifndef LCDA_LANE_MODEL_TEST_H
#define LCDA_LANE_MODEL_TEST_H

/**
 * @file lane_model_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lane_Model_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "camera_data_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lane_model.h"
#include "lane_model_camera_data.h"
#include "lcda_core_calibration.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "pa_data.h"
#include "pa_reuse.h"
}


/**
 * Class used to create a fixture for Polygon_Intersection_test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lane_Model_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Lcda_Input_T lcda_input{};
   Pa_Data_T data{};

   Lcda_Core_Calibration_T &cals = lcda_instance.calibration;

   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;

   Camera_Data_T cam_data{};
   Lane_Model_Output_T lane_model_output{};
   Lane_Model_Output_Camera_T lane_output_camera{};

   const uint8_t NAVI_DATA_URBAN_ONEWAY = 5u;  // NAV_URBAN_ONEWAY
   const uint8_t NAVI_DATA_HIGHWAY      = 17u; // NAV_HIGHWAY

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&cals);
      /* Initialize context data */
      lcda_input.camera_data                    = &cam_data;
      lcda_input.f_lcda_enable_basic_lane_model = 1u;

      cals.k_lm_enable_use_camera_data                 = FBK_FALSE;
      cals.k_lm_enable_use_navigation_data             = FBK_TRUE;
      cals.k_lm_enable_use_vehicle_dyn                 = FBK_TRUE;
      cals.k_lm_use_default_lane_information           = FBK_FALSE;
      cals.k_lm_lane_width_city                        = 2.0f;
      cals.k_lm_lane_width_highway                     = 3.0f;
      cals.k_lcda_lm_lane_width_defaultcountry_default = 2.5f;
      cals.k_lm_lane_center_offset_default             = 0.0f;
      cals.k_lm_min_speed_hway                         = 16.0f;
      cals.k_lm_hys_delta_speed_hway                   = 1.5f;
      cals.k_lm_min_yawrate_city_abs                   = 1.5f;
      cals.k_lm_hys_delta_yawrate_city_abs             = 0.2f;
      cals.k_lm_min_count_in_state                     = 10u;
      cals.k_lm_max_plausible_lc_offset_factor         = 0.9f;

      cals.k_lm_min_lane_exist_prob_percent   = 70.0f;
      cals.k_lm_min_qualification_cycle_count = 3u;
      cals.k_lm_min_output_hold_cycles        = 3u;
      cals.k_lm_min_plausible_lane_width      = 1.0f;
      cals.k_lm_max_plausible_lane_width      = 7.0f;

      cam_data.lane_distance_first_left               = 1.5f;
      cam_data.lane_distance_first_right              = 1.5f;
      cam_data.lane_existance_probability_first_left  = 80.0f;
      cam_data.lane_existance_probability_first_right = 80.0f;

      // Initialize the lane model
      Lcda_Initialize_Lane_Model(&cals, &lane_model_output);
   }
};

#endif
