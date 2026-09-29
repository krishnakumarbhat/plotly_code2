#ifndef RECW_CRASH_PROB_STEER_BRAKE_TEST_HPP
#define RECW_CRASH_PROB_STEER_BRAKE_TEST_HPP

/**
 * @file recw_crash_prob_steer_brake_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is crash probability calculation test header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_guardrail_data_t.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "recw_core_calibration.h"
#include "recw_core_input_t.h"
#include "recw_types.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Crash_Prob_Steer_Brake_Test : public ::testing::Test
{
 protected:
   Recw_Core_Input_T core_input{};
   Recw_Core_Calibration_T recw_cals;
   Recw_Object_T recw_object{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Guardrail_Data_T *guardrail_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      /* Initialize calibration values */
      Recw_Core_Cal_Update_Defaults(&recw_cals);

      /* Initialize context data */
      object_data    = data.object_data;
      guardrail_data = data.guardrail_data;
      p_vehicle_data = &(data.vehicle_data);

      /* Initialize core input */
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* RECW_CRASH_PROB_STEER_BRAKE_TEST_HPP */
