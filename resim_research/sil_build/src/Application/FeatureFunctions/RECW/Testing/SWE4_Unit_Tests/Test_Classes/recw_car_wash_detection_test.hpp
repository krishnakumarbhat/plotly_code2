#ifndef RECW_CAR_WASH_DETECTION_TEST_HPP
#define RECW_CAR_WASH_DETECTION_TEST_HPP

/**
 * @file recw_car_wash_detection_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is car wash detection test header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "recw.h"
#include "recw_car_wash_detection.h" // IWYU pragma: keep
#include "recw_core_calibration.h"
#include "recw_core_input_t.h"
#include "recw_types.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Car_Wash_Detection_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Recw_Core_Input_T core_input{};
   Recw_Core_Calibration_T recw_cals;
   Recw_Object_T recw_object{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Recw_Persistent_T recw_pers{};

   void SetUp() override
   {
      /* Initialize calibration values */
      Recw_Core_Cal_Update_Defaults(&recw_cals);

      /* Initialize context data */
      core_input.p_pa_data = &data;
      object_data          = data.object_data;
      p_vehicle_data       = &(data.vehicle_data);

      /* Initialize core input */
      /* Init persistent data */
      Recw_Reset_Persistent(&recw_pers);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* RECW_CAR_WASH_DETECTION_TEST_HPP */
