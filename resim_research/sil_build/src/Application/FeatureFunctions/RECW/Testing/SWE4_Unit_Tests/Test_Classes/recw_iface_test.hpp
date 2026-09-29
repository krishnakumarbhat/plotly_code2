#ifndef RECW_IFACE_TEST_HPP
#define RECW_IFACE_TEST_HPP

/**
 * @file recw_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for RECW iface unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "fbk_output.h"
#include "recw_input_t.h"
#include "recw_instance.h"
#include "recw_output_t.h"
#include "recw_persistent_t.h"
#include "gtest/gtest_pred_impl.h"


/**
 * Class used to create a fixture for RECW test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Iface_Test : public ::testing::Test
{
 public:
   Recw_Persistent_T recw_pers{};
   Recw_Instance_T recw_instance{};
   Recw_Input_T recw_input{};
   Recw_Output_T recw_output{};
   Fbk_Output_T fbk_output{};
   Pa_Data_T pa_data{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      fbk_output.p_pa_data = &pa_data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*RECW_IFACE_TEST_HPP*/
