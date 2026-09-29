#ifndef TA_POST_RUN_TEST_HPP
#define TA_POST_RUN_TEST_HPP

/**
 * @file ta_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Generic ta post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "pa_shared_types.h"
#include "ta_core_calibration.h"
#include "ta_core_output_t.h"
#include "ta_iface.h"
#include "ta_input_t.h"
#include "ta_output_t.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Post_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ta_Instance_T ta_instance{};
   Ta_Output_T ta_output{};
   Ta_Input_T ta_input{};
   Ta_Core_Calibration_T *p_cals    = &ta_instance.calibration;
   Ta_Core_Output_T &ta_core_output = ta_instance.core_output;
   Pa_Data_T data{};
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;

   void SetUp() override
   {
      Ta_Core_Cal_Update_Defaults(p_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* TA_POST_RUN_TEST */