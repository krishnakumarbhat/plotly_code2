#ifndef RECW_POST_RUN_TEST_HPP
#define RECW_POST_RUN_TEST_HPP

/**
 * @file recw_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Generic post run unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "recw_core_calibration.h"
#include "recw_core_output_t.h"
#include "recw_input_t.h"
#include "recw_instance.h"
#include "recw_output_t.h"
}

/**
 * Class used to create a fixture for Generic Recw post run test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Post_Run_Test : public ::testing::Test
{
 public:
   Recw_Instance_T recw_instance{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;
   Recw_Core_Calibration_T &recw_cal  = recw_instance.calibration;
   Recw_Input_T recw_input{};
   Recw_Output_T recw_output{};
   Recw_Core_Output_T &recw_core_output = recw_instance.core_output;

   void SetUp() override
   {
      /* Initialize calibration values */
      Recw_Core_Cal_Update_Defaults(&recw_cal);

      /* Initialize context data */
      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

      recw_core_output.recw_crash_prob_combined    = 0.7f;
      recw_core_output.recw_ttc                    = 8.5f;
      recw_core_output.recw_id                     = 10u;
      recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_2;
      recw_core_output.ttc_threshold_alert_level_1 = 5.0;
      recw_core_output.ttc_threshold_alert_level_2 = 8.0f;


      /* Initialize input */
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* RECW_POST_RUN_TEST_HPP */
