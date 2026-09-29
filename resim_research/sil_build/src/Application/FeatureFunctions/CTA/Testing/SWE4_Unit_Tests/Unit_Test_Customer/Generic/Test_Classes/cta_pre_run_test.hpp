#ifndef CTA_PRE_RUN_TEST
#define CTA_PRE_RUN_TEST

/**
 * @file cta_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Generic pre run
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"


extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Pre_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Core_Calibration_T *p_cta_cal    = nullptr;
   Cta_Core_Output_T *p_cta_core_output = nullptr;
   Cta_Core_Input_T *p_cta_core_input   = nullptr;
   Cta_Instance_T cta_instance{};
   Cta_Input_T cta_input{};
   Fbk_Output_T fbk_output{};
   Pt_Output_T pt_output{};

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;

   void SetUp() override
   {
      p_cta_core_input     = &cta_instance.core_input;
      p_cta_cal            = &cta_instance.calibration;
      fbk_output.p_pa_data = &data;
      object_data          = data.object_data;
      p_vehicle_data       = &(data.vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* CTA_PRE_RUN_TEST */