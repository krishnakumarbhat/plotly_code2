#ifndef CTA_STRUCT_INITIALIZER_TEST
#define CTA_STRUCT_INITIALIZER_TEST

/**
 * @file cta_struct_initializer_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_struct_initializer module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_persistent_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Struct_Initializer_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Core_Output_T cta_core_output;
   Cta_Core_Input_T cta_core_input;
   Cta_Persistent_T cta_persistent;
   Pa_Context_T context;

   void SetUp() override
   {
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }


 protected:
};

#endif /* CTA_STRUCT_INITIALIZER */