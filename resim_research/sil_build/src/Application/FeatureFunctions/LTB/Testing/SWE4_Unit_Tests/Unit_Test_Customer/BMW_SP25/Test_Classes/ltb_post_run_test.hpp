#ifndef LTB_POST_RUN_TEST
#define LTB_POST_RUN_TEST

/**
 * @file ltb_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for BMW SP25 ltb post run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_core_calibration.h"
#include "ltb_core_output_t.h"
#include "ltb_input_t.h"
#include "ltb_instance.h"
#include "ltb_output_t.h"
#include "ltb_state_machine.h"
#include "pa_shared_types.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Post_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ltb_Core_Calibration_T cals;
   Ltb_Output_T ltb_output{};
   Ltb_Input_T ltb_input{};
   Ltb_Instance_T ltb_instance{};
   Ltb_Core_Output_T *p_ltb_core_output;
   Pa_Obj_Class_T tracker_obj_class;

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Ltb_State_T *p_ltb_current_state;

   void SetUp() override
   {
      Ltb_Core_Cal_Update_Defaults(&cals);
      object_data                       = data.object_data;
      p_vehicle_data                    = &data.vehicle_data;
      p_ltb_current_state               = Ltb_Get_Current_State();
      p_ltb_core_output                 = &ltb_instance.core_output;
      ltb_instance.core_input.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* LTB_POST_RUN_TEST */