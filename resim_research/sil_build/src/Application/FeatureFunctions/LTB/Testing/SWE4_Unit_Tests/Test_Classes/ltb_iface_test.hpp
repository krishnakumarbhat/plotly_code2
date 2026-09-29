#ifndef LTB_IFACE_TEST_H
#define LTB_IFACE_TEST_H

/**
 * @file ltb_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for ltb_iface unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"

extern "C"
{
#include "ltb_core_calibration.h"
#include "ltb_iface.h"
}


/**
 * Class used to create a fixture for Ltb_Iface_Test.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Iface_Test : public ::testing::Test
{
 protected:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ltb_Core_Calibration_T cals;

   virtual void SetUp()
   {
      ltb_instance.core_input.p_pa_data = &data;
      fbk_output.p_pa_data              = &data;
      Ltb_Core_Cal_Update_Defaults(&cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
   Pa_Data_T data{};
   Fbk_Output_T fbk_output;
   Ltb_Input_T ltb_input{};
   Ltb_Output_T ltb_output{};
   Ltb_Instance_T ltb_instance{};
};

#endif /* LTB_IFACE_TEST_H */