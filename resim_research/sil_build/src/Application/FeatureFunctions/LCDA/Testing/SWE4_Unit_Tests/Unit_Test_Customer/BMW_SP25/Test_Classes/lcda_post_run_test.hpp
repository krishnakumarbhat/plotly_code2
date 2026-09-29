#ifndef LCDA_POST_RUN_TEST_HPP
#define LCDA_POST_RUN_TEST_HPP

/**
 * @file lcda_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Post_Run_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_customer_calibration.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "lcda_output_t.h"
#include "lcda_state_machine.h"
#include "lcda_types.h"
#include "pa_data.h"
}

class Lcda_Post_Run_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Lcda_Input_T lcda_input{};
   Lcda_Output_T lcda_output{};
   Pa_Data_T data{};

   Lcda_Core_Input_T &lcda_core_input         = lcda_instance.core_input;
   Lcda_Core_Output_T &lcda_core_output       = lcda_instance.core_output;
   Lcda_Core_Calibration_T &cals              = lcda_instance.calibration;
   Lcda_Customer_Calibration_T &customer_cals = lcda_instance.customer_calibration;

   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;
   Fbk_Output_T fbk_output;
   LCDA_FF_State_T *current_state;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&cals);
      Lcda_Customer_Cal_Update_Defaults(&customer_cals);
      /* Initialize context data */
      lcda_core_input.p_pa_data = &data;

      current_state = Lcda_Get_State_Output_Ptr();

      lcda_core_output.lcda_status                      = LCDA_STATUS_ACTIVE;
      lcda_core_output.bsw_core_output.f_bsw_is_enabled = FBK_TRUE;
      lcda_core_output.cvw_core_output.f_cvw_is_enabled = FBK_TRUE;
      lcda_core_output.slc_core_output.f_slc_is_enabled = FBK_TRUE;
      lcda_core_output.elc_core_output.f_elc_is_enabled = FBK_TRUE;
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* LCDA_POST_RUN_TEST_HPP */
