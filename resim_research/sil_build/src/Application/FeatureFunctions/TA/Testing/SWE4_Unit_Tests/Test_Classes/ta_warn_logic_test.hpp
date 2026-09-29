#ifndef TA_WARN_LOGIC_TEST_HPP
#define TA_WARN_LOGIC_TEST_HPP

/**
 * @file ta_warn_logic_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "pa_data.h"
#include "ta_core_calibration.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_input_t.h"
#include "ta_output_t.h"
#include "ta_persistent_t.h"
#include "ta_types.h"
}

/**
 * Class used to create a fixture for TA test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Warn_Logic_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ta_Input_T ta_input{};
   Ta_Output_T ta_output{};
   Ta_Core_Output_T ta_core_output_temp{};
   Ta_Core_Input_T ta_core_input{};
   Ta_Core_Output_T ta_core_output{};
   Ta_Persistent_T ta_persistent{};
   Ta_Core_Calibration_T ta_cal;
   Ta_Object_T ta_object{};
   Pa_Context_T ta_context{};
   Pa_Data_T data{};

   void SetUp() override
   {
      Ta_Core_Cal_Update_Defaults(&ta_cal);
      ta_cal.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj = 0u;
      ta_cal.k_rta_f_higher_obj_crit_based_on_lower_ttp             = 1u;

      /* Initialize context data */
      ta_context.p_data = &data;

      ta_core_input.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*TA_WARN_LOGIC_TEST_HPP*/
