#ifndef CTA_BMW_SP25_WARN_STATE_TEST
#define CTA_BMW_SP25_WARN_STATE_TEST

/**
 * @file cta_bmw_sp25_warn_state_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for BMW SP25 cta state machine
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_bmw_sp25_warn_state.h"
#include "cta_core_calibration.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_instance.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Bmw_Sp25_Warn_State_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Core_Calibration_T *p_cta_cals = nullptr;
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;
   Cta_Input_T cta_input{};
   Cta_Instance_T cta_instance{};
   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Cta_Crit_Level_T cta_alert_level;
   Ctb_State_Output_T p_ctb_current_state;
   Ctb_Function_Error_T ctb_error;
   void SetUp() override
   {
      p_cta_cals = &cta_instance.calibration;
      Cta_Core_Cal_Update_Defaults(p_cta_cals);
      fbk_output.p_pa_data = &data;
      object_data          = data.object_data;
      p_vehicle_data       = &(data.vehicle_data);
      ctb_error            = BMW_CTA_NO_ERROR;
      cta_alert_level      = CTA_CRIT_LEVEL_NONE;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* CTA_BMW_SP25_WARN_STATE_TEST*/