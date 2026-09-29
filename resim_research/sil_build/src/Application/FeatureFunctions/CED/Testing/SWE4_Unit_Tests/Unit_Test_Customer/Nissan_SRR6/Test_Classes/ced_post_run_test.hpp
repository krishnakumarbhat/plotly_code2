#ifndef CED_POST_RUN_TEST
#define CED_POST_RUN_TEST

/**
 * @file ced_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Nissan_SRR6 ced post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "ced_core_calibration.h"
#include "ced_core_output_t.h"
#include "ced_input_t.h"
#include "ced_instance.h"
#include "ced_output_t.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_shared_types.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_Post_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ced_Core_Calibration_T cals;
   Ced_Output_T ced_output{};
   Ced_Input_T ced_input{};
   Ced_Instance_T ced_instance{};
   Pa_Obj_Class_T tracker_obj_class;

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;


   void SetUp() override
   {
      Ced_Core_Cal_Update_Defaults(&cals);

      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

      ced_instance.core_input.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* CED_POST_RUN_TEST */
