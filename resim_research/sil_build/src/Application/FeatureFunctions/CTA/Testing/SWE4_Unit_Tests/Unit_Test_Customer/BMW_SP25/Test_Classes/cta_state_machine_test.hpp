#ifndef CTA_STATE_MACHINE_TEST
#define CTA_STATE_MACHINE_TEST

/**
 * @file cta_state_machine_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for BMW SP25 cta state machine
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_instance.h"
#include "cta_state_machine.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_State_Machine_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Core_Calibration_T *p_cta_cals = nullptr;
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;
   Cta_Input_T cta_input{};
   Cta_Instance_T cta_instance{};
   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Ctb_Flag_Output_T ctb_state_machine_flag{};
   Ctb_State_Output_T p_ctb_current_state;
   Ctb_Function_Error_T ctb_error;
   Ctb_State_Output_T *Ctb_Current_State;

   void SetUp() override
   {
      p_cta_cals = &cta_instance.calibration;
      Cta_Core_Cal_Update_Defaults(p_cta_cals);
      fbk_output.p_pa_data                          = &data;
      Ctb_Current_State                             = Cta_Get_State_Output_Ptr();
      object_data                                   = data.object_data;
      p_vehicle_data                                = &(data.vehicle_data);
      ctb_state_machine_flag.f_ctb_flag_ctb_enabled = FBK_TRUE;
      p_ctb_current_state                           = *Cta_Get_State_Output_Ptr();
      ctb_error                                     = BMW_CTA_NO_ERROR;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* CTA_STATE_MACHINE_TEST*/