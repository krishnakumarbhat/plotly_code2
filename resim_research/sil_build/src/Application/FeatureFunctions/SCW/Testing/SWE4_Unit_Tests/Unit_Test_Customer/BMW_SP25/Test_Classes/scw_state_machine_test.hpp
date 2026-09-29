#ifndef SCW_STATE_MACHINE_TEST_HPP
#define SCW_STATE_MACHINE_TEST_HPP

/**
 * @file scw_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for SCW state machine unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "scw_core_calibration.h"
#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_iface.h"
#include "scw_instance_t.h"
#include "scw_output_t.h"
#include "scw_persistent_t.h"
#include "scw_state_machine.h"
}

/**
 * Class used to create a fixture for SCW bmw sp5 pre run tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Scw_State_Machine_Test : public ::testing::Test
{
 protected:
   Scw_Instance_T scw_instance{};
   Scw_Core_Calibration_T *p_scw_calibration;
   Scw_Core_Input_T *p_scw_core_input;
   Scw_Input_T scw_input{};
   Scw_Output_T scw_output{};
   Pa_Data_T pa_data{};
   Fbk_Vehicle_Data_T *p_vehicle_data;
   SCW_States_T *p_scw_current_state;
   Scw_State_Flags_T scw_state_flags;
   boolean_T f_scw_error;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      p_scw_calibration = &(scw_instance.calibration);
      p_scw_core_input  = &(scw_instance.core_input);

      p_vehicle_data = &(pa_data.vehicle_data);

      /* Initialize core input */
      p_scw_core_input->p_pa_data = &pa_data;

      /* Initialize calibration values */
      Scw_Core_Cal_Update_Defaults(p_scw_calibration);

      p_scw_current_state = Scw_Get_State_Output_Ptr();

      /* Initialize input */
      scw_state_flags.f_scw_enable_check                        = FBK_FALSE;
      scw_state_flags.f_min_vel_upper_limit_check               = FBK_FALSE;
      scw_state_flags.f_max_vel_upper_limit_check               = FBK_FALSE;
      scw_state_flags.f_min_vel_lower_limit_check               = FBK_FALSE;
      scw_state_flags.f_max_vel_lower_limit_check               = FBK_FALSE;
      f_scw_error                                               = FBK_FALSE;
      scw_input.scw_vehicle_input.vehicle_dynamometer_status    = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_NO_DYNAMOMETER;
      scw_input.scw_vehicle_input.vehicle_end_of_line_status    = SCW_BMW_STATUS_END_OF_LINE_MODE_NOT_SET;
      scw_input.scw_vehicle_input.vehicle_condition             = SCW_BMW_PWF_STATE_DRIVING;
      scw_input.scw_vehicle_input.vehicle_driving_direction     = SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD;
      scw_input.scw_coding_parameters.c_scw_max_vel_lower_limit = FBK_ZERO_UINT;
      scw_input.scw_coding_parameters.c_scw_min_vel_upper_limit = FBK_ZERO_UINT;
      scw_input.scw_coding_parameters.c_scw_min_vel_lower_limit = FBK_ZERO_UINT;
      scw_input.scw_coding_parameters.c_scw_max_vel_upper_limit = FBK_ZERO_UINT;
   }
   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* SCW_STATE_MACHINE_TEST_HPP*/
