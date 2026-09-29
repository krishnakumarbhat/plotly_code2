#ifndef CED_POST_RUN_BOARDNET_MIRROR_LED_TEST
#define CED_POST_RUN_BOARDNET_MIRROR_LED_TEST

/**
 * @file ced_post_run_boardnet_mirror_led_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for bmw sp25 ced post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "ced_bmw_sp25_init.h"
#include "ced_core_calibration.h"
#include "ced_core_output_t.h"
#include "ced_input_t.h"
#include "ced_output_t.h"
#include "ced_post_run_boardnet_mirror_led.h"
#include "ced_state_machine.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "pa_shared_types.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_Post_Run_Boardnet_Mirror_Led_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ced_Input_T ced_input{};
   Ced_Output_T ced_output{};
   Ced_Core_Output_T ced_core_output{};
   Ced_Door_Position_T door_position;
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Bmw_Ced_Output_Bus_Signals_T ced_output_bus_signals{};

   void SetUp() override
   {
      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

      Ced_Init_Coding_Parameters(&ced_input.ced_coding_parameters);
      Ced_Init_Boardnet_Signals(&ced_input.bmw_boardnet_signals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* CED_POST_RUN_BOARDNET_MIRROR_LED_TEST */