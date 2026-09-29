#ifndef CTA_BRAKE_LOGIC_TEST
#define CTA_BRAKE_LOGIC_TEST

/**
 * @file cta_brake_logic_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_brake_logic module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_instance.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
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
class Cta_Braking_Logic_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Mode_T mode;
   Cta_Instance_T cta_instance;
   Cta_Object_Attributes_T attributes;
   Cta_Core_Calibration_T &cals = cta_instance.calibration;
   Cta_Persistent_T cta_persistent{};
   Cta_Core_Input_T core_input{};
   Cta_Comparison_Data_T cta_comp{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Output_T fbk_output;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Pa_Context_T context{};

   void SetUp() override
   {
      mode = CTA_MODE_REAR;

      Cta_Core_Cal_Update_Defaults(&cals);
      core_input.p_pa_data                                               = &data;
      cta_comp.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes = &attributes;

      fbk_output.p_pa_data = &data;
      object_data          = data.object_data;
      p_vehicle_data       = &(data.vehicle_data);

      cta_instance.core_input.cta_stop_mode = CTA_STOP_MODE_TTC;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif /* CTA_BRAKE_LOGIC_TEST */