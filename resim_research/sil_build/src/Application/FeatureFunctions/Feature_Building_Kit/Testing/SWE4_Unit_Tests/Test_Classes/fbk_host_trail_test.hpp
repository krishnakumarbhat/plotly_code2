#ifndef FBK_HOST_TRAIL_TEST_HPP
#define FBK_HOST_TRAIL_TEST_HPP

/**
 * @file fbk_host_trail_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for FBK host trail.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_core_calibration.h"
#include "fbk_iface.h"
#include "fbk_iface_types.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture for fbk_index_lookup module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Host_Trail_Test : public ::testing::Test
{

 public:
   Fbk_Host_Trail_T host_trail{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Fbk_Core_Calibration_T fbk_cals;

   virtual void SetUp()
   {
      /* Initialize context data */
      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

      /* Initialize calibration values */
      Fbk_Core_Cal_Update_Defaults(&fbk_cals);

      /* Set calibration values */
      fbk_cals.k_fbk_host_trail_max_recording_speed = 15.0f;
      fbk_cals.k_fbk_host_trail_dist_separation     = 5.0f;
      fbk_cals.k_fbk_host_trail_heading_separation  = 0.2617f;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_HOST_TRAIL_TEST_HPP */
