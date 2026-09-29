#ifndef LCDA_LANE_MODEL_CAMERA_DATA_TEST_H
#define LCDA_LANE_MODEL_CAMERA_DATA_TEST_H

/**
 * @file lane_model_camera_data_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lane_Model_Camera_Data_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "camera_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lane_model_camera_data.h"
#include "lcda_core_calibration.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "pa_data.h"
#include "pa_reuse.h"
}


/**
 * Class used to create a fixture for Lane_Model_Camera_Data_Test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lane_Model_Camera_Data_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Lcda_Input_T lcda_input{};
   Pa_Data_T data{};

   Lcda_Core_Calibration_T &cals      = lcda_instance.calibration;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;

   Camera_Data_T cam_data{};
   Lane_Model_Output_Camera_T lane_model_camera_output{};

   const uint8_t LCDA_EGO_AVERAGE_QUALITY_LANE_WIDTH = 1u;
   const uint8_t LCDA_EGO_NORMAL_QUALITY_LANE_WIDTH  = 2u;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&cals);

      lcda_input.camera_data = &cam_data;

      cals.k_lm_min_lane_exist_prob_percent    = 70.0f;
      cals.k_lm_min_qualification_cycle_count  = 3u;
      cals.k_lm_min_output_hold_cycles         = 3u;
      cals.k_lm_min_plausible_lane_width       = 1.0f;
      cals.k_lm_max_plausible_lane_width       = 7.0f;
      cals.k_lm_max_plausible_lc_offset_factor = 0.9f;

      cam_data.lane_distance_first_left               = 2.0f;
      cam_data.lane_distance_first_right              = 1.5f;
      cam_data.lane_existance_probability_first_left  = 80.0f;
      cam_data.lane_existance_probability_first_right = 80.0f;
      cam_data.lane_width_ego                         = 4.0f;
      cam_data.quality_lane_width_ego                 = LCDA_EGO_AVERAGE_QUALITY_LANE_WIDTH;

      // Initialize the lane model
      Lcda_Init_Lane_Model_Camera();
   }
};

#endif
