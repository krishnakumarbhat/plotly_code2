#ifndef TA_PRE_RUN_TEST_HPP
#define TA_PRE_RUN_TEST_HPP

/**
 * @file ta_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Rivian TA pre run unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest.h> // IWYU pragma: keep
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "Radar_Config.h"
#include "ta_core_calibration.h"
#include "ta_core_input_t.h"
#include "ta_input_t.h"
#include "ta_instance_t.h"
}

/**
 * Class used to create a fixture for BMW SRR5 TA pre run test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Pre_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ta_Instance_T ta_instance{};
   Ta_Input_T ta_input{};
   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Radar_Position_T radar_position{};

   Ta_Core_Input_T &ta_core_input     = ta_instance.core_input;
   Ta_Core_Calibration_T &ta_cal      = ta_instance.calibration;
   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;


   void SetUp() override
   {

      Ta_Core_Cal_Update_Defaults(&ta_cal);
      /* Initialize context data */
      radar_position = UNKNOWN_POSITION;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* TA_PRE_RUN_TEST_HPP */
