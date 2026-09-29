#ifndef RECW_POST_RUN_TEST_HPP
#define RECW_POST_RUN_TEST_HPP

/**
 * @file recw_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for BMW SRR5 RECW post run unit tests
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "recw_core_calibration.h"
#include "recw_core_output_t.h"
#include "recw_input_t.h"
#include "recw_instance.h"
#include "recw_output_t.h"
}

/**
 * Class used to create a fixture for BMW SRR5 Recw post run test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Post_Run_Test : public ::testing::Test
{
 public:
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Recw_Core_Calibration_T recw_cals;
   Recw_Input_T recw_input{};
   Recw_Output_T recw_output{};
   Recw_Instance_T recw_instance{};
   Recw_Core_Output_T recw_core_output{};

   void SetUp() override
   {
      /* Initialize calibration values */
      Recw_Core_Cal_Update_Defaults(&recw_cals);

      /* Initialize context data */
      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* RECW_POST_RUN_TEST_HPP */
