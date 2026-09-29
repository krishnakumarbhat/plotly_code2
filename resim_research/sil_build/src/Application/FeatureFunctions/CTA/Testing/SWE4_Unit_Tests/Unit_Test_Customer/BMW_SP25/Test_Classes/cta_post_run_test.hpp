#ifndef CTA_POST_RUN_TEST
#define CTA_POST_RUN_TEST

/**
 * @file cta_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for bmw srr5 cta post run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_output_t.h"
#include "cta_customer_calibration.h"
#include "cta_input_t.h"
#include "cta_output_t.h"
#include "cta_post_run.c"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Post_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Core_Calibration_T *p_cta_cals   = nullptr;
   Fbk_Object_Data_T *object_data       = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data   = nullptr;
   Cta_Core_Output_T *p_cta_core_output = nullptr;
   Cta_Output_T cta_output{};
   Cta_Input_T cta_input{};
   Cta_Instance_T cta_instance{};
   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};

   Bmw_Ctb_Output_Bus_Signals_T output_signals;
   Bmw_Ctb_Output_Algo_State_T algo_warn_states;
   Ctb_State_Output_T *Ctb_Current_State;
   void SetUp() override
   {
      p_cta_cals = &cta_instance.calibration;
      Cta_Core_Cal_Update_Defaults(p_cta_cals);
      Cta_Customer_Cal_Update_Defaults(&cta_instance.customer_calibration);

      Ctb_Current_State                 = Cta_Get_State_Output_Ptr();
      data.time_diff_to_last_cycle      = 0.05f;
      fbk_output.p_pa_data              = &data;
      object_data                       = data.object_data;
      p_vehicle_data                    = &(data.vehicle_data);
      cta_instance.core_input.p_pa_data = &data;

      output_signals.qualifier_function_ctb                                            = CTB_STATE_UNFILLED;
      algo_warn_states.ctb_acute_warning[BMW_CTB_ALERT_POSITION_FRONT_RIGHT]           = BMW_CTB_NO_WARNING;
      algo_warn_states.ctb_acute_warning[BMW_CTB_ALERT_POSITION_FRONT_LEFT]            = BMW_CTB_NO_WARNING;
      algo_warn_states.ctb_acute_warning[BMW_CTB_ALERT_POSITION_REAR_RIGHT]            = BMW_CTB_NO_WARNING;
      algo_warn_states.ctb_acute_warning[BMW_CTB_ALERT_POSITION_REAR_LEFT]             = BMW_CTB_NO_WARNING;
      algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_NO_WARNING;
      algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_LEFT]  = BMW_CTB_NO_WARNING;
      algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_RIGHT]  = BMW_CTB_NO_WARNING;
      algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_LEFT]   = BMW_CTB_NO_WARNING;
      algo_warn_states.warning_acoustics[BMW_CTB_ALERT_POSITION_FRONT_RIGHT]           = BMW_CTB_NO_WARNING;
      algo_warn_states.warning_acoustics[BMW_CTB_ALERT_POSITION_FRONT_LEFT]            = BMW_CTB_NO_WARNING;
      algo_warn_states.warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_RIGHT]            = BMW_CTB_NO_WARNING;
      algo_warn_states.warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_LEFT]             = BMW_CTB_NO_WARNING;
      algo_warn_states.ctb_warning[BMW_CTB_ALERT_POSITION_FRONT_RIGHT]                 = BMW_CTB_NO_WARNING;
      algo_warn_states.ctb_warning[BMW_CTB_ALERT_POSITION_FRONT_LEFT]                  = BMW_CTB_NO_WARNING;
      algo_warn_states.ctb_warning[BMW_CTB_ALERT_POSITION_REAR_RIGHT]                  = BMW_CTB_NO_WARNING;
      algo_warn_states.ctb_warning[BMW_CTB_ALERT_POSITION_REAR_LEFT]                   = BMW_CTB_NO_WARNING;
      algo_warn_states.request_extmirror_warning[BMW_CTB_ALERT_SIDE_LEFT]  = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
      algo_warn_states.request_extmirror_warning[BMW_CTB_ALERT_SIDE_RIGHT] = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
      algo_warn_states.status_brake_requirement                            = BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING;
   }
   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /* CTA_POST_RUN_TEST */