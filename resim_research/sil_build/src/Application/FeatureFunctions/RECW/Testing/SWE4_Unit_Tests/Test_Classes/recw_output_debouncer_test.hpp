#ifndef RECW_OUTPUT_DEBOUNCER_TEST_HPP
#define RECW_OUTPUT_DEBOUNCER_TEST_HPP

/**
 * @file recw_output_debouncer_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is RECW output debouncer test header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "recw.h"
#include "recw_core_calibration.h"
#include "recw_core_output_t.h"
#include "recw_persistent_t.h"
}

class Recw_Output_Debouncer_Test : public ::testing::Test
{
 protected:
   Recw_Core_Calibration_T recw_cals;
   Recw_Core_Output_T recw_core_output{};
   Recw_Persistent_T recw_pers{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Init cals */
      Recw_Core_Cal_Update_Defaults(&recw_cals);
      recw_cals.k_recw_f_only_allow_consecutive_alert_levels = 0u;

      /* Init recw_core_output */
      Recw_Reset_Core_Output(&recw_core_output);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
};

#endif // RECW_OUTPUT_DEBOUNCER_TEST_HPP
