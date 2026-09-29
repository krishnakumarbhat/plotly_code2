#ifndef LCDA_PRE_RUN_TEST_HPP
#define LCDA_PRE_RUN_TEST_HPP

/**
 * @file lcda_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Pre_Run_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "camera_data_t.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_customer_calibration.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "lcda_output_t.h"
#include "pa_data.h"
}

class Lcda_Pre_Run_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Lcda_Input_T lcda_input{};
   Lcda_Output_T lcda_output{};
   Pa_Data_T data{};

   Lcda_Core_Input_T &lcda_core_input     = lcda_instance.core_input;
   Lcda_Core_Calibration_T &cals          = lcda_instance.calibration;
   Lcda_Customer_Calibration_T &cust_cals = lcda_instance.customer_calibration;

   Fbk_Object_Data_T *object_data       = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data   = &data.vehicle_data;
   Fbk_Guardrail_Data_T *guardrail_data = data.guardrail_data;

   Camera_Data_T cam_data{};
   Fbk_Output_T fbk_output;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&cals);
      Lcda_Customer_Cal_Update_Defaults(&cust_cals);
      /* Initialize context data */

      lcda_core_input.p_pa_data = &data;
      lcda_input.camera_data    = &cam_data;

      lcda_input.f_lcda_enable_environment_plausibilization = 1u;

      /* Set basic vehicle data */
      p_vehicle_data->host_length = 5.0f;
      p_vehicle_data->host_width  = 2.0f;
      fbk_output.p_pa_data        = &data;
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* LCDA_PRE_RUN_TEST_HPP */
