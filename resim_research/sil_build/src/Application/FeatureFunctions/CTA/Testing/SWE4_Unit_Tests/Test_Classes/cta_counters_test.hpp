#ifndef CTA_COUNTERS_TEST
#define CTA_COUNTERS_TEST

/**
 * @file cta_counters_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_counters module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_object_data_t.h"
}


/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Counters_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Core_Calibration_T cals;
   Cta_Object_Data_T object{};
   Fbk_Object_Data_T tracker_output{};
   Cta_Object_Attributes_T attributes{};
   Cta_Object_Persistent_T persistent{};
   Cta_Persistent_T cta_persistent{};
   Cta_Mode_T mode;

   void SetUp() override
   {
      Cta_Core_Cal_Update_Defaults(&cals);

      object.attributes   = &attributes;
      object.tracker_data = tracker_output;
      object.persistent   = &persistent;

      cals.k_cta_cycle_count_hold_true_warning      = 10u;
      cals.k_cta_f_prevent_fall_back_to_critlevel_1 = 1;
      mode                                          = CTA_MODE_REAR;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }


 protected:
};

#endif // CTA_COUNTERS_TEST