#ifndef FBK_VEHICLE_VALIDATION_TEST_HPP
#define FBK_VEHICLE_VALIDATION_TEST_HPP

/**
 * @file fbk_validation_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for FBK object validation.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture for fbk_index_lookup module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Vehicle_Validation_Test : public ::testing::Test
{
 public:
   Pa_Context_T context{};
   Pa_Data_T data{};
   Fbk_Vehicle_Data_T *vehicle_data;
   Fbk_Vehicle_Data_T fbk_vehicle_data{};

   virtual void SetUp()
   {
      /* Initialize context data */
      context.p_data = &data;
      vehicle_data   = &(context.p_data->vehicle_data);
      /* set typical vehicle data */
      vehicle_data->host_length        = 2.0f;
      vehicle_data->host_width         = 1.0f;
      vehicle_data->rear_axle_position = -1.6f;
      vehicle_data->wheelbase          = 1.0f;
      vehicle_data->host_speed         = 5.0f;
      vehicle_data->steering_angle     = 0.0f;
      vehicle_data->yawrate            = 0.0f;
      vehicle_data->long_vel           = 4.0f;
      vehicle_data->long_acc           = 0.0f;
      vehicle_data->lat_acc            = 0.0f;
      vehicle_data->prndl              = PA_VEH_PRNDL_STATE_PARK;
      vehicle_data->lane_width         = 3.0f;
      vehicle_data->lane_center_offset = 0.0f;
      vehicle_data->turn_signal        = 1u;
      vehicle_data->curvature          = 0.0f;
      vehicle_data->f_reverse          = FBK_FALSE;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_VEHICLE_VALIDATION_TEST_HPP */
