#ifndef CTA_IFACE_TEST
#define CTA_IFACE_TEST

/**
 * @file cta_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_iface module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_output_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_output_t.h"
#include "cta_persistent_t.h"
#include "fbk_output.h"
#include "pa_context.h"
#include "pt_output_t.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Iface_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Cta_Instance_T cta_instance = {};
   Cta_Output_T cta_output;
   Cta_Core_Output_T &cta_core_output = cta_instance.core_output;
   Cta_Persistent_T &cta_persistent   = cta_instance.persistent;
   Cta_Input_T cta_input;
   Fbk_Output_T fbk_output;
   Pa_Data_T pa_data     = {};
   Pt_Output_T pt_output = {};

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


 protected:
};

#endif // CTA_IFACE_TEST