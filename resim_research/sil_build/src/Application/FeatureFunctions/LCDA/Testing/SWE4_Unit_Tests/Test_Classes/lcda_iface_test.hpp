#ifndef LCDA_IFACE_TEST_HPP
#define LCDA_IFACE_TEST_HPP

/**
 * @file lcda_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Iface_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include "fbk_output.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "lcda_output_t.h"
#include "gtest/gtest_pred_impl.h"

/**
 * Class used to create a fixture for lcda_tests.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Iface_Test : public ::testing::Test
{
 protected:
   static Lcda_Instance_T Lcda_Instance;
   static Lcda_Input_T Lcda_Input;
   Lcda_Output_T lcda_output{};
   Pa_Data_T pa_data{};

   Fbk_Output_T fbk_output;
   Fbk_Index_Id_Lookup_Table_T lookup_table{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      fbk_output.p_pa_data               = &pa_data;
      fbk_output.p_index_id_lookup_table = &lookup_table;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
};

#endif /* LCDA_IFACE_TEST_HPP */
