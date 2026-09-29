#ifndef PT_POST_RUN_TEST
#define PT_POST_RUN_TEST

/**
 * @file pt_version_ford_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Ford_DAT2_1 pt versiona
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_shared_types.h"
#include "pt_core_calibration.h"
#include "pt_input_t.h"
#include "pt_output_t.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Pt_Version_Ford_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */


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


#endif /* pt_POST_RUN_TEST */