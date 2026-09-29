#ifndef RECW_POST_RUN_TEST_HPP
#define RECW_POST_RUN_TEST_HPP

/**
 * @file ltb_post_run_test.hpp
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
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_core_calibration.h"
#include "ltb_core_output_t.h"
#include "ltb_input_t.h"
#include "ltb_instance.h"
#include "ltb_output_t.h"
}

/**
 * Class used to create a fixture for Generic Ltb post run test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Post_Run_Test : public ::testing::Test
{
 public:
   Ltb_Instance_T ltb_instance{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;
   Ltb_Core_Calibration_T &ltb_cal    = ltb_instance.calibration;
   Ltb_Input_T ltb_input{};
   Ltb_Output_T ltb_output{};
   Ltb_Core_Output_T &ltb_core_output = ltb_instance.core_output;

   void SetUp() override
   {
      /* Initialize calibration values */
      Ltb_Core_Cal_Update_Defaults(&ltb_cal);

      /* Initialize context data */
      ltb_core_output.ltb_id[FBK_SIDE_LEFT]              = 5u;
      ltb_core_output.ltb_id[FBK_SIDE_RIGHT]             = 10u;
      ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]             = 1.5f;
      ltb_core_output.ltb_ttc[FBK_SIDE_RIGHT]            = 2.5f;
      ltb_core_output.ltb_ttb[FBK_SIDE_LEFT]             = 3.5f;
      ltb_core_output.ltb_ttb[FBK_SIDE_RIGHT]            = 4.5f;
      ltb_core_output.ltb_decel_estimate[FBK_SIDE_LEFT]  = 5.5f;
      ltb_core_output.ltb_decel_estimate[FBK_SIDE_RIGHT] = 6.5f;
      ltb_core_output.ltb_distance[FBK_SIDE_LEFT]        = 7.5f;
      ltb_core_output.ltb_distance[FBK_SIDE_RIGHT]       = 8.5f;
      ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]     = ALERT_ACTIVE_LEVEL_3;
      ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT]    = ALERT_ACTIVE_LEVEL_2;
      ltb_core_output.ltb_most_critical_side             = FBK_SIDE_LEFT;


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
