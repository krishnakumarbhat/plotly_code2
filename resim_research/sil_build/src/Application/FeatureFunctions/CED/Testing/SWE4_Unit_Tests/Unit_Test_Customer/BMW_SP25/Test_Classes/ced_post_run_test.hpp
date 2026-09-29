#ifndef CED_POST_RUN_TEST
#define CED_POST_RUN_TEST

/**
 * @file ced_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for bmw sp25 ced post run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "ced_bmw_sp25_init.h"
#include "ced_core_calibration.h"
#include "ced_core_output_t.h"
#include "ced_input_t.h"
#include "ced_instance.h"
#include "ced_output_t.h"
#include "ced_state_machine.h"
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

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Pa_Obj_Class_T tracker_obj_class;

   Ced_Instance_T ced_instance{};
   Ced_Core_Calibration_T &cals = ced_instance.calibration;

   Ced_Output_T ced_output{};
   Ced_Input_T ced_input{};

   CED_FF_STATE_T *current_state;
   Bmw_Ced_Output_Bus_Signals_T ced_output_bus_signals{};

   void SetUp() override
   {
      Ced_Core_Cal_Update_Defaults(&cals);

      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

      ced_instance.core_input.p_pa_data = &data;

      current_state = Ced_Get_State_Output_Ptr();
      Ced_Init_Coding_Parameters(&ced_input.ced_coding_parameters);
      Ced_Init_Boardnet_Signals(&ced_input.bmw_boardnet_signals);

      ced_input.f_ced_enable = FBK_TRUE;
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