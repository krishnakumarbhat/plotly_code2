#ifndef TA_POST_RUN_TEST_HPP
#define TA_POST_RUN_TEST_HPP

/**
 * @file ta_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Rivian TA post run unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest.h> // IWYU pragma: keep
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_core_calibration.h"
#include "ta_core_output_t.h"
#include "ta_input_t.h"
#include "ta_instance_t.h"
#include "ta_output_t.h"
#include "ta_post_run.h" // IWYU pragma: keep
}

/**
 * Class used to create a fixture for Rivian TA post run test
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
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;
   Ta_Input_T ta_input{};
   Ta_Output_T ta_output{};
   Ta_Core_Output_T &ta_core_output = ta_instance.core_output;
   Ta_Core_Calibration_T &ta_cals   = ta_instance.calibration;

   void SetUp() override
   {
      /* Initialize context data */
      p_vehicle_data->host_speed = 3.0f;

      Ta_Core_Cal_Update_Defaults(&ta_cals);
      Ta_Post_Run_Init();
      ta_instance.core_input.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

   void Ta_Ut_Helper_Set_Tracker_Object_Data(Fbk_Object_Data_T *p_object_data, uint8_t obj_idx);
};

void inline Ta_Post_Run_Test::Ta_Ut_Helper_Set_Tracker_Object_Data(Fbk_Object_Data_T *p_object_data, uint8_t obj_idx)
{
   p_object_data[obj_idx].id                    = 0;
   p_object_data[obj_idx].age                   = 0;
   p_object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   p_object_data[obj_idx].existence_probability = 0.9f;
   p_object_data[obj_idx].vcs_heading           = 0.01f;
   p_object_data[obj_idx].vcs_vel.x             = 3.0f;
   p_object_data[obj_idx].vcs_vel.y             = 0.4f;
   p_object_data[obj_idx].vcs_accel.x           = 0.03f;
   p_object_data[obj_idx].vcs_accel.y           = 0.02f;
   p_object_data[obj_idx].heading_rate          = 0.01f;
   p_object_data[obj_idx].length                = 5.0f;
   p_object_data[obj_idx].width                 = 2.0f;
   p_object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
}

#endif /* TA_POST_RUN_TEST_HPP */
