#ifndef CTA_POST_RUN_TEST_HPP
#define CTA_POST_RUN_TEST_HPP

/**
 * @file cta_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Generic Cta post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_output_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
}

class Cta_Post_Run_Test : public ::testing::Test
{
 protected:
   Cta_Instance_T cta_instance{};
   Fbk_Output_T fbk_output{};
   Cta_Input_T cta_input{};
   Cta_Output_T cta_output{};

   Cta_Core_Input_T &cta_core_input   = cta_instance.core_input;
   Cta_Core_Output_T &cta_core_output = cta_instance.core_output;
   Cta_Core_Calibration_T &cals       = cta_instance.calibration;
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Cta_Core_Cal_Update_Defaults(&cals);

      /* Initialize context data */
      cta_core_input.p_pa_data = &data;
      fbk_output.p_pa_data     = &data;

      cta_core_output.cta_status    = CTA_STATUS_ACTIVE;
      cta_core_output.f_cta_enabled = FBK_TRUE;
   }
   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* CTA_POST_RUN_TEST_HPP */
