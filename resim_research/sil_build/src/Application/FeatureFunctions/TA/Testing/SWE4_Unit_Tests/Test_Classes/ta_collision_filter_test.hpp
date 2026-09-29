#ifndef TA_COLLISION_FILTER_TEST_HPP
#define TA_COLLISION_FILTER_TEST_HPP

/**
 * @file ta_collision_filter_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "ta_constants.h"
#include "ta_core_calibration.h"
#include "ta_core_input_t.h"
#include "ta_factory.h"
#include "ta_persistent_t.h"
#include "ta_types.h"
}

/**
 * Class used to create a fixture for TA test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Collision_Filter_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ta_Object_T ta_object{};
   Ta_Core_Input_T ta_core_input{};
   Ta_Persistent_T ta_persistent{};
   Ta_Persistent_T *p_ta_persistent;
   Ta_Core_Calibration_T ta_cal;
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Fbk_Trajectory_T ego_trajectory{};


   void SetUp() override
   {
      /* Initialize context data */
      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

      ta_core_input.p_pa_data = &data;

      p_ta_persistent = &ta_persistent;

      Ta_Core_Cal_Update_Defaults(&ta_cal);

      Ta_Init_Prediction_Time_Step(p_ta_persistent, &ta_cal);

      Ta_Reset_Trajectory(&ego_trajectory, &ta_cal);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*TA_COLLISION_FILTER_TEST_HPP*/
