#ifndef ESA_TEST_HPP
#define ESA_TEST_HPP

/**
 * @file esa_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Esa_Test
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"
#include <assert.h>

extern "C"
{
#include "esa.h"
#include "esa_core_calibration.h"
#include "esa_input_t.h"
#include "esa_instance_t.h"
#include "esa_output_t.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
}

/**
 * Class used to create a fixture for esa_tests.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Esa_Test : public ::testing::Test
{
 protected:
   Esa_Instance_T esa_instance{};
   Esa_Persistent_T *p_esa_persistent;
   Esa_Core_Calibration_T *p_esa_calibration;
   Esa_Core_Input_T *p_esa_core_input;
   Esa_Core_Output_T *p_esa_core_output;
   Pa_Data_T pa_data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      p_esa_persistent  = &(esa_instance.persistent);
      p_esa_calibration = &(esa_instance.calibration);
      p_esa_core_input  = &(esa_instance.core_input);
      p_esa_core_output = &(esa_instance.core_output);

      /* Initialize context data */
      object_data    = pa_data.object_data;
      p_vehicle_data = &(pa_data.vehicle_data);

      p_esa_core_input->p_pa_data = &pa_data;

      Esa_Core_Cal_Update_Defaults(p_esa_calibration);

      p_esa_calibration->k_esa_f_enable         = FBK_ONE_UINT;
      p_esa_calibration->k_esa_f_enable_via_cal = FBK_ZERO_UINT;

      p_esa_calibration->k_esa_host_activation_speed_min     = 2.0f;
      p_esa_calibration->k_esa_host_activation_speed_min_hys = 0.5f;

      // so that ESA is not deactivated due to small curve radius
      p_esa_calibration->k_esa_f_allow_min_curve_radius = 0u;

      /* Set basic vehicle data */
      p_vehicle_data->host_speed         = 2.0f * p_esa_calibration->k_esa_host_activation_speed_min;
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      // initialize ESA
      Esa_Reset(p_esa_core_input, p_esa_core_output, p_esa_persistent);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* ESA_TEST_HPP */
